#ifndef __OPTIMIZER_LLVM_UTILS_BLOCK_LAYOUT_OPTIMIZATION_H__
#define __OPTIMIZER_LLVM_UTILS_BLOCK_LAYOUT_OPTIMIZATION_H__

#include "../pass.h"
#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>

namespace Transform
{
    class BlockLayoutOptimizationPass : public Pass
    {
      private:
        bool modified_;

      public:
        explicit BlockLayoutOptimizationPass(LLVMIR::IR* ir);
        void Execute() override;
        bool wasModified() const { return modified_; }

      private:
        void processFunction(CFG* cfg);

        class BlockChain
        {
          public:
            std::vector<LLVMIR::IRBlock*> blocks;

            void addBlock(LLVMIR::IRBlock* block) { blocks.push_back(block); }

            LLVMIR::IRBlock* getLastBlock() const { return blocks.empty() ? nullptr : blocks.back(); }

            LLVMIR::IRBlock* getFirstBlock() const { return blocks.empty() ? nullptr : blocks.front(); }

            bool contains(LLVMIR::IRBlock* block) const
            {
                return std::find(blocks.begin(), blocks.end(), block) != blocks.end();
            }

            size_t size() const { return blocks.size(); }

            void merge(const BlockChain& other)
            {
                blocks.insert(blocks.end(), other.blocks.begin(), other.blocks.end());
            }
        };

        std::vector<LLVMIR::IRBlock*> optimizeBlockLayout(CFG* cfg);
        void                          buildChains(CFG* cfg, std::vector<BlockChain>& chains);
        void                          buildSingleChain(
                                     LLVMIR::IRBlock* start_block, BlockChain& chain, std::set<LLVMIR::IRBlock*>& placed_blocks, CFG* cfg);
        LLVMIR::IRBlock* selectBestSuccessor(
            LLVMIR::IRBlock* block, const std::set<LLVMIR::IRBlock*>& placed_blocks, CFG* cfg);
        double calculateEdgeWeight(LLVMIR::IRBlock* from, LLVMIR::IRBlock* to, CFG* cfg);
        bool   shouldMergeChains(const BlockChain& chain1, const BlockChain& chain2, CFG* cfg);

        int calculateLayoutCost(const std::vector<LLVMIR::IRBlock*>& layout, CFG* cfg);
        int calculateJumpDistance(LLVMIR::IRBlock* from, LLVMIR::IRBlock* to,
            const std::unordered_map<int, int>& block_positions, const std::unordered_map<int, int>& block_sizes);

        void renumberBlocks(CFG* cfg, const std::vector<LLVMIR::IRBlock*>& new_layout);
        void updateBranchTargets(CFG* cfg, const std::unordered_map<int, int>& old_to_new_id);

        std::vector<LLVMIR::IRBlock*> getSuccessors(LLVMIR::IRBlock* block, CFG* cfg);
        std::vector<LLVMIR::IRBlock*> getPredecessors(LLVMIR::IRBlock* block, CFG* cfg);
        int                           getBlockInstructionCount(LLVMIR::IRBlock* block);
        bool                          isFallthrough(LLVMIR::IRBlock* from, LLVMIR::IRBlock* to);
        LLVMIR::IRBlock*              getFallthroughBlock(LLVMIR::IRBlock* block, CFG* cfg);
        LLVMIR::IRBlock*              getEntryBlock(CFG* cfg);
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_UTILS_BLOCK_LAYOUT_OPTIMIZATION_H__
