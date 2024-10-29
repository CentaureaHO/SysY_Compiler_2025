#include <ast/basic_node.h>
#include <ast/statement.h>
#include <ast/expression.h>
#include <ast/helper.h>
#include <str/format_str.h>
#include <iostream>
#include <string>
using namespace std;

/* Definition of HelperNode */
HelperNode::HelperNode(int line_num) : Node(line_num) {}
HelperNode::~HelperNode() {}

void HelperNode::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << prefix << (isLast ? "└── " : "├── ") << "HelperNode\n";
}

/* Definition of InitNode */
InitNode::InitNode(int line_num) : HelperNode(line_num) {}
InitNode::~InitNode() {}

void InitNode::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << prefix << (isLast ? "└── " : "├── ") << "InitNode\n";
}

/* Definition of InitSingle */
InitSingle::InitSingle(ExprNode* expr) : InitNode(), expr(expr) {}
InitSingle::~InitSingle() { delete expr; }

void InitSingle::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << "InitSingle\n";
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    if (expr) expr->printAST(oss, newPrefix + "|   ", true);
}

/* Definition of InitMulti */
InitMulti::InitMulti(vector<InitNode*>* es) : InitNode(), exprs(es) {}
InitMulti::~InitMulti()
{
    if (!exprs) return;
    for (auto expr : *exprs) delete expr;
    delete exprs;
}

void InitMulti::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << "InitMulti\n";
    if (!exprs) return;
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    for (size_t i = 0; i < exprs->size(); ++i) (*exprs)[i]->printAST(oss, newPrefix + "|   ", i == exprs->size() - 1);
}

int InitMulti::getSize() { return exprs ? static_cast<int>(exprs->size()) : 0; }

/* Definition of DefNode */
DefNode::DefNode(ExprNode* lval, InitNode* rval) : HelperNode(), lval(lval), rval(rval) {}
DefNode::~DefNode()
{
    delete lval;
    delete rval;
}

void DefNode::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << "DefNode\n";
    string newPrefix = isLast ? removeLastPrefix(prefix) : prefix;
    lval->printAST(oss, newPrefix + "|   Var: ", rval != nullptr);
    if (rval)
        rval->printAST(oss, newPrefix + "|   Init: ", true);
    else
        *oss << newPrefix << "`-- Init: no initializer\n";
}

/* Definition of FuncParamDefNode */
FuncParamDefNode::FuncParamDefNode(Type* type, Symbol::Entry* entry, vector<ExprNode*>* dims)
    : HelperNode(), baseType(type), entry(entry), dims(dims)
{}
FuncParamDefNode::~FuncParamDefNode()
{
    if (dims)
    {
        for (auto dim : *dims) delete dim;
        delete dims;
    }
}

void FuncParamDefNode::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << getFirstPrefix(prefix, isLast) << baseType->getTypeName() << ' ' << entry->getName();
    if (dims)
    {
        for (size_t i = 0; i < dims->size(); ++i) *oss << "[Dim" << i << "]";
        *oss << '\n';

        for (size_t i = 0; i < dims->size(); ++i)
        {
            (*dims)[i]->printAST(oss, prefix + "|   Dim" + to_string(i) + " = ", i == dims->size() - 1);
        }
    }
    else
        *oss << '\n';
}