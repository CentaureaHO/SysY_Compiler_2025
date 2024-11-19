#include <ast/basic_node.h>
#include <ast/statement.h>
#include <ast/expression.h>
#include <iostream>
#include <string>
using namespace std;

/* Definition of Node */
Node::Node(int line_num) : line_num(line_num) { attr.line_num = line_num; }
Node::~Node() {}

void Node::set_line(int line_num)
{
    this->line_num = line_num;
    attr.line_num  = line_num;
}
int Node::get_line() const { return line_num; }

/* Definition of ASTree */
ASTree::ASTree(vector<StmtNode*>* stmts, int line_num) : Node(line_num), stmts(stmts) {}
ASTree::~ASTree()
{
    if (!stmts) return;
    for (auto stmt : *stmts)
    {
        delete stmt;
        stmt = nullptr;
    }
    delete stmts;
    stmts = nullptr;
}

void ASTree::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << prefix << "ASTree" << '\n';
    if (!stmts) return;

    size_t s = stmts->size() - 1;
    for (size_t i = 0; i <= s; ++i)
        if ((*stmts)[i]) (*stmts)[i]->printAST(oss, prefix + "|   ", i == s);
}