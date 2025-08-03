#include <backend/rv64/passes/optimize/rv64_cse.h>
#include <backend/rv64/insts.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>

namespace Backend::RV64::Passes::Optimize
{
    RV64CSEPass::RV64CSEPass(std::vector<Backend::RV64::Function*>& functions) : functions_(functions) {}

    bool RV64CSEPass::run()
    {
        bool changed = false;
        for (auto* func : functions_)
        {
            optimizeFunction(func);
            changed = true;
        }
        return changed;
    }

    void RV64CSEPass::optimizeFunction(Backend::RV64::Function* func)
    {
        bool is_changed = true;

        while (is_changed)
        {
            is_changed = false;
            erase_set.clear();
            reg_replace_map.clear();

            domTreeWalkCSE(func);

            if (!erase_set.empty() || !reg_replace_map.empty())
            {
                is_changed = true;

                applyRegisterReplacement(func);
                eraseInstructions(func);
                eliminateDeadCode(func);
            }
        }
    }

    void RV64CSEPass::domTreeWalkCSE(Backend::RV64::Function* func)
    {
        inst_cse_map.clear();
        erase_set.clear();
        reg_replace_map.clear();

        dfsDomTree(func, 0);
    }

    void RV64CSEPass::dfsDomTree(Backend::RV64::Function* func, int block_id)
    {
        if (func->cfg->blocks.find(block_id) == func->cfg->blocks.end()) return;

        Backend::RV64::Block*                 block = func->cfg->blocks[block_id];
        std::set<Backend::RV64::Instruction*> tmp_cse_set;

        for (auto inst : block->insts)
        {
            if (!isCSECandidate(inst)) continue;

            RV64InstCSEInfo info      = getCSEInfo(inst);
            bool            found_cse = false;

            for (auto* old_inst : inst_cse_map)
            {
                if (isSameInstruction(inst, old_inst))
                {
                    auto write_regs     = inst->getWriteRegs();
                    auto old_write_regs = old_inst->getWriteRegs();

                    if (!write_regs.empty() && !old_write_regs.empty())
                    {
                        reg_replace_map[write_regs[0]->reg_num] = old_write_regs[0]->reg_num;
                        erase_set.insert(inst);
                        found_cse = true;
                        break;
                    }
                }
            }

            if (!found_cse)
            {
                tmp_cse_set.insert(inst);
                inst_cse_map.insert(inst);
            }
        }

        if (func->cfg->dom_tree && !func->cfg->dom_tree->dom_tree.empty())
        {
            if (block_id < (int)func->cfg->dom_tree->dom_tree.size())
            {
                for (int child_id : func->cfg->dom_tree->dom_tree[block_id])
                {
                    if (child_id != block_id) { dfsDomTree(func, child_id); }
                }
            }
        }

        for (auto* inst : tmp_cse_set) { inst_cse_map.erase(inst); }
    }

    bool RV64CSEPass::isCSECandidate(Backend::RV64::Instruction* inst)
    {
        if (hasPhysicalReg(inst)) return false;

        if (inst->inst_type == Backend::RV64::InstType::MOVE) { return true; }

        if (inst->inst_type == Backend::RV64::InstType::RV64)
        {
            Backend::RV64::RV64Inst* rv_inst = static_cast<Backend::RV64::RV64Inst*>(inst);

            switch (rv_inst->op)
            {
                case Backend::RV64::RV64InstType::LUI:
                case Backend::RV64::RV64InstType::ADDI:
                case Backend::RV64::RV64InstType::ADD:
                case Backend::RV64::RV64InstType::MUL:
                case Backend::RV64::RV64InstType::ADDW: return true;
                default: return false;
            }
        }

        return false;
    }

    bool RV64CSEPass::hasPhysicalReg(Backend::RV64::Instruction* inst)
    {
        auto read_regs  = inst->getReadRegs();
        auto write_regs = inst->getWriteRegs();

        for (auto* reg : read_regs)
        {
            if (!reg->is_virtual) return true;
        }

        for (auto* reg : write_regs)
        {
            if (!reg->is_virtual) return true;
        }

        return false;
    }

