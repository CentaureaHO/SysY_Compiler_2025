#pragma once
#include "Expression.h"

namespace LLVMIR
{
    /*
     * @brief Load表达式类，表示LLVM IR中的加载操作
     * @details 该类继承自Expression，表示LLVM IR中的加载操作表达
     */
    class LoadExpression : public Expression
    {
      public:
        LoadExpression(IROpCode op, std::vector<Operand*>& ops) : Expression(op, ops) {}

        bool equals(const Expression& other) const override
        {
            if (opcode != other.opcode || other.operands.size() != operands.size()) return false;

            // 对于LOAD，需要考虑内存别名分析
            // 此处可以调用别名分析模块判断
            return compareOperands(operands[0], other.operands[0]);
        }

        size_t hash() const override
        {
            // TODO:实现哈希
            size_t hash = std::hash<int>()(static_cast<int>(this->opcode));
            for (const auto& op : this->operands)
            {
                size_t operand_hash = hashOperand(op);
                // 使用乘法和位移组合哈希值，提高分布性
                hash = hash * 31 + operand_hash;
            }
            return hash;
        }
    };
}  // namespace LLVMIR