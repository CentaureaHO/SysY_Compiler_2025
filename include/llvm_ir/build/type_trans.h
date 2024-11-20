#ifndef __COMMON_TYPE_TYPECALC_H__
#define __COMMON_TYPE_TYPECALC_H__

#include <vector>
#include <common/type/type_defs.h>
#include <llvm_ir/defs.h>
#include <ast/expression.h>
#include <llvm_ir/block.h>

#define TYPE2LLVM(x) type2LLVM_vec[static_cast<int>(x)]

extern std::vector<LLVMIR::DataType> type2LLVM_vec;

void IR_GenUnary(ExprNode* expr, OpCode op, LLVMIR::IRBlock* block);
void IR_GenBinary(ExprNode* lhs, ExprNode* rhs, OpCode op, LLVMIR::IRBlock* block);

#endif