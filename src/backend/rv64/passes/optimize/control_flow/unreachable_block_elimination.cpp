#include <backend/rv64/passes/optimize/control_flow/unreachable_block_elimination.h>
#include <iostream>
#include <algorithm>

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    UnreachableBlockEliminationPass::UnreachableBlockEliminationPass(std::vector<Function*>& functions)
        : functions_(functions), eliminated_blocks_count_(0)
    {}

    bool UnreachableBlockEliminationPass::run()
    {
        bool modified            = false;
        eliminated_blocks_count_ = 0;

        for (auto* func : functions_)
        {
            if (optimizeFunction(func)) { modified = true; }
        }

        if (eliminated_blocks_count_ > 0)
        {
            // std::cerr << "Eliminated " << eliminated_blocks_count_ << " unreachable blocks" << std::endl;
        }

        return modified;
    }

    bool UnreachableBlockEliminationPass::optimizeFunction(Function* func)
    {
        if (func == nullptr || func->cfg == nullptr || func->cfg->blocks.empty()) return false;

        std::unordered_set<int> reachable_blocks = findReachableBlocks(func);

        return removeUnreachableBlocks(func, reachable_blocks);
    }

    std::unordered_set<int> UnreachableBlockEliminationPass::findReachableBlocks(Function* func)
    {
        std::unordered_set<int> reachable;
        std::queue<int>         worklist;

        int entry_label = 0;
        if (func->cfg->blocks.find(entry_label) != func->cfg->blocks.end()) worklist.push(entry_label);

        while (!worklist.empty())
        {
            int current = worklist.front();
            worklist.pop();

            if (reachable.find(current) != reachable.end()) continue;

            reachable.insert(current);

            if (current < static_cast<int>(func->cfg->graph.size()))
            {
                for (Block* successor : func->cfg->graph[current])
                {
                    if (successor != nullptr && reachable.find(successor->label_num) == reachable.end())
                    {
                        worklist.push(successor->label_num);
                    }
                }
            }
        }

        return reachable;
    }

    bool UnreachableBlockEliminationPass::removeUnreachableBlocks(
        Function* func, const std::unordered_set<int>& reachable_blocks)
    {
        bool                    modified = false;
        std::unordered_set<int> removed_blocks;

        std::vector<int> blocks_to_remove;
        for (const auto& [label, block] : func->cfg->blocks)
        {
            if (reachable_blocks.find(label) == reachable_blocks.end()) { blocks_to_remove.push_back(label); }
        }

        for (int label : blocks_to_remove)
        {
            auto it = func->cfg->blocks.find(label);
            if (it != func->cfg->blocks.end())
            {
                Block* block = it->second;

                func->cfg->blocks.erase(it);

                auto vec_it = std::find(func->blocks.begin(), func->blocks.end(), block);
                if (vec_it != func->blocks.end()) { func->blocks.erase(vec_it); }

                delete block;

                removed_blocks.insert(label);
                eliminated_blocks_count_++;
                modified = true;
            }
        }

        if (modified) updateCFGAfterRemoval(func, removed_blocks);

        return modified;
    }

    void UnreachableBlockEliminationPass::updateCFGAfterRemoval(
        Function* func, const std::unordered_set<int>& removed_blocks)
    {
        for (int removed_label : removed_blocks)
        {
            if (removed_label < static_cast<int>(func->cfg->graph.size())) func->cfg->graph[removed_label].clear();
        }

        for (size_t from_label = 0; from_label < func->cfg->graph.size(); ++from_label)
        {
            auto& successors = func->cfg->graph[from_label];
            successors.erase(std::remove_if(successors.begin(),
                                 successors.end(),
                                 [&removed_blocks](Block* block) {
                                     return block != nullptr &&
                                            removed_blocks.find(block->label_num) != removed_blocks.end();
                                 }),
                successors.end());
        }

        for (int removed_label : removed_blocks)
        {
            if (removed_label < static_cast<int>(func->cfg->inv_graph.size()))
            {
                func->cfg->inv_graph[removed_label].clear();
            }
        }

        for (size_t to_label = 0; to_label < func->cfg->inv_graph.size(); ++to_label)
        {
            auto& predecessors = func->cfg->inv_graph[to_label];
            predecessors.erase(std::remove_if(predecessors.begin(),
                                   predecessors.end(),
                                   [&removed_blocks](Block* block) {
                                       return block != nullptr &&
                                              removed_blocks.find(block->label_num) != removed_blocks.end();
                                   }),
                predecessors.end());
        }

        func->cfg->dom_tree.reset();
        func->cfg->post_dom_tree.reset();
        func->cfg->loop_forest.reset();
    }

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow
