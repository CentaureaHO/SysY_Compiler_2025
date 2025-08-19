#include "constant_branch_folding.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <iostream>
#include <algorithm>

// #define DBGMODE_CBF

#ifdef DBGMODE_CBF
template <typename... Args>
void cbf_dbg_impl(Args&&... args)
{
    ((std::cout << args), ...);
    std::cout << std::endl;
}
#define CBFINFO(...) cbf_dbg_impl(__VA_ARGS__)
#else
#define CBFINFO(...) \
    do {             \
    } while (0)
#endif

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
                        if (processConstantBranch(branch, block, cfg))
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

    bool ConstantBranchFoldingPass::processConstantBranch(
        LLVMIR::BranchCondInst* branch, LLVMIR::IRBlock* block, CFG* cfg)
    {
        int condition_value;

        if (!getConstantValue(branch->cond, condition_value)) return false;

        LLVMIR::Operand* target_label;
        LLVMIR::Operand* unreachable_label;

        if (condition_value != 0)
        {
            target_label      = branch->true_label;
            unreachable_label = branch->false_label;
            CBFINFO(
                "Constant branch folding: condition is true, jumping to true label, false label becomes unreachable");
        }
        else
        {
            target_label      = branch->false_label;
            unreachable_label = branch->true_label;
            CBFINFO(
                "Constant branch folding: condition is false, jumping to false label, true label becomes unreachable");
        }

        cleanupPhiNodes(unreachable_label, branch->block_id, cfg);

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

    void ConstantBranchFoldingPass::cleanupPhiNodes(LLVMIR::Operand* unreachable_label, int from_block_id, CFG* cfg)
    {
        if (!unreachable_label || unreachable_label->type != LLVMIR::OperandType::LABEL) return;

        auto* label_op        = static_cast<LLVMIR::LabelOperand*>(unreachable_label);
        int   target_block_id = label_op->label_num;

        CBFINFO("Cleaning up PHI nodes in block ", target_block_id, " removing entries from block ", from_block_id);

        auto block_it = cfg->block_id_to_block.find(target_block_id);
        if (block_it == cfg->block_id_to_block.end()) return;

        auto* target_block = block_it->second;
        if (!target_block) return;

        for (auto* inst : target_block->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI)
            {
                auto* phi             = static_cast<LLVMIR::PhiInst*>(inst);
                auto& vals_for_labels = phi->vals_for_labels;

                auto it = std::remove_if(vals_for_labels.begin(),
                    vals_for_labels.end(),
                    [from_block_id](const std::pair<LLVMIR::Operand*, LLVMIR::Operand*>& entry) {
                        if (entry.second && entry.second->type == LLVMIR::OperandType::LABEL)
                        {
                            auto* label = static_cast<LLVMIR::LabelOperand*>(entry.second);
                            return label->label_num == from_block_id;
                        }
                        return false;
                    });

                if (it != vals_for_labels.end())
                {
                    CBFINFO("Removed PHI entry from block ",
                        from_block_id,
                        " in PHI instruction at block ",
                        target_block_id);
                    vals_for_labels.erase(it, vals_for_labels.end());
                }
            }
            else { break; }
        }
    }

}  // namespace Transform
