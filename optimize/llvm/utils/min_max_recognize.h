#ifndef __MIN_MAX_RECOGNIZE_H__
#define __MIN_MAX_RECOGNIZE_H__

#include "../pass.h"
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/defs.h>

namespace Transform
{
    class MinMaxRecognizePass : public Pass
    {
      private:
        bool modified;

        bool recognizePhiMinMax(
            LLVMIR::PhiInst* phi_inst, LLVMIR::IRBlock* block, std::map<int, LLVMIR::Instruction*>& define_map);
        bool recognizeSelectMinMax(LLVMIR::SelectInst* select_inst, LLVMIR::IRBlock* block);
        bool recognizeArrayMinMax(
            LLVMIR::BranchCondInst* br_inst, LLVMIR::IRBlock* block, std::map<int, LLVMIR::Instruction*>& define_map);

        LLVMIR::IROpCode getMinMaxOpCode(LLVMIR::IcmpCond cond, bool is_min, bool is_signed);
        LLVMIR::IROpCode getFMinMaxOpCode(LLVMIR::FcmpCond cond, bool is_min);

        bool recognizeIntegerSelectMinMax(LLVMIR::SelectInst* select_inst, LLVMIR::IcmpInst* icmp_inst,
            LLVMIR::IRBlock* block, std::map<int, LLVMIR::Instruction*>& define_map);

        bool recognizeFloatSelectMinMax(LLVMIR::SelectInst* select_inst, LLVMIR::FcmpInst* fcmp_inst,
            LLVMIR::IRBlock* block, std::map<int, LLVMIR::Instruction*>& define_map);

      public:
        MinMaxRecognizePass(LLVMIR::IR* ir);
        void Execute() override;
        bool wasModified() const { return modified; }
    };
}  // namespace Transform

#endif
