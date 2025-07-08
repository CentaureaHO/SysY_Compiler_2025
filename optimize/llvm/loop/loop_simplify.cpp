#include "llvm/loop/loop_simplify.h"
#include "llvm_ir/ir_builder.h"
#include <algorithm>
#include <cassert>
#include <stack>
#include <functional>

namespace StructuralTransform
{
    LoopSimplifyPass::LoopSimplifyPass(LLVMIR::IR* ir) : Pass(ir) {}

    void LoopSimplifyPass::Execute()
    {
        loops_processed = 0;
        loops_modified  = 0;

        for (const auto& [func_def, cfg] : ir->cfg) loopSimplify(cfg);
    }

    void LoopSimplifyPass::loopSimplify(CFG* cfg)
    {
        if (!cfg || !cfg->LoopForest) return;

        for (const auto& [id, bb] : cfg->block_id_to_block) { bb->comment = ""; }

        std::function<void(CFG*, NaturalLoopForest&, NaturalLoop*)> processLoop =
            [&](CFG* config, NaturalLoopForest& loop_forest, NaturalLoop* loop) -> void {
            ++loops_processed;

            if (static_cast<size_t>(loop->loop_id) < loop_forest.loopG.size())
                for (auto* child_loop : loop_forest.loopG[loop->loop_id]) processLoop(config, loop_forest, child_loop);

            if (isLoopSimplified(loop, config)) return;

            loop->simplify(config);
            ++loops_modified;
        };

        for (auto* loop : cfg->LoopForest->getRootLoops()) processLoop(cfg, *cfg->LoopForest, loop);

        eliminateUselessPhi(cfg);
    }

    bool LoopSimplifyPass::isLoopSimplified(NaturalLoop* loop, CFG* cfg) const
    {
        if (!loop || !cfg) return false;

        return loop->isSimplified(cfg);
    }

    std::pair<int, int> LoopSimplifyPass::getSimplificationStats() const { return {loops_processed, loops_modified}; }

    void LoopSimplifyPass::eliminateUselessPhi(CFG* cfg)
    {
        if (!cfg) return;

        bool changed = true;
        while (changed)
        {
            changed = false;

            for (const auto& [id, bb] : cfg->block_id_to_block)
            {
                auto it = bb->insts.begin();
                while (it != bb->insts.end())
                {
                    if ((*it)->opcode != LLVMIR::IROpCode::PHI) break;

                    auto* phi = dynamic_cast<LLVMIR::PhiInst*>(*it);

                    if (phi->vals_for_labels.size() == 1)
                        changed = true;
                    else if (phi->vals_for_labels.size() > 1)
                    {
                        auto* first_val = phi->vals_for_labels[0].first;

                        changed |= std::all_of(phi->vals_for_labels.begin(),
                            phi->vals_for_labels.end(),
                            [first_val](const auto& pair) { return pair.first == first_val; });
                    }

                    ++it;
                }
            }
        }
    }

}  // namespace StructuralTransform
