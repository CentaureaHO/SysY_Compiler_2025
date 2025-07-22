#pragma once
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include <vector>
namespace LLVMIR
{
    class Expression
    {
      protected:
        // 比较下Operand的情况，因为是SSA模式，如果是REG，那么一定值不会变
        bool compareOperands(const Operand* op1, const Operand* op2) const
        {
            if (op1->type != op2->type) return false;
            OperandType type = op1->type;
            if (type == OperandType::GLOBAL)
            {
                // 对于全局变量，我们只需要比较名字
                auto globalOp1 = (const GlobalOperand*)op1;
                auto globalOp2 = (const GlobalOperand*)op2;
                // TODO：这里我们可能需要处理别的问题，比如SCCP没有处理GLOBAL的数值
                return false;
            }
            if (type == OperandType::IMMEI32)
            {
                // 对于立即数，我们只需要比较值
                auto immOp1 = (const ImmeI32Operand*)op1;
                auto immOP2 = (const ImmeI32Operand*)op2;
                return immOp1->value == immOP2->value;
            }
            if (type == OperandType::IMMEF32)
            {
                // 对于浮点数，我们只需要比较值
                auto immOp1 = (const ImmeF32Operand*)op1;
                auto immOP2 = (const ImmeF32Operand*)op2;
                return immOp1->value == immOP2->value;
            }
            if (type == OperandType::REG)
            {
                // 对于寄存器，我们只需要比较寄存器编号
                // 因为我们使用的是SSA模式，寄存器编号在SSA中是唯一的
                // 一旦寄存器被定义，它的编号就不会改变
                auto regOp1 = (const RegOperand*)op1;
                auto regOp2 = (const RegOperand*)op2;
                return regOp1->reg_num == regOp2->reg_num;
            }
            // 剩下的就是label和Imm63，因为我们没处理64,就先不处理
            return false;  // 其他类型不支持比较
        }

      public:
        // 用于表示LLVM IR中的表达式，在这里我们用这里记录表达式的相关信息
        IROpCode              opcode;    // 表达式的操作码
        std::vector<Operand*> operands;  // 表达式的操作数

        Expression(IROpCode op, std::vector<Operand*>& ops) : opcode(op), operands(ops) {}
        virtual ~Expression() = default;

        // 纯虚函数，子类必须实现
        virtual bool   equals(const Expression& other) const = 0;
        virtual size_t hash() const                          = 0;
    };

};  // namespace LLVMIR