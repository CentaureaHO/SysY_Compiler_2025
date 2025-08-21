#include "arith_inst_reduce.h"
#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdlib>

namespace Transform
{
    ArithInstReduce::ArithInstReduce(LLVMIR::IR* ir) : Pass(ir) {}

    void ArithInstReduce::Execute()
    {
        cur_cfg = nullptr;

        for (auto* func : ir->functions) runOnFunction(func);
    }

    void ArithInstReduce::runOnFunction(LLVMIR::IRFunction* func)
    {
        cur_func = func;

        cur_cfg = nullptr;
        for (const auto& [func_def, cfg_ptr] : ir->cfg)
        {
            if (func_def == func->func_def)
            {
                cur_cfg = cfg_ptr;
                break;
            }
        }

        if (!cur_cfg) return;

        for (auto* block : func->blocks) runOnBlock(block);
    }

    void ArithInstReduce::runOnBlock(LLVMIR::IRBlock* block)
    {
        auto it = block->insts.begin();
        while (it != block->insts.end())
        {
            auto* inst = *it;

            if (inst->opcode == LLVMIR::IROpCode::MUL || inst->opcode == LLVMIR::IROpCode::DIV ||
                inst->opcode == LLVMIR::IROpCode::MOD)
            {
                auto* arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                if (arith_inst && arith_inst->type == LLVMIR::DataType::I32)
                    if (optimizeArithmeticInst(arith_inst, block->insts, it)) continue;
            }
            else if (inst->opcode == LLVMIR::IROpCode::FMUL || inst->opcode == LLVMIR::IROpCode::FDIV ||
                     inst->opcode == LLVMIR::IROpCode::FADD || inst->opcode == LLVMIR::IROpCode::FSUB)
            {
                auto* arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                if (arith_inst && arith_inst->type == LLVMIR::DataType::F32)
                    if (optimizeFloatArithmeticInst(arith_inst, block->insts, it)) continue;
            }

            ++it;
        }
    }

    bool ArithInstReduce::isPowerOfTwo(int value) { return value > 0 && (value & (value - 1)) == 0; }

    int ArithInstReduce::getLog2(int value)
    {
        if (value <= 0) return -1;
        int log = 0;
        while ((1 << log) < value) log++;
        return (1 << log) == value ? log : -1;
    }

    uint32_t ArithInstReduce::getFloatBits(float value)
    {
        union
        {
            float    f;
            uint32_t i;
        } converter;
        converter.f = value;
        return converter.i;
    }

    float ArithInstReduce::makeFloatFromBits(uint32_t bits)
    {
        union
        {
            float    f;
            uint32_t i;
        } converter;
        converter.i = bits;
        return converter.f;
    }

    bool ArithInstReduce::isFloatPowerOfTwo(float value, int* exponent)
    {
        if (value <= 0.0f) return false;

        uint32_t bits = getFloatBits(value);

        // IEEE 754格式：符号位(1) + 指数位(8) + 尾数位(23)
        uint32_t sign     = (bits >> 31) & 0x1;
        uint32_t exp      = (bits >> 23) & 0xFF;
        uint32_t mantissa = bits & 0x7FFFFF;

        // 必须是正数，且尾数为0（即隐含的1.0）
        if (sign != 0 || mantissa != 0) return false;

        // 避免无穷大和NaN
        if (exp == 0 || exp == 0xFF) return false;

        if (exponent) *exponent = (int)exp - 127;

        return true;
    }

    bool ArithInstReduce::isFloatReciprocalPowerOfTwo(float value, int* exponent)
    {
        if (value <= 0.0f) return false;

        float reciprocal = 1.0f / value;
        return isFloatPowerOfTwo(reciprocal, exponent);
    }

    bool ArithInstReduce::isFloatSimpleFraction(float value, int* numerator, int* denominator)
    {
        if (value <= 0.0f) return false;

        const float common_fractions[][3] = {{1.0f / 3.0f, 1, 3},
            {2.0f / 3.0f, 2, 3},
            {1.0f / 5.0f, 1, 5},
            {2.0f / 5.0f, 2, 5},
            {3.0f / 5.0f, 3, 5},
            {4.0f / 5.0f, 4, 5},
            {1.0f / 7.0f, 1, 7},
            {2.0f / 7.0f, 2, 7},
            {3.0f / 7.0f, 3, 7},
            {4.0f / 7.0f, 4, 7},
            {5.0f / 7.0f, 5, 7},
            {6.0f / 7.0f, 6, 7},
            {0.25f, 1, 4},
            {0.75f, 3, 4},
            {0.125f, 1, 8},
            {0.375f, 3, 8},
            {0.625f, 5, 8},
            {0.875f, 7, 8}};

        const float epsilon = 1e-6f;
        for (const auto& frac : common_fractions)
        {
            if (std::abs(value - frac[0]) < epsilon)
            {
                if (numerator) *numerator = (int)frac[1];
                if (denominator) *denominator = (int)frac[2];
                return true;
            }
        }

        return false;
    }

