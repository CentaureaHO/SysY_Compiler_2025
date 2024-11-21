#include <llvm_ir/instruction.h>
#include <cassert>
#include <sstream>
#include <map>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <stack>
#include <algorithm>
#include <cstring>
#include <bit>
using namespace std;
using namespace LLVMIR;

using DT = DataType;
using OT = OperandType;

#define FLOAT_TO_DOUBLE_BITS(f)                            \
    ([](float value) {                                     \
        double             d = static_cast<double>(value); \
        unsigned long long rawDoubleBytes;                 \
        std::memcpy(&rawDoubleBytes, &d, sizeof(double));  \
        return static_cast<long long>(rawDoubleBytes);     \
    }(f))

Operand::Operand(OperandType t) : type(t) {}

RegOperand::RegOperand(int num) : Operand(OT::REG), reg_num(num) {}
string RegOperand::getName() { return "%reg_" + to_string(reg_num); }

ImmeI32Operand::ImmeI32Operand(int v) : Operand(OT::IMMEI32), value(v) {}
string ImmeI32Operand::getName() { return to_string(value); }

ImmeF32Operand::ImmeF32Operand(float v) : Operand(OT::IMMEF32), value(v) {}
string ImmeF32Operand::getName()
{
    stringstream ss;
    ss << "0x" << hex << FLOAT_TO_DOUBLE_BITS(value);
    return ss.str();
}

LabelOperand::LabelOperand(int num) : Operand(OT::LABEL), label_num(num) {}
string LabelOperand::getName() { return "%Block" + to_string(label_num); }

GlobalOperand::GlobalOperand(string name) : Operand(OT::GLOBAL), global_name(name) {}
string GlobalOperand::getName() { return "@" + global_name; }

/*
    Followed insts
 */
Instruction::Instruction(IROpCode op) : opcode(op) {}

LoadInst::LoadInst(DataType t, Operand* p, Operand* r) : Instruction(IROpCode::LOAD), type(t), ptr(p), res(r) {}
void LoadInst::printIR(ostream& s) { s << res << " = load " << type << ", ptr " << ptr << "\n"; }

StoreInst::StoreInst(DataType t, Operand* p, Operand* r) : Instruction(IROpCode::STORE), type(t), ptr(p), val(r) {}
void StoreInst::printIR(ostream& s) { s << "store " << type << " " << val << ", ptr " << ptr << "\n"; }

ArithmeticInst::ArithmeticInst(IROpCode op, DataType t, Operand* l, Operand* r, Operand* res)
    : Instruction(op), type(t), lhs(l), rhs(r), res(res)
{}
void ArithmeticInst::printIR(ostream& s)
{
    s << res << " = " << opcode << " " << type << " " << lhs << ", " << rhs << "\n";
}

IcmpInst::IcmpInst(DataType t, IcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::ICMP), type(t), cond(c), lhs(l), rhs(r), res(res)
{}
void IcmpInst::printIR(ostream& s)
{
    s << res << " = icmp " << cond << " " << type << " " << lhs << ", " << rhs << "\n";
}

FcmpInst::FcmpInst(DataType t, FcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::FCMP), type(t), cond(c), lhs(l), rhs(r), res(res)
{}
void FcmpInst::printIR(ostream& s)
{
    s << res << " = fcmp " << cond << " " << type << " " << lhs << ", " << rhs << "\n";
}

AllocInst::AllocInst(DataType t, Operand* r, vector<int> d) : Instruction(IROpCode::ALLOCA), type(t), res(r), dims(d) {}
void AllocInst::printIR(ostream& s)
{
    s << res << " = alloca ";
    if (dims.empty())
    {
        s << type << "\n";
        return;
    }

    for (int& dim : dims) s << "[" << dim << " x ";
    s << type << string(dims.size(), ']') << "\n";
}

BranchCondInst::BranchCondInst(Operand* c, Operand* t, Operand* f)
    : Instruction(IROpCode::BR_COND), cond(c), true_label(t), false_label(f)
{}
void BranchCondInst::printIR(ostream& s)
{
    s << "br i1 " << cond << ", label " << true_label << ", label " << false_label << "\n";
}

BranchUncondInst::BranchUncondInst(Operand* t) : Instruction(IROpCode::BR_UNCOND), target_label(t) {}
void BranchUncondInst::printIR(ostream& s) { s << "br label " << target_label << "\n"; }

