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
    // cerr << "LeftValueExpr genIRCode not implemented" << endl;

    Operand*     ptr_operand = getGlobalOperand(entry->getName());
    VarAttribute val         = semTable->glbSymMap[entry];

    DT dtype = TYPE2LLVM(val.type->getKind());

    if (isLval || attr.val.type->getKind() == TypeKind::Ptr) return;

    ++max_reg;
    IRBlock* block = builder.getBlock(cur_func, cur_label);
    block->insertLoad(dtype, ptr_operand, max_reg);
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