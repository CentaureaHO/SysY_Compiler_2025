#ifndef __OPTIMIZER_LLVM_LOOP_STRENGTH_REDUCE_H__
#define __OPTIMIZER_LLVM_LOOP_STRENGTH_REDUCE_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "cfg.h"
#include <set>
#include <map>
#include <tuple>
#include "llvm/loop/scev_analysis.h"

namespace Transform
{
    class StrengthReduce
    {
      public:
        Analysis::SCEVAnalyser* scev;  // 循环的SCEV分析器
        NaturalLoop*            loop;  // 要优化的循环
        LLVMIR::IR*             ir;    // 获取支配树用

        std::map<int, int>                replace;
        std::map<int, int>                todel;
        std::vector<LLVMIR::Instruction*> new_add_insts;

        std::pair<LLVMIR::GEPInst*, LLVMIR::Operand*> checkGEP(LLVMIR::GEPInst* ins);
        void                                          doMulStrengthReduce();
        void                                          doGEPStrengthReduce();
        bool                                          checkDom(int dom, int node);

        StrengthReduce(Analysis::SCEVAnalyser* scev, NaturalLoop* loop, LLVMIR::IR* ir)
        {
            this->scev = scev;
            this->loop = loop;
            this->ir   = ir;
            replace.clear();
            todel.clear();
            new_add_insts.clear();
        }
    };

    class StrengthReducePass : public Pass
    {
      public:
        // 循环信息在ir的cfg里
        Analysis::SCEVAnalyser* scev;

        StrengthReducePass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev) : Pass(ir) { this->scev = scev; }
        void Execute();
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_LOOP_STRENGTH_REDUCE_H__
