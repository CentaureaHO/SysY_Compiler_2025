#include "llvm/make_cfg.h"

void MakeCFGPass::Execute()
{
    // ir->BuildCFG();
    ir->cfg.clear();
    for (auto func : ir->functions)
    {
        CFG* new_cfg            = new CFG;
        ir->cfg[func->func_def] = new_cfg;
        new_cfg->func           = func;
        for (auto block : func->blocks) { new_cfg->block_id_to_block[block->block_id] = block; }
        new_cfg->BuildCFG();
    }
}