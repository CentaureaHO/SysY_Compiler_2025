#ifndef __AST_EXPRESSION_H__
#define __AST_EXPRESSION_H__

#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <ast/basic_node.h>

namespace LLVMIR
{
    class Operand;
}

/**
 * @brief 抽象语法树中所有表达式节点基类
 */
class ExprNode : public Node
{
  protected:
    bool isConst;

  public:
    ExprNode(int line_num = -1, bool isConst = false);
    virtual ~ExprNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
    void genIRCode() override;

    void setConst();
    void setNonConst();
};

class LeftValueExpr : public ExprNode
{
  public:
    bool                    isLval;
    Symbol::Entry*          entry;
    std::vector<ExprNode*>* dims;
    int                     scope;
    LLVMIR::Operand*        lv_ptr;

  public:
    LeftValueExpr(
        Symbol::Entry* entry = nullptr, std::vector<ExprNode*>* dims = nullptr, int scope = -1, bool isL = false);
    ~LeftValueExpr();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
    void genIRCode() override;
};

/**
 * @brief 常量表达式节点
 */
class ConstExpr : public ExprNode
{
  public:
    std::variant<int, long long, float, double, bool, std::shared_ptr<std::string>> value;
    int                                                                             type;

  public:
    ConstExpr();
    ConstExpr(int val);
    ConstExpr(long long val);
    ConstExpr(float val);
    ConstExpr(double val);
    ConstExpr(bool val);
    ConstExpr(std::shared_ptr<std::string> val);
    ~ConstExpr();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
    void genIRCode() override;
};

/**
 * @brief 一元运算符表达式节点
 */
class UnaryExpr : public ExprNode
{
  public:
    OpCode    op;
    ExprNode* val;

  public:
    UnaryExpr(OpCode op = OpCode::PlaceHolder, ExprNode* val = nullptr);
    ~UnaryExpr();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
    void genIRCode() override;
};

/**
 * @brief 二元运算符表达式节点
 */
class BinaryExpr : public ExprNode
{
  public:
    OpCode    op;
    ExprNode* lhs;
    ExprNode* rhs;

  public:
    BinaryExpr(OpCode op = OpCode::PlaceHolder, ExprNode* lhs = nullptr, ExprNode* rhs = nullptr);
    ~BinaryExpr();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;

    void genIRCode() override;
    void genIRCode_LogicalAnd();
    void genIRCode_LogicalOr();
    void genIRCode_Assign();
};

class FuncCallExpr : public ExprNode
{
  public:
    Symbol::Entry*          entry;
    std::vector<ExprNode*>* args;

  public:
    FuncCallExpr(Symbol::Entry* entry = nullptr, std::vector<ExprNode*>* args = nullptr);
    ~FuncCallExpr();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
    void genIRCode() override;
};

#endif