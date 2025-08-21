#include "same_source_phi_elimination.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <unordered_set>

namespace Transform
{
    SameSourcePhiEliminationPass::SameSourcePhiEliminationPass(LLVMIR::IR* ir)
        : Pass(ir), preserve_lcssa_(true), last_execution_modified_(false)
    {}

    void SameSourcePhiEliminationPass::Execute()
    {
        last_execution_modified_ = false;

        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (preserve_lcssa_)
            {
                assert(cfg->LoopForest != nullptr && "Loop information is required when preserve_lcssa is enabled");
            }

            bool                                 changed = false;
            std::vector<LLVMIR::PhiInst*>        phis_to_remove;
            std::unordered_set<LLVMIR::PhiInst*> processed_phis;

            for (const auto& [block_id, block] : cfg->block_id_to_block)
            {
                if (preserve_lcssa_ && isLoopExitBlock(block, cfg)) continue;

                for (auto* inst : block->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::PHI)
                    {
                        auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                        if (processed_phis.find(phi) != processed_phis.end()) continue;

                        if (processSameSourcePhi(phi, block, cfg))
                        {
                            phis_to_remove.push_back(phi);
                            processed_phis.insert(phi);
                            changed = true;
                        }
                    }
                }
            }

            for (auto* phi : phis_to_remove)
            {
                auto block_it = cfg->block_id_to_block.find(phi->block_id);
                if (block_it != cfg->block_id_to_block.end())
                {
                    auto& insts = block_it->second->insts;
                    insts.erase(std::remove(insts.begin(), insts.end(), phi), insts.end());
                }
            }

            if (changed) last_execution_modified_ = true;
        }
    }

    bool SameSourcePhiEliminationPass::isLoopExitBlock(LLVMIR::IRBlock* block, CFG* cfg) const
    {
        if (!cfg->LoopForest || cfg->LoopForest->loop_set.empty()) return false;

        for (auto* loop : cfg->LoopForest->loop_set)
            if (loop->exit_nodes.count(block) > 0) return true;

        return false;
    }

    bool SameSourcePhiEliminationPass::processSameSourcePhi(LLVMIR::PhiInst* phi, LLVMIR::IRBlock* block, CFG* cfg)
    {
        if (phi->vals_for_labels.size() < 2) return false;

        LLVMIR::Operand* common_source = nullptr;
        if (!allSourcesAreSame(phi, common_source)) return false;

        if (!common_source || !phi->res) return false;

        auto* copy_inst = createCopyInst(phi->type, common_source, phi->res);
        if (!copy_inst) return false;

        copy_inst->block_id = phi->block_id;

        // 找到所有PHI节点的结束位置，在那里插入copy指令
        // 这样可以确保PHI节点仍然在基本块顶部，copy指令在PHI节点之后
        auto& insts      = block->insts;
        auto  insert_pos = insts.begin();

        // 跳过所有PHI指令（包括当前要删除的PHI）
        while (insert_pos != insts.end() && (*insert_pos)->opcode == LLVMIR::IROpCode::PHI) { ++insert_pos; }

        // 现在insert_pos指向第一个非PHI指令，在这里插入copy指令
        insts.insert(insert_pos, copy_inst);

        return true;
    }

    bool SameSourcePhiEliminationPass::allSourcesAreSame(LLVMIR::PhiInst* phi, LLVMIR::Operand*& common_source) const
    {
        if (phi->vals_for_labels.empty()) return false;

        common_source = phi->vals_for_labels[0].first;
        if (!common_source) return false;

        for (size_t i = 1; i < phi->vals_for_labels.size(); ++i)
        {
            auto* current_source = phi->vals_for_labels[i].first;
            if (!operandsEqual(common_source, current_source)) { return false; }
        }

        return true;
    }

    bool SameSourcePhiEliminationPass::operandsEqual(LLVMIR::Operand* op1, LLVMIR::Operand* op2) const
    {
        if (!op1 || !op2) return op1 == op2;

        if (op1->type != op2->type) return false;

        switch (op1->type)
        {
            case LLVMIR::OperandType::REG:
            {
                auto* reg1 = static_cast<LLVMIR::RegOperand*>(op1);
                auto* reg2 = static_cast<LLVMIR::RegOperand*>(op2);
                return reg1->reg_num == reg2->reg_num;
            }
            case LLVMIR::OperandType::IMMEI32:
            {
                auto* imm1 = static_cast<LLVMIR::ImmeI32Operand*>(op1);
                auto* imm2 = static_cast<LLVMIR::ImmeI32Operand*>(op2);
                return imm1->value == imm2->value;
            }
            case LLVMIR::OperandType::IMMEF32:
            {
                auto* imm1 = static_cast<LLVMIR::ImmeF32Operand*>(op1);
                auto* imm2 = static_cast<LLVMIR::ImmeF32Operand*>(op2);
                return imm1->value == imm2->value;
            }
            case LLVMIR::OperandType::GLOBAL:
            {
                auto* glob1 = static_cast<LLVMIR::GlobalOperand*>(op1);
                auto* glob2 = static_cast<LLVMIR::GlobalOperand*>(op2);
                return glob1->global_name == glob2->global_name;
            }

            default: return false;
        }
    }

}  // namespace Transform
