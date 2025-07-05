#include "../include/cdg.h"
#include "cfg.h"

void CDGAnalyzer::Execute() { BuildCDG(); }

void CDGAnalyzer::BuildCDG()
{
    for (auto [defI, C] : ir->cfg) { BuildSingleCDG(C); }
}

void CDGAnalyzer::BuildSingleCDG(CFG* C)
{
    int size = C->block_id_to_block.size();
    CDG[C].resize(size);
    invCDG[C].resize(size);
    for (auto [id, bb] : C->block_id_to_block)
    {
        // 建立CDG图
        // 对于每个id的控制边界
        // std::cout << "Id is " << id << " has: ";

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