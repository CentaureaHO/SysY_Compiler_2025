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

            // Also check for unrolled memset patterns
            if (recognizeUnrolledMemsetPattern(cfg)) { unrolled_converted++; }
        }

        if (loops_converted > 0)
        {
            std::cout << "[MemsetPattern] Converted " << loops_converted << " loops to memset calls" << std::endl;
        }
        if (unrolled_converted > 0)
        {
            std::cout << "[MemsetPattern] Converted " << unrolled_converted << " unrolled sequences to memset calls"
                      << std::endl;
        }
    }

    void MemsetPatternRecognizePass::processFunction(CFG* cfg)
    {
        if (!cfg || !cfg->LoopForest) return;

        buildResultMap(cfg);

        std::function<void(NaturalLoop*)> processLoop = [&](NaturalLoop* loop) -> void {
            ++loops_processed;

            // Process nested loops first
            if (static_cast<size_t>(loop->loop_id) < cfg->LoopForest->loopG.size())
            {
                for (auto* child_loop : cfg->LoopForest->loopG[loop->loop_id]) { processLoop(child_loop); }
            }

            // Try to recognize memset pattern
            if (recognizeMemsetPattern(cfg, loop)) { ++loops_converted; }
        };

        for (auto* loop : cfg->LoopForest->getRootLoops()) { processLoop(loop); }
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
        // Basic loop structure checks - accept any loop structure as long as it has simple exit pattern
        if (loop->exit_nodes.size() != 1)
        {
            return false;  // Must have exactly one exit
        }

        auto* exit = *loop->exit_nodes.begin();

        // Check header has exactly 2 phi instructions
        std::vector<LLVMIR::PhiInst*> phi_insts;
        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI) { phi_insts.push_back(static_cast<LLVMIR::PhiInst*>(inst)); }
            else
            {
                break;  // Phi instructions should be at the beginning
            }
        }

        if (phi_insts.size() != 2) { return false; }

        // Identify counter phi and pointer phi
        LLVMIR::PhiInst* counter_phi = nullptr;
        LLVMIR::PhiInst* pointer_phi = nullptr;

        for (auto* phi : phi_insts)
        {
            if (phi->type == LLVMIR::DataType::I32)
            {
                // Check if this is a counter phi (has any constant initial value)
                bool is_counter = false;
                for (const auto& [val_op, label_op] : phi->vals_for_labels)
                {
                    if (val_op->type == LLVMIR::OperandType::IMMEI32)
                    {
                        is_counter = true;
                        break;
                    }
                }
                if (is_counter) { counter_phi = phi; }
            }
            else if (phi->type == LLVMIR::DataType::PTR) { pointer_phi = phi; }
        }

        if (!counter_phi || !pointer_phi) { return false; }

        // Find the store instruction
        LLVMIR::StoreInst* store_inst = nullptr;
        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                if (store_inst)
                {
                    // Multiple stores not allowed
                    return false;
                }
                store_inst = static_cast<LLVMIR::StoreInst*>(inst);
            }
        }

        if (!store_inst) { return false; }

        // Check if the store value is valid for memset (I8 range)
        if (!isValidMemsetValue(store_inst->val)) { return false; }

        // Check if store uses the pointer phi
        if (store_inst->ptr != pointer_phi->res) { return false; }

        // Check if loop is side-effect free (except for the store)
        if (!isLoopSideEffectFree(cfg, loop, store_inst)) { return false; }

        // Check that we have the right increment patterns
        if (!checkLoopIncrementPattern(cfg, loop, counter_phi, pointer_phi, store_inst)) { return false; }

        // Get loop upper bound
        LLVMIR::Operand* upper_bound = getLoopIterations(cfg, loop);
        if (!upper_bound) { return false; }

        // Transform the loop to memset
        transformLoopToMemset(cfg, loop, counter_phi, pointer_phi, store_inst, upper_bound, exit);

        // std::cout << "[MemsetPattern] Converted loop " << loop->loop_id << " with value " <<
        // store_inst->val->GetImm() << " to memset" << std::endl;
        return true;
    }

    bool MemsetPatternRecognizePass::isLoopSideEffectFree(CFG* cfg, NaturalLoop* loop, LLVMIR::StoreInst* allowed_store)
    {
        for (auto* node : loop->loop_nodes)
        {
            for (auto* inst : node->insts)
            {
                if (inst == allowed_store)
                {
                    continue;  // This store is allowed
                }

                // Check for side effects
                switch (inst->opcode)
                {
                    case LLVMIR::IROpCode::STORE: return false;  // Additional store not allowed
                    case LLVMIR::IROpCode::CALL: return false;   // Function calls not allowed
                    default: break;
                }
            }
        }
        return true;
    }

    bool MemsetPatternRecognizePass::isValidMemsetValue(LLVMIR::Operand* value)
    {
        if (value->type != LLVMIR::OperandType::IMMEI32) { return false; }

        // Now we can handle all i32 constant values using either memset or memset_i32
        return true;
    }

    bool MemsetPatternRecognizePass::canUseStandardMemset(LLVMIR::Operand* value)
    {
        if (value->type != LLVMIR::OperandType::IMMEI32) { return false; }

        int val = value->GetImm();

        // Special case: 0 is always valid for standard memset
        if (val == 0) { return true; }

        // Check if all 4 bytes of the i32 value are the same
        // This allows memset for patterns like 0x12121212, 0x00000000, etc.
        unsigned int  uval  = static_cast<unsigned int>(val);
        unsigned char byte0 = (uval >> 0) & 0xFF;
        unsigned char byte1 = (uval >> 8) & 0xFF;
        unsigned char byte2 = (uval >> 16) & 0xFF;
        unsigned char byte3 = (uval >> 24) & 0xFF;

        return (byte0 == byte1) && (byte1 == byte2) && (byte2 == byte3);
    }

    LLVMIR::Operand* MemsetPatternRecognizePass::getLoopIterations(CFG* cfg, NaturalLoop* loop)
    {
        // Find the counter phi instruction in the loop header
        LLVMIR::PhiInst* counter_phi = nullptr;
        for (auto* inst : loop->header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI)
            {
                auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                if (phi->type == LLVMIR::DataType::I32)
                {
                    // Check if this is a counter phi (has any constant initial value)
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
            else { break; }  // Phi instructions should be at the beginning
        }

        if (!counter_phi)
        {
            std::cout << "[DEBUG] No counter phi found in loop header" << std::endl;
            return nullptr;
        }

        // Find the comparison instruction in any exiting block
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

        if (!cmp_inst) { return nullptr; }

        // Only accept simple comparison conditions suitable for counting loops
        // Reject complex conditions like NE (!=) which are not simple increment patterns
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
                break;                   // These are acceptable for simple increment loops
            default:
                // Reject complex conditions like NE, EQ which indicate complex loop patterns
                std::cout << "[DEBUG] Rejecting loop with complex condition: " << (int)cmp_inst->cond << std::endl;
                return nullptr;
        }

        // Verify that one of the comparison operands is the counter or derived from the counter
        bool             counter_in_comparison = false;
        LLVMIR::Operand* upper_bound           = nullptr;

        // Check if lhs is the counter or derived from it
        if (cmp_inst->lhs->type == LLVMIR::OperandType::REG)
        {
            // Check if it's directly the counter
            if (cmp_inst->lhs == counter_phi->res)
            {
                counter_in_comparison = true;
                upper_bound           = cmp_inst->rhs;
            }
            else
            {
                // Check if it's derived from the counter (e.g., counter + 1)
                auto iter = result_map.find(cmp_inst->lhs->GetRegNum());
                if (iter != result_map.end())
                {
                    auto* def_inst = iter->second;
                    if (def_inst->opcode == LLVMIR::IROpCode::ADD)
                    {
                        auto* add_inst = static_cast<LLVMIR::ArithmeticInst*>(def_inst);
                        // Check if one operand is the counter and the other is a constant
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

        // Check if rhs is the counter or derived from it
        if (!counter_in_comparison && cmp_inst->rhs->type == LLVMIR::OperandType::REG)
        {
            if (cmp_inst->rhs == counter_phi->res)
            {
                counter_in_comparison = true;
                upper_bound           = cmp_inst->lhs;
            }
            else
            {
                // Check if it's derived from the counter (e.g., counter + 1)
                auto iter = result_map.find(cmp_inst->rhs->GetRegNum());
                if (iter != result_map.end())
                {
                    auto* def_inst = iter->second;
                    if (def_inst->opcode == LLVMIR::IROpCode::ADD)
                    {
                        auto* add_inst = static_cast<LLVMIR::ArithmeticInst*>(def_inst);
                        // Check if one operand is the counter and the other is a constant
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

        if (!counter_in_comparison)
        {
            std::cout << "[DEBUG] Comparison does not use loop counter - not a simple increment pattern" << std::endl;
            return nullptr;
        }

        // Extract the upper bound operand
        if (!upper_bound)
        {
            // Fallback logic for cases where both operands are registers
            if (cmp_inst->lhs->type == LLVMIR::OperandType::REG && cmp_inst->rhs->type != LLVMIR::OperandType::REG)
            {
                upper_bound = cmp_inst->rhs;
            }
            else if (cmp_inst->rhs->type == LLVMIR::OperandType::REG && cmp_inst->lhs->type != LLVMIR::OperandType::REG)
            {
                upper_bound = cmp_inst->lhs;
            }
            else if (cmp_inst->rhs->type == LLVMIR::OperandType::REG)
            {
                // Both are registers, assume rhs is the loop bound (more common pattern)
                upper_bound = cmp_inst->rhs;
            }
            else if (cmp_inst->lhs->type == LLVMIR::OperandType::REG)
            {
                // lhs is register, rhs might be constant
                upper_bound = cmp_inst->lhs;
            }
        }

        return upper_bound;  // In a real implementation, this would be the computed iteration count
    }

    bool MemsetPatternRecognizePass::checkLoopIncrementPattern(CFG* cfg, NaturalLoop* loop,
        LLVMIR::PhiInst* counter_phi, LLVMIR::PhiInst* pointer_phi, LLVMIR::StoreInst* store_inst)
    {
        // Check counter increment: counter_phi should be updated by counter + 1
        bool found_counter_increment = false;
        for (const auto& [val_op, label_op] : counter_phi->vals_for_labels)
        {
            // Skip non-register values (initial values)
            if (val_op->type != LLVMIR::OperandType::REG) { continue; }

            auto iter = result_map.find(val_op->GetRegNum());
            if (iter == result_map.end()) { continue; }

            auto* counter_update_inst = iter->second;
            if (!counter_update_inst || counter_update_inst->opcode != LLVMIR::IROpCode::ADD) { continue; }

            auto* add_inst = static_cast<LLVMIR::ArithmeticInst*>(counter_update_inst);
            // Check if it's counter + 1
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

        // Check pointer increment: pointer_phi should be updated by GEP with offset 1
        bool found_pointer_increment = false;
        for (const auto& [val_op, label_op] : pointer_phi->vals_for_labels)
        {
            // Skip non-register values (initial values)
            if (val_op->type != LLVMIR::OperandType::REG) { continue; }

            auto ptr_iter = result_map.find(val_op->GetRegNum());
            if (ptr_iter == result_map.end()) { continue; }

            auto* pointer_update_inst = ptr_iter->second;
            if (!pointer_update_inst || pointer_update_inst->opcode != LLVMIR::IROpCode::GETELEMENTPTR) { continue; }

            auto* gep_inst = static_cast<LLVMIR::GEPInst*>(pointer_update_inst);
            if (gep_inst->base_ptr == pointer_phi->res && gep_inst->idxs.size() == 1 &&
                gep_inst->idxs[0]->type == LLVMIR::OperandType::IMMEI32 && gep_inst->idxs[0]->GetImm() == 1)
            {
                found_pointer_increment = true;
                break;
            }
        }

        if (!found_pointer_increment) { return false; }

        // Check that we have an icmp and conditional branch in some loop block
        bool found_icmp    = false;
        bool found_br_cond = false;
        for (auto* loop_block : loop->loop_nodes)
        {
            for (auto* inst : loop_block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ICMP) { found_icmp = true; }
                if (inst->opcode == LLVMIR::IROpCode::BR_COND) { found_br_cond = true; }
            }
        }

        return found_icmp && found_br_cond;
    }

    void MemsetPatternRecognizePass::transformLoopToMemset(CFG* cfg, NaturalLoop* loop, LLVMIR::PhiInst* counter_phi,
        LLVMIR::PhiInst* pointer_phi, LLVMIR::StoreInst* store_inst, LLVMIR::Operand* upper_bound,
        LLVMIR::IRBlock* exit)
    {
        std::cout << "[DEBUG] Starting transformLoopToMemset" << std::endl;
        std::cout << "[DEBUG] Loop header: Block" << loop->header->block_id << std::endl;
        std::cout << "[DEBUG] Store value: " << store_inst->val->GetImm() << std::endl;

        // Get counter start value before clearing loops
        LLVMIR::Operand* start_value = nullptr;
        for (const auto& [val_op, label_op] : counter_phi->vals_for_labels)
        {
            if (val_op->type == LLVMIR::OperandType::IMMEI32)
            {
                start_value = val_op;
                std::cout << "[DEBUG] Found start value: " << start_value->GetImm() << std::endl;
                break;
            }
        }

        if (!start_value)
        {
            std::cout << "[DEBUG] ERROR: No start value found" << std::endl;
            return;
        }

        // If upper_bound is a register defined in the loop, we need to regenerate it
        LLVMIR::Operand* safe_upper_bound = upper_bound;
        if (upper_bound->type == LLVMIR::OperandType::REG)
        {
            auto iter = result_map.find(upper_bound->GetRegNum());
            if (iter != result_map.end())
            {
                auto* def_inst = iter->second;
                // Check if the defining instruction is in any loop block
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
                    // Clone the instruction to the preheader with a new register
                    LLVMIR::Operand* new_reg     = LLVMIR::RegOperand::get(++cfg->func->max_reg);
                    auto*            cloned_inst = def_inst->Clone(new_reg->GetRegNum());
                    safe_upper_bound             = new_reg;

                    // Insert before the branch in preheader
                    auto& preheader_insts = loop->preheader->insts;
                    preheader_insts.insert(preheader_insts.end() - 1, cloned_inst);
                    std::cout << "[DEBUG] Cloned upper bound instruction to preheader" << std::endl;
                }
            }
        }

        std::cout << "[DEBUG] Finding comparison instruction BEFORE clearing blocks" << std::endl;
        // Find the comparison instruction to determine the correct iteration count
        // CRITICAL: Do this BEFORE clearing loop blocks!
        LLVMIR::IcmpInst* cmp_inst = nullptr;

        // First try to find in exiting nodes (most common case)
        for (auto* exiting : loop->exiting_nodes)
        {
            for (auto* inst : exiting->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ICMP)
                {
                    cmp_inst = static_cast<LLVMIR::IcmpInst*>(inst);
                    std::cout << "[DEBUG] Found comparison in exiting node: ";
                    cmp_inst->printIR(std::cout);
                    break;
                }
            }
            if (cmp_inst) break;
        }

        // If not found in exiting nodes, search in all loop blocks
        if (!cmp_inst)
        {
            std::cout << "[DEBUG] No comparison in exiting nodes, searching all loop blocks" << std::endl;
            for (auto* loop_block : loop->loop_nodes)
            {
                for (auto* inst : loop_block->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::ICMP)
                    {
                        cmp_inst = static_cast<LLVMIR::IcmpInst*>(inst);
                        std::cout << "[DEBUG] Found comparison in loop block " << loop_block->block_id << ": ";
                        cmp_inst->printIR(std::cout);
                        break;
                    }
                }
                if (cmp_inst) break;
            }
        }

        if (!cmp_inst)
        {
            std::cout << "[DEBUG] ERROR: No comparison instruction found in any loop block" << std::endl;
            return;
        }

        std::cout << "[DEBUG] Getting base pointer" << std::endl;
        // Get base pointer (should be the non-loop value in the pointer phi)
        LLVMIR::Operand* base_pointer = nullptr;
        for (const auto& [val_op, label_op] : pointer_phi->vals_for_labels)
        {
            // Find a pointer value that's not a register (i.e., from outside the loop)
            if (val_op->type != LLVMIR::OperandType::REG)
            {
                base_pointer = val_op;
                std::cout << "[DEBUG] Found non-register base pointer" << std::endl;
                break;
            }
            // Or if it's a register, check if it's not produced by an instruction in the loop
            bool is_from_loop = false;
            if (val_op->type == LLVMIR::OperandType::REG)
            {
                auto iter = result_map.find(val_op->GetRegNum());
                if (iter != result_map.end())
                {
                    // Check if the instruction is in any loop block
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
                std::cout << "[DEBUG] Found register base pointer from outside loop" << std::endl;
                break;
            }
        }

        if (!base_pointer)
        {
            std::cout << "[DEBUG] ERROR: No base pointer found" << std::endl;
            return;
        }

        // CRITICAL BUG FIX: The comparison direction might be swapped!
        // We need to check if the upper_bound we got from getLoopIterations
        // matches the operands in the comparison instruction we just found
        bool operands_swapped = false;
        if (cmp_inst && safe_upper_bound->type == LLVMIR::OperandType::REG)
        {
            int upper_bound_reg = safe_upper_bound->GetRegNum();

            // Check if upper_bound matches lhs or rhs of the comparison
            if (cmp_inst->lhs->type == LLVMIR::OperandType::REG && cmp_inst->lhs->GetRegNum() == upper_bound_reg)
            {
                // upper_bound is lhs, so comparison is like "n <= i", operands are swapped
                operands_swapped = true;
                std::cout << "[DEBUG] Upper bound is lhs - operands swapped" << std::endl;
            }
            else if (cmp_inst->rhs->type == LLVMIR::OperandType::REG && cmp_inst->rhs->GetRegNum() == upper_bound_reg)
            {
                // upper_bound is rhs, so comparison is like "i <= n", normal order
                operands_swapped = false;
                std::cout << "[DEBUG] Upper bound is rhs - normal order" << std::endl;
            }
            else
            {
                std::cout << "[DEBUG] Upper bound register not found in comparison operands (this is normal - it's a "
                             "newly created register)"
                          << std::endl;
            }
        }
        else { std::cout << "[DEBUG] Cannot check operand order (cmp_inst or safe_upper_bound issue)" << std::endl; }

        // Determine if we need +1 based on the comparison condition
        bool needs_plus_one = false;
        if (cmp_inst)
        {
            // Consider the actual comparison direction based on operand order
            LLVMIR::IcmpCond effective_cond = cmp_inst->cond;

            // If operands are swapped (upper_bound is lhs), we need to flip the condition
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

            // Now determine +1 based on the effective condition
            // For loop continuation conditions:
            // - i <= n (continue while i <= n) → iterate from start to n inclusive → +1
            // - i < n (continue while i < n) → iterate from start to n-1 inclusive → no +1
            switch (effective_cond)
            {
                case LLVMIR::IcmpCond::SLE:  // i <= n (inclusive upper bound)
                case LLVMIR::IcmpCond::ULE:  // i <= n (unsigned)
                    needs_plus_one = true;
                    break;
                case LLVMIR::IcmpCond::SLT:  // i < n (exclusive upper bound)
                case LLVMIR::IcmpCond::ULT:  // i < n (unsigned)
                    needs_plus_one = false;
                    break;
                case LLVMIR::IcmpCond::SGE:  // n >= i (this is weird for a loop continuation condition)
                case LLVMIR::IcmpCond::UGE:  // n >= i (unsigned)
                case LLVMIR::IcmpCond::SGT:  // n > i
                case LLVMIR::IcmpCond::UGT:  // n > i (unsigned)
                    // These would be exit conditions rather than continuation conditions
                    // For safety, assume no +1
                    needs_plus_one = false;
                    break;
                default:
                    // For other conditions (==, !=), assume it's exclusive
                    needs_plus_one = false;
                    break;
            }

            std::cout << "[DEBUG] Final effective_cond: " << (int)effective_cond
                      << ", needs_plus_one: " << needs_plus_one << std::endl;
        }
        else { std::cout << "[DEBUG] No comparison instruction, defaulting needs_plus_one = false" << std::endl; }

        // Calculate actual loop iterations: safe_upper_bound - start_value (+ 1 for inclusive bounds)
        LLVMIR::Operand* actual_iterations = nullptr;
        if (start_value->type == LLVMIR::OperandType::IMMEI32 && safe_upper_bound->type == LLVMIR::OperandType::IMMEI32)
        {
            // Both are constants, calculate at compile time
            int start      = start_value->GetImm();
            int end        = safe_upper_bound->GetImm();
            int iterations = end - start;
            if (needs_plus_one) iterations += 1;

            std::cout << "[DEBUG] Constant calculation: start=" << start << ", end=" << end
                      << ", base_iterations=" << (end - start) << ", final_iterations=" << iterations << std::endl;

            actual_iterations = getImmeI32Operand(iterations);
        }
        else
        {
            // Need to generate subtraction instruction: (upper_bound - start_value [+ 1])
            LLVMIR::Operand* temp_reg = LLVMIR::RegOperand::get(++cfg->func->max_reg);
            auto*            sub_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::SUB, LLVMIR::DataType::I32, safe_upper_bound, start_value, temp_reg);

            // Insert calculation in preheader if available, otherwise we'll insert in header later
            if (loop->preheader != nullptr)
            {
                loop->preheader->insts.insert(loop->preheader->insts.end() - 1, sub_inst);
            }

            if (needs_plus_one)
            {
                LLVMIR::Operand* iterations_reg = LLVMIR::RegOperand::get(++cfg->func->max_reg);
                auto*            add_inst       = new LLVMIR::ArithmeticInst(
                    LLVMIR::IROpCode::ADD, LLVMIR::DataType::I32, temp_reg, getImmeI32Operand(1), iterations_reg);

                // Insert calculation in preheader if available
                if (loop->preheader != nullptr)
                {
                    loop->preheader->insts.insert(loop->preheader->insts.end() - 1, add_inst);
                }
                actual_iterations = iterations_reg;
            }
            else { actual_iterations = temp_reg; }

            // If no preheader, we need to insert these calculations in header before memset
            if (loop->preheader == nullptr)
            {
                // Store instructions to insert later
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

                // Clear header and insert calculation instructions first
                loop->header->insts.clear();
                for (auto* inst : calc_insts) { loop->header->insts.push_back(inst); }
            }
        }

        std::cout << "[DEBUG] Generated memset parameters, creating call" << std::endl;

        // If we have preheader, clear header now. If no preheader, header was already cleared above
        if (loop->preheader != nullptr) { loop->header->insts.clear(); }

        // Choose between standard memset and custom memset_i32
        LLVMIR::CallInst* memset_call = nullptr;
        if (canUseStandardMemset(store_inst->val))
        {
            // Use standard memset
            // Calculate byte count: actual_iterations * 4 (assuming i32 elements)
            // Use shift instead of multiply for performance: actual_iterations << 2
            LLVMIR::Operand* shift_amount   = getImmeI32Operand(2);
            LLVMIR::Operand* byte_count_reg = LLVMIR::RegOperand::get(++cfg->func->max_reg);

            auto* shift_inst = new LLVMIR::ArithmeticInst(
                LLVMIR::IROpCode::SHL, LLVMIR::DataType::I32, actual_iterations, shift_amount, byte_count_reg);

            // Convert store value to i8
            LLVMIR::Operand* i8_value = store_inst->val;
            if (store_inst->val->type == LLVMIR::OperandType::IMMEI32)
            {
                int val = store_inst->val->GetImm();
                // Since we only allow values where all 4 bytes are the same,
                // we can safely use any byte as the memset fill value
                i8_value = getImmeI32Operand(val & 0xFF);
            }

            // Create standard memset call
            std::vector<std::pair<LLVMIR::DataType, LLVMIR::Operand*>> memset_args = {
                {LLVMIR::DataType::PTR, base_pointer},
                {LLVMIR::DataType::I8, i8_value},
                {LLVMIR::DataType::I32, byte_count_reg}  // volatile = false
            };

            memset_call = new LLVMIR::CallInst(LLVMIR::DataType::VOID, "lsccll.lib.memset_i8", memset_args, nullptr);

            std::cout << "[DEBUG] Created standard memset call" << std::endl;

            // Insert size calculation in header
            loop->header->insts.push_back(shift_inst);

            // Insert memset call
            loop->header->insts.push_back(memset_call);
        }
        else
        {
            // Use custom memset_i32 for non-uniform byte patterns
            std::vector<std::pair<LLVMIR::DataType, LLVMIR::Operand*>> memset_i32_args = {
                {LLVMIR::DataType::PTR, base_pointer},
                {LLVMIR::DataType::I32, store_inst->val},
                {LLVMIR::DataType::I32, actual_iterations}};

            memset_call =
                new LLVMIR::CallInst(LLVMIR::DataType::VOID, "lsccll.lib.memset_i32", memset_i32_args, nullptr);

            std::cout << "[DEBUG] Created custom memset_i32 call" << std::endl;

            // Insert memset call directly
            loop->header->insts.push_back(memset_call);
        }

        // Add unconditional branch to exit from header
        auto* br_inst = new LLVMIR::BranchUncondInst(LLVMIR::LabelOperand::get(exit->block_id));
        loop->header->insts.push_back(br_inst);

        std::cout << "[DEBUG] Cleared loop header and added memset call with branch to exit" << std::endl;

        // Clear all other loop blocks and make them jump directly to exit
        for (auto* loop_block : loop->loop_nodes)
        {
            if (loop_block != loop->header)
            {
                std::cout << "[DEBUG] Clearing loop block " << loop_block->block_id << std::endl;
                loop_block->insts.clear();
                auto* block_br = new LLVMIR::BranchUncondInst(LLVMIR::LabelOperand::get(exit->block_id));
                loop_block->insts.push_back(block_br);
            }
        }

        std::cout << "[DEBUG] Successfully transformed loop to memset and cleared all loop blocks!" << std::endl;
    }

    std::pair<int, int> MemsetPatternRecognizePass::getRecognitionStats() const
    {
        return {loops_processed, loops_converted};
    }

    bool MemsetPatternRecognizePass::recognizeUnrolledMemsetPattern(CFG* cfg)
    {
        bool converted_any = false;

        // Process each basic block
        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            if (processUnrolledMemsetInBlock(block)) { converted_any = true; }
        }

        return converted_any;
    }

    bool MemsetPatternRecognizePass::processUnrolledMemsetInBlock(LLVMIR::IRBlock* block)
    {
        const int MIN_STORES    = 4;  // Minimum stores to consider for memset conversion
        bool      converted_any = false;

        auto& insts = block->insts;
        auto  it    = insts.begin();

        while (it != insts.end())
        {
            // Look for a store instruction
            if ((*it)->opcode != LLVMIR::IROpCode::STORE)
            {
                ++it;
                continue;
            }

            auto* first_store = static_cast<LLVMIR::StoreInst*>(*it);

            // Check if store value is valid for memset
            if (!isValidMemsetValue(first_store->val))
            {
                ++it;
                continue;
            }

            // Collect consecutive stores with the same value
            std::vector<LLVMIR::StoreInst*> store_sequence;
            std::vector<LLVMIR::StoreInst*> stores_to_remove;  // Only collect store instructions for removal

            store_sequence.push_back(first_store);
            stores_to_remove.push_back(first_store);

            // Look for pattern: getelementptr + store, getelementptr + store, ...
            auto             next_it       = std::next(it);
            LLVMIR::Operand* expected_base = first_store->ptr;

            while (next_it != insts.end())
            {
                // Expect: getelementptr followed by store
                auto gep_it   = next_it;
                auto store_it = std::next(gep_it);

                if (store_it != insts.end() && (*gep_it)->opcode == LLVMIR::IROpCode::GETELEMENTPTR &&
                    (*store_it)->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto* gep_inst   = static_cast<LLVMIR::GEPInst*>(*gep_it);
                    auto* store_inst = static_cast<LLVMIR::StoreInst*>(*store_it);

                    // Check if this continues the pattern
                    if (gep_inst->base_ptr == expected_base && gep_inst->idxs.size() == 1 &&
                        gep_inst->idxs[0]->type == LLVMIR::OperandType::IMMEI32 && gep_inst->idxs[0]->GetImm() == 1 &&
                        store_inst->ptr == gep_inst->res && store_inst->val->type == first_store->val->type &&
                        store_inst->val->GetImm() == first_store->val->GetImm())
                    {
                        store_sequence.push_back(store_inst);
                        // Only add store to removal list, leave GEP for DCE
                        stores_to_remove.push_back(store_inst);
                        expected_base = gep_inst->res;
                        next_it       = std::next(store_it);
                    }
                    else { break; }
                }
                else { break; }
            }

            // If we found enough consecutive stores, convert to memset
            if (store_sequence.size() >= MIN_STORES)
            {
                // Calculate memset parameters
                int count = store_sequence.size();

                // Use the base pointer from the first store
                // This ensures the memset uses a pointer that's already defined
                LLVMIR::Operand* base_ptr      = first_store->ptr;
                LLVMIR::Operand* store_value   = first_store->val;
                LLVMIR::Operand* count_operand = getImmeI32Operand(count);

                LLVMIR::CallInst* call_inst = nullptr;

                // Choose between standard memset and custom memset_i32
                if (canUseStandardMemset(store_value))
                {
                    // Use standard memset
                    // Create size operand
                    LLVMIR::Operand* size_bytes = getImmeI32Operand(count * 4);

                    // Convert store value to i8
                    LLVMIR::Operand* i8_value = store_value;
                    if (store_value->type == LLVMIR::OperandType::IMMEI32)
                    {
                        int val = store_value->GetImm();
                        // Since we only allow values where all 4 bytes are the same,
                        // we can safely use any byte as the memset fill value
                        i8_value = getImmeI32Operand(val & 0xFF);
                    }

                    // Create standard memset call
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
                    // Use custom memset_i32 for non-uniform byte patterns
                    std::vector<std::pair<LLVMIR::DataType, LLVMIR::Operand*>> memset_i32_args = {
                        {LLVMIR::DataType::PTR, base_ptr},
                        {LLVMIR::DataType::I32, store_value},
                        {LLVMIR::DataType::I32, count_operand}};

                    call_inst =
                        new LLVMIR::CallInst(LLVMIR::DataType::VOID, "lsccll.lib.memset_i32", memset_i32_args, nullptr);
                }

                // Insert memset call at the position of the first store
                // This ensures the memset appears after all necessary pointer definitions
                auto memset_it = insts.insert(it, call_inst);

                // Remove only store instructions, leave GEP instructions for DCE to handle
                // Use remove-erase idiom to safely remove by pointer comparison
                insts.erase(std::remove_if(insts.begin(),
                                insts.end(),
                                [&stores_to_remove](LLVMIR::Instruction* inst) {
                                    return std::find(stores_to_remove.begin(), stores_to_remove.end(), inst) !=
                                           stores_to_remove.end();
                                }),
                    insts.end());

                std::cout << "[MemsetPattern] Converted " << count
                          << " unrolled stores to memset, left GEP instructions for DCE" << std::endl;

                converted_any = true;

                // Reset iterator to continue from the beginning since we modified the container
                it = insts.begin();

                // Skip past the memset we just inserted
                while (it != insts.end() && *it != call_inst) { ++it; }
                if (it != insts.end()) { ++it; }
            }
            else { ++it; }
        }

        return converted_any;
    }

    bool MemsetPatternRecognizePass::instructionUsesRegister(LLVMIR::Instruction* inst, int reg_num)
    {
        // Use the built-in GetUsedRegs() method to check if the instruction uses the register
        auto used_regs = inst->GetUsedRegs();
        return std::find(used_regs.begin(), used_regs.end(), reg_num) != used_regs.end();
    }

}  // namespace Transform