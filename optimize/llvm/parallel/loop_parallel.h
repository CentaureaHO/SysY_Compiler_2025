#pragma once
#include "llvm_ir/function.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/ir_builder.h"
#include "llvm/defuse_analysis/edefuse.h"
#include "llvm/loop/scev_analysis.h"
#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include "llvm/loop/scev_analysis.h"
#include <map>
#include <set>
#include <vector>
#include <functional>

namespace Transform
{
    // 并行化决策信息
    struct ParallelizationInfo
    {
        bool        can_parallelize    = false;
        bool        should_parallelize = false;
        std::string reason             = "";

        // 依赖分析结果
        bool                              has_loop_carried_dependency = false;
        std::vector<LLVMIR::Instruction*> dependent_instructions;

        // 并行化参数
        int estimated_iterations = 0;
        int thread_count         = 4;
        int priority             = 0;  // 并行化优先级

        // 嵌套循环冲突信息
        bool             conflicts_with_inner = false;
        bool             conflicts_with_outer = false;
        std::vector<int> conflicting_loop_ids;
    };

    class LoopParallelizationPass : public Pass
    {
      public:
        LoopParallelizationPass(LLVMIR::IR* ir, Analysis::AliasAnalyser* alias_analysis = nullptr,
            Analysis::SCEVAnalyser* scev_analyser = nullptr, Analysis::EDefUseAnalysis* def_use_analysis = nullptr)
            : Pass(ir),
              alias_analysis_(alias_analysis),
              scev_analyser_(scev_analyser),
              def_use_analysis_(def_use_analysis),
              loops_processed_(0),
              loops_parallelized_(0)
        {}

        void Execute() override;

        // 获取并行化结果
        const std::map<NaturalLoop*, ParallelizationInfo>& getParallelizationResults() const
        {
            return parallelization_info_;
        }

        int getLoopsProcessed() const { return loops_processed_; }
        int getLoopsParallelized() const { return loops_parallelized_; }

      private:
        // 主要处理方法（类比LoopFullUnrollPass）
        void processAllLoops();
        void processFunction(CFG* cfg);
        bool processLoop(CFG* cfg, NaturalLoop* loop);

        // 并行化分析和决策
        bool tryParallelizeLoop(CFG* cfg, NaturalLoop* loop);
        bool canParallelize(NaturalLoop* loop) const;
        bool isParallelizationProfitable(NaturalLoop* loop, const ParallelizationInfo& info) const;

        // 依赖分析方法
        bool analyzeLoopDependencies(NaturalLoop* loop, ParallelizationInfo& info);
        bool hasLoopCarriedMemoryDependency(NaturalLoop* loop);
        bool checkInstructionDependency(NaturalLoop* loop, LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2);

        // 嵌套循环冲突处理
        void handleNestedLoopConflicts(NaturalLoop* loop);
        bool hasConflictingInnerLoops(NaturalLoop* loop);
        void markConflictingLoops(NaturalLoop* loop);

        // 并行化实施
        bool performParallelization(CFG* cfg, NaturalLoop* loop);
        void insertParallelizationMarkers(NaturalLoop* loop);
        bool generateParallelFunction(CFG* cfg, NaturalLoop* loop);
        void addParallelLibraryFunctions();
        void insertIterationCountCheck(
            CFG* cfg, NaturalLoop* loop, LLVMIR::IRBlock* check_block, int loop_depth, int inst_count);
        bool insertDynamicParallelCheck(CFG* cfg, NaturalLoop* loop);
        void redirectControlFlow(CFG* cfg, NaturalLoop* loop, LLVMIR::IRBlock* check_block);
        std::tuple<std::set<int>, std::set<int>, std::set<int>> analyzeLoopExternalVariables(
            CFG* cfg, NaturalLoop* loop);

        std::string         generateParallelFunctionName(CFG* cfg, NaturalLoop* loop) const;
        LLVMIR::IRFunction* createParallelFunction(const std::string& func_name, int max_reg);
        bool                copyLoopBodyToFunction(CFG* cfg, NaturalLoop* loop, LLVMIR::IRFunction* parallel_func,
                           const std::set<int>& i32_vars, const std::set<int>& ptr_vars, const std::set<int>& float_vars);

