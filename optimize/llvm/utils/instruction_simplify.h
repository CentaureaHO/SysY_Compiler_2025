#ifndef __OPTIMIZER_LLVM_UTILS_INSTRUCTION_SIMPLIFY_H__
#define __OPTIMIZER_LLVM_UTILS_INSTRUCTION_SIMPLIFY_H__

#include "../pass.h"
#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <map>
#include <set>
#include <functional>

namespace Transform
{
    class InstructionSimplifyPass : public Pass
    {
      private:
        bool modified_;

      public:
        explicit InstructionSimplifyPass(LLVMIR::IR* ir);
        void Execute() override;
        bool wasModified() const { return modified_; }

      private:
        void processFunction(CFG* cfg);

        void reorderOperandsForConstants(LLVMIR::Instruction* inst);
        void convertSubtractionToAddition(LLVMIR::Instruction*& inst);
        void simplifyZeroResultOperations(LLVMIR::Instruction*& inst);
        void eliminateRedundantInstructions(CFG* cfg);
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_UTILS_INSTRUCTION_SIMPLIFY_H__
