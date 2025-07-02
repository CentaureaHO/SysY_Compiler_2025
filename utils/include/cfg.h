#ifndef CFG_H
#define CFG_H

#include "llvm_ir/function.h"
#include <map>
#include <vector>

class CFG
{
  public:
    LLVMIR::IRFunction*             func;               // CFG对应函数
    std::map<int, LLVMIR::IRBlock*> block_id_to_block;  // 块id到块指针

    // 邻接表，用id索引到后继的指针 vector
    std::vector<std::vector<LLVMIR::IRBlock*>> G{};
    std::vector<std::vector<LLVMIR::IRBlock*>> invG{};

    std::vector<std::vector<int>> G_id{};  // 发现支配树是用这种形式构建的
    std::vector<std::vector<int>> invG_id{};

    CFG() { block_id_to_block.clear(); }
    void BuildCFG();
};

#endif