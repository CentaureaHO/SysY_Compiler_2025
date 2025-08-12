#include <backend/rv64/rv64_block.h>
using namespace Backend::RV64;
using namespace std;

Block::Block(int id) : label_num(id) {}

Block::~Block()
{
    for (auto* inst : insts) { Instruction::delInst(inst); }

    for (auto& pair : phi_copy_map) { delete pair.second; }
}
