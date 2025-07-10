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

        redom_tree->solve(iter.second->G_id, std::vector<int>{0}, reverse, true);
    }
}