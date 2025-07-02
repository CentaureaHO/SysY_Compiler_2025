#include <backend/factory.h>
#include <backend/rv64/pipeline.h>
#include <stdexcept>
#include <algorithm>

namespace Backend
{

    std::unique_ptr<BasePipeline> Factory::createBackend(
        TargetArch arch, LLVMIR::IR* ir, std::ostream& out, int optLevel)
    {
        switch (arch)
        {
            case TargetArch::RV64: return std::make_unique<RV64::Pipeline>(ir, out, optLevel);
            default: throw std::runtime_error("Unsupported target architecture");
        }
    }

}  // namespace Backend
