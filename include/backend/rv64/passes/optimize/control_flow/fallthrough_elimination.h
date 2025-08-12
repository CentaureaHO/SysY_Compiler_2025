#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_FALLTHROUGH_ELIMINATION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_FALLTHROUGH_ELIMINATION_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include "../../../rv64_cfg.h"
#include "../../../rv64_block.h"
#include <vector>
#include <unordered_set>

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    class FallthroughEliminationPass : public BasePass
    {
      public:
        explicit FallthroughEliminationPass(std::vector<Function*>& functions);
        ~FallthroughEliminationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "FallthroughElimination"; }

      private:
        std::vector<Function*>& functions_;

        bool optimizeFunction(Function* func);
        bool eliminateFallthroughJumps(Function* func);

        bool isFallthroughJump(Block* block, Block* next_block);
        bool isUnconditionalJump(Instruction* inst);
        bool isConditionalBranch(Instruction* inst);
        int  getJumpTarget(Instruction* inst);
        void removeLastInstruction(Block* block);
        bool hasConditionalJumpPattern(Block* block);

        int eliminated_jumps_count_;
    };

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_FALLTHROUGH_ELIMINATION_H__
