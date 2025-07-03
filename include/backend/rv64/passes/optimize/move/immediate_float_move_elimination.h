#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_IMMEDIATE_FLOAT_MOVE_ELIMINATION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_IMMEDIATE_FLOAT_MOVE_ELIMINATION_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include <vector>

namespace Backend::RV64::Passes::Optimize::Move
{

    class ImmediateFloatMoveEliminationPass : public BasePass
    {
      public:
        explicit ImmediateFloatMoveEliminationPass(std::vector<Function*>& functions);
        ~ImmediateFloatMoveEliminationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "ImmediateFloatMoveElimination"; }

      private:
        std::vector<Function*>& functions_;

        void eliminateImmeFMoveInst(Function* func);
    };

}  // namespace Backend::RV64::Passes::Optimize::Move

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_IMMEDIATE_FLOAT_MOVE_ELIMINATION_H__