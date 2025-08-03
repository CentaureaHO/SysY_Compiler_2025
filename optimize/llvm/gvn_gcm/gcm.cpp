#include "gcm.h"
#include "cfg.h"
#include "dom_analyzer.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/ir_builder.h"
#include <cassert>
#include <cstddef>
#include <deque>
#include <iostream>
#include <ostream>
#include <unordered_set>
#include <functional>
#include <fstream>

namespace LLVMIR
{
    static CFG* get_cfg_by_name(IR* ir, const std::string& name)
    {
        for (auto& func : ir->functions)
        {
            if (func->func_def->func_name == name)
            {
                if (ir->cfg.count(func->func_def)) return ir->cfg.at(func->func_def);
            }
        }
        return nullptr;
    }

    // 得到支配树的后序遍历
    static std::vector<IRBlock*> get_post_order(CFG* cfg, Cele::Algo::DomAnalyzer* dom_tree)
    {
        std::vector<IRBlock*>         post_order;
        std::unordered_set<IRBlock*>  visited;
        std::function<void(IRBlock*)> dfs = [&](IRBlock* block) {
            if (visited.count(block)) return;
            visited.insert(block);
            for (auto& succ : dom_tree->dom_tree[block->block_id]) { dfs(cfg->block_id_to_block[succ]); }
            post_order.push_back(block);
        };
        dfs(cfg->block_id_to_block[0]);  // 从入口块开始遍历
        return post_order;
    }

    bool GCM::isControlDependent(CFG* cfg, Instruction* inst, int target_id)
    {
        auto origin_id      = inst->block_id;
        auto control_blocks = cdgAnalyzer->GetCDGPre(cfg, origin_id);
        for (auto pre : control_blocks)
        {
            if (!domAnalyzer->isDomate(pre->block_id, target_id)) { return false; }
        }
        return true;
    }

    void GCM::CollectPhiVals(CFG* cfg)
    {
        for (auto [id, block] : cfg->block_id_to_block)
        {
            for (auto inst : block->insts)
            {
                if (inst->opcode != IROpCode::PHI) { continue; }
                auto phi_inst = dynamic_cast<PhiInst*>(inst);
                if (phi_inst)
                {
                    for (auto [val, label] : phi_inst->vals_for_labels)
                    {
                        if (!PhiVals.count(val)) { PhiVals.insert(val); }
                    }
                }
            }
        }
    }

    bool GCM::isSafeCall(Instruction* inst)
    {
        auto call_inst = dynamic_cast<CallInst*>(inst);
        if (call_inst)
        {
            auto func_cfg = get_cfg_by_name(ir, call_inst->func_name);
            // 说明这个函数没有副作用
            if ((func_cfg && aliasAnalyser->isNoSideEffect(func_cfg)) || call_inst->func_name == "llvm.memset.p0.i32")
            {
                return true;
            }
        }
        return false;
    }

