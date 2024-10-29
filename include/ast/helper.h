#ifndef __AST_HELPER_H__
#define __AST_HELPER_H__

#include <ast/basic_node.h>
#include <ast/statement.h>
#include <ast/expression.h>

class HelperNode : public Node
{
  public:
    HelperNode(int line_num = -1);
    virtual ~HelperNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
};

class InitNode : public HelperNode  // 初始化类型的节点
{
  public:
    InitNode(int line_num = -1);
    virtual ~InitNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
};

class InitSingle : public InitNode  // 单一表达式的初始化节点
{
  private:
    ExprNode* expr;

  public:
    InitSingle(ExprNode* expr);
    ~InitSingle();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
};

class InitMulti : public InitNode
{
  private:
    std::vector<InitNode*>* exprs;

  public:
    InitMulti(std::vector<InitNode*>* es);
    ~InitMulti();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);

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

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
};

class FuncParamDefNode : public HelperNode
{
  private:
    Type*                   baseType;
    Symbol::Entry*          entry;
    std::vector<ExprNode*>* dims;

  public:
    FuncParamDefNode(Type* type, Symbol::Entry* entry, std::vector<ExprNode*>* dims = nullptr);
    ~FuncParamDefNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
};

#endif
