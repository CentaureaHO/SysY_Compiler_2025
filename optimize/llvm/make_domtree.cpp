#include "llvm/make_domtree.h"
#include <iostream>

void MakeDomTreePass::Execute()
{
    for (auto iter : ir->cfg)
    {
        Cele::Algo::DomAnalyzer* dom_tree = new Cele::Algo::DomAnalyzer;
        ir->DomTrees[iter.second]         = dom_tree;

        dom_tree->solve(iter.second->G_id, std::vector<int>{0}, false, true);
    }
}