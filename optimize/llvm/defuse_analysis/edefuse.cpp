#include "edefuse.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"

namespace Analysis
{
#ifdef DEBUG
    void EDefUseAnalysis::print()
    {
        std::cout << "EDefUse Analysis Results:" << std::endl;
        for (const auto& [cfg, def_map] : DefMaps)
        {
            for (const auto& [op, inst] : def_map)
            {
                std::cout << "Operand: " << op << " defined by instruction: " << inst->opcode << std::endl;
            }
        }
        for (const auto& [cfg, use_map] : UseMaps)
        {
            for (const auto& [op, inst_set] : use_map)
            {
                std::cout << "Operand: " << op << " used by instructions: ";
                for (const auto& inst : inst_set) { std::cout << inst << ", "; }
                std::cout << std::endl;
            }
        }
    }
#endif
    void EDefUseAnalysis::run()
    {
        // 先重置
        DefMaps.clear();
        UseMaps.clear();

        for (auto& global_def : ir->global_def)
        {
            auto global_inst = dynamic_cast<LLVMIR::GlbvarDefInst*>(global_def);
            if (global_inst)
            {
                auto global_op = getGlobalOperand(global_inst->name);
                if (global_op)
                {
                    for (auto [_, cfg] : ir->cfg) { DefMaps[cfg][global_op] = global_inst; }
                }
            }
        }
        for (auto& [func, cfg] : ir->cfg)
        {
            for (auto arg : func->arg_regs) { DefMaps[cfg][arg] = func; }
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
                        DefMaps[cfg][def_op] = inst;
                    }
                }

                auto used_ops = inst->GetUsedOperands();
                for (auto& op : used_ops)
                {
                    if (op->type == LLVMIR::OperandType::REG || op->type == LLVMIR::OperandType::GLOBAL)
                    {  // 说明使用了一个操作数
                        UseMaps[cfg][op].insert(inst);
                    }
                }
#ifdef DEBUG
                std::cout << "Instruction: " << inst->opcode << " in block " << id << " uses: ";
                for (auto& op : used_ops) { std::cout << op->getName() << ", "; }
                std::cout << "defs: " << (def_op ? def_op->getName() : "None") << std::endl;
#endif
            }
        }
    }
}  // namespace Analysis