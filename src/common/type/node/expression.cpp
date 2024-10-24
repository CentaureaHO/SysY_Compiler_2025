#include <common/type/node/expression.h>
using namespace std;

/* Definition of ExprNode: head */

ExprNode::ExprNode(int line_num, bool isConst) : Node(line_num), isConst(isConst) {}
ExprNode::~ExprNode() {}

void ExprNode::printAST(ostream* oss, int pad) { *oss << string(pad, ' ') << "ExprNode\n"; }

void ExprNode::setConst() { isConst = true; }
void ExprNode::setNonConst() { isConst = false; }

/* Definition of ExprNode: tail */
/* Definition of LeftValueExpr: head */

LeftValueExpr::LeftValueExpr(Symbol::Entry* entry, vector<ExprNode*>* dims, int scope)
    : entry(entry), dims(dims), scope(scope)
{}
LeftValueExpr::~LeftValueExpr() {}

void LeftValueExpr::printAST(std::ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "LeftValueExpr ";
    if (entry) *oss << entry->getName();
    if (dims)
    {
        *oss << " [";
        for (auto dim : *dims) { dim->printAST(oss, 0); }
        *oss << "]";
    }
    *oss << '\n';
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
    *oss << string(pad, ' ') << "Const ";
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
UnaryExpr::~UnaryExpr() {}

void UnaryExpr::printAST(std::ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "UnaryExpr " << getOpStr(op) << '\n';
    val->printAST(oss, pad + 2);
}

/* Definition of UnaryExpr: tail */
/* Definition of BinaryExpr: head */

BinaryExpr::BinaryExpr(OpCode op, ExprNode* lhs, ExprNode* rhs) : op(op), lhs(lhs), rhs(rhs) {}
BinaryExpr::~BinaryExpr() {}

void BinaryExpr::printAST(std::ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "BinaryExpr " << getOpStr(op) << '\n';
    lhs->printAST(oss, pad + 2);
    rhs->printAST(oss, pad + 2);
}

/* Definition of BinaryExpr: tail */