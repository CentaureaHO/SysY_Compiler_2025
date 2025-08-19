#include "eliminate_double_br_uncond.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <stack>
#include <unordered_set>

// #define DBGMODE

#ifdef DBGMODE
template <typename... Args>
void dbg_impl(Args&&... args)
{
    ((std::cout << args), ...);
    std::cout << std::endl;
}
#define DBGINFO(...) dbg_impl(__VA_ARGS__)
#else
#define DBGINFO(...) \
    do {             \
    } while (0)
#endif

namespace Transform
{
    EliminateDoubleBrUncondPass::EliminateDoubleBrUncondPass(LLVMIR::IR* ir) : Pass(ir), modified_(false) {}

    void EliminateDoubleBrUncondPass::Execute()
    {
        modified_ = false;

        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (cfg && !cfg->block_id_to_block.empty()) { processFunction(cfg); }
        }
    }

    void EliminateDoubleBrUncondPass::processFunction(CFG* cfg)
    {
        DBGINFO("=== EliminateDoubleBrUncond: Processing function ===");
        if (cfg && cfg->func && cfg->func->func_def) { DBGINFO("Function name: ", cfg->func->func_def->func_name); }

        bool                                       changed = true;
        std::vector<std::vector<LLVMIR::IRBlock*>> G;
        std::vector<std::vector<LLVMIR::IRBlock*>> invG;
        std::unordered_map<int, int>               visited;
        std::unordered_map<int, int>               PhiMap;
        std::unordered_map<int, int>               OtherPhiMap;
        std::stack<LLVMIR::IRBlock*>               bbstack;

        while (changed)
        {
            changed = false;

            buildGraph(cfg, G, invG);

            visited.clear();
            PhiMap.clear();
            OtherPhiMap.clear();

            for (const auto& [id, bb] : cfg->block_id_to_block) visited[id] = 0;

            if (!cfg->block_id_to_block.empty()) bbstack.push(cfg->block_id_to_block.begin()->second);
            while (!bbstack.empty())
            {
                auto* bbu = bbstack.top();
                int   uid = bbu->block_id;
                bbstack.pop();
                visited[uid] = 1;

                if (uid >= G.size()) continue;

                for (auto* bbv : G[uid])
                {
                    int vid = bbv->block_id;
                    if (visited[vid] == 1) continue;

                    if (uid == cfg->block_id_to_block.begin()->first) { bbstack.push(bbv); }

                    bool check1 = (G[uid].size() >= 1 && vid < G.size() && G[vid].size() >= 1 && !G[uid].empty() &&
                                   G[uid][0] == bbv && !G[vid].empty() && G[vid][0] == bbu);
                    bool check2 = (G[uid].size() >= 1 && vid < G.size() && G[vid].size() > 1 && !G[uid].empty() &&
                                   G[uid][0] == bbv && G[vid].size() > 1 && G[vid][1] == bbu);
                    bool check3 = (G[uid].size() > 1 && vid < G.size() && G[vid].size() >= 1 && G[uid].size() > 1 &&
                                   G[uid][1] == bbv && !G[vid].empty() && G[vid][0] == bbu);
                    bool check4 = (G[uid].size() > 1 && vid < G.size() && G[vid].size() > 1);

                    bool check = ((!check4) && (check1 || check2 || check3));

                    if (check)
                    {
                        int x, y;
                        if (check1)
                        {
                            x = 0;
                            y = 0;
                        }
                        else if (check2)
                        {
                            x = 0;
                            y = 1;
                        }
                        else
                        {
                            x = 1;
                            y = 0;
                        }

                        if (G[vid].size() > 1)
                        {
                            changed   = true;
                            modified_ = true;

                            DBGINFO("Merging block ", vid, " into block ", uid);
                            DBGINFO("Block ", vid, " has ", bbv->insts.size(), " instructions");
                            DBGINFO("Block ", uid, " has ", bbu->insts.size(), " instructions before merge");

                            if (uid < G.size() && x < G[uid].size()) G[uid][x] = bbu;

                            auto* inv   = G[vid][y ^ 1];
                            int   invid = inv->block_id;

                            if (invid < invG.size())
                            {
                                for (size_t i = 0; i < invG[invid].size(); ++i)
                                {
                                    if (invG[invid][i] == bbv)
                                    {
                                        invG[invid][i] = bbu;
                                        break;
                                    }
                                }
                            }

                            PhiMap[vid] = uid;

                            if (!bbu->insts.empty()) bbu->insts.pop_back();
                            mergeBlocks(bbu, bbv);

                            DBGINFO("Block ", uid, " has ", bbu->insts.size(), " instructions after merge");

                            bbv->insts.clear();
                            cfg->block_id_to_block.erase(vid);
                            removeBlockFromFunction(cfg, bbv);
                            DBGINFO("Removed block ", vid);
                        }
                        else if (bbv->insts.size() == 1 && !bbv->insts.empty() &&
                                 bbv->insts.back()->opcode == LLVMIR::IROpCode::BR_UNCOND)
                        {
                            if (G[uid].size() == 1) continue;

                            changed   = true;
                            modified_ = true;

                            auto* endI = static_cast<LLVMIR::BranchCondInst*>(bbu->insts.back());
                            bbu->insts.pop_back();

                            auto* trueLabel  = static_cast<LLVMIR::LabelOperand*>(endI->true_label);
                            auto* falseLabel = static_cast<LLVMIR::LabelOperand*>(endI->false_label);

                            if (trueLabel->label_num == vid)
                                endI->true_label = getLabelOperand(uid);
                            else
                                endI->false_label = getLabelOperand(uid);

                            bbu->insts.push_back(endI);
                            PhiMap[vid] = uid;
                            cfg->block_id_to_block.erase(vid);
                            removeBlockFromFunction(cfg, bbv);
                        }
                        else
                            bbstack.push(bbv);
                    }
                    else if (G[uid].size() == 1 && vid < invG.size() && invG[vid].size() == 1)
                    {
                        changed     = true;
                        modified_   = true;
                        PhiMap[vid] = uid;

                        DBGINFO("Merging single successor block ", vid, " into block ", uid);
                        DBGINFO("Block ", vid, " has ", bbv->insts.size(), " instructions");
                        DBGINFO("Block ", uid, " has ", bbu->insts.size(), " instructions before merge");

                        G[uid].clear();
                        if (vid < G.size())
                        {
                            for (auto* inv : G[vid])
                            {
                                int invid = inv->block_id;
                                G[uid].push_back(inv);
                                if (invid < invG.size())
                                {
                                    for (size_t i = 0; i < invG[invid].size(); ++i)
                                    {
                                        if (invG[invid][i] == bbv)
                                        {
                                            invG[invid][i] = bbu;
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        if (!bbu->insts.empty()) bbu->insts.pop_back();

                        mergeBlocks(bbu, bbv);

                        bbv->insts.clear();
                        cfg->block_id_to_block.erase(vid);
                        removeBlockFromFunction(cfg, bbv);
                        bbstack.push(bbu);
                    }
                    else if (G[uid].size() == 2 && vid < invG.size() && invG[vid].size() == 1 && vid < G.size() &&
                             G[vid].size() == 1 && bbv->insts.size() == 1 && !bbv->insts.empty() &&
                             bbv->insts.back()->opcode == LLVMIR::IROpCode::BR_UNCOND)
                    {
                        if (vid >= G.size() || G[vid].empty()) continue;

                        auto* inv   = G[vid][0];
                        int   invid = inv->block_id;
                        auto* I     = bbu->insts.back();

                        if (I->opcode == LLVMIR::IROpCode::BR_COND)
                        {
                            auto* brCond     = static_cast<LLVMIR::BranchCondInst*>(I);
                            auto* trueLabel  = static_cast<LLVMIR::LabelOperand*>(brCond->true_label);
                            auto* falseLabel = static_cast<LLVMIR::LabelOperand*>(brCond->false_label);

                            if (trueLabel->label_num == invid || falseLabel->label_num == invid)
                            {
                                bbstack.push(bbv);
                                continue;
                            }
                        }

                        changed          = true;
                        modified_        = true;
                        OtherPhiMap[vid] = uid;
                        PhiMap[vid]      = invid;

                        if (uid < G.size())
                        {
                            for (size_t i = 0; i < G[uid].size(); ++i)
                            {
                                if (G[uid][i] == bbv)
                                {
                                    G[uid][i] = inv;
                                    break;
                                }
                            }
                        }

                        if (invid < invG.size())
                        {
                            for (size_t i = 0; i < invG[invid].size(); ++i)
                            {
                                if (invG[invid][i] == bbv)
                                {
                                    invG[invid][i] = bbu;
                                    break;
                                }
                            }
                        }

                        auto* endI = static_cast<LLVMIR::BranchCondInst*>(bbu->insts.back());
                        bbu->insts.pop_back();

                        auto* trueLabel  = static_cast<LLVMIR::LabelOperand*>(endI->true_label);
                        auto* falseLabel = static_cast<LLVMIR::LabelOperand*>(endI->false_label);

                        if (trueLabel->label_num == vid)
                            endI->true_label = getLabelOperand(invid);
                        else
                            endI->false_label = getLabelOperand(invid);

                        bbu->insts.push_back(endI);
                        cfg->block_id_to_block.erase(vid);
                        removeBlockFromFunction(cfg, bbv);
                        bbstack.push(bbu);
                    }
                    else
                        bbstack.push(bbv);
                }
            }

            if (changed)
            {
                updatePhiNodes(cfg, PhiMap, OtherPhiMap);
                updateBranchInstructions(cfg, PhiMap);
            }
        }

        cleanupSinglePredecessorPhis(cfg);

        renumberBlocks(cfg);

        cfg->BuildCFG();
    }

    void EliminateDoubleBrUncondPass::buildGraph(
        CFG* cfg, std::vector<std::vector<LLVMIR::IRBlock*>>& G, std::vector<std::vector<LLVMIR::IRBlock*>>& invG)
    {
        int max_id = 0;
        for (const auto& [id, bb] : cfg->block_id_to_block) max_id = std::max(max_id, id);

        G.clear();
        invG.clear();
        G.resize(max_id + 1);
        invG.resize(max_id + 1);

        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            if (bb->insts.empty()) continue;

            auto* terminator = bb->insts.back();
            if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
            {
                auto* br        = static_cast<LLVMIR::BranchUncondInst*>(terminator);
                auto* label_op  = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                int   target_id = label_op->label_num;

                auto it = cfg->block_id_to_block.find(target_id);
                if (it != cfg->block_id_to_block.end())
                {
                    G[id].push_back(it->second);
                    invG[target_id].push_back(bb);
                }
            }
            else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* br          = static_cast<LLVMIR::BranchCondInst*>(terminator);
                auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);

                int true_id  = true_label->label_num;
                int false_id = false_label->label_num;

                auto true_it = cfg->block_id_to_block.find(true_id);
                if (true_it != cfg->block_id_to_block.end())
                {
                    G[id].push_back(true_it->second);
                    invG[true_id].push_back(bb);
                }

                auto false_it = cfg->block_id_to_block.find(false_id);
                if (false_it != cfg->block_id_to_block.end())
                {
                    G[id].push_back(false_it->second);
                    invG[false_id].push_back(bb);
                }
            }
        }
    }

    void EliminateDoubleBrUncondPass::updatePhiNodes(
        CFG* cfg, const std::unordered_map<int, int>& phiMap, const std::unordered_map<int, int>& otherPhiMap)
    {
        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            for (auto* inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::PHI)
                {
                    auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                    std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> new_vals;

                    for (const auto& [val, label] : phi->vals_for_labels)
                    {
                        auto* label_op      = static_cast<LLVMIR::LabelOperand*>(label);
                        int   label_num     = label_op->label_num;
                        int   new_label_num = label_num;

                        auto other_it = otherPhiMap.find(label_num);
                        if (other_it != otherPhiMap.end())
                        {
                            new_label_num = other_it->second;
                            while (phiMap.find(new_label_num) != phiMap.end()) new_label_num = phiMap.at(new_label_num);
                        }
                        else
                        {
                            while (phiMap.find(new_label_num) != phiMap.end()) new_label_num = phiMap.at(new_label_num);
                        }

                        if (new_label_num != label_num)
                            new_vals.emplace_back(val, getLabelOperand(new_label_num));
                        else
                            new_vals.emplace_back(val, label);
                    }

                    phi->vals_for_labels = new_vals;
                }
            }
        }
    }

    void EliminateDoubleBrUncondPass::updateBranchInstructions(CFG* cfg, const std::unordered_map<int, int>& phiMap)
    {
        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            if (bb->insts.empty()) continue;

            auto* terminator = bb->insts.back();

            if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
            {
                auto* br            = static_cast<LLVMIR::BranchUncondInst*>(terminator);
                auto* label_op      = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                int   label_num     = label_op->label_num;
                int   new_label_num = label_num;

                while (phiMap.find(new_label_num) != phiMap.end()) { new_label_num = phiMap.at(new_label_num); }

                if (new_label_num != label_num) { br->target_label = getLabelOperand(new_label_num); }
            }
            else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* br = static_cast<LLVMIR::BranchCondInst*>(terminator);

                auto* true_label     = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                int   true_label_num = true_label->label_num;
                int   new_true_label = true_label_num;

                while (phiMap.find(new_true_label) != phiMap.end()) new_true_label = phiMap.at(new_true_label);

                if (new_true_label != true_label_num) br->true_label = getLabelOperand(new_true_label);

                auto* false_label     = static_cast<LLVMIR::LabelOperand*>(br->false_label);
                int   false_label_num = false_label->label_num;
                int   new_false_label = false_label_num;

                while (phiMap.find(new_false_label) != phiMap.end()) new_false_label = phiMap.at(new_false_label);

                if (new_false_label != false_label_num) br->false_label = getLabelOperand(new_false_label);
            }
        }
    }

    void EliminateDoubleBrUncondPass::renumberBlocks(CFG* cfg)
    {
        int                          cnt = 0;
        std::unordered_map<int, int> newMap;

        for (const auto& [id, bb] : cfg->block_id_to_block) newMap[id] = cnt++;

        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            for (auto* inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::PHI)
                {
                    auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                    std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> new_vals;

                    for (const auto& [val, label] : phi->vals_for_labels)
                    {
                        auto* label_op  = static_cast<LLVMIR::LabelOperand*>(label);
                        int   label_num = label_op->label_num;

                        if (newMap.find(label_num) != newMap.end())
                            new_vals.emplace_back(val, getLabelOperand(newMap[label_num]));
                        else
                            new_vals.emplace_back(val, label);
                    }

                    phi->vals_for_labels = new_vals;
                }
                else if (inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
                {
                    auto* br        = static_cast<LLVMIR::BranchUncondInst*>(inst);
                    auto* label_op  = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                    int   label_num = label_op->label_num;

                    if (newMap.find(label_num) != newMap.end()) br->target_label = getLabelOperand(newMap[label_num]);
                }
                else if (inst->opcode == LLVMIR::IROpCode::BR_COND)
                {
                    auto* br = static_cast<LLVMIR::BranchCondInst*>(inst);

                    auto* true_label     = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                    int   true_label_num = true_label->label_num;
                    if (newMap.find(true_label_num) != newMap.end())
                        br->true_label = getLabelOperand(newMap[true_label_num]);

                    auto* false_label     = static_cast<LLVMIR::LabelOperand*>(br->false_label);
                    int   false_label_num = false_label->label_num;
                    if (newMap.find(false_label_num) != newMap.end())
                        br->false_label = getLabelOperand(newMap[false_label_num]);
                }
            }
        }

        std::map<int, LLVMIR::IRBlock*> new_block_map;
        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            bb->block_id              = newMap[id];
            new_block_map[newMap[id]] = bb;
        }

        cfg->block_id_to_block = new_block_map;

        if (cfg->func) cfg->func->max_label = cnt;
    }

    void EliminateDoubleBrUncondPass::removeBlockFromFunction(CFG* cfg, LLVMIR::IRBlock* block)
    {
        if (!cfg->func) return;

        auto& blocks = cfg->func->blocks;
        auto  it     = std::find(blocks.begin(), blocks.end(), block);
        if (it != blocks.end()) blocks.erase(it);
    }

    void EliminateDoubleBrUncondPass::cleanupSinglePredecessorPhis(CFG* cfg)
    {
        DBGINFO("=== Cleaning up single predecessor PHI nodes ===");
        std::map<int, LLVMIR::Operand*> phi_replacements;

        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            auto it = block->insts.begin();
            while (it != block->insts.end())
            {
                auto* inst = *it;
                if (inst->opcode == LLVMIR::IROpCode::PHI)
                {
                    auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

                    if (phi->vals_for_labels.size() <= 1)
                    {
                        if (phi->res && phi->res->type == LLVMIR::OperandType::REG)
                        {
                            int reg_num = static_cast<LLVMIR::RegOperand*>(phi->res)->reg_num;
                            DBGINFO("Removing PHI node for reg_%", reg_num, " in block ", block_id);
                            DBGINFO("PHI has ", phi->vals_for_labels.size(), " operands");

                            for (size_t i = 0; i < phi->vals_for_labels.size(); ++i)
                            {
                                auto* val   = phi->vals_for_labels[i].first;
                                auto* label = phi->vals_for_labels[i].second;
                                if (val && val->type == LLVMIR::OperandType::REG)
                                {
                                    int val_reg = static_cast<LLVMIR::RegOperand*>(val)->reg_num;
                                    DBGINFO("  Operand ",
                                        i,
                                        ": reg_%",
                                        val_reg,
                                        " from block ",
                                        static_cast<LLVMIR::LabelOperand*>(label)->label_num);
                                }
                                else if (val && val->type == LLVMIR::OperandType::IMMEI32)
                                {
                                    int val_imm = static_cast<LLVMIR::ImmeI32Operand*>(val)->value;
                                    DBGINFO("  Operand ",
                                        i,
                                        ": imm ",
                                        val_imm,
                                        " from block ",
                                        static_cast<LLVMIR::LabelOperand*>(label)->label_num);
                                }
                                else
                                {
                                    DBGINFO("  Operand ",
                                        i,
                                        ": unknown type from block ",
                                        static_cast<LLVMIR::LabelOperand*>(label)->label_num);
                                }
                            }

                            if (phi->vals_for_labels.empty())
                            {
                                DBGINFO("PHI is empty, replacing with nullptr");
                                phi_replacements[reg_num] = nullptr;
                            }
                            else
                            {
                                auto* replacement = phi->vals_for_labels[0].first;
                                if (replacement && replacement->type == LLVMIR::OperandType::REG)
                                {
                                    int repl_reg = static_cast<LLVMIR::RegOperand*>(replacement)->reg_num;
                                    DBGINFO("PHI has one operand, replacing reg_%", reg_num, " with reg_%", repl_reg);
                                }
                                else if (replacement && replacement->type == LLVMIR::OperandType::IMMEI32)
                                {
                                    int repl_imm = static_cast<LLVMIR::ImmeI32Operand*>(replacement)->value;
                                    DBGINFO("PHI has one operand, replacing reg_%", reg_num, " with imm ", repl_imm);
                                }
                                else { DBGINFO("PHI has one operand, replacing reg_%", reg_num, " with unknown type"); }
                                phi_replacements[reg_num] = replacement;
                            }
                        }

                        it = block->insts.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }

        if (!phi_replacements.empty())
        {
            DBGINFO("Replacing ", phi_replacements.size(), " PHI usages");
            for (const auto& [reg, replacement] : phi_replacements)
            {
                DBGINFO("reg_%", reg, " -> ", (replacement ? "valid operand" : "nullptr"));
            }

            bool has_changes = true;

            while (has_changes)
            {
                has_changes = false;

                for (const auto& [block_id, block] : cfg->block_id_to_block)
                {
                    for (auto* inst : block->insts)
                    {
                        if (inst && replacePhiUsages(inst, phi_replacements)) { has_changes = true; }
                    }
                }
            }
        }
    }

    bool EliminateDoubleBrUncondPass::replacePhiUsages(
        LLVMIR::Instruction* inst, const std::map<int, LLVMIR::Operand*>& replacements)
    {
        if (!inst) return false;

        bool has_replacement = false;

        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::PHI:
            {
                auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                for (auto& [val, label] : phi->vals_for_labels)
                {
                    if (val && val->type == LLVMIR::OperandType::REG)
                    {
                        int  reg_num = static_cast<LLVMIR::RegOperand*>(val)->reg_num;
                        auto it      = replacements.find(reg_num);
                        if (it != replacements.end() && it->second)
                        {
                            DBGINFO("  In PHI instruction at block ",
                                inst->block_id,
                                ": replacing reg_%",
                                reg_num,
                                " with new operand",
                                it->second);
                            val             = it->second;
                            has_replacement = true;
                        }
                    }
                }
                if (has_replacement) { DBGINFO("  PHI instruction updated"); }
                break;
            }
            case LLVMIR::IROpCode::BR_COND:
            {
                auto* br = static_cast<LLVMIR::BranchCondInst*>(inst);
                if (br->cond && br->cond->type == LLVMIR::OperandType::REG)
                {
                    int  reg_num = static_cast<LLVMIR::RegOperand*>(br->cond)->reg_num;
                    auto it      = replacements.find(reg_num);
                    if (it != replacements.end() && it->second)
                    {
                        br->cond        = it->second;
                        has_replacement = true;
                    }
                }
                break;
            }
            default:
            {
                std::map<int, LLVMIR::Operand*> operand_replacements;
                for (const auto& [old_reg, new_operand] : replacements)
                {
                    if (new_operand) { operand_replacements[old_reg] = new_operand; }
                }
                if (!operand_replacements.empty())
                {
                    DBGINFO("Replacing operands in instruction ", inst, " at block ", inst->block_id);
                    for (const auto& [old_reg, new_operand] : operand_replacements)
                    {
                        if (new_operand && new_operand->type == LLVMIR::OperandType::REG)
                        {
                            int new_reg = static_cast<LLVMIR::RegOperand*>(new_operand)->reg_num;
                            DBGINFO("  Will replace reg_%", old_reg, " with reg_%", new_reg);
                        }
                        else if (new_operand && new_operand->type == LLVMIR::OperandType::IMMEI32)
                        {
                            int new_imm = static_cast<LLVMIR::ImmeI32Operand*>(new_operand)->value;
                            DBGINFO("  Will replace reg_%", old_reg, " with imm ", new_imm);
                        }
                        else { DBGINFO("  Will replace reg_%", old_reg, " with unknown operand type"); }
                    }
                    has_replacement |= inst->SubstituteOperands(operand_replacements);
                    DBGINFO("  Substitution completed");
                }
                break;
            }
        }

        return has_replacement;
    }

    void Transform::EliminateDoubleBrUncondPass::mergeBlocks(LLVMIR::IRBlock* target, LLVMIR::IRBlock* source)
    {
        if (!target || !source) return;

        DBGINFO("Merging block ", source->block_id, " into block ", target->block_id);
        DBGINFO("Source block has ", source->insts.size(), " instructions");
        DBGINFO("Target block has ", target->insts.size(), " instructions before merge");

        std::vector<LLVMIR::Instruction*> phi_insts;
        std::vector<LLVMIR::Instruction*> non_phi_insts;

        for (auto* inst : source->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::EMPTY)
            {
                DBGINFO("Processing instruction in source block: ", inst->opcode);
                if (inst->GetResultReg() != -1) { DBGINFO("  Instruction defines reg_%", inst->GetResultReg()); }
                inst->block_id = target->block_id;
                if (inst->opcode == LLVMIR::IROpCode::PHI) { phi_insts.push_back(inst); }
                else { non_phi_insts.push_back(inst); }
            }
        }

        size_t phi_end_pos = 0;
        for (size_t i = 0; i < target->insts.size(); ++i)
        {
            if (target->insts[i]->opcode == LLVMIR::IROpCode::PHI) { phi_end_pos = i + 1; }
            else { break; }
        }

        auto it = target->insts.begin() + phi_end_pos;
        target->insts.insert(it, phi_insts.begin(), phi_insts.end());

        target->insts.insert(target->insts.end(), non_phi_insts.begin(), non_phi_insts.end());

        DBGINFO("Target block now has ", target->insts.size(), " instructions after merge");
        DBGINFO("Added ", phi_insts.size(), " PHI instructions and ", non_phi_insts.size(), " non-PHI instructions");
    }

}  // namespace Transform
