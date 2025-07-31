#include "edefuse.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"

namespace Analysis
{
#ifdef DEBUG
    void EDefUseAnalysis::print()
    {
        std::cout << "EDefUse Analysis Results:" << std::endl;
        for (const auto& [op, inst] : DefMaps)
        {
            std::cout << "Operand: " << op->getName() << " defined by instruction: " << inst->opcode << std::endl;
        }
        for (const auto& [op, insts] : UseMaps)
        {
            std::cout << "Operand: " << op->getName() << " used by instructions: ";
            for (const auto& inst : insts) { std::cout << inst->opcode << ", "; }
            std::cout << std::endl;
        }
    }
#endif
    void EDefUseAnalysis::run()
    {
        for (auto& global_def : ir->global_def)
        {
            auto global_inst = dynamic_cast<LLVMIR::GlbvarDefInst*>(global_def);
            if (global_inst)
            {
                auto global_op = getGlobalOperand(global_inst->name);
                if (global_op) { DefMaps[global_op] = global_inst; }
            }
        }
        for (auto& [func, cfg] : ir->cfg)
        {
            // 加入函数参数
            for (auto& param : func->arg_regs)
            {
                DefMaps[param] = func;  // 参数没有定义指令
            }
            ExecuteInSingleCFG(cfg);
        }
    }

    void EDefUseAnalysis::ExecuteInSingleCFG(CFG* cfg)
    {
        for (auto& [id, block] : cfg->block_id_to_block)
        {
            for (auto& inst : block->insts)
            {
                auto def_op = inst->GetResultOperand();
                if (def_op)
                {
                    if (def_op->type == LLVMIR::OperandType::REG || def_op->type == LLVMIR::OperandType::GLOBAL)
                    {  // 说明定义了一个操作数
                        DefMaps[def_op] = inst;
                    }
                }

                auto used_ops = inst->GetUsedOperands();
                for (auto& op : used_ops)
                {
                    if (op->type == LLVMIR::OperandType::REG || op->type == LLVMIR::OperandType::GLOBAL)
                    {  // 说明使用了一个操作数
                        UseMaps[op].insert(inst);
                    }
                }
            }
        }
    }
}  // namespace Analysis