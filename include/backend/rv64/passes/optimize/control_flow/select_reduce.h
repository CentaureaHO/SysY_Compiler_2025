#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_SELECT_REDUCE_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_SELECT_REDUCE_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include <vector>

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    class SelectReducePass : public BasePass
    {
      public:
        explicit SelectReducePass(std::vector<Function*>& functions);
        ~SelectReducePass() override = default;

        bool        run() override;
        const char* getName() const override { return "SelectReduce"; }

      private:
        std::vector<Function*>& functions_;

        void reduceFunction(Function* func);
    };

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_SELECT_REDUCE_H__
