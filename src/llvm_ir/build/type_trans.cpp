#include <llvm_ir/build/type_trans.h>
#include <unordered_map>
#include <llvm_ir/defs.h>
#include <llvm_ir/ir_builder.h>
#include <cassert>
using namespace std;
using namespace LLVMIR;

#define INT(x) static_cast<int>(x)

using DT = DataType;

using UnaryIRFunc  = void (*)(IRBlock* block, int reg);
using BinaryIRFunc = void (*)(IRBlock* block, int lhs_reg, int rhs_reg);

extern IRTable irgen_table;
extern IR      builder;

extern FuncDefInst* cur_func;
extern Type*        ret_type;

extern int cur_label;
extern int loop_start_label;  // continue;
extern int loop_end_label;    // break;

extern int max_label;
extern int max_reg;

vector<DT> type2LLVM_vec = {
    DT::VOID,    // Type::Void
    DT::I32,     // Type::Int
    DT::I32,     // Type::LL, but cheat as I32
    DT::F32,     // Type::Float
    DT::DOUBLE,  // Type::Double, not implemented
    DT::I1,      // Type::Bool
    DT::PTR,     // Type::Ptr
    DT::PTR,     // Type::Arr, not implemented
    DT::PTR,     // Type::Func, not implemented
    DT::PTR,     // Type::CStr, not implemented
    DT::I8,      // Type::Char, not implemented
    DT::PTR      // Type::Str, not implemented
};

void IR_UnaryAddInt(IRBlock* block, int reg)
{
    // nothing to do
}

void IR_UnaryAddFloat(IRBlock* block, int reg)
{
    // nothing to do
}

void IR_UnarySubInt(IRBlock* block, int reg) { block->insertArithmeticI32_ImmeLeft(IROpCode::SUB, 0, reg, ++max_reg); }

void IR_UnarySubFloat(IRBlock* block, int reg)
{
    block->insertArithmeticF32_ImmeLeft(IROpCode::FSUB, 0, reg, ++max_reg);
}

void IR_UnaryNotInt(IRBlock* block, int reg) { block->insertIcmp_ImmeRight(IcmpCond::EQ, reg, 0, ++max_reg); }

void IR_UnaryNotFloat(IRBlock* block, int reg) { block->insertFcmp_ImmeRight(FcmpCond::OEQ, reg, 0, ++max_reg); }

unordered_map<int, UnaryIRFunc> UnaryIRInt = {
    {INT(OpCode::Add), IR_UnaryAddInt}, {INT(OpCode::Sub), IR_UnarySubInt}, {INT(OpCode::Not), IR_UnaryNotInt}};

unordered_map<int, UnaryIRFunc> UnaryIRFloat = {
    {INT(OpCode::Add), IR_UnaryAddFloat}, {INT(OpCode::Sub), IR_UnarySubFloat}, {INT(OpCode::Not), IR_UnaryNotFloat}};

void IR_UnaryInt(ExprNode* expr, OpCode op, IRBlock* block)
{
    expr->genIRCode();
    UnaryIRInt[INT(op)](block, max_reg);
    // IR_UnaryAddInt(block, max_reg);
}

void IR_UnaryFloat(ExprNode* expr, OpCode op, IRBlock* block)
{
    expr->genIRCode();
    UnaryIRFloat[INT(op)](block, max_reg);
    // IR_UnaryAddFloat(block, max_reg);
}

void IR_UnaryBool(ExprNode* expr, OpCode op, IRBlock* block)
{
    expr->genIRCode();
    block->insertTypeConvert(expr->attr.val.type->getKind(), TypeKind::Int, max_reg);
    UnaryIRInt[INT(op)](block, max_reg);
    // IR_UnaryAddInt(block, max_reg);
}

void IR_UnaryErr(ExprNode* expr, OpCode op, IRBlock* block) { assert(false); }

