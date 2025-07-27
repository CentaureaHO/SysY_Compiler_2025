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
            ++loops_modified;
        };

        for (auto* loop : cfg->LoopForest->getRootLoops()) processLoop(cfg, *cfg->LoopForest, loop);

        eliminateUselessPhi(cfg);
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

                    // std::cout << "    SIMPLIFY_CHECK: PHI reg_" << phi->GetResultReg() << " in Block" << bb->block_id
                    //           << " has " << phi->vals_for_labels.size() << " entries" << std::endl;

                    bool             should_eliminate = false;
                    LLVMIR::Operand* replacement_val  = nullptr;

                    if (phi->vals_for_labels.size() == 1)
                    {
                        should_eliminate = true;
                        replacement_val  = phi->vals_for_labels[0].first;
                        // std::cout << "      SIMPLIFY_ELIMINATE: single entry PHI" << std::endl;
                    }
                    else if (phi->vals_for_labels.size() > 1)
                    {
                        auto* first_val = phi->vals_for_labels[0].first;

                        should_eliminate = std::all_of(phi->vals_for_labels.begin(),
                            phi->vals_for_labels.end(),
                            [first_val](const auto& pair) { return pair.first == first_val; });

                        if (should_eliminate)
                        {
                            replacement_val = first_val;
                            // std::cout << "      SIMPLIFY_ELIMINATE: all values identical" << std::endl;
                        }
                    }

                    if (should_eliminate && replacement_val) { phi_to_eliminate.push_back({phi, replacement_val}); }

                    ++it;
                }

                for (const auto& [phi, replacement_val] : phi_to_eliminate)
                {
                    /* std::cout << "      SIMPLIFY_REPLACING: reg_" << phi->GetResultReg() << " with ";
                    if (auto* reg_val = dynamic_cast<LLVMIR::RegOperand*>(replacement_val))
                        std::cout << "reg_" << reg_val->reg_num;
                    else std::cout << "immediate";
                    std::cout << std::endl;
                    */

                    std::map<int, int>   replace_map;
                    LLVMIR::Instruction* new_inst = nullptr;

                    if (auto* reg_val = dynamic_cast<LLVMIR::RegOperand*>(replacement_val))
                    {
                        replace_map[phi->GetResultReg()] = reg_val->reg_num;
                        // std::cout << "      SIMPLIFY_REPLACE_REG: reg_" << phi->GetResultReg() << " -> reg_"
                        //           << reg_val->reg_num << std::endl;
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

                        // std::cout << "      SIMPLIFY_REPLACE_IMM: reg_" << phi->GetResultReg() << " -> "
                        //           << imm_i32->value << " (via reg_" << new_reg << ")" << std::endl;
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

                        // std::cout << "      SIMPLIFY_REPLACE_FLOAT: reg_" << phi->GetResultReg() << " -> "
                        //           << imm_f32->value << " (via reg_" << new_reg << ")" << std::endl;
                    }
                    else
                    {
                        // std::cout << "      SIMPLIFY_SKIP: unsupported operand type" << std::endl;
                        continue;
                    }

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
                        if (new_inst)
                        {
                            *phi_it = new_inst;
                            // std::cout << "      SIMPLIFY_REPLACED: PHI reg_" << phi->GetResultReg()
                            //           << " with assignment instruction" << std::endl;
                        }
                        else
                        {
                            bb->insts.erase(phi_it);
                            // std::cout << "      SIMPLIFY_DELETED: PHI reg_" << phi->GetResultReg() << std::endl;
                        }

                        delete phi;
                        changed = true;
                    }
                }
            }
        }
    }

}  // namespace StructuralTransform
