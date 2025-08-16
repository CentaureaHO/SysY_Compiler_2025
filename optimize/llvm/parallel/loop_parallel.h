#pragma once
#include "llvm_ir/ir_builder.h"
#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm/parallel/parallel_analysis.h"

namespace Transform
{  // optimize/llvm/auto_parallel/loop_parallelizer.h
    class LoopParallelizer : public Pass
    {
      private:
        LLVMIR::IR*    ir;
        Analysis::ParallelAnalysisPass* parallel_analysis;

        enum ParallelStrategy
        {
            OPENMP,    // 使用OpenMP指令
            PTHREADS,  // 使用POSIX线程
            SIMD       // 使用向量化SIMD指令
        };

        ParallelStrategy strategy;

        // 将循环转换为OpenMP形式
        bool convertToOpenMP(NaturalLoop* loop);

        // 将循环转换为POSIX线程形式
        bool convertToPThreads(NaturalLoop* loop);

        // 将循环转换为SIMD指令
        bool convertToSIMD(NaturalLoop* loop);

        // 生成辅助函数(用于pthread实现)
        LLVMIR::IRFunction* generateWorkerFunction(NaturalLoop* loop);

      public:
        LoopParallelizer(LLVMIR::IR* ir, Analysis::ParallelAnalysisPass* analysis, ParallelStrategy strategy = OPENMP)
            : Pass(ir), parallel_analysis(analysis), strategy(strategy)
        {}

        void                Execute() override;
        std::pair<int, int> getParallelStats() const;  // 返回处理/成功的循环数
    };
}  // namespace Transform