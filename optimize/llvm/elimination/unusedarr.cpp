#include "unusedarr.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include <deque>
#include <iostream>
#include <unordered_set>

namespace LLVMIR
{
    void UnusedArrEliminator::collectArrayDefinitions()
    {
        // 全局数组
        for (auto global_def : ir->global_def)
        {
            auto glb_inst = dynamic_cast<GlbvarDefInst*>(global_def);
            if (glb_inst)
            {
#ifdef DEBUG
                std::cout << "Found global array: " << glb_inst->name << " Type is " << glb_inst->type << std::endl;
#endif
                if (!glb_inst->arr_init.dims.empty())
                {
                    // 说明是全局数组
                    auto glb_op = getGlobalOperand(glb_inst->name);
                    for (auto [_, cfg] : ir->cfg)
                    {
                        array_defs[cfg].insert(glb_op);
                        array2def[cfg][glb_op] = global_def;
                    }
                }
            }
        }

        // 局部数组
        for (auto& [func, cfg] : ir->cfg)
        {
            for (auto& [id, block] : cfg->block_id_to_block)
            {
                for (auto inst : block->insts)
                {
                    if (inst->opcode == IROpCode::ALLOCA)
                    {
                        // 说明是alloca
                        auto alloca_inst = dynamic_cast<AllocInst*>(inst);
                        if (alloca_inst && alloca_inst->dims.size() > 0)
                        {
                            // 说明是数组
                            auto result_op = alloca_inst->GetResultOperand();
                            array_defs[cfg].insert(result_op);
                            array2def[cfg][result_op] = inst;
                        }
                    }
                }
            }
        }
    }

    void UnusedArrEliminator::markAccessedArrays()
    {
#ifdef DEBUG
        std::cout << array_defs.size() << " arrays found." << std::endl;
#endif
        for (auto& [cfg, arrays] : array_defs)
        {
            for (auto array : arrays)
            {
#ifdef DEBUG
                std::cout << array->getName() << " is ";
#endif
                if (array->type == OperandType::GLOBAL)
                {
                    // 全局数组
                    // 全局数组只要被访问过就认为是有意义的
                    bool accessed = false;
                    for (auto [_, cfg] : ir->cfg)
                    {
                        if (edefUseAnalysis->getUses(cfg, array).size() > 0) { accessed = true; }
                    }
                    if (accessed)
                    {
                        for (auto [_, cfg] : ir->cfg) { accessed_arrays[cfg].insert(array); }
                    }
                }
                else
                {
                    // 局部数组
                    // 我们检查所有的use，如果都是store、getelementptr等指令，则认为没有意义，因为往局部变量只写不读，没有意义
                    std::unordered_set<Instruction*> uses = edefUseAnalysis->getUses(cfg, array);
                    std::unordered_set<Instruction*> visited;
                    while (!uses.empty())
                    {
                        auto use = *uses.begin();
                        uses.erase(use);
                        visited.insert(use);
#ifdef DEBUG
                        std::cout << "Use instruction found: " << use->opcode << " Op is "
                                  << (use->GetResultOperand() ? use->GetResultOperand()->getName() : "N/A")
                                  << std::endl;
#endif
                        if (use->opcode == IROpCode::LOAD)
                        {
                            // 说明这个数组被访问了
                            accessed_arrays[cfg].insert(array);
                            break;
                        }
                        else if (use->opcode == IROpCode::CALL)
                        {
                            auto call_inst = dynamic_cast<CallInst*>(use);
                            if (call_inst && call_inst->func_name != "llvm.memset.p0.i32")
                            {
                                // std::cout << call_inst->func_name << " is not memset, marking array as accessed."
                                        //   << std::endl;
                                // 说明调用了其他函数，可能会访问这个数组
                                accessed_arrays[cfg].insert(array);
                                break;
                            }
                        }
                        else if (use->opcode == IROpCode::GETELEMENTPTR)
                        {
                            // 继续查找这个getelementptr的结果是否有其他use
                            auto result_op = use->GetResultOperand();
                            for (auto use2 : edefUseAnalysis->getUses(cfg, result_op))
                            {
                                if (!visited.count(use2)) { uses.insert(use2); }
                            }
                        }
                    }
                }
            }
        }
    }

