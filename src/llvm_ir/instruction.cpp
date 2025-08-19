#include <cstddef>
#include <llvm_ir/instruction.h>
#include <llvm_ir/ir_block.h>
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

// Followed Get functions
// RegOp
int    RegOperand::GetRegNum() const { return reg_num; }
string RegOperand::GetGlobal() const { return ""; }
int    RegOperand::GetImm() const { return -1; }
float  RegOperand::GetImmF() const { return -1.0f; }
// ImmeI32Op
int    ImmeI32Operand::GetRegNum() const { return -1; }
string ImmeI32Operand::GetGlobal() const { return ""; }
int    ImmeI32Operand::GetImm() const { return value; }
float  ImmeI32Operand::GetImmF() const { return -1.0f; }
// ImmeF32Op
int    ImmeF32Operand::GetRegNum() const { return -1; }
string ImmeF32Operand::GetGlobal() const { return ""; }
int    ImmeF32Operand::GetImm() const { return -1; }
float  ImmeF32Operand::GetImmF() const { return value; }
// LabelOp
string LabelOperand::GetGlobal() const { return ""; }
int    LabelOperand::GetRegNum() const { return -1; }
int    LabelOperand::GetImm() const { return -1; }
float  LabelOperand::GetImmF() const { return -1.0f; }
// GlobalOp
string GlobalOperand::GetGlobal() const { return global_name; }
int    GlobalOperand::GetRegNum() const { return -1; }
int    GlobalOperand::GetImm() const { return -1; }
float  GlobalOperand::GetImmF() const { return -1.0f; }
/*
    Followed insts
 */
Instruction::Instruction(IROpCode op) : opcode(op) {}

LoadInst::LoadInst(DataType t, Operand* p, Operand* r) : Instruction(IROpCode::LOAD), type(t), ptr(p), res(r) {}
void LoadInst::printIR(ostream& s) { s << res << " = load " << type << ", ptr " << ptr << "\n"; }

int LoadInst::GetResultReg()
{
    if (this->res->type != OperandType::REG) { return -1; }  // 不是寄存器
    return ((RegOperand*)(this->res))->reg_num;
}

std::vector<int> LoadInst::GetUsedRegs()
{
    std::vector<int> ret{};
    if (this->ptr->type != OperandType::REG) { return ret; }
    ret.push_back(((RegOperand*)(this->ptr))->reg_num);
    return ret;
}

std::vector<Operand*> LoadInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    if (this->ptr) { ret.push_back(this->ptr); }
    return ret;
}

Operand* LoadInst::GetResultOperand() { return this->res; }

void LoadInst::Rename(std::map<int, int>& replace)
{
    if (this->ptr->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->ptr))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            ptr = getRegOperand(replace[reg]);
        }
    }

    if (this->res->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            res = getRegOperand(replace[reg]);
        }
    }
}

StoreInst::StoreInst(DataType t, Operand* p, Operand* r) : Instruction(IROpCode::STORE), type(t), ptr(p), val(r) {}
void StoreInst::printIR(ostream& s) { s << "store " << type << " " << val << ", ptr " << ptr << "\n"; }

int StoreInst::GetResultReg() { return -1; }

std::vector<int> StoreInst::GetUsedRegs()
{
    std::vector<int> ret{};
    if (this->ptr->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->ptr))->reg_num); }
    if (this->val->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->val))->reg_num); }
    return ret;
}

std::vector<Operand*> StoreInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    if (this->ptr) { ret.push_back(this->ptr); }
    if (this->val) { ret.push_back(this->val); }
    return ret;
}
Operand* StoreInst::GetResultOperand() { return nullptr; }

void StoreInst::Rename(std::map<int, int>& replace)
{
    if (this->ptr->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->ptr))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            ptr = getRegOperand(replace[reg]);
        }
    }

    if (this->val->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->val))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            val = getRegOperand(replace[reg]);
        }
    }
}

ArithmeticInst::ArithmeticInst(IROpCode op, DataType t, Operand* l, Operand* r, Operand* res)
    : Instruction(op), type(t), lhs(l), rhs(r), res(res)
{}
void ArithmeticInst::printIR(ostream& s)
{
    if (opcode == IROpCode::SMIN_I32)
    {
        s << res << " = call i32 @llvm.smin.i32(i32 " << lhs << ", i32 " << rhs << ")\n";
    }
    else if (opcode == IROpCode::SMAX_I32)
    {
        s << res << " = call i32 @llvm.smax.i32(i32 " << lhs << ", i32 " << rhs << ")\n";
    }
    else if (opcode == IROpCode::UMIN_I32)
    {
        s << res << " = call i32 @llvm.umin.i32(i32 " << lhs << ", i32 " << rhs << ")\n";
    }
    else if (opcode == IROpCode::UMAX_I32)
    {
        s << res << " = call i32 @llvm.umax.i32(i32 " << lhs << ", i32 " << rhs << ")\n";
    }
    else if (opcode == IROpCode::FMIN_F32)
    {
        s << res << " = call float @llvm.minnum.f32(float " << lhs << ", float " << rhs << ")\n";
    }
    else if (opcode == IROpCode::FMAX_F32)
    {
        s << res << " = call float @llvm.maxnum.f32(float " << lhs << ", float " << rhs << ")\n";
    }
    else { s << res << " = " << opcode << " " << type << " " << lhs << ", " << rhs << "\n"; }
}

int ArithmeticInst::GetResultReg()
{
    if (this->res->type != OperandType::REG) { return -1; }  // 不是寄存器
    return ((RegOperand*)(this->res))->reg_num;
}

std::vector<int> ArithmeticInst::GetUsedRegs()
{
    std::vector<int> ret{};
    if (this->lhs->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->lhs))->reg_num); }
    if (this->rhs->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->rhs))->reg_num); }
    return ret;
}

std::vector<Operand*> ArithmeticInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    if (this->lhs) { ret.push_back(this->lhs); }
    if (this->rhs) { ret.push_back(this->rhs); }
    return ret;
}

Operand* ArithmeticInst::GetResultOperand() { return this->res; }

void ArithmeticInst::Rename(std::map<int, int>& replace)
{
    // if(this->GetResultReg()==33){
    //     std::cout<<"11111111111111111"<<std::endl;
    //     if(this->lhs->type==OperandType::REG){std::cout<<((RegOperand*)(this->lhs))->reg_num<<std::endl;}
    // }

    if (this->lhs->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->lhs))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            lhs = getRegOperand(replace[reg]);
        }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = getRegOperand(replace[reg]); }
    }
}

IcmpInst::IcmpInst(DataType t, IcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::ICMP), type(t), cond(c), lhs(l), rhs(r), res(res)
{}
void IcmpInst::printIR(ostream& s)
{
    s << res << " = icmp " << cond << " " << type << " " << lhs << ", " << rhs << "\n";
}

int IcmpInst::GetResultReg()
{
    if (this->res->type != OperandType::REG) { return -1; }  // 不是寄存器
    return ((RegOperand*)(this->res))->reg_num;
}

std::vector<int> IcmpInst::GetUsedRegs()
{
    std::vector<int> ret{};
    if (this->lhs->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->lhs))->reg_num); }
    if (this->rhs->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->rhs))->reg_num); }
    return ret;
}

