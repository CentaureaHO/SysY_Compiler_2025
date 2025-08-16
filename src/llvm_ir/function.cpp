#include <llvm_ir/function.h>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

IRFunction::IRFunction(Type* rt, FuncDefInst* fd)
    : ret_type(rt),
      func_def(fd),
      blocks({}),
      cur_label(0),
      max_label(-1),
      loop_start_label(-1),
      loop_end_label(-1),
      max_reg(-1)
{}
IRFunction::~IRFunction()
{
    delete func_def;
    func_def = nullptr;
    for (auto& block : blocks)
    {
        delete block;
        block = nullptr;
    }
}

IRBlock* IRFunction::createBlock()
{
    IRBlock* block = new IRBlock(++max_label);
    blocks.push_back(block);
    return block;
}
IRBlock* IRFunction::getBlock(int label) { return blocks[label]; }

Instruction* IRFunction::findDef(Operand* op)
{
    if (op->type != OperandType::REG) { return nullptr; }
    int reg_num = static_cast<RegOperand*>(op)->reg_num;
    for (auto* block : blocks)
    {
        for (auto* inst : block->insts)
        {
            if (inst->GetResultReg() == reg_num) { return inst; }
        }
    }
    return nullptr;
}
