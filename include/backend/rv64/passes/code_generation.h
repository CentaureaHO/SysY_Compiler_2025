#ifndef __BACKEND_RV64_PASSES_CODE_GENERATION_H__
#define __BACKEND_RV64_PASSES_CODE_GENERATION_H__

#include "../../base_pass.h"
#include "../function.h"
#include "../defs.h"
#include <memory>
#include <iostream>
#include <vector>

namespace LLVMIR
{
    class Instruction;
}

namespace Backend::RV64::Passes
{

    class CodeGenerationPass : public BasePass
    {
      public:
        CodeGenerationPass(
            std::vector<Function*>& functions, std::vector<LLVMIR::Instruction*>& glb_defs, std::ostream& out);
        ~CodeGenerationPass() override = default;

        bool        run() override;
        const char* getName() const override { return "CodeGeneration"; }

      private:
        std::vector<Function*>&            functions_;
        std::vector<LLVMIR::Instruction*>& glb_defs_;
        std::ostream&                      out_;

        void printGlobalDefinitions();
        void printFunctions();
        void printASM(RV64Inst* inst);
        void printASM(MoveInst* inst);
        void printASM(PhiInst* inst);
        void printOperand(Register r);
        void printOperand(Register* r);
        void printOperand(RV64Label l);
        void printOperand(RV64Label* l);
        void printOperand(Operand* op);

        // Current context for printing
        Function* cur_func_;
        Block*    cur_block_;
    };

}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_CODE_GENERATION_H__
