#ifndef __OPTIMIZER_LLVM_GLOBAL_CONST_REPLACE_H__
#define __OPTIMIZER_LLVM_GLOBAL_CONST_REPLACE_H__

#include "llvm/pass.h"
#include <llvm_ir/ir_builder.h>
#include <map>
#include <string>

using namespace LLVMIR;

namespace Transform
{
    class GlobalConstReplacePass : public Pass
    {
      private:
        std::map<std::string, VarAttribute> global_const_map;

        void analyzeGlobalDefinitions();
        void analyzeGlobalModifications();
        void replaceGlobalConstLoadsInCFG(CFG* cfg);

        Instruction* createConstAssignment(LoadInst* load_inst, const VarAttribute& var_attr);

      public:
        GlobalConstReplacePass(IR* ir) : Pass(ir) {}

        void Execute() override;
    };

}  // namespace Transform

#endif  // __OPTIMIZER_LLVM_GLOBAL_CONST_REPLACE_H__
