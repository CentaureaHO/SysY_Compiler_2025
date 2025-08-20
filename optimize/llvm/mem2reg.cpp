#include "llvm/mem2reg.h"
#include "llvm_ir/instruction.h"
#include <iostream>
#include <queue>
// #define DEBUG_PRINT

bool Mem2Reg::CanMem2Reg(LLVMIR::Instruction* alloca_inst)
{
    // 数组不能被mem2reg，别的忘了，可能有问题，标记
    if (alloca_inst->opcode != LLVMIR::IROpCode::ALLOCA) { return 0; }
    LLVMIR::AllocInst* inst = (LLVMIR::AllocInst*)alloca_inst;
    if (inst->dims.size()) { return 0; }
    return 1;
}
void Mem2Reg::Mem2RegUseDefInSameBlock(CFG* C, std::set<int>& vset, int block_id)
{
    std::map<int, int> reg_now;
    for (auto iter : (C->block_id_to_block)[block_id]->insts)
    {
        if (iter->opcode == LLVMIR::IROpCode::ALLOCA)
        {
            LLVMIR::AllocInst* ins = (LLVMIR::AllocInst*)iter;
            if (ins->res->type == LLVMIR::OperandType::REG)
            {
                LLVMIR::RegOperand* op      = (LLVMIR::RegOperand*)(ins->res);
                int                 reg_num = op->reg_num;
                if (vset.find(reg_num) != vset.end()) { todel[ins] = 1; }
            }
        }
        else if (iter->opcode == LLVMIR::IROpCode::STORE)
        {
            LLVMIR::StoreInst* ins = (LLVMIR::StoreInst*)iter;
            if (ins->ptr->type == LLVMIR::OperandType::REG)
            {
                LLVMIR::RegOperand* op      = (LLVMIR::RegOperand*)(ins->ptr);
                LLVMIR::RegOperand* val     = (LLVMIR::RegOperand*)(ins->val);
                int                 reg_num = op->reg_num;
                int                 val_reg = val->reg_num;
                if (vset.find(reg_num) != vset.end())
                {
                    reg_now[reg_num] = reg_now[val_reg] != 0 ? reg_now[val_reg] : val_reg;
                    // std::cout << "reg now " << reg_num << " is set to" << val_reg << std::endl;
                    todel[ins] = 1;
                }
            }
        }
        else if (iter->opcode == LLVMIR::IROpCode::LOAD)
        {
            LLVMIR::LoadInst* ins = (LLVMIR::LoadInst*)iter;
            if (ins->ptr->type == LLVMIR::OperandType::REG)
            {
                LLVMIR::RegOperand* op      = (LLVMIR::RegOperand*)(ins->ptr);
                LLVMIR::RegOperand* res_reg = (LLVMIR::RegOperand*)(ins->res);
                int                 reg_num = op->reg_num;
                int                 res_num = res_reg->reg_num;
                if (vset.find(reg_num) != vset.end())
                {
                    replaces[res_num] = reg_now[reg_num];
                    // std::cout << "reg now " << reg_num << " is " << reg_now[reg_num] << std::endl;
                    reg_now[res_num] = reg_now[reg_num];
                    todel[ins]       = 1;
                }
            }
        }
    }
}
void Mem2Reg::InsertPhi(CFG* cfg)
{
#ifdef DEBUG_PRINT
    std::cout << "call insertphi" << std::endl;
#endif
    // 初始化
    alloca_regs.clear();
    replaces.clear();
    new_phis.clear();
    todel.clear();
    std::map<int, LLVMIR::AllocInst*> allocaid2inst;  // alloca寄存器号到其对应指令
    // 找出可优化的alloca，仅在入口块alloca
    for (unsigned int i = 0; i < (cfg->block_id_to_block)[0]->insts.size(); i++)
    {
        auto inst = (cfg->block_id_to_block)[0]->insts[i];
        if (CanMem2Reg(inst))
        {
            LLVMIR::AllocInst* alloca_inst = (LLVMIR::AllocInst*)inst;
            // alloca_regs.insert(alloca_inst->GetResultReg());
            alloca_regs.insert(((LLVMIR::RegOperand*)(alloca_inst->res))->reg_num);
            todel[inst]                                                       = 1;            // 标记删除
            allocaid2inst[((LLVMIR::RegOperand*)(alloca_inst->res))->reg_num] = alloca_inst;  // 后面插入phi用到
        }
    }
#ifdef DEBUG_PRINT
    std::cout << "alloca reg to deal with:" << std::endl;
    for (auto iter : alloca_regs) { std::cout << iter << ' '; }
    std::cout << std::endl;
#endif
    // 遍历每个alloca寄存器的store use情况
    std::map<int, std::set<int>> def, use;  // reg号到def和use的块
    std::map<int, int>           def_cnt{};
    for (auto iter : alloca_regs) def_cnt[iter] = 0;

    for (auto iter1 : cfg->block_id_to_block)
    {  // 对每个块
        for (auto iter2 : iter1.second->insts)
        {  // 对每条指令
            if (iter2->opcode == LLVMIR::IROpCode::LOAD)
            {  // use
                LLVMIR::LoadInst* ins = (LLVMIR::LoadInst*)iter2;
                if (ins->ptr->type == LLVMIR::OperandType::REG)
                {
                    use[((LLVMIR::RegOperand*)(ins->ptr))->reg_num].insert(iter1.first);
                }
            }
            else if (iter2->opcode == LLVMIR::IROpCode::STORE)
            {
                LLVMIR::StoreInst* ins = (LLVMIR::StoreInst*)iter2;
                if (ins->ptr->type == LLVMIR::OperandType::REG)
                {
                    def[((LLVMIR::RegOperand*)(ins->ptr))->reg_num].insert(iter1.first);
                    def_cnt[((LLVMIR::RegOperand*)(ins->ptr))->reg_num] += 1;
                }
            }
        }
    }
    // 按情况分别处理
    // 关于那个一个def的，会表现为phi里只有一个来源,我们遵照一般处理，再把只有一个来源的phi删掉
    std::set<int>                no_use_set, normal_set;
    std::map<int, std::set<int>> block2vset2;
    for (auto iter : alloca_regs)
    {
        if (use[iter].size() == 0) { no_use_set.insert(iter); }
        else if (def[iter].size() == 1 && use[iter].size() == 1 && *use[iter].begin() == *def[iter].begin())
        {
            block2vset2[*use[iter].begin()].insert(iter);
        }
        else { normal_set.insert(iter); }
    }
#ifdef DEBUG_PRINT
    std::cout << "no use set:" << std::endl;
    for (auto iter : no_use_set) std::cout << iter << ' ';
    std::cout << std::endl;
    std::cout << "normal set:" << std::endl;
    for (auto iter : normal_set) std::cout << iter << ' ';
    std::cout << std::endl;
#endif
    // 删除只def不use的alloca的alloca和def语句
    for (auto iter1 : cfg->block_id_to_block)
    {
        for (auto iter2 : iter1.second->insts)
        {
            if (iter2->opcode == LLVMIR::IROpCode::ALLOCA)
            {
                LLVMIR::AllocInst* ins = (LLVMIR::AllocInst*)iter2;
                if (ins->res->type == LLVMIR::OperandType::REG)
                {
                    LLVMIR::RegOperand* op = (LLVMIR::RegOperand*)ins->res;
                    if (no_use_set.find(op->reg_num) != no_use_set.end()) { todel[iter2] = 1; }
                }
            }
            else if (iter2->opcode == LLVMIR::IROpCode::STORE)
            {
                LLVMIR::StoreInst* ins = (LLVMIR::StoreInst*)iter2;
                if (ins->ptr->type == LLVMIR::OperandType::REG)
                {
                    LLVMIR::RegOperand* op = (LLVMIR::RegOperand*)ins->ptr;
                    if (no_use_set.find(op->reg_num) != no_use_set.end()) { todel[iter2] = 1; }
                }
            }
        }
    }
    // def与use在同一块
    for (auto iter : block2vset2) { Mem2RegUseDefInSameBlock(cfg, iter.second, iter.first); }
    // 一般情况的处理
    auto dom_tree = this->ir->DomTrees[cfg];

    for (auto iter : normal_set)
    {  // 消除的alloca的对应ptr寄存器号
        std::vector<int> visited;
        visited.resize(cfg->block_id_to_block.size() + 1);
        std::set<int> worklist1 = def[iter];

        while (!worklist1.empty())
        {
            int block_id = *worklist1.begin();
            worklist1.erase(block_id);  // 性能问题？ 标记

            for (auto iter2 : dom_tree->dom_frontier[block_id])
            {  // 支配边界块号
                if (visited[iter2]) { continue; }
                visited[iter2] = 1;
                // 插入phi
                LLVMIR::PhiInst* phi =
                    new LLVMIR::PhiInst(allocaid2inst[iter]->type, getRegOperand(++cfg->func->max_reg));
                cfg->block_id_to_block[iter2]->insts.push_front(phi);
#ifdef DEBUG_PRINT
                std::cout << "insert phi to block " << iter2 << " for reg " << iter << "inst type is " << phi->opcode
                          << std::endl;
                std::cout << "insertphi of reg " << iter << " to block " << iter2 << std::endl;
#endif
                new_phis[phi] = iter;
                worklist1.insert(iter2);
            }
        }
    }
}

