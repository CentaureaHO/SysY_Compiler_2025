#include "llvm/loop/loop_find.h"
#include "llvm_ir/ir_builder.h"
#include <stack>
#include <queue>
#include <algorithm>
#include <cassert>
#include <set>

namespace Analysis
{
    LoopAnalysisPass::LoopAnalysisPass(LLVMIR::IR* ir) : Pass(ir) {}

    void LoopAnalysisPass::Execute()
    {
        for (const auto& [func_def, cfg] : ir->cfg) { buildLoopInfo(cfg); }
    }

    bool LoopAnalysisPass::isDominated(CFG* cfg, int dominator_id, int node_id) const
    {
        if (!cfg || !ir->DomTrees.count(cfg)) return false;

        const auto& dom_tree = ir->DomTrees.at(cfg)->dom_tree;
        if (dominator_id == node_id) return true;

        if (static_cast<size_t>(dominator_id) >= dom_tree.size()) return false;

        std::function<bool(int, int)> checkDomination = [&](int current, int target) -> bool {
            if (current == target) return true;
            if (static_cast<size_t>(current) >= dom_tree.size()) return false;

            return std::any_of(dom_tree[current].begin(), dom_tree[current].end(), [&](int child) {
                return checkDomination(child, target);
            });
        };

        return checkDomination(dominator_id, node_id);
    }

    void LoopAnalysisPass::buildLoopHierarchy(NaturalLoopForest* forest)
    {
        assert(forest != nullptr && "Forest cannot be null");
        forest->buildHierarchy();
    }

    void LoopAnalysisPass::buildLoopInfo(CFG* cfg)
    {
        assert(cfg != nullptr && "CFG cannot be null");

        // Initialize or reset loop forest
        if (!cfg->LoopForest)
            cfg->LoopForest = new NaturalLoopForest();
        else
            cfg->LoopForest->clear();

        auto* forest = cfg->LoopForest;

        int loop_count = 0;
        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            if (static_cast<size_t>(block_id) >= cfg->G.size()) continue;

            for (const auto& successor : cfg->G[block_id])
            {
                // Check if this is a back edge (successor dominates current block)
                if (!isDominated(cfg, successor->block_id, block_id)) continue;

                // Create new loop
                auto* loop   = new NaturalLoop();
                loop->cfg    = cfg;
                loop->header = successor;
                loop->latches.insert(block);
                loop->loop_id    = loop_count++;
                loop->loop_nodes = findNodesInLoop(cfg, block, successor);

                forest->loop_set.insert(loop);
            }
        }

        forest->loop_cnt = loop_count;
        forest->combineSameHeadLoops();

        for (auto* loop : forest->loop_set) loop->findExitNodes(cfg);

        buildLoopHierarchy(forest);
        identifyLoopPreheaders(cfg);
    }

    bool LoopAnalysisPass::hasLoopInfo(CFG* cfg) const
    {
        return cfg && cfg->LoopForest && !cfg->LoopForest->loop_set.empty();
    }

    NaturalLoop* LoopAnalysisPass::getLoopForBlock(CFG* cfg, LLVMIR::IRBlock* block) const
    {
        if (!hasLoopInfo(cfg) || !block) return nullptr;

        NaturalLoop* innermost_loop     = nullptr;
        size_t       smallest_loop_size = SIZE_MAX;

        for (auto* loop : cfg->LoopForest->loop_set)
        {
            if (loop->loop_nodes.find(block) == loop->loop_nodes.end()) continue;
            if (loop->loop_nodes.size() >= smallest_loop_size) continue;

            smallest_loop_size = loop->loop_nodes.size();
            innermost_loop     = loop;
        }

        return innermost_loop;
    }

    std::vector<NaturalLoop*> LoopAnalysisPass::getLoopsInDFSOrder(CFG* cfg) const
    {
        std::vector<NaturalLoop*> result;

        if (!hasLoopInfo(cfg)) return result;

        const auto*            forest = cfg->LoopForest;
        std::set<NaturalLoop*> visited;

        std::function<void(NaturalLoop*)> dfs = [&](NaturalLoop* loop) {
            if (!loop || visited.count(loop)) return;

            visited.insert(loop);
            result.push_back(loop);

            if (static_cast<size_t>(loop->loop_id) < forest->loopG.size())
                for (auto* child : forest->loopG[loop->loop_id]) dfs(child);
        };

        for (auto* loop : forest->getRootLoops()) dfs(loop);

        return result;
    }

    void LoopAnalysisPass::identifyLoopPreheaders(CFG* cfg)
    {
        if (!cfg || !cfg->LoopForest) return;

        for (auto* loop : cfg->LoopForest->loop_set) { identifyLoopPreheader(cfg, loop); }
    }

    void LoopAnalysisPass::identifyLoopPreheader(CFG* cfg, NaturalLoop* loop)
    {
        if (!loop || !cfg) return;

        loop->preheader = nullptr;

        std::set<LLVMIR::IRBlock*> out_of_loop_predecessors;

        if (static_cast<size_t>(loop->header->block_id) < cfg->invG.size())
        {
            for (const auto& pred : cfg->invG[loop->header->block_id])
            {
                if (loop->loop_nodes.find(pred) == loop->loop_nodes.end()) { out_of_loop_predecessors.insert(pred); }
            }
        }

        if (out_of_loop_predecessors.empty()) return;

        if (out_of_loop_predecessors.size() == 1)
        {
            auto* candidate = *out_of_loop_predecessors.begin();
            if (isValidPreheader(cfg, candidate, loop))
            {
                loop->preheader = candidate;
                return;
            }
        }

        // 多个外前驱需在 LoopSimplifyPass 中处理为单前驱
    }

    bool LoopAnalysisPass::isValidPreheader(CFG* cfg, LLVMIR::IRBlock* candidate, NaturalLoop* loop) const
    {
        if (!candidate || !loop || !cfg) return false;

        if (loop->loop_nodes.find(candidate) != loop->loop_nodes.end()) return false;

        if (candidate->block_id == 0) return false;

        if (static_cast<size_t>(candidate->block_id) < cfg->G.size())
        {
            const auto& successors = cfg->G[candidate->block_id];
            if (successors.size() != 1 || successors[0] != loop->header) { return false; }
        }

        if (!candidate->insts.empty())
        {
            auto* last_inst = candidate->insts.back();
            if (last_inst->opcode != LLVMIR::IROpCode::BR_UNCOND) { return false; }
        }

        return true;
    }

}  // namespace Analysis
