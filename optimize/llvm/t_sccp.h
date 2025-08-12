#ifndef __OPTIMIZER_LLVM_T_SCCP_H__
#define __OPTIMIZER_LLVM_T_SCCP_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_find.h"
#include "llvm_ir/defs.h"
#include "dom_analyzer.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <memory>

namespace Transform
{
    /**
     * 表示一个精确的内存位置
     * @details 由真正的基址指针和偏移量构成，用于在SCCP中精确跟踪数组元素或结构体字段
     * 支持经过GEPStrengthReduce优化后的扁平化GEP指令
     */
    struct MemoryLocation
    {
        LLVMIR::Operand* base_ptr;        ///< 基址指针 (alloca 或全局变量)
        int              element_offset;  ///< 从基址开始的元素偏移量

        MemoryLocation() : base_ptr(nullptr), element_offset(0) {}
        MemoryLocation(LLVMIR::Operand* base) : base_ptr(base), element_offset(0) {}
        MemoryLocation(LLVMIR::Operand* base, int offset) : base_ptr(base), element_offset(offset) {}

        bool operator==(const MemoryLocation& other) const
        {
            return base_ptr == other.base_ptr && element_offset == other.element_offset;
        }

        bool operator!=(const MemoryLocation& other) const { return !(*this == other); }

        /// 检查该内存位置是否有效 (基址指针非空)
        bool isValid() const { return base_ptr != nullptr; }

        /// 返回内存位置的字符串表示，便于调试
        std::string toString() const;
    };

    /// 为MemoryLocation提供哈希函数，使其能用于哈希表
    struct MemoryLocationHash
    {
        size_t operator()(const MemoryLocation& loc) const
        {
            size_t hash = std::hash<void*>()(loc.base_ptr);
            hash ^= std::hash<int>()(loc.element_offset) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };

    /// 表示SCCP算法中的格值
    class LatticeValue
    {
      public:
        /// 格值的状态
        enum class ValueStatus
        {
            TOP      = 0,  ///< 未初始化，最高状态
            CONSTANT = 1,  ///< 常量
            BOTTOM   = 2   ///< 非常量或未知值，最低状态
        };

        /// 常量值的具体类型
        enum class ValueType
        {
            NONE    = 0,
            INTEGER = 1,
            FLOAT   = 2
        };

      private:
        ValueStatus status_;  ///< 当前格值的状态
        ValueType   type_;    ///< 如果是常量，其类型
        union                 ///< 存储常量值
        {
            int   int_val;
            float float_val;
        } value_;

      public:
        LatticeValue() : status_(ValueStatus::TOP), type_(ValueType::NONE) {}

        LatticeValue(int val) : status_(ValueStatus::CONSTANT), type_(ValueType::INTEGER) { value_.int_val = val; }

        LatticeValue(float val) : status_(ValueStatus::CONSTANT), type_(ValueType::FLOAT) { value_.float_val = val; }

        /// 创建一个BOTTOM格值
        static LatticeValue createBottom()
        {
            LatticeValue result;
            result.status_ = ValueStatus::BOTTOM;
            return result;
        }

        ValueStatus getStatus() const { return status_; }
        ValueType   getType() const { return type_; }
        int         getIntValue() const { return value_.int_val; }
        float       getFloatValue() const { return value_.float_val; }

        /// 判断格值的状态
        bool isTop() const { return status_ == ValueStatus::TOP; }
        bool isConstant() const { return status_ == ValueStatus::CONSTANT; }
        bool isBottom() const { return status_ == ValueStatus::BOTTOM; }

        /// 执行格的meet(交)操作，用于合并来自不同路径的值
        LatticeValue meet(const LatticeValue& other) const;

        bool operator==(const LatticeValue& other) const;
        bool operator!=(const LatticeValue& other) const { return !(*this == other); }
    };

    class InstructionVisitor;

    class TSCCPPass : public Pass
    {
      private:
        /// 指令结果 -> 结果格值
        std::unordered_map<LLVMIR::Instruction*, LatticeValue> value_map_;
        /// 指令 -> 使用者列表
        std::unordered_map<LLVMIR::Instruction*, std::vector<LLVMIR::Instruction*>> use_chains_;
        /// 寄存器 -> 定义指令
        std::unordered_map<int, LLVMIR::Instruction*> def_map_;

        /// 控制流工作列表，存储待处理的可达边
        std::vector<std::pair<int, int>> cfg_worklist_;
        /// SSA工作列表，存储值已改变，需重新访问的指令
        std::vector<LLVMIR::Instruction*> ssa_worklist_;
        /// 存储已被确定为可达的控制流边
        std::set<std::pair<int, int>> executable_edges_;

        /// 指令访问器，根据不同指令类型更新格值
        std::unique_ptr<InstructionVisitor> instruction_visitor_;
        /// 当前正在处理的函数的CFG
        CFG* current_cfg_;
        /// 跟踪内存位置的格值，用于处理load/store
        std::unordered_map<MemoryLocation, LatticeValue, MemoryLocationHash> memory_state_;