std::vector<Operand*> IcmpInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    if (this->lhs) { ret.push_back(this->lhs); }
    if (this->rhs) { ret.push_back(this->rhs); }
    return ret;
}

Operand* IcmpInst::GetResultOperand() { return this->res; }

void IcmpInst::Rename(std::map<int, int>& replace)
{
    if (this->lhs->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->lhs))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            lhs = getRegOperand(replace[reg]);
        }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = getRegOperand(replace[reg]); }
    }
}

FcmpInst::FcmpInst(DataType t, FcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::FCMP), type(t), cond(c), lhs(l), rhs(r), res(res)
{}
void FcmpInst::printIR(ostream& s)
{
    s << res << " = fcmp " << cond << " " << type << " " << lhs << ", " << rhs << "\n";
}
int FcmpInst::GetResultReg()
{
    if (this->res->type != OperandType::REG) { return -1; }  // 不是寄存器
    return ((RegOperand*)(this->res))->reg_num;
}

std::vector<int> FcmpInst::GetUsedRegs()
{
    std::vector<int> ret{};
    if (this->lhs->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->lhs))->reg_num); }
    if (this->rhs->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->rhs))->reg_num); }
    return ret;
}

std::vector<Operand*> FcmpInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    if (this->lhs) { ret.push_back(this->lhs); }
    if (this->rhs) { ret.push_back(this->rhs); }
    return ret;
}

Operand* FcmpInst::GetResultOperand() { return this->res; }

void FcmpInst::Rename(std::map<int, int>& replace)
{
    if (this->lhs->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->lhs))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            lhs = getRegOperand(replace[reg]);
        }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = getRegOperand(replace[reg]); }
    }
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

int AllocInst::GetResultReg()
{
    if (res->type != OperandType::REG) { return -1; }
    else { return ((RegOperand*)res)->reg_num; }
}

std::vector<int> AllocInst::GetUsedRegs()
{
    std::vector<int> ret{};
    return ret;
}

std::vector<Operand*> AllocInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    return ret;
}

Operand* AllocInst::GetResultOperand() { return this->res; }

void AllocInst::Rename(std::map<int, int>& replace) {}

BranchCondInst::BranchCondInst(Operand* c, Operand* t, Operand* f)
    : Instruction(IROpCode::BR_COND), cond(c), true_label(t), false_label(f)
{}
void BranchCondInst::printIR(ostream& s)
{
    s << "br i1 " << cond << ", label " << true_label << ", label " << false_label << "\n";
}

int BranchCondInst::GetResultReg() { return -1; }

std::vector<int> BranchCondInst::GetUsedRegs()
{
    std::vector<int> ret{};
    if (this->cond->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->cond))->reg_num); }
    return ret;
}

std::vector<Operand*> BranchCondInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    if (this->cond) { ret.push_back(this->cond); }
    if (this->true_label) { ret.push_back(this->true_label); }
    if (this->false_label) { ret.push_back(this->false_label); }
    return ret;
}

Operand* BranchCondInst::GetResultOperand() { return nullptr; }

void BranchCondInst::Rename(std::map<int, int>& replace)
{  // 其实这里是cmp出来的寄存器，不会被mem2reg波及
    if (this->cond->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->cond))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            cond = getRegOperand(replace[reg]);
        }
    }
}

BranchUncondInst::BranchUncondInst(Operand* t) : Instruction(IROpCode::BR_UNCOND), target_label(t) {}
void BranchUncondInst::printIR(ostream& s) { s << "br label " << target_label << "\n"; }
int  BranchUncondInst::GetResultReg() { return -1; }

std::vector<int> BranchUncondInst::GetUsedRegs()
{
    std::vector<int> ret{};
    return ret;
}

std::vector<Operand*> BranchUncondInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    if (this->target_label) { ret.push_back(this->target_label); }
    return ret;
}
Operand* BranchUncondInst::GetResultOperand() { return nullptr; }

void BranchUncondInst::Rename(std::map<int, int>& replace) {}

GlbvarDefInst::GlbvarDefInst(DataType t, string n, Operand* v)
    : Instruction(IROpCode::GLOBAL_VAR), type(t), name(n), init(v)
{
    block_id = -1;
}
GlbvarDefInst::GlbvarDefInst(DataType t, string n, VarAttribute a)
    : Instruction(IROpCode::GLOBAL_VAR), type(t), name(n), arr_init(a)
{
    block_id = -1;
}
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
int GlbvarDefInst::GetResultReg() { return -1; }

std::vector<int> GlbvarDefInst::GetUsedRegs()
{
    std::vector<int> ret{};
    return ret;
}

std::vector<Operand*> GlbvarDefInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    return ret;
}

Operand* GlbvarDefInst::GetResultOperand() { return nullptr; }

void GlbvarDefInst::Rename(std::map<int, int>& replace) {}

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
int CallInst::GetResultReg()
{
    if (this->res == nullptr) { return -1; }
    if (this->res->type != OperandType::REG) { return -1; }
    return ((RegOperand*)res)->reg_num;
}

std::vector<int> CallInst::GetUsedRegs()
{
    std::vector<int> ret{};
    for (auto iter : args)
    {
        auto op = iter.second;
        if (op->type == OperandType::REG) { ret.push_back(((RegOperand*)op)->reg_num); }
    }
    return ret;
}

std::vector<Operand*> CallInst::GetUsedOperands()
{
    std::vector<Operand*> ret{};
    for (auto iter : args) { ret.push_back(iter.second); }
    return ret;
}

Operand* CallInst::GetResultOperand() { return this->res; }

void CallInst::Rename(std::map<int, int>& replace)
{
    for (auto& iter : args)
    {
        auto op = iter.second;
        if (op->type == OperandType::REG)
        {
            int regno = ((RegOperand*)op)->reg_num;
            if (replace.find(regno) != replace.end()) { iter.second = getRegOperand(replace[regno]); }
        }
    }
}

RetInst::RetInst(DataType t, Operand* r) : Instruction(IROpCode::RET), ret_type(t), ret(r) {}
void RetInst::printIR(ostream& s)
{
    s << "ret " << ret_type;
    if (ret) s << " " << ret;
    s << "\n";
}

int RetInst::GetResultReg() { return -1; }

std::vector<int> RetInst::GetUsedRegs()
{
    std::vector<int> retvec{};

    if (ret != nullptr && ret->type == OperandType::REG) { retvec.push_back(((RegOperand*)ret)->reg_num); }

    return retvec;
}

std::vector<Operand*> RetInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    if (ret) { retvec.push_back(ret); }
    return retvec;
}

Operand* RetInst::GetResultOperand() { return nullptr; }

void RetInst::Rename(std::map<int, int>& replace)
{
    if (ret != nullptr && ret->type == OperandType::REG)
    {
        int regno = ((RegOperand*)ret)->reg_num;
        if (replace.find(regno) != replace.end()) ret = getRegOperand(replace[regno]);
    }
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

int GEPInst::GetResultReg()
{
    if (this->res->type != OperandType::REG) { return -1; }
    return ((RegOperand*)res)->reg_num;
}

std::vector<int> GEPInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    if (base_ptr->type == OperandType::REG) { retvec.push_back(((RegOperand*)base_ptr)->reg_num); }
    for (auto iter : idxs)
    {
        if (iter->type == OperandType::REG) { retvec.push_back(((RegOperand*)iter)->reg_num); }
    }
    return retvec;
}

