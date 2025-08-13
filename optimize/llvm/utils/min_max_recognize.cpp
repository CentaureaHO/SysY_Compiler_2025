#include "min_max_recognize.h"
#include <llvm_ir/defs.h>
#include <llvm_ir/instruction.h>
#include <llvm_ir/ir_block.h>
#include <iostream>
#include <map>
#include <algorithm>

namespace Transform
{
    MinMaxRecognizePass::MinMaxRecognizePass(LLVMIR::IR* ir) : Pass(ir), modified(false) {}

    void MinMaxRecognizePass::Execute()
    {
        modified = false;

        for (auto& [func_def, cfg] : ir->cfg)
        {
            std::map<int, LLVMIR::Instruction*> define_map;

            for (auto& [id, block] : cfg->block_id_to_block)
            {
                for (auto& inst : block->insts)
                {
                    if (inst->GetResultReg() != -1) { define_map[inst->GetResultReg()] = inst; }
                }
            }

            for (auto& [id, block] : cfg->block_id_to_block)
            {
                for (auto& inst : block->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::PHI)
                    {
                        auto phi_inst = dynamic_cast<LLVMIR::PhiInst*>(inst);
                        if (recognizePhiMinMax(phi_inst, block, define_map)) { modified = true; }
                    }
                    else if (inst->opcode == LLVMIR::IROpCode::SELECT)
                    {
                        auto select_inst = dynamic_cast<LLVMIR::SelectInst*>(inst);
                        if (recognizeSelectMinMax(select_inst, block)) { modified = true; }
                    }
                    if (inst->opcode == LLVMIR::IROpCode::BR_COND)
                    {
                        auto br_inst = dynamic_cast<LLVMIR::BranchCondInst*>(inst);
                        if (recognizeArrayMinMax(br_inst, block, define_map)) { modified = true; }
                    }
                }
            }
        }
    }

    bool MinMaxRecognizePass::recognizePhiMinMax(
        LLVMIR::PhiInst* phi_inst, LLVMIR::IRBlock* block, std::map<int, LLVMIR::Instruction*>& define_map)
    {
        if (!phi_inst || phi_inst->vals_for_labels.size() != 2) return false;

        if (phi_inst->type != LLVMIR::DataType::I32 && phi_inst->type != LLVMIR::DataType::F32) return false;

        CFG* cfg = nullptr;
        for (auto& [func_def, c] : ir->cfg)
        {
            for (auto& [id, b] : c->block_id_to_block)
            {
                if (b == block)
                {
                    cfg = c;
                    break;
                }
            }
            if (cfg) break;
        }
        if (!cfg) return false;

        auto& pair1 = phi_inst->vals_for_labels[0];
        auto& pair2 = phi_inst->vals_for_labels[1];

        auto val1   = pair1.first;
        auto val2   = pair2.first;
        auto label1 = pair1.second;
        auto label2 = pair2.second;

        if (label1->type != LLVMIR::OperandType::LABEL || label2->type != LLVMIR::OperandType::LABEL) return false;

        auto label1_op = dynamic_cast<LLVMIR::LabelOperand*>(label1);
        auto label2_op = dynamic_cast<LLVMIR::LabelOperand*>(label2);
        if (!label1_op || !label2_op) return false;

        int label1_id = label1_op->label_num;
        int label2_id = label2_op->label_num;

        auto bb1 = cfg->block_id_to_block[label1_id];
        auto bb2 = cfg->block_id_to_block[label2_id];
        if (!bb1 || !bb2) return false;

        auto endI1 = bb1->insts.empty() ? nullptr : bb1->insts.back();
        auto endI2 = bb2->insts.empty() ? nullptr : bb2->insts.back();
        if (!endI1 || !endI2) return false;

        if (endI2->opcode == LLVMIR::IROpCode::BR_COND && endI1->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            std::swap(endI1, endI2);
            std::swap(label1_op, label2_op);
            std::swap(val1, val2);
            std::swap(bb1, bb2);
            std::swap(label1_id, label2_id);
        }

        if (endI1->opcode != LLVMIR::IROpCode::BR_COND || endI2->opcode != LLVMIR::IROpCode::BR_UNCOND) return false;

        auto br_cond = dynamic_cast<LLVMIR::BranchCondInst*>(endI1);
        if (!br_cond || br_cond->cond->type != LLVMIR::OperandType::REG) return false;

        int  cond_reg = br_cond->cond->GetRegNum();
        auto cmp_inst = define_map[cond_reg];
        if (!cmp_inst) return false;

        if (phi_inst->type == LLVMIR::DataType::I32 && cmp_inst->opcode == LLVMIR::IROpCode::ICMP)
        {
            auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(cmp_inst);
            if (!icmp_inst) return false;

            bool is_min    = true;
            bool is_signed = true;
            auto cmp_cond  = icmp_inst->cond;

            switch (cmp_cond)
            {
                case LLVMIR::IcmpCond::SLT:
                case LLVMIR::IcmpCond::SLE:
                    is_min    = true;
                    is_signed = true;
                    break;
                case LLVMIR::IcmpCond::SGT:
                case LLVMIR::IcmpCond::SGE:
                    is_min    = false;
                    is_signed = true;
                    break;
                case LLVMIR::IcmpCond::ULT:
                case LLVMIR::IcmpCond::ULE:
                    is_min    = true;
                    is_signed = false;
                    break;
                case LLVMIR::IcmpCond::UGT:
                case LLVMIR::IcmpCond::UGE:
                    is_min    = false;
                    is_signed = false;
                    break;
                default: return false;
            }

            auto icmp_op1 = icmp_inst->lhs;
            auto icmp_op2 = icmp_inst->rhs;
            auto phi_op1  = val1;
            auto phi_op2  = val2;
            auto phi_l1   = label1_op;
            auto phi_l2   = label2_op;

            auto operand_equals = [&define_map](LLVMIR::Operand* op1, LLVMIR::Operand* op2) -> bool {
                if (op1->type != op2->type) return false;
                switch (op1->type)
                {
                    case LLVMIR::OperandType::REG:
                    {
                        if (op1->GetRegNum() == op2->GetRegNum()) return true;

                        auto inst1 = define_map[op1->GetRegNum()];
                        auto inst2 = define_map[op2->GetRegNum()];
                        if (inst1 && inst2 && inst1->opcode == LLVMIR::IROpCode::LOAD &&
                            inst2->opcode == LLVMIR::IROpCode::LOAD)
                        {
                            auto load1 = dynamic_cast<LLVMIR::LoadInst*>(inst1);
                            auto load2 = dynamic_cast<LLVMIR::LoadInst*>(inst2);
                            if (load1 && load2 && load1->ptr->type == LLVMIR::OperandType::REG &&
                                load2->ptr->type == LLVMIR::OperandType::REG)
                            {
                                return load1->ptr->GetRegNum() == load2->ptr->GetRegNum();
                            }
                        }
                        return false;
                    }
                    case LLVMIR::OperandType::IMMEI32: return op1->GetImm() == op2->GetImm();
                    case LLVMIR::OperandType::IMMEF32: return op1->GetImmF() == op2->GetImmF();
                    case LLVMIR::OperandType::GLOBAL: return op1->GetGlobal() == op2->GetGlobal();
                    default: return false;
                }
            };

            // 确定正确的操作数顺序
            LLVMIR::Operand* final_op1 = nullptr;
            LLVMIR::Operand* final_op2 = nullptr;

            if (operand_equals(icmp_op1, phi_op1) && operand_equals(icmp_op2, phi_op2))
            {
                final_op1 = icmp_op1;
                final_op2 = icmp_op2;
            }
            else if (operand_equals(icmp_op1, phi_op2) && operand_equals(icmp_op2, phi_op1))
            {
                final_op1 = icmp_op1;
                final_op2 = icmp_op2;
                std::swap(phi_l1, phi_l2);
                is_min = !is_min;
            }
            else { return false; }

            auto true_label_op  = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
            auto false_label_op = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->false_label);
            if (!true_label_op || !false_label_op) return false;

            int true_label  = true_label_op->label_num;
            int false_label = false_label_op->label_num;

            if (true_label == bb2->block_id && false_label == block->block_id) { is_min = !is_min; }
            else if (true_label != block->block_id || false_label != bb2->block_id) { return false; }

            int bb2_pred_count = 0;
            for (auto& [id, b] : cfg->block_id_to_block)
            {
                for (auto& inst : b->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::BR_COND)
                    {
                        auto br = dynamic_cast<LLVMIR::BranchCondInst*>(inst);
                        auto tl = dynamic_cast<LLVMIR::LabelOperand*>(br->true_label);
                        auto fl = dynamic_cast<LLVMIR::LabelOperand*>(br->false_label);
                        if ((tl && tl->label_num == bb2->block_id) || (fl && fl->label_num == bb2->block_id))
                            bb2_pred_count++;
                    }
                    else if (inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
                    {
                        auto br = dynamic_cast<LLVMIR::BranchUncondInst*>(inst);
                        auto tl = dynamic_cast<LLVMIR::LabelOperand*>(br->target_label);
                        if (tl && tl->label_num == bb2->block_id) bb2_pred_count++;
                    }
                }
            }
            if (bb2_pred_count > 1) return false;

            if (!((phi_l1->label_num == bb1->block_id && phi_l2->label_num == bb2->block_id) ||
                    (phi_l1->label_num == bb2->block_id && phi_l2->label_num == bb1->block_id)))
                return false;

            LLVMIR::IROpCode minmax_op = is_signed ? (is_min ? LLVMIR::IROpCode::SMIN_I32 : LLVMIR::IROpCode::SMAX_I32)
                                                   : (is_min ? LLVMIR::IROpCode::UMIN_I32 : LLVMIR::IROpCode::UMAX_I32);

            auto minmax_inst =
                new LLVMIR::ArithmeticInst(minmax_op, phi_inst->type, final_op1, final_op2, phi_inst->res);

            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                if (*it == phi_inst)
                {
                    *it = minmax_inst;
                    delete phi_inst;
                    break;
                }
            }

            return true;
        }
        // TODO: 处理浮点数比较 (FCMP)

        return false;
    }

    bool MinMaxRecognizePass::recognizeSelectMinMax(LLVMIR::SelectInst* select_inst, LLVMIR::IRBlock* block)
    {
        if (!select_inst) return false;

        auto cond      = select_inst->cond;
        auto true_val  = select_inst->true_val;
        auto false_val = select_inst->false_val;
        auto result    = select_inst->res;

        if (cond->type != LLVMIR::OperandType::REG) return false;

        CFG* cfg = nullptr;
        for (auto& [func_def, c] : ir->cfg)
        {
            for (auto& [id, b] : c->block_id_to_block)
            {
                if (b == block)
                {
                    cfg = c;
                    break;
                }
            }
            if (cfg) break;
        }

        if (!cfg) return false;

        std::map<int, LLVMIR::Instruction*> define_map;
        for (auto& [id, b] : cfg->block_id_to_block)
        {
            for (auto& inst : b->insts)
            {
                if (inst->GetResultReg() != -1) { define_map[inst->GetResultReg()] = inst; }
            }
        }

        int  cond_reg = cond->GetRegNum();
        auto cmp_inst = define_map[cond_reg];

        if (!cmp_inst) return false;

        if (cmp_inst->opcode == LLVMIR::IROpCode::ICMP)
        {
            auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(cmp_inst);
            if (!icmp_inst) return false;

            return recognizeIntegerSelectMinMax(select_inst, icmp_inst, block, define_map);
        }
        else if (cmp_inst->opcode == LLVMIR::IROpCode::FCMP)
        {
            auto fcmp_inst = dynamic_cast<LLVMIR::FcmpInst*>(cmp_inst);
            if (!fcmp_inst) return false;

            return recognizeFloatSelectMinMax(select_inst, fcmp_inst, block, define_map);
        }

        return false;
    }

    bool MinMaxRecognizePass::recognizeArrayMinMax(
        LLVMIR::BranchCondInst* br_inst, LLVMIR::IRBlock* block, std::map<int, LLVMIR::Instruction*>& define_map)
    {
        if (!br_inst) return false;

        if (br_inst->cond->type != LLVMIR::OperandType::REG) return false;
        int  cond_reg = br_inst->cond->GetRegNum();
        auto cmp_inst = define_map[cond_reg];

        if (!cmp_inst || cmp_inst->opcode != LLVMIR::IROpCode::ICMP) return false;
        auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(cmp_inst);
        if (!icmp_inst) return false;

        bool is_min    = true;
        bool is_signed = true;
        auto cmp_cond  = icmp_inst->cond;

        switch (cmp_cond)
        {
            case LLVMIR::IcmpCond::SLT:
            case LLVMIR::IcmpCond::SLE:
                is_min    = false;
                is_signed = true;
                break;
            case LLVMIR::IcmpCond::SGT:
            case LLVMIR::IcmpCond::SGE:
                is_min    = false;
                is_signed = true;
                break;
            case LLVMIR::IcmpCond::ULT:
            case LLVMIR::IcmpCond::ULE:
                is_min    = false;
                is_signed = false;
                break;
            case LLVMIR::IcmpCond::UGT:
            case LLVMIR::IcmpCond::UGE:
                is_min    = false;
                is_signed = false;
                break;
            default: return false;
        }

        auto icmp_op1 = icmp_inst->lhs;
        auto icmp_op2 = icmp_inst->rhs;

        if (icmp_op1->type != LLVMIR::OperandType::REG) return false;

        CFG* cfg = nullptr;
        for (auto& [func_def, c] : ir->cfg)
        {
            for (auto& [id, b] : c->block_id_to_block)
            {
                if (b == block)
                {
                    cfg = c;
                    break;
                }
            }
            if (cfg) break;
        }
        if (!cfg) return false;

        auto true_label_op  = dynamic_cast<LLVMIR::LabelOperand*>(br_inst->true_label);
        auto false_label_op = dynamic_cast<LLVMIR::LabelOperand*>(br_inst->false_label);
        if (!true_label_op || !false_label_op) return false;

        auto bb1 = cfg->block_id_to_block[true_label_op->label_num];
        auto bb2 = cfg->block_id_to_block[false_label_op->label_num];
        if (!bb1 || !bb2) return false;

        LLVMIR::IRBlock* store_block = nullptr;

        if (bb1->insts.size() == 2 && bb1->insts[0]->opcode == LLVMIR::IROpCode::STORE &&
            bb1->insts[1]->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            store_block = bb1;
        }
        else if (bb2->insts.size() == 2 && bb2->insts[0]->opcode == LLVMIR::IROpCode::STORE &&
                 bb2->insts[1]->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            store_block = bb2;
            is_min      = !is_min;
        }
        else { return false; }

        int store_block_pred_count = 0;
        for (auto& [id, b] : cfg->block_id_to_block)
        {
            for (auto& inst : b->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::BR_COND)
                {
                    auto br          = dynamic_cast<LLVMIR::BranchCondInst*>(inst);
                    auto true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br->true_label);
                    auto false_label = dynamic_cast<LLVMIR::LabelOperand*>(br->false_label);
                    if ((true_label && true_label->label_num == store_block->block_id) ||
                        (false_label && false_label->label_num == store_block->block_id))
                    {
                        store_block_pred_count++;
                    }
                }
                else if (inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
                {
                    auto br    = dynamic_cast<LLVMIR::BranchUncondInst*>(inst);
                    auto label = dynamic_cast<LLVMIR::LabelOperand*>(br->target_label);
                    if (label && label->label_num == store_block->block_id) { store_block_pred_count++; }
                }
            }
        }
        if (store_block_pred_count > 1) return false;

        auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(store_block->insts[0]);
        if (!store_inst) return false;

        auto br_uncond = dynamic_cast<LLVMIR::BranchUncondInst*>(store_block->insts[1]);
        if (!br_uncond) return false;

        auto br_uncond_label = dynamic_cast<LLVMIR::LabelOperand*>(br_uncond->target_label);
        auto br_false_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_inst->false_label);
        if (!br_uncond_label || !br_false_label || br_uncond_label->label_num != br_false_label->label_num)
        {
            return false;
        }

        int  icmp_op1_reg = icmp_op1->GetRegNum();
        auto load_inst    = define_map[icmp_op1_reg];
        if (!load_inst || load_inst->opcode != LLVMIR::IROpCode::LOAD) return false;
        auto load = dynamic_cast<LLVMIR::LoadInst*>(load_inst);
        if (!load) return false;

        LLVMIR::Operand* final_op1 = nullptr;
        LLVMIR::Operand* final_op2 = nullptr;

        if (icmp_op2->type == LLVMIR::OperandType::REG)
        {
            int  icmp_op2_reg = icmp_op2->GetRegNum();
            auto load2_inst   = define_map[icmp_op2_reg];
            if (load2_inst && load2_inst->opcode == LLVMIR::IROpCode::LOAD)
            {
                auto load2 = dynamic_cast<LLVMIR::LoadInst*>(load2_inst);
                if (store_inst->val->GetRegNum() == icmp_op1_reg &&
                    store_inst->ptr->GetRegNum() == load2->ptr->GetRegNum())
                {
                    final_op1 = icmp_op1;
                    final_op2 = icmp_op2;
                }
                else if (store_inst->val->GetRegNum() == icmp_op2_reg &&
                         store_inst->ptr->GetRegNum() == load->ptr->GetRegNum())
                {
                    final_op1 = icmp_op1;
                    final_op2 = icmp_op2;
                }
                else { return false; }
            }
            else
            {
                if (store_inst->val->GetRegNum() != icmp_op2_reg ||
                    store_inst->ptr->GetRegNum() != load->ptr->GetRegNum())
                {
                    return false;
                }
                final_op1 = icmp_op1;
                final_op2 = icmp_op2;
            }
        }
        else
        {
            bool operand_match = false;
            if (icmp_op2->type == LLVMIR::OperandType::IMMEI32 && store_inst->val->type == LLVMIR::OperandType::IMMEI32)
            {
                operand_match = (icmp_op2->GetImm() == store_inst->val->GetImm());
            }
            else if (icmp_op2->type == LLVMIR::OperandType::IMMEF32 &&
                     store_inst->val->type == LLVMIR::OperandType::IMMEF32)
            {
                operand_match = (icmp_op2->GetImmF() == store_inst->val->GetImmF());
            }

            if (!operand_match || store_inst->ptr->GetRegNum() != load->ptr->GetRegNum()) { return false; }
            final_op1 = icmp_op1;
            final_op2 = icmp_op2;
        }

        LLVMIR::IROpCode minmax_op = is_signed ? (is_min ? LLVMIR::IROpCode::SMIN_I32 : LLVMIR::IROpCode::SMAX_I32)
                                               : (is_min ? LLVMIR::IROpCode::UMIN_I32 : LLVMIR::IROpCode::UMAX_I32);

        auto result_reg = getRegOperand(++cfg->func->max_reg);

        auto minmax_inst = new LLVMIR::ArithmeticInst(minmax_op, store_inst->type, final_op1, final_op2, result_reg);

        store_block->insts.insert(store_block->insts.begin(), minmax_inst);

        store_inst->val = result_reg;

        auto new_br_uncond = new LLVMIR::BranchUncondInst(getLabelOperand(store_block->block_id));

        for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
        {
            if (*it == br_inst)
            {
                *it = new_br_uncond;
                delete br_inst;
                break;
            }
        }

        return true;
    }

    LLVMIR::IROpCode MinMaxRecognizePass::getMinMaxOpCode(LLVMIR::IcmpCond cond, bool is_min, bool is_signed)
    {
        if (is_signed) { return is_min ? LLVMIR::IROpCode::SMIN_I32 : LLVMIR::IROpCode::SMAX_I32; }
        else { return is_min ? LLVMIR::IROpCode::UMIN_I32 : LLVMIR::IROpCode::UMAX_I32; }
    }

    LLVMIR::IROpCode MinMaxRecognizePass::getFMinMaxOpCode(LLVMIR::FcmpCond cond, bool is_min)
    {
        return is_min ? LLVMIR::IROpCode::FMIN_F32 : LLVMIR::IROpCode::FMAX_F32;
    }

    bool MinMaxRecognizePass::recognizeIntegerSelectMinMax(LLVMIR::SelectInst* select_inst, LLVMIR::IcmpInst* icmp_inst,
        LLVMIR::IRBlock* block, std::map<int, LLVMIR::Instruction*>& define_map)
    {
        auto true_val  = select_inst->true_val;
        auto false_val = select_inst->false_val;
        auto cmp_lhs   = icmp_inst->lhs;
        auto cmp_rhs   = icmp_inst->rhs;
        auto cmp_cond  = icmp_inst->cond;

        bool match_case1 = false;
        bool match_case2 = false;

        auto operand_equals = [](LLVMIR::Operand* op1, LLVMIR::Operand* op2) -> bool {
            if (op1->type != op2->type) return false;

            switch (op1->type)
            {
                case LLVMIR::OperandType::REG: return op1->GetRegNum() == op2->GetRegNum();
                case LLVMIR::OperandType::IMMEI32: return op1->GetImm() == op2->GetImm();
                case LLVMIR::OperandType::IMMEF32: return op1->GetImmF() == op2->GetImmF();
                case LLVMIR::OperandType::GLOBAL: return op1->GetGlobal() == op2->GetGlobal();
                default: return false;
            }
        };

        match_case1 = operand_equals(true_val, cmp_lhs) && operand_equals(false_val, cmp_rhs);
        match_case2 = operand_equals(true_val, cmp_rhs) && operand_equals(false_val, cmp_lhs);

        if (!match_case1 && !match_case2) return false;

        bool is_min    = false;
        bool is_signed = true;

        switch (cmp_cond)
        {
            case LLVMIR::IcmpCond::SLT:
            case LLVMIR::IcmpCond::SLE:
                is_min    = match_case1;
                is_signed = true;
                break;
            case LLVMIR::IcmpCond::SGT:
            case LLVMIR::IcmpCond::SGE:
                is_min    = match_case2;
                is_signed = true;
                break;
            case LLVMIR::IcmpCond::ULT:
            case LLVMIR::IcmpCond::ULE:
                is_min    = match_case1;
                is_signed = false;
                break;
            case LLVMIR::IcmpCond::UGT:
            case LLVMIR::IcmpCond::UGE:
                is_min    = match_case2;
                is_signed = false;
                break;
            default: return false;
        }

        LLVMIR::IROpCode minmax_op = is_signed ? (is_min ? LLVMIR::IROpCode::SMIN_I32 : LLVMIR::IROpCode::SMAX_I32)
                                               : (is_min ? LLVMIR::IROpCode::UMIN_I32 : LLVMIR::IROpCode::UMAX_I32);

        auto minmax_inst = new LLVMIR::ArithmeticInst(minmax_op, select_inst->type, cmp_lhs, cmp_rhs, select_inst->res);

        for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
        {
            if (*it == select_inst)
            {
                *it = minmax_inst;
                delete select_inst;
                break;
            }
        }

        return true;
    }

    bool MinMaxRecognizePass::recognizeFloatSelectMinMax(LLVMIR::SelectInst* select_inst, LLVMIR::FcmpInst* fcmp_inst,
        LLVMIR::IRBlock* block, std::map<int, LLVMIR::Instruction*>& define_map)
    {
        auto true_val  = select_inst->true_val;
        auto false_val = select_inst->false_val;
        auto cmp_lhs   = fcmp_inst->lhs;
        auto cmp_rhs   = fcmp_inst->rhs;
        auto cmp_cond  = fcmp_inst->cond;

        bool match_case1 = false;
        bool match_case2 = false;

        auto operand_equals = [](LLVMIR::Operand* op1, LLVMIR::Operand* op2) -> bool {
            if (op1->type != op2->type) return false;

            switch (op1->type)
            {
                case LLVMIR::OperandType::REG: return op1->GetRegNum() == op2->GetRegNum();
                case LLVMIR::OperandType::IMMEF32: return op1->GetImmF() == op2->GetImmF();
                case LLVMIR::OperandType::GLOBAL: return op1->GetGlobal() == op2->GetGlobal();
                default: return false;
            }
        };

        match_case1 = operand_equals(true_val, cmp_lhs) && operand_equals(false_val, cmp_rhs);
        match_case2 = operand_equals(true_val, cmp_rhs) && operand_equals(false_val, cmp_lhs);

        if (!match_case1 && !match_case2) return false;

        bool is_min = false;

        switch (cmp_cond)
        {
            case LLVMIR::FcmpCond::OLT:
            case LLVMIR::FcmpCond::OLE:
            case LLVMIR::FcmpCond::ULT:
            case LLVMIR::FcmpCond::ULE: is_min = match_case1; break;
            case LLVMIR::FcmpCond::OGT:
            case LLVMIR::FcmpCond::OGE:
            case LLVMIR::FcmpCond::UGT:
            case LLVMIR::FcmpCond::UGE: is_min = match_case2; break;
            default: return false;
        }

        LLVMIR::IROpCode minmax_op = is_min ? LLVMIR::IROpCode::FMIN_F32 : LLVMIR::IROpCode::FMAX_F32;

        auto minmax_inst = new LLVMIR::ArithmeticInst(minmax_op, select_inst->type, cmp_lhs, cmp_rhs, select_inst->res);

        for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
        {
            if (*it == select_inst)
            {
                *it = minmax_inst;
                delete select_inst;
                break;
            }
        }

        return true;
    }
}  // namespace Transform
