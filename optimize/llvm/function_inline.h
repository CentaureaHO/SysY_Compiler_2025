#ifndef __OPTIMIZER_LLVM_FUNCTION_INLINE_H__
#define __OPTIMIZER_LLVM_FUNCTION_INLINE_H__

#include "llvm/pass.h"
#include "llvm/loop/loop_def.h"
#include <map>
#include <vector>
#include <string>
#include <set>

namespace Transform
{
    class FunctionInlineAnalyzer
    {
      private:
        struct FunctionInfo
        {
            LLVMIR::IRFunction*            func;
            int                            instruction_count;
            int                            block_count;
            int                            call_count;    // number of calls this function makes
            int                            called_count;  // number of times this function is called
            bool                           is_recursive;
            bool                           has_loops;
            bool                           has_pointer_params;
            int                            complexity_score;
            std::vector<LLVMIR::CallInst*> calls_made;
            std::vector<LLVMIR::CallInst*> call_sites;

            FunctionInfo()
                : func(nullptr),
                  instruction_count(0),
                  block_count(0),
                  call_count(0),
                  called_count(0),
                  is_recursive(false),
                  has_loops(false),
                  has_pointer_params(false),
                  complexity_score(0)
            {}
        };

        struct CallSiteInfo
        {
            LLVMIR::CallInst*   call_inst;
            LLVMIR::IRFunction* caller;
            LLVMIR::IRFunction* callee;
            bool                in_loop;
            int                 nesting_level;
            int                 estimated_frequency;
            bool                has_pointer_args;

            CallSiteInfo()
                : call_inst(nullptr),
                  caller(nullptr),
                  callee(nullptr),
                  in_loop(false),
                  nesting_level(0),
                  estimated_frequency(1),
                  has_pointer_args(false)
            {}
        };

        LLVMIR::IR*                                                  ir;
        std::map<LLVMIR::IRFunction*, FunctionInfo>                  function_info;
        std::vector<CallSiteInfo>                                    all_call_sites;
        std::map<LLVMIR::IRFunction*, std::set<LLVMIR::IRFunction*>> call_graph;
        std::map<LLVMIR::IRFunction*, std::set<LLVMIR::IRFunction*>> reverse_call_graph;
        std::vector<LLVMIR::IRFunction*>                             topo_order;

        void buildCallGraph();
        void analyzeFunction(LLVMIR::IRFunction* func);
        void detectRecursion();
        void computeTopologicalOrder();
        void analyzeCallSites();
        void computeComplexityScores();

        bool                isInLoop(LLVMIR::CallInst* call_inst, LLVMIR::IRFunction* func);
        int                 estimateCallFrequency(LLVMIR::CallInst* call_inst);
        int                 calculateNestingLevel(LLVMIR::CallInst* call_inst);
        bool                hasPointerParameters(LLVMIR::IRFunction* func);
        bool                hasPointerArguments(LLVMIR::CallInst* call_inst);
        int                 computeFunctionComplexity(LLVMIR::IRFunction* func);
        bool                wouldCauseTooMuchGrowth(LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee);
        LLVMIR::IRFunction* findFunction(const std::string& name);

        NaturalLoop* getLoopForBlock(LLVMIR::IRFunction* func, LLVMIR::IRBlock* block);
        int          getLoopDepth(LLVMIR::IRFunction* func, LLVMIR::IRBlock* block);
        int          getControlFlowNesting(LLVMIR::IRFunction* func, LLVMIR::CallInst* call_inst);
        int          getDominatorDepth(LLVMIR::IRFunction* func, LLVMIR::IRBlock* block);

      public:
        FunctionInlineAnalyzer(LLVMIR::IR* ir);

        bool shouldInline(LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee, LLVMIR::CallInst* call_inst);
        void analyze();

        std::vector<LLVMIR::IRFunction*> getProcessingOrder() const { return topo_order; }
        const FunctionInfo&              getFunctionInfo(LLVMIR::IRFunction* func) const;
        bool                             isRecursive(LLVMIR::IRFunction* func);
        int                              getCallCount(LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee);

        std::string getInlineReason(
            LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee, LLVMIR::CallInst* call_inst);

        void printAnalysisReport() const;
    };

    class FunctionInlinePass : public Pass
    {
      private:
        FunctionInlineAnalyzer* analyzer;

        LLVMIR::IRFunction* findFunction(const std::string& name);
        void inlineFunction(LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee, LLVMIR::CallInst* call_inst);

        void updateRegAndLabelMaps(LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee, LLVMIR::CallInst* call_inst,
            std::map<int, int>& reg_map, std::map<int, int>& label_map);

        LLVMIR::Instruction* copyInstruction(LLVMIR::Instruction* inst, const std::map<int, int>& reg_map,
            const std::map<int, int>& label_map, LLVMIR::IRFunction* caller);

        LLVMIR::IRBlock* findCallBlock(LLVMIR::IRFunction* func, LLVMIR::CallInst* call_inst);

        void splitBlockAtCall(LLVMIR::IRFunction* caller, LLVMIR::IRBlock* call_block, LLVMIR::CallInst* call_inst,
            LLVMIR::IRBlock*& continue_block);

        void updatePhiInstructions(LLVMIR::IRFunction* caller, LLVMIR::IRBlock* old_block, LLVMIR::IRBlock* new_block);

      public:
        FunctionInlinePass(LLVMIR::IR* ir);
        ~FunctionInlinePass();

        virtual void Execute() override;
    };
}  // namespace Transform

#endif
