#include <cfg/cfg.h>
#include <llvm_ir/instruction.h>
using namespace std;
using namespace LLVMIR;

CFG::CFG(LLVMIR::FuncDefInst* f) : cur_func(f), block_map(nullptr), successorGraph({}), reverseGraph({}) {}