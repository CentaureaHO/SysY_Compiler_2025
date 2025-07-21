#ifndef __LLVM_STRENGTH_REDUCTION_GEP_STRENGTH_REDUCE_H__
#define __LLVM_STRENGTH_REDUCTION_GEP_STRENGTH_REDUCE_H__

#include "llvm/pass.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"
#include <set>
#include <map>
#include <vector>
#include <queue>

namespace Transform
{
    class GEPStrengthReduce : public Pass
    {
      public:
        GEPStrengthReduce(LLVMIR::IR* ir);
        virtual void Execute() override;

      private:
        CFG*                cur_cfg;
        LLVMIR::IRFunction* cur_func;

        void runOnFunction(LLVMIR::IRFunction* func);

        struct GEPInfo
        {
            LLVMIR::GEPInst*              gep_inst;
            LLVMIR::Operand*              base_ptr;
            std::vector<LLVMIR::Operand*> indices;
            std::vector<int>              dims;
            int                           result_reg;
            int                           block_id;

            GEPInfo() : gep_inst(nullptr), base_ptr(nullptr), result_reg(-1), block_id(-1) {}

            GEPInfo(LLVMIR::GEPInst* inst)
                : gep_inst(inst),
                  base_ptr(inst->base_ptr),
                  indices(inst->idxs),
                  dims(inst->dims),
                  result_reg(inst->GetResultReg()),
                  block_id(inst->block_id)
            {}
        };

        struct ArithInfo
        {
            LLVMIR::ArithmeticInst* arith_inst;
            LLVMIR::Operand*        operand1;
            LLVMIR::Operand*        operand2;
            int                     result_reg;
            LLVMIR::IROpCode        opcode;

            ArithInfo()
                : arith_inst(nullptr),
                  operand1(nullptr),
                  operand2(nullptr),
                  result_reg(-1),
                  opcode(LLVMIR::IROpCode::OTHER)
            {}

            ArithInfo(LLVMIR::ArithmeticInst* inst)
                : arith_inst(inst),
                  operand1(inst->lhs),
                  operand2(inst->rhs),
                  result_reg(inst->GetResultReg()),
                  opcode(inst->opcode)
            {}
        };

        void processDominatedBlock(int block_id, std::vector<GEPInfo>& available_geps,
            std::map<int, ArithInfo>& arithmetic_defs, std::set<int>& visited);

        bool canReduceGEP(const GEPInfo& base_gep, const GEPInfo& target_gep,
            const std::map<int, ArithInfo>& arith_defs, int& offset) const;

        bool calculateOffset(const GEPInfo& base_gep, const GEPInfo& target_gep,
            const std::map<int, ArithInfo>& arith_defs, int& offset) const;

        bool operandsEqual(LLVMIR::Operand* op1, LLVMIR::Operand* op2) const;

        int calculateDimensionSize(const std::vector<int>& dims, int dim_index) const;

        bool isImmediateOperand(LLVMIR::Operand* op, int& value) const;

        bool isRegisterDefinedByArith(LLVMIR::Operand* op, const std::map<int, ArithInfo>& arith_defs,
            LLVMIR::Operand*& base_operand, int& constant_offset) const;

        void optimizeGEP(LLVMIR::GEPInst* target_gep, LLVMIR::GEPInst* base_gep, int offset);

        void collectArithmeticDefs(LLVMIR::IRBlock* block, std::map<int, ArithInfo>& arith_defs);
        void collectGEPInstructions(LLVMIR::IRBlock* block, std::vector<GEPInfo>& gep_infos);

        static const int MAX_IMMEDIATE_OFFSET = 2047;
        static const int MIN_IMMEDIATE_OFFSET = -2048;

        bool isValidImmediateOffset(int offset) const
        {
            return offset >= MIN_IMMEDIATE_OFFSET && offset <= MAX_IMMEDIATE_OFFSET;
        }
    };
}  // namespace Transform

#endif  // __LLVM_STRENGTH_REDUCTION_GEP_STRENGTH_REDUCE_H__
