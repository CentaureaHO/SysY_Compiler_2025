#include <vector>
#include <ast/expression.h>
#include <ast/statement.h>
#include <common/type/type_calc.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace SemanticTable;

extern vector<string> semanticErrMsgs;
extern Table          semTable;
extern bool           inGlb;

void ExprNode::typeCheck() {}

void LeftValueExpr::typeCheck()
{
    isLval                 = false;
    bool        const_dims = true;
    vector<int> arr_dims;

    if (dims)
        for (auto dim : *dims)
        {
            dim->typeCheck();
            if (dim->attr.val.type == voidType || dim->attr.val.type == floatType || dim->attr.val.type == boolType)
                semanticErrMsgs.emplace_back("Invalid array dimension at line " + to_string(dim->attr.line_num));
            else if (dim->attr.val.type == intType)
            {
                arr_dims.emplace_back(get<int>(dim->attr.val.value));
                const_dims &= dim->attr.val.isConst;
            }
        }

    VarAttribute* val = semTable.symTable.getSymbol(entry);
    if (!val)
    {
        auto it = semTable.glbSymMap.find(entry);
        if (it == semTable.glbSymMap.end())
        {
            semanticErrMsgs.emplace_back(
                "Variable " + entry->getName() + " not declared at line " + to_string(attr.line_num));
            return;
        }

        val = &it->second;
    }

    attr.val.type = val->type;
    if (attr.val.type == intType)
        attr.val.value = 0;
    else if (attr.val.type == floatType)
        attr.val.value = static_cast<float>(0);

    if (val->type == voidType) { cout << "Error: Void type variable at line " << attr.line_num << endl; }

    if (arr_dims.size() == val->dims.size())
    {
        attr.val.type    = val->type;
        attr.val.isConst = val->isConst && const_dims;
        if (!attr.val.isConst) return;

        attr.val.value = 0;
        int idx        = 0;
        for (size_t i = 0; i < arr_dims.size(); ++i)
        {
            idx *= val->dims[i];
            idx += arr_dims[i];
        }
        if (attr.val.type == intType)
            attr.val.value = get<int>(val->initVals[idx]);
        else if (attr.val.type == llType)
            attr.val.value = get<long long>(val->initVals[idx]);
        else if (attr.val.type == floatType)
            attr.val.value = get<float>(val->initVals[idx]);
    }
    else if (arr_dims.size() < val->dims.size())
    {
        size_t level = val->dims.size() - arr_dims.size();
        Type*  type  = val->type;
        for (size_t i = 0; i < level; i++) type = TypeSystem::getPointerType(type);
        attr.val.type    = type;
        attr.val.isConst = false;
    }
    else
        semanticErrMsgs.emplace_back(
            "Array " + entry->getName() + " dimension mismatch at line " + to_string(attr.line_num));
}

void ConstExpr::typeCheck()
{
    attr.val.isConst = true;
    switch (type)
    {
        case 1:
            attr.val.type  = intType;
            attr.val.value = get<int>(value);
            // printf("ConstExpr: int %d\n", TO_INT(attr.val.value));
            break;
        case 2:
            attr.val.type  = llType;
            attr.val.value = get<long long>(value);
            // printf("ConstExpr: ll %lld\n", TO_LL(attr.val.value));
            break;
        case 3:
            attr.val.type  = floatType;
            attr.val.value = get<float>(value);
            break;
        case 4:
            attr.val.type  = doubleType;
            attr.val.value = get<double>(value);
            break;
        case 5:
            attr.val.type  = boolType;
            attr.val.value = get<bool>(value);
            break;
        case 6:
            attr.val.type  = strType;
            attr.val.value = get<shared_ptr<string>>(value);
            break;
        default: break;
    }

    return;
}

void UnaryExpr::typeCheck()
{
    val->typeCheck();
    /*
    if (val->attr.val.type == intType) cout << "UnaryExpr: int " << TO_INT(val->attr.val.value);
    if (val->attr.val.type == llType) cout << "UnaryExpr: ll " << TO_LL(val->attr.val.value);
    if (val->attr.val.type == floatType) cout << "UnaryExpr: float " << TO_FLOAT(val->attr.val.value);
    if (val->attr.val.type == boolType) cout << "UnaryExpr: bool " << TO_BOOL(val->attr.val.value);
    */

    attr = Semantic(val->attr, op);

    /*
    cout << "\t" << getOpStr(op) << "\t";
    if (attr.val.type == intType) cout << "UnaryExpr: int " << TO_INT(attr.val.value) << endl;
    if (attr.val.type == llType) cout << "UnaryExpr: ll " << TO_LL(attr.val.value) << endl;
    if (attr.val.type == floatType) cout << "UnaryExpr: float " << TO_FLOAT(attr.val.value) << endl;
    if (attr.val.type == boolType) cout << "UnaryExpr: bool " << TO_BOOL(attr.val.value) << endl;
    */
}

void BinaryExpr::typeCheck()
{
    lhs->typeCheck();
    rhs->typeCheck();

    attr = Semantic(lhs->attr, rhs->attr, op);
}

void FuncCallExpr::typeCheck()
{
    size_t arg_size = 0;
    if (args)
    {
        arg_size = args->size();
        for (auto& arg : *args)
        {
            arg->typeCheck();
            if (arg->attr.val.type == voidType)
                semanticErrMsgs.emplace_back("Void type argument detected at line " + to_string(arg->attr.line_num));
        }
    }

    auto it = semTable.funcDeclMap.find(entry);
    if (it == semTable.funcDeclMap.end())
    {
        semanticErrMsgs.emplace_back(
            "Function " + entry->getName() + " not declared at line " + to_string(attr.line_num));
        return;
    }

    FuncDeclStmt* funDecl    = it->second;
    auto          f_params   = funDecl->params;
    size_t        param_size = 0;
    if (f_params) param_size = f_params->size();

    if (arg_size != param_size)
    {
        semanticErrMsgs.emplace_back("Function " + entry->getName() + " expects " + to_string(param_size) +
                                     " arguments, but got " + to_string(arg_size) + " at line " +
                                     to_string(attr.line_num));
        return;
    }

    attr.val.type = funDecl->returnType;
}