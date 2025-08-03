#ifndef __BACKEND_RV64_PASSES_OPTIMIZATION_PASSES_H__
#define __BACKEND_RV64_PASSES_OPTIMIZATION_PASSES_H__

#include "optimize/all_passes.h"

namespace Backend::RV64::Passes
{
    using PhiDestructionPass              = Optimize::ControlFlow::PhiDestructionPass;
    using ImmediateFMoveEliminationPass   = Optimize::Move::ImmediateFloatMoveEliminationPass;
    using ImmediateIMoveEliminationPass   = Optimize::Move::ImmediateIntegerMoveEliminationPass;
    using MoveEliminationPass             = Optimize::Move::GeneralMoveEliminationPass;
    using ArithmeticStrengthReductionPass = Optimize::ArithmeticStrengthReductionPass;
    using SSAPeepholePass                 = Optimize::Peehole::SSAPeepholePass;
    using RV64CSEPass                     = Optimize::RV64CSEPass;
}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_OPTIMIZATION_PASSES_H__
