#pragma once

#include "llvm_ir/ir_builder.h"
#include "llvm/pass.h"
#include <unordered_map>
#include <unordered_set>
// #define DEBUG_TRENCH_PATH

namespace LLVMIR
{
    class TrenchPath : Pass
    {
      private:
        std::unordered_map<int, int> replace_map;   // 用于记录需要替换的块
        std::unordered_set<int>      erase_blocks;  // 用于记录需要删除的块
        std::unordered_set<int>      phi_deps;      // 用于记录Phi依赖
        void                         CollectPHIDeps(CFG* cfg);
        void                         ExecuteInSingleCFG(CFG* cfg);
        int                          TraceToBlock(int block_id);

      public:
        TrenchPath(IR* ir) : Pass(ir) {};
        void Execute() override;
    };
}  // namespace LLVMIR