#include "adce.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include <deque>
#include <set>

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
        auto instlist = bb->insts;
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
            if (live.find(inst) != live.end())
            {
                new_instlist.push_back(inst);
                // 如果跳转的位置label没有用,那么就需要进行替换
                // 更换成后继第一个活跃的上
                // if (inst->GetOpcode() == BasicInstruction::BR_COND) {
                //     auto BRCONDInst = (BrCondInstruction *)inst;
                //     int true_label = ((LabelOperand *)(BRCONDInst->GetTrueLabel()))->GetLabelNo();
                //     int false_label = ((LabelOperand *)(BRCONDInst->GetFalseLabel()))->GetLabelNo();
                //     if (live_bb.find(true_label) == live_bb.end()) {
                //         int newtrue_label = true_label;
                //         while (live_bb.find(newtrue_label) == live_bb.end()) {
                //             newtrue_label = ReDom->GetDomTree(C)->redom_tree[newtrue_label]->block_id;
                //             std::cout << "New label is " << newtrue_label << std::endl;
                //         }
                //         BRCONDInst->ReplcaeLabel(newtrue_label, 1);
                //     }
                //     if (live_bb.find(false_label) == live_bb.end()) {
                //         int newfalse_label = false_label;
                //         while (live_bb.find(newfalse_label) == live_bb.end()) {
                //             newfalse_label = ReDom->GetDomTree(C)->redom_tree[newfalse_label]->block_id;
                //             std::cout << "New label is " << newfalse_label << std::endl;
                //         }
                //         BRCONDInst->ReplcaeLabel(newfalse_label, 0);
                //     }
                // }

                // if (inst->GetOpcode() == BasicInstruction::BR_UNCOND) {
                //     auto BRUNCONDInst = (BrUncondInstruction *)inst;
                //     int destlabel = ((LabelOperand *)(BRUNCONDInst->GetDestLabel()))->GetLabelNo();
                //     if (live_bb.find(destlabel) == live_bb.end()) {
                //         int newdest_label = destlabel;
                //         while (live_bb.find(newdest_label) == live_bb.end()) {
                //             newdest_label = ReDom->GetDomTree(C)->redom_tree[newdest_label]->block_id;
                //             std::cout << "New label is " << newdest_label << std::endl;
                //         }
                //         BRUNCONDInst->ReplcaeLabel(newdest_label);
                //     }
                // }

                // if (inst->GetOpcode() == BasicInstruction::PHI) {
                //     auto PhiInst = (PhiInstruction *)inst;
                //     auto philist = PhiInst->GetPhilist();
                //     int index = 0;
                //     for (auto [label, val] : philist) {
                //         auto labelno = ((LabelOperand *)label)->GetLabelNo();
                //         if (live_bb.find(labelno) == live_bb.end()) {
                //             int newlabel = labelno;
                //             while (live_bb.find(newlabel) == live_bb.end()) {
                //                 newlabel = ReDom->GetDomTree(C)->redom_tree[newlabel]->block_id;
                //             }
                //             PhiInst->ReplcaeLabel(newlabel, index);
                //         }
                //         index++;
                //     }
                // }
            }
            else
            {
                if (inst == instlist.back())
                {
                    int newlabel = ReDom->imm_dom[id];
                    // std::cout << "New id of " << id << " is " << newlabel << std::endl;

                    // while (live_bb.find(newlabel) == live_bb.end()) {
                    //     newlabel = ReDom->GetDomTree(C)->idom[newlabel]->block_id;
                    //     std::cout << "New id of " << id << " is " << newlabel << std::endl;
                    // }
                    auto I = new BranchUncondInst(getLabelOperand(newlabel));
                    new_instlist.push_back(I);
                }
            }
        }
        instlist = new_instlist;
    }
}