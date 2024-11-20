#include <ast/basic_node.h>
#include <ast/statement.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/build/type_trans.h>
#include <ast/expression.h>
#include <common/type/symtab/semantic_table.h>
#include <iostream>
#include <cassert>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

#define NEW_BLOCK() builder.createBlock(cur_func, ++max_label)

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
    }
    else
    {
        val_ptr = getRegOperand(local_reg_num);
        val     = &irgen_table.regMap[local_reg_num];
        auto it = irgen_table.formalArrTab.find(local_reg_num);
        if (it != irgen_table.formalArrTab.end()) param_arr_flag = true;
    }

    DT dtype = TYPE2LLVM(val->type->getKind());

    if (arr_flag || attr.val.type->getKind() == TypeKind::Ptr)
    {
        if (!param_arr_flag) idxs.insert(idxs.begin(), new ImmeI32Operand(0));

        block->insertGEP_I32(dtype, val_ptr, val->dims, idxs, ++max_reg);
        val_ptr = getRegOperand(max_reg);
    }

    if (!isLval && attr.val.type->getKind() != TypeKind::Ptr) block->insertLoad(dtype, val_ptr, ++max_reg);

    lv_ptr = val_ptr;
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

void UnaryExpr::genIRCode()
{
    IRBlock* block = builder.getBlock(cur_func, cur_label);
    IR_GenUnary(val, op, block);
}

void BinaryExpr::genIRCode_Assign()
{
    IRBlock* block = builder.getBlock(cur_func, cur_label);

    lhs->genIRCode();
    rhs->genIRCode();

    LeftValueExpr* lval = dynamic_cast<LeftValueExpr*>(lhs);
    assert(lval != nullptr);

    DT dtype = TYPE2LLVM(lval->attr.val.type->getKind());

    block->insertTypeConvert(rhs->attr.val.type->getKind(), lval->attr.val.type->getKind(), max_reg);

    block->insertStore(dtype, getRegOperand(max_reg), lval->lv_ptr);
}

/*
Logical AND Short Circuit Evaluation:

        +-------------------+
        |                   |
        |   Evaluate Left   |
        |     Operand       |
        |                   |
        +---------+---------+
                  |
         Is Left True?
                  |
          +-------+------+
          |              |
        Yes             No
          |              |
          |          Jump to False Label
          |              |
   +------+------+
   |             |
   |  Evaluate   |
   |  Right      |
   |  Operand    |
   |             |
   +------+------+
          |
     Is Right True?
          |
    +-----+-----+
    |           |
   Yes         No
    |           |
  Jump to     Jump to
  True Label  False Label
*/
void BinaryExpr::genIRCode_LogicalAnd()
{
    IRBlock* right_eval_block = NEW_BLOCK();
    right_eval_block->comment = "And opertor at line " + to_string(line_num);

    lhs->true_label  = right_eval_block->block_id;
    lhs->false_label = false_label;

    rhs->true_label  = true_label;
    rhs->false_label = false_label;

    lhs->genIRCode();
    IRBlock* block = builder.getBlock(cur_func, cur_label);
    block->insertTypeConvert(lhs->attr.val.type->getKind(), TypeKind::Bool, max_reg);
    block->insertCondBranch(max_reg, lhs->true_label, lhs->false_label);

    cur_label = lhs->true_label;
    rhs->genIRCode();
    block = builder.getBlock(cur_func, cur_label);
    block->insertTypeConvert(rhs->attr.val.type->getKind(), TypeKind::Bool, max_reg);
    // block->insertCondBranch(max_reg, rhs->true_label, rhs->false_label);
}

/*
Logical OR Short Circuit Evaluation:

        +-------------------+
        |                   |
        |   Evaluate Left   |
        |     Operand       |
        |                   |
        +---------+---------+
                  |
         Is Left False?
                  |
          +-------+------+
          |              |
         No            Yes
          |              |
      Jump to       +----+-----+
     True Label     |          |
                    | Evaluate |
                    |  Right   |
                    | Operand  |
                    |          |
                    +----+-----+
                         |
                   Is Right True?
                         |
                   +-----+-----+
                   |           |
                  Yes         No
                   |           |
                 Jump to     Jump to
                 True Label  False Label
*/
void BinaryExpr::genIRCode_LogicalOr()
{
    IRBlock* right_eval_block = NEW_BLOCK();
    right_eval_block->comment = "Or opertor at line " + to_string(line_num);

    lhs->true_label  = true_label;
    lhs->false_label = right_eval_block->block_id;

    rhs->true_label  = true_label;
    rhs->false_label = false_label;

    lhs->genIRCode();
    IRBlock* block = builder.getBlock(cur_func, cur_label);
    block->insertTypeConvert(lhs->attr.val.type->getKind(), TypeKind::Bool, max_reg);
    block->insertCondBranch(max_reg, lhs->true_label, lhs->false_label);

    cur_label = right_eval_block->block_id;
    rhs->genIRCode();
    block = builder.getBlock(cur_func, cur_label);
    block->insertTypeConvert(rhs->attr.val.type->getKind(), TypeKind::Bool, max_reg);
}

void BinaryExpr::genIRCode()
{
    if (op == OpCode::Assign)
    {
        genIRCode_Assign();
        return;
    }
    else if (op == OpCode::And)
    {
        genIRCode_LogicalAnd();
        return;
    }
    else if (op == OpCode::Or)
    {
        genIRCode_LogicalOr();
        return;
    }

    IRBlock* block = builder.getBlock(cur_func, cur_label);
    IR_GenBinary(lhs, rhs, op, block);
}

void FuncCallExpr::genIRCode()
{
    IRBlock* block = builder.getBlock(cur_func, cur_label);

    FuncDeclStmt* func_decl = semTable->funcDeclMap[entry];
    Type*         ret_type  = func_decl->returnType;
    DT            dtype     = TYPE2LLVM(ret_type->getKind());

    if (!args)
    {
        if (ret_type == voidType)
            block->insertCallVoidNoargs(dtype, entry->getName());
        else
            block->insertCallNoargs(dtype, entry->getName(), ++max_reg);
        return;
    }

    vector<pair<DataType, Operand*>> llvm_args;

    auto& params_vec = *func_decl->params;
    auto& args_vec   = *args;

    size_t param_size = params_vec.size();
    size_t args_size  = args_vec.size();
    assert(param_size == args_size);

    for (size_t i = 0; i < param_size; ++i)
    {
        TypeKind param_kind = params_vec[i]->baseType->getKind();
        if (params_vec[i]->dims) param_kind = TypeKind::Ptr;

        DT param_dtype = TYPE2LLVM(param_kind);

        args_vec[i]->genIRCode();
        block->insertTypeConvert(args_vec[i]->attr.val.type->getKind(), param_kind, max_reg);
        llvm_args.push_back({param_dtype, getRegOperand(max_reg)});
    }

    if (ret_type == voidType)
        block->insertCallVoid(dtype, entry->getName(), llvm_args);
    else
        block->insertCall(dtype, entry->getName(), llvm_args, ++max_reg);
}