#ifndef __BACKEND_RV64_PASSES_INSTRUCTION_SELECTION_H__
#define __BACKEND_RV64_PASSES_INSTRUCTION_SELECTION_H__

#include "../../base_pass.h"
#include <memory>
#include "../selector.h"
#include <llvm_ir/ir_builder.h>

namespace Backend::RV64
{
    class Function;
}

namespace Backend::RV64::Passes
{

    class InstructionSelectionPass : public BasePass
    {
      public:
        explicit InstructionSelectionPass(LLVMIR::IR* ir, std::vector<Backend::RV64::Function*>& functions,
            std::vector<LLVMIR::Instruction*>& glb_defs);
        ~InstructionSelectionPass() override = default;

        bool        run() override;
        const char* getName() const override { return "InstructionSelection"; }

      private:
        LLVMIR::IR*                              ir_;
        std::vector<Backend::RV64::Function*>*   functions_;
        std::vector<LLVMIR::Instruction*>*       glb_defs_;
        std::unique_ptr<Backend::RV64::Selector> selector_;
    };

}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_INSTRUCTION_SELECTION_H__
