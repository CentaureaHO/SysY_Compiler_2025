#pragma once
#include "./Expressin/Expression.h"
#include "unordered_map"

namespace LLVMIR
{
    class Value
    {
      public:
        // 用于表示LLVM IR中的值，可以是寄存器、全局变量等
        // 这里的值对于不同的情况会是不同的，但总的来说其实都是Operand
        // 对于结果为常数的Expression，即为常数Operand
        // 对于寄存器的Expression，即为寄存器Operand
        // 对于全局变量的Expression，即为全局变量Operand
        OperandType type;  // 值的类型
        std::string name;  // 值的名称

        Value(OperandType t, const std::string& n) : type(t), name(n) {}

        bool operator==(const Value& other) const { return type == other.type && name == other.name; }
    };

    class ValueMap
    {
      public:
        // 将表达式映射到其值
        void mapExpressionToValue(const Expression& expr, const Value& value);

        // 根据表达式获取其值
        Value getValueForExpression(const Expression& expr) const;

      private:
        // 存储表达式与值的映射关系
        std::unordered_map<Expression, Value> exprToValueMap;
    };
}  // namespace LLVMIR