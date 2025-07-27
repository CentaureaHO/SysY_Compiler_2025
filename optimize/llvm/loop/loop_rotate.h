#ifndef __OPTIMIZER_LLVM_LOOP_ROTATE_H__
#define __OPTIMIZER_LLVM_LOOP_ROTATE_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "cfg.h"
#include <set>
#include <map>

namespace StructuralTransform
{
    class LoopRotatePass : public Pass
    {
      private:
        bool                                            canRotate(NaturalLoop* loop);
        std::pair<std::set<LLVMIR::Instruction*>, bool> checkHeaderUse(NaturalLoop* loop, int result_reg);
        void resolveConflict(CFG* cfg, NaturalLoop* loop, LLVMIR::Instruction* header_def, int new_reg,
            std::set<LLVMIR::Instruction*> body_uses);

        LLVMIR::IRBlock* createGuardBlock(CFG* cfg, NaturalLoop* loop);

        void restructureLoop(CFG* cfg, NaturalLoop* loop, LLVMIR::IRBlock* condition_block);

      public:
        explicit LoopRotatePass(LLVMIR::IR* ir);
        void Execute() override;
    };

}  // namespace StructuralTransform

#endif  // __OPTIMIZER_LLVM_LOOP_ROTATE_H__
