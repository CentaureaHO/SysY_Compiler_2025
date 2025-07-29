#ifndef __OPTIMIZER_LLVM_LOOP_SIMPLIFY_H__
#define __OPTIMIZER_LLVM_LOOP_SIMPLIFY_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "cfg.h"
#include <set>
#include <functional>
#include <memory>

namespace StructuralTransform
{
    class LoopSimplifyPass : public Pass
    {
      private:
        void             eliminateUselessPhi(CFG* cfg);
        void             addLoopStructureComments(CFG* cfg);
        void             cleanupInvalidPhiReferences(CFG* cfg);
        void             simplifyExitNodes(NaturalLoop* loop, CFG* cfg);
        LLVMIR::IRBlock* findUltimateTarget(LLVMIR::IRBlock* exit_node, CFG* cfg);
        LLVMIR::Operand* traceValueThroughChain(
            LLVMIR::IRBlock* exiting, LLVMIR::IRBlock* exit, LLVMIR::IRBlock* ultimate, int phi_reg, CFG* cfg);

      public:
        explicit LoopSimplifyPass(LLVMIR::IR* ir);

        void Execute() override;

        void                loopSimplify(CFG* cfg);
        bool                isLoopSimplified(NaturalLoop* loop, CFG* cfg) const;
        std::pair<int, int> getSimplificationStats() const;

      private:
        mutable int loops_processed = 0;
        mutable int loops_modified  = 0;
    };

}  // namespace StructuralTransform

#endif  // __OPTIMIZER_LLVM_LOOP_SIMPLIFY_H__
