#ifndef __OPTIMIZER_LLVM_VERIFY_PHI_PRECURSOR_H__
#define __OPTIMIZER_LLVM_VERIFY_PHI_PRECURSOR_H__

#include "llvm/pass.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace Verify
{
    class PhiPrecursorPass : public Pass
    {
      private:
        bool        verifyPhiPrecursors(CFG* cfg);
        bool        checkPhiInstruction(LLVMIR::PhiInst* phi_inst, CFG* cfg, int block_id);
        std::string getErrorMessage(LLVMIR::PhiInst* phi_inst, int block_id, const std::string& error_detail);

      public:
        PhiPrecursorPass(LLVMIR::IR* ir);
        virtual void Execute() override;
    };

}  // namespace Verify

#endif
