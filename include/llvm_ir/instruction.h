#ifndef __LLVMIR_INSTRUCTION_H__
#define __LLVMIR_INSTRUCTION_H__

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <common/type/type_defs.h>
#include <llvm_ir/defs.h>

#define IS_BR(inst) (inst->opcode == LLVMIR::IROpCode::BR_COND || inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
#define IS_RET(inst) (inst->opcode == LLVMIR::IROpCode::RET)

// Inst不需要管理其操作数内存，统一交由Inst/Cleaner管理

namespace LLVMIR
{
    class Operand
    {
      public:
        OperandType type;

        Operand(OperandType t = OperandType::UNKNOWN);
        virtual ~Operand() = default;

        virtual std::string getName() = 0;
    };

    class RegOperand : public Operand
    {
      public:
        int reg_num;

        RegOperand(int num);
        virtual ~RegOperand() = default;

        virtual std::string getName();
    };

    class ImmeI32Operand : public Operand
    {
      public:
        int value;

        ImmeI32Operand(int v);
        virtual ~ImmeI32Operand() = default;

        virtual std::string getName();
    };

    class ImmeF32Operand : public Operand
    {
      public:
        float value;

        ImmeF32Operand(float v);
        virtual ~ImmeF32Operand() = default;

        virtual std::string getName();
    };

    class LabelOperand : public Operand
    {
      public:
        int label_num;

        LabelOperand(int num);
        virtual ~LabelOperand() = default;

        virtual std::string getName();
    };

    class GlobalOperand : public Operand
    {
      public:
        std::string global_name;

        GlobalOperand(std::string name);
        virtual ~GlobalOperand() = default;

        virtual std::string getName();
    };

    class Instruction
    {
      public:
        IROpCode opcode;

        Instruction(IROpCode op = IROpCode::OTHER);
        virtual ~Instruction() = default;

        virtual void printIR(std::ostream& s) = 0;
    };

    class LoadInst : public Instruction
    {
      public:
        DataType type;
        Operand* ptr;
        Operand* res;

        LoadInst(DataType t, Operand* p, Operand* r);

        virtual void printIR(std::ostream& s);
    };

    class StoreInst : public Instruction
    {
      public:
        DataType type;
        Operand* ptr;
        Operand* val;

        StoreInst(DataType t, Operand* p, Operand* v);

        virtual void printIR(std::ostream& s);
    };

    class ArithmeticInst : public Instruction
    {
      public:
        DataType type;
        Operand* lhs;
        Operand* rhs;
        Operand* res;

        ArithmeticInst(IROpCode op, DataType t, Operand* l, Operand* r, Operand* res);

        virtual void printIR(std::ostream& s);
    };

    class IcmpInst : public Instruction
    {
      public:
        DataType type;
        IcmpCond cond;
        Operand* lhs;
        Operand* rhs;
        Operand* res;

        IcmpInst(DataType t, IcmpCond c, Operand* l, Operand* r, Operand* res);

        virtual void printIR(std::ostream& s);
    };

    class FcmpInst : public Instruction
    {
      public:
        DataType type;
        FcmpCond cond;
        Operand* lhs;
        Operand* rhs;
        Operand* res;

        FcmpInst(DataType t, FcmpCond c, Operand* l, Operand* r, Operand* res);

        virtual void printIR(std::ostream& s);
    };

    class AllocInst : public Instruction
    {
      public:
        DataType         type;
        Operand*         res;
        std::vector<int> dims;

        AllocInst(DataType t, Operand* r, std::vector<int> d = {});

        virtual void printIR(std::ostream& s);
    };

    class BranchCondInst : public Instruction
    {
      public:
        Operand* cond;
        Operand* true_label;
        Operand* false_label;

        BranchCondInst(Operand* c, Operand* t, Operand* f);

        virtual void printIR(std::ostream& s);
    };

    class BranchUncondInst : public Instruction
    {
      public:
        Operand* target_label;

        BranchUncondInst(Operand* t);

        virtual void printIR(std::ostream& s);
    };

    class GlbvarDefInst : public Instruction
    {
      public:
        DataType     type;
        std::string  name;
        Operand*     init;
        VarAttribute arr_init;

        GlbvarDefInst(DataType t, std::string n, Operand* i);
        GlbvarDefInst(DataType t, std::string n, VarAttribute a);

        virtual void printIR(std::ostream& s);
    };

    class CallInst : public Instruction
    {
      public:
        DataType                                   ret_type;
        std::string                                func_name;
        std::vector<std::pair<DataType, Operand*>> args;
        Operand*                                   res;

        CallInst(DataType rt, std::string fn, Operand* r);
        CallInst(DataType rt, std::string fn, std::vector<std::pair<DataType, Operand*>> a, Operand* r);

        virtual void printIR(std::ostream& s);
    };

    class RetInst : public Instruction
    {
      public:
        DataType ret_type;
        Operand* ret;

        RetInst(DataType t, Operand* r);

        virtual void printIR(std::ostream& s);
    };

    class GEPInst : public Instruction
    {
      public:
        DataType              type;
        DataType              idx_type;
        Operand*              base_ptr;
        Operand*              res;
        std::vector<int>      dims;
        std::vector<Operand*> idxs;

        GEPInst(
            DataType t, DataType it, Operand* bp, Operand* r, std::vector<int> d = {}, std::vector<Operand*> is = {});

        virtual void printIR(std::ostream& s);
    };

    class FuncDeclareInst : public Instruction
    {
      public:
        DataType              ret_type;
        std::string           func_name;
        std::vector<DataType> arg_types;

        FuncDeclareInst(DataType rt, std::string fn, std::vector<DataType> at = {});

        virtual void printIR(std::ostream& s);
    };

    class FuncDefInst : public FuncDeclareInst
    {
      public:
        std::vector<Operand*> arg_regs;

        FuncDefInst(DataType rt, std::string fn, std::vector<DataType> at = {});

        virtual void printIR(std::ostream& s);
    };

    class SI2FPInst : public Instruction
    {
      public:
        Operand* f_si;
        Operand* t_fp;

        SI2FPInst(Operand* f, Operand* t);

        virtual void printIR(std::ostream& s);
    };

    class FP2SIInst : public Instruction
    {
      public:
        Operand* f_fp;
        Operand* t_si;

        FP2SIInst(Operand* f, Operand* t);

        virtual void printIR(std::ostream& s);
    };

    class ZextInst : public Instruction
    {
      public:
        DataType from;
        DataType to;
        Operand* src;
        Operand* dest;

        ZextInst(DataType f, DataType t, Operand* s, Operand* d);

        virtual void printIR(std::ostream& s);
    };
}  // namespace LLVMIR

std::ostream& operator<<(std::ostream& s, LLVMIR::Operand* op);

LLVMIR::ImmeI32Operand* getImmeI32Operand(int num);
LLVMIR::ImmeF32Operand* getImmeF32Operand(float num);
LLVMIR::RegOperand*     getRegOperand(int num);
LLVMIR::LabelOperand*   getLabelOperand(int num);
LLVMIR::GlobalOperand*  getGlobalOperand(std::string name);

#endif