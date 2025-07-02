#ifndef __BACKEND_RV64_PASSES_REGISTER_ALLOCATION_H__
#define __BACKEND_RV64_PASSES_REGISTER_ALLOCATION_H__

#include "../../base_pass.h"
#include "../reg_assign.h"
#include <memory>
#include <vector>

namespace Backend::RV64::Passes
{

    class RegisterAllocationPass : public BasePass
    {
      public:
        explicit RegisterAllocationPass(
            std::vector<Function*>& functions, const std::string& allocatorType = "linear_scan");
        ~RegisterAllocationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "RegisterAllocation"; }

      private:
        std::vector<Function*>&               functions_;
        std::unique_ptr<BaseRegisterAssigner> regAssigner_;

        std::unique_ptr<BaseRegisterAssigner> createAllocator(const std::string& type);
    };

}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_REGISTER_ALLOCATION_H__
