#include "indvars_simplify.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <functional>

namespace Transform
{

    IndVarsSimplifyPass::IndVarsSimplifyPass(LLVMIR::IR* ir, Analysis::SCEVAnalyser* scev)
        : Pass(ir), scev_analyser_(scev)
    {}

    void IndVarsSimplifyPass::Execute()
    {
        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (!cfg || !cfg->LoopForest || cfg->LoopForest->loop_set.empty()) continue;

            processFunction(cfg);
        }
    }

    void IndVarsSimplifyPass::processFunction(CFG* cfg)
    {
        std::vector<NaturalLoop*> loops_to_process;
        for (auto* loop : cfg->LoopForest->loop_set) loops_to_process.push_back(loop);

        std::sort(loops_to_process.begin(), loops_to_process.end(), [](const NaturalLoop* a, const NaturalLoop* b) {
            return a->loop_id > b->loop_id;
        });

        for (auto* loop : loops_to_process) { processLoop(cfg, loop); }
    }

    bool IndVarsSimplifyPass::processLoop(CFG* cfg, NaturalLoop* loop)
    {
        bool changed = false;

        auto induction_vars = findInductionVariables(cfg, loop);
        if (induction_vars.empty())
        {
            // std::cout << "[IndVars]\tNo induction variables found in loop " << loop->loop_id << std::endl;
            return false;
        }

        // std::cout << "[IndVars]\tFound " << induction_vars.size() << " induction variables" << std::endl;

        auto* canonical_iv = createCanonicalInductionVariable(cfg, loop, induction_vars);
        if (!canonical_iv)
        {
            // std::cout << "[IndVars]\tFailed to create canonical induction variable" << std::endl;
            return false;
        }

        // std::cout << "[IndVars]\tCanonical IV reg: " << canonical_iv->GetResultReg() << std::endl;
        changed = true;

        rewriteInductionVariables(cfg, loop, canonical_iv, induction_vars);

        canonicalizeExitConditions(cfg, loop, canonical_iv);

        auto derived_exprs = findDerivedExpressions(cfg, loop, induction_vars);
        if (!derived_exprs.empty()) recomputeDerivedExpressionsOutside(cfg, loop, canonical_iv, derived_exprs);

        promotePointerArithmetic(cfg, loop);

        return changed;
    }

    std::vector<InductionVariable> IndVarsSimplifyPass::findInductionVariables(CFG* cfg, NaturalLoop* loop)
    {
        std::vector<InductionVariable> induction_vars;

        if (!loop->header) return induction_vars;

        for (auto* inst : loop->header->insts)
        {
            auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
            if (!phi || phi->vals_for_labels.size() != 2) continue;

            InductionVariable iv;
            iv.phi_inst     = phi;
            iv.is_canonical = false;
            iv.scev_expr    = nullptr;

            LLVMIR::Operand* init_value = nullptr;
            LLVMIR::Operand* loop_value = nullptr;

            for (const auto& [val, label] : phi->vals_for_labels)
            {
                auto* label_op = dynamic_cast<LLVMIR::LabelOperand*>(label);
                if (!label_op) continue;

                bool is_from_preheader = false;
                if (loop->preheader && label_op->label_num == loop->preheader->block_id)
                    is_from_preheader = true;
                else
                {
                    auto it = cfg->block_id_to_block.find(label_op->label_num);
                    if (it != cfg->block_id_to_block.end())
                        if (loop->loop_nodes.find(it->second) == loop->loop_nodes.end()) is_from_preheader = true;
                }

                if (is_from_preheader)
                    init_value = val;
                else
                    loop_value = val;
            }

            if (!init_value || !loop_value) continue;

            auto* loop_val_reg = dynamic_cast<LLVMIR::RegOperand*>(loop_value);
            if (!loop_val_reg) continue;

            LLVMIR::Instruction* inc_inst = nullptr;
            for (auto* block : loop->loop_nodes)
            {
                for (auto* inst : block->insts)
                {
                    if (inst->GetResultReg() != loop_val_reg->reg_num) continue;

                    inc_inst = inst;
                    break;
                }
                if (inc_inst) break;
            }

            if (!inc_inst) continue;

            LLVMIR::Operand* step;
            LLVMIR::IROpCode opcode;
            if (isInductionIncrement(inc_inst, phi, step, opcode))
            {
                iv.start_value = init_value;
                iv.step_value  = step;
                iv.step_opcode = opcode;

                auto* init_imm = dynamic_cast<LLVMIR::ImmeI32Operand*>(init_value);
                auto* step_imm = dynamic_cast<LLVMIR::ImmeI32Operand*>(step);
                if (init_imm && step_imm && init_imm->value == 0 && step_imm->value == 1 &&
                    opcode == LLVMIR::IROpCode::ADD)
                {
                    iv.is_canonical = true;
                }

                induction_vars.push_back(iv);
            }
        }

        return induction_vars;
    }

    bool IndVarsSimplifyPass::isInductionIncrement(
        LLVMIR::Instruction* inst, LLVMIR::PhiInst* phi, LLVMIR::Operand*& step, LLVMIR::IROpCode& opcode)
    {
        auto* arith = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
        if (!arith) return false;

        if (arith->opcode != LLVMIR::IROpCode::ADD && arith->opcode != LLVMIR::IROpCode::SUB) return false;

        opcode = arith->opcode;

        auto* phi_reg = dynamic_cast<LLVMIR::RegOperand*>(phi->res);
        if (!phi_reg) return false;

        auto* lhs_reg = dynamic_cast<LLVMIR::RegOperand*>(arith->lhs);
        auto* rhs_reg = dynamic_cast<LLVMIR::RegOperand*>(arith->rhs);

        if (lhs_reg && lhs_reg->reg_num == phi_reg->reg_num)
        {
            step = arith->rhs;
            return true;
        }
        else if (rhs_reg && rhs_reg->reg_num == phi_reg->reg_num && arith->opcode == LLVMIR::IROpCode::ADD)
        {
            step = arith->lhs;
            return true;
        }

        return false;
    }

    LLVMIR::PhiInst* IndVarsSimplifyPass::createCanonicalInductionVariable(
        CFG* cfg, NaturalLoop* loop, const std::vector<InductionVariable>& ivs)
    {
        for (const auto& iv : ivs)
            if (iv.is_canonical) return iv.phi_inst;

        if (!loop->header || !loop->preheader) return nullptr;

        int phi_reg = ++cfg->func->max_reg;
        int inc_reg = ++cfg->func->max_reg;

        LLVMIR::IRBlock* main_latch = nullptr;
        if (!loop->latches.empty()) main_latch = *loop->latches.begin();

        if (!main_latch) return nullptr;

        std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> phi_values;
        phi_values.push_back({getImmeI32Operand(0), getLabelOperand(loop->preheader->block_id)});
        phi_values.push_back({getRegOperand(inc_reg), getLabelOperand(main_latch->block_id)});

        auto* canonical_phi = new LLVMIR::PhiInst(LLVMIR::DataType::I32, getRegOperand(phi_reg), &phi_values);

        insertInstructionAt(loop->header, canonical_phi, 0);

        auto* inc_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
            LLVMIR::DataType::I32,
            getRegOperand(phi_reg),
            getImmeI32Operand(1),
            getRegOperand(inc_reg));

        bool inserted = false;
        for (size_t i = 0; i < main_latch->insts.size(); ++i)
        {
            if (!IS_BR(main_latch->insts[i])) continue;

            auto it = main_latch->insts.begin() + i;
            main_latch->insts.insert(it, inc_inst);
            inserted = true;
            break;
        }

        if (!inserted) main_latch->insts.push_back(inc_inst);

        return canonical_phi;
    }

    void IndVarsSimplifyPass::rewriteInductionVariables(
        CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* canonical_iv, const std::vector<InductionVariable>& ivs)
    {
        auto* canonical_reg = dynamic_cast<LLVMIR::RegOperand*>(canonical_iv->res);
        if (!canonical_reg) return;

        int rewritten_count = 0;

        for (const auto& iv : ivs)
        {
            if (iv.is_canonical || iv.phi_inst == canonical_iv) continue;

            auto* iv_reg = dynamic_cast<LLVMIR::RegOperand*>(iv.phi_inst->res);
            if (!iv_reg) continue;

            auto* start_imm = dynamic_cast<LLVMIR::ImmeI32Operand*>(iv.start_value);
            auto* step_imm  = dynamic_cast<LLVMIR::ImmeI32Operand*>(iv.step_value);

            if (!start_imm || !step_imm) continue;

            int start_val = start_imm->value;
            int step_val  = step_imm->value;

            if (iv.step_opcode == LLVMIR::IROpCode::SUB) step_val = -step_val;

            LLVMIR::Operand* converted_iv = nullptr;

            if (step_val == 1)
            {
                if (start_val == 0)
                {
                    // old_iv = canonical_iv
                    converted_iv = getRegOperand(canonical_reg->reg_num);
                }
                else
                {
                    // old_iv = canonical_iv + start
                    int   add_reg  = ++cfg->func->max_reg;
                    auto* add_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                        LLVMIR::DataType::I32,
                        getRegOperand(canonical_reg->reg_num),
                        getImmeI32Operand(start_val),
                        getRegOperand(add_reg));
                    insertInstructionAt(loop->header, add_inst, 1);
                    converted_iv = getRegOperand(add_reg);
                }
            }
            else
            {
                // old_iv = start + step * canonical_iv
                int   mul_reg  = ++cfg->func->max_reg;
                auto* mul_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::MUL,
                    LLVMIR::DataType::I32,
                    getRegOperand(canonical_reg->reg_num),
                    getImmeI32Operand(step_val),
                    getRegOperand(mul_reg));

                if (start_val == 0)
                {
                    insertInstructionAt(loop->header, mul_inst, 0);
                    converted_iv = getRegOperand(mul_reg);
                }
                else
                {
                    int   add_reg  = ++cfg->func->max_reg;
                    auto* add_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                        LLVMIR::DataType::I32,
                        getImmeI32Operand(start_val),
                        getRegOperand(mul_reg),
                        getRegOperand(add_reg));

                    size_t phi_end = 0;
                    for (size_t i = 0; i < loop->header->insts.size(); ++i)
                    {
                        if (dynamic_cast<LLVMIR::PhiInst*>(loop->header->insts[i]))
                            phi_end = i + 1;
                        else
                            break;
                    }

                    auto it1 = loop->header->insts.begin() + phi_end;
                    loop->header->insts.insert(it1, mul_inst);

                    auto it2 = loop->header->insts.begin() + phi_end + 1;
                    loop->header->insts.insert(it2, add_inst);

                    converted_iv = getRegOperand(add_reg);
                }
            }

            if (converted_iv)
            {
                replaceUsesInLoop(cfg, loop, iv_reg->reg_num, converted_iv);

                auto& header_insts = loop->header->insts;
                auto  phi_it       = std::find(header_insts.begin(), header_insts.end(), iv.phi_inst);
                if (phi_it != header_insts.end())
                {
                    header_insts.erase(phi_it);
                    delete iv.phi_inst;
                }
                rewritten_count++;
            }
        }

        std::cout << "[IndVars]\tRewrote " << rewritten_count << " induction variables" << std::endl;
    }

    void IndVarsSimplifyPass::canonicalizeExitConditions(CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* canonical_iv)
    {
        auto* canonical_reg = dynamic_cast<LLVMIR::RegOperand*>(canonical_iv->res);
        if (!canonical_reg) return;

        int canonicalized_count = 0;
        int skipped_count       = 0;

        // 直接遍历退出块，无需预先收集
        for (auto* exiting_block : loop->exiting_nodes)
        {
            LLVMIR::Instruction* cmp_inst = nullptr;
            LLVMIR::Instruction* br_inst  = nullptr;

            for (auto* inst : exiting_block->insts)
            {
                if (auto* icmp = dynamic_cast<LLVMIR::IcmpInst*>(inst))
                    cmp_inst = icmp;
                else if (IS_BR(inst))
                    br_inst = inst;
            }

            if (!cmp_inst || !br_inst) continue;

            auto* icmp = dynamic_cast<LLVMIR::IcmpInst*>(cmp_inst);
            if (!icmp) continue;

            if (!isIVBasedExit(cfg, loop, icmp, canonical_iv))
            {
                // std::cout << "[IndVars]\t  Skipping exit condition not based on induction variable" << std::endl;
                skipped_count++;
                continue;
            }

            bool is_main_loop_exit = false;
            for (auto* latch : loop->latches)
            {
                if (latch == exiting_block)
                {
                    is_main_loop_exit = true;
                    break;
                }
            }

            if (!is_main_loop_exit)
            {
                // std::cout << "[IndVars]\t  Skipping early exit condition (break/continue), not in latch block"
                //           << std::endl;
                skipped_count++;
                continue;
            }

            auto trip_count_opt = computeTripCount(cfg, loop, canonical_iv);
            if (!trip_count_opt)
            {
                // std::cout << "[IndVars]\t  Cannot compute trip count for exit condition" << std::endl;
                skipped_count++;
                continue;
            }

            auto* br_cond = dynamic_cast<LLVMIR::BranchCondInst*>(br_inst);
            if (!br_cond) continue;

            auto* true_target  = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
            auto* false_target = dynamic_cast<LLVMIR::LabelOperand*>(br_cond->false_label);
            if (!true_target || !false_target) continue;

            bool true_in_loop = false;
            for (auto* loop_block : loop->loop_nodes)
            {
                if (loop_block->block_id == true_target->label_num)
                {
                    true_in_loop = true;
                    break;
                }
            }

            // true分支在循环内用NE，否则用EQ
            LLVMIR::IcmpCond new_cond = true_in_loop ? LLVMIR::IcmpCond::NE : LLVMIR::IcmpCond::EQ;

            // 若latch 为 exiting block，为 do while 形式，使用post-increment
            bool             use_post_inc = (loop->latches.count(exiting_block) > 0);
            LLVMIR::Operand* cmp_var      = nullptr;

            if (use_post_inc)
            {
                for (auto* inst : exiting_block->insts)
                {
                    auto* arith = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                    if (arith && arith->opcode == LLVMIR::IROpCode::ADD)
                    {
                        auto* arith_lhs = dynamic_cast<LLVMIR::RegOperand*>(arith->lhs);
                        auto* arith_rhs = dynamic_cast<LLVMIR::ImmeI32Operand*>(arith->rhs);

                        if (arith_lhs && arith_lhs->reg_num == canonical_reg->reg_num && arith_rhs &&
                            arith_rhs->value == 1)
                        {
                            cmp_var = arith->res;
                            break;
                        }
                    }
                }
            }

            if (!cmp_var) cmp_var = getRegOperand(canonical_reg->reg_num);

            int   new_cmp_reg = ++cfg->func->max_reg;
            auto* new_cmp     = new LLVMIR::IcmpInst(
                LLVMIR::DataType::I32, new_cond, cmp_var, *trip_count_opt, getRegOperand(new_cmp_reg));

            auto& insts      = exiting_block->insts;
            auto  insert_pos = insts.end();

            if (use_post_inc && cmp_var != getRegOperand(canonical_reg->reg_num))
            {
                auto* cmp_var_reg = dynamic_cast<LLVMIR::RegOperand*>(cmp_var);
                if (cmp_var_reg)
                {
                    for (auto it = insts.begin(); it != insts.end(); ++it)
                    {
                        if ((*it)->GetResultReg() == cmp_var_reg->reg_num)
                        {
                            insert_pos = std::next(it);
                            break;
                        }
                    }
                }
            }
            else
            {
                for (auto it = insts.begin(); it != insts.end(); ++it)
                {
                    if (*it == icmp)
                    {
                        insert_pos = it;
                        break;
                    }
                }
            }

            if (insert_pos != insts.end()) { insts.insert(insert_pos, new_cmp); }

            std::map<int, LLVMIR::Operand*> substitutions;
            int                             old_cmp_reg = icmp->GetResultReg();
            substitutions[old_cmp_reg]                  = getRegOperand(new_cmp_reg);
            br_cond->SubstituteOperands(substitutions);

            auto& block_insts = exiting_block->insts;
            auto  icmp_it     = std::find(block_insts.begin(), block_insts.end(), icmp);
            if (icmp_it != block_insts.end())
            {
                block_insts.erase(icmp_it);
                delete icmp;
            }
            canonicalized_count++;
        }

        if (canonicalized_count > 0)
        {
            // std::cout << "[IndVars]\tCanonicalized " << canonicalized_count << " exit conditions" << std::endl;
        }
        if (skipped_count > 0)
        {
            std::cout << "[IndVars]\tPreserved " << skipped_count << " non-IV-based exit conditions" << std::endl;
        }
    }

    std::vector<DerivedExpression> IndVarsSimplifyPass::findDerivedExpressions(
        CFG* cfg, NaturalLoop* loop, const std::vector<InductionVariable>& ivs)
    {
        // TODO: 查找所有从归纳变量派生的表达式
        // 这个函数需要分析循环内的指令，找出那些基于归纳变量的计算表达式
        // 例如：i*2, i+offset 等形式，这些表达式可以在循环外部预计算
        std::vector<DerivedExpression> derived_exprs;
        return derived_exprs;
    }

    void IndVarsSimplifyPass::recomputeDerivedExpressionsOutside(
        CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* canonical_iv, const std::vector<DerivedExpression>& derived_exprs)
    {
        // TODO: 在循环外部重新计算派生表达式
        // 这个函数需要：
        // 1. 在循环的preheader中插入计算这些表达式最终值的指令
        // 2. 替换循环内对这些表达式的使用
        // 3. 从而减少循环内部的计算量
    }

    void IndVarsSimplifyPass::promotePointerArithmetic(CFG* cfg, NaturalLoop* loop)
    {
        // TODO: 提升指针算术为数组下标
        // 这个函数需要：
        // 1. 识别形如 ptr = base + i*sizeof(type) 的模式
        // 2. 将其转换为数组下标访问 base[i]
        // 3. 这在SysY中主要用于多维数组的线性化访问优化
        // 4. 但数组访问扁平化实际已经在 GEP Strength reduction 中处理，因此实际可能也不需要实现
    }

    std::optional<LLVMIR::Operand*> IndVarsSimplifyPass::computeTripCount(
        CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* canonical_iv)
    {
        if (scev_analyser_)
        {
            auto trip_count = scev_analyser_->getLoopTripCount(loop);
            if (trip_count) return getImmeI32Operand(*trip_count);
        }

        return std::nullopt;
    }

    LLVMIR::Instruction* IndVarsSimplifyPass::createCanonicalExitCondition(
        CFG* cfg, LLVMIR::PhiInst* canonical_iv, LLVMIR::Operand* trip_count, LLVMIR::IRBlock* exiting_block)
    {
        int   cmp_reg       = ++cfg->func->max_reg;
        auto* canonical_reg = dynamic_cast<LLVMIR::RegOperand*>(canonical_iv->res);
        if (!canonical_reg) return nullptr;

        // cmp = icmp ne i32 %canonical_iv, %trip_count
        auto* cmp_inst = new LLVMIR::IcmpInst(LLVMIR::DataType::I32,
            LLVMIR::IcmpCond::NE,
            getRegOperand(canonical_reg->reg_num),
            trip_count,
            getRegOperand(cmp_reg));

        return cmp_inst;
    }

    bool IndVarsSimplifyPass::isLoopInvariant(LLVMIR::Operand* operand, NaturalLoop* loop)
    {
        if (operand->type == LLVMIR::OperandType::IMMEI32 || operand->type == LLVMIR::OperandType::IMMEF32) return true;

        auto* reg = dynamic_cast<LLVMIR::RegOperand*>(operand);
        if (!reg) return true;

        for (auto* block : loop->loop_nodes)
            for (auto* inst : block->insts)
                if (inst->GetResultReg() == reg->reg_num) return false;

        return true;  // 在循环外定义，是不变量，由 LICM pass 保证
    }

    void IndVarsSimplifyPass::insertInstructionAt(LLVMIR::IRBlock* block, LLVMIR::Instruction* inst, size_t position)
    {
        size_t phi_end = 0;
        for (size_t i = 0; i < block->insts.size(); ++i)
        {
            if (dynamic_cast<LLVMIR::PhiInst*>(block->insts[i]))
                phi_end = i + 1;
            else
                break;
        }

        size_t actual_position = std::max(phi_end, position);

        if (actual_position >= block->insts.size())
            block->insts.push_back(inst);
        else
        {
            auto it = block->insts.begin();
            std::advance(it, actual_position);
            block->insts.insert(it, inst);
        }
    }

    void IndVarsSimplifyPass::replaceUsesInLoop(CFG* cfg, NaturalLoop* loop, int old_reg, LLVMIR::Operand* new_operand)
    {
        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                auto used_regs         = inst->GetUsedRegs();
                bool needs_replacement = false;

                for (int reg : used_regs)
                {
                    if (reg == old_reg)
                    {
                        needs_replacement = true;
                        break;
                    }
                }

                if (needs_replacement)
                {
                    std::map<int, LLVMIR::Operand*> substitutions;
                    substitutions[old_reg] = new_operand;

                    inst->SubstituteOperands(substitutions);
                }
            }
        }
    }

    LLVMIR::PhiInst* IndVarsSimplifyPass::getPhiForCounter(CFG* cfg, NaturalLoop* loop, int reg_num)
    {
        std::set<int> visited;
        return getPhiForCounterRec(cfg, loop, reg_num, visited);
    }

    LLVMIR::PhiInst* IndVarsSimplifyPass::getPhiForCounterRec(
        CFG* cfg, NaturalLoop* loop, int reg_num, std::set<int>& visited)
    {
        if (visited.count(reg_num)) return nullptr;
        visited.insert(reg_num);

        for (auto* block : loop->loop_nodes)
        {
            for (auto* inst : block->insts)
            {
                if (inst->GetResultReg() == reg_num)
                {
                    auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
                    if (phi && block == loop->header) { return phi; }

                    auto* arith = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                    if (arith && (arith->opcode == LLVMIR::IROpCode::ADD || arith->opcode == LLVMIR::IROpCode::SUB))
                    {
                        auto* lhs_reg = dynamic_cast<LLVMIR::RegOperand*>(arith->lhs);
                        auto* rhs_reg = dynamic_cast<LLVMIR::RegOperand*>(arith->rhs);

                        if (lhs_reg && dynamic_cast<LLVMIR::ImmeI32Operand*>(arith->rhs))
                        {
                            auto* phi = getPhiForCounterRec(cfg, loop, lhs_reg->reg_num, visited);
                            if (phi) return phi;
                        }

                        if (rhs_reg && arith->opcode == LLVMIR::IROpCode::ADD &&
                            dynamic_cast<LLVMIR::ImmeI32Operand*>(arith->lhs))
                        {
                            auto* phi = getPhiForCounterRec(cfg, loop, rhs_reg->reg_num, visited);
                            if (phi) return phi;
                        }
                    }

                    return nullptr;
                }
            }
        }
        return nullptr;
    }

    LLVMIR::PhiInst* IndVarsSimplifyPass::findPhiInHeader(NaturalLoop* loop, int reg_num)
    {
        for (auto* inst : loop->header->insts)
        {
            auto* phi = dynamic_cast<LLVMIR::PhiInst*>(inst);
            if (phi && inst->GetResultReg() == reg_num) { return phi; }
        }
        return nullptr;
    }

    bool IndVarsSimplifyPass::isIVBasedExit(
        CFG* cfg, NaturalLoop* loop, LLVMIR::IcmpInst* icmp, LLVMIR::PhiInst* canonical_iv)
    {
        bool lhs_invariant = isLoopInvariant(icmp->lhs, loop);
        bool rhs_invariant = isLoopInvariant(icmp->rhs, loop);

        // 如果两个操作数都不是循环不变量，或者都是循环不变量，则不处理
        if (lhs_invariant == rhs_invariant) return false;

        LLVMIR::Operand* variant_operand   = lhs_invariant ? icmp->rhs : icmp->lhs;
        LLVMIR::Operand* invariant_operand = lhs_invariant ? icmp->lhs : icmp->rhs;

        auto* variant_reg = dynamic_cast<LLVMIR::RegOperand*>(variant_operand);
        if (!variant_reg) return false;

        auto* phi = getPhiForCounter(cfg, loop, variant_reg->reg_num);
        if (!phi) return false;

        // 仅当 PHI 就是 canonical IV，或者与 canonical IV 有简单关系，才做处理
        auto* canonical_reg = dynamic_cast<LLVMIR::RegOperand*>(canonical_iv->res);
        if (!canonical_reg) return false;

        // 如果就是canonical IV本身，可以处理
        if (phi == canonical_iv) return true;

        // TODO: 检查 PHI 是否是 canonical IV 的线性变换
        // 这一情况也应当允许，但目前未实现

        return false;
    }

}  // namespace Transform
