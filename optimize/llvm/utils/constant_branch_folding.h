#ifndef __OPTIMIZER_LLVM_UTILS_CONSTANT_BRANCH_FOLDING_H__
#define __OPTIMIZER_LLVM_UTILS_CONSTANT_BRANCH_FOLDING_H__

#include "llvm/pass.h"
#include "llvm_ir/defs.h"

namespace Transform
{
    class ConstantBranchFoldingPass : public Pass
    {
      private:
        bool last_execution_modified_;  ///< 记录上次执行是否做了修改

        bool getConstantValue(LLVMIR::Operand* operand, int& value) const;
        bool processConstantBranch(LLVMIR::BranchCondInst* branch, LLVMIR::IRBlock* block, CFG* cfg);
        void cleanupPhiNodes(LLVMIR::Operand* unreachable_label, int from_block_id, CFG* cfg);

      public:
        ConstantBranchFoldingPass(LLVMIR::IR* ir);
        virtual ~ConstantBranchFoldingPass() = default;

        bool wasModified() const { return last_execution_modified_; }

        virtual void Execute() override;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_UTILS_CONSTANT_BRANCH_FOLDING_H__
