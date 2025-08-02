#ifndef __OPTIMIZER_LLVM_LOOP_UNROLL_H__
#define __OPTIMIZER_LLVM_LOOP_UNROLL_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "cfg.h"
#include <set>
#include <map>
#include <tuple>
#include "llvm/loop/scev_analysis.h"
#include "llvm/make_domtree.h"

namespace StructuralTransform
{
    class LoopUnroll{
    public:
        Analysis::SCEVAnalyser* scev;//循环的SCEV分析器
        NaturalLoop* loop; //要优化的循环
        LLVMIR::IR* ir;//获取支配树用
        
        

        bool ConstantLoopUnroll();

        LoopUnroll(Analysis::SCEVAnalyser* scev,NaturalLoop* loop,LLVMIR::IR* ir){
            this->scev=scev;
            this->loop=loop;
            this->ir=ir;
        }

    };

    class LoopUnrollPass : public Pass
    {
      public:
        Analysis::SCEVAnalyser* scev;
        MakeDomTreePass* makedom;

        LoopUnrollPass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev,MakeDomTreePass* makedom ) : Pass(ir) { 
            this->scev = scev;
            this->makedom=makedom;
        }
        void Execute() override;
    };   

}  // namespace StructuralTransform

#endif  // __OPTIMIZER_LLVM_LOOP_UNROLL_H__