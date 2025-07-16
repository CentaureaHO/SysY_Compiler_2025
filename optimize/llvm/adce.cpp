#include "adce.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include <deque>
#include <set>

using namespace LLVMIR;

void ADCEPass::Execute()
{
    for (auto [defI, C] : ir->cfg) { ADceInSingleCFG(C); }
}

void ADCEPass::ADceInSingleCFG(CFG* C)
{

    std::map<Instruction*, int> WorkList{};
    // 首先扫描得到所有的store等
    for (auto [id, bb] : C->block_id_to_block)
    {
        auto& instlist = bb->insts;
        for (auto inst : instlist)
        {
            if (inst->opcode == IROpCode::STORE)
            {
                WorkList[inst] = id;
                continue;
            }
            if (inst->opcode == IROpCode::CALL)
            {
                WorkList[inst] = id;
                continue;
            }
            if (inst->opcode == IROpCode::RET)
            {
                WorkList[inst] = id;
                continue;
            }
        }
    }
    // std::cout << "Init Worklist size is " << WorkList.size() << std::endl;
    // std::cout << "Function is " << C->func->func_def->func_name << std::endl;

    // 得到了起始的WorkList

    auto defs = DefUse->GetDefMap(C);

    auto defs2id = DefUse->GetDef2idmap(C);

    // 记录所有活跃块
    std::set<Instruction*> live{};

    // 记录所有活跃的块
    std::set<int> live_bb{};

    // 记录所有活跃指令的use
    std::set<int> live_use{};

    auto ReDom = ir->ReDomTrees[C];

    // 然后根据算法处理
    while (!WorkList.empty())
    {
        auto [I, id] = *WorkList.begin();
        WorkList.erase(I);

        live.insert(I);
        live_bb.insert(id);
        auto S = I->GetUsedRegs();
        for (auto s : S) { live_use.insert(s); }
        if (I->opcode == IROpCode::PHI)
        {
            auto Phi = (PhiInst*)I;
            auto L   = Phi->vals_for_labels;
            for (auto l : L)
            {
                auto label   = l.second;
                auto labelno = ((LabelOperand*)label)->label_num;
                live_bb.insert(labelno);
                auto preinst = (C->block_id_to_block)[labelno]->insts.back();
                if (live.find(preinst) == live.end()) { WorkList[preinst] = labelno; }
            }
        }

        // 对于这个块的所有前驱控制依赖
        for (auto bb : CDG->GetCDGPre(C, id))
        {
            auto preinst = bb->insts.back();
            // std::cout << "Block is " << bb->block_id << std::endl;

            if (live.find(preinst) == live.end()) { WorkList[preinst] = bb->block_id; }
        }

        // 对每个这个指令用到的所有uses
        auto uses = I->GetUsedRegs();
        for (auto use : uses)
        {
            // std::cout << "Use is " << use << std::endl;
            if (defs.find(use) != defs.end())
            {
                auto def = defs[use];
                if (live.find(def) == live.end()) { WorkList[def] = defs2id[def]; }
            }
        }
    }

    for (auto [id, bb] : (C->block_id_to_block))
    {
        auto&                    instlist = bb->insts;
        std::deque<Instruction*> new_instlist;
        for (auto inst : instlist)
        {
            if (live.find(inst) != live.end()) { new_instlist.push_back(inst); }
            else
            {
                if (inst == instlist.back())
                {
                    int  newlabel = ReDom->imm_dom[id];
                    auto I        = new BranchUncondInst(getLabelOperand(newlabel));
                    new_instlist.push_back(I);
                }
            }
        }
        instlist = new_instlist;
    }
}