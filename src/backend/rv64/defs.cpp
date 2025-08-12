#include <backend/rv64/rv64_defs.h>
using namespace Backend::RV64;
using namespace std;

DataType INT32OBJ(DataType::INT, DataType::B32);
DataType INT64OBJ(DataType::INT, DataType::B64);
DataType INT128OBJ(DataType::INT, DataType::B128);

DataType FLOAT32OBJ(DataType::FLOAT, DataType::B32);
DataType FLOAT64OBJ(DataType::FLOAT, DataType::B64);
DataType FLOAT128OBJ(DataType::FLOAT, DataType::B128);

DataType *Backend::RV64::INT32 = &INT32OBJ, *Backend::RV64::INT64 = &INT64OBJ, *Backend::RV64::INT128 = &INT128OBJ;
DataType *Backend::RV64::FLOAT32 = &FLOAT32OBJ, *Backend::RV64::FLOAT64 = &FLOAT64OBJ,
         *Backend::RV64::FLOAT128 = &FLOAT128OBJ;

OpInfo::OpInfo() : _asm("NULL"), latency(1) {}
OpInfo::OpInfo(string a, RV64OpType t, int lat) : _asm(a), type(t), latency(lat) {}

Register::Register() : reg_num(-1), data_type(INT32), is_virtual(true) {}
Register::Register(int reg, DataType* dt, bool is_v) : reg_num(reg), data_type(dt), is_virtual(is_v) {}
bool Register::operator<(Register other) const
{
    if (is_virtual != other.is_virtual) return is_virtual < other.is_virtual;
    if (reg_num != other.reg_num) return reg_num < other.reg_num;
    if (data_type != other.data_type)
    {
        if (data_type->data_type != other.data_type->data_type)
            return data_type->data_type < other.data_type->data_type;
        return data_type->data_length < other.data_type->data_length;
    }
    return false;
}
bool Register::operator==(Register other) const
{
    return reg_num == other.reg_num && data_type == other.data_type && is_virtual == other.is_virtual;
}

Operand::Operand(OperandType ot) : operand_type(ot) {}

RegOperand::RegOperand(Register r) : Operand(OperandType::REG), reg(r) {}
string RegOperand::to_string()
{
    if (reg.is_virtual) return "v" + std::to_string(reg.reg_num);
    return "r" + std::to_string(reg.reg_num);
}

ImmeI32Operand::ImmeI32Operand(int i32) : Operand(OperandType::IMMEI32), val(i32) {}
string ImmeI32Operand::to_string() { return std::to_string(val); }

ImmeF32Operand::ImmeF32Operand(float f32) : Operand(OperandType::IMMEF32), val(f32) {}
string ImmeF32Operand::to_string() { return std::to_string(val); }

ImmeF64Operand::ImmeF64Operand(double f64) : Operand(OperandType::IMMEF64), val(f64) {}
string ImmeF64Operand::to_string() { return std::to_string(val); }

Instruction::Instruction(InstType it) : inst_type(it) {}

RV64Label::RV64Label(bool la) : is_data_addr(false), is_hi(false), jmp_label(0), seq_label(0), is_la(la) {}
RV64Label::RV64Label(string n, bool hi, bool la)
    : name(n), is_data_addr(true), is_hi(hi), jmp_label(0), seq_label(0), is_la(la)
{}
RV64Label::RV64Label(int jmp, int seq, bool la)
    : is_data_addr(false), is_hi(false), jmp_label(jmp), seq_label(seq), is_la(la)
{}
RV64Label::RV64Label(int jmp, bool la) : is_data_addr(false), is_hi(false), jmp_label(jmp), seq_label(0), is_la(la) {}

