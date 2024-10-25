#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
#include <common/type/node/helper.h>
#include <str/format_str.h>
#include <iostream>
#include <string>
using namespace std;

const string RESET   = "\033[0m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";

/* Definition of StmtNode: head */
StmtNode::StmtNode(int line_num) : Node(line_num) {}
StmtNode::~StmtNode() {}

void StmtNode::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << prefix << (isLast ? "└── " : "├── ") << MAGENTA << "StmtNode" << RESET << '\n';
}

/* Definition of ExprStmt: head */
ExprStmt::ExprStmt(std::vector<ExprNode*>* exprs) : exprs(exprs) {}
ExprStmt::~ExprStmt()
{
    if (!exprs) return;
    for (auto expr : *exprs) delete expr;
    delete exprs;
}

void ExprStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << GREEN << "ExprStmt\n" << RESET;
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    for (size_t i = 0; i < exprs->size(); ++i) 
        (*exprs)[i]->printAST(oss, newPrefix + "|   ", i == exprs->size() - 1);
}

/* Definition of VarDeclStmt: head */
VarDeclStmt::VarDeclStmt(Type* bt, vector<DefNode*>* defs, bool isConst) : baseType(bt), defs(defs), isConst(isConst) {}
VarDeclStmt::~VarDeclStmt()
{
    if (!defs) return;
    for (auto def : *defs) delete def;
    delete defs;
}

void VarDeclStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << MAGENTA << "VarDecl BaseType: " << RESET 
         << (isConst ? "const " : "") << YELLOW << baseType->getTypeName() << RESET << '\n';
    if (!defs) return;
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    for (size_t i = 0; i < defs->size(); ++i) 
        (*defs)[i]->printAST(oss, newPrefix + "|   ", i == defs->size() - 1);
}

/* Definition of BlockStmt: head */
BlockStmt::BlockStmt(vector<StmtNode*>* stmts) : stmts(stmts) {}
BlockStmt::~BlockStmt()
{
    if (!stmts) return;
    for (auto stmt : *stmts) delete stmt;
    delete stmts;
}

void BlockStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << MAGENTA << "BlockStmt" << RESET << '\n';
    if (!stmts) return;
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    newPrefix += "|   ";
    for (size_t i = 0; i < stmts->size(); ++i) 
        (*stmts)[i]->printAST(oss, newPrefix, i == stmts->size() - 1);
}

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

void FuncDeclStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << YELLOW << "FuncDecl @Name: " << RESET << BLUE << entry->getName() 
         << RESET << " -> @RetType: " << MAGENTA << returnType->getTypeName() << RESET << '\n';

    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;

    *oss << newPrefix << "|-- " << CYAN << "Params:\n" << RESET;

    if (!params || params->empty())
        *oss << newPrefix << "|       None\n";
    else
    {
        string newnewPrefix = newPrefix + "|   |   ";
        for (size_t i = 0; i < params->size(); ++i) 
            (*params)[i]->printAST(oss, newnewPrefix, i == params->size() - 1);
    }

    *oss << newPrefix << "`-- " << CYAN << "Body:\n" << RESET;
    body->printAST(oss, newPrefix + "    |   ", true);
}

/* Definition of ReturnStmt: head */
ReturnStmt::ReturnStmt(ExprNode* expr) : expr(expr) {}
ReturnStmt::~ReturnStmt() { delete expr; }

void ReturnStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << MAGENTA << "ReturnStmt" << RESET << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    if (expr)
        expr->printAST(oss, newPrefix + "|   ", true);
    else
        *oss << newPrefix << "`-- No return value\n";
}

/* Definition of WhileStmt: head */
WhileStmt::WhileStmt(ExprNode* cond, StmtNode* body) : condition(cond), body(body) {}
WhileStmt::~WhileStmt()
{
    delete condition;
    delete body;
}

void WhileStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << CYAN << "WhileStmt" << RESET << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    *oss << "    |-- " << GREEN << "Cond:\n" << RESET;
    condition->printAST(oss, newPrefix + "|   |   ", true);
    *oss << "    `-- " << BLUE << "Body:\n" << RESET;
    body->printAST(oss, newPrefix + "    |   ", true);
}

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

void IfStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << GREEN << "IfStmt" << RESET << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;

    *oss << newPrefix << "|-- " << GREEN << "Condition:\n" << RESET;
    if (condition) condition->printAST(oss, newPrefix + "|   |   ", true);

    *oss << newPrefix << "|-- " << CYAN << "Thenbody:\n" << RESET;
    if (thenBody) thenBody->printAST(oss, newPrefix + "|   |   ", true);

    *oss << newPrefix << "`-- " << YELLOW << "Elsebody:\n" << RESET;
    if (elseBody)
        elseBody->printAST(oss, newPrefix + "    |   ", true);
    else
        *oss << newPrefix << "    `-- None\n";
}

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

void ForStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << BLUE << "ForStmt" << RESET << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;

    *oss << newPrefix << "|-- " << GREEN << "Init:\n" << RESET;
    if (init)
        init->printAST(oss, newPrefix + "|   |   ", true);
    else
        *oss << newPrefix << "|   |   `-- None\n";

    *oss << newPrefix << "|-- " << CYAN << "Condition:\n" << RESET;
    if (condition)
        condition->printAST(oss, newPrefix + "|   |   ", true);
    else
        *oss << newPrefix << "|   |   `-- None\n";

    *oss << newPrefix << "|-- " << YELLOW << "Update:\n" << RESET;
    if (update)
        update->printAST(oss, newPrefix + "|   |   ", true);
    else
        *oss << newPrefix << "|   |   `-- None\n";

    *oss << newPrefix << "`-- " << MAGENTA << "Body:\n" << RESET;
    if (body)
        body->printAST(oss, newPrefix + "    |   ", true);
    else
        *oss << newPrefix << "    `-- None\n";
}

/* Definition of BreakStmt: head */
BreakStmt::BreakStmt() {}
BreakStmt::~BreakStmt() {}

void BreakStmt::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << BLUE << "BreakStmt" << RESET << '\n';
}
