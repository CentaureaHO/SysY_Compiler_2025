#include <common/type/node/basic_node.h>
#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
#include <iostream>
#include <string>
using namespace std;

/* ANSI color codes */
const std::string RESET   = "\033[0m";
const std::string RED     = "\033[31m";

/* Definition of Node */
Node::Node(int line_num) : line_num(line_num) {}
Node::~Node() {}

/* Definition of ASTree */
ASTree::ASTree(vector<StmtNode*>* stmts, int line_num) : Node(line_num), stmts(stmts) {}
ASTree::~ASTree()
{
    if (!stmts) return;
    for (auto stmt : *stmts) delete stmt;
    delete stmts;
}

void ASTree::printAST(ostream* oss, const string& prefix, bool isLast)
{
    *oss << prefix  << RED << "ASTree" << RESET << '\n';
    if (!stmts) return;

    for (size_t i = 0; i < stmts->size(); ++i)
    {
        (*stmts)[i]->printAST(oss, prefix + (isLast ? "    " : "│   "), i == stmts->size() - 1);
    }
}
