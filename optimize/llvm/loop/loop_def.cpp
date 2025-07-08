#include "llvm/loop/loop_def.h"
#include "cfg.h"
#include "llvm_ir/ir_builder.h"
#include <iostream>
#include <cassert>
#include <stack>
#include <queue>
#include <functional>
#include <algorithm>
#include <cassert>

void NaturalLoop::findExitNodes(CFG* cfg)
{
    exit_nodes.clear();
    exiting_nodes.clear();

    for (const auto& node : loop_nodes)
    {
        if (node->insts.empty()) continue;

        const auto* terminator = node->insts.back();

        switch (terminator->opcode)
        {
            case LLVMIR::IROpCode::BR_UNCOND:
            {
                const auto* br_uncond = dynamic_cast<const LLVMIR::BranchUncondInst*>(terminator);
                const auto* label_op  = dynamic_cast<const LLVMIR::LabelOperand*>(br_uncond->target_label);

                if (auto it = cfg->block_id_to_block.find(label_op->label_num); it != cfg->block_id_to_block.end())
                {
                    const auto* next_bb = it->second;
                    if (loop_nodes.find(const_cast<LLVMIR::IRBlock*>(next_bb)) == loop_nodes.end())
                    {
                        exiting_nodes.insert(node);
                        exit_nodes.insert(const_cast<LLVMIR::IRBlock*>(next_bb));
                    }
                }
                break;
            }

            case LLVMIR::IROpCode::RET: exiting_nodes.insert(node); break;

            case LLVMIR::IROpCode::BR_COND:
            {
                const auto* br_cond     = dynamic_cast<const LLVMIR::BranchCondInst*>(terminator);
                const auto* false_label = dynamic_cast<const LLVMIR::LabelOperand*>(br_cond->false_label);
                const auto* true_label  = dynamic_cast<const LLVMIR::LabelOperand*>(br_cond->true_label);

                bool is_exit = false;

                // Check false branch
                if (auto it1 = cfg->block_id_to_block.find(false_label->label_num); it1 != cfg->block_id_to_block.end())
                {
                    const auto* next_bb1 = it1->second;
                    if (loop_nodes.find(const_cast<LLVMIR::IRBlock*>(next_bb1)) == loop_nodes.end())
                    {
                        exit_nodes.insert(const_cast<LLVMIR::IRBlock*>(next_bb1));
                        is_exit = true;
                    }
                }

                // Check true branch
                if (auto it2 = cfg->block_id_to_block.find(true_label->label_num); it2 != cfg->block_id_to_block.end())
                {
                    const auto* next_bb2 = it2->second;
                    if (loop_nodes.find(const_cast<LLVMIR::IRBlock*>(next_bb2)) == loop_nodes.end())
                    {
                        exit_nodes.insert(const_cast<LLVMIR::IRBlock*>(next_bb2));
                        is_exit = true;
                    }
                }

                if (is_exit) exiting_nodes.insert(node);
                break;
            }

            default: break;
        }
    }
}

void NaturalLoop::simplify(CFG* cfg)
{
    addPreheader(cfg);
    insertDedicatedExits(cfg);
    insertSingleLatch(cfg);
}

bool NaturalLoop::isSimplified(CFG* cfg) const
{
    if (!cfg) return false;
    if (latches.size() != 1) return false;
    if (!preheader) return false;

    const auto reconstructed = findNodesInLoop(cfg, *latches.begin(), header);
    if (reconstructed.size() != loop_nodes.size()) return false;

    for (const auto& node : loop_nodes)
        if (reconstructed.find(node) == reconstructed.end()) return false;

    for (const auto& exit : exit_nodes)
    {
        int out_of_loop_predecessors = 0;

        if (static_cast<size_t>(exit->block_id) < cfg->invG.size())
        {
            for (const auto& pred : cfg->invG[exit->block_id])
                if (loop_nodes.find(pred) == loop_nodes.end()) ++out_of_loop_predecessors;
        }
        if (out_of_loop_predecessors > 0) return false;
    }

    return true;
}

void NaturalLoop::addPreheader(CFG* cfg)
{
    std::set<LLVMIR::IRBlock*> out_of_loop_predecessors;

    if (static_cast<size_t>(header->block_id) < cfg->invG.size())
    {
        for (const auto& pred : cfg->invG[header->block_id])
            if (loop_nodes.find(pred) == loop_nodes.end()) out_of_loop_predecessors.insert(pred);
    }

    assert(!out_of_loop_predecessors.empty() && "Loop header has no predecessors outside the loop");

    if (out_of_loop_predecessors.size() == 1)
    {
        auto* candidate = *out_of_loop_predecessors.begin();
        if (candidate->block_id != 0 && static_cast<size_t>(candidate->block_id) < cfg->G.size() &&
            cfg->G[candidate->block_id].size() == 1)
        {
            preheader = candidate;
            return;
        }
    }

    // Create new preheader && update parent loops
    preheader = insertTransferBlock(cfg, out_of_loop_predecessors, header);
    for (auto* current = fa_loop; current != nullptr; current = current->fa_loop) current->loop_nodes.insert(preheader);
}

