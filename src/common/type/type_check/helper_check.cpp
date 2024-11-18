#include <ast/helper.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace SemanticTable;
extern bool inGlb;

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
    for (auto& expr : *exprs) expr->typeCheck();
}

void DefNode::typeCheck() {}

void FuncParamDefNode::typeCheck() {}