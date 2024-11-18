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

#define TO_BOOL(x) std::visit(safe_cast_to_bool, x)
#define TO_INT(x) std::visit(safe_cast_to_int, x)
#define TO_LL(x) std::visit(safe_cast_to_ll, x)
#define TO_FLOAT(x) std::visit(safe_cast_to_float, x)

namespace
{
    auto safe_cast_to_bool = [](auto&& value) -> int {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, bool>)
            return static_cast<bool>(value);
        else if constexpr (std::is_same_v<T, int>)
            return static_cast<bool>(value);
        else if constexpr (std::is_same_v<T, long long>)
            return static_cast<bool>(value);
        else if constexpr (std::is_same_v<T, float>)
            return static_cast<bool>(value);
        else
        {
            std::cerr << "Unexpected type: " << typeid(T).name() << '\n';
            throw std::bad_variant_access();
        }
    };

    auto safe_cast_to_int = [](auto&& value) -> int {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, bool>)
            return static_cast<int>(value);
        else if constexpr (std::is_same_v<T, int>)
            return static_cast<int>(value);
        else if constexpr (std::is_same_v<T, long long>)
            return static_cast<int>(value);
        else if constexpr (std::is_same_v<T, float>)
            return static_cast<int>(value);
        else
        {
            std::cerr << "Unexpected type: " << typeid(T).name() << '\n';
            throw std::bad_variant_access();
        }
    };

    auto safe_cast_to_ll = [](auto&& value) -> int {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, bool>)
            return static_cast<long long>(value);
        else if constexpr (std::is_same_v<T, int>)
            return static_cast<long long>(value);
        else if constexpr (std::is_same_v<T, long long>)
            return static_cast<long long>(value);
        else if constexpr (std::is_same_v<T, float>)
            return static_cast<long long>(value);
        else
        {
            std::cerr << "Unexpected type: " << typeid(T).name() << '\n';
            throw std::bad_variant_access();
        }
    };

    auto safe_cast_to_float = [](auto&& value) -> int {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, bool>)
            return static_cast<float>(value);
        else if constexpr (std::is_same_v<T, int>)
            return static_cast<float>(value);
        else if constexpr (std::is_same_v<T, long long>)
            return static_cast<float>(value);
        else if constexpr (std::is_same_v<T, float>)
            return static_cast<float>(value);
        else
        {
            std::cerr << "Unexpected type: " << typeid(T).name() << '\n';
            throw std::bad_variant_access();
        }
    };
}  // namespace

namespace
{
    size_t loop_counts = 0;

    int getMindimStep(VarAttribute& val, int relativePos, int dimsIdx, int& max_subBlock_size)
    {
        int min_dim_step = 1;
        int blockSz      = 1;
        for (size_t i = dimsIdx + 1; i < val.dims.size(); ++i) { blockSz *= val.dims[i]; }
        while (relativePos % blockSz != 0)
        {
            min_dim_step++;
            blockSz /= val.dims[dimsIdx + min_dim_step - 1];
        }
        max_subBlock_size = blockSz;
        return min_dim_step;
    }

    void arrayInit(InitMulti* in, VarAttribute& val, int begPos, int endPos, int dimsIdx)
    {
        if (!in) return;
        int pos = begPos;

        for (auto& expr : *in->exprs)
        {
            InitMulti* im = dynamic_cast<InitMulti*>(expr);

            if (im == nullptr)  // 为InitSingle
            {
                NodeAttribute& attr = expr->attr;

                if (attr.val.type == voidType)
                {
                    semanticErrMsgs.push_back(
                        "Error: Initialization with void type expression at line " + to_string(attr.line_num));
                    return;
                }

                if (val.type == intType)
                {
                    val.initVals[pos] = TO_INT(attr.val.value);
                    cout << "Set " << pos << " to " << TO_INT(attr.val.value) << endl;
                }
                else if (val.type == llType)
                    val.initVals[pos] = TO_LL(attr.val.value);
                else if (val.type == floatType)
                    val.initVals[pos] = TO_FLOAT(attr.val.value);
                else
                    semanticErrMsgs.push_back("Error: Invalid initialization at line " + to_string(attr.line_num));

                ++pos;
                continue;
            }

            // 为InitMulti
            int max_subBlock_sz = 0;
            int min_dim_step    = getMindimStep(val, pos - begPos, dimsIdx, max_subBlock_sz);
            arrayInit(im, val, pos, pos + max_subBlock_sz - 1, dimsIdx + min_dim_step);
            pos += max_subBlock_sz;
        }
    }