void NaturalLoop::insertSingleLatch(CFG* cfg)
{
    assert(!latches.empty() && "Loop has no latch nodes");
    if (latches.size() == 1) return;  // Already has single latch

    auto* new_latch = insertTransferBlock(cfg, latches, header);
    latches.clear();
    latches.insert(new_latch);
    loop_nodes.insert(new_latch);

    for (auto* current = fa_loop; current != nullptr; current = current->fa_loop) current->loop_nodes.insert(new_latch);
}

void NaturalLoop::insertDedicatedExits(CFG* cfg)
{
    exit_nodes.clear();
    exiting_nodes.clear();
    findExitNodes(cfg);

    std::set<LLVMIR::IRBlock*>                   in_loop_predecessors;
    std::map<LLVMIR::IRBlock*, LLVMIR::IRBlock*> exit_mapping;

    for (const auto& exit : exit_nodes)
    {
        in_loop_predecessors.clear();
        bool has_out_of_loop_predecessor = false;

        if (static_cast<size_t>(exit->block_id) < cfg->invG.size())
        {
            for (const auto& pred : cfg->invG[exit->block_id])
            {
                if (loop_nodes.find(pred) != loop_nodes.end())
                    in_loop_predecessors.insert(pred);
                else
                    has_out_of_loop_predecessor = true;
            }
        }

        if (!(has_out_of_loop_predecessor && !in_loop_predecessors.empty())) continue;

        auto* new_exit = insertTransferBlock(cfg, in_loop_predecessors, exit);
        for (auto* current = fa_loop; current != nullptr; current = current->fa_loop)
        {
            if (current->loop_nodes.find(exit) != current->loop_nodes.end()) current->loop_nodes.insert(new_exit);

            current->exit_nodes.clear();
            current->exiting_nodes.clear();
            current->findExitNodes(cfg);
        }
        exit_mapping[exit] = new_exit;
    }

    // Update exit nodes
    for (const auto& [old_exit, new_exit] : exit_mapping)
    {
        exit_nodes.erase(old_exit);
        exit_nodes.insert(new_exit);
    }
}

NaturalLoopForest::~NaturalLoopForest() { clear(); }

void NaturalLoopForest::combineSameHeadLoops()
{
    std::set<LLVMIR::IRBlock*> seen_headers;
    std::set<NaturalLoop*>     loops_to_remove;

    for (auto* loop : loop_set)
    {
        if (seen_headers.find(loop->header) == seen_headers.end())
        {
            seen_headers.insert(loop->header);
            header_loop_map[loop->header] = loop;
            continue;
        }

        loops_to_remove.insert(loop);
        auto it = header_loop_map.find(loop->header);
        if (it == header_loop_map.end()) continue;

        auto* existing_loop = it->second;
        // Merge loop nodes and latches
        existing_loop->loop_nodes.insert(loop->loop_nodes.begin(), loop->loop_nodes.end());
        existing_loop->latches.insert(loop->latches.begin(), loop->latches.end());
    }

    for (auto* loop : loops_to_remove)
    {
        loop_set.erase(loop);
        delete loop;
    }
}

void NaturalLoopForest::buildHierarchy()
{
    loopG.clear();
    loopG.resize(loop_cnt + 1);

    std::vector<std::vector<NaturalLoop*>>    temp_graph(loop_cnt + 1);
    std::vector<std::pair<int, NaturalLoop*>> in_degree(loop_cnt + 1);

    for (auto* loop : loop_set) in_degree[loop->loop_id].second = loop;
    for (auto* l1 : loop_set)
    {
        for (auto* l2 : loop_set)
        {
            if (l1 == l2) continue;

            if (judgeLoopContain(l1, l2))
            {
                temp_graph[l1->loop_id].push_back(l2);
                ++in_degree[l2->loop_id].first;
            }
        }
    }

    std::queue<NaturalLoop*> queue;

    for (const auto& [degree, loop] : in_degree)
        if (degree == 0 && loop != nullptr) queue.push(loop);

    while (!queue.empty())
    {
        auto* current = queue.front();
        queue.pop();

        for (auto* child : temp_graph[current->loop_id])
        {
            --in_degree[child->loop_id].first;
            if (in_degree[child->loop_id].first != 0) continue;

            loopG[current->loop_id].push_back(child);
            child->fa_loop = current;
            queue.push(child);
        }
    }
}

void NaturalLoopForest::clear()
{
    for (auto* loop : loop_set) delete loop;

    loop_set.clear();
    header_loop_map.clear();
    loopG.clear();
    loop_cnt = 0;
}

