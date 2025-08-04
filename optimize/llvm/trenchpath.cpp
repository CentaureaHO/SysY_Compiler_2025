#include "trenchpath.h"
#include "cfg.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include <map>
#include <vector>

namespace LLVMIR
{
    void TrenchPath::Execute()
    {
        for (auto [func, cfg] : ir->cfg)
        {
            // 重置
            replace_map.clear();
            erase_blocks.clear();
            ExecuteInSingleCFG(cfg);
        }
    }

    int TrenchPath::TraceToBlock(int block_id)
    {
        // 这里可以实现一个追踪逻辑，返回最终的目标块ID
        // 目前简单返回传入的block_id
        int target_id = block_id;
        while (replace_map.find(target_id) != replace_map.end())
        {
#ifdef DEBUG_TRENCH_PATH
            std::cout << "Tracing block " << target_id << " to " << replace_map[target_id] << std::endl;
#endif

            target_id = replace_map[target_id];
        }
        return target_id;
    }

    void TrenchPath::ExecuteInSingleCFG(CFG* cfg)
    {
        for (auto [id, block] : cfg->block_id_to_block)
        {
            if (id == 0) continue;  // 忽略入口块
            auto inst = block->insts.front();
            if (inst->opcode == IROpCode::BR_UNCOND)
            {
                // Handle unconditional branch
                // 只有bruncond的块实际上是没必要的
                auto brun_inst  = dynamic_cast<BranchUncondInst*>(inst);
                auto target_id  = dynamic_cast<LabelOperand*>(brun_inst->target_label)->label_num;
                replace_map[id] = target_id;
                erase_blocks.insert(id);
                continue;
            }
        }

// 输出调试信息
#ifdef DEBUG_TRENCH_PATH
        std::cout << "TrenchPath: Replacing blocks in CFG for function: " << cfg->func->func_def->func_name
                  << std::endl;
        for (const auto& [from, to] : replace_map)
        {
            std::cout << "  Block " << from << " will be replaced with block " << to << std::endl;
        }
#endif

        for (auto erase_block : erase_blocks)
        {
            // 删除不必要的块
            cfg->block_id_to_block.erase(erase_block);

            cfg->G_id[erase_block].clear();
            cfg->invG_id[erase_block].clear();
        }

        for (auto func : ir->functions)
        {
            if (func->func_def == cfg->func->func_def)
            {
                std::vector<IRBlock*> new_blocks;
                for (auto block : func->blocks)
                {
                    if (erase_blocks.count(block->block_id) == 0) { new_blocks.push_back(block); }
                }
                func->blocks = std::move(new_blocks);
            }
        }

        for (auto [id, block] : cfg->block_id_to_block)
        {
            for (auto inst : block->insts)
            {
                switch (inst->opcode)
                {
                    case IROpCode::BR_UNCOND:
                    {
                        // 如果是无条件跳转，替换为目标块
                        auto               brun_inst = dynamic_cast<BranchUncondInst*>(inst);
                        auto               target_id = dynamic_cast<LabelOperand*>(brun_inst->target_label)->label_num;
                        int                final_target_id = TraceToBlock(target_id);
                        std::map<int, int> Map;
                        Map[target_id] = final_target_id;
                        if (final_target_id != target_id) { brun_inst->ReplaceLabels(Map); }
                    }
                    break;
                    case IROpCode::BR_COND:
                    {
                        // 条件跳转可能需要处理
                        // 这里可以添加更多逻辑来处理条件跳转
                        auto br_cond_inst      = dynamic_cast<BranchCondInst*>(inst);
                        auto true_label        = dynamic_cast<LabelOperand*>(br_cond_inst->true_label)->label_num;
                        auto false_label       = dynamic_cast<LabelOperand*>(br_cond_inst->false_label)->label_num;
                        int  final_true_label  = TraceToBlock(true_label);
                        int  final_false_label = TraceToBlock(false_label);
                        std::map<int, int> Map;
                        if (final_true_label != true_label) { Map[true_label] = final_true_label; }
                        if (final_false_label != false_label) { Map[false_label] = final_false_label; }
                        if (final_true_label != true_label || final_false_label != false_label)
                        {
                            br_cond_inst->ReplaceLabels(Map);
                        }
                    }
                    break;
                    case IROpCode::PHI:
                    {
                        auto               phi_inst = dynamic_cast<PhiInst*>(inst);
                        std::map<int, int> Map;
                        // 对于Phi指令，可能需要替换其输入
                        for (auto& [val, label] : phi_inst->vals_for_labels)
                        {
                            int label_id       = dynamic_cast<LabelOperand*>(label)->label_num;
                            int final_label_id = TraceToBlock(label_id);
                            if (final_label_id != label_id) { Map[label_id] = final_label_id; }
                        }
                        if (!Map.empty()) { phi_inst->ReplaceLabels(Map); }
                    }
                    break;
                    default: break;
                }
            }
        }
    }

}  // namespace LLVMIR