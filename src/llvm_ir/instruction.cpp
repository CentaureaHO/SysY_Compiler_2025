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
void LoadInst::printIR(ostream& s)
{
    s << res << " = load " << type << ", ptr " << ptr;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}

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

void LoadInst::Rename(std::map<int, int>& replace)
{
    if (this->ptr->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->ptr))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            ptr = new RegOperand(replace[reg]);
        }
    }

    if (this->res->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->res))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            res = new RegOperand(replace[reg]);
        }
    }
}

StoreInst::StoreInst(DataType t, Operand* p, Operand* r) : Instruction(IROpCode::STORE), type(t), ptr(p), val(r) {}
void StoreInst::printIR(ostream& s)
{
    s << "store " << type << " " << val << ", ptr " << ptr;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}

int StoreInst::GetResultReg() { return -1; }

std::vector<int> StoreInst::GetUsedRegs()
{
    std::vector<int> ret{};
    if (this->ptr->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->ptr))->reg_num); }
    if (this->val->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->val))->reg_num); }
    return ret;
}

void StoreInst::Rename(std::map<int, int>& replace)
{
    if (this->ptr->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->ptr))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            ptr = new RegOperand(replace[reg]);
        }
    }

    if (this->val->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->val))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            val = new RegOperand(replace[reg]);
        }
    }
}

ArithmeticInst::ArithmeticInst(IROpCode op, DataType t, Operand* l, Operand* r, Operand* res)
    : Instruction(op), type(t), lhs(l), rhs(r), res(res)
{}
void ArithmeticInst::printIR(ostream& s)
{
    s << res << " = " << opcode << " " << type << " " << lhs << ", " << rhs;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
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
            lhs = new RegOperand(replace[reg]);
        }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = new RegOperand(replace[reg]); }
    }
}

IcmpInst::IcmpInst(DataType t, IcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::ICMP), type(t), cond(c), lhs(l), rhs(r), res(res)
{}
void IcmpInst::printIR(ostream& s)
{
    s << res << " = icmp " << cond << " " << type << " " << lhs << ", " << rhs;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
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

void IcmpInst::Rename(std::map<int, int>& replace)
{
    if (this->lhs->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->lhs))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            lhs = new RegOperand(replace[reg]);
        }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = new RegOperand(replace[reg]); }
    }
}

FcmpInst::FcmpInst(DataType t, FcmpCond c, Operand* l, Operand* r, Operand* res)
    : Instruction(IROpCode::FCMP), type(t), cond(c), lhs(l), rhs(r), res(res)
{}
void FcmpInst::printIR(ostream& s)
{
    s << res << " = fcmp " << cond << " " << type << " " << lhs << ", " << rhs;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
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

void FcmpInst::Rename(std::map<int, int>& replace)
{
    if (this->lhs->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->lhs))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            lhs = new RegOperand(replace[reg]);
        }
    }

    if (this->rhs->type == OperandType::REG)
    {
        int reg = ((RegOperand*)(this->rhs))->reg_num;
        if (replace.find(reg) != replace.end()) { rhs = new RegOperand(replace[reg]); }
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
    s << type << string(dims.size(), ']');
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
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

void AllocInst::Rename(std::map<int, int>& replace) {}

BranchCondInst::BranchCondInst(Operand* c, Operand* t, Operand* f)
    : Instruction(IROpCode::BR_COND), cond(c), true_label(t), false_label(f)
{}
void BranchCondInst::printIR(ostream& s)
{
    s << "br i1 " << cond << ", label " << true_label << ", label " << false_label;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}

int BranchCondInst::GetResultReg() { return -1; }

std::vector<int> BranchCondInst::GetUsedRegs()
{
    std::vector<int> ret{};
    if (this->cond->type == OperandType::REG) { ret.push_back(((RegOperand*)(this->cond))->reg_num); }
    return ret;
}

void BranchCondInst::Rename(std::map<int, int>& replace)
{  // 其实这里是cmp出来的寄存器，不会被mem2reg波及
    if (this->cond->type == OperandType::REG)
    {  // 保证是可替换的reg
        int reg = ((RegOperand*)(this->cond))->reg_num;
        if (replace.find(reg) != replace.end())
        {  // 保证在替换表中
            cond = new RegOperand(replace[reg]);
        }
    }
}

BranchUncondInst::BranchUncondInst(Operand* t) : Instruction(IROpCode::BR_UNCOND), target_label(t) {}
void BranchUncondInst::printIR(ostream& s)
{
    s << "br label " << target_label;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}
int BranchUncondInst::GetResultReg() { return -1; }

std::vector<int> BranchUncondInst::GetUsedRegs()
{
    std::vector<int> ret{};
    return ret;
}

void BranchUncondInst::Rename(std::map<int, int>& replace) {}

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

    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}
