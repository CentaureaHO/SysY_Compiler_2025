#ifndef __OPTIMIZER_LLVM_UTILS_SINGLE_SOURCE_PHI_ELIMINATION_H__
#define __OPTIMIZER_LLVM_UTILS_SINGLE_SOURCE_PHI_ELIMINATION_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm_ir/defs.h"
#include <unordered_set>

namespace Transform
{
    class SingleSourcePhiEliminationPass : public Pass
    {
      private:
        bool preserve_lcssa_;           ///< 是否保护LCSSA形式，如果为true则跳过循环exit块的PHI指令
        bool last_execution_modified_;  ///< 记录上次执行是否做了修改

        bool isLoopExitBlock(LLVMIR::IRBlock* block, CFG* cfg) const;
        bool processSingleSourcePhi(LLVMIR::PhiInst* phi, LLVMIR::IRBlock* block, CFG* cfg);

      public:
        SingleSourcePhiEliminationPass(LLVMIR::IR* ir);
        virtual ~SingleSourcePhiEliminationPass() = default;

        void setPreserveLCSSA(bool preserve) { preserve_lcssa_ = preserve; }
        bool getPreserveLCSSA() const { return preserve_lcssa_; }

        bool wasModified() const { return last_execution_modified_; }

        virtual void Execute() override;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_UTILS_SINGLE_SOURCE_PHI_ELIMINATION_H__