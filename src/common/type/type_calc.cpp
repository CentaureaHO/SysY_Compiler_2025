#include <common/type/type_calc.h>
#include <unordered_map>
#include <limits>
#include <iostream>
#include <vector>
#include <string>
#include <climits>
using namespace std;

using UnaryFunc  = NodeAttribute (*)(const NodeAttribute& a);
using BinaryFunc = NodeAttribute (*)(const NodeAttribute& a, const NodeAttribute& b);

#define IN_INT(lvalue) (lvalue >= std::numeric_limits<int>::min() && lvalue <= std::numeric_limits<int>::max())

extern std::vector<std::string> semanticErrMsgs;

namespace
{
    NodeAttribute UnaryAddInt(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst) result.val.value = TO_INT(node.val.value);
        return result;
    }

    NodeAttribute UnaryAddLL(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst) result.val.value = TO_LL(node.val.value);
        return result;
    }

    NodeAttribute UnaryAddFloat(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = floatType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst) result.val.value = std::get<float>(node.val.value);
        return result;
    }

    NodeAttribute UnarySubInt(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst)
        {
            int intValue = TO_INT(node.val.value);
            if (intValue == INT_MIN)
            {
                result.val.type = llType;
                // cout << "\n" << 2147483648LL << "\n";
                result.val.value = 2147483648LL;
                return result;
            }

            long long longValue = -TO_INT(node.val.value);
            if (IN_INT(longValue))
                result.val.value = static_cast<int>(longValue);
            else
            {
                result.val.value = longValue;
                result.val.type  = llType;
            }
        }

        return result;
    }

    NodeAttribute UnarySubLL(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst)
        {
            long long longValue = -TO_LL(node.val.value);

            if (IN_INT(longValue))
            {
                // cout << "Dbg in subll int: " << longValue << endl;
                // cout << "To int: " << static_cast<int>(longValue) << "\t";
                result.val.value = static_cast<int>(longValue);
                result.val.type  = intType;
            }
            else
            {
                // cout << "Dbg in subll ll: " << longValue << endl;
                result.val.value = longValue;
            }
        }

        return result;
    }

    NodeAttribute UnarySubFloat(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = floatType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst) result.val.value = -std::get<float>(node.val.value);
        return result;
    }

    NodeAttribute NotInt(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(!TO_INT(node.val.value));
        return result;
    }

    NodeAttribute NotLL(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(!TO_LL(node.val.value));
        return result;
    }

    NodeAttribute NotFloat(const NodeAttribute& node)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = node.val.isConst;
        result.line_num    = node.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(!std::get<float>(node.val.value));
        return result;
    }

    NodeAttribute BinaryAddInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = static_cast<long long>(TO_INT(lhs.val.value)) + TO_INT(rhs.val.value);
            if (IN_INT(longValue))
                result.val.value = static_cast<int>(longValue);
            else
            {
                result.val.value = longValue;
                result.val.type  = llType;
            }
        }

        return result;
    }

    NodeAttribute BinaryAddLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = TO_LL(lhs.val.value) + TO_LL(rhs.val.value);
            if (IN_INT(longValue))
            {
                result.val.value = static_cast<int>(longValue);
                result.val.type  = intType;
            }
            else
                result.val.value = longValue;
        }

        return result;
    }

    NodeAttribute BinaryAddFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = floatType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<float>(std::get<float>(lhs.val.value) + std::get<float>(rhs.val.value));

        return result;
    }

    NodeAttribute BinarySubInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = static_cast<long long>(TO_INT(lhs.val.value)) - TO_INT(rhs.val.value);
            if (IN_INT(longValue))
                result.val.value = static_cast<int>(longValue);
            else
            {
                result.val.value = longValue;
                result.val.type  = llType;
            }
        }

        return result;
    }

    NodeAttribute BinarySubLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = TO_LL(lhs.val.value) - TO_LL(rhs.val.value);
            if (IN_INT(longValue))
            {
                result.val.value = static_cast<int>(longValue);
                result.val.type  = intType;
            }
            else
                result.val.value = longValue;
        }

        return result;
    }

    NodeAttribute BinarySubFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = floatType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = std::get<float>(lhs.val.value) - std::get<float>(rhs.val.value);
        return result;
    }

    NodeAttribute MulInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = static_cast<long long>(TO_INT(lhs.val.value)) * TO_INT(rhs.val.value);
            if (IN_INT(longValue))
                result.val.value = static_cast<int>(longValue);
            else
            {
                result.val.value = longValue;
                result.val.type  = llType;
            }
        }

        return result;
    }

    NodeAttribute MulLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = TO_LL(lhs.val.value) * TO_LL(rhs.val.value);
            if (IN_INT(longValue))
            {
                result.val.value = static_cast<int>(longValue);
                result.val.type  = intType;
            }
            else
                result.val.value = longValue;
        }

        return result;
    }

    NodeAttribute MulFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = floatType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = std::get<float>(lhs.val.value) * std::get<float>(rhs.val.value);
        return result;
    }

    NodeAttribute DivInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        int rhsValue = TO_INT(rhs.val.value);
        if (rhsValue == 0 && rhs.val.isConst)
        {
            semanticErrMsgs.push_back("Division by zero at line " + std::to_string(lhs.line_num));
            return NodeAttribute(OpCode::PlaceHolder, ConstValue(), lhs.line_num);
        }

        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = static_cast<long long>(TO_INT(lhs.val.value)) / rhsValue;
            if (IN_INT(longValue))
                result.val.value = static_cast<int>(longValue);
            else
            {
                result.val.value = longValue;
                result.val.type  = llType;
            }
        }

        return result;
    }

    NodeAttribute DivLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        long long rhsValue = TO_LL(rhs.val.value);
        if (rhsValue == 0 && rhs.val.isConst)
        {
            semanticErrMsgs.push_back("Division by zero at line " + std::to_string(lhs.line_num));
            return NodeAttribute(OpCode::PlaceHolder, ConstValue(), lhs.line_num);
        }

        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = TO_LL(lhs.val.value) / rhsValue;
            if (IN_INT(longValue))
            {
                result.val.value = static_cast<int>(longValue);
                result.val.type  = intType;
            }
            else
                result.val.value = longValue;
        }

        return result;
    }

    NodeAttribute DivFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        float rhsValue = std::get<float>(rhs.val.value);
        if (rhsValue == 0 && rhs.val.isConst)
        {
            semanticErrMsgs.push_back("Division by zero at line " + std::to_string(lhs.line_num));
            return NodeAttribute(OpCode::PlaceHolder, ConstValue(), lhs.line_num);
        }

        NodeAttribute result;
        result.val.type    = floatType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = std::get<float>(lhs.val.value) / rhsValue;
        return result;
    }

    NodeAttribute ModInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        int rhsValue = TO_INT(rhs.val.value);
        if (rhsValue == 0 && rhs.val.isConst)
        {
            semanticErrMsgs.push_back("Mod by zero at line " + std::to_string(lhs.line_num));
            return NodeAttribute(OpCode::PlaceHolder, ConstValue(), lhs.line_num);
        }

        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = static_cast<long long>(TO_INT(lhs.val.value)) % rhsValue;
            if (IN_INT(longValue))
                result.val.value = static_cast<int>(longValue);
            else
            {
                result.val.value = longValue;
                result.val.type  = llType;
            }
        }

        return result;
    }

    NodeAttribute ModLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        long long rhsValue = TO_LL(rhs.val.value);
        if (rhsValue == 0 && rhs.val.isConst)
        {
            semanticErrMsgs.push_back("Mod by zero at line " + std::to_string(lhs.line_num));
            return NodeAttribute(OpCode::PlaceHolder, ConstValue(), lhs.line_num);
        }

        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
        {
            long long longValue = TO_LL(lhs.val.value) % rhsValue;
            if (IN_INT(longValue))
            {
                result.val.value = static_cast<int>(longValue);
                result.val.type  = intType;
            }
            else
                result.val.value = longValue;
        }

        return result;
    }

    NodeAttribute ModFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        semanticErrMsgs.push_back("Mod operation on float at line " + std::to_string(lhs.line_num));
        return NodeAttribute(OpCode::PlaceHolder, ConstValue(), lhs.line_num);
    }

    NodeAttribute GtInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_INT(lhs.val.value) > TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute GtLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_LL(lhs.val.value) > TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute GtFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<bool>(std::get<float>(lhs.val.value) > std::get<float>(rhs.val.value));
        return result;
    }

    NodeAttribute GeInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_INT(lhs.val.value) >= TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute GeLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_LL(lhs.val.value) >= TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute GeFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<bool>(std::get<float>(lhs.val.value) >= std::get<float>(rhs.val.value));
        return result;
    }

    NodeAttribute LtInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_INT(lhs.val.value) < TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute LtLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_LL(lhs.val.value) < TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute LtFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<bool>(std::get<float>(lhs.val.value) < std::get<float>(rhs.val.value));
        return result;
    }

    NodeAttribute LeInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_INT(lhs.val.value) <= TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute LeLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_LL(lhs.val.value) <= TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute LeFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<bool>(std::get<float>(lhs.val.value) <= std::get<float>(rhs.val.value));
        return result;
    }

    NodeAttribute EqInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_INT(lhs.val.value) == TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute EqLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_LL(lhs.val.value) == TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute EqFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<bool>(std::get<float>(lhs.val.value) == std::get<float>(rhs.val.value));
        return result;
    }

    NodeAttribute NeqInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_INT(lhs.val.value) != TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute NeqLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_LL(lhs.val.value) != TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute NeqFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<bool>(std::get<float>(lhs.val.value) != std::get<float>(rhs.val.value));
        return result;
    }

    NodeAttribute BitOrInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<int>(TO_INT(lhs.val.value) | TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute BitOrLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<long long>(TO_LL(lhs.val.value) | TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute BitOrFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        semanticErrMsgs.push_back("Bitwise operation on float at line " + std::to_string(lhs.line_num));
        return NodeAttribute(OpCode::PlaceHolder, ConstValue(), lhs.line_num);
    }

    NodeAttribute BitAndInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<int>(TO_INT(lhs.val.value) & TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute BitAndLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) static_cast<long long>(TO_LL(lhs.val.value) & TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute BitAndFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        semanticErrMsgs.push_back("Bitwise operation on float at line " + std::to_string(lhs.line_num));
        return NodeAttribute(OpCode::PlaceHolder, ConstValue(), lhs.line_num);
    }

    NodeAttribute AndInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_INT(lhs.val.value) && TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute AndLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_LL(lhs.val.value) && TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute AndFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<bool>(std::get<float>(lhs.val.value) && std::get<float>(rhs.val.value));
        return result;
    }

    NodeAttribute OrInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_INT(lhs.val.value) || TO_INT(rhs.val.value));
        return result;
    }

    NodeAttribute OrLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = static_cast<bool>(TO_LL(lhs.val.value) || TO_LL(rhs.val.value));
        return result;
    }

    NodeAttribute OrFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = boolType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst)
            result.val.value = static_cast<bool>(std::get<float>(lhs.val.value) || std::get<float>(rhs.val.value));
        return result;
    }

    NodeAttribute AssignInt(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        // cout << "Enter AssignInt" << endl;
        NodeAttribute result;
        result.val.type    = intType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = TO_INT(rhs.val.value);
        // cout << "Exit AssignInt" << endl;
        return result;
    }

    NodeAttribute AssignLL(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = llType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = TO_LL(rhs.val.value);
        return result;
    }

    NodeAttribute AssignFloat(const NodeAttribute& lhs, const NodeAttribute& rhs)
    {
        NodeAttribute result;
        result.val.type    = floatType;
        result.val.isConst = lhs.val.isConst && rhs.val.isConst;
        result.line_num    = lhs.line_num;
        if (result.val.isConst) result.val.value = std::get<float>(rhs.val.value);
        return result;
    }
}  // namespace

