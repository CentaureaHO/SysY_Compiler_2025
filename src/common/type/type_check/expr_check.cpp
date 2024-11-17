#include <ast/expression.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace Semantic;

extern vector<string>  semanticErrMsgs;
extern Semantic::Table semTable;

void ExprNode::typeCheck() {}

void LeftValueExpr::typeCheck() {}

void ConstExpr::typeCheck() {}

void UnaryExpr::typeCheck() {}

void BinaryExpr::typeCheck() {}

void FuncCallExpr::typeCheck() {}