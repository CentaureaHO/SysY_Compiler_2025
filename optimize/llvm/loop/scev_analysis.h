#ifndef __OPTIMIZER_LLVM_LOOP_SCEV_ANALYSIS_H__
#define __OPTIMIZER_LLVM_LOOP_SCEV_ANALYSIS_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm_ir/ir_builder.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/defs.h"
#include "cfg.h"
#include <map>
#include <optional>
#include <set>
#include <vector>
#include <memory>

namespace Analysis
{
    enum class CROperator
    {
        ADD,  ///< 加法运算 +
        MUL   ///< 乘法运算 *
    };

    enum class ArithOp
    {
        ADD,
        SUB,
        MUL
    };

    class ArithmeticExpr
    {
      public:
        ArithOp                          op;
        std::unique_ptr<class CROperand> lhs;
        std::unique_ptr<class CROperand> rhs;

      public:
        ArithmeticExpr(ArithOp operation, std::unique_ptr<CROperand> left, std::unique_ptr<CROperand> right)
            : op(operation), lhs(std::move(left)), rhs(std::move(right))
        {}

        ArithmeticExpr(const ArithmeticExpr& other);
        ArithmeticExpr& operator=(const ArithmeticExpr& other);
        ArithmeticExpr(ArithmeticExpr&& other) noexcept            = default;
        ArithmeticExpr& operator=(ArithmeticExpr&& other) noexcept = default;

        std::optional<int32_t> getConstantValue() const;
        LLVMIR::Instruction*   generateInstruction(CFG* cfg) const;
        bool                   isLoopInvariant(const std::set<int>& invariant_set) const;
        void                   print() const;
        bool                   operator==(const ArithmeticExpr& other) const;

        std::unique_ptr<CROperand> simplify() const;
    };

    class CROperand
    {
      public:
        enum Type
        {
            CONSTANT,
            LLVM_OPERAND,
            CHAIN_OF_RECURRENCES,
            ARITHMETIC_EXPR
        };

        Type type;
        union
        {
            int32_t          const_val;
            LLVMIR::Operand* llvm_op;
        };
        std::unique_ptr<class ChainOfRecurrences> nested_cr;
        std::unique_ptr<class ArithmeticExpr>     arith_expr;

      public:
        CROperand() : type(CONSTANT), const_val(0) {}
        explicit CROperand(int32_t val) : type(CONSTANT), const_val(val) {}
        explicit CROperand(LLVMIR::Operand* op) : type(LLVM_OPERAND), llvm_op(op) {}
        explicit CROperand(std::unique_ptr<ChainOfRecurrences> cr);
        explicit CROperand(std::unique_ptr<class ArithmeticExpr> expr);

        CROperand(const CROperand& other);
        CROperand& operator=(const CROperand& other);
        CROperand(CROperand&& other) noexcept;
        CROperand& operator=(CROperand&& other) noexcept;
        ~CROperand() = default;

        std::optional<int32_t> getConstantValue() const;
        LLVMIR::Instruction*   generateInstruction(CFG* cfg) const;
        bool                   isLoopInvariant(const std::set<int>& invariant_set) const;
        void                   print() const;
        bool                   operator==(const CROperand& other) const;

        static CROperand createAdd(const CROperand& lhs, const CROperand& rhs);
        static CROperand createMul(const CROperand& lhs, const CROperand& rhs);
        static CROperand createSub(const CROperand& lhs, const CROperand& rhs);
    };

    class ChainOfRecurrences
    {
      public:
        std::vector<CROperand>  operands;   ///< 操作数序列
        std::vector<CROperator> operators;  ///< 操作符序列

      public:
        ChainOfRecurrences() = default;
        ChainOfRecurrences(const CROperand& start, CROperator op, const CROperand& step);
        ChainOfRecurrences(const std::vector<CROperand>& ops, const std::vector<CROperator>& opers);

        size_t length() const { return operands.size() - 1; }
        bool   isPureSum() const;
        bool   isPureProduct() const;
        bool   isSimple() const;
        int    getCostIndex() const;

        ChainOfRecurrences operator+(const ChainOfRecurrences& other) const;
        ChainOfRecurrences operator*(const ChainOfRecurrences& other) const;
        ChainOfRecurrences operator+(const CROperand& constant) const;
        ChainOfRecurrences operator*(const CROperand& constant) const;

        static ChainOfRecurrences createWithArithmeticExpr(
            const ChainOfRecurrences& base, const CROperand& operand, CROperator op);

        CROperand evaluateClosedForm(int iteration) const;

        void                              print() const;
        std::vector<LLVMIR::Instruction*> generateInstructions(CFG* cfg) const;
    };

    class CRBuilder
    {
      public:
        static std::unique_ptr<ChainOfRecurrences> createFromBasicInductionVar(
            LLVMIR::Operand* start, LLVMIR::Operand* step, CROperator op = CROperator::ADD);