namespace
{
    unordered_map<size_t, UnaryFunc> UnaryInt = {
        {SIZE_T(OpCode::Add), UnaryAddInt}, {SIZE_T(OpCode::Sub), UnarySubInt}, {SIZE_T(OpCode::Not), NotInt}};
    unordered_map<size_t, UnaryFunc> UnaryLL = {
        {SIZE_T(OpCode::Add), UnaryAddLL}, {SIZE_T(OpCode::Sub), UnarySubLL}, {SIZE_T(OpCode::Not), NotLL}};
    unordered_map<size_t, UnaryFunc> UnaryFloat = {
        {SIZE_T(OpCode::Add), UnaryAddFloat}, {SIZE_T(OpCode::Sub), UnarySubFloat}, {SIZE_T(OpCode::Not), NotFloat}};

    unordered_map<size_t, BinaryFunc> BinaryInt = {{SIZE_T(OpCode::Add), BinaryAddInt},
        {SIZE_T(OpCode::Sub), BinarySubInt},
        {SIZE_T(OpCode::Mul), MulInt},
        {SIZE_T(OpCode::Div), DivInt},
        {SIZE_T(OpCode::Mod), ModInt},
        {SIZE_T(OpCode::Gt), GtInt},
        {SIZE_T(OpCode::Ge), GeInt},
        {SIZE_T(OpCode::Lt), LtInt},
        {SIZE_T(OpCode::Le), LeInt},
        {SIZE_T(OpCode::Eq), EqInt},
        {SIZE_T(OpCode::Neq), NeqInt},
        {SIZE_T(OpCode::BitOr), BitOrInt},
        {SIZE_T(OpCode::BitAnd), BitAndInt},
        {SIZE_T(OpCode::And), AndInt},
        {SIZE_T(OpCode::Or), OrInt},
        {SIZE_T(OpCode::Assign), AssignInt}};

