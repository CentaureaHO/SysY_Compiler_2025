#include "llvm/loop/licm.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <cassert>

// Define this to allow hoisting of comparison instructions (ICMP/FCMP)
#define ALLOW_CMP_HOIST

// #define DBGMODE

#ifdef DBGMODE
template <typename... Args>
void dbg_impl(Args&&... args)
{
    ((std::cout << args), ...);
    std::cout << std::endl;
}
#define DBGINFO(...) dbg_impl(__VA_ARGS__)
#else
#define DBGINFO(...) \
    do {             \
    } while (0)
#endif

namespace StructuralTransform
{
    static std::map<std::string, CFG*> cfg_map;

    LICMPass::LICMPass(LLVMIR::IR* ir, Analysis::AliasAnalyser* aa)
        : Pass(ir), alias_analyser(aa), optimization_changed(false)
    {}

    void LICMPass::Execute()
    {
        cfg_map.clear();

        for (const auto& [func_def, cfg] : ir->cfg) cfg_map[func_def->func_name] = cfg;
        for (const auto& [func_def, cfg] : ir->cfg) performLICM(cfg);
    }

    bool LICMPass::dominatesAllExits(CFG* cfg, LLVMIR::IRBlock* bb, NaturalLoop* loop)
    {
        if (!ir->DomTrees.count(cfg)) return false;

        const auto& dom_tree = ir->DomTrees.at(cfg);

        std::function<bool(int, int)> checkDomination = [&](int dominator, int dominated) -> bool {
            if (dominator == dominated) return true;
            if (static_cast<size_t>(dominator) >= dom_tree->dom_tree.size()) return false;

            return std::any_of(dom_tree->dom_tree[dominator].begin(),
                dom_tree->dom_tree[dominator].end(),
                [&](int child) { return checkDomination(child, dominated); });
        };

        return std::all_of(loop->exit_nodes.begin(), loop->exit_nodes.end(), [&](const auto* exit_bb) {
            return checkDomination(bb->block_id, exit_bb->block_id);
        });
    }