    void UnusedArrEliminator::eliminateDeadArrays()
    {
        std::unordered_map<CFG*, std::unordered_set<Operand*>> dead_array;
        std::unordered_set<Operand*>                           global_dead_array;
        for (auto [cfg, array_ops] : array_defs)
        {
            for (auto array_op : array_ops)
            {
                if (!accessed_arrays[cfg].count(array_op))
                {
                    auto def_inst = array2def[cfg][array_op];
                    if (def_inst->opcode == IROpCode::GLOBAL_VAR) { global_dead_array.insert(array_op); }
                    else { dead_array[cfg].insert(array_op); }
                }
            }
        }

        // 打印未使用的数组
#ifdef DEBUG
        std::cout << "Dead arrays found: " << dead_array.size() << std::endl;
        for (auto& [cfg, arrays] : dead_array)
        {
            std::cout << "CFG: " << cfg->func->func_def->func_name << " has dead arrays: ";
            for (auto array : arrays) { std::cout << array->getName() << ", "; }
            std::cout << std::endl;
        }
#endif

        // 删除未使用的全局数组
        auto it = ir->global_def.begin();
        while (it != ir->global_def.end())
        {
            auto glb_def  = (*it);
            auto glb_inst = dynamic_cast<GlbvarDefInst*>(glb_def);
            auto glb_op   = getGlobalOperand(glb_inst->name);
            if (global_dead_array.count(glb_op))
            {
                // 说明需要删除
                it = ir->global_def.erase(it);
                continue;
            }
            ++it;
        }
        // 删除未使用的局部数组及其uses
        std::unordered_set<Instruction*> to_remove;
        for (auto [cfg, arrays] : dead_array)
        {
            for (auto array : arrays)
            {
                auto                             def_inst = array2def[cfg][array];
                std::unordered_set<Instruction*> defs;
                defs.insert(def_inst);
                while (!defs.empty())
                {
                    auto inst = *defs.begin();
                    defs.erase(inst);
                    to_remove.insert(inst);
                    // 删除所有的use
                    for (auto use : edefUseAnalysis->getUses(cfg, inst->GetResultOperand()))
                    {
                        if (use->opcode == IROpCode::GETELEMENTPTR)
                        {
                            // 继续查找这个getelementptr的结果是否有其他use
                            auto result_op = use->GetResultOperand();
#ifdef DEBUG
                            std::cout << "result is of instruction " << use->opcode << "" << " result op is "
                                      << result_op << std::endl;
#endif
                            for (auto use2 : edefUseAnalysis->getUses(cfg, result_op))
                            {
#ifdef DEBUG
                                std::cout << "Removing use: " << use2->opcode << " for array " << array->getName()
                                          << std::endl;
#endif
                                defs.insert(use2);
                            }
                        }
                        to_remove.insert(use);
                    }
                }
            }

            for (auto& [id, block] : cfg->block_id_to_block)
            {
                std::deque<Instruction*> new_insts;
                for (auto inst : block->insts)
                {
                    if (to_remove.count(inst) == 0) { new_insts.push_back(inst); }
                }
                block->insts = std::move(new_insts);
            }
            to_remove.clear();
        }
    }

    void UnusedArrEliminator::Execute()
    {
        collectArrayDefinitions();
#ifdef DEBUG
        std::cout << "Collected array definitions: " << array_defs.size() << std::endl;
        for (auto [cfg, arrays] : array_defs)
        {
            std::cout << "CFG: " << cfg->func->func_def->func_name << " has arrays: ";
            for (auto array : arrays) { std::cout << array->getName() << ", "; }
            std::cout << std::endl;
        }
#endif
        markAccessedArrays();
#ifdef DEBUG
        std::cout << "Marked accessed arrays: " << accessed_arrays.size() << std::endl;
        for (auto [cfg, arrays] : accessed_arrays)
        {
            for (auto array : arrays)
            {
                std::cout << "CFG: " << cfg->func->func_def->func_name << " Accessed Array: " << array->getName()
                          << std::endl;
            }
        }
#endif
        eliminateDeadArrays();
#ifdef DEBUG
        std::cout << "Eliminated dead arrays." << std::endl;
#endif
    }
}  // namespace LLVMIR