    bool RV64CSEPass::isSameInstruction(Backend::RV64::Instruction* inst1, Backend::RV64::Instruction* inst2)
    {
        if (inst1->inst_type != inst2->inst_type) return false;

        if (inst1->inst_type == Backend::RV64::InstType::MOVE)
        {
            Backend::RV64::MoveInst* move1 = static_cast<Backend::RV64::MoveInst*>(inst1);
            Backend::RV64::MoveInst* move2 = static_cast<Backend::RV64::MoveInst*>(inst2);

            if (move1->src->operand_type != move2->src->operand_type) return false;
            return (move1->src->to_string() == move2->src->to_string() && move1->data_type == move2->data_type);
        }

        if (inst1->inst_type == Backend::RV64::InstType::RV64)
        {
            Backend::RV64::RV64Inst* rv1 = static_cast<Backend::RV64::RV64Inst*>(inst1);
            Backend::RV64::RV64Inst* rv2 = static_cast<Backend::RV64::RV64Inst*>(inst2);

            if (rv1->op != rv2->op) return false;

            switch (rv1->op)
            {
                case Backend::RV64::RV64InstType::ADD:
                case Backend::RV64::RV64InstType::MUL:
                case Backend::RV64::RV64InstType::ADDW:
                    return (
                        (rv1->rs1.reg_num == rv2->rs1.reg_num && rv1->rs2.reg_num == rv2->rs2.reg_num &&
                            rv1->rs1.is_virtual == rv2->rs1.is_virtual && rv1->rs2.is_virtual == rv2->rs2.is_virtual) ||
                        (rv1->rs1.reg_num == rv2->rs2.reg_num && rv1->rs2.reg_num == rv2->rs1.reg_num &&
                            rv1->rs1.is_virtual == rv2->rs2.is_virtual && rv1->rs2.is_virtual == rv2->rs1.is_virtual));

                case Backend::RV64::RV64InstType::ADDI:
                    if (rv1->use_label != rv2->use_label) return false;
                    if (rv1->use_label)
                    {
                        return (rv1->rs1.reg_num == rv2->rs1.reg_num && rv1->rs1.is_virtual == rv2->rs1.is_virtual &&
                                rv1->label.name == rv2->label.name);
                    }
                    else
                    {
                        return (rv1->rs1.reg_num == rv2->rs1.reg_num && rv1->rs1.is_virtual == rv2->rs1.is_virtual &&
                                rv1->imme == rv2->imme);
                    }

                case Backend::RV64::RV64InstType::LUI:
                    if (rv1->use_label != rv2->use_label) return false;
                    if (rv1->use_label) { return rv1->label.name == rv2->label.name; }
                    else { return rv1->imme == rv2->imme; }

                default: return false;
            }
        }

        return false;
    }

    RV64InstCSEInfo RV64CSEPass::getCSEInfo(Backend::RV64::Instruction* inst)
    {
        RV64InstCSEInfo info;
        info.inst_type = inst->inst_type;

        if (inst->inst_type == Backend::RV64::InstType::MOVE)
        {
            Backend::RV64::MoveInst* move_inst = static_cast<Backend::RV64::MoveInst*>(inst);
            info.operand_list.push_back("MOVE");
            info.operand_list.push_back(move_inst->src->to_string());
        }
        else if (inst->inst_type == Backend::RV64::InstType::RV64)
        {
            Backend::RV64::RV64Inst* rv_inst = static_cast<Backend::RV64::RV64Inst*>(inst);
            info.rv64_opcode                 = rv_inst->op;

            switch (rv_inst->op)
            {
                case Backend::RV64::RV64InstType::ADD:
                    info.operand_list.push_back("ADD");
                    info.operand_list.push_back(registerToString(rv_inst->rs1));
                    info.operand_list.push_back(registerToString(rv_inst->rs2));
                    break;
                case Backend::RV64::RV64InstType::ADDW:
                    info.operand_list.push_back("ADDW");
                    info.operand_list.push_back(registerToString(rv_inst->rs1));
                    info.operand_list.push_back(registerToString(rv_inst->rs2));
                    break;
                case Backend::RV64::RV64InstType::ADDI:
                    info.operand_list.push_back("ADDI");
                    info.operand_list.push_back(registerToString(rv_inst->rs1));
                    if (rv_inst->use_label)
                        info.operand_list.push_back(rv_inst->label.name);
                    else
                        info.operand_list.push_back(std::to_string(rv_inst->imme));
                    break;
                default: info.operand_list.push_back("OTHER"); break;
            }
        }

        return info;
    }

    void RV64CSEPass::applyRegisterReplacement(Backend::RV64::Function* func)
    {
        if (reg_replace_map.empty()) return;

        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto inst : block->insts) { inst->replaceAllOperands(reg_replace_map); }
        }
    }

    void RV64CSEPass::eraseInstructions(Backend::RV64::Function* func)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            block->insts.erase(std::remove_if(block->insts.begin(),
                                   block->insts.end(),
                                   [this](Backend::RV64::Instruction* inst) { return erase_set.count(inst) > 0; }),
                block->insts.end());
        }
    }

    void RV64CSEPass::eliminateDeadCode(Backend::RV64::Function* func)
    {
        std::map<int, int> vreg_refcnt;

        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto inst : block->insts)
            {
                auto read_regs = inst->getReadRegs();
                for (auto* reg : read_regs)
                {
                    if (reg->is_virtual) { vreg_refcnt[reg->reg_num]++; }
                }
            }
        }

        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end();)
            {
                auto inst       = *it;
                auto write_regs = inst->getWriteRegs();

                if (write_regs.size() == 1)
                {
                    auto* rd = write_regs[0];
                    if (rd->is_virtual && vreg_refcnt[rd->reg_num] == 0)
                    {
                        it = block->insts.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }
    }

    std::string RV64CSEPass::registerToString(Backend::RV64::Register reg)
    {
        std::stringstream ss;
        if (reg.is_virtual) { ss << "v" << reg.reg_num; }
        else { ss << "r" << reg.reg_num; }
        return ss.str();
    }

}  // namespace Backend::RV64::Passes::Optimize
