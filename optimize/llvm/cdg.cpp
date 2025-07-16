#include "cdg.h"
#include "cfg.h"
#include <algorithm>

using namespace LLVMIR;

void CDGAnalyzer::Execute() { BuildCDG(); }

void CDGAnalyzer::BuildCDG()
{
    for (auto [defI, C] : ir->cfg) { BuildSingleCDG(C); }
}

void CDGAnalyzer::BuildSingleCDG(CFG* C)
{
    // std::cout<<"Building CDG for CFG: " << C->func->func_def->func_name << std::endl;
    auto cmp       = [](auto& a, auto& b) { return a.first < b.first; };
    int  max_block = std::max_element(C->block_id_to_block.begin(), C->block_id_to_block.end(), cmp)->first;
    int  size      = max_block + 1;
    // std::cout << "CDG size is " << size << std::endl;
    CDG[C].resize(size);
    invCDG[C].resize(size);
    for (auto [id, bb] : C->block_id_to_block)
    {
        // 建立CDG图
        // 对于每个id的控制边界
        auto ReDom = ir->ReDomTrees[C];
        for (auto cfg : ReDom->dom_frontier[id])
        {
            // 如果x在y的反支配树的支配边界上，那么说明x控制依赖y，即x ->  y
            CDG[C][cfg].push_back((C->block_id_to_block)[id]);
            invCDG[C][id].push_back((C->block_id_to_block)[cfg]);
        }
    }

    // for (auto [id, bb] : *C->block_map) {
    //     std::cout << "CDG size is " << CDG[C][bb->block_id].size() << std::endl;
    //     std::cout << "invCDG size is " << invCDG[C][bb->block_id].size() << std::endl;
    // }
}