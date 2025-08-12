#pragma once

#include "cfg.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "llvm/alias_analysis/arralias_analysis.h"
#include "llvm/alias_analysis/ealias_analysis.h"
#include "llvm/defuse_analysis/edefuse.h"
#include "llvm/loop/loop_def.h"
#include "llvm/pass.h"
#include <unordered_map>
#include <unordered_set>

namespace LLVMIR
{
    struct MemoryLocation
    {
        LLVMIR::Operand*              base_ptr;
        int                           element_offset;
        std::vector<LLVMIR::Operand*> indices;  // 存储GEP指令的索引

        MemoryLocation() : base_ptr(nullptr), element_offset(0) {}
        MemoryLocation(LLVMIR::Operand* ptr, int offset) : base_ptr(ptr), element_offset(offset) {}

        bool isValid() const { return base_ptr != nullptr; }
        bool operator==(const MemoryLocation& other) const
        {
            return base_ptr == other.base_ptr && element_offset == other.element_offset;
        }
    };

    class DSEPass : public Pass
    {
      private:
        EAliasAnalysis::EAliasAnalyser* ealias_analyser;

        Analysis::EDefUseAnalysis* edef_use_analysis;

        Analysis::ArrAliasAnalysis* arralias_analysis;

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

        bool allPathsNoLoad(CFG* cfg, int start, int through, Operand* ptr, Instruction* newstore);

        bool pointsToGlobalOrEscapes(Operand* ptr, CFG* cfg);

        // bool isInLoop(LLVMIR::IRBlock* block, CFG* cfg);

        int calculateTotalOffset(LLVMIR::Operand* ptr, CFG* cfg, LLVMIR::Operand* alloca);

        bool mayAlias(Operand* ptr1, Operand* ptr2, CFG* cfg);

        bool mustAlias(Operand* ptr1, Operand* ptr2, CFG* cfg);

      public:
        DSEPass(LLVMIR::IR* ir, EAliasAnalysis::EAliasAnalyser* eaa, Analysis::EDefUseAnalysis* edef,
            Analysis::ArrAliasAnalysis* arralias)
            : Pass(ir), ealias_analyser(eaa), edef_use_analysis(edef), arralias_analysis(arralias){};
        void Execute() override;
    };
}  // namespace LLVMIR