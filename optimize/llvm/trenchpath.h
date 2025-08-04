#pragma once

#include "llvm_ir/ir_builder.h"
#include "llvm/pass.h"
#include <map>
// #define DEBUG_TRENCH_PATH

namespace LLVMIR
{
    class TrenchPath : Pass
    {
      private:
        std::map<int, int> replace_map;   // 用于记录需要替换的块
        std::unordered_set<int>      erase_blocks;  // 用于记录需要删除的块
        void               ExecuteInSingleCFG(CFG* cfg);
        int                TraceToBlock(int block_id);

      public:
        TrenchPath(IR* ir) : Pass(ir){};
        void Execute() override;
    };
}  // namespace LLVMIR