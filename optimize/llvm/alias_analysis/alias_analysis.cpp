#include "llvm/alias_analysis/alias_analysis.h"
#include <cassert>
#include <queue>
#include <vector>
#include <map>
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include "cfg.h"
#include <sstream>

namespace Analysis
{
    void MemLocation::addTarget(LLVMIR::Operand* op)
    {
        if (op != nullptr) targets.insert(op);
    }

    void MemLocation::merge(const MemLocation& other)
    {
        for (auto* target : other.targets) targets.insert(target);
        escapes_function = escapes_function || other.escapes_function;
        is_stack_local   = is_stack_local && other.is_stack_local;
    }

    void FuncMemProfile::addRead(LLVMIR::Operand* op)
    {
        if (op != nullptr) mem_reads.insert(op);
    }

    void FuncMemProfile::addWrite(LLVMIR::Operand* op)
    {
        if (op != nullptr) mem_writes.insert(op);
    }

    void FuncMemProfile::addReads(const std::vector<LLVMIR::Operand*>& ops)
    {
        for (auto* op : ops) addRead(op);
    }

    void FuncMemProfile::addWrites(const std::vector<LLVMIR::Operand*>& ops)
    {
        for (auto* op : ops) addWrite(op);
    }

    void FuncMemProfile::combineProfile(
        LLVMIR::CallInst* call, const FuncMemProfile& other, const std::unordered_map<int, MemLocation>& locations)
    {
        has_external_deps = has_external_deps || other.has_external_deps;

        for (auto* op : other.mem_reads)
        {
            if (op->type == LLVMIR::OperandType::GLOBAL)
                addRead(op);
            else if (op->type == LLVMIR::OperandType::REG)
            {
                auto* reg_ptr = static_cast<LLVMIR::RegOperand*>(op);
                int   arg_idx = reg_ptr->reg_num;
                if (arg_idx < static_cast<int>(call->args.size()))
                {
                    auto* actual_arg = call->args[arg_idx].second;
                    if (actual_arg->type == LLVMIR::OperandType::REG)
                    {
                        auto* actual_reg = static_cast<LLVMIR::RegOperand*>(actual_arg);
                        auto  loc_it     = locations.find(actual_reg->reg_num);
                        if (loc_it != locations.end() && !loc_it->second.isLocal())
                        {
                            for (auto* target : loc_it->second.targets) addRead(target);
                        }
                    }
                    else
                        addRead(actual_arg);
                }
            }
        }

        for (auto* op : other.mem_writes)
        {
            if (op->type == LLVMIR::OperandType::GLOBAL) { addWrite(op); }
            else if (op->type == LLVMIR::OperandType::REG)
            {
                auto* reg_ptr = static_cast<LLVMIR::RegOperand*>(op);
                int   arg_idx = reg_ptr->reg_num;
                if (arg_idx < static_cast<int>(call->args.size()))
                {
                    auto* actual_arg = call->args[arg_idx].second;
                    if (actual_arg->type == LLVMIR::OperandType::REG)
                    {
                        auto* actual_reg = static_cast<LLVMIR::RegOperand*>(actual_arg);
                        auto  loc_it     = locations.find(actual_reg->reg_num);
                        if (loc_it != locations.end() && !loc_it->second.isLocal())
                        {
                            for (auto* target : loc_it->second.targets) { addWrite(target); }
                        }
                    }
                    else { addWrite(actual_arg); }
                }
            }
        }
    }

    static CFG* getCfgByName(LLVMIR::IR* ir, const std::string& name)
    {
        for (auto& func : ir->functions)
        {
            if (func->func_def->func_name == name)
            {
                if (ir->cfg.count(func->func_def)) return ir->cfg.at(func->func_def);
            }
        }
        return nullptr;
    }

    AliasAnalyser::AliasAnalyser(LLVMIR::IR* ir) : ir(ir) {}

