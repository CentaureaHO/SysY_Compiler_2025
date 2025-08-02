#include "llvm/loop/loop_rotate.h"
#include "llvm_ir/ir_builder.h"
#include <iostream>
#include <algorithm>

namespace StructuralTransform
{
    LoopRotatePass::LoopRotatePass(LLVMIR::IR* ir) : Pass(ir) {}

    void LoopRotatePass::Execute()
    {
        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (!cfg || !cfg->LoopForest) continue;

            std::vector<NaturalLoop*> eligible_loops;
            for (auto* loop : cfg->LoopForest->loop_set)
                if (canRotate(loop)) eligible_loops.push_back(loop);

            for (auto* loop : eligible_loops)
            {
                std::map<int, int> value_map;
                auto*              guard_block = createGuardBlock(cfg, loop, value_map);
                guard_block->comment           = "LoopRotateGuardBlock for loop " + std::to_string(loop->loop_id);
                restructureLoop(cfg, loop, guard_block, value_map);

                // std::cout << "LoopRotate applied to loop " << loop->loop_id << " (header: " << loop->header->block_id
                //           << ")" << std::endl;
            }

            cfg->BuildCFG();
            for (const auto& [id, bb] : cfg->block_id_to_block)
                for (auto* inst : bb->insts) inst->block_id = bb->block_id;
        }
    }

    bool LoopRotatePass::canRotate(NaturalLoop* loop)
    {
        if (!loop)
        {
            // std::cout << "LoopRotatePass: loop is nullptr" << std::endl;
            return false;
        }

        if (loop->exit_nodes.size() != 1 || loop->latches.size() != 1 || loop->loop_nodes.size() <= 1)
        {
            // std::cout << "LoopRotatePass: not single exit | not single latch | multiple nodes" << std::endl;
            return false;
        }

        if (loop->exiting_nodes.find(loop->header) == loop->exiting_nodes.end())
        {
            // std::cout << "LoopRotatePass: header is not exiting node" << std::endl;
            return false;
        }

        /*
        if (loop->exiting_nodes.size() != 1)
        {
            std::cout << "LoopRotatePass: multiple exiting nodes" << std::endl;
            return false;
        }
        */

        auto* latch = *loop->latches.begin();
        if (latch->insts.empty()) return false;

        auto* last_inst         = latch->insts.back();
        bool  has_uncond_branch = (last_inst->opcode == LLVMIR::IROpCode::BR_UNCOND);
        return has_uncond_branch;
    }

    std::pair<std::set<LLVMIR::Instruction*>, bool> LoopRotatePass::checkHeaderUse(
        CFG* cfg, NaturalLoop* loop, int result_reg)
    {
        std::set<LLVMIR::Instruction*> body_uses;
        auto*                          latch = *loop->latches.begin();

        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
            for (const auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                if (!(label_op && label_op->label_num == latch->block_id && val->type == LLVMIR::OperandType::REG))
                    continue;

                auto* reg_val = dynamic_cast<LLVMIR::RegOperand*>(val);
                if (reg_val && reg_val->reg_num == result_reg) return {body_uses, false};
            }
        }

        // for (auto* bb : loop->loop_nodes)
        for (auto& [id, bb] : cfg->block_id_to_block)
        {
            if (bb == loop->header) continue;

            for (auto* inst : bb->insts)
            {
                auto used_regs = inst->GetUsedRegs();
                if (std::find(used_regs.begin(), used_regs.end(), result_reg) == used_regs.end()) continue;

                body_uses.insert(inst);
            }
        }

        return {body_uses, true};
    }

    void LoopRotatePass::resolveConflict(CFG* cfg, NaturalLoop* loop, LLVMIR::Instruction* header_def, int new_reg,
        std::set<LLVMIR::Instruction*> body_uses, const std::map<int, int>& value_map)
    {
        if (header_def->GetResultReg() == -1) return;

        int old_reg = header_def->GetResultReg();

        LLVMIR::DataType phi_type       = header_def->GetResultType();
        auto*            resolution_phi = new LLVMIR::PhiInst(phi_type, getRegOperand(++cfg->func->max_reg));

        int  guard_reg = old_reg;
        auto it        = value_map.find(old_reg);
        if (it != value_map.end()) { guard_reg = it->second; }

        resolution_phi->Insert_into_PHI(getRegOperand(guard_reg), getLabelOperand(loop->guard->block_id));
        resolution_phi->Insert_into_PHI(getRegOperand(new_reg), getLabelOperand((*loop->latches.begin())->block_id));

        loop->header->insts.insert(loop->header->insts.begin(), resolution_phi);

        std::map<int, int> reg_mapping{{old_reg, resolution_phi->GetResultReg()}};
        for (auto* use_inst : body_uses) use_inst->Rename(reg_mapping);
    }

    LLVMIR::IRBlock* LoopRotatePass::createGuardBlock(CFG* cfg, NaturalLoop* loop, std::map<int, int>& value_map)
    {
        auto* guard_block                             = cfg->func->createBlock();
        cfg->block_id_to_block[guard_block->block_id] = guard_block;
        loop->guard                                   = guard_block;

        std::map<int, int> reg_remapping;

        for (auto* original_inst : loop->header->insts)
        {
            LLVMIR::Instruction* cloned_inst = nullptr;

            if (original_inst->opcode == LLVMIR::IROpCode::PHI)
            {
                auto* original_phi = dynamic_cast<LLVMIR::PhiInst*>(original_inst);
                auto* new_phi      = new LLVMIR::PhiInst(original_phi->type, getRegOperand(++cfg->func->max_reg));

                for (const auto& [val, label] : original_phi->vals_for_labels)
                {
                    auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                    if (label_op && label_op->label_num == loop->preheader->block_id)
                    {
                        new_phi->Insert_into_PHI(val, label);
                        break;
                    }
                }

                reg_remapping[original_inst->GetResultReg()] = new_phi->GetResultReg();
                cloned_inst                                  = new_phi;
            }
            else if (original_inst->opcode != LLVMIR::IROpCode::BR_COND &&
                     original_inst->opcode != LLVMIR::IROpCode::BR_UNCOND &&
                     original_inst->opcode != LLVMIR::IROpCode::RET)
            {
                if (original_inst->GetResultReg() != -1)
                    reg_remapping[original_inst->GetResultReg()] = ++cfg->func->max_reg;

                cloned_inst = original_inst->Clone(reg_remapping[original_inst->GetResultReg()]);
                if (cloned_inst) cloned_inst->Rename(reg_remapping);
            }
            else if (original_inst->opcode == LLVMIR::IROpCode::BR_COND)
            {
                cloned_inst = original_inst->Clone(-1);
                if (cloned_inst)
                {
                    cloned_inst->Rename(reg_remapping);

                    auto* br_cond     = dynamic_cast<LLVMIR::BranchCondInst*>(cloned_inst);
                    auto* exit_block  = *loop->exit_nodes.begin();
                    auto* true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
                    auto* false_label = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

                    if (true_label && true_label->label_num == exit_block->block_id)
                        br_cond->false_label = getLabelOperand(loop->header->block_id);
                    else if (false_label && false_label->label_num == exit_block->block_id)
                        br_cond->true_label = getLabelOperand(loop->header->block_id);
                }
            }

            if (cloned_inst) guard_block->insts.push_back(cloned_inst);
        }

        value_map = reg_remapping;
        return guard_block;
    }

    void LoopRotatePass::restructureLoop(
        CFG* cfg, NaturalLoop* loop, LLVMIR::IRBlock* guard_block, const std::map<int, int>& value_map)
    {
        auto* exit_block = *loop->exit_nodes.begin();
        auto* latch      = *loop->latches.begin();

        // preheader -> guard_block
        auto* preheader_term = loop->preheader->insts.back();
        if (preheader_term->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br_cond     = dynamic_cast<LLVMIR::BranchCondInst*>(preheader_term);
            auto* true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
            auto* false_label = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

            if (true_label && true_label->label_num == loop->header->block_id)
                br_cond->true_label = getLabelOperand(guard_block->block_id);
            if (false_label && false_label->label_num == loop->header->block_id)
                br_cond->false_label = getLabelOperand(guard_block->block_id);
        }
        else if (preheader_term->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* br_uncond         = dynamic_cast<LLVMIR::BranchUncondInst*>(preheader_term);
            br_uncond->target_label = getLabelOperand(guard_block->block_id);
        }

        for (auto* inst : exit_block->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto*            phi_inst     = dynamic_cast<LLVMIR::PhiInst*>(inst);
            LLVMIR::Operand* header_value = nullptr;

            for (const auto& [val, label] : phi_inst->vals_for_labels)
            {
                auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                if (label_op && label_op->label_num == loop->header->block_id)
                {
                    header_value = val;
                    break;
                }
            }

            if (!header_value) continue;

            bool resolved = false;
            for (auto* header_inst : loop->header->insts)
            {
                if (header_inst->opcode != LLVMIR::IROpCode::PHI) break;

                auto* header_phi     = dynamic_cast<LLVMIR::PhiInst*>(header_inst);
                auto* header_result  = dynamic_cast<LLVMIR::RegOperand*>(header_phi->res);
                auto* header_val_reg = dynamic_cast<LLVMIR::RegOperand*>(header_value);

                if (!(header_result && header_val_reg && header_result->reg_num == header_val_reg->reg_num)) continue;

                for (const auto& [val, label] : header_phi->vals_for_labels)
                {
                    auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                    if (label_op && label_op->label_num != loop->preheader->block_id) continue;

                    phi_inst->Insert_into_PHI(val, getLabelOperand(guard_block->block_id));
                    resolved = true;
                    break;
                }
                break;
            }

            if (!resolved) phi_inst->Insert_into_PHI(header_value, getLabelOperand(guard_block->block_id));
        }

        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* phi_inst = dynamic_cast<LLVMIR::PhiInst*>(inst);
            for (auto& [val, label] : phi_inst->vals_for_labels)
            {
                auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                if (label_op && label_op->label_num != loop->preheader->block_id) continue;

                label = getLabelOperand(guard_block->block_id);
                break;
            }
        }

        std::set<LLVMIR::Instruction*>                                 problematic_defs;
        std::map<LLVMIR::Instruction*, std::set<LLVMIR::Instruction*>> def_use_map;

        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI || inst->GetResultReg() == -1) continue;

            auto [body_uses, can_proceed] = checkHeaderUse(cfg, loop, inst->GetResultReg());
            if (!can_proceed) continue;

            if (!body_uses.empty())
            {
                problematic_defs.insert(inst);
                def_use_map[inst] = body_uses;
            }
        }

        if (!latch->insts.empty() && (latch->insts.back()->opcode == LLVMIR::IROpCode::BR_UNCOND ||
                                         latch->insts.back()->opcode == LLVMIR::IROpCode::BR_COND))
        {
            delete latch->insts.back();
            latch->insts.pop_back();
        }

        std::map<int, LLVMIR::Operand*> value_substitutions;
        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
            for (const auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                if (label_op && label_op->label_num == latch->block_id && val->type == LLVMIR::OperandType::REG)
                {
                    value_substitutions[phi->GetResultReg()] = val;
                    break;
                }
            }
        }

        std::map<int, int> latch_reg_mapping;
        LLVMIR::IRBlock*   loop_body_entry = nullptr;

        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI)
            {
                auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
                for (auto& [val, label] : phi->vals_for_labels)
                {
                    auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                    if (label_op && (label_op->label_num == loop->preheader->block_id ||
                                        label_op->label_num == guard_block->block_id))
                    {
                        if (val->type == LLVMIR::OperandType::REG)
                        {
                            auto* reg_val = dynamic_cast<LLVMIR::RegOperand*>(val);
                            if (reg_val)
                            {
                                auto it = value_map.find(reg_val->reg_num);
                                if (it != value_map.end()) { val = getRegOperand(it->second); }
                            }
                        }

                        label = getLabelOperand(guard_block->block_id);
                        break;
                    }
                }
            }
        }

        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI) { continue; }
            else if (inst->opcode != LLVMIR::IROpCode::BR_COND && inst->opcode != LLVMIR::IROpCode::BR_UNCOND &&
                     inst->opcode != LLVMIR::IROpCode::RET)
            {
                if (inst->GetResultReg() != -1) { latch_reg_mapping[inst->GetResultReg()] = ++cfg->func->max_reg; }

                auto* latch_inst = inst->Clone(latch_reg_mapping[inst->GetResultReg()]);
                if (latch_inst)
                {
                    latch_inst->Rename(latch_reg_mapping);
                    latch_inst->SubstituteOperands(value_substitutions);
                    latch->insts.push_back(latch_inst);
                }
            }
            else if (inst->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* latch_br = inst->Clone(-1);
                if (latch_br)
                {
                    latch_br->Rename(latch_reg_mapping);
                    latch_br->SubstituteOperands(value_substitutions);

                    auto* br_cond     = dynamic_cast<LLVMIR::BranchCondInst*>(latch_br);
                    auto* true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
                    auto* false_label = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

                    if (true_label && true_label->label_num == exit_block->block_id)
                    {
                        loop_body_entry      = cfg->block_id_to_block[false_label->label_num];
                        br_cond->false_label = getLabelOperand(loop->header->block_id);
                    }
                    else if (false_label && false_label->label_num == exit_block->block_id)
                    {
                        loop_body_entry     = cfg->block_id_to_block[true_label->label_num];
                        br_cond->true_label = getLabelOperand(loop->header->block_id);
                    }

                    latch->insts.push_back(latch_br);

                    if (loop_body_entry)
                    {
                        for (auto it = loop->header->insts.begin(); it != loop->header->insts.end(); ++it)
                        {
                            if (*it == inst)
                            {
                                delete *it;
                                *it = new LLVMIR::BranchUncondInst(getLabelOperand(loop_body_entry->block_id));
                                break;
                            }
                        }
                    }
                }

                for (auto* def_inst : problematic_defs)
                {
                    auto it = def_use_map.find(def_inst);
                    if (it != def_use_map.end())
                    {
                        resolveConflict(
                            cfg, loop, def_inst, latch_reg_mapping[def_inst->GetResultReg()], it->second, value_map);
                    }
                }
                break;
            }
        }

        for (auto* inst : exit_block->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
            for (auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                if (label_op && label_op->label_num == loop->header->block_id)
                {
                    label = getLabelOperand(latch->block_id);

                    if (val->type == LLVMIR::OperandType::REG)
                    {
                        auto* reg_val = dynamic_cast<LLVMIR::RegOperand*>(val);
                        int   reg_num = reg_val->reg_num;

                        bool updated = false;
                        for (auto* header_inst : loop->header->insts)
                        {
                            if (header_inst->opcode != LLVMIR::IROpCode::PHI) break;

                            auto* header_phi = dynamic_cast<LLVMIR::PhiInst*>(header_inst);
                            if (header_phi->GetResultReg() == reg_num)
                            {
                                for (const auto& [phi_val, phi_label] : header_phi->vals_for_labels)
                                {
                                    auto* phi_label_op = dynamic_cast<LLVMIR::LabelOperand*>(phi_label);
                                    if (phi_label_op && phi_label_op->label_num == latch->block_id)
                                    {
                                        val     = phi_val;
                                        updated = true;
                                        break;
                                    }
                                }
                                break;
                            }
                        }

                        if (!updated && latch_reg_mapping.find(reg_num) != latch_reg_mapping.end())
                        {
                            val = getRegOperand(latch_reg_mapping[reg_num]);
                        }
                    }
                    break;
                }
            }
        }

        auto it = loop->header->insts.begin();
        while (it != loop->header->insts.end())
        {
            auto* inst = *it;
            if (inst->opcode != LLVMIR::IROpCode::PHI && inst->opcode != LLVMIR::IROpCode::BR_COND &&
                inst->opcode != LLVMIR::IROpCode::BR_UNCOND && inst->opcode != LLVMIR::IROpCode::RET)
            {
                delete *it;
                it = loop->header->insts.erase(it);
            }
            else { ++it; }
        }

        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            for (auto* inst : bb->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::PHI) break;

                auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
                if (!phi) continue;

                for (auto& [val, label] : phi->vals_for_labels)
                {
                    auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                    if (label_op && label_op->label_num != guard_block->block_id) continue;

                    if (val->type != LLVMIR::OperandType::REG) continue;

                    auto* reg_val = dynamic_cast<LLVMIR::RegOperand*>(val);
                    if (!reg_val) continue;

                    for (auto* header_inst : loop->header->insts)
                    {
                        if (header_inst->opcode != LLVMIR::IROpCode::PHI) break;

                        auto* header_phi = dynamic_cast<LLVMIR::PhiInst*>(header_inst);
                        if (header_phi && header_phi->GetResultReg() != reg_val->reg_num) continue;

                        for (const auto& [h_val, h_label] : header_phi->vals_for_labels)
                        {
                            auto* h_label_op = dynamic_cast<LLVMIR::LabelOperand*>(h_label);
                            if (h_label_op && h_label_op->label_num != guard_block->block_id) continue;

                            val = h_val;
                            break;
                        }
                        break;
                    }
                }
            }
        }
    }

}  // namespace StructuralTransform
