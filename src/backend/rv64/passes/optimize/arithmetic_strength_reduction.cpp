#include <backend/rv64/passes/optimize/arithmetic_strength_reduction.h>
#include <backend/rv64/insts.h>

namespace Backend::RV64::Passes::Optimize
{

    ArithmeticStrengthReductionPass::ArithmeticStrengthReductionPass(std::vector<Function*>& functions)
        : functions_(functions)
    {}

    bool ArithmeticStrengthReductionPass::run()
    {
        for (auto& func : functions_) { optimizeFunction(func); }
        return true;
    }

    void ArithmeticStrengthReductionPass::optimizeFunction(Function* func)
    {
        std::map<int, int> IntConstPool;

        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                Instruction* base_inst = *it;
                if (base_inst->inst_type != InstType::MOVE) continue;

                MoveInst* move_inst = (MoveInst*)base_inst;
                if (move_inst->data_type != INT64) continue;
                if (move_inst->src->operand_type != OperandType::IMMEI32) continue;

                assert(move_inst->dst->operand_type == OperandType::REG);
                RegOperand* dst_reg_op = (RegOperand*)move_inst->dst;
                Register    dst_reg    = dst_reg_op->reg;
                if (dst_reg.is_virtual)
                {
                    ImmeI32Operand* src_imm       = (ImmeI32Operand*)move_inst->src;
                    IntConstPool[dst_reg.reg_num] = src_imm->val;
                }
            }
        }

        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                Instruction* base_inst = *it;
                if (base_inst->inst_type != InstType::RV64) continue;

                RV64Inst*    rv_inst = (RV64Inst*)base_inst;
                RV64InstType cur_op  = rv_inst->op;

                if (cur_op == RV64InstType::MUL || cur_op == RV64InstType::MULW)
                {
                    Register reg1 = rv_inst->rs1;
                    Register reg2 = rv_inst->rs2;

                    if (reg1.is_virtual)
                    {
                        if (IntConstPool.find(reg1.reg_num) != IntConstPool.end())
                        {
                            Register temp = reg1;
                            reg1          = reg2;
                            reg2          = temp;
                        }
                    }

                    if (!reg2.is_virtual) continue;

                    if (IntConstPool.find(reg2.reg_num) != IntConstPool.end())
                    {
                        int      const_val  = IntConstPool[reg2.reg_num];
                        Register result_reg = rv_inst->rd;

                        if (const_val == 1)
                        {
                            it = block->insts.erase(it);
                            block->insts.insert(it, createMoveInst(INT64, result_reg, reg1));
                            --it;
                        }
                        else if (const_val == -1)
                        {
                            it              = block->insts.erase(it);
                            RV64InstType op = RV64InstType::SUBW;
                            if (cur_op == RV64InstType::MUL)
                                op = RV64InstType::SUB;
                            else if (cur_op == RV64InstType::MULW)
                                op = RV64InstType::SUBW;
                            block->insts.insert(it, createRInst(op, result_reg, preg_x0, reg1));
                            --it;
                        }
                        else if (__builtin_popcount(const_val) == 1)
                        {
                            it              = block->insts.erase(it);
                            RV64InstType op = cur_op;
                            if (cur_op == RV64InstType::MUL)
                                op = RV64InstType::SLLI;
                            else if (cur_op == RV64InstType::MULW)
                                op = RV64InstType::SLLIW;
                            block->insts.insert(it, createIInst(op, result_reg, reg1, __builtin_ctz(const_val)));
                            --it;
                        }
                        else if (__builtin_popcount(const_val + 1) == 1)
                        {
                            // x * (2^n - 1) = (x << n) - x
                            it               = block->insts.erase(it);
                            RV64InstType op  = cur_op;
                            RV64InstType op2 = cur_op;
                            if (cur_op == RV64InstType::MUL)
                            {
                                op  = RV64InstType::SLLI;
                                op2 = RV64InstType::SUB;
                            }
                            else if (cur_op == RV64InstType::MULW)
                            {
                                op  = RV64InstType::SLLIW;
                                op2 = RV64InstType::SUBW;
                            }
                            Register mid_reg = func->getNewReg(INT64);
                            block->insts.insert(it, createIInst(op, mid_reg, reg1, __builtin_ctz(const_val + 1)));
                            block->insts.insert(it, createRInst(op2, result_reg, mid_reg, reg1));
                            --it;
                        }
                        else if (__builtin_popcount(const_val - 1) == 1)
                        {
                            // x * (2^n + 1) = (x << n) + x
                            it               = block->insts.erase(it);
                            RV64InstType op  = cur_op;
                            RV64InstType op2 = cur_op;
                            if (cur_op == RV64InstType::MUL)
                            {
                                op  = RV64InstType::SLLI;
                                op2 = RV64InstType::ADD;
                            }
                            else if (cur_op == RV64InstType::MULW)
                            {
                                op  = RV64InstType::SLLIW;
                                op2 = RV64InstType::ADDW;
                            }
                            Register mid_reg = func->getNewReg(INT64);
                            block->insts.insert(it, createIInst(op, mid_reg, reg1, __builtin_ctz(const_val - 1)));
                            block->insts.insert(it, createRInst(op2, result_reg, mid_reg, reg1));
                            --it;
                        }
                    }
                }
                else if (cur_op == RV64InstType::DIV || cur_op == RV64InstType::DIVW || cur_op == RV64InstType::REM ||
                         cur_op == RV64InstType::REMW)
                {
                    Register reg1 = rv_inst->rs1;
                    Register reg2 = rv_inst->rs2;

                    if (!reg2.is_virtual) continue;
                    if (IntConstPool.find(reg2.reg_num) == IntConstPool.end()) continue;

                    int      const_val  = IntConstPool[reg2.reg_num];
                    bool     negative   = false;
                    Register result_reg = rv_inst->rd;

                    if (cur_op == RV64InstType::DIV || cur_op == RV64InstType::DIVW)
                    {
                        if (const_val < 0)
                        {
                            const_val  = -const_val;
                            negative   = true;
                            result_reg = func->getNewReg(INT64);
                        }

                        if (const_val == 1)
                        {
                            it = block->insts.erase(it);
                            block->insts.insert(it, createMoveInst(INT64, result_reg, reg1));
                        }
                        else if (const_val == -1)
                        {
                            it              = block->insts.erase(it);
                            RV64InstType op = RV64InstType::SUBW;
                            if (cur_op == RV64InstType::DIV)
                                op = RV64InstType::SUB;
                            else if (cur_op == RV64InstType::DIVW)
                                op = RV64InstType::SUBW;
                            block->insts.insert(it, createRInst(op, result_reg, preg_x0, reg1));
                        }
                        else if (const_val == 2)
                        {
                            it                    = block->insts.erase(it);
                            Register     mid_reg  = func->getNewReg(INT64);
                            Register     mid2_reg = func->getNewReg(INT64);
                            RV64InstType srli_op =
                                (cur_op == RV64InstType::DIVW) ? RV64InstType::SRLIW : RV64InstType::SRLI;
                            block->insts.insert(it, createIInst(srli_op, mid_reg, reg1, 31));
                            block->insts.insert(it, createRInst(RV64InstType::ADD, mid2_reg, reg1, mid_reg));
                            RV64InstType srai_op =
                                (cur_op == RV64InstType::DIVW) ? RV64InstType::SRAIW : RV64InstType::SRAI;
                            block->insts.insert(it, createIInst(srai_op, result_reg, mid2_reg, 1));
                        }
                        else if (__builtin_popcount(const_val) == 1)
                        {
                            int log           = __builtin_ctz(const_val);
                            it                = block->insts.erase(it);
                            Register mid_reg  = func->getNewReg(INT64);
                            Register mid2_reg = func->getNewReg(INT64);
                            Register mid3_reg = func->getNewReg(INT64);
                            block->insts.insert(it, createIInst(RV64InstType::SLLI, mid_reg, reg1, 1));
                            block->insts.insert(it, createIInst(RV64InstType::SRLI, mid2_reg, mid_reg, 64 - log));
                            block->insts.insert(it, createRInst(RV64InstType::ADD, mid3_reg, reg1, mid2_reg));
                            RV64InstType srai_op =
                                (cur_op == RV64InstType::DIVW) ? RV64InstType::SRAIW : RV64InstType::SRAI;
                            block->insts.insert(it, createIInst(srai_op, result_reg, mid3_reg, log));
                        }
                        else
                        {
                            Multiplier multiplier = chooseMultiplier(const_val, 31);
                            int        mult       = multiplier.m & (0xFFFFFFFF);
                            Register   imm_reg    = func->getNewReg(INT64);
                            Register   mul_reg    = func->getNewReg(INT64);
                            it                    = block->insts.erase(it);
                            block->insts.insert(it, createMoveInst(INT64, imm_reg, mult));
                            block->insts.insert(it, createRInst(RV64InstType::MUL, mul_reg, reg1, imm_reg));

                            if (mult > 0)
                            {
                                Register sign_reg   = func->getNewReg(INT64);
                                Register preres_reg = func->getNewReg(INT64);
                                block->insts.insert(it, createIInst(RV64InstType::SRLI, sign_reg, mul_reg, 63));
                                RV64InstType op = RV64InstType::SRAI;
                                if (multiplier.l == 0) { op = RV64InstType::SRLI; }
                                block->insts.insert(it, createIInst(op, preres_reg, mul_reg, 32 + multiplier.l));
                                block->insts.insert(
                                    it, createRInst(RV64InstType::ADD, result_reg, preres_reg, sign_reg));
                            }
                            else
                            {
                                Register premul_reg  = func->getNewReg(INT64);
                                Register realmul_reg = func->getNewReg(INT64);
                                Register sign_reg    = func->getNewReg(INT64);
                                Register preres_reg  = func->getNewReg(INT64);
                                block->insts.insert(it, createIInst(RV64InstType::SRLI, premul_reg, mul_reg, 32));
                                block->insts.insert(it, createRInst(RV64InstType::ADD, realmul_reg, reg1, premul_reg));
                                RV64InstType srli_op =
                                    (cur_op == RV64InstType::DIVW) ? RV64InstType::SRLIW : RV64InstType::SRLI;
                                RV64InstType srai_op =
                                    (cur_op == RV64InstType::DIVW) ? RV64InstType::SRAIW : RV64InstType::SRAI;
                                block->insts.insert(it, createIInst(srli_op, sign_reg, realmul_reg, 31));
                                block->insts.insert(it, createIInst(srai_op, preres_reg, realmul_reg, multiplier.l));
                                block->insts.insert(
                                    it, createRInst(RV64InstType::ADD, result_reg, preres_reg, sign_reg));
                            }
                        }

                        if (negative)
                            block->insts.insert(it, createRInst(RV64InstType::SUBW, rv_inst->rd, preg_x0, result_reg));

                        --it;
                    }
                    else if (cur_op == RV64InstType::REM || cur_op == RV64InstType::REMW)
                    {
                        if (const_val < 0) const_val = -const_val;

                        if (const_val == 1)
                        {
                            it = block->insts.erase(it);
                            block->insts.insert(it, createMoveInst(INT64, rv_inst->rd, 0));
                            --it;
                        }
                        else if (const_val == 2)
                        {
                            it                    = block->insts.erase(it);
                            Register     mid1_reg = func->getNewReg(INT64);
                            Register     mid2_reg = func->getNewReg(INT64);
                            Register     mid3_reg = func->getNewReg(INT64);
                            RV64InstType srli_op =
                                (cur_op == RV64InstType::REMW) ? RV64InstType::SRLIW : RV64InstType::SRLI;
                            block->insts.insert(it, createIInst(srli_op, mid1_reg, reg1, 31));
                            block->insts.insert(it, createRInst(RV64InstType::ADD, mid2_reg, mid1_reg, reg1));
                            block->insts.insert(it, createIInst(RV64InstType::ANDI, mid3_reg, mid2_reg, -2));
                            block->insts.insert(it, createRInst(RV64InstType::SUBW, rv_inst->rd, reg1, mid3_reg));
                            --it;
                        }
                        else if (__builtin_popcount(const_val) == 1)
                        {
                            int log           = __builtin_ctz(const_val);
                            it                = block->insts.erase(it);
                            Register mid1_reg = func->getNewReg(INT64);
                            Register mid2_reg = func->getNewReg(INT64);
                            Register mid3_reg = func->getNewReg(INT64);
                            Register mid4_reg = func->getNewReg(INT64);
                            block->insts.insert(it, createIInst(RV64InstType::SLLI, mid1_reg, reg1, 1));
                            block->insts.insert(it, createIInst(RV64InstType::SRLI, mid2_reg, mid1_reg, 64 - log));
                            block->insts.insert(it, createRInst(RV64InstType::ADD, mid3_reg, mid2_reg, reg1));
                            block->insts.insert(it, createIInst(RV64InstType::ANDI, mid4_reg, mid3_reg, -const_val));
                            block->insts.insert(it, createRInst(RV64InstType::SUBW, rv_inst->rd, reg1, mid4_reg));
                            --it;
                        }
                        else
                        {
                            Multiplier multiplier = chooseMultiplier(const_val, 31);
                            int        mult       = multiplier.m & (0xFFFFFFFF);
                            Register   imm_reg    = func->getNewReg(INT64);
                            Register   mul_reg    = func->getNewReg(INT64);
                            Register   div_reg    = func->getNewReg(INT64);
                            it                    = block->insts.erase(it);
                            block->insts.insert(it, createMoveInst(INT64, imm_reg, mult));
                            block->insts.insert(it, createRInst(RV64InstType::MUL, mul_reg, reg1, imm_reg));

                            if (mult > 0)
                            {
                                Register sign_reg   = func->getNewReg(INT64);
                                Register preres_reg = func->getNewReg(INT64);
                                block->insts.insert(it, createIInst(RV64InstType::SRLI, sign_reg, mul_reg, 63));
                                RV64InstType op = RV64InstType::SRAI;
                                if (multiplier.l == 0) { op = RV64InstType::SRLI; }
                                block->insts.insert(it, createIInst(op, preres_reg, mul_reg, 32 + multiplier.l));
                                block->insts.insert(it, createRInst(RV64InstType::ADD, div_reg, preres_reg, sign_reg));
                            }
                            else
                            {
                                Register premul_reg  = func->getNewReg(INT64);
                                Register realmul_reg = func->getNewReg(INT64);
                                Register sign_reg    = func->getNewReg(INT64);
                                Register preres_reg  = func->getNewReg(INT64);
                                block->insts.insert(it, createIInst(RV64InstType::SRLI, premul_reg, mul_reg, 32));
                                block->insts.insert(it, createRInst(RV64InstType::ADD, realmul_reg, reg1, premul_reg));
                                RV64InstType srli_op =
                                    (cur_op == RV64InstType::REMW) ? RV64InstType::SRLIW : RV64InstType::SRLI;
                                RV64InstType srai_op =
                                    (cur_op == RV64InstType::REMW) ? RV64InstType::SRAIW : RV64InstType::SRAI;
                                block->insts.insert(it, createIInst(srli_op, sign_reg, realmul_reg, 31));
                                block->insts.insert(it, createIInst(srai_op, preres_reg, realmul_reg, multiplier.l));
                                block->insts.insert(it, createRInst(RV64InstType::ADD, div_reg, preres_reg, sign_reg));
                            }

                            Register mul_imm     = func->getNewReg(INT64);
                            Register product_reg = func->getNewReg(INT64);
                            block->insts.insert(it, createMoveInst(INT64, mul_imm, const_val));
                            block->insts.insert(it, createRInst(RV64InstType::MUL, product_reg, div_reg, mul_imm));
                            block->insts.insert(it, createRInst(RV64InstType::SUBW, rv_inst->rd, reg1, product_reg));
                            --it;
                        }
                    }
                }
            }
        }
    }

    Multiplier ArithmeticStrengthReductionPass::chooseMultiplier(uint32_t d, int p)
    {
        constexpr int N    = 32;
        int           l    = N - __builtin_clz(d - 1);
        uint64_t      low  = (uint64_t(1) << (N + l)) / d;
        uint64_t      high = ((uint64_t(1) << (N + l)) + (uint64_t(1) << (N + l - p))) / d;
        while ((low >> 1) < (high >> 1) && l > 0) low >>= 1, high >>= 1, --l;
        return {high, l};
    }

}  // namespace Backend::RV64::Passes::Optimize