std::vector<Operand*> GEPInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    if (base_ptr) { retvec.push_back(base_ptr); }
    for (auto& iter : idxs) { retvec.push_back(iter); }
    return retvec;
}

Operand* GEPInst::GetResultOperand() { return this->res; }

void GEPInst::Rename(std::map<int, int>& replace)
{
    if (base_ptr->type == OperandType::REG)
    {
        int regno = ((RegOperand*)base_ptr)->reg_num;
        if (replace.find(regno) != replace.end()) { base_ptr = getRegOperand(replace[regno]); }
    }
    for (auto& iter : idxs)
    {
        if (iter->type == OperandType::REG)
        {
            // std::cout<<"'!!!!!!!!!!!!!!!!!!!!!!!!!!!'"<<std::endl;
            int regno = ((RegOperand*)iter)->reg_num;
            if (replace.find(regno) != replace.end()) { iter = getRegOperand(replace[regno]); }
        }
    }
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

int FuncDeclareInst::GetResultReg() { return -1; }

std::vector<int> FuncDeclareInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    return retvec;
}

std::vector<Operand*> FuncDeclareInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    return retvec;
}

Operand* FuncDeclareInst::GetResultOperand() { return nullptr; }

void FuncDeclareInst::Rename(std::map<int, int>& replace) {}

FuncDefInst::FuncDefInst(DataType rt, string fn, vector<DataType> at) : FuncDeclareInst(rt, fn, at), arg_regs({})
{
    // 设置函数定义时的block_id为-1，表示不在所有block内
    block_id = -1;
}
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

int FuncDefInst::GetResultReg() { return -1; }

std::vector<int> FuncDefInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    return retvec;
}

std::vector<Operand*> FuncDefInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    for (auto& op : arg_regs) { retvec.push_back(op); }
    return retvec;
}

Operand* FuncDefInst::GetResultOperand() { return nullptr; }

void FuncDefInst::Rename(std::map<int, int>& replace) {}

SI2FPInst::SI2FPInst(Operand* f, Operand* t) : Instruction(IROpCode::SITOFP), f_si(f), t_fp(t) {}
void SI2FPInst::printIR(ostream& s) { s << t_fp << " = sitofp i32 " << f_si << " to float\n"; }
int  SI2FPInst::GetResultReg()
{
    if (this->t_fp->type != OperandType::REG) { return -1; }
    return ((RegOperand*)t_fp)->reg_num;
}

std::vector<int> SI2FPInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    if (f_si->type == OperandType::REG) { retvec.push_back(((RegOperand*)f_si)->reg_num); }
    return retvec;
}

std::vector<Operand*> SI2FPInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    if (f_si) { retvec.push_back(f_si); }
    return retvec;
}

Operand* SI2FPInst::GetResultOperand() { return this->t_fp; }

void SI2FPInst::Rename(std::map<int, int>& replace)
{
    if (f_si->type == OperandType::REG)
    {
        int regno = ((RegOperand*)f_si)->reg_num;
        if (replace.find(regno) != replace.end()) { f_si = getRegOperand(replace[regno]); }
    }
}

FP2SIInst::FP2SIInst(Operand* f, Operand* t) : Instruction(IROpCode::FPTOSI), f_fp(f), t_si(t) {}
void FP2SIInst::printIR(ostream& s) { s << t_si << " = fptosi float " << f_fp << " to i32\n"; }
int  FP2SIInst::GetResultReg()
{
    if (this->t_si->type != OperandType::REG) { return -1; }
    return ((RegOperand*)t_si)->reg_num;
}

std::vector<int> FP2SIInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    if (f_fp->type == OperandType::REG) { retvec.push_back(((RegOperand*)f_fp)->reg_num); }
    return retvec;
}

std::vector<Operand*> FP2SIInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    if (f_fp) { retvec.push_back(f_fp); }
    return retvec;
}

Operand* FP2SIInst::GetResultOperand() { return this->t_si; }

void FP2SIInst::Rename(std::map<int, int>& replace)
{
    if (f_fp->type == OperandType::REG)
    {
        int regno = ((RegOperand*)f_fp)->reg_num;
        if (replace.find(regno) != replace.end()) { f_fp = getRegOperand(replace[regno]); }
    }
}

ZextInst::ZextInst(DataType f, DataType t, Operand* s, Operand* d)
    : Instruction(IROpCode::ZEXT), from(f), to(t), src(s), dest(d)
{}
void ZextInst::printIR(ostream& s) { s << dest << " = zext " << from << " " << src << " to " << to << "\n"; }

int ZextInst::GetResultReg()
{
    if (this->dest->type != OperandType::REG) { return -1; }
    return ((RegOperand*)dest)->reg_num;
}

std::vector<int> ZextInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    if (src->type == OperandType::REG) { retvec.push_back(((RegOperand*)src)->reg_num); }
    return retvec;
}

std::vector<Operand*> ZextInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    if (src) { retvec.push_back(src); }
    return retvec;
}

Operand* ZextInst::GetResultOperand() { return this->dest; }

void ZextInst::Rename(std::map<int, int>& replace)
{
    if (src->type == OperandType::REG)
    {
        int regno = ((RegOperand*)src)->reg_num;
        if (replace.find(regno) != replace.end()) { src = getRegOperand(replace[regno]); }
    }
}

FPExtInst::FPExtInst(Operand* s, Operand* d) : Instruction(IROpCode::FPEXT), src(s), dest(d) {}
void FPExtInst::printIR(ostream& s) { s << dest << " = fpext float" << " " << src << " to " << "double" << "\n"; }
int  FPExtInst::GetResultReg()
{
    if (this->dest->type != OperandType::REG) { return -1; }
    return ((RegOperand*)dest)->reg_num;
}

std::vector<int> FPExtInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    if (src->type == OperandType::REG) { retvec.push_back(((RegOperand*)src)->reg_num); }
    return retvec;
}

std::vector<Operand*> FPExtInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    if (src) { retvec.push_back(src); }
    return retvec;
}

Operand* FPExtInst::GetResultOperand() { return this->dest; }

void FPExtInst::Rename(std::map<int, int>& replace)
{
    if (src->type == OperandType::REG)
    {
        int regno = ((RegOperand*)src)->reg_num;
        if (replace.find(regno) != replace.end()) { src = getRegOperand(replace[regno]); }
    }
}

PhiInst::PhiInst(DataType t, Operand* r, const std::vector<std::pair<Operand*, Operand*>>* vfl)
    : Instruction(IROpCode::PHI), type(t), res(r), vals_for_labels({})
{
    if (vfl) vals_for_labels = *vfl;
}
void PhiInst::printIR(ostream& s)
{
    s << res << " = phi " << type << " ";
    auto it = vals_for_labels.begin();
    auto cp = it;
    for (; it != vals_for_labels.end(); ++it)
    {
        s << "[ " << it->first << ", " << it->second << " ]";
        ++cp;
        if (cp != vals_for_labels.end()) s << ", ";
    }
    s << "\n";
}
int PhiInst::GetResultReg()
{
    if (this->res->type != OperandType::REG) { return -1; }
    return ((RegOperand*)res)->reg_num;
}

