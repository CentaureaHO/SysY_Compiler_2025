#ifndef __OPTIMIZER_LLVM_MEMDEP_MEMDEP_H__
#define __OPTIMIZER_LLVM_MEMDEP_MEMDEP_H__

#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include <map>
#include <vector>

namespace Analyser
{
    class MemoryDependenceAnalyser
    {
      private:
        LLVMIR::IR*                                                      ir;
        AliasAnalyser*                                                   alias_analyser;
        std::map<CFG*, std::vector<LLVMIR::Instruction*>>                cfg_store_inst_map;
        std::map<CFG*, std::map<int, std::vector<LLVMIR::Instruction*>>> result_used_map;

      public:
        MemoryDependenceAnalyser(LLVMIR::IR* ir, AliasAnalyser* aa);
        ~MemoryDependenceAnalyser() = default;

        void setIR(LLVMIR::IR* ir) { this->ir = ir; }

        void run();

        // return if I2 depends on I1(if I2 is after I1 in one BB)
        // now we only consider two memory access instructions
        bool isDepend(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg);

        bool haveMemAccessBetween(LLVMIR::Instruction* start, LLVMIR::Instruction* end, CFG* cfg);

        bool isLoadSameMemory(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg);

        void memDepTest();
    };
}  // namespace Analyser

#endif