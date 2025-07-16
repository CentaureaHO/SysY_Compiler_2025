#include "llvm/tail_recursion.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"
#include <algorithm>
#include <memory>
#include <queue>

namespace Transform
{
    ParameterAnalyzer::ParameterAnalyzer(CFG* cfg) : target_cfg(cfg) {}

    bool ParameterAnalyzer::isTransformationValid()
    {
        if (!validateParameterConstraints()) return false;
        if (!checkArrayParameterUsage()) return false;
        return true;
    }

    bool ParameterAnalyzer::validateParameterConstraints()
    {
        auto func_signature = target_cfg->func->func_def;

        if (func_signature->arg_types.size() > 5) return false;

        return true;
    }

    bool ParameterAnalyzer::checkArrayParameterUsage()
    {
        std::unordered_map<int, bool> array_allocations;
        std::unordered_set<int>       gep_derived_pointers;

        auto entry_block = target_cfg->block_id_to_block.at(0);

        for (auto instruction : entry_block->insts)
        {
            if (instruction->opcode != LLVMIR::IROpCode::ALLOCA) continue;

            auto alloc_inst = dynamic_cast<LLVMIR::AllocInst*>(instruction);
            if (!alloc_inst || alloc_inst->dims.empty()) continue;

            array_allocations[alloc_inst->GetResultReg()] = true;
        }

        if (array_allocations.empty()) return true;
        for (auto const& [block_id, block] : target_cfg->block_id_to_block)
        {
            for (auto instruction : block->insts)
            {
                if (instruction->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                {
                    auto gep_inst = dynamic_cast<LLVMIR::GEPInst*>(instruction);
                    auto base_ptr = dynamic_cast<LLVMIR::RegOperand*>(gep_inst->base_ptr);

                    if (base_ptr && array_allocations.count(base_ptr->reg_num))
                    {
                        gep_derived_pointers.insert(gep_inst->GetResultReg());
                    }
                }
                else if (instruction->opcode == LLVMIR::IROpCode::CALL)
                {
                    auto call_inst = dynamic_cast<LLVMIR::CallInst*>(instruction);

                    for (auto const& [arg_type, arg_operand] : call_inst->args)
                    {
                        auto arg_reg = dynamic_cast<LLVMIR::RegOperand*>(arg_operand);
                        if (arg_reg && gep_derived_pointers.count(arg_reg->reg_num)) return false;
                    }
                }
            }
        }

        return true;
    }

    std::vector<ParameterAnalyzer::ParameterInfo> ParameterAnalyzer::analyzeParameters()
    {
        std::vector<ParameterInfo> param_info;
        auto                       func_signature = target_cfg->func->func_def;

        for (size_t i = 0; i < func_signature->arg_types.size(); ++i)
        {
            ParameterInfo info;
            info.original_index    = static_cast<int>(i);
            info.is_pointer_type   = (func_signature->arg_types[i] == LLVMIR::DataType::PTR);
            info.requires_spilling = false;
            info.spill_location    = nullptr;

            param_info.push_back(info);
        }

        return param_info;
    }

    TailRecursionOptimizer::TailRecursionOptimizer(CFG* cfg)
        : target_cfg(cfg), param_analyzer(std::make_unique<ParameterAnalyzer>(cfg))
    {}

    bool TailRecursionOptimizer::canOptimize() { return param_analyzer->isTransformationValid(); }

    void TailRecursionOptimizer::performOptimization()
    {
        if (!canOptimize()) return;

        discoverTailCalls();
        if (discovered_tail_calls.empty()) return;

        setupParameterSpilling();
        rewriteTailCalls();
        finalizeTransformation();
    }

    void TailRecursionOptimizer::discoverTailCalls()
    {
        auto function_name = target_cfg->func->func_def->func_name;

        for (auto const& [block_id, block] : target_cfg->block_id_to_block)
        {
            if (block->insts.empty()) continue;

            auto last_inst = block->insts.back();
            if (last_inst->opcode != LLVMIR::IROpCode::RET) continue;

            auto return_inst = dynamic_cast<LLVMIR::RetInst*>(last_inst);

            LLVMIR::CallInst* potential_tail_call = nullptr;
            for (auto it = block->insts.rbegin(); it != block->insts.rend(); ++it)
            {
                if ((*it)->opcode != LLVMIR::IROpCode::CALL) continue;

                potential_tail_call = dynamic_cast<LLVMIR::CallInst*>(*it);
                break;
            }

            if (!potential_tail_call || potential_tail_call->func_name != function_name) continue;

            bool is_tail_call = false;
            if (potential_tail_call->res && return_inst->ret)
                is_tail_call = (potential_tail_call->res->getName() == return_inst->ret->getName());
            else if (!potential_tail_call->res && return_inst->ret_type == LLVMIR::DataType::VOID)
                is_tail_call = true;

            if (!is_tail_call) continue;

            TailCallInfo call_info;
            call_info.call_instruction   = potential_tail_call;
            call_info.containing_block   = block;
            call_info.is_valid_tail_call = true;

            for (size_t i = 0; i < potential_tail_call->args.size(); ++i)
                call_info.parameter_mappings.emplace_back(static_cast<int>(i), potential_tail_call->args[i].second);

            discovered_tail_calls.push_back(call_info);
            instructions_to_remove.insert(potential_tail_call);
            instructions_to_remove.insert(return_inst);
        }
    }

    void TailRecursionOptimizer::setupParameterSpilling()
    {
        auto                              param_info  = param_analyzer->analyzeParameters();
        auto                              entry_block = target_cfg->block_id_to_block.at(0);
        std::vector<LLVMIR::Instruction*> spill_setup_instructions;

        std::unordered_map<int, bool> param_needs_spilling;
        for (const auto& tail_call : discovered_tail_calls)
            for (const auto& [param_idx, operand] : tail_call.parameter_mappings)
            {
                auto reg_operand = dynamic_cast<LLVMIR::RegOperand*>(operand);
                if (reg_operand && reg_operand->reg_num != param_idx) { param_needs_spilling[param_idx] = true; }
            }

        for (size_t i = 0; i < param_info.size(); ++i)
            if (param_info[i].is_pointer_type && param_needs_spilling[i])
            {
                auto spill_reg  = getRegOperand(++target_cfg->func->max_reg);
                auto alloc_inst = new LLVMIR::AllocInst(LLVMIR::DataType::PTR, spill_reg);
                auto store_inst =
                    new LLVMIR::StoreInst(LLVMIR::DataType::PTR, spill_reg, target_cfg->func->func_def->arg_regs[i]);

                spill_setup_instructions.push_back(alloc_inst);
                spill_setup_instructions.push_back(store_inst);

                param_info[i].spill_location = spill_reg;
            }

        for (auto it = spill_setup_instructions.rbegin(); it != spill_setup_instructions.rend(); ++it)
            entry_block->insts.push_front(*it);

        for (auto const& [block_id, block] : target_cfg->block_id_to_block)
        {
            if (block_id == 0) continue;

            std::vector<LLVMIR::Instruction*> updated_instructions;
            for (auto inst : block->insts)
            {
                auto used_regs    = inst->GetUsedRegs();
                bool needs_update = false;

                for (size_t reg_idx = 0; reg_idx < used_regs.size(); ++reg_idx)
                {
                    for (size_t param_idx = 0; param_idx < param_info.size(); ++param_idx)
                    {
                        if (!param_info[param_idx].spill_location) continue;

                        auto arg_reg =
                            dynamic_cast<LLVMIR::RegOperand*>(target_cfg->func->func_def->arg_regs[param_idx]);
                        if (!arg_reg || used_regs[reg_idx] != arg_reg->reg_num) continue;

                        auto load_reg = getRegOperand(++target_cfg->func->max_reg);
                        auto load_inst =
                            new LLVMIR::LoadInst(LLVMIR::DataType::PTR, param_info[param_idx].spill_location, load_reg);
                        updated_instructions.push_back(load_inst);

                        register_remapping[arg_reg->reg_num] = load_reg->reg_num;
                        needs_update                         = true;
                        break;
                    }
                }

                if (needs_update)
                {
                    std::map<int, int> rename_map(register_remapping.begin(), register_remapping.end());
                    inst->Rename(rename_map);
                }
                updated_instructions.push_back(inst);
            }

            block->insts = std::deque<LLVMIR::Instruction*>(updated_instructions.begin(), updated_instructions.end());
        }
    }

    void TailRecursionOptimizer::rewriteTailCalls()
    {
        auto entry_block = target_cfg->block_id_to_block.at(0);

        for (const auto& tail_call : discovered_tail_calls)
        {
            auto call_block = tail_call.containing_block;

            for (const auto& [param_idx, new_value] : tail_call.parameter_mappings)
            {
                auto call_reg = dynamic_cast<LLVMIR::RegOperand*>(new_value);
                if (!call_reg || call_reg->reg_num == param_idx) continue;

                LLVMIR::AllocInst* target_alloc = nullptr;
                for (auto inst : entry_block->insts)
                {
                    if (inst->opcode != LLVMIR::IROpCode::ALLOCA) continue;

                    auto alloc = dynamic_cast<LLVMIR::AllocInst*>(inst);
                    if (!alloc) continue;

                    bool matches_param = false;
                    for (auto store_inst : entry_block->insts)
                    {
                        if (store_inst->opcode != LLVMIR::IROpCode::STORE) continue;

                        auto store = dynamic_cast<LLVMIR::StoreInst*>(store_inst);
                        if (store && store->ptr != alloc->res) continue;

                        auto stored_arg = dynamic_cast<LLVMIR::RegOperand*>(store->val);
                        auto param_arg =
                            dynamic_cast<LLVMIR::RegOperand*>(target_cfg->func->func_def->arg_regs[param_idx]);
                        if (stored_arg && param_arg && stored_arg->reg_num == param_arg->reg_num)
                        {
                            matches_param = true;
                            break;
                        }
                    }
                    if (matches_param)
                    {
                        target_alloc = alloc;
                        break;
                    }
                }

                if (target_alloc)
                {
                    auto store_inst = new LLVMIR::StoreInst(
                        tail_call.call_instruction->args[param_idx].first, target_alloc->res, new_value);
                    call_block->insts.push_back(store_inst);
                }
            }

            auto loop_branch = new LLVMIR::BranchUncondInst(getLabelOperand(1));
            call_block->insts.push_back(loop_branch);
        }
    }

    void TailRecursionOptimizer::finalizeTransformation()
    {
        for (auto const& [block_id, block] : target_cfg->block_id_to_block)
        {
            block->insts.erase(
                std::remove_if(block->insts.begin(),
                    block->insts.end(),
                    [this](LLVMIR::Instruction* inst) { return instructions_to_remove.count(inst) > 0; }),
                block->insts.end());
        }

        std::unordered_set<LLVMIR::Instruction*> skip_instructions;
        auto                                     entry_block = target_cfg->block_id_to_block.at(0);
        for (auto inst : entry_block->insts)
            if (inst->opcode == LLVMIR::IROpCode::ALLOCA || inst->opcode == LLVMIR::IROpCode::STORE)
                skip_instructions.insert(inst);

        for (auto const& [block_id, block] : target_cfg->block_id_to_block)
            for (auto inst : block->insts)
                if (skip_instructions.count(inst) == 0)
                {
                    std::map<int, int> rename_map(register_remapping.begin(), register_remapping.end());
                    inst->Rename(rename_map);
                }

        target_cfg->BuildCFG();
    }

    TailRecursionPass::TailRecursionPass(LLVMIR::IR* ir) : Pass(ir) {}

    void TailRecursionPass::optimizeFunction(CFG* cfg)
    {
        auto optimizer = std::make_unique<TailRecursionOptimizer>(cfg);
        optimizer->performOptimization();
    }

    void TailRecursionPass::Execute()
    {
        for (auto const& [func_def, cfg] : ir->cfg) optimizeFunction(cfg);
    }

}  // namespace Transform