        /// 主要算法实现，分析并填充value_map_
        void runSCCPAnalysis(CFG* cfg);
        /// 根据value_map_，对指令的操作数进行常量替换
        void propagateConstants(CFG* cfg);
        /// 移除可以被确定为死代码的指令
        void eliminateDeadInstructions(CFG* cfg);

        /// 构建当前函数的def-use和use-def链
        void         buildDefUseChains(CFG* cfg);
        void         addToSSAWorklist(LLVMIR::Instruction* inst);
        void         addToCFGWorklist(int from_bb, int to_bb);
        bool         isEdgeExecutable(int from_bb, int to_bb) const;
        void         markEdgeExecutable(int from_bb, int to_bb);
        LatticeValue getValueForOperand(LLVMIR::Operand* operand) const;
        void         setValue(LLVMIR::Instruction* inst, const LatticeValue& value);
        LatticeValue getValue(LLVMIR::Instruction* inst) const;
        void         replaceWithConstant(LLVMIR::Operand*& operand, const LatticeValue& value);
        void         updateInstructionOperands(LLVMIR::Instruction* inst);
        void         mapRegToConstant(LLVMIR::Instruction* inst, int reg_num, const LatticeValue& value);

        /// 处理store指令，更新memory_state_
        void handleStoreInstruction(LLVMIR::StoreInst* store);
        /// 处理load指令，从memory_state_或通过支配树分析获取值
        LatticeValue handleLoadInstruction(LLVMIR::LoadInst* load);
        /// 从指针操作数解析出MemoryLocation
        MemoryLocation getMemoryLocation(LLVMIR::Operand* ptr) const;
        /// 收集所有可能到达load指令的store指令
        std::vector<LLVMIR::StoreInst*> collectReachingStores(LLVMIR::LoadInst* load, const MemoryLocation& loc);
        /// 使用支配树查找可能到达load指令的store指令
        void findReachingStoresWithDominatorTree(const MemoryLocation& loc, LLVMIR::LoadInst* target_load,
            Cele::Algo::DomAnalyzer* dom_analyzer, std::vector<LLVMIR::StoreInst*>& reaching_stores);

        /// 判断一个函数调用是否"安全"（即不修改可被分析的内存）
        bool isSafeFunction(const std::string& func_name) const;

        LLVMIR::Operand* getArrayBasePointer(LLVMIR::Operand* ptr) const;
        bool             isRelatedToArray(const MemoryLocation& loc, LLVMIR::Operand* array_base) const;
        bool             isSameArray(LLVMIR::Operand* ptr1, LLVMIR::Operand* ptr2) const;
        /// 检查两个内存位置是否可能指向同一地址
        bool mayAlias(const MemoryLocation& loc1, const MemoryLocation& loc2) const;
        /// 检查store指令是否存在循环依赖 (e.g., a[i] = a[i] + 1)
        bool hasCircularDependency(LLVMIR::Instruction* inst, const MemoryLocation& store_loc) const;
        bool hasCircularDependencyHelper(
            LLVMIR::Instruction* inst, const MemoryLocation& store_loc, std::set<LLVMIR::Instruction*>& visited) const;

        friend class InstructionVisitor;

      public:
        TSCCPPass(LLVMIR::IR* ir);
        virtual ~TSCCPPass();

        virtual void Execute() override;
    };

    class InstructionVisitor
    {
      private:
        TSCCPPass*           pass_;
        LLVMIR::Instruction* current_inst_;

      public:
        InstructionVisitor(TSCCPPass* pass) : pass_(pass), current_inst_(nullptr) {}

        void visit(LLVMIR::Instruction* inst);

      private:
        void visitPhi(LLVMIR::PhiInst* phi);
        void visitBranch(LLVMIR::Instruction* br);
        void visitArithmetic(LLVMIR::ArithmeticInst* arith);
        void visitComparison(LLVMIR::Instruction* cmp);
        void visitConversion(LLVMIR::Instruction* conv);
        void visitLoad(LLVMIR::LoadInst* load);
        void visitStore(LLVMIR::StoreInst* store);
        void visitCall(LLVMIR::CallInst* call);
        void visitOther(LLVMIR::Instruction* inst);

        // 编译时常量表达式运算
        LatticeValue foldBinaryOperation(LLVMIR::IROpCode opcode, const LatticeValue& lhs, const LatticeValue& rhs);
        LatticeValue foldComparisonOperation(LLVMIR::IROpCode opcode, LLVMIR::IcmpCond icond, LLVMIR::FcmpCond fcond,
            const LatticeValue& lhs, const LatticeValue& rhs);
        LatticeValue foldConversionOperation(LLVMIR::IROpCode opcode, const LatticeValue& operand);
    };

}  // namespace Transform

#endif
