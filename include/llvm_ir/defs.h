#ifndef __LLVMIR_DEFS_H__
#define __LLVMIR_DEFS_H__

#include <iostream>

#define IR_DATATYPE  \
    X(I32, i32, 1)   \
    X(F32, float, 2) \
    X(PTR, ptr, 3)   \
    X(VOID, void, 4) \
    X(I8, i8, 5)     \
    X(I1, i1, 6)     \
    X(I64, i64, 7)   \
    X(DOUBLE, double, 8)

#define IR_OPERAND \
    X(UNKNOWN, 0)  \
    X(REG, 1)      \
    X(IMMEI32, 2)  \
    X(IMMEF32, 3)  \
    X(GLOBAL, 4)   \
    X(LABEL, 5)    \
    X(IMMEI64, 6)

#define IR_OPCODE              \
    X(OTHER, other, 0)         \
    X(LOAD, load, 1)           \
    X(STORE, store, 2)         \
    X(ADD, add, 3)             \
    X(SUB, sub, 4)             \
    X(ICMP, icmp, 5)           \
    X(PHI, phi, 6)             \
    X(ALLOCA, alloca, 7)       \
    X(MUL, mul, 8)             \
    X(DIV, sdiv, 9)            \
    X(BR_COND, br, 10)         \
    X(BR_UNCOND, br, 11)       \
    X(FADD, fadd, 12)          \
    X(FSUB, fsub, 13)          \
    X(FMUL, fmul, 14)          \
    X(FDIV, fdiv, 15)          \
    X(FCMP, fcmp, 16)          \
    X(MOD, srem, 17)           \
    X(BITXOR, xor, 18)         \
    X(RET, NONE, 19)           \
    X(ZEXT, NONE, 20)          \
    X(SHL, shl, 21)            \
    X(FPTOSI, NONE, 24)        \
    X(GETELEMENTPTR, NONE, 25) \
    X(CALL, NONE, 26)          \
    X(SITOFP, NONE, 27)        \
    X(GLOBAL_VAR, NONE, 28)    \
    X(GLOBAL_STR, NONE, 29)    \
    X(FPEXT, NONE, 30)

#define IR_ICMP    \
    X(EQ, eq, 1)   \
    X(NE, ne, 2)   \
    X(UGT, ugt, 3) \
    X(UGE, uge, 4) \
    X(ULT, ult, 5) \
    X(ULE, ule, 6) \
    X(SGT, sgt, 7) \
    X(SGE, sge, 8) \
    X(SLT, slt, 9) \
    X(SLE, sle, 10)

#define IR_FCMP        \
    X(FALSE, false, 1) \
    X(OEQ, oeq, 2)     \
    X(OGT, ogt, 3)     \
    X(OGE, oge, 4)     \
    X(OLT, olt, 5)     \
    X(OLE, ole, 6)     \
    X(ONE, one, 7)     \
    X(ORD, ord, 8)     \
    X(UEQ, ueq, 9)     \
    X(UGT, ugt, 10)    \
    X(UGE, uge, 11)    \
    X(ULT, ult, 12)    \
    X(ULE, ule, 13)    \
    X(UNE, une, 14)    \
    X(UNO, uno, 15)    \
    X(TRUE, true, 16)

namespace LLVMIR
{
    enum class DataType
    {
#define X(type, name, idx) type = idx,
        IR_DATATYPE
#undef X
    };

    enum class OperandType
    {
#define X(op, idx) op = idx,
        IR_OPERAND
#undef X
    };

    enum class IROpCode
    {
#define X(op, name, idx) op = idx,
        IR_OPCODE
#undef X
    };

    enum class IcmpCond
    {
#define X(cond, name, idx) cond = idx,
        IR_ICMP
#undef X
    };

    enum class FcmpCond
    {
#define X(cond, name, idx) cond = idx,
        IR_FCMP
#undef X
    };

}  // namespace LLVMIR

std::ostream& operator<<(std::ostream& os, LLVMIR::DataType type);
std::ostream& operator<<(std::ostream& os, LLVMIR::IROpCode type);
std::ostream& operator<<(std::ostream& os, LLVMIR::IcmpCond type);
std::ostream& operator<<(std::ostream& os, LLVMIR::FcmpCond type);

#endif
