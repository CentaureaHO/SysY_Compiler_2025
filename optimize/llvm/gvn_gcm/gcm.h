#pragma once

#include "dom_analyzer.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include "llvm/cdg.h"
#include "llvm/defuse_analysis/edefuse.h"
#include "llvm/global_analysis/readonly.h"
#include "llvm/memdep/memdep.h"
#include "llvm_ir/instruction.h"
#include "llvm/pass.h"
#include <unordered_map>
#include <unordered_set>
#include "unordered_map"

namespace LLVMIR
{
    class GCM : Pass
    {
      private:
        Cele::Algo::DomAnalyzer* domAnalyzer;      // 支配关系分析器
        Cele::Algo::DomAnalyzer* postdomAnalyzer;  // 后支配关系分析器
        // DefUseAnalysisPass*      defuseAnalysis;   // 定义使用分析
        // 更新后的
        Analysis::EDefUseAnalysis* defuseAnalysis;  // 定义使用分析

        // 别名分析
        Analysis::AliasAnalyser* aliasAnalyser;  // 别名分析器
        // 内存依赖分析
        Analysis::MemoryDependenceAnalyser* memdep;  // 内存依赖分析器
        // 只读全局变量
        Analysis::ReadOnlyGlobalAnalysis* readOnlyGlobalAnalysis;  // 只读全局变量分析
        // 控制依赖图
        CDGAnalyzer* cdgAnalyzer;

        std::unordered_set<Instruction*> erase_set;                            // 用于存储需要删除的指令
        std::unordered_map<int, std::multimap<int, Instruction*>> latest_map;  // 用于存储每个块的最新指令队列
        std::unordered_map<Instruction*, int> instorder;                       // 用于存储指令的顺序

        std::unordered_set<Operand*> params;

        std::unordered_set<Operand*> PhiVals;

        // 记录指令最早和最迟的位置
        std::unordered_map<Instruction*, int> earliestBlockId;
        std::unordered_map<Instruction*, int> latestBlockId;

        bool IsSafeInst(CFG* cfg, Instruction* inst);

        // 判断是否满足控制依赖
        bool isControlDependent(CFG* cfg, Instruction* inst, int target_id);

        // 收集phi的vals
        void CollectPhiVals(CFG* cfg);

        int  ComputeEarliestBlockId(CFG* func_cfg, Instruction* inst);
        int  ComputeLatestBlockId(CFG* func_cfg, Instruction* inst);
        void EraseInstructions(CFG* func_cfg);
        void MoveInstructions(CFG* func_cfg);

        void ExecuteInSingleCFG(CFG* func_cfg);

        // 生成辅助信息
        void GenerateInformation(CFG* func_cfg);

        // 判断call是否安全，比较简单
        bool isSafeCall(Instruction* inst);

      public:
        GCM(LLVMIR::IR* ir, Analysis::EDefUseAnalysis* DefUseAnalysis, Analysis::AliasAnalyser* AliasAnalyser,
            Analysis::MemoryDependenceAnalyser* MemoryDependenceAnalyser,
            Analysis::ReadOnlyGlobalAnalysis* ReadOnlyGlobalAnalysis, CDGAnalyzer* CDGAnalyzer)
            : Pass(ir)
        {
            defuseAnalysis         = DefUseAnalysis;
            aliasAnalyser          = AliasAnalyser;
            memdep                 = MemoryDependenceAnalyser;
            readOnlyGlobalAnalysis = ReadOnlyGlobalAnalysis;
            cdgAnalyzer            = CDGAnalyzer;
        }

        void Execute() override;
    };
}  // namespace LLVMIR