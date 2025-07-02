#ifndef __BACKEND_FACTORY_H__
#define __BACKEND_FACTORY_H__

#include <memory>
#include <string>
#include <iostream>
#include "base_backend.h"

namespace LLVMIR
{
    class IR;
}

namespace Backend
{

    class Factory
    {
      public:
        enum class TargetArch
        {
            RV64,
        };

        static std::unique_ptr<BasePipeline> createBackend(
            TargetArch arch, LLVMIR::IR* ir, std::ostream& out, int optLevel = 0);
    };

}  // namespace Backend

#endif  // __BACKEND_FACTORY_H__