        // 辅助方法
        std::vector<LLVMIR::Instruction*> collectMemoryInstructions(NaturalLoop* loop);
        bool                              isSimpleForLoop(NaturalLoop* loop) const;
        int                               estimateIterationCount(NaturalLoop* loop) const;
        int                               calculateParallelizationPriority(NaturalLoop* loop) const;
        void                              logResult(NaturalLoop* loop, bool success, const std::string& reason) const;

        // 分析归纳变量
        bool analyzeLoopInductionVariables(NaturalLoop* loop);
        struct PhiMapping
        {
            LLVMIR::Instruction* original_phi;              // 原始PHI节点
            LLVMIR::Operand*     latch_incoming_value;      // 从latch来的值
            LLVMIR::Instruction* latch_def_inst;            // 在latch中定义这个值的指令
            int                  original_phi_result_reg;   // 原始PHI的结果寄存器
            LLVMIR::Operand*     preheader_incoming_value;  // 从preheader来的初始值
        };
        std::vector<PhiMapping> phi_mappings;

        // 成员变量
        Analysis::AliasAnalyser*                    alias_analysis_;
        Analysis::SCEVAnalyser*                     scev_analyser_;
        std::map<NaturalLoop*, ParallelizationInfo> parallelization_info_;
        Analysis::EDefUseAnalysis*                  def_use_analysis_;

        // 统计信息
        int                        loops_processed_;
        int                        loops_parallelized_;
        std::set<LLVMIR::Operand*> parallel_loop_global_;
        std::set<CFG*>             cannot_parallelize_cfgs_;

        void CollectAllGlobal(CFG* cfg);

        // 并行化限制常量
        static constexpr int MAX_PARALLEL_LOOPS_PER_FUNCTION = 10;
        static constexpr int MIN_ITERATIONS_FOR_PARALLEL     = 10;
        static constexpr int MIN_LOOP_SIZE_FOR_PARALLEL      = 3;

        // 循环体复制相关方法
        bool setupFunctionEntry(LLVMIR::IRFunction* parallel_func, LLVMIR::IRBlock* entry_block,
            const std::set<int>& i32_vars, const std::set<int>& ptr_vars, const std::set<int>& float_vars,
            std::map<int, int>& reg_replace_map, int& max_reg, LLVMIR::IRBlock*& new_header);

        bool createNewLoopStructure(LLVMIR::IRFunction* parallel_func, NaturalLoop* loop,
            std::map<int, int>& label_replace_map, int& max_label, LLVMIR::IRBlock*& new_header,
            LLVMIR::IRBlock*& new_latch, LLVMIR::IRBlock*& new_exit, LLVMIR::IRBlock*& entry_block);

        bool copyLoopInstructions(CFG* cfg, NaturalLoop* loop, LLVMIR::IRFunction* parallel_func,
            std::map<int, int>& reg_replace_map, std::map<int, int>& label_replace_map, int& max_reg);

        LLVMIR::Instruction* copyInstruction(LLVMIR::Instruction* old_inst, std::map<int, int>& reg_replace_map,
            std::map<int, int>& label_replace_map, int& max_reg);

        bool setupLoopControl(CFG* cfg, NaturalLoop* loop, LLVMIR::IRBlock* entry_block, LLVMIR::IRBlock* new_header,
            LLVMIR::IRBlock* new_latch, LLVMIR::IRBlock* new_exit, std::map<int, int>& reg_replace_map, int& max_reg);

        void connectControlFlow(LLVMIR::IRBlock* entry_block, LLVMIR::IRBlock* new_header, LLVMIR::IRBlock* new_exit);

        void updateJumpTargets(LLVMIR::Instruction* inst, std::map<int, int>& label_replace_map);

        void addParallelCallToBlock(CFG* cfg, NaturalLoop* loop, LLVMIR::IRBlock* block);

        // 临时存储循环边界寄存器
        int loop_start_reg_ = -1;
        int loop_end_reg_   = -1;
    };

}  // namespace Transform