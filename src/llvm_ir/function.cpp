#include <llvm_ir/function.h>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

IRFunction::IRFunction(Type* rt, FuncDefInst* fd)
    : ret_type(rt),
      func_def(fd),
      blocks({}),
      symTab(),
      regMap({}),
      formalArrTab({}),
      cur_label(0),
      max_label(-1),
      loop_start_label(-1),
      loop_end_label(-1),
      max_reg(-1)
{}

inline IRBlock* IRFunction::createBlock()
{
    IRBlock* block = new IRBlock(++max_label);
    blocks.push_back(block);
    return block;
}
inline IRBlock* IRFunction::getBlock(int label) { return blocks[label]; }