#ifndef __AST_BASICNODE_H__
#define __AST_BASICNODE_H__

#include <iostream>
#include <common/type/type_defs.h>
#include <vector>
#include <string>

class StmtNode;
class ExprNode;

class Node
{
  public:
    int           line_num;
    NodeAttribute attr;

  public:
    Node(int line_num = -1);
    virtual ~Node();

    virtual void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) = 0;
    virtual void typeCheck()                                                                     = 0;
    virtual void genIRCode()                                                                     = 0;

    void set_line(int line_num);
    int  get_line() const;
};

class ASTree : public Node
{
  private:
    std::vector<StmtNode*>* stmts;

  public:
    ASTree(std::vector<StmtNode*>* stmts = nullptr, int line_num = -1);
    virtual ~ASTree();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

#endif
