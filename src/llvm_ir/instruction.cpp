#include <llvm_ir/instruction.h>
using namespace std;
using namespace LLVMIR;

using DT = DataType;
using OT = OperandType;

Operand::Operand(OperandType t) : type(t) {}

RegOperand::RegOperand(int num) : Operand(OT::REG), reg_num(num) {}
string RegOperand::getName()
{
    // todo
    return "";
}

ImmeI32Operand::ImmeI32Operand(int v) : Operand(OT::IMMEI32), value(v) {}
string ImmeI32Operand::getName()
{
    // todo
    return "";
}

ImmeI64Operand::ImmeI64Operand(long long v) : Operand(OT::IMMEI64), value(v) {}
string ImmeI64Operand::getName()
{
    // todo
    return "";
}

ImmeF32Operand::ImmeF32Operand(float v) : Operand(OT::IMMEF32), value(v) {}
string ImmeF32Operand::getName()
{
    // todo
    return "";
}

LabelOperand::LabelOperand(int num) : Operand(OT::LABEL), label_num(num) {}
string LabelOperand::getName()
{
    // todo
    return "";
}

GlobalOperand::GlobalOperand(string name) : Operand(OT::GLOBAL), global_name(name) {}
string GlobalOperand::getName()
{
    // todo
    return "";
}

/*
    Followed insts
 */
Instruction::Instruction(IROpCode op) : opcode(op) {}

LoadInst::LoadInst(DataType t, Operand* p, Operand* r) : Instruction(IROpCode::LOAD), type(t), ptr(p), res(r) {}
void LoadInst::PrintIR(ostream& s)
{
    // todo
}

StoreInst::StoreInst(DataType t, Operand* p, Operand* r) : Instruction(IROpCode::STORE), type(t), ptr(p), val(r) {}
void StoreInst::PrintIR(ostream& s)
{
    // todo
}

ArithmeticInst::ArithmeticInst(IROpCode op, DataType t, Operand* l, Operand* r, Operand* res)
    : Instruction(op), type(t), lhs(l), rhs(r), result(res)
{}
void ArithmeticInst::PrintIR(ostream& s)
{
    // todo
}

IcmpInst::IcmpInst(DataType t, IcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::ICMP), type(t), cond(c), lhs(l), rhs(r), result(res)
{}
void IcmpInst::PrintIR(ostream& s)
{
    // todo
}