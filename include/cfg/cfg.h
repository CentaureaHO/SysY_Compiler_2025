#ifndef __CFG_CFG_H__
#define __CFG_CFG_H__

#include <bitset>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <llvm_ir/instruction.h>
#include <llvm_ir/block.h>
#include <ast/basic_node.h>
#include <ast/expression.h>
#include <ast/statement.h>
#include <ast/helper.h>

class CFG
{
  public:
    LLVMIR::FuncDefInst*                       cur_func;
    std::map<int, LLVMIR::IRBlock*>*           block_map;
    std::vector<std::vector<LLVMIR::IRBlock*>> successorGraph;
    std::vector<std::vector<LLVMIR::IRBlock*>> reverseGraph;

    CFG(LLVMIR::FuncDefInst* f = nullptr);
};

#endif