std::vector<int> PhiInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    for (auto iter : vals_for_labels)
    {
        if (iter.first->type == OperandType::REG) { retvec.push_back(((RegOperand*)iter.first)->reg_num); }
    }
    return retvec;
}

std::vector<Operand*> PhiInst::GetUsedOperands()
{
    std::vector<Operand*> retvec{};
    for (auto iter : vals_for_labels)
    {
        retvec.push_back(iter.first);
        retvec.push_back(iter.second);
    }
    return retvec;
}

Operand* PhiInst::GetResultOperand() { return this->res; }

void PhiInst::Rename(std::map<int, int>& replace)
{
    for (auto& iter : vals_for_labels)
    {
        if (iter.first->type == OperandType::REG)
        {
            int regno = ((RegOperand*)iter.first)->reg_num;
            if (replace.find(regno) != replace.end()) { iter.first = getRegOperand(replace[regno]); }
        }
    }
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

ImmeI32Operand* ImmeI32Operand::get(int value) { return new ImmeI32Operand(value); }
ImmeI32Operand* getImmeI32Operand(int num)
{
    auto it = ImmeI32OperandMap.find(num);
    if (it == ImmeI32OperandMap.end())
    {
        ImmeI32Operand* op     = ImmeI32Operand::get(num);
        ImmeI32OperandMap[num] = op;
        return op;
    }
    return it->second;
}

ImmeF32Operand* ImmeF32Operand::get(float value) { return new ImmeF32Operand(value); }
ImmeF32Operand* getImmeF32Operand(float num)
{
    auto it = ImmeF32OperandMap.find(num);
    if (it == ImmeF32OperandMap.end())
    {
        ImmeF32Operand* op     = ImmeF32Operand::get(num);
        ImmeF32OperandMap[num] = op;
        return op;
    }
    return it->second;
}

RegOperand* RegOperand::get(int num) { return new RegOperand(num); }
RegOperand* getRegOperand(int num)
{
    auto it = RegOperandMap.find(num);
    if (it == RegOperandMap.end())
    {
        RegOperand* op     = RegOperand::get(num);
        RegOperandMap[num] = op;
        return op;
    }
    return it->second;
}

LabelOperand* LabelOperand::get(int num) { return new LabelOperand(num); }
LabelOperand* getLabelOperand(int num)
{
    auto it = LabelOperandMap.find(num);
    if (it == LabelOperandMap.end())
    {
        LabelOperand* op     = LabelOperand::get(num);
        LabelOperandMap[num] = op;
        return op;
    }
    return it->second;
}

GlobalOperand* GlobalOperand::get(string name) { return new GlobalOperand(name); }
GlobalOperand* getGlobalOperand(string name)
{
    auto it = GlobalOperandMap.find(name);
    if (it == GlobalOperandMap.end())
    {
        GlobalOperand* op      = GlobalOperand::get(name);
        GlobalOperandMap[name] = op;
        return op;
    }
    return it->second;
}

Instruction* LoadInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : res;
    return new LoadInst(type, ptr, result_op);
}

Instruction* StoreInst::Clone(int new_result_reg) const { return new StoreInst(type, ptr, val); }

Instruction* ArithmeticInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : res;
    return new ArithmeticInst(opcode, type, lhs, rhs, result_op);
}

Instruction* IcmpInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : res;
    return new IcmpInst(type, cond, lhs, rhs, result_op);
}

Instruction* FcmpInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : res;
    return new FcmpInst(type, cond, lhs, rhs, result_op);
}

Instruction* AllocInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : res;
    return new AllocInst(type, result_op, dims);
}

Instruction* BranchCondInst::Clone(int new_result_reg) const
{
    return new BranchCondInst(cond, true_label, false_label);
}

Instruction* BranchUncondInst::Clone(int new_result_reg) const { return new BranchUncondInst(target_label); }

Instruction* GlbvarDefInst::Clone(int new_result_reg) const
{
    if (arr_init.dims.empty())
        return new GlbvarDefInst(type, name, init);
    else
        return new GlbvarDefInst(type, name, arr_init);
}

Instruction* CallInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : res;
    return new CallInst(ret_type, func_name, args, result_op);
}

Instruction* RetInst::Clone(int new_result_reg) const { return new RetInst(ret_type, ret); }

Instruction* GEPInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : res;
    return new GEPInst(type, idx_type, base_ptr, result_op, dims, idxs);
}

Instruction* FuncDeclareInst::Clone(int new_result_reg) const
{
    return new FuncDeclareInst(ret_type, func_name, arg_types);
}

Instruction* FuncDefInst::Clone(int new_result_reg) const
{
    auto* cloned     = new FuncDefInst(ret_type, func_name, arg_types);
    cloned->arg_regs = arg_regs;
    return cloned;
}

Instruction* SI2FPInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : t_fp;
    return new SI2FPInst(f_si, result_op);
}

Instruction* FP2SIInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : t_si;
    return new FP2SIInst(f_fp, result_op);
}

Instruction* ZextInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : dest;
    return new ZextInst(from, to, src, result_op);
}

Instruction* FPExtInst::Clone(int new_result_reg) const
{
    auto* result_op = (new_result_reg != -1) ? getRegOperand(new_result_reg) : dest;
    return new FPExtInst(src, result_op);
}

Instruction* PhiInst::Clone(int new_result_reg) const
{
    auto* result_op         = (new_result_reg != -1) ? getRegOperand(new_result_reg) : res;
    auto* cloned            = new PhiInst(type, result_op);
    cloned->vals_for_labels = vals_for_labels;
    return cloned;
}

// SubstituteOperands implementations for all instruction classes
bool LoadInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (ptr && ptr->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(ptr);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            ptr = it->second;
            return true;
        }
    }
    return false;
}

bool StoreInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    bool has_replacement = false;

    if (ptr && ptr->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(ptr);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            ptr             = it->second;
            has_replacement = true;
        }
    }
    if (val && val->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(val);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            val             = it->second;
            has_replacement = true;
        }
    }
    return has_replacement;
}

bool ArithmeticInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    bool has_replacement = false;

    if (lhs && lhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(lhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            lhs             = it->second;
            has_replacement = true;
        }
    }
    if (rhs && rhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(rhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            rhs             = it->second;
            has_replacement = true;
        }
    }
    return has_replacement;
}

bool IcmpInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    bool has_replacement = false;

    if (lhs && lhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(lhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            lhs             = it->second;
            has_replacement = true;
        }
    }
    if (rhs && rhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(rhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            rhs             = it->second;
            has_replacement = true;
        }
    }
    return has_replacement;
}

bool FcmpInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    bool has_replacement = false;

    if (lhs && lhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(lhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            lhs             = it->second;
            has_replacement = true;
        }
    }
    if (rhs && rhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(rhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            rhs             = it->second;
            has_replacement = true;
        }
    }
    return has_replacement;
}

