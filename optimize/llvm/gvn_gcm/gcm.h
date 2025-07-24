#pragma once

#include "dom_analyzer.h"
#include "llvm/def_use.h"
#include "llvm_ir/instruction.h"
#include "llvm/pass.h"
#include "unordered_map"

namespace LLVMIR
{
    class GCM : Pass
    {
      private:
        Cele::Algo::DomAnalyzer* domAnalyzer;      // 支配关系分析器
        Cele::Algo::DomAnalyzer* postdomAnalyzer;  // 后支配关系分析器
        DefUseAnalysisPass*      defuseAnalysis;   // 定义使用分析

        // 记录指令最早和最迟的位置
        std::unordered_map<Instruction*, int> earliestBlockId;
        std::unordered_map<Instruction*, int> latestBlockId;

        bool IsSafeInst(Instruction* inst);

        int  ComputeEarliestBlockId(CFG* func_cfg, Instruction* inst);
        int  ComputeLatestBlockId(CFG* func_cfg, Instruction* inst);
        void MoveInstructions(CFG* func_cfg);

      public:
        GCM(LLVMIR::IR* ir, DefUseAnalysisPass* DefUseAnalysis) : Pass(ir) { defuseAnalysis = DefUseAnalysis; }

        void Execute() override;

        void ExecuteInSingleCFG(CFG* func_cfg);
    };
}  // namespace LLVMIR