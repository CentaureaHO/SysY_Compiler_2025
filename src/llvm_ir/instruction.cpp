#include <llvm_ir/instruction.h>
#include <cassert>
#include <sstream>
#include <map>
#include <unordered_map>
using namespace std;
using namespace LLVMIR;

using DT = DataType;
using OT = OperandType;

static unordered_map<int, RegOperand*> RegOperandMap;
static map<int, LabelOperand*>         LabelOperandMap;
static map<string, GlobalOperand*>     GlobalOperandMap;

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

long long float2DoubleBits(float f)
{
    float              rawFloat     = f;
    unsigned long long rawFloatByte = *((int*)&rawFloat);
    unsigned long long signBit      = rawFloatByte >> 31;
    unsigned long long expBits      = (rawFloatByte >> 23) & ((1 << 8) - 1);
    unsigned long long part1        = rawFloatByte & ((1 << 23) - 1);

    unsigned long long out_signBit                 = signBit << 63;
    unsigned long long out_sigBits                 = part1 << 29;
    unsigned long long expBits_highestBit          = (expBits & (1 << 7)) << 3;
    unsigned long long expBits_lowerBit            = (expBits & ((1 << 7) - 1));
    unsigned long long expBits_lowerBit_highestBit = expBits_lowerBit & (1 << 6);
    unsigned long long expBits_lowerBit_ext = (expBits_lowerBit_highestBit) | (expBits_lowerBit_highestBit << 1) |
                                              (expBits_lowerBit_highestBit << 2) | (expBits_lowerBit_highestBit << 3);
    unsigned long long expBits_full     = expBits_highestBit | expBits_lowerBit | expBits_lowerBit_ext;
    unsigned long long out_expBits      = expBits_full << 52;
    unsigned long long out_rawFloatByte = out_signBit | out_expBits | out_sigBits;

    return out_rawFloatByte;
}

Operand::Operand(OperandType t) : type(t) {}

RegOperand::RegOperand(int num) : Operand(OT::REG), reg_num(num) {}
string RegOperand::getName() { return "%r" + to_string(reg_num); }

ImmeI32Operand::ImmeI32Operand(int v) : Operand(OT::IMMEI32), value(v) {}
string ImmeI32Operand::getName() { return to_string(value); }

ImmeI64Operand::ImmeI64Operand(long long v) : Operand(OT::IMMEI64), value(v) {}
string ImmeI64Operand::getName() { return to_string(value); }

ImmeF32Operand::ImmeF32Operand(float v) : Operand(OT::IMMEF32), value(v) {}
string ImmeF32Operand::getName()
{
    stringstream ss;
    ss << "0x" << hex << float2DoubleBits(value);
    return ss.str();
}

LabelOperand::LabelOperand(int num) : Operand(OT::LABEL), label_num(num) {}
string LabelOperand::getName() { return "%L" + to_string(label_num); }

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
    s << res << " = " << opcode << " " << type << " " << lhs << "," << rhs << "\n";
}

IcmpInst::IcmpInst(DataType t, IcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::ICMP), type(t), cond(c), lhs(l), rhs(r), res(res)
{}
void IcmpInst::printIR(ostream& s)
{
    s << res << " = icmp " << cond << " " << type << " " << lhs << "," << rhs << "\n";
}

FcmpInst::FcmpInst(DataType t, FcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::FCMP), type(t), cond(c), lhs(l), rhs(r), res(res)
{}
void FcmpInst::printIR(ostream& s)
{
    s << res << " = fcmp " << cond << " " << type << " " << lhs << "," << rhs << "\n";
}

PhiInst::PhiInst(DataType t, Operand* r, vector<pair<Operand*, Operand*>> v)
    : Instruction(IROpCode::PHI), type(t), res(r), vals(v)
{}
void PhiInst::printIR(ostream& s)
{
    s << res << " = phi " << type << " ";
    auto it = vals.begin();
    auto cp = it;
    for (; it != vals.end(); ++it)
    {
        s << "[" << it->second << "," << it->first << "]";
        ++cp;
        if (cp != vals.end()) s << ",";
    }
    s << "\n";
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
void recursive_init(ostream& s, DataType type, VarAttribute& v, int dimDph, int beginPos, int endPos)
{
    if (dimDph == 0)
    {
        bool all_zero = true;
        if (v.type == intType || v.type == llType || v.type == boolType)
        {
            for (auto& val : v.initVals)
                if (TO_INT(val) != 0)
                {
                    all_zero = false;
                    break;
                }
        }
        else
        {
            for (auto& val : v.initVals)
                if (TO_FLOAT(val) != 0)
                {
                    all_zero = false;
                    break;
                }
        }

        if (all_zero)
        {
            for (int dim : v.dims) s << "[" << dim << " x ";
            s << type << string(v.dims.size(), ']') << " zeroinitializer";

            return;
        }
    }
    if (beginPos == endPos)
    {
        if (type == DT::I32)
            s << type << " " << TO_INT(v.initVals[beginPos]);
        else if (type == DT::I64)
            s << type << " " << TO_LL(v.initVals[beginPos]);
        else if (type == DT::I1)
            s << type << " " << TO_INT(v.initVals[beginPos]);
        else if (type == DT::F32)
            s << type << " 0x" << hex << float2DoubleBits(TO_FLOAT(v.initVals[beginPos])) << dec;

        return;
    }

    for (size_t i = dimDph; i < v.dims.size(); ++i) s << "[" << v.dims[i] << " x ";

    s << type << string(v.dims.size() - dimDph, ']') << " [";

    int step = 1;
    for (size_t i = dimDph + 1; i < v.dims.size(); ++i) step *= v.dims[i];

    for (int i = 0; i < v.dims[dimDph]; ++i)
    {
        recursive_init(s, type, v, dimDph + 1, beginPos + i * step, beginPos + (i + 1) * step - 1);
        if (i != v.dims[dimDph] - 1) s << ",";
    }

    s << "]";
}
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

    recursive_init(s, type, arr_init, 0, 0, step - 1);

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
        if (cp != args.end()) s << ",";
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
        if (cp != arg_types.end()) s << ",";
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
        if (i != arg_num - 1) s << ",";
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

ostream& operator<<(std::ostream& s, LLVMIR::Operand* op)
{
    s << op->getName();
    return s;
}