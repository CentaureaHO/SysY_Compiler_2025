#include <ast/statement.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace Semantic;

extern bool            mainExists;
extern vector<string>  semanticErrMsgs;
extern Semantic::Table semTable;

void StmtNode::typeCheck() {}

void ExprStmt::typeCheck() {}

void VarDeclStmt::typeCheck() {}

void BlockStmt::typeCheck() {}

void FuncDeclStmt::typeCheck() {}

void ReturnStmt::typeCheck() {}

void WhileStmt::typeCheck() {}

void IfStmt::typeCheck() {}

void ForStmt::typeCheck() {}

void BreakStmt::typeCheck() {}

void ContinueStmt::typeCheck() {}