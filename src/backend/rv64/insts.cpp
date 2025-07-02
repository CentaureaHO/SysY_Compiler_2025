#include <backend/rv64/insts.h>
using namespace Backend::RV64;
using namespace std;

RV64Inst* Backend::RV64::createRInst(RV64InstType op, Register rd, Register rs1, Register rs2)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = false;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::R);

    inst->rd  = rd;
    inst->rs1 = rs1;
    inst->rs2 = rs2;

    return inst;
}
RV64Inst* Backend::RV64::createR2Inst(RV64InstType op, Register rd, Register rs)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = false;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::R2);

    inst->rd  = rd;
    inst->rs1 = rs;

    return inst;
}

RV64Inst* Backend::RV64::createIInst(RV64InstType op, Register rd, Register rs1, int imme)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = false;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::I);

    inst->rd   = rd;
    inst->rs1  = rs1;
    inst->imme = imme;
    assert(imme >= -2048 && imme <= 2047);

    return inst;
}
RV64Inst* Backend::RV64::createIInst(RV64InstType op, Register rd, Register rs1, RV64Label label)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = true;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end());

    // cerr << (size_t)it->second.type << endl;
    Assert(it->second.type == RV64OpType::I);

    inst->rd    = rd;
    inst->rs1   = rs1;
    inst->label = label;

    return inst;
}

RV64Inst* Backend::RV64::createSInst(RV64InstType op, Register val, Register ptr, int imme)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = false;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::S);

    inst->rs1  = val;
    inst->rs2  = ptr;
    inst->imme = imme;
    assert(imme >= -2048 && imme <= 2047);

    return inst;
}
RV64Inst* Backend::RV64::createSInst(RV64InstType op, Register val, Register ptr, RV64Label label)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = true;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::S);

    inst->rs1   = val;
    inst->rs2   = ptr;
    inst->label = label;

    return inst;
}

RV64Inst* Backend::RV64::createBInst(RV64InstType op, Register rs1, Register rs2, RV64Label label)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = true;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::B);

    inst->rs1   = rs1;
    inst->rs2   = rs2;
    inst->label = label;

    return inst;
}

RV64Inst* Backend::RV64::createUInst(RV64InstType op, Register rd, int imme)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = false;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::U);

    inst->rd   = rd;
    inst->imme = imme;
    // assert(imme >= -2048 && imme <= 2047);

    return inst;
}
RV64Inst* Backend::RV64::createUInst(RV64InstType op, Register rd, RV64Label label)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = true;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::U);

    inst->rd    = rd;
    inst->label = label;

    return inst;
}

RV64Inst* Backend::RV64::createJInst(RV64InstType op, Register rd, RV64Label label)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = true;
    auto it         = opInfoTable.find(op);
    Assert(it != opInfoTable.end() && it->second.type == RV64OpType::J);

    inst->rd    = rd;
    inst->label = label;

    return inst;
}

RV64Inst* Backend::RV64::createCallInst(RV64InstType op, RV64Label label)
{
    RV64Inst* inst = new RV64Inst();

    cerr << "Not implemented yet!" << endl;
    assert(false);

    return nullptr;
}

MoveInst* Backend::RV64::createMoveInst(DataType* type, Register dst, Register src)
{
    assert(dst.data_type == src.data_type);
    assert(dst.data_type == type);

    MoveInst* inst = new MoveInst(type, new RegOperand(src), new RegOperand(dst));
    return inst;
}
MoveInst* Backend::RV64::createMoveInst(DataType* type, Register dst, int src)
{
    assert(dst.data_type == type);
    assert(type->data_type == DataType::INT);

    MoveInst* inst = new MoveInst(type, new ImmeI32Operand(src), new RegOperand(dst));
    return inst;
}
MoveInst* Backend::RV64::createMoveInst(DataType* type, Register dst, float src)
{
    assert(dst.data_type == type);
    assert(type->data_type == DataType::FLOAT);

    MoveInst* inst = new MoveInst(type, new ImmeF32Operand(src), new RegOperand(dst));
    return inst;
}
MoveInst* Backend::RV64::createMoveInst(DataType* type, Register dst, double src)
{
    assert(dst.data_type == type);
    assert(type->data_type == DataType::FLOAT);

    MoveInst* inst = new MoveInst(type, new ImmeF64Operand(src), new RegOperand(dst));
    return inst;
}

RV64Inst* Backend::RV64::createCallInst(RV64InstType op, std::string name, int ireg_para_cnt, int freg_para_cnt)
{
    RV64Inst* inst = new RV64Inst();

    inst->op        = op;
    inst->use_label = true;
    inst->label     = RV64Label(name, false);
    auto it         = opInfoTable.find(op);
    assert(it != opInfoTable.end() && it->second.type == RV64OpType::CALL);
    inst->call_ireg_cnt = ireg_para_cnt;
    inst->call_freg_cnt = freg_para_cnt;

    return inst;
}