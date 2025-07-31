#include "arralias_analysis.h"
#include "llvm_ir/defs.h"

namespace Analysis
{
#ifdef DEBUG
    void ArrAliasAnalysis::print()
    {
        std::cout << "Array Alias Analysis Results:" << std::endl;
        for (const auto& [alias, base] : array_alias)
        {
            std::cout << "Alias: " << alias->getName() << " -> Base: " << base->getName() << std::endl;
        }
        std::cout << "Array Original Operands:" << std::endl;
        for (const auto& op : array_operands) { std::cout << op->getName() << std::endl; }
    }
#endif
    void ArrAliasAnalysis::run()
    {
        // 将所有全局变量设为定义
        for (auto& global : ir->global_def)
        {
            auto global_def = dynamic_cast<LLVMIR::GlbvarDefInst*>(global);
            if (global_def)
            {
                if (global_def->arr_init.dims.size() > 0)
                {
                    // 如果是数组定义，直接将其作为数组操作数
                    auto global_op = getGlobalOperand(global_def->name);
                    array_operands.insert(global_op);  // 全局数组也视为数组操作数
                }
            }
        }
        for (auto& [func, cfg] : ir->cfg) { processSingleCFG(cfg); }
    }

    void ArrAliasAnalysis::processSingleCFG(CFG* cfg)
    {
        for (auto& [id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ALLOCA) { array_operands.insert(inst->GetResultOperand()); }
                if (inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                {
                    auto* gep_inst = static_cast<LLVMIR::GEPInst*>(inst);
                    // 处理数组别名分析
                    if (gep_inst->base_ptr->type == LLVMIR::OperandType::REG ||
                        gep_inst->base_ptr->type == LLVMIR::OperandType::GLOBAL)
                    {
                        // 这里我们记录下所有情况,因为我们现在不能只存储最终的数组baseptr
                        array_alias[gep_inst->GetResultOperand()] = gep_inst->base_ptr;
                    }
                }
            }
        }
    }

    LLVMIR::Operand* ArrAliasAnalysis::traceToBase(LLVMIR::Operand* op)
    {
        // 实现数组别名的追踪逻辑
        // 这里可以根据具体需求实现
        std::unordered_set<LLVMIR::Operand*> visited;
        while (array_alias.count(op))
        {
#ifdef DEBUG
            std::cout << "op is " << op->getName() << " and type is "
                      << ((op->type == LLVMIR::OperandType::GLOBAL) ? "Global" : "Other") << std::endl;
#endif
            if (array_operands.count(op)) return op;  // 如果已经是最后alloca的，直接返回
            if (visited.count(op)) break;             // 防止 alias 循环
            visited.insert(op);
            op = array_alias[op];
#ifdef DEBUG
            std::cout << "op is " << op->getName() << " and type is "
                      << ((op->type == LLVMIR::OperandType::GLOBAL) ? "Global" : "Other") << std::endl;
#endif
        }
        if (array_operands.count(op)) return op;
        return nullptr;
    }
}  // namespace Analysis