#include <backend/rv64/passes/register_allocation.h>

namespace Backend::RV64::Passes
{

    RegisterAllocationPass::RegisterAllocationPass(std::vector<Function*>& functions, const std::string& allocatorType)
        : functions_(functions)
    {
        regAssigner_ = createAllocator(allocatorType);
    }

    bool RegisterAllocationPass::run()
    {
        regAssigner_->assignRegisters(functions_);
        return true;  // Modified the IR
    }

    std::unique_ptr<BaseRegisterAssigner> RegisterAllocationPass::createAllocator(const std::string& type)
    {
        return std::make_unique<RegisterAssigner>();
    }

}  // namespace Backend::RV64::Passes
