#include <llvm_ir/build/type_trans.h>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

vector<DT> type2LLVM_vec = {
    DT::VOID,    // Type::Void
    DT::I32,     // Type::Int
    DT::I32,     // Type::LL, but cheat as I32
    DT::F32,     // Type::Float
    DT::DOUBLE,  // Type::Double, not implemented
    DT::I1,      // Type::Bool
    DT::PTR,     // Type::Ptr
    DT::PTR,     // Type::Arr, not implemented
    DT::PTR,     // Type::Func, not implemented
    DT::PTR,     // Type::CStr, not implemented
    DT::I8,      // Type::Char, not implemented
    DT::PTR      // Type::Str, not implemented
};