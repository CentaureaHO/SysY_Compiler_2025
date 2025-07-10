#ifndef __OPTIMIZER_LLVM_ALIAS_ANALYSIS_ALIAS_ANALYSIS_H__
#define __OPTIMIZER_LLVM_ALIAS_ANALYSIS_ALIAS_ANALYSIS_H__

#include <cassert>
#include <vector>
#include <map>
#include <set>
#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"

namespace Analysis
{
    class PtrRegMemInfo
    {
      public:
        bool is_full_mem = false;
        bool is_local    = true;

        std::vector<LLVMIR::Operand*> possible_ptrs;

        // if the ptr op is new, return true.
        // else, return false.
        bool insertNewPtrs(LLVMIR::Operand* op, std::map<int, PtrRegMemInfo>& ptr_map, CFG* cfg);
        bool pushPossiblePtr(LLVMIR::Operand* op);

        void printDebugInfo();
    };

    class FunctionMemRWInfo
    {
      public:
        bool have_external_call = false;

        std::vector<LLVMIR::Operand*> read_ptrs;
        std::vector<LLVMIR::Operand*> write_ptrs;

        // if the ptr op is new, return true.
        // else, return false.
        bool insertNewReadPtrs(LLVMIR::Operand* op);
        bool insertNewWritePtrs(LLVMIR::Operand* op);
        bool insertNewReadPtrs(std::vector<LLVMIR::Operand*> ops);
        bool insertNewWritePtrs(std::vector<LLVMIR::Operand*> ops);
        bool mergeCall(LLVMIR::CallInst* call_inst, FunctionMemRWInfo& rw_info, std::map<int, PtrRegMemInfo>& ptr_map);

        bool isIndependent() { return (!have_external_call) && read_ptrs.empty() && write_ptrs.empty(); }
        bool isNoSideEffect() { return (!have_external_call) && write_ptrs.empty(); }
        bool isReadMem() { return !read_ptrs.empty() || have_external_call; }
        bool isWriteMem() { return !write_ptrs.empty() || have_external_call; }
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
        LLVMIR::IR*                                         ir;
        std::map<CFG*, FunctionMemRWInfo>                   cfg_mem_rw_map;
        std::map<CFG*, std::map<int, PtrRegMemInfo>>        ptr_reg_mem_map;
        std::map<CFG*, std::map<int, LLVMIR::Instruction*>> cfg_result_map;

        void analyzeCFG(CFG* cfg);
        bool isSamePtrWithDiffConstIndex(LLVMIR::Operand* p1, LLVMIR::Operand* p2, CFG* cfg);

      public:
        AliasAnalyser(LLVMIR::IR* ir);

        void setLLVMIR(LLVMIR::IR* ir) { this->ir = ir; }

        void         run();
        AliasResult  queryAlias(LLVMIR::Operand* op1, LLVMIR::Operand* op2, CFG* cfg);
        ModRefResult queryInstModRef(LLVMIR::Instruction* inst, LLVMIR::Operand* op, CFG* cfg);

        bool isReadMem(CFG* cfg) { return cfg_mem_rw_map[cfg].isReadMem(); }
        bool isWriteMem(CFG* cfg) { return cfg_mem_rw_map[cfg].isWriteMem(); }
        bool isIndependent(CFG* cfg) { return cfg_mem_rw_map[cfg].isIndependent(); }
        bool isNoSideEffect(CFG* cfg) { return cfg_mem_rw_map[cfg].isNoSideEffect(); }
        bool haveExternalCall(CFG* cfg) { return cfg_mem_rw_map[cfg].have_external_call; }
        bool isLocalPtr(CFG* cfg, LLVMIR::Operand* ptr);

        std::vector<LLVMIR::Operand*> getWritePtrs(CFG* cfg) { return cfg_mem_rw_map.at(cfg).write_ptrs; }
        std::vector<LLVMIR::Operand*> getReadPtrs(CFG* cfg) { return cfg_mem_rw_map.at(cfg).read_ptrs; }
    };
}  // namespace Analysis
#endif