    bool LICMPass::isLoopInvariant(
        CFG* cfg, LLVMIR::Instruction* inst, NaturalLoop* loop, const std::vector<LLVMIR::Instruction*>& write_insts)
    {
        if (inst->opcode == LLVMIR::IROpCode::STORE || inst->opcode == LLVMIR::IROpCode::PHI ||
            inst->opcode == LLVMIR::IROpCode::BR_COND || inst->opcode == LLVMIR::IROpCode::BR_UNCOND
#ifdef ALLOW_CMP_HOIST
            || inst->opcode == LLVMIR::IROpCode::ICMP || inst->opcode == LLVMIR::IROpCode::FCMP
#endif
        )
            return false;

        const auto used_regs = inst->GetUsedRegs();
        for (const auto op_reg : used_regs)
        {
            const auto inv_it = invariant_map.find(op_reg);
            if (inv_it != invariant_map.end() && inv_it->second) continue;

            const auto result_it = result_map.find(op_reg);
            if (result_it == result_map.end()) continue;

            const auto* result_inst = result_it->second;
            const int   inst_bb_id  = result_inst->block_id;

            // Special handling for FuncDefInst which has block_id = -1; made by lhz
            if (inst_bb_id == -1)
            {
                invariant_map[op_reg] = true;
                continue;
            }

            const auto* inst_bb = cfg->block_id_to_block.at(inst_bb_id);

            if (loop->loop_nodes.find(const_cast<LLVMIR::IRBlock*>(inst_bb)) != loop->loop_nodes.end())
                return false;
            else
                invariant_map[op_reg] = true;
        }

        // Special handling for FuncDefInst which has block_id = -1
        if (inst->block_id == -1) return false;

        const bool dominates_exits = dominatesAllExits(cfg, cfg->block_id_to_block.at(inst->block_id), loop);
        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::LOAD:
            {
                const auto* load_inst = static_cast<const LLVMIR::LoadInst*>(inst);

                bool is_safe_load = (load_inst->ptr->type == LLVMIR::OperandType::GLOBAL);
                if (!is_safe_load && load_inst->ptr->type == LLVMIR::OperandType::REG)
                {
                    const auto* reg_ptr = static_cast<const LLVMIR::RegOperand*>(load_inst->ptr);
                    if (const auto result_it = result_map.find(reg_ptr->reg_num); result_it != result_map.end())
                    {
                        const auto* result_inst = result_it->second;
                        const auto* result_bb   = cfg->block_id_to_block.at(result_inst->block_id);
                        is_safe_load =
                            (loop->loop_nodes.find(const_cast<LLVMIR::IRBlock*>(result_bb)) == loop->loop_nodes.end());
                    }
                }

                if (!is_safe_load && !dominates_exits) return false;
                const auto* ptr = load_inst->ptr;
                return std::none_of(write_insts.begin(), write_insts.end(), [this, ptr, cfg](const auto* write_inst) {
                    const auto result = alias_analyser->queryInstModRef(
                        const_cast<LLVMIR::Instruction*>(write_inst), const_cast<LLVMIR::Operand*>(ptr), cfg);
                    return result == Analysis::AliasAnalyser::Mod || result == Analysis::AliasAnalyser::ModRef;
                });
            }

            case LLVMIR::IROpCode::CALL:
            {
                const auto* call_inst = static_cast<const LLVMIR::CallInst*>(inst);

                const auto cfg_it = cfg_map.find(call_inst->func_name);
                if (cfg_it == cfg_map.end()) return false;

                auto* target_cfg = cfg_it->second;

                bool no_side_effect = alias_analyser->isNoSideEffect(target_cfg);
                bool is_pure        = no_side_effect && alias_analyser->getReadPtrs(target_cfg).empty();

                if (!no_side_effect) return false;
                if (is_pure) return true;
                if (!dominates_exits)
                    ;  // might still be safe to hoist if no memory conflicts exist

                const auto                    read_ptrs = alias_analyser->getReadPtrs(target_cfg);
                std::vector<LLVMIR::Operand*> real_read_ptrs;
                real_read_ptrs.reserve(read_ptrs.size());

                for (const auto* ptr : read_ptrs)
                {
                    if (ptr->type == LLVMIR::OperandType::GLOBAL)
                        real_read_ptrs.push_back(const_cast<LLVMIR::Operand*>(ptr));
                    else if (ptr->type == LLVMIR::OperandType::REG)
                    {
                        const auto* reg_ptr   = static_cast<const LLVMIR::RegOperand*>(ptr);
                        const int   ptr_regno = reg_ptr->reg_num;

                        if (ptr_regno >= static_cast<int>(call_inst->args.size())) return false;

                        auto* real_ptr = call_inst->args[ptr_regno].second;
                        real_read_ptrs.push_back(real_ptr);
                    }
                    else
                        return false;
                }

                for (const auto* write_inst : write_insts)
                {
                    for (const auto* r_ptr : real_read_ptrs)
                    {
                        const auto result = alias_analyser->queryInstModRef(
                            const_cast<LLVMIR::Instruction*>(write_inst), const_cast<LLVMIR::Operand*>(r_ptr), cfg);
                        if (result == Analysis::AliasAnalyser::Mod || result == Analysis::AliasAnalyser::ModRef)
                            return false;
                    }
                }

                return true;
            }

            case LLVMIR::IROpCode::DIV:
            case LLVMIR::IROpCode::MOD:
            {
                if (dominates_exits) break;

                const auto* arith_inst = static_cast<const LLVMIR::ArithmeticInst*>(inst);
                if (arith_inst->rhs->type != LLVMIR::OperandType::IMMEI32) return false;

                const auto* imm_op = static_cast<const LLVMIR::ImmeI32Operand*>(arith_inst->rhs);
                if (imm_op->value == 0) return false;

                break;
            }

            default: break;
        }

        const int result_reg = inst->GetResultReg();
        if (result_reg != -1)
        {
            for (const auto& [block_id, bb] : cfg->block_id_to_block)
            {
                for (const auto* use_inst : bb->insts)
                {
                    if (use_inst == inst) continue;

                    const auto used_regs = const_cast<LLVMIR::Instruction*>(use_inst)->GetUsedRegs();
                    bool uses_our_result = std::find(used_regs.begin(), used_regs.end(), result_reg) != used_regs.end();

                    if (uses_our_result)
                    {
                        if (loop->loop_nodes.find(const_cast<LLVMIR::IRBlock*>(bb)) != loop->loop_nodes.end()) continue;

                        DBGINFO("LICM: Instruction with result reg ",
                            result_reg,
                            " has use in block ",
                            bb->block_id,
                            " which is outside current loop ",
                            loop->loop_id);
                    }
                }
            }
            invariant_map[result_reg] = true;
        }

