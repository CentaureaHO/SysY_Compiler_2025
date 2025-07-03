#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_GENERAL_MOVE_ELIMINATION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_GENERAL_MOVE_ELIMINATION_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include <vector>

namespace Backend::RV64::Passes::Optimize::Move
{

    class GeneralMoveEliminationPass : public BasePass
    {
      public:
        explicit GeneralMoveEliminationPass(std::vector<Function*>& functions);
        ~GeneralMoveEliminationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "GeneralMoveElimination"; }

      private:
        std::vector<Function*>& functions_;

        void eliminateMoveInst(Function* func);
    };

}  // namespace Backend::RV64::Passes::Optimize::Move

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_GENERAL_MOVE_ELIMINATION_H__