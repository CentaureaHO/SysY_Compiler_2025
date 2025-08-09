#ifndef __OPTIMIZER_LLVM_UNUSED_FUNC_ELIMINATION_H__
#define __OPTIMIZER_LLVM_UNUSED_FUNC_ELIMINATION_H__

#include "pass.h"
#include "cfg.h"
#include <map>
#include <queue>

namespace Transform
{
    class UnusedFuncEliminationPass : public Pass
    {
      public:
        UnusedFuncEliminationPass(LLVMIR::IR* ir): Pass(ir){}
        virtual void Execute() override;
    };

}  // namespace Transform



#endif //__OPTIMIZER_LLVM_UNUSED_FUNC_ELIMINATION_H__