bool AllocInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) { return false; }

bool BranchCondInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (cond && cond->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(cond);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            cond = it->second;
            return true;
        }
    }
    return false;
}

bool BranchUncondInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) { return false; }

bool GlbvarDefInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (init && init->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(init);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            init = it->second;
            return true;
        }
    }
    return false;
}

bool CallInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    bool has_replacement = false;

    for (auto& arg : args)
    {
        if (arg.second && arg.second->type == OperandType::REG)
        {
            auto* reg_op = dynamic_cast<RegOperand*>(arg.second);
            auto  it     = substitutions.find(reg_op->reg_num);
            if (it != substitutions.end())
            {
                arg.second      = it->second;
                has_replacement = true;
            }
        }
    }

    return has_replacement;
}

bool RetInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (ret && ret->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(ret);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            ret = it->second;
            return true;
        }
    }
    return false;
}

bool GEPInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    bool has_replacement = false;

    if (base_ptr && base_ptr->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(base_ptr);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            base_ptr        = it->second;
            has_replacement = true;
        }
    }
    for (auto& idx : idxs)
    {
        if (idx && idx->type == OperandType::REG)
        {
            auto* reg_op = dynamic_cast<RegOperand*>(idx);
            auto  it     = substitutions.find(reg_op->reg_num);
            if (it != substitutions.end())
            {
                idx             = it->second;
                has_replacement = true;
            }
        }
    }
    return has_replacement;
}

bool FuncDeclareInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) { return false; }

bool FuncDefInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) { return false; }

bool SI2FPInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (f_si && f_si->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(f_si);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            f_si = it->second;
            return true;
        }
    }
    return false;
}

bool FP2SIInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (f_fp && f_fp->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(f_fp);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            f_fp = it->second;
            return true;
        }
    }
    return false;
}

bool ZextInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (src && src->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(src);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            src = it->second;
            return true;
        }
    }
    return false;
}

bool FPExtInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (src && src->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(src);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end())
        {
            src = it->second;
            return true;
        }
    }
    return false;
}

bool PhiInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    bool has_replacement = false;

    for (auto& val_label : vals_for_labels)
    {
        if (val_label.first && val_label.first->type == OperandType::REG)
        {
            auto* reg_op = dynamic_cast<RegOperand*>(val_label.first);
            auto  it     = substitutions.find(reg_op->reg_num);
            if (it != substitutions.end())
            {
                val_label.first = it->second;
                has_replacement = true;
            }
        }
    }

    return has_replacement;
}

DataType LoadInst::GetResultType() const { return type; }
DataType StoreInst::GetResultType() const { return DataType::VOID; }
DataType ArithmeticInst::GetResultType() const { return type; }
DataType IcmpInst::GetResultType() const { return DataType::I1; }
DataType FcmpInst::GetResultType() const { return DataType::I1; }
DataType AllocInst::GetResultType() const { return DataType::PTR; }
DataType BranchCondInst::GetResultType() const { return DataType::VOID; }
DataType BranchUncondInst::GetResultType() const { return DataType::VOID; }
DataType GlbvarDefInst::GetResultType() const { return DataType::VOID; }
DataType CallInst::GetResultType() const { return ret_type; }
DataType RetInst::GetResultType() const { return DataType::VOID; }
DataType GEPInst::GetResultType() const { return DataType::PTR; }
DataType FuncDeclareInst::GetResultType() const { return DataType::VOID; }
DataType FuncDefInst::GetResultType() const { return DataType::VOID; }
DataType SI2FPInst::GetResultType() const { return DataType::F32; }
DataType FP2SIInst::GetResultType() const { return DataType::I32; }
DataType ZextInst::GetResultType() const { return to; }
DataType FPExtInst::GetResultType() const { return DataType::DOUBLE; }
DataType PhiInst::GetResultType() const { return type; }

LLVMIR::Operand* copyOperand(
    LLVMIR::Operand* operand, const std::map<int, int>& reg_map, const std::map<int, int>& label_map)
{
    if (!operand) return nullptr;

    switch (operand->type)
    {
        case LLVMIR::OperandType::REG:
        {
            auto reg_op = static_cast<LLVMIR::RegOperand*>(operand);
            auto it     = reg_map.find(reg_op->reg_num);
            if (it != reg_map.end()) { return getRegOperand(it->second); }
            return getRegOperand(reg_op->reg_num);
        }
        case LLVMIR::OperandType::IMMEI32:
        {
            auto imme_op = static_cast<LLVMIR::ImmeI32Operand*>(operand);
            return getImmeI32Operand(imme_op->value);
        }
        case LLVMIR::OperandType::IMMEF32:
        {
            auto imme_op = static_cast<LLVMIR::ImmeF32Operand*>(operand);
            return getImmeF32Operand(imme_op->value);
        }
        case LLVMIR::OperandType::LABEL:
        {
            auto label_op = static_cast<LLVMIR::LabelOperand*>(operand);
            auto it       = label_map.find(label_op->label_num);
            if (it != label_map.end()) { return getLabelOperand(it->second); }
            return getLabelOperand(label_op->label_num);
        }
        case LLVMIR::OperandType::GLOBAL:
        {
            auto global_op = static_cast<LLVMIR::GlobalOperand*>(operand);
            return getGlobalOperand(global_op->global_name);
        }
        default: return nullptr;
    }
}

