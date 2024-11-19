#ifndef __LLVMIR_DEFS_H__
#define __LLVMIR_DEFS_H__

namespace LLVMIR
{
    enum class DataType
    {
        I32    = 1,
        F32    = 2,
        PTR    = 3,
        VOID   = 4,
        I8     = 5,
        I1     = 6,
        I64    = 7,
        DOUBLE = 8
    };

    enum class OperandType
    {
        UNKNOWN = 0,
        REG     = 1,
        IMMEI32 = 2,
        IMMEF32 = 3,
        GLOBAL  = 4,
        LABEL   = 5,
        IMMEI64 = 6
    };

    enum class IROpCode
    {
        OTHER         = 0,
        LOAD          = 1,
        STORE         = 2,
        ADD           = 3,
        SUB           = 4,
        ICMP          = 5,
        PHI           = 6,
        ALLOCA        = 7,
        MUL           = 8,
        DIV           = 9,
        BR_COND       = 10,
        BR_UNCOND     = 11,
        FADD          = 12,
        FSUB          = 13,
        FMUL          = 14,
        FDIV          = 15,
        FCMP          = 16,
        MOD           = 17,
        BITXOR        = 18,
        RET           = 19,
        ZEXT          = 20,
        SHL           = 21,
        FPTOSI        = 24,
        GETELEMENTPTR = 25,
        CALL          = 26,
        SITOFP        = 27,
        GLOBAL_VAR    = 28,
        GLOBAL_STR    = 29,
    };

    enum class IcmpCond
    {
        eq  = 1,
        ne  = 2,
        ugt = 3,
        uge = 4,
        ult = 5,
        ule = 6,
        sgt = 7,
        sge = 8,
        slt = 9,
        sle = 10
    };

    enum class FcmpCond
    {
        FALSE = 1,
        OEQ   = 2,
        OGT   = 3,
        OGE   = 4,
        OLT   = 5,
        OLE   = 6,
        ONE   = 7,
        ORD   = 8,
        UEQ   = 9,
        UGT   = 10,
        UGE   = 11,
        ULT   = 12,
        ULE   = 13,
        UNE   = 14,
        UNO   = 15,
        TRUE  = 16
    };
}  // namespace LLVMIR

#endif