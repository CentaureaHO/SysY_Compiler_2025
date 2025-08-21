#include <backend/rv64/passes/rv64_loop_find.h>
#include <iostream>
#include <queue>
#include <algorithm>

namespace Backend::RV64::Passes
{

    LoopFindPass::LoopFindPass(std::vector<Function*>& functions) : functions_(functions) {}

    bool LoopFindPass::run()
    {
        for (auto* func : functions_)
        {
            if (func && func->cfg) { findLoopsInFunction(func); }
        }
        return true;
    }

    void LoopFindPass::findLoopsInFunction(Function* func)
    {
        if (!func->cfg) return;

        if (!func->cfg->loop_forest) { func->cfg->loop_forest = std::make_unique<NaturalLoopForest>(); }
        else { func->cfg->loop_forest->clear(); }

        auto* loop_forest = func->cfg->loop_forest.get();

        std::vector<std::pair<Block*, Block*>> back_edges;
        findBackEdges(func->cfg, back_edges);

        for (const auto& [latch, header] : back_edges)
        {
            auto* loop = createLoopFromBackEdge(func->cfg, latch, header);
            if (loop)
            {
                loop->loop_id = loop_forest->loop_cnt++;
                loop_forest->loop_set.insert(loop);
            }
        }

        loop_forest->combineSameHeadLoops();

        loop_forest->buildHierarchy();

        for (auto* loop : loop_forest->loop_set) { loop->findExitNodes(func->cfg); }
    }

    void LoopFindPass::findBackEdges(CFG* cfg, std::vector<std::pair<Block*, Block*>>& back_edges)
    {
        std::map<Block*, int>    dfn;
        std::map<Block*, Block*> parent;
        buildDFSTree(cfg, dfn, parent);

        for (const auto& [block_id, block] : cfg->blocks)
        {
            if (static_cast<size_t>(block_id) >= cfg->graph.size()) continue;

            for (auto* successor : cfg->graph[block_id])
            {
                if (!successor) continue;

                if (dfn.count(successor) && dfn.count(block) && dfn[successor] <= dfn[block])
                {
                    Block* current     = block;
                    bool   is_ancestor = false;
                    while (parent.count(current) && parent[current])
                    {
                        if (parent[current] == successor)
                        {
                            is_ancestor = true;
                            break;
                        }
                        current = parent[current];
                    }

                    if (is_ancestor || successor == block) { back_edges.emplace_back(block, successor); }
                }
            }
        }
    }

    NaturalLoop* LoopFindPass::createLoopFromBackEdge(CFG* cfg, Block* latch, Block* header)
    {
        if (!latch || !header) return nullptr;

        auto* loop   = new NaturalLoop();
        loop->header = header;
        loop->latches.insert(latch);

        loop->loop_nodes = findNodesInLoop(cfg, latch, header);

        return loop;
    }

    void LoopFindPass::buildDFSTree(CFG* cfg, std::map<Block*, int>& dfn, std::map<Block*, Block*>& parent)
    {
        dfn.clear();
        parent.clear();

        std::set<Block*> visited;
        int              time = 0;

        if (cfg->entry_block)
        {
            parent[cfg->entry_block] = nullptr;
            dfsVisit(cfg->entry_block, cfg, time, dfn, parent, visited);
        }
    }

    void LoopFindPass::dfsVisit(Block* block, CFG* cfg, int& time, std::map<Block*, int>& dfn,
        std::map<Block*, Block*>& parent, std::set<Block*>& visited)
    {
        if (visited.count(block)) return;

        visited.insert(block);
        dfn[block] = time++;

        int block_id = block->label_num;
        if (static_cast<size_t>(block_id) >= cfg->graph.size()) return;

        for (auto* successor : cfg->graph[block_id])
        {
            if (!successor) continue;

            if (!visited.count(successor))
            {
                parent[successor] = block;
                dfsVisit(successor, cfg, time, dfn, parent, visited);
            }
        }
    }

}  // namespace Backend::RV64::Passes
