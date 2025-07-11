#ifndef __OPTIMIZER_LLVM_MEMDEP_MEMDEP_H__
#define __OPTIMIZER_LLVM_MEMDEP_MEMDEP_H__

#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include <map>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace Analysis
{
    class MemoryDef;
    class MemoryUse;
    class MemoryPhi;
    class MemoryAccess;

    class MemorySSA
    {
      public:
        enum MemoryAccessType
        {
            DEF,
            USE,
            PHI
        };

        struct MemoryLocation
        {
            LLVMIR::Operand* ptr;
            LLVMIR::DataType size;
            bool             operator<(const MemoryLocation& other) const
            {
                if (ptr != other.ptr) return ptr < other.ptr;
                return size < other.size;
            }
        };

        class MemoryAccess
        {
          public:
            MemoryAccessType           type;
            LLVMIR::Instruction*       inst;
            MemoryLocation             location;
            int                        block_id;
            int                        inst_id;
            MemoryAccess*              defining_access;
            std::vector<MemoryAccess*> uses;

            MemoryAccess(MemoryAccessType t, LLVMIR::Instruction* i = nullptr)
                : type(t), inst(i), block_id(-1), inst_id(-1), defining_access(nullptr)
            {}
        };

        class MemoryDef : public MemoryAccess
        {
          public:
            MemoryDef(LLVMIR::Instruction* i) : MemoryAccess(DEF, i) {}
        };

        class MemoryUse : public MemoryAccess
        {
          public:
            MemoryUse(LLVMIR::Instruction* i) : MemoryAccess(USE, i) {}
        };

        class MemoryPhi : public MemoryAccess
        {
          public:
            std::vector<std::pair<int, MemoryAccess*>> incoming_values;
            MemoryPhi(int bb_id) : MemoryAccess(PHI, nullptr) { block_id = bb_id; }
        };

      private:
        LLVMIR::IR*    ir;
        AliasAnalyser* alias_analyser;

        std::map<CFG*, std::map<int, std::vector<MemoryAccess*>>>            bb_memory_accesses;
        std::map<CFG*, std::map<int, MemoryPhi*>>                            bb_memory_phis;
        std::map<CFG*, std::map<MemoryLocation, std::vector<MemoryAccess*>>> location_accesses;
        std::map<LLVMIR::Instruction*, MemoryAccess*>                        inst_to_access;

        void           buildMemorySSA(CFG* cfg);
        void           insertMemoryPhis(CFG* cfg);
        void           renameMemoryAccesses(CFG* cfg);
        void           processBlock(CFG* cfg, int bb_id, std::map<MemoryLocation, MemoryAccess*>& current_def);
        MemoryLocation getMemoryLocation(LLVMIR::Instruction* inst);
        bool           mayAlias(const MemoryLocation& loc1, const MemoryLocation& loc2, CFG* cfg);
        MemoryAccess*  getDefiningAccess(LLVMIR::Instruction* inst, CFG* cfg);

      public:
        MemorySSA(LLVMIR::IR* ir, AliasAnalyser* aa);
        void run();
        bool isLoadSameMemory(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg);
        bool isDepend(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg);
        bool haveMemAccessBetween(LLVMIR::Instruction* start, LLVMIR::Instruction* end, CFG* cfg);
    };

    class MemoryDependenceAnalyser
    {
      private:
        LLVMIR::IR* ir;
        MemorySSA*  memory_ssa;

      public:
        MemoryDependenceAnalyser(LLVMIR::IR* ir, AliasAnalyser* aa);
        ~MemoryDependenceAnalyser();

        void setIR(LLVMIR::IR* ir) { this->ir = ir; }

        void run();

        bool isDepend(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg);
        bool haveMemAccessBetween(LLVMIR::Instruction* start, LLVMIR::Instruction* end, CFG* cfg);
        bool isLoadSameMemory(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg);
    };
}  // namespace Analysis

#endif