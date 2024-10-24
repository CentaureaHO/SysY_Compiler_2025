#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
using namespace std;

/* Definition of StmtNode: head */

StmtNode::StmtNode(int line_num) : Node(line_num) {}
StmtNode::~StmtNode() {}

void StmtNode::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "StmtNode\n";
}

/* Definition of StmtNode: tail */
/* Definition of ExprStmt: head */

ExprStmt::ExprStmt(ExprNode* expr) : expr(expr) {}
ExprStmt::~ExprStmt() 
{
    delete expr;
}

void ExprStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "ExprStmt\n";
    expr->printAST(oss, pad+2);
}

/* Definition of ExprStmt: tail */