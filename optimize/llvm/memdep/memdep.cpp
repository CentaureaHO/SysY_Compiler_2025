#include "llvm/memdep/memdep.h"
#include <queue>
#include <set>
#include <algorithm>
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include "cfg.h"
#include "llvm/alias_analysis/alias_analysis.h"

namespace Analysis
{
    static CFG* getCfgByName(LLVMIR::IR* ir, const std::string& name)
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

    MemoryDependenceAnalyser::MemoryDependenceAnalyser(LLVMIR::IR* ir, AliasAnalyser* aa) : ir(ir), alias_analyser(aa)
    {}

    static std::set<LLVMIR::Instruction*> getLoadClobbers(
        LLVMIR::Instruction* inst, CFG* cfg, AliasAnalyser* alias_analyser, LLVMIR::IR* ir)
    {
        std::set<LLVMIR::Instruction*> res;

        LLVMIR::Operand* ptr;
        assert(inst->opcode == LLVMIR::IROpCode::LOAD || inst->opcode == LLVMIR::IROpCode::STORE);
        if (inst->opcode == LLVMIR::IROpCode::LOAD)
            ptr = dynamic_cast<LLVMIR::LoadInst*>(inst)->ptr;
        else
            ptr = dynamic_cast<LLVMIR::StoreInst*>(inst)->ptr;

        auto inst_bb = cfg->block_id_to_block.at(inst->block_id);

        int i_idx = -1;
        for (int i = 0; i < (int)inst_bb->insts.size(); ++i)
        {
            if (inst_bb->insts[i] == inst)
            {
                i_idx = i;
                break;
            }
        }
        assert(i_idx != -1);

        for (int i = i_idx - 1; i >= 0; --i)
        {
            auto tmp_inst = inst_bb->insts[i];
            if (tmp_inst->opcode == LLVMIR::IROpCode::STORE)
            {
                auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(tmp_inst);
                if (alias_analyser->queryAlias(ptr, store_inst->ptr, cfg) == AliasAnalyser::MustAlias)
                {
                    res.insert(store_inst);
                    return res;
                }
            }
            else if (tmp_inst->opcode == LLVMIR::IROpCode::CALL)
            {
                auto call_inst  = dynamic_cast<LLVMIR::CallInst*>(tmp_inst);
                auto call_name  = call_inst->func_name;
                auto target_cfg = getCfgByName(ir, call_name);

                if (!target_cfg)
                {
                    res.insert(call_inst);
                    return res;
                }
                if (!alias_analyser->isNoSideEffect(target_cfg))
                {
                    res.insert(call_inst);
                    return res;
                }
            }
        }

        std::queue<LLVMIR::IRBlock*> q;
        for (auto pred : cfg->invG[inst_bb->block_id]) q.push(pred);

        if (inst_bb->block_id == 0 && q.empty())
        {
            res.insert(cfg->func->func_def);
            return res;
        }

        std::map<LLVMIR::IRBlock*, bool> visited;
        while (!q.empty())
        {
            auto x = q.front();
            q.pop();
            if (visited[x]) continue;
            visited[x] = true;

            bool found = false;
            for (int i = x->insts.size() - 1; i >= 0; --i)
            {
                auto tmp_inst = x->insts[i];
                if (tmp_inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(tmp_inst);
                    if (alias_analyser->queryAlias(ptr, store_inst->ptr, cfg) == AliasAnalyser::MustAlias)
                    {
                        res.insert(store_inst);
                        found = true;
                        break;
                    }
                }
                else if (tmp_inst->opcode == LLVMIR::IROpCode::CALL)
                {
                    auto call_inst  = dynamic_cast<LLVMIR::CallInst*>(tmp_inst);
                    auto call_name  = call_inst->func_name;
                    auto target_cfg = getCfgByName(ir, call_name);

                    if (!target_cfg)
                    {
                        res.insert(call_inst);
                        found = true;
                        break;
                    }
                    if (!alias_analyser->isNoSideEffect(target_cfg))
                    {
                        res.insert(call_inst);
                        found = true;
                        break;
                    }
                }
            }
            if (!found)
            {
                for (auto pred : cfg->invG[x->block_id]) q.push(pred);
            }
            if (x->block_id == 0 && !found) res.insert(cfg->func->func_def);
        }

        return res;
    }

