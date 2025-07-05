#include <backend/rv64/passes/instruction_selection.h>
#include <backend/rv64/passes/frame_lowering.h>
#include <backend/rv64/passes/optimization_passes.h>
#include <backend/rv64/passes/register_allocation.h>
#include <backend/rv64/passes/stack_lowering.h>
#include <backend/rv64/passes/code_generation.h>

#include <backend/rv64/pass_set_generator.h>

using namespace Backend::RV64;
using namespace Backend::RV64::Passes;

extern bool no_reg_alloc;

std::vector<std::unique_ptr<Backend::BasePass>> PassSetGenerator::generate(
    LLVMIR::IR* ir, std::vector<Function*>& functions, std::vector<LLVMIR::Instruction*>& glb_defs, std::ostream& out)
{
    std::vector<std::unique_ptr<Backend::BasePass>> passes;

    passes.emplace_back(std::make_unique<InstructionSelectionPass>(ir, functions, glb_defs));
    passes.emplace_back(std::make_unique<FrameLoweringPass>(functions));
    passes.emplace_back(std::make_unique<PhiDestructionPass>(functions));
    passes.emplace_back(std::make_unique<ImmediateFMoveEliminationPass>(functions));
    passes.emplace_back(std::make_unique<ImmediateIMoveEliminationPass>(functions));
    passes.emplace_back(std::make_unique<MoveEliminationPass>(functions));
    if (!no_reg_alloc)
    {
        passes.emplace_back(std::make_unique<RegisterAllocationPass>(functions));
        passes.emplace_back(std::make_unique<StackLoweringPass>(functions));
    }

    passes.emplace_back(std::make_unique<CodeGenerationPass>(functions, glb_defs, out));

    return passes;
}
