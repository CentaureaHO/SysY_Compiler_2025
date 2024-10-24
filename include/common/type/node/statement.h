#ifndef __COMMON_TYPE_NODE_STATEMENT_H__
#define __COMMON_TYPE_NODE_STATEMENT_H__

#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <common/type/node/basic_node.h>

/**
 * @brief 抽象语法树中所有语句节点的基类
 *
 * 这是所有具体语句节点（如条件语句、循环语句、赋值语句等）的基类。
 * 它继承自 `Node` 类，表示程序中的语句单元。
 */
class StmtNode : public Node
{
  public:
    StmtNode(int line_num = -1) : Node(line_num) {}
    virtual ~StmtNode() {}

    void printAST(std::ostream* oss, int pad) override { *oss << std::string(pad, ' ') << "StmtNode" << std::endl; }
};

class FuncDeclStmt : public StmtNode
{
  private:
    Symbol::Entry*      funcEntry;
    Type*               retType;
    std::vector<Type*>* paramTypes;

  public:
};

#endif