    static bool isExternalCallReadPtr(
        LLVMIR::CallInst* inst, LLVMIR::Operand* ptr, CFG* cfg, AliasAnalyser* alias_analyser)
    {
        auto n = inst->func_name;
        if (n == "getint" || n == "getch" || n == "getfloat" || n == "putint" || n == "putch" || n == "putfloat" ||
            n == "_sysy_starttime" || n == "_sysy_stoptime" || n == "llvm.memset.p0.i32")
            return false;

        if (n == "putarray" || n == "putfarray")
        {
            assert(inst->args.size() == 2);
            auto arg2 = inst->args[1].second;
            assert(arg2->type == LLVMIR::OperandType::REG);
            auto res = alias_analyser->queryAlias(ptr, arg2, cfg);
            if (res == AliasAnalyser::MustAlias) return true;
            return false;
        }
        return true;
    }

    static std::set<LLVMIR::Instruction*> getStorePostClobbers(
        LLVMIR::Instruction* inst, CFG* cfg, AliasAnalyser* alias_analyser, LLVMIR::IR* ir)
    {
        std::set<LLVMIR::Instruction*> res;
        assert(inst->opcode == LLVMIR::IROpCode::STORE);
        auto ptr = dynamic_cast<LLVMIR::StoreInst*>(inst)->ptr;

        auto inst_bb = cfg->block_id_to_block.at(inst->block_id);

        int i_idx = -1;
        for (int i = 0; i < (int)inst_bb->insts.size(); ++i)
        {
            if (inst_bb->insts[i] == inst)
            {
                i_idx = i;
                break;
            }
        }
        assert(i_idx != -1);

        for (int i = i_idx + 1; i < (int)inst_bb->insts.size(); ++i)
        {
            auto tmp_inst = inst_bb->insts[i];
            if (tmp_inst->opcode == LLVMIR::IROpCode::LOAD)
            {
                auto load_inst = dynamic_cast<LLVMIR::LoadInst*>(tmp_inst);
                if (alias_analyser->queryAlias(ptr, load_inst->ptr, cfg) == AliasAnalyser::MustAlias)
                {
                    res.insert(load_inst);
                    return res;
                }
            }
            else if (tmp_inst->opcode == LLVMIR::IROpCode::CALL)
            {
                auto call_inst  = dynamic_cast<LLVMIR::CallInst*>(tmp_inst);
                auto call_name  = call_inst->func_name;
                auto target_cfg = getCfgByName(ir, call_name);

                if (!target_cfg)
                {
                    if (!isExternalCallReadPtr(call_inst, ptr, cfg, alias_analyser)) continue;
                    res.insert(call_inst);
                    return res;
                }
                if (!alias_analyser->isIndependent(target_cfg))
                {
                    res.insert(call_inst);
                    return res;
                }
            }
        }

        std::queue<LLVMIR::IRBlock*> q;
        for (auto succ : cfg->G[inst_bb->block_id]) q.push(succ);

        if (cfg->G[inst_bb->block_id].empty())
        {
            for (auto const& inst : cfg->func->blocks.back()->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::RET)
                {
                    res.insert(inst);
                    break;
                }
            }
            return res;
        }

