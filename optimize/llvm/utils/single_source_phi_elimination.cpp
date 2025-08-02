#include "single_source_phi_elimination.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <unordered_set>

namespace Transform
{
    SingleSourcePhiEliminationPass::SingleSourcePhiEliminationPass(LLVMIR::IR* ir)
        : Pass(ir), preserve_lcssa_(true), last_execution_modified_(false)
    {}

    void SingleSourcePhiEliminationPass::Execute()
    {
        last_execution_modified_ = false;

        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (preserve_lcssa_)
            {
                assert(cfg->LoopForest != nullptr && "Loop information is required when preserve_lcssa is enabled");
            }

            bool                                 changed = false;
            std::vector<LLVMIR::PhiInst*>        phis_to_remove;
            std::unordered_set<LLVMIR::PhiInst*> processed_phis;

            for (const auto& [block_id, block] : cfg->block_id_to_block)
            {
                if (preserve_lcssa_ && isLoopExitBlock(block, cfg)) continue;

                for (auto* inst : block->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::PHI)
                    {
                        auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                        if (processed_phis.find(phi) != processed_phis.end()) continue;

                        if (processSingleSourcePhi(phi, block, cfg))
                        {
                            phis_to_remove.push_back(phi);
                            processed_phis.insert(phi);
                            changed = true;
                        }
                    }
                }
            }

            for (auto* phi : phis_to_remove)
            {
                auto block_it = cfg->block_id_to_block.find(phi->block_id);
                if (block_it != cfg->block_id_to_block.end())
                {
                    auto& insts = block_it->second->insts;
                    insts.erase(std::remove(insts.begin(), insts.end(), phi), insts.end());
                }
            }

            if (changed) last_execution_modified_ = true;
        }
    }

    bool SingleSourcePhiEliminationPass::isLoopExitBlock(LLVMIR::IRBlock* block, CFG* cfg) const
    {
        if (!cfg->LoopForest || cfg->LoopForest->loop_set.empty()) return false;

        for (auto* loop : cfg->LoopForest->loop_set)
            if (loop->exit_nodes.count(block) > 0) return true;

        return false;
    }

    bool SingleSourcePhiEliminationPass::processSingleSourcePhi(LLVMIR::PhiInst* phi, LLVMIR::IRBlock* block, CFG* cfg)
    {
        if (phi->vals_for_labels.size() != 1) return false;

        auto& [source_val, source_label] = phi->vals_for_labels[0];
        auto* dest_operand               = phi->res;

        if (!source_val || !dest_operand) return false;

        auto* copy_inst = createCopyInst(phi->type, source_val, dest_operand);
        if (!copy_inst) return false;

        copy_inst->block_id = phi->block_id;

        auto& insts  = block->insts;
        auto  phi_it = std::find(insts.begin(), insts.end(), phi);
        if (phi_it != insts.end())
            insts.insert(phi_it, copy_inst);
        else
            insts.insert(insts.begin(), copy_inst);

        return true;
    }

}  // namespace Transform
