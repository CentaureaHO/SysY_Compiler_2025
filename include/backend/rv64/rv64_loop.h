#ifndef __BACKEND_RV64_LOOP_H__
#define __BACKEND_RV64_LOOP_H__

#include "rv64_block.h"
#include "rv64_cfg.h"
#include <set>
#include <map>
#include <vector>
#include <memory>

namespace Backend::RV64
{
    class NaturalLoop
    {
      public:
        // Loop structure
        std::set<Block*> loop_nodes;           ///< All blocks in the loop
        std::set<Block*> exit_nodes;           ///< Blocks outside loop that have predecessors in loop
        std::set<Block*> exiting_nodes;        ///< Blocks in loop that have successors outside loop
        std::set<Block*> latches;              ///< Blocks with back edges to header
        Block*           header    = nullptr;  ///< Loop header (entry point)
        Block*           preheader = nullptr;  ///< Unique predecessor outside loop
        int              loop_id   = -1;       ///< Unique loop identifier

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
        void addPreheader(CFG* cfg, Function* func);
        bool isSimplified() const;

      private:
        void insertSingleLatch(CFG* cfg, Function* func);
        void insertDedicatedExits(CFG* cfg, Function* func);
    };

    class NaturalLoopForest
    {
      public:
        int                                    loop_cnt = 0;
        std::set<NaturalLoop*>                 loop_set;
        std::map<Block*, NaturalLoop*>         header_loop_map;
        std::vector<std::vector<NaturalLoop*>> loopG;  ///< Loop containment hierarchy

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

    // Helper functions
    Block*           insertTransferBlock(CFG* cfg, Function* func, const std::set<Block*>& froms, Block* to);
    std::set<Block*> findNodesInLoop(CFG* cfg, Block* latch, Block* header);
    bool             judgeLoopContain(const NaturalLoop* outer, const NaturalLoop* inner);
    int              getBlockLoopDepth(Block* block, NaturalLoopForest* forest);

}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_LOOP_H__