int GlbvarDefInst::GetResultReg() { return -1; }

std::vector<int> GlbvarDefInst::GetUsedRegs()
{
    std::vector<int> ret{};
    return ret;
}

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
    s << ")";
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
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

void CallInst::Rename(std::map<int, int>& replace)
{
    for (auto& iter : args)
    {
        auto op = iter.second;
        if (op->type == OperandType::REG)
        {
            int regno = ((RegOperand*)op)->reg_num;
            if (replace.find(regno) != replace.end()) { iter.second = new RegOperand(replace[regno]); }
        }
    }
}

RetInst::RetInst(DataType t, Operand* r) : Instruction(IROpCode::RET), ret_type(t), ret(r) {}
void RetInst::printIR(ostream& s)
{
    s << "ret " << ret_type;
    if (ret) s << " " << ret;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}

int RetInst::GetResultReg() { return -1; }

std::vector<int> RetInst::GetUsedRegs()
{
    std::vector<int> retvec{};

    if (ret != nullptr && ret->type == OperandType::REG) { retvec.push_back(((RegOperand*)ret)->reg_num); }

    return retvec;
}

void RetInst::Rename(std::map<int, int>& replace)
{
    if (ret != nullptr && ret->type == OperandType::REG)
    {
        int regno = ((RegOperand*)ret)->reg_num;
        if (replace.find(regno) != replace.end()) ret = new RegOperand(replace[regno]);
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
    if (!comment.empty()) s << " ; " << comment;
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

void GEPInst::Rename(std::map<int, int>& replace)
{
    if (base_ptr->type == OperandType::REG)
    {
        int regno = ((RegOperand*)base_ptr)->reg_num;
        if (replace.find(regno) != replace.end()) { base_ptr = new RegOperand(replace[regno]); }
    }
    for (auto& iter : idxs)
    {
        if (iter->type == OperandType::REG)
        {
            // std::cout<<"'!!!!!!!!!!!!!!!!!!!!!!!!!!!'"<<std::endl;
            int regno = ((RegOperand*)iter)->reg_num;
            if (replace.find(regno) != replace.end()) { iter = new RegOperand(replace[regno]); }
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
    s << ")";
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}

int FuncDeclareInst::GetResultReg() { return -1; }

std::vector<int> FuncDeclareInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    return retvec;
}

void FuncDeclareInst::Rename(std::map<int, int>& replace) {}

FuncDefInst::FuncDefInst(DataType rt, string fn, vector<DataType> at) : FuncDeclareInst(rt, fn, at), arg_regs({})
{
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

    s << ")";
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}

int FuncDefInst::GetResultReg() { return -1; }

std::vector<int> FuncDefInst::GetUsedRegs()
{
    std::vector<int> retvec{};
    return retvec;
}

void FuncDefInst::Rename(std::map<int, int>& replace) {}

SI2FPInst::SI2FPInst(Operand* f, Operand* t) : Instruction(IROpCode::SITOFP), f_si(f), t_fp(t) {}
void SI2FPInst::printIR(ostream& s)
{
    s << t_fp << " = sitofp i32 " << f_si << " to float";
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}
int SI2FPInst::GetResultReg()
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

void SI2FPInst::Rename(std::map<int, int>& replace)
{
    if (f_si->type == OperandType::REG)
    {
        int regno = ((RegOperand*)f_si)->reg_num;
        if (replace.find(regno) != replace.end()) { f_si = new RegOperand(replace[regno]); }
    }
}

FP2SIInst::FP2SIInst(Operand* f, Operand* t) : Instruction(IROpCode::FPTOSI), f_fp(f), t_si(t) {}
void FP2SIInst::printIR(ostream& s)
{
    s << t_si << " = fptosi float " << f_fp << " to i32";
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}
int FP2SIInst::GetResultReg()
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

void FP2SIInst::Rename(std::map<int, int>& replace)
{
    if (f_fp->type == OperandType::REG)
    {
        int regno = ((RegOperand*)f_fp)->reg_num;
        if (replace.find(regno) != replace.end()) { f_fp = new RegOperand(replace[regno]); }
    }
}

ZextInst::ZextInst(DataType f, DataType t, Operand* s, Operand* d)
    : Instruction(IROpCode::ZEXT), from(f), to(t), src(s), dest(d)
{}
void ZextInst::printIR(ostream& s)
{
    s << dest << " = zext " << from << " " << src << " to " << to;
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}

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

void ZextInst::Rename(std::map<int, int>& replace)
{
    if (src->type == OperandType::REG)
    {
        int regno = ((RegOperand*)src)->reg_num;
        if (replace.find(regno) != replace.end()) { src = new RegOperand(replace[regno]); }
    }
}

FPExtInst::FPExtInst(Operand* s, Operand* d) : Instruction(IROpCode::FPEXT), src(s), dest(d) {}
void FPExtInst::printIR(ostream& s)
{
    s << dest << " = fpext float" << " " << src << " to " << "double";
    if (!comment.empty()) s << " ; " << comment;
    s << "\n";
}
int FPExtInst::GetResultReg()
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

void FPExtInst::Rename(std::map<int, int>& replace)
{
    if (src->type == OperandType::REG)
    {
        int regno = ((RegOperand*)src)->reg_num;
        if (replace.find(regno) != replace.end()) { src = new RegOperand(replace[regno]); }
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
    if (!comment.empty()) s << " ; " << comment;
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

void PhiInst::Rename(std::map<int, int>& replace)
{
    for (auto& iter : vals_for_labels)
    {
        if (iter.first->type == OperandType::REG)
        {
            int regno = ((RegOperand*)iter.first)->reg_num;
            if (replace.find(regno) != replace.end()) { iter.first = new RegOperand(replace[regno]); }
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
void LoadInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (ptr && ptr->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(ptr);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { ptr = it->second; }
    }
}

void StoreInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (ptr && ptr->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(ptr);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { ptr = it->second; }
    }
    if (val && val->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(val);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { val = it->second; }
    }
}

void ArithmeticInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (lhs && lhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(lhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { lhs = it->second; }
    }
    if (rhs && rhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(rhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { rhs = it->second; }
    }
}

void IcmpInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (lhs && lhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(lhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { lhs = it->second; }
    }
    if (rhs && rhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(rhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { rhs = it->second; }
    }
}

void FcmpInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (lhs && lhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(lhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { lhs = it->second; }
    }
    if (rhs && rhs->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(rhs);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { rhs = it->second; }
    }
}

void AllocInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) {}

void BranchCondInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (cond && cond->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(cond);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { cond = it->second; }
    }
}

void BranchUncondInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) {}

void GlbvarDefInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (init && init->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(init);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { init = it->second; }
    }
}

void CallInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    for (auto& arg : args)
    {
        if (arg.second && arg.second->type == OperandType::REG)
        {
            auto* reg_op = dynamic_cast<RegOperand*>(arg.second);
            auto  it     = substitutions.find(reg_op->reg_num);
            if (it != substitutions.end()) { arg.second = it->second; }
        }
    }
}

void RetInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (ret && ret->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(ret);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { ret = it->second; }
    }
}

void GEPInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (base_ptr && base_ptr->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(base_ptr);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { base_ptr = it->second; }
    }
    for (auto& idx : idxs)
    {
        if (idx && idx->type == OperandType::REG)
        {
            auto* reg_op = dynamic_cast<RegOperand*>(idx);
            auto  it     = substitutions.find(reg_op->reg_num);
            if (it != substitutions.end()) { idx = it->second; }
        }
    }
}

void FuncDeclareInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) {}

void FuncDefInst::SubstituteOperands(const std::map<int, Operand*>& substitutions) {}

void SI2FPInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (f_si && f_si->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(f_si);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { f_si = it->second; }
    }
}

void FP2SIInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (f_fp && f_fp->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(f_fp);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { f_fp = it->second; }
    }
}

void ZextInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (src && src->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(src);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { src = it->second; }
    }
}

void FPExtInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    if (src && src->type == OperandType::REG)
    {
        auto* reg_op = dynamic_cast<RegOperand*>(src);
        auto  it     = substitutions.find(reg_op->reg_num);
        if (it != substitutions.end()) { src = it->second; }
    }
}

void PhiInst::SubstituteOperands(const std::map<int, Operand*>& substitutions)
{
    for (auto& val_label : vals_for_labels)
    {
        if (val_label.first && val_label.first->type == OperandType::REG)
        {
            auto* reg_op = dynamic_cast<RegOperand*>(val_label.first);
            auto  it     = substitutions.find(reg_op->reg_num);
            if (it != substitutions.end()) { val_label.first = it->second; }
        }
    }
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
