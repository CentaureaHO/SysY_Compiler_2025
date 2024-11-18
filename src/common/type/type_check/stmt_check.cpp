#include <ast/statement.h>
#include <ast/expression.h>
#include <ast/helper.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace SemanticTable;

extern bool           mainExists;
extern bool           inGlb;
extern vector<string> semanticErrMsgs;
extern Table          semTable;

namespace
{
    size_t loop_counts = 0;

    void fillIntInitials(const InitNode* initVals, VarAttribute& var) {}

    void fillLLInitials(const InitNode* initVals, VarAttribute& var) {}

    void fillFloatInitials(const InitNode* initVals, VarAttribute& var) {}

}  // namespace

void StmtNode::typeCheck() {}

void ExprStmt::typeCheck()
{
    for (auto& expr : *exprs) expr->typeCheck();
}

void VarDeclStmt::typeCheck()
{
    if (inGlb)
    {
        for (auto& def : *defs)
        {
            LeftValueExpr* lval = static_cast<LeftValueExpr*>(def->lval);
            InitNode*      rval = def->rval;

            auto it = semTable.glbSymMap.find(lval->entry);
            if (it != semTable.glbSymMap.end())
            {
                semanticErrMsgs.push_back("Error: Redefinition of global variable " + lval->entry->getName() +
                                          " at line " + to_string(attr.line_num));
                continue;
            }

            VarAttribute val;
            val.isConst = isConst;
            val.type    = baseType;

            if (lval->dims)
            {
                vector<ExprNode*>& dims = *lval->dims;
                for (auto& dim : dims)
                {
                    dim->typeCheck();
                    if (dim->attr.val.type != intType)
                        semanticErrMsgs.push_back(
                            "Error: Array dimension is not an integer at line " + to_string(attr.line_num));
                    if (!dim->attr.val.isConst)
                        semanticErrMsgs.push_back(
                            "Error: Array dimension is not a constant at line " + to_string(attr.line_num));
                    val.dims.push_back(get<int>(dim->attr.val.value));
                }
            }

            if (rval)
            {
                rval->typeCheck();
                if (rval->attr.val.type == intType)
                    fillIntInitials(rval, val);
                else if (rval->attr.val.type == llType)
                    fillLLInitials(rval, val);
                else if (rval->attr.val.type == floatType)
                    fillFloatInitials(rval, val);
                else if (rval->attr.val.type == boolType)
                {
                    rval->attr.val.type  = intType;
                    rval->attr.val.value = (get<bool>(rval->attr.val.value) ? 1 : 0);
                    fillIntInitials(rval, val);
                }
                else
                {
                    semanticErrMsgs.push_back(
                        "Error: Initialization value type mismatch at line " + to_string(attr.line_num));
                }
            }
        }
        return;
    }
}

void BlockStmt::typeCheck()
{
    if (!stmts) return;

    semTable.symTable.enterScope();

    for (auto stmt : *stmts)
    {
        if (!stmt) continue;

        stmt->typeCheck();
    }

    semTable.symTable.exitScope();
}

void FuncDeclStmt::typeCheck()
{
    if (!inGlb)
        semanticErrMsgs.push_back(
            "Error: Function declaration in non-global scope at line " + to_string(attr.line_num));

    inGlb = false;
    if (entry->getName() == "main") mainExists = true;

    body->typeCheck();

    inGlb = true;
}

void ReturnStmt::typeCheck()
{
    if (!expr) return;
    if (inGlb)
    {
        semanticErrMsgs.push_back("Error: Return statement in global scope at line " + to_string(attr.line_num));
        return;
    }

    expr->typeCheck();

    if (expr->attr.val.type == voidType)
        semanticErrMsgs.push_back(
            "Error: Return statement with void type expression at line " + to_string(attr.line_num));

    semanticErrMsgs.push_back("Return val" + to_string(get<int>(expr->attr.val.value)));
}

void WhileStmt::typeCheck()
{
    if (inGlb) semanticErrMsgs.push_back("Error: While statement in global scope at line " + to_string(attr.line_num));

    condition->typeCheck();
    if (condition->attr.val.type == voidType)
        semanticErrMsgs.push_back(
            "Error: While statement with void type condition at line " + to_string(attr.line_num));

    ++loop_counts;
    if (body) body->typeCheck();
    --loop_counts;
}

void IfStmt::typeCheck()
{
    if (inGlb) semanticErrMsgs.push_back("Error: If statement in global scope at line " + to_string(attr.line_num));

    condition->typeCheck();
    if (condition->attr.val.type == voidType)
        semanticErrMsgs.push_back("Error: If statement with void type condition at line " + to_string(attr.line_num));

    if (thenBody) thenBody->typeCheck();
    if (elseBody) elseBody->typeCheck();
}

void ForStmt::typeCheck()
{
    // todo

    if (inGlb) semanticErrMsgs.push_back("Error: For statement in global scope at line " + to_string(attr.line_num));
}

void BreakStmt::typeCheck()
{
    if (inGlb) semanticErrMsgs.push_back("Error: Break statement in global scope at line " + to_string(attr.line_num));

    if (loop_counts == 0)
        semanticErrMsgs.push_back("Error: Break statement outside of loop at line " + to_string(attr.line_num));
}

void ContinueStmt::typeCheck()
{
    if (inGlb)
        semanticErrMsgs.push_back("Error: Continue statement in global scope at line " + to_string(attr.line_num));

    if (loop_counts == 0)
        semanticErrMsgs.push_back("Error: Continue statement outside of loop at line " + to_string(attr.line_num));
}