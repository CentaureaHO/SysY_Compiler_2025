#include <ast/basic_node.h>
#include <ast/statement.h>
#include <ast/expression.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/build/type_trans.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

extern SemanticTable::Table* semTable;

Symbol::RegTable regTable;

IRTable irgen_table;
IR      builder;

FuncDefInst* cur_func = nullptr;
Type*        ret_type = nullptr;

int cur_label = 0;

int loop_start_label = -1;  // continue;
int loop_end_label   = -1;  // break;

int max_label = -1;
int max_reg   = -1;

void ASTree::handleGlobalVarDecl(VarDeclStmt* decls)
{
    for (auto& def : *decls->defs)
    {
        LeftValueExpr* lval  = static_cast<LeftValueExpr*>(def->lval);
        InitNode*      rval  = def->rval;
        VarAttribute   val   = semTable->glbSymMap[lval->entry];
        DT             dtype = TYPE2LLVM(val.type->getKind());

        Instruction* decl_inst = nullptr;

        if (lval->dims)
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), val);
        else if (!rval)
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), nullptr);
        else if (dtype == DT::I32)
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), new ImmeI32Operand(TO_INT(val.initVals[0])));
        else if (dtype == DT::F32)
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), new ImmeF32Operand(TO_FLOAT(val.initVals[0])));
        else
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), nullptr);

        builder.global_def.push_back(decl_inst);
    }
}

void ASTree::genIRCode()
{
    irgen_table.symTab = &regTable;

    builder.registerLibraryFunctions();

    for (auto& stmt : *stmts)
    {
        VarDeclStmt* varDecl = dynamic_cast<VarDeclStmt*>(stmt);
        if (varDecl)
        {
            handleGlobalVarDecl(varDecl);
            continue;
        }
        stmt->genIRCode();
    }
}