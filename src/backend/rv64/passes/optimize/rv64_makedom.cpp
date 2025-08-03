#include <backend/rv64/passes/optimize/rv64_makedom.h>
#include <backend/rv64/rv64_function.h>
#include <backend/rv64/rv64_cfg.h>
#include <backend/rv64/rv64_block.h>
#include <dom_analyzer.h>

namespace Backend::RV64::Passes::Optimize
{
    RV64MakeDomTreePass::RV64MakeDomTreePass(std::vector<Backend::RV64::Function*>& functions) : functions_(&functions)
    {}

    bool RV64MakeDomTreePass::run()
    {
        for (auto* function : *functions_)
        {
            if (function && function->cfg)
            {
                buildDominanceTree(function);
                buildPostDominanceTree(function);
            }
        }
        return true;
    }

    void RV64MakeDomTreePass::buildDominanceTree(Backend::RV64::Function* function)
    {
        CFG* cfg = function->cfg;
        if (!cfg || cfg->blocks.empty()) return;

        cfg->dom_tree = std::make_unique<Cele::Algo::DomAnalyzer>();

        std::vector<std::vector<int>> graph = cfg->buildGraphAdjacencyList();

        std::vector<int> entry_points;
        if (cfg->entry_block)
        {
            for (const auto& [block_id, block] : cfg->blocks)
            {
                if (block == cfg->entry_block)
                {
                    entry_points.push_back(block_id);
                    break;
                }
            }
        }

        if (entry_points.empty() && !cfg->blocks.empty()) { entry_points.push_back(cfg->blocks.begin()->first); }
        if (!entry_points.empty()) { cfg->dom_tree->solve(graph, entry_points, false, true); }
    }

    void RV64MakeDomTreePass::buildPostDominanceTree(Backend::RV64::Function* function)
    {
        CFG* cfg = function->cfg;
        if (!cfg || cfg->blocks.empty()) return;

        cfg->post_dom_tree = std::make_unique<Cele::Algo::DomAnalyzer>();

        std::vector<std::vector<int>> graph = cfg->buildGraphAdjacencyList();

        std::vector<int> exit_points;

        for (const auto& [block_id, block] : cfg->blocks)
        {
            if (block_id < graph.size() && graph[block_id].empty()) { exit_points.push_back(block_id); }
        }

        if (exit_points.empty() && cfg->ret_block)
        {
            for (const auto& [block_id, block] : cfg->blocks)
            {
                if (block == cfg->ret_block)
                {
                    exit_points.push_back(block_id);
                    break;
                }
            }
        }

        if (!exit_points.empty()) { cfg->post_dom_tree->solve(graph, exit_points, true, true); }
    }

}  // namespace Backend::RV64::Passes::Optimize