Instruction* LoadInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new LoadInst(type, copyOperand(ptr, reg_map, label_map), copyOperand(res, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* StoreInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst = new StoreInst(type, copyOperand(ptr, reg_map, label_map), copyOperand(val, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* ArithmeticInst::CloneWithMapping(
    const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new ArithmeticInst(opcode,
        type,
        copyOperand(lhs, reg_map, label_map),
        copyOperand(rhs, reg_map, label_map),
        copyOperand(res, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* IcmpInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new IcmpInst(type,
        cond,
        copyOperand(lhs, reg_map, label_map),
        copyOperand(rhs, reg_map, label_map),
        copyOperand(res, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* FcmpInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new FcmpInst(type,
        cond,
        copyOperand(lhs, reg_map, label_map),
        copyOperand(rhs, reg_map, label_map),
        copyOperand(res, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* AllocInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new AllocInst(type, copyOperand(res, reg_map, label_map), dims);
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* BranchCondInst::CloneWithMapping(
    const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new BranchCondInst(copyOperand(cond, reg_map, label_map),
        copyOperand(true_label, reg_map, label_map),
        copyOperand(false_label, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* BranchUncondInst::CloneWithMapping(
    const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new BranchUncondInst(copyOperand(target_label, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* GlbvarDefInst::CloneWithMapping(
    const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new GlbvarDefInst(type, name, copyOperand(init, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* CallInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    std::vector<std::pair<DataType, Operand*>> new_args;
    for (const auto& arg : args) { new_args.push_back({arg.first, copyOperand(arg.second, reg_map, label_map)}); }
    auto* new_inst     = new CallInst(ret_type, func_name, new_args, copyOperand(res, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* RetInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new RetInst(ret_type, copyOperand(ret, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* GEPInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    std::vector<Operand*> new_idxs;
    for (auto idx : idxs) { new_idxs.push_back(copyOperand(idx, reg_map, label_map)); }
    auto* new_inst     = new GEPInst(type,
        idx_type,
        copyOperand(base_ptr, reg_map, label_map),
        copyOperand(res, reg_map, label_map),
        dims,
        new_idxs);
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* FuncDeclareInst::CloneWithMapping(
    const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new FuncDeclareInst(ret_type, func_name, arg_types);
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* FuncDefInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst = new FuncDefInst(ret_type, func_name, arg_types);
    for (auto arg_reg : arg_regs) { new_inst->arg_regs.push_back(copyOperand(arg_reg, reg_map, label_map)); }
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* SI2FPInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new SI2FPInst(copyOperand(f_si, reg_map, label_map), copyOperand(t_fp, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* FP2SIInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new FP2SIInst(copyOperand(f_fp, reg_map, label_map), copyOperand(t_si, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* ZextInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst =
        new ZextInst(from, to, copyOperand(src, reg_map, label_map), copyOperand(dest, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* FPExtInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    auto* new_inst     = new FPExtInst(copyOperand(src, reg_map, label_map), copyOperand(dest, reg_map, label_map));
    new_inst->block_id = block_id;
    return new_inst;
}

Instruction* PhiInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    std::vector<std::pair<Operand*, Operand*>> new_vals;
    for (const auto& val_label : vals_for_labels)
    {
        new_vals.push_back(
            {copyOperand(val_label.first, reg_map, label_map), copyOperand(val_label.second, reg_map, label_map)});
    }
    auto* new_inst     = new PhiInst(type, copyOperand(res, reg_map, label_map), &new_vals);
    new_inst->block_id = block_id;
    return new_inst;
}

std::vector<Operand*> LoadInst::GetCSEOperands() const { return {ptr}; }

std::vector<Operand*> StoreInst::GetCSEOperands() const
{
    return {};  // Store指令不参与CSE
}

std::vector<Operand*> ArithmeticInst::GetCSEOperands() const { return {lhs, rhs}; }

std::vector<Operand*> IcmpInst::GetCSEOperands() const { return {lhs, rhs}; }

std::vector<Operand*> FcmpInst::GetCSEOperands() const { return {lhs, rhs}; }

std::vector<Operand*> AllocInst::GetCSEOperands() const
{
    return {};  // Alloca指令不参与CSE
}

std::vector<Operand*> BranchCondInst::GetCSEOperands() const
{
    return {};  // 分支指令不参与CSE
}

std::vector<Operand*> BranchUncondInst::GetCSEOperands() const
{
    return {};  // 分支指令不参与CSE
}

std::vector<Operand*> GlbvarDefInst::GetCSEOperands() const
{
    return {};  // 全局变量定义不参与CSE
}

std::vector<Operand*> CallInst::GetCSEOperands() const
{
    std::vector<Operand*> operands;
    for (const auto& arg : args) { operands.push_back(arg.second); }
    return operands;
}

std::vector<Operand*> RetInst::GetCSEOperands() const
{
    return {};  // Return指令不参与CSE
}

std::vector<Operand*> GEPInst::GetCSEOperands() const
{
    std::vector<Operand*> operands;
    operands.push_back(base_ptr);
    for (auto idx : idxs) { operands.push_back(idx); }
    return operands;
}

std::vector<Operand*> FuncDeclareInst::GetCSEOperands() const
{
    return {};  // 函数声明不参与CSE
}

std::vector<Operand*> FuncDefInst::GetCSEOperands() const
{
    return {};  // 函数定义不参与CSE
}

std::vector<Operand*> SI2FPInst::GetCSEOperands() const { return {f_si}; }

std::vector<Operand*> FP2SIInst::GetCSEOperands() const { return {f_fp}; }

std::vector<Operand*> ZextInst::GetCSEOperands() const { return {src}; }

std::vector<Operand*> FPExtInst::GetCSEOperands() const { return {src}; }

std::vector<Operand*> PhiInst::GetCSEOperands() const
{
    return {};  // PHI指令不参与CSE
}

void LoadInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->ptr->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->ptr))->reg_num;
        if (replace.find(reg) != replace.end()) { ptr = getRegOperand(replace[reg]); }
    }

    if (this->res->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end()) { res = getRegOperand(replace[reg]); }
    }
}

void StoreInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->ptr->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->ptr))->reg_num;
        if (replace.find(reg) != replace.end()) { ptr = getRegOperand(replace[reg]); }
    }

    if (this->val->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->val))->reg_num;
        if (replace.find(reg) != replace.end()) { val = getRegOperand(replace[reg]); }
    }
}

void ArithmeticInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->lhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->lhs))->reg_num;
        if (replace.find(reg) != replace.end()) { lhs = getRegOperand(replace[reg]); }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = getRegOperand(replace[reg]); }
    }

    if (this->res->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end()) { res = getRegOperand(replace[reg]); }
    }
}

void IcmpInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->lhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->lhs))->reg_num;
        if (replace.find(reg) != replace.end()) { lhs = getRegOperand(replace[reg]); }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = getRegOperand(replace[reg]); }
    }

    if (this->res->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end()) { res = getRegOperand(replace[reg]); }
    }
}

void FcmpInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->lhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->lhs))->reg_num;
        if (replace.find(reg) != replace.end()) { lhs = getRegOperand(replace[reg]); }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = getRegOperand(replace[reg]); }
    }

    if (this->res->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end()) { res = getRegOperand(replace[reg]); }
    }
}

void AllocInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->res->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end()) { res = getRegOperand(replace[reg]); }
    }
}

void BranchCondInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->cond->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->cond))->reg_num;
        if (replace.find(reg) != replace.end()) { cond = getRegOperand(replace[reg]); }
    }
}

void BranchUncondInst::ReplaceAllOperands(std::map<int, int>& replace) {}

void GlbvarDefInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->init && this->init->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->init))->reg_num;
        if (replace.find(reg) != replace.end()) { init = getRegOperand(replace[reg]); }
    }
}

void CallInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    for (auto& arg : args)
    {
        if (arg.second && arg.second->type == OperandType::REG)
        {
            int reg = ((RegOperand*)(arg.second))->reg_num;
            if (replace.find(reg) != replace.end()) { arg.second = getRegOperand(replace[reg]); }
        }
    }

    if (this->res && this->res->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end()) { res = getRegOperand(replace[reg]); }
    }
}

void RetInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->ret && this->ret->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->ret))->reg_num;
        if (replace.find(reg) != replace.end()) { ret = getRegOperand(replace[reg]); }
    }
}

void GEPInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->base_ptr->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->base_ptr))->reg_num;
        if (replace.find(reg) != replace.end()) { base_ptr = getRegOperand(replace[reg]); }
    }

    for (auto& idx : idxs)
    {
        if (idx && idx->type == OperandType::REG)
        {
            int reg = ((RegOperand*)(idx))->reg_num;
            if (replace.find(reg) != replace.end()) { idx = getRegOperand(replace[reg]); }
        }
    }

    if (this->res->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end()) { res = getRegOperand(replace[reg]); }
    }
}

