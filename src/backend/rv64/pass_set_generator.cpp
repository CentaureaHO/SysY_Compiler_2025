#include <backend/rv64/passes/instruction_selection.h>
#include <backend/rv64/passes/frame_lowering.h>
#include <backend/rv64/passes/optimization_passes.h>
#include <backend/rv64/passes/register_allocation.h>
#include <backend/rv64/passes/stack_lowering.h>
#include <backend/rv64/passes/code_generation.h>
#include <backend/rv64/passes/optimize/control_flow/block_layout.h>
#include <backend/rv64/passes/optimize/control_flow/fallthrough_elimination.h>
#include <backend/rv64/passes/cfg_builder.h>

#include <backend/rv64/pass_set_generator.h>

using namespace Backend::RV64;
using namespace Backend::RV64::Passes;
using namespace Backend::RV64::Passes::Optimize::ControlFlow;

extern bool no_reg_alloc;
extern bool no_select_lower;

std::vector<std::unique_ptr<Backend::BasePass>> PassSetGenerator::generate(LLVMIR::IR* ir,
    std::vector<Function*>& functions, std::vector<LLVMIR::Instruction*>& glb_defs, std::ostream& out, int optLevel)
{
    std::vector<std::unique_ptr<Backend::BasePass>> passes;

    passes.emplace_back(std::make_unique<InstructionSelectionPass>(ir, functions, glb_defs));
    passes.emplace_back(std::make_unique<SelectReducePass>(functions));
    passes.emplace_back(std::make_unique<CFGBuilderPass>(functions));
    passes.emplace_back(std::make_unique<Optimize::RV64MakeDomTreePass>(functions));
    passes.emplace_back(std::make_unique<FrameLoweringPass>(functions));
    if (optLevel)
    {
        // passes.emplace_back(std::make_unique<ArithmeticStrengthReductionPass>(functions));
        // passes.emplace_back(std::make_unique<Optimize::Peehole::SSAPeepholePass>(functions));
        passes.emplace_back(std::make_unique<Optimize::Peehole::SSADeadDefEliminatePass>(functions));
        passes.emplace_back(std::make_unique<Optimize::RV64CSEPass>(functions));
    }
    passes.emplace_back(std::make_unique<PhiDestructionPass>(functions));
    passes.emplace_back(std::make_unique<ImmediateFMoveEliminationPass>(functions));
    passes.emplace_back(std::make_unique<ImmediateIMoveEliminationPass>(functions));
    passes.emplace_back(std::make_unique<MoveEliminationPass>(functions));
    if (optLevel)
    {
        passes.emplace_back(std::make_unique<BlockLayoutPass>(functions));
        passes.emplace_back(std::make_unique<CFGBuilderPass>(functions));

        // passes.emplace_back(std::make_unique<FallthroughEliminationPass>(functions)); 违背寄存器分配假设了，不应用
    }
    if (!no_reg_alloc)
    {
        passes.emplace_back(std::make_unique<RegisterAllocationPass>(functions));
        passes.emplace_back(std::make_unique<StackLoweringPass>(functions));
    }

    passes.emplace_back(std::make_unique<CodeGenerationPass>(functions, glb_defs, out));

    return passes;
}
