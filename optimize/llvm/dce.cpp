#include "dce.h"
#include "llvm_ir/defs.h"
#include <deque>
#include <set>

using namespace LLVMIR;

void DCEPass::Execute()
{
    for (auto [defI, C] : ir->cfg) { DceInSingleCFG(C); }
}

void DCEPass::DceInSingleCFG(CFG* C)
{

    // 记录需要删除的指令,最后一起删除
    std::set<Instruction*> erase_set{};

    auto& defs = DefUse->GetDefMap(C);
    auto& uses = DefUse->GetUseMap(C);

    // 接下来就是记录下所有的参数，然后对不是函数参数的来处理
    std::set<int> params_set{};

    int max_reg = C->func->func_def->arg_regs.size();
    for (int i = 0; i < max_reg; i++) { params_set.insert(i); }

    std::set<int> WorkList;
    for (auto [key, val] : defs) { WorkList.insert(key); }

    while (!WorkList.empty())
    {
        int val = *(WorkList.begin());
        WorkList.erase(val);
        // std::cout << val << " ";
        // std::cout << uses[val].size() << std::endl;

        if (params_set.find(val) != params_set.end()) { continue; }
        // 使用列表为空
        if (uses[val].empty())
        {
            auto I = defs[val];

            if (I->opcode == IROpCode::CALL) { continue; }

            // 我们不能删除所有参与函数调用的处理,所以我们这里要作出判断

            erase_set.insert(I);
            auto params_set = I->GetUsedRegs();
            for (auto param : params_set)
            {
                if (uses.find(param) != uses.end())
                {
                    if (I->opcode != IROpCode::CALL) { uses[param].erase(I); }
                }
                WorkList.insert(param);
            }
        }
    }

    // 进行指令的删除
    for (auto [id, bb] : C->block_id_to_block)
    {
        auto&                    instlist = bb->insts;
        std::deque<Instruction*> new_instlist;
        for (auto inst : instlist)
        {
            if (erase_set.find(inst) != erase_set.end()) { continue; }
            new_instlist.push_back(inst);
            // if(inst->opcode == IROpCode::PHI) {
            //     std::cout<<"phi found in block " << id << std::endl;
            // }
        }
        bb->insts = new_instlist;
    }
}