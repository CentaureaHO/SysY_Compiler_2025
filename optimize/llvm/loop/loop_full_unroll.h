#ifndef __OPTIMIZER_LLVM_LOOP_FULL_UNROLL_H__
#define __OPTIMIZER_LLVM_LOOP_FULL_UNROLL_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm/loop/scev_analysis.h"
#include "cfg.h"
#include <map>
#include <vector>

namespace Transform
{
    class LoopFullUnrollPass : public Pass
    {
      private:
        static constexpr int    MAX_UNROLLED_INSTRUCTIONS = 1024;  // 降低到1024，参考旧实现
        static constexpr int    MAX_LOOP_BLOCKS           = 5;     // 循环基本块数量限制
        static constexpr int    MAX_GLOBAL_INSTRUCTIONS   = 2048;  // 全局指令数限制
        Analysis::SCEVAnalyser* scev_analyser_;
        int                     loops_processed_;
        int                     loops_fully_unrolled_;

      public:
        explicit LoopFullUnrollPass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev);
        void                Execute() override;
        std::pair<int, int> getUnrollStats() const { return {loops_processed_, loops_fully_unrolled_}; }

      private:
        void processAllLoops();
        void processFunction(CFG* cfg);
        void processLoop(CFG* cfg, NaturalLoop* loop);

        bool tryFullyUnrollLoop(CFG* cfg, NaturalLoop* loop);

        bool canFullyUnroll(NaturalLoop* loop) const;
        bool isUnrollProfitable(NaturalLoop* loop, int trip_count) const;

        bool performFullUnroll(CFG* cfg, NaturalLoop* loop, int trip_count);

        using ValueMap = std::map<int, LLVMIR::Operand*>;
        using BlockMap = std::map<LLVMIR::IRBlock*, LLVMIR::IRBlock*>;

        int  getLoopSize(NaturalLoop* loop) const;
        int  getGlobalInstructionCount() const;  // 检查全局指令数
        void logResult(NaturalLoop* loop, bool success, const std::string& reason) const;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_LOOP_FULL_UNROLL_H__
