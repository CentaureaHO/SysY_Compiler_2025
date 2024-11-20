#include <ast/helper.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace LLVMIR;

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

void HelperNode::genIRCode()
{
    // todo
}