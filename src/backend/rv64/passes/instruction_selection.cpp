#include <backend/rv64/passes/instruction_selection.h>
#include <backend/rv64/selector.h>

namespace Backend::RV64::Passes
{

    InstructionSelectionPass::InstructionSelectionPass(
        LLVMIR::IR* ir, std::vector<Function*>& functions, std::vector<LLVMIR::Instruction*>& glb_defs)
        : ir_(ir),
          functions_(&functions),
          glb_defs_(&glb_defs),
          selector_(std::make_unique<Selector>(ir_, *functions_, *glb_defs_))
    {}

    bool InstructionSelectionPass::run()
    {
        selector_->selectInstruction();
        return true;  // Modified the IR
    }

}  // namespace Backend::RV64::Passes