RV64Inst::RV64Inst() : Instruction(InstType::RV64), use_label(false), imme(0) {}
vector<Register*> RV64Inst::getReadRegs()
{
    switch (opInfoTable[op].type)
    {
        case RV64OpType::R: return {&rs1, &rs2};
        case RV64OpType::R2: return {&rs1};
        case RV64OpType::I:
        {
            vector<Register*> res;
            res.push_back(&rs1);
            if (op == RV64InstType::JALR)
            {
                if (ret_type == 1) res.push_back(&preg_a0);
                if (ret_type == 2) res.push_back(&preg_fa0);
            }
            return res;
        }
        case RV64OpType::S: return {&rs1, &rs2};
        case RV64OpType::B: return {&rs1, &rs2};
        case RV64OpType::U: return {};
        case RV64OpType::J: return {};
        case RV64OpType::R4: return {&rs1, &rs2, &rs3};
        case RV64OpType::CALL:
        {
            vector<Register*> res;
            // for (int i = 0; i < call_ireg_cnt; ++i) res.push_back(&getPhyReg(static_cast<int>(REG::x10) + i));
            // for (int i = 0; i < call_freg_cnt; ++i) res.push_back(&getPhyReg(static_cast<int>(REG::f10) + i));

            int icnt = min(call_ireg_cnt, 7);
            int fcnt = min(call_freg_cnt, 7);
            switch (icnt)
            {
                case 7: res.push_back(&preg_a7);
                case 6: res.push_back(&preg_a6);
                case 5: res.push_back(&preg_a5);
                case 4: res.push_back(&preg_a4);
                case 3: res.push_back(&preg_a3);
                case 2: res.push_back(&preg_a2);
                case 1: res.push_back(&preg_a1);
                case 0: res.push_back(&preg_a0); break;
                default: assert(false);
            }
            switch (fcnt)
            {
                case 7: res.push_back(&preg_fa7);
                case 6: res.push_back(&preg_fa6);
                case 5: res.push_back(&preg_fa5);
                case 4: res.push_back(&preg_fa4);
                case 3: res.push_back(&preg_fa3);
                case 2: res.push_back(&preg_fa2);
                case 1: res.push_back(&preg_fa1);
                case 0: res.push_back(&preg_fa0); break;
                default: assert(false);
            }

            return res;
        }
        default: cerr << "Not implemented" << endl; assert(false);
    }

    return {};
}
vector<Register*> RV64Inst::getWriteRegs()
{
    switch (opInfoTable[op].type)
    {
        case RV64OpType::R: return {&rd};
        case RV64OpType::R2: return {&rd};
        case RV64OpType::I: return {&rd};
        case RV64OpType::S: return {};
        case RV64OpType::B: return {};
        case RV64OpType::U: return {&rd};
        case RV64OpType::J: return {&rd};
        case RV64OpType::R4: return {&rd};
        case RV64OpType::CALL:
        {
            static bool              init = false;
            static vector<Register*> res;
            if (!init)
            {
#define X(name, alias, saver) \
    if (saver == 0) res.push_back(&preg_##alias);
                RV64_REGS
#undef X
                init = true;
            }
            return res;
        }
        default: cerr << "Not implemented" << endl; assert(false);
    }

    return {};
}

void RV64Inst::replaceAllOperands(const std::map<int, int>& reg_replace_map)
{
    if (rd.is_virtual && reg_replace_map.find(rd.reg_num) != reg_replace_map.end())
    {
        rd.reg_num = reg_replace_map.at(rd.reg_num);
    }

    if (rs1.reg_num != -1 && rs1.is_virtual && reg_replace_map.find(rs1.reg_num) != reg_replace_map.end())
    {
        rs1.reg_num = reg_replace_map.at(rs1.reg_num);
    }

    if (rs2.reg_num != -1 && rs2.is_virtual && reg_replace_map.find(rs2.reg_num) != reg_replace_map.end())
    {
        rs2.reg_num = reg_replace_map.at(rs2.reg_num);
    }

    if (rs3.reg_num != -1 && rs3.is_virtual && reg_replace_map.find(rs3.reg_num) != reg_replace_map.end())
    {
        rs3.reg_num = reg_replace_map.at(rs3.reg_num);
    }
}

int RV64Inst::getLatency() const
{
    auto it = opInfoTable.find(op);
    if (it != opInfoTable.end()) { return it->second.latency; }
    return 1;  // Default latency
}

PhiInst::PhiInst(Register r) : Instruction(InstType::PHI), res_reg(r) {}
PhiInst::~PhiInst()
{
    for (auto& pair : phi_list) { delete pair.second; }
}
vector<Register*> PhiInst::getReadRegs()
{
    vector<Register*> read_regs;
    for (auto& [label, operand] : phi_list)
    {
        if (operand->operand_type == OperandType::REG)
        {
            RegOperand* reg_operand = (RegOperand*)operand;
            read_regs.push_back(&reg_operand->reg);
        }
    }
    return read_regs;
}
vector<Register*> PhiInst::getWriteRegs()
{
    vector<Register*> write_regs;
    write_regs.push_back(&res_reg);
    return write_regs;
}

void PhiInst::replaceAllOperands(const std::map<int, int>& reg_replace_map)
{
    if (res_reg.is_virtual && reg_replace_map.find(res_reg.reg_num) != reg_replace_map.end())
    {
        res_reg.reg_num = reg_replace_map.at(res_reg.reg_num);
    }

    for (auto& [label, operand] : phi_list)
    {
        if (operand->operand_type == OperandType::REG)
        {
            RegOperand* reg_operand = (RegOperand*)operand;
            if (reg_operand->reg.is_virtual && reg_replace_map.find(reg_operand->reg.reg_num) != reg_replace_map.end())
            {
                reg_operand->reg.reg_num = reg_replace_map.at(reg_operand->reg.reg_num);
            }
        }
    }
}

MoveInst::MoveInst(DataType* dt, Operand* s, Operand* d) : Instruction(InstType::MOVE), data_type(dt), src(s), dst(d) {}
MoveInst::~MoveInst()
{
    delete src;
    delete dst;
}
vector<Register*> MoveInst::getReadRegs()
{
    vector<Register*> read_regs;
    if (src->operand_type == OperandType::REG)
    {
        RegOperand* reg_src = (RegOperand*)src;
        read_regs.push_back(&reg_src->reg);
    }
    return read_regs;
}
vector<Register*> MoveInst::getWriteRegs()
{
    vector<Register*> write_regs;
    if (dst->operand_type == OperandType::REG)
    {
        RegOperand* reg_dst = (RegOperand*)dst;
        write_regs.push_back(&reg_dst->reg);
    }
    return write_regs;
}

void MoveInst::replaceAllOperands(const std::map<int, int>& reg_replace_map)
{
    if (src->operand_type == OperandType::REG)
    {
        RegOperand* reg_src = (RegOperand*)src;
        if (reg_src->reg.is_virtual && reg_replace_map.find(reg_src->reg.reg_num) != reg_replace_map.end())
        {
            reg_src->reg.reg_num = reg_replace_map.at(reg_src->reg.reg_num);
        }
    }

    if (dst->operand_type == OperandType::REG)
    {
        RegOperand* reg_dst = (RegOperand*)dst;
        if (reg_dst->reg.is_virtual && reg_replace_map.find(reg_dst->reg.reg_num) != reg_replace_map.end())
        {
            reg_dst->reg.reg_num = reg_replace_map.at(reg_dst->reg.reg_num);
        }
    }
}

SelectInst::SelectInst(Register cond, Register dst, Operand* tv, Operand* fv)
    : Instruction(InstType::SELECT), cond_reg(cond), dst_reg(dst), true_val(tv), false_val(fv)
{}

SelectInst::~SelectInst()
{
    delete true_val;
    delete false_val;
}

vector<Register*> SelectInst::getReadRegs()
{
    vector<Register*> read_regs;

    read_regs.push_back(&cond_reg);

    if (true_val && true_val->operand_type == OperandType::REG)
    {
        RegOperand* reg_operand = (RegOperand*)true_val;
        read_regs.push_back(&reg_operand->reg);
    }

    if (false_val && false_val->operand_type == OperandType::REG)
    {
        RegOperand* reg_operand = (RegOperand*)false_val;
        read_regs.push_back(&reg_operand->reg);
    }

    return read_regs;
}

vector<Register*> SelectInst::getWriteRegs()
{
    vector<Register*> write_regs;
    write_regs.push_back(&dst_reg);
    return write_regs;
}

void SelectInst::replaceAllOperands(const std::map<int, int>& reg_replace_map)
{
    if (cond_reg.is_virtual && reg_replace_map.find(cond_reg.reg_num) != reg_replace_map.end())
    {
        cond_reg.reg_num = reg_replace_map.at(cond_reg.reg_num);
    }

    if (dst_reg.is_virtual && reg_replace_map.find(dst_reg.reg_num) != reg_replace_map.end())
    {
        dst_reg.reg_num = reg_replace_map.at(dst_reg.reg_num);
    }

    if (true_val && true_val->operand_type == OperandType::REG)
    {
        RegOperand* reg_operand = (RegOperand*)true_val;
        if (reg_operand->reg.is_virtual && reg_replace_map.find(reg_operand->reg.reg_num) != reg_replace_map.end())
        {
            reg_operand->reg.reg_num = reg_replace_map.at(reg_operand->reg.reg_num);
        }
    }

    if (false_val && false_val->operand_type == OperandType::REG)
    {
        RegOperand* reg_operand = (RegOperand*)false_val;
        if (reg_operand->reg.is_virtual && reg_replace_map.find(reg_operand->reg.reg_num) != reg_replace_map.end())
        {
            reg_operand->reg.reg_num = reg_replace_map.at(reg_operand->reg.reg_num);
        }
    }
}

map<RV64InstType, OpInfo> Backend::RV64::opInfoTable = {
#define X(name, type, _asm) {RV64InstType::name, OpInfo(#_asm, RV64OpType::type, 1)},
    RV64_INSTS
#undef X
};

// Initialize latency information based on instruction characteristics
static void initializeLatencyInfo()
{
    // Arithmetic operations - 1 cycle
    opInfoTable[RV64InstType::ADD].latency  = 1;
    opInfoTable[RV64InstType::ADDW].latency = 1;
    opInfoTable[RV64InstType::SUB].latency  = 1;
    opInfoTable[RV64InstType::SUBW].latency = 1;
    opInfoTable[RV64InstType::ADDI].latency = 1;

    // Multiplication - 3 cycles
    opInfoTable[RV64InstType::MUL].latency  = 3;
    opInfoTable[RV64InstType::MULW].latency = 3;

    // Division - 20 cycles
    opInfoTable[RV64InstType::DIV].latency  = 20;
    opInfoTable[RV64InstType::DIVW].latency = 20;
    opInfoTable[RV64InstType::REM].latency  = 20;
    opInfoTable[RV64InstType::REMW].latency = 20;

    // Float operations - 4 cycles for add/sub, 5 for mul, 24 for div
    opInfoTable[RV64InstType::FADD_S].latency = 4;
    opInfoTable[RV64InstType::FSUB_S].latency = 4;
    opInfoTable[RV64InstType::FMUL_S].latency = 5;
    opInfoTable[RV64InstType::FDIV_S].latency = 24;

    // Memory operations - 3 cycles
    opInfoTable[RV64InstType::LW].latency  = 3;
    opInfoTable[RV64InstType::LD].latency  = 3;
    opInfoTable[RV64InstType::FLW].latency = 3;
    opInfoTable[RV64InstType::FLD].latency = 3;
    opInfoTable[RV64InstType::SW].latency  = 1;
    opInfoTable[RV64InstType::SD].latency  = 1;
    opInfoTable[RV64InstType::FSW].latency = 1;
    opInfoTable[RV64InstType::FSD].latency = 1;

    // Shift operations - 1 cycle
    opInfoTable[RV64InstType::SLL].latency  = 1;
    opInfoTable[RV64InstType::SRL].latency  = 1;
    opInfoTable[RV64InstType::SRA].latency  = 1;
    opInfoTable[RV64InstType::SLLI].latency = 1;
    opInfoTable[RV64InstType::SRLI].latency = 1;
    opInfoTable[RV64InstType::SRAI].latency = 1;

    // Branch operations - 1 cycle
    opInfoTable[RV64InstType::BEQ].latency = 1;
    opInfoTable[RV64InstType::BNE].latency = 1;
    opInfoTable[RV64InstType::BLT].latency = 1;
    opInfoTable[RV64InstType::BGE].latency = 1;

    // Jump operations - 1 cycle
    opInfoTable[RV64InstType::JAL].latency  = 1;
    opInfoTable[RV64InstType::JALR].latency = 1;

    // Conversion operations - 2 cycles
    opInfoTable[RV64InstType::FMV_W_X].latency  = 2;
    opInfoTable[RV64InstType::FMV_X_W].latency  = 2;
    opInfoTable[RV64InstType::FCVT_S_W].latency = 2;
    opInfoTable[RV64InstType::FCVT_W_S].latency = 2;
}

// Call initialization function
static bool latency_initialized = []() {
    initializeLatencyInfo();
    return true;
}();

map<int, string> Backend::RV64::rv64_reg_name_map = {
#define X(name, alias, saver) {static_cast<int>(REG::name), #alias},
    RV64_REGS
#undef X
};

DataType* Backend::RV64::getPhyRegType(int reg_num)
{
    size_t reg_num_size = (size_t)reg_num;
    if (reg_num_size >= static_cast<size_t>(REG::x0) && reg_num_size <= static_cast<size_t>(REG::x31)) return INT64;
    if (reg_num_size >= static_cast<size_t>(REG::f0) && reg_num_size <= static_cast<size_t>(REG::f31)) return FLOAT64;

    cerr << "Unknown register number: " << reg_num << endl;
    assert(false);
    return nullptr;
}

Register Backend::RV64::getPhyReg(int reg_num) { return Register(reg_num, getPhyRegType(reg_num), false); }

#define X(name, alias, saver) \
    Register Backend::RV64::preg_##alias = Backend::RV64::getPhyReg(static_cast<int>(Backend::RV64::REG::name));
RV64_REGS
#undef X
