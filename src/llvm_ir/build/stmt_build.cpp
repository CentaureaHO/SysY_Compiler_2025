#include <ast/statement.h>
#include <ast/expression.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/build/type_trans.h>
#include <common/type/symtab/semantic_table.h>
#include <iostream>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

extern SemanticTable::Table* semTable;

extern IRTable irgen_table;
extern IR      builder;

extern FuncDefInst* cur_func;
extern Type*        ret_type;

extern int cur_label;
extern int loop_start_label;  // continue;
extern int loop_end_label;    // break;

extern int max_label;
extern int max_reg;

#define NEW_BLOCK() builder.createBlock(cur_func, ++max_label)

void StmtNode::genIRCode() { cerr << "StmtNode genIRCode not implemented" << endl; }

void ExprStmt::genIRCode() { cerr << "ExprStmt genIRCode not implemented" << endl; }

void VarDeclStmt::genIRCode() { cerr << "VarDeclStmt genIRCode not implemented" << endl; }

void BlockStmt::genIRCode() { cerr << "BlockStmt genIRCode not implemented" << endl; }

void FuncDeclStmt::genIRCode()
{
    irgen_table.symTab->enterScope();
    irgen_table.regMap.clear();
    irgen_table.formalArrTab.clear();

    DT           llvm_ret_type = TYPE2LLVM(returnType->getKind());
    FuncDefInst* func_def      = new FuncDefInst(llvm_ret_type, entry->getName());

    cur_func = func_def;
    ret_type = returnType;

    cur_label = 0;
    max_label = -1;

    if (params)
        max_reg = static_cast<int>(params->size() - 1);
    else
        max_reg = -1;

    builder.enterFunc(func_def);
    IRBlock* block = NEW_BLOCK();

    if (params)
    {
        auto& params_vec = *params;
        for (size_t i = 0; i < params_vec.size(); ++i)
        {
            VarAttribute val;
            auto&        pdefNode = *params_vec[i];
            val.type              = pdefNode.baseType;
            DT dtype              = TYPE2LLVM(val.type->getKind());

            if (pdefNode.dims)
            {
                func_def->arg_types.push_back(DT::PTR);
                irgen_table.formalArrTab[i] = true;
                for (size_t j = 1; j < pdefNode.dims->size(); ++j)
                    val.dims.push_back(TO_INT(pdefNode.dims->at(j)->attr.val.value));

                irgen_table.symTab->addSymbol(pdefNode.entry, max_reg);
                irgen_table.regMap[max_reg] = val;
                func_def->arg_regs.push_back(getRegOperand(i));
            }
            else
            {
                func_def->arg_types.push_back(dtype);

                ++max_reg;
                block->insertAlloc(dtype, max_reg);
                block->insertStore(dtype, i, getRegOperand(max_reg));

                irgen_table.symTab->addSymbol(pdefNode.entry, max_reg);
                irgen_table.regMap[max_reg] = val;
                func_def->arg_regs.push_back(getRegOperand(i));
            }
        }
    }

    block->insertUncondBranch(1);

    block     = NEW_BLOCK();
    cur_label = max_label;

    if (body) body->genIRCode();

    for (auto& [idx, block] : builder.function_block_map[func_def])
    {
        bool broken = false;

        if (block->insts.empty())
            broken = true;
        else
        {
            auto& last_inst = block->insts.back();
            broken          = !(IS_BR(last_inst) || IS_RET(last_inst));
        }

        if (broken)
        {
            if (llvm_ret_type == DT::I32)
                block->insertRetImmI32(DT::I32, 0);
            else if (llvm_ret_type == DT::F32)
                block->insertRetImmF32(DT::F32, 0.3);
            else
                block->insertRetVoid();
        }
    }

    irgen_table.symTab->exitScope();
}

void ReturnStmt::genIRCode() { cerr << "ReturnStmt genIRCode not implemented" << endl; }

void WhileStmt::genIRCode() { cerr << "WhileStmt genIRCode not implemented" << endl; }

void IfStmt::genIRCode() { cerr << "IfStmt genIRCode not implemented" << endl; }

void ForStmt::genIRCode() { cerr << "ForStmt genIRCode not implemented" << endl; }

void BreakStmt::genIRCode() { cerr << "BreakStmt genIRCode not implemented" << endl; }

void ContinueStmt::genIRCode() { cerr << "ContinueStmt genIRCode not implemented" << endl; }