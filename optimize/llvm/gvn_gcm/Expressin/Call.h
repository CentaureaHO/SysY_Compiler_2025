#pragma once
#include "Expression.h"

namespace LLVMIR
{
    /*
     * @brief Call表达式类，表示LLVM IR中的函数调用操作
     * @details 该类继承自Expression，表示LLVM IR中的函数调用表达式。
     */
    class CallExpression : public Expression
    {
      public:
        CallExpression(IROpCode op, std::vector<Operand*>& ops) : Expression(op, ops) {}

        // 这里需要注意的是，我们需要考虑是否在函数内有输入，所以我们需要考虑
        bool equals(const Expression& other) const override
        {
            // TODO：分析函数，考虑函数内是否有输入
            return false;  // 这里我们不考虑函数调用的情况
            if (opcode != other.opcode || other.operands.size() != operands.size()) return false;

            // 首先比较被调用的函数
            if (!compareOperands(operands[0], other.operands[0])) return false;

            // 然后比较参数
            for (size_t i = 1; i < operands.size(); ++i)
            {
                if (!compareOperands(operands[i], other.operands[i])) return false;
            }
            // return true;
        }

        size_t hash() const override
        {
            size_t hash = std::hash<IROpCode>()(opcode);
            for (const auto& op : operands) { hash = hash * 31 + Expression::hashOperand(op); }
            return hash;
        }
    };
}  // namespace LLVMIR