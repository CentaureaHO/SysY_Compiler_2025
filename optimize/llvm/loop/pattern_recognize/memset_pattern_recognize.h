#ifndef __OPTIMIZER_LLVM_LOOP_PATTERN_RECOGNIZE_MEMSET_H__
#define __OPTIMIZER_LLVM_LOOP_PATTERN_RECOGNIZE_MEMSET_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include "cfg.h"
#include <set>
#include <map>
#include <memory>

namespace Transform
{
    class MemsetPatternRecognizePass : public Pass
    {
      private:
        std::map<int, LLVMIR::Instruction*> result_map;

        void             buildResultMap(CFG* cfg);
        bool             recognizeMemsetPattern(CFG* cfg, NaturalLoop* loop);
        bool             isLoopSideEffectFree(CFG* cfg, NaturalLoop* loop, LLVMIR::StoreInst* store_inst);
        bool             isValidMemsetValue(LLVMIR::Operand* value);
        bool             canUseStandardMemset(LLVMIR::Operand* value);
        LLVMIR::Operand* getLoopIterations(CFG* cfg, NaturalLoop* loop);
        bool             checkLoopIncrementPattern(CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* counter_phi,
                        LLVMIR::PhiInst* pointer_phi, LLVMIR::StoreInst* store_inst);
        void             transformLoopToMemset(CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* counter_phi,
                        LLVMIR::PhiInst* pointer_phi, LLVMIR::StoreInst* store_inst, LLVMIR::Operand* upper_bound,
                        LLVMIR::IRBlock* exit);
        bool             recognizeUnrolledMemsetPattern(CFG* cfg);
        bool             processUnrolledMemsetInBlock(LLVMIR::IRBlock* block);
        bool             instructionUsesRegister(LLVMIR::Instruction* inst, int reg_num);

      public:
        explicit MemsetPatternRecognizePass(LLVMIR::IR* ir);

        void Execute() override;
        void processFunction(CFG* cfg);

        std::pair<int, int> getRecognitionStats() const;

      private:
        mutable int loops_processed = 0;
        mutable int loops_converted = 0;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_LOOP_PATTERN_RECOGNIZE_MEMSET_H__