#ifndef __LLVMIR_BLOCK_H__
#define __LLVMIR_BLOCK_H__

#include <deque>
#include <iostream>
#include <set>
#include <vector>
#include <llvm_ir/instruction.h>
#include <llvm_ir/defs.h>

namespace LLVMIR
{
    class IRBlock
    {
      public:
        std::string comment;
        int         block_id;

        std::deque<Instruction*> insts;

        IRBlock(int id = 0);
        ~IRBlock();

        void printIR(std::ostream& s);

      public:
        // void insertInst(Instruction* inst, bool is_back = 1);

      public:
        void insertLoad(DataType t, Operand* ptr, int res_reg);
        void insertStore(DataType t, int val_reg, Operand* ptr);
        void insertStore(DataType t, Operand* val, Operand* ptr);

        void insertArithmeticI32(IROpCode op, int lhs_reg, int rhs_reg, int res_reg);
        void insertArithmeticF32(IROpCode op, int lhs_reg, int rhs_reg, int res_reg);

        void insertArithmeticI32_ImmeLeft(IROpCode op, int lhs_val, int rhs_reg, int res_reg);
        void insertArithmeticF32_ImmeLeft(IROpCode op, float lhs_val, int rhs_reg, int res_reg);

        void insertArithmeticI32_ImmeAll(IROpCode op, int lhs_val, int rhs_val, int res_reg);
        void insertArithmeticF32_ImmeAll(IROpCode op, float lhs_val, float rhs_val, int res_reg);

        void insertIcmp(IcmpCond cond, int lhs_reg, int rhs_reg, int res_reg);
        void insertFcmp(FcmpCond cond, int lhs_reg, int rhs_reg, int res_reg);

        void insertIcmp_ImmeRight(IcmpCond cond, int lhs_reg, int rhs_val, int res_reg);
        void insertFcmp_ImmeRight(FcmpCond cond, int lhs_reg, float rhs_val, int res_reg);

        void insertFP2SI(int src_reg, int dest_reg);
        void insertSI2FP(int src_reg, int dest_reg);
        void insertZextI1toI32(int src_reg, int dest_reg);

        void insertGEP_I32(DataType t, Operand* ptr, std::vector<int> dims, std::vector<Operand*> is, int res_reg);
        void insertGEP_I64(DataType t, Operand* ptr, std::vector<int> dims, std::vector<Operand*> is, int res_reg);

        void insertCall(
            DataType t, std::string func_name, std::vector<std::pair<DataType, Operand*>> args, int res_reg);
        void insertCallVoid(DataType t, std::string func_name, std::vector<std::pair<DataType, Operand*>> args);
        void insertCallNoargs(DataType t, std::string func_name, int res_reg);
        void insertCallVoidNoargs(DataType t, std::string func_name);

        void insertRetReg(DataType t, int reg);
        void insertRetImmI32(DataType t, int val);
        void insertRetImmF32(DataType t, float val);
        void insertRetVoid();

        void insertCondBranch(int cond_reg, int true_label, int false_label);
        void insertUncondBranch(int dst_label);

        void insertAlloc(DataType t, int reg);
        void insertAllocArray(DataType t, std::vector<int> dims, int reg);

        void insertTypeConvert(TypeKind from, TypeKind to, int src_reg);
    };
}  // namespace LLVMIR

#endif