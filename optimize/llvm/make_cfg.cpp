#include "llvm/make_cfg.h"
#include <iostream>

void MakeCFGPass::Execute()
{
    // ir->BuildCFG();
    for (auto func : ir->functions)
    {

        CFG* new_cfg            = new CFG;
        ir->cfg[func->func_def] = new_cfg;
        new_cfg->func           = func;
        for (auto block : func->blocks) { new_cfg->block_id_to_block[block->block_id] = block; }
        // std::cout<<111<<std::endl;
        new_cfg->BuildCFG();
    }
}