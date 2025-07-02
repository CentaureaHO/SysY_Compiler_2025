#include <backend/rv64/function.h>
#include <queue>
#include <set>
using namespace Backend::RV64;
using namespace std;

Function::Function(string n, CFG* c)
    : name(n), cur_max_label(0), stack_size(0), param_size(0), has_stack_param(false), cfg(c)
{}

Function::~Function()
{
    // 清理所有Block（Block的析构函数会清理指令）
    for (auto* block : blocks) { delete block; }

    // 清理alloc_insts向量中的指令
    // 注意：这些指令可能已经在blocks中的指令列表中，但由于它们也在alloc_insts中，
    // 我们需要避免重复删除。实际上alloc_insts应该只是引用，不应该独立删除。
    // 但是为了安全起见，我们清空这个向量
    alloc_insts.clear();

    // 清理CFG
    delete cfg;
}

Register Function::getNewReg(DataType* type)
{
    static int new_regno = 0;

    Register ret;
    ret.is_virtual = true;
    ret.reg_num    = new_regno++;
    ret.data_type  = type;

    return ret;
}

Block* Function::getNewBlock()
{
    int    id = ++cur_max_label;
    Block* b  = new Block(id);
    b->parent = this;
    blocks.push_back(b);
    cfg->addNewBlock(id, b);

    return b;
}
