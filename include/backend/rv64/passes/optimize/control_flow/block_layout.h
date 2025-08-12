#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_BLOCK_LAYOUT_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_BLOCK_LAYOUT_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include "../../../rv64_cfg.h"
#include "../../../rv64_block.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    class BlockLayoutPass : public BasePass
    {
      public:
        explicit BlockLayoutPass(std::vector<Function*>& functions);
        ~BlockLayoutPass() override = default;

        bool        run() override;
        const char* getName() const override { return "BlockLayout"; }

      private:
        std::vector<Function*>& functions_;

        bool optimizeFunctionLayout(Function* func);

        class BlockChain
        {
          public:
            std::vector<Block*> blocks;

            void   addBlock(Block* block) { blocks.push_back(block); }
            Block* getLastBlock() const { return blocks.empty() ? nullptr : blocks.back(); }
            Block* getFirstBlock() const { return blocks.empty() ? nullptr : blocks.front(); }
            bool   contains(Block* block) const;
            size_t size() const { return blocks.size(); }
            void   merge(const BlockChain& other);
        };

        std::vector<Block*> optimizeBlockLayout(CFG* cfg);
        void                buildChains(CFG* cfg, std::vector<BlockChain>& chains);
        void buildSingleChain(Block* start_block, BlockChain& chain, std::set<Block*>& placed_blocks, CFG* cfg);

        void renumberBlocks(Function* func, const std::vector<Block*>& new_layout);
        void updateInstructionLabels(Function* func, const std::unordered_map<int, int>& label_mapping);
        void updateCFGBlockMapping(CFG* cfg, const std::unordered_map<int, int>& label_mapping);

        Block* selectBestSuccessor(Block* block, const std::set<Block*>& placed_blocks, CFG* cfg);
        double calculateEdgeWeight(Block* from, Block* to, CFG* cfg);
        int    calculateLayoutCost(const std::vector<Block*>& layout, CFG* cfg);
        int    calculateJumpDistance(Block* from, Block* to, const std::unordered_map<int, int>& block_positions,
               const std::unordered_map<int, int>& block_sizes);

        std::vector<Block*> getSuccessors(Block* block, CFG* cfg);
        std::vector<Block*> getPredecessors(Block* block, CFG* cfg);
        int                 getBlockInstructionCount(Block* block);
        bool                isFallthrough(Block* from, Block* to);
        Block*              getFallthroughBlock(Block* block, CFG* cfg);
        Block*              getEntryBlock(CFG* cfg);

        bool                      hasConditionalJumpPattern(Block* block);
        bool                      hasUnconditionalBranch(Block* block);
        bool                      isReturn(Block* block);
        std::pair<Block*, Block*> getConditionalTargets(Block* block, CFG* cfg);
    };

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_BLOCK_LAYOUT_H__
