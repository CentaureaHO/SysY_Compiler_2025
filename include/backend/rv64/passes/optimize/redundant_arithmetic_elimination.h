#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_REDUNDANT_ARITHMETIC_ELIMINATION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_REDUNDANT_ARITHMETIC_ELIMINATION_H__

#include "../../../base_pass.h"
#include "../../rv64_function.h"
#include <vector>

namespace Backend::RV64::Passes::Optimize
{

    class RedundantArithmeticEliminationPass : public BasePass
    {
      public:
        explicit RedundantArithmeticEliminationPass(std::vector<Function*>& functions);
        ~RedundantArithmeticEliminationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "RedundantArithmeticElimination"; }

      private:
        std::vector<Function*>& functions_;

        void eliminateRedundantArithmetic(Function* func);
        bool isRedundantInstruction(RV64Inst* inst);
    };

}  // namespace Backend::RV64::Passes::Optimize

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_REDUNDANT_ARITHMETIC_ELIMINATION_H__
