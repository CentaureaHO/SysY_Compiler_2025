#include "constant_branch_folding.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <iostream>
#include <algorithm>

namespace Transform
{
    ConstantBranchFoldingPass::ConstantBranchFoldingPass(LLVMIR::IR* ir) : Pass(ir), last_execution_modified_(false) {}

    void ConstantBranchFoldingPass::Execute()
    {
        last_execution_modified_ = false;

        for (const auto& [func_def, cfg] : ir->cfg)
        {
            bool                                 changed = false;
            std::vector<LLVMIR::BranchCondInst*> branches_to_replace;

            for (const auto& [block_id, block] : cfg->block_id_to_block)
            {
                for (auto* inst : block->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::BR_COND)
                    {
                        auto* branch = static_cast<LLVMIR::BranchCondInst*>(inst);
                        if (processConstantBranch(branch, block))
                        {
                            branches_to_replace.push_back(branch);
                            changed = true;
                        }
                    }
                }
            }

            for (auto* branch : branches_to_replace)
            {
                auto block_it = cfg->block_id_to_block.find(branch->block_id);
                if (block_it != cfg->block_id_to_block.end())
                {
                    auto& insts = block_it->second->insts;
                    insts.erase(std::remove(insts.begin(), insts.end(), branch), insts.end());
                }
            }

            if (changed) last_execution_modified_ = true;
        }
    }

    bool ConstantBranchFoldingPass::getConstantValue(LLVMIR::Operand* operand, int& value) const
    {
        if (!operand) return false;

        if (operand->type == LLVMIR::OperandType::IMMEI32)
        {
            auto* imm = static_cast<LLVMIR::ImmeI32Operand*>(operand);
            value     = imm->value;
            return true;
        }

        return false;
    }

    bool ConstantBranchFoldingPass::processConstantBranch(LLVMIR::BranchCondInst* branch, LLVMIR::IRBlock* block)
    {
        int condition_value;

        if (!getConstantValue(branch->cond, condition_value)) return false;

        LLVMIR::Operand* target_label;
        if (condition_value != 0)
            target_label = branch->true_label;
        else
            target_label = branch->false_label;

        auto* uncond_branch     = new LLVMIR::BranchUncondInst(target_label);
        uncond_branch->block_id = branch->block_id;

        auto& insts     = block->insts;
        auto  branch_it = std::find(insts.begin(), insts.end(), branch);
        if (branch_it != insts.end())
            insts.insert(branch_it, uncond_branch);
        else
            insts.push_back(uncond_branch);

        return true;
    }

}  // namespace Transform
