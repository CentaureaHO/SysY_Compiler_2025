#include <ast/helper.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace SemanticTable;
extern bool           inGlb;
extern Table          semTable;
extern vector<string> semanticErrMsgs;

void HelperNode::typeCheck() {}

void InitNode::typeCheck() {}

void InitSingle::typeCheck()
{
    expr->typeCheck();
    attr = expr->attr;
}

void InitMulti::typeCheck()
{
    if (!exprs) return;
    for (auto& expr : *exprs)
    {
        if (!expr) continue;
        expr->typeCheck();
    }
}

void DefNode::typeCheck()
{
    lval->typeCheck();
    rval->typeCheck();
}

void FuncParamDefNode::typeCheck()
{
    VarAttribute param;
    param.type    = baseType;
    param.isConst = false;
    param.scope   = semTable.symTable.currentScope->scopeLevel;
    // cout << "param name: " << entry->getName() << " at scope " << param.scope << endl;

    if (dims)
    {
        param.type = TypeSystem::getPointerType(param.type);
        bool flag  = 0;
        for (auto& dim : *dims)
        {
            dim->typeCheck();

            if (dim->attr.val.type == voidType)
                semanticErrMsgs.emplace_back("Error: Array dimension is void type");
            else if (dim->attr.val.type == floatType)
                semanticErrMsgs.emplace_back("Error: Array dimension is float");
            else if (!dim->attr.val.isConst)
                semanticErrMsgs.emplace_back("Error: Array dimension is not a constant");

            int d = TO_INT(dim->attr.val.value);
            if (d < 0 && flag)
                semanticErrMsgs.emplace_back("Error: Array dimension is negative, may be out of i32 range");

            flag = 1;
            param.dims.push_back(d);
        }
    }

    if (semTable.symTable.getSymbolScope(entry) != -1)
    {
        semanticErrMsgs.emplace_back("Error: Redefinition of parameter " + entry->getName());
        return;
    }

    // cout << "Registering param " << entry->getName() << " at scope " << param.scope << endl;

    semTable.symTable.addSymbol(entry, param);

    attr.val.type = param.type;
}