        return true;
    }

    std::vector<LLVMIR::Instruction*> LICMPass::getLoopMemWriteInsts(CFG* cfg, NaturalLoop* loop)
    {
        std::vector<LLVMIR::Instruction*> write_instructions;

        for (const auto* bb : loop->loop_nodes)
        {
            for (const auto* inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    write_instructions.push_back(const_cast<LLVMIR::Instruction*>(inst));
                    continue;
                }

                if (inst->opcode == LLVMIR::IROpCode::CALL)
                {
                    const auto* call_inst = static_cast<const LLVMIR::CallInst*>(inst);

                    if (const auto it = cfg_map.find(call_inst->func_name); it == cfg_map.end())
                        write_instructions.push_back(const_cast<LLVMIR::Instruction*>(inst));
                    else if (alias_analyser->isWriteMem(it->second))
                        write_instructions.push_back(const_cast<LLVMIR::Instruction*>(inst));
                }
            }
        }

        return write_instructions;
    }

    std::vector<LLVMIR::Instruction*> LICMPass::findInvariantInsts(CFG* cfg, NaturalLoop* loop)
    {
        invariant_map.clear();
        const auto loop_mem_write_insts = getLoopMemWriteInsts(cfg, loop);

        std::vector<LLVMIR::Instruction*> invariant_insts_list;

        DBGINFO("LICM: Finding invariant instructions in loop ", loop->loop_id);
        for (const auto* loop_bb : loop->loop_nodes)
        {
            for (auto* inst : loop_bb->insts)
            {
                if (isLoopInvariant(cfg, inst, loop, loop_mem_write_insts))
                {
                    DBGINFO("LICM: Found invariant instruction with result reg ",
                        inst->GetResultReg(),
                        " in block ",
                        inst->block_id);
                    invariant_insts_list.push_back(inst);
                }
            }
        }
        DBGINFO("LICM: Found ", invariant_insts_list.size(), " invariant instructions in loop ", loop->loop_id);
        return invariant_insts_list;
    }

    void LICMPass::hoistInstructionsToPreheader(
        NaturalLoop* loop, const std::vector<LLVMIR::Instruction*>& insts_to_hoist)
    {
        if (insts_to_hoist.empty()) return;

        optimization_changed = true;

        DBGINFO("LICM: Hoisting ", insts_to_hoist.size(), " instructions to preheader of loop ", loop->loop_id);
        DBGINFO("LICM: Preheader block is ",
            loop->preheader->block_id,
            " with ",
            loop->preheader->insts.size(),
            " instructions");
        for (auto* inst : insts_to_hoist)
        {
            DBGINFO(
                "LICM: Hoisting instruction with result reg ", inst->GetResultReg(), " from block ", inst->block_id);
        }

        auto* end_inst = loop->preheader->insts.back();
        assert(end_inst->opcode == LLVMIR::IROpCode::BR_UNCOND);
        loop->preheader->insts.pop_back();

        const std::set<LLVMIR::Instruction*> erase_set(insts_to_hoist.begin(), insts_to_hoist.end());

        DBGINFO("LICM: Adding instructions to preheader block ", loop->preheader->block_id);
        for (auto* inst : insts_to_hoist)
        {
            inst->block_id = loop->preheader->block_id;
            loop->preheader->insts.push_back(inst);
            DBGINFO("LICM: Added instruction with result reg ",
                inst->GetResultReg(),
                " to preheader, now has ",
                loop->preheader->insts.size(),
                " instructions");
        }

        for (auto* bb : loop->loop_nodes)
        {
            auto& insts    = bb->insts;
            auto  old_size = insts.size();
            insts.erase(std::remove_if(insts.begin(),
                            insts.end(),
                            [&erase_set](const auto* inst) {
                                return erase_set.find(const_cast<LLVMIR::Instruction*>(inst)) != erase_set.end();
                            }),
                insts.end());
            auto new_size = insts.size();
            if (old_size != new_size)
            {
                DBGINFO("LICM: Removed ", (old_size - new_size), " instructions from block ", bb->block_id);
            }
        }

        loop->preheader->insts.push_back(end_inst);
    }

    void LICMPass::hoistLoopInvariants(CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop)
    {
        bool has_changes = true;

        while (has_changes)
        {
            buildResultMap(cfg);

            const auto invariant_insts_list = findInvariantInsts(cfg, loop);

            if (invariant_insts_list.empty())
            {
                has_changes = false;
                break;
            }

            hoistInstructionsToPreheader(loop, invariant_insts_list);
        }
    }

    void LICMPass::promoteMemToRegs(CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop)
    {
        DBGINFO("LICM promoteMemToRegs: Processing loop ", loop->loop_id);
        if (loop->exit_nodes.size() != 1) return;

        auto* exit = *loop->exit_nodes.begin();

        std::map<int, std::set<LLVMIR::Instruction*>>         invariant_ptr_insts_map;  ///< RegNo -> instructions
        std::map<int, LLVMIR::DataType>                       ptr_type_map;             ///< RegNo -> type
        std::map<std::string, std::set<LLVMIR::Instruction*>> global_ptr_inst_map;      ///< GlobalName -> instructions
        std::map<std::string, LLVMIR::DataType>               global_type_map;          ///< GlobalName -> type

        std::set<LLVMIR::Instruction*> mem_rw_insts;

        for (const auto* bb : loop->loop_nodes)
        {
            for (auto* inst : bb->insts)
            {
                LLVMIR::Operand* ptr = nullptr;
                LLVMIR::DataType type;

                switch (inst->opcode)
                {
                    case LLVMIR::IROpCode::STORE:
                    {
                        const auto* store_inst = static_cast<const LLVMIR::StoreInst*>(inst);
                        ptr                    = store_inst->ptr;
                        type                   = store_inst->type;
                        mem_rw_insts.insert(inst);
                        break;
                    }
                    case LLVMIR::IROpCode::LOAD:
                    {
                        const auto* load_inst = static_cast<const LLVMIR::LoadInst*>(inst);
                        ptr                   = load_inst->ptr;
                        type                  = load_inst->type;
                        mem_rw_insts.insert(inst);
                        break;
                    }
                    case LLVMIR::IROpCode::CALL:
                    {
                        mem_rw_insts.insert(inst);
                        continue;
                    }
                    default: continue;
                }

                if (ptr->type == LLVMIR::OperandType::GLOBAL)
                {
                    const auto* global_op = static_cast<const LLVMIR::GlobalOperand*>(ptr);
                    global_ptr_inst_map[global_op->global_name].insert(inst);
                    global_type_map[global_op->global_name] = type;
                }
                else if (ptr->type == LLVMIR::OperandType::REG)
                {
                    const auto* reg_ptr   = static_cast<const LLVMIR::RegOperand*>(ptr);
                    const auto  result_it = result_map.find(reg_ptr->reg_num);

                    if (result_it != result_map.end())
                    {
                        const auto* result_inst = result_it->second;
                        const auto* result_bb   = cfg->block_id_to_block.at(result_inst->block_id);

                        if (loop->loop_nodes.find(const_cast<LLVMIR::IRBlock*>(result_bb)) == loop->loop_nodes.end())
                        {
                            invariant_ptr_insts_map[reg_ptr->reg_num].insert(inst);
                            ptr_type_map[reg_ptr->reg_num] = type;
                        }
                    }
                }
            }
        }

        bool is_motion_store = false;

        for (const auto& ptr_inst_pair : invariant_ptr_insts_map)
        {
            const int   ptr_regno = ptr_inst_pair.first;
            const auto& insts     = ptr_inst_pair.second;
            auto*       ptr       = getRegOperand(ptr_regno);

            bool can_licm = true;
            for (const auto* rw_inst : mem_rw_insts)
            {
                if (insts.find(const_cast<LLVMIR::Instruction*>(rw_inst)) != insts.end()) continue;

                const auto result =
                    alias_analyser->queryInstModRef(const_cast<LLVMIR::Instruction*>(rw_inst), ptr, cfg);
                if (result != Analysis::AliasAnalyser::NoModRef)
                {
                    can_licm = false;
                    break;
                }
            }

            bool dom_tag = false;
            for (const auto* inst : insts)
            {
                if (dominatesAllExits(cfg, cfg->block_id_to_block.at(inst->block_id), loop))
                {
                    dom_tag = true;
                    break;
                }
            }

            if (!can_licm || !dom_tag) continue;

            is_motion_store = true;

            const int temp_reg    = ++cfg->func->max_reg;
            auto*     alloca_inst = new LLVMIR::AllocInst(ptr_type_map[ptr_regno], getRegOperand(temp_reg));
            cfg->block_id_to_block[0]->insts.push_front(alloca_inst);

            for (auto* inst : insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    auto* load_inst = static_cast<LLVMIR::LoadInst*>(inst);
                    load_inst->ptr  = getRegOperand(temp_reg);
                    mem_rw_insts.erase(inst);
                }
                else if (inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto* store_inst = static_cast<LLVMIR::StoreInst*>(inst);
                    store_inst->ptr  = getRegOperand(temp_reg);
                    mem_rw_insts.erase(inst);
                }
            }

            // Insert load in preheader
            auto* end_inst = loop->preheader->insts.back();
            assert(end_inst->opcode == LLVMIR::IROpCode::BR_UNCOND);
            loop->preheader->insts.pop_back();

            // Load original value into temp variable
            auto* load_orig = new LLVMIR::LoadInst(
                ptr_type_map[ptr_regno], getRegOperand(ptr_regno), getRegOperand(++cfg->func->max_reg));
            auto* store_temp = new LLVMIR::StoreInst(
                ptr_type_map[ptr_regno], getRegOperand(temp_reg), getRegOperand(cfg->func->max_reg));

            loop->preheader->insts.push_back(load_orig);
            loop->preheader->insts.push_back(store_temp);
            loop->preheader->insts.push_back(end_inst);

            // Insert store in exit
            auto it = exit->insts.begin();
            while (it != exit->insts.end() && (*it)->opcode == LLVMIR::IROpCode::PHI) { ++it; }

            if (it != exit->insts.end())
            {
                auto* load_temp = new LLVMIR::LoadInst(
                    ptr_type_map[ptr_regno], getRegOperand(temp_reg), getRegOperand(++cfg->func->max_reg));
                auto* store_orig = new LLVMIR::StoreInst(
                    ptr_type_map[ptr_regno], getRegOperand(ptr_regno), getRegOperand(cfg->func->max_reg));

                exit->insts.insert(it, load_temp);
                exit->insts.insert(it, store_orig);
            }
        }

        // Process global pointers
        for (const auto& global_inst_pair : global_ptr_inst_map)
        {
            const std::string& global_name = global_inst_pair.first;
            const auto&        insts       = global_inst_pair.second;
            auto*              ptr         = getGlobalOperand(global_name);

            bool can_licm = true;
            for (const auto* rw_inst : mem_rw_insts)
            {
                if (insts.find(const_cast<LLVMIR::Instruction*>(rw_inst)) != insts.end()) continue;

                const auto result =
                    alias_analyser->queryInstModRef(const_cast<LLVMIR::Instruction*>(rw_inst), ptr, cfg);
                if (result != Analysis::AliasAnalyser::NoModRef)
                {
                    can_licm = false;
                    break;
                }
            }

            if (!can_licm) continue;

            std::set<LLVMIR::Instruction*> all_global_insts;
            for (const auto& [block_id, bb] : cfg->block_id_to_block)
            {
                for (auto* inst : bb->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::LOAD || inst->opcode == LLVMIR::IROpCode::STORE)
                    {
                        LLVMIR::Operand* access_ptr = nullptr;
                        if (inst->opcode == LLVMIR::IROpCode::LOAD)
                        {
                            access_ptr = static_cast<const LLVMIR::LoadInst*>(inst)->ptr;
                        }
                        else if (inst->opcode == LLVMIR::IROpCode::STORE)
                        {
                            access_ptr = static_cast<const LLVMIR::StoreInst*>(inst)->ptr;
                        }

                        if (access_ptr && access_ptr->type == LLVMIR::OperandType::GLOBAL)
                        {
                            const auto* global_op = static_cast<const LLVMIR::GlobalOperand*>(access_ptr);
                            if (global_op->global_name == global_name) { all_global_insts.insert(inst); }
                        }
                    }
                }
            }

            bool all_accesses_in_loop = true;
            for (const auto* global_inst : all_global_insts)
            {
                bool found_in_loop = false;
                for (const auto* loop_bb : loop->loop_nodes)
                {
                    for (const auto* loop_inst : loop_bb->insts)
                    {
                        if (loop_inst == global_inst)
                        {
                            found_in_loop = true;
                            break;
                        }
                    }
                    if (found_in_loop) break;
                }
                if (!found_in_loop)
                {
                    all_accesses_in_loop = false;
                    DBGINFO("LICM: Global variable ",
                        global_name,
                        " has accesses outside loop ",
                        loop->loop_id,
                        " (in block ",
                        global_inst->block_id,
                        "), skipping promotion");
                    break;
                }
            }

            if (!all_accesses_in_loop) continue;

            DBGINFO("LICM: Promoting global variable ", global_name, " to register in loop ", loop->loop_id);

            is_motion_store = true;

            // Create temporary alloca variable
            const int temp_reg    = ++cfg->func->max_reg;
            auto*     alloca_inst = new LLVMIR::AllocInst(global_type_map[global_name], getRegOperand(temp_reg));
            cfg->block_id_to_block[0]->insts.push_front(alloca_inst);

            DBGINFO("LICM: Found ", all_global_insts.size(), " total accesses to global ", global_name, " in loop");

            for (auto* inst : insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    auto* load_inst = static_cast<LLVMIR::LoadInst*>(inst);
                    DBGINFO("LICM: Redirecting LOAD from global ",
                        global_name,
                        " to temp reg ",
                        temp_reg,
                        " in block ",
                        inst->block_id);
                    load_inst->ptr = getRegOperand(temp_reg);
                    mem_rw_insts.erase(inst);
                }
                else if (inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto* store_inst = static_cast<LLVMIR::StoreInst*>(inst);
                    DBGINFO("LICM: Redirecting STORE from global ",
                        global_name,
                        " to temp reg ",
                        temp_reg,
                        " in block ",
                        inst->block_id);
                    store_inst->ptr = getRegOperand(temp_reg);
                    mem_rw_insts.erase(inst);
                }
            }

            // Insert in preheader
            auto* end_inst = loop->preheader->insts.back();
            loop->preheader->insts.pop_back();

            auto* load_orig = new LLVMIR::LoadInst(
                global_type_map[global_name], getGlobalOperand(global_name), getRegOperand(++cfg->func->max_reg));
            auto* store_temp = new LLVMIR::StoreInst(
                global_type_map[global_name], getRegOperand(temp_reg), getRegOperand(cfg->func->max_reg));

            loop->preheader->insts.push_back(load_orig);
            loop->preheader->insts.push_back(store_temp);
            loop->preheader->insts.push_back(end_inst);

            // Insert in exit
            auto it = exit->insts.begin();
            while (it != exit->insts.end() && (*it)->opcode == LLVMIR::IROpCode::PHI) { ++it; }

            if (it != exit->insts.end())
            {
                auto* load_temp = new LLVMIR::LoadInst(
                    global_type_map[global_name], getRegOperand(temp_reg), getRegOperand(++cfg->func->max_reg));
                auto* store_orig = new LLVMIR::StoreInst(
                    global_type_map[global_name], getGlobalOperand(global_name), getRegOperand(cfg->func->max_reg));

                exit->insts.insert(it, load_temp);
                exit->insts.insert(it, store_orig);
            }
        }

        if (is_motion_store)
        {
            optimization_changed = true;
            buildResultMap(cfg);
        }
    }

    void LICMPass::hoistInvariantLoads(CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop)
    {
        std::map<int, std::vector<LLVMIR::Instruction*>>         invariant_ptr_loads;  ///< RegNo -> load_instructions
        std::map<std::string, std::vector<LLVMIR::Instruction*>> global_ptr_loads;  ///< GlobalName -> load_instructions
        std::set<LLVMIR::Instruction*>                           write_insts;       ///< All write instructions in loop

        // Collect write instructions and potential invariant loads
        for (const auto* bb : loop->loop_nodes)
        {
            for (auto* inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::STORE)
                    write_insts.insert(inst);
                else if (inst->opcode == LLVMIR::IROpCode::CALL)
                {
                    const auto* call_inst = static_cast<const LLVMIR::CallInst*>(inst);
                    const auto  cfg_it    = cfg_map.find(call_inst->func_name);

                    if (cfg_it == cfg_map.end() || alias_analyser->isWriteMem(cfg_it->second)) write_insts.insert(inst);
                }
                else if (inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    const auto* load_inst = static_cast<const LLVMIR::LoadInst*>(inst);

                    if (load_inst->ptr->type == LLVMIR::OperandType::GLOBAL)
                    {
                        const auto* global_op = static_cast<const LLVMIR::GlobalOperand*>(load_inst->ptr);
                        global_ptr_loads[global_op->global_name].push_back(inst);
                    }
                    else if (load_inst->ptr->type == LLVMIR::OperandType::REG)
                    {
                        const auto* reg_ptr = static_cast<const LLVMIR::RegOperand*>(load_inst->ptr);

                        if (const auto result_it = result_map.find(reg_ptr->reg_num); result_it != result_map.end())
                        {
                            const auto* result_inst = result_it->second;
                            const auto* result_bb   = cfg->block_id_to_block.at(result_inst->block_id);

                            if (loop->loop_nodes.find(const_cast<LLVMIR::IRBlock*>(result_bb)) ==
                                loop->loop_nodes.end())
                                invariant_ptr_loads[reg_ptr->reg_num].push_back(inst);
                        }
                    }
                }
            }
        }

        std::set<LLVMIR::Instruction*> loads_to_hoist;

        for (const auto& [ptr_regno, load_insts] : invariant_ptr_loads)
        {
            auto* ptr = getRegOperand(ptr_regno);

            const bool can_hoist =
                std::none_of(write_insts.begin(), write_insts.end(), [this, ptr, cfg](const auto* write_inst) {
                    const auto result =
                        alias_analyser->queryInstModRef(const_cast<LLVMIR::Instruction*>(write_inst), ptr, cfg);
                    return result == Analysis::AliasAnalyser::Mod || result == Analysis::AliasAnalyser::ModRef;
                });

            if (can_hoist)
                for (auto* load_inst : load_insts)
                    if (dominatesAllExits(cfg, cfg->block_id_to_block.at(load_inst->block_id), loop))
                        loads_to_hoist.insert(load_inst);
        }

        for (const auto& [global_name, load_insts] : global_ptr_loads)
        {
            auto* ptr = getGlobalOperand(global_name);

            const bool can_hoist =
                std::none_of(write_insts.begin(), write_insts.end(), [this, ptr, cfg](const auto* write_inst) {
                    const auto result =
                        alias_analyser->queryInstModRef(const_cast<LLVMIR::Instruction*>(write_inst), ptr, cfg);
                    return result == Analysis::AliasAnalyser::Mod || result == Analysis::AliasAnalyser::ModRef;
                });

            if (can_hoist) loads_to_hoist.insert(load_insts.begin(), load_insts.end());
        }

        if (loads_to_hoist.empty()) return;

        const std::vector<LLVMIR::Instruction*> loads_vector(loads_to_hoist.begin(), loads_to_hoist.end());
        hoistInstructionsToPreheader(loop, loads_vector);
    }

    void LICMPass::hoistInvariantCalls(CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop)
    {
        std::vector<LLVMIR::Instruction*> call_insts_to_hoist;
        const auto                        write_insts = getLoopMemWriteInsts(cfg, loop);

        for (const auto* bb : loop->loop_nodes)
        {
            for (auto* inst : bb->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::CALL) continue;

                const auto* call_inst = static_cast<const LLVMIR::CallInst*>(inst);
                if (!isLoopInvariant(cfg, inst, loop, write_insts)) continue;

                const bool all_args_invariant = std::all_of(
                    call_inst->args.begin(), call_inst->args.end(), [this, loop, cfg](const auto& arg_pair) {
                        const auto& [arg_type, arg_operand] = arg_pair;

                        if (arg_operand->type != LLVMIR::OperandType::REG) return true;

                        const auto* reg_op    = static_cast<const LLVMIR::RegOperand*>(arg_operand);
                        const auto  result_it = result_map.find(reg_op->reg_num);

                        if (result_it == result_map.end()) return false;

                        const auto* result_inst = result_it->second;
                        const auto* result_bb   = cfg->block_id_to_block.at(result_inst->block_id);
                        return loop->loop_nodes.find(const_cast<LLVMIR::IRBlock*>(result_bb)) == loop->loop_nodes.end();
                    });

                if (!all_args_invariant) continue;

                const bool dominates_exits = dominatesAllExits(cfg, cfg->block_id_to_block.at(inst->block_id), loop);

                const auto cfg_it = cfg_map.find(call_inst->func_name);
                if (cfg_it == cfg_map.end()) continue;

                const bool is_no_side_effect = alias_analyser->isNoSideEffect(cfg_it->second);
                const bool is_pure_function  = is_no_side_effect && alias_analyser->getReadPtrs(cfg_it->second).empty();

                if (is_pure_function || (is_no_side_effect && dominates_exits)) { call_insts_to_hoist.push_back(inst); }
            }
        }

        if (call_insts_to_hoist.empty()) return;

        hoistInstructionsToPreheader(loop, call_insts_to_hoist);
        buildResultMap(cfg);
    }

    void LICMPass::processLoopRecursively(
        CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop, OptimizationFunction opt_func)
    {
        opt_func(cfg, loop_forest, loop);

        if (loop->loop_id >= 0 && static_cast<size_t>(loop->loop_id) < loop_forest.loopG.size())
            for (auto* inner_loop : loop_forest.loopG[loop->loop_id])
                processLoopRecursively(cfg, loop_forest, inner_loop, opt_func);
    }

    void LICMPass::buildResultMap(CFG* cfg)
    {
        result_map.clear();

        for (const auto* arg_reg : cfg->func->func_def->arg_regs)
            if (arg_reg->type == LLVMIR::OperandType::REG)
            {
                const auto* reg_op          = static_cast<const LLVMIR::RegOperand*>(arg_reg);
                result_map[reg_op->reg_num] = cfg->func->func_def;
            }

        for (const auto& [block_id, bb] : cfg->block_id_to_block)
            for (auto* inst : bb->insts)
            {
                inst->block_id = bb->block_id;

                if (const int reg_num = inst->GetResultReg(); reg_num != -1) result_map[reg_num] = inst;
            }
    }

    void LICMPass::performLICM(CFG* cfg)
    {
        if (!cfg->LoopForest) return;

        buildResultMap(cfg);

        std::vector<NaturalLoop*> top_level_loops;
        for (auto* loop : cfg->LoopForest->loop_set)
            if (loop->fa_loop == nullptr) top_level_loops.push_back(loop);

        for (auto* loop : top_level_loops)
        {
            bool          loop_changed    = false;
            int           iteration_count = 0;
            constexpr int MAX_ITERATIONS  = 10;

            do {
                loop_changed = false;
                ++iteration_count;

                if (iteration_count > MAX_ITERATIONS) break;

                optimization_changed = false;
                processLoopRecursively(
                    cfg, *cfg->LoopForest, loop, [this](CFG* c, NaturalLoopForest& lf, NaturalLoop* l) {
                        hoistLoopInvariants(c, lf, l);
                    });
                loop_changed |= optimization_changed;

                optimization_changed = false;
                processLoopRecursively(
                    cfg, *cfg->LoopForest, loop, [this](CFG* c, NaturalLoopForest& lf, NaturalLoop* l) {
                        hoistInvariantCalls(c, lf, l);
                    });
                loop_changed |= optimization_changed;

                optimization_changed = false;
                processLoopRecursively(
                    cfg, *cfg->LoopForest, loop, [this](CFG* c, NaturalLoopForest& lf, NaturalLoop* l) {
                        promoteMemToRegs(c, lf, l);
                    });
                loop_changed |= optimization_changed;

                optimization_changed = false;
                processLoopRecursively(
                    cfg, *cfg->LoopForest, loop, [this](CFG* c, NaturalLoopForest& lf, NaturalLoop* l) {
                        hoistInvariantLoads(c, lf, l);
                    });
                loop_changed |= optimization_changed;

            } while (loop_changed);
        }
    }

}  // namespace StructuralTransform
