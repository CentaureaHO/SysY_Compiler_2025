#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_PEEHOLE_SSA_DEAD_DEF_ELIMINATE_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_PEEHOLE_SSA_DEAD_DEF_ELIMINATE_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include <vector>
#include <map>

namespace Backend::RV64::Passes::Optimize::Peehole
{

    class SSADeadDefEliminatePass : public BasePass
    {
      public:
        explicit SSADeadDefEliminatePass(std::vector<Function*>& functions);
        ~SSADeadDefEliminatePass() override = default;

        bool        run() override;
        const char* getName() const override { return "SSADeadDefEliminate"; }

      private:
        std::vector<Function*>& functions_;

        void eliminateFunction(Function* func);
        void countRegisterReferences(Function* func, std::map<int, int>& vreg_refcnt);
        void removeDeadDefinitions(Function* func, const std::map<int, int>& vreg_refcnt);
    };

}  // namespace Backend::RV64::Passes::Optimize::Peehole

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_PEEHOLE_SSA_DEAD_DEF_ELIMINATE_H__
