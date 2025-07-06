#ifndef __OPTIMIZER_LLVM_BRANCH_CSE_H__
#define __OPTIMIZER_LLVM_BRANCH_CSE_H__

#include "llvm/pass.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"
#include <map>
#include <set>
#include <vector>
#include <string>
#include <functional>

namespace Optimizer
{
    struct CmpInstCSEInfo
    {
        LLVMIR::IROpCode         opcode;
        int                      cond = -1;
        std::vector<std::string> operand_list;

        bool operator<(const CmpInstCSEInfo& x) const
        {
            if (opcode != x.opcode) return opcode < x.opcode;
            if (cond != x.cond) return cond < x.cond;
            if (operand_list.size() != x.operand_list.size()) return operand_list.size() < x.operand_list.size();

            for (int i = 0; i < (int)operand_list.size(); ++i)
            {
                auto op_str   = operand_list[i];
                auto x_op_str = x.operand_list[i];
                if (op_str != x_op_str) return op_str < x_op_str;
            }
            return false;
        }
    };

    class BranchCSEPass : public Pass
    {
      private:
        CmpInstCSEInfo getCSEInfo(LLVMIR::Instruction* inst);
        bool           canReach(int bb1_id, int bb2_id, CFG* cfg);
        bool           canJump(bool is_left, int x1_id, int x2_id, CFG* cfg);
        bool           blockDefNoUseCheck(CFG* cfg, int bb_id, int st_id);
        bool           emptyBlockJumping(CFG* cfg);
        bool           compareInstructionCSE(CFG* cfg);
        void           dfs(int now_id, CFG* cfg, std::map<CmpInstCSEInfo, std::vector<LLVMIR::Instruction*>>& cmp_map);

      public:
        BranchCSEPass(LLVMIR::IR* ir);
        virtual void Execute() override;
    };

}  // namespace Optimizer

#endif
