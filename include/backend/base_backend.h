#ifndef __BACKEND_BASE_PIPELINE_H__
#define __BACKEND_BASE_PIPELINE_H__

#include <memory>
#include <vector>
#include <iostream>
#include "base_pass.h"

namespace LLVMIR
{
    class IR;
}

namespace Backend
{

    class BasePipeline
    {
      public:
        explicit BasePipeline(LLVMIR::IR* ir, std::ostream& out) : ir_(ir), out_(out) {}
        virtual ~BasePipeline() = default;

        virtual void run() = 0;

        virtual const char* getBackendName() const = 0;

      protected:
        LLVMIR::IR*                            ir_;
        std::ostream&                          out_;
        std::vector<std::unique_ptr<BasePass>> passes_;

        void addPass(std::unique_ptr<BasePass> pass) { passes_.push_back(std::move(pass)); }

        void runPasses()
        {
            for (auto& pass : passes_) { pass->run(); }
        }

        BasePipeline(const BasePipeline&)            = delete;
        BasePipeline& operator=(const BasePipeline&) = delete;
    };

}  // namespace Backend

#endif  // __BACKEND_BASE_PIPELINE_H__
