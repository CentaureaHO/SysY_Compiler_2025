#include <backend/rv64/block.h>
using namespace Backend::RV64;
using namespace std;

Block::Block(int id) : label_num(id) {}

Block::~Block()
{
    for (auto* inst : insts) { delete inst; }

    for (auto& pair : phi_copy_map) { delete pair.second; }
}