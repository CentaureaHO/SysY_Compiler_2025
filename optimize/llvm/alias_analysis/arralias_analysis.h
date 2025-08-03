#pragma once
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include "unordered_map"
// #define DEBUG
namespace Analysis
{
    class ArrAliasAnalysis
    {
      private:
        LLVMIR::IR* ir;
        // 用于存储数组别名分析的结果
        std::unordered_map<CFG*, std::unordered_map<LLVMIR::Operand*, LLVMIR::Operand*>> array_alias;
        // 存储所有的数组
        std::unordered_map<CFG*, std::unordered_set<LLVMIR::Operand*>> array_operands;

        // 单个cfg处理
        void processSingleCFG(CFG* cfg);
        void processPHI(CFG* cfg);

      public:
        ArrAliasAnalysis(LLVMIR::IR* ir) : ir(ir) {}
        void run();
        // 用于获取一个Operand的数组别名分析，如果找到了base，
        // 那么我们返回对应的alloca的寄存器，标记为use
        LLVMIR::Operand* traceToBase(CFG* cfg, LLVMIR::Operand* op);

#ifdef DEBUG
        void print();
#endif
    };
}  // namespace Analysis