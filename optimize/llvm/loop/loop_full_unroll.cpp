#include "llvm/loop/loop_full_unroll.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include <iostream>
#include <algorithm>
#include <functional>

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

extern int max_unroll_count;

namespace Transform
{
    LoopFullUnrollPass::LoopFullUnrollPass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev)
        : Pass(ir), scev_analyser_(scev), loops_processed_(0), loops_fully_unrolled_(0)
    {}

    void LoopFullUnrollPass::Execute()
    {
        static int exec_cnt = 0;
        processAllLoops();
    }

    void LoopFullUnrollPass::processAllLoops()
    {
        for (const auto& [func_def, cfg] : ir->cfg)
            if (cfg && cfg->LoopForest && !cfg->LoopForest->loop_set.empty()) processFunction(cfg);
    }

    void LoopFullUnrollPass::processFunction(CFG* cfg)
    {
        if (!cfg->LoopForest) return;

        std::function<void(NaturalLoop*)> dfs = [&](NaturalLoop* loop) {
            for (auto* child_loop : cfg->LoopForest->loopG[loop->loop_id])
                if (child_loop != nullptr) dfs(child_loop);

            bool has_children = false;
            for (auto* child_loop : cfg->LoopForest->loopG[loop->loop_id])
            {
                if (child_loop != nullptr)
                {
                    has_children = true;
                    break;
                }
            }

            if (!has_children) processLoop(cfg, loop);
        };

        auto root_loops = cfg->LoopForest->getRootLoops();
        for (auto* loop : root_loops) dfs(loop);
    }

    void LoopFullUnrollPass::processLoop(CFG* cfg, NaturalLoop* loop)
    {
        scev_analyser_->analyzeSingleLoop(loop);
        tryFullyUnrollLoop(cfg, loop);
    }

    bool LoopFullUnrollPass::tryFullyUnrollLoop(CFG* cfg, NaturalLoop* loop)
    {
        ++loops_processed_;

        // 分步检查各种限制条件，提供详细的失败原因
        if (!loop || !loop->header || !loop->preheader || loop->latches.size() != 1)
        {
            logResult(loop, false, "Basic structure check failed");
            return false;
        }

        if (!scev_analyser_->canFullyUnrollLoop(loop))
        {
            logResult(loop, false, "SCEV analysis failed");
            return false;
        }

        if (loop->loop_nodes.size() > MAX_LOOP_BLOCKS)
        {
            logResult(loop,
                false,
                "Too many loop blocks (" + std::to_string(loop->loop_nodes.size()) + " > " +
                    std::to_string(MAX_LOOP_BLOCKS) + ")");
            return false;
        }

        int global_insts = getGlobalInstructionCount();
        if (global_insts >= MAX_GLOBAL_INSTRUCTIONS)
        {
            logResult(loop,
                false,
                "Too many global instructions (" + std::to_string(global_insts) +
                    " >= " + std::to_string(MAX_GLOBAL_INSTRUCTIONS) + ")");
            return false;
        }

        auto trip_count_opt = scev_analyser_->getLoopTripCount(loop);
        if (!trip_count_opt)
        {
            logResult(loop, false, "Trip count unknown");
            return false;
        }

        int trip_count = *trip_count_opt;
        if (!isUnrollProfitable(loop, trip_count))
        {
            int loop_size = getLoopSize(loop);
            logResult(loop,
                false,
                "Not profitable (size=" + std::to_string(loop_size) + " * count=" + std::to_string(trip_count) + " = " +
                    std::to_string(loop_size * trip_count) + " > " + std::to_string(MAX_UNROLLED_INSTRUCTIONS) + ")");
            return false;
        }

        if (performFullUnroll(cfg, loop, trip_count))
        {
            ++loops_fully_unrolled_;
            logResult(loop,
                true,
                "Successfully unrolled (blocks=" + std::to_string(loop->loop_nodes.size()) +
                    ", trip_count=" + std::to_string(trip_count) + ")");
            return true;
        }
        else
        {
            logResult(loop, false, "Unroll implementation failed");
            return false;
        }
    }

    bool LoopFullUnrollPass::canFullyUnroll(NaturalLoop* loop) const
    {
        // 只做基本的结构检查，详细检查在tryFullyUnrollLoop中进行
        return loop && loop->header && loop->preheader && loop->latches.size() == 1;
    }

    bool LoopFullUnrollPass::isUnrollProfitable(NaturalLoop* loop, int trip_count) const
    {
        int loop_size = getLoopSize(loop);
        return loop_size * trip_count <= MAX_UNROLLED_INSTRUCTIONS;
    }

    int LoopFullUnrollPass::getLoopSize(NaturalLoop* loop) const
    {
        int size = 0;
        for (auto* block : loop->loop_nodes) size += block->insts.size();
        return size;
    }

    int LoopFullUnrollPass::getGlobalInstructionCount() const
    {
        int total_instructions = 0;
        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (cfg)
            {
                for (const auto& [block_id, block] : cfg->block_id_to_block)
                {
                    total_instructions += block->insts.size();
                }
            }
        }
        return total_instructions;
    }

    bool LoopFullUnrollPass::performFullUnroll(CFG* cfg, NaturalLoop* loop, int trip_count)
    {
        auto* preheader = loop->preheader;
        auto* header    = loop->header;
        auto* latch     = *loop->latches.begin();

        // Step 1: Collect original PHI nodes
        std::vector<LLVMIR::PhiInst*> orig_phi_nodes;
        for (auto* inst : header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) continue;
            orig_phi_nodes.push_back(static_cast<LLVMIR::PhiInst*>(inst));
        }

        // Step 2: Clone iterations
        ValueMap                      last_value_map;
        std::vector<LLVMIR::IRBlock*> headers, latches;

        std::set<int> original_loop_regs;
        for (auto* block : loop->loop_nodes)
            for (auto* inst : block->insts)
            {
                int reg = inst->GetResultReg();
                if (reg != -1) original_loop_regs.insert(reg);
            }

        headers.push_back(header);
        latches.push_back(latch);

        for (int reg : original_loop_regs) last_value_map[reg] = getRegOperand(reg);

        for (int iteration = 1; iteration < trip_count; ++iteration)
        {
            std::map<LLVMIR::IRBlock*, LLVMIR::IRBlock*> block_map;
            std::map<int, int>                           reg_map, label_map;

            for (auto* orig_block : loop->loop_nodes)
            {
                auto* new_block                 = cfg->createBlock();
                block_map[orig_block]           = new_block;
                label_map[orig_block->block_id] = new_block->block_id;
                new_block->comment              = "Iteration " + std::to_string(iteration) + " of loop with header " +
                                     std::to_string(loop->header->block_id) + " for block " +
                                     std::to_string(orig_block->block_id);
            }

            std::set<int> phi_result_regs;
            for (auto* inst : header->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::PHI) phi_result_regs.insert(inst->GetResultReg());
            }

            for (int orig_reg : original_loop_regs)
            {
                if (phi_result_regs.find(orig_reg) == phi_result_regs.end())
                {
                    int new_reg       = ++cfg->func->max_reg;
                    reg_map[orig_reg] = new_reg;
                }
            }

            std::map<int, int> combined_reg_map = reg_map;

            for (const auto& [orig_reg, new_operand] : last_value_map)
            {
                if (new_operand->type == LLVMIR::OperandType::REG)
                {
                    auto* reg_op = static_cast<LLVMIR::RegOperand*>(new_operand);

                    if (reg_map.find(orig_reg) == reg_map.end()) combined_reg_map[orig_reg] = reg_op->reg_num;
                }
            }

            for (auto* inst : header->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::PHI) continue;

                auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

                // Find latch incoming value
                LLVMIR::Operand* in_val = nullptr;
                for (const auto& [val, label] : phi->vals_for_labels)
                {
                    auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                    if (label_op->label_num == latch->block_id)
                    {
                        in_val = val;
                        if (val->type == LLVMIR::OperandType::REG) auto* reg_op = static_cast<LLVMIR::RegOperand*>(val);

                        break;
                    }
                }

                if (in_val && in_val->type == LLVMIR::OperandType::REG && iteration > 1)
                {
                    auto* reg_op = static_cast<LLVMIR::RegOperand*>(in_val);
                    auto  it     = last_value_map.find(reg_op->reg_num);
                    if (it != last_value_map.end())
                    {
                        if (it->second->type == LLVMIR::OperandType::REG)
                            auto* new_reg_op = static_cast<LLVMIR::RegOperand*>(it->second);

                        in_val = it->second;
                    }
                }

                if (in_val && in_val->type != LLVMIR::OperandType::REG) continue;

                auto* reg_op                          = static_cast<LLVMIR::RegOperand*>(in_val);
                combined_reg_map[phi->GetResultReg()] = reg_op->reg_num;
            }

            // Step 3: Clone all non-PHI instructions with updated mapping
            for (auto* orig_block : loop->loop_nodes)
            {
                auto* new_block = block_map[orig_block];

                for (auto* inst : orig_block->insts)
                {
                    if (orig_block == header && inst->opcode == LLVMIR::IROpCode::PHI) continue;
                    auto* cloned_inst = inst->CloneWithMapping(combined_reg_map, label_map);
                    new_block->insts.push_back(cloned_inst);
                    cloned_inst->block_id = new_block->block_id;
                }
            }

            for (const auto& [orig_reg, new_reg] : reg_map) last_value_map[orig_reg] = getRegOperand(new_reg);

            for (auto* orig_block : loop->loop_nodes)
            {
                auto* new_block = block_map[orig_block];

                std::vector<LLVMIR::IRBlock*> successors;
                if (!orig_block->insts.empty())
                {
                    auto* terminator = orig_block->insts.back();
                    if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
                    {
                        auto* br       = static_cast<LLVMIR::BranchUncondInst*>(terminator);
                        auto* label_op = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                        auto  it       = cfg->block_id_to_block.find(label_op->label_num);
                        if (it != cfg->block_id_to_block.end()) successors.push_back(it->second);
                    }
                    else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
                    {
                        auto* br          = static_cast<LLVMIR::BranchCondInst*>(terminator);
                        auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                        auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);

                        auto it_true = cfg->block_id_to_block.find(true_label->label_num);
                        if (it_true != cfg->block_id_to_block.end()) successors.push_back(it_true->second);

                        auto it_false = cfg->block_id_to_block.find(false_label->label_num);
                        if (it_false != cfg->block_id_to_block.end()) successors.push_back(it_false->second);
                    }
                }

                for (auto* succ : successors)
                {
                    if (loop->loop_nodes.find(succ) != loop->loop_nodes.end()) continue;

                    for (auto* inst : succ->insts)
                    {
                        if (inst->opcode != LLVMIR::IROpCode::PHI) continue;

                        auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

                        LLVMIR::Operand* incoming = nullptr;
                        for (const auto& [val, label] : phi->vals_for_labels)
                        {
                            auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                            if (label_op->label_num == orig_block->block_id)
                            {
                                incoming = val;
                                break;
                            }
                        }

                        if (incoming)
                        {
                            if (incoming->type == LLVMIR::OperandType::REG)
                            {
                                auto* reg_op = static_cast<LLVMIR::RegOperand*>(incoming);
                                auto  it     = combined_reg_map.find(reg_op->reg_num);
                                if (it != combined_reg_map.end()) incoming = getRegOperand(it->second);
                            }

                            phi->vals_for_labels.emplace_back(incoming, getLabelOperand(new_block->block_id));
                        }
                    }
                }
            }

            if (auto it = block_map.find(header); it != block_map.end()) headers.push_back(it->second);
            if (auto it = block_map.find(latch); it != block_map.end()) latches.push_back(it->second);
        }

        // Step 4: Handle original PHI nodes for complete unroll
        for (auto* phi : orig_phi_nodes)
        {
            LLVMIR::Operand* preheader_value = nullptr;
            for (const auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                if (label_op->label_num == preheader->block_id)
                {
                    preheader_value = val;
                    break;
                }
            }

            if (preheader_value)
            {
                std::map<int, LLVMIR::Operand*> substitutions;
                substitutions[phi->GetResultReg()] = preheader_value;

                for (auto& [_, block] : cfg->block_id_to_block)
                {
                    for (auto* inst : block->insts)
                    {
                        if (inst == phi) continue;
                        inst->SubstituteOperands(substitutions);
                    }
                }

                auto it = std::find(header->insts.begin(), header->insts.end(), phi);
                if (it != header->insts.end())
                {
                    header->insts.erase(it);
                    delete phi;
                }
            }
        }

        // Step 5: Optimize conditional branches in completely unrolled loop

        if (trip_count > 1)
        {
            for (size_t i = 0; i < latches.size(); ++i)
            {
                auto* latch_block       = latches[i];
                bool  is_last_iteration = (i == latches.size() - 1);

                for (auto* inst : latch_block->insts)
                {
                    if (inst->opcode != LLVMIR::IROpCode::BR_COND) continue;

                    auto* br          = static_cast<LLVMIR::BranchCondInst*>(inst);
                    auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                    auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);

                    LLVMIR::LabelOperand* target = nullptr;

                    if (is_last_iteration)
                        target = false_label;
                    else
                    {
                        size_t next_iteration = i + 1;
                        if (next_iteration < headers.size())
                            target = getLabelOperand(headers[next_iteration]->block_id);
                    }

                    if (target)
                    {
                        auto* new_br     = new LLVMIR::BranchUncondInst(target);
                        new_br->opcode   = LLVMIR::IROpCode::BR_UNCOND;
                        new_br->block_id = latch_block->block_id;

                        auto it = std::find(latch_block->insts.begin(), latch_block->insts.end(), inst);
                        if (it != latch_block->insts.end())
                        {
                            *it = new_br;
                            delete br;
                        }
                    }
                    break;
                }
            }
        }

        // Step 6: Clean up PHI nodes in exit blocks after branch optimization
        std::set<LLVMIR::IRBlock*> exit_blocks;
        for (auto* orig_block : loop->loop_nodes)
        {
            if (orig_block->insts.empty()) continue;

            auto* terminator = orig_block->insts.back();
            if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
            {
                auto* br       = static_cast<LLVMIR::BranchUncondInst*>(terminator);
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                auto  it       = cfg->block_id_to_block.find(label_op->label_num);
                if (it != cfg->block_id_to_block.end())
                {
                    auto* succ = it->second;
                    if (loop->loop_nodes.find(succ) == loop->loop_nodes.end()) exit_blocks.insert(succ);
                }
            }
            else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* br          = static_cast<LLVMIR::BranchCondInst*>(terminator);
                auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);

                auto true_it = cfg->block_id_to_block.find(true_label->label_num);
                if (true_it != cfg->block_id_to_block.end())
                {
                    auto* succ = true_it->second;
                    if (loop->loop_nodes.find(succ) == loop->loop_nodes.end()) exit_blocks.insert(succ);
                }

                auto false_it = cfg->block_id_to_block.find(false_label->label_num);
                if (false_it != cfg->block_id_to_block.end())
                {
                    auto* succ = false_it->second;
                    if (loop->loop_nodes.find(succ) == loop->loop_nodes.end()) exit_blocks.insert(succ);
                }
            }
        }

        for (size_t i = 0; i < latches.size(); ++i)
        {
            auto* latch_block = latches[i];
            if (latch_block->insts.empty()) continue;

            auto* terminator = latch_block->insts.back();
            if (terminator->opcode != LLVMIR::IROpCode::BR_UNCOND) continue;

            auto* br       = static_cast<LLVMIR::BranchUncondInst*>(terminator);
            auto* label_op = static_cast<LLVMIR::LabelOperand*>(br->target_label);
            auto  it       = cfg->block_id_to_block.find(label_op->label_num);
            if (it != cfg->block_id_to_block.end())
            {
                auto* succ = it->second;
                if (loop->loop_nodes.find(succ) == loop->loop_nodes.end()) exit_blocks.insert(succ);
            }
        }

        for (auto* exit_block : exit_blocks)
        {
            for (auto* inst : exit_block->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::PHI) continue;

                auto*                                                      phi = static_cast<LLVMIR::PhiInst*>(inst);
                std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> valid_entries;

                for (const auto& [val, label] : phi->vals_for_labels)
                {
                    auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                    auto  pred_it  = cfg->block_id_to_block.find(label_op->label_num);

                    if (pred_it == cfg->block_id_to_block.end()) continue;

                    auto* pred_block = pred_it->second;

                    bool actually_jumps_here = false;
                    if (!pred_block->insts.empty())
                    {
                        auto* pred_terminator = pred_block->insts.back();
                        if (pred_terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
                        {
                            auto* br           = static_cast<LLVMIR::BranchUncondInst*>(pred_terminator);
                            auto* target_label = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                            if (target_label->label_num == exit_block->block_id) actually_jumps_here = true;
                        }
                        else if (pred_terminator->opcode == LLVMIR::IROpCode::BR_COND)
                        {
                            auto* br          = static_cast<LLVMIR::BranchCondInst*>(pred_terminator);
                            auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                            auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);
                            if (true_label->label_num == exit_block->block_id ||
                                false_label->label_num == exit_block->block_id)
                                actually_jumps_here = true;
                        }
                    }

                    if (actually_jumps_here) valid_entries.push_back({val, label});
                }

                phi->vals_for_labels = valid_entries;
            }
        }

        // Step 7: Connect latches to next iteration headers (LLVM line 843-847)

        for (size_t i = 0; i < latches.size(); ++i)
        {
            size_t j = (i + 1) % latches.size();

            for (auto* inst : latches[i]->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::BR_COND) continue;

                auto* br          = static_cast<LLVMIR::BranchCondInst*>(inst);
                auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                auto* false_label = static_cast<LLVMIR::LabelOperand*>(br->false_label);

                if (true_label->label_num == headers[i]->block_id)
                    br->true_label = getLabelOperand(headers[j]->block_id);
                if (false_label->label_num == headers[i]->block_id)
                    br->false_label = getLabelOperand(headers[j]->block_id);

                break;
            }
        }

        return true;
    }

    void LoopFullUnrollPass::logResult(NaturalLoop* loop, bool success, const std::string& reason) const
    {
#ifdef DBGMODE
        std::cout << "Loop " << loop->loop_id << ": " << (success ? "UNROLLED" : "SKIPPED") << " - " << reason
                  << std::endl;
#endif
    }

}  // namespace Transform