std::vector<NaturalLoop*> NaturalLoopForest::getRootLoops() const
{
    std::vector<NaturalLoop*> roots;

    for (auto* loop : loop_set)
        if (loop->fa_loop == nullptr) roots.push_back(loop);

    return roots;
}

LLVMIR::IRBlock* insertTransferBlock(CFG* cfg, const std::set<LLVMIR::IRBlock*>& froms, LLVMIR::IRBlock* to)
{
    assert(cfg != nullptr && to != nullptr && "CFG and target block cannot be null");
    assert(!froms.empty() && "Source blocks cannot be empty");

    auto* transfer_block                             = cfg->func->createBlock();
    cfg->block_id_to_block[transfer_block->block_id] = transfer_block;

    std::vector<LLVMIR::PhiInst*> phi_instructions;
    for (auto* inst : to->insts)
    {
        if (inst->opcode != LLVMIR::IROpCode::PHI) break;
        phi_instructions.push_back(dynamic_cast<LLVMIR::PhiInst*>(inst));
    }

    for (auto* phi : phi_instructions)
    {
        auto* new_reg      = getRegOperand(++cfg->func->max_reg);
        auto* transfer_phi = new LLVMIR::PhiInst(phi->type, new_reg);

        // Add entries for each source block
        for (const auto* from : froms)
        {
            for (const auto& [val, label] : phi->vals_for_labels)
            {
                const auto* label_op = dynamic_cast<const LLVMIR::LabelOperand*>(label);
                if (label_op && label_op->label_num != from->block_id) continue;

                transfer_phi->Insert_into_PHI(val, getLabelOperand(from->block_id));
                break;
            }
        }

        if (!transfer_phi->vals_for_labels.empty())
        {
            transfer_block->insts.push_back(transfer_phi);

            // Update original phi
            std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> new_phi_vals;

            for (const auto& [val, label] : phi->vals_for_labels)
            {
                const auto* label_op    = dynamic_cast<const LLVMIR::LabelOperand*>(label);
                bool        should_keep = true;

                if (label_op)
                {
                    for (const auto* from : froms)
                    {
                        if (label_op->label_num == from->block_id)
                        {
                            should_keep = false;
                            break;
                        }
                    }
                }

                if (should_keep) new_phi_vals.emplace_back(val, label);
            }

            new_phi_vals.emplace_back(new_reg, getLabelOperand(transfer_block->block_id));
            phi->vals_for_labels = std::move(new_phi_vals);
        }
        else
            delete transfer_phi;
    }

    transfer_block->insts.push_back(new LLVMIR::BranchUncondInst(getLabelOperand(to->block_id)));
    // Update source blocks to branch to transfer block
    for (auto* from : froms)
    {
        if (from->insts.empty()) continue;

        auto* terminator = from->insts.back();

        if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* br_inst         = dynamic_cast<LLVMIR::BranchUncondInst*>(terminator);
            br_inst->target_label = getLabelOperand(transfer_block->block_id);
        }
        else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br_inst     = dynamic_cast<LLVMIR::BranchCondInst*>(terminator);
            auto* false_label = dynamic_cast<LLVMIR::LabelOperand*>(br_inst->false_label);
            auto* true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_inst->true_label);

            if (false_label && false_label->label_num == to->block_id)
                br_inst->false_label = getLabelOperand(transfer_block->block_id);
            if (true_label && true_label->label_num == to->block_id)
                br_inst->true_label = getLabelOperand(transfer_block->block_id);
        }
    }

    cfg->BuildCFG();
    return transfer_block;
}

std::set<LLVMIR::IRBlock*> findNodesInLoop(CFG* cfg, LLVMIR::IRBlock* latch, LLVMIR::IRBlock* header)
{
    assert(latch != nullptr && header != nullptr && "Latch and header cannot be null");

    std::set<LLVMIR::IRBlock*>   loop_nodes;
    std::stack<LLVMIR::IRBlock*> worklist;

    loop_nodes.insert(latch);
    loop_nodes.insert(header);

    if (latch == header) return loop_nodes;

    worklist.push(latch);

    while (!worklist.empty())
    {
        auto* current = worklist.top();
        worklist.pop();

        if (static_cast<size_t>(current->block_id) >= cfg->invG.size()) continue;

        for (auto* predecessor : cfg->invG[current->block_id])
        {
            if (loop_nodes.find(predecessor) == loop_nodes.end())
            {
                loop_nodes.insert(predecessor);
                worklist.push(predecessor);
            }
        }
    }

    return loop_nodes;
}

bool judgeLoopContain(const NaturalLoop* outer, const NaturalLoop* inner)
{
    if (!outer || !inner) return false;
    if (outer == inner) return false;

    return std::all_of(inner->loop_nodes.begin(), inner->loop_nodes.end(), [&outer](const auto* node) {
        return outer->loop_nodes.find(const_cast<LLVMIR::IRBlock*>(node)) != outer->loop_nodes.end();
    });
}
