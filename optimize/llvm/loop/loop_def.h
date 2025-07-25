#ifndef __OPTIMIZER_LLVM_LOOP_DEF_H__
#define __OPTIMIZER_LLVM_LOOP_DEF_H__

#include "llvm_ir/ir_block.h"
#include "llvm_ir/instruction.h"
#include <set>
#include <map>
#include <vector>
#include <memory>
#include <functional>

class CFG;

namespace LLVMIR
{
    class IR;
}

class NaturalLoop
{
  public:
    // Loop structure
    std::set<LLVMIR::IRBlock*> loop_nodes;           ///< All nodes in the loop
    std::set<LLVMIR::IRBlock*> exit_nodes;           ///< Nodes outside loop that have predecessors in loop
    std::set<LLVMIR::IRBlock*> exiting_nodes;        ///< Nodes in loop that have successors outside loop
    std::set<LLVMIR::IRBlock*> latches;              ///< Nodes with back edges to header
    LLVMIR::IRBlock*           header    = nullptr;  ///< Loop header (entry point)
    LLVMIR::IRBlock*           preheader = nullptr;  ///< Unique predecessor outside loop
    LLVMIR::IRBlock*           guard     = nullptr;  ///< Guard block for loop rotation
    int                        loop_id   = -1;       ///< Unique loop identifier

    // Loop hierarchy
    NaturalLoop* fa_loop = nullptr;  ///< Parent loop in nested structure

  public:
    NaturalLoop()  = default;
    ~NaturalLoop() = default;

    NaturalLoop(const NaturalLoop&)            = delete;
    NaturalLoop& operator=(const NaturalLoop&) = delete;
    NaturalLoop(NaturalLoop&&)                 = default;
    NaturalLoop& operator=(NaturalLoop&&)      = default;

    void findExitNodes(CFG* cfg);
    void simplify(CFG* cfg);
    bool isSimplified(CFG* cfg) const;

  private:
    void addPreheader(CFG* cfg);
    void insertSingleLatch(CFG* cfg);
    void insertDedicatedExits(CFG* cfg);
};

class NaturalLoopForest
{
  public:
    int                                      loop_cnt = 0;     ///< Total number of loops
    std::set<NaturalLoop*>                   loop_set;         ///< All loops in the forest
    std::map<LLVMIR::IRBlock*, NaturalLoop*> header_loop_map;  ///< Header -> Loop mapping

    std::vector<std::vector<NaturalLoop*>> loopG;  ///< Loop containment hierarchy (legacy name)

  public:
    NaturalLoopForest() = default;
    ~NaturalLoopForest();

    NaturalLoopForest(const NaturalLoopForest&)            = delete;
    NaturalLoopForest& operator=(const NaturalLoopForest&) = delete;
    NaturalLoopForest(NaturalLoopForest&&)                 = default;
    NaturalLoopForest& operator=(NaturalLoopForest&&)      = default;

    void combineSameHeadLoops();
    void buildHierarchy();

    void clear();

    std::vector<NaturalLoop*> getRootLoops() const;
};

LLVMIR::IRBlock*           insertTransferBlock(CFG* cfg, const std::set<LLVMIR::IRBlock*>& froms, LLVMIR::IRBlock* to);
std::set<LLVMIR::IRBlock*> findNodesInLoop(CFG* cfg, LLVMIR::IRBlock* n, LLVMIR::IRBlock* d);
bool                       judgeLoopContain(const NaturalLoop* l1, const NaturalLoop* l2);

#endif  // __OPTIMIZER_LLVM_LOOP_DEF_H__
