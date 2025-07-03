#ifndef __BACKEND_RV64_PASSES_FRAME_LOWERING_H__
#define __BACKEND_RV64_PASSES_FRAME_LOWERING_H__

#include "../../base_pass.h"
#include "../rv64_function.h"
#include <vector>

namespace Backend::RV64::Passes
{

    class FrameLoweringPass : public BasePass
    {
      public:
        explicit FrameLoweringPass(std::vector<Function*>& functions);
        ~FrameLoweringPass() override = default;

        bool        run() override;
        const char* getName() const override { return "FrameLowering"; }

      private:
        std::vector<Function*>& functions_;

        void lowerFrame(Function* func);
    };

}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_FRAME_LOWERING_H__