void Mem2Reg::Rename(CFG* cfg)
{
#ifdef DEBUG_PRINT
    std::cout << "call rename" << std::endl;
#endif
    std::vector<int>                  visited;
    std::map<int, std::map<int, int>> incoming_values;

    std::map<LLVMIR::PhiInst*, std::set<int>> phi_predecessors_added;
    visited.resize(cfg->G.size() + 1);
    std::queue<int> worklist2;
    worklist2.push(0);

    while (!worklist2.empty())
    {
        int block_id = worklist2.front();
        worklist2.pop();
        if (visited[block_id]) { continue; }
        visited[block_id] = 1;

        for (auto inst : (cfg->block_id_to_block)[block_id]->insts)
        {  // 对块中每条指令
            if (inst->opcode == LLVMIR::IROpCode::ALLOCA)
            {
                int ptr_reg = ((LLVMIR::RegOperand*)((LLVMIR::AllocInst*)inst)->res)->reg_num;
                if (alloca_regs.find(ptr_reg) != alloca_regs.end()) todel[inst] = 1;
            }
            else if (inst->opcode == LLVMIR::IROpCode::LOAD)
            {
                LLVMIR::LoadInst* load_ins = (LLVMIR::LoadInst*)inst;
                if (load_ins->ptr->type == LLVMIR::OperandType::REG)
                {
                    // 建立对应关系 加载到的reg直接用ptr reg当前的对应值代替
                    int ptr_reg     = ((LLVMIR::RegOperand*)(load_ins->ptr))->reg_num;
                    int load_to_reg = ((LLVMIR::RegOperand*)(load_ins->res))->reg_num;
                    if (alloca_regs.find(ptr_reg) != alloca_regs.end())
                    {
                        todel[inst]                            = 1;
                        replaces[load_to_reg]                  = incoming_values[block_id][ptr_reg];
                        incoming_values[block_id][load_to_reg] = incoming_values[block_id][ptr_reg];
                    }
                }
            }
            else if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                LLVMIR::StoreInst* store_ins = (LLVMIR::StoreInst*)inst;
                if (store_ins->ptr->type == LLVMIR::OperandType::REG)
                {
                    int ptr_reg        = ((LLVMIR::RegOperand*)(store_ins->ptr))->reg_num;
                    int store_from_reg = ((LLVMIR::RegOperand*)(store_ins->val))->reg_num;
                    if (alloca_regs.find(ptr_reg) != alloca_regs.end())
                    {
                        todel[inst] = 1;
                        incoming_values[block_id][ptr_reg] =
                            incoming_values[block_id].find(store_from_reg) != incoming_values[block_id].end()
                                ? incoming_values[block_id][store_from_reg]
                                : store_from_reg;
                    }
                }
            }
            else if (inst->opcode == LLVMIR::IROpCode::PHI && !todel[inst])
            {
                LLVMIR::PhiInst* phi_ins = (LLVMIR::PhiInst*)inst;
                if (new_phis.find(phi_ins) != new_phis.end())
                {
                    incoming_values[block_id][new_phis[phi_ins]] = phi_ins->GetResultReg();
                }
            }
        }

        // 更新后继
        for (auto succ : cfg->G[block_id])
        {
            for (auto iter : incoming_values[block_id]) incoming_values[succ->block_id][iter.first] = iter.second;
            worklist2.push(succ->block_id);
            // 对子结点的phi，添加一个当前路径的值
            for (auto phi : succ->insts)
            {
                if (phi->opcode != LLVMIR::IROpCode::PHI) { break; }
                LLVMIR::PhiInst* phi_ins = (LLVMIR::PhiInst*)phi;
                if (new_phis.find(phi_ins) == new_phis.end()) { continue; }

                if (phi_predecessors_added[phi_ins].find(block_id) != phi_predecessors_added[phi_ins].end())
                {
                    continue;
                }

                int alloca_reg = new_phis[phi_ins];
                if (incoming_values[block_id].find(alloca_reg) != incoming_values[block_id].end())
                {
                    LLVMIR::RegOperand*   regop   = getRegOperand(incoming_values[block_id][alloca_reg]);
                    LLVMIR::LabelOperand* labelop = getLabelOperand(block_id);
                    phi_ins->Insert_into_PHI(regop, labelop);
                    phi_predecessors_added[phi_ins].insert(block_id);
                }
                else { todel[phi_ins] = 1; }
            }
        }
    }
#ifdef DEBUG_PRINT
    std::cout << "replaces: " << std::endl;
    for (auto iter : replaces) { std::cout << iter.first << ' ' << iter.second << std::endl; }
#endif
    // 删除标记删除的指令与变量重命名
    for (auto iter : cfg->block_id_to_block)
    {
        auto temp = iter.second->insts;
        iter.second->insts.clear();
        for (auto ins : temp)
        {
            if (!todel[ins])
            {
                ins->Rename(replaces);
                iter.second->insts.push_back(ins);
            }
        }
    }
}

void Mem2Reg::Mem2Reg_func(CFG* cfg)
{
    InsertPhi(cfg);
    Rename(cfg);
}

void Mem2Reg::Execute()
{
    for (auto iter : ir->cfg)
    {
#ifdef DEBUG_PRINT
        std::cout << "func now " << iter.second->func->func_def->func_name << "---------------" << std::endl;
#endif
        Mem2Reg_func(iter.second);
    }
}