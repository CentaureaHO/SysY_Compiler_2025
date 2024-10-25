#include <common/type/node/expression.h>
#include <iostream>
#include <memory>
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

/* Definition of ExprNode: head */

ExprNode::ExprNode(int line_num, bool isConst) : Node(line_num), isConst(isConst) {}
ExprNode::~ExprNode() {}

void ExprNode::printAST(ostream* oss, int pad) {
    *oss << string(pad, ' ') << MAGENTA << "ExprNode" << RESET << '\n';
}

void ExprNode::setConst() { isConst = true; }
void ExprNode::setNonConst() { isConst = false; }

/* Definition of ExprNode: tail */
/* Definition of LeftValueExpr: head */

LeftValueExpr::LeftValueExpr(Symbol::Entry* entry, vector<ExprNode*>* dims, int scope)
    : entry(entry), dims(dims), scope(scope) {}
LeftValueExpr::~LeftValueExpr()
{
    if (dims)
    {
        for (auto dim : *dims) { delete dim; }
        delete dims;
    }
}

void LeftValueExpr::printAST(std::ostream* oss, int pad)
{
    *oss << string(pad, ' ') << CYAN << "LeftValueExpr " << RESET;
    if (entry) *oss << entry->getName();
    if (dims)
    {
        size_t i = 0;
        for (auto dim : *dims)
        {
            *oss << "[Dim " << i << "]";
            ++i;
        }
        *oss << '\n';
        i = 0;
        for (auto dim : *dims) 
        { 
            *oss << string(pad + 2, ' ') << "Dim " << i << ": ";
            dim->printAST(oss, pad + 4); 
            ++i;
        }
    }
    else *oss << '\n';
}

/* Definition of LeftValueExpr: tail */
/* Definition of ConstExpr: head */

ConstExpr::ConstExpr() : value(0), type(0) {}
ConstExpr::ConstExpr(int val) : value(val), type(1) {}
ConstExpr::ConstExpr(long long val) : value(val), type(2) {}
ConstExpr::ConstExpr(float val) : value(val), type(3) {}
ConstExpr::ConstExpr(shared_ptr<string> val) : value(val), type(4) {}
ConstExpr::~ConstExpr() {}

void ConstExpr::printAST(std::ostream* oss, int pad)
{
    *oss << string(pad, ' ') << YELLOW << "Const " << RESET;
    switch (type)
    {
        case 0: *oss << "undefined"; break;
        case 1: *oss << "int: " << get<int>(value); break;
        case 2: *oss << "long long: " << get<long long>(value); break;
        case 3: *oss << "float: " << get<float>(value); break;
        case 4: *oss << "string: " << *get<shared_ptr<string>>(value); break;
        default: *oss << "unknown"; break;
    }
    *oss << '\n';
}

/* Definition of ConstExpr: tail */
/* Definition of UnaryExpr: head */

UnaryExpr::UnaryExpr(OpCode op, ExprNode* expr) : op(op), val(expr) {}
UnaryExpr::~UnaryExpr()
{
    if (val) delete val;
}

void UnaryExpr::printAST(std::ostream* oss, int pad)
{
    *oss << string(pad, ' ') << GREEN << "UnaryExpr " << getOpStr(op) << RESET << '\n';
    val->printAST(oss, pad + 4);
}

/* Definition of UnaryExpr: tail */
/* Definition of BinaryExpr: head */

BinaryExpr::BinaryExpr(OpCode op, ExprNode* lhs, ExprNode* rhs) : op(op), lhs(lhs), rhs(rhs) {}
BinaryExpr::~BinaryExpr()
{
    if (lhs) delete lhs;
    if (rhs) delete rhs;
}

void BinaryExpr::printAST(std::ostream* oss, int pad)
{
    *oss << string(pad, ' ') << BLUE << "BinaryExpr " << getOpStr(op) << RESET << '\n';
    lhs->printAST(oss, pad + 4);
    rhs->printAST(oss, pad + 4);
}

/* Definition of BinaryExpr: tail */
