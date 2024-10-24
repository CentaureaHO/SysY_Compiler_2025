#ifndef __COMMON_TYPE_NODE_BASICNODE_H__
#define __COMMON_TYPE_NODE_BASICNODE_H__

#include <iostream>
#include <common/type/type_defs.h>

class StmtNode;
class ExprNode;

class Node
{
  protected:
    int           line_num;
    NodeAttribute attr;

  public:
    Node(int line_num = -1);
    virtual ~Node();

    virtual void printAST(std::ostream* oss, int pad) = 0;
};

class ASTree : public Node
{
  private:
    std::vector<StmtNode*>* stmts;

  public:
    ASTree(std::vector<StmtNode*>* stmts = nullptr, int line_num = -1);
    virtual ~ASTree();

    void printAST(std::ostream* oss, int pad = 0) override;
};

#endif