#include "llvm/verify/phi_precursor.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"
#include <iostream>
#include <sstream>

namespace Verify
{
    PhiPrecursorPass::PhiPrecursorPass(LLVMIR::IR* ir) : Pass(ir) {}

    std::string PhiPrecursorPass::getErrorMessage(
        LLVMIR::PhiInst* phi_inst, int block_id, const std::string& error_detail)
    {
        std::stringstream ss;
        ss << "PHI验证错误 - 函数中的Block" << block_id << ": " << error_detail;
        if (phi_inst->res) ss << " (PHI结果寄存器: %" << phi_inst->res->getName() << ")";
        return ss.str();
    }

    bool PhiPrecursorPass::checkPhiInstruction(LLVMIR::PhiInst* phi_inst, CFG* cfg, int block_id)
    {
        bool is_valid = true;

        std::set<int> predecessors;
        for (size_t pred_id = 0; pred_id < cfg->G.size(); ++pred_id)
        {
            const auto& succ_blocks = cfg->G[pred_id];
            for (auto* succ_block : succ_blocks)
            {
                if (succ_block->block_id == block_id) { predecessors.insert(static_cast<int>(pred_id)); }
            }
        }

        std::set<int> phi_labels;
        for (const auto& [val, label] : phi_inst->vals_for_labels)
        {
            if (label->type == LLVMIR::OperandType::LABEL)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                int   label_id = label_op->label_num;
                phi_labels.insert(label_id);

                if (predecessors.find(label_id) == predecessors.end())
                {
                    std::cerr << getErrorMessage(
                                     phi_inst, block_id, "PHI指令包含非前驱块的标签: Block" + std::to_string(label_id))
                              << std::endl;
                    is_valid = false;
                }
            }
            else
            {
                std::cerr << getErrorMessage(phi_inst, block_id, "PHI指令包含非标签类型的操作数") << std::endl;
                is_valid = false;
            }
        }

        for (int pred_id : predecessors)
            if (phi_labels.find(pred_id) == phi_labels.end())
            {
                std::cerr << getErrorMessage(
                                 phi_inst, block_id, "PHI指令缺少前驱块的对应: Block" + std::to_string(pred_id))
                          << std::endl;
                is_valid = false;
            }

        if (phi_labels.size() != phi_inst->vals_for_labels.size())
        {
            std::cerr << getErrorMessage(phi_inst, block_id, "PHI指令包含重复的标签") << std::endl;
            is_valid = false;
        }

        return is_valid;
    }

    bool PhiPrecursorPass::verifyPhiPrecursors(CFG* cfg)
    {
        bool        all_valid = true;
        std::string func_name = cfg->func->func_def->func_name;

        std::cout << "验证函数 " << func_name << " 中的PHI节点前驱..." << std::endl;

        for (const auto& [block_id, bb] : cfg->block_id_to_block)
            for (auto* inst : bb->insts)
                if (inst->opcode == LLVMIR::IROpCode::PHI)
                {
                    auto* phi_inst = static_cast<LLVMIR::PhiInst*>(inst);

                    if (!checkPhiInstruction(phi_inst, cfg, block_id)) all_valid = false;
                }

        if (all_valid)
            std::cout << "函数 " << func_name << " 中的所有PHI节点都通过验证" << std::endl;
        else
            std::cout << "函数 " << func_name << " 中发现PHI节点验证错误" << std::endl;

        return all_valid;
    }

    void PhiPrecursorPass::Execute()
    {
        bool all_functions_valid = true;

        std::cout << "=== 开始PHI节点前驱验证 ===" << std::endl;

        for (const auto& [func_def, cfg] : ir->cfg)
            if (!verifyPhiPrecursors(cfg)) all_functions_valid = false;

        std::cout << "=== PHI节点前驱验证完成 ===" << std::endl;

        if (all_functions_valid)
            std::cout << "所有函数的PHI节点都通过验证！" << std::endl;
        else
            std::cout << "发现PHI节点验证错误，请检查上述错误信息。" << std::endl;
    }

}  // namespace Verify
