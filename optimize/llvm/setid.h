#pragma once
#include "llvm/pass.h"
namespace LLVMIR
{
    class SetIdAnalysis : Pass
    {
      public:   
        SetIdAnalysis(LLVMIR::IR* ir) : Pass(ir) {}
        void Execute() override;
        void ExecuteInSingleCFG(CFG* func_cfg);
    };
};  // namespace LLVMIR