GlbvarDefInst::GlbvarDefInst(DataType t, string n, Operand* v)
    : Instruction(IROpCode::GLOBAL_VAR), type(t), name(n), init(v)
{}
GlbvarDefInst::GlbvarDefInst(DataType t, string n, VarAttribute a)
    : Instruction(IROpCode::GLOBAL_VAR), type(t), name(n), arr_init(a)
{}
namespace
{
    void glb_arr_init(ostream& s, DataType type, VarAttribute& v, int dimDph, int beginPos, int endPos)
    {
        if (dimDph == 0)
        {
            bool all_zero = std::all_of(v.initVals.begin(), v.initVals.end(), [&](auto& val) {
                return (v.type == intType || v.type == llType || v.type == boolType) ? TO_INT(val) == 0
                                                                                     : TO_FLOAT(val) == 0;
            });

            if (all_zero)
            {
                for (int dim : v.dims) s << "[" << dim << " x ";
                s << type << string(v.dims.size(), ']') << " zeroinitializer";
                return;
            }
        }

        if (beginPos == endPos)
        {
            switch (type)
            {
                case DT::I1:
                case DT::I32:
                case DT::I64: s << type << " " << TO_INT(v.initVals[beginPos]); break;
                case DT::F32:
                    s << type << " 0x" << hex << FLOAT_TO_DOUBLE_BITS(TO_FLOAT(v.initVals[beginPos])) << dec;
                    break;
                default: assert(false);
            }
            return;
        }

        for (size_t i = dimDph; i < v.dims.size(); ++i) s << "[" << v.dims[i] << " x ";
        s << type << string(v.dims.size() - dimDph, ']') << " [";

        int step = std::accumulate(v.dims.begin() + dimDph + 1, v.dims.end(), 1, std::multiplies<int>());

        for (int i = 0; i < v.dims[dimDph]; ++i)
        {
            if (i != 0) s << ",";
            glb_arr_init(s, type, v, dimDph + 1, beginPos + i * step, beginPos + (i + 1) * step - 1);
        }

        s << "]";
    }
}  // namespace
void GlbvarDefInst::printIR(ostream& s)
{
    s << "@" << name << " = global ";
    if (arr_init.dims.empty())
    {
        s << type << " ";

        if (init)
            s << init << "\n";
        else
            s << "zeroinitializer\n";

        return;
    }

    int step = 1;
    for (int& dim : arr_init.dims) step *= dim;

    glb_arr_init(s, type, arr_init, 0, 0, step - 1);

    s << "\n";
}

CallInst::CallInst(DataType rt, string fn, Operand* r)
    : Instruction(IROpCode::CALL), ret_type(rt), func_name(fn), args({}), res(r)
{}
CallInst::CallInst(DataType rt, string fn, vector<pair<DataType, Operand*>> a, Operand* r)
    : Instruction(IROpCode::CALL), ret_type(rt), func_name(fn), args(a), res(r)
{}
void CallInst::printIR(ostream& s)
{
    if (ret_type != DT::VOID) s << res << " = ";
    s << "call " << ret_type << " @" << func_name << "(";

    auto it = args.begin();
    auto cp = it;
    for (; it != args.end(); ++it)
    {
        s << it->first << " " << it->second;
        ++cp;
        if (cp != args.end()) s << ", ";
    }
    s << ")\n";
}

RetInst::RetInst(DataType t, Operand* r) : Instruction(IROpCode::RET), ret_type(t), ret(r) {}
void RetInst::printIR(ostream& s)
{
    s << "ret " << ret_type;
    if (ret) s << " " << ret;
    s << "\n";
}

GEPInst::GEPInst(DataType t, DataType it, Operand* bp, Operand* r, vector<int> d, vector<Operand*> is)
    : Instruction(IROpCode::GETELEMENTPTR), type(t), idx_type(it), base_ptr(bp), res(r), dims(d), idxs(is)
{}
void GEPInst::printIR(ostream& s)
{
    s << res << " = getelementptr ";
    if (dims.empty())
        s << type;
    else
    {
        for (int& dim : dims) s << "[" << dim << " x ";
        s << type << string(dims.size(), ']');
    }

    s << ", ptr " << base_ptr;
    for (auto& idx : idxs) s << ", " << idx_type << " " << idx;
    s << "\n";
}

FuncDeclareInst::FuncDeclareInst(DataType rt, string fn, vector<DataType> at)
    : Instruction(IROpCode::OTHER), ret_type(rt), func_name(fn), arg_types(at)
{}
void FuncDeclareInst::printIR(ostream& s)
{
    s << "declare " << ret_type << " @" << func_name << "(";
    auto it = arg_types.begin();
    auto cp = it;
    for (; it != arg_types.end(); ++it)
    {
        s << *it;
        ++cp;
        if (cp != arg_types.end()) s << ", ";
    }
    s << ")\n";
}

