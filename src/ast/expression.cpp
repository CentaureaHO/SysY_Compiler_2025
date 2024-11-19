#include <ast/expression.h>
#include <str/format_str.h>
#include <iostream>
#include <memory>
#include <string>
using namespace std;

/* Definition of ExprNode */
ExprNode::ExprNode(int line_num, bool isConst) : Node(line_num), isConst(isConst) {}
ExprNode::~ExprNode() {}

void ExprNode::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << prefix << (isLast ? "└── " : "├── ") << "ExprNode\n";
}

void ExprNode::setConst() { isConst = true; }
void ExprNode::setNonConst() { isConst = false; }

/* Definition of LeftValueExpr */
LeftValueExpr::LeftValueExpr(Symbol::Entry* entry, vector<ExprNode*>* dims, int scope, bool isL)
    : isLval(isL), entry(entry), dims(dims), scope(scope)
{}
LeftValueExpr::~LeftValueExpr()
{
    if (dims)
    {
        for (auto dim : *dims)
        {
            delete dim;
            dim = nullptr;
        }
        delete dims;
        dims = nullptr;
    }
}

void LeftValueExpr::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << "LeftValueExpr " << entry->getName();
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    if (dims)
    {
        for (size_t i = 0; i < dims->size(); ++i) *oss << "[Dim" << i << "]";
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
ConstExpr::ConstExpr(double val) : value(val), type(4) {}
ConstExpr::ConstExpr(bool val) : value(val), type(5) {}
ConstExpr::ConstExpr(shared_ptr<string> val) : value(val), type(6) {}
ConstExpr::~ConstExpr() {}

void ConstExpr::printAST(std::ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << "Const ";
    switch (type)
    {
        case 1: *oss << "Int: " << get<int>(value); break;
        case 2: *oss << "LL: " << get<long long>(value); break;
        case 3: *oss << "Float: " << get<float>(value); break;
        case 4: *oss << "Double: " << get<double>(value); break;
        case 5: *oss << "Bool: " << get<bool>(value); break;
        case 6: *oss << "String: " << *get<shared_ptr<string>>(value); break;
        default: *oss << "Undefined"; break;
    }
    *oss << '\n';
}

/* Definition of UnaryExpr */
UnaryExpr::UnaryExpr(OpCode op, ExprNode* expr) : op(op), val(expr) {}
UnaryExpr::~UnaryExpr()
{
    delete val;
    val = nullptr;
}

void UnaryExpr::printAST(std::ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << "UnaryExpr " << getOpStr(op) << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    if (val) val->printAST(oss, newPrefix + "|   ", true);
}

/* Definition of BinaryExpr */
BinaryExpr::BinaryExpr(OpCode op, ExprNode* lhs, ExprNode* rhs) : op(op), lhs(lhs), rhs(rhs) {}
BinaryExpr::~BinaryExpr()
{
    delete lhs;
    delete rhs;

    lhs = nullptr;
    rhs = nullptr;
}

void BinaryExpr::printAST(std::ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << "BinaryExpr " << getOpStr(op) << '\n';
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
        for (auto arg : *args)
        {
            delete arg;
            arg = nullptr;
        }
        delete args;
        args = nullptr;
    }
}

void FuncCallExpr::printAST(std::ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << "FuncCallExpr " << entry->getName() << '\n';
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    if (args)
    {
        for (size_t i = 0; i < args->size(); ++i)
        {
            (*args)[i]->printAST(oss, newPrefix + "|   Arg" + to_string(i) + " = ", i == args->size() - 1);
        }
    }
    else
        *oss << newPrefix << "`-- No args\n";
}