#ifndef __OPTIMIZER_LLVM_ALIAS_ANALYSIS_ALIAS_ANALYSIS_H__
#define __OPTIMIZER_LLVM_ALIAS_ANALYSIS_ALIAS_ANALYSIS_H__

#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"

namespace Analysis
{
    class MemLocation
    {
      public:
        // The set of base pointers (from alloca or globals) this location might refer to.
        std::unordered_set<LLVMIR::Operand*> targets;
        // True if the pointer escapes the function (e.g., returned, stored in global memory).
        bool escapes_function = false;
        // True if all targets are stack-allocated within the current function.
        bool is_stack_local = true;

        void addTarget(LLVMIR::Operand* op);
        void markEscaped()
        {
            escapes_function = true;
            is_stack_local   = false;
        }
        void markNonLocal() { is_stack_local = false; }
        bool isLocal() const { return is_stack_local && !escapes_function; }
        void merge(const MemLocation& other);
    };

    class FuncMemProfile
    {
      public:
        bool has_external_deps = false;
        // Stores the base pointers of memory locations that are read.
        std::unordered_set<LLVMIR::Operand*> mem_reads;
        // Stores the base pointers of memory locations that are written.
        std::unordered_set<LLVMIR::Operand*> mem_writes;

        void addRead(LLVMIR::Operand* op);
        void addWrite(LLVMIR::Operand* op);
        void addReads(const std::vector<LLVMIR::Operand*>& ops);
        void addWrites(const std::vector<LLVMIR::Operand*>& ops);
        void combineProfile(
            LLVMIR::CallInst* call, const FuncMemProfile& other, const std::unordered_map<int, MemLocation>& locations);

        bool isPure() const { return !has_external_deps && mem_reads.empty() && mem_writes.empty(); }
        bool hasNoWrites() const { return !has_external_deps && mem_writes.empty(); }
        bool readsMemory() const { return !mem_reads.empty() || has_external_deps; }
        bool writesMemory() const { return !mem_writes.empty() || has_external_deps; }
    };

    class AliasAnalyser
    {
      public:
        enum AliasResult
        {
            NoAlias   = 1,
            MayAlias  = 2,
            MustAlias = 3,
        };

        enum ModRefResult
        {
            NoModRef = 0,
            Ref      = 1,
            Mod      = 2,
            ModRef   = 3,
        };

      private:
        LLVMIR::IR* ir;

        std::unordered_map<CFG*, FuncMemProfile>                                func_profiles;
        std::unordered_map<CFG*, std::unordered_map<int, MemLocation>>          reg_locations;
        std::unordered_map<CFG*, std::unordered_map<int, LLVMIR::Instruction*>> def_map;

        void buildDefMap(CFG* cfg);
        void processFunction(CFG* cfg);
        void handlePtrPropagation(CFG* cfg, std::unordered_map<int, MemLocation>& locations);
        void collectMemAccesses(CFG* cfg, const std::unordered_map<int, MemLocation>& locations);
        bool checkSameBaseWithDistinctOffset(LLVMIR::Operand* p1, LLVMIR::Operand* p2, CFG* cfg);
        bool checkIdenticalGEP(LLVMIR::Operand* p1, LLVMIR::Operand* p2, CFG* cfg);

      public:
        AliasAnalyser(LLVMIR::IR* ir);

        void setLLVMIR(LLVMIR::IR* ir) { this->ir = ir; }

        void         run();
        AliasResult  queryAlias(LLVMIR::Operand* op1, LLVMIR::Operand* op2, CFG* cfg);
        ModRefResult queryInstModRef(LLVMIR::Instruction* inst, LLVMIR::Operand* op, CFG* cfg);

        bool isReadMem(CFG* cfg) { return func_profiles[cfg].readsMemory(); }
        bool isWriteMem(CFG* cfg) { return func_profiles[cfg].writesMemory(); }
        bool isIndependent(CFG* cfg) { return func_profiles[cfg].isPure(); }
        bool isNoSideEffect(CFG* cfg) { return func_profiles[cfg].hasNoWrites(); }
        bool haveExternalCall(CFG* cfg) { return func_profiles[cfg].has_external_deps; }
        bool isLocalPtr(CFG* cfg, LLVMIR::Operand* ptr);

        std::vector<LLVMIR::Operand*> getWritePtrs(CFG* cfg);
        std::vector<LLVMIR::Operand*> getReadPtrs(CFG* cfg);
    };
}  // namespace Analysis
#endif
