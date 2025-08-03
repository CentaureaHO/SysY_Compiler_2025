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
#include <queue>
#include <unordered_set>
#include <functional>

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

    void GCM::CollectPhiValsAndMem(CFG* cfg)
    {
        for (auto [id, block] : cfg->block_id_to_block)
        {
            for (auto inst : block->insts)
            {
                if (inst->opcode == IROpCode::PHI)
                {
                    auto phi_inst = dynamic_cast<PhiInst*>(inst);
                    if (phi_inst)
                    {
                        for (auto [val, label] : phi_inst->vals_for_labels)
                        {
                            if (!PhiVals.count(val)) { PhiVals.insert(val); }
                        }
                    }
                }
                if (inst->opcode == IROpCode::STORE || inst->opcode == IROpCode::LOAD ||
                    inst->opcode == IROpCode::GETELEMENTPTR)
                {
                    MemSet.insert(inst);
                }
                else { continue; }
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

    void GCM::handler(CFG* cfg, Operand* op, std::unordered_set<int>& used_blocks, Operand* Base_ptr, Operand* Base_val)
    {
        if (op && (op->type == OperandType::REG || op->type == OperandType::GLOBAL))
        {
            if (Base_ptr)
            {
                if (aliasAnalyser->queryAlias(Base_ptr, op, cfg))
                {
                    auto def = defuseAnalysis->getDef(cfg, op);
                    if (def)
                    {
                        if (!used_blocks.count(def->block_id)) { used_blocks.insert(def->block_id); }
                    }
                }
            }
            if (Base_val && Base_val->type == OperandType::REG)
            {
                // 如果是地址访问
                if (aliasAnalyser->queryAlias(Base_val, op, cfg))
                {
                    auto def = defuseAnalysis->getDef(cfg, op);
                    if (def)
                    {
                        if (!used_blocks.count(def->block_id)) { used_blocks.insert(def->block_id); }
                    }
                }
            }
        }
    }

    std::unordered_set<int> GCM::GetAllPaths(CFG* cfg, int start, int end)
    {
        std::unordered_set<int> ret    = {};
        std::queue<int>         blocks = {};
        std::unordered_set<int> visited;
        blocks.push(start);
        while (!blocks.empty())
        {
            auto curr_block = blocks.front();
            blocks.pop();
            visited.insert(curr_block);
            ret.insert(curr_block);
            if (curr_block == end) { continue; }
            auto block        = cfg->block_id_to_block[curr_block];
            auto control_inst = block->insts.back();
            if (control_inst->opcode == IROpCode::BR_COND)
            {
                auto br_inst = dynamic_cast<BranchCondInst*>(control_inst);
                if (br_inst)
                {
                    auto true_label  = dynamic_cast<LabelOperand*>(br_inst->true_label);
                    auto false_label = dynamic_cast<LabelOperand*>(br_inst->false_label);
                    if (true_label && !visited.count(true_label->label_num)) blocks.push(true_label->label_num);
                    if (false_label && !visited.count(false_label->label_num)) blocks.push(false_label->label_num);
                }
            }
            else if (control_inst->opcode == IROpCode::BR_UNCOND)
            {
                auto brun_inst = dynamic_cast<BranchUncondInst*>(control_inst);
                if (brun_inst)
                {
                    auto target_label = dynamic_cast<LabelOperand*>(brun_inst->target_label);
                    if (target_label && !visited.count(target_label->label_num)) blocks.push(target_label->label_num);
                }
            }
            else if (control_inst->opcode == IROpCode::RET) { continue; }
            else
            {
                return std::unordered_set<int>{};
            }
        }
        return ret;
    }

    int GCM::ComputeLatestBlockId(CFG* cfg, Instruction* inst)
    {
        int L              = -1;
        int currentBlockId = inst->block_id;

        // 对于store指令,我们进行sink,对于所有使用到ptr和val的指令,我们都要进行判断
        auto store_inst = dynamic_cast<StoreInst*>(inst);
        auto ptr        = store_inst ? store_inst->ptr : nullptr;
        auto val        = store_inst ? store_inst->val : nullptr;
        if (!ptr || !val) return -1;
        std::unordered_set<int> used_blocks;
        // 我们都需要考虑是否能够正确进行别名分析

        for (auto inst : MemSet)
        {
            if (inst->opcode == IROpCode::LOAD)
            {
                // 对于load,我们处理ptr
                auto load_inst = dynamic_cast<LoadInst*>(inst);
                if (load_inst) { handler(cfg, load_inst->ptr, used_blocks, ptr, val); }
            }
            if (inst->opcode == IROpCode::STORE)
            {
                // 对于store,处理ptr和val
                auto store_inst = dynamic_cast<StoreInst*>(inst);
                if (store_inst)
                {
                    handler(cfg, store_inst->ptr, used_blocks, ptr, val);
                    handler(cfg, store_inst->val, used_blocks, ptr, val);
                }
            }
            if (inst->opcode == IROpCode::GETELEMENTPTR)
            {
                // 对于getelementptr,处理所有参数
                auto gep_inst = dynamic_cast<GEPInst*>(inst);
                if (gep_inst)
                {
                    handler(cfg, gep_inst->base_ptr, used_blocks, ptr, val);
                    for (auto& idx : gep_inst->idxs) { handler(cfg, idx, used_blocks, ptr, val); }
                }
            }
        }

        // 处理postdominator
        if (used_blocks.empty())
        {
            return -1;
        }
        else
        {
            L = *used_blocks.begin();
            for (int blk : used_blocks) { L = postdomAnalyzer->LCA(L, blk); }
            if (L == -1)
            {
                L = currentBlockId;  // 如果没有找到有效的L，返回当前块ID
            }
        }

        if (L == -1) { return L; }
        if (postdomAnalyzer->isDomate(currentBlockId, L)) { return -1; }
        // 判断所有路径上都没有再次使用
        auto all_paths = GetAllPaths(cfg, currentBlockId, L);
        // 先简单检查
        for (auto inst : MemSet)
        {
            if (all_paths.count(inst->block_id))
            {
                // 说明存在内存操作
                if (inst->opcode == IROpCode::LOAD)
                {
                    auto load_inst = dynamic_cast<LoadInst*>(inst);
                    if (aliasAnalyser->queryAlias(load_inst->ptr, ptr, cfg))
                    {
                        // 存在别名关系
                        // 不能sink
                        L = -1;
                        break;
                    }
                }
                else if (inst->opcode == IROpCode::STORE)
                {
                    auto store_inst = dynamic_cast<StoreInst*>(inst);
                    if (aliasAnalyser->queryAlias(store_inst->ptr, ptr, cfg) ||
                        aliasAnalyser->queryAlias(store_inst->val, val, cfg))
                    {
                        // 存在别名关系
                        // 不能sink
                        L = -1;
                        break;
                    }
                }
            }
        }
        return L;
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
            latest_map[E].insert({instorder[inst], inst});
        }

        // 处理sink的
        for (auto& [inst, L] : latestBlockId)
        {
            if (L == inst->block_id) continue;

            erase_set.insert(inst);

            latest_map[L].insert({instorder[inst], inst});
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
            CollectPhiValsAndMem(cfg);
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
            PhiVals.clear();
            MemSet.clear();
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
                if (inst->opcode == IROpCode::STORE)
                {
                    int L = 0;
                    L     = ComputeLatestBlockId(func_cfg, inst);
                    if (L != -1)
                    {
                        latestBlockId[inst] = L;
                        instorder[inst]     = order++;
                    }
                    continue;
                }

                int E = ComputeEarliestBlockId(func_cfg, inst);

                if (E != -1)
                {
                    earliestBlockId[inst] = E;
                    instorder[inst]       = order++;

                    // inst->block_id           = E;               // 更新指令的基本块ID
                    // original_block_ids[inst] = inst->block_id;  // 记录初始块ID
                }
            }
        }
    }
}  // namespace LLVMIR