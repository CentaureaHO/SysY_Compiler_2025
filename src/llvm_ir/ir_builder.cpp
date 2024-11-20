#include <llvm_ir/ir_builder.h>
#include <common/type/symtab/semantic_table.h>
#include <common/type/symtab/symbol_table.h>
using namespace std;
using namespace LLVMIR;

IRTable::IRTable() : symTab(nullptr), regMap({}), formalArrTab({}) {}

IR::IR() : global_def({}), function_declare({}), function_block_map({}) {}