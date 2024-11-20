#include <llvm_ir/block.h>
#include <llvm_ir/build/type_trans.h>
#include <map>
#include <unordered_map>
#include <cassert>
using namespace std;
using namespace LLVMIR;

using DT = DataType;
using IC = IcmpCond;
using FC = FcmpCond;

static unordered_map<int, RegOperand*> RegOperandMap;
static map<int, LabelOperand*>         LabelOperandMap;
static map<string, GlobalOperand*>     GlobalOperandMap;

extern int max_reg;

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

IRBlock::IRBlock(int id) : comment(""), block_id(id), insts({}) {}
IRBlock::~IRBlock() {}

void IRBlock::printIR(ostream& s)
{
    s << "L" << block_id << ":  ;" << comment << "\n";
    for (auto& inst : insts)
    {
        s << "\t";
        inst->printIR(s);
    }
}

void IRBlock::insertLoad(DataType t, Operand* ptr, int res_reg)
{
    insts.emplace_back(new LoadInst(t, ptr, getRegOperand(res_reg)));
}

void IRBlock::insertStore(DataType t, int val_reg, Operand* ptr)
{
    insts.emplace_back(new StoreInst(t, ptr, getRegOperand(val_reg)));
}

void IRBlock::insertStore(DataType t, Operand* val, Operand* ptr) { insts.emplace_back(new StoreInst(t, ptr, val)); }

void IRBlock::insertArithmeticI32(IROpCode op, int lhs_reg, int rhs_reg, int res_reg)
{
    insts.emplace_back(
        new ArithmeticInst(op, DT::I32, getRegOperand(lhs_reg), getRegOperand(rhs_reg), getRegOperand(res_reg)));
}

void IRBlock::insertArithmeticF32(IROpCode op, int lhs_reg, int rhs_reg, int res_reg)
{
    insts.emplace_back(
        new ArithmeticInst(op, DT::F32, getRegOperand(lhs_reg), getRegOperand(rhs_reg), getRegOperand(res_reg)));
}

void IRBlock::insertArithmeticI32_ImmeLeft(IROpCode op, int lhs_val, int rhs_reg, int res_reg)
{
    insts.emplace_back(
        new ArithmeticInst(op, DT::I32, new ImmeI32Operand(lhs_val), getRegOperand(rhs_reg), getRegOperand(res_reg)));
}

void IRBlock::insertArithmeticF32_ImmeLeft(IROpCode op, float lhs_val, int rhs_reg, int res_reg)
{
    insts.emplace_back(
        new ArithmeticInst(op, DT::F32, new ImmeF32Operand(lhs_val), getRegOperand(rhs_reg), getRegOperand(res_reg)));
}

void IRBlock::insertArithmeticI32_ImmeAll(IROpCode op, int lhs_val, int rhs_val, int res_reg)
{
    insts.emplace_back(new ArithmeticInst(
        op, DT::I32, new ImmeI32Operand(lhs_val), new ImmeI32Operand(rhs_val), getRegOperand(res_reg)));
}

void IRBlock::insertArithmeticF32_ImmeAll(IROpCode op, float lhs_val, float rhs_val, int res_reg)
{
    insts.emplace_back(new ArithmeticInst(
        op, DT::F32, new ImmeF32Operand(lhs_val), new ImmeF32Operand(rhs_val), getRegOperand(res_reg)));
}

void IRBlock::insertIcmp(IcmpCond cond, int lhs_reg, int rhs_reg, int res_reg)
{
    insts.emplace_back(
        new IcmpInst(DT::I32, cond, getRegOperand(lhs_reg), getRegOperand(rhs_reg), getRegOperand(res_reg)));
}

void IRBlock::insertFcmp(FcmpCond cond, int lhs_reg, int rhs_reg, int res_reg)
{
    insts.emplace_back(
        new FcmpInst(DT::F32, cond, getRegOperand(lhs_reg), getRegOperand(rhs_reg), getRegOperand(res_reg)));
}

void IRBlock::insertIcmp_ImmeRight(IcmpCond cond, int lhs_reg, int rhs_val, int res_reg)
{
    insts.emplace_back(
        new IcmpInst(DT::I32, cond, getRegOperand(lhs_reg), new ImmeI32Operand(rhs_val), getRegOperand(res_reg)));
}

void IRBlock::insertFcmp_ImmeRight(FcmpCond cond, int lhs_reg, float rhs_val, int res_reg)
{
    insts.emplace_back(
        new FcmpInst(DT::F32, cond, getRegOperand(lhs_reg), new ImmeF32Operand(rhs_val), getRegOperand(res_reg)));
}

void IRBlock::insertFP2SI(int src_reg, int dest_reg)
{
    insts.emplace_back(new FP2SIInst(getRegOperand(src_reg), getRegOperand(dest_reg)));
}