    bool ArithInstReduce::optimizeArithmeticInst(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
        std::deque<LLVMIR::Instruction*>::iterator& it)
    {
        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::MUL: return optimizeMultiplication(inst, insts, it);
            case LLVMIR::IROpCode::DIV: return optimizeDivision(inst, insts, it);
            case LLVMIR::IROpCode::MOD: return optimizeModulo(inst, insts, it);
            default: return false;
        }
    }

    bool ArithInstReduce::optimizeMultiplication(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
        std::deque<LLVMIR::Instruction*>::iterator& it)
    {
        LLVMIR::ImmeI32Operand* imme_operand  = nullptr;
        LLVMIR::Operand*        other_operand = nullptr;

        if (inst->rhs->type == LLVMIR::OperandType::IMMEI32)
        {
            imme_operand  = dynamic_cast<LLVMIR::ImmeI32Operand*>(inst->rhs);
            other_operand = inst->lhs;
        }
        else if (inst->lhs->type == LLVMIR::OperandType::IMMEI32)
        {
            imme_operand  = dynamic_cast<LLVMIR::ImmeI32Operand*>(inst->lhs);
            other_operand = inst->rhs;
        }

        if (!imme_operand) return false;

        int value = imme_operand->value;

        if (value == 0)
        {
            // x * 0 = 0
            auto* new_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::ADD, LLVMIR::DataType::I32, getImmeI32Operand(0), getImmeI32Operand(0), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            // std::cout << "ArithInstReduce: Optimized multiplication by 0" << std::endl;
            return true;
        }

        if (value == 1)
        {
            // x * 1 = x，使用ADD x, 0实现
            auto* new_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::ADD, LLVMIR::DataType::I32, other_operand, getImmeI32Operand(0), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        if (isPowerOfTwo(value))
        {
            // x * 2^n = x << n
            int   shift_amount = getLog2(value);
            auto* new_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::SHL,
                LLVMIR::DataType::I32,
                other_operand,
                getImmeI32Operand(shift_amount),
                inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        return false;
    }

    bool ArithInstReduce::optimizeDivision(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
        std::deque<LLVMIR::Instruction*>::iterator& it)
    {
        if (inst->rhs->type != LLVMIR::OperandType::IMMEI32) return false;

        auto* imme_operand = dynamic_cast<LLVMIR::ImmeI32Operand*>(inst->rhs);
        if (!imme_operand) return false;

        int value = imme_operand->value;

        if (value == 1)
        {
            // x / 1 = x，使用ADD x, 0实现
            auto* new_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::ADD, LLVMIR::DataType::I32, inst->lhs, getImmeI32Operand(0), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            // std::cout << "ArithInstReduce: Optimized division by 1" << std::endl;
            return true;
        }

        if (isPowerOfTwo(value))
        {
            // mask = x >> 31; adjustment = mask & (2^n - 1); result = (x + adjustment) >> n
            int shift_amount = getLog2(value);
            int mask_value   = value - 1;  // 2^n - 1

            int mask_reg              = ++cur_func->max_reg;
            int adjustment_reg        = ++cur_func->max_reg;
            int adjusted_dividend_reg = ++cur_func->max_reg;

            // mask = x >> 31
            auto* mask_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ASHR,
                LLVMIR::DataType::I32,
                inst->lhs,
                getImmeI32Operand(31),
                getRegOperand(mask_reg));
            mask_inst->block_id = inst->block_id;

            // adjustment = mask & (2^n - 1)
            auto* adjustment_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::BITAND,
                LLVMIR::DataType::I32,
                getRegOperand(mask_reg),
                getImmeI32Operand(mask_value),
                getRegOperand(adjustment_reg));
            adjustment_inst->block_id = inst->block_id;

            // adjusted_dividend = x + adjustment
            auto* add_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                LLVMIR::DataType::I32,
                inst->lhs,
                getRegOperand(adjustment_reg),
                getRegOperand(adjusted_dividend_reg));
            add_inst->block_id = inst->block_id;

            // result = adjusted_dividend >> n
            auto* shift_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ASHR,
                LLVMIR::DataType::I32,
                getRegOperand(adjusted_dividend_reg),
                getImmeI32Operand(shift_amount),
                inst->res);
            shift_inst->block_id = inst->block_id;

            *it = mask_inst;
            ++it;
            it = insts.insert(it, adjustment_inst);
            ++it;
            it = insts.insert(it, add_inst);
            ++it;
            it = insts.insert(it, shift_inst);
            ++it;

            delete inst;

            return true;
        }

        return false;
    }

    bool ArithInstReduce::optimizeModulo(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
        std::deque<LLVMIR::Instruction*>::iterator& it)
    {
        if (inst->rhs->type != LLVMIR::OperandType::IMMEI32) return false;

        auto* imme_operand = dynamic_cast<LLVMIR::ImmeI32Operand*>(inst->rhs);
        if (!imme_operand) return false;

        int value = imme_operand->value;

        if (value == 1)
        {
            // x % 1 = 0
            auto* new_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::ADD, LLVMIR::DataType::I32, getImmeI32Operand(0), getImmeI32Operand(0), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            std::cout << "ArithInstReduce: Optimized modulo by 1" << std::endl;
            return true;
        }

        if (isPowerOfTwo(value))
        {
            // 1. 计算简单的位运算结果: simple_mod = x & (2^n - 1)
            // 2. 检查是否需要调整: 当x为负数且simple_mod != 0时，结果应该为负
            // 3. 使用符号位来计算最终结果

            int mask_value = value - 1;  // 2^n - 1

            int simple_mod_reg      = ++cur_func->max_reg;
            int sign_mask_reg       = ++cur_func->max_reg;
            int is_zero_mask_reg    = ++cur_func->max_reg;
            int is_nonzero_mask_reg = ++cur_func->max_reg;
            int correction_reg      = ++cur_func->max_reg;

            // simple_mod = x & (2^n - 1)
            auto* simple_mod_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::BITAND,
                LLVMIR::DataType::I32,
                inst->lhs,
                getImmeI32Operand(mask_value),
                getRegOperand(simple_mod_reg));
            simple_mod_inst->block_id = inst->block_id;

            // sign_mask = x >> 31
            auto* sign_mask_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ASHR,
                LLVMIR::DataType::I32,
                inst->lhs,
                getImmeI32Operand(31),
                getRegOperand(sign_mask_reg));
            sign_mask_inst->block_id = inst->block_id;

            // is_zero_mask = (simple_mod - 1) >> 31 (如果simple_mod == 0则为-1，否则为0)
            // 0-1 = -1 (0xFFFFFFFF)，右移31位后仍然是-1，而正数会变为0
            int   temp_sub_reg      = ++cur_func->max_reg;
            auto* temp_sub_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::SUB,
                LLVMIR::DataType::I32,
                getRegOperand(simple_mod_reg),
                getImmeI32Operand(1),
                getRegOperand(temp_sub_reg));
            temp_sub_inst->block_id = inst->block_id;

            auto* is_zero_mask_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ASHR,
                LLVMIR::DataType::I32,
                getRegOperand(temp_sub_reg),
                getImmeI32Operand(31),
                getRegOperand(is_zero_mask_reg));
            is_zero_mask_inst->block_id = inst->block_id;

            // is_nonzero_mask = ~is_zero_mask = is_zero_mask ^ -1
            auto* is_nonzero_mask_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::BITXOR,
                LLVMIR::DataType::I32,
                getRegOperand(is_zero_mask_reg),
                getImmeI32Operand(-1),
                getRegOperand(is_nonzero_mask_reg));
            is_nonzero_mask_inst->block_id = inst->block_id;

            // correction = sign_mask & is_nonzero_mask & (-2^n)
            // 分两步：先计算combined_mask = sign_mask & is_nonzero_mask
            int   combined_mask_reg      = ++cur_func->max_reg;
            auto* combined_mask_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::BITAND,
                LLVMIR::DataType::I32,
                getRegOperand(sign_mask_reg),
                getRegOperand(is_nonzero_mask_reg),
                getRegOperand(combined_mask_reg));
            combined_mask_inst->block_id = inst->block_id;

            // 然后计算correction = combined_mask & (-value)
            auto* correction_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::BITAND,
                LLVMIR::DataType::I32,
                getRegOperand(combined_mask_reg),
                getImmeI32Operand(-value),
                getRegOperand(correction_reg));
            correction_inst->block_id = inst->block_id;

            // result = simple_mod + correction
            auto* final_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                LLVMIR::DataType::I32,
                getRegOperand(simple_mod_reg),
                getRegOperand(correction_reg),
                inst->res);
            final_inst->block_id = inst->block_id;

            *it = simple_mod_inst;
            ++it;
            it = insts.insert(it, sign_mask_inst);
            ++it;
            it = insts.insert(it, temp_sub_inst);
            ++it;
            it = insts.insert(it, is_zero_mask_inst);
            ++it;
            it = insts.insert(it, is_nonzero_mask_inst);
            ++it;
            it = insts.insert(it, combined_mask_inst);
            ++it;
            it = insts.insert(it, correction_inst);
            ++it;
            it = insts.insert(it, final_inst);
            ++it;

            delete inst;

            return true;
        }

        return false;
    }

    bool ArithInstReduce::optimizeFloatArithmeticInst(LLVMIR::ArithmeticInst* inst,
        std::deque<LLVMIR::Instruction*>& insts, std::deque<LLVMIR::Instruction*>::iterator& it)
    {
        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::FMUL: return optimizeFloatMultiplication(inst, insts, it);
            case LLVMIR::IROpCode::FDIV: return optimizeFloatDivision(inst, insts, it);
            case LLVMIR::IROpCode::FADD:
            case LLVMIR::IROpCode::FSUB:
                // TODO: 实现浮点加法和减法优化
                return false;
            default: return false;
        }
    }

    bool ArithInstReduce::optimizeFloatMultiplication(LLVMIR::ArithmeticInst* inst,
        std::deque<LLVMIR::Instruction*>& insts, std::deque<LLVMIR::Instruction*>::iterator& it)
    {
        LLVMIR::ImmeF32Operand* imme_operand  = nullptr;
        LLVMIR::Operand*        other_operand = nullptr;

        if (inst->rhs->type == LLVMIR::OperandType::IMMEF32)
        {
            imme_operand  = dynamic_cast<LLVMIR::ImmeF32Operand*>(inst->rhs);
            other_operand = inst->lhs;
        }
        else if (inst->lhs->type == LLVMIR::OperandType::IMMEF32)
        {
            imme_operand  = dynamic_cast<LLVMIR::ImmeF32Operand*>(inst->lhs);
            other_operand = inst->rhs;
        }

        if (!imme_operand) return false;

        float value = imme_operand->value;

        // x * 0.0 = 0.0
        if (value == 0.0f)
        {
            auto* new_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::FADD,
                LLVMIR::DataType::F32,
                getImmeF32Operand(0.0f),
                getImmeF32Operand(0.0f),
                inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        // x * 1.0 = x，使用FADD x, 0.0实现
        if (value == 1.0f)
        {
            auto* new_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::FADD, LLVMIR::DataType::F32, other_operand, getImmeF32Operand(0.0f), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        // x * -1.0 = -x，使用FSUB 0.0, x实现
        if (value == -1.0f)
        {
            auto* new_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::FSUB, LLVMIR::DataType::F32, getImmeF32Operand(0.0f), other_operand, inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        // x * 2^n = x
        int exponent;
        if (isFloatPowerOfTwo(value, &exponent))
        {
            // 对于小的指数，可以考虑转换为加法
            if (exponent == 1)  // x * 2.0 = x + x
            {
                auto* new_inst = new LLVMIR::ArithmeticInst(
                    LLVMIR::IROpCode::FADD, LLVMIR::DataType::F32, other_operand, other_operand, inst->res);
                new_inst->block_id = inst->block_id;

                *it = new_inst;
                delete inst;
                ++it;

                return true;
            }
        }

        return false;
    }

    bool ArithInstReduce::optimizeFloatDivision(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
        std::deque<LLVMIR::Instruction*>::iterator& it)
    {
        if (inst->rhs->type != LLVMIR::OperandType::IMMEF32) return false;

        auto* imme_operand = dynamic_cast<LLVMIR::ImmeF32Operand*>(inst->rhs);
        if (!imme_operand) return false;

        float value = imme_operand->value;

        if (value == 0.0f) return false;

        // x / 1.0 = x，使用FADD x, 0.0实现
        if (value == 1.0f)
        {
            auto* new_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::FADD, LLVMIR::DataType::F32, inst->lhs, getImmeF32Operand(0.0f), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        // x / -1.0 = -x，使用FSUB 0.0, x实现
        if (value == -1.0f)
        {
            auto* new_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::FSUB, LLVMIR::DataType::F32, getImmeF32Operand(0.0f), inst->lhs, inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        // x / 2^n = x * (1/2^n)
        int exponent;
        if (isFloatPowerOfTwo(value, &exponent))
        {
            float reciprocal = 1.0f / value;
            auto* new_inst   = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::FMUL, LLVMIR::DataType::F32, inst->lhs, getImmeF32Operand(reciprocal), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        // x / (1/2^n) = x * 2^n
        if (isFloatReciprocalPowerOfTwo(value, &exponent))
        {
            float multiplier = 1.0f / value;
            auto* new_inst   = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::FMUL, LLVMIR::DataType::F32, inst->lhs, getImmeF32Operand(multiplier), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        // x / 简单分数 = x * 其倒数
        int numerator, denominator;
        if (isFloatSimpleFraction(value, &numerator, &denominator))
        {
            float reciprocal = (float)denominator / (float)numerator;
            auto* new_inst   = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::FMUL, LLVMIR::DataType::F32, inst->lhs, getImmeF32Operand(reciprocal), inst->res);
            new_inst->block_id = inst->block_id;

            *it = new_inst;
            delete inst;
            ++it;

            return true;
        }

        return false;
    }
}  // namespace Transform