    unordered_map<size_t, BinaryFunc> BinaryLL = {{SIZE_T(OpCode::Add), BinaryAddLL},
        {SIZE_T(OpCode::Sub), BinarySubLL},
        {SIZE_T(OpCode::Mul), MulLL},
        {SIZE_T(OpCode::Div), DivLL},
        {SIZE_T(OpCode::Mod), ModLL},
        {SIZE_T(OpCode::Gt), GtLL},
        {SIZE_T(OpCode::Ge), GeLL},
        {SIZE_T(OpCode::Lt), LtLL},
        {SIZE_T(OpCode::Le), LeLL},
        {SIZE_T(OpCode::Eq), EqLL},
        {SIZE_T(OpCode::Neq), NeqLL},
        {SIZE_T(OpCode::BitOr), BitOrLL},
        {SIZE_T(OpCode::BitAnd), BitAndLL},
        {SIZE_T(OpCode::And), AndLL},
        {SIZE_T(OpCode::Or), OrLL},
        {SIZE_T(OpCode::Assign), AssignLL}};

    unordered_map<size_t, BinaryFunc> BinaryFloat = {{SIZE_T(OpCode::Add), BinaryAddFloat},
        {SIZE_T(OpCode::Sub), BinarySubFloat},
        {SIZE_T(OpCode::Mul), MulFloat},
        {SIZE_T(OpCode::Div), DivFloat},
        {SIZE_T(OpCode::Mod), ModFloat},
        {SIZE_T(OpCode::Gt), GtFloat},
        {SIZE_T(OpCode::Ge), GeFloat},
        {SIZE_T(OpCode::Lt), LtFloat},
        {SIZE_T(OpCode::Le), LeFloat},
        {SIZE_T(OpCode::Eq), EqFloat},
        {SIZE_T(OpCode::Neq), NeqFloat},
        {SIZE_T(OpCode::BitOr), BitOrFloat},
        {SIZE_T(OpCode::BitAnd), BitAndFloat},
        {SIZE_T(OpCode::And), AndFloat},
        {SIZE_T(OpCode::Or), OrFloat},
        {SIZE_T(OpCode::Assign), AssignFloat}};
}  // namespace

