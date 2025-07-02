#ifndef __BACKEND_RV64_PASSES_STACK_LOWERING_H__
#define __BACKEND_RV64_PASSES_STACK_LOWERING_H__

#include "../../base_pass.h"
#include "../function.h"
#include "../defs.h"
#include <vector>

namespace Backend::RV64::Passes
{

    class StackLoweringPass : public BasePass
    {
      public:
        explicit StackLoweringPass(std::vector<Function*>& functions);
        ~StackLoweringPass() override = default;

        bool        run() override;
        const char* getName() const override { return "StackLowering"; }

      private:
        std::vector<Function*>& functions_;

        void lowerStack(Function* func);
        void gatherRegsToSave(Function* func, MAT2(int) & reg_def_blocks, MAT2(int) & reg_access_blocks);
    };

}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_STACK_LOWERING_H__
