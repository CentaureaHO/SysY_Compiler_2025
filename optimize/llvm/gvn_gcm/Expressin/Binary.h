#pragma once
#include "Expression.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include <cstddef>
#include <functional>
namespace LLVMIR
{
    /*
     * @brief 二元表达式类，表示LLVM IR中的二元操作
     * @details 该类继承自Expression，表示LLVM IR中的二元操作表达式。不可以交换的类型
     */
    class BinaryExpression : public Expression
    {
        // 这里处理不能交换的
        /*
        // 不可交换操作
        IROpCode::SUB    // 减法:    a - b ≠ b - a
        IROpCode::DIV    // 除法:    a / b ≠ b / a
        IROpCode::FSUB   // 浮点减法: a - b ≠ b - a
        IROpCode::FDIV   // 浮点除法: a / b ≠ b / a
        IROpCode::MOD    // 取模:    a % b ≠ b % a
        IROpCode::SHL    // 左移:    a << b ≠ b << a
        IROpCode::ASHR   // 算术右移: a >> b ≠ b >> a
        IROpCode::LSHR   // 逻辑右移: a >>> b ≠ b >> a
        IROpCode::ICMP   // 整数比较: 大多数比较操作不可交换
        IROpCode::FCMP   // 浮点比较: 大多数比较操作不可交换
        */
      public:
        BinaryExpression(IROpCode op, std::vector<Operand*>& ops) : Expression(op, ops) {}

        // 实现equals方法，比较两个BinaryExpression是否相等
        bool equals(const Expression& other) const override
        {
            if (this->opcode != other.opcode || this->operands.size() != other.operands.size()) return false;

            for (size_t i = 0; i < this->operands.size(); ++i)
            {
                if (!compareOperands(this->operands[i], other.operands[i])) return false;
            }
            return true;
        }

        // 实现hash方法，计算BinaryExpression的哈希值
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
};  // namespace LLVMIR