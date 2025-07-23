#pragma once

#include "gcm.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include <unordered_set>

namespace LLVMIR
{
    bool GCM::IsSafeInst(Instruction* inst)
    {
        switch (inst->opcode)
        {
            case IROpCode::ADD:
            case IROpCode::SUB:
            case IROpCode::MUL:
            case IROpCode::DIV:
            case IROpCode::MOD:
            case IROpCode::FADD:
            case IROpCode::FSUB:
            case IROpCode::FMUL:
            case IROpCode::FDIV:
            case IROpCode::BITXOR:
            case IROpCode::BITAND:
            case IROpCode::SHL:
            case IROpCode::ASHR:
            case IROpCode::LSHR:
            case IROpCode::ICMP:
            case IROpCode::FCMP:
            case IROpCode::ZEXT:
            case IROpCode::FPTOSI:
            case IROpCode::SITOFP:
            case IROpCode::FPEXT:
            case IROpCode::GETELEMENTPTR: return true;

            default: return false;
        }
    }

    int GCM::ComputeEarliestBlockId(CFG* cfg, Instruction* inst)
    {
        int currentBlockId = inst->block_id;

        // 所有操作数的定义块
        std::set<int> def_blocks;

        for (auto* op : inst->GetUsedOperands())
        {
            if (op->type == OperandType::REG)
            {
                auto         reg_op   = dynamic_cast<RegOperand*>(op);
                int          reg_num  = reg_op->reg_num;
                Instruction* def_inst = defuseAnalysis->GetDefMap(cfg)[reg_num];  // 自己维护一个 def 表
                def_blocks.insert(def_inst->block_id);
            }
        }

        // 找所有定义块的最近共同祖先（LCA）
        int E = *def_blocks.begin();
        for (int blk : def_blocks) { E = domAnalyzer->LCA(E, blk); }

        return E;
    }

    int GCM::ComputeLatestBlockId(CFG* cfg, Instruction* inst)
    {
        std::set<int> use_blocks;
        auto          result_op = inst->GetResultReg();
        if (result_op == -1) { return -1; }                          // 如果没有结果寄存器，直接返回 -1
        for (auto& use : defuseAnalysis->GetUseMap(cfg)[result_op])  // 你应该有 use 集合
        {
            use_blocks.insert(use->block_id);
        }

        // 求 use block 的最近公共后支配点（post-dominator）
        int L = *use_blocks.begin();
        for (int blk : use_blocks)
        {
            L = postdomAnalyzer->LCA(L, blk);  // 后支配树中的 LCA
        }

        return L;
    }

    void GCM::MoveInstructions(CFG* func_cfg)
    {
        auto                                  id2block = func_cfg->block_id_to_block;
        std::unordered_set<Instruction*>      erase_set;
        std::unordered_map<Instruction*, int> move_map;

        for (auto& [inst, E] : earliestBlockId)
        {
            int L = latestBlockId[inst];
            if (E == L) continue;  // 已在最优位置，无需移动

            // 1. 从原始位置移除
            // IRBlock* old_block = id2block[inst->block_id];
            // old_block->insts.remove(inst);  // 你也可以使用 erase+find
            erase_set.insert(inst);

            // 2. 移动到最早可执行块 E 的结尾（除去 terminator）
            IRBlock* target_block = id2block[E];
            inst->block_id        = E;

            // 插入点在跳转指令（terminator）之前
            auto it  = target_block->insts.end();
            auto itr = target_block->insts.end();
            --itr;  // 指向最后一个指令
            for (; itr != target_block->insts.begin(); --itr)
            {
                if ((*itr)->opcode == IROpCode::BR_COND || (*itr)->opcode == IROpCode::BR_UNCOND ||
                    (*itr)->opcode == IROpCode::RET)
                {
                    it = itr;
                    break;
                }
            }
            target_block->insts.insert(it, inst);
        }
    }

    void GCM::Execute()
    {
        for (auto& [func, cfg] : ir->cfg) { ExecuteInSingleCFG(cfg); }
    }

    void GCM::ExecuteInSingleCFG(CFG* func_cfg)
    {
        for (auto& [id, block] : func_cfg->block_id_to_block)
        {
            for (auto& inst : block->insts)
            {
                if (!IsSafeInst(inst)) { continue; }
                // 是可以处理的
                int E = ComputeEarliestBlockId(func_cfg, inst);
                int L = ComputeLatestBlockId(func_cfg, inst);

                earliestBlockId[inst] = E;
                latestBlockId[inst]   = L;
            }
        }
    }
}  // namespace LLVMIR