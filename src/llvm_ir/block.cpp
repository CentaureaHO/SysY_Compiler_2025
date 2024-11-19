#include <llvm_ir/block.h>
using namespace std;
using namespace LLVMIR;

IRBlock::IRBlock(int id) : comment(""), block_id(id), insts({}) {}