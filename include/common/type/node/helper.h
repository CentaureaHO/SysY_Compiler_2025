#ifndef __COMMON_TYPE_NODE_HELPER_H__
#define __COMMON_TYPE_NODE_HELPER_H__

#include <common/type/node/basic_node.h>
#include <common/type/node/statement.h>
#include <common/type/node/expression.h>

class HelperNode : public Node
{
  public:
    HelperNode(int line_num = -1);
    virtual ~HelperNode();

    void printAST(std::ostream* oss, int pad);
};

class InitNode : public HelperNode
{
  public:
    InitNode(int line_num = -1);
    virtual ~InitNode();

    void printAST(std::ostream* oss, int pad);
};

class InitSingle : public InitNode
{
  private:
    ExprNode* expr;

  public:
    InitSingle(ExprNode* expr);
    ~InitSingle();

    void printAST(std::ostream* oss, int pad);
};

class InitMulti : public InitNode
{
  private:
    std::vector<InitNode*>* exprs;

  public:
    InitMulti(std::vector<InitNode*>* es);
    ~InitMulti();

    void printAST(std::ostream* oss, int pad);

    int getSize();
};

class DefNode : public HelperNode
{
  private:
    ExprNode* lval;
    InitNode* rval;

  public:
    DefNode(ExprNode* lval, InitNode* rval);
    ~DefNode();

    void printAST(std::ostream* oss, int pad);
};

class FuncParamDefNode : public HelperNode
{
  private:
    Type*          baseType;
    Symbol::Entry* entry;
    std::vector<ExprNode*>* dims;

  public:
    FuncParamDefNode(Type* type, Symbol::Entry* entry, std::vector<ExprNode*>* dims = nullptr);
    ~FuncParamDefNode();

    void printAST(std::ostream* oss, int pad);
};

#endif