#ifndef __COMMON_TYPE_NODE_EXPRESSION_H__
#define __COMMON_TYPE_NODE_EXPRESSION_H__

#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <common/type/node/basic_node.h>

/**
 * @brief 抽象语法树中所有表达式节点基类
 */
class ExprNode : public Node
{
  protected:
    bool isConst;

  public:
    ExprNode(int line_num = -1, bool isConst = true);
    virtual ~ExprNode();

    void printAST(std::ostream* oss, int pad) override;

    void setConst();
    void setNonConst();
};

class LeftValueExpr : public ExprNode
{
  private:
    Symbol::Entry*          entry;
    std::vector<ExprNode*>* dims;
    int                     scope;

  public:
    LeftValueExpr(Symbol::Entry* entry = nullptr, std::vector<ExprNode*>* dims = nullptr, int scope = -1);
    ~LeftValueExpr();

    void printAST(std::ostream* oss, int pad) override;
};

/**
 * @brief 常量表达式节点
 *
 * 用于表示抽象语法树中的常量表达式，例如整数常量或浮点常量或字符串常量。
 */
class ConstExpr : public ExprNode
{
  private:
    std::variant<int, long long, float, std::shared_ptr<std::string>> value;
    int                                                               type;

  public:
    ConstExpr();
    ConstExpr(int val);
    ConstExpr(long long val);
    ConstExpr(float val);
    ConstExpr(std::shared_ptr<std::string> val);
    ~ConstExpr();

    void printAST(std::ostream* oss, int pad) override;
};

/**
 * @brief 一元运算符表达式节点
 *
 * 用于表示抽象语法树中的一元运算表达式，例如逻辑非(!)或取负(-)等运算。
 * 该类表示的表达式只有一个操作数。
 */
class UnaryExpr : public ExprNode
{
  private:
    OpCode    op;
    ExprNode* val;

  public:
    UnaryExpr(OpCode op = OpCode::PlaceHolder, ExprNode* val = nullptr);
    ~UnaryExpr();

    void printAST(std::ostream* oss, int pad) override;
};

/**
 * @brief 二元运算符表达式节点
 *
 * 用于表示抽象语法树中的二元运算表达式，例如加法(+)、减法(-)等运算。
 * 该类表示的表达式有两个操作数。
 */
class BinaryExpr : public ExprNode
{
  private:
    OpCode    op;
    ExprNode* lhs;
    ExprNode* rhs;

  public:
    BinaryExpr(OpCode op = OpCode::PlaceHolder, ExprNode* lhs = nullptr, ExprNode* rhs = nullptr);
    ~BinaryExpr();

    void printAST(std::ostream* oss, int pad) override;
};

#endif