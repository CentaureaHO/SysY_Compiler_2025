#ifndef __OPTIMIZER_LLVM_STRENGTH_REDUCTION_CONST_BRANCH_REDUCE_H__
#define __OPTIMIZER_LLVM_STRENGTH_REDUCTION_CONST_BRANCH_REDUCE_H__

#include "llvm/pass.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"

namespace Transform
{
    // 将常量分支跳转转换为无条件跳转
    // 并删除这一改变后的不可达块

    class ConstBranchReduce : public Pass
    {
      public:
        ConstBranchReduce(LLVMIR::IR* ir);
        virtual void Execute() override;

      private:
        CFG* cur_cfg;

        void                                       runOnBlock(LLVMIR::IRBlock* block);
        std::deque<LLVMIR::Instruction*>::iterator getCondbrIter(LLVMIR::IRBlock* block);
        void                                       elimateUnreachableBlocks();
        void                                       cleanupPhiInstructions();
    };
}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_STRENGTH_REDUCTION_CONST_BRANCH_REDUCE_H__