void IRBlock::insertSI2FP(int src_reg, int dest_reg)
{
    insts.emplace_back(new SI2FPInst(getRegOperand(src_reg), getRegOperand(dest_reg)));
}

void IRBlock::insertZextI1toI32(int src_reg, int dest_reg)
{
    insts.emplace_back(new ZextInst(DT::I1, DT::I32, getRegOperand(src_reg), getRegOperand(dest_reg)));
}

void IRBlock::insertGEP_I32(DataType t, Operand* ptr, std::vector<int> dims, std::vector<Operand*> is, int res_reg)
{
    insts.emplace_back(new GEPInst(t, DT::I32, ptr, getRegOperand(res_reg), dims, is));
}

void IRBlock::insertGEP_I64(DataType t, Operand* ptr, std::vector<int> dims, std::vector<Operand*> is, int res_reg)
{
    insts.emplace_back(new GEPInst(t, DT::I64, ptr, getRegOperand(res_reg), dims, is));
}

void IRBlock::insertCall(
    DataType t, std::string func_name, std::vector<std::pair<DataType, Operand*>> args, int res_reg)
{
    insts.emplace_back(new CallInst(t, func_name, args, getRegOperand(res_reg)));
}

void IRBlock::insertCallVoid(DataType t, std::string func_name, std::vector<std::pair<DataType, Operand*>> args)
{
    insts.emplace_back(new CallInst(t, func_name, args, nullptr));
}

void IRBlock::insertCallNoargs(DataType t, std::string func_name, int res_reg)
{
    insts.emplace_back(new CallInst(t, func_name, getRegOperand(res_reg)));
}

void IRBlock::insertCallVoidNoargs(DataType t, std::string func_name)
{
    insts.emplace_back(new CallInst(t, func_name, nullptr));
}

void IRBlock::insertRetReg(DataType t, int reg) { insts.emplace_back(new RetInst(t, getRegOperand(reg))); }

void IRBlock::insertRetImmI32(DataType t, int val) { insts.emplace_back(new RetInst(t, new ImmeI32Operand(val))); }

void IRBlock::insertRetImmF32(DataType t, float val) { insts.emplace_back(new RetInst(t, new ImmeF32Operand(val))); }

void IRBlock::insertRetVoid() { insts.emplace_back(new RetInst(DT::VOID, nullptr)); }

void IRBlock::insertCondBranch(int cond_reg, int true_label, int false_label)
{
    insts.emplace_back(
        new BranchCondInst(getRegOperand(cond_reg), getLabelOperand(true_label), getLabelOperand(false_label)));
}

void IRBlock::insertUncondBranch(int dst_label)
{
    insts.emplace_back(new BranchUncondInst(getLabelOperand(dst_label)));
}

void IRBlock::insertAlloc(DataType t, int reg) { insts.emplace_back(new AllocInst(t, getRegOperand(reg))); }

void IRBlock::insertAllocArray(DataType t, std::vector<int> dims, int reg)
{
    insts.emplace_back(new AllocInst(t, getRegOperand(reg), dims));
}

void IRBlock::insertTypeConvert(TypeKind from, TypeKind to, int src_reg)
{
    if (from == to) return;

    switch (static_cast<int>(from))
    {
        case static_cast<int>(TypeKind::Int):
        case static_cast<int>(TypeKind::LL):
        {
            switch (static_cast<int>(to))
            {
                case static_cast<int>(TypeKind::Int):
                case static_cast<int>(TypeKind::LL): return;
                case static_cast<int>(TypeKind::Float):
                {
                    insertSI2FP(src_reg, ++max_reg);
                    break;
                }
                case static_cast<int>(TypeKind::Bool):
                {
                    insertIcmp_ImmeRight(IC::NE, src_reg, 0, ++max_reg);
                    break;
                }
                default: assert(false);
            }
            break;
        }
        case static_cast<int>(TypeKind::Float):
        {
            switch (static_cast<int>(to))
            {
                case static_cast<int>(TypeKind::Int):
                case static_cast<int>(TypeKind::LL):
                {
                    insertFP2SI(src_reg, ++max_reg);
                    break;
                }
                case static_cast<int>(TypeKind::Bool):
                {
                    insertFcmp_ImmeRight(FC::ONE, src_reg, 0, ++max_reg);
                    break;
                }
                default: assert(false);
            }
            break;
        }
        case static_cast<int>(TypeKind::Bool):
        {
            switch (static_cast<int>(to))
            {
                case static_cast<int>(TypeKind::Int):
                case static_cast<int>(TypeKind::LL):
                {
                    insertZextI1toI32(src_reg, ++max_reg);
                    break;
                }
                case static_cast<int>(TypeKind::Float):
                {
                    int tmp  = ++max_reg;
                    int dest = ++max_reg;
                    insertZextI1toI32(src_reg, tmp);
                    insertSI2FP(tmp, dest);
                    break;
                }
                default: assert(false);
            }
            break;
        }
        default: assert(false);
    }
}