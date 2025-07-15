#include "unify_return.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include <algorithm>
#include <iostream>

namespace Transform
{
    UnifyReturnPass::UnifyReturnPass(LLVMIR::IR* ir) : Pass(ir) {}

    void UnifyReturnPass::Execute()
    {
        for (auto& [func_def, cfg] : ir->cfg) unifyFunctionReturns(cfg);
    }

    void UnifyReturnPass::unifyFunctionReturns(CFG* cfg)
    {
        auto ret_instructions = findReturnInstructions(cfg);
        if (ret_instructions.size() <= 1) return;

        LLVMIR::IRBlock* exit_block = createUnifiedExitBlock(cfg);

        std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> return_values;
        LLVMIR::DataType                                           return_type = LLVMIR::DataType::VOID;

        for (auto* ret_inst : ret_instructions)
        {
            LLVMIR::IRBlock* containing_block = nullptr;
            for (auto& [block_id, block] : cfg->block_id_to_block)
            {
                auto it = std::find(block->insts.begin(), block->insts.end(), ret_inst);
                if (it == block->insts.end()) continue;

                containing_block = block;
                break;
            }

            if (!containing_block) continue;

            return_type = ret_inst->ret_type;

            if (ret_inst->ret)
                return_values.push_back({ret_inst->ret, getLabelOperand(containing_block->block_id)});
            else
                return_values.push_back({nullptr, getLabelOperand(containing_block->block_id)});

            auto it = std::find(containing_block->insts.begin(), containing_block->insts.end(), ret_inst);
            if (it != containing_block->insts.end())
            {
                auto* branch_inst     = new LLVMIR::BranchUncondInst(getLabelOperand(exit_block->block_id));
                branch_inst->block_id = containing_block->block_id;

                *it = branch_inst;

                delete ret_inst;
            }
        }

        if (return_type != LLVMIR::DataType::VOID && !return_values.empty())
        {
            LLVMIR::Operand* result_reg = getRegOperand(++cfg->func->max_reg);

            std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> valid_values;
            for (auto& [val, label] : return_values)
                if (val != nullptr) valid_values.push_back({val, label});

            if (!valid_values.empty())
            {
                auto* phi_inst     = new LLVMIR::PhiInst(return_type, result_reg, &valid_values);
                phi_inst->block_id = exit_block->block_id;
                exit_block->insts.push_back(phi_inst);

                auto* final_ret     = new LLVMIR::RetInst(return_type, result_reg);
                final_ret->block_id = exit_block->block_id;
                exit_block->insts.push_back(final_ret);
            }
            else
            {
                auto* final_ret     = new LLVMIR::RetInst(LLVMIR::DataType::VOID, nullptr);
                final_ret->block_id = exit_block->block_id;
                exit_block->insts.push_back(final_ret);
            }
        }
        else
        {
            auto* final_ret     = new LLVMIR::RetInst(LLVMIR::DataType::VOID, nullptr);
            final_ret->block_id = exit_block->block_id;
            exit_block->insts.push_back(final_ret);
        }

        cfg->BuildCFG();
    }

    std::vector<LLVMIR::RetInst*> UnifyReturnPass::findReturnInstructions(CFG* cfg)
    {
        std::vector<LLVMIR::RetInst*> ret_instructions;

        for (auto& [block_id, block] : cfg->block_id_to_block)
            for (auto* inst : block->insts)
                if (inst->opcode == LLVMIR::IROpCode::RET)
                    ret_instructions.push_back(static_cast<LLVMIR::RetInst*>(inst));

        return ret_instructions;
    }

    LLVMIR::IRBlock* UnifyReturnPass::createUnifiedExitBlock(CFG* cfg)
    {
        LLVMIR::IRBlock* exit_block                  = cfg->func->createBlock();
        cfg->block_id_to_block[exit_block->block_id] = exit_block;
        return exit_block;
    }

}  // namespace Transform
