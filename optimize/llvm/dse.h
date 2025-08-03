#pragma once

#include "cfg.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include "llvm/memdep/memdep.h"
#include "llvm/pass.h"
#include <unordered_map>
#include <unordered_set>

namespace LLVMIR
{
    class DSEPass : public Pass
    {
      private:
        Analysis::AliasAnalyser* alias_analyser;

        // 删除的store指令
        std::unordered_map<int, std::unordered_set<Instruction*> > erase_set;

        // 收集所有的store指令
        std::unordered_set<Instruction*> store_insts;

        std::unordered_set<Operand*> params;

        // 在一个cfg中执行
        void ExecuteInSingleCFG(CFG* cfg);

        // 在一个cfg中进行判断
        void GenerateElimination(CFG* cfg);

        // 在一个cfg中进行指令的更换
        void EraseStoreInst(CFG* cfg);

        // 收集所有的store
        void CollectStores(CFG* cfg);

        // 判断是否dead store
        bool isDeadStore(CFG* cfg, Instruction* store);

        bool allPathsGoThrough(CFG* cfg, int start, int end, int through);

        bool allPathsNoLoad(CFG* cfg, int start, int end, int through, Operand* ptr);

        bool pointsToGlobalOrEscapes(Operand* ptr, CFG* cfg);

        bool mayAlias(Operand* ptr1, Operand* ptr2, CFG* cfg);

        bool mustAlias(Operand* ptr1, Operand* ptr2, CFG* cfg);

      public:
        DSEPass(LLVMIR::IR* ir, Analysis::AliasAnalyser* aa) : Pass(ir), alias_analyser(aa){};
        void Execute() override;
    };
}  // namespace LLVMIR