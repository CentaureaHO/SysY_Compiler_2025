#include <backend/rv64/passes/instruction_selection.h>
#include <backend/rv64/passes/frame_lowering.h>
#include <backend/rv64/passes/optimization_passes.h>
#include <backend/rv64/passes/register_allocation.h>
#include <backend/rv64/passes/stack_lowering.h>
#include <backend/rv64/passes/code_generation.h>
#include <backend/rv64/passes/optimize/control_flow/block_layout.h>
#include <backend/rv64/passes/optimize/control_flow/fallthrough_elimination.h>
#include <backend/rv64/passes/optimize/instruction_schedule.h>
#include <backend/rv64/passes/cfg_builder.h>
#include <backend/rv64/passes/rv64_loop_find.h>
#include <backend/rv64/passes/optimize/rv64_licm.h>

#include <backend/rv64/pass_set_generator.h>

using namespace Backend::RV64;
using namespace Backend::RV64::Passes;
using namespace Backend::RV64::Passes::Optimize::ControlFlow;

extern bool no_reg_alloc;
extern bool no_select_lower;

bool Backend::RV64::force_no_schedule = false;

std::vector<std::unique_ptr<Backend::BasePass>> PassSetGenerator::generate(LLVMIR::IR* ir,
    std::vector<Function*>& functions, std::vector<LLVMIR::Instruction*>& glb_defs, std::ostream& out, int optLevel)
{
    std::vector<std::unique_ptr<Backend::BasePass>> passes;

    InstructionSelectionPass(ir, functions, glb_defs).run();
    SelectReducePass(functions).run();
    CFGBuilderPass(functions).run();
    Optimize::RV64MakeDomTreePass(functions).run();
    FrameLoweringPass(functions).run();
    if (optLevel)
    {
        ArithmeticStrengthReductionPass(functions).run();

        Optimize::Peehole::SSADeadDefEliminatePass(functions).run();
        Optimize::RV64CSEPass(functions).run();

        LoopFindPass(functions).run();
        Optimize::LICMPass(functions).run();

        Optimize::RV64CSEPass(functions).run();
    }
    PhiDestructionPass(functions).run();
    ImmediateFMoveEliminationPass(functions).run();
    ImmediateIMoveEliminationPass(functions).run();
    MoveEliminationPass(functions).run();
    if (optLevel)
    {
        BlockLayoutPass(functions).run();
        CFGBuilderPass(functions).run();

        // Optimize::InstructionSchedulePass(functions).run();
    }
    if (!no_reg_alloc)
    {
        CFGBuilderPass(functions).run();
        LoopFindPass(functions).run();
        Optimize::RV64MakeDomTreePass(functions).run();

        RegisterAllocationPass(functions).run();
        StackLoweringPass(functions).run();
    }

    if (optLevel) { FallthroughEliminationPass(functions).run(); }

    CodeGenerationPass(functions, glb_defs, out).run();

    // passes.emplace_back(std::make_unique<InstructionSelectionPass>(ir, functions, glb_defs));
    // passes.emplace_back(std::make_unique<SelectReducePass>(functions));
    // passes.emplace_back(std::make_unique<CFGBuilderPass>(functions));
    // passes.emplace_back(std::make_unique<Optimize::RV64MakeDomTreePass>(functions));
    // passes.emplace_back(std::make_unique<FrameLoweringPass>(functions));
    // if (optLevel)
    // {
    //     // passes.emplace_back(std::make_unique<ArithmeticStrengthReductionPass>(functions));
    //     // passes.emplace_back(std::make_unique<Optimize::Peehole::SSAPeepholePass>(functions));
    //     passes.emplace_back(std::make_unique<Optimize::Peehole::SSADeadDefEliminatePass>(functions));
    //     passes.emplace_back(std::make_unique<Optimize::RV64CSEPass>(functions));
    // }
    // passes.emplace_back(std::make_unique<PhiDestructionPass>(functions));
    // passes.emplace_back(std::make_unique<ImmediateFMoveEliminationPass>(functions));
    // passes.emplace_back(std::make_unique<ImmediateIMoveEliminationPass>(functions));
    // passes.emplace_back(std::make_unique<MoveEliminationPass>(functions));
    // if (optLevel)
    // {
    //     passes.emplace_back(std::make_unique<BlockLayoutPass>(functions));
    //     passes.emplace_back(std::make_unique<CFGBuilderPass>(functions));

    //     // passes.emplace_back(std::make_unique<FallthroughEliminationPass>(functions)); 违背寄存器分配假设了，不应用

    //     // 指令调度优化 - 在寄存器分配前进行以考虑寄存器压力
    //     if (optLevel >= 2)
    //     passes.emplace_back(std::make_unique<Passes::Optimize::InstructionSchedulePass>(functions));
    // }
    // if (!no_reg_alloc)
    // {
    //     passes.emplace_back(std::make_unique<CFGBuilderPass>(functions));
    //     passes.emplace_back(std::make_unique<Optimize::RV64MakeDomTreePass>(functions));

    //     passes.emplace_back(std::make_unique<RegisterAllocationPass>(functions));
    //     passes.emplace_back(std::make_unique<StackLoweringPass>(functions));
    // }

    // passes.emplace_back(std::make_unique<CodeGenerationPass>(functions, glb_defs, out));

    return passes;
}
