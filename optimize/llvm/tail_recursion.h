#ifndef __OPTIMIZER_LLVM_TAIL_RECURSION_H__
#define __OPTIMIZER_LLVM_TAIL_RECURSION_H__

#include "llvm/pass.h"
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace Transform
{
    // 尾递归调用信息结构
    struct TailCallInfo
    {
        LLVMIR::CallInst*                             call_instruction;
        LLVMIR::IRBlock*                              containing_block;
        bool                                          is_valid_tail_call;
        std::vector<std::pair<int, LLVMIR::Operand*>> parameter_mappings;
    };

    // 函数参数分析器
    class ParameterAnalyzer
    {
      public:
        struct ParameterInfo
        {
            bool                is_pointer_type;
            bool                requires_spilling;
            int                 original_index;
            LLVMIR::RegOperand* spill_location;
        };

        ParameterAnalyzer(CFG* cfg);
        bool                       isTransformationValid();
        std::vector<ParameterInfo> analyzeParameters();

      private:
        CFG* target_cfg;
        bool checkArrayParameterUsage();
        bool validateParameterConstraints();
    };

    // 尾递归优化器核心类
    class TailRecursionOptimizer
    {
      public:
        TailRecursionOptimizer(CFG* cfg);
        bool canOptimize();
        void performOptimization();

      private:
        CFG*                                     target_cfg;
        std::unique_ptr<ParameterAnalyzer>       param_analyzer;
        std::vector<TailCallInfo>                discovered_tail_calls;
        std::unordered_set<LLVMIR::Instruction*> instructions_to_remove;
        std::unordered_map<int, int>             register_remapping;

        void discoverTailCalls();
        void setupParameterSpilling();
        void rewriteTailCalls();
        void finalizeTransformation();
    };

    class TailRecursionPass : public Pass
    {
      private:
        void optimizeFunction(CFG* cfg);

      public:
        TailRecursionPass(LLVMIR::IR* ir);
        virtual void Execute() override;
    };

}  // namespace Transform

#endif
