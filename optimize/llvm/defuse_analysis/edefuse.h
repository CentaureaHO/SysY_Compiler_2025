#pragma once

#include "cfg.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include <unordered_map>
#include <unordered_set>
// #define DEBUG
namespace Analysis
{
    class EDefUseAnalysis
    {
      private:
        LLVMIR::IR* ir;
        //   通过一个operand找到其def
        std::unordered_map<CFG*, std::unordered_map<LLVMIR::Operand*, LLVMIR::Instruction*>> DefMaps;
        // 通过一个use找到对应的use指令
        std::unordered_map<CFG*, std::unordered_map<LLVMIR::Operand*, std::unordered_set<LLVMIR::Instruction*>>>
             UseMaps;
        void ExecuteInSingleCFG(CFG* cfg);

      public:
        EDefUseAnalysis(LLVMIR::IR* IR) : ir(IR) {}

        void                 run();
        LLVMIR::Instruction* getDef(LLVMIR::Operand* op) const
        {
            auto it = DefMaps.find(op);
            return (it != DefMaps.end()) ? it->second : nullptr;
        }
        std::unordered_set<LLVMIR::Instruction*> getUses(LLVMIR::Operand* op) const
        {
            auto it = UseMaps.find(op);
            return (it != UseMaps.end()) ? it->second : std::unordered_set<LLVMIR::Instruction*>();
        }
#ifdef DEBUG
        void print();
#endif
    };
}  // namespace Analysis