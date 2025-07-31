#include "unusedarr.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
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
                    array_defs.insert(glb_op);
                    array2def[glb_op] = global_def;
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
                            array_defs.insert(result_op);
                            array2def[result_op] = inst;
                        }
                    }
                }
            }
        }
    }

    void UnusedArrEliminator::markAccessedArrays()
    {
        for (auto array : array_defs)
        {
            if (!edefUseAnalysis->getUses(array).empty())
            {
                // 标记为被访问
                accessed_arrays.insert(array);
            }
        }
    }

    void UnusedArrEliminator::eliminateDeadArrays()
    {
        std::unordered_set<Operand*> dead_array;
        std::unordered_set<Operand*> global_dead_array;
        for (auto array_op : array_defs)
        {
            if (!accessed_arrays.count(array_op))
            {
                auto def_inst = array2def[array_op];
                if (def_inst->opcode == IROpCode::GLOBAL_VAR) { global_dead_array.insert(array_op); }
                else { dead_array.insert(array_op); }
            }
        }

        // 删除未使用的
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
    }

    void UnusedArrEliminator::Execute()
    {
        collectArrayDefinitions();

        markAccessedArrays();

        eliminateDeadArrays();
    }
}  // namespace LLVMIR