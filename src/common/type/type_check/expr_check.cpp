#include <ast/expression.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace Semantic;

extern vector<string>  semanticErrMsgs;
extern Semantic::Table semTable;

void ExprNode::typeCheck() {}

void LeftValueExpr::typeCheck() {}

void ConstExpr::typeCheck()
{
    ConstValue cv = attr.getVal();
    cv.value      = value;
    visit(
        [&](auto&& val) {
            using T = decay_t<decltype(val)>;
            if constexpr (is_same_v<T, int>)
                cv.type = intType;
            else if constexpr (is_same_v<T, long long>)
                cv.type = llType;
            else if constexpr (is_same_v<T, float>)
                cv.type = floatType;
            else if constexpr (is_same_v<T, double>)
                cv.type = doubleType;
            else if constexpr (is_same_v<T, bool>)
                cv.type = boolType;
            else if constexpr (is_same_v<T, shared_ptr<string>>)
                cv.type = strType;
            else
                semanticErrMsgs.push_back("Undefined type in ConstExpr at line " + to_string(line_num));
        },
        value);
}

void UnaryExpr::typeCheck() { val->typeCheck(); }

void BinaryExpr::typeCheck()
{
    lhs->typeCheck();
    rhs->typeCheck();
}

void FuncCallExpr::typeCheck()
{
    for (auto& arg : *args) { arg->typeCheck(); }
}