NodeAttribute SemanticInt(NodeAttribute a, OpCode op)
{
    // return UnaryAddInt(a);
    return UnaryInt[SIZE_T(op)](a);
}

NodeAttribute SemanticLL(NodeAttribute a, OpCode op)
{
    // return UnaryAddLL(a);
    return UnaryLL[SIZE_T(op)](a);
}

NodeAttribute SemanticFloat(NodeAttribute a, OpCode op)
{
    // return UnaryAddFloat(a);
    return UnaryFloat[SIZE_T(op)](a);
}

NodeAttribute SemanticBool(NodeAttribute a, OpCode op)
{
    NodeAttribute tmp_a = a;
    tmp_a.val.type      = intType;
    tmp_a.val.value     = TO_INT(a.val.value);
    // return UnaryAddInt(tmp_a);
    return UnaryInt[SIZE_T(op)](tmp_a);
}

NodeAttribute SemanticErr(NodeAttribute a, OpCode op)
{
    semanticErrMsgs.push_back("Invalid unary operator " + getOpStr(op) + " at line " + to_string(a.line_num));
    return NodeAttribute();
}

NodeAttribute Semantic(NodeAttribute a, OpCode op)
{
    switch (a.val.type->getKind())
    {
        case TypeKind::Int: return SemanticInt(a, op);
        case TypeKind::LL: return SemanticLL(a, op);
        case TypeKind::Float: return SemanticFloat(a, op);
        case TypeKind::Bool: return SemanticBool(a, op);
        default: return SemanticErr(a, op);
    }
    return NodeAttribute();
}

