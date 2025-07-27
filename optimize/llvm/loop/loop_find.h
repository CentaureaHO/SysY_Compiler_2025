#ifndef __OPTIMIZER_LLVM_LOOP_FIND_H__
#define __OPTIMIZER_LLVM_LOOP_FIND_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "cfg.h"
#include <set>
#include <functional>
#include <memory>

namespace Analysis
{
    class LoopAnalysisPass : public Pass
    {
      private:
        bool isDominated(CFG* cfg, int dominator_id, int node_id) const;
        void buildLoopHierarchy(NaturalLoopForest* forest);

        void identifyLoopPreheaders(CFG* cfg);
        void identifyLoopPreheader(CFG* cfg, NaturalLoop* loop);
        bool isValidPreheader(CFG* cfg, LLVMIR::IRBlock* candidate, NaturalLoop* loop) const;

      public:
        explicit LoopAnalysisPass(LLVMIR::IR* ir);

        void Execute() override;

        void buildLoopInfo(CFG* cfg);
        bool hasLoopInfo(CFG* cfg) const;

        NaturalLoop*              getLoopForBlock(CFG* cfg, LLVMIR::IRBlock* block) const;
        std::vector<NaturalLoop*> getLoopsInDFSOrder(CFG* cfg) const;
    };

}  // namespace Analysis

#endif  // __OPTIMIZER_LLVM_LOOP_FIND_H__
