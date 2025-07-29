#include "llvm/loop/loop_simplify.h"
#include "llvm_ir/ir_builder.h"
#include <algorithm>
#include <cassert>
#include <stack>
#include <functional>

namespace StructuralTransform
{
    LoopSimplifyPass::LoopSimplifyPass(LLVMIR::IR* ir) : Pass(ir) {}

    void LoopSimplifyPass::Execute()
    {
        loops_processed = 0;
        loops_modified  = 0;

        for (const auto& [func_def, cfg] : ir->cfg) loopSimplify(cfg);
    }

    void LoopSimplifyPass::loopSimplify(CFG* cfg)
    {
        if (!cfg || !cfg->LoopForest) return;

        // Clear existing comments
        for (const auto& [id, bb] : cfg->block_id_to_block) { bb->comment = ""; }

        // Add loop structure comments
        addLoopStructureComments(cfg);

        std::function<void(CFG*, NaturalLoopForest&, NaturalLoop*)> processLoop =
            [&](CFG* config, NaturalLoopForest& loop_forest, NaturalLoop* loop) -> void {
            ++loops_processed;

            if (static_cast<size_t>(loop->loop_id) < loop_forest.loopG.size())
                for (auto* child_loop : loop_forest.loopG[loop->loop_id]) processLoop(config, loop_forest, child_loop);

            if (isLoopSimplified(loop, config)) return;

            loop->simplify(config);
            // simplifyExitNodes(loop, config);

            ++loops_modified;
        };

        for (auto* loop : cfg->LoopForest->getRootLoops()) processLoop(cfg, *cfg->LoopForest, loop);

        cleanupInvalidPhiReferences(cfg);
        eliminateUselessPhi(cfg);

        // cfg->BuildCFG();
    }

    bool LoopSimplifyPass::isLoopSimplified(NaturalLoop* loop, CFG* cfg) const
    {
        if (!loop || !cfg) return false;

        return loop->isSimplified(cfg);
    }

    std::pair<int, int> LoopSimplifyPass::getSimplificationStats() const { return {loops_processed, loops_modified}; }

    void LoopSimplifyPass::addLoopStructureComments(CFG* cfg)
    {
        if (!cfg || !cfg->LoopForest) return;

        // Helper to get depth of a loop
        auto getLoopDepth = [](NaturalLoop* loop) -> int {
            int depth = 0;
            for (auto* current = loop->fa_loop; current != nullptr; current = current->fa_loop) depth++;
            return depth;
        };

        // Add comments for all loop blocks
        for (auto* loop : cfg->LoopForest->loop_set)
        {
            int         depth      = getLoopDepth(loop);
            std::string depth_info = (depth > 0) ? " (nested depth: " + std::to_string(depth) + ")" : "";

            // Mark header
            if (loop->header && loop->header->comment.empty())
            {
                loop->header->comment = "Loop " + std::to_string(loop->loop_id) + " header" + depth_info;
            }

            // Mark preheader if exists
            if (loop->preheader && loop->preheader->comment.empty())
            {
                loop->preheader->comment = "Loop " + std::to_string(loop->loop_id) + " preheader" + depth_info;
            }

            // Mark latches
            for (auto* latch : loop->latches)
            {
                if (latch && latch->comment.empty())
                {
                    if (loop->latches.size() == 1)
                        latch->comment = "Loop " + std::to_string(loop->loop_id) + " latch" + depth_info;
                    else
                        latch->comment = "Loop " + std::to_string(loop->loop_id) + " latch (multiple)" + depth_info;
                }
            }

            // Mark exiting blocks
            for (auto* exiting : loop->exiting_nodes)
            {
                if (exiting && exiting->comment.empty())
                {
                    exiting->comment = "Loop " + std::to_string(loop->loop_id) + " exiting block" + depth_info;
                }
            }

            // Mark other loop body blocks
            for (auto* node : loop->loop_nodes)
            {
                if (node && node->comment.empty() && node != loop->header &&
                    loop->latches.find(node) == loop->latches.end() &&
                    loop->exiting_nodes.find(node) == loop->exiting_nodes.end())
                {
                    node->comment = "Loop " + std::to_string(loop->loop_id) + " body" + depth_info;
                }
            }
        }

        // Mark exit blocks (blocks outside loops that receive control from loop exits)
        for (auto* loop : cfg->LoopForest->loop_set)
        {
            int         depth      = getLoopDepth(loop);
            std::string depth_info = (depth > 0) ? " (nested depth: " + std::to_string(depth) + ")" : "";

            for (auto* exit : loop->exit_nodes)
            {
                if (exit && exit->comment.empty())
                {
                    exit->comment = "Loop " + std::to_string(loop->loop_id) + " exit target" + depth_info;
                }
            }
        }
    }

