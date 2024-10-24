#ifndef __COMMON_TYPE_NODE_STATEMENT_H__
#define __COMMON_TYPE_NODE_STATEMENT_H__

#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <common/type/node/basic_node.h>

class DefNode;

/**
 * @brief 抽象语法树中所有语句节点的基类
 *
 * 这是所有具体语句节点（如条件语句、循环语句、赋值语句等）的基类。
 * 它继承自 `Node` 类，表示程序中的语句单元。
 */
class StmtNode : public Node
{
  public:
    StmtNode(int line_num = -1);
    virtual ~StmtNode();

    void printAST(std::ostream* oss, int pad) override;
};

class ExprStmt : public StmtNode
{
  private:
    ExprNode* expr;

  public:
    ExprStmt(ExprNode* expr = nullptr);
    ~ExprStmt();

    void printAST(std::ostream* oss, int pad) override;
};

class VarDeclStmt : public StmtNode
{
  private:
    Type*                  baseType;
    std::vector<DefNode*>* defs;
    bool                   isConst;

  public:
    VarDeclStmt(Type* bt = voidType, std::vector<DefNode*>* defs = nullptr, bool isConst = false);
    ~VarDeclStmt();

    void printAST(std::ostream* oss, int pad) override;
};

#endif