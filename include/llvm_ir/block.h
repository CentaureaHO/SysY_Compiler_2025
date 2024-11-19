#ifndef __LLVMIR_BLOCK_H__
#define __LLVMIR_BLOCK_H__

#include <deque>
#include <iostream>
#include <set>
#include <vector>
#include <llvm_ir/instruction.h>
#include <llvm_ir/defs.h>

namespace LLVMIR
{
    class IRBlock
    {
      public:
        std::string comment;
        int         block_id = 0;

        std::deque<Instruction*> insts;

        IRBlock(int id);

        void printIR(std::ostream& s);
    };
}  // namespace LLVMIR

#endif