#include <llvm_ir/defs.h>
using namespace std;
using namespace LLVMIR;

ostream& operator<<(std::ostream& os, DataType type)
{
    int idx = static_cast<int>(type);
    switch (idx)
    {
#define X(type, name, idx) \
    case idx: os << #name; break;
        IR_DATATYPE
#undef X
        default: os << "unknown";
    }
    return os;
}

ostream& operator<<(std::ostream& os, IROpCode type)
{
    int idx = static_cast<int>(type);
    switch (idx)
    {
#define X(op, name, idx) \
    case idx: os << #name; break;
        IR_OPCODE
#undef X
        default: os << "unknown";
    }
    return os;
}

ostream& operator<<(std::ostream& os, IcmpCond type)
{
    int idx = static_cast<int>(type);
    switch (idx)
    {
#define X(cond, name, idx) \
    case idx: os << #name; break;
        IR_ICMP
#undef X
        default: os << "unknown";
    }
    return os;
}

ostream& operator<<(std::ostream& os, FcmpCond type)
{
    int idx = static_cast<int>(type);
    switch (idx)
    {
#define X(cond, name, idx) \
    case idx: os << #name; break;
        IR_FCMP
#undef X
        default: os << "unknown";
    }
    return os;
}