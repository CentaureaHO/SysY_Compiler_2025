#include "def_use.h"
#include <vector>

using namespace LLVMIR;

/*
伪代码:
for BasicBlock bb in C
    for Instruction I in bb
        if I 有结果寄存器
            int regno = I->GetResultRegNo()
            IRDefMaps[C][regno] = I
*/
void DefUseAnalysisPass::GetDefUseInSingleCFG(CFG* C)
{
    // 首先得到参数的寄存器编号，不能消除这些
    // 我们可以知道所有的函数参数，一定都是寄存器
    // std::set<int> param_set{};

    // // 根据实现，其实这里的size就是最大寄存器号加一
    // int max_reg = C->function_def->formals_reg.size();
    // for (int i = 0; i < max_reg; i++) {
    //     param_set.insert(i);
    // }

    // 这里记录了也没用其实

    for (auto [id, IRB] : C->block_id_to_block)
    {
        auto& instlist = IRB->insts;
        for (auto inst : instlist)
        {
            // 这里其实和mem2reg不一样，我们只要有结果，就可以消除，而不需要考虑是不是数组
            // 那么接下来就是重复性工作了
            int regno = inst->GetResultReg();
            if (regno >= 0)
            {
                // 表示不是参数中的
                // if (param_set.find(regno) == param_set.end()) {
                IRDef2bbid[C][inst] = id;
                IRDefMaps[C][regno] = inst;
                // }
            }

            // 这里针对参数进行考虑了
            std::vector<int> useset = inst->GetUsedRegs();
            for (auto reg : useset) { IRUseMaps[C][reg].insert(inst); }
        }
    }
}

void DefUseAnalysisPass::Execute()
{
    for (auto iter : ir->cfg)
    {
        CFG* C = iter.second;
        GetDefUseInSingleCFG(C);
    }
}