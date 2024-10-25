#include <common/type/node/expression.h>
#include <str/format_str.h>
#include <iostream>
#include <memory>
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

/* Definition of ExprNode */
ExprNode::ExprNode(int line_num, bool isConst) : Node(line_num), isConst(isConst) {}
ExprNode::~ExprNode() {}

void ExprNode::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << prefix << (isLast ? "└── " : "├── ") << MAGENTA << "ExprNode" << RESET << '\n';
}

void ExprNode::setConst() { isConst = true; }
void ExprNode::setNonConst() { isConst = false; }

/* Definition of LeftValueExpr */
LeftValueExpr::LeftValueExpr(Symbol::Entry* entry, vector<ExprNode*>* dims, int scope)
    : entry(entry), dims(dims), scope(scope)
{}
LeftValueExpr::~LeftValueExpr()
{
    if (dims)
    {
        for (auto dim : *dims) { delete dim; }
        delete dims;
    }
}

void LeftValueExpr::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << CYAN << "LeftValueExpr " << RESET << entry->getName();
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    size_t pos       = newPrefix.rfind('|');
    if (pos + 4 < newPrefix.size()) newPrefix.erase(pos + 4);
    if (dims)
    {
        for (size_t i = 0; i < dims->size(); ++i) *oss << YELLOW << "[Dim" << i << "]" << RESET;
        *oss << '\n';
        for (size_t i = 0; i < dims->size(); ++i)
            (*dims)[i]->printAST(oss, newPrefix + "|   Dim" + to_string(i) + " = ", i == dims->size() - 1);
    }
    else
        *oss << '\n';
}

/* Definition of ConstExpr */
ConstExpr::ConstExpr() : value(0), type(0) {}
ConstExpr::ConstExpr(int val) : value(val), type(1) {}
ConstExpr::ConstExpr(long long val) : value(val), type(2) {}
ConstExpr::ConstExpr(float val) : value(val), type(3) {}
ConstExpr::ConstExpr(shared_ptr<string> val) : value(val), type(4) {}
ConstExpr::~ConstExpr() {}

void ConstExpr::printAST(std::ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << YELLOW << "Const " << RESET;
    switch (type)
    {
        case 1: *oss << YELLOW << "Int: " << RESET << get<int>(value); break;
        case 2: *oss << YELLOW << "LL: " << RESET << get<long long>(value); break;
        case 3: *oss << YELLOW << "Float: " << RESET << get<float>(value); break;
        case 4: *oss << YELLOW << "Str: " << RESET << *get<shared_ptr<string>>(value); break;
        default: *oss << YELLOW << "Undefined" << RESET; break;
    }
    *oss << '\n';
}

/* Definition of UnaryExpr */
UnaryExpr::UnaryExpr(OpCode op, ExprNode* expr) : op(op), val(expr) {}
UnaryExpr::~UnaryExpr() { delete val; }

void UnaryExpr::printAST(std::ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << GREEN << "UnaryExpr " << RESET << getOpStr(op) << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    if (val) val->printAST(oss, newPrefix + "|   ", true);
}

/* Definition of BinaryExpr */
BinaryExpr::BinaryExpr(OpCode op, ExprNode* lhs, ExprNode* rhs) : op(op), lhs(lhs), rhs(rhs) {}
BinaryExpr::~BinaryExpr()
{
    delete lhs;
    delete rhs;
}

void BinaryExpr::printAST(std::ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << BLUE << "BinaryExpr " << RESET << getOpStr(op) << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    if (lhs) lhs->printAST(oss, newPrefix + "|   ", false);
    if (rhs) rhs->printAST(oss, newPrefix + "|   ", true);
}

/* Definition of FuncCallExpr */
FuncCallExpr::FuncCallExpr(Symbol::Entry* entry, vector<ExprNode*>* args) : entry(entry), args(args) {}
FuncCallExpr::~FuncCallExpr()
{
    if (args)
    {
        for (auto arg : *args) { delete arg; }
        delete args;
    }
}

void FuncCallExpr::printAST(std::ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << YELLOW << "FuncCallExpr " << RESET << entry->getName() << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    if (args)
    {
        for (size_t i = 0; i < args->size(); ++i)
        {
            (*args)[i]->printAST(oss, newPrefix + "|   " + MAGENTA + "Arg" + to_string(i) + " = " + RESET, i == args->size() - 1);
        }
    }
    else
        *oss << newPrefix << "`-- " << RED << "No args" << RESET << '\n';
}
