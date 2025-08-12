#include "instruction_simplify.h"
#include "llvm_ir/defs.h"
#include <iostream>
#include <algorithm>

namespace Transform
{

    InstructionSimplifyPass::InstructionSimplifyPass(LLVMIR::IR* ir) : Pass(ir), modified_(false) {}

    void InstructionSimplifyPass::Execute()
    {
        modified_ = false;
        for (auto& [func_def, cfg] : ir->cfg) processFunction(cfg);
    }

    void InstructionSimplifyPass::processFunction(CFG* cfg)
    {
        for (auto& [id, block] : cfg->block_id_to_block)
            for (auto& inst : block->insts)
            {
                reorderOperandsForConstants(inst);
                convertSubtractionToAddition(inst);
                simplifyZeroResultOperations(inst);
            }

        eliminateRedundantInstructions(cfg);
    }

    void InstructionSimplifyPass::reorderOperandsForConstants(LLVMIR::Instruction* inst)
    {
        if (inst->opcode == LLVMIR::IROpCode::ADD || inst->opcode == LLVMIR::IROpCode::MUL)
        {
            auto arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
            if (arith_inst && arith_inst->lhs->type == LLVMIR::OperandType::IMMEI32)
            {
                auto temp       = arith_inst->lhs;
                arith_inst->lhs = arith_inst->rhs;
                arith_inst->rhs = temp;
                modified_       = true;
            }
        }

        if (inst->opcode == LLVMIR::IROpCode::ICMP)
        {
            auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(inst);
            if (icmp_inst && (icmp_inst->cond == LLVMIR::IcmpCond::EQ || icmp_inst->cond == LLVMIR::IcmpCond::NE))
            {
                if (icmp_inst->lhs->type == LLVMIR::OperandType::IMMEI32)
                {
                    auto temp      = icmp_inst->lhs;
                    icmp_inst->lhs = icmp_inst->rhs;
                    icmp_inst->rhs = temp;
                    modified_      = true;
                }
            }
        }
    }

    void InstructionSimplifyPass::convertSubtractionToAddition(LLVMIR::Instruction*& inst)
    {
        if (inst->opcode != LLVMIR::IROpCode::SUB) return;

        auto arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
        if (arith_inst && arith_inst->rhs->type != LLVMIR::OperandType::IMMEI32) return;

        auto imm_op = dynamic_cast<LLVMIR::ImmeI32Operand*>(arith_inst->rhs);
        if (!imm_op) return;

        int imm_val = imm_op->value;
        if (imm_val == -2147483648) return;

        arith_inst->rhs    = LLVMIR::ImmeI32Operand::get(-imm_val);
        arith_inst->opcode = LLVMIR::IROpCode::ADD;
        modified_          = true;
    }

    void InstructionSimplifyPass::simplifyZeroResultOperations(LLVMIR::Instruction*& inst)
    {
        if (inst->opcode == LLVMIR::IROpCode::SUB)
        {
            auto arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
            if (arith_inst && arith_inst->lhs->getName() == arith_inst->rhs->getName())
            {
                auto new_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                    arith_inst->type,
                    LLVMIR::ImmeI32Operand::get(0),
                    LLVMIR::ImmeI32Operand::get(0),
                    arith_inst->res);
                inst          = new_inst;
                modified_     = true;
            }
        }

