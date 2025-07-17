#include "llvm/make_domtree.h"

void MakeDomTreePass::Execute()
{
    for (auto iter : ir->cfg)
    {
        Cele::Algo::DomAnalyzer* dom_tree = new Cele::Algo::DomAnalyzer;
        ir->DomTrees[iter.second]         = dom_tree;

        dom_tree->solve(iter.second->G_id, std::vector<int>{0}, false, true);
    }
}

/*
 * @brief reverse should be true
 */
void MakeDomTreePass::Execute(bool reverse)
{
    for (auto iter : ir->cfg)
    {
        Cele::Algo::DomAnalyzer* redom_tree = new Cele::Algo::DomAnalyzer;
        ir->ReDomTrees[iter.second]         = redom_tree;

        std::vector<int> exit_points;
        for (auto& [block_id, block] : iter.second->block_id_to_block)
            for (auto* inst : block->insts)
                if (inst->opcode == LLVMIR::IROpCode::RET)
                {
                    exit_points.push_back(block_id);
                    break;
                }

        redom_tree->solve(iter.second->G_id, exit_points, reverse, true);
    }
}