#include <ast/basic_node.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/build/type_trans.h>
#include <ast/expression.h>
#include <common/type/symtab/semantic_table.h>
#include <iostream>
#include <cassert>
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

void ExprNode::genIRCode() { cerr << "ExprNode genIRCode not implemented" << endl; }

void LeftValueExpr::genIRCode()
{
    IRBlock*         block = builder.getBlock(cur_func, cur_label);
    vector<Operand*> idxs;
    // VarAttribute     val   = semTable->glbSymMap[entry];
    VarAttribute* val;
    bool          arr_flag = false;

    if (dims)
    {
        for (auto dim : *dims)
        {
            dim->genIRCode();
            block->insertTypeConvert(dim->attr.val.type->getKind(), TypeKind::Int, max_reg);
            idxs.push_back(getRegOperand(max_reg));
        }

        arr_flag = !idxs.empty();
    }

    Operand* val_ptr        = nullptr;
    int      local_reg_num  = irgen_table.symTab->getReg(entry);
    bool     param_arr_flag = false;
    if (local_reg_num == -1)  // 本地不存在，为全局变量
    {
        val_ptr = getGlobalOperand(entry->getName());
        val     = &semTable->glbSymMap[entry];

        auto it = irgen_table.formalArrTab.find(local_reg_num);
        if (it != irgen_table.formalArrTab.end()) param_arr_flag = true;
    }
    else
    {
        val_ptr = getRegOperand(local_reg_num);
        val     = &irgen_table.regMap[local_reg_num];
    }

    DT dtype = TYPE2LLVM(val->type->getKind());

    if (arr_flag || attr.val.type->getKind() == TypeKind::Ptr)
    {
        if (!param_arr_flag) idxs.insert(idxs.begin(), new ImmeI32Operand(0));

        block->insertGEP_I32(dtype, val_ptr, val->dims, idxs, ++max_reg);
        val_ptr = getRegOperand(max_reg);
    }

    if (!isLval && attr.val.type->getKind() != TypeKind::Ptr) block->insertLoad(dtype, val_ptr, ++max_reg);
}

void ConstExpr::genIRCode()
{
    IRBlock* block = builder.getBlock(cur_func, cur_label);

    ++max_reg;
    switch (type)
    {
        case 1:
        case 2:
        {
            int val = TO_INT(value);
            block->insertArithmeticI32_ImmeAll(IROpCode::ADD, val, 0, max_reg);
            break;
        }
        case 3:
        {
            float val = TO_FLOAT(value);
            block->insertArithmeticF32_ImmeAll(IROpCode::FADD, val, 0, max_reg);
            break;
        }
        default: assert(false);
    }
}

void UnaryExpr::genIRCode() { cerr << "UnaryExpr genIRCode not implemented" << endl; }

void BinaryExpr::genIRCode() { cerr << "BinaryExpr genIRCode not implemented" << endl; }

void FuncCallExpr::genIRCode() { cerr << "FuncCallExpr genIRCode not implemented" << endl; }