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

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

class InitNode : public HelperNode  // 初始化类型的节点
{
  public:
    InitNode(int line_num = -1);
    virtual ~InitNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
};

class InitSingle : public InitNode  // 单一表达式的初始化节点
{
  public:
    ExprNode* expr;

  public:
    InitSingle(ExprNode* expr);
    ~InitSingle();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
};

class InitMulti : public InitNode
{
  public:
    std::vector<InitNode*>* exprs;

  public:
    InitMulti(std::vector<InitNode*>* es);
    ~InitMulti();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;

    int getSize();
};

class DefNode : public HelperNode
{
  public:
    ExprNode* lval;
    InitNode* rval;

  public:
    DefNode(ExprNode* lval, InitNode* rval);
    ~DefNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
};

class FuncParamDefNode : public HelperNode
{
  public:
    Type*                   baseType;
    Symbol::Entry*          entry;
    std::vector<ExprNode*>* dims;

  public:
    FuncParamDefNode(Type* type, Symbol::Entry* entry, std::vector<ExprNode*>* dims = nullptr);
    ~FuncParamDefNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
};

#endif