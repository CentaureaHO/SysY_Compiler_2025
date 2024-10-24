#include <common/type/node/basic_node.h>
#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
using namespace std;

Node::Node(int line_num) : line_num(line_num) {}
Node::~Node() {}

ASTree::ASTree(vector<StmtNode*>* stmts, int line_num) : Node(line_num), stmts(stmts) {}
ASTree::~ASTree()
{
    if (!stmts) { return; }
    delete stmts;
}

void ASTree::printAST(ostream* oss, int pad)
{
    *oss << "ASTree" << endl;
    if (!stmts) { return; }
    for (auto stmt : *stmts) { stmt->printAST(oss, pad + 4); }
}