// bool & other
NodeAttribute SemanticBool_Bool(NodeAttribute a, NodeAttribute b, OpCode op)
{
    NodeAttribute tmp_a = a;
    tmp_a.val.type      = intType;
    tmp_a.val.value     = TO_INT(a.val.value);
    NodeAttribute tmp_b = b;
    tmp_b.val.type      = intType;
    tmp_b.val.value     = TO_INT(b.val.value);

    // return BinaryAddInt(tmp_a, tmp_b);
    return BinaryInt[SIZE_T(op)](tmp_a, tmp_b);
}

NodeAttribute SemanticBool_Int(NodeAttribute a, NodeAttribute b, OpCode op)
{
    NodeAttribute tmp_a = a;
    tmp_a.val.type      = intType;
    tmp_a.val.value     = TO_INT(a.val.value);

    // return BinaryAddInt(tmp_a, b);
    return BinaryInt[SIZE_T(op)](tmp_a, b);
}

NodeAttribute SemanticBool_LL(NodeAttribute a, NodeAttribute b, OpCode op)
{
    NodeAttribute tmp_a = a;
    tmp_a.val.type      = llType;
    tmp_a.val.value     = TO_LL(a.val.value);

    // return BinaryAddLL(tmp_a, b);
    return BinaryLL[SIZE_T(op)](tmp_a, b);
}

NodeAttribute SemanticBool_Float(NodeAttribute a, NodeAttribute b, OpCode op)
{
    NodeAttribute tmp_a = a;
    tmp_a.val.type      = floatType;
    tmp_a.val.value     = TO_FLOAT(a.val.value);

    // return BinaryAddFloat(tmp_a, b);
    return BinaryFloat[SIZE_T(op)](tmp_a, b);
}

// int & other
NodeAttribute SemanticInt_Int(NodeAttribute a, NodeAttribute b, OpCode op)
{
    // return BinaryAddInt(a, b);
    return BinaryInt[SIZE_T(op)](a, b);
}