FuncDefInst::FuncDefInst(DataType rt, string fn, vector<DataType> at) : FuncDeclareInst(rt, fn, at), arg_regs({}) {}
void FuncDefInst::printIR(ostream& s)
{
    size_t arg_num = arg_types.size();
    size_t reg_num = arg_regs.size();
    assert(arg_num == reg_num);

    s << "define " << ret_type << " @" << func_name << "(";

    for (size_t i = 0; i < arg_num; ++i)
    {
        s << arg_types[i] << " " << arg_regs[i];
        if (i != arg_num - 1) s << ", ";
    }

    s << ")\n";
}

SI2FPInst::SI2FPInst(Operand* f, Operand* t) : Instruction(IROpCode::SITOFP), f_si(f), t_fp(t) {}
void SI2FPInst::printIR(ostream& s) { s << t_fp << " = sitofp i32 " << f_si << " to float\n"; }

FP2SIInst::FP2SIInst(Operand* f, Operand* t) : Instruction(IROpCode::FPTOSI), f_fp(f), t_si(t) {}
void FP2SIInst::printIR(ostream& s) { s << t_si << " = fptosi float " << f_fp << " to i32\n"; }

ZextInst::ZextInst(DataType f, DataType t, Operand* s, Operand* d)
    : Instruction(IROpCode::ZEXT), from(f), to(t), src(s), dest(d)
{}
void ZextInst::printIR(ostream& s) { s << dest << " = zext " << from << " " << src << " to " << to << "\n"; }

FPExtInst::FPExtInst(Operand* s, Operand* d) : Instruction(IROpCode::FPEXT), src(s), dest(d) {}
void FPExtInst::printIR(ostream& s)
{
    s << dest << " = fpext float"
      << " " << src << " to "
      << "double"
      << "\n";
}

ostream& operator<<(std::ostream& s, LLVMIR::Operand* op)
{
    s << op->getName();
    return s;
}

namespace
{
    unordered_map<int, ImmeI32Operand*>   ImmeI32OperandMap;
    unordered_map<float, ImmeF32Operand*> ImmeF32OperandMap;
    unordered_map<int, RegOperand*>       RegOperandMap;
    map<int, LabelOperand*>               LabelOperandMap;
    map<string, GlobalOperand*>           GlobalOperandMap;

    class Cleaner
    {
      public:
        Cleaner() {}
        ~Cleaner()
        {
            for (auto& [_, op] : ImmeI32OperandMap)
            {
                // if (!op) continue;
                delete op;
                op = nullptr;
            }
            for (auto& [_, op] : ImmeF32OperandMap)
            {
                // if (!op) continue;
                delete op;
                op = nullptr;
            }
            for (auto& [_, op] : RegOperandMap)
            {
                // if (!op) continue;
                delete op;
                op = nullptr;
            }
            for (auto& [_, op] : LabelOperandMap)
            {
                // if (!op) continue;
                delete op;
                op = nullptr;
            }
            for (auto& [_, op] : GlobalOperandMap)
            {
                // if (!op) continue;
                delete op;
                op = nullptr;
            }

            RegOperandMap.clear();
            LabelOperandMap.clear();
            GlobalOperandMap.clear();
        }
    } cleaner;
}  // namespace

ImmeI32Operand* getImmeI32Operand(int num)
{
    auto it = ImmeI32OperandMap.find(num);
    if (it == ImmeI32OperandMap.end())
    {
        ImmeI32Operand* op     = new ImmeI32Operand(num);
        ImmeI32OperandMap[num] = op;
        return op;
    }
    return it->second;
}

ImmeF32Operand* getImmeF32Operand(float num)
{
    auto it = ImmeF32OperandMap.find(num);
    if (it == ImmeF32OperandMap.end())
    {
        ImmeF32Operand* op     = new ImmeF32Operand(num);
        ImmeF32OperandMap[num] = op;
        return op;
    }
    return it->second;
}

RegOperand* getRegOperand(int num)
{
    auto it = RegOperandMap.find(num);
    if (it == RegOperandMap.end())
    {
        RegOperand* op     = new RegOperand(num);
        RegOperandMap[num] = op;
        return op;
    }
    return it->second;
}

LabelOperand* getLabelOperand(int num)
{
    auto it = LabelOperandMap.find(num);
    if (it == LabelOperandMap.end())
    {
        LabelOperand* op     = new LabelOperand(num);
        LabelOperandMap[num] = op;
        return op;
    }
    return it->second;
}

GlobalOperand* getGlobalOperand(string name)
{
    auto it = GlobalOperandMap.find(name);
    if (it == GlobalOperandMap.end())
    {
        GlobalOperand* op      = new GlobalOperand(name);
        GlobalOperandMap[name] = op;
        return op;
    }
    return it->second;
}