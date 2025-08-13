#include <backend/rv64/passes/stack_lowering.h>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <algorithm>

namespace
{
    void getDfsOrder(Cele::Algo::DomAnalyzer* dom_tree, int cur, std::vector<int>& order, std::map<int, int>& visited)
    {
        order.push_back(cur);
        visited[cur] = 1;
        for (auto child : dom_tree->dom_tree[cur])
        {
            if (visited.find(child) == visited.end())
            {
                visited[child] = 1;
                getDfsOrder(dom_tree, child, order, visited);
            }
        }
    }

    void getDepth(Cele::Algo::DomAnalyzer* dom_tree, int dom_root, std::map<int, int>& depth)
    {
        std::queue<int> q;
        q.push(dom_root);
        depth[dom_root] = 1;
        while (!q.empty())
        {
            auto cur = q.front();
            q.pop();
            for (auto child : dom_tree->dom_tree[cur])
            {
                if (depth.find(child) == depth.end())
                {
                    depth[child] = depth[cur] + 1;
                    q.push(child);
                }
            }
        }
    }
}  // namespace

namespace Backend::RV64::Passes
{

    StackLoweringPass::StackLoweringPass(std::vector<Function*>& functions) : functions_(functions) {}

    bool StackLoweringPass::run()
    {
        for (auto& func : functions_) { lowerStack(func); }
        return true;
    }

