#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_UNREACHABLE_BLOCK_ELIMINATION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_UNREACHABLE_BLOCK_ELIMINATION_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include "../../../rv64_cfg.h"
#include "../../../rv64_block.h"
#include "../../../rv64_loop.h"
#include <vector>
#include <unordered_set>
#include <queue>

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    class UnreachableBlockEliminationPass : public BasePass
    {
      public:
        explicit UnreachableBlockEliminationPass(std::vector<Function*>& functions);
        ~UnreachableBlockEliminationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "UnreachableBlockElimination"; }

      private:
        std::vector<Function*>& functions_;

        bool                    optimizeFunction(Function* func);
        std::unordered_set<int> findReachableBlocks(Function* func);
        bool removeUnreachableBlocks(Function* func, const std::unordered_set<int>& reachable_blocks);
        void updateCFGAfterRemoval(Function* func, const std::unordered_set<int>& removed_blocks);

        int eliminated_blocks_count_;
    };

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_CONTROL_FLOW_UNREACHABLE_BLOCK_ELIMINATION_H__
