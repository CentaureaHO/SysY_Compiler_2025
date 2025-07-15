#ifndef __OPTIMIZER_LLVM_UNIFY_RETURN_H__
#define __OPTIMIZER_LLVM_UNIFY_RETURN_H__

#include "pass.h"
#include "cfg.h"
#include <vector>
#include <map>

namespace Transform
{
    class UnifyReturnPass : public Pass
    {
      private:
        void                          unifyFunctionReturns(CFG* cfg);
        std::vector<LLVMIR::RetInst*> findReturnInstructions(CFG* cfg);
        LLVMIR::IRBlock*              createUnifiedExitBlock(CFG* cfg);

      public:
        UnifyReturnPass(LLVMIR::IR* ir);

        virtual void Execute() override;
    };

}  // namespace Transform

#endif
