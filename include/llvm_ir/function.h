#ifndef __LLVMIR_FUNCTION_H__
#define __LLVMIR_FUNCTION_H__

#include <llvm_ir/defs.h>
#include <llvm_ir/block.h>
#include <llvm_ir/instruction.h>
#include <common/type/symtab/semantic_table.h>
#include <vector>

namespace LLVMIR
{
    class IRFunction
    {
      public:
        Type*        ret_type;
        FuncDefInst* func_def;

      public:
        std::vector<IRBlock*>       blocks;
        Symbol::RegTable            symTab;
        std::map<int, VarAttribute> regMap;
        std::set<int>               formalArrTab;

        int cur_label;
        int max_label;

        int loop_start_label;
        int loop_end_label;

        int max_reg;

      public:
        IRFunction(Type* rt, FuncDefInst* fd);

        inline IRBlock* createBlock();
        inline IRBlock* getBlock(int label);
    };
}  // namespace LLVMIR

#endif