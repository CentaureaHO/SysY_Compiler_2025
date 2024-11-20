#include <ast/basic_node.h>
#include <llvm_ir/ir_builder.h>
#include <common/type/symtab/semantic_table.h>
using namespace std;
using namespace LLVMIR;

extern SemanticTable::Table* semTable;

Symbol::RegTable regTable;

IRTable irgen_table;
IR      builder;

FuncDefInst* cur_func = nullptr;
Type*        ret_type = nullptr;

int cur_label = 0;

int loop_start_label = -1;  // continue;
int loop_end_label   = -1;  // break;

int max_label = -1;
int max_reg   = -1;

void ASTree::genIRCode()
{
    // todo
    irgen_table.symTab = &regTable;

    builder.registerLibraryFunctions();
}