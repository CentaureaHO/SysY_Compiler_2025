#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_LOAD_STORE_REDUNDANT_ELIMINATION_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_LOAD_STORE_REDUNDANT_ELIMINATION_H__

#include "../../../../base_pass.h"
#include "../../../rv64_function.h"
#include <vector>
#include <set>

namespace Backend::RV64::Passes::Optimize::Move
{

    class LoadStoreRedundantEliminationPass : public BasePass
    {
      public:
        explicit LoadStoreRedundantEliminationPass(std::vector<Function*>& functions);
        ~LoadStoreRedundantEliminationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "LoadStoreRedundantElimination"; }

      private:
        std::vector<Function*>& functions_;

        void eliminateLoadStoreRedundant(Function* func);
        bool isLoadInstruction(RV64Inst* inst);
        bool isStoreInstruction(RV64Inst* inst);
        bool isBranchInstruction(RV64Inst* inst);
        bool isJalrInstruction(RV64Inst* inst);
        bool isConditionalBranchInstruction(RV64Inst* inst);
        bool isMemoryLocationSame(RV64Inst* load_inst, RV64Inst* store_inst);
        bool isStoringSameRegister(RV64Inst* load_inst, RV64Inst* store_inst);
        bool isRegisterWritten(RV64Inst* inst, Register target_reg);
        bool isRegisterUsed(RV64Inst* inst, Register target_reg);
    };

}  // namespace Backend::RV64::Passes::Optimize::Move

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_MOVE_LOAD_STORE_REDUNDANT_ELIMINATION_H__
