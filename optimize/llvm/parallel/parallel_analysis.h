#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm/loop/scev_analysis.h"
#include "llvm/alias_analysis/alias_analysis.h"

namespace Analysis
{
    // optimize/llvm/auto_parallel/parallel_analysis.h
    class ParallelAnalysisPass : public Pass
    {
      private:
        LLVMIR::IR*              ir;
        Analysis::SCEVAnalyser*  scev_analyser;
        Analysis::AliasAnalyser* alias_analyser;

        // 候选循环信息
        struct ParallelLoopInfo
        {
            NaturalLoop*                      loop;
            bool                              is_parallelizable;
            std::string                       reason;  // 若不可并行化，记录原因
            int                               estimated_speedup;
            std::vector<LLVMIR::Instruction*> reduction_insts;
            bool                              has_memory_dependencies;
        };

        std::map<NaturalLoop*, ParallelLoopInfo> loop_info;

      public:
        ParallelAnalysisPass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev, Analysis::AliasAnalyser* aa)
            : Pass(ir), scev_analyser(scev), alias_analyser(aa)
        {}

        void Execute() override;

        // 主要分析函数
        bool analyzeLoopForParallelization(NaturalLoop* loop);
        bool checkDependencies(NaturalLoop* loop);
        bool identifyReductions(NaturalLoop* loop);
        int  estimateParallelSpeedup(NaturalLoop* loop);

        // 获取分析结果
        const ParallelLoopInfo&   getParallelInfo(NaturalLoop* loop) const;
        std::vector<NaturalLoop*> getParallelizableLoops() const;
    };
}  // namespace Transform