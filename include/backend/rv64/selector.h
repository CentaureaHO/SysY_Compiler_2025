#ifndef __BACKEND_RV64_SELECTOR_H__
#define __BACKEND_RV64_SELECTOR_H__

#include <vector>
#include <llvm_ir/ir_builder.h>
#include "rv64_function.h"

namespace Backend::RV64
{
    class Selector
    {
      public:
        LLVMIR::IR*                        ir;
        std::vector<Function*>&            functions;
        std::vector<LLVMIR::Instruction*>& glb_defs;
        Function*                          cur_func;
        Block*                             cur_block;

        std::unordered_map<int, Register>        ir_reg_mapper;    // ir_reg -> rv64_reg
        std::unordered_map<int, int>             alloc_shift_map;  // var in stack -> shift
        std::unordered_map<uint64_t, bool>       global_imm_vsd;   // already defined?
        std::map<Register, LLVMIR::Instruction*> cmp_context;

        int cur_offset;

      public:
        Selector(LLVMIR::IR* ir, std::vector<Function*>& funcs, std::vector<LLVMIR::Instruction*>& glb_defs);

      public:
        void selectInstruction();

      private:
        Register getReg(DataType* type);
        Register getLLVMReg(int ir_reg, DataType* type);

      private:
        Register extractIROp2Reg(LLVMIR::Operand* op, DataType* type);
        int      extractIROp2ImmeI32(LLVMIR::Operand* op);
        float    extractIROp2ImmeF32(LLVMIR::Operand* op);

      private:
        void convertAndAppend(LLVMIR::Instruction* inst);
        void convertAndAppend(LLVMIR::LoadInst* inst);
        void convertAndAppend(LLVMIR::StoreInst* inst);
        void convertAndAppend(LLVMIR::ArithmeticInst* inst);
        void convertAndAppend(LLVMIR::IcmpInst* inst);
        void convertAndAppend(LLVMIR::FcmpInst* inst);
        void convertAndAppend(LLVMIR::AllocInst* inst);
        void convertAndAppend(LLVMIR::BranchCondInst* inst);
        void convertAndAppend(LLVMIR::BranchUncondInst* inst);
        void convertAndAppend(LLVMIR::GlbvarDefInst* inst);
        void convertAndAppend(LLVMIR::CallInst* inst);
        void convertAndAppend(LLVMIR::RetInst* inst);
        void convertAndAppend(LLVMIR::GEPInst* inst);
        void convertAndAppend(LLVMIR::FuncDeclareInst* inst);
        void convertAndAppend(LLVMIR::FuncDefInst* inst);
        void convertAndAppend(LLVMIR::SI2FPInst* inst);
        void convertAndAppend(LLVMIR::FP2SIInst* inst);
        void convertAndAppend(LLVMIR::ZextInst* inst);
        void convertAndAppend(LLVMIR::FPExtInst* inst);
        void convertAndAppend(LLVMIR::PhiInst* inst);
        void convertAndAppend(LLVMIR::SelectInst* inst);

      private:
        void convertADD(LLVMIR::ArithmeticInst* inst);
        void convertSUB(LLVMIR::ArithmeticInst* inst);
        void convertMUL(LLVMIR::ArithmeticInst* inst);
        void convertDIV(LLVMIR::ArithmeticInst* inst);
        void convertFADD(LLVMIR::ArithmeticInst* inst);
        void convertFSUB(LLVMIR::ArithmeticInst* inst);
        void convertFMUL(LLVMIR::ArithmeticInst* inst);
        void convertFDIV(LLVMIR::ArithmeticInst* inst);
        void convertMOD(LLVMIR::ArithmeticInst* inst);
        void convertSHL(LLVMIR::ArithmeticInst* inst);
        void convertASHR(LLVMIR::ArithmeticInst* inst);
        void convertLSHR(LLVMIR::ArithmeticInst* inst);
        void convertBITXOR(LLVMIR::ArithmeticInst* inst);
        void convertBITAND(LLVMIR::ArithmeticInst* inst);
        void convertSMIN_I32(LLVMIR::ArithmeticInst* inst);
        void convertSMAX_I32(LLVMIR::ArithmeticInst* inst);
        void convertUMIN_I32(LLVMIR::ArithmeticInst* inst);
        void convertUMAX_I32(LLVMIR::ArithmeticInst* inst);
        void convertFMIN_F32(LLVMIR::ArithmeticInst* inst);
        void convertFMAX_F32(LLVMIR::ArithmeticInst* inst);
    };
}  // namespace Backend::RV64

#endif
