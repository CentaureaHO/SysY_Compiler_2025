#ifndef __COMMON_TYPE_TYPECALC_H__
#define __COMMON_TYPE_TYPECALC_H__

#include <vector>
#include <common/type/type_defs.h>
#include <llvm_ir/defs.h>

#define TYPE2LLVM(x) type2LLVM_vec(static_cast<int>(x))

extern std::vector<LLVMIR::DataType> type2LLVM_vec;

#endif