    void AliasAnalyser::buildDefMap(CFG* cfg)
    {
        def_map[cfg].clear();

        for (const auto* arg_reg : cfg->func->func_def->arg_regs)
        {
            if (arg_reg->type == LLVMIR::OperandType::REG)
            {
                const auto* reg_op            = static_cast<const LLVMIR::RegOperand*>(arg_reg);
                def_map[cfg][reg_op->reg_num] = cfg->func->func_def;
            }
        }

        for (const auto& [block_id, bb] : cfg->block_id_to_block)
        {
            for (auto* inst : bb->insts)
            {
                inst->block_id = bb->block_id;
                if (const int reg_num = inst->GetResultReg(); reg_num != -1) { def_map[cfg][reg_num] = inst; }
            }
        }
    }

    void AliasAnalyser::processFunction(CFG* cfg)
    {
        auto& locations = reg_locations[cfg];
        locations.clear();

        auto& profile = func_profiles[cfg];
        profile       = FuncMemProfile();

        buildDefMap(cfg);

        LLVMIR::FuncDefInst* func_def = cfg->func->func_def;
        for (unsigned i = 0; i < func_def->arg_regs.size(); ++i)
        {
            if (func_def->arg_types[i] == LLVMIR::DataType::PTR)
            {
                auto* arg_op = func_def->arg_regs[i];
                int   reg_no = static_cast<LLVMIR::RegOperand*>(arg_op)->reg_num;

                auto& loc = locations[reg_no];
                loc.addTarget(arg_op);
                loc.markEscaped();
            }
        }

        for (auto* inst : cfg->block_id_to_block.at(0)->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::ALLOCA)
            {
                auto* alloca_inst = static_cast<LLVMIR::AllocInst*>(inst);
                int   def_reg     = alloca_inst->GetResultReg();
                locations[def_reg].addTarget(alloca_inst->res);
            }
        }