void FuncDeclareInst::ReplaceAllOperands(std::map<int, int>& replace) {}

void FuncDefInst::ReplaceAllOperands(std::map<int, int>& replace) {}

void SI2FPInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->f_si->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->f_si))->reg_num;
        if (replace.find(reg) != replace.end()) { f_si = getRegOperand(replace[reg]); }
    }

    if (this->t_fp->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->t_fp))->reg_num;
        if (replace.find(reg) != replace.end()) { t_fp = getRegOperand(replace[reg]); }
    }
}

void FP2SIInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->f_fp->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->f_fp))->reg_num;
        if (replace.find(reg) != replace.end()) { f_fp = getRegOperand(replace[reg]); }
    }

    if (this->t_si->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->t_si))->reg_num;
        if (replace.find(reg) != replace.end()) { t_si = getRegOperand(replace[reg]); }
    }
}

void ZextInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->src->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->src))->reg_num;
        if (replace.find(reg) != replace.end()) { src = getRegOperand(replace[reg]); }
    }

    if (this->dest->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->dest))->reg_num;
        if (replace.find(reg) != replace.end()) { dest = getRegOperand(replace[reg]); }
    }
}

void FPExtInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    if (this->src->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->src))->reg_num;
        if (replace.find(reg) != replace.end()) { src = getRegOperand(replace[reg]); }
    }

    if (this->dest->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->dest))->reg_num;
        if (replace.find(reg) != replace.end()) { dest = getRegOperand(replace[reg]); }
    }
}

void PhiInst::ReplaceAllOperands(std::map<int, int>& replace)
{
    for (auto& val_label : vals_for_labels)
    {
        if (val_label.first && val_label.first->type == OperandType::REG)
        {
            int reg = ((RegOperand*)(val_label.first))->reg_num;
            if (replace.find(reg) != replace.end()) { val_label.first = getRegOperand(replace[reg]); }
        }
    }

    if (this->res->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end()) { res = getRegOperand(replace[reg]); }
    }
}

void updateBlockRegisterMapping(IRBlock* block, const std::map<int, int>& reg_mapping)
{
    if (!block || reg_mapping.empty()) return;

    for (auto* inst : block->insts)
    {
        if (!inst) continue;

        std::map<int, int> mutable_mapping = reg_mapping;
        inst->ReplaceAllOperands(mutable_mapping);
    }
}

unsigned getInstructionCost(LLVMIR::Instruction* inst)
{
    if (!inst) return 0;

    switch (inst->opcode)
    {
        case LLVMIR::IROpCode::PHI: return 0;  // PHI节点在机器代码中不产生实际指令
        case LLVMIR::IROpCode::BR_UNCOND:
        case LLVMIR::IROpCode::BR_COND: return 1;  // 分支指令
        case LLVMIR::IROpCode::CALL: return 3;     // 函数调用代价较高
        case LLVMIR::IROpCode::LOAD:
        case LLVMIR::IROpCode::STORE: return 2;  // 内存访问指令
        case LLVMIR::IROpCode::ADD:
        case LLVMIR::IROpCode::SUB:
        case LLVMIR::IROpCode::MUL:
        case LLVMIR::IROpCode::DIV:
        case LLVMIR::IROpCode::ICMP:
        case LLVMIR::IROpCode::FCMP: return 1;  // 基本算术和比较指令
        default: return 1;                      // 其他指令的默认代价
    }
}

// 根据数据类型创建中转指令的实现
LLVMIR::Instruction* createCopyInst(LLVMIR::DataType type, LLVMIR::Operand* src, LLVMIR::Operand* dest)
{
    Instruction* inst = nullptr;

    switch (type)
    {
        case LLVMIR::DataType::I1:
        case LLVMIR::DataType::I8:
        case LLVMIR::DataType::I32:
            // 创建 add i32 src, 0 指令
            inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD, type, src, getImmeI32Operand(0), dest);
            break;
        case LLVMIR::DataType::F32:
            // 创建 fadd float src, 0.0 指令
            inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::FADD, type, src, getImmeF32Operand(0.0f), dest);
            break;
        case LLVMIR::DataType::PTR:
            // 创建 getelementptr ptr src, i32 0 指令 (不添加偏移)
            inst = new LLVMIR::GEPInst(type, LLVMIR::DataType::I32, src, dest, {}, {getImmeI32Operand(0)});
            break;
        default:
            std::cout << "Unsupported data type for createCopyInst: " << type << std::endl;
            assert(false && "Unsupported data type for createCopyInst");
    }

    inst->comment = "Created by createCopyInst";
    return inst;
}

string Instruction::toString()
{
    stringstream ss;
    printIR(ss);

    string str = ss.str();

    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    return str;
}

void LoadInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void StoreInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void ArithmeticInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void IcmpInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void FcmpInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void AllocInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void BranchCondInst::ReplaceLabels(const std::map<int, int>& label_mapping)
{
    if (true_label && true_label->type == OperandType::LABEL)
    {
        auto* label_op = dynamic_cast<LabelOperand*>(true_label);
        auto  it       = label_mapping.find(label_op->label_num);
        if (it != label_mapping.end()) { true_label = getLabelOperand(it->second); }
    }
    if (false_label && false_label->type == OperandType::LABEL)
    {
        auto* label_op = dynamic_cast<LabelOperand*>(false_label);
        auto  it       = label_mapping.find(label_op->label_num);
        if (it != label_mapping.end()) { false_label = getLabelOperand(it->second); }
    }
}

void BranchUncondInst::ReplaceLabels(const std::map<int, int>& label_mapping)
{
    if (target_label && target_label->type == OperandType::LABEL)
    {
        auto* label_op = dynamic_cast<LabelOperand*>(target_label);
        auto  it       = label_mapping.find(label_op->label_num);
        if (it != label_mapping.end()) { target_label = getLabelOperand(it->second); }
    }
}

void GlbvarDefInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void CallInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void RetInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void GEPInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void FuncDeclareInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void FuncDefInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void SI2FPInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void FP2SIInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void ZextInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void FPExtInst::ReplaceLabels(const std::map<int, int>& label_mapping) {}

void PhiInst::ReplaceLabels(const std::map<int, int>& label_mapping)
{
    for (auto& val_label : vals_for_labels)
    {
        if (val_label.second && val_label.second->type == OperandType::LABEL)
        {
            auto* label_op = dynamic_cast<LabelOperand*>(val_label.second);
            auto  it       = label_mapping.find(label_op->label_num);
            if (it != label_mapping.end()) { val_label.second = getLabelOperand(it->second); }
        }
    }
}

Operand* PhiInst::GetValOperandOfBlock(int block_id)
{
    for (auto [val, label] : vals_for_labels)
    {
        if (label && label->type == OperandType::LABEL)
        {
            auto* label_op = dynamic_cast<LabelOperand*>(label);
            if (label_op->label_num == block_id) { return val; }
        }
    }
    assert(false && "Block ID not found in PhiInst operands");
    return nullptr;  // 如果没有找到对应的块ID，返回nullptr
}

