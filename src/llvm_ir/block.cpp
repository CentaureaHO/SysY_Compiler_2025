#include <llvm_ir/block.h>
#include <map>
using namespace std;
using namespace LLVMIR;

IRBlock::IRBlock(int id) : comment(""), block_id(id), insts({}) {}

void IRBlock::printIR(ostream& s)
{
    // todo
}