NodeAttribute SemanticInt_LL(NodeAttribute a, NodeAttribute b, OpCode op)
{
    NodeAttribute tmp_a = a;
    tmp_a.val.type      = llType;
    tmp_a.val.value     = TO_LL(a.val.value);

    // return BinaryAddLL(tmp_a, b);
    return BinaryLL[SIZE_T(op)](tmp_a, b);
}

NodeAttribute SemanticInt_Float(NodeAttribute a, NodeAttribute b, OpCode op)
{
    NodeAttribute tmp_a = a;
    tmp_a.val.type      = floatType;
    tmp_a.val.value     = TO_FLOAT(a.val.value);

    // return BinaryAddFloat(tmp_a, b);
    return BinaryFloat[SIZE_T(op)](tmp_a, b);
}

// long long & other
NodeAttribute SemanticLL_LL(NodeAttribute a, NodeAttribute b, OpCode op)
{
    // return BinaryAddLL(a, b);
    return BinaryLL[SIZE_T(op)](a, b);
}

NodeAttribute SemanticLL_Float(NodeAttribute a, NodeAttribute b, OpCode op)
{
    NodeAttribute tmp_a = a;
    tmp_a.val.type      = floatType;
    tmp_a.val.value     = TO_FLOAT(a.val.value);

    // return BinaryAddFloat(tmp_a, b);
    return BinaryFloat[SIZE_T(op)](tmp_a, b);
}

// float
NodeAttribute SemanticFloat_Float(NodeAttribute a, NodeAttribute b, OpCode op)
{
    // return BinaryAddFloat(a, b);
    return BinaryFloat[SIZE_T(op)](a, b);
}

NodeAttribute SemanticErr(NodeAttribute a, NodeAttribute b, OpCode op)
{
    semanticErrMsgs.push_back("Invalid binary operator " + getOpStr(op) + " at line " + to_string(a.line_num));
    return NodeAttribute();
}

NodeAttribute Semantic(NodeAttribute a, NodeAttribute b, OpCode op)
{
    switch (a.val.type->getKind())
    {
        case TypeKind::Bool:
            switch (b.val.type->getKind())
            {
                case TypeKind::Bool: return SemanticBool_Bool(a, b, op);
                case TypeKind::Int: return SemanticBool_Int(a, b, op);
                case TypeKind::LL: return SemanticBool_LL(a, b, op);
                case TypeKind::Float: return SemanticBool_Float(a, b, op);
                default: return SemanticErr(a, b, op);
            }
            break;
        case TypeKind::Int:
            switch (b.val.type->getKind())
            {
                case TypeKind::Bool: return SemanticBool_Int(b, a, op);
                case TypeKind::Int: return SemanticInt_Int(a, b, op);
                case TypeKind::LL: return SemanticInt_LL(a, b, op);
                case TypeKind::Float: return SemanticInt_Float(a, b, op);
                default: return SemanticErr(a, b, op);
            }
            break;
        case TypeKind::LL:
            switch (b.val.type->getKind())
            {
                case TypeKind::Bool: return SemanticBool_LL(b, a, op);
                case TypeKind::Int: return SemanticInt_LL(b, a, op);
                case TypeKind::LL: return SemanticLL_LL(a, b, op);
                case TypeKind::Float: return SemanticLL_Float(a, b, op);
                default: return SemanticErr(a, b, op);
            }
            break;
        case TypeKind::Float:
            switch (b.val.type->getKind())
            {
                case TypeKind::Bool: return SemanticBool_Float(b, a, op);
                case TypeKind::Int: return SemanticInt_Float(b, a, op);
                case TypeKind::LL: return SemanticLL_Float(b, a, op);
                case TypeKind::Float: return SemanticFloat_Float(a, b, op);
                default: return SemanticErr(a, b, op);
            }
            break;
        default: return SemanticErr(a, b, op);
    }
    return NodeAttribute();
}