#ifndef __OPTIMIZER_LLVM_UTILS_PHI_CLEAN_H__
#define __OPTIMIZER_LLVM_UTILS_PHI_CLEAN_H__

#include "llvm/pass.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"

namespace Transform
{
    // PHI指令清理优化
    // 去除PHI指令中重复的来源，如：
    // %reg_41 = phi i32 [ %reg_5, %Block0 ], [ %reg_5, %Block0 ], [ %reg_40, %Block5 ], [ %reg_40, %Block5 ]
    // 优化为：
    // %reg_41 = phi i32 [ %reg_5, %Block0 ], [ %reg_40, %Block5 ]
    class PhiCleanPass : public Pass
    {
      private:
        bool last_execution_modified_;  ///< 记录上次执行是否做了修改

        // 清理单个PHI指令的重复来源
        bool cleanPhiInstruction(LLVMIR::PhiInst* phi);

        // 检查两个操作数是否相等
        bool areOperandsEqual(LLVMIR::Operand* op1, LLVMIR::Operand* op2);

      public:
        PhiCleanPass(LLVMIR::IR* ir);
        virtual ~PhiCleanPass() = default;

        bool wasModified() const { return last_execution_modified_; }

        virtual void Execute() override;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_UTILS_PHI_CLEAN_H__
