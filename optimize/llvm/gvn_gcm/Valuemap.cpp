#pragma once
#include "Valuemap.h"

namespace LLVMIR
{
    void ValueMap::mapExpressionToValue(const Expression*& expr, const Value& value)
    {
        // 将表达式映射到其值
        exprToValueMap[const_cast<Expression*>(expr)] = value;
    }

    Value ValueMap::getValueForExpression(const Expression*& expr) const
    {
        // 根据表达式获取其值
        auto it = exprToValueMap.find(const_cast<Expression*>(expr));
        if (it != exprToValueMap.end()) { return it->second; }
        // 如果没有找到，返回一个默认值
        return Value(false);
    }
}  // namespace LLVMIR