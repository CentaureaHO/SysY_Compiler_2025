#include "llvm/setid.h"
namespace LLVMIR
{
    void SetIdAnalysis::Execute()
    {
        for (auto& [id, func_cfg] : ir->cfg) { ExecuteInSingleCFG(func_cfg); }
    }

    void SetIdAnalysis::ExecuteInSingleCFG(CFG* func_cfg)
    {
        // 遍历每个块，设置块的 ID
        for (auto& [id, block] : func_cfg->block_id_to_block)
        {
            for (auto& inst : block->insts)
            {
                inst->block_id = id;  // 设置指令的块 ID
            }
        }
    }
}  // namespace LLVMIR