#ifndef __BACKEND_RV64_PASSES_CFG_BUILDER_H__
#define __BACKEND_RV64_PASSES_CFG_BUILDER_H__

#include "../../base_pass.h"
#include "../rv64_function.h"
#include "../rv64_cfg.h"
#include "../rv64_block.h"
#include "../rv64_defs.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace Backend::RV64::Passes
{

    class CFGBuilderPass : public BasePass
    {
      public:
        explicit CFGBuilderPass(std::vector<Function*>& functions);
        ~CFGBuilderPass() override = default;

        bool        run() override;
        const char* getName() const override { return "CFGBuilder"; }

      private:
        std::vector<Function*>& functions_;

        bool buildCFGForFunction(Function* func);
        void analyzeBlockSuccessors(Block* block, CFG* cfg);
        void clearCFGEdges(CFG* cfg);
        void setEntryAndExitBlocks(CFG* cfg);

        // Handle fallthrough after elimination
        void   addFallthroughEdges(Function* func, CFG* cfg);
        Block* getNextBlockInLayout(Function* func, Block* current_block);

        std::vector<int> getBlockTargets(Block* block);
        bool             hasConditionalBranch(Block* block);
        bool             hasUnconditionalBranch(Block* block);
        bool             isReturn(Instruction* inst);
        bool             isConditionalBranch(Instruction* inst);
        bool             isUnconditionalBranch(Instruction* inst);
        int              extractJumpTarget(Instruction* inst);

        std::pair<int, int> analyzeConditionalJumpPattern(Block* block);
    };

}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_CFG_BUILDER_H__
