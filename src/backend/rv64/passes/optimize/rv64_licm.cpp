#include <backend/rv64/passes/optimize/rv64_licm.h>
#include <backend/rv64/passes/rv64_loop_find.h>
#include <iostream>
#include <functional>

#define REGISTER_PRESSURE_AWARE

namespace Backend::RV64::Passes::Optimize
{

    LICMPass::LICMPass(std::vector<Function*>& functions)
        : functions_(functions), current_func(nullptr), curr_loop(nullptr)
    {}

    bool LICMPass::run()
    {
        for (auto* func : functions_)
        {
            if (func && func->cfg) { executeInFunction(func); }
        }
        return true;
    }

    void LICMPass::executeInFunction(Function* func)
    {
        current_func = func;
        initResultMapInCurrFunc();

        if (!func->cfg || !func->cfg->loop_forest) { return; }

        auto* loop_forest = func->cfg->loop_forest.get();

        for (auto* loop : loop_forest->getRootLoops()) { processLoopRecursively(loop); }
    }

    void LICMPass::processLoopRecursively(NaturalLoop* loop)
    {
        curr_loop = loop;
        licmInCurrLoop();

        if (loop->loop_id >= 0 && static_cast<size_t>(loop->loop_id) < current_func->cfg->loop_forest->loopG.size())
        {
            for (auto* inner_loop : current_func->cfg->loop_forest->loopG[loop->loop_id])
            {
                processLoopRecursively(inner_loop);
            }
        }
    }

    void LICMPass::initResultMapInCurrFunc()
    {
        inst_def_map.clear();

        for (const auto& [block_id, block] : current_func->cfg->blocks)
        {
            for (auto* inst : block->insts)
            {
                auto write_regs = inst->getWriteRegs();
                if (write_regs.size() == 1)
                {
                    auto* reg = write_regs[0];
                    if (reg && reg->is_virtual) { inst_def_map[reg->reg_num] = block; }
                }
            }
        }
    }

    bool LICMPass::isNeedLicm(Instruction* inst)
    {
        if (!inst) return false;

        auto read_regs  = inst->getReadRegs();
        auto write_regs = inst->getWriteRegs();

        for (auto* reg : read_regs)
        {
            if (reg && !reg->is_virtual) return false;
        }
        for (auto* reg : write_regs)
        {
            if (reg && !reg->is_virtual) return false;
        }

        switch (inst->inst_type)
        {
            case InstType::MOVE: return true;

            case InstType::RV64:
            {
                auto* rv_inst = static_cast<RV64Inst*>(inst);
                switch (rv_inst->op)
                {
                    case RV64InstType::LUI:
                    case RV64InstType::ADDI:
                    case RV64InstType::ADD:
                    case RV64InstType::MUL: return true;
                    default: return false;
                }
            }

            default: return false;
        }
    }