    bool GCM::IsSafeInst(CFG* cfg, Instruction* inst)
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
            case IROpCode::GETELEMENTPTR:
            {
                // 因为我们这些指令根本不会涉及到全局变量，所以可以直接返回 true
                return true;
            }
            case IROpCode::ALLOCA:
            {
                return false;  // ALLOCA 指令不能移动
            }
            case IROpCode::CALL:
            {
                if (isSafeCall(inst)) { return true; }
                else { return false; }
            }
            case IROpCode::LOAD:
            {
                // 其实可以,但是这里对samples没有意义了
                return false;
            }
            case IROpCode::STORE:
            {
                return true;
            }
            // PHI不能移动,所以这里用到PHI的也不能移动
            default:
            {
                return false;
            }
        }
    }

    int GCM::ComputeEarliestBlockId(CFG* cfg, Instruction* inst)
    {
        int  currentBlockId = inst->block_id;
        auto result_op      = inst->GetResultOperand();
        if (result_op)
        {
            if (PhiVals.count(result_op)) { return -1; }
        }

        // 所有用到这条def的指令块
        std::unordered_set<int> def_blocks;
        // 首先得到所有用到这条指令的
        auto all_use = defuseAnalysis->getUses(cfg, inst->GetResultOperand());
        for (auto* use : all_use)
        {
            // 我们需要所有考虑所有的使用
            if (use) { def_blocks.insert(use->block_id); }
        }

        int E = -1;
        if (def_blocks.empty())
        {
            // 如果没有被使用，那么说明这个def没用,或者没有返回resultop
            if (inst->opcode == IROpCode::CALL)
            {
                auto call_inst = dynamic_cast<CallInst*>(inst);
                if (call_inst && call_inst->func_name == "llvm.memset.p0.i32") { return 0; }
            }
            E = 0;
        }
        else
        {  // 找所有定义块的最近共同祖先（LCA）
            E = *def_blocks.begin();
            for (int blk : def_blocks) { E = domAnalyzer->LCA(E, blk); }
            if (E == -1)
            {
                E = currentBlockId;  // 如果没有找到有效的E，返回当前块ID
            }
        }
        // 基于该指令使用的所有操作数，进行判断
        for (auto op : inst->GetUsedOperands())
        {
            auto def_inst = defuseAnalysis->getDef(cfg, op);
            if (def_inst)
            {
                if (def_inst->opcode == IROpCode::PHI)
                {
                    // 我们先不处理phi
                    return -1;
                }
                auto def_id = earliestBlockId.count(def_inst) ? earliestBlockId[def_inst] : def_inst->block_id;
                if (!domAnalyzer->isDomate(def_id, E))
                {
                    E = currentBlockId;
                    break;
                }
            }
        }

        // 判断是否满足控制依赖
        if (!isControlDependent(cfg, inst, E)) { E = currentBlockId; }
        if (domAnalyzer->isDomate(inst->block_id, E)) { return -1; }
        return E;
    }

    int GCM::ComputeLatestBlockId(CFG* cfg, Instruction* inst)
    {
        int L = -1;

        // 因为没有use,所以不用考虑

        // 这里只计算store指令
        // auto store_inst = dynamic_cast<StoreInst*>(inst);
        // auto ptr = store_inst->ptr;
        // auto val = store_inst->val;
        // auto def_ptr = defuseAnalysis->getDef(cfg, ptr);
        // auto def_val = defuseAnalysis->getDef(cfg, val);
        // if(!domAnalyzer->isDomate(def_ptr->block_id, ))
    }

    void GCM::GenerateInformation(CFG* func_cfg)
    {
        for (auto& [inst, E] : earliestBlockId)
        {
            // int L = latestBlockId[inst];
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
    }

    void GCM::EraseInstructions(CFG* func_cfg)
    {
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
    }

    void GCM::MoveInstructions(CFG* func_cfg)
    {
        // 接下来进行重新处理
        for (auto [id, block] : func_cfg->block_id_to_block)
        {
            std::deque<Instruction*> new_insts;
            for (auto& inst : block->insts)
            {
                // 如果到达barrier
                if (inst->opcode == IROpCode::BR_COND || inst->opcode == IROpCode::BR_UNCOND ||
                    inst->opcode == IROpCode::RET)
                {
                    // 将最新的指令添加到当前块
                    if (latest_map.find(id) != latest_map.end())
                    {
                        for (auto& [_, moved_inst] : latest_map[id])
                        {
                            if (moved_inst->opcode == IROpCode::CALL)
                            {
                                auto call_inst = dynamic_cast<CallInst*>(moved_inst);
                            }
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
    }

    void GCM::Execute()
    {
        for (auto& [func, cfg] : ir->cfg)
        {
            CollectPhiVals(cfg);
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
            instorder.clear();
            params.clear();
        }
    }

    void GCM::ExecuteInSingleCFG(CFG* func_cfg)
    {
        this->domAnalyzer     = ir->DomTrees[func_cfg];
        this->postdomAnalyzer = ir->ReDomTrees[func_cfg];
        // 记录下指令的顺序
        int order = 0;

        // 记录下函数参数
        for (auto arg : func_cfg->func->func_def->arg_regs) { params.insert(arg); }
        auto post_order = get_post_order(func_cfg, domAnalyzer);
        for (size_t i = 0; i < post_order.size(); i++)
        {
            auto block = post_order[i];
            for (auto& inst : block->insts)
            {
                if (!IsSafeInst(func_cfg, inst)) { continue; }

                int E = ComputeEarliestBlockId(func_cfg, inst);
                // int L = ComputeLatestBlockId(func_cfg, inst);
                int L = 0;

                if (E != -1)
                {
                    earliestBlockId[inst] = E;
                    latestBlockId[inst]   = L;
                    instorder[inst]       = order++;

                    // inst->block_id           = E;               // 更新指令的基本块ID
                    // original_block_ids[inst] = inst->block_id;  // 记录初始块ID
                }
            }
        }
    }
}  // namespace LLVMIR