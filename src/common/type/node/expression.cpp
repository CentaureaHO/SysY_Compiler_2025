#include <common/type/node/expression.h>
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

void ExprNode::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << MAGENTA << "ExprNode" << RESET << '\n';
}

void ExprNode::setConst() { isConst = true; }
void ExprNode::setNonConst() { isConst = false; }

/* Definition of LeftValueExpr */
LeftValueExpr::LeftValueExpr(Symbol::Entry* entry, vector<ExprNode*>* dims, int scope)
    : entry(entry), dims(dims), scope(scope) {}
LeftValueExpr::~LeftValueExpr() {
    if (dims) {
        for (auto dim : *dims) { delete dim; }
        delete dims;
    }
}

void LeftValueExpr::printAST(ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << CYAN << "LeftValueExpr " << RESET;
    if (entry) *oss << entry->getName();
    *oss << '\n';

    if (dims) {
        for (size_t i = 0; i < dims->size(); ++i) {
            (*dims)[i]->printAST(oss, prefix + (isLast ? "    " : "│   "), i == dims->size() - 1);
        }
    }
}

/* Definition of ConstExpr */
ConstExpr::ConstExpr() : value(0), type(0) {}
ConstExpr::ConstExpr(int val) : value(val), type(1) {}
ConstExpr::ConstExpr(long long val) : value(val), type(2) {}
ConstExpr::ConstExpr(float val) : value(val), type(3) {}
ConstExpr::ConstExpr(shared_ptr<string> val) : value(val), type(4) {}
ConstExpr::~ConstExpr() {}

void ConstExpr::printAST(std::ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << YELLOW << "Const " << RESET;
    switch (type) {
        case 1: *oss << "int: " << get<int>(value); break;
        case 2: *oss << "long long: " << get<long long>(value); break;
        case 3: *oss << "float: " << get<float>(value); break;
        case 4: *oss << "string: " << *get<shared_ptr<string>>(value); break;
        default: *oss << "undefined"; break;
    }
    *oss << '\n';
}

/* Definition of UnaryExpr */
UnaryExpr::UnaryExpr(OpCode op, ExprNode* expr) : op(op), val(expr) {}
UnaryExpr::~UnaryExpr() { delete val; }

void UnaryExpr::printAST(std::ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << GREEN << "UnaryExpr " << getOpStr(op) << RESET << '\n';
    if (val) val->printAST(oss, prefix + (isLast ? "    " : "│   "), true);
}

/* Definition of BinaryExpr */
BinaryExpr::BinaryExpr(OpCode op, ExprNode* lhs, ExprNode* rhs) : op(op), lhs(lhs), rhs(rhs) {}
BinaryExpr::~BinaryExpr() {
    delete lhs;
    delete rhs;
}

void BinaryExpr::printAST(std::ostream* oss, const string& prefix, bool isLast) {
    *oss << prefix << (isLast ? "└── " : "├── ") << BLUE << "BinaryExpr " << getOpStr(op) << RESET << '\n';
    if (lhs) lhs->printAST(oss, prefix + (isLast ? "    " : "│   "), false);
    if (rhs) rhs->printAST(oss, prefix + (isLast ? "    " : "│   "), true);
}
