#include "llvm/loop/loop_partial_unroll.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>

// #define DBGMODE
// #define INSERT_PUTCH_CALLS

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
    LoopPartialUnrollPass::LoopPartialUnrollPass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev)
        : Pass(ir), scev_analyser_(scev), loops_processed_(0), loops_partially_unrolled_(0)
    {}

    void LoopPartialUnrollPass::Execute()
    {
        unroll_factors_.clear();
        DBGINFO("Starting Loop Partial Unroll Pass");
        processAllLoops();
        DBGINFO("Loop Partial Unroll Pass completed. Processed: ",
            loops_processed_,
            ", Unrolled: ",
            loops_partially_unrolled_);
    }

    void LoopPartialUnrollPass::processAllLoops()
    {
        for (const auto& [func_def, cfg] : ir->cfg)
            if (cfg && cfg->LoopForest && !cfg->LoopForest->loop_set.empty()) processFunction(cfg);
    }

    void LoopPartialUnrollPass::processFunction(CFG* cfg)
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

    void LoopPartialUnrollPass::processLoop(CFG* cfg, NaturalLoop* loop)
    {
        scev_analyser_->analyzeSingleLoop(loop);
        tryPartialUnrollLoop(cfg, loop);
    }

    bool LoopPartialUnrollPass::tryPartialUnrollLoop(CFG* cfg, NaturalLoop* loop)
    {
        ++loops_processed_;

        if (!isSimpleLoop(loop))
        {
            logResult(loop, false, 0, "Not a simple loop");
            return false;
        }

        if (!canPartialUnroll(loop))
        {
            logResult(loop, false, 0, "Cannot partial unroll");
            return false;
        }

        if (!isBoundaryTypeSupported(loop))
        {
            logResult(loop, false, 0, "Unsupported boundary type (CHAIN_OF_RECURRENCES or ARITHMETIC_EXPR)");
            return false;
        }

        int global_insts = getGlobalInstructionCount();
        if (global_insts >= MAX_GLOBAL_INSTRUCTIONS)
        {
            logResult(loop,
                false,
                0,
                "Too many global instructions (" + std::to_string(global_insts) +
                    " >= " + std::to_string(MAX_GLOBAL_INSTRUCTIONS) + ")");
            return false;
        }

        int unroll_factor = selectUnrollFactor(loop);
        if (unroll_factor <= 1)
        {
            logResult(loop, false, unroll_factor, "Unroll factor too small");
            return false;
        }

        if (!isUnrollProfitable(loop, unroll_factor))
        {
            int loop_size = getLoopSize(loop);
            logResult(loop,
                false,
                unroll_factor,
                "Not profitable (size=" + std::to_string(loop_size) + " * factor=" + std::to_string(unroll_factor) +
                    " = " + std::to_string(loop_size * unroll_factor) + " > " +
                    std::to_string(MAX_UNROLLED_INSTRUCTIONS) + ")");
            return false;
        }

        if (performPartialUnroll(cfg, loop, unroll_factor))
        {
            ++loops_partially_unrolled_;
            logResult(loop,
                true,
                unroll_factor,
                "Successfully unrolled (blocks=" + std::to_string(loop->loop_nodes.size()) +
                    ", factor=" + std::to_string(unroll_factor) + ")");
            return true;
        }
        else
        {
            logResult(loop, false, unroll_factor, "Unroll implementation failed");
            return false;
        }
    }

    bool LoopPartialUnrollPass::isSimpleLoop(NaturalLoop* loop) const
    {
        if (!loop) return false;

        auto* info = scev_analyser_->getLoopInfo(loop);
        if (!info) return false;

        if (!info->is_simple_loop) return false;

        if (loop->exit_nodes.size() != 1) return false;
        if (loop->exiting_nodes.size() != 1) return false;
        if (loop->latches.size() != 1) return false;
        if (!loop->preheader) return false;

        auto* latch   = *loop->latches.begin();
        auto* exiting = *loop->exiting_nodes.begin();
        if (latch != exiting)
        {
            DBGINFO("Not a do-while loop: latch Block", latch->block_id, " != exiting Block", exiting->block_id);
            return false;
        }

        auto step_val = info->loop_info.step.getConstantValue();
        if (!step_val || *step_val != 1) return false;

        if (info->loop_info.cond != LLVMIR::IcmpCond::SLE && info->loop_info.cond != LLVMIR::IcmpCond::SLT)
            return false;

        DBGINFO("Confirmed do-while loop: latch/exiting Block", latch->block_id);
        return true;
    }

    bool LoopPartialUnrollPass::canPartialUnroll(NaturalLoop* loop) const
    {
        if (loop->loop_nodes.size() > MAX_LOOP_BLOCKS) { return false; }

        int loop_size = getLoopSize(loop);
        if (loop_size > MAX_INST_PER_ITERATION) { return false; }

        return true;
    }

    bool LoopPartialUnrollPass::isBoundaryTypeSupported(NaturalLoop* loop) const
    {
        auto* info = scev_analyser_->getLoopInfo(loop);
        if (!info) { return false; }

        const auto& lowerbound = info->loop_info.lowerbound;
        if (lowerbound.type == Analysis::CROperand::CHAIN_OF_RECURRENCES ||
            lowerbound.type == Analysis::CROperand::ARITHMETIC_EXPR)
        {
            DBGINFO("Lower bound has unsupported type: ", lowerbound.type);
            return false;
        }

        const auto& upperbound = info->loop_info.upperbound;
        if (upperbound.type == Analysis::CROperand::CHAIN_OF_RECURRENCES ||
            upperbound.type == Analysis::CROperand::ARITHMETIC_EXPR)
        {
            DBGINFO("Upper bound has unsupported type: ", upperbound.type);
            return false;
        }

        return true;
    }

    bool LoopPartialUnrollPass::isUnrollProfitable(NaturalLoop* loop, int unroll_factor) const
    {
        int loop_size = getLoopSize(loop);
        return loop_size * unroll_factor <= MAX_UNROLLED_INSTRUCTIONS;
    }

    int LoopPartialUnrollPass::selectUnrollFactor(NaturalLoop* loop) const
    {
        int  loop_size = getLoopSize(loop);
        bool has_array = hasArrayAccess(loop);

        int unroll_factor = 4;

        if (loop_size <= 20 && has_array) { unroll_factor = 8; }

        unroll_factor = std::min(unroll_factor, MAX_UNROLL_FACTOR);

        while (unroll_factor > 1 && loop_size * unroll_factor > MAX_UNROLLED_INSTRUCTIONS) { unroll_factor /= 2; }

        return unroll_factor;
    }

    bool LoopPartialUnrollPass::performPartialUnroll(CFG* cfg, NaturalLoop* loop, int unroll_factor)
    {
        DBGINFO("Performing partial unroll with factor ", unroll_factor, " for loop ", loop->loop_id);

        UnrollContext ctx;
        ctx.cfg                  = cfg;
        ctx.loop                 = loop;
        ctx.unroll_factor        = unroll_factor;
        ctx.preheader            = loop->preheader;
        ctx.header               = loop->header;
        ctx.latch                = *loop->latches.begin();
        ctx.exiting              = *loop->exiting_nodes.begin();
        ctx.exit                 = *loop->exit_nodes.begin();
        ctx.cond_block           = nullptr;
        ctx.new_preheader        = nullptr;
        ctx.remain_preheader     = nullptr;
        ctx.final_unrolled_latch = nullptr;

        ctx.loop_info = scev_analyser_->getLoopInfo(loop);
        if (!ctx.loop_info)
        {
            DBGINFO("Failed to get loop info");
            return false;
        }

        if (!checkLoopBoundsDominance(ctx))
        {
            DBGINFO("Loop bounds depend on loop-internal values, skipping partial unroll");
            return false;
        }

        ctx.original_condition   = nullptr;
        ctx.original_branch      = nullptr;
        ctx.adjusted_upper_bound = nullptr;

        for (auto* inst : ctx.latch->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::BR_COND)
            {
                ctx.original_branch = static_cast<LLVMIR::BranchCondInst*>(inst);
                break;
            }
        }

        if (ctx.original_branch)
        {
            for (auto* inst : ctx.latch->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ICMP)
                {
                    auto* icmp = static_cast<LLVMIR::IcmpInst*>(inst);
                    if (icmp->GetResultOperand() == ctx.original_branch->cond)
                    {
                        ctx.original_condition = icmp;
                        break;
                    }
                }
            }
        }

        if (!initializePhiInfos(ctx))
        {
            DBGINFO("Failed to initialize PHI infos");
            return false;
        }

        if (!setupUnrollEntryInPreheader(ctx))
        {
            DBGINFO("Failed to setup unroll entry in preheader");
            return false;
        }

        if (!createRemainderLoop(ctx))
        {
            DBGINFO("Failed to create remainder loop");
            return false;
        }

        if (!updatePhiNodesForNewPreheader(ctx))
        {
            DBGINFO("Failed to update PHI nodes for new preheader");
            return false;
        }

        if (!unrollLoopBodies(ctx))
        {
            DBGINFO("Failed to unroll loop bodies");
            return false;
        }

        if (!connectUnrolledLoop(ctx))
        {
            DBGINFO("Failed to connect unrolled loop");
            return false;
        }

        unroll_factors_[loop] = unroll_factor;
        DBGINFO("Successfully performed partial unroll with factor ", unroll_factor);
        return true;
    }

    bool LoopPartialUnrollPass::setupUnrollEntryInPreheader(UnrollContext& ctx)
    {
        DBGINFO("Setting up unroll entry in preheader");

        auto* preheader = ctx.preheader;

        ctx.new_preheader          = ctx.cfg->createBlock();
        ctx.new_preheader->comment = "New preheader for unrolled loop " + std::to_string(ctx.loop->loop_id);

        ctx.remain_preheader          = ctx.cfg->createBlock();
        ctx.remain_preheader->comment = "Preheader for remainder loop " + std::to_string(ctx.loop->loop_id);

        assert(!preheader->insts.empty() && "Preheader cannot be empty");
        if (preheader->insts.back()->isTerminator())
        {
            delete preheader->insts.back();
            preheader->insts.pop_back();
        }

        auto get_operand = [&](const Analysis::CROperand& cr_op) -> LLVMIR::Operand* {
            if (cr_op.type == Analysis::CROperand::CONSTANT) { return getImmeI32Operand(cr_op.const_val); }

            auto* inst = cr_op.generateInstruction(ctx.cfg);
            if (inst)
            {
                inst->block_id = preheader->block_id;
                preheader->insts.push_back(inst);
                return inst->GetResultOperand();
            }

            if (cr_op.type == Analysis::CROperand::LLVM_OPERAND) { return cr_op.llvm_op; }
            return nullptr;
        };

        ctx.trip_count_lower_bound = get_operand(ctx.loop_info->loop_info.lowerbound);
        ctx.trip_count_upper_bound = get_operand(ctx.loop_info->loop_info.upperbound);
        assert(ctx.trip_count_lower_bound && ctx.trip_count_upper_bound && "Failed to get loop bounds operands");

        auto step_val = ctx.loop_info->loop_info.step.getConstantValue();
        int  step     = step_val ? *step_val : 1;

        int   bound_adjustment            = (ctx.unroll_factor - 1) * step;
        auto* adjusted_upper_bound_reg    = getRegOperand(++ctx.cfg->func->max_reg);
        auto* adjust_upper_bound_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::SUB,
            LLVMIR::DataType::I32,
            ctx.trip_count_upper_bound,
            getImmeI32Operand(bound_adjustment),
            adjusted_upper_bound_reg);
        adjust_upper_bound_inst->block_id = preheader->block_id;
        adjust_upper_bound_inst->comment  = "Adjusted upper bound for unrolled loop final condition";
        preheader->insts.push_back(adjust_upper_bound_inst);
        ctx.adjusted_upper_bound = adjusted_upper_bound_reg;

        auto* diff_reg      = getRegOperand(++ctx.cfg->func->max_reg);
        auto* diff_inst     = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::SUB,
            LLVMIR::DataType::I32,
            ctx.trip_count_upper_bound,
            ctx.trip_count_lower_bound,
            diff_reg);
        diff_inst->block_id = preheader->block_id;
        preheader->insts.push_back(diff_inst);

        auto* adjusted_diff_reg = getRegOperand(++ctx.cfg->func->max_reg);
        int   adjustment        = (ctx.loop_info->loop_info.cond == LLVMIR::IcmpCond::SLE) ? step : step - 1;
        auto* adjust_inst       = new LLVMIR::ArithmeticInst(
            LLVMIR::IROpCode::ADD, LLVMIR::DataType::I32, diff_reg, getImmeI32Operand(adjustment), adjusted_diff_reg);
        adjust_inst->block_id = preheader->block_id;
        preheader->insts.push_back(adjust_inst);

        LLVMIR::Operand* trip_count_op = adjusted_diff_reg;
        if (step != 1)
        {
            auto* trip_count_reg = getRegOperand(++ctx.cfg->func->max_reg);
            auto* div_inst       = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::DIV,
                LLVMIR::DataType::I32,
                adjusted_diff_reg,
                getImmeI32Operand(step),
                trip_count_reg);
            div_inst->block_id   = preheader->block_id;
            preheader->insts.push_back(div_inst);
            trip_count_op = trip_count_reg;
        }

        auto* cmp_reg  = getRegOperand(++ctx.cfg->func->max_reg);
        auto* cmp_inst = new LLVMIR::IcmpInst(
            LLVMIR::DataType::I32, LLVMIR::IcmpCond::SGE, trip_count_op, getImmeI32Operand(ctx.unroll_factor), cmp_reg);
        cmp_inst->block_id = preheader->block_id;
        preheader->insts.push_back(cmp_inst);

        auto* br_cond = new LLVMIR::BranchCondInst(
            cmp_reg, getLabelOperand(ctx.new_preheader->block_id), getLabelOperand(ctx.remain_preheader->block_id));
        br_cond->block_id = preheader->block_id;
        preheader->insts.push_back(br_cond);

        auto* br_to_header     = new LLVMIR::BranchUncondInst(getLabelOperand(ctx.header->block_id));
        br_to_header->block_id = ctx.new_preheader->block_id;
        ctx.new_preheader->insts.push_back(br_to_header);

        auto* placeholder_br     = new LLVMIR::BranchUncondInst(getLabelOperand(ctx.exit->block_id));
        placeholder_br->block_id = ctx.remain_preheader->block_id;
        ctx.remain_preheader->insts.push_back(placeholder_br);

        return true;
    }

    bool LoopPartialUnrollPass::updatePhiNodesForNewPreheader(UnrollContext& ctx)
    {
        DBGINFO("Updating PHI nodes for new preheader");

        int phi_index = 0;
        for (auto* inst : ctx.header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;
            auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

            DBGINFO("  Before: PHI has ", phi->vals_for_labels.size(), " entries");
            for (const auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                DBGINFO("    Entry from Block", label_op->label_num);
            }

            LLVMIR::Operand* initial_value = nullptr;
            if (phi_index < (int)ctx.phi_mappings.size() && ctx.phi_mappings[phi_index].preheader_incoming_value)
            {
                initial_value = ctx.phi_mappings[phi_index].preheader_incoming_value;
                DBGINFO("  Using original preheader value: ", initial_value->getName());
            }
            else
                assert(false && "Cannot find preheader incoming value for PHI node");

            phi->vals_for_labels.clear();
            phi->vals_for_labels.emplace_back(initial_value, getLabelOperand(ctx.new_preheader->block_id));
            DBGINFO("  Cleared and set single entry from new_preheader Block", ctx.new_preheader->block_id);

            DBGINFO("  After: PHI has ", phi->vals_for_labels.size(), " entries");
            for (const auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                DBGINFO("    Entry from Block", label_op->label_num);
            }

            phi_index++;
        }

        return true;
    }

    bool LoopPartialUnrollPass::unrollLoopBodies(UnrollContext& ctx)
    {
        // std::cout << "DEBUG: Entering unrollLoopBodies function" << std::endl;
        DBGINFO("Unrolling loop bodies with factor ", ctx.unroll_factor);

        LLVMIR::IRBlock*           old_header     = ctx.header;
        LLVMIR::IRBlock*           old_latch      = ctx.latch;
        std::set<LLVMIR::IRBlock*> old_loop_nodes = ctx.loop->loop_nodes;

        ValueMap last_value_map;

        for (auto* block : old_loop_nodes)
        {
            for (auto* inst : block->insts)
            {
                int result_reg = inst->GetResultReg();
                if (result_reg != -1) { last_value_map[result_reg] = getRegOperand(result_reg); }
            }
        }

        LLVMIR::IRBlock* prev_latch = ctx.latch;

        DBGINFO("About to start unroll loop with factor ",
            ctx.unroll_factor,
            ", will iterate ",
            ctx.unroll_factor - 1,
            " times");

        for (int iteration = 1; iteration < ctx.unroll_factor; ++iteration)
        {
            DBGINFO("=== Starting unroll iteration ", iteration, " ===");

            std::map<int, int>                           reg_map, label_map;
            std::map<LLVMIR::IRBlock*, LLVMIR::IRBlock*> block_map;
            std::set<LLVMIR::IRBlock*>                   new_loop_nodes;
            LLVMIR::IRBlock*                             new_header = nullptr;
            LLVMIR::IRBlock*                             new_latch  = nullptr;

            for (auto* blk : old_loop_nodes)
            {
                for (auto* inst : blk->insts)
                {
                    int result_reg = inst->GetResultReg();
                    if (result_reg != -1 && reg_map.find(result_reg) == reg_map.end())
                    {
                        reg_map[result_reg] = ++ctx.cfg->func->max_reg;
                    }
                }
            }

            for (auto* old_block : old_loop_nodes)
            {
                auto* new_block    = ctx.cfg->createBlock();
                new_block->comment = "Unroll iteration " + std::to_string(iteration);
                new_loop_nodes.insert(new_block);
                label_map[old_block->block_id] = new_block->block_id;
                block_map[old_block]           = new_block;

                if (old_block == old_header) new_header = new_block;
                if (old_block == old_latch) new_latch = new_block;
            }

            ValueMap vmap;

            for (auto* old_block : old_loop_nodes)
            {
                LLVMIR::IRBlock* new_block = block_map[old_block];
                for (auto* inst : old_block->insts)
                {
                    int   result_reg     = inst->GetResultReg();
                    auto* cloned_inst    = inst->CloneWithMapping(reg_map, label_map);
                    cloned_inst->comment = "Generated at line " + std::to_string(__LINE__) + " (unroll iteration " +
                                           std::to_string(iteration) + ")";
                    new_block->insts.push_back(cloned_inst);

                    if (result_reg != -1)
                    {
                        auto it = reg_map.find(result_reg);
                        if (it != reg_map.end()) { vmap[result_reg] = getRegOperand(it->second); }
                    }
                }
            }

            if (new_header)
            {
                std::vector<LLVMIR::PhiInst*> cloned_phis_to_remove;
                for (auto* inst : new_header->insts)
                {
                    if (inst->opcode != LLVMIR::IROpCode::PHI) break;
                    cloned_phis_to_remove.push_back(static_cast<LLVMIR::PhiInst*>(inst));
                }

                for (const auto& mapping : ctx.phi_mappings)
                {
                    LLVMIR::Operand* in_val = mapping.latch_incoming_value;
                    DBGINFO("  Processing PHI reg",
                        mapping.original_phi_result_reg,
                        " with latch value ",
                        in_val->getName());

                    if (in_val && in_val->type == LLVMIR::OperandType::REG)
                    {
                        auto* reg_op = static_cast<LLVMIR::RegOperand*>(in_val);
                        auto  it     = last_value_map.find(reg_op->reg_num);
                        if (it != last_value_map.end())
                        {
                            in_val = it->second;
                            DBGINFO("  Using value from LastValueMap for PHI reg",
                                mapping.original_phi_result_reg,
                                ": reg",
                                reg_op->reg_num,
                                " -> ",
                                in_val->getName());
                        }
                        else if (mapping.latch_def_inst)
                        {
                            DBGINFO(
                                "ERROR: Could not find value from previous iteration for PHI reg ", reg_op->reg_num);
                            assert(false && "Failed to find value from previous iteration. Check value mapping logic.");
                        }
                        else
                        {
                            DBGINFO("  No mapping found in LastValueMap for reg",
                                reg_op->reg_num,
                                " (assuming loop-invariant)");
                        }
                    }

                    if (in_val) { vmap[mapping.original_phi_result_reg] = in_val; }
                }

                for (auto* phi_to_remove : cloned_phis_to_remove)
                {
                    auto it = std::find(new_header->insts.begin(), new_header->insts.end(), phi_to_remove);
                    if (it != new_header->insts.end())
                    {
                        delete *it;
                        new_header->insts.erase(it);
                    }
                }
#ifdef INSERT_PUTCH_CALLS
                std::vector<LLVMIR::Instruction*> putch_insts;

                std::vector<int> chars = {80,
                    97,
                    114,
                    116,
                    105,
                    97,
                    108,
                    108,
                    121,
                    32,
                    117,
                    110,
                    114,
                    111,
                    108,
                    108,
                    101,
                    100,
                    32,
                    108,
                    111,
                    111,
                    112,
                    10};

                for (int char_code : chars)
                {
                    auto*                                                      arg  = getImmeI32Operand(char_code);
                    std::vector<std::pair<LLVMIR::DataType, LLVMIR::Operand*>> args = {{LLVMIR::DataType::I32, arg}};

                    auto* call_inst     = new LLVMIR::CallInst(LLVMIR::DataType::VOID, "putch", args, nullptr);
                    call_inst->block_id = new_header->block_id;
                    call_inst->comment  = "Debug output for partially unrolled loop";
                    putch_insts.push_back(call_inst);
                }

                auto phi_end = new_header->insts.begin();
                while (phi_end != new_header->insts.end() && (*phi_end)->opcode == LLVMIR::IROpCode::PHI) ++phi_end;

                new_header->insts.insert(phi_end, putch_insts.begin(), putch_insts.end());
#endif
            }

            std::map<int, LLVMIR::Operand*> substitution_map;
            for (const auto& [orig_reg, new_operand] : vmap)
            {
                auto it = reg_map.find(orig_reg);
                if (it != reg_map.end()) { substitution_map[it->second] = new_operand; }
                substitution_map[orig_reg] = new_operand;
            }

            for (auto* new_block : new_loop_nodes)
            {
                for (auto* inst : new_block->insts) { inst->SubstituteOperands(substitution_map); }
            }

            delete prev_latch->insts.back();
            prev_latch->insts.pop_back();
            auto* br_to_next    = new LLVMIR::BranchUncondInst(getLabelOperand(new_header->block_id));
            br_to_next->comment = "Generated at line " + std::to_string(__LINE__) + " (connect to next iteration)";
            prev_latch->insts.push_back(br_to_next);

            last_value_map = vmap;

            for (auto* new_block : new_loop_nodes) { ctx.loop->loop_nodes.insert(new_block); }

            prev_latch = new_latch;
            if (iteration == ctx.unroll_factor - 1) { ctx.final_unrolled_latch = new_latch; }
        }

        ctx.final_value_map = last_value_map;

        DBGINFO("Successfully unrolled loop bodies");
        return true;
    }

    bool LoopPartialUnrollPass::connectUnrolledLoop(UnrollContext& ctx)
    {
        DBGINFO("Connecting unrolled loop back to original header");

        if (!ctx.final_unrolled_latch->insts.empty())
        {
            auto* last_inst = ctx.final_unrolled_latch->insts.back();
            if (last_inst->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* br_cond      = static_cast<LLVMIR::BranchCondInst*>(last_inst);
                auto* true_target  = static_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
                auto* false_target = static_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

                auto step_val         = ctx.loop_info->loop_info.step.getConstantValue();
                int  step             = step_val ? *step_val : 1;
                int  bound_adjustment = (ctx.unroll_factor - 1) * step;

                auto* cond_op = br_cond->cond;
                if (cond_op->type == LLVMIR::OperandType::REG)
                {
                    auto*                cond_reg_op = static_cast<LLVMIR::RegOperand*>(cond_op);
                    LLVMIR::Instruction* def_inst    = nullptr;
                    for (auto* inst : ctx.final_unrolled_latch->insts)
                    {
                        if (inst->GetResultReg() == cond_reg_op->reg_num)
                        {
                            def_inst = inst;
                            break;
                        }
                    }

                    if (def_inst && def_inst->opcode == LLVMIR::IROpCode::ICMP)
                    {
                        auto* icmp_inst = static_cast<LLVMIR::IcmpInst*>(def_inst);

                        bool lhs_is_upper_bound = (icmp_inst->lhs == ctx.trip_count_upper_bound);
                        bool rhs_is_upper_bound = (icmp_inst->rhs == ctx.trip_count_upper_bound);

                        if (!lhs_is_upper_bound && !rhs_is_upper_bound)
                        {
                            if (ctx.trip_count_upper_bound->type == LLVMIR::OperandType::REG)
                            {
                                auto* upper_bound_reg   = static_cast<LLVMIR::RegOperand*>(ctx.trip_count_upper_bound);
                                auto  final_upper_bound = findValueInFinalIteration(ctx, ctx.trip_count_upper_bound);

                                lhs_is_upper_bound = (icmp_inst->lhs == final_upper_bound);
                                rhs_is_upper_bound = (icmp_inst->rhs == final_upper_bound);
                            }
                        }

                        if (icmp_inst->cond == LLVMIR::IcmpCond::SLE || icmp_inst->cond == LLVMIR::IcmpCond::SLT)
                        {
                            if (rhs_is_upper_bound)
                            {
                                icmp_inst->rhs = ctx.adjusted_upper_bound;
                                DBGINFO("  Replaced upper bound (rhs) with pre-computed adjusted upper bound");
                            }
                            else if (lhs_is_upper_bound)
                            {
                                icmp_inst->lhs = ctx.adjusted_upper_bound;
                                DBGINFO("  Replaced upper bound (lhs) with pre-computed adjusted upper bound");
                            }
                            else
                            {
                                icmp_inst->rhs = ctx.adjusted_upper_bound;
                                DBGINFO("  Default: replaced rhs with pre-computed adjusted upper bound");
                            }
                        }
                        else if (icmp_inst->cond == LLVMIR::IcmpCond::SGE || icmp_inst->cond == LLVMIR::IcmpCond::SGT)
                        {
                            if (lhs_is_upper_bound)
                            {
                                icmp_inst->lhs = ctx.adjusted_upper_bound;
                                DBGINFO("  Replaced upper bound (lhs) with pre-computed adjusted upper bound");
                            }
                            else if (rhs_is_upper_bound)
                            {
                                icmp_inst->rhs = ctx.adjusted_upper_bound;
                                DBGINFO("  Replaced upper bound (rhs) with pre-computed adjusted upper bound");
                            }
                            else
                            {
                                icmp_inst->lhs = ctx.adjusted_upper_bound;
                                DBGINFO("  Default: replaced lhs with pre-computed adjusted upper bound");
                            }
                        }
                    }
                }

                auto* intermediate_block = createGuardedIntermediateBlock(ctx);

                bool exit_in_true  = (true_target->label_num == ctx.exit->block_id);
                bool exit_in_false = (false_target->label_num == ctx.exit->block_id);

                if (exit_in_true)
                {
                    br_cond->true_label = getLabelOperand(intermediate_block->block_id);
                    DBGINFO("  Modified final latch: true branch -> intermediate Block", intermediate_block->block_id);

                    br_cond->false_label = getLabelOperand(ctx.header->block_id);
                    DBGINFO("  Modified final latch: false branch -> original header Block", ctx.header->block_id);
                }
                else if (exit_in_false)
                {
                    br_cond->false_label = getLabelOperand(intermediate_block->block_id);
                    DBGINFO("  Modified final latch: false branch -> intermediate Block", intermediate_block->block_id);

                    br_cond->true_label = getLabelOperand(ctx.header->block_id);
                    DBGINFO("  Modified final latch: true branch -> original header Block", ctx.header->block_id);
                }

                int phi_idx = 0;
                for (auto* inst : ctx.remainder_header->insts)
                {
                    if (inst->opcode != LLVMIR::IROpCode::PHI) break;
                    auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

                    if (phi_idx < (int)ctx.phi_mappings.size())
                    {
                        const auto&      mapping   = ctx.phi_mappings[phi_idx];
                        LLVMIR::Operand* final_val = findValueInFinalIteration(ctx, mapping.latch_incoming_value);
                        if (final_val)
                        {
                            phi->vals_for_labels.emplace_back(final_val, getLabelOperand(intermediate_block->block_id));
                            DBGINFO("  Added remainder PHI[", phi_idx, "] incoming from intermediate block");
                        }
                    }
                    phi_idx++;
                }

                updateExitBlockPhiNodes(ctx);
            }
            else
            {
                delete ctx.final_unrolled_latch->insts.back();
                ctx.final_unrolled_latch->insts.pop_back();

                LLVMIR::Operand*  cond_op   = nullptr;
                LLVMIR::IcmpInst* icmp_inst = nullptr;
                for (auto it = ctx.final_unrolled_latch->insts.rbegin(); it != ctx.final_unrolled_latch->insts.rend();
                     ++it)
                {
                    if ((*it)->opcode == LLVMIR::IROpCode::ICMP)
                    {
                        icmp_inst = static_cast<LLVMIR::IcmpInst*>(*it);
                        cond_op   = icmp_inst->GetResultOperand();
                        break;
                    }
                }

                if (icmp_inst)
                {
                    bool lhs_is_upper_bound = (icmp_inst->lhs == ctx.trip_count_upper_bound);
                    bool rhs_is_upper_bound = (icmp_inst->rhs == ctx.trip_count_upper_bound);

                    if (!lhs_is_upper_bound && !rhs_is_upper_bound)
                    {
                        if (ctx.trip_count_upper_bound->type == LLVMIR::OperandType::REG)
                        {
                            auto final_upper_bound = findValueInFinalIteration(ctx, ctx.trip_count_upper_bound);

                            lhs_is_upper_bound = (icmp_inst->lhs == final_upper_bound);
                            rhs_is_upper_bound = (icmp_inst->rhs == final_upper_bound);
                        }
                    }

                    if (icmp_inst->cond == LLVMIR::IcmpCond::SLE || icmp_inst->cond == LLVMIR::IcmpCond::SLT)
                    {
                        if (rhs_is_upper_bound)
                        {
                            icmp_inst->rhs = ctx.adjusted_upper_bound;
                            DBGINFO("  Replaced upper bound (rhs) with pre-computed adjusted upper bound (else path)");
                        }
                        else if (lhs_is_upper_bound)
                        {
                            icmp_inst->lhs = ctx.adjusted_upper_bound;
                            DBGINFO("  Replaced upper bound (lhs) with pre-computed adjusted upper bound (else path)");
                        }
                        else
                        {
                            icmp_inst->rhs = ctx.adjusted_upper_bound;
                            DBGINFO("  Default: replaced rhs with pre-computed adjusted upper bound (else path)");
                        }
                    }
                    else if (icmp_inst->cond == LLVMIR::IcmpCond::SGE || icmp_inst->cond == LLVMIR::IcmpCond::SGT)
                    {
                        if (lhs_is_upper_bound)
                        {
                            icmp_inst->lhs = ctx.adjusted_upper_bound;
                            DBGINFO("  Replaced upper bound (lhs) with pre-computed adjusted upper bound (else path)");
                        }
                        else if (rhs_is_upper_bound)
                        {
                            icmp_inst->rhs = ctx.adjusted_upper_bound;
                            DBGINFO("  Replaced upper bound (rhs) with pre-computed adjusted upper bound (else path)");
                        }
                        else
                        {
                            icmp_inst->lhs = ctx.adjusted_upper_bound;
                            DBGINFO("  Default: replaced lhs with pre-computed adjusted upper bound (else path)");
                        }
                    }
                }

                if (!cond_op) assert(false && "Could not find cond op in last iter exiting.");

                auto* intermediate_block = createGuardedIntermediateBlock(ctx);

                auto* br_to_next_or_rem = new LLVMIR::BranchCondInst(
                    cond_op, getLabelOperand(ctx.header->block_id), getLabelOperand(intermediate_block->block_id));
                br_to_next_or_rem->block_id = ctx.final_unrolled_latch->block_id;
                br_to_next_or_rem->comment  = "Generated (final latch: cond -> header, !cond -> intermediate)";
                ctx.final_unrolled_latch->insts.push_back(br_to_next_or_rem);

                int phi_idx = 0;
                for (auto* inst : ctx.remainder_header->insts)
                {
                    if (inst->opcode != LLVMIR::IROpCode::PHI) break;
                    auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

                    if (phi_idx < (int)ctx.phi_mappings.size())
                    {
                        const auto&      mapping   = ctx.phi_mappings[phi_idx];
                        LLVMIR::Operand* final_val = findValueInFinalIteration(ctx, mapping.latch_incoming_value);
                        if (final_val)
                        {
                            phi->vals_for_labels.emplace_back(final_val, getLabelOperand(intermediate_block->block_id));
                            DBGINFO("  Added remainder PHI[", phi_idx, "] incoming from intermediate block");
                        }
                    }
                    phi_idx++;
                }

                DBGINFO("  Rebuilt final latch to conditional branch to header/intermediate");

                updateExitBlockPhiNodes(ctx);
            }
        }

        return updateOriginalHeaderPhiNodes(ctx);
    }

    bool LoopPartialUnrollPass::updateExitBlockPhiNodes(UnrollContext& ctx)
    {
        DBGINFO("Updating exit block PHI nodes");

        int remainder_exit_phi_index = 0;
        for (auto* inst : ctx.exit->insts)
        {
            if (auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst))
            {
                phi->ErasePhiWithBlock(ctx.latch->block_id);

                LLVMIR::Operand* remainder_exit_value = nullptr;
                int              seen_phi             = 0;
                for (auto* remainder_inst : ctx.remainder_exit->insts)
                {
                    if (remainder_inst->opcode == LLVMIR::IROpCode::PHI)
                    {
                        if (seen_phi == remainder_exit_phi_index)
                        {
                            remainder_exit_value = remainder_inst->GetResultOperand();
                            break;
                        }
                        ++seen_phi;
                    }
                }

                if (remainder_exit_value)
                {
                    phi->Insert_into_PHI(remainder_exit_value, getLabelOperand(ctx.remainder_exit->block_id));
                    DBGINFO("  Exit PHI add incoming from remainder_exit Block", ctx.remainder_exit->block_id);
                }

                ++remainder_exit_phi_index;
            }
        }

        return true;
    }

    bool LoopPartialUnrollPass::updateOriginalHeaderPhiNodes(UnrollContext& ctx)
    {
        DBGINFO("Updating original header PHI nodes to receive from final unrolled latch");

        std::vector<LLVMIR::Operand*> final_values;

        for (const auto& mapping : ctx.phi_mappings)
        {
            LLVMIR::Operand* final_val = findValueInFinalIteration(ctx, mapping.latch_incoming_value);

            if (mapping.latch_def_inst && final_val == mapping.latch_incoming_value)
            {
                DBGINFO("ERROR: Could not find final unrolled value for a loop-variant PHI. reg=",
                    mapping.original_phi_result_reg);
                assert(false && "Failed to find final value for a loop-variant PHI. Check value mapping logic.");
            }

            if (final_val)
            {
                DBGINFO("  Using final value for remainder header PHI reg",
                    mapping.original_phi_result_reg,
                    ": ",
                    final_val->getName());
            }

            final_values.push_back(final_val);
        }

        int phi_idx = 0;
        for (auto* inst : ctx.header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;
            auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

            if (phi_idx < (int)final_values.size() && final_values[phi_idx])
            {
                phi->vals_for_labels.emplace_back(
                    final_values[phi_idx], getLabelOperand(ctx.final_unrolled_latch->block_id));
                DBGINFO("  Updated original header PHI[",
                    phi_idx,
                    "] with final value from Block",
                    ctx.final_unrolled_latch->block_id);
            }

            phi_idx++;
        }

        return true;
    }

    int LoopPartialUnrollPass::getLoopSize(NaturalLoop* loop) const
    {
        int size = 0;
        for (auto* block : loop->loop_nodes) size += block->insts.size();
        return size;
    }

    int LoopPartialUnrollPass::getGlobalInstructionCount() const
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

    bool LoopPartialUnrollPass::hasArrayAccess(NaturalLoop* loop) const
    {
        for (auto* block : loop->loop_nodes)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::STORE || inst->opcode == LLVMIR::IROpCode::LOAD) { return true; }
            }
        }
        return false;
    }

    bool LoopPartialUnrollPass::initializePhiInfos(UnrollContext& ctx)
    {
        DBGINFO("Initializing PHI mappings for loop ", ctx.loop->loop_id);

        ctx.phi_mappings.clear();

        for (auto* inst : ctx.header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto*                     phi = static_cast<LLVMIR::PhiInst*>(inst);
            UnrollContext::PhiMapping mapping;
            mapping.original_phi             = phi;
            mapping.original_phi_result_reg  = phi->GetResultReg();
            mapping.latch_incoming_value     = nullptr;
            mapping.latch_def_inst           = nullptr;
            mapping.preheader_incoming_value = nullptr;

            for (const auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                if (label_op->label_num == ctx.latch->block_id) { mapping.latch_incoming_value = val; }
                else if (label_op->label_num == ctx.preheader->block_id) { mapping.preheader_incoming_value = val; }
            }

            if (mapping.latch_incoming_value && mapping.latch_incoming_value->type == LLVMIR::OperandType::REG)
            {
                auto* reg_op = static_cast<LLVMIR::RegOperand*>(mapping.latch_incoming_value);

                for (auto* latch_inst : ctx.latch->insts)
                {
                    if (latch_inst->GetResultReg() == reg_op->reg_num)
                    {
                        mapping.latch_def_inst = latch_inst;
                        break;
                    }
                }
            }

            ctx.phi_mappings.push_back(mapping);
            DBGINFO("  Found PHI mapping: phi_reg=",
                mapping.original_phi_result_reg,
                " latch_value=",
                mapping.latch_incoming_value ? mapping.latch_incoming_value->getName() : "null",
                " preheader_value=",
                mapping.preheader_incoming_value ? mapping.preheader_incoming_value->getName() : "null",
                " latch_def_inst=",
                mapping.latch_def_inst ? "found" : "not_found");
        }

        DBGINFO("Initialized ", ctx.phi_mappings.size(), " PHI mappings");
        return true;
    }

    LLVMIR::Operand* LoopPartialUnrollPass::findValueInFinalIteration(
        UnrollContext& ctx, LLVMIR::Operand* original_value)
    {
        if (!original_value || original_value->type != LLVMIR::OperandType::REG) { return original_value; }

        LLVMIR::Operand* current_value = original_value;
        std::set<int>    visited_regs;

        while (current_value && current_value->type == LLVMIR::OperandType::REG)
        {
            auto* reg_op = static_cast<LLVMIR::RegOperand*>(current_value);

            if (visited_regs.find(reg_op->reg_num) != visited_regs.end()) break;
            visited_regs.insert(reg_op->reg_num);

            auto it = ctx.final_value_map.find(reg_op->reg_num);
            if (it != ctx.final_value_map.end() && it->second != current_value)
            {
                current_value = it->second;
                DBGINFO("  Tracing mapping chain: reg", reg_op->reg_num, " -> ", current_value->getName());
            }
            else { break; }
        }

        return current_value;
    }

    void LoopPartialUnrollPass::logResult(
        NaturalLoop* loop, bool success, int unroll_factor, const std::string& reason) const
    {
#ifdef DBGMODE
        std::cout << "Loop " << loop->loop_id << ": " << (success ? "PARTIALLY UNROLLED" : "SKIPPED")
                  << " (factor=" << unroll_factor << ") - " << reason << std::endl;
#endif
    }

    LLVMIR::IRBlock* LoopPartialUnrollPass::createGuardedIntermediateBlock(UnrollContext& ctx)
    {
        auto* intermediate_block    = ctx.cfg->createBlock();
        intermediate_block->comment = "Intermediate block to guard remainder loop entry";

        if (!ctx.original_condition || !ctx.original_branch)
        {
            assert(false && "Could not find original condition in loop");
        }
        else
        {
            LLVMIR::Operand* final_lhs = findValueInFinalIteration(ctx, ctx.original_condition->lhs);
            LLVMIR::Operand* final_rhs = findValueInFinalIteration(ctx, ctx.original_condition->rhs);

            auto* cmp_reg  = getRegOperand(++ctx.cfg->func->max_reg);
            auto* cmp_inst = new LLVMIR::IcmpInst(
                ctx.original_condition->type, ctx.original_condition->cond, final_lhs, final_rhs, cmp_reg);
            cmp_inst->block_id = intermediate_block->block_id;
            cmp_inst->comment  = "Guard condition for remainder loop";
            intermediate_block->insts.push_back(cmp_inst);

            auto* true_target  = static_cast<LLVMIR::LabelOperand*>(ctx.original_branch->true_label);
            auto* false_target = static_cast<LLVMIR::LabelOperand*>(ctx.original_branch->false_label);

            LLVMIR::Operand* remainder_target = getLabelOperand(ctx.remainder_header->block_id);
            LLVMIR::Operand* exit_target      = getLabelOperand(ctx.remainder_exit->block_id);

            if (true_target->label_num == ctx.exit->block_id)
            {
                auto* br_cond     = new LLVMIR::BranchCondInst(cmp_reg, exit_target, remainder_target);
                br_cond->block_id = intermediate_block->block_id;
                br_cond->comment  = "Guard: exit if condition true, enter remainder if false";
                intermediate_block->insts.push_back(br_cond);

                updateRemainderExitPhiNodes(ctx, intermediate_block);
            }
            else
            {
                auto* br_cond     = new LLVMIR::BranchCondInst(cmp_reg, remainder_target, exit_target);
                br_cond->block_id = intermediate_block->block_id;
                br_cond->comment  = "Guard: enter remainder if condition true, exit if false";
                intermediate_block->insts.push_back(br_cond);

                updateRemainderExitPhiNodes(ctx, intermediate_block);
            }

            DBGINFO("  Created guarded intermediate block with original condition logic");
        }

        return intermediate_block;
    }

    void LoopPartialUnrollPass::updateRemainderExitPhiNodes(UnrollContext& ctx, LLVMIR::IRBlock* guard_block)
    {
        DBGINFO("Updating remainder exit PHI nodes for guard block");

        int phi_index = 0;
        for (auto* exit_inst : ctx.exit->insts)
        {
            if (exit_inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* exit_phi = static_cast<LLVMIR::PhiInst*>(exit_inst);

            LLVMIR::Operand* original_latch_value = nullptr;
            for (const auto& [val, label] : exit_phi->vals_for_labels)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                if (label_op->label_num == ctx.latch->block_id)
                {
                    original_latch_value = val;
                    break;
                }
            }

            if (original_latch_value)
            {
                LLVMIR::Operand* final_value = findValueInFinalIteration(ctx, original_latch_value);

                if (final_value)
                {
                    int seen_phi = 0;
                    for (auto* remainder_inst : ctx.remainder_exit->insts)
                    {
                        if (remainder_inst->opcode == LLVMIR::IROpCode::PHI)
                        {
                            if (seen_phi == phi_index)
                            {
                                auto* remainder_phi = static_cast<LLVMIR::PhiInst*>(remainder_inst);

                                remainder_phi->vals_for_labels.emplace_back(
                                    final_value, getLabelOperand(guard_block->block_id));
                                DBGINFO("  Added guard PHI[", phi_index, "] incoming from guard block: ", final_value);
                                break;
                            }
                            ++seen_phi;
                        }
                    }
                }
                else
                {
                    DBGINFO("  Warning: Could not find final value for PHI[",
                        phi_index,
                        "] from guard block, original value: ",
                        original_latch_value);
                }
            }
            else { DBGINFO("  Warning: No latch incoming value found for exit PHI[", phi_index, "]"); }

            phi_index++;
        }

        DBGINFO("  Updated ", phi_index, " remainder exit PHI nodes with guard incoming values");
    }

    bool LoopPartialUnrollPass::checkLoopBoundsDominance(UnrollContext& ctx)
    {
        DBGINFO("Checking loop bounds dominance");

        auto checkOperand = [&](const Analysis::CROperand& cr_op, const std::string& name) -> bool {
            if (cr_op.type == Analysis::CROperand::LLVM_OPERAND)
            {
                auto* operand = cr_op.llvm_op;
                if (auto* reg_op = dynamic_cast<LLVMIR::RegOperand*>(operand))
                {
                    for (auto* block : ctx.cfg->func->blocks)
                    {
                        for (auto* inst : block->insts)
                        {
                            if (inst->GetResultOperand() == operand)
                            {
                                if (ctx.loop->loop_nodes.count(block))
                                {
                                    DBGINFO("  ",
                                        name,
                                        " bound operand ",
                                        operand->getName(),
                                        " is defined inside loop in Block",
                                        block->block_id);
                                    return false;
                                }
                                break;
                            }
                        }
                    }
                }
            }
            return true;
        };

        if (!checkOperand(ctx.loop_info->loop_info.lowerbound, "Lower")) { return false; }

        if (!checkOperand(ctx.loop_info->loop_info.upperbound, "Upper")) { return false; }

        DBGINFO("  Loop bounds are loop-invariant, safe for partial unroll");
        return true;
    }

    std::vector<LLVMIR::IRBlock*> LoopPartialUnrollPass::getPredecessors(LLVMIR::IRBlock* block, CFG* cfg)
    {
        std::vector<LLVMIR::IRBlock*> predecessors;
        if (block->block_id < static_cast<int>(cfg->invG.size())) { predecessors = cfg->invG[block->block_id]; }
        return predecessors;
    }

    void LoopPartialUnrollPass::updateBranchTarget(LLVMIR::IRBlock* block, int old_target, int new_target)
    {
        if (block->insts.empty()) return;

        auto* last_inst = block->insts.back();
        if (last_inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* br     = static_cast<LLVMIR::BranchUncondInst*>(last_inst);
            auto* target = static_cast<LLVMIR::LabelOperand*>(br->target_label);
            if (target->label_num == old_target) { br->target_label = getLabelOperand(new_target); }
        }
        else if (last_inst->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br           = static_cast<LLVMIR::BranchCondInst*>(last_inst);
            auto* true_target  = static_cast<LLVMIR::LabelOperand*>(br->true_label);
            auto* false_target = static_cast<LLVMIR::LabelOperand*>(br->false_label);

            if (true_target->label_num == old_target) { br->true_label = getLabelOperand(new_target); }
            if (false_target->label_num == old_target) { br->false_label = getLabelOperand(new_target); }
        }
    }

    bool LoopPartialUnrollPass::createRemainderLoop(UnrollContext& ctx)
    {
        DBGINFO("Creating remainder loop for remaining iterations (before unrolling)");

        ctx.remainder_exit          = ctx.cfg->createBlock();
        ctx.remainder_exit->comment = "Remainder loop exit for loop " + std::to_string(ctx.loop->loop_id);

        int phi_index = 0;
        for (auto* inst : ctx.exit->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* exit_phi               = static_cast<LLVMIR::PhiInst*>(inst);
            auto* result_reg             = getRegOperand(++ctx.cfg->func->max_reg);
            auto* remainder_exit_phi     = new LLVMIR::PhiInst(exit_phi->type, result_reg);
            remainder_exit_phi->block_id = ctx.remainder_exit->block_id;
            remainder_exit_phi->comment  = "Remainder exit PHI for original PHI " + std::to_string(phi_index);
            ctx.remainder_exit->insts.push_back(remainder_exit_phi);

            LLVMIR::Operand* original_latch_value = nullptr;
            for (const auto& [val, label] : exit_phi->vals_for_labels)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                if (label_op->label_num == ctx.latch->block_id)
                {
                    original_latch_value = val;
                    break;
                }
            }

            if (original_latch_value)
            {
                DBGINFO(
                    "  Original exit PHI[", phi_index, "] gets value ", original_latch_value->getName(), " from latch");
            }

            phi_index++;
        }

        auto* br_to_original_exit     = new LLVMIR::BranchUncondInst(getLabelOperand(ctx.exit->block_id));
        br_to_original_exit->block_id = ctx.remainder_exit->block_id;
        br_to_original_exit->comment  = "Jump to original exit block";
        ctx.remainder_exit->insts.push_back(br_to_original_exit);

        if (!cloneLoopBlocks(
                ctx, ctx.remain_preheader, ctx.remainder_exit, ctx.remainder_blocks, ctx.remainder_value_map))
        {
            DBGINFO("Failed to clone loop blocks for remainder loop");
            return false;
        }

        if (!connectRemainderLoop(ctx, ctx.remainder_blocks, ctx.remainder_value_map))
        {
            DBGINFO("Failed to connect remainder loop");
            return false;
        }

        if (!updateRemainderPhiNodes(ctx, ctx.remainder_blocks, ctx.remainder_value_map))
        {
            DBGINFO("Failed to update remainder loop PHI nodes");
            return false;
        }

        if (!ctx.remain_preheader->insts.empty())
        {
            delete ctx.remain_preheader->insts.back();
            ctx.remain_preheader->insts.pop_back();
        }

        auto* br_to_remainder     = new LLVMIR::BranchUncondInst(getLabelOperand(ctx.remainder_header->block_id));
        br_to_remainder->block_id = ctx.remain_preheader->block_id;
        br_to_remainder->comment  = "Jump to remainder loop header";
        ctx.remain_preheader->insts.push_back(br_to_remainder);

        DBGINFO("Successfully created remainder loop with ", ctx.remainder_blocks.size(), " blocks");
        return true;
    }

    bool LoopPartialUnrollPass::cloneLoopBlocks(UnrollContext& ctx, LLVMIR::IRBlock* insert_top,
        LLVMIR::IRBlock* insert_bot, std::vector<LLVMIR::IRBlock*>& new_blocks, ValueMap& vmap)
    {
        DBGINFO("Cloning loop blocks for remainder loop");

        std::set<LLVMIR::IRBlock*> original_blocks = ctx.loop->loop_nodes;

        std::map<int, int>              label_map;
        std::map<int, LLVMIR::IRBlock*> block_map;
        for (auto* orig_block : original_blocks)
        {
            auto* new_block                 = ctx.cfg->createBlock();
            new_block->comment              = "Remainder " + orig_block->comment;
            label_map[orig_block->block_id] = new_block->block_id;
            block_map[orig_block->block_id] = new_block;
            new_blocks.push_back(new_block);

            if (orig_block == ctx.header) { ctx.remainder_header = new_block; }
            if (orig_block == ctx.latch) { ctx.remainder_latch = new_block; }
        }

        std::map<int, int> reg_map;
        for (auto* orig_block : original_blocks)
        {
            for (auto* inst : orig_block->insts)
            {
                int result_reg = inst->GetResultReg();
                if (result_reg != -1) { reg_map[result_reg] = ++ctx.cfg->func->max_reg; }
            }
        }

        for (auto* orig_block : original_blocks)
        {
            auto* new_block = block_map[orig_block->block_id];

            for (auto* inst : orig_block->insts)
            {
                auto* cloned_inst     = inst->CloneWithMapping(reg_map, label_map);
                cloned_inst->comment  = "Remainder: " + (inst->comment.empty() ? "cloned" : inst->comment);
                cloned_inst->block_id = new_block->block_id;
                new_block->insts.push_back(cloned_inst);
            }
        }

        for (const auto& [orig_reg, new_reg] : reg_map) { vmap[orig_reg] = getRegOperand(new_reg); }

        DBGINFO("Cloned ", new_blocks.size(), " blocks for remainder loop");
        return true;
    }

    bool LoopPartialUnrollPass::connectRemainderLoop(
        UnrollContext& ctx, const std::vector<LLVMIR::IRBlock*>& remainder_blocks, const ValueMap& vmap)
    {
        DBGINFO("Connecting remainder loop");

        if (!ctx.remainder_latch->insts.empty())
        {
            auto* last_inst = ctx.remainder_latch->insts.back();
            if (last_inst->opcode == LLVMIR::IROpCode::BR_COND)
            {
                auto* br_cond = static_cast<LLVMIR::BranchCondInst*>(last_inst);

                auto* true_target  = static_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
                auto* false_target = static_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

                if (true_target->label_num == ctx.exit->block_id)
                {
                    br_cond->true_label = getLabelOperand(ctx.remainder_exit->block_id);
                }
                if (false_target->label_num == ctx.exit->block_id)
                {
                    br_cond->false_label = getLabelOperand(ctx.remainder_exit->block_id);
                }

                if (true_target->label_num == ctx.header->block_id)
                {
                    br_cond->true_label = getLabelOperand(ctx.remainder_header->block_id);
                }
                if (false_target->label_num == ctx.header->block_id)
                {
                    br_cond->false_label = getLabelOperand(ctx.remainder_header->block_id);
                }
            }
            else if (last_inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
            {
                auto* br_uncond = static_cast<LLVMIR::BranchUncondInst*>(last_inst);
                auto* target    = static_cast<LLVMIR::LabelOperand*>(br_uncond->target_label);

                if (target->label_num != ctx.remainder_header->block_id &&
                    target->label_num != ctx.remainder_exit->block_id)
                {
                    DBGINFO("Warning: Unexpected unconditional branch in remainder latch");
                }
            }
        }

        DBGINFO("Successfully connected remainder loop");
        return true;
    }

    bool LoopPartialUnrollPass::updateRemainderPhiNodes(
        UnrollContext& ctx, const std::vector<LLVMIR::IRBlock*>& remainder_blocks, const ValueMap& vmap)
    {
        DBGINFO("Updating remainder loop PHI nodes");

        int phi_index = 0;
        for (auto* inst : ctx.remainder_header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

            phi->vals_for_labels.clear();

            if (phi_index < (int)ctx.phi_mappings.size())
            {
                const auto& mapping = ctx.phi_mappings[phi_index];
                if (mapping.preheader_incoming_value)
                {
                    LLVMIR::Operand* remain_value = mapping.preheader_incoming_value;

                    if (mapping.preheader_incoming_value->type == LLVMIR::OperandType::REG)
                    {
                        auto* reg_op = static_cast<LLVMIR::RegOperand*>(mapping.preheader_incoming_value);
                        auto  it     = ctx.remainder_value_map.find(reg_op->reg_num);
                        if (it != ctx.remainder_value_map.end())
                        {
                            remain_value = it->second;
                            DBGINFO("  Found remain_preheader value for PHI[",
                                phi_index,
                                "]: reg",
                                reg_op->reg_num,
                                " -> ",
                                remain_value->getName());
                        }
                    }

                    phi->vals_for_labels.emplace_back(remain_value, getLabelOperand(ctx.remain_preheader->block_id));
                    DBGINFO("  Added remainder PHI[",
                        phi_index,
                        "] incoming from remain_preheader: ",
                        remain_value->getName());
                }
            }

            if (phi_index < (int)ctx.phi_mappings.size())
            {
                const auto& mapping = ctx.phi_mappings[phi_index];
                if (mapping.latch_incoming_value)
                {
                    LLVMIR::Operand* remainder_value = mapping.latch_incoming_value;
                    if (mapping.latch_incoming_value->type == LLVMIR::OperandType::REG)
                    {
                        auto* reg_op = static_cast<LLVMIR::RegOperand*>(mapping.latch_incoming_value);
                        auto  it     = vmap.find(reg_op->reg_num);
                        if (it != vmap.end())
                        {
                            remainder_value = it->second;
                            DBGINFO("  Found remainder value for PHI[",
                                phi_index,
                                "]: reg",
                                reg_op->reg_num,
                                " -> ",
                                remainder_value->getName());
                        }
                    }

                    phi->vals_for_labels.emplace_back(remainder_value, getLabelOperand(ctx.remainder_latch->block_id));
                }
            }

            phi_index++;
        }

        phi_index                    = 0;
        int remainder_exit_phi_index = 0;
        for (auto* exit_inst : ctx.exit->insts)
        {
            if (exit_inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* exit_phi = static_cast<LLVMIR::PhiInst*>(exit_inst);

            auto* remainder_exit_phi =
                static_cast<LLVMIR::PhiInst*>(ctx.remainder_exit->insts[remainder_exit_phi_index]);

            LLVMIR::Operand* original_latch_value = nullptr;
            for (const auto& [val, label] : exit_phi->vals_for_labels)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                if (label_op->label_num == ctx.latch->block_id)
                {
                    original_latch_value = val;
                    break;
                }
            }

            if (original_latch_value)
            {
                LLVMIR::Operand* remainder_value = original_latch_value;
                if (original_latch_value->type == LLVMIR::OperandType::REG)
                {
                    auto* reg_op = static_cast<LLVMIR::RegOperand*>(original_latch_value);
                    auto  it     = vmap.find(reg_op->reg_num);
                    if (it != vmap.end()) { remainder_value = it->second; }
                }

                remainder_exit_phi->vals_for_labels.emplace_back(
                    remainder_value, getLabelOperand(ctx.remainder_latch->block_id));
                DBGINFO("  Set remainder_exit PHI[",
                    phi_index,
                    "] from remainder_latch: ",
                    remainder_value->getName(),
                    " (for original latch value ",
                    original_latch_value->getName(),
                    ")");
            }

            phi_index++;
            remainder_exit_phi_index++;
        }

        DBGINFO("Successfully updated remainder loop PHI nodes");
        return true;
    }

}  // namespace Transform
