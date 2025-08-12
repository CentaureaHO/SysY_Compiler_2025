#include <backend/rv64/passes/optimize/control_flow/select_reduce.h>
#include <backend/rv64/insts.h>
#include <backend/rv64/rv64_defs.h>
#include <cassert>

using namespace std;

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    SelectReducePass::SelectReducePass(std::vector<Function*>& functions) : functions_(functions) {}

    bool SelectReducePass::run()
    {
        // - 启用Zicond：czero + or
        // - 未启用Zicond：(bitwise select)
        for (auto* func : functions_) reduceFunction(func);
        return true;
    }

    void SelectReducePass::reduceFunction(Function* func)
    {
        for (auto& [bid, block] : func->cfg->blocks)
        {
            auto& insts = block->insts;
            for (auto it = insts.begin(); it != insts.end();)
            {
                Instruction* base = *it;
                if (base->inst_type != InstType::SELECT)
                {
                    ++it;
                    continue;
                }

                SelectInst* sel = static_cast<SelectInst*>(base);

                DataType* dt = sel->dst_reg.data_type;

#if RV64_ENABLE_ZICOND
                {
                    // 使用 zicond: czero + or
                    Register v3 = func->getNewReg(dt);
                    Register v4 = func->getNewReg(dt);

                    auto emitCzeroEqz = [&](Register rd, Operand* val, Register cond) {
                        if (val->operand_type == OperandType::REG)
                        {
                            Register rs1 = static_cast<RegOperand*>(val)->reg;
                            insts.insert(it, createRInst(RV64InstType::CZERO_EQZ, rd, rs1, cond));
                        }
                        else if (val->operand_type == OperandType::IMMEI32)
                        {
                            Register tmp = func->getNewReg(INT64);
                            int      iv  = static_cast<ImmeI32Operand*>(val)->val;
                            insts.insert(it, createMoveInst(INT64, tmp, iv));
                            insts.insert(it, createRInst(RV64InstType::CZERO_EQZ, rd, tmp, cond));
                        }
                        else if (val->operand_type == OperandType::IMMEF32)
                        {
                            Register tmp = func->getNewReg(FLOAT64);
                            float    fv  = static_cast<ImmeF32Operand*>(val)->val;
                            insts.insert(it, createMoveInst(FLOAT64, tmp, fv));
                            insts.insert(it, createRInst(RV64InstType::CZERO_EQZ, rd, tmp, cond));
                        }
                        else
                            assert(false);
                    };

                    auto emitCzeroNez = [&](Register rd, Operand* val, Register cond) {
                        if (val->operand_type == OperandType::REG)
                        {
                            Register rs1 = static_cast<RegOperand*>(val)->reg;
                            insts.insert(it, createRInst(RV64InstType::CZERO_NEZ, rd, rs1, cond));
                        }
                        else if (val->operand_type == OperandType::IMMEI32)
                        {
                            Register tmp = func->getNewReg(INT64);
                            int      iv  = static_cast<ImmeI32Operand*>(val)->val;
                            insts.insert(it, createMoveInst(INT64, tmp, iv));
                            insts.insert(it, createRInst(RV64InstType::CZERO_NEZ, rd, tmp, cond));
                        }
                        else if (val->operand_type == OperandType::IMMEF32)
                        {
                            Register tmp = func->getNewReg(FLOAT64);
                            float    fv  = static_cast<ImmeF32Operand*>(val)->val;
                            insts.insert(it, createMoveInst(FLOAT64, tmp, fv));
                            insts.insert(it, createRInst(RV64InstType::CZERO_NEZ, rd, tmp, cond));
                        }
                        else
                            assert(false);
                    };

                    emitCzeroEqz(v3, sel->true_val, sel->cond_reg);
                    emitCzeroNez(v4, sel->false_val, sel->cond_reg);
                    insts.insert(it, createRInst(RV64InstType::OR, sel->dst_reg, v3, v4));
                }
#else
                {
                    // 无 zicond：按位选择
                    // 对整数：r = b ^ ((a ^ b) & mask)，其中 mask = -cond
                    // 对浮点：将 a,b bitcast 到int，按位选择后再 fmv.w.x 回写

                    auto ensureReg = [&](Operand* val, DataType* type) -> Register {
                        if (val->operand_type == OperandType::REG) return static_cast<RegOperand*>(val)->reg;
                        if (type == INT64)
                        {
                            Register tmp = func->getNewReg(INT64);
                            int      iv  = static_cast<ImmeI32Operand*>(val)->val;
                            insts.insert(it, createMoveInst(INT64, tmp, iv));
                            return tmp;
                        }
                        else if (type == FLOAT64)
                        {
                            Register tmp = func->getNewReg(FLOAT64);
                            float    fv  = static_cast<ImmeF32Operand*>(val)->val;
                            insts.insert(it, createMoveInst(FLOAT64, tmp, fv));
                            return tmp;
                        }
                        assert(false);
                        return func->getNewReg(INT64);
                    };

                    // cond_mask = -cond
                    Register cond_mask = func->getNewReg(INT64);
                    insts.insert(it, createRInst(RV64InstType::SUB, cond_mask, preg_x0, sel->cond_reg));

                    if (dt->data_type == DataType::INT)
                    {
                        Register a = ensureReg(sel->true_val, INT64);
                        Register b = ensureReg(sel->false_val, INT64);
                        Register t = func->getNewReg(INT64);
                        Register u = func->getNewReg(INT64);

                        insts.insert(it, createRInst(RV64InstType::XOR, t, a, b));
                        insts.insert(it, createRInst(RV64InstType::AND, u, t, cond_mask));
                        insts.insert(it, createRInst(RV64InstType::XOR, sel->dst_reg, b, u));
                    }
                    else if (dt->data_type == DataType::FLOAT)
                    {
                        // a_f, b_f to integer bit patterns
                        Register a_f = ensureReg(sel->true_val, FLOAT64);
                        Register b_f = ensureReg(sel->false_val, FLOAT64);

                        Register a_i = func->getNewReg(INT64);
                        Register b_i = func->getNewReg(INT64);
                        Register t   = func->getNewReg(INT64);
                        Register u   = func->getNewReg(INT64);

                        insts.insert(it, createR2Inst(RV64InstType::FMV_X_W, a_i, a_f));
                        insts.insert(it, createR2Inst(RV64InstType::FMV_X_W, b_i, b_f));
                        insts.insert(it, createRInst(RV64InstType::XOR, t, a_i, b_i));
                        insts.insert(it, createRInst(RV64InstType::AND, u, t, cond_mask));

                        Register r_i = func->getNewReg(INT64);
                        insts.insert(it, createRInst(RV64InstType::XOR, r_i, b_i, u));
                        insts.insert(it, createR2Inst(RV64InstType::FMV_W_X, sel->dst_reg, r_i));
                    }
                    else
                        assert(false);
                }
#endif

                it = insts.erase(it);
                delete sel;
            }
        }
    }

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow
