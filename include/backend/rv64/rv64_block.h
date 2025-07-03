#ifndef __BACKEND_RV64_BLOCK_H__
#define __BACKEND_RV64_BLOCK_H__

#include "rv64_defs.h"
#include "insts.h"
#include <deque>
#include <list>

namespace Backend::RV64
{
    class Function;

    class Block
    {
      public:
        Function* parent;

        int                     label_num;
        std::list<Instruction*> insts;

        std::map<Register, Operand*> phi_copy_map;

      public:
        Block(int id);
        ~Block();
    };
}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_BLOCK_H__