void PhiInst::SetValOperandOfBlock(int block_id, int reg_id)
{
    for (auto& [val, label] : vals_for_labels)
    {
        if (label && label->type == OperandType::LABEL)
        {
            auto* label_op = dynamic_cast<LabelOperand*>(label);
            if (label_op->label_num == block_id)
            {
                val = getRegOperand(reg_id);  // 更新对应块ID的值为新的寄存器操作数
                return;
            }
        }
    }
    assert(false && "Block ID not found in PhiInst operands for setting value");
}

void PhiInst::ErasePhiWithBlock(int block_id)
{
    for (auto it = vals_for_labels.begin(); it != vals_for_labels.end(); ++it)
    {
        auto [val, label] = *it;
        if (((LabelOperand*)label)->label_num == block_id)
        {
            vals_for_labels.erase(it);  // 删除对应块ID的操作数
            return;
        }
    }
}

void PhiInst::SetNewFrom(int old_from, int new_from)
{
    std::cout << "for phi with result " << res->GetRegNum() << " old from and new from " << old_from << ' ' << new_from
              << std::endl;
    for (auto& [val, label] : vals_for_labels)
    {
        // if (val && val->type == OperandType::REG)
        // {
        //     auto* reg_op = dynamic_cast<RegOperand*>(val);
        //     if (reg_op->reg_num == old_from)
        //     {
        //         label = getLabelOperand(new_from);  // 更新对应寄存器操作数的标签
        //         return;
        //     }
        // }

        // 牛魔的copilot害人

        if (label && label->type == OperandType::LABEL)
        {
            auto label_op = (LabelOperand*)label;
            if (label_op->label_num == old_from)
            {
                label = getLabelOperand(new_from);
                return;
            }
        }
    }
}

// SelectInst implementation
SelectInst::SelectInst(DataType t, Operand* tv, Operand* fv, Operand* c, Operand* r)
    : Instruction(LLVMIR::IROpCode::SELECT), type(t), cond(c), true_val(tv), false_val(fv), res(r)
{}

void SelectInst::printIR(std::ostream& s)
{
    s << res << " = select i1 " << cond << ", " << type << " " << true_val << ", " << type << " " << false_val
      << std::endl;
}

void SelectInst::Rename(std::map<int, int>& replace)
{
    if (cond->type == OperandType::REG)
    {
        int reg_num = ((RegOperand*)cond)->reg_num;
        if (replace.find(reg_num) != replace.end()) { cond = getRegOperand(replace[reg_num]); }
    }
    if (true_val->type == OperandType::REG)
    {
        int reg_num = ((RegOperand*)true_val)->reg_num;
        if (replace.find(reg_num) != replace.end()) { true_val = getRegOperand(replace[reg_num]); }
    }
    if (false_val->type == OperandType::REG)
    {
        int reg_num = ((RegOperand*)false_val)->reg_num;
        if (replace.find(reg_num) != replace.end()) { false_val = getRegOperand(replace[reg_num]); }
    }
    if (res->type == OperandType::REG)
    {
        int reg_num = ((RegOperand*)res)->reg_num;
        if (replace.find(reg_num) != replace.end()) { res = getRegOperand(replace[reg_num]); }
    }
}

void SelectInst::ReplaceAllOperands(std::map<int, int>& replace) { Rename(replace); }

int SelectInst::GetResultReg()
{
    if (res->type == OperandType::REG) { return ((RegOperand*)res)->reg_num; }
    return -1;
}

std::vector<int> SelectInst::GetUsedRegs()
{
    std::vector<int> used_regs;
    if (cond->type == OperandType::REG) { used_regs.push_back(((RegOperand*)cond)->reg_num); }
    if (true_val->type == OperandType::REG) { used_regs.push_back(((RegOperand*)true_val)->reg_num); }
    if (false_val->type == OperandType::REG) { used_regs.push_back(((RegOperand*)false_val)->reg_num); }
    return used_regs;
}

std::vector<Operand*> SelectInst::GetUsedOperands() { return {cond, true_val, false_val}; }

Operand* SelectInst::GetResultOperand() { return res; }

Instruction* SelectInst::Clone(int new_result_reg) const
{
    Operand* new_res = (new_result_reg == -1) ? res : getRegOperand(new_result_reg);
    return new SelectInst(type, true_val, false_val, cond, new_res);
}

bool SelectInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    bool has_replacement = false;
    if (cond->type == OperandType::REG)
    {
        int  reg_num = ((RegOperand*)cond)->reg_num;
        auto it      = substitutions.find(reg_num);
        if (it != substitutions.end())
        {
            cond            = it->second;
            has_replacement = true;
        }
    }
    if (true_val->type == OperandType::REG)
    {
        int  reg_num = ((RegOperand*)true_val)->reg_num;
        auto it      = substitutions.find(reg_num);
        if (it != substitutions.end())
        {
            true_val        = it->second;
            has_replacement = true;
        }
    }
    if (false_val->type == OperandType::REG)
    {
        int  reg_num = ((RegOperand*)false_val)->reg_num;
        auto it      = substitutions.find(reg_num);
        if (it != substitutions.end())
        {
            false_val       = it->second;
            has_replacement = true;
        }
    }
    return has_replacement;
}

DataType SelectInst::GetResultType() const { return type; }

Instruction* SelectInst::CloneWithMapping(const std::map<int, int>& reg_map, const std::map<int, int>& label_map) const
{
    Operand* new_cond      = copyOperand(cond, reg_map, label_map);
    Operand* new_true_val  = copyOperand(true_val, reg_map, label_map);
    Operand* new_false_val = copyOperand(false_val, reg_map, label_map);
    Operand* new_res       = copyOperand(res, reg_map, label_map);
    return new SelectInst(type, new_true_val, new_false_val, new_cond, new_res);
}

std::vector<Operand*> SelectInst::GetCSEOperands() const { return {cond, true_val, false_val}; }

void SelectInst::ReplaceLabels(const std::map<int, int>& label_map) {}

EmptyInst::EmptyInst() : Instruction(LLVMIR::IROpCode::EMPTY) {}
void EmptyInst::printIR(std::ostream& s)
{
    if (comment.empty()) return;

    s << "; " << comment << std::endl;
}

void                  EmptyInst::Rename(std::map<int, int>&) {}
void                  EmptyInst::ReplaceAllOperands(std::map<int, int>&) {}
int                   EmptyInst::GetResultReg() { return -1; }
std::vector<int>      EmptyInst::GetUsedRegs() { return {}; }
std::vector<Operand*> EmptyInst::GetUsedOperands() { return {}; }
Operand*              EmptyInst::GetResultOperand() { return nullptr; }
Instruction*          EmptyInst::Clone(int) const { return new EmptyInst(); }
bool                  EmptyInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) { return false; }
DataType              EmptyInst::GetResultType() const { return DataType::I32; }
Instruction*          EmptyInst::CloneWithMapping(const std::map<int, int>&, const std::map<int, int>&) const
{
    return new EmptyInst();
}
std::vector<Operand*> EmptyInst::GetCSEOperands() const { return {}; }
void                  EmptyInst::ReplaceLabels(const std::map<int, int>&) {}
