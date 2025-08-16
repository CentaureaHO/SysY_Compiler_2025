#include "memset_pattern_recognize.h"
#include "llvm_ir/ir_builder.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <set>

namespace Transform
{
    MemsetPatternRecognizePass::MemsetPatternRecognizePass(LLVMIR::IR* ir) : Pass(ir) {}

    void MemsetPatternRecognizePass::Execute()
    {
        loops_processed        = 0;
        loops_converted        = 0;
        int unrolled_converted = 0;

        for (const auto& [func_def, cfg] : ir->cfg)
        {
            processFunction(cfg);

            if (recognizeUnrolledMemsetPattern(cfg)) unrolled_converted++;
        }
    }

    void MemsetPatternRecognizePass::processFunction(CFG* cfg)
    {
        if (!cfg || !cfg->LoopForest) return;

        buildResultMap(cfg);

        std::function<void(NaturalLoop*)> processLoop = [&](NaturalLoop* loop) -> void {
            ++loops_processed;

            if (static_cast<size_t>(loop->loop_id) < cfg->LoopForest->loopG.size())
            {
                for (auto* child_loop : cfg->LoopForest->loopG[loop->loop_id]) processLoop(child_loop);
            }
            if (recognizeMemsetPattern(cfg, loop)) ++loops_converted;
        };

        for (auto* loop : cfg->LoopForest->getRootLoops()) processLoop(loop);
    }

