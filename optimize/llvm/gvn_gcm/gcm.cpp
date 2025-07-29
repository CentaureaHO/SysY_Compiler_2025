#include "gcm.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include <cassert>
#include <deque>
#include <ostream>
#include <unordered_set>
// #define DEBUG_GCM
// #define DEBUG_PHI
// #define DEBUG_CALL
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
            {  // case IROpCode::GETELEMENTPTR:
#ifdef DEBUG_CALL
                if (inst->opcode == IROpCode::CALL)
                {
                    std::cout << "In IsSafeInst, inst is " << inst->opcode << " in block " << inst->block_id
                              << std::endl;
                    std::cout << inst->GetUsedOperands().size() << std::endl;
                    for (auto& op : inst->GetUsedOperands()) { std::cout << "Operand: " << op->getName() << std::endl; }
                }
#endif
                for (auto& op : inst->GetUsedOperands())
                {
                    if (op->type == OperandType::GLOBAL)
                    {
                        if (inst->GetResultOperand()) { cannot_move.insert(inst->GetResultOperand()); }
                        return false;  // 如果有全局变量，不能处理
                    }
                }
                return true;
            }
            // PHI不能移动,所以这里用到PHI的也不能移动
            default:
            {
                if (inst->GetResultOperand()) { cannot_move.insert(inst->GetResultOperand()); }
                return false;
            }
        }
    }

    int GCM::ComputeEarliestBlockId(CFG* cfg, Instruction* inst)
    {
        int currentBlockId = inst->block_id;

        // 所有操作数的定义块
        std::set<int> def_blocks;
#ifdef DEBUG_GCM
        std::cout << "Processing instruction: " << inst->opcode << " in block " << currentBlockId << std::endl;
        std::cout << "used operands size: " << inst->GetUsedOperands().size() << std::endl;
#endif
        for (auto* op : inst->GetUsedOperands())
        {
#ifdef DEBUG_GCM
            std::cout << op->getName() << std::endl;
#endif
            if (cannot_move.find(op) != cannot_move.end())
            {
#ifdef DEBUG_GCM
                std::cout << "Cannot move operand: " << op->getName() << std::endl;
#endif
                cannot_move.insert(inst->GetResultOperand());
                return -1;  // 如果操作数不能移动，返回 -1
            }
            if (op->type == OperandType::REG)
            {
                auto         reg_op   = dynamic_cast<RegOperand*>(op);
                int          reg_num  = reg_op->reg_num;
                Instruction* def_inst = defuseAnalysis->GetDefMap(cfg)[reg_num];
#ifdef DEBUG_GCM
                if (def_inst) { std::cout << def_inst->opcode << std::endl; }
                else { std::cout << "No def_inst found for reg: " << reg_num << std::endl; }
#endif
                if (def_inst) { def_blocks.insert(def_inst->block_id); }
            }
        }

#ifdef DEBUG_GCM
        std::cout << "Def blocks: ";
        for (int blk : def_blocks) std::cout << blk << " ";
        std::cout << "here" << std::endl;
#endif
        // 找所有定义块的最近共同祖先（LCA）
        int E = *def_blocks.begin();
#ifdef DEBUG_GCM
        std::cout << "Initial E: " << E << std::endl;
#endif
        for (int blk : def_blocks) { E = domAnalyzer->LCA(E, blk); }

        return E;
    }

    int GCM::ComputeLatestBlockId(CFG* cfg, Instruction* inst)
    {
        std::set<int> use_blocks;
        auto          result_op = inst->GetResultOperand();
        if (cannot_move.find(result_op) != cannot_move.end())
        {
#ifdef DEBUG_GCM
            std::cout << "Cannot move result operand: " << result_op->getName() << std::endl;
#endif
            cannot_move.insert(result_op);  // 如果结果操作数不能移动，记录下来
            return -1;                      // 如果结果操作数不能移动，返回 -1
        }
        auto reg_op = dynamic_cast<RegOperand*>(result_op)->reg_num;
        if (reg_op == -1) { return -1; }                          // 如果没有结果寄存器，直接返回 -1
        for (auto& use : defuseAnalysis->GetUseMap(cfg)[reg_op])  // 你应该有 use 集合
        {
            // if (reg_op == 24)
            // {
            //     std::cout << use->GetResultReg() << std::endl;
            //     std::cout << "In ComputeLatestBlockId, use is " << use->opcode << " in block " << use->block_id
            //               << std::endl;
            // }
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

    void GCM::GenerateInformation(CFG* func_cfg)
    {
        for (auto& [inst, E] : earliestBlockId)
        {
            int L = latestBlockId[inst];
            if (E == L) continue;               // 已在最优位置，无需移动
            if (E == inst->block_id) continue;  // 如果 E 是当前块，说明不需要移动

            // 记录下删除的指令
            erase_set.insert(inst);

            // 记录下需要放到最早可执行块E的指令,注意除去 terminator
            if (E == -1)
            {
                std::cout << "Warning: E is -1 for instruction: " << inst->opcode << std::endl;
                continue;  // 如果 E 是 -1，说明没有可执行的块，跳
            }
            latest_map[E].insert({instorder[inst], inst});
        }
#ifdef DEBUG_GCM
        std::cout << "Generate information completed" << std::endl;
        std::cout << "Erase set size: " << erase_set.size() << std::endl;
        std::cout << "Latest map size: " << latest_map.size() << std::endl;
#endif
    }

    void GCM::EraseInstructions(CFG* func_cfg)
    {
#ifdef DEBUG_GCM
        std::cout << "Erase: func is " << func_cfg->func->func_def->func_name << std::endl;
#endif

        for (auto [id, block] : func_cfg->block_id_to_block)
        {
            std::deque<Instruction*> new_insts;
            for (auto& inst : block->insts)
            {
                // 如果指令不在 erase_set 中，则保留
                if (erase_set.find(inst) == erase_set.end()) { new_insts.push_back(inst); }
            }
            block->insts = std::move(new_insts);
        }
#ifdef DEBUG_GCM
        std::cout << "Erase instructions of " << func_cfg->func->func_def->func_name << " completed" << std::endl;
#endif
    }

    void GCM::MoveInstructions(CFG* func_cfg)
    {
#ifdef DEBUG_GCM
        std::cout << "Move: func is " << func_cfg->func->func_def->func_name << std::endl;
#endif
        // 接下来进行重新处理
        for (auto [id, block] : func_cfg->block_id_to_block)
        {
            std::deque<Instruction*> new_insts;
            for (auto& inst : block->insts)
            {
                // 如果到达了结尾
                if (inst->opcode == IROpCode::BR_COND || inst->opcode == IROpCode::BR_UNCOND ||
                    inst->opcode == IROpCode::RET)
                {
                    // 将最新的指令添加到当前块
                    if (latest_map.find(id) != latest_map.end())
                    {
                        for (auto& [_, moved_inst] : latest_map[id])
                        {
                            new_insts.push_back(moved_inst);
                            moved_inst->block_id = id;
                        }
                    }
                }
                // 我们已经删除了需要删去的,所以这里只需要添加当前指令
                new_insts.push_back(inst);
            }
            block->insts = std::move(new_insts);
        }
#ifdef DEBUG_GCM
        std::cout << "Move instructions of " << func_cfg->func->func_def->func_name << " completed" << std::endl;
#endif
    }

    void GCM::Execute()
    {
        for (auto& [func, cfg] : ir->cfg)
        {
#ifdef DEBUG_GCM
            std::cout << "Processing GCM for function: " << func->func_name << std::endl;
#endif
            ExecuteInSingleCFG(cfg);
            // 生成移动辅助信息
            GenerateInformation(cfg);
            // 移除指令
            EraseInstructions(cfg);
            // 移动指令
            MoveInstructions(cfg);

            earliestBlockId.clear();
            latestBlockId.clear();
            erase_set.clear();
            latest_map.clear();
            cannot_move.clear();
            instorder.clear();
        }
#ifdef DEBUG_GCM
        std::cout << "GCM completed" << std::endl;
#endif
    }

    void GCM::ExecuteInSingleCFG(CFG* func_cfg)
    {
        this->domAnalyzer     = ir->DomTrees[func_cfg];
        this->postdomAnalyzer = ir->ReDomTrees[func_cfg];
        // 记录下指令的顺序
        int order = 0;

#ifdef DEBUG_GCM
        std::cout << "Processing GCM for function: " << func_cfg->func->func_def->func_name << std::endl;
#endif
        for (auto& [id, block] : func_cfg->block_id_to_block)
        {
#ifdef DEBUG_GCM
            int cnt = 0;
            std::cout << "Processing block: " << id << std::endl;
#endif
            for (auto& inst : block->insts)
            {
#ifdef DEBUG_GCM
                std::cout << "In processing instruction NO is " << ++cnt << std::endl;
#endif
                if (!IsSafeInst(inst)) { continue; }
// 是可以处理的
#ifdef DEBUG_CALL
                std::cout << "In ExecuteInSingleCFG, inst is " << inst->opcode << " in block " << inst->block_id
                          << std::endl;
#endif
                int E = ComputeEarliestBlockId(func_cfg, inst);
                int L = ComputeLatestBlockId(func_cfg, inst);

                if (E != -1 && L != -1)
                {
                    earliestBlockId[inst] = E;
                    latestBlockId[inst]   = L;
                    instorder[inst]       = order++;
                    inst->block_id        = E;
                }
            }
        }
    }
}  // namespace LLVMIR