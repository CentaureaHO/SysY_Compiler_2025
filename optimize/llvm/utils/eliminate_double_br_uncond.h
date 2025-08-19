#ifndef __OPTIMIZER_LLVM_UTILS_ELIMINATE_DOUBLE_BR_UNCOND_H__
#define __OPTIMIZER_LLVM_UTILS_ELIMINATE_DOUBLE_BR_UNCOND_H__

#include "../pass.h"
#include "llvm_ir/ir_builder.h"
#include <map>
#include <set>
#include <stack>
#include <unordered_map>

namespace Transform
{
    class EliminateDoubleBrUncondPass : public Pass
    {
      private:
        bool modified_;

      public:
        explicit EliminateDoubleBrUncondPass(LLVMIR::IR* ir);
        void Execute() override;
        bool wasModified() const { return modified_; }

      private:
        void processFunction(CFG* cfg);
        void buildGraph(
            CFG* cfg, std::vector<std::vector<LLVMIR::IRBlock*>>& G, std::vector<std::vector<LLVMIR::IRBlock*>>& invG);
        void updatePhiNodes(
            CFG* cfg, const std::unordered_map<int, int>& phiMap, const std::unordered_map<int, int>& otherPhiMap);
        void updateBranchInstructions(CFG* cfg, const std::unordered_map<int, int>& phiMap);
        void renumberBlocks(CFG* cfg);
        void removeBlockFromFunction(CFG* cfg, LLVMIR::IRBlock* block);
        void mergeBlocks(LLVMIR::IRBlock* target, LLVMIR::IRBlock* source);
        void cleanupSinglePredecessorPhis(CFG* cfg);
        bool replacePhiUsages(LLVMIR::Instruction* inst, const std::map<int, LLVMIR::Operand*>& replacements);
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_UTILS_ELIMINATE_DOUBLE_BR_UNCOND_H__
