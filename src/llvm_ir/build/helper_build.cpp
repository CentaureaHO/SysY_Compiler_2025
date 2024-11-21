#include <ast/helper.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/function.h>
#include <common/type/symtab/semantic_table.h>
#include <iostream>
using namespace std;
using namespace LLVMIR;

extern IRTable irgen_table;
extern IR      builder;

extern FuncDefInst* cur_func;
extern Type*        ret_type;

void HelperNode::genIRCode() { cerr << "HelperNode genIRCode not implemented" << endl; }