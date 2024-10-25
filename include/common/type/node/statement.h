#ifndef __COMMON_TYPE_NODE_STATEMENT_H__
#define __COMMON_TYPE_NODE_STATEMENT_H__

#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <common/type/node/basic_node.h>

class DefNode;
class FuncParamDefNode;

class StmtNode : public Node
{
  public:
    StmtNode(int line_num = -1);
    virtual ~StmtNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
};

class ExprStmt : public StmtNode
{
  private:
    ExprNode* expr;

  public:
    ExprStmt(ExprNode* expr = nullptr);
    ~ExprStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) ;
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

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
};

class BlockStmt : public StmtNode
{
  private:
    std::vector<StmtNode*>* stmts;

  public:
    BlockStmt(std::vector<StmtNode*>* stmts = nullptr);
    ~BlockStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) ;
};

class FuncDeclStmt : public StmtNode
{
  private:
    Symbol::Entry*                  entry;
    Type*                           returnType;
    std::vector<FuncParamDefNode*>* params;
    StmtNode*                       body;

  public:
    FuncDeclStmt(Symbol::Entry* entry = nullptr, Type* returnType = voidType,
        std::vector<FuncParamDefNode*>* params = nullptr, StmtNode* body = nullptr);
    ~FuncDeclStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) ;
};

class ReturnStmt : public StmtNode
{
  private:
    ExprNode* expr;

  public:
    ReturnStmt(ExprNode* expr = nullptr);
    ~ReturnStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) ;
};

class WhileStmt : public StmtNode
{
  private:
    ExprNode* condition;
    StmtNode* body;

  public:
    WhileStmt(ExprNode* condition = nullptr, StmtNode* body = nullptr);
    ~WhileStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) ;
};

class IfStmt : public StmtNode
{
  private:
    ExprNode* condition;
    StmtNode* thenBody;
    StmtNode* elseBody;

  public:
    IfStmt(ExprNode* condition = nullptr, StmtNode* thenBody = nullptr, StmtNode* elseBody = nullptr);
    ~IfStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) ;
};

class ForStmt : public StmtNode
{
  private:
    StmtNode* init;
    ExprNode* condition;
    StmtNode* update;
    StmtNode* body;

  public:
    ForStmt(
        StmtNode* init = nullptr, ExprNode* condition = nullptr, StmtNode* update = nullptr, StmtNode* body = nullptr);
    ~ForStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) ;
};

#endif
