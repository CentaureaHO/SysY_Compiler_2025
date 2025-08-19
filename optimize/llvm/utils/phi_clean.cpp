#include "phi_clean.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/function.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

namespace Transform
{
    PhiCleanPass::PhiCleanPass(LLVMIR::IR* ir) : Pass(ir), last_execution_modified_(false) {}

    void PhiCleanPass::Execute()
    {
        last_execution_modified_ = false;

        for (auto* func : ir->functions)
        {
            for (auto* block : func->blocks)
            {
                for (auto* inst : block->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::PHI)
                    {
                        auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                        if (cleanPhiInstruction(phi)) { last_execution_modified_ = true; }
                    }
                }
            }
        }
    }

    bool PhiCleanPass::cleanPhiInstruction(LLVMIR::PhiInst* phi)
    {
        if (phi->vals_for_labels.empty()) { return false; }

        // 使用vector来存储去重后的结果
        std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> unique_vals;

        for (const auto& [val_op, label_op] : phi->vals_for_labels)
        {
            // 检查是否已经存在相同的组合
            bool found_duplicate = false;
            for (const auto& [existing_val, existing_label] : unique_vals)
            {
                if (areOperandsEqual(val_op, existing_val) && areOperandsEqual(label_op, existing_label))
                {
                    found_duplicate = true;
                    break;
                }
            }

            // 如果没有找到重复，则添加到唯一列表中
            if (!found_duplicate) { unique_vals.push_back({val_op, label_op}); }
        }

        // 如果去重后的大小与原大小不同，则说明有重复被移除
        bool modified = (unique_vals.size() != phi->vals_for_labels.size());

        if (modified)
        {
            // 更新PHI指令的vals_for_labels
            phi->vals_for_labels = unique_vals;
        }

        return modified;
    }

    bool PhiCleanPass::areOperandsEqual(LLVMIR::Operand* op1, LLVMIR::Operand* op2)
    {
        if (!op1 || !op2) { return op1 == op2; }

        if (op1->type != op2->type) { return false; }

        switch (op1->type)
        {
            case LLVMIR::OperandType::REG: return op1->GetRegNum() == op2->GetRegNum();

            case LLVMIR::OperandType::IMMEI32: return op1->GetImm() == op2->GetImm();

            case LLVMIR::OperandType::IMMEF32: return op1->GetImmF() == op2->GetImmF();

            case LLVMIR::OperandType::GLOBAL: return op1->GetGlobal() == op2->GetGlobal();

            case LLVMIR::OperandType::LABEL: return op1->GetRegNum() == op2->GetRegNum();  // Label也使用GetRegNum

            default: return false;
        }
    }

}  // namespace Transform
