#include <backend/rv64/selector.h>
#include <backend/rv64/insts.h>
#include <cfg.h>
#include <llvm_ir/defs.h>
#include <llvm_ir/instruction.h>
#include <vector>
using namespace Backend::RV64;
using namespace std;
using LOC = LLVMIR::IROpCode;

#define IS_BR(inst) (inst->opcode == LLVMIR::IROpCode::BR_COND || inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
#define IS_RET(inst) (inst->opcode == LLVMIR::IROpCode::RET)

#define IS_ALLOC(inst) (inst->opcode == LLVMIR::IROpCode::ALLOCA)
#define IS_LOAD(inst) (inst->opcode == LLVMIR::IROpCode::LOAD)
#define IS_STORE(inst) (inst->opcode == LLVMIR::IROpCode::STORE)
#define IS_PHI(inst) (inst->opcode == LLVMIR::IROpCode::PHI)
#define IS_CALL(inst) (inst->opcode == LLVMIR::IROpCode::CALL)
#define IS_RET(inst) (inst->opcode == LLVMIR::IROpCode::RET)

#define IS_REG(operand) (operand->type == LLVMIR::OperandType::REG)
#define IS_GLOBAL(operand) (operand->type == LLVMIR::OperandType::GLOBAL)
#define IS_IMMEI32(operand) (operand->type == LLVMIR::OperandType::IMMEI32)
#define IS_IMMEF32(operand) (operand->type == LLVMIR::OperandType::IMMEF32)

Selector::Selector(LLVMIR::IR* _ir, std::vector<Function*>& funcs, std::vector<LLVMIR::Instruction*>& _glb_defs)
    : ir(_ir), functions(funcs), glb_defs(_glb_defs), cur_func(nullptr), cur_block(nullptr), cur_offset(0)
{}

#define FUNC_CLEAR_STATE     \
    ir_reg_mapper.clear();   \
    alloc_shift_map.clear(); \
    cmp_context.clear();     \
    cur_offset = 0;

void Selector::selectInstruction()
{
    for (auto& func : ir->functions)
    {
        FUNC_CLEAR_STATE

        const string& func_name = func->func_def->func_name;
        ::CFG*        ir_cfg    = ir->cfg[func->func_def];
        RV64::CFG*    mcfg      = new RV64::CFG();

        cur_func       = nullptr;
        cur_func       = new Function(func_name, mcfg);
        cur_func->unit = nullptr;  // 暂时设为 nullptr，后续可能完全移除这个字段

        functions.push_back(cur_func);

        LLVMIR::FuncDefInst* ir_func    = func->func_def;
        size_t               param_size = ir_func->arg_types.size();
        for (size_t i = 0; i < param_size; ++i)
        {
            assert(IS_REG(ir_func->arg_regs[i]));
            DataType* type = nullptr;
            if (ir_func->arg_types[i] == LLVMIR::DataType::I32 || ir_func->arg_types[i] == LLVMIR::DataType::PTR)
                type = INT64;
            else if (ir_func->arg_types[i] == LLVMIR::DataType::F32)
                type = FLOAT64;
            else
                assert(false);
            cur_func->params.push_back(getLLVMReg(((LLVMIR::RegOperand*)ir_func->arg_regs[i])->reg_num, type));
        }

        for (auto ir_block : func->blocks)
        {
            cur_block = nullptr;
            cur_block = new Block(ir_block->block_id);

            mcfg->addNewBlock(ir_block->block_id, cur_block);
            if (ir_block->block_id > cur_func->cur_max_label) cur_func->cur_max_label = ir_block->block_id;
            // Note: ret_block logic needs to be adapted based on the new CFG structure

            cur_block->parent = cur_func;
            cur_func->blocks.push_back(cur_block);

            for (auto& inst : ir_block->insts) convertAndAppend(inst);
        }

        if (cur_offset % 8) cur_offset = ((cur_offset + 7) / 8) * 8;
        cur_func->stack_size = cur_offset + cur_func->param_size;
        for (auto& alloc_inst : cur_func->alloc_insts) alloc_inst->imme += cur_func->param_size;

        // Build control flow graph using the new CFG structure
        if (ir_cfg != nullptr)
        {
            for (size_t i = 0; i < ir_cfg->G_id.size(); ++i)
            {
                const vector<int>& tos = ir_cfg->G_id[i];
                for (const auto& to : tos) mcfg->makeEdge(i, to);
            }
        }
    }
}

Register Selector::getReg(DataType* type) { return cur_func->getNewReg(type); }
Register Selector::getLLVMReg(int ir_reg, DataType* type)
{
    if (ir_reg_mapper.find(ir_reg) == ir_reg_mapper.end())
    {
        Register rv64_reg     = getReg(type);
        ir_reg_mapper[ir_reg] = rv64_reg;
    }
    assert(ir_reg_mapper[ir_reg].data_type == type);
    return ir_reg_mapper[ir_reg];
}

Register Selector::extractIROp2Reg(LLVMIR::Operand* op, DataType* type)
{
    if (IS_REG(op))
    {
        int reg_num = ((LLVMIR::RegOperand*)op)->reg_num;
        return getLLVMReg(reg_num, type);
    }
    else if (IS_IMMEI32(op))
    {
        int      val = ((LLVMIR::ImmeI32Operand*)op)->value;
        Register reg = getReg(INT64);
        cur_block->insts.push_back(createMoveInst(INT64, reg, val));
        return reg;
    }
    else if (IS_IMMEF32(op))
    {
        float    val = ((LLVMIR::ImmeF32Operand*)op)->value;
        Register reg = getReg(FLOAT64);
        cur_block->insts.push_back(createMoveInst(FLOAT64, reg, val));
        return reg;
    }
    else
        assert(false);
}
int Selector::extractIROp2ImmeI32(LLVMIR::Operand* op)
{
    assert(IS_IMMEI32(op));
    return ((LLVMIR::ImmeI32Operand*)op)->value;
}
float Selector::extractIROp2ImmeF32(LLVMIR::Operand* op)
{
    assert(IS_IMMEF32(op));
    return ((LLVMIR::ImmeF32Operand*)op)->value;
}

