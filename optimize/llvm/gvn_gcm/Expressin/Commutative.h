#pragma once
#include "Expression.h"
#include "llvm/gvn_gcm/Expressin/Binary.h"

namespace LLVMIR
{
    /*
     * @brief 可交换的二元表达式类，表示LLVM IR中的可交换二元操作
     * @details 该类继承自BinaryExpression，表示LLVM IR中的可交换二元操作表达式。
     */
    class CommutativeBinaryExpression : public BinaryExpression
    {
        /*
        // 可交换操作
        IROpCode::ADD    // 整数加法: a + b = b + a
        IROpCode::MUL    // 整数乘法: a * b = b * a
        IROpCode::FADD   // 浮点加法: a + b = b + a
        IROpCode::FMUL   // 浮点乘法: a * b = b * a
        IROpCode::BITXOR // 按位异或: a ^ b = b ^ a
        IROpCode::BITAND // 按位与:  a & b = b & a
        */
      public:
        CommutativeBinaryExpression(IROpCode op, std::vector<Operand*>& ops) : BinaryExpression(op, ops) {}

        bool equals(const Expression& other) const override
        {
            if (opcode != other.opcode || other.operands.size() != 2) return false;

            return (compareOperands(operands[0], other.operands[0]) &&
                       compareOperands(operands[1], other.operands[1])) ||
                   (compareOperands(operands[0], other.operands[1]) && compareOperands(operands[1], other.operands[0]));
        }
    };
}  // namespace LLVMIR