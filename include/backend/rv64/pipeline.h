#ifndef __BACKEND_RV64_PIPELINE_H__
#define __BACKEND_RV64_PIPELINE_H__

#include "../base_backend.h"
#include "../pass_manager.h"
#include "pass_set_generator.h"

namespace Backend::RV64
{

    class Pipeline : public BasePipeline
    {
      public:
        Pipeline(LLVMIR::IR* ir, std::ostream& out, int optLevel = 0);
        ~Pipeline() override;

        void        run() override;
        const char* getBackendName() const override { return "RISC-V 64-bit"; }

      private:
        PassManager            passManager_;
        int                    optLevel_;
        std::vector<Function*> functions_;

        void setupPipeline();
    };

}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_PIPELINE_H__