void Selector::convertAndAppend(LLVMIR::Instruction* inst)
{
    switch (inst->opcode)
    {
        case LOC::LOAD: convertAndAppend((LLVMIR::LoadInst*)inst); break;
        case LOC::STORE: convertAndAppend((LLVMIR::StoreInst*)inst); break;
        case LOC::ICMP: convertAndAppend((LLVMIR::IcmpInst*)inst); break;
        case LOC::FCMP: convertAndAppend((LLVMIR::FcmpInst*)inst); break;
        case LOC::ALLOCA: convertAndAppend((LLVMIR::AllocInst*)inst); break;
        case LOC::BR_COND: convertAndAppend((LLVMIR::BranchCondInst*)inst); break;
        case LOC::BR_UNCOND: convertAndAppend((LLVMIR::BranchUncondInst*)inst); break;
        case LOC::RET: convertAndAppend((LLVMIR::RetInst*)inst); break;
        case LOC::ZEXT: convertAndAppend((LLVMIR::ZextInst*)inst); break;
        case LOC::FPTOSI: convertAndAppend((LLVMIR::FP2SIInst*)inst); break;
        case LOC::SITOFP: convertAndAppend((LLVMIR::SI2FPInst*)inst); break;
        case LOC::FPEXT: convertAndAppend((LLVMIR::FPExtInst*)inst); break;
        case LOC::GETELEMENTPTR: convertAndAppend((LLVMIR::GEPInst*)inst); break;
        case LOC::PHI: convertAndAppend((LLVMIR::PhiInst*)inst); break;
        case LOC::CALL: convertAndAppend((LLVMIR::CallInst*)inst); break;
        case LOC::ADD:
        case LOC::SUB:
        case LOC::MUL:
        case LOC::DIV:
        case LOC::FADD:
        case LOC::FSUB:
        case LOC::FMUL:
        case LOC::FDIV:
        case LOC::MOD:
        case LOC::SHL:
        case LOC::BITXOR: convertAndAppend((LLVMIR::ArithmeticInst*)inst); break;
        default: cerr << "Unknown opcode: " << inst->opcode << endl; assert(false);
    }
}
void Selector::convertAndAppend(LLVMIR::LoadInst* inst)
{
    if (IS_GLOBAL(inst->ptr))
    {
        LLVMIR::GlobalOperand* global_op = (LLVMIR::GlobalOperand*)inst->ptr;
        LLVMIR::RegOperand*    rd_op     = (LLVMIR::RegOperand*)inst->res;

        Register hi       = getReg(INT64);
        int      ir_regno = rd_op->reg_num;

        if (inst->type == LLVMIR::DataType::I32)
        {
            Register rd = getLLVMReg(ir_regno, INT64);
            cur_block->insts.push_back(
                createUInst(RV64InstType::LA, hi, RV64Label(global_op->global_name, false, true)));
            cur_block->insts.push_back(createIInst(RV64InstType::LW, rd, hi, 0));
        }
        else if (inst->type == LLVMIR::DataType::F32)
        {
            Register rd = getLLVMReg(ir_regno, FLOAT64);
            cur_block->insts.push_back(
                createUInst(RV64InstType::LA, hi, RV64Label(global_op->global_name, false, true)));
            cur_block->insts.push_back(createIInst(RV64InstType::FLW, rd, hi, 0));
        }
        else if (inst->type == LLVMIR::DataType::PTR)
        {
            Register rd = getLLVMReg(ir_regno, INT64);
            cur_block->insts.push_back(
                createUInst(RV64InstType::LA, hi, RV64Label(global_op->global_name, false, true)));
            cur_block->insts.push_back(createIInst(RV64InstType::LD, rd, hi, 0));
        }
        else
            assert(false);

        return;
    }
    else if (!IS_REG(inst->ptr))
        assert(false);

    // REG
    LLVMIR::RegOperand* ptr_op = (LLVMIR::RegOperand*)inst->ptr;
    LLVMIR::RegOperand* rd_op  = (LLVMIR::RegOperand*)inst->res;

    int ptr_ir_regno = ptr_op->reg_num;
    int ir_regno     = rd_op->reg_num;

    if (inst->type == LLVMIR::DataType::I32)
    {
        Register rd = getLLVMReg(ir_regno, INT64);
        auto     it = alloc_shift_map.find(ptr_ir_regno);
        if (it == alloc_shift_map.end())
        {
            Register ptr = getLLVMReg(ptr_ir_regno, INT64);
            cur_block->insts.push_back(createIInst(RV64InstType::LW, rd, ptr, 0));
            return;
        }

        int offset = it->second;
        // RV64Inst* lw     = createIInst(RV64InstType::LW, rd, preg_sp, offset);
        RV64Inst* lw = nullptr;

        if (offset >= -2048 && offset <= 2047)
            lw = createIInst(RV64InstType::LW, rd, preg_sp, offset);
        else
        {
            /*
            Register shift_reg = getReg(INT64);
            cur_block->insts.push_back(createUInst(RV64InstType::LUI, shift_reg, offset >> 12));
            assert(((offset << 20) >> 20) <= 2047);
            cur_block->insts.push_back(createIInst(RV64InstType::ADDI, shift_reg, shift_reg, (offset << 20) >> 20));
            Register ptr = getReg(INT64);
            cur_block->insts.push_back(createRInst(RV64InstType::ADD, ptr, preg_sp, shift_reg));
            lw = createIInst(RV64InstType::LW, rd, ptr, 0);
            */
            Register num_reg = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset));
            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
            lw = createIInst(RV64InstType::LW, rd, num_reg, 0);
        }

        cur_func->alloc_insts.push_back(lw);
        cur_block->insts.push_back(lw);
    }
    else if (inst->type == LLVMIR::DataType::F32)
    {
        Register rd = getLLVMReg(ir_regno, FLOAT64);
        auto     it = alloc_shift_map.find(ptr_ir_regno);
        if (it == alloc_shift_map.end())
        {
            Register ptr = getLLVMReg(ptr_ir_regno, INT64);
            cur_block->insts.push_back(createIInst(RV64InstType::FLW, rd, ptr, 0));
            return;
        }

        int offset = it->second;
        // RV64Inst* lw     = createIInst(RV64InstType::FLW, rd, preg_sp, offset);
        RV64Inst* lw = nullptr;

        if (offset >= -2048 && offset <= 2047)
            lw = createIInst(RV64InstType::FLW, rd, preg_sp, offset);
        else
        {
            /*
            Register shift_reg = getReg(INT64);
            cur_block->insts.push_back(createUInst(RV64InstType::LUI, shift_reg, offset >> 12));
            assert(((offset << 20) >> 20) <= 2047);
            cur_block->insts.push_back(createIInst(RV64InstType::ADDI, shift_reg, shift_reg, (offset << 20) >> 20));
            Register ptr = getReg(INT64);
            cur_block->insts.push_back(createRInst(RV64InstType::ADD, ptr, preg_sp, shift_reg));
            lw = createIInst(RV64InstType::FLW, rd, ptr, 0);
            */
            Register num_reg = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset));
            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
            lw = createIInst(RV64InstType::FLW, rd, num_reg, 0);
        }

        cur_func->alloc_insts.push_back(lw);
        cur_block->insts.push_back(lw);
    }
    else if (inst->type == LLVMIR::DataType::PTR)
    {
        Register rd = getLLVMReg(ir_regno, INT64);
        auto     it = alloc_shift_map.find(ptr_ir_regno);
        if (it == alloc_shift_map.end())
        {
            Register ptr = getLLVMReg(ptr_ir_regno, INT64);
            cur_block->insts.push_back(createIInst(RV64InstType::LD, rd, ptr, 0));
            return;
        }

        int       offset = it->second;
        RV64Inst* lw     = createIInst(RV64InstType::LD, rd, preg_sp, offset);
        cur_func->alloc_insts.push_back(lw);
        cur_block->insts.push_back(lw);
    }
    else
        assert(false);
}
void Selector::convertAndAppend(LLVMIR::StoreInst* inst)
{
    Register val_reg;

    if (IS_REG(inst->val))
    {
        int              reg_num  = ((LLVMIR::RegOperand*)inst->val)->reg_num;
        LLVMIR::DataType val_type = inst->type;
        if (val_type == LLVMIR::DataType::I32)
            val_reg = getLLVMReg(reg_num, INT64);
        else if (val_type == LLVMIR::DataType::F32)
            val_reg = getLLVMReg(reg_num, FLOAT64);
        else
            assert(false);
    }
    else if (IS_IMMEI32(inst->val))
    {
        val_reg = getReg(INT64);
        int val = ((LLVMIR::ImmeI32Operand*)inst->val)->value;
        cur_block->insts.push_back(createMoveInst(INT64, val_reg, val));
        // cur_block->insts.push_back(createIInst(RV64InstType::ADDI, val_reg, preg_x0, val));
    }
    else if (IS_IMMEF32(inst->val))
    {
        val_reg   = getReg(FLOAT64);
        float val = ((LLVMIR::ImmeF32Operand*)inst->val)->value;
        cur_block->insts.push_back(createMoveInst(INT64, val_reg, val));
        // cur_block->insts.push_back(createIInst(RV64InstType::FCVT_S_W, val_reg, preg_x0, val));
    }
    else
        assert(false);

    if (IS_GLOBAL(inst->ptr))
    {
        LLVMIR::GlobalOperand* global_op = (LLVMIR::GlobalOperand*)inst->ptr;

        Register hi = getReg(INT64);

        cur_block->insts.push_back(createUInst(RV64InstType::LA, hi, RV64Label(global_op->global_name, false, true)));
        if (inst->type == LLVMIR::DataType::I32)
            cur_block->insts.push_back(createSInst(RV64InstType::SW, val_reg, hi, 0));
        else if (inst->type == LLVMIR::DataType::F32)
            cur_block->insts.push_back(createSInst(RV64InstType::FSW, val_reg, hi, 0));
        else
            assert(false);

        return;
    }
    else if (!IS_REG(inst->ptr))
        assert(false);

    // REG
    LLVMIR::RegOperand* ptr_op       = (LLVMIR::RegOperand*)inst->ptr;
    int                 ptr_ir_regno = ptr_op->reg_num;

    auto it = alloc_shift_map.find(ptr_ir_regno);
    if (inst->type == LLVMIR::DataType::I32)
    {
        if (it == alloc_shift_map.end())
        {
            Register ptr = getLLVMReg(ptr_ir_regno, INT64);
            cur_block->insts.push_back(createSInst(RV64InstType::SW, val_reg, ptr, 0));
            return;
        }

        int offset = it->second;
        // RV64Inst* sw     = createSInst(RV64InstType::SW, val_reg, preg_sp, offset);
        RV64Inst* sw = nullptr;
        if (offset >= -2048 && offset <= 2047)
            sw = createSInst(RV64InstType::SW, val_reg, preg_sp, offset);
        else
        {
            /*
            Register shift_reg = getReg(INT64);
            cur_block->insts.push_back(createUInst(RV64InstType::LUI, shift_reg, ((unsigned int)offset + 0x800) >> 12));
            assert(((offset << 20) >> 20) <= 2047);
            cur_block->insts.push_back(createIInst(RV64InstType::ADDI, shift_reg, shift_reg, (offset << 20) >> 20));
            Register ptr_reg = getReg(INT64);
            cur_block->insts.push_back(createRInst(RV64InstType::ADD, ptr_reg, preg_sp, shift_reg));
            sw = createSInst(RV64InstType::SW, val_reg, ptr_reg, 0);
            */
            Register num_reg = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset));
            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
            sw = createSInst(RV64InstType::SW, val_reg, num_reg, 0);
        }

        cur_func->alloc_insts.push_back(sw);
        cur_block->insts.push_back(sw);
    }
    else if (inst->type == LLVMIR::DataType::F32)
    {
        if (it == alloc_shift_map.end())
        {
            Register ptr = getLLVMReg(ptr_ir_regno, INT64);
            cur_block->insts.push_back(createSInst(RV64InstType::FSW, val_reg, ptr, 0));
            return;
        }

        int offset = it->second;
        // RV64Inst* sw     = createSInst(RV64InstType::FSW, val_reg, preg_sp, offset);
        RV64Inst* sw = nullptr;

        if (offset >= -2048 && offset <= 2047)
            sw = createSInst(RV64InstType::FSW, val_reg, preg_sp, offset);
        else
        {
            /*
            Register shift_reg = getReg(INT64);
            cur_block->insts.push_back(createUInst(RV64InstType::LUI, shift_reg, ((unsigned int)offset + 0x800) >> 12));
            assert(((offset << 20) >> 20) <= 2047);
            cur_block->insts.push_back(createIInst(RV64InstType::ADDI, shift_reg, shift_reg, (offset << 20) >> 20));
            Register ptr_reg = getReg(INT64);
            cur_block->insts.push_back(createRInst(RV64InstType::ADD, ptr_reg, preg_sp, shift_reg));
            sw = createSInst(RV64InstType::FSW, val_reg, ptr_reg, 0);
            */
            Register num_reg = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset));
            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
            sw = createSInst(RV64InstType::FSW, val_reg, num_reg, 0);
        }

        cur_func->alloc_insts.push_back(sw);
        cur_block->insts.push_back(sw);
    }
    else
        assert(false);
}
void Selector::convertADD(LLVMIR::ArithmeticInst* inst)
{
    if (inst->type != LLVMIR::DataType::I32) assert(false);

    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEI32;

    // REG + REG
    if (lhs_type == ir_reg_type && lhs_type == rhs_type)
    {
        int lhs_reg = ((LLVMIR::RegOperand*)inst->lhs)->reg_num;
        int rhs_reg = ((LLVMIR::RegOperand*)inst->rhs)->reg_num;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register lhs = getLLVMReg(lhs_reg, INT64);
        Register rhs = getLLVMReg(rhs_reg, INT64);
        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createRInst(RV64InstType::ADD, res, lhs, rhs));
    }
    // REG + IMME
    else if (lhs_type == ir_reg_type && rhs_type == imme_type)
    {
        int lhs_reg = ((LLVMIR::RegOperand*)inst->lhs)->reg_num;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register lhs = getLLVMReg(lhs_reg, INT64);
        Register res = getLLVMReg(res_reg, INT64);

        assert(rhs_val >= -2048 && rhs_val <= 2047);
        cur_block->insts.push_back(createIInst(RV64InstType::ADDI, res, lhs, rhs_val));
    }
    // IMME + REG
    else if (lhs_type == imme_type && rhs_type == ir_reg_type)
    {
        int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
        int rhs_reg = ((LLVMIR::RegOperand*)inst->rhs)->reg_num;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register rhs = getLLVMReg(rhs_reg, INT64);
        Register res = getLLVMReg(res_reg, INT64);

        assert(lhs_val >= -2048 && lhs_val <= 2047);
        cur_block->insts.push_back(createIInst(RV64InstType::ADDI, res, rhs, lhs_val));
    }
    // IMME + IMME
    else if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createMoveInst(INT64, res, lhs_val + rhs_val));
        // cur_block->insts.push_back(createIInst(RV64InstType::ADDI, res, preg_x0, lhs_val + rhs_val));
    }

    else
        assert(false);
}
void Selector::convertSUB(LLVMIR::ArithmeticInst* inst)
{
    if (inst->type != LLVMIR::DataType::I32) assert(false);

    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEI32;

    // IMME - IMME
    if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createMoveInst(INT64, res, lhs_val - rhs_val));
    }
    // REG - IMME
    else if (lhs_type == ir_reg_type && rhs_type == imme_type)
    {
        int lhs_reg = ((LLVMIR::RegOperand*)inst->lhs)->reg_num;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register lhs = getLLVMReg(lhs_reg, INT64);
        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createIInst(RV64InstType::ADDI, res, lhs, -rhs_val));
    }
    else
    {
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register lhs = extractIROp2Reg(inst->lhs, INT64);
        Register rhs = extractIROp2Reg(inst->rhs, INT64);
        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createRInst(RV64InstType::SUB, res, lhs, rhs));
    }

    /*
        // REG - REG
        if (lhs_type == ir_reg_type && lhs_type == rhs_type)
        {
            int lhs_reg = ((LLVMIR::RegOperand*)inst->lhs)->reg_num;
            int rhs_reg = ((LLVMIR::RegOperand*)inst->rhs)->reg_num;
            int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

            Register lhs = getLLVMReg(lhs_reg, INT64);
            Register rhs = getLLVMReg(rhs_reg, INT64);
            Register res = getLLVMReg(res_reg, INT64);

            cur_block->insts.push_back(createRInst(RV64InstType::SUB, res, lhs, rhs));
        }
        // REG - IMME
        else if (lhs_type == ir_reg_type && rhs_type == imme_type)
        {
            int lhs_reg = ((LLVMIR::RegOperand*)inst->lhs)->reg_num;
            int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
            int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

            Register lhs = getLLVMReg(lhs_reg, INT64);
            Register res = getLLVMReg(res_reg, INT64);

            cur_block->insts.push_back(createIInst(RV64InstType::ADDI, res, lhs, -rhs_val));
        }
        // IMME - IMME
        else if (lhs_type == imme_type && lhs_type == rhs_type)
        {
            int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
            int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
            int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

            Register res = getLLVMReg(res_reg, INT64);

            cur_block->insts.push_back(createMoveInst(INT64, res, lhs_val - rhs_val));
        }
        else
            assert(false);
    */
}
void Selector::convertMUL(LLVMIR::ArithmeticInst* inst)
{
    if (inst->type != LLVMIR::DataType::I32) assert(false);

    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEI32;

    // IMME * IMME
    if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createMoveInst(INT64, res, lhs_val * rhs_val));
    }
    else
    {
        assert(IS_REG(inst->res));

        Register rd = getLLVMReg(((LLVMIR::RegOperand*)inst->res)->reg_num, INT64);
        Register lhs, rhs;

        if (IS_IMMEI32(inst->lhs))
        {
            int val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
            lhs     = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, lhs, val));
        }
        else
            lhs = getLLVMReg(((LLVMIR::RegOperand*)inst->lhs)->reg_num, INT64);
        if (IS_IMMEI32(inst->rhs))
        {
            int val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
            rhs     = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, rhs, val));
        }
        else
            rhs = getLLVMReg(((LLVMIR::RegOperand*)inst->rhs)->reg_num, INT64);

        cur_block->insts.push_back(createRInst(RV64InstType::MULW, rd, lhs, rhs));
    }
}
void Selector::convertDIV(LLVMIR::ArithmeticInst* inst)
{
    if (inst->type != LLVMIR::DataType::I32) assert(false);

    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEI32;

    // IMME / IMME
    if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createMoveInst(INT64, res, lhs_val / rhs_val));
    }
    else
    {
        assert(IS_REG(inst->res));

        Register rd = getLLVMReg(((LLVMIR::RegOperand*)inst->res)->reg_num, INT64);
        Register lhs, rhs;

        if (IS_IMMEI32(inst->lhs))
        {
            int val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
            lhs     = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, lhs, val));
        }
        else
            lhs = getLLVMReg(((LLVMIR::RegOperand*)inst->lhs)->reg_num, INT64);
        if (IS_IMMEI32(inst->rhs))
        {
            int val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
            rhs     = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, rhs, val));
        }
        else
            rhs = getLLVMReg(((LLVMIR::RegOperand*)inst->rhs)->reg_num, INT64);

        cur_block->insts.push_back(createRInst(RV64InstType::DIVW, rd, lhs, rhs));
    }
}
void Selector::convertFADD(LLVMIR::ArithmeticInst* inst)
{
    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEF32;

    // IMME + IMME
    if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        float lhs_val = ((LLVMIR::ImmeF32Operand*)inst->lhs)->value;
        float rhs_val = ((LLVMIR::ImmeF32Operand*)inst->rhs)->value;
        int   res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, FLOAT64);

        cur_block->insts.push_back(createMoveInst(FLOAT64, res, lhs_val + rhs_val));
    }
    else
    {
        Register rd = getLLVMReg(((LLVMIR::RegOperand*)inst->res)->reg_num, FLOAT64);

        cur_block->insts.push_back(createRInst(
            RV64InstType::FADD_S, rd, extractIROp2Reg(inst->lhs, FLOAT64), extractIROp2Reg(inst->rhs, FLOAT64)));
    }
}
void Selector::convertFSUB(LLVMIR::ArithmeticInst* inst)
{
    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEF32;

    // IMME - IMME
    if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        float lhs_val = ((LLVMIR::ImmeF32Operand*)inst->lhs)->value;
        float rhs_val = ((LLVMIR::ImmeF32Operand*)inst->rhs)->value;
        int   res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, FLOAT64);

        cur_block->insts.push_back(createMoveInst(FLOAT64, res, lhs_val - rhs_val));
        return;
    }

    Register rd = getLLVMReg(((LLVMIR::RegOperand*)inst->res)->reg_num, FLOAT64);

    LLVMIR::Operand* lhs_op = inst->lhs;
    LLVMIR::Operand* rhs_op = inst->rhs;

    if (IS_IMMEF32(lhs_op))
    {
        float val = ((LLVMIR::ImmeF32Operand*)lhs_op)->value;
        if (val == 0)
        {
            cur_block->insts.push_back(createR2Inst(RV64InstType::FNEG_S, rd, extractIROp2Reg(rhs_op, FLOAT64)));
            return;
        }
    }
    else if (IS_IMMEF32(rhs_op))
    {
        float val = ((LLVMIR::ImmeF32Operand*)rhs_op)->value;
        if (val == 0)
        {
            cur_block->insts.push_back(createMoveInst(FLOAT64, rd, extractIROp2Reg(lhs_op, FLOAT64)));
            return;
        }
    }

    Register lhs, rhs;

    if (IS_IMMEF32(lhs_op))
    {
        float val = ((LLVMIR::ImmeF32Operand*)lhs_op)->value;
        lhs       = getReg(FLOAT64);
        cur_block->insts.push_back(createMoveInst(FLOAT64, lhs, val));
    }
    else if (IS_REG(lhs_op))
        lhs = getLLVMReg(((LLVMIR::RegOperand*)lhs_op)->reg_num, FLOAT64);
    else
        assert(false);

    if (IS_IMMEF32(rhs_op))
    {
        float val = ((LLVMIR::ImmeF32Operand*)rhs_op)->value;
        rhs       = getReg(FLOAT64);
        cur_block->insts.push_back(createMoveInst(FLOAT64, rhs, val));
    }
    else if (IS_REG(rhs_op))
        rhs = getLLVMReg(((LLVMIR::RegOperand*)rhs_op)->reg_num, FLOAT64);
    else
        assert(false);

    cur_block->insts.push_back(createRInst(RV64InstType::FSUB_S, rd, lhs, rhs));
}
void Selector::convertFMUL(LLVMIR::ArithmeticInst* inst)
{
    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEF32;

    // IMME * IMME
    if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        float lhs_val = ((LLVMIR::ImmeF32Operand*)inst->lhs)->value;
        float rhs_val = ((LLVMIR::ImmeF32Operand*)inst->rhs)->value;
        int   res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, FLOAT64);

        cur_block->insts.push_back(createMoveInst(FLOAT64, res, lhs_val * rhs_val));
    }
    else
    {
        Register rd = getLLVMReg(((LLVMIR::RegOperand*)inst->res)->reg_num, FLOAT64);
        Register lhs, rhs;

        if (IS_IMMEF32(inst->lhs))
        {
            float val = ((LLVMIR::ImmeF32Operand*)inst->lhs)->value;
            lhs       = getReg(FLOAT64);
            cur_block->insts.push_back(createMoveInst(FLOAT64, lhs, val));
        }
        else if (IS_REG(inst->lhs))
            lhs = getLLVMReg(((LLVMIR::RegOperand*)inst->lhs)->reg_num, FLOAT64);
        else
            assert(false);
        if (IS_IMMEF32(inst->rhs))
        {
            float val = ((LLVMIR::ImmeF32Operand*)inst->rhs)->value;
            rhs       = getReg(FLOAT64);
            cur_block->insts.push_back(createMoveInst(FLOAT64, rhs, val));
        }
        else if (IS_REG(inst->rhs))
            rhs = getLLVMReg(((LLVMIR::RegOperand*)inst->rhs)->reg_num, FLOAT64);
        else
            assert(false);

        cur_block->insts.push_back(createRInst(RV64InstType::FMUL_S, rd, lhs, rhs));
    }
}
void Selector::convertFDIV(LLVMIR::ArithmeticInst* inst)
{
    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEF32;

    // IMME / IMME
    if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        float lhs_val = ((LLVMIR::ImmeF32Operand*)inst->lhs)->value;
        float rhs_val = ((LLVMIR::ImmeF32Operand*)inst->rhs)->value;
        int   res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, FLOAT64);

        cur_block->insts.push_back(createMoveInst(FLOAT64, res, lhs_val / rhs_val));

        return;
    }

    Register rd = getLLVMReg(((LLVMIR::RegOperand*)inst->res)->reg_num, FLOAT64);
    Register lhs, rhs;

    if (IS_IMMEF32(inst->lhs))
    {
        float val = ((LLVMIR::ImmeF32Operand*)inst->lhs)->value;
        lhs       = getReg(FLOAT64);
        cur_block->insts.push_back(createMoveInst(FLOAT64, lhs, val));
    }
    else if (IS_REG(inst->lhs))
        lhs = getLLVMReg(((LLVMIR::RegOperand*)inst->lhs)->reg_num, FLOAT64);
    else
        assert(false);

    if (IS_IMMEF32(inst->rhs))
    {
        float val = ((LLVMIR::ImmeF32Operand*)inst->rhs)->value;
        rhs       = getReg(FLOAT64);
        cur_block->insts.push_back(createMoveInst(FLOAT64, rhs, val));
    }
    else if (IS_REG(inst->rhs))
        rhs = getLLVMReg(((LLVMIR::RegOperand*)inst->rhs)->reg_num, FLOAT64);
    else
        assert(false);

    cur_block->insts.push_back(createRInst(RV64InstType::FDIV_S, rd, lhs, rhs));
}
void Selector::convertMOD(LLVMIR::ArithmeticInst* inst)
{
    if (inst->type != LLVMIR::DataType::I32) assert(false);

    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEI32;

    // IMME / IMME
    if (lhs_type == imme_type && lhs_type == rhs_type)
    {
        int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createMoveInst(INT64, res, lhs_val % rhs_val));
    }
    else
    {
        assert(IS_REG(inst->res));

        Register rd = getLLVMReg(((LLVMIR::RegOperand*)inst->res)->reg_num, INT64);
        Register lhs, rhs;

        if (IS_IMMEI32(inst->lhs))
        {
            int val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
            lhs     = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, lhs, val));
        }
        else
            lhs = getLLVMReg(((LLVMIR::RegOperand*)inst->lhs)->reg_num, INT64);
        if (IS_IMMEI32(inst->rhs))
        {
            int val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
            rhs     = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, rhs, val));
        }
        else
            rhs = getLLVMReg(((LLVMIR::RegOperand*)inst->rhs)->reg_num, INT64);

        cur_block->insts.push_back(createRInst(RV64InstType::REM, rd, lhs, rhs));
    }
}
void Selector::convertSHL(LLVMIR::ArithmeticInst* inst)
{
    std::cerr << "Not implemented yet" << std::endl;
    assert(false);
}
void Selector::convertBITXOR(LLVMIR::ArithmeticInst* inst)
{
    if (inst->type != LLVMIR::DataType::I32) assert(false);

    LLVMIR::OperandType lhs_type = inst->lhs->type;
    LLVMIR::OperandType rhs_type = inst->rhs->type;

    LLVMIR::OperandType ir_reg_type = LLVMIR::OperandType::REG;
    LLVMIR::OperandType imme_type   = LLVMIR::OperandType::IMMEI32;

    // REG ^ REG
    if (lhs_type == ir_reg_type && lhs_type == rhs_type)
    {
        int lhs_reg = ((LLVMIR::RegOperand*)inst->lhs)->reg_num;
        int rhs_reg = ((LLVMIR::RegOperand*)inst->rhs)->reg_num;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register lhs = getLLVMReg(lhs_reg, INT64);
        Register rhs = getLLVMReg(rhs_reg, INT64);
        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createRInst(RV64InstType::XOR, res, lhs, rhs));
    }
    // REG ^ IMME
    else if (lhs_type == ir_reg_type && rhs_type == imme_type)
    {
        int lhs_reg = ((LLVMIR::RegOperand*)inst->lhs)->reg_num;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register lhs = getLLVMReg(lhs_reg, INT64);
        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createIInst(RV64InstType::XORI, res, lhs, rhs_val));
    }
    // IMME ^ REG
    else if (lhs_type == imme_type && rhs_type == ir_reg_type)
    {
        int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
        int rhs_reg = ((LLVMIR::RegOperand*)inst->rhs)->reg_num;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register rhs = getLLVMReg(rhs_reg, INT64);
        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createIInst(RV64InstType::XORI, res, rhs, lhs_val));
    }
    // IMME ^ IMME
    else
    {
        int lhs_val = ((LLVMIR::ImmeI32Operand*)inst->lhs)->value;
        int rhs_val = ((LLVMIR::ImmeI32Operand*)inst->rhs)->value;
        int res_reg = ((LLVMIR::RegOperand*)inst->res)->reg_num;

        Register res = getLLVMReg(res_reg, INT64);

        cur_block->insts.push_back(createMoveInst(INT64, res, lhs_val ^ rhs_val));
    }
}
void Selector::convertAndAppend(LLVMIR::ArithmeticInst* inst)
{
    switch (inst->opcode)
    {
        case LOC::ADD: convertADD(inst); break;
        case LOC::SUB: convertSUB(inst); break;
        case LOC::MUL: convertMUL(inst); break;
        case LOC::DIV: convertDIV(inst); break;
        case LOC::FADD: convertFADD(inst); break;
        case LOC::FSUB: convertFSUB(inst); break;
        case LOC::FMUL: convertFMUL(inst); break;
        case LOC::FDIV: convertFDIV(inst); break;
        case LOC::MOD: convertMOD(inst); break;
        case LOC::SHL: convertSHL(inst); break;
        case LOC::BITXOR: convertBITXOR(inst); break;
        default: cerr << "Unknown opcode: " << inst->opcode << endl; assert(false);
    }
}
void Selector::convertAndAppend(LLVMIR::IcmpInst* inst)
{
    assert(IS_REG(inst->res));
    LLVMIR::RegOperand* res_op  = (LLVMIR::RegOperand*)inst->res;
    int                 ir_reg  = res_op->reg_num;
    Register            res_reg = getLLVMReg(ir_reg, INT64);
    cmp_context[res_reg]        = inst;
}
void Selector::convertAndAppend(LLVMIR::FcmpInst* inst)
{
    assert(IS_REG(inst->res));
    LLVMIR::RegOperand* res_op  = (LLVMIR::RegOperand*)inst->res;
    int                 ir_reg  = res_op->reg_num;
    Register            res_reg = getLLVMReg(ir_reg, INT64);
    cmp_context[res_reg]        = inst;
}
void Selector::convertAndAppend(LLVMIR::AllocInst* inst)
{
    assert(IS_REG(inst->res));
    LLVMIR::RegOperand* res_op = (LLVMIR::RegOperand*)inst->res;
    int                 size   = 1;
    for (auto dim : inst->dims) size *= dim;
    size       = size << 2;
    int ir_reg = res_op->reg_num;

    alloc_shift_map[ir_reg] = cur_offset;
    cur_offset += size;
}
void Selector::convertAndAppend(LLVMIR::BranchCondInst* inst)
{
    LLVMIR::RegOperand*  cond_reg = (LLVMIR::RegOperand*)inst->cond;
    Register             br_reg   = getLLVMReg(cond_reg->reg_num, INT64);
    LLVMIR::Instruction* cmp_inst = cmp_context[br_reg];

    RV64InstType op;
    Register     cmp_rd, cmp_op1, cmp_op2;

    if (cmp_inst->opcode == LLVMIR::IROpCode::ICMP)
    {
        LLVMIR::IcmpInst* icmp_inst = (LLVMIR::IcmpInst*)cmp_inst;
        LLVMIR::Operand * ir_lhs = icmp_inst->lhs, *ir_rhs = icmp_inst->rhs, *ir_res = icmp_inst->res;

        if (IS_REG(ir_lhs))
            cmp_op1 = getLLVMReg(((LLVMIR::RegOperand*)ir_lhs)->reg_num, INT64);
        else if (IS_IMMEI32(ir_lhs))
        {
            int val = ((LLVMIR::ImmeI32Operand*)ir_lhs)->value;
            cmp_op1 = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, cmp_op1, val));
        }
        else
            assert(false);

        if (IS_REG(ir_rhs))
            cmp_op2 = getLLVMReg(((LLVMIR::RegOperand*)ir_rhs)->reg_num, INT64);
        else if (IS_IMMEI32(ir_rhs))
        {
            int val = ((LLVMIR::ImmeI32Operand*)ir_rhs)->value;
            cmp_op2 = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, cmp_op2, val));
        }
        else
            assert(false);

        switch (icmp_inst->cond)
        {
            case LLVMIR::IcmpCond::EQ: op = RV64InstType::BEQ; break;
            case LLVMIR::IcmpCond::NE: op = RV64InstType::BNE; break;
            case LLVMIR::IcmpCond::SGT: op = RV64InstType::BGT; break;
            case LLVMIR::IcmpCond::SGE: op = RV64InstType::BGE; break;
            case LLVMIR::IcmpCond::SLT: op = RV64InstType::BLT; break;
            case LLVMIR::IcmpCond::SLE: op = RV64InstType::BLE; break;
            case LLVMIR::IcmpCond::UGT: op = RV64InstType::BGTU; break;
            case LLVMIR::IcmpCond::UGE: op = RV64InstType::BGEU; break;
            case LLVMIR::IcmpCond::ULT: op = RV64InstType::BLTU; break;
            case LLVMIR::IcmpCond::ULE: op = RV64InstType::BLEU; break;
            default: assert(false);
        }
    }
    else if (cmp_inst->opcode == LLVMIR::IROpCode::FCMP)
    {
        LLVMIR::FcmpInst* fcmp_inst = (LLVMIR::FcmpInst*)cmp_inst;
        LLVMIR::Operand * ir_lhs = fcmp_inst->lhs, *ir_rhs = fcmp_inst->rhs, *ir_res = fcmp_inst->res;

        if (IS_REG(ir_lhs))
            cmp_op1 = getLLVMReg(((LLVMIR::RegOperand*)ir_lhs)->reg_num, FLOAT64);
        else if (IS_IMMEF32(ir_lhs))
        {
            float val = ((LLVMIR::ImmeF32Operand*)ir_lhs)->value;
            cmp_op1   = getReg(FLOAT64);
            cur_block->insts.push_back(createMoveInst(FLOAT64, cmp_op1, val));
        }
        else
            assert(false);

        if (IS_REG(ir_rhs))
            cmp_op2 = getLLVMReg(((LLVMIR::RegOperand*)ir_rhs)->reg_num, FLOAT64);
        else if (IS_IMMEF32(ir_rhs))
        {
            float val = ((LLVMIR::ImmeF32Operand*)ir_rhs)->value;
            cmp_op2   = getReg(FLOAT64);
            cur_block->insts.push_back(createMoveInst(FLOAT64, cmp_op2, val));
        }
        else
            assert(false);

        cmp_rd = getReg(INT64);

        LLVMIR::FcmpCond cond = fcmp_inst->cond;
        // cerr << "fcmp cond: " << static_cast<int>(cond) << endl;
        switch (cond)
        {
            case LLVMIR::FcmpCond::OEQ:
            case LLVMIR::FcmpCond::UEQ:
                cur_block->insts.push_back(createRInst(RV64InstType::FEQ_S, cmp_rd, cmp_op1, cmp_op2));
                op = RV64InstType::BNE;
                break;
            case LLVMIR::FcmpCond::OGT:
            case LLVMIR::FcmpCond::UGT:
                cur_block->insts.push_back(createRInst(RV64InstType::FLT_S, cmp_rd, cmp_op2, cmp_op1));
                op = RV64InstType::BNE;
                break;
            case LLVMIR::FcmpCond::OGE:
            case LLVMIR::FcmpCond::UGE:
                cur_block->insts.push_back(createRInst(RV64InstType::FLE_S, cmp_rd, cmp_op2, cmp_op1));
                op = RV64InstType::BNE;
                break;
            case LLVMIR::FcmpCond::OLT:
            case LLVMIR::FcmpCond::ULT:
                cur_block->insts.push_back(createRInst(RV64InstType::FLT_S, cmp_rd, cmp_op1, cmp_op2));
                op = RV64InstType::BNE;
                break;
            case LLVMIR::FcmpCond::OLE:
            case LLVMIR::FcmpCond::ULE:
                cur_block->insts.push_back(createRInst(RV64InstType::FLE_S, cmp_rd, cmp_op1, cmp_op2));
                op = RV64InstType::BNE;
                break;
            case LLVMIR::FcmpCond::ONE:
            case LLVMIR::FcmpCond::UNE:
                cur_block->insts.push_back(createRInst(RV64InstType::FEQ_S, cmp_rd, cmp_op1, cmp_op2));
                op = RV64InstType::BEQ;
                break;
            default: assert(false);
        }

        cmp_op1 = cmp_rd;
        cmp_op2 = preg_x0;
    }
    else
        assert(false);

    LLVMIR::LabelOperand* true_label  = (LLVMIR::LabelOperand*)inst->true_label;
    LLVMIR::LabelOperand* false_label = (LLVMIR::LabelOperand*)inst->false_label;

    cur_block->insts.push_back(
        createBInst(op, cmp_op1, cmp_op2, RV64Label(true_label->label_num, false_label->label_num)));
    cur_block->insts.push_back(createJInst(RV64InstType::JAL, preg_x0, RV64Label(false_label->label_num)));
}
void Selector::convertAndAppend(LLVMIR::BranchUncondInst* inst)
{
    RV64Label tar_label(((LLVMIR::LabelOperand*)inst->target_label)->label_num);

    cur_block->insts.push_back(createJInst(RV64InstType::JAL, preg_x0, tar_label));
}
void Selector::convertAndAppend(LLVMIR::GlbvarDefInst* inst)
{
    cerr << "GlbvarDefInst" << endl;
    assert(false);
}
namespace
{
    Register paramPhysReg(int i, bool is_int = true)
    {
        if (is_int)
        {
            switch (i)
            {
                case 0: return preg_a0;
                case 1: return preg_a1;
                case 2: return preg_a2;
                case 3: return preg_a3;
                case 4: return preg_a4;
                case 5: return preg_a5;
                case 6: return preg_a6;
                case 7: return preg_a7;
                default: assert(false);
            }
        }
        else
        {
            switch (i)
            {
                case 0: return preg_fa0;
                case 1: return preg_fa1;
                case 2: return preg_fa2;
                case 3: return preg_fa3;
                case 4: return preg_fa4;
                case 5: return preg_fa5;
                case 6: return preg_fa6;
                case 7: return preg_fa7;
                default: assert(false);
            }
        }
    }
}  // namespace
void Selector::convertAndAppend(LLVMIR::CallInst* inst)
{
    if (inst->func_name == "llvm.memset.p0.i32")
    {
        // dst
        int  dst_reg_num = ((LLVMIR::RegOperand*)inst->args[0].second)->reg_num;
        auto dst_it      = alloc_shift_map.find(dst_reg_num);
        if (dst_it == alloc_shift_map.end())
            cur_block->insts.push_back(createMoveInst(INT64, preg_a0, getLLVMReg(dst_reg_num, INT64)));
        else
        {
            int offset = dst_it->second;
            // RV64Inst* addr_inst = createIInst(RV64InstType::ADDI, preg_a0, preg_sp, offset);
            RV64Inst* addr_inst = nullptr;
            if (offset <= 2047 && offset >= -2048)
                addr_inst = createIInst(RV64InstType::ADDI, preg_a0, preg_sp, offset);
            else
            {
                Register num_reg = getReg(INT64);
                cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset));
                addr_inst = createRInst(RV64InstType::ADD, preg_a0, preg_sp, num_reg);
            }

            cur_func->alloc_insts.push_back(addr_inst);
            cur_block->insts.push_back(addr_inst);
        }

        // value
        int val_imme = ((LLVMIR::ImmeI32Operand*)inst->args[1].second)->value;
        cur_block->insts.push_back(createMoveInst(INT64, preg_a1, val_imme));

        // len
        LLVMIR::Operand* len_op = inst->args[2].second;
        if (IS_IMMEI32(len_op))
        {
            int len_imme = ((LLVMIR::ImmeI32Operand*)len_op)->value;
            cur_block->insts.push_back(createMoveInst(INT64, preg_a2, len_imme));
        }
        else
        {
            int  len_reg_num = ((LLVMIR::RegOperand*)len_op)->reg_num;
            auto len_it      = alloc_shift_map.find(len_reg_num);
            if (len_it == alloc_shift_map.end())
                cur_block->insts.push_back(createMoveInst(INT64, preg_a2, getLLVMReg(len_reg_num, INT64)));
            else
            {
                int       offset    = len_it->second;
                RV64Inst* addr_inst = createIInst(RV64InstType::ADDI, preg_a2, preg_sp, offset);

                cur_func->alloc_insts.push_back(addr_inst);
                cur_block->insts.push_back(addr_inst);
            }
        }

        // isvolatile? never care

        // call
        cur_block->insts.push_back(createCallInst(RV64InstType::CALL, "memset", 3, 0));

        return;
    }

    int ireg_para_cnt = 0, freg_para_cnt = 0, stack_para_cnt = 0;
    for (auto& [ir_type, arg_ir_op] : inst->args)
    {
        if (ir_type == LLVMIR::DataType::I32 || ir_type == LLVMIR::DataType::PTR || ir_type == LLVMIR::DataType::I8 ||
            ir_type == LLVMIR::DataType::I1)
        {
            if (ireg_para_cnt < 8)
            {
                if (IS_REG(arg_ir_op))
                {
                    int      arg_reg_num = ((LLVMIR::RegOperand*)arg_ir_op)->reg_num;
                    Register arg_reg     = getLLVMReg(arg_reg_num, INT64);

                    auto it = alloc_shift_map.find(arg_reg_num);
                    if (it == alloc_shift_map.end())
                    {
                        cur_block->insts.push_back(createMoveInst(INT64, paramPhysReg(ireg_para_cnt), arg_reg));
                    }
                    else
                    {
                        int offset = it->second;

                        if (offset <= 2047 && offset >= -2048)
                        {
                            cur_block->insts.push_back(
                                createIInst(RV64InstType::LW, paramPhysReg(ireg_para_cnt), preg_sp, offset));
                        }
                        else
                        {
                            Register num_reg = getReg(INT64);
                            cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset));
                            cur_block->insts.push_back(
                                createRInst(RV64InstType::ADD, paramPhysReg(ireg_para_cnt), preg_sp, num_reg));
                        }
                    }
                }
                else if (IS_GLOBAL(arg_ir_op))
                {
                    Register mid_reg = getReg(INT64);
                    string&  glb_var = ((LLVMIR::GlobalOperand*)arg_ir_op)->global_name;

                    cur_block->insts.push_back(createUInst(RV64InstType::LA, mid_reg, RV64Label(glb_var, false, true)));
                }
                else if (IS_IMMEI32(arg_ir_op))
                {
                    int imme_val = ((LLVMIR::ImmeI32Operand*)arg_ir_op)->value;
                    cur_block->insts.push_back(createMoveInst(INT64, paramPhysReg(ireg_para_cnt), imme_val));
                }
                else
                    assert(false);
            }
            else
                ++stack_para_cnt;

            ++ireg_para_cnt;
        }
        else if (ir_type == LLVMIR::DataType::F32)
        {
            if (freg_para_cnt < 8)
            {
                if (IS_REG(arg_ir_op))
                {
                    int      arg_reg_num = ((LLVMIR::RegOperand*)arg_ir_op)->reg_num;
                    Register arg_reg     = getLLVMReg(arg_reg_num, FLOAT64);

                    auto it = alloc_shift_map.find(arg_reg_num);
                    if (it == alloc_shift_map.end())
                    {
                        cur_block->insts.push_back(
                            createMoveInst(FLOAT64, paramPhysReg(freg_para_cnt, false), arg_reg));
                    }
                    else
                    {
                        int offset = it->second;

                        if (offset <= 2047 && offset >= -2048)
                        {
                            cur_block->insts.push_back(
                                createIInst(RV64InstType::FLD, paramPhysReg(freg_para_cnt, false), preg_sp, offset));
                        }
                        else
                        {
                            Register num_reg = getReg(INT64);
                            cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset));
                            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                        }
                    }
                }
                else if (IS_GLOBAL(arg_ir_op))
                {
                    Register mid_reg = getReg(INT64);
                    string&  glb_var = ((LLVMIR::GlobalOperand*)arg_ir_op)->global_name;

                    cur_block->insts.push_back(createUInst(RV64InstType::LA, mid_reg, RV64Label(glb_var, false, true)));
                }
                else if (IS_IMMEF32(arg_ir_op))
                {
                    float imme_val = ((LLVMIR::ImmeF32Operand*)arg_ir_op)->value;
                    cur_block->insts.push_back(createMoveInst(FLOAT64, paramPhysReg(freg_para_cnt, false), imme_val));
                }
                else
                    assert(false);
            }
            else
                ++stack_para_cnt;

            ++freg_para_cnt;
        }
        else
        {
            // cerr << (int)ir_type << endl;
            assert(false);
        }
    }

    if (stack_para_cnt > 0)
    {
        int arg_shift = 0;
        int ireg_cnt = 0, freg_cnt = 0;

        for (auto& [ir_type, arg_ir_op] : inst->args)
        {
            if (ir_type == LLVMIR::DataType::I32 || ir_type == LLVMIR::DataType::PTR ||
                ir_type == LLVMIR::DataType::I8 || ir_type == LLVMIR::DataType::I1)
            {
                if (ireg_cnt < 8)
                {
                    ++ireg_cnt;
                    continue;
                }

                if (IS_REG(arg_ir_op))
                {
                    int      arg_reg_num = ((LLVMIR::RegOperand*)arg_ir_op)->reg_num;
                    Register arg_reg     = getLLVMReg(arg_reg_num, INT64);

                    auto it = alloc_shift_map.find(arg_reg_num);
                    if (it == alloc_shift_map.end())
                    {
                        if (arg_shift >= -2048 && arg_shift <= 2047)
                        {
                            cur_block->insts.push_back(createSInst(RV64InstType::SD, arg_reg, preg_sp, arg_shift));
                        }
                        else
                        {
                            Register num_reg = getReg(INT64);
                            cur_block->insts.push_back(createMoveInst(INT64, num_reg, arg_shift));
                            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                            cur_block->insts.push_back(createSInst(RV64InstType::SD, arg_reg, num_reg, 0));
                        }
                    }
                    else
                    {
                        int offset = it->second;

                        if (offset <= 2047 && offset >= -2048)
                        {
                            if ((offset + arg_shift) >= -2048 && (offset + arg_shift) <= 2047)
                            {
                                cur_block->insts.push_back(
                                    createIInst(RV64InstType::SW, arg_reg, preg_sp, offset + arg_shift));
                            }
                            else
                            {
                                Register num_reg = getReg(INT64);
                                cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset + arg_shift));
                                cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                                cur_block->insts.push_back(createSInst(RV64InstType::SD, arg_reg, num_reg, 0));
                            }
                        }
                        else
                        {
                            Register num_reg = getReg(INT64);
                            cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset + arg_shift));
                            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                            cur_block->insts.push_back(createSInst(RV64InstType::SD, arg_reg, num_reg, 0));
                        }
                    }
                }
                else if (IS_GLOBAL(arg_ir_op))
                {
                    Register mid_reg = getReg(INT64);
                    string&  glb_var = ((LLVMIR::GlobalOperand*)arg_ir_op)->global_name;

                    cur_block->insts.push_back(createUInst(RV64InstType::LA, mid_reg, RV64Label(glb_var, false, true)));
                }
                else if (IS_IMMEI32(arg_ir_op))
                {
                    int      imme_val = ((LLVMIR::ImmeI32Operand*)arg_ir_op)->value;
                    Register imme_reg = getReg(INT64);

                    cur_block->insts.push_back(createMoveInst(INT64, imme_reg, imme_val));
                    if (arg_shift >= -2048 && arg_shift <= 2047)
                    {
                        cur_block->insts.push_back(createSInst(RV64InstType::SD, imme_reg, preg_sp, arg_shift));
                    }
                    else
                    {
                        Register num_reg = getReg(INT64);
                        cur_block->insts.push_back(createMoveInst(INT64, num_reg, arg_shift));
                        cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                        cur_block->insts.push_back(createSInst(RV64InstType::SD, imme_reg, num_reg, 0));
                    }
                }

                arg_shift += 8;
            }
            else if (ir_type == LLVMIR::DataType::F32)
            {
                if (freg_cnt < 8)
                {
                    ++freg_cnt;
                    continue;
                }

                if (IS_REG(arg_ir_op))
                {
                    int      arg_reg_num = ((LLVMIR::RegOperand*)arg_ir_op)->reg_num;
                    Register arg_reg     = getLLVMReg(arg_reg_num, FLOAT64);

                    auto it = alloc_shift_map.find(arg_reg_num);
                    if (it == alloc_shift_map.end())
                    {
                        if (arg_shift >= -2048 && arg_shift <= 2047)
                        {
                            cur_block->insts.push_back(createSInst(RV64InstType::FSD, arg_reg, preg_sp, arg_shift));
                        }
                        else
                        {
                            Register num_reg = getReg(INT64);
                            cur_block->insts.push_back(createMoveInst(INT64, num_reg, arg_shift));
                            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                            cur_block->insts.push_back(createSInst(RV64InstType::FSD, arg_reg, num_reg, 0));
                        }
                    }
                    else
                    {
                        int offset = it->second;

                        if (offset <= 2047 && offset >= -2048)
                        {
                            if ((offset + arg_shift) >= -2048 && (offset + arg_shift) <= 2047)
                            {
                                cur_block->insts.push_back(
                                    createIInst(RV64InstType::FSD, arg_reg, preg_sp, offset + arg_shift));
                            }
                            else
                            {
                                Register num_reg = getReg(INT64);
                                cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset + arg_shift));
                                cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                                cur_block->insts.push_back(createSInst(RV64InstType::FSD, arg_reg, num_reg, 0));
                            }
                        }
                        else
                        {
                            Register num_reg = getReg(INT64);
                            cur_block->insts.push_back(createMoveInst(INT64, num_reg, offset + arg_shift));
                            cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                            cur_block->insts.push_back(createSInst(RV64InstType::FSD, arg_reg, num_reg, 0));
                        }
                    }
                }
                else if (IS_GLOBAL(arg_ir_op))
                {
                    Register mid_reg = getReg(INT64);
                    string&  glb_var = ((LLVMIR::GlobalOperand*)arg_ir_op)->global_name;

                    cur_block->insts.push_back(createUInst(RV64InstType::LA, mid_reg, RV64Label(glb_var, false, true)));
                }
                else if (IS_IMMEF32(arg_ir_op))
                {
                    float val     = ((LLVMIR::ImmeF32Operand*)arg_ir_op)->value;
                    int   int_val = *(int*)&val;

                    Register imme_reg = getReg(INT64);
                    cur_block->insts.push_back(createMoveInst(INT64, imme_reg, int_val));
                    if (arg_shift >= -2048 && arg_shift <= 2047)
                    {
                        cur_block->insts.push_back(createSInst(RV64InstType::SD, imme_reg, preg_sp, arg_shift));
                    }
                    else
                    {
                        Register num_reg = getReg(INT64);
                        cur_block->insts.push_back(createMoveInst(INT64, num_reg, arg_shift));
                        cur_block->insts.push_back(createRInst(RV64InstType::ADD, num_reg, preg_sp, num_reg));
                        cur_block->insts.push_back(createSInst(RV64InstType::SD, imme_reg, num_reg, 0));
                    }
                }

                arg_shift += 8;
            }
            else
                assert(false);
        }
    }

    std::string&     func_name = inst->func_name;
    LLVMIR::DataType ret_type  = inst->ret_type;

    cur_block->insts.push_back(createCallInst(RV64InstType::CALL, func_name, ireg_para_cnt, freg_para_cnt));
    int param_size = stack_para_cnt * 8;
    if (param_size > cur_func->param_size) cur_func->param_size = param_size;

    if (ret_type == LLVMIR::DataType::VOID) {}
    else if (ret_type == LLVMIR::DataType::I32)
    {
        int      ret_reg_num = ((LLVMIR::RegOperand*)inst->res)->reg_num;
        Register res_reg     = getLLVMReg(ret_reg_num, INT64);
        cur_block->insts.push_back(createMoveInst(INT64, res_reg, preg_a0));
    }
    else if (ret_type == LLVMIR::DataType::F32)
    {
        int      ret_reg_num = ((LLVMIR::RegOperand*)inst->res)->reg_num;
        Register res_reg     = getLLVMReg(ret_reg_num, FLOAT64);
        cur_block->insts.push_back(createMoveInst(FLOAT64, res_reg, preg_fa0));
    }
    else
        assert(false);
}
void Selector::convertAndAppend(LLVMIR::RetInst* inst)
{
    int ret_type = 0;  // default: void

    if (inst->ret != nullptr)
    {
        if (IS_REG(inst->ret))
        {
            if (inst->ret_type == LLVMIR::DataType::I32)
            {
                ret_type = 1;

                int      reg_num = ((LLVMIR::RegOperand*)inst->ret)->reg_num;
                Register res_reg = getLLVMReg(reg_num, INT64);

                cur_block->insts.push_back(createMoveInst(INT64, preg_a0, res_reg));
                // cur_block->insts.push_back(createIInst(RV64InstType::ADDI, preg_a0, res_reg, 0));
            }
            else if (inst->ret_type == LLVMIR::DataType::F32)
            {
                ret_type = 2;

                int      reg_num = ((LLVMIR::RegOperand*)inst->ret)->reg_num;
                Register res_reg = getLLVMReg(reg_num, FLOAT64);

                cur_block->insts.push_back(createMoveInst(FLOAT64, preg_fa0, res_reg));
            }
            else
                assert(false);
        }
        else if (IS_IMMEI32(inst->ret))
        {
            int res_val = ((LLVMIR::ImmeI32Operand*)inst->ret)->value;

            cur_block->insts.push_back(createMoveInst(INT64, preg_a0, res_val));
            // cur_block->insts.push_back(createIInst(RV64InstType::ADDI, preg_a0, preg_x0, res_val));
        }
        else if (IS_IMMEF32(inst->ret))
        {
            float res_val = ((LLVMIR::ImmeF32Operand*)inst->ret)->value;

            cur_block->insts.push_back(createMoveInst(FLOAT64, preg_fa0, res_val));
        }
        else
            assert(false);
    }
    if (inst->ret_type == LLVMIR::DataType::I32)
        ret_type = 1;
    else if (inst->ret_type == LLVMIR::DataType::F32)
        ret_type = 2;

    RV64Inst* ret_inst = createIInst(RV64InstType::JALR, preg_x0, preg_ra, 0);
    ret_inst->ret_type = ret_type;

    cur_block->insts.push_back(ret_inst);
}
void Selector::convertAndAppend(LLVMIR::GEPInst* inst)
{
    LLVMIR::RegOperand* res_op             = (LLVMIR::RegOperand*)inst->res;
    int                 ir_reg             = res_op->reg_num;
    int                 offset             = 0;
    Register            res_reg            = getLLVMReg(ir_reg, INT64);
    Register            shift_reg          = getReg(INT64);
    bool                shift_reg_assigned = false;

    int size = 1;
    for (auto dim : inst->dims) size *= dim;

    for (size_t i = 0; i < inst->idxs.size(); ++i)
    {
        if (IS_IMMEI32(inst->idxs[i]))
            offset += ((LLVMIR::ImmeI32Operand*)inst->idxs[i])->value * size;
        else
        {
            Register idx_reg = getLLVMReg(((LLVMIR::RegOperand*)inst->idxs[i])->reg_num, INT64);

            if (size == 1)
            {
                if (shift_reg_assigned)
                {
                    Register new_shift_reg = getReg(INT64);
                    cur_block->insts.push_back(createRInst(RV64InstType::ADD, new_shift_reg, shift_reg, idx_reg));
                    shift_reg = new_shift_reg;
                }
                else
                {
                    shift_reg_assigned = true;
                    cur_block->insts.push_back(createMoveInst(INT64, shift_reg, idx_reg));
                }
            }
            else
            {
                Register inc_reg  = getReg(INT64);
                Register prod_reg = getReg(INT64);

                cur_block->insts.push_back(createMoveInst(INT64, prod_reg, size));
                // index * prod -> inc
                cur_block->insts.push_back(createRInst(RV64InstType::MUL, inc_reg, idx_reg, prod_reg));

                if (shift_reg_assigned)
                {
                    Register new_shift_reg = getReg(INT64);
                    cur_block->insts.push_back(createRInst(RV64InstType::ADD, new_shift_reg, shift_reg, inc_reg));
                    shift_reg = new_shift_reg;
                }
                else
                {
                    shift_reg_assigned = true;
                    cur_block->insts.push_back(createMoveInst(INT64, shift_reg, inc_reg));
                }
            }
        }

        if (i < inst->dims.size()) size /= inst->dims[i];
    }

    bool all_imme = false;
    if (offset)
    {
        if (shift_reg_assigned)
        {
            Register new_shift_reg = getReg(INT64);
            cur_block->insts.push_back(createIInst(RV64InstType::ADDI, new_shift_reg, shift_reg, offset));
            res_reg = new_shift_reg;
        }
        else
        {
            shift_reg_assigned = true;
            all_imme           = true;
            cur_block->insts.push_back(createMoveInst(INT64, shift_reg, offset * 4));
        }
    }

    if (IS_REG(inst->base_ptr))
    {
        LLVMIR::RegOperand* ptr_op = (LLVMIR::RegOperand*)inst->base_ptr;

        Register shift_full_reg = getReg(INT64);

        if (shift_reg_assigned)
        {
            RV64Inst* sll_inst = nullptr;
            if (!all_imme) { sll_inst = createIInst(RV64InstType::SLLI, shift_full_reg, shift_reg, 2); }
            else { shift_full_reg = shift_reg; }

            auto it = alloc_shift_map.find(ptr_op->reg_num);
            if (it == alloc_shift_map.end())
            {
                if (!all_imme) cur_block->insts.push_back(sll_inst);
                cur_block->insts.push_back(
                    createRInst(RV64InstType::ADD, res_reg, getLLVMReg(ptr_op->reg_num, INT64), shift_full_reg));
            }
            else
            {
                int      sp_shift = it->second;
                Register base_reg = getReg(INT64);

                // RV64Inst* load_base_inst = createIInst(RV64InstType::ADDI, base_reg, preg_sp, sp_shift);

                RV64Inst* load_base_inst = nullptr;
                if (sp_shift <= 2047 && sp_shift >= -2048)
                    load_base_inst = createIInst(RV64InstType::ADDI, base_reg, preg_sp, sp_shift);
                else
                {
                    Register num_reg = getReg(INT64);
                    cur_block->insts.push_back(createMoveInst(INT64, num_reg, sp_shift));
                    load_base_inst = createRInst(RV64InstType::ADD, base_reg, preg_sp, num_reg);
                }

                cur_block->insts.push_back(load_base_inst);
                cur_func->alloc_insts.push_back(load_base_inst);

                if (!all_imme) cur_block->insts.push_back(sll_inst);

                cur_block->insts.push_back(createRInst(RV64InstType::ADD, res_reg, base_reg, shift_full_reg));
            }
        }
        else
        {
            auto it = alloc_shift_map.find(ptr_op->reg_num);

            if (it == alloc_shift_map.end())
                cur_block->insts.push_back(createMoveInst(INT64, res_reg, getLLVMReg(ptr_op->reg_num, INT64)));
            else
            {
                int sp_shift = it->second;

                // RV64Inst* load_base_inst = createIInst(RV64InstType::ADDI, res_reg, preg_sp, sp_shift);
                RV64Inst* load_base_inst = nullptr;

                if (sp_shift <= 2047 && sp_shift >= -2048)
                    load_base_inst = createIInst(RV64InstType::ADDI, res_reg, preg_sp, sp_shift);
                else
                {
                    Register num_reg = getReg(INT64);
                    cur_block->insts.push_back(createMoveInst(INT64, num_reg, sp_shift));
                    load_base_inst = createRInst(RV64InstType::ADD, res_reg, preg_sp, num_reg);
                }

                cur_block->insts.push_back(load_base_inst);
                cur_func->alloc_insts.push_back(load_base_inst);
            }
        }
    }
    else if (IS_GLOBAL(inst->base_ptr))
    {
        if (shift_reg_assigned)
        {
            Register hi_reg         = getReg(INT64);
            Register hi_lo_reg      = getReg(INT64);
            Register shift_full_reg = getReg(INT64);

            cur_block->insts.push_back(createUInst(RV64InstType::LA,
                hi_reg,
                RV64Label(((LLVMIR::GlobalOperand*)inst->base_ptr)->global_name, false, true)));
            RV64Inst* slli_inst = nullptr;
            if (!all_imme) { slli_inst = createIInst(RV64InstType::SLLI, shift_full_reg, shift_reg, 2); }
            else { shift_full_reg = shift_reg; }
            RV64Inst* addshift_inst = createRInst(RV64InstType::ADD, res_reg, hi_reg, shift_full_reg);

            if (!all_imme) cur_block->insts.push_back(slli_inst);
            cur_block->insts.push_back(addshift_inst);
        }
        else
        {
            Register hi_reg = getReg(INT64);

            cur_block->insts.push_back(createUInst(RV64InstType::LA,
                hi_reg,
                RV64Label(((LLVMIR::GlobalOperand*)inst->base_ptr)->global_name, false, true)));
            cur_block->insts.push_back(createMoveInst(INT64, res_reg, hi_reg));
        }
    }
    else
        assert(false);
}
void Selector::convertAndAppend(LLVMIR::FuncDeclareInst* inst)
{
    cerr << "FuncDeclareInst" << endl;
    assert(false);
}
void Selector::convertAndAppend(LLVMIR::FuncDefInst* inst)
{
    cerr << "FuncDefInst" << endl;
    assert(false);
}
// fcvt.w.s rd, fs1, rmode
// -> fs1(float) -> rd(signed int)
void Selector::convertAndAppend(LLVMIR::SI2FPInst* inst)
{
    int              dst_reg_num = ((LLVMIR::RegOperand*)inst->t_fp)->reg_num;
    Register         dst_reg     = getLLVMReg(dst_reg_num, FLOAT64);
    LLVMIR::Operand* src_op      = inst->f_si;

    if (IS_REG(src_op))
    {
        int      src_reg_num = ((LLVMIR::RegOperand*)src_op)->reg_num;
        Register src_reg     = getLLVMReg(src_reg_num, INT64);
        cur_block->insts.push_back(createR2Inst(RV64InstType::FCVT_S_W, dst_reg, src_reg));
    }
    else if (IS_IMMEI32(src_op))
    {
        Register tmp_reg = getReg(INT64);
        int      imme    = ((LLVMIR::ImmeI32Operand*)src_op)->value;

        cur_block->insts.push_back(createMoveInst(INT64, tmp_reg, imme));
        cur_block->insts.push_back(createR2Inst(RV64InstType::FCVT_S_W, dst_reg, tmp_reg));
    }
}
// fcvt.s.w fd, rs1, rmode
// -> rs1(signed int) -> fd(float)
void Selector::convertAndAppend(LLVMIR::FP2SIInst* inst)
{
    int              dst_reg_num = ((LLVMIR::RegOperand*)inst->t_si)->reg_num;
    Register         dst_reg     = getLLVMReg(dst_reg_num, INT64);
    LLVMIR::Operand* src_op      = inst->f_fp;

    if (IS_REG(src_op))
    {
        int      src_reg_num = ((LLVMIR::RegOperand*)src_op)->reg_num;
        Register src_reg     = getLLVMReg(src_reg_num, FLOAT64);
        cur_block->insts.push_back(createR2Inst(RV64InstType::FCVT_W_S, dst_reg, src_reg));
    }
    else if (IS_IMMEF32(src_op))
    {
        Register tmp_reg = getReg(FLOAT64);
        float    imme    = ((LLVMIR::ImmeF32Operand*)src_op)->value;

        cur_block->insts.push_back(createMoveInst(FLOAT64, tmp_reg, imme));
        cur_block->insts.push_back(createR2Inst(RV64InstType::FCVT_W_S, dst_reg, tmp_reg));
    }
}
void Selector::convertAndAppend(LLVMIR::ZextInst* inst)
{
    assert(IS_REG(inst->src));
    assert(IS_REG(inst->dest));

    Register             ext_reg  = getLLVMReg(((LLVMIR::RegOperand*)inst->src)->reg_num, INT64);
    Register             res_reg  = getLLVMReg(((LLVMIR::RegOperand*)inst->dest)->reg_num, INT64);
    LLVMIR::Instruction* cmp_inst = cmp_context[ext_reg];

    if (!cmp_inst)
    {
        PhiInst* phi_inst = new PhiInst(res_reg);
        assert(false);
        return;
    }
    // cerr << (size_t)cmp_inst->opcode << endl;
    if (cmp_inst->opcode == LLVMIR::IROpCode::ICMP)
    {
        LLVMIR::IcmpInst* icmp_inst = (LLVMIR::IcmpInst*)cmp_inst;
        LLVMIR::IcmpCond  cond      = icmp_inst->cond;
        LLVMIR::Operand*  lhs_op    = icmp_inst->lhs;
        LLVMIR::Operand*  rhs_op    = icmp_inst->rhs;
        Register          lhs, rhs;

        if (IS_IMMEI32(lhs_op))
        {
            // change to rhs
            LLVMIR::Operand* tmp = lhs_op;
            lhs_op               = rhs_op;
            rhs_op               = tmp;

            // change to negative
            switch (cond)
            {
                case LLVMIR::IcmpCond::EQ:
                case LLVMIR::IcmpCond::NE: break;
                case LLVMIR::IcmpCond::UGT:
                case LLVMIR::IcmpCond::ULE:
                case LLVMIR::IcmpCond::UGE:
                case LLVMIR::IcmpCond::ULT:
                    cerr << "Not implemented yet" << endl;
                    assert(false);
                    break;
                case LLVMIR::IcmpCond::SGT: cond = LLVMIR::IcmpCond::SLT; break;
                case LLVMIR::IcmpCond::SLE: cond = LLVMIR::IcmpCond::SGE; break;
                case LLVMIR::IcmpCond::SGE: cond = LLVMIR::IcmpCond::SLE; break;
                case LLVMIR::IcmpCond::SLT: cond = LLVMIR::IcmpCond::SGT; break;
                default: cerr << "Unknown icmp cond: " << cond << endl; assert(false);
            }
        }

        // IMME icmp IMME
        if (IS_IMMEI32(lhs_op))
        {
            int cmp_res = 0;
            int lhs_val = ((LLVMIR::ImmeI32Operand*)lhs_op)->value;
            int rhs_val = ((LLVMIR::ImmeI32Operand*)rhs_op)->value;
            switch (cond)
            {
                case LLVMIR::IcmpCond::EQ: cmp_res = (lhs_val == rhs_val); break;
                case LLVMIR::IcmpCond::NE: cmp_res = (lhs_val != rhs_val); break;
                case LLVMIR::IcmpCond::UGT:
                case LLVMIR::IcmpCond::ULE:
                case LLVMIR::IcmpCond::UGE:
                case LLVMIR::IcmpCond::ULT:
                    cerr << "Not implemented yet" << endl;
                    assert(false);
                    break;
                case LLVMIR::IcmpCond::SGT: cmp_res = (lhs_val > rhs_val); break;
                case LLVMIR::IcmpCond::SLE: cmp_res = (lhs_val <= rhs_val); break;
                case LLVMIR::IcmpCond::SGE: cmp_res = (lhs_val >= rhs_val); break;
                case LLVMIR::IcmpCond::SLT: cmp_res = (lhs_val < rhs_val); break;
                default: cerr << "Unknown icmp cond: " << cond << endl; assert(false);
            }

            cur_block->insts.push_back(createMoveInst(INT64, res_reg, cmp_res));
            return;
        }

        if (IS_IMMEI32(rhs_op))
        {
            Register lhs_reg = getLLVMReg(((LLVMIR::RegOperand*)lhs_op)->reg_num, INT64);

            if (((LLVMIR::ImmeI32Operand*)rhs_op)->value == 0)
            {
                switch (cond)
                {
                    case LLVMIR::IcmpCond::EQ:
                        cur_block->insts.push_back(createIInst(RV64InstType::SLTIU, res_reg, lhs_reg, 1));
                        break;
                    case LLVMIR::IcmpCond::NE:
                        cur_block->insts.push_back(createRInst(RV64InstType::SLTU, res_reg, preg_x0, lhs_reg));
                        break;
                    case LLVMIR::IcmpCond::UGT:
                    case LLVMIR::IcmpCond::ULE:
                    case LLVMIR::IcmpCond::UGE:
                    case LLVMIR::IcmpCond::ULT:
                        cerr << "Not implemented yet" << endl;
                        assert(false);
                        break;
                    case LLVMIR::IcmpCond::SGT:
                        cur_block->insts.push_back(createRInst(RV64InstType::SLT, res_reg, preg_x0, lhs_reg));
                        break;
                    case LLVMIR::IcmpCond::SLE:
                        cur_block->insts.push_back(createRInst(RV64InstType::SLT, res_reg, preg_x0, lhs_reg));
                        cur_block->insts.push_back(createIInst(RV64InstType::XORI, res_reg, res_reg, 1));
                        break;
                    case LLVMIR::IcmpCond::SGE:
                        cur_block->insts.push_back(createRInst(RV64InstType::SLT, res_reg, lhs_reg, preg_x0));
                        cur_block->insts.push_back(createIInst(RV64InstType::XORI, res_reg, res_reg, 1));
                        break;
                    case LLVMIR::IcmpCond::SLT:
                        cur_block->insts.push_back(createRInst(RV64InstType::SLT, res_reg, lhs_reg, preg_x0));
                        break;
                    default: cerr << "Unknown icmp cond: " << cond << endl; assert(false);
                }
            }
            else if (cond == LLVMIR::IcmpCond::SLT)
            {
                int rhs_val = ((LLVMIR::ImmeI32Operand*)rhs_op)->value;
                cur_block->insts.push_back(createIInst(RV64InstType::SLTI, res_reg, lhs_reg, rhs_val));
                return;
            }
            else if (cond == LLVMIR::IcmpCond::ULT)
            {
                int rhs_val = ((LLVMIR::ImmeI32Operand*)rhs_op)->value;
                cur_block->insts.push_back(createIInst(RV64InstType::SLTIU, res_reg, lhs_reg, rhs_val));
                return;
            }

            rhs = getReg(INT64);
            cur_block->insts.push_back(createMoveInst(INT64, rhs, ((LLVMIR::ImmeI32Operand*)rhs_op)->value));
        }

        lhs = getLLVMReg(((LLVMIR::RegOperand*)lhs_op)->reg_num, INT64);
        if (IS_REG(rhs_op)) rhs = getLLVMReg(((LLVMIR::RegOperand*)rhs_op)->reg_num, INT64);

        switch (cond)
        {
            case LLVMIR::IcmpCond::EQ:
                cur_block->insts.push_back(createRInst(RV64InstType::SUB, res_reg, lhs, rhs));
                cur_block->insts.push_back(createIInst(RV64InstType::SLTIU, res_reg, res_reg, 1));
                break;
            case LLVMIR::IcmpCond::NE:
                cur_block->insts.push_back(createRInst(RV64InstType::SUB, res_reg, lhs, rhs));
                cur_block->insts.push_back(createRInst(RV64InstType::SLTU, res_reg, preg_x0, res_reg));
                break;
            case LLVMIR::IcmpCond::UGT:
            case LLVMIR::IcmpCond::ULE:
            case LLVMIR::IcmpCond::UGE:
            case LLVMIR::IcmpCond::ULT:
                cerr << "Not implemented yet" << endl;
                assert(false);
                break;
            case LLVMIR::IcmpCond::SGT:
                cur_block->insts.push_back(createRInst(RV64InstType::SLT, res_reg, rhs, lhs));
                break;
            case LLVMIR::IcmpCond::SLE:  // not SGT
                cur_block->insts.push_back(createRInst(RV64InstType::SLT, res_reg, rhs, lhs));
                cur_block->insts.push_back(createIInst(RV64InstType::XORI, res_reg, res_reg, 1));
                break;
            case LLVMIR::IcmpCond::SGE:  // not SLT
                cur_block->insts.push_back(createRInst(RV64InstType::SLT, res_reg, lhs, rhs));
                cur_block->insts.push_back(createIInst(RV64InstType::XORI, res_reg, res_reg, 1));
                break;
            case LLVMIR::IcmpCond::SLT:
                cur_block->insts.push_back(createRInst(RV64InstType::SLT, res_reg, lhs, rhs));
                break;
            default: cerr << "Unknown icmp cond: " << cond << endl; assert(false);
        }
    }
    else if (cmp_inst->opcode == LLVMIR::IROpCode::FCMP)
    {
        LLVMIR::FcmpInst* fcmp_inst  = (LLVMIR::FcmpInst*)cmp_inst;
        LLVMIR::Operand*  ir_cmp_op1 = fcmp_inst->lhs;
        LLVMIR::Operand*  ir_cmp_op2 = fcmp_inst->rhs;
        Register          cmp_op1, cmp_op2;

        if (IS_REG(ir_cmp_op1))
        {
            int reg_num = ((LLVMIR::RegOperand*)ir_cmp_op1)->reg_num;
            cmp_op1     = getLLVMReg(reg_num, FLOAT64);
        }
        else if (IS_IMMEF32(ir_cmp_op1))
        {
            float imme = ((LLVMIR::ImmeF32Operand*)ir_cmp_op1)->value;
            cmp_op1    = getReg(FLOAT64);
            cur_block->insts.push_back(createMoveInst(FLOAT64, cmp_op1, imme));
        }
        else
            assert(false);

        if (IS_REG(ir_cmp_op2))
        {
            int reg_num = ((LLVMIR::RegOperand*)ir_cmp_op2)->reg_num;
            cmp_op2     = getLLVMReg(reg_num, FLOAT64);
        }
        else if (IS_IMMEF32(ir_cmp_op2))
        {
            float imme = ((LLVMIR::ImmeF32Operand*)ir_cmp_op2)->value;
            cmp_op2    = getReg(FLOAT64);
            cur_block->insts.push_back(createMoveInst(FLOAT64, cmp_op2, imme));
        }
        else
            assert(false);

        switch (fcmp_inst->cond)
        {
            case LLVMIR::FcmpCond::OEQ:
            case LLVMIR::FcmpCond::UEQ:
                cur_block->insts.push_back(createRInst(RV64InstType::FEQ_S, res_reg, cmp_op1, cmp_op2));
                break;
            case LLVMIR::FcmpCond::OGT:
            case LLVMIR::FcmpCond::UGT:
                cur_block->insts.push_back(createRInst(RV64InstType::FLT_S, res_reg, cmp_op2, cmp_op1));
                break;
            case LLVMIR::FcmpCond::OGE:
            case LLVMIR::FcmpCond::UGE:
                cur_block->insts.push_back(createRInst(RV64InstType::FLE_S, res_reg, cmp_op2, cmp_op1));
                break;
            case LLVMIR::FcmpCond::OLT:
            case LLVMIR::FcmpCond::ULT:
                cur_block->insts.push_back(createRInst(RV64InstType::FLT_S, res_reg, cmp_op1, cmp_op2));
                break;
            case LLVMIR::FcmpCond::OLE:
            case LLVMIR::FcmpCond::ULE:
                cur_block->insts.push_back(createRInst(RV64InstType::FLE_S, res_reg, cmp_op1, cmp_op2));
                break;
            case LLVMIR::FcmpCond::ONE:
            case LLVMIR::FcmpCond::UNE:
                // cur_block->insts.push_back(createRInst(RV64InstType::FNE_S, res_reg, cmp_op1, cmp_op2));

                // FEQ_S, then XORI 1
                cur_block->insts.push_back(createRInst(RV64InstType::FEQ_S, res_reg, cmp_op1, cmp_op2));
                cur_block->insts.push_back(createIInst(RV64InstType::XORI, res_reg, res_reg, 1));
                break;
            default: assert(false);
        }
    }
    else
        assert(false);
}
void Selector::convertAndAppend(LLVMIR::FPExtInst* inst)
{
    std::cerr << "Not implemented yet" << std::endl;
    assert(false);
}
void Selector::convertAndAppend(LLVMIR::PhiInst* inst)
{
    LLVMIR::Operand* res_op = inst->res;
    Register         res_reg;

    if (inst->type == LLVMIR::DataType::I32 || inst->type == LLVMIR::DataType::PTR ||
        inst->type == LLVMIR::DataType::I1)
        res_reg = getLLVMReg(((LLVMIR::RegOperand*)res_op)->reg_num, INT64);
    else if (inst->type == LLVMIR::DataType::F32)
        res_reg = getLLVMReg(((LLVMIR::RegOperand*)res_op)->reg_num, FLOAT64);
    else
    {
        cerr << "Unknown type: " << (size_t)inst->type << endl;
        assert(false);
    }

    PhiInst* phi_inst = new PhiInst(res_reg);

    for (auto& [val, label] : inst->vals_for_labels)
    {
        LLVMIR::LabelOperand* label_op = (LLVMIR::LabelOperand*)label;

        if (IS_REG(val))
        {
            if (inst->type == LLVMIR::DataType::I32 || inst->type == LLVMIR::DataType::PTR ||
                inst->type == LLVMIR::DataType::I1)
            {
                Register val_reg = getLLVMReg(((LLVMIR::RegOperand*)val)->reg_num, INT64);
                // phi_inst->vals_for_labels[val_reg] = label_op->label_num;
                phi_inst->phi_list[label_op->label_num] = new RegOperand(val_reg);
            }
            else if (inst->type == LLVMIR::DataType::F32)
            {
                Register val_reg = getLLVMReg(((LLVMIR::RegOperand*)val)->reg_num, FLOAT64);
                // phi_inst->vals_for_labels[val_reg] = label_op->label_num;
                phi_inst->phi_list[label_op->label_num] = new RegOperand(val_reg);
            }
            else
                assert(false);
        }
        else if (IS_IMMEI32(val))
        {
            int val_val = ((LLVMIR::ImmeI32Operand*)val)->value;
            // phi_inst->vals_for_labels[val_val] = label_op->label_num;
            phi_inst->phi_list[label_op->label_num] = new ImmeI32Operand(val_val);
        }
        else if (IS_IMMEF32(val))
        {
            float val_val = ((LLVMIR::ImmeF32Operand*)val)->value;
            // phi_inst->vals_for_labels[val_val] = label_op->label_num;
            phi_inst->phi_list[label_op->label_num] = new ImmeF32Operand(val_val);
        }
        else
            assert(false);
    }

    cur_block->insts.push_back(phi_inst);
}