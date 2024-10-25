#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
#include <common/type/node/helper.h>
#include <iostream>
#include <string>
using namespace std;

/* ANSI color codes */
const std::string RESET   = "\033[0m";
const std::string RED     = "\033[31m";
const std::string GREEN   = "\033[32m";
const std::string YELLOW  = "\033[33m";
const std::string BLUE    = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN    = "\033[36m";

/* Definition of StmtNode: head */
StmtNode::StmtNode(int line_num) : Node(line_num) {}
StmtNode::~StmtNode() {}

void StmtNode::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << MAGENTA << "StmtNode" << RESET << '\n';
}

/* Definition of ExprStmt: head */
ExprStmt::ExprStmt(ExprNode* expr) : expr(expr) {}
ExprStmt::~ExprStmt() { delete expr; }

void ExprStmt::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << GREEN << "ExprStmt" << RESET << '\n';
    if (expr) expr->printAST(oss, prefix + (isLast ? "    " : "│   "), true);
}

/* Definition of VarDeclStmt: head */
VarDeclStmt::VarDeclStmt(Type* bt, vector<DefNode*>* defs, bool isConst)
    : baseType(bt), defs(defs), isConst(isConst) {}
VarDeclStmt::~VarDeclStmt() {
    if (!defs) return;
    for (auto def : *defs) delete def;
    delete defs;
}

void VarDeclStmt::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << MAGENTA << "VarDeclareStmt " 
         << (isConst ? "const " : "") << baseType->getTypeName() << RESET << '\n';
    if (!defs) return;
    for (size_t i = 0; i < defs->size(); ++i)
        (*defs)[i]->printAST(oss, prefix + (isLast ? "    " : "│   "), i == defs->size() - 1);
}

/* Definition of BlockStmt: head */
BlockStmt::BlockStmt(vector<StmtNode*>* stmts) : stmts(stmts) {}
BlockStmt::~BlockStmt() {
    if (!stmts) return;
    for (auto stmt : *stmts) delete stmt;
    delete stmts;
}

void BlockStmt::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << MAGENTA << "BlockStmt" << RESET << '\n';
    if (!stmts) return;
    for (size_t i = 0; i < stmts->size(); ++i)
        (*stmts)[i]->printAST(oss, prefix + (isLast ? "    " : "│   "), i == stmts->size() - 1);
}

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

void FuncDeclStmt::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << YELLOW << "FuncDeclStmt " << entry->getName()
         << " -> " << returnType->getTypeName() << RESET << '\n';
    if (params) {
        for (size_t i = 0; i < params->size(); ++i)
            (*params)[i]->printAST(oss, prefix + (isLast ? "    " : "│   "), i == params->size() - 1);
    }
    if (body) body->printAST(oss, prefix + (isLast ? "    " : "│   "), true);
}

/* Definition of ReturnStmt: head */
ReturnStmt::ReturnStmt(ExprNode* expr) : expr(expr) {}
ReturnStmt::~ReturnStmt() { delete expr; }

void ReturnStmt::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << MAGENTA << "ReturnStmt" << RESET << '\n';
    if (expr) expr->printAST(oss, prefix + (isLast ? "    " : "│   "), true);
}

/* Definition of WhileStmt: head */
WhileStmt::WhileStmt(ExprNode* cond, StmtNode* body) : condition(cond), body(body) {}
WhileStmt::~WhileStmt() {
    delete condition;
    delete body;
}

void WhileStmt::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << CYAN << "WhileStmt" << RESET << '\n';
    if (condition) condition->printAST(oss, prefix + (isLast ? "    " : "│   "), false);
    if (body) body->printAST(oss, prefix + (isLast ? "    " : "│   "), true);
}

/* Definition of IfStmt: head */
IfStmt::IfStmt(ExprNode* cond, StmtNode* thenBody, StmtNode* elseBody)
    : condition(cond), thenBody(thenBody), elseBody(elseBody) {}
IfStmt::~IfStmt() {
    delete condition;
    delete thenBody;
    delete elseBody;
}

void IfStmt::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << GREEN << "IfStmt" << RESET << '\n';
    if (condition) condition->printAST(oss, prefix + (isLast ? "    " : "│   "), false);
    if (thenBody) thenBody->printAST(oss, prefix + (isLast ? "    " : "│   "), elseBody == nullptr);
    if (elseBody) elseBody->printAST(oss, prefix + (isLast ? "    " : "│   "), true);
}

/* Definition of ForStmt: head */
ForStmt::ForStmt(StmtNode* init, ExprNode* cond, StmtNode* update, StmtNode* body)
    : init(init), condition(cond), update(update), body(body) {}
ForStmt::~ForStmt() {
    delete init;
    delete condition;
    delete update;
    delete body;
}

void ForStmt::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << BLUE << "ForStmt" << RESET << '\n';
    if (init) init->printAST(oss, prefix + (isLast ? "    " : "│   "), false);
    if (condition) condition->printAST(oss, prefix + (isLast ? "    " : "│   "), false);
    if (update) update->printAST(oss, prefix + (isLast ? "    " : "│   "), false);
    if (body) body->printAST(oss, prefix + (isLast ? "    " : "│   "), true);
}
