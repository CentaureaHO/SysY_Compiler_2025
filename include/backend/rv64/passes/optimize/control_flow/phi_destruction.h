#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_PHI_DESTRUCTION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_PHI_DESTRUCTION_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include <vector>

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    class PhiDestructionPass : public BasePass
    {
      public:
        explicit PhiDestructionPass(std::vector<Function*>& functions);
        ~PhiDestructionPass() override = default;

        bool        run() override;
        const char* getName() const override { return "PhiDestruction"; }

      private:
        std::vector<Function*>& functions_;

        void phiInstDestory(Function* func);
        void redirectBranchInst(Function* func, int from, int origin_to, int new_to);
        void generatePhiCopyInstructions(Function* func);
    };

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_PHI_DESTRUCTION_H__