#include "lcssa.h"
#include "llvm_ir/ir_builder.h"
#include <cassert>

namespace StructuralTransform
{
    LCSSAPass::LCSSAPass(LLVMIR::IR* ir) : Pass(ir) {}

    void LCSSAPass::Execute()
    {
        for (const auto& [func_def, cfg] : ir->cfg) transformToLCSSA(cfg);
    }

    void LCSSAPass::transformToLCSSA(CFG* cfg)
    {
        if (!cfg || !cfg->LoopForest) return;

        for (auto* loop : cfg->LoopForest->loop_set) transformLoopToLCSSA(cfg, loop);
    }

    void LCSSAPass::transformLoopToLCSSA(CFG* cfg, NaturalLoop* loop)
    {
        if (loop->exit_nodes.size() > 1) return;

        auto [var_set, type_map] = getUsedOperandOutOfLoop(cfg, loop);
        std::map<int, int> replace_map;

        if (loop->exit_nodes.empty()) return;

        auto* exit_bb = *loop->exit_nodes.begin();

        for (int var_reg : var_set)
        {
            std::vector<std::pair<LLVMIR::Operand*, LLVMIR::Operand*>> phi_vals;

            if (static_cast<size_t>(exit_bb->block_id) < cfg->invG.size())
            {
                for (auto* pred_bb : cfg->invG[exit_bb->block_id])
                {
                    if (loop->loop_nodes.find(pred_bb) != loop->loop_nodes.end())
                        phi_vals.push_back({getRegOperand(var_reg), getLabelOperand(pred_bb->block_id)});
                }
            }

            if (phi_vals.empty()) continue;

            int new_reg = ++cfg->func->max_reg;

            auto* phi_inst = new LLVMIR::PhiInst(type_map[var_reg], getRegOperand(new_reg), &phi_vals);

            auto it = exit_bb->insts.begin();
            while (it != exit_bb->insts.end() && (*it)->opcode == LLVMIR::IROpCode::PHI) ++it;
            exit_bb->insts.insert(it, phi_inst);

            replace_map[var_reg] = new_reg;
        }

        if (replace_map.empty()) return;

        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            if (loop->loop_nodes.find(bb) != loop->loop_nodes.end()) continue;

            for (auto* inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::PHI && bb == *loop->exit_nodes.begin()) continue;

                inst->Rename(replace_map);
            }
        }
    }

    std::tuple<std::set<int>, std::map<int, LLVMIR::DataType>> LCSSAPass::getUsedOperandOutOfLoop(
        CFG* cfg, NaturalLoop* loop)
    {
        std::set<int>                   var_list;
        std::map<int, LLVMIR::DataType> type_map;

        for (auto* bb : loop->loop_nodes)
        {
            for (auto* inst : bb->insts)
            {
                int result_reg = inst->GetResultReg();
                if (result_reg == -1) continue;

                var_list.insert(result_reg);
                type_map[result_reg] = inst->GetResultType();
            }
        }

        std::set<int>                   reg_used_set;  // Variables defined in loop but used outside loop
        std::map<int, LLVMIR::DataType> reg_used_type_map;

        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            if (loop->loop_nodes.find(bb) != loop->loop_nodes.end()) continue;

            for (auto* inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::PHI && loop->exit_nodes.find(bb) != loop->exit_nodes.end())
                    continue;

                std::vector<int> used_regs = inst->GetUsedRegs();
                for (int reg : used_regs)
                {
                    if (var_list.find(reg) == var_list.end()) continue;

                    reg_used_set.insert(reg);
                    reg_used_type_map[reg] = type_map[reg];
                }
            }
        }

        return std::make_tuple(reg_used_set, reg_used_type_map);
    }

}  // namespace StructuralTransform
