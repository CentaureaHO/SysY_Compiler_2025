#pragma once

#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include "llvm/defuse_analysis/edefuse.h"
#include "llvm/pass.h"
#include <unordered_set>
// #define DEBUG
namespace LLVMIR
{
    class UnusedArrEliminator : Pass
    {
      private:
        Analysis::EDefUseAnalysis* edefUseAnalysis;

        // 记录数组定义
        std::unordered_set<Operand*> array_defs;
        // 记录数组定义到指令的映射
        std::unordered_map<Operand*, Instruction*> array2def;
        // 记录被访问的数组
        std::unordered_set<Operand*> accessed_arrays;

        // 收集所有arr的定义
        void collectArrayDefinitions();

        // 标记所有的被访问的数组
        void markAccessedArrays();

        void eliminateDeadArrays();

      public:
        UnusedArrEliminator(IR* ir, Analysis::EDefUseAnalysis* EDefUseAnalysis)
            : Pass(ir), edefUseAnalysis(EDefUseAnalysis)
        {}

        void Execute() override;
    };
}  // namespace LLVMIR