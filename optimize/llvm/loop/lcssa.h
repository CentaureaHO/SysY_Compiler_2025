#ifndef __OPTIMIZER_LLVM_LOOP_LCSSA_H__
#define __OPTIMIZER_LLVM_LOOP_LCSSA_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "cfg.h"
#include <set>
#include <map>
#include <tuple>

namespace StructuralTransform
{
    class LCSSAPass : public Pass
    {
      public:
        explicit LCSSAPass(LLVMIR::IR* ir);

        void Execute() override;

      private:
        void transformToLCSSA(CFG* cfg);
        void transformLoopToLCSSA(CFG* cfg, NaturalLoop* loop);

        std::tuple<std::set<int>, std::map<int, LLVMIR::DataType>> getUsedOperandOutOfLoop(CFG* cfg, NaturalLoop* loop);
    };

}  // namespace StructuralTransform

#endif  // __OPTIMIZER_LLVM_LOOP_LCSSA_H__
