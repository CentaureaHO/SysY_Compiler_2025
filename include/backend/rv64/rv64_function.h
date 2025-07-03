#ifndef __BACKEND_RV64_FUNCTION_H__
#define __BACKEND_RV64_FUNCTION_H__

#include "rv64_defs.h"
#include "rv64_cfg.h"
#include <string>
#include <vector>

namespace Backend::RV64
{
    class Unit;
    class Block;

    class Function
    {
      public:
        Unit* unit;

        std::string           name;
        std::vector<Register> params;
        int                   cur_max_label;

        int  stack_size;
        int  param_size;
        bool has_stack_param;

        CFG*                cfg;
        std::vector<Block*> blocks;

        std::vector<RV64Inst*> alloc_insts;

      public:
        Function(std::string n, CFG* c = nullptr);
        ~Function();

      public:
        Register getNewReg(DataType* type);
        Block*   getNewBlock();
    };
};  // namespace Backend::RV64

#endif  // __BACKEND_RV64_FUNCTION_H__
