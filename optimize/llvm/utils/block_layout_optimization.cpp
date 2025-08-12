#include "block_layout_optimization.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include <unordered_set>

namespace Transform
{
    BlockLayoutOptimizationPass::BlockLayoutOptimizationPass(LLVMIR::IR* ir) : Pass(ir), modified_(false) {}

    void BlockLayoutOptimizationPass::Execute()
    {
        modified_ = false;

        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (cfg && !cfg->block_id_to_block.empty()) { processFunction(cfg); }
        }
    }

    void BlockLayoutOptimizationPass::processFunction(CFG* cfg)
    {
        if (cfg->block_id_to_block.size() <= 2) { return; }

        std::vector<LLVMIR::IRBlock*> original_layout;
        for (const auto& [id, block] : cfg->block_id_to_block) { original_layout.push_back(block); }
        std::sort(original_layout.begin(), original_layout.end(), [](LLVMIR::IRBlock* a, LLVMIR::IRBlock* b) {
            return a->block_id < b->block_id;
        });

        int original_cost = calculateLayoutCost(original_layout, cfg);

        auto new_layout = optimizeBlockLayout(cfg);

        if (new_layout.size() != original_layout.size())
        {
            std::cerr << "Warning: Block count mismatch in layout optimization" << std::endl;
            return;
        }

        int new_cost = calculateLayoutCost(new_layout, cfg);

        std::cout << "Block layout analysis: original cost " << original_cost << ", optimized cost " << new_cost;

        if (new_cost < original_cost)
        {
            renumberBlocks(cfg, new_layout);
            modified_ = true;
            std::cout << " -> Applied (improvement: " << (original_cost - new_cost) << ")" << std::endl;
        }
        else
            std::cout << " -> No improvement, keeping original layout" << std::endl;
    }

    std::vector<LLVMIR::IRBlock*> BlockLayoutOptimizationPass::optimizeBlockLayout(CFG* cfg)
    {
        std::vector<BlockChain> chains;
        buildChains(cfg, chains);

        std::vector<LLVMIR::IRBlock*> layout;
        std::set<LLVMIR::IRBlock*>    placed_blocks;

        auto* entry_block  = getEntryBlock(cfg);
        bool  entry_placed = false;

        for (const auto& chain : chains)
        {
            if (chain.contains(entry_block))
            {
                for (auto* block : chain.blocks)
                {
                    if (!placed_blocks.count(block))
                    {
                        layout.push_back(block);
                        placed_blocks.insert(block);
                    }
                }
                entry_placed = true;
                break;
            }
        }

        for (const auto& chain : chains)
        {
            if (entry_placed && chain.contains(entry_block)) { continue; }

            for (auto* block : chain.blocks)
            {
                if (!placed_blocks.count(block))
                {
                    layout.push_back(block);
                    placed_blocks.insert(block);
                }
            }
        }

        for (const auto& [id, block] : cfg->block_id_to_block)
        {
            if (!placed_blocks.count(block)) { layout.push_back(block); }
        }

        return layout;
    }

    void BlockLayoutOptimizationPass::buildChains(CFG* cfg, std::vector<BlockChain>& chains)
    {
        std::set<LLVMIR::IRBlock*>                        placed_blocks;
        std::unordered_map<LLVMIR::IRBlock*, BlockChain*> block_to_chain;

        auto* entry_block = getEntryBlock(cfg);
        if (entry_block)
        {
            BlockChain main_chain;
            buildSingleChain(entry_block, main_chain, placed_blocks, cfg);

            for (auto* block : main_chain.blocks) { block_to_chain[block] = &main_chain; }
            chains.push_back(std::move(main_chain));
        }

        for (const auto& [id, block] : cfg->block_id_to_block)
        {
            if (!placed_blocks.count(block))
            {
                BlockChain chain;
                buildSingleChain(block, chain, placed_blocks, cfg);

                for (auto* chain_block : chain.blocks) { block_to_chain[chain_block] = &chain; }
                chains.push_back(std::move(chain));
            }
        }
    }

    void BlockLayoutOptimizationPass::buildSingleChain(
        LLVMIR::IRBlock* start_block, BlockChain& chain, std::set<LLVMIR::IRBlock*>& placed_blocks, CFG* cfg)
    {
        auto* current_block = start_block;

        while (current_block && !placed_blocks.count(current_block))
        {
            chain.addBlock(current_block);
            placed_blocks.insert(current_block);

            auto* best_successor = selectBestSuccessor(current_block, placed_blocks, cfg);
            current_block        = best_successor;
        }
    }

    LLVMIR::IRBlock* BlockLayoutOptimizationPass::selectBestSuccessor(
        LLVMIR::IRBlock* block, const std::set<LLVMIR::IRBlock*>& placed_blocks, CFG* cfg)
    {
        auto successors = getSuccessors(block, cfg);

        auto* fallthrough_block = getFallthroughBlock(block, cfg);
        if (fallthrough_block && !placed_blocks.count(fallthrough_block)) { return fallthrough_block; }

        LLVMIR::IRBlock* best_successor = nullptr;
        double           best_weight    = 0.0;

        for (auto* succ : successors)
        {
            if (!placed_blocks.count(succ))
            {
                double weight = calculateEdgeWeight(block, succ, cfg);
                if (weight > best_weight)
                {
                    best_weight    = weight;
                    best_successor = succ;
                }
            }
        }

        return best_successor;
    }

    double BlockLayoutOptimizationPass::calculateEdgeWeight(LLVMIR::IRBlock* from, LLVMIR::IRBlock* to, CFG* cfg)
    {
        if (isFallthrough(from, to)) return 2.0;

        if (from->insts.empty()) return 0.5;

        auto* terminator = from->insts.back();

        if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* br           = static_cast<LLVMIR::BranchUncondInst*>(terminator);
            auto* target_label = static_cast<LLVMIR::LabelOperand*>(br->target_label);
            if (target_label->label_num == to->block_id) { return 1.0; }
        }
        else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br          = static_cast<LLVMIR::BranchCondInst*>(terminator);
            auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
            auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);

            int true_id  = true_label->label_num;
            int false_id = false_label->label_num;

            double p_true  = 0.5;
            double p_false = 0.5;

            if (true_id < from->block_id)
            {
                p_true  = 0.67;
                p_false = 0.33;
            }
            else if (false_id < from->block_id)
            {
                p_true  = 0.33;
                p_false = 0.67;
            }

            if (to->block_id == true_id) { return p_true; }
            else if (to->block_id == false_id) { return p_false; }
        }

        return 0.1;
    }

    int BlockLayoutOptimizationPass::calculateLayoutCost(const std::vector<LLVMIR::IRBlock*>& layout, CFG* cfg)
    {
        std::unordered_map<int, int> block_positions;
        std::unordered_map<int, int> block_sizes;

        int current_position = 0;
        for (auto* block : layout)
        {
            block_positions[block->block_id] = current_position;
            int size_bytes                   = 4 * getBlockInstructionCount(block);
            block_sizes[block->block_id]     = size_bytes;
            current_position += size_bytes;
        }

        int total_cost = 0;

        std::unordered_map<int, int> layout_order;
        for (size_t i = 0; i < layout.size(); ++i) { layout_order[layout[i]->block_id] = static_cast<int>(i); }

        for (auto* block : layout)
        {
            if (block->insts.empty()) continue;

            auto* terminator = block->insts.back();

            if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
            {
                auto* br           = static_cast<LLVMIR::BranchUncondInst*>(terminator);
                auto* target_label = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                int   target_id    = target_label->label_num;

                if (block_positions.count(target_id))
                {
                    auto* target_block   = cfg->block_id_to_block[target_id];
                    bool  is_fallthrough = false;
                    if (layout_order.count(block->block_id) && layout_order.count(target_id))
                    {
                        is_fallthrough = (layout_order[target_id] == layout_order[block->block_id] + 1);
                    }

                    if (!is_fallthrough)
                    {
                        int cost = calculateJumpDistance(block, target_block, block_positions, block_sizes);
                        total_cost += cost;
                    }
                }
            }
            else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* br          = static_cast<LLVMIR::BranchCondInst*>(terminator);
                auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);

                int true_id  = true_label->label_num;
                int false_id = false_label->label_num;

                double p_true  = 0.5;
                double p_false = 0.5;

                if (true_id < block->block_id)
                {
                    p_true  = 0.67;
                    p_false = 0.33;
                }
                else if (false_id < block->block_id)
                {
                    p_true  = 0.33;
                    p_false = 0.67;
                }

                if (block_positions.count(true_id))
                {
                    auto* true_block     = cfg->block_id_to_block[true_id];
                    bool  is_fallthrough = false;
                    if (layout_order.count(block->block_id) && layout_order.count(true_id))
                    {
                        is_fallthrough = (layout_order[true_id] == layout_order[block->block_id] + 1);
                    }

                    if (!is_fallthrough)
                    {
                        int dist = calculateJumpDistance(block, true_block, block_positions, block_sizes);
                        total_cost += static_cast<int>(p_true * dist);
                    }
                }

                if (block_positions.count(false_id))
                {
                    auto* false_block    = cfg->block_id_to_block[false_id];
                    bool  is_fallthrough = false;
                    if (layout_order.count(block->block_id) && layout_order.count(false_id))
                    {
                        is_fallthrough = (layout_order[false_id] == layout_order[block->block_id] + 1);
                    }

                    if (!is_fallthrough)
                    {
                        int dist = calculateJumpDistance(block, false_block, block_positions, block_sizes);
                        total_cost += static_cast<int>(p_false * dist);
                    }
                }
            }
        }

        return total_cost;
    }

    int BlockLayoutOptimizationPass::calculateJumpDistance(LLVMIR::IRBlock* from, LLVMIR::IRBlock* to,
        const std::unordered_map<int, int>& block_positions, const std::unordered_map<int, int>& block_sizes)
    {
        auto from_it = block_positions.find(from->block_id);
        auto to_it   = block_positions.find(to->block_id);

        if (from_it == block_positions.end() || to_it == block_positions.end()) { return 0; }

        int from_pos = from_it->second + block_sizes.at(from->block_id) - 1;
        int to_pos   = to_it->second;

        if (to_pos > from_pos) { return to_pos - from_pos - 1; }
        else if (from_pos > to_pos) { return from_pos - to_pos - 1; }

        return 0;
    }

    void BlockLayoutOptimizationPass::renumberBlocks(CFG* cfg, const std::vector<LLVMIR::IRBlock*>& new_layout)
    {
        std::unordered_map<int, int> old_to_new_id;
        for (size_t i = 0; i < new_layout.size(); ++i) old_to_new_id[new_layout[i]->block_id] = static_cast<int>(i);

        updateBranchTargets(cfg, old_to_new_id);

        std::map<int, LLVMIR::IRBlock*> new_block_map;
        for (size_t i = 0; i < new_layout.size(); ++i)
        {
            auto* block                        = new_layout[i];
            block->block_id                    = static_cast<int>(i);
            new_block_map[static_cast<int>(i)] = block;
        }

        cfg->block_id_to_block = new_block_map;

        if (cfg->func)
        {
            cfg->func->blocks.clear();
            for (auto* block : new_layout) cfg->func->blocks.push_back(block);
            cfg->func->max_label = static_cast<int>(new_layout.size() - 1);
        }

        cfg->BuildCFG();
    }

    void BlockLayoutOptimizationPass::updateBranchTargets(CFG* cfg, const std::unordered_map<int, int>& old_to_new_id)
    {
        for (const auto& [id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
                {
                    auto* br     = static_cast<LLVMIR::BranchUncondInst*>(inst);
                    auto* label  = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                    int   old_id = label->label_num;
                    if (old_to_new_id.count(old_id))
                    {
                        br->target_label = LLVMIR::LabelOperand::get(old_to_new_id.at(old_id));
                    }
                }
                else if (inst->opcode == LLVMIR::IROpCode::BR_COND)
                {
                    auto* br = static_cast<LLVMIR::BranchCondInst*>(inst);

                    auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                    int   true_old_id = true_label->label_num;
                    if (old_to_new_id.count(true_old_id))
                    {
                        br->true_label = LLVMIR::LabelOperand::get(old_to_new_id.at(true_old_id));
                    }

                    auto* false_label  = static_cast<LLVMIR::LabelOperand*>(br->false_label);
                    int   false_old_id = false_label->label_num;
                    if (old_to_new_id.count(false_old_id))
                    {
                        br->false_label = LLVMIR::LabelOperand::get(old_to_new_id.at(false_old_id));
                    }
                }
                else if (inst->opcode == LLVMIR::IROpCode::PHI)
                {
                    auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                    std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> new_vals;

                    for (const auto& [val, label] : phi->vals_for_labels)
                    {
                        auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                        int   old_id   = label_op->label_num;
                        if (old_to_new_id.count(old_id))
                        {
                            new_vals.emplace_back(val, LLVMIR::LabelOperand::get(old_to_new_id.at(old_id)));
                        }
                        else { new_vals.emplace_back(val, label); }
                    }

                    phi->vals_for_labels = new_vals;
                }
            }
        }
    }

    std::vector<LLVMIR::IRBlock*> BlockLayoutOptimizationPass::getSuccessors(LLVMIR::IRBlock* block, CFG* cfg)
    {
        std::vector<LLVMIR::IRBlock*> successors;

        if (block->block_id < static_cast<int>(cfg->G.size())) { successors = cfg->G[block->block_id]; }

        return successors;
    }

    std::vector<LLVMIR::IRBlock*> BlockLayoutOptimizationPass::getPredecessors(LLVMIR::IRBlock* block, CFG* cfg)
    {
        std::vector<LLVMIR::IRBlock*> predecessors;

        if (block->block_id < static_cast<int>(cfg->invG.size())) { predecessors = cfg->invG[block->block_id]; }

        return predecessors;
    }

    int BlockLayoutOptimizationPass::getBlockInstructionCount(LLVMIR::IRBlock* block)
    {
        return static_cast<int>(block->insts.size());
    }

    bool BlockLayoutOptimizationPass::isFallthrough(LLVMIR::IRBlock* from, LLVMIR::IRBlock* to)
    {
        if (from->insts.empty()) return false;

        auto* terminator = from->insts.back();

        if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* br           = static_cast<LLVMIR::BranchUncondInst*>(terminator);
            auto* target_label = static_cast<LLVMIR::LabelOperand*>(br->target_label);
            return target_label->label_num == to->block_id;
        }
        else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br          = static_cast<LLVMIR::BranchCondInst*>(terminator);
            auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);
            return false_label->label_num == to->block_id;
        }

        return false;
    }

    LLVMIR::IRBlock* BlockLayoutOptimizationPass::getFallthroughBlock(LLVMIR::IRBlock* block, CFG* cfg)
    {
        if (block->insts.empty()) return nullptr;

        auto* terminator = block->insts.back();

        if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* br           = static_cast<LLVMIR::BranchUncondInst*>(terminator);
            auto* target_label = static_cast<LLVMIR::LabelOperand*>(br->target_label);
            int   target_id    = target_label->label_num;

            auto it = cfg->block_id_to_block.find(target_id);
            return (it != cfg->block_id_to_block.end()) ? it->second : nullptr;
        }
        else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br          = static_cast<LLVMIR::BranchCondInst*>(terminator);
            auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);
            int   false_id    = false_label->label_num;

            auto it = cfg->block_id_to_block.find(false_id);
            return (it != cfg->block_id_to_block.end()) ? it->second : nullptr;
        }

        return nullptr;
    }

    LLVMIR::IRBlock* BlockLayoutOptimizationPass::getEntryBlock(CFG* cfg)
    {
        LLVMIR::IRBlock* entry_block = nullptr;
        for (const auto& [id, block] : cfg->block_id_to_block)
        {
            if (!entry_block || block->block_id < entry_block->block_id) entry_block = block;
        }
        return entry_block;
    }

}  // namespace Transform