    void StackLoweringPass::gatherRegsToSave(Function* func, MAT2(int) & reg_def_blocks, MAT2(int) & reg_access_blocks)
    {
        reg_def_blocks.clear();
        reg_access_blocks.clear();
        reg_def_blocks.resize(64);
        reg_access_blocks.resize(64);

        bool need_save[64];

        for (auto& block : func->blocks)
        {
#define X(name, alias, saver)                   \
    if (saver == 1)                             \
        need_save[preg_##alias.reg_num] = true; \
    else                                        \
        need_save[preg_##alias.reg_num] = false;
            RV64_REGS
#undef X
            need_save[preg_ra.reg_num] = true;

            for (auto& inst : block->insts)
            {
                for (auto& reg : inst->getWriteRegs())
                {
                    assert(reg->is_virtual == false);
                    if (need_save[reg->reg_num])
                    {
                        need_save[reg->reg_num] = false;
                        reg_def_blocks[reg->reg_num].push_back(block->label_num);
                        reg_access_blocks[reg->reg_num].push_back(block->label_num);
                    }
                }
                for (auto& reg : inst->getReadRegs())
                {
                    assert(reg->is_virtual == false);
                    if (need_save[reg->reg_num])
                    {
                        need_save[reg->reg_num] = false;
                        reg_access_blocks[reg->reg_num].push_back(block->label_num);
                    }
                }
            }
        }

        if (func->has_stack_param)
        {
            reg_def_blocks[preg_fp.reg_num].push_back(0);
            reg_access_blocks[preg_fp.reg_num].push_back(0);
        }
    }

    int StackLoweringPass::calculatePairLCA(
        int x, int y, Cele::Algo::DomAnalyzer* dom_tree, std::map<int, int>& depth, Function* func)
    {
        while (depth[x] > depth[y]) { x = dom_tree->imm_dom[x]; }
        while (depth[y] > depth[x]) { y = dom_tree->imm_dom[y]; }
        while (x != y)
        {
            x = dom_tree->imm_dom[x];
            y = dom_tree->imm_dom[y];
        }

        if (func && func->cfg && func->cfg->blocks.find(x) != func->cfg->blocks.end())
        {
            auto* block = func->cfg->blocks[x];
            while (getBlockLoopDepth(block, func->cfg->loop_forest.get()) > 0)
            {
                if (dom_tree->imm_dom[x] == x) break;
                x = dom_tree->imm_dom[x];
                if (func->cfg->blocks.find(x) == func->cfg->blocks.end()) break;
                block = func->cfg->blocks[x];
            }
        }

        return x;
    }

    int StackLoweringPass::calculateGroupLCA(
        const std::vector<int>& blocks, Cele::Algo::DomAnalyzer* dom_tree, Function* func)
    {
        if (blocks.empty()) return -1;
        if (blocks.size() == 1) return blocks[0];

        std::vector<int>   dfs_order;
        std::map<int, int> visited;
        std::map<int, int> depth;

        int dom_root = 0;
        getDfsOrder(dom_tree, dom_root, dfs_order, visited);
        getDepth(dom_tree, dom_root, depth);

        int                first = -1, last = -1;
        std::map<int, int> block_has_reg;
        for (auto b : blocks) { block_has_reg[b] = 1; }

        for (auto it = dfs_order.begin(); it != dfs_order.end(); ++it)
        {
            if (block_has_reg[*it])
            {
                first = *it;
                break;
            }
        }
        for (auto it = dfs_order.rbegin(); it != dfs_order.rend(); ++it)
        {
            if (block_has_reg[*it])
            {
                last = *it;
                break;
            }
        }

        if (first == -1 || last == -1) return -1;

        return calculatePairLCA(first, last, dom_tree, depth, func);
    }

    void StackLoweringPass::lowerStack(Function* func)
    {
        if (func->cfg && func->cfg->dom_tree && func->cfg->post_dom_tree)
            lowerStackWithLCA(func);
        else
            lowerStackSimple(func);
    }

    void StackLoweringPass::lowerStackWithLCA(Function* func)
    {
        std::vector<int> sd_blocks(64, -1);
        std::vector<int> ld_blocks(64, -1);
        std::vector<int> restore_offset(64, 0);
        int              saveregnum = 0, cur_restore_offset = 0;

        MAT2(int) reg_def_blocks, reg_access_blocks;
        gatherRegsToSave(func, reg_def_blocks, reg_access_blocks);

        for (size_t i = 0; i < reg_access_blocks.size(); ++i)
        {
            if (!reg_access_blocks[i].empty()) { ++saveregnum; }
        }

        func->stack_size += saveregnum * 8;

        bool use_simple_strategy = (-8 + func->stack_size) >= 2048;

        if (use_simple_strategy)
        {
            func->stack_size -= saveregnum * 8;
            lowerStackSimple(func);
            return;
        }

        for (size_t i = 0; i < reg_access_blocks.size(); ++i)
        {
            if (reg_access_blocks[i].empty()) continue;

            cur_restore_offset -= 8;
            restore_offset[i] = cur_restore_offset;

            std::vector<int> exit_blocks;
            for (auto& block : func->blocks)
            {
                if (block->insts.empty()) continue;

                auto      last_inst = block->insts.back();
                RV64Inst* rv64_inst = (RV64Inst*)last_inst;
                if (rv64_inst->op == RV64InstType::JALR && rv64_inst->rd == preg_x0 && rv64_inst->rs1 == preg_ra)
                {
                    for (int access_block : reg_access_blocks[i])
                    {
                        if (func->cfg->post_dom_tree->isDomate(access_block, block->label_num))
                        {
                            exit_blocks.push_back(block->label_num);
                            break;
                        }
                    }
                }
            }

            if (exit_blocks.empty())
            {
                for (auto& block : func->blocks)
                {
                    if (block->insts.empty()) continue;
                    auto      last_inst = block->insts.back();
                    RV64Inst* rv64_inst = (RV64Inst*)last_inst;
                    if (rv64_inst->op == RV64InstType::JALR && rv64_inst->rd == preg_x0 && rv64_inst->rs1 == preg_ra)
                    {
                        exit_blocks.push_back(block->label_num);
                    }
                }
            }

            if (!exit_blocks.empty())
            {
                ld_blocks[i] = calculateGroupLCA(exit_blocks, func->cfg->post_dom_tree.get(), func);
            }
            else { ld_blocks[i] = 0; }

            std::vector<int> extended_def_blocks = reg_def_blocks[i];
            if (ld_blocks[i] != -1) { extended_def_blocks.push_back(ld_blocks[i]); }

            sd_blocks[i] = calculateGroupLCA(extended_def_blocks, func->cfg->dom_tree.get(), func);
        }

        for (size_t i = 0; i < reg_access_blocks.size(); ++i)
        {
            if (reg_access_blocks[i].empty()) continue;

            if (sd_blocks[i] == -1 || ld_blocks[i] == -1)
            {
                func->stack_size -= saveregnum * 8;
                lowerStackSimple(func);
                return;
            }

            if (func->cfg->blocks.find(sd_blocks[i]) == func->cfg->blocks.end() ||
                func->cfg->blocks.find(ld_blocks[i]) == func->cfg->blocks.end())
            {
                func->stack_size -= saveregnum * 8;
                lowerStackSimple(func);
                return;
            }
        }

        for (size_t i = 0; i < reg_access_blocks.size(); ++i)
        {
            if (reg_access_blocks[i].empty()) continue;

            int regno     = static_cast<int>(i);
            int sp_offset = restore_offset[i] + func->stack_size;

            if (func->cfg->blocks.find(sd_blocks[i]) != func->cfg->blocks.end())
            {
                Block* store_block = func->cfg->blocks[sd_blocks[i]];

                if (!(func->has_stack_param && static_cast<int>(i) == preg_fp.reg_num && sd_blocks[i] == 0))
                {
                    if (static_cast<int>(REG::x0) <= regno && regno <= static_cast<int>(REG::x31))
                    {
                        store_block->insts.push_front(
                            createSInst(RV64InstType::SD, getPhyReg(regno), preg_sp, sp_offset));
                    }
                    else
                    {
                        store_block->insts.push_front(
                            createSInst(RV64InstType::FSD, getPhyReg(regno), preg_sp, sp_offset));
                    }
                }
            }

            if (func->cfg->blocks.find(ld_blocks[i]) != func->cfg->blocks.end())
            {
                Block* load_block = func->cfg->blocks[ld_blocks[i]];

                auto it = load_block->insts.end();
                if (!load_block->insts.empty())
                {
                    --it;
                    if (static_cast<int>(REG::x0) <= regno && regno <= static_cast<int>(REG::x31))
                    {
                        load_block->insts.insert(
                            it, createIInst(RV64InstType::LD, getPhyReg(regno), preg_sp, sp_offset));
                    }
                    else
                    {
                        load_block->insts.insert(
                            it, createIInst(RV64InstType::FLD, getPhyReg(regno), preg_sp, sp_offset));
                    }
                }
            }
        }

        handleStackAllocation(func);
    }

    void StackLoweringPass::lowerStackSimple(Function* func)
    {
        std::vector<int> restore_offset(64, 0);
        int              saveregnum = 0, cur_restore_offset = 0;

        MAT2(int) reg_def_blocks, reg_access_blocks;
        gatherRegsToSave(func, reg_def_blocks, reg_access_blocks);

        for (size_t i = 0; i < reg_def_blocks.size(); ++i)
        {
            if (!reg_access_blocks[i].empty()) { ++saveregnum; }
        }

        func->stack_size += saveregnum * 8;

        for (auto& block : func->blocks)
        {
            if (block->label_num == 0)
            {
                if (func->stack_size <= 2048)
                    block->insts.push_front(createIInst(RV64InstType::ADDI, preg_sp, preg_sp, -func->stack_size));
                else
                {
                    block->insts.push_front(createRInst(RV64InstType::SUB, preg_sp, preg_sp, preg_t0));
                    block->insts.push_front(createUInst(RV64InstType::LI, preg_t0, func->stack_size));
                }

                if (func->has_stack_param)
                    block->insts.push_front(createRInst(RV64InstType::ADD, preg_fp, preg_sp, preg_x0));

                int offset = 0;
                for (int i = 0; i < 64; ++i)
                {
                    if (reg_access_blocks[i].empty()) continue;

                    offset -= 8;
                    if (static_cast<int>(REG::x0) <= i && i <= static_cast<int>(REG::x31))
                    {
                        block->insts.push_front(createSInst(RV64InstType::SD, getPhyReg(i), preg_sp, offset));
                    }
                    else { block->insts.push_front(createSInst(RV64InstType::FSD, getPhyReg(i), preg_sp, offset)); }
                }
            }

            if (block->insts.empty()) continue;
            Instruction* last_inst = block->insts.back();

            RV64Inst* rv64_inst = (RV64Inst*)last_inst;
            if (rv64_inst->op != RV64InstType::JALR) continue;
            if (!(rv64_inst->rd == preg_x0)) continue;
            if (!(rv64_inst->rs1 == preg_ra)) continue;

            block->insts.pop_back();
            if (func->stack_size <= 2047)
                block->insts.push_back(createIInst(RV64InstType::ADDI, preg_sp, preg_sp, func->stack_size));
            else
            {
                block->insts.push_back(createUInst(RV64InstType::LI, preg_t0, func->stack_size));
                block->insts.push_back(createRInst(RV64InstType::ADD, preg_sp, preg_sp, preg_t0));
            }

            int offset = 0;
            for (int i = 0; i < 32; ++i)
            {
                if (reg_access_blocks[i].empty()) continue;
                offset -= 8;
                block->insts.push_back(createIInst(RV64InstType::LD, getPhyReg(i), preg_sp, offset));
            }
            for (int i = 32; i < 64; ++i)
            {
                if (reg_access_blocks[i].empty()) continue;
                offset -= 8;
                block->insts.push_back(createIInst(RV64InstType::FLD, getPhyReg(i), preg_sp, offset));
            }

            block->insts.push_back(rv64_inst);
        }
    }

    void StackLoweringPass::handleStackAllocation(Function* func)
    {
        for (auto& block : func->blocks)
        {
            if (block->label_num == 0)
            {
                if (func->stack_size <= 2048)
                {
                    block->insts.push_front(createIInst(RV64InstType::ADDI, preg_sp, preg_sp, -func->stack_size));
                }
                else
                {
                    block->insts.push_front(createRInst(RV64InstType::SUB, preg_sp, preg_sp, preg_t0));
                    block->insts.push_front(createUInst(RV64InstType::LI, preg_t0, func->stack_size));
                }

                if (func->has_stack_param)
                {
                    block->insts.push_front(createRInst(RV64InstType::ADD, preg_fp, preg_sp, preg_x0));
                }
            }

            if (block->insts.empty()) continue;
            Instruction* last_inst = block->insts.back();

            RV64Inst* rv64_inst = (RV64Inst*)last_inst;
            if (rv64_inst->op == RV64InstType::JALR && rv64_inst->rd == preg_x0 && rv64_inst->rs1 == preg_ra)
            {
                block->insts.pop_back();

                if (func->stack_size <= 2047)
                {
                    block->insts.push_back(createIInst(RV64InstType::ADDI, preg_sp, preg_sp, func->stack_size));
                }
                else
                {
                    block->insts.push_back(createUInst(RV64InstType::LI, preg_t0, func->stack_size));
                    block->insts.push_back(createRInst(RV64InstType::ADD, preg_sp, preg_sp, preg_t0));
                }

                block->insts.push_back(rv64_inst);
            }
        }
    }

}  // namespace Backend::RV64::Passes
