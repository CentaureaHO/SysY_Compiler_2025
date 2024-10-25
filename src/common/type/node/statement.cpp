#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
#include <common/type/node/helper.h>
#include <iostream>
#include <string>
using namespace std;

/* ANSI color codes */
const std::string RESET   = "\033[0m";
const std::string BLACK   = "\033[30m";      /* Black */
const std::string RED     = "\033[31m";      /* Red */
const std::string GREEN   = "\033[32m";      /* Green */
const std::string YELLOW  = "\033[33m";      /* Yellow */
const std::string BLUE    = "\033[34m";      /* Blue */
const std::string MAGENTA = "\033[35m";      /* Magenta */
const std::string CYAN    = "\033[36m";      /* Cyan */
const std::string WHITE   = "\033[37m";      /* White */

/* Definition of StmtNode: head */

StmtNode::StmtNode(int line_num) : Node(line_num) {}
StmtNode::~StmtNode() {}

void StmtNode::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << MAGENTA << "StmtNode" << RESET << '\n';
}

/* Definition of StmtNode: tail */
/* Definition of ExprStmt: head */

ExprStmt::ExprStmt(ExprNode* expr) : expr(expr) {}
ExprStmt::~ExprStmt() { delete expr; }

void ExprStmt::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << GREEN << "ExprStmt" << RESET << '\n';
    expr->printAST(oss, pad + 4);
}

/* Definition of ExprStmt: tail */
/* Definition of VarDeclStmt: head */

VarDeclStmt::VarDeclStmt(Type* bt, vector<DefNode*>* defs, bool isConst)
    : baseType(bt), defs(defs), isConst(isConst) {}
VarDeclStmt::~VarDeclStmt() {
    if (!defs) return;
    for (auto def : *defs) delete def;
    delete defs;
}

void VarDeclStmt::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << MAGENTA << "VarDeclareStmt " << (isConst ? "const " : "")
         << baseType->getTypeName() << RESET << '\n';
    if (!defs) return;
    for (auto def : *defs) def->printAST(oss, pad + 4);
}

/* Definition of VarDeclStmt: tail */
/* Definition of BlockStmt: head */

BlockStmt::BlockStmt(vector<StmtNode*>* stmts) : stmts(stmts) {}
BlockStmt::~BlockStmt() {
    if (!stmts) return;
    for (auto stmt : *stmts) delete stmt;
    delete stmts;
}

void BlockStmt::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << MAGENTA << "BlockStmt" << RESET << '\n';
    if (!stmts) return;
    for (auto stmt : *stmts) stmt->printAST(oss, pad + 4);
}

/* Definition of BlockStmt: tail */
/* Definition of FuncDeclStmt: head */

FuncDeclStmt::FuncDeclStmt(
    Symbol::Entry* entry, Type* returnType, std::vector<FuncParamDefNode*>* params, StmtNode* body)
    : entry(entry), returnType(returnType), params(params), body(body) {}
FuncDeclStmt::~FuncDeclStmt() {
    if (params) {
        for (auto param : *params) delete param;
        delete params;
    }
    delete body;
}

void FuncDeclStmt::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << YELLOW << "FuncDeclStmt " << entry->getName()
         << " -> " << returnType->getTypeName() << RESET << '\n';
    if (params) {
        for (auto param : *params) param->printAST(oss, pad + 4);
    }
    if (body) body->printAST(oss, pad + 4);
}

/* Definition of FuncDeclStmt: tail */
/* Definition of ReturnStmt: head */

ReturnStmt::ReturnStmt(ExprNode* expr) : expr(expr) {}
ReturnStmt::~ReturnStmt() { delete expr; }

void ReturnStmt::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << RED << "ReturnStmt" << RESET << '\n';
    if (expr) expr->printAST(oss, pad + 4);
}

/* Definition of ReturnStmt: tail */
/* Definition of WhileStmt: head */

WhileStmt::WhileStmt(ExprNode* cond, StmtNode* body) : condition(cond), body(body) {}
WhileStmt::~WhileStmt() {
    delete condition;
    delete body;
}

void WhileStmt::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << CYAN << "WhileStmt" << RESET << '\n';
    condition->printAST(oss, pad + 4);
    body->printAST(oss, pad + 4);
}

/* Definition of WhileStmt: tail */
/* Definition of IfStmt: head */

IfStmt::IfStmt(ExprNode* cond, StmtNode* thenBody, StmtNode* elseBody)
    : condition(cond), thenBody(thenBody), elseBody(elseBody) {}
IfStmt::~IfStmt() {
    delete condition;
    delete thenBody;
    delete elseBody;
}

void IfStmt::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << GREEN << "IfStmt" << RESET << '\n';
    condition->printAST(oss, pad + 4);
    thenBody->printAST(oss, pad + 4);
    if (elseBody) elseBody->printAST(oss, pad + 4);
}

/* Definition of IfStmt: tail */
/* Definition of ForStmt: head */

ForStmt::ForStmt(StmtNode* init, ExprNode* cond, StmtNode* update, StmtNode* body)
    : init(init), condition(cond), update(update), body(body) {}
ForStmt::~ForStmt() {
    delete init;
    delete condition;
    delete update;
    delete body;
}

void ForStmt::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << BLUE << "ForStmt" << RESET << '\n';
    if (init) init->printAST(oss, pad + 4);
    if (condition) condition->printAST(oss, pad + 4);
    if (update) update->printAST(oss, pad + 4);
    body->printAST(oss, pad + 4);
}

/* Definition of ForStmt: tail */
