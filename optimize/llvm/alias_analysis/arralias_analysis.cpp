#include "arralias_analysis.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include <cstddef>

namespace Analysis
{
#ifdef DEBUG
    void ArrAliasAnalysis::print()
    {
        std::cout << "Array Alias Analysis Results:" << std::endl;
        for (const auto& [cfg, pairs] : array_alias)
        {
            for (const auto& [alias, base] : pairs)
            {
                std::cout << "Alias: " << alias->getName() << " -> Base: " << base->getName() << std::endl;
            }
        }
        std::cout << "Array Original Operands:" << std::endl;
        for (const auto& [cfg, ops] : array_operands)
        {
            for (const auto& op : ops) { std::cout << op->getName() << std::endl; }
        }
    }
#endif
    void ArrAliasAnalysis::run()
    {
        // 对于函数参数,如果是ptr,我们也要加入
        // 将所有全局变量设为定义
        for (auto& global : ir->global_def)
        {
            auto global_def = dynamic_cast<LLVMIR::GlbvarDefInst*>(global);
            if (global_def)
            {

                // 如果是数组定义，直接将其作为数组操作数
                auto global_op = getGlobalOperand(global_def->name);
                for (auto [func, cfg] : ir->cfg)
                {
                    array_operands[cfg].insert(global_op);  // 全局数组也视为数组操作数}
                    array_alias[cfg][global_op] = global_op;
                }
            }
            for (auto& [func, cfg] : ir->cfg)
            {
                for (size_t i = 0; i < func->arg_regs.size(); i++)
                {
                    auto arg      = func->arg_regs[i];
                    auto arg_type = func->arg_types[i];
                    if (arg_type == LLVMIR::DataType::PTR)
                    {
                        array_operands[cfg].insert(arg);
                        array_alias[cfg][arg] = arg;
                    }
                }
                processSingleCFG(cfg);
                processPHI(cfg);
            }
        }
    }

    void ArrAliasAnalysis::processSingleCFG(CFG* cfg)
    {
        for (auto& [id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ALLOCA)
                {
                    auto op = inst->GetResultOperand();
                    array_operands[cfg].insert(op);
                    array_alias[cfg][op] = op;
                }
                if (inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                {
                    auto* gep_inst = static_cast<LLVMIR::GEPInst*>(inst);
                    auto  res      = gep_inst->res;
                    auto  reg      = static_cast<LLVMIR::RegOperand*>(res);
                    // 处理数组别名分析
                    if (gep_inst->base_ptr->type == LLVMIR::OperandType::REG ||
                        gep_inst->base_ptr->type == LLVMIR::OperandType::GLOBAL)
                    {
                        // 这里我们记录下所有情况,因为我们现在不能只存储最终的数组baseptr

                        array_alias[cfg][gep_inst->res] = gep_inst->base_ptr;
                    }
                }
                if (inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    auto* load_inst = static_cast<LLVMIR::LoadInst*>(inst);
                    if (load_inst->ptr->type == LLVMIR::OperandType::REG ||
                        load_inst->ptr->type == LLVMIR::OperandType::GLOBAL)
                    {
                        array_alias[cfg][load_inst->res] = load_inst->ptr;
                    }
                }
            }
        }
    }

    void ArrAliasAnalysis::processPHI(CFG* cfg)
    {
        for (auto& [id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::PHI)
                {
                    // 对于phi,查看其中参数有没有
                    auto* phi_inst = static_cast<LLVMIR::PhiInst*>(inst);
                    for (auto& [label, val] : phi_inst->vals_for_labels)
                    {
                        if (array_alias[cfg].count(val)) { array_alias[cfg][phi_inst->res] = val; }
                    }
                }
            }
        }
    }

    LLVMIR::Operand* ArrAliasAnalysis::traceToBase(CFG* cfg, LLVMIR::Operand* op)
    {
        // 实现数组别名的追踪逻辑
        // 这里可以根据具体需求实现
        std::unordered_set<LLVMIR::Operand*> visited;
        if (!array_alias.count(cfg)) { return nullptr; }

        while (array_alias[cfg].count(op))
        {
#ifdef DEBUG
            std::cout << "op is " << op->getName() << " and type is "
                      << ((op->type == LLVMIR::OperandType::GLOBAL) ? "Global" : "Other") << std::endl;
#endif
            if (array_operands[cfg].count(op)) return op;  // 如果已经是最后alloca的，直接返回
            if (visited.count(op)) break;                  // 防止 alias 循环
            visited.insert(op);
            op = array_alias[cfg][op];
#ifdef DEBUG
            std::cout << "op is " << op->getName() << " and type is "
                      << ((op->type == LLVMIR::OperandType::GLOBAL) ? "Global" : "Other") << std::endl;
#endif
        }

        if (array_operands[cfg].count(op)) return op;
        return nullptr;
    }
}  // namespace Analysis