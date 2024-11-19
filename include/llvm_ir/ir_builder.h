#ifndef __LLVMIR_IRBUILDER_H__
#define __LLVMIR_IRBUILDER_H__

#include <vector>
#include <map>
#include <llvm_ir/instruction.h>
#include <llvm_ir/block.h>

namespace LLVMIR
{
    class IR
    {
      public:
        std::vector<Instruction*>                       global_def;
        std::vector<Instruction*>                       function_declare;
        std::map<FuncDefInst*, std::map<int, IRBlock*>> function_block_map;

        IR();

        void printIR(std::ostream& s);
    };
}  // namespace LLVMIR

#endif