    void fillIntInitials(InitNode* initVals, VarAttribute& var)
    {
        var.type        = intType;
        size_t arr_size = 1;
        for (auto& dim : var.dims) arr_size *= dim;
        var.initVals.resize(arr_size, 0);

        if (var.dims.empty())  // 非数组
        {
            InitSingle* is = dynamic_cast<InitSingle*>(initVals);
            if (!is)
            {
                semanticErrMsgs.push_back(
                    "Error: Invalid initialization at line " + to_string(initVals->attr.line_num));
                return;
            }

            if (is->attr.val.type == voidType)
            {
                semanticErrMsgs.push_back(
                    "Error: Initialization with void type expression at line " + to_string(initVals->attr.line_num));
                return;
            }
            else if (is->attr.val.type == intType)
                var.initVals[0] = get<int>(is->attr.val.value);
            else if (is->attr.val.type == llType)
                var.initVals[0] = static_cast<int>(get<long long>(is->attr.val.value));
            else if (is->attr.val.type == floatType)
                var.initVals[0] = static_cast<int>(get<float>(is->attr.val.value));
            else if (is->attr.val.type == boolType)
                var.initVals[0] = static_cast<int>(get<bool>(is->attr.val.value));
            else
                semanticErrMsgs.push_back(
                    "Error: Invalid initialization at line " + to_string(initVals->attr.line_num));

            return;
        }
        else  // 数组
        {
            InitMulti* im = dynamic_cast<InitMulti*>(initVals);
            if (!im)
            {
                semanticErrMsgs.push_back(
                    "Error: Invalid initialization at line " + to_string(initVals->attr.line_num));
                return;
            }

            arrayInit(im, var, 0, arr_size - 1, 0);
        }
    }

    void fillFloatInitials(InitNode* initVals, VarAttribute& var)
    {
        var.type        = floatType;
        size_t arr_size = 1;
        for (auto& dim : var.dims) arr_size *= dim;
        var.initVals.resize(arr_size, static_cast<float>(0));

        if (var.dims.empty())  // 非数组
        {
            var.initVals[0] = static_cast<float>(0.0);
            InitSingle* is  = dynamic_cast<InitSingle*>(initVals);
            if (!is)
            {
                semanticErrMsgs.push_back(
                    "Error: Invalid initialization at line " + to_string(initVals->attr.line_num));
                return;
            }

            if (is->attr.val.type == voidType)
            {
                semanticErrMsgs.push_back(
                    "Error: Initialization with void type expression at line " + to_string(initVals->attr.line_num));
                return;
            }
            else if (is->attr.val.type == intType)
                var.initVals[0] = static_cast<float>(get<int>(is->attr.val.value));
            else if (is->attr.val.type == llType)
                var.initVals[0] = static_cast<float>(get<long long>(is->attr.val.value));
            else if (is->attr.val.type == floatType)
                var.initVals[0] = get<float>(is->attr.val.value);
            else if (is->attr.val.type == boolType)
                var.initVals[0] = static_cast<float>(get<bool>(is->attr.val.value));
            else
                semanticErrMsgs.push_back(
                    "Error: Invalid initialization at line " + to_string(initVals->attr.line_num));

            return;
        }
        else  // 数组
        {
            InitMulti* im = dynamic_cast<InitMulti*>(initVals);
            if (!im)
            {
                semanticErrMsgs.push_back(
                    "Error: Invalid initialization at line " + to_string(initVals->attr.line_num));
                return;
            }

            arrayInit(im, var, 0, arr_size - 1, 0);
        }
    }
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
                if (baseType == intType)
                    fillIntInitials(rval, val);
                else if (baseType == floatType)
                    fillFloatInitials(rval, val);
            }

            semTable.glbSymMap[lval->entry] = val;
        }
        return;
    }

    for (auto& def : *defs)
    {
        LeftValueExpr* lval = static_cast<LeftValueExpr*>(def->lval);
        InitNode*      rval = def->rval;

        auto& curTable = semTable.symTable.currentScope->symbolMap;
        auto  it       = curTable.find(lval->entry);
        if (it != curTable.end())
        {
            semanticErrMsgs.push_back(
                "Error: Redefinition of variable " + lval->entry->getName() + " at line " + to_string(attr.line_num));
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
            if (baseType == intType)
                fillIntInitials(rval, val);
            else if (baseType == floatType)
                fillFloatInitials(rval, val);
        }

        curTable[lval->entry] = val;
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