        handlePtrPropagation(cfg, locations);
        collectMemAccesses(cfg, locations);
    }

    void AliasAnalyser::handlePtrPropagation(CFG* cfg, std::unordered_map<int, MemLocation>& locations)
    {
        bool changed = true;
        while (changed)
        {
            changed = false;

            std::queue<LLVMIR::IRBlock*>         worklist;
            std::unordered_set<LLVMIR::IRBlock*> visited;
            worklist.push(cfg->block_id_to_block.at(0));

            while (!worklist.empty())
            {
                auto* bb = worklist.front();
                worklist.pop();

                if (visited.count(bb)) continue;
                visited.insert(bb);

                for (auto* inst : bb->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                    {
                        auto* gep_inst   = static_cast<LLVMIR::GEPInst*>(inst);
                        auto* base_ptr   = gep_inst->base_ptr;
                        int   result_reg = gep_inst->GetResultReg();

                        if (base_ptr->type == LLVMIR::OperandType::REG)
                        {
                            auto* base_reg = static_cast<LLVMIR::RegOperand*>(base_ptr);
                            auto  base_it  = locations.find(base_reg->reg_num);
                            if (base_it != locations.end())
                            {
                                auto old_size = locations[result_reg].targets.size();
                                locations[result_reg].merge(base_it->second);
                                if (locations[result_reg].targets.size() != old_size) { changed = true; }
                            }
                            else
                            {
                                locations[result_reg].addTarget(base_ptr);
                                locations[result_reg].markEscaped();
                                changed = true;
                            }
                        }
                        else
                        {
                            auto old_size = locations[result_reg].targets.size();
                            locations[result_reg].addTarget(base_ptr);
                            locations[result_reg].markNonLocal();
                            if (locations[result_reg].targets.size() != old_size) { changed = true; }
                        }
                    }
                    else if (inst->opcode == LLVMIR::IROpCode::PHI)
                    {
                        auto* phi_inst = static_cast<LLVMIR::PhiInst*>(inst);
                        if (phi_inst->type == LLVMIR::DataType::PTR)
                        {
                            int  result_reg = phi_inst->GetResultReg();
                            auto old_size   = locations[result_reg].targets.size();

                            for (const auto& [val, label] : phi_inst->vals_for_labels)
                            {
                                if (val->type == LLVMIR::OperandType::REG)
                                {
                                    auto* val_reg = static_cast<LLVMIR::RegOperand*>(val);
                                    auto  val_it  = locations.find(val_reg->reg_num);
                                    if (val_it != locations.end()) { locations[result_reg].merge(val_it->second); }
                                    else
                                    {
                                        locations[result_reg].addTarget(val);
                                        locations[result_reg].markEscaped();
                                    }
                                }
                                else
                                {
                                    locations[result_reg].addTarget(val);
                                    locations[result_reg].markNonLocal();
                                }
                            }

                            if (locations[result_reg].targets.size() != old_size) { changed = true; }
                        }
                    }
                }

                for (auto* succ_bb : cfg->G[bb->block_id]) { worklist.push(succ_bb); }
            }
        }
    }

    void AliasAnalyser::collectMemAccesses(CFG* cfg, const std::unordered_map<int, MemLocation>& locations)
    {
        auto& profile = func_profiles[cfg];

        for (const auto& [block_id, bb] : cfg->block_id_to_block)
        {
            for (auto* inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto* store_inst = static_cast<LLVMIR::StoreInst*>(inst);
                    auto* ptr        = store_inst->ptr;

                    if (ptr->type == LLVMIR::OperandType::GLOBAL) { profile.addWrite(ptr); }
                    else if (ptr->type == LLVMIR::OperandType::REG)
                    {
                        auto* ptr_reg = static_cast<LLVMIR::RegOperand*>(ptr);
                        auto  loc_it  = locations.find(ptr_reg->reg_num);
                        if (loc_it != locations.end() && !loc_it->second.isLocal())
                        {
                            for (auto* target : loc_it->second.targets) { profile.addWrite(target); }
                        }
                    }
                }
                else if (inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    auto* load_inst = static_cast<LLVMIR::LoadInst*>(inst);
                    auto* ptr       = load_inst->ptr;

                    if (ptr->type == LLVMIR::OperandType::GLOBAL) { profile.addRead(ptr); }
                    else if (ptr->type == LLVMIR::OperandType::REG)
                    {
                        auto* ptr_reg = static_cast<LLVMIR::RegOperand*>(ptr);
                        auto  loc_it  = locations.find(ptr_reg->reg_num);
                        if (loc_it != locations.end() && !loc_it->second.isLocal())
                        {
                            for (auto* target : loc_it->second.targets) { profile.addRead(target); }
                        }
                    }
                }
            }
        }
    }

    bool AliasAnalyser::checkSameBaseWithDistinctOffset(LLVMIR::Operand* p1, LLVMIR::Operand* p2, CFG* cfg)
    {
        if (p1->type != LLVMIR::OperandType::REG || p2->type != LLVMIR::OperandType::REG) { return false; }

        auto* r1   = static_cast<LLVMIR::RegOperand*>(p1);
        auto* r2   = static_cast<LLVMIR::RegOperand*>(p2);
        int   reg1 = r1->reg_num;
        int   reg2 = r2->reg_num;

        auto def_it1 = def_map[cfg].find(reg1);
        auto def_it2 = def_map[cfg].find(reg2);

        if (def_it1 == def_map[cfg].end() || def_it2 == def_map[cfg].end()) { return false; }

        auto* inst1 = def_it1->second;
        auto* inst2 = def_it2->second;

        if (!inst1 || !inst2 || inst1->opcode != LLVMIR::IROpCode::GETELEMENTPTR ||
            inst2->opcode != LLVMIR::IROpCode::GETELEMENTPTR)
        {
            return false;
        }

        auto* gep1 = static_cast<LLVMIR::GEPInst*>(inst1);
        auto* gep2 = static_cast<LLVMIR::GEPInst*>(inst2);

        if (gep1->idxs.size() != gep2->idxs.size() || gep1->idxs.empty()) { return false; }

        // Check if base pointers are the same
        if (gep1->base_ptr->getName() != gep2->base_ptr->getName()) { return false; }

        // Check if last indices are different constants
        auto* idx1 = gep1->idxs.back();
        auto* idx2 = gep2->idxs.back();

        if (idx1->type == LLVMIR::OperandType::IMMEI32 && idx2->type == LLVMIR::OperandType::IMMEI32)
        {
            auto* imm1 = static_cast<LLVMIR::ImmeI32Operand*>(idx1);
            auto* imm2 = static_cast<LLVMIR::ImmeI32Operand*>(idx2);
            return imm1->value != imm2->value;
        }

        return false;
    }

    bool AliasAnalyser::checkIdenticalGEP(LLVMIR::Operand* p1, LLVMIR::Operand* p2, CFG* cfg)
    {
        // 必须都是寄存器操作数
        if (p1->type != LLVMIR::OperandType::REG || p2->type != LLVMIR::OperandType::REG) { return false; }

        // 获取寄存器编号
        auto* r1   = static_cast<LLVMIR::RegOperand*>(p1);
        auto* r2   = static_cast<LLVMIR::RegOperand*>(p2);
        int   reg1 = r1->reg_num;
        int   reg2 = r2->reg_num;

        // 查找定义这些寄存器的指令
        auto def_it1 = def_map[cfg].find(reg1);
        auto def_it2 = def_map[cfg].find(reg2);

        if (def_it1 == def_map[cfg].end() || def_it2 == def_map[cfg].end()) { return false; }

        // 获取定义指令
        auto* inst1 = def_it1->second;
        auto* inst2 = def_it2->second;

        // 检查两个定义都是GEP指令
        if (!inst1 || !inst2) { return false; }

        if (inst1->opcode != LLVMIR::IROpCode::GETELEMENTPTR || inst2->opcode != LLVMIR::IROpCode::GETELEMENTPTR)
        {
            return false;
        }

        // 转换为GEP指令
        auto* gep1 = static_cast<LLVMIR::GEPInst*>(inst1);
        auto* gep2 = static_cast<LLVMIR::GEPInst*>(inst2);

        // 检查基指针是否相同
        if (gep1->base_ptr->getName() != gep2->base_ptr->getName()) { return false; }

        // 检查索引数量是否相同
        if (gep1->idxs.size() != gep2->idxs.size()) { return false; }

        // 检查每个索引是否完全相同
        for (size_t i = 0; i < gep1->idxs.size(); ++i)
        {
            auto* idx1 = gep1->idxs[i];
            auto* idx2 = gep2->idxs[i];

            // 如果都是常量索引
            if (idx1->type == LLVMIR::OperandType::IMMEI32 && idx2->type == LLVMIR::OperandType::IMMEI32)
            {
                auto* imm1 = static_cast<LLVMIR::ImmeI32Operand*>(idx1);
                auto* imm2 = static_cast<LLVMIR::ImmeI32Operand*>(idx2);
                if (imm1->value != imm2->value) { return false; }
            }
            // 如果都是寄存器索引
            else if (idx1->type == LLVMIR::OperandType::REG && idx2->type == LLVMIR::OperandType::REG)
            {
                auto* reg1 = static_cast<LLVMIR::RegOperand*>(idx1);
                auto* reg2 = static_cast<LLVMIR::RegOperand*>(idx2);

                // 检查寄存器编号是否相同
                if (reg1->reg_num != reg2->reg_num) { return false; }
            }
            // 索引类型不同
            else if (idx1->type != idx2->type) { return false; }
            // 其他类型的索引，比较名称
            else if (idx1->getName() != idx2->getName()) { return false; }
        }

        // 所有条件都满足，确定是相同的GEP指令
        return true;
    }

    std::vector<LLVMIR::Operand*> AliasAnalyser::getWritePtrs(CFG* cfg)
    {
        std::vector<LLVMIR::Operand*> result;
        for (auto* op : func_profiles[cfg].mem_writes) { result.push_back(op); }
        return result;
    }

    std::vector<LLVMIR::Operand*> AliasAnalyser::getReadPtrs(CFG* cfg)
    {
        std::vector<LLVMIR::Operand*> result;
        for (auto* op : func_profiles[cfg].mem_reads) { result.push_back(op); }
        return result;
    }

    void AliasAnalyser::run()
    {
        for (auto const& [func_def, cfg] : ir->cfg) buildDefMap(cfg);
        for (auto const& [func_def, cfg] : ir->cfg) processFunction(cfg);

        std::map<CFG*, std::vector<LLVMIR::CallInst*>> call_map;
        for (auto const& [func_def, cfg] : ir->cfg)
        {
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                for (auto* inst : bb->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::CALL)
                    {
                        call_map[cfg].push_back(static_cast<LLVMIR::CallInst*>(inst));
                    }
                }
            }
        }

        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto const& [func_def, cfg] : ir->cfg)
            {
                auto& profile = func_profiles[cfg];
                if (profile.has_external_deps) continue;

                const auto& locations = reg_locations[cfg];

                if (call_map.find(cfg) == call_map.end()) continue;

                for (auto* call_inst : call_map.at(cfg))
                {
                    std::string call_name = call_inst->func_name;
                    auto*       call_cfg  = getCfgByName(ir, call_name);

                    /*
                    std::cerr << "DEBUG: Processing CALL to " << call_name << ", call_cfg=" << (void*)call_cfg
                              << std::endl;
                    */

                    if (call_cfg == nullptr)
                    {
                        if (!profile.has_external_deps)
                        {
                            profile.has_external_deps = true;
                            changed                   = true;
                        }
                        continue;
                    }

                    const auto& callee_profile = func_profiles.at(call_cfg);
                    if (callee_profile.has_external_deps)
                    {
                        if (!profile.has_external_deps)
                        {
                            profile.has_external_deps = true;
                            changed                   = true;
                        }
                        continue;
                    }

                    auto old_reads_size  = profile.mem_reads.size();
                    auto old_writes_size = profile.mem_writes.size();

                    profile.combineProfile(call_inst, callee_profile, locations);

                    if (profile.mem_reads.size() != old_reads_size || profile.mem_writes.size() != old_writes_size)
                    {
                        changed = true;
                    }
                }
            }
        }
    }

    AliasAnalyser::AliasResult AliasAnalyser::queryAlias(LLVMIR::Operand* op1, LLVMIR::Operand* op2, CFG* cfg)
    {
        if (checkIdenticalGEP(op1, op2, cfg)) { return MustAlias; }
        AliasResult result = NoAlias;
        if (reg_locations.count(cfg))
        {
            const auto& locations = reg_locations[cfg];

            MemLocation loc1, loc2;
            bool        found1 = false, found2 = false;

            // Get location info for op1
            if (op1->type == LLVMIR::OperandType::REG)
            {
                auto* reg1 = static_cast<LLVMIR::RegOperand*>(op1);
                auto  it1  = locations.find(reg1->reg_num);
                if (it1 != locations.end())
                {
                    loc1   = it1->second;
                    found1 = true;
                }
            }
            else if (op1->type == LLVMIR::OperandType::GLOBAL)
            {
                loc1.addTarget(op1);
                loc1.markNonLocal();
                found1 = true;
            }

            // Get location info for op2
            if (op2->type == LLVMIR::OperandType::REG)
            {
                auto* reg2 = static_cast<LLVMIR::RegOperand*>(op2);
                auto  it2  = locations.find(reg2->reg_num);
                if (it2 != locations.end())
                {
                    loc2   = it2->second;
                    found2 = true;
                }
            }
            else if (op2->type == LLVMIR::OperandType::GLOBAL)
            {
                loc2.addTarget(op2);
                loc2.markNonLocal();
                found2 = true;
            }

            if (found1 && found2)
            {
                // Handle escapes_function case
                // If either pointer escapes the function, they may alias (conservative approach)
                if (loc1.escapes_function || loc2.escapes_function) { result = MustAlias; }
                else
                {
                    if (loc1.targets.size() == 1 && loc2.targets.size() == 1)
                    {
                        if (checkSameBaseWithDistinctOffset(op1, op2, cfg)) { result = NoAlias; }
                        else
                        {
                            for (auto* target1 : loc1.targets)
                            {
                                for (auto* target2 : loc2.targets)
                                {
                                    if (target1->getName() == target2->getName())
                                    {
                                        result = MustAlias;
                                        break;
                                    }
                                }
                                if (result == MustAlias) break;
                            }
                        }
                    }
                    else
                    {
                        for (auto* target1 : loc1.targets)
                        {
                            for (auto* target2 : loc2.targets)
                            {
                                if (target1->getName() == target2->getName())
                                {
                                    result = MustAlias;
                                    break;
                                }
                            }
                            if (result == MustAlias) break;
                        }
                    }
                }
            }
        }

        return result;
    }

    AliasAnalyser::ModRefResult AliasAnalyser::queryInstModRef(LLVMIR::Instruction* inst, LLVMIR::Operand* op, CFG* cfg)
    {
        ModRefResult result = NoModRef;

        if (reg_locations.count(cfg))
        {
            const auto& locations = reg_locations[cfg];

            MemLocation op_loc;
            bool        found_op = false;

            if (op->type == LLVMIR::OperandType::REG)
            {
                auto* reg_op = static_cast<LLVMIR::RegOperand*>(op);
                auto  it     = locations.find(reg_op->reg_num);
                if (it != locations.end())
                {
                    op_loc   = it->second;
                    found_op = true;
                }
                else
                {
                    // Unknown pointer should -> escaping
                    op_loc.addTarget(op);
                    op_loc.markEscaped();
                    found_op = true;
                }
            }
            else if (op->type == LLVMIR::OperandType::GLOBAL)
            {
                op_loc.addTarget(op);
                op_loc.markNonLocal();
                found_op = true;
            }

            if (!found_op) { return NoModRef; }

            if (inst->opcode == LLVMIR::IROpCode::LOAD)
            {
                auto* load_inst = static_cast<LLVMIR::LoadInst*>(inst);
                auto* ptr       = load_inst->ptr;

                MemLocation ptr_loc;
                bool        found_ptr = false;

                if (ptr->type == LLVMIR::OperandType::REG)
                {
                    auto* ptr_reg = static_cast<LLVMIR::RegOperand*>(ptr);
                    auto  it      = locations.find(ptr_reg->reg_num);
                    if (it != locations.end())
                    {
                        ptr_loc   = it->second;
                        found_ptr = true;
                    }
                    else
                    {
                        // Unknown pointer -> escaping
                        ptr_loc.addTarget(ptr);
                        ptr_loc.markEscaped();
                        found_ptr = true;
                    }
                }
                else if (ptr->type == LLVMIR::OperandType::GLOBAL)
                {
                    ptr_loc.addTarget(ptr);
                    ptr_loc.markNonLocal();
                    found_ptr = true;
                }

                if (found_ptr)
                {
                    if (ptr_loc.escapes_function || op_loc.escapes_function) { result = Ref; }
                    else
                    {
                        for (auto* ptr_target : ptr_loc.targets)
                        {
                            for (auto* op_target : op_loc.targets)
                            {
                                if (ptr_target->getName() == op_target->getName())
                                {
                                    result = Ref;
                                    break;
                                }
                            }
                            if (result == Ref) break;
                        }
                    }
                }
            }
            else if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                auto* store_inst = static_cast<LLVMIR::StoreInst*>(inst);
                auto* ptr        = store_inst->ptr;

                MemLocation ptr_loc;
                bool        found_ptr = false;

                if (ptr->type == LLVMIR::OperandType::REG)
                {
                    auto* ptr_reg = static_cast<LLVMIR::RegOperand*>(ptr);
                    auto  it      = locations.find(ptr_reg->reg_num);
                    if (it != locations.end())
                    {
                        ptr_loc   = it->second;
                        found_ptr = true;
                    }
                    else
                    {
                        ptr_loc.addTarget(ptr);
                        ptr_loc.markEscaped();
                        found_ptr = true;
                    }
                }
                else if (ptr->type == LLVMIR::OperandType::GLOBAL)
                {
                    ptr_loc.addTarget(ptr);
                    ptr_loc.markNonLocal();
                    found_ptr = true;
                }

                if (found_ptr)
                {
                    if (ptr_loc.escapes_function || op_loc.escapes_function) { result = Mod; }
                    else
                    {
                        for (auto* ptr_target : ptr_loc.targets)
                        {
                            for (auto* op_target : op_loc.targets)
                            {
                                if (ptr_target->getName() == op_target->getName())
                                {
                                    result = Mod;
                                    break;
                                }
                            }
                            if (result == Mod) break;
                        }
                    }
                }
            }
            else if (inst->opcode == LLVMIR::IROpCode::CALL)
            {
                auto*       call_inst = static_cast<LLVMIR::CallInst*>(inst);
                std::string call_name = call_inst->func_name;
                auto*       call_cfg  = getCfgByName(ir, call_name);

                // std::cerr << "DEBUG: Processing CALL to " << call_name << ", call_cfg=" << (void*)call_cfg <<
                // std::endl;

                if (call_cfg == nullptr)
                {
                    if (call_name.find("llvm.memset") != std::string::npos ||
                        call_name.find("llvm.memcpy") != std::string::npos ||
                        call_name.find("llvm.memmove") != std::string::npos)
                        result = ModRef;
                    else if (op_loc.escapes_function || !op_loc.isLocal() || op->type == LLVMIR::OperandType::GLOBAL)
                    {
                        result = ModRef;
                    }
                }
                else
                {
                    const auto& callee_profile = func_profiles.at(call_cfg);

                    if (callee_profile.has_external_deps)
                    {
                        if (op_loc.escapes_function || !op_loc.isLocal() || op->type == LLVMIR::OperandType::GLOBAL)
                        {
                            result = ModRef;
                        }
                    }
                    else
                    {
                        bool is_mod = false, is_ref = false;

                        if (op_loc.escapes_function)
                        {
                            if (!callee_profile.isPure())
                            {
                                is_ref = true;
                                is_mod = true;
                            }
                        }

                        if (!is_ref || !is_mod)
                        {
                            for (auto* callee_ptr : callee_profile.mem_reads)
                            {
                                bool should_ref = false;

                                if (callee_ptr->type == LLVMIR::OperandType::GLOBAL)
                                {
                                    if (op->type == LLVMIR::OperandType::GLOBAL &&
                                        callee_ptr->getName() == op->getName())
                                    {
                                        should_ref = true;
                                    }
                                    else if (op->type == LLVMIR::OperandType::REG)
                                    {
                                        for (auto* op_target : op_loc.targets)
                                        {
                                            if (op_target->getName() == callee_ptr->getName())
                                            {
                                                should_ref = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                else if (callee_ptr->type == LLVMIR::OperandType::REG)
                                {
                                    auto* callee_reg  = static_cast<LLVMIR::RegOperand*>(callee_ptr);
                                    int   param_index = callee_reg->reg_num;

                                    if (param_index < (int)call_inst->args.size())
                                    {
                                        auto [arg_type, actual_arg] = call_inst->args[param_index];

                                        if (actual_arg->type == LLVMIR::OperandType::GLOBAL &&
                                            op->type == LLVMIR::OperandType::GLOBAL &&
                                            actual_arg->getName() == op->getName())
                                        {
                                            should_ref = true;
                                        }
                                        else if (actual_arg->type == LLVMIR::OperandType::REG &&
                                                 op->type == LLVMIR::OperandType::REG)
                                        {
                                            auto* arg_reg = static_cast<LLVMIR::RegOperand*>(actual_arg);
                                            auto* op_reg  = static_cast<LLVMIR::RegOperand*>(op);
                                            auto  arg_it  = locations.find(arg_reg->reg_num);
                                            auto  op_it   = locations.find(op_reg->reg_num);

                                            if (arg_it != locations.end() && op_it != locations.end())
                                            {
                                                for (auto* arg_target : arg_it->second.targets)
                                                {
                                                    for (auto* op_target : op_it->second.targets)
                                                    {
                                                        if (arg_target->getName() == op_target->getName())
                                                        {
                                                            should_ref = true;
                                                            break;
                                                        }
                                                    }
                                                    if (should_ref) break;
                                                }
                                            }
                                        }
                                    }
                                }

                                if (should_ref)
                                {
                                    is_ref = true;
                                    break;
                                }
                            }

                            if (!is_mod)
                            {
                                for (auto* callee_ptr : callee_profile.mem_writes)
                                {
                                    bool should_mod = false;

                                    if (callee_ptr->type == LLVMIR::OperandType::GLOBAL)
                                    {
                                        if (op->type == LLVMIR::OperandType::GLOBAL &&
                                            callee_ptr->getName() == op->getName())
                                        {
                                            should_mod = true;
                                        }
                                        else if (op->type == LLVMIR::OperandType::REG)
                                        {
                                            for (auto* op_target : op_loc.targets)
                                            {
                                                if (op_target->getName() == callee_ptr->getName())
                                                {
                                                    should_mod = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else if (callee_ptr->type == LLVMIR::OperandType::REG)
                                    {
                                        auto* callee_reg  = static_cast<LLVMIR::RegOperand*>(callee_ptr);
                                        int   param_index = callee_reg->reg_num;

                                        if (param_index < (int)call_inst->args.size())
                                        {
                                            auto [arg_type, actual_arg] = call_inst->args[param_index];

                                            if (actual_arg->type == LLVMIR::OperandType::GLOBAL &&
                                                op->type == LLVMIR::OperandType::GLOBAL &&
                                                actual_arg->getName() == op->getName())
                                            {
                                                should_mod = true;
                                            }
                                            else if (actual_arg->type == LLVMIR::OperandType::REG &&
                                                     op->type == LLVMIR::OperandType::REG)
                                            {
                                                auto* arg_reg = static_cast<LLVMIR::RegOperand*>(actual_arg);
                                                auto* op_reg  = static_cast<LLVMIR::RegOperand*>(op);
                                                auto  arg_it  = locations.find(arg_reg->reg_num);
                                                auto  op_it   = locations.find(op_reg->reg_num);

                                                if (arg_it != locations.end() && op_it != locations.end())
                                                {
                                                    for (auto* arg_target : arg_it->second.targets)
                                                    {
                                                        for (auto* op_target : op_it->second.targets)
                                                        {
                                                            if (arg_target->getName() == op_target->getName())
                                                            {
                                                                should_mod = true;
                                                                break;
                                                            }
                                                        }
                                                        if (should_mod) break;
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (should_mod)
                                    {
                                        is_mod = true;
                                        break;
                                    }
                                }
                            }
                        }

                        if (is_mod && is_ref)
                            result = ModRef;
                        else if (is_mod)
                            result = Mod;
                        else if (is_ref)
                            result = Ref;
                        else
                            result = NoModRef;
                    }
                }
            }
        }

        return result;
    }

    bool AliasAnalyser::isLocalPtr(CFG* cfg, LLVMIR::Operand* ptr)
    {
        if (ptr->type != LLVMIR::OperandType::REG) { return true; }

        auto& location = reg_locations[cfg];
        auto  reg      = static_cast<LLVMIR::RegOperand*>(ptr);

        if (location.count(reg->reg_num))
        {
            const auto& memLoc = location.at(reg->reg_num);
            return memLoc.escapes_function || !memLoc.is_stack_local;
        }
        return true;
    }

}  // namespace Analysis
