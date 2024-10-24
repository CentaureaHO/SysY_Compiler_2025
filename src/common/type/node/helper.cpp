#include <common/type/node/basic_node.h>
#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
#include <common/type/node/helper.h>
using namespace std;

/* Definition of HelperNode: head */

HelperNode::HelperNode(int line_num) : Node(line_num) {}
HelperNode::~HelperNode() {}

void HelperNode::printAST(ostream* oss, int pad) { *oss << string(pad, ' ') << "HelperNode\n"; }

/* Definition of HelperNode: tail */
/* Definition of InitNode: head */

InitNode::InitNode(int line_num) : HelperNode(line_num) {}
InitNode::~InitNode() {}

void InitNode::printAST(ostream* oss, int pad) { *oss << string(pad, ' ') << "InitNode\n"; }

/* Definition of InitNode: tail */
/* Definition of InitSingle: head */

InitSingle::InitSingle(ExprNode* expr) : InitNode(), expr(expr) {}
InitSingle::~InitSingle() { delete expr; }

void InitSingle::printAST(ostream* oss, int pad)
{
    if (!expr) return;
    *oss << string(pad, ' ') << "Init\n";
    expr->printAST(oss, pad + 4);
}

/* Definition of InitSingle: tail */
/* Definition of InitMulti: head */

InitMulti::InitMulti(vector<InitNode*>* es) : InitNode(), exprs(es) {}
InitMulti::~InitMulti()
{
    if (!exprs) return;
    for (auto expr : *exprs) delete expr;
    delete exprs;
}

void InitMulti::printAST(ostream* oss, int pad)
{
    if (!exprs) return;
    *oss << string(pad, ' ') << "Init\n";
    for (auto expr : *exprs) expr->printAST(oss, pad + 4);
}

int InitMulti::getSize()
{
    if (!exprs) return 0;
    return static_cast<int>(exprs->size());
}

/* Definition of InitMulti: tail */
/* Definition of DefNode: head */

DefNode::DefNode(ExprNode* lval, InitNode* rval) : HelperNode(), lval(lval), rval(rval) {}
DefNode::~DefNode()
{
    delete lval;
    delete rval;
}

void DefNode::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "Def\n";
    lval->printAST(oss, pad + 4);
    if (rval) rval->printAST(oss, pad + 4);
}

/* Definition of DefNode: tail */
/* Definition of FuncParamDefNode: head */

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

void FuncParamDefNode::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << "FuncParamDef ";
    if (baseType) *oss << baseType->getTypeName() << ' ';
    if (entry) *oss << entry->getName() << '\n';
    if (dims)
    {
        size_t i = 0;
        i = 0;
        for (auto dim : *dims)
        {
            *oss << string(pad + 2, ' ') << "Dim " << i << ": ";
            dim->printAST(oss, pad + 4);
            ++i;
        }
    }
}

/* Definition of FuncParamDefNode: tail */