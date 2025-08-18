#ifndef __OPTIMIZER_LLVM_LOOP_PARTIAL_UNROLL_H__
#define __OPTIMIZER_LLVM_LOOP_PARTIAL_UNROLL_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm/loop/scev_analysis.h"
#include "cfg.h"
#include <map>
#include <vector>

namespace Transform
{
    class LoopPartialUnrollPass : public Pass
    {
      private:
        using ValueMap = std::map<int, LLVMIR::Operand*>;
        using BlockMap = std::map<LLVMIR::IRBlock*, LLVMIR::IRBlock*>;

        static constexpr int MAX_UNROLL_FACTOR         = 8;     // 最大展开因子
        static constexpr int MAX_LOOP_BLOCKS           = 5;     // 最大循环块数
        static constexpr int MAX_INST_PER_ITERATION    = 50;    // 每次迭代最大指令数
        static constexpr int MAX_UNROLLED_INSTRUCTIONS = 400;   // 展开后最大指令数
        static constexpr int MAX_GLOBAL_INSTRUCTIONS   = 2048;  // 全局最大指令数

        Analysis::SCEVAnalyser* scev_analyser_;
        int                     loops_processed_;
        int                     loops_partially_unrolled_;

      public:
        explicit LoopPartialUnrollPass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev);
        void                Execute() override;
        std::pair<int, int> getUnrollStats() const { return {loops_processed_, loops_partially_unrolled_}; }

      private:
        void processAllLoops();
        void processFunction(CFG* cfg);
        void processLoop(CFG* cfg, NaturalLoop* loop);

        bool tryPartialUnrollLoop(CFG* cfg, NaturalLoop* loop);

        bool isSimpleLoop(NaturalLoop* loop) const;
        bool canPartialUnroll(NaturalLoop* loop) const;
        bool isBoundaryTypeSupported(NaturalLoop* loop) const;
        bool isUnrollProfitable(NaturalLoop* loop, int unroll_factor) const;

        int selectUnrollFactor(NaturalLoop* loop) const;

        bool performPartialUnroll(CFG* cfg, NaturalLoop* loop, int unroll_factor);

        struct UnrollContext
        {
            CFG*                  cfg;
            NaturalLoop*          loop;
            int                   unroll_factor;
            LLVMIR::IRBlock*      preheader;
            LLVMIR::IRBlock*      header;
            LLVMIR::IRBlock*      latch;
            LLVMIR::IRBlock*      exiting;
            LLVMIR::IRBlock*      exit;
            LLVMIR::IRBlock*      cond_block;
            LLVMIR::IRBlock*      new_preheader;
            LLVMIR::IRBlock*      remain_preheader;
            LLVMIR::IRBlock*      final_unrolled_latch;
            Analysis::LoopCRInfo* loop_info;

            LLVMIR::Operand* trip_count_upper_bound;
            LLVMIR::Operand* trip_count_lower_bound;

            // 原始条件信息
            LLVMIR::IcmpInst*       original_condition;
            LLVMIR::BranchCondInst* original_branch;

            // 余数循环相关
            LLVMIR::IRBlock*              remainder_header;
            LLVMIR::IRBlock*              remainder_latch;
            LLVMIR::IRBlock*              remainder_exit;
            std::vector<LLVMIR::IRBlock*> remainder_blocks;
            ValueMap                      remainder_value_map;

            struct PhiMapping
            {
                LLVMIR::PhiInst*     original_phi;              // 原始PHI节点
                LLVMIR::Operand*     latch_incoming_value;      // 从latch来的值
                LLVMIR::Instruction* latch_def_inst;            // 在latch中定义这个值的指令
                int                  original_phi_result_reg;   // 原始PHI的结果寄存器
                LLVMIR::Operand*     preheader_incoming_value;  // 从preheader来的初始值
            };
            std::vector<PhiMapping> phi_mappings;

            ValueMap final_value_map;
        };

        bool setupUnrollEntryInPreheader(UnrollContext& ctx);
        bool updatePhiNodesForNewPreheader(UnrollContext& ctx);
        bool unrollLoopBodies(UnrollContext& ctx);

        bool connectUnrolledLoop(UnrollContext& ctx);
        bool updateOriginalHeaderPhiNodes(UnrollContext& ctx);
        bool updateExitBlockPhiNodes(UnrollContext& ctx);

        bool createRemainderLoop(UnrollContext& ctx);
        bool cloneLoopBlocks(UnrollContext& ctx, LLVMIR::IRBlock* insert_top, LLVMIR::IRBlock* insert_bot,
            std::vector<LLVMIR::IRBlock*>& new_blocks, ValueMap& vmap);
        bool connectRemainderLoop(
            UnrollContext& ctx, const std::vector<LLVMIR::IRBlock*>& remainder_blocks, const ValueMap& vmap);
        bool updateRemainderPhiNodes(
            UnrollContext& ctx, const std::vector<LLVMIR::IRBlock*>& remainder_blocks, const ValueMap& vmap);

        bool             initializePhiInfos(UnrollContext& ctx);
        LLVMIR::Operand* findValueInFinalIteration(UnrollContext& ctx, LLVMIR::Operand* original_value);

        std::vector<LLVMIR::IRBlock*> getPredecessors(LLVMIR::IRBlock* block, CFG* cfg);
        void                          updateBranchTarget(LLVMIR::IRBlock* block, int old_target, int new_target);

        int  getLoopSize(NaturalLoop* loop) const;
        int  getGlobalInstructionCount() const;
        bool hasArrayAccess(NaturalLoop* loop) const;
        void logResult(NaturalLoop* loop, bool success, int unroll_factor, const std::string& reason) const;

        LLVMIR::IRBlock* createGuardedIntermediateBlock(UnrollContext& ctx);
        void             updateRemainderExitPhiNodes(UnrollContext& ctx, LLVMIR::IRBlock* guard_block);
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_LOOP_PARTIAL_UNROLL_H__
