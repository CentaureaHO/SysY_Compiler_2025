#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_IMMEDIATE_INTEGER_MOVE_ELIMINATION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_IMMEDIATE_INTEGER_MOVE_ELIMINATION_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include <vector>

namespace Backend::RV64::Passes::Optimize::Move
{

    class ImmediateIntegerMoveEliminationPass : public BasePass
    {
      public:
        explicit ImmediateIntegerMoveEliminationPass(std::vector<Function*>& functions);
        ~ImmediateIntegerMoveEliminationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "ImmediateIntegerMoveElimination"; }

      private:
        std::vector<Function*>& functions_;

        void eliminateImmeIMoveInst(Function* func);
    };

}  // namespace Backend::RV64::Passes::Optimize::Move

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_IMMEDIATE_INTEGER_MOVE_ELIMINATION_H__