        if (inst->opcode == LLVMIR::IROpCode::MUL)
        {
            auto arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
            if (arith_inst)
            {
                bool is_zero = false;

                if (arith_inst->lhs->type == LLVMIR::OperandType::IMMEI32)
                {
                    auto imm1 = dynamic_cast<LLVMIR::ImmeI32Operand*>(arith_inst->lhs);
                    if (imm1 && imm1->value == 0) { is_zero = true; }
                }

                if (arith_inst->rhs->type == LLVMIR::OperandType::IMMEI32)
                {
                    auto imm2 = dynamic_cast<LLVMIR::ImmeI32Operand*>(arith_inst->rhs);
                    if (imm2 && imm2->value == 0) { is_zero = true; }
                }

                if (is_zero)
                {
                    auto new_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                        arith_inst->type,
                        LLVMIR::ImmeI32Operand::get(0),
                        LLVMIR::ImmeI32Operand::get(0),
                        arith_inst->res);
                    inst          = new_inst;
                    modified_     = true;
                }
            }
        }
    }

    void InstructionSimplifyPass::eliminateRedundantInstructions(CFG* cfg)
    {
        std::map<int, int>             union_find_map;
        std::set<LLVMIR::Instruction*> erase_set;

        std::function<int(int)> union_find = [&](int reg_no) -> int {
            if (union_find_map.find(reg_no) == union_find_map.end()) union_find_map[reg_no] = reg_no;
            if (union_find_map[reg_no] == reg_no) return reg_no;
            return union_find_map[reg_no] = union_find(union_find_map[reg_no]);
        };

        auto connect = [&](LLVMIR::Operand* result_op, LLVMIR::Operand* replace_op) -> void {
            auto reg1 = dynamic_cast<LLVMIR::RegOperand*>(result_op);
            auto reg0 = dynamic_cast<LLVMIR::RegOperand*>(replace_op);
            if (reg1 && reg0)
            {
                int reg1_no = reg1->reg_num;
                int reg0_no = reg0->reg_num;
                if (union_find_map.find(reg1_no) == union_find_map.end()) union_find_map[reg1_no] = reg1_no;
                if (union_find_map.find(reg0_no) == union_find_map.end()) union_find_map[reg0_no] = reg0_no;
                union_find_map[union_find(reg1_no)] = union_find(reg0_no);
            }
        };

        for (auto& [id, block] : cfg->block_id_to_block)
        {
            for (auto inst : block->insts)
            {
                auto used_ops = inst->GetUsedOperands();
                if (used_ops.size() <= 1) continue;

                if (used_ops[0]->type != LLVMIR::OperandType::REG) continue;
                if (used_ops[1]->type == LLVMIR::OperandType::REG) continue;

                if (inst->opcode == LLVMIR::IROpCode::ADD)
                {
                    auto arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                    if (arith_inst && arith_inst->rhs->type == LLVMIR::OperandType::IMMEI32)
                    {
                        auto imm_op = dynamic_cast<LLVMIR::ImmeI32Operand*>(arith_inst->rhs);
                        if (imm_op && imm_op->value == 0)
                        {
                            connect(arith_inst->res, arith_inst->lhs);
                            erase_set.insert(inst);
                            modified_ = true;
                        }
                    }
                }

                if (inst->opcode == LLVMIR::IROpCode::MUL || inst->opcode == LLVMIR::IROpCode::DIV)
                {
                    auto arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                    if (arith_inst && arith_inst->rhs->type == LLVMIR::OperandType::IMMEI32)
                    {
                        auto imm_op = dynamic_cast<LLVMIR::ImmeI32Operand*>(arith_inst->rhs);
                        if (imm_op && imm_op->value == 1)
                        {
                            connect(arith_inst->res, arith_inst->lhs);
                            erase_set.insert(inst);
                            modified_ = true;
                        }
                    }
                }

                if (inst->opcode == LLVMIR::IROpCode::FADD)
                {
                    auto arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                    if (arith_inst && arith_inst->rhs->type == LLVMIR::OperandType::IMMEF32)
                    {
                        auto imm_op = dynamic_cast<LLVMIR::ImmeF32Operand*>(arith_inst->rhs);
                        if (imm_op && imm_op->value == 0.0f)
                        {
                            connect(arith_inst->res, arith_inst->lhs);
                            erase_set.insert(inst);
                            modified_ = true;
                        }
                    }
                }

                if (inst->opcode == LLVMIR::IROpCode::FMUL || inst->opcode == LLVMIR::IROpCode::FDIV)
                {
                    auto arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                    if (arith_inst && arith_inst->rhs->type == LLVMIR::OperandType::IMMEF32)
                    {
                        auto imm_op = dynamic_cast<LLVMIR::ImmeF32Operand*>(arith_inst->rhs);
                        if (imm_op && imm_op->value == 1.0f)
                        {
                            connect(arith_inst->res, arith_inst->lhs);
                            erase_set.insert(inst);
                            modified_ = true;
                        }
                    }
                }
            }
        }

        for (auto& [id, block] : cfg->block_id_to_block)
        {
            for (auto& inst : block->insts)
            {
                int result_reg_no = inst->GetResultReg();
                if (union_find_map.find(result_reg_no) != union_find_map.end())
                {
                    union_find_map[result_reg_no] = union_find(result_reg_no);
                }

                auto used_ops = inst->GetUsedOperands();
                for (auto op : used_ops)
                {
                    if (op->type != LLVMIR::OperandType::REG) continue;

                    auto reg_op = dynamic_cast<LLVMIR::RegOperand*>(op);
                    if (!reg_op) continue;

                    int reg_no = reg_op->reg_num;
                    if (union_find_map.find(reg_no) != union_find_map.end() && union_find_map[reg_no] != reg_no)
                        union_find_map[reg_no] = union_find(reg_no);
                }
            }
        }

        for (auto& [id, block] : cfg->block_id_to_block)
        {
            auto new_inst_list = std::deque<LLVMIR::Instruction*>();
            for (auto& inst : block->insts)
            {
                if (erase_set.find(inst) == erase_set.end())
                {
                    inst->Rename(union_find_map);
                    new_inst_list.push_back(inst);
                }
            }
            block->insts = new_inst_list;
        }
    }

}  // namespace Transform
