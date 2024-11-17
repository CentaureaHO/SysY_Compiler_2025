#ifndef __AST_STATEMENT_H__
#define __AST_STATEMENT_H__

#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <ast/basic_node.h>

class DefNode;
class FuncParamDefNode;

class StmtNode : public Node
{
  public:
    StmtNode(int line_num = -1);
    virtual ~StmtNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
    void genIRCode() override;
};

class ExprStmt : public StmtNode
{
  private:
    std::vector<ExprNode*>* exprs;

  public:
    ExprStmt(std::vector<ExprNode*>* exprs = nullptr);
    ~ExprStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
};

class VarDeclStmt : public StmtNode
{
  private:
    const Type*            baseType;
    std::vector<DefNode*>* defs;
    bool                   isConst;

  public:
    VarDeclStmt(const Type* bt = voidType, std::vector<DefNode*>* defs = nullptr, bool isConst = false);
    ~VarDeclStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
};

class BlockStmt : public StmtNode
{
  private:
    std::vector<StmtNode*>* stmts;

  public:
    BlockStmt(std::vector<StmtNode*>* stmts = nullptr);
    ~BlockStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
};

class FuncDeclStmt : public StmtNode
{
  private:
    const Symbol::Entry*            entry;
    const Type*                     returnType;
    std::vector<FuncParamDefNode*>* params;
    StmtNode*                       body;

  public:
    FuncDeclStmt(const Symbol::Entry* entry = nullptr, const Type* returnType = voidType,
        std::vector<FuncParamDefNode*>* params = nullptr, StmtNode* body = nullptr);
    ~FuncDeclStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
};

class ReturnStmt : public StmtNode
{
  private:
    ExprNode* expr;

  public:
    ReturnStmt(ExprNode* expr = nullptr);
    ~ReturnStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
};

class WhileStmt : public StmtNode
{
  private:
    ExprNode* condition;
    StmtNode* body;

  public:
    WhileStmt(ExprNode* condition = nullptr, StmtNode* body = nullptr);
    ~WhileStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
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

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
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

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
};

class BreakStmt : public StmtNode
{
  public:
    BreakStmt();
    ~BreakStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
};

class ContinueStmt : public StmtNode
{
  public:
    ContinueStmt();
    ~ContinueStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true);
    void typeCheck() override;
};

#endif