        static std::unique_ptr<ChainOfRecurrences> createFromArithmeticExpr(
            LLVMIR::ArithmeticInst* inst, const std::map<int, std::unique_ptr<ChainOfRecurrences>>& cr_map);

        // 基本运算
        static std::unique_ptr<ChainOfRecurrences> crAdd(const ChainOfRecurrences& lhs, const ChainOfRecurrences& rhs);
        static std::unique_ptr<ChainOfRecurrences> crMul(const ChainOfRecurrences& lhs, const ChainOfRecurrences& rhs);
        static std::unique_ptr<ChainOfRecurrences> crProd(const ChainOfRecurrences& phi, const ChainOfRecurrences& psi);
        static std::unique_ptr<ChainOfRecurrences> crExpt(
            const ChainOfRecurrences& base, const ChainOfRecurrences& exponent);

        static std::unique_ptr<ChainOfRecurrences> simplify(std::unique_ptr<ChainOfRecurrences> cr);
        static std::unique_ptr<ChainOfRecurrences> constantFold(std::unique_ptr<ChainOfRecurrences> cr);
    };

    class ForLoopInfo
    {
      public:
        CROperand           lowerbound;            ///< 下界
        CROperand           upperbound;            ///< 上界
        CROperand           step;                  ///< 步长
        LLVMIR::IcmpCond    cond;                  ///< 比较条件
        bool                is_upperbound_closed;  ///< 上界是否闭合
        ChainOfRecurrences* selected_iv_cr;        ///< 选中的归纳变量CR

      public:
        ForLoopInfo() : cond(LLVMIR::IcmpCond::SLT), is_upperbound_closed(false), selected_iv_cr(nullptr) {}

        /// 判断是否为乘法递增的归纳变量
        bool isMultiplicativeInductionVar() const { return selected_iv_cr && selected_iv_cr->isPureProduct(); }
    };

    class LoopCRInfo
    {
      public:
        NaturalLoop*                                       loop;            ///< 循环指针
        std::set<int>                                      invariant_regs;  ///< 循环不变量寄存器
        std::map<int, std::unique_ptr<ChainOfRecurrences>> cr_map;          ///< 寄存器到CR的映射
        bool                                               is_simple_loop;  ///< 是否为简单循环
        ForLoopInfo                                        loop_info;       ///< 循环信息

      public:
        explicit LoopCRInfo(NaturalLoop* l) : loop(l), is_simple_loop(false) {}

        ChainOfRecurrences* getCR(int reg_num) const;
        bool isInvariant(int reg_num) const { return invariant_regs.find(reg_num) != invariant_regs.end(); }
        bool canUnroll() const;
        std::optional<int> getConstantTripCount() const;

        void printInfo() const;
    };

    class SCEVAnalyser
    {
      private:
        LLVMIR::IR*                                         ir_;             ///< IR指针
        std::map<int, LLVMIR::Instruction*>                 result_map_;     ///< 寄存器到定义指令的映射
        std::map<NaturalLoop*, std::unique_ptr<LoopCRInfo>> loop_analysis_;  ///< 循环分析结果

      public:
        explicit SCEVAnalyser(LLVMIR::IR* ir) : ir_(ir) {}

        void run();
        void clear();

        void analyzeSingleLoop(NaturalLoop* loop);

        LoopCRInfo*         getLoopInfo(NaturalLoop* loop) const;
        ChainOfRecurrences* getCR(NaturalLoop* loop, int reg_num) const;
        bool                isLoopInvariant(NaturalLoop* loop, int reg_num) const;

        std::vector<NaturalLoop*> getAnalyzedLoops() const;
        bool                      canFullyUnrollLoop(NaturalLoop* loop) const;
        std::optional<int>        getLoopTripCount(NaturalLoop* loop) const;

        void printAllResults() const;

      private:
        void analyzeLoop(CFG* cfg, NaturalLoop* loop);
        void buildResultMap(CFG* cfg);

        void findInvariantVars(LoopCRInfo* info, CFG* cfg);
        void findBasicInductionVars(LoopCRInfo* info, CFG* cfg);
        void findCRRecurrences(LoopCRInfo* info, CFG* cfg);
        void analyzeSimpleLoop(LoopCRInfo* info, CFG* cfg);

        bool isI32Invariant(LLVMIR::Operand* operand, const std::set<int>& invariant_set) const;
        std::pair<LLVMIR::Operand*, LLVMIR::IROpCode> findBasicIndVarCycleVarDef(
            LoopCRInfo* info, int start_reg, int target_reg);
    };
}  // namespace Analysis

#endif  // __OPTIMIZER_LLVM_LOOP_SCEV_ANALYSIS_H__
