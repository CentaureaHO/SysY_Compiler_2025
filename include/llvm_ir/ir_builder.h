#ifndef __LLVMIR_IRBUILDER_H__
#define __LLVMIR_IRBUILDER_H__

#include <vector>
#include <map>
#include <llvm_ir/instruction.h>
#include <llvm_ir/block.h>
#include <llvm_ir/function.h>

namespace Symbol
{
    class RegTable;
}

namespace LLVMIR
{
    class IRTable
    {
      public:
        Symbol::RegTable*           symTab;
        std::map<int, VarAttribute> regMap;
        std::map<int, bool>         formalArrTab;

        IRTable();
    };

    class IR
    {
      public:
        std::vector<Instruction*> global_def;
        std::vector<Instruction*> function_declare;
        std::vector<IRFunction*>  functions;
        // std::map<FuncDefInst*, std::map<int, IRBlock*>> function_block_map;

        IRFunction* cur_func;

      public:
        IR();
        ~IR();

        void registerLibraryFunctions();

        void enterFunc(IRFunction* func);

        IRBlock* createBlock();
        IRBlock* getBlock(int label);

        void printIR(std::ostream& s);
    };
}  // namespace LLVMIR

#endif