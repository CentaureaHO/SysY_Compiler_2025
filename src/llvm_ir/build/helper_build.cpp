#include <ast/helper.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/function.h>
#include <common/type/symtab/semantic_table.h>
#include <iostream>
#include <cassert>
using namespace std;
using namespace LLVMIR;

extern IRTable irgen_table;
extern IR      builder;

void HelperNode::genIRCode()
{
    cerr << "Base HelperNode genIRCode should not be called, occurred at line " << line_num << endl;
    assert(false);
}