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

        virtual std::string getName()         = 0;
        virtual int         GetRegNum() const = 0;
        virtual std::string GetGlobal() const = 0;
        virtual int         GetImm() const    = 0;
        virtual float       GetImmF() const   = 0;
    };

    struct OperandPtrHash
    {
        std::size_t operator()(const Operand* op) const
        {
            if (!op) return 0;
            switch (op->type)
            {
                case OperandType::REG: return std::hash<int>()(op->GetRegNum());
                case OperandType::IMMEI32: return std::hash<int>()(op->GetImm());
                case OperandType::IMMEF32: return std::hash<float>()(op->GetImmF());
                case OperandType::GLOBAL: return std::hash<std::string>()(op->GetGlobal());
                default: return std::hash<int>()(static_cast<int>(op->type));
            }
        }
    };

    struct OperandPtrEqual
    {
        bool operator()(const Operand* lhs, const Operand* rhs) const
        {
            if (!lhs || !rhs) return lhs == rhs;
            if (lhs->type != rhs->type) return false;

            switch (lhs->type)
            {
                case OperandType::REG: return lhs->GetRegNum() == rhs->GetRegNum();
                case OperandType::IMMEI32: return lhs->GetImm() == rhs->GetImm();
                case OperandType::IMMEF32: return lhs->GetImmF() == rhs->GetImmF();
                case OperandType::GLOBAL: return lhs->GetGlobal() == rhs->GetGlobal();
                default: return false;
            }
        }
    };

    class RegOperand : public Operand
    {
      public:
        int reg_num;

        virtual ~RegOperand() = default;

        virtual std::string getName();
        virtual int         GetRegNum() const;
        virtual std::string GetGlobal() const;
        virtual int         GetImm() const;
        virtual float       GetImmF() const;
        static RegOperand*  get(int num);

      private:
        RegOperand(int num);
    };

    class ImmeI32Operand : public Operand
    {
      public:
        int value;

        virtual ~ImmeI32Operand() = default;

        virtual std::string    getName();
        virtual int            GetRegNum() const;
        virtual std::string    GetGlobal() const;
        virtual int            GetImm() const;
        virtual float          GetImmF() const;
        static ImmeI32Operand* get(int value);

      private:
        ImmeI32Operand(int v);
    };

    class ImmeF32Operand : public Operand
    {
      public:
        float value;

        virtual ~ImmeF32Operand() = default;

        virtual std::string    getName();
        virtual int            GetRegNum() const;
        virtual std::string    GetGlobal() const;
        virtual int            GetImm() const;
        virtual float          GetImmF() const;
        static ImmeF32Operand* get(float value);

      private:
        ImmeF32Operand(float v);
    };

    class LabelOperand : public Operand
    {
      public:
        int label_num;

        virtual ~LabelOperand() = default;

        virtual std::string  getName();
        virtual int          GetRegNum() const;
        virtual std::string  GetGlobal() const;
        virtual int          GetImm() const;
        virtual float        GetImmF() const;
        static LabelOperand* get(int num);

      private:
        LabelOperand(int num);
    };

    class GlobalOperand : public Operand
    {
      public:
        std::string global_name;

        virtual ~GlobalOperand() = default;

        virtual std::string   getName();
        virtual int           GetRegNum() const;
        virtual std::string   GetGlobal() const;
        virtual int           GetImm() const;
        virtual float         GetImmF() const;
        static GlobalOperand* get(std::string name);

      private:
        GlobalOperand(const std::string name);
    };

    class Instruction
    {
      public:
        IROpCode    opcode;
        int         block_id;
        std::string comment;

        Instruction(IROpCode op = IROpCode::OTHER);
        virtual ~Instruction() = default;

        virtual void printIR(std::ostream& s) = 0;

        virtual void Rename(std::map<int, int>& replace) = 0;
        virtual void ReplaceAllOperands(std::map<int, int>& replace) = 0;  // 新增：替换所有操作数（包括结果寄存器）
        virtual void ReplaceAllOperands(std::map<int, Operand*>& replace) = 0;  // 替换所有操作数（包括结果寄存器）
        virtual int                   GetResultReg()     = 0;
        virtual std::vector<int>      GetUsedRegs()      = 0;
        virtual std::vector<Operand*> GetUsedOperands()  = 0;
        virtual Operand*              GetResultOperand() = 0;

        virtual Instruction* Clone(int new_result_reg = -1) const                             = 0;
        virtual void         SubstituteOperands(const std::map<int, Operand*>& substitutions) = 0;
        virtual DataType     GetResultType() const                                            = 0;
        virtual Instruction* CloneWithMapping(
            const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const = 0;
        virtual std::vector<Operand*> GetCSEOperands() const                              = 0;

        std::string  toString();
        virtual void ReplaceLabels(const std::map<int, int>& label_map) = 0;
        bool         isTerminator() const
        {
            return opcode == IROpCode::BR_COND || opcode == IROpCode::BR_UNCOND || opcode == IROpCode::RET;
        }
    };

    class LoadInst : public Instruction
    {
      public:
        DataType type;
        Operand* ptr;
        Operand* res;

        LoadInst(DataType t, Operand* p, Operand* r);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class StoreInst : public Instruction
    {
      public:
        DataType type;
        Operand* ptr;
        Operand* val;

        StoreInst(DataType t, Operand* p, Operand* v);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class ArithmeticInst : public Instruction
    {
      public:
        DataType type;
        Operand* lhs;
        Operand* rhs;
        Operand* res;

        ArithmeticInst(IROpCode op, DataType t, Operand* l, Operand* r, Operand* res);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
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

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
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

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class AllocInst : public Instruction
    {
      public:
        DataType         type;
        Operand*         res;
        std::vector<int> dims;

        AllocInst(DataType t, Operand* r, std::vector<int> d = {});

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class BranchCondInst : public Instruction
    {
      public:
        Operand* cond;
        Operand* true_label;
        Operand* false_label;

        BranchCondInst(Operand* c, Operand* t, Operand* f);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class BranchUncondInst : public Instruction
    {
      public:
        Operand* target_label;

        BranchUncondInst(Operand* t);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
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

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
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

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class RetInst : public Instruction
    {
      public:
        DataType ret_type;
        Operand* ret;

        RetInst(DataType t, Operand* r);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
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

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class FuncDeclareInst : public Instruction
    {
      public:
        DataType              ret_type;
        std::string           func_name;
        std::vector<DataType> arg_types;
        bool                  is_var_arg;

        FuncDeclareInst(DataType rt, std::string fn, std::vector<DataType> at = {}, bool is_va = false);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class FuncDefInst : public FuncDeclareInst
    {
      public:
        std::vector<Operand*> arg_regs;

        FuncDefInst(DataType rt, std::string fn, std::vector<DataType> at = {});

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class SI2FPInst : public Instruction
    {
      public:
        Operand* f_si;
        Operand* t_fp;

        SI2FPInst(Operand* f, Operand* t);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class FP2SIInst : public Instruction
    {
      public:
        Operand* f_fp;
        Operand* t_si;

        FP2SIInst(Operand* f, Operand* t);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class ZextInst : public Instruction
    {
      public:
        DataType from;
        DataType to;
        Operand* src;
        Operand* dest;

        ZextInst(DataType f, DataType t, Operand* s, Operand* d);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class FPExtInst : public Instruction
    {
      public:
        Operand* src;
        Operand* dest;

        FPExtInst(Operand* s, Operand* d);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    class PhiInst : public Instruction  // phi valsa_for_labels.first valsa_for_labels.second val在前 label在后
    {
      public:
        DataType type;
        Operand* res;
        using ValOp   = Operand*;
        using LabelOp = Operand*;
        std::vector<std::pair<ValOp, LabelOp>> vals_for_labels;

        PhiInst(DataType t, Operand* r, const std::vector<std::pair<ValOp, LabelOp>>* vfl = nullptr);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;

        void Insert_into_PHI(ValOp val, LabelOp label)
        {
            vals_for_labels.push_back(std::pair<ValOp, LabelOp>(val, label));
        }
        void ReplaceLabels(const std::map<int, int>& label_map) override;

        Operand* GetValOperandOfBlock(int block_id);
        void     SetValOperandOfBlock(int block_id, int reg_id);
        void     ErasePhiWithBlock(int block_id);
        void     SetNewFrom(int old_from, int new_from);
    };

    class SelectInst : public Instruction
    {
      public:
        DataType type;
        Operand* cond;
        Operand* true_val;
        Operand* false_val;
        Operand* res;

        SelectInst(DataType t, Operand* tv, Operand* fv, Operand* c, Operand* r);

        virtual void printIR(std::ostream& s) override;

        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };

    // empty inst, just for debug
    class EmptyInst : public Instruction
    {
      public:
        EmptyInst();

        virtual void                  printIR(std::ostream& s) override;
        virtual void                  Rename(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, int>& replace) override;
        virtual void                  ReplaceAllOperands(std::map<int, Operand*>& replace) override;
        virtual int                   GetResultReg() override;
        virtual std::vector<int>      GetUsedRegs() override;
        virtual std::vector<Operand*> GetUsedOperands() override;
        virtual Operand*              GetResultOperand() override;
        virtual Instruction*          Clone(int new_result_reg = -1) const override;
        virtual void                  SubstituteOperands(const std::map<int, Operand*>& substitutions) override;
        virtual DataType              GetResultType() const override;
        virtual Instruction*          CloneWithMapping(
                     const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const override;
        virtual std::vector<Operand*> GetCSEOperands() const override;
        void                          ReplaceLabels(const std::map<int, int>& label_map) override;
    };
}  // namespace LLVMIR

// 操作数复制辅助函数声明
LLVMIR::Operand* copyOperand(
    LLVMIR::Operand* operand, const std::map<int, int>& reg_map, const std::map<int, int>& label_map);

// 根据数据类型创建中转指令的函数声明
LLVMIR::Instruction* createCopyInst(LLVMIR::DataType type, LLVMIR::Operand* src, LLVMIR::Operand* dest);

std::ostream& operator<<(std::ostream& s, LLVMIR::Operand* op);

LLVMIR::ImmeI32Operand* getImmeI32Operand(int num);
LLVMIR::ImmeF32Operand* getImmeF32Operand(float num);
LLVMIR::RegOperand*     getRegOperand(int num);
LLVMIR::LabelOperand*   getLabelOperand(int num);
LLVMIR::GlobalOperand*  getGlobalOperand(std::string name);

#endif
