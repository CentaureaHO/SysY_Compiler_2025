#ifndef __OPTIMIZER_LLVM_LOOP_INDVARS_SIMPLIFY_H__
#define __OPTIMIZER_LLVM_LOOP_INDVARS_SIMPLIFY_H__

#include "../pass.h"
#include "loop_def.h"
#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/defs.h"
#include "cfg.h"
#include "scev_analysis.h"
#include <map>
#include <optional>
#include <set>
#include <vector>
#include <memory>

namespace Transform
{
    struct InductionVariable
    {
        LLVMIR::PhiInst*              phi_inst;      ///< PHI指令
        LLVMIR::Operand*              start_value;   ///< 初始值
        LLVMIR::Operand*              step_value;    ///< 步长值
        LLVMIR::IROpCode              step_opcode;   ///< 步长运算符 (ADD/SUB)
        bool                          is_canonical;  ///< 是否已经是规范形式 (start=0, step=1)
        Analysis::ChainOfRecurrences* scev_expr;     ///< SCEV表达式
    };

    struct DerivedExpression
    {
        LLVMIR::Instruction* instruction;  ///< 指令
        LLVMIR::Operand*     iv_operand;   ///< 基础归纳变量
        LLVMIR::Operand*     offset;       ///< 偏移量
        LLVMIR::Operand*     scale;        ///< 缩放因子
        bool                 is_address;   ///< 是否为地址计算(GEP)
    };

    class IndVarsSimplifyPass : public Pass
    {
      private:
        Analysis::SCEVAnalyser* scev_analyser_;

        void                           processFunction(CFG* cfg);
        bool                           processLoop(CFG* cfg, NaturalLoop* loop);
        std::vector<InductionVariable> findInductionVariables(CFG* cfg, NaturalLoop* loop);
        LLVMIR::PhiInst*               createCanonicalInductionVariable(
                          CFG* cfg, NaturalLoop* loop, const std::vector<InductionVariable>& ivs);
        void rewriteInductionVariables(
            CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* canonical_iv, const std::vector<InductionVariable>& ivs);

        void canonicalizeExitConditions(CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* canonical_iv);

        std::vector<DerivedExpression> findDerivedExpressions(
            CFG* cfg, NaturalLoop* loop, const std::vector<InductionVariable>& ivs);
        void recomputeDerivedExpressionsOutside(CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* canonical_iv,
            const std::vector<DerivedExpression>& derived_exprs);

        void promotePointerArithmetic(CFG* cfg, NaturalLoop* loop);

        bool isInductionIncrement(
            LLVMIR::Instruction* inst, LLVMIR::PhiInst* phi, LLVMIR::Operand*& step, LLVMIR::IROpCode& opcode);

        std::optional<LLVMIR::Operand*> computeTripCount(CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* canonical_iv);
        LLVMIR::Instruction*            createCanonicalExitCondition(
                       CFG* cfg, LLVMIR::PhiInst* canonical_iv, LLVMIR::Operand* trip_count, LLVMIR::IRBlock* exiting_block);

        bool isLoopInvariant(LLVMIR::Operand* operand, NaturalLoop* loop);
        void insertInstructionAt(LLVMIR::IRBlock* block, LLVMIR::Instruction* inst, size_t position = 0);

        LLVMIR::PhiInst* getPhiForCounter(CFG* cfg, NaturalLoop* loop, int reg_num);
        LLVMIR::PhiInst* getPhiForCounterRec(CFG* cfg, NaturalLoop* loop, int reg_num, std::set<int>& visited);
        LLVMIR::PhiInst* findPhiInHeader(NaturalLoop* loop, int reg_num);

        void replaceUsesInLoop(CFG* cfg, NaturalLoop* loop, int old_reg, LLVMIR::Operand* new_operand);
        bool isIVBasedExit(CFG* cfg, NaturalLoop* loop, LLVMIR::IcmpInst* icmp, LLVMIR::PhiInst* canonical_iv);

      public:
        IndVarsSimplifyPass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev = nullptr);
        virtual ~IndVarsSimplifyPass() = default;

        virtual void Execute() override;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_LOOP_INDVARS_SIMPLIFY_H__
