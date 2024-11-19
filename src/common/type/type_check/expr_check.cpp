#include <vector>
#include <ast/expression.h>
#include <ast/statement.h>
#include <ast/helper.h>
#include <common/type/type_calc.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace SemanticTable;

extern vector<string> semanticErrMsgs;
extern Table*         semTable;
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
            if (dim->attr.val.type == voidType || dim->attr.val.type == floatType)
                semanticErrMsgs.emplace_back("Invalid array dimension at line " + to_string(dim->attr.line_num));
            else if (dim->attr.val.type == intType || dim->attr.val.type == llType || dim->attr.val.type == boolType)
            {
                arr_dims.emplace_back(TO_INT(dim->attr.val.value));
                const_dims &= dim->attr.val.isConst;
            }
        }

    VarAttribute* val = semTable->symTable.getSymbol(entry);
    if (!val)
    {
        auto it = semTable->glbSymMap.find(entry);
        if (it == semTable->glbSymMap.end())
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
        attr.val.type      = val->type;
        PointerType* pType = dynamic_cast<PointerType*>(val->type);
        if (pType != nullptr)
        {
            // for (size_t i = 1; i < arr_dims.size(); ++i) pType = dynamic_cast<PointerType*>(pType->baseType);
            attr.val.type = pType->baseType;
        }

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
            attr.val.value = TO_INT(val->initVals[idx]);
        else if (attr.val.type == llType)
            attr.val.value = TO_LL(val->initVals[idx]);
        else if (attr.val.type == floatType)
            attr.val.value = TO_FLOAT(val->initVals[idx]);
    }
    else if (arr_dims.size() < val->dims.size())
    {
        size_t level = val->dims.size() - arr_dims.size();
        Type*  type  = val->type;
        if (type->getKind() != TypeKind::Ptr) type = TypeSystem::getPointerType(type);
        // for (size_t i = 0; i < level; ++i) type = TypeSystem::getPointerType(type);
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
            attr.val.value = TO_INT(value);
            // printf("ConstExpr: int %d\n", TO_INT(attr.val.value));
            break;
        case 2:
            attr.val.type  = llType;
            attr.val.value = TO_LL(value);
            // printf("ConstExpr: ll %lld\n", TO_LL(attr.val.value));
            break;
        case 3:
            attr.val.type  = floatType;
            attr.val.value = TO_FLOAT(value);
            break;
        case 4:
            attr.val.type  = doubleType;
            attr.val.value = get<double>(value);
            break;
        case 5:
            attr.val.type  = boolType;
            attr.val.value = TO_BOOL(value);
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

    if (op == OpCode::Assign)
    {
        LeftValueExpr* lval = dynamic_cast<LeftValueExpr*>(lhs);
        if (lval)
        {
            VarAttribute* val = semTable->symTable.getSymbol(lval->entry);
            if (!val)
            {
                auto it = semTable->glbSymMap.find(lval->entry);
                if (it == semTable->glbSymMap.end())
                {
                    // 如果不存在也在上面的typeCheck中报过了，不需要重复报
                    return;
                }

                val = &it->second;
            }

            if (val->isConst)
            {
                semanticErrMsgs.emplace_back("Assign to const variable at line " + to_string(attr.line_num));
                return;
            }
        }
    }

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

    auto it = semTable->funcDeclMap.find(entry);
    if (it == semTable->funcDeclMap.end())
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

    // 考虑到隐式转换，目前仅仅实现数字类型，都可以互相转，不检查参数类型是否匹配
    // 错了，有个指针类型

    for (size_t i = 0; i < arg_size; ++i)
    {
        if ((*f_params)[i]->attr.val.type != (*args)[i]->attr.val.type)
        {
            if ((*f_params)[i]->attr.val.type->getKind() == TypeKind::Ptr ||
                (*args)[i]->attr.val.type->getKind() == TypeKind::Ptr)
            {
                PointerType* p1 = dynamic_cast<PointerType*>((*f_params)[i]->attr.val.type);
                PointerType* p2 = dynamic_cast<PointerType*>((*args)[i]->attr.val.type);
                if (p1 == nullptr || p2 == nullptr || p1->baseType != p2->baseType)
                {
                    semanticErrMsgs.emplace_back("Function " + entry->getName() + " expects " +
                                                 (*f_params)[i]->attr.val.type->getTypeName() + " but got " +
                                                 (*args)[i]->attr.val.type->getTypeName() + " at line " +
                                                 to_string(attr.line_num));
                    return;
                }
            }
            else
                continue;
        }
    }

    attr.val.type    = funDecl->returnType;
    attr.val.isConst = false;
}