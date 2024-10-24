#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
#include <common/type/node/helper.h>
using namespace std;

/* Definition of StmtNode: head */

StmtNode::StmtNode(int line_num) : Node(line_num) {}
StmtNode::~StmtNode() {}

void StmtNode::printAST(ostream* oss, int pad) { *oss << string(pad, ' ') << "StmtNode\n"; }

/* Definition of StmtNode: tail */
/* Definition of ExprStmt: head */

ExprStmt::ExprStmt(ExprNode* expr) : expr(expr) {}
ExprStmt::~ExprStmt() { delete expr; }

void ExprStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "ExprStmt\n";
    expr->printAST(oss, pad + 4);
}

/* Definition of ExprStmt: tail */
/* Definition of VarDeclareStmt: head */

VarDeclStmt::VarDeclStmt(Type* bt, vector<DefNode*>* defs, bool isConst) : baseType(bt), defs(defs), isConst(isConst) {}
VarDeclStmt::~VarDeclStmt()
{
    if (!defs) return;
    for (auto def : *defs) delete def;
    delete defs;
}

void VarDeclStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "VarDeclareStmt ";
    if (isConst) *oss << "const ";
    *oss << '\n';
    if (!defs) return;
    for (auto def : *defs) def->printAST(oss, pad + 4);
}

/* Definition of VarDeclareStmt: tail */
/* Definition of BlockStmt: head */

BlockStmt::BlockStmt(vector<StmtNode*>* stmts) : stmts(stmts) {}
BlockStmt::~BlockStmt()
{
    if (!stmts) return;
    for (auto stmt : *stmts) delete stmt;
    delete stmts;
}

void BlockStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "BlockStmt\n";
    if (!stmts) return;
    for (auto stmt : *stmts) stmt->printAST(oss, pad + 4);
}

/* Definition of BlockStmt: tail */