        std::map<LLVMIR::IRBlock*, bool> visited;
        while (!q.empty())
        {
            auto x = q.front();
            q.pop();
            if (visited[x]) continue;
            visited[x] = true;

            bool found = false;
            for (auto tmp_inst : x->insts)
            {
                if (tmp_inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    auto load_inst = dynamic_cast<LLVMIR::LoadInst*>(tmp_inst);
                    if (alias_analyser->queryAlias(ptr, load_inst->ptr, cfg) == AliasAnalyser::MustAlias)
                    {
                        res.insert(load_inst);
                        found = true;
                        break;
                    }
                }
                else if (tmp_inst->opcode == LLVMIR::IROpCode::CALL)
                {
                    auto call_inst  = dynamic_cast<LLVMIR::CallInst*>(tmp_inst);
                    auto call_name  = call_inst->func_name;
                    auto target_cfg = getCfgByName(ir, call_name);

                    if (!target_cfg)
                    {
                        if (!isExternalCallReadPtr(call_inst, ptr, cfg, alias_analyser)) continue;
                        res.insert(call_inst);
                        found = true;
                        break;
                    }
                    if (!alias_analyser->isIndependent(target_cfg))
                    {
                        res.insert(call_inst);
                        found = true;
                        break;
                    }
                }
            }
            if (!found)
            {
                for (auto succ : cfg->G[x->block_id]) q.push(succ);
            }

            if (cfg->G[x->block_id].empty())
            {
                for (auto const& inst : cfg->func->blocks.back()->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::RET)
                    {
                        res.insert(inst);
                        break;
                    }
                }
            }
        }
        return res;
    }

    static std::set<int> getAllBlockInPath(int id1, int id2, CFG* cfg)
    {
        std::set<int>    ans;
        std::set<int>    id1_tos, to_id2s;
        std::queue<int>  q;
        std::vector<int> vis(cfg->func->max_label + 1, 0);

        q.push(id1);
        while (!q.empty())
        {
            int x = q.front();
            q.pop();
            if (vis[x]) continue;
            id1_tos.insert(x);
            vis[x] = 1;
            for (auto v : cfg->G[x]) q.push(v->block_id);
        }

        std::fill(vis.begin(), vis.end(), 0);
        q.push(id2);
        while (!q.empty())
        {
            int x = q.front();
            q.pop();
            if (vis[x]) continue;
            to_id2s.insert(x);
            vis[x] = 1;
            if (x == id1) continue;
            for (auto v : cfg->invG[x]) q.push(v->block_id);
        }

        for (int i = 0; i <= (int)cfg->func->max_label; ++i)
        {
            if (id1_tos.count(i) && to_id2s.count(i)) ans.insert(i);
        }
        return ans;
    }

    static bool isNoStore(
        LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg, AliasAnalyser* alias_analyser, LLVMIR::IR* ir)
    {
        LLVMIR::Operand* ptr;
        if (inst1->opcode == LLVMIR::IROpCode::LOAD)
            ptr = dynamic_cast<LLVMIR::LoadInst*>(inst1)->ptr;
        else if (inst1->opcode == LLVMIR::IROpCode::STORE)
            ptr = dynamic_cast<LLVMIR::StoreInst*>(inst1)->ptr;
        else
            return true;

        auto blocks = getAllBlockInPath(inst1->block_id, inst2->block_id, cfg);
        if (blocks.empty()) return false;

        for (auto id : blocks)
        {
            auto bb = cfg->block_id_to_block.at(id);
            int  st = 0, ed = bb->insts.size();
            if (id == inst1->block_id)
            {
                for (unsigned i = 0; i < bb->insts.size(); ++i)
                {
                    if (bb->insts[i] == inst1)
                    {
                        st = i + 1;
                        break;
                    }
                }
            }
            if (id == inst2->block_id)
            {
                for (unsigned i = 0; i < bb->insts.size(); ++i)
                {
                    if (bb->insts[i] == inst2)
                    {
                        ed = i;
                        break;
                    }
                }
            }

            for (int i = st; i < ed; ++i)
            {
                auto inst = bb->insts[i];
                if (inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(inst);
                    if (alias_analyser->queryAlias(ptr, store_inst->ptr, cfg) == AliasAnalyser::MustAlias) return false;
                }
                else if (inst->opcode == LLVMIR::IROpCode::CALL)
                {
                    auto call_inst  = dynamic_cast<LLVMIR::CallInst*>(inst);
                    auto call_name  = call_inst->func_name;
                    auto target_cfg = getCfgByName(ir, call_name);
                    if (!target_cfg || !alias_analyser->isNoSideEffect(target_cfg)) return false;
                }
            }
        }
        return true;
    }

    void MemoryDependenceAnalyser::run()
    {
        for (auto const& [func_def, cfg] : ir->cfg)
        {
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                for (auto inst : bb->insts) inst->block_id = bb->block_id;
            }
        }
    }

    bool MemoryDependenceAnalyser::isDepend(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg)
    {
        if (inst1->block_id != inst2->block_id) return true;

        int  idx1 = -1, idx2 = -1;
        auto bb = cfg->block_id_to_block.at(inst1->block_id);
        for (int i = 0; i < (int)bb->insts.size(); ++i)
        {
            if (bb->insts[i] == inst1) idx1 = i;
            if (bb->insts[i] == inst2) idx2 = i;
        }

        if (idx1 == -1 || idx2 == -1 || idx1 >= idx2) return true;

        LLVMIR::Operand* p1;
        if (inst1->opcode == LLVMIR::IROpCode::LOAD)
            p1 = dynamic_cast<LLVMIR::LoadInst*>(inst1)->ptr;
        else if (inst1->opcode == LLVMIR::IROpCode::STORE)
            p1 = dynamic_cast<LLVMIR::StoreInst*>(inst1)->ptr;
        else
            return false;

        for (int i = idx1 + 1; i < idx2; ++i)
        {
            auto inst = bb->insts[i];
            if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                auto p2 = dynamic_cast<LLVMIR::StoreInst*>(inst)->ptr;
                if (alias_analyser->queryAlias(p1, p2, cfg) != AliasAnalyser::NoAlias) return true;
            }
            else if (inst->opcode == LLVMIR::IROpCode::CALL)
            {
                auto call_cfg = getCfgByName(ir, dynamic_cast<LLVMIR::CallInst*>(inst)->func_name);
                if (!call_cfg || !alias_analyser->isNoSideEffect(call_cfg)) return true;
            }
        }
        return false;
    }

    bool MemoryDependenceAnalyser::haveMemAccessBetween(LLVMIR::Instruction* start, LLVMIR::Instruction* end, CFG* cfg)
    {
        if (start->block_id != end->block_id) return true;

        int  idx1 = -1, idx2 = -1;
        auto bb = cfg->block_id_to_block.at(start->block_id);
        for (int i = 0; i < (int)bb->insts.size(); ++i)
        {
            if (bb->insts[i] == start) idx1 = i;
            if (bb->insts[i] == end) idx2 = i;
        }

        if (idx1 == -1 || idx2 == -1 || idx1 >= idx2) return true;

        for (int i = idx1 + 1; i < idx2; ++i)
        {
            auto inst = bb->insts[i];
            if (inst->opcode == LLVMIR::IROpCode::LOAD || inst->opcode == LLVMIR::IROpCode::STORE ||
                inst->opcode == LLVMIR::IROpCode::CALL)
                return true;
        }
        return false;
    }

    bool MemoryDependenceAnalyser::isLoadSameMemory(LLVMIR::Instruction* a, LLVMIR::Instruction* b, CFG* cfg)
    {
        auto mem1 = getLoadClobbers(a, cfg, alias_analyser, ir);
        auto mem2 = getLoadClobbers(b, cfg, alias_analyser, ir);

        if (mem1.size() != mem2.size()) return false;

        for (auto inst : mem1)
        {
            if (mem2.find(inst) == mem2.end()) return false;
        }

        int id1 = a->block_id;
        int id2 = b->block_id;
        if (mem1.size() > 1 && id1 != id2)
        {
            auto dom_analyser = ir->DomTrees.at(cfg);

            bool            id1_dom_id2 = false;
            std::queue<int> q1;
            q1.push(id1);
            while (!q1.empty())
            {
                int u = q1.front();
                q1.pop();
                if (u == id2)
                {
                    id1_dom_id2 = true;
                    break;
                }
                if ((unsigned)u < dom_analyser->dom_tree.size())
                {
                    for (int v : dom_analyser->dom_tree[u]) q1.push(v);
                }
            }

            bool            id2_dom_id1 = false;
            std::queue<int> q2;
            q2.push(id2);
            while (!q2.empty())
            {
                int u = q2.front();
                q2.pop();
                if (u == id1)
                {
                    id2_dom_id1 = true;
                    break;
                }
                if ((unsigned)u < dom_analyser->dom_tree.size())
                {
                    for (int v : dom_analyser->dom_tree[u]) q2.push(v);
                }
            }

            if (id1_dom_id2)
            {
                if (isNoStore(a, b, cfg, alias_analyser, ir)) return true;
            }
            else if (id2_dom_id1)
            {
                if (isNoStore(b, a, cfg, alias_analyser, ir)) return true;
            }
            return false;
        }

        return true;
    }

    void MemoryDependenceAnalyser::memDepTest() {}

}  // namespace Analysis
