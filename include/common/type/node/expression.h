#ifndef __COMMON_TYPE_NODE_EXPRESSION_H__
#define __COMMON_TYPE_NODE_EXPRESSION_H__

#include <common/type/node/basic_node.h>

/**
 * @brief 抽象语法树中所有表达式节点基类
 */
class ExprNode : public Node
{};

/**
 * @brief 常量表达式节点
 *
 * 用于表示抽象语法树中的常量表达式，例如整数常量或浮点常量或字符串常量。
 */
class ConstExpr : public ExprNode
{};

/**
 * @brief 标识符表达式节点
 *
 * 用于表示抽象语法树中的标识符（变量、函数等）。
 */
class IdentExpr : public ExprNode
{};

/**
 * @brief 一元运算符表达式节点
 *
 * 用于表示抽象语法树中的一元运算表达式，例如逻辑非(!)或取负(-)等运算。
 * 该类表示的表达式只有一个操作数。
 */
class UnaryExpr : public ExprNode
{};

/**
 * @brief 二元运算符表达式节点
 *
 * 用于表示抽象语法树中的二元运算表达式，例如加法(+)、减法(-)等运算。
 * 该类表示的表达式有两个操作数。
 */
class BinaryExpr : public ExprNode
{};

#endif