    void LoopSimplifyPass::eliminateUselessPhi(CFG* cfg)
    {
        if (!cfg) return;

        bool changed = true;
        while (changed)
        {
            changed = false;

            for (const auto& [id, bb] : cfg->block_id_to_block)
            {
                std::vector<std::pair<LLVMIR::PhiInst*, LLVMIR::Operand*>> phi_to_eliminate;

                auto it = bb->insts.begin();
                while (it != bb->insts.end())
                {
                    if ((*it)->opcode != LLVMIR::IROpCode::PHI) break;

                    auto* phi = dynamic_cast<LLVMIR::PhiInst*>(*it);
                    if (!phi)
                    {
                        ++it;
                        continue;
                    }

                    bool             should_eliminate = false;
                    LLVMIR::Operand* replacement_val  = nullptr;

                    if (phi->vals_for_labels.size() == 1)
                    {
                        should_eliminate = true;
                        replacement_val  = phi->vals_for_labels[0].first;
                    }
                    else if (phi->vals_for_labels.size() > 1)
                    {
                        auto* first_val = phi->vals_for_labels[0].first;

                        should_eliminate = std::all_of(phi->vals_for_labels.begin(),
                            phi->vals_for_labels.end(),
                            [first_val](const auto& pair) { return pair.first == first_val; });

                        if (should_eliminate) { replacement_val = first_val; }
                    }

                    if (should_eliminate && replacement_val) { phi_to_eliminate.push_back({phi, replacement_val}); }

                    ++it;
                }

                for (const auto& [phi, replacement_val] : phi_to_eliminate)
                {

                    std::map<int, int>   replace_map;
                    LLVMIR::Instruction* new_inst = nullptr;

                    if (auto* reg_val = dynamic_cast<LLVMIR::RegOperand*>(replacement_val))
                    {
                        replace_map[phi->GetResultReg()] = reg_val->reg_num;
                    }
                    else if (auto* imm_i32 = dynamic_cast<LLVMIR::ImmeI32Operand*>(replacement_val))
                    {
                        int new_reg                      = ++cfg->func->max_reg;
                        replace_map[phi->GetResultReg()] = new_reg;

                        new_inst          = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                            LLVMIR::DataType::I32,
                            getImmeI32Operand(imm_i32->value),
                            getImmeI32Operand(0),
                            getRegOperand(new_reg));
                        new_inst->comment = "SIMPLIFY_CREATED: immediate assignment for PHI elimination";
                    }
                    else if (auto* imm_f32 = dynamic_cast<LLVMIR::ImmeF32Operand*>(replacement_val))
                    {
                        int new_reg                      = ++cfg->func->max_reg;
                        replace_map[phi->GetResultReg()] = new_reg;

                        new_inst          = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::FADD,
                            LLVMIR::DataType::F32,
                            getImmeF32Operand(imm_f32->value),
                            getImmeF32Operand(0.0f),
                            getRegOperand(new_reg));
                        new_inst->comment = "SIMPLIFY_CREATED: float immediate assignment for PHI elimination";
                    }
                    else { continue; }

                    for (const auto& [block_id, block] : cfg->block_id_to_block)
                    {
                        for (auto* inst : block->insts)
                        {
                            if (inst != phi) { inst->Rename(replace_map); }
                        }
                    }

                    auto phi_it = std::find(bb->insts.begin(), bb->insts.end(), phi);
                    if (phi_it != bb->insts.end())
                    {
                        if (new_inst) { *phi_it = new_inst; }
                        else { bb->insts.erase(phi_it); }

                        delete phi;
                        changed = true;
                    }
                }
            }
        }

        for (auto& [id, bb] : cfg->block_id_to_block)
        {
            for (auto* inst : bb->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::PHI) break;

                auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
                if (!phi) continue;

                std::map<int, LLVMIR::Operand*> unique_entries;

                for (auto& [val, label] : phi->vals_for_labels)
                {
                    auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                    if (!label_op) continue;

                    int block_id = label_op->label_num;

                    if (unique_entries.find(block_id) != unique_entries.end())
                    {
                        auto* existing_val = unique_entries[block_id];
                        if (existing_val != val)
                        {
                            // 指针差异，如果出问题再留意此处
                            std::cout << "Warning: PHI reg_" << phi->GetResultReg()
                                      << " has different values from Block" << block_id << std::endl;
                        }
                    }
                    else { unique_entries[block_id] = val; }
                }

                phi->vals_for_labels.clear();
                for (auto& [block_id, val] : unique_entries)
                {
                    phi->vals_for_labels.push_back({val, getLabelOperand(block_id)});
                }
            }
        }
    }

    LLVMIR::IRBlock* LoopSimplifyPass::findUltimateTarget(LLVMIR::IRBlock* exit_node, CFG* cfg)
    {
        if (!exit_node || !cfg) return nullptr;

        std::set<LLVMIR::IRBlock*> visited;
        LLVMIR::IRBlock*           current = exit_node;

        while (current)
        {
            if (visited.find(current) != visited.end()) return current;
            visited.insert(current);

            if (current->insts.size() != 1) return current;

            auto* inst = current->insts[0];
            if (inst->opcode != LLVMIR::IROpCode::BR_UNCOND) return current;

            auto* br_uncond = dynamic_cast<LLVMIR::BranchUncondInst*>(inst);
            if (!br_uncond || !br_uncond->target_label) return current;

            auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(br_uncond->target_label);
            if (!label_op) return current;

            int  target_id = label_op->label_num;
            auto it        = cfg->block_id_to_block.find(target_id);
            if (it == cfg->block_id_to_block.end()) return current;

            current = it->second;
        }

        return exit_node;
    }

    void LoopSimplifyPass::simplifyExitNodes(NaturalLoop* loop, CFG* cfg)
    {
        if (!loop || !cfg || loop->exit_nodes.empty()) return;

        if (loop->exit_nodes.size() == 1) return;

        std::map<LLVMIR::IRBlock*, LLVMIR::IRBlock*> exit_to_ultimate;
        LLVMIR::IRBlock*                             common_ultimate = nullptr;
        bool                                         can_simplify    = true;

        for (auto* exit : loop->exit_nodes)
        {
            auto* ultimate = findUltimateTarget(exit, cfg);
            if (ultimate != exit)
            {
                exit_to_ultimate[exit] = ultimate;
                if (common_ultimate == nullptr)
                    common_ultimate = ultimate;
                else if (common_ultimate != ultimate)
                {
                    can_simplify = false;
                    break;
                }
            }
            else
            {
                can_simplify = false;
                break;
            }
        }

        if (!can_simplify || !common_ultimate || exit_to_ultimate.empty()) return;

        std::map<LLVMIR::IRBlock*, std::set<LLVMIR::IRBlock*>> exiting_to_exits;

        for (auto* exiting : loop->exiting_nodes)
        {
            if (exiting->insts.empty()) continue;

            auto*                         terminator = exiting->insts.back();
            std::vector<LLVMIR::IRBlock*> targets;

            if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
            {
                auto* br_uncond = dynamic_cast<LLVMIR::BranchUncondInst*>(terminator);
                if (br_uncond && br_uncond->target_label)
                {
                    auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(br_uncond->target_label);
                    if (label_op)
                    {
                        auto it = cfg->block_id_to_block.find(label_op->label_num);
                        if (it != cfg->block_id_to_block.end()) { targets.push_back(it->second); }
                    }
                }
            }
            else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* br_cond = dynamic_cast<LLVMIR::BranchCondInst*>(terminator);
                if (br_cond)
                {
                    auto* true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
                    auto* false_label = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

                    if (true_label)
                    {
                        auto it = cfg->block_id_to_block.find(true_label->label_num);
                        if (it != cfg->block_id_to_block.end()) { targets.push_back(it->second); }
                    }

                    if (false_label)
                    {
                        auto it = cfg->block_id_to_block.find(false_label->label_num);
                        if (it != cfg->block_id_to_block.end()) { targets.push_back(it->second); }
                    }
                }
            }

            for (auto* target : targets)
            {
                if (loop->exit_nodes.find(target) != loop->exit_nodes.end())
                {
                    exiting_to_exits[exiting].insert(target);
                }
            }
        }

        std::map<LLVMIR::IRBlock*, std::map<int, LLVMIR::Operand*>> final_values_to_transfer;

        for (auto& [exiting, exit_targets] : exiting_to_exits)
        {
            for (auto* exit : exit_targets)
            {
                if (exit_to_ultimate.find(exit) == exit_to_ultimate.end()) continue;

                auto* ultimate = exit_to_ultimate[exit];

                for (auto* inst : ultimate->insts)
                {
                    if (inst->opcode != LLVMIR::IROpCode::PHI) break;

                    auto* ultimate_phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
                    if (!ultimate_phi) continue;

                    LLVMIR::Operand* traced_value =
                        traceValueThroughChain(exiting, exit, ultimate, ultimate_phi->GetResultReg(), cfg);

                    if (traced_value)
                    {
                        final_values_to_transfer[exiting][ultimate_phi->GetResultReg()] = traced_value;
                    }
                }
            }
        }

        for (auto& [exiting, exit_targets] : exiting_to_exits)
        {
            if (exiting->insts.empty()) continue;

            auto* terminator = exiting->insts.back();

            if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
            {
                auto* br_uncond = dynamic_cast<LLVMIR::BranchUncondInst*>(terminator);
                if (br_uncond && br_uncond->target_label)
                {
                    auto* old_label = dynamic_cast<LLVMIR::LabelOperand*>(br_uncond->target_label);
                    if (old_label)
                    {
                        auto old_target_it = cfg->block_id_to_block.find(old_label->label_num);
                        if (old_target_it != cfg->block_id_to_block.end() &&
                            loop->exit_nodes.find(old_target_it->second) != loop->exit_nodes.end())
                        {
                            br_uncond->target_label = getLabelOperand(common_ultimate->block_id);
                        }
                    }
                }
            }
            else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* br_cond = dynamic_cast<LLVMIR::BranchCondInst*>(terminator);
                if (br_cond)
                {
                    auto* true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
                    auto* false_label = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

                    if (true_label)
                    {
                        auto true_target_it = cfg->block_id_to_block.find(true_label->label_num);
                        if (true_target_it != cfg->block_id_to_block.end() &&
                            loop->exit_nodes.find(true_target_it->second) != loop->exit_nodes.end())
                        {
                            br_cond->true_label = getLabelOperand(common_ultimate->block_id);
                        }
                    }

                    if (false_label)
                    {
                        auto false_target_it = cfg->block_id_to_block.find(false_label->label_num);
                        if (false_target_it != cfg->block_id_to_block.end() &&
                            loop->exit_nodes.find(false_target_it->second) != loop->exit_nodes.end())
                        {
                            br_cond->false_label = getLabelOperand(common_ultimate->block_id);
                        }
                    }
                }
            }
        }

        for (auto* inst : common_ultimate->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
            if (!phi) continue;

            std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> new_vals_for_labels;
            std::set<LLVMIR::IRBlock*>                                 processed_exitings;

            for (auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                if (!label_op) continue;

                int  block_id = label_op->label_num;
                auto block_it = cfg->block_id_to_block.find(block_id);
                if (block_it == cfg->block_id_to_block.end()) continue;

                auto* block = block_it->second;

                if (exit_to_ultimate.find(block) == exit_to_ultimate.end())
                {
                    new_vals_for_labels.push_back({val, label});
                }
            }

            for (auto& [exiting, phi_map] : final_values_to_transfer)
            {
                auto phi_val_it = phi_map.find(phi->GetResultReg());
                if (phi_val_it != phi_map.end())
                {
                    new_vals_for_labels.push_back({phi_val_it->second, getLabelOperand(exiting->block_id)});
                    processed_exitings.insert(exiting);
                }
            }

            phi->vals_for_labels = new_vals_for_labels;
        }
    }

    LLVMIR::Operand* LoopSimplifyPass::traceValueThroughChain(
        LLVMIR::IRBlock* exiting, LLVMIR::IRBlock* exit, LLVMIR::IRBlock* ultimate, int phi_reg, CFG* cfg)
    {
        LLVMIR::PhiInst* ultimate_phi = nullptr;
        for (auto* inst : ultimate->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
            if (phi && phi->GetResultReg() == phi_reg)
            {
                ultimate_phi = phi;
                break;
            }
        }

        if (!ultimate_phi) { return nullptr; }

        LLVMIR::Operand* value_from_exit = nullptr;
        for (auto& [val, label] : ultimate_phi->vals_for_labels)
        {
            auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
            if (label_op && label_op->label_num == exit->block_id)
            {
                value_from_exit = val;
                break;
            }
        }

        if (!value_from_exit) { return nullptr; }

        if (exit->insts.size() == 1 && exit->insts[0]->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* reg_val = dynamic_cast<LLVMIR::RegOperand*>(value_from_exit);
            if (reg_val)
            {
                int search_reg = reg_val->reg_num;

                for (auto* inst : exit->insts)
                {
                    if (inst->opcode != LLVMIR::IROpCode::PHI) break;

                    auto* exit_phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
                    if (exit_phi && exit_phi->GetResultReg() == search_reg)
                    {
                        for (auto& [exit_val, exit_label] : exit_phi->vals_for_labels)
                        {
                            auto* exit_label_op = dynamic_cast<LLVMIR::LabelOperand*>(exit_label);
                            if (exit_label_op && exit_label_op->label_num == exiting->block_id) { return exit_val; }
                        }
                    }
                }
            }
        }

        return value_from_exit;
    }

    void LoopSimplifyPass::cleanupInvalidPhiReferences(CFG* cfg)
    {
        if (!cfg) return;

        for (auto& [id, bb] : cfg->block_id_to_block)
        {
            for (auto* inst : bb->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::PHI) break;

                auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
                if (!phi) continue;

                std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> valid_entries;
                bool                                                       changed = false;

                for (auto& [val, label] : phi->vals_for_labels)
                {
                    auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                    if (!label_op) continue;

                    int  pred_id = label_op->label_num;
                    auto pred_it = cfg->block_id_to_block.find(pred_id);

                    if (pred_it == cfg->block_id_to_block.end())
                    {
                        changed = true;
                        continue;
                    }

                    auto* pred_block = pred_it->second;

                    bool is_valid_predecessor = false;

                    if (!pred_block->insts.empty())
                    {
                        auto* terminator = pred_block->insts.back();

                        if (terminator->opcode == LLVMIR::IROpCode::BR_UNCOND)
                        {
                            auto* br_uncond = dynamic_cast<LLVMIR::BranchUncondInst*>(terminator);
                            if (br_uncond && br_uncond->target_label)
                            {
                                auto* target_label = dynamic_cast<LLVMIR::LabelOperand*>(br_uncond->target_label);
                                if (target_label && target_label->label_num == bb->block_id)
                                {
                                    is_valid_predecessor = true;
                                }
                            }
                        }
                        else if (terminator->opcode == LLVMIR::IROpCode::BR_COND)
                        {
                            auto* br_cond = dynamic_cast<LLVMIR::BranchCondInst*>(terminator);
                            if (br_cond)
                            {
                                auto* true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
                                auto* false_label = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

                                if ((true_label && true_label->label_num == bb->block_id) ||
                                    (false_label && false_label->label_num == bb->block_id))
                                {
                                    is_valid_predecessor = true;
                                }
                            }
                        }
                    }

                    if (is_valid_predecessor) { valid_entries.push_back({val, label}); }
                    else { changed = true; }
                }

                if (changed) { phi->vals_for_labels = valid_entries; }
            }
        }
    }

}  // namespace StructuralTransform
