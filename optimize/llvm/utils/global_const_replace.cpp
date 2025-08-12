#include "global_const_replace.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include <iostream>
#include <set>

using namespace LLVMIR;

namespace Transform
{
    void GlobalConstReplacePass::Execute()
    {
        analyzeGlobalDefinitions();
        analyzeGlobalModifications();
        for (auto& [func_def, cfg] : ir->cfg) replaceGlobalConstLoadsInCFG(cfg);
    }

    void GlobalConstReplacePass::analyzeGlobalDefinitions()
    {
        for (auto* inst : ir->global_def)
        {
            if (inst->opcode != IROpCode::GLOBAL_VAR) continue;

            auto* global_def = dynamic_cast<GlbvarDefInst*>(inst);
            if (!global_def) continue;

            bool         hasInitValue = false;
            VarAttribute attr;

            if (!global_def->arr_init.initVals.empty())
            {
                hasInitValue = true;
                attr         = global_def->arr_init;
            }
            else if (global_def->init != nullptr && global_def->arr_init.dims.empty())
            {
                hasInitValue = true;

                if (global_def->type == DataType::I32)
                {
                    attr.type = intType;
                    if (global_def->init->type == OperandType::IMMEI32)
                    {
                        auto* imm = dynamic_cast<ImmeI32Operand*>(global_def->init);
                        if (imm)
                        {
                            VarValue val(imm->value);
                            attr.initVals.push_back(val);
                        }
                    }
                }
                else if (global_def->type == DataType::F32)
                {
                    attr.type = floatType;
                    if (global_def->init->type == OperandType::IMMEF32)
                    {
                        auto* imm = dynamic_cast<ImmeF32Operand*>(global_def->init);
                        if (imm)
                        {
                            VarValue val(imm->value);
                            attr.initVals.push_back(val);
                        }
                    }
                }
            }

            if (hasInitValue) { global_const_map[global_def->name] = attr; }
        }
    }

    void GlobalConstReplacePass::analyzeGlobalModifications()
    {
        std::set<std::string>      modified_globals;
        std::map<int, std::string> gep_to_global_map;

        for (auto& [func_def, cfg] : ir->cfg)
        {
            for (auto* func : ir->functions)
            {
                if (func->func_def != func_def) continue;

                for (auto* block : func->blocks)
                {
                    for (auto* inst : block->insts)
                    {
                        if (inst->opcode == IROpCode::GETELEMENTPTR)
                        {
                            auto* gep_inst = dynamic_cast<GEPInst*>(inst);
                            if (!gep_inst) continue;

                            if (gep_inst->base_ptr->type == OperandType::GLOBAL)
                            {
                                auto* global_op = dynamic_cast<GlobalOperand*>(gep_inst->base_ptr);
                                if (global_op && gep_inst->res && gep_inst->res->type == OperandType::REG)
                                {
                                    auto* res_reg = dynamic_cast<RegOperand*>(gep_inst->res);
                                    if (res_reg) { gep_to_global_map[res_reg->reg_num] = global_op->global_name; }
                                }
                            }
                        }
                    }
                }

                for (auto* block : func->blocks)
                {
                    for (auto* inst : block->insts)
                    {
                        if (inst->opcode == IROpCode::STORE)
                        {
                            auto* store_inst = dynamic_cast<StoreInst*>(inst);
                            if (!store_inst) continue;

                            if (store_inst->ptr->type == OperandType::GLOBAL)
                            {
                                auto* global_op = dynamic_cast<GlobalOperand*>(store_inst->ptr);
                                if (global_op) { modified_globals.insert(global_op->global_name); }
                            }
                            else if (store_inst->ptr->type == OperandType::REG)
                            {
                                auto* reg_op = dynamic_cast<RegOperand*>(store_inst->ptr);
                                if (reg_op && gep_to_global_map.find(reg_op->reg_num) != gep_to_global_map.end())
                                {
                                    modified_globals.insert(gep_to_global_map[reg_op->reg_num]);
                                }
                            }
                        }
                    }
                }
            }
        }

        for (const auto& modified : modified_globals) { global_const_map.erase(modified); }
    }

    void GlobalConstReplacePass::replaceGlobalConstLoadsInCFG(CFG* cfg)
    {
        auto* func = cfg->func;
        if (!func) return;

        for (auto* block : func->blocks)
        {
            auto it = block->insts.begin();
            while (it != block->insts.end())
            {
                auto* inst = *it;

                if (inst->opcode == IROpCode::LOAD)
                {
                    auto* load_inst = dynamic_cast<LoadInst*>(inst);
                    if (!load_inst || load_inst->ptr->type != OperandType::GLOBAL)
                    {
                        ++it;
                        continue;
                    }

                    auto* global_op = dynamic_cast<GlobalOperand*>(load_inst->ptr);
                    if (!global_op)
                    {
                        ++it;
                        continue;
                    }

                    // std::cout << "global_op->global_name: " << global_op->global_name << std::endl;
                    // if (global_op->global_name == "base")
                    // {
                    //     ++it;
                    //     continue;
                    // }

                    auto const_it = global_const_map.find(global_op->global_name);
                    if (const_it == global_const_map.end())
                    {
                        ++it;
                        continue;
                    }

                    auto* new_inst = createConstAssignment(load_inst, const_it->second);
                    if (new_inst) *it = new_inst;
                }

                ++it;
            }
        }
    }

    Instruction* GlobalConstReplacePass::createConstAssignment(LoadInst* load_inst, const VarAttribute& var_attr)
    {
        if (var_attr.initVals.empty()) { return nullptr; }

        const auto& init_val = var_attr.initVals[0];

        if (load_inst->type == DataType::I32 && init_val.type == VarValue::Type::Int)
        {
            auto* zero     = ImmeI32Operand::get(0);
            auto* constant = ImmeI32Operand::get(init_val.int_val);

            return new ArithmeticInst(IROpCode::ADD, DataType::I32, zero, constant, load_inst->res);
        }
        else if (load_inst->type == DataType::F32 && init_val.type == VarValue::Type::Float)
        {
            auto* zero     = ImmeF32Operand::get(0.0f);
            auto* constant = ImmeF32Operand::get(init_val.float_val);

            return new ArithmeticInst(IROpCode::FADD, DataType::F32, zero, constant, load_inst->res);
        }

        return nullptr;
    }
}  // namespace Transform
