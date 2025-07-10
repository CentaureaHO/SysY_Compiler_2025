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
                switch (inst->opcode)
                {
                    case LLVMIR::IROpCode::LOAD:
                    {
                        auto* load_inst      = static_cast<LLVMIR::LoadInst*>(inst);
                        type_map[result_reg] = load_inst->type;
                        break;
                    }
                    case LLVMIR::IROpCode::ADD:
                    case LLVMIR::IROpCode::SUB:
                    case LLVMIR::IROpCode::MUL:
                    case LLVMIR::IROpCode::DIV:
                    case LLVMIR::IROpCode::MOD:
                    case LLVMIR::IROpCode::BITXOR:
                    case LLVMIR::IROpCode::SHL:
                    {
                        auto* arith_inst     = static_cast<LLVMIR::ArithmeticInst*>(inst);
                        type_map[result_reg] = arith_inst->type;
                        break;
                    }
                    case LLVMIR::IROpCode::FADD:
                    case LLVMIR::IROpCode::FSUB:
                    case LLVMIR::IROpCode::FMUL:
                    case LLVMIR::IROpCode::FDIV:
                    {
                        auto* arith_inst     = static_cast<LLVMIR::ArithmeticInst*>(inst);
                        type_map[result_reg] = arith_inst->type;
                        break;
                    }
                    case LLVMIR::IROpCode::ICMP:
                    case LLVMIR::IROpCode::FCMP:
                    {
                        type_map[result_reg] = LLVMIR::DataType::I1;
                        break;
                    }
                    case LLVMIR::IROpCode::CALL:
                    {
                        auto* call_inst      = static_cast<LLVMIR::CallInst*>(inst);
                        type_map[result_reg] = call_inst->ret_type;
                        break;
                    }
                    case LLVMIR::IROpCode::ALLOCA:
                    {
                        type_map[result_reg] = LLVMIR::DataType::PTR;
                        break;
                    }
                    case LLVMIR::IROpCode::GETELEMENTPTR:
                    {
                        type_map[result_reg] = LLVMIR::DataType::PTR;
                        break;
                    }
                    case LLVMIR::IROpCode::PHI:
                    {
                        auto* phi_inst       = static_cast<LLVMIR::PhiInst*>(inst);
                        type_map[result_reg] = phi_inst->type;
                        break;
                    }
                    case LLVMIR::IROpCode::ZEXT:
                    {
                        auto* zext_inst      = static_cast<LLVMIR::ZextInst*>(inst);
                        type_map[result_reg] = zext_inst->to;
                        break;
                    }
                    case LLVMIR::IROpCode::SITOFP:
                    {
                        type_map[result_reg] = LLVMIR::DataType::F32;
                        break;
                    }
                    case LLVMIR::IROpCode::FPTOSI:
                    {
                        type_map[result_reg] = LLVMIR::DataType::I32;
                        break;
                    }
                    case LLVMIR::IROpCode::FPEXT:
                    {
                        type_map[result_reg] = LLVMIR::DataType::DOUBLE;
                        break;
                    }
                    default:
                    {
                        type_map[result_reg] = LLVMIR::DataType::I32;
                        break;
                    }
                }
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
