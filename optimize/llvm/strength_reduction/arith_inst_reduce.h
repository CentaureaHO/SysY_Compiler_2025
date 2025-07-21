#ifndef __LLVM_STRENGTH_REDUCTION_ARITH_INST_REDUCE_H__
#define __LLVM_STRENGTH_REDUCTION_ARITH_INST_REDUCE_H__

#include "llvm/pass.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"

namespace Transform
{
    // 将算术指令转换为更简单的形式
    // 尚未找到 BOOM v3 的指令延迟，因此可能存在负优化
    // TODO：查找 / 测试 BOOM v3 的指令延迟
    class ArithInstReduce : public Pass
    {
      public:
        ArithInstReduce(LLVMIR::IR* ir);
        virtual void Execute() override;

      private:
        CFG*                cur_cfg;
        LLVMIR::IRFunction* cur_func;

        void runOnFunction(LLVMIR::IRFunction* func);
        void runOnBlock(LLVMIR::IRBlock* block);

        bool isPowerOfTwo(int value);
        int  getLog2(int value);

        bool     isFloatPowerOfTwo(float value, int* exponent = nullptr);
        bool     isFloatReciprocalPowerOfTwo(float value, int* exponent = nullptr);
        bool     isFloatSimpleFraction(float value, int* numerator = nullptr, int* denominator = nullptr);
        uint32_t getFloatBits(float value);
        float    makeFloatFromBits(uint32_t bits);

        bool optimizeArithmeticInst(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
            std::deque<LLVMIR::Instruction*>::iterator& it);

        // 整数
        bool optimizeMultiplication(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
            std::deque<LLVMIR::Instruction*>::iterator& it);

        bool optimizeDivision(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
            std::deque<LLVMIR::Instruction*>::iterator& it);

        bool optimizeModulo(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
            std::deque<LLVMIR::Instruction*>::iterator& it);

        // 浮点
        bool optimizeFloatArithmeticInst(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
            std::deque<LLVMIR::Instruction*>::iterator& it);
        bool optimizeFloatMultiplication(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
            std::deque<LLVMIR::Instruction*>::iterator& it);
        bool optimizeFloatDivision(LLVMIR::ArithmeticInst* inst, std::deque<LLVMIR::Instruction*>& insts,
            std::deque<LLVMIR::Instruction*>::iterator& it);
    };
}  // namespace Transform

#endif  // __LLVM_STRENGTH_REDUCTION_ARITH_INST_REDUCE_H__
