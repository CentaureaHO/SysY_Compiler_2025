#pragma once
#include "./Expressin/Expression.h"
#include "llvm_ir/instruction.h"
#include "unordered_map"

namespace LLVMIR
{
    class Value
    {
      public:
        // 其实就是一个RegOperand，我们可以直接记录
        Operand* operand;
        bool     notfound;  // 是否找到对应的值
        // Value(Operand* op, bool notfound = false) : operand(op), notfound(notfound) {}
        // Value(bool notfound) : operand(nullptr), notfound(notfound) {}
    };

    class ValueMap
    {
      public:
        // 将表达式映射到其值
        void mapExpressionToValue(const Expression*& expr, const Value& value);

        // 根据表达式获取其值
        Value getValueForExpression(const Expression*& expr) const;

      private:
        // 存储表达式与值的映射关系
        std::unordered_map<Expression*, Value, ExpressionPtrHash, ExpressionPtrEqual> exprToValueMap;
    };
}  // namespace LLVMIR