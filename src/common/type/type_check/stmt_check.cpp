#include <ast/statement.h>
#include <ast/expression.h>
#include <ast/helper.h>
#include <common/type/type_defs.h>
#include <common/type/symtab/semantic_table.h>
#include <common/array/indexing.h>
using namespace std;
using namespace SemanticTable;

extern bool           mainExists;
extern bool           inGlb;
extern vector<string> semanticErrMsgs;
extern Table*         semTable;

Type* curFuncRetType = voidType;

namespace
{
    size_t loop_counts     = 0;
    bool   funcWithReturn  = false;
    bool   funcDeclareOnly = false;
}  // namespace

void StmtNode::typeCheck() {}

void ExprStmt::typeCheck()
{
    for (auto& expr : *exprs) expr->typeCheck();
}

bool VarDeclStmt::isRedefinedGlobal(LeftValueExpr* lval)
{
    if (semTable->glbSymMap.find(lval->entry) != semTable->glbSymMap.end())
    {
        semanticErrMsgs.push_back("Error: Redefinition of global variable " + lval->entry->getName() + " at line " +
                                  to_string(attr.line_num));
        return true;
    }
    return false;
}
bool VarDeclStmt::isRedefinedLocal(LeftValueExpr* lval)
{
    auto& curTable = semTable->symTable.currentScope->symbolMap;

    if (curTable.find(lval->entry) != curTable.end())
    {
        semanticErrMsgs.push_back(
            "Error: Redefinition of variable " + lval->entry->getName() + " at line " + to_string(attr.line_num));
        return true;
    }

    if (semTable->symTable.getSymbolScope(lval->entry) == 1)
    {
        /*
        semanticErrMsgs.push_back(
            "Error: Redefinition with parameter " + lval->entry->getName() + " at line " + to_string(attr.line_num));
        return true;
        */
    }

    return false;
}
bool VarDeclStmt::checkArrayDimensions(LeftValueExpr* lval, VarAttribute& val)
{
    vector<ExprNode*>& dims = *lval->dims;

    for (auto& dim : dims)
    {
        dim->typeCheck();
        if (dim->attr.val.type != intType)
        {
            semanticErrMsgs.push_back("Error: Array dimension is not an integer at line " + to_string(attr.line_num));
            return false;
        }
        if (!dim->attr.val.isConst)
        {
            semanticErrMsgs.push_back("Error: Array dimension is not a constant at line " + to_string(attr.line_num));
            return false;
        }
        val.dims.push_back(TO_INT(dim->attr.val.value));
    }

    return true;
}
void VarDeclStmt::arrayInit(InitMulti* in, VarAttribute& val, int begPos, int endPos, int dimsIdx, LeftValueExpr* lval)
{
    if (!in) return;
    int pos = begPos;

    if (!in->exprs) return;
    for (auto& expr : *in->exprs)
    {
        if (pos > endPos)
        {
            semanticErrMsgs.push_back("Error: Too many initializers for variable '" + lval->entry->getName() +
                                      "' at line " + std::to_string(expr->attr.line_num));
            return;
        }

        InitMulti* im = dynamic_cast<InitMulti*>(expr);

        if (im == nullptr)  // InitSingle
        {
            NodeAttribute& attr = expr->attr;

            if (attr.val.type == voidType)
            {
                semanticErrMsgs.push_back("Error: Initialization with void type expression for variable '" +
                                          lval->entry->getName() + "' at line " + std::to_string(attr.line_num));
                return;
            }

            if (val.type == intType)
                val.initVals[pos] = TO_INT(attr.val.value);
            else if (val.type == floatType)
                val.initVals[pos] = TO_FLOAT(attr.val.value);
            else
            {
                semanticErrMsgs.push_back("Error: Unsupported variable type for variable '" + lval->entry->getName() +
                                          "' at line " + std::to_string(attr.line_num));
                return;
            }

            ++pos;
            continue;
        }

        // InitMulti
        int max_subBlock_sz = 0;
        int min_dim_step    = FindMinStepForPosition(val.dims, pos - begPos, dimsIdx, max_subBlock_sz);

        int sub_begPos = pos;
        int sub_endPos = pos + max_subBlock_sz - 1;

        if (sub_endPos > endPos)
        {
            semanticErrMsgs.push_back("Error: Too many initializers for variable '" + lval->entry->getName() +
                                      "' at line " + std::to_string(expr->attr.line_num));
            return;
        }

        im->attr.line_num = in->attr.line_num;
        arrayInit(im, val, sub_begPos, sub_endPos, dimsIdx + min_dim_step, lval);

        pos = sub_endPos + 1;
    }
}
void VarDeclStmt::fillInitials(InitNode* initVals, VarAttribute& var, LeftValueExpr* lval)
{
    size_t arr_size = 1;
    for (auto& dim : var.dims) arr_size *= dim;
    var.initVals.resize(arr_size);

    if (var.type == intType)
        std::fill(var.initVals.begin(), var.initVals.end(), 0);
    else if (var.type == floatType)
        std::fill(var.initVals.begin(), var.initVals.end(), 0.0f);
    else
    {
        semanticErrMsgs.push_back("Error: Unsupported variable type for variable '" + lval->entry->getName() +
                                  "' at line " + std::to_string(initVals->attr.line_num));
        return;
    }

    if (var.dims.empty())
    {
        InitSingle* is = dynamic_cast<InitSingle*>(initVals);
        if (!is)
        {
            semanticErrMsgs.push_back("Error: Invalid initialization for variable '" + lval->entry->getName() +
                                      "' at line " + std::to_string(initVals->attr.line_num));
            return;
        }

        if (is->attr.val.type == voidType)
        {
            semanticErrMsgs.push_back("Error: Initialization with void type expression for variable '" +
                                      lval->entry->getName() + "' at line " + std::to_string(initVals->attr.line_num));
            return;
        }
        else if (is->attr.val.type == intType || is->attr.val.type == llType || is->attr.val.type == floatType ||
                 is->attr.val.type == boolType)
        {
            if (var.type == intType || var.type == boolType || var.type == llType)
                var.initVals[0] = TO_INT(is->attr.val.value);
            else if (var.type == floatType)
                var.initVals[0] = TO_FLOAT(is->attr.val.value);
            else
            {
                semanticErrMsgs.push_back("Error: Unsupported variable type for variable '" + lval->entry->getName() +
                                          "' at line " + std::to_string(initVals->attr.line_num));
                return;
            }
        }
        else
        {
            semanticErrMsgs.push_back("Error: Invalid initialization for variable '" + lval->entry->getName() +
                                      "' at line " + std::to_string(initVals->attr.line_num));
        }
        return;
    }

    InitMulti* im = dynamic_cast<InitMulti*>(initVals);
    if (!im)
    {
        semanticErrMsgs.push_back("Error: Invalid initialization for variable '" + lval->entry->getName() +
                                  "' at line " + std::to_string(initVals->attr.line_num));
        return;
    }

    arrayInit(im, var, 0, arr_size - 1, 0, lval);
}
void VarDeclStmt::typeCheck()
{
    for (auto& def : *defs)
    {
        LeftValueExpr* lval = static_cast<LeftValueExpr*>(def->lval);
        InitNode*      rval = def->rval;

        if (inGlb && isRedefinedGlobal(lval))
            continue;
        else if (!inGlb && isRedefinedLocal(lval))
            continue;

        VarAttribute val;
        val.isConst = isConst;
        val.type    = baseType;
        val.scope   = semTable->symTable.currentScope->scopeLevel;

        if (lval->dims && !checkArrayDimensions(lval, val)) continue;

        if (rval)
        {
            rval->typeCheck();
            fillInitials(rval, val, lval);
        }

        if (inGlb)
            semTable->glbSymMap[lval->entry] = val;
        else
            semTable->symTable.currentScope->symbolMap[lval->entry] = val;
    }
}

