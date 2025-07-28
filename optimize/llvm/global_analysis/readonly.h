#pragma once
#include "llvm_ir/instruction.h"
#include "llvm/alias_analysis/alias_analysis.h"
// #define DEBUG
namespace Analysis
{
    class ReadOnlyGlobalAnalysis
    {
      public:
        ReadOnlyGlobalAnalysis(LLVMIR::IR* ir, AliasAnalyser* alias_analyser);

        void run();                                // 执行分析，填充结果
        bool isReadOnly(LLVMIR::Operand* global);  // 查询某个全局变量是否只读
        const std::unordered_set<LLVMIR::Operand*>& getReadOnlyGlobals();
        LLVMIR::Operand*                            traceToGlobal(LLVMIR::Operand* op);

#ifdef DEBUG
        // 打印只读全局变量列表
        void print()
        {
            std::cout << "Read-Only Global Variables:" << std::endl;
            for (auto* global : readonly_globals) { std::cout << "  " << global->getName() << std::endl; }
        }
#endif

      private:
        LLVMIR::IR*                          ir;
        std::unordered_set<LLVMIR::Operand*> readonly_globals;
        std::unordered_set<LLVMIR::Operand*> maybe_written_globals;
        // 用于记录所有可能的别名寄存器
        // mapping from register to its possible alias
        std::unordered_map<LLVMIR::Operand*, LLVMIR::Operand*> maybe_alias_regs;

        // 加入别名分析，考虑函数调用的副作用
        AliasAnalyser* alias_analyser;

        void scanForWrites();
        void buildGlobalAliasPtrSet();
    };

};  // namespace Analysis