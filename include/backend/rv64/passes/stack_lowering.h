#ifndef __BACKEND_RV64_PASSES_STACK_LOWERING_H__
#define __BACKEND_RV64_PASSES_STACK_LOWERING_H__

#include "../../base_pass.h"
#include "../rv64_function.h"
#include "../rv64_defs.h"
#include "../rv64_loop.h"
#include <dom_analyzer.h>
#include <vector>
#include <map>
#include <queue>

namespace Backend::RV64::Passes
{

    class StackLoweringPass : public BasePass
    {
      public:
        explicit StackLoweringPass(std::vector<Function*>& functions);
        ~StackLoweringPass() override = default;

        bool        run() override;
        const char* getName() const override { return "StackLowering"; }

      private:
        std::vector<Function*>& functions_;

        void lowerStack(Function* func);
        void lowerStackWithLCA(Function* func);
        void lowerStackSimple(Function* func);
        void handleStackAllocation(Function* func);
        void gatherRegsToSave(Function* func, MAT2(int) & reg_def_blocks, MAT2(int) & reg_access_blocks);
        int  calculateGroupLCA(const std::vector<int>& blocks, Cele::Algo::DomAnalyzer* dom_tree, Function* func);
        int  calculatePairLCA(
             int x, int y, Cele::Algo::DomAnalyzer* dom_tree, std::map<int, int>& depth, Function* func);
    };

}  // namespace Backend::RV64::Passes

#endif  // __BACKEND_RV64_PASSES_STACK_LOWERING_H__
