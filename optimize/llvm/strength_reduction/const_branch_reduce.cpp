#include "const_branch_reduce.h"
#include <set>
#include <queue>
#include <map>
#include <iostream>

namespace Transform
{
    ConstBranchReduce::ConstBranchReduce(LLVMIR::IR* ir) : Pass(ir) {}

    void ConstBranchReduce::Execute()
    {
        cur_cfg = nullptr;

        for (const auto& [func_def, cfg_ptr] : ir->cfg)
        {
            cur_cfg = cfg_ptr;

            for (auto& block : cur_cfg->block_id_to_block) runOnBlock(block.second);

            elimateUnreachableBlocks();

            cleanupPhiInstructions();
        }
    }

    void ConstBranchReduce::runOnBlock(LLVMIR::IRBlock* block)
    {
        auto condbr_iter = getCondbrIter(block);
        if (condbr_iter == block->insts.end()) return;

        LLVMIR::BranchCondInst* condbr = static_cast<LLVMIR::BranchCondInst*>(*condbr_iter);

        if (condbr->cond->type != LLVMIR::OperandType::IMMEI32) return;

        // std::cout << "ConstBranchReduce: Found constant branch in block " << block->block_id << std::endl;
        LLVMIR::Operand* target_label =
            (static_cast<LLVMIR::ImmeI32Operand*>(condbr->cond)->value == 0) ? condbr->false_label : condbr->true_label;

        auto uncond_br      = new LLVMIR::BranchUncondInst(target_label);
        uncond_br->block_id = block->block_id;

        *condbr_iter = uncond_br;
        delete condbr;
        condbr = nullptr;
    }

    std::deque<LLVMIR::Instruction*>::iterator ConstBranchReduce::getCondbrIter(LLVMIR::IRBlock* block)
    {
        for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
        {
            auto opcode = (*it)->opcode;

            if (opcode == LLVMIR::IROpCode::RET || opcode == LLVMIR::IROpCode::BR_UNCOND) return block->insts.end();
            if (opcode == LLVMIR::IROpCode::BR_COND) return it;
        }

        assert(false && "No terminating instruction found in block");
        return block->insts.end();
    }

    void ConstBranchReduce::elimateUnreachableBlocks()
    {
        cur_cfg->BuildCFG();

        /* BuildCFG中已经干了
        int             entry = 0;
        std::set<int>   reachable_blocks;
        std::queue<int> bfs_list;
        bfs_list.push(entry);

        while (!bfs_list.empty())
        {
            int current = bfs_list.front();
            bfs_list.pop();

            if (reachable_blocks.find(current) != reachable_blocks.end()) continue;

            reachable_blocks.insert(current);

            for (auto* succ : cur_cfg->G[current])
            {
                if (succ->block_id != -1 && reachable_blocks.find(succ->block_id) == reachable_blocks.end())
                {
                    bfs_list.push(succ->block_id);
                }
            }
        }
        */
    }

    void ConstBranchReduce::cleanupPhiInstructions()
    {
        // 获取当前CFG中所有存在的基本块ID
        std::set<int> reachable_blocks;
        for (const auto& [block_id, block_ptr] : cur_cfg->block_id_to_block) { reachable_blocks.insert(block_id); }

        // 遍历所有基本块，查找并清理PHI指令
        for (const auto& [block_id, block_ptr] : cur_cfg->block_id_to_block)
        {
            for (auto& inst : block_ptr->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::PHI)
                {
                    LLVMIR::PhiInst* phi_inst = static_cast<LLVMIR::PhiInst*>(inst);

                    // 使用map来去重和检查可达性
                    std::map<int, LLVMIR::Operand*> unique_sources;

                    // 收集有效的来源对
                    for (const auto& [val, label] : phi_inst->vals_for_labels)
                    {
                        if (label->type == LLVMIR::OperandType::LABEL)
                        {
                            LLVMIR::LabelOperand* label_op        = static_cast<LLVMIR::LabelOperand*>(label);
                            int                   source_block_id = label_op->label_num;

                            // 只保留存在于CFG中的基本块的来源
                            if (reachable_blocks.find(source_block_id) != reachable_blocks.end())
                            {
                                unique_sources[source_block_id] = val;
                            }
                        }
                    }

                    // 重建PHI指令的vals_for_labels
                    phi_inst->vals_for_labels.clear();
                    for (const auto& [source_block_id, val] : unique_sources)
                    {
                        LLVMIR::LabelOperand* label_op = getLabelOperand(source_block_id);
                        phi_inst->vals_for_labels.push_back(std::make_pair(val, label_op));
                    }

                    // 如果PHI指令没有有效的来源，这通常表示存在死代码
                    // 但根据用户要求，即使只有一个来源也不简化，所以这里只是确保至少有一个来源
                    if (phi_inst->vals_for_labels.empty())
                    {
                        std::cerr << "Warning: PHI instruction with no valid sources found in block " << block_id
                                  << std::endl;
                    }
                }
            }
        }
    }
}  // namespace Transform
