#include "if_conversion.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "cfg.h"
#include <iostream>
#include <algorithm>
#include <set>

namespace Transform
{
    IfConversionPass::IfConversionPass(LLVMIR::IR* ir) : Pass(ir), modified_(false) {}

    void IfConversionPass::Execute()
    {
        modified_ = false;

        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (cfg && !cfg->block_id_to_block.empty()) processFunction(cfg);
        }
    }

    void IfConversionPass::processFunction(CFG* cfg)
    {
        std::vector<int> blocks_to_erase;

        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            auto predecessors = getPredecessors(block, cfg);
            if (predecessors.size() != 2) { continue; }

            int              phi_count = 0;
            LLVMIR::PhiInst* phi       = nullptr;
            for (auto* inst : block->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::PHI) break;
                phi_count++;
                phi = static_cast<LLVMIR::PhiInst*>(inst);
            }

            if (phi_count != 1) continue;
            if (phi->type != LLVMIR::DataType::I32) continue;
            if (phi->vals_for_labels.size() != 2) continue;

            LLVMIR::IRBlock*                  ancient_block = nullptr;
            std::vector<LLVMIR::Instruction*> middle_block_insts;

            if (canConvertControlFlowToSelect(block, phi, predecessors, cfg, ancient_block, middle_block_insts))
            {
                convertControlFlowToSelect(block, phi, ancient_block, middle_block_insts, cfg);
                modified_ = true;
            }
        }

        if (modified_) cfg->BuildCFG();
    }

    bool IfConversionPass::isOpcodeProhibitedForConversion(LLVMIR::IROpCode opcode) const
    {
        static const std::set<LLVMIR::IROpCode> banned_opcodes = {LLVMIR::IROpCode::BR_COND,
            LLVMIR::IROpCode::STORE,
            LLVMIR::IROpCode::CALL,
            LLVMIR::IROpCode::DIV,
            LLVMIR::IROpCode::MOD};

        if (banned_opcodes.find(opcode) != banned_opcodes.end()) return true;

        static const std::set<LLVMIR::IROpCode> permitted_opcodes = {LLVMIR::IROpCode::BR_UNCOND,
            LLVMIR::IROpCode::ADD,
            LLVMIR::IROpCode::SUB,
            LLVMIR::IROpCode::MUL,
            LLVMIR::IROpCode::BITXOR,
            LLVMIR::IROpCode::SHL,
            LLVMIR::IROpCode::LOAD,
            LLVMIR::IROpCode::BITAND,
            LLVMIR::IROpCode::FPEXT,
            LLVMIR::IROpCode::SELECT};

        if (permitted_opcodes.find(opcode) != permitted_opcodes.end()) return false;

        return true;
    }

    bool IfConversionPass::canConvertControlFlowToSelect(LLVMIR::IRBlock* block, LLVMIR::PhiInst* phi,
        const std::vector<LLVMIR::IRBlock*>& predecessors, CFG* cfg, LLVMIR::IRBlock*& ancient_block,
        std::vector<LLVMIR::Instruction*>& middle_block_insts) const
    {
        const int MAX_MERGE_NUM        = 4;
        const int MAX_SINGLE_MERGE_NUM = 2;

        auto pred0 = predecessors[0];
        auto pred1 = predecessors[1];

        auto pred0_predecessors = getPredecessors(pred0, cfg);
        auto pred1_predecessors = getPredecessors(pred1, cfg);
        auto pred0_successors   = getSuccessors(pred0, cfg);
        auto pred1_successors   = getSuccessors(pred1, cfg);

        bool has_side_effect = false;
        middle_block_insts.clear();

        if (pred0_predecessors.size() == 1 && pred0_predecessors[0] == pred1 && pred0_successors.size() == 1)
        {
            ancient_block = pred1;

            for (auto* inst : pred0->insts)
            {
                if (isOpcodeProhibitedForConversion(inst->opcode))
                {
                    has_side_effect = true;
                    break;
                }
            }

            if (has_side_effect || pred0->insts.size() >= MAX_SINGLE_MERGE_NUM) return false;

            for (auto* inst : pred0->insts)
                if (inst->opcode != LLVMIR::IROpCode::BR_UNCOND) middle_block_insts.push_back(inst);
        }
        else if (pred1_predecessors.size() == 1 && pred1_predecessors[0] == pred0 && pred1_successors.size() == 1)
        {
            ancient_block = pred0;

            for (auto* inst : pred1->insts)
            {
                if (isOpcodeProhibitedForConversion(inst->opcode))
                {
                    has_side_effect = true;
                    break;
                }
            }

            if (has_side_effect || pred1->insts.size() >= MAX_SINGLE_MERGE_NUM) return false;

            for (auto* inst : pred1->insts)
                if (inst->opcode != LLVMIR::IROpCode::BR_UNCOND) middle_block_insts.push_back(inst);
        }
        else if (pred0_predecessors.size() == 1 && pred1_predecessors.size() == 1 &&
                 pred0_predecessors[0] == pred1_predecessors[0] && pred0_successors.size() == 1 &&
                 pred1_successors.size() == 1)
        {
            ancient_block = pred0_predecessors[0];

            for (auto* inst : pred0->insts)
            {
                if (isOpcodeProhibitedForConversion(inst->opcode))
                {
                    has_side_effect = true;
                    break;
                }
            }

            if (!has_side_effect)
            {
                for (auto* inst : pred1->insts)
                {
                    if (isOpcodeProhibitedForConversion(inst->opcode))
                    {
                        has_side_effect = true;
                        break;
                    }
                }
            }

            if (has_side_effect || pred0->insts.size() + pred1->insts.size() >= MAX_MERGE_NUM ||
                pred0->insts.size() >= MAX_SINGLE_MERGE_NUM || pred1->insts.size() >= MAX_SINGLE_MERGE_NUM)
                return false;

            for (auto* inst : pred0->insts)
                if (inst->opcode != LLVMIR::IROpCode::BR_UNCOND) middle_block_insts.push_back(inst);
            for (auto* inst : pred1->insts)
                if (inst->opcode != LLVMIR::IROpCode::BR_UNCOND) middle_block_insts.push_back(inst);
        }
        else
            return false;

        if (ancient_block->insts.size() < 2) return false;

        auto second_last_inst = ancient_block->insts[ancient_block->insts.size() - 2];
        if (second_last_inst->opcode != LLVMIR::IROpCode::ICMP) return false;

        return true;
    }

    void IfConversionPass::convertControlFlowToSelect(LLVMIR::IRBlock* block, LLVMIR::PhiInst* phi,
        LLVMIR::IRBlock* ancient_block, const std::vector<LLVMIR::Instruction*>& middle_block_insts, CFG* cfg)
    {
        auto* br_cond = static_cast<LLVMIR::BranchCondInst*>(ancient_block->insts.back());
        auto* icmp    = static_cast<LLVMIR::IcmpInst*>(ancient_block->insts[ancient_block->insts.size() - 2]);

        ancient_block->insts.pop_back();
        ancient_block->insts.pop_back();

        for (auto* inst : middle_block_insts) ancient_block->insts.push_back(inst);

        auto* br_uncond     = new LLVMIR::BranchUncondInst(LLVMIR::LabelOperand::get(block->block_id));
        br_uncond->block_id = ancient_block->block_id;
        ancient_block->insts.push_back(br_uncond);

        auto& phi_pairs = phi->vals_for_labels;
        auto  pair1     = phi_pairs[0];
        auto  pair2     = phi_pairs[1];

        auto* label1 = static_cast<LLVMIR::LabelOperand*>(pair1.second);
        auto* label2 = static_cast<LLVMIR::LabelOperand*>(pair2.second);

        auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
        auto* false_label = static_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

        auto op1 = pair1.first;
        auto op2 = pair2.first;

        LLVMIR::Operand* true_val  = op1;
        LLVMIR::Operand* false_val = op2;

        if (label1->label_num == true_label->label_num && label2->label_num == false_label->label_num)
        {
            true_val  = op1;
            false_val = op2;
        }
        else if (label2->label_num == true_label->label_num && label1->label_num == false_label->label_num)
        {
            true_val  = op2;
            false_val = op1;
        }
        else if (label1->label_num == true_label->label_num && label2->label_num == ancient_block->block_id &&
                 false_label->label_num == block->block_id)
        {
            true_val  = op1;
            false_val = op2;
        }
        else if (label2->label_num == true_label->label_num && label1->label_num == ancient_block->block_id &&
                 false_label->label_num == block->block_id)
        {
            true_val  = op2;
            false_val = op1;
        }
        else if (label1->label_num == false_label->label_num && label2->label_num == ancient_block->block_id &&
                 true_label->label_num == block->block_id)
        {
            true_val  = op2;
            false_val = op1;
        }
        else if (label2->label_num == false_label->label_num && label1->label_num == ancient_block->block_id &&
                 true_label->label_num == block->block_id)
        {
            true_val  = op1;
            false_val = op2;
        }
        else
        {
            std::cerr << "If Conversion Error: Cannot determine true_val and false_val" << std::endl;
            true_val  = op1;
            false_val = op2;
        }

        auto* select_inst     = new LLVMIR::SelectInst(phi->type, true_val, false_val, icmp->res, phi->res);
        select_inst->block_id = block->block_id;

        auto phi_it = std::find(block->insts.begin(), block->insts.end(), phi);
        if (phi_it != block->insts.end()) *phi_it = select_inst;

        block->insts.insert(block->insts.begin(), icmp);
        icmp->block_id = block->block_id;

        modified_ = true;
    }

    void IfConversionPass::buildGraph(
        CFG* cfg, std::vector<std::vector<LLVMIR::IRBlock*>>& G, std::vector<std::vector<LLVMIR::IRBlock*>>& invG)
    {
        G    = cfg->G;
        invG = cfg->invG;
    }

    std::vector<LLVMIR::IRBlock*> IfConversionPass::getPredecessors(LLVMIR::IRBlock* block, CFG* cfg) const
    {
        std::vector<LLVMIR::IRBlock*> predecessors;

        if (block->block_id < (int)cfg->invG.size()) predecessors = cfg->invG[block->block_id];

        return predecessors;
    }

    std::vector<LLVMIR::IRBlock*> IfConversionPass::getSuccessors(LLVMIR::IRBlock* block, CFG* cfg) const
    {
        std::vector<LLVMIR::IRBlock*> successors;

        if (block->block_id < (int)cfg->G.size()) successors = cfg->G[block->block_id];

        return successors;
    }

}  // namespace Transform
