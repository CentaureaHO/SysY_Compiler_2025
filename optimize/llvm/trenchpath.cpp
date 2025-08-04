#include "trenchpath.h"
#include "cfg.h"
#include "llvm_ir/defs.h"

namespace LLVMIR
{
    void TrenchPath::Execute()
    {
        for (auto [func, cfg] : ir->cfg) { ExecuteInSingleCFG(cfg); }
    }

    void TrenchPath::ExecuteInSingleCFG(CFG* cfg)
    {
        for (auto [id, block] : cfg->block_id_to_block) {
            auto inst = block->insts.front();
            if (inst->opcode == IROpCode::BR_UNCOND) {
                // Handle unconditional branch
                // 只有bruncond的块实际上是没必要的
                
            }
        }
    }
}  // namespace LLVMIR