void BlockStmt::typeCheck()
{
    if (!stmts) return;

    semTable->symTable.enterScope();

    for (auto stmt : *stmts)
    {
        if (!stmt) continue;

        stmt->typeCheck();
    }

    semTable->symTable.exitScope();
}

void FuncDeclStmt::typeCheck()
{
    if (!inGlb)
        semanticErrMsgs.push_back(
            "Error: Function declaration in non-global scope at line " + to_string(attr.line_num));

    if (entry->getName() == "main") mainExists = true;

    curFuncRetType = returnType;

    semTable->symTable.enterScope();
    inGlb = false;

    semTable->funcDeclMap[entry] = this;

    if (params)
    {
        for (auto param : *params) param->typeCheck();
    }

    funcWithReturn  = false;
    funcDeclareOnly = false;

    if (body)
        body->typeCheck();
    else
        funcDeclareOnly = true;

    if (!funcDeclareOnly && !funcWithReturn && returnType != voidType && entry->getName() != "main")
        semanticErrMsgs.push_back("Error: Function without return statement at line " + to_string(attr.line_num));

    semTable->symTable.exitScope();
    inGlb          = true;
    curFuncRetType = voidType;
}

void ReturnStmt::typeCheck()
{
    funcWithReturn = true;
    if (!expr)
    {
        if (curFuncRetType != voidType)
            semanticErrMsgs.push_back("Error: Return void in non-void function at line " + to_string(attr.line_num));
        return;
    }
    if (inGlb)
    {
        semanticErrMsgs.push_back("Error: Return statement in global scope at line " + to_string(attr.line_num));
        return;
    }

    expr->typeCheck();

    if (expr->attr.val.type == voidType || expr->attr.val.type->getKind() == TypeKind::Ptr)
        semanticErrMsgs.push_back(
            "Error: Return statement with error type expression at line " + to_string(attr.line_num));
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
    if (inGlb) semanticErrMsgs.push_back("Error: For statement in global scope at line " + to_string(attr.line_num));

    semTable->symTable.enterScope();

    if (init) init->typeCheck();
    if (condition) condition->typeCheck();
    if (update) update->typeCheck();

    ++loop_counts;

    if (body) body->typeCheck();

    --loop_counts;
    semTable->symTable.exitScope();
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