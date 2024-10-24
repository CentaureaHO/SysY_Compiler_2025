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
    *oss << baseType->getTypeName() << '\n';
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
/* Definition of FuncDeclStmt: head */

FuncDeclStmt::FuncDeclStmt(
    Symbol::Entry* entry, Type* returnType, std::vector<FuncParamDefNode*>* params, StmtNode* body)
    : entry(entry), returnType(returnType), params(params), body(body)
{}
FuncDeclStmt::~FuncDeclStmt()
{
    if (params)
    {
        for (auto param : *params) delete param;
        delete params;
    }
    delete body;
}

void FuncDeclStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "FuncDeclStmt ";
    *oss << entry->getName() << " -> " << returnType->getTypeName() << '\n';
    if (params)
    {
        for (auto param : *params) param->printAST(oss, pad + 4);
    }
    if (body) body->printAST(oss, pad + 4);
}

/* Definition of FuncDeclStmt: tail */
/* Definition of ReturnStmt: head */

ReturnStmt::ReturnStmt(ExprNode* expr) : expr(expr) {}
ReturnStmt::~ReturnStmt() { delete expr; }

void ReturnStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "ReturnStmt\n";
    if (expr) expr->printAST(oss, pad + 4);
}

/* Definition of ReturnStmt: head */
/* Definition of WhileStmt: head */

WhileStmt::WhileStmt(ExprNode* cond, StmtNode* body) : condition(cond), body(body) {}
WhileStmt::~WhileStmt()
{
    delete condition;
    delete body;
}

void WhileStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "WhileStmt\n";
    condition->printAST(oss, pad + 4);
    body->printAST(oss, pad + 4);
}

/* Definition of WhileStmt: head */
/* Definition of IfStmt: head */

IfStmt::IfStmt(ExprNode* cond, StmtNode* thenBody, StmtNode* elseBody)
    : condition(cond), thenBody(thenBody), elseBody(elseBody)
{}
IfStmt::~IfStmt()
{
    delete condition;
    delete thenBody;
    delete elseBody;
}

void IfStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "IfStmt\n";
    condition->printAST(oss, pad + 4);
    thenBody->printAST(oss, pad + 4);
    if (elseBody) elseBody->printAST(oss, pad + 4);
}

/* Definition of IfStmt: head */
/* Definition of ForStmt: head */

ForStmt::ForStmt(StmtNode* init, ExprNode* cond, StmtNode* update, StmtNode* body)
    : init(init), condition(cond), update(update), body(body)
{}
ForStmt::~ForStmt()
{
    delete init;
    delete condition;
    delete update;
    delete body;
}

void ForStmt::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "ForStmt\n";
    if (init) init->printAST(oss, pad + 4);
    if (condition) condition->printAST(oss, pad + 4);
    if (update) update->printAST(oss, pad + 4);
    body->printAST(oss, pad + 4);
}

/* Definition of ForStmt: head */