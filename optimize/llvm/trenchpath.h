#pragma once

#include "llvm_ir/ir_builder.h"
#include "llvm/pass.h"

namespace LLVMIR
{
    class TrenchPath : Pass
    {
      private:
        void ExecuteInSingleCFG(CFG* cfg);

      public:
        TrenchPath(IR* ir) : Pass(ir){};
        void Execute() override;
    };
}  // namespace LLVMIR