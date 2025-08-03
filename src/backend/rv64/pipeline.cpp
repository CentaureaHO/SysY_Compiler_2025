#include <backend/rv64/pipeline.h>
#include <backend/base_pass.h>
#include <backend/rv64/pass_set_generator.h>
#include <backend/rv64/rv64_function.h>
#include <memory>

namespace Backend::RV64
{

    Pipeline::Pipeline(LLVMIR::IR* ir, std::ostream& out, int optLevel) : BasePipeline(ir, out), optLevel_(optLevel)
    {
        setupPipeline();
    }

    Pipeline::~Pipeline()
    {
        for (auto* func : functions_) { delete func; }
        functions_.clear();
    }

    void Pipeline::setupPipeline()
    {
        functions_.clear();

        std::vector<std::unique_ptr<Backend::BasePass>> passes =
            PassSetGenerator::generate(ir_, functions_, ir_->global_def, out_, optLevel_);

        for (auto& pass : passes) { passManager_.addPass(std::move(pass)); }
    }

    void Pipeline::run() { passManager_.executeAll(); }

}  // namespace Backend::RV64
