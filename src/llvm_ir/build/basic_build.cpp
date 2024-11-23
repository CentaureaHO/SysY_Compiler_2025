#include <ast/basic_node.h>
#include <ast/statement.h>
#include <ast/expression.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/build/type_trans.h>
#include <llvm_ir/function.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

Symbol::RegTable regTable;

IRTable                       irgen_table;
IR                            builder;
IRFunction*                   ir_func = nullptr;
set<pair<Operand*, Operand*>> phi_list;

void ASTree::handleGlobalVarDecl(VarDeclStmt* decls)
{
    for (auto& def : *decls->defs)
    {
        LeftValueExpr*      lval  = static_cast<LeftValueExpr*>(def->lval);
        InitNode*           rval  = def->rval;
        const VarAttribute& val   = semTable->glbSymMap[lval->entry];
        DT                  dtype = TYPE2LLVM(val.type->getKind());

        Instruction* decl_inst = nullptr;

        if (lval->dims)
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), val);
        else if (!rval)
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), nullptr);
        else if (dtype == DT::I32)
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), getImmeI32Operand(TO_INT(val.initVals[0])));
        else if (dtype == DT::F32)
            decl_inst = new GlbvarDefInst(dtype, lval->entry->getName(), getImmeF32Operand(TO_FLOAT(val.initVals[0])));
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