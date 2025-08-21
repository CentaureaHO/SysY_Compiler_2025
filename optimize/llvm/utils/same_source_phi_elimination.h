#ifndef __OPTIMIZER_LLVM_UTILS_SAME_SOURCE_PHI_ELIMINATION_H__
#define __OPTIMIZER_LLVM_UTILS_SAME_SOURCE_PHI_ELIMINATION_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm_ir/defs.h"
#include <unordered_set>

namespace Transform
{
    class SameSourcePhiEliminationPass : public Pass
    {
      private:
        bool preserve_lcssa_;           ///< 是否保护LCSSA形式，如果为true则跳过循环exit块的PHI指令
        bool last_execution_modified_;  ///< 记录上次执行是否做了修改

        bool isLoopExitBlock(LLVMIR::IRBlock* block, CFG* cfg) const;
        bool processSameSourcePhi(LLVMIR::PhiInst* phi, LLVMIR::IRBlock* block, CFG* cfg);
        bool allSourcesAreSame(LLVMIR::PhiInst* phi, LLVMIR::Operand*& common_source) const;
        bool operandsEqual(LLVMIR::Operand* op1, LLVMIR::Operand* op2) const;

      public:
        SameSourcePhiEliminationPass(LLVMIR::IR* ir);
        virtual ~SameSourcePhiEliminationPass() = default;

        void setPreserveLCSSA(bool preserve) { preserve_lcssa_ = preserve; }
        bool getPreserveLCSSA() const { return preserve_lcssa_; }

        bool wasModified() const { return last_execution_modified_; }

        virtual void Execute() override;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_UTILS_SAME_SOURCE_PHI_ELIMINATION_H__
