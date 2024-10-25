#include <common/type/node/basic_node.h>
#include <common/type/node/statement.h>
#include <common/type/node/expression.h>
#include <iostream>
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

/* Definition of Node: head */
Node::Node(int line_num) : line_num(line_num) {}
Node::~Node() {}

/* Definition of ASTree: head */
ASTree::ASTree(vector<StmtNode*>* stmts, int line_num) : Node(line_num), stmts(stmts) {}
ASTree::~ASTree()
{
    if (!stmts) { return; }
    for (auto stmt : *stmts) { delete stmt; }
    delete stmts;
}

void ASTree::printAST(ostream* oss, int pad)
{
    *oss << string(pad, ' ') << RED << "ASTree" << RESET << '\n';
    if (!stmts) { return; }
    for (auto stmt : *stmts) { stmt->printAST(oss, pad + 4); }
}
/* Definition of ASTree: tail */
