#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_ARITHMETIC_STRENGTH_REDUCTION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_ARITHMETIC_STRENGTH_REDUCTION_H__

#include "../../../base_pass.h"
#include "../../rv64_function.h"
#include <vector>
#include <map>

namespace Backend::RV64::Passes::Optimize
{

    struct Multiplier
    {
        uint64_t m;
        int      l;
    };

    class ArithmeticStrengthReductionPass : public BasePass
    {
      public:
        explicit ArithmeticStrengthReductionPass(std::vector<Function*>& functions);
        ~ArithmeticStrengthReductionPass() override = default;

        bool        run() override;
        const char* getName() const override { return "ArithmeticStrengthReduction"; }

      private:
        std::vector<Function*>& functions_;

        void optimizeFunction(Function* func);

        Multiplier chooseMultiplier(uint32_t d, int p);
    };

}  // namespace Backend::RV64::Passes::Optimize

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_ARITHMETIC_STRENGTH_REDUCTION_H__
