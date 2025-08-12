#include <backend/rv64/passes/optimize/control_flow/block_layout.h>
#include <queue>
#include <algorithm>
#include <iostream>

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    BlockLayoutPass::BlockLayoutPass(std::vector<Function*>& functions) : functions_(functions) {}

    bool BlockLayoutPass::run()
    {
        bool modified = false;
        for (auto* func : functions_) { modified |= optimizeFunctionLayout(func); }
        return modified;
    }

    bool BlockLayoutPass::optimizeFunctionLayout(Function* func)
    {
        if (func == nullptr || func->cfg == nullptr) return false;
        if (func->blocks.size() <= 2) return false;

        std::vector<Block*> original_layout = func->blocks;
        int                 original_cost   = calculateLayoutCost(original_layout, func->cfg);

        auto new_layout = optimizeBlockLayout(func->cfg);

        if (new_layout.size() != original_layout.size())
        {
            std::cerr << "Warning: Block count mismatch in layout optimization" << std::endl;
            return false;
        }

        int new_cost = calculateLayoutCost(new_layout, func->cfg);

        std::cout << "Block layout analysis for " << func->name << ": original cost " << original_cost
                  << ", optimized cost " << new_cost;

        if (new_cost < original_cost)
        {
            func->blocks = new_layout;

            renumberBlocks(func, new_layout);

            std::cout << " -> Applied (improvement: " << (original_cost - new_cost) << ")" << std::endl;
            return true;
        }
        else
        {
            std::cout << " -> No improvement, keeping original layout" << std::endl;
            return false;
        }
    }

    std::vector<Block*> BlockLayoutPass::optimizeBlockLayout(CFG* cfg)
    {
        std::vector<BlockChain> chains;
        buildChains(cfg, chains);

        std::vector<Block*> layout;
        std::set<Block*>    placed_blocks;

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

        for (const auto& [id, block] : cfg->blocks)
        {
            if (!placed_blocks.count(block)) { layout.push_back(block); }
        }

        return layout;
    }

    void BlockLayoutPass::buildChains(CFG* cfg, std::vector<BlockChain>& chains)
    {
        std::set<Block*> placed_blocks;

        auto* entry_block = getEntryBlock(cfg);
        if (entry_block)
        {
            BlockChain main_chain;
            buildSingleChain(entry_block, main_chain, placed_blocks, cfg);
            chains.push_back(std::move(main_chain));
        }

        for (const auto& [id, block] : cfg->blocks)
        {
            if (!placed_blocks.count(block))
            {
                BlockChain chain;
                buildSingleChain(block, chain, placed_blocks, cfg);
                chains.push_back(std::move(chain));
            }
        }
    }

    void BlockLayoutPass::buildSingleChain(
        Block* start_block, BlockChain& chain, std::set<Block*>& placed_blocks, CFG* cfg)
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

    Block* BlockLayoutPass::selectBestSuccessor(Block* block, const std::set<Block*>& placed_blocks, CFG* cfg)
    {
        auto successors = getSuccessors(block, cfg);

        auto* fallthrough_block = getFallthroughBlock(block, cfg);
        if (fallthrough_block && !placed_blocks.count(fallthrough_block)) { return fallthrough_block; }

        Block* best_successor = nullptr;
        double best_weight    = 0.0;

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

    double BlockLayoutPass::calculateEdgeWeight(Block* from, Block* to, CFG* cfg)
    {
        if (isFallthrough(from, to)) return 2.0;

        if (from->insts.empty()) return 0.5;

        if (hasConditionalJumpPattern(from))
        {
            auto [true_target, false_target] = getConditionalTargets(from, cfg);

            if (to == true_target)
            {
                if (to->label_num < from->label_num)
                    return 0.8;
                else
                    return 0.3;
            }
            else if (to == false_target)
            {
                if (to->label_num < from->label_num)
                    return 0.7;
                else
                    return 0.7;
            }
        }
        else if (hasUnconditionalBranch(from)) { return 1.0; }

        return 0.1;
    }

    int BlockLayoutPass::calculateLayoutCost(const std::vector<Block*>& layout, CFG* cfg)
    {
        std::unordered_map<int, int> block_positions;
        std::unordered_map<int, int> block_sizes;

        int current_position = 0;
        for (auto* block : layout)
        {
            block_positions[block->label_num] = current_position;
            int size_bytes                    = 4 * getBlockInstructionCount(block);  // 假设每条指令4字节
            block_sizes[block->label_num]     = size_bytes;
            current_position += size_bytes;
        }

        int                          total_cost = 0;
        std::unordered_map<int, int> layout_order;
        for (size_t i = 0; i < layout.size(); ++i) { layout_order[layout[i]->label_num] = static_cast<int>(i); }

        for (auto* block : layout)
        {
            if (block->insts.empty()) continue;

            auto successors = getSuccessors(block, cfg);
            for (auto* succ : successors)
            {
                int succ_id = succ->label_num;

                bool is_fallthrough = false;
                if (layout_order.count(block->label_num) && layout_order.count(succ_id))
                {
                    is_fallthrough = (layout_order[succ_id] == layout_order[block->label_num] + 1);
                }

                if (!is_fallthrough && block_positions.count(succ_id))
                {
                    double weight   = calculateEdgeWeight(block, succ, cfg);
                    int    distance = calculateJumpDistance(block, succ, block_positions, block_sizes);
                    total_cost += static_cast<int>(weight * distance);
                }
            }
        }

        return total_cost;
    }

    int BlockLayoutPass::calculateJumpDistance(Block* from, Block* to,
        const std::unordered_map<int, int>& block_positions, const std::unordered_map<int, int>& block_sizes)
    {
        auto from_it = block_positions.find(from->label_num);
        auto to_it   = block_positions.find(to->label_num);

        if (from_it == block_positions.end() || to_it == block_positions.end()) { return 0; }

        int from_pos = from_it->second + block_sizes.at(from->label_num) - 1;
        int to_pos   = to_it->second;

        return std::abs(to_pos - from_pos);
    }

    void BlockLayoutPass::renumberBlocks(Function* func, const std::vector<Block*>& new_layout)
    {
        std::unordered_map<int, int> label_mapping;

        for (size_t i = 0; i < new_layout.size(); ++i)
        {
            Block* block     = new_layout[i];
            int    old_label = block->label_num;
            int    new_label = static_cast<int>(i);

            label_mapping[old_label] = new_label;
            block->label_num         = new_label;
        }

        updateInstructionLabels(func, label_mapping);

        updateCFGBlockMapping(func->cfg, label_mapping);

        std::cout << "Blocks renumbered from 0 to " << (new_layout.size() - 1) << std::endl;
    }

    void BlockLayoutPass::updateInstructionLabels(Function* func, const std::unordered_map<int, int>& label_mapping)
    {
        for (auto* block : func->blocks)
        {
            for (auto* inst : block->insts)
            {
                if (inst->inst_type == InstType::RV64)
                {
                    auto* rv64_inst = static_cast<RV64Inst*>(inst);
                    if (rv64_inst->use_label)
                    {
                        auto jmp_it = label_mapping.find(rv64_inst->label.jmp_label);
                        if (jmp_it != label_mapping.end()) { rv64_inst->label.jmp_label = jmp_it->second; }

                        auto seq_it = label_mapping.find(rv64_inst->label.seq_label);
                        if (seq_it != label_mapping.end()) { rv64_inst->label.seq_label = seq_it->second; }
                    }
                }
            }
        }
    }

    std::vector<Block*> BlockLayoutPass::getSuccessors(Block* block, CFG* cfg)
    {
        std::vector<Block*> successors;
        if (block->label_num < static_cast<int>(cfg->graph.size())) { successors = cfg->graph[block->label_num]; }
        return successors;
    }

    std::vector<Block*> BlockLayoutPass::getPredecessors(Block* block, CFG* cfg)
    {
        std::vector<Block*> predecessors;
        if (block->label_num < static_cast<int>(cfg->inv_graph.size()))
        {
            predecessors = cfg->inv_graph[block->label_num];
        }
        return predecessors;
    }

    int BlockLayoutPass::getBlockInstructionCount(Block* block) { return static_cast<int>(block->insts.size()); }

    bool BlockLayoutPass::isFallthrough(Block* from, Block* to)
    {
        if (from->insts.empty()) return false;

        auto* last_inst = from->insts.back();
        if (last_inst->inst_type != InstType::RV64) return false;

        auto* rv64_inst = static_cast<RV64Inst*>(last_inst);

        if (rv64_inst->use_label && rv64_inst->op == RV64InstType::JAL &&
            rv64_inst->rd.reg_num == static_cast<int>(REG::x0))
        {
            return rv64_inst->label.jmp_label == to->label_num;
        }

        return false;
    }

    Block* BlockLayoutPass::getFallthroughBlock(Block* block, CFG* cfg)
    {
        if (block->insts.empty()) return nullptr;

        auto* last_inst = block->insts.back();
        if (last_inst->inst_type != InstType::RV64) return nullptr;

        auto* rv64_inst = static_cast<RV64Inst*>(last_inst);

        if (rv64_inst->use_label)
        {
            int  target_label = rv64_inst->label.jmp_label;
            auto it           = cfg->blocks.find(target_label);
            if (it != cfg->blocks.end()) { return it->second; }
        }

        return nullptr;
    }

    Block* BlockLayoutPass::getEntryBlock(CFG* cfg)
    {
        auto it = cfg->blocks.find(0);
        return (it != cfg->blocks.end()) ? it->second : nullptr;
    }

    bool BlockLayoutPass::hasConditionalJumpPattern(Block* block)
    {
        if (block->insts.size() < 2) return false;

        auto* last_inst = block->insts.back();
        if (last_inst->inst_type != InstType::RV64) return false;
        auto* last_rv64 = static_cast<RV64Inst*>(last_inst);
        if (last_rv64->op != RV64InstType::JAL || last_rv64->rd.reg_num != static_cast<int>(REG::x0)) return false;

        auto it = block->insts.end();
        --it;
        --it;
        auto* second_last_inst = *it;

        if (second_last_inst->inst_type != InstType::RV64) return false;
        auto* second_rv64 = static_cast<RV64Inst*>(second_last_inst);
        return (second_rv64->op >= RV64InstType::BEQ && second_rv64->op <= RV64InstType::BLEU);
    }

    bool BlockLayoutPass::hasUnconditionalBranch(Block* block)
    {
        if (block->insts.empty()) return false;

        auto* last_inst = block->insts.back();
        if (last_inst->inst_type != InstType::RV64) return false;

        auto* rv64_inst = static_cast<RV64Inst*>(last_inst);
        return (rv64_inst->op == RV64InstType::JAL && rv64_inst->rd.reg_num == static_cast<int>(REG::x0));
    }

    bool BlockLayoutPass::isReturn(Block* block)
    {
        if (block->insts.empty()) return false;

        auto* last_inst = block->insts.back();
        if (last_inst->inst_type != InstType::RV64) return false;

        auto* rv64_inst = static_cast<RV64Inst*>(last_inst);
        return (rv64_inst->op == RV64InstType::JALR && rv64_inst->rd.reg_num == static_cast<int>(REG::x0) &&
                rv64_inst->rs1.reg_num == static_cast<int>(REG::x1));  // x1 is ra
    }

    std::pair<Block*, Block*> BlockLayoutPass::getConditionalTargets(Block* block, CFG* cfg)
    {
        if (!hasConditionalJumpPattern(block)) return {nullptr, nullptr};

        auto it = block->insts.end();
        --it;
        --it;
        auto* conditional_inst = *it;

        if (conditional_inst->inst_type != InstType::RV64) return {nullptr, nullptr};
        auto* cond_rv64 = static_cast<RV64Inst*>(conditional_inst);

        if (!cond_rv64->use_label) return {nullptr, nullptr};
        int true_target_id = cond_rv64->label.jmp_label;

        auto* unconditional_inst = block->insts.back();
        auto* uncond_rv64        = static_cast<RV64Inst*>(unconditional_inst);

        if (!uncond_rv64->use_label) return {nullptr, nullptr};
        int false_target_id = uncond_rv64->label.jmp_label;

        Block* true_target  = nullptr;
        Block* false_target = nullptr;

        auto true_it = cfg->blocks.find(true_target_id);
        if (true_it != cfg->blocks.end()) true_target = true_it->second;

        auto false_it = cfg->blocks.find(false_target_id);
        if (false_it != cfg->blocks.end()) false_target = false_it->second;

        return {true_target, false_target};
    }

    bool BlockLayoutPass::BlockChain::contains(Block* block) const
    {
        return std::find(blocks.begin(), blocks.end(), block) != blocks.end();
    }

    void BlockLayoutPass::BlockChain::merge(const BlockChain& other)
    {
        blocks.insert(blocks.end(), other.blocks.begin(), other.blocks.end());
    }

    void BlockLayoutPass::updateCFGBlockMapping(CFG* cfg, const std::unordered_map<int, int>& label_mapping)
    {
        std::map<int, Block*> new_blocks;
        for (const auto& [old_label, block] : cfg->blocks)
        {
            auto it = label_mapping.find(old_label);
            if (it != label_mapping.end()) { new_blocks[it->second] = block; }
        }
        cfg->blocks = new_blocks;

        cfg->max_label = static_cast<int>(new_blocks.size()) - 1;

        auto entry_it = cfg->blocks.find(0);
        if (entry_it != cfg->blocks.end()) { cfg->entry_block = entry_it->second; }
    }

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow
