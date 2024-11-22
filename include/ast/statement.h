#ifndef __AST_STATEMENT_H__
#define __AST_STATEMENT_H__

#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <ast/basic_node.h>
#include <ast/helper.h>
#include <ast/expression.h>

class DefNode;
class FuncParamDefNode;
class InitNode;
class InitMulti;

class StmtNode : public Node
{
  public:
    StmtNode(int line_num = -1);
    virtual ~StmtNode();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
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

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

class VarDeclStmt : public StmtNode
{
  public:
    Type*                  baseType;
    std::vector<DefNode*>* defs;
    bool                   isConst;

  public:
    VarDeclStmt(Type* bt = voidType, std::vector<DefNode*>* defs = nullptr, bool isConst = false);
    ~VarDeclStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;

    bool isRedefinedGlobal(LeftValueExpr* lval);
    bool isRedefinedLocal(LeftValueExpr* lval);
    bool checkArrayDimensions(LeftValueExpr* lval, VarAttribute& val);
    void arrayInit(InitMulti* in, VarAttribute& val, int begPos, int endPos, int dimsIdx, LeftValueExpr* lval);
    void fillInitials(InitNode* initVals, VarAttribute& var, LeftValueExpr* lval);

    void typeCheck() override;
    void genIRCode() override;
};

class BlockStmt : public StmtNode
{
  private:
    std::vector<StmtNode*>* stmts;

  public:
    BlockStmt(std::vector<StmtNode*>* stmts = nullptr);
    ~BlockStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

class FuncDeclStmt : public StmtNode
{
  public:
    const Symbol::Entry*            entry;
    Type*                           returnType;
    std::vector<FuncParamDefNode*>* params;
    StmtNode*                       body;

  public:
    FuncDeclStmt(const Symbol::Entry* entry = nullptr, Type* returnType = voidType,
        std::vector<FuncParamDefNode*>* params = nullptr, StmtNode* body = nullptr);
    ~FuncDeclStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

class ReturnStmt : public StmtNode
{
  private:
    ExprNode* expr;

  public:
    ReturnStmt(ExprNode* expr = nullptr);
    ~ReturnStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

class WhileStmt : public StmtNode
{
  private:
    ExprNode* condition;
    StmtNode* body;

  public:
    WhileStmt(ExprNode* condition = nullptr, StmtNode* body = nullptr);
    ~WhileStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
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

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
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

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

class BreakStmt : public StmtNode
{
  public:
    BreakStmt();
    ~BreakStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

class ContinueStmt : public StmtNode
{
  public:
    ContinueStmt();
    ~ContinueStmt();

    void printAST(std::ostream* oss, const std::string& prefix = "", bool isLast = true) override;
    void typeCheck() override;
    void genIRCode() override;
};

#endif