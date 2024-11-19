#include <llvm_ir/ir_builder.h>
using namespace std;
using namespace LLVMIR;

IR::IR() : global_def({}), function_declare({}), function_block_map({}) {}