    void MemsetPatternRecognizePass::buildResultMap(CFG* cfg)
    {
        result_map.clear();

        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            for (auto* inst : bb->insts)
            {
                int result_reg = inst->GetResultReg();
                if (result_reg != -1) { result_map[result_reg] = inst; }
            }
        }
    }

    bool MemsetPatternRecognizePass::recognizeMemsetPattern(CFG* cfg, NaturalLoop* loop)
    {
        if (loop->exit_nodes.size() != 1) return false;

        auto* exit = *loop->exit_nodes.begin();

        std::vector<LLVMIR::PhiInst*> phi_insts;
        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI)
                phi_insts.push_back(static_cast<LLVMIR::PhiInst*>(inst));
            else
                break;
        }

        if (phi_insts.size() != 2) return false;

        LLVMIR::PhiInst* counter_phi = nullptr;
        LLVMIR::PhiInst* pointer_phi = nullptr;

        for (auto* phi : phi_insts)
        {
            if (phi->type == LLVMIR::DataType::I32)
            {
                bool is_counter = false;
                for (const auto& [val_op, label_op] : phi->vals_for_labels)
                {
                    if (val_op->type == LLVMIR::OperandType::IMMEI32)
                    {
                        is_counter = true;
                        break;
                    }
                }
                if (is_counter) counter_phi = phi;
            }
            else if (phi->type == LLVMIR::DataType::PTR)
                pointer_phi = phi;
        }

        if (!counter_phi || !pointer_phi) return false;

        LLVMIR::StoreInst* store_inst = nullptr;
        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                if (store_inst) return false;

                store_inst = static_cast<LLVMIR::StoreInst*>(inst);
            }
        }

        if (!store_inst) return false;

        if (!isValidMemsetValue(store_inst->val)) return false;
        if (store_inst->ptr != pointer_phi->res) return false;
        if (!isLoopSideEffectFree(cfg, loop, store_inst)) return false;
        if (!checkLoopIncrementPattern(cfg, loop, counter_phi, pointer_phi, store_inst)) return false;
        LLVMIR::Operand* upper_bound = getLoopIterations(cfg, loop);
        if (!upper_bound) return false;
        transformLoopToMemset(cfg, loop, counter_phi, pointer_phi, store_inst, upper_bound, exit);

        return true;
    }

    bool MemsetPatternRecognizePass::isLoopSideEffectFree(CFG* cfg, NaturalLoop* loop, LLVMIR::StoreInst* allowed_store)
    {
        for (auto* node : loop->loop_nodes)
        {
            for (auto* inst : node->insts)
            {
                if (inst == allowed_store) continue;

                switch (inst->opcode)
                {
                    case LLVMIR::IROpCode::STORE: return false;
                    case LLVMIR::IROpCode::CALL: return false;
                    default: break;
                }
            }
        }
        return true;
    }

    bool MemsetPatternRecognizePass::isValidMemsetValue(LLVMIR::Operand* value)
    {
        if (value->type != LLVMIR::OperandType::IMMEI32) return false;

        return true;
    }

    bool MemsetPatternRecognizePass::canUseStandardMemset(LLVMIR::Operand* value)
    {
        if (value->type != LLVMIR::OperandType::IMMEI32) return false;

        int val = value->GetImm();

        if (val == 0) return true;

        unsigned int  uval  = static_cast<unsigned int>(val);
        unsigned char byte0 = (uval >> 0) & 0xFF;
        unsigned char byte1 = (uval >> 8) & 0xFF;
        unsigned char byte2 = (uval >> 16) & 0xFF;
        unsigned char byte3 = (uval >> 24) & 0xFF;

        return (byte0 == byte1) && (byte1 == byte2) && (byte2 == byte3);
    }

    LLVMIR::Operand* MemsetPatternRecognizePass::getLoopIterations(CFG* cfg, NaturalLoop* loop)
    {
        LLVMIR::PhiInst* counter_phi = nullptr;
        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI)
            {
                auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                if (phi->type == LLVMIR::DataType::I32)
                {
                    for (const auto& [val_op, label_op] : phi->vals_for_labels)
                    {
                        if (val_op->type == LLVMIR::OperandType::IMMEI32)
                        {
                            counter_phi = phi;
                            break;
                        }
                    }
                    if (counter_phi) break;
                }
            }
            else
                break;
        }

        if (!counter_phi) return nullptr;

        LLVMIR::IcmpInst* cmp_inst = nullptr;

        for (auto* exiting : loop->exiting_nodes)
        {
            for (auto* inst : exiting->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ICMP)
                {
                    cmp_inst = static_cast<LLVMIR::IcmpInst*>(inst);
                    break;
                }
            }
            if (cmp_inst) break;
        }

        if (!cmp_inst) return nullptr;

        switch (cmp_inst->cond)
        {
            case LLVMIR::IcmpCond::SLT:  // i < n (signed)
            case LLVMIR::IcmpCond::SLE:  // i <= n (signed)
            case LLVMIR::IcmpCond::ULT:  // i < n (unsigned)
            case LLVMIR::IcmpCond::ULE:  // i <= n (unsigned)
            case LLVMIR::IcmpCond::SGT:  // n > i (signed)
            case LLVMIR::IcmpCond::SGE:  // n >= i (signed)
            case LLVMIR::IcmpCond::UGT:  // n > i (unsigned)
            case LLVMIR::IcmpCond::UGE:  // n >= i (unsigned)
                break;
            default: return nullptr;
        }

        bool             counter_in_comparison = false;
        LLVMIR::Operand* upper_bound           = nullptr;

        if (cmp_inst->lhs->type == LLVMIR::OperandType::REG)
        {
            if (cmp_inst->lhs == counter_phi->res)
            {
                counter_in_comparison = true;
                upper_bound           = cmp_inst->rhs;
            }
            else
            {
                auto iter = result_map.find(cmp_inst->lhs->GetRegNum());
                if (iter != result_map.end())
                {
                    auto* def_inst = iter->second;
                    if (def_inst->opcode == LLVMIR::IROpCode::ADD)
                    {
                        auto* add_inst = static_cast<LLVMIR::ArithmeticInst*>(def_inst);
                        if ((add_inst->lhs == counter_phi->res &&
                                add_inst->rhs->type == LLVMIR::OperandType::IMMEI32) ||
                            (add_inst->rhs == counter_phi->res && add_inst->lhs->type == LLVMIR::OperandType::IMMEI32))
                        {
                            counter_in_comparison = true;
                            upper_bound           = cmp_inst->rhs;
                        }
                    }
                }
            }
        }

        if (!counter_in_comparison && cmp_inst->rhs->type == LLVMIR::OperandType::REG)
        {
            if (cmp_inst->rhs == counter_phi->res)
            {
                counter_in_comparison = true;
                upper_bound           = cmp_inst->lhs;
            }
            else
            {
                auto iter = result_map.find(cmp_inst->rhs->GetRegNum());
                if (iter != result_map.end())
                {
                    auto* def_inst = iter->second;
                    if (def_inst->opcode == LLVMIR::IROpCode::ADD)
                    {
                        auto* add_inst = static_cast<LLVMIR::ArithmeticInst*>(def_inst);
                        if ((add_inst->lhs == counter_phi->res &&
                                add_inst->rhs->type == LLVMIR::OperandType::IMMEI32) ||
                            (add_inst->rhs == counter_phi->res && add_inst->lhs->type == LLVMIR::OperandType::IMMEI32))
                        {
                            counter_in_comparison = true;
                            upper_bound           = cmp_inst->lhs;
                        }
                    }
                }
            }
        }

        if (!counter_in_comparison) return nullptr;

        if (!upper_bound)
        {
            if (cmp_inst->lhs->type == LLVMIR::OperandType::REG && cmp_inst->rhs->type != LLVMIR::OperandType::REG)
            {
                upper_bound = cmp_inst->rhs;
            }
            else if (cmp_inst->rhs->type == LLVMIR::OperandType::REG && cmp_inst->lhs->type != LLVMIR::OperandType::REG)
            {
                upper_bound = cmp_inst->lhs;
            }
            else if (cmp_inst->rhs->type == LLVMIR::OperandType::REG) { upper_bound = cmp_inst->rhs; }
            else if (cmp_inst->lhs->type == LLVMIR::OperandType::REG) { upper_bound = cmp_inst->lhs; }
        }

        return upper_bound;
    }

    bool MemsetPatternRecognizePass::checkLoopIncrementPattern(CFG* cfg, NaturalLoop* loop,
        LLVMIR::PhiInst* counter_phi, LLVMIR::PhiInst* pointer_phi, LLVMIR::StoreInst* store_inst)
    {
        bool found_counter_increment = false;
        for (const auto& [val_op, label_op] : counter_phi->vals_for_labels)
        {
            if (val_op->type != LLVMIR::OperandType::REG) continue;

            auto iter = result_map.find(val_op->GetRegNum());
            if (iter == result_map.end()) continue;

            auto* counter_update_inst = iter->second;
            if (!counter_update_inst || counter_update_inst->opcode != LLVMIR::IROpCode::ADD) continue;

            auto* add_inst = static_cast<LLVMIR::ArithmeticInst*>(counter_update_inst);

            if ((add_inst->lhs == counter_phi->res && add_inst->rhs->type == LLVMIR::OperandType::IMMEI32 &&
                    add_inst->rhs->GetImm() == 1) ||
                (add_inst->rhs == counter_phi->res && add_inst->lhs->type == LLVMIR::OperandType::IMMEI32 &&
                    add_inst->lhs->GetImm() == 1))
            {
                found_counter_increment = true;
                break;
            }
        }

        if (!found_counter_increment) { return false; }

        bool found_pointer_increment = false;
        for (const auto& [val_op, label_op] : pointer_phi->vals_for_labels)
        {
            if (val_op->type != LLVMIR::OperandType::REG) continue;

            auto ptr_iter = result_map.find(val_op->GetRegNum());
            if (ptr_iter == result_map.end()) continue;

            auto* pointer_update_inst = ptr_iter->second;
            if (!pointer_update_inst || pointer_update_inst->opcode != LLVMIR::IROpCode::GETELEMENTPTR) continue;

            auto* gep_inst = static_cast<LLVMIR::GEPInst*>(pointer_update_inst);
            if (gep_inst->base_ptr == pointer_phi->res && gep_inst->idxs.size() == 1 &&
                gep_inst->idxs[0]->type == LLVMIR::OperandType::IMMEI32 && gep_inst->idxs[0]->GetImm() == 1)
            {
                found_pointer_increment = true;
                break;
            }
        }

        if (!found_pointer_increment) { return false; }

        bool found_icmp    = false;
        bool found_br_cond = false;
        for (auto* loop_block : loop->loop_nodes)
        {
            for (auto* inst : loop_block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ICMP) found_icmp = true;
                if (inst->opcode == LLVMIR::IROpCode::BR_COND) found_br_cond = true;
            }
        }

        return found_icmp && found_br_cond;
    }

    void MemsetPatternRecognizePass::transformLoopToMemset(CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* counter_phi,
        LLVMIR::PhiInst* pointer_phi, LLVMIR::StoreInst* store_inst, LLVMIR::Operand* upper_bound,
        LLVMIR::IRBlock* exit)
    {
        LLVMIR::Operand* start_value = nullptr;
        for (const auto& [val_op, label_op] : counter_phi->vals_for_labels)
        {
            if (val_op->type == LLVMIR::OperandType::IMMEI32)
            {
                start_value = val_op;
                break;
            }
        }

        if (!start_value) return;

        LLVMIR::Operand* safe_upper_bound = upper_bound;
        if (upper_bound->type == LLVMIR::OperandType::REG)
        {
            auto iter = result_map.find(upper_bound->GetRegNum());
            if (iter != result_map.end())
            {
                auto* def_inst = iter->second;

                bool is_in_loop = false;
                for (auto* loop_block : loop->loop_nodes)
                {
                    for (auto* inst : loop_block->insts)
                    {
                        if (inst == def_inst)
                        {
                            is_in_loop = true;
                            break;
                        }
                    }
                    if (is_in_loop) break;
                }

                if (is_in_loop)
                {
                    LLVMIR::Operand* new_reg     = LLVMIR::RegOperand::get(++cfg->func->max_reg);
                    auto*            cloned_inst = def_inst->Clone(new_reg->GetRegNum());
                    safe_upper_bound             = new_reg;

                    auto& preheader_insts = loop->preheader->insts;
                    preheader_insts.insert(preheader_insts.end() - 1, cloned_inst);
                }
            }
        }

        LLVMIR::IcmpInst* cmp_inst = nullptr;

        for (auto* exiting : loop->exiting_nodes)
        {
            for (auto* inst : exiting->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ICMP)
                {
                    cmp_inst = static_cast<LLVMIR::IcmpInst*>(inst);
                    cmp_inst->printIR(std::cout);
                    break;
                }
            }
            if (cmp_inst) break;
        }

        if (!cmp_inst)
        {
            for (auto* loop_block : loop->loop_nodes)
            {
                for (auto* inst : loop_block->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::ICMP)
                    {
                        cmp_inst = static_cast<LLVMIR::IcmpInst*>(inst);
                        break;
                    }
                }
                if (cmp_inst) break;
            }
        }

        if (!cmp_inst) return;

        LLVMIR::Operand* base_pointer = nullptr;
        for (const auto& [val_op, label_op] : pointer_phi->vals_for_labels)
        {
            if (val_op->type != LLVMIR::OperandType::REG)
            {
                base_pointer = val_op;
                break;
            }
            bool is_from_loop = false;
            if (val_op->type == LLVMIR::OperandType::REG)
            {
                auto iter = result_map.find(val_op->GetRegNum());
                if (iter != result_map.end())
                {
                    for (auto* loop_block : loop->loop_nodes)
                    {
                        for (auto* inst : loop_block->insts)
                        {
                            if (inst == iter->second)
                            {
                                is_from_loop = true;
                                break;
                            }
                        }
                        if (is_from_loop) break;
                    }
                }
            }
            if (!is_from_loop)
            {
                base_pointer = val_op;
                break;
            }
        }

        if (!base_pointer) return;

        bool operands_swapped = false;
        if (cmp_inst && safe_upper_bound->type == LLVMIR::OperandType::REG)
        {
            int upper_bound_reg = safe_upper_bound->GetRegNum();

            if (cmp_inst->lhs->type == LLVMIR::OperandType::REG && cmp_inst->lhs->GetRegNum() == upper_bound_reg)
                operands_swapped = true;
            else if (cmp_inst->rhs->type == LLVMIR::OperandType::REG && cmp_inst->rhs->GetRegNum() == upper_bound_reg)
                operands_swapped = false;
        }

        bool needs_plus_one = false;
        if (cmp_inst)
        {
            LLVMIR::IcmpCond effective_cond = cmp_inst->cond;

            if (operands_swapped)
            {
                switch (cmp_inst->cond)
                {
                    case LLVMIR::IcmpCond::SLE: effective_cond = LLVMIR::IcmpCond::SGE; break;
                    case LLVMIR::IcmpCond::SLT: effective_cond = LLVMIR::IcmpCond::SGT; break;
                    case LLVMIR::IcmpCond::SGE: effective_cond = LLVMIR::IcmpCond::SLE; break;
                    case LLVMIR::IcmpCond::SGT: effective_cond = LLVMIR::IcmpCond::SLT; break;
                    case LLVMIR::IcmpCond::ULE: effective_cond = LLVMIR::IcmpCond::UGE; break;
                    case LLVMIR::IcmpCond::ULT: effective_cond = LLVMIR::IcmpCond::UGT; break;
                    case LLVMIR::IcmpCond::UGE: effective_cond = LLVMIR::IcmpCond::ULE; break;
                    case LLVMIR::IcmpCond::UGT: effective_cond = LLVMIR::IcmpCond::ULT; break;
                    default: break;
                }
            }

            switch (effective_cond)
            {
                case LLVMIR::IcmpCond::SLE:
                case LLVMIR::IcmpCond::ULE: needs_plus_one = true; break;
                case LLVMIR::IcmpCond::SLT:
                case LLVMIR::IcmpCond::ULT: needs_plus_one = false; break;
                case LLVMIR::IcmpCond::SGE:
                case LLVMIR::IcmpCond::UGE:
                case LLVMIR::IcmpCond::SGT:
                case LLVMIR::IcmpCond::UGT: needs_plus_one = false; break;
                default: needs_plus_one = false; break;
            }
        }

        LLVMIR::Operand* actual_iterations = nullptr;
        if (start_value->type == LLVMIR::OperandType::IMMEI32 && safe_upper_bound->type == LLVMIR::OperandType::IMMEI32)
        {
            int start      = start_value->GetImm();
            int end        = safe_upper_bound->GetImm();
            int iterations = end - start;
            if (needs_plus_one) iterations += 1;

            actual_iterations = getImmeI32Operand(iterations);
        }
        else
        {
            LLVMIR::Operand* temp_reg = LLVMIR::RegOperand::get(++cfg->func->max_reg);
            auto*            sub_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::SUB, LLVMIR::DataType::I32, safe_upper_bound, start_value, temp_reg);

            if (loop->preheader != nullptr) loop->preheader->insts.insert(loop->preheader->insts.end() - 1, sub_inst);

            if (needs_plus_one)
            {
                LLVMIR::Operand* iterations_reg = LLVMIR::RegOperand::get(++cfg->func->max_reg);
                auto*            add_inst       = new LLVMIR::ArithmeticInst(
                    LLVMIR::IROpCode::ADD, LLVMIR::DataType::I32, temp_reg, getImmeI32Operand(1), iterations_reg);

                if (loop->preheader != nullptr)
                    loop->preheader->insts.insert(loop->preheader->insts.end() - 1, add_inst);

                actual_iterations = iterations_reg;
            }
            else
                actual_iterations = temp_reg;

            if (loop->preheader == nullptr)
            {
                std::vector<LLVMIR::Instruction*> calc_insts;
                calc_insts.push_back(sub_inst);
                if (needs_plus_one && actual_iterations != temp_reg)
                {
                    auto* add_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                        LLVMIR::DataType::I32,
                        temp_reg,
                        getImmeI32Operand(1),
                        actual_iterations);
                    calc_insts.push_back(add_inst);
                }

                loop->header->insts.clear();
                for (auto* inst : calc_insts) loop->header->insts.push_back(inst);
            }
        }

        if (loop->preheader != nullptr) loop->header->insts.clear();
        LLVMIR::CallInst* memset_call = nullptr;
        if (canUseStandardMemset(store_inst->val))
        {
            LLVMIR::Operand* shift_amount   = getImmeI32Operand(2);
            LLVMIR::Operand* byte_count_reg = LLVMIR::RegOperand::get(++cfg->func->max_reg);

            auto* shift_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::SHL, LLVMIR::DataType::I32, actual_iterations, shift_amount, byte_count_reg);

            LLVMIR::Operand* i8_value = store_inst->val;
            if (store_inst->val->type == LLVMIR::OperandType::IMMEI32)
            {
                int val  = store_inst->val->GetImm();
                i8_value = getImmeI32Operand(val & 0xFF);
            }

            std::vector<std::pair<LLVMIR::DataType, LLVMIR::Operand*>> memset_args = {
                {LLVMIR::DataType::PTR, base_pointer},
                {LLVMIR::DataType::I8, i8_value},
                {LLVMIR::DataType::I32, byte_count_reg}};

            memset_call = new LLVMIR::CallInst(LLVMIR::DataType::VOID, "lsccll.lib.memset_i8", memset_args, nullptr);
            loop->header->insts.push_back(shift_inst);
            loop->header->insts.push_back(memset_call);
        }
        else
        {
            std::vector<std::pair<LLVMIR::DataType, LLVMIR::Operand*>> memset_i32_args = {
                {LLVMIR::DataType::PTR, base_pointer},
                {LLVMIR::DataType::I32, store_inst->val},
                {LLVMIR::DataType::I32, actual_iterations}};

            memset_call =
                new LLVMIR::CallInst(LLVMIR::DataType::VOID, "lsccll.lib.memset_i32", memset_i32_args, nullptr);

            loop->header->insts.push_back(memset_call);
        }

        auto* br_inst = new LLVMIR::BranchUncondInst(LLVMIR::LabelOperand::get(exit->block_id));
        loop->header->insts.push_back(br_inst);

        for (auto* loop_block : loop->loop_nodes)
        {
            if (loop_block != loop->header)
            {
                loop_block->insts.clear();
                auto* block_br = new LLVMIR::BranchUncondInst(LLVMIR::LabelOperand::get(exit->block_id));
                loop_block->insts.push_back(block_br);
            }
        }
    }

    std::pair<int, int> MemsetPatternRecognizePass::getRecognitionStats() const
    {
        return {loops_processed, loops_converted};
    }

    bool MemsetPatternRecognizePass::recognizeUnrolledMemsetPattern(CFG* cfg)
    {
        bool converted_any = false;

        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            if (processUnrolledMemsetInBlock(block)) { converted_any = true; }
        }

        return converted_any;
    }

    bool MemsetPatternRecognizePass::processUnrolledMemsetInBlock(LLVMIR::IRBlock* block)
    {
        const int MIN_STORES    = 4;
        bool      converted_any = false;

        auto& insts = block->insts;
        auto  it    = insts.begin();

        while (it != insts.end())
        {
            if ((*it)->opcode != LLVMIR::IROpCode::STORE)
            {
                ++it;
                continue;
            }

            auto* first_store = static_cast<LLVMIR::StoreInst*>(*it);

            if (!isValidMemsetValue(first_store->val))
            {
                ++it;
                continue;
            }

            std::vector<LLVMIR::StoreInst*> store_sequence;
            std::vector<LLVMIR::StoreInst*> stores_to_remove;

            store_sequence.push_back(first_store);
            stores_to_remove.push_back(first_store);

            auto             next_it       = std::next(it);
            LLVMIR::Operand* expected_base = first_store->ptr;

            while (next_it != insts.end())
            {
                auto gep_it   = next_it;
                auto store_it = std::next(gep_it);

                if (store_it != insts.end() && (*gep_it)->opcode == LLVMIR::IROpCode::GETELEMENTPTR &&
                    (*store_it)->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto* gep_inst   = static_cast<LLVMIR::GEPInst*>(*gep_it);
                    auto* store_inst = static_cast<LLVMIR::StoreInst*>(*store_it);

                    if (gep_inst->base_ptr == expected_base && gep_inst->idxs.size() == 1 &&
                        gep_inst->idxs[0]->type == LLVMIR::OperandType::IMMEI32 && gep_inst->idxs[0]->GetImm() == 1 &&
                        store_inst->ptr == gep_inst->res && store_inst->val->type == first_store->val->type &&
                        store_inst->val->GetImm() == first_store->val->GetImm())
                    {
                        store_sequence.push_back(store_inst);
                        stores_to_remove.push_back(store_inst);
                        expected_base = gep_inst->res;
                        next_it       = std::next(store_it);
                    }
                    else
                        break;
                }
                else
                    break;
            }

            if (store_sequence.size() >= MIN_STORES)
            {
                int count = store_sequence.size();

                LLVMIR::Operand* base_ptr      = first_store->ptr;
                LLVMIR::Operand* store_value   = first_store->val;
                LLVMIR::Operand* count_operand = getImmeI32Operand(count);

                LLVMIR::CallInst* call_inst = nullptr;

                if (canUseStandardMemset(store_value))
                {
                    LLVMIR::Operand* size_bytes = getImmeI32Operand(count * 4);

                    LLVMIR::Operand* i8_value = store_value;
                    if (store_value->type == LLVMIR::OperandType::IMMEI32)
                    {
                        int val  = store_value->GetImm();
                        i8_value = getImmeI32Operand(val & 0xFF);
                    }

                    std::vector<std::pair<LLVMIR::DataType, LLVMIR::Operand*>> memset_args = {
                        {LLVMIR::DataType::PTR, base_ptr},
                        {LLVMIR::DataType::I8, i8_value},
                        {LLVMIR::DataType::I32, size_bytes},
                    };

                    call_inst =
                        new LLVMIR::CallInst(LLVMIR::DataType::VOID, "lsccll.lib.memset_i8", memset_args, nullptr);
                }
                else
                {
                    std::vector<std::pair<LLVMIR::DataType, LLVMIR::Operand*>> memset_i32_args = {
                        {LLVMIR::DataType::PTR, base_ptr},
                        {LLVMIR::DataType::I32, store_value},
                        {LLVMIR::DataType::I32, count_operand}};

                    call_inst =
                        new LLVMIR::CallInst(LLVMIR::DataType::VOID, "lsccll.lib.memset_i32", memset_i32_args, nullptr);
                }

                auto memset_it = insts.insert(it, call_inst);

                insts.erase(std::remove_if(insts.begin(),
                                insts.end(),
                                [&stores_to_remove](LLVMIR::Instruction* inst) {
                                    return std::find(stores_to_remove.begin(), stores_to_remove.end(), inst) !=
                                           stores_to_remove.end();
                                }),
                    insts.end());

                converted_any = true;

                it = insts.begin();

                while (it != insts.end() && *it != call_inst) ++it;
                if (it != insts.end()) ++it;
            }
            else { ++it; }
        }

        return converted_any;
    }

    bool MemsetPatternRecognizePass::instructionUsesRegister(LLVMIR::Instruction* inst, int reg_num)
    {
        auto used_regs = inst->GetUsedRegs();
        return std::find(used_regs.begin(), used_regs.end(), reg_num) != used_regs.end();
    }

}  // namespace Transform