void IR_GenUnary(ExprNode* expr, OpCode op, IRBlock* block)
{
    switch (expr->attr.val.type->getKind())
    {
        case TypeKind::Int: IR_UnaryInt(expr, op, block); break;
        case TypeKind::LL: IR_UnaryInt(expr, op, block); break;  // 其它地方也作为int处理了
        case TypeKind::Float: IR_UnaryFloat(expr, op, block); break;
        case TypeKind::Bool: IR_UnaryBool(expr, op, block); break;
        default: IR_UnaryErr(expr, op, block); break;
    }
}

void IR_BinaryAddInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticI32(IROpCode::ADD, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryAddFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticF32(IROpCode::FADD, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinarySubInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticI32(IROpCode::SUB, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinarySubFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticF32(IROpCode::FSUB, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryMulInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticI32(IROpCode::MUL, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryMulFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticF32(IROpCode::FMUL, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryDivInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticI32(IROpCode::DIV, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryDivFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticF32(IROpCode::FDIV, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryModInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertArithmeticI32(IROpCode::MOD, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryModFloat(IRBlock* block, int lhs_reg, int rhs_reg) { assert(false); }

void IR_BinaryGtInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertIcmp(IcmpCond::SGT, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryGtFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertFcmp(FcmpCond::OGT, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryGeInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertIcmp(IcmpCond::SGE, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryGeFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertFcmp(FcmpCond::OGE, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryLtInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertIcmp(IcmpCond::SLT, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryLtFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertFcmp(FcmpCond::OLT, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryLeInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertIcmp(IcmpCond::SLE, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryLeFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertFcmp(FcmpCond::OLE, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryEqInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertIcmp(IcmpCond::EQ, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryEqFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertFcmp(FcmpCond::OEQ, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryNeqInt(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertIcmp(IcmpCond::NE, lhs_reg, rhs_reg, ++max_reg);
}

void IR_BinaryNeqFloat(IRBlock* block, int lhs_reg, int rhs_reg)
{
    block->insertFcmp(FcmpCond::ONE, lhs_reg, rhs_reg, ++max_reg);
}

map<int, BinaryIRFunc> BinaryIRInt = {{INT(OpCode::Add), IR_BinaryAddInt},
    {INT(OpCode::Sub), IR_BinarySubInt},
    {INT(OpCode::Mul), IR_BinaryMulInt},
    {INT(OpCode::Div), IR_BinaryDivInt},
    {INT(OpCode::Mod), IR_BinaryModInt},
    {INT(OpCode::Gt), IR_BinaryGtInt},
    {INT(OpCode::Ge), IR_BinaryGeInt},
    {INT(OpCode::Lt), IR_BinaryLtInt},
    {INT(OpCode::Le), IR_BinaryLeInt},
    {INT(OpCode::Eq), IR_BinaryEqInt},
    {INT(OpCode::Neq), IR_BinaryNeqInt}};

map<int, BinaryIRFunc> BinaryIRFloat = {{INT(OpCode::Add), IR_BinaryAddFloat},
    {INT(OpCode::Sub), IR_BinarySubFloat},
    {INT(OpCode::Mul), IR_BinaryMulFloat},
    {INT(OpCode::Div), IR_BinaryDivFloat},
    {INT(OpCode::Mod), IR_BinaryModFloat},
    {INT(OpCode::Gt), IR_BinaryGtFloat},
    {INT(OpCode::Ge), IR_BinaryGeFloat},
    {INT(OpCode::Lt), IR_BinaryLtFloat},
    {INT(OpCode::Le), IR_BinaryLeFloat},
    {INT(OpCode::Eq), IR_BinaryEqFloat},
    {INT(OpCode::Neq), IR_BinaryNeqFloat}};

void IR_BinaryBool_Bool(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    block->insertTypeConvert(lhs->attr.val.type->getKind(), TypeKind::Int, max_reg);
    int lhs_reg = max_reg;

    rhs->genIRCode();
    block->insertTypeConvert(rhs->attr.val.type->getKind(), TypeKind::Int, max_reg);

    BinaryIRInt[INT(op)](block, lhs_reg, max_reg);
}

void IR_BinaryBool_Int(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    block->insertTypeConvert(lhs->attr.val.type->getKind(), TypeKind::Int, max_reg);
    int lhs_reg = max_reg;

    rhs->genIRCode();

    BinaryIRInt[INT(op)](block, lhs_reg, max_reg);
}

void IR_BinaryBool_Float(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    block->insertTypeConvert(lhs->attr.val.type->getKind(), TypeKind::Float, max_reg);
    int lhs_reg = max_reg;

    rhs->genIRCode();

    BinaryIRFloat[INT(op)](block, lhs_reg, max_reg);
}

void IR_BinaryInt_Bool(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    int lhs_reg = max_reg;

    rhs->genIRCode();
    block->insertTypeConvert(rhs->attr.val.type->getKind(), TypeKind::Int, max_reg);

    BinaryIRInt[INT(op)](block, lhs_reg, max_reg);
}

void IR_BinaryInt_Int(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    int lhs_reg = max_reg;

    rhs->genIRCode();
    BinaryIRInt[INT(op)](block, lhs_reg, max_reg);
}

void IR_BinaryInt_Float(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    block->insertTypeConvert(lhs->attr.val.type->getKind(), TypeKind::Float, max_reg);
    int lhs_reg = max_reg;

    rhs->genIRCode();
    BinaryIRFloat[INT(op)](block, lhs_reg, max_reg);
}

void IR_BinaryFloat_Bool(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    int lhs_reg = max_reg;

    rhs->genIRCode();
    block->insertTypeConvert(rhs->attr.val.type->getKind(), TypeKind::Float, max_reg);

    BinaryIRFloat[INT(op)](block, lhs_reg, max_reg);
}

void IR_BinaryFloat_Int(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    int lhs_reg = max_reg;

    rhs->genIRCode();
    block->insertTypeConvert(rhs->attr.val.type->getKind(), TypeKind::Float, max_reg);

    BinaryIRFloat[INT(op)](block, lhs_reg, max_reg);
}

void IR_BinaryFloat_Float(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    lhs->genIRCode();
    int lhs_reg = max_reg;

    rhs->genIRCode();
    BinaryIRFloat[INT(op)](block, lhs_reg, max_reg);
}

void IR_GenBinary(ExprNode* lhs, ExprNode* rhs, OpCode op, IRBlock* block)
{
    switch (lhs->attr.val.type->getKind())
    {
        case TypeKind::Bool:
        {
            switch (rhs->attr.val.type->getKind())
            {
                case TypeKind::Bool: IR_BinaryBool_Bool(lhs, rhs, op, block); break;
                case TypeKind::Int: IR_BinaryBool_Int(lhs, rhs, op, block); break;
                case TypeKind::Float: IR_BinaryBool_Float(lhs, rhs, op, block); break;
                default: assert(false); break;
            }
            break;
        }
        case TypeKind::Int:
        {
            switch (rhs->attr.val.type->getKind())
            {
                case TypeKind::Bool: IR_BinaryInt_Bool(lhs, rhs, op, block); break;
                case TypeKind::Int: IR_BinaryInt_Int(lhs, rhs, op, block); break;
                case TypeKind::Float: IR_BinaryInt_Float(lhs, rhs, op, block); break;
                default: assert(false); break;
            }
            break;
        }
        case TypeKind::Float:
        {
            switch (rhs->attr.val.type->getKind())
            {
                case TypeKind::Bool: IR_BinaryFloat_Bool(lhs, rhs, op, block); break;
                case TypeKind::Int: IR_BinaryFloat_Int(lhs, rhs, op, block); break;
                case TypeKind::Float: IR_BinaryFloat_Float(lhs, rhs, op, block); break;
                default: assert(false); break;
            }
            break;
        }
        default: assert(false); break;
    }
}