    bool LICMPass::isInvariant(CFG* cfg, Instruction* inst, NaturalLoop* loop)
    {
        if (!isNeedLicm(inst)) { return false; }

#ifdef REGISTER_PRESSURE_AWARE
        if (currloop_motion_count >= max_motion_count)
        {
            if (inst->inst_type != InstType::RV64) { return false; }
            auto* rv_inst = static_cast<RV64Inst*>(inst);
            if (rv_inst->op != RV64InstType::MUL && rv_inst->op != RV64InstType::ADDI) { return false; }
        }
#endif

        auto write_regs = inst->getWriteRegs();
        if (write_regs.size() != 1) return false;

        auto* result_reg = write_regs[0];
        if (!result_reg || !result_reg->is_virtual) return false;

        int result_reg_num = result_reg->reg_num;

        switch (inst->inst_type)
        {
            case InstType::MOVE:
            {
                auto* move_inst = static_cast<MoveInst*>(inst);
                auto  read_regs = inst->getReadRegs();

                if (read_regs.empty())
                {
                    invariant_set.insert(result_reg_num);
                    currloop_motion_count += 1;
                    return true;
                }
                else if (read_regs.size() == 1)
                {
                    auto* src_reg = read_regs[0];
                    if (src_reg && src_reg->is_virtual)
                    {
                        int src_reg_num = src_reg->reg_num;
                        if (invariant_set.find(src_reg_num) != invariant_set.end())
                        {
                            invariant_set.insert(result_reg_num);
                            currloop_motion_count += 1;
                            return true;
                        }

                        auto def_it = inst_def_map.find(src_reg_num);
                        if (def_it != inst_def_map.end())
                        {
                            auto* def_block = def_it->second;
                            if (loop->loop_nodes.find(def_block) == loop->loop_nodes.end())
                            {
                                invariant_set.insert(result_reg_num);
                                currloop_motion_count += 1;
                                return true;
                            }
                        }
                    }
                }
                return false;
            }

            case InstType::RV64:
            {
                auto* rv_inst = static_cast<RV64Inst*>(inst);

                if (rv_inst->op == RV64InstType::LUI)
                {
                    invariant_set.insert(result_reg_num);
                    currloop_motion_count += 1;
                    return true;
                }
                else if (rv_inst->op == RV64InstType::ADDI)
                {
                    int rs1_num = rv_inst->rs1.reg_num;
                    if (invariant_set.find(rs1_num) != invariant_set.end())
                    {
                        invariant_set.insert(result_reg_num);
                        currloop_motion_count += 1;
                        return true;
                    }

                    auto def_it = inst_def_map.find(rs1_num);
                    if (def_it != inst_def_map.end())
                    {
                        auto* def_block = def_it->second;
                        if (loop->loop_nodes.find(def_block) == loop->loop_nodes.end())
                        {
                            invariant_set.insert(result_reg_num);
                            currloop_motion_count += 1;
                            return true;
                        }
                    }
                    return false;
                }
                else if (rv_inst->op == RV64InstType::MUL || rv_inst->op == RV64InstType::ADD)
                {
                    int rs1_num = rv_inst->rs1.reg_num;
                    int rs2_num = rv_inst->rs2.reg_num;

                    bool rs1_invariant = (invariant_set.find(rs1_num) != invariant_set.end());
                    bool rs2_invariant = (invariant_set.find(rs2_num) != invariant_set.end());

                    if (!rs1_invariant)
                    {
                        auto def_it = inst_def_map.find(rs1_num);
                        if (def_it != inst_def_map.end())
                        {
                            auto* def_block = def_it->second;
                            if (loop->loop_nodes.find(def_block) == loop->loop_nodes.end()) { rs1_invariant = true; }
                        }
                    }

                    if (!rs2_invariant)
                    {
                        auto def_it = inst_def_map.find(rs2_num);
                        if (def_it != inst_def_map.end())
                        {
                            auto* def_block = def_it->second;
                            if (loop->loop_nodes.find(def_block) == loop->loop_nodes.end()) { rs2_invariant = true; }
                        }
                    }

                    if (rs1_invariant && rs2_invariant)
                    {
                        invariant_set.insert(result_reg_num);
                        currloop_motion_count += 1;
                        return true;
                    }
                    return false;
                }
                else { return false; }
            }

            default: return false;
        }
    }

    void LICMPass::getInvariantInCurrLoop()
    {
        invariant_inst_list.clear();
        invariant_inst_set.clear();
        invariant_set.clear();

        std::set<Instruction*> ins_visited;

        int change_flag = 1;
        while (change_flag)
        {
            change_flag = 0;
            for (auto* block : curr_loop->loop_nodes)
            {
                for (auto* inst : block->insts)
                {
                    if (ins_visited.find(inst) == ins_visited.end() && isInvariant(current_func->cfg, inst, curr_loop))
                    {
                        change_flag = 1;
                        ins_visited.insert(inst);
                        invariant_inst_list.push_back(inst);
                        invariant_inst_set.insert(inst);
                    }
                }
            }
        }

#ifdef DBG_MODE
        std::cerr << "\nheader" << curr_loop->header->label_num << " InvariantInstList:\n";
        for (auto* inst : invariant_inst_list) { std::cerr << getLICMInstInfo(inst) << "\n"; }
#endif
    }

    void LICMPass::addPreheader() { curr_loop->addPreheader(current_func->cfg, current_func); }

    void LICMPass::licmInCurrLoop()
    {
        currloop_motion_count = 0;
        max_motion_count      = getMaxInstMotionNumber(curr_loop);
        getInvariantInCurrLoop();

        if (invariant_inst_list.empty()) { return; }

        addPreheader();

#ifdef DBG_MODE
        std::cerr << "add preheader " << curr_loop->preheader->label_num << "\n";
#endif

        for (auto* block : curr_loop->loop_nodes)
        {
            auto it = block->insts.begin();
            while (it != block->insts.end())
            {
                if (invariant_inst_set.find(*it) != invariant_inst_set.end()) { it = block->insts.erase(it); }
                else { ++it; }
            }
        }

        for (auto* inst : invariant_inst_list)
        {
            if (!curr_loop->preheader->insts.empty())
            {
                auto insert_pos = curr_loop->preheader->insts.end();
                --insert_pos;
                curr_loop->preheader->insts.insert(insert_pos, inst);
            }
            else { curr_loop->preheader->insts.push_back(inst); }

            auto write_regs = inst->getWriteRegs();
            if (!write_regs.empty() && write_regs[0]->is_virtual)
            {
                inst_def_map[write_regs[0]->reg_num] = curr_loop->preheader;
            }
        }
    }

    int LICMPass::getMaxInstMotionNumber(NaturalLoop* loop)
    {
        int inst_number = 0;
        int bb_number   = loop->loop_nodes.size();
        int ans         = 6;

        for (auto* block : loop->loop_nodes) { inst_number += block->insts.size(); }

        if (current_func->cfg->loop_forest->loopG[loop->loop_id].empty()) { ans += 6; }

        if (inst_number > 100) { ans -= 1; }

        if (bb_number > 3 && bb_number < 10) { ans -= 1; }

        if (bb_number >= 10) { ans -= 2; }

        return ans;
    }

    std::string LICMPass::getLICMInstInfo(Instruction* inst)
    {
        if (!inst) return "";

        switch (inst->inst_type)
        {
            case InstType::MOVE:
            {
                auto* move_inst  = static_cast<MoveInst*>(inst);
                auto  write_regs = inst->getWriteRegs();
                auto  read_regs  = inst->getReadRegs();

                if (!write_regs.empty() && !read_regs.empty())
                {
                    return "%" + std::to_string(write_regs[0]->reg_num) + " = MOVE %" +
                           std::to_string(read_regs[0]->reg_num);
                }
                return "MOVE instruction";
            }

            case InstType::RV64:
            {
                auto* rv_inst = static_cast<RV64Inst*>(inst);
                switch (rv_inst->op)
                {
                    case RV64InstType::ADD:
                        return "%" + std::to_string(rv_inst->rd.reg_num) + " = %" +
                               std::to_string(rv_inst->rs1.reg_num) + " + %" + std::to_string(rv_inst->rs2.reg_num);
                    case RV64InstType::ADDI:
                        return "%" + std::to_string(rv_inst->rd.reg_num) + " = %" +
                               std::to_string(rv_inst->rs1.reg_num) + " + " + std::to_string(rv_inst->imme);
                    case RV64InstType::MUL:
                        return "%" + std::to_string(rv_inst->rd.reg_num) + " = %" +
                               std::to_string(rv_inst->rs1.reg_num) + " * %" + std::to_string(rv_inst->rs2.reg_num);
                    case RV64InstType::LUI:
                        return "%" + std::to_string(rv_inst->rd.reg_num) + " = lui " + std::to_string(rv_inst->imme);
                    default: return "RV64 instruction";
                }
            }

            default: return "Unknown instruction";
        }
    }

}  // namespace Backend::RV64::Passes::Optimize
