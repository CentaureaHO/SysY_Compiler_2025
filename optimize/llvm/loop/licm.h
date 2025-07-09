#ifndef __OPTIMIZER_LLVM_LOOP_LICM_H__
#define __OPTIMIZER_LLVM_LOOP_LICM_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include "cfg.h"
#include <set>
#include <map>
#include <vector>
#include <functional>

namespace StructuralTransform
{
    class LICMPass : public Pass
    {
      public:
        explicit LICMPass(LLVMIR::IR* ir, Analysis::AliasAnalyser* aa);

        void Execute() override;

      private:
        Analysis::AliasAnalyser*            alias_analyser;
        std::map<int, bool>                 invariant_map;  ///< Register number -> is_invariant
        std::map<int, LLVMIR::Instruction*> result_map;     ///< Register number -> defining_instruction

        bool optimization_changed;

        bool dominatesAllExits(CFG* cfg, LLVMIR::IRBlock* bb, NaturalLoop* loop);
        bool isLoopInvariant(CFG* cfg, LLVMIR::Instruction* inst, NaturalLoop* loop,
            const std::vector<LLVMIR::Instruction*>& write_insts);

        std::vector<LLVMIR::Instruction*> getLoopMemWriteInsts(CFG* cfg, NaturalLoop* loop);
        std::vector<LLVMIR::Instruction*> findInvariantInsts(CFG* cfg, NaturalLoop* loop);

        void hoistInstructionsToPreheader(NaturalLoop* loop, const std::vector<LLVMIR::Instruction*>& insts_to_hoist);

        void hoistLoopInvariants(CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop);
        void promoteMemToRegs(CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop);
        void hoistInvariantLoads(CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop);
        void hoistInvariantCalls(CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop);

        using OptimizationFunction = std::function<void(CFG*, NaturalLoopForest&, NaturalLoop*)>;
        void processLoopRecursively(
            CFG* cfg, NaturalLoopForest& loop_forest, NaturalLoop* loop, OptimizationFunction opt_func);

        void performLICM(CFG* cfg);
        void buildResultMap(CFG* cfg);
    };

}  // namespace StructuralTransform

#endif  // __OPTIMIZER_LLVM_LOOP_LICM_H__
