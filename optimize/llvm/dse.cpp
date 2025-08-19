#include "dse.h"
#include "cfg.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/ir_builder.h"
#include <deque>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <iostream>  // 添加头文件用于输出调试信息
#include <fstream>   // 添加文件输出支持
#include <utility>

// #define DEBUG_DSE
// #define LOG_DSE_REMOVAL  // 控制删除记录输出的开关

namespace LLVMIR
{
    void DSEPass::Execute()
    {
#ifdef DEBUG_DSE
        std::cout << "===== DSE Pass Start =====" << std::endl;
        int total_eliminated = 0;
#endif
#ifdef LOG_DSE_REMOVAL
        std::ofstream log_file("dse_removal_log.txt", std::ios::app);
        log_file << "===== DSE Pass Execution (" << __DATE__ << " " << __TIME__ << ") =====" << std::endl;
        int removal_count = 0;
#endif
        ealias_analyser->run();
        edef_use_analysis->run();
        arralias_analysis->run();
        const int MAX_ITERATIONS = 5;
        int       Iteration      = 0;
        bool      changed        = true;

        while (changed && Iteration < MAX_ITERATIONS)
        {
            changed = false;
#ifdef DEBUG_DSE
            std::cout << "Iteration " << Iteration + 1 << " of DSE Pass" << std::endl;
#endif
            Iteration++;
            for (auto [func, cfg] : ir->cfg)
            {
#ifdef DEBUG_DSE
                std::cout << "Processing function: " << func->func_name << std::endl;
#endif
#ifdef LOG_DSE_REMOVAL
                log_file << "Processing function: " << func->func_name << std::endl;
#endif
                params.clear();
                // 收集函数参数
                for (auto arg : func->arg_regs)
                {
                    params.insert(arg);
#ifdef DEBUG_DSE
                    std::cout << "  Adding parameter: " << arg << std::endl;
#endif
                }

                // 处理当前函数的CFG
                ExecuteInSingleCFG(cfg, changed);

#ifdef DEBUG_DSE
                // 统计该函数消除的存储指令
                int func_eliminated = 0;
                for (auto& [id, stores] : erase_set) { func_eliminated += stores.size(); }
                total_eliminated += func_eliminated;

                std::cout << "  Eliminated " << func_eliminated << " dead stores in function " << func->func_name
                          << std::endl;
#endif
#ifdef LOG_DSE_REMOVAL
                int func_removed = 0;
                for (auto& [id, stores] : erase_set) { func_removed += stores.size(); }
                removal_count += func_removed;

                if (func_removed > 0)
                {
                    log_file << "  [REMOVED] " << func_removed << " dead stores in function " << func->func_name
                             << std::endl;

                    // 可选：记录每个被删除的store指令详情
                    for (auto& [block_id, stores] : erase_set)
                    {
                        for (auto* store : stores)
                        {
                            auto* storeInst = dynamic_cast<StoreInst*>(store);
                            log_file << "    - Block " << block_id << ": " << storeInst->ptr << " = " << storeInst->val
                                     << std::endl;
                        }
                    }
                }
#endif
            }

#ifdef DEBUG_DSE
            std::cout << "DSE Pass complete: eliminated " << total_eliminated << " dead stores in total" << std::endl;
            std::cout << "===== DSE Pass End =====" << std::endl;
#endif
#ifdef LOG_DSE_REMOVAL
            log_file << "DSE Pass summary: removed " << removal_count << " dead stores in total" << std::endl;
            if (removal_count == 0) { log_file << "WARNING: No stores were removed in this pass!" << std::endl; }
            log_file << "===== DSE Pass End =====" << std::endl << std::endl;
            log_file.close();
#endif
        }
    }

    void DSEPass::ExecuteInSingleCFG(CFG* cfg, bool& changed)
    {
#ifdef DEBUG_DSE
        std::cout << "  ExecuteInSingleCFG: Starting analysis" << std::endl;
#endif

        // 重置状态
        store_insts.clear();
        erase_set.clear();

        // 收集所有存储指令
        CollectStores(cfg);
#ifdef DEBUG_DSE
        std::cout << "  Collected " << store_insts.size() << " store instructions" << std::endl;
#endif

        // 分析找出死代码
        GenerateElimination(cfg);
        if (!erase_set.empty()) { changed = true; }

#ifdef DEBUG_DSE
        // 计算找到的死代码数量
        int elimination_count = 0;
        for (auto& [id, stores] : erase_set) { elimination_count += stores.size(); }
        std::cout << "  Found " << elimination_count << " dead store instructions" << std::endl;
#endif

        // 删除死代码
        EraseStoreInst(cfg);
#ifdef DEBUG_DSE
        std::cout << "  Removed all dead store instructions" << std::endl;
#endif
        // store_insts.clear();
        // erase_set.clear();  // 清空erase_set以便下次使用
        // CollectStores(cfg);
        // // 对于store但是全局都没有对于mayAlias的读取，那么我们可以将其删除
        // NoUseStore(cfg);
        // if (!erase_set.empty()) { changed = true; }
        // EraseStoreInst(cfg);
    }

    void DSEPass::CollectStores(CFG* cfg)
    {
#ifdef DEBUG_DSE
        std::cout << "    CollectStores: Finding all store instructions" << std::endl;
        int count = 0;
#endif

        for (auto [id, block] : cfg->block_id_to_block)
        {
            for (auto inst : block->insts)
            {
                if (inst->opcode == IROpCode::STORE)
                {
                    store_insts.insert(inst);
#ifdef DEBUG_DSE
                    count++;

                    if (count % 20 == 0)
                    {  // 避免输出过多
                        std::cout << "      Collected " << count << " stores so far" << std::endl;
                    }
#endif
                }
            }
        }
#ifdef DEBUG_DSE
        std::cout << "    CollectStores: Found " << count << " store instructions in total" << std::endl;
#endif
    }

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

    static std::unordered_set<int> getDominatedBlocks(IR* ir, CFG* cfg, int block_id)
    {
        std::unordered_set<int> ret;
        auto                    domTree = ir->DomTrees[cfg];

        std::function<void(int)> collectDominated = [&](int id) {
            for (int childId : domTree->dom_tree[id])
            {
                ret.insert(childId);
                collectDominated(childId);
            }
        };

        collectDominated(block_id);
#ifdef DEBUG_DSE
        std::cout << "    Block " << block_id << " dominates " << ret.size() << " blocks" << std::endl;
#endif
        return ret;
    }

    // 检查所有从start到end的路径是否都经过through
    bool DSEPass::allPathsGoThrough(CFG* cfg, int start, int end, int through)
    {
#ifdef DEBUG_DSE
        std::cout << "    Checking paths from " << start << " to " << end << " through " << through << std::endl;
#endif

        // 如果through就是start或end，条件满足
        if (through == start || through == end)
        {
#ifdef DEBUG_DSE
            std::cout << "      Quick check: through block is start/end - all paths go through" << std::endl;
#endif
            return true;
        }

        // 使用BFS寻找从start到end不经过through的路径
        std::queue<int> q;
        std::set<int>   visited;

        q.push(start);
        visited.insert(start);
        visited.insert(through);  // 标记through为已访问，使搜索避开它

        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            auto* block = cfg->block_id_to_block[current];

            if (current == end)
            {
                // 找到一条不经过through的路径
#ifdef DEBUG_DSE
                std::cout << "      Found path from " << start << " to " << end << " NOT going through " << through
                          << std::endl;
#endif
                return false;
            }

            for (int succ : cfg->G_id[block->block_id])
            {
                if (visited.count(succ) == 0)
                {
                    visited.insert(succ);
                    q.push(succ);
                }
            }
        }

        // 没有找到不经过through的路径
#ifdef DEBUG_DSE
        std::cout << "      All paths from " << start << " to " << end << " go through " << through << std::endl;
#endif
        return true;
    }

    bool DSEPass::allPathsNoLoad(CFG* cfg, int start, int through, Operand* ptr, Instruction* newstore)
    {
#ifdef DEBUG_DSE
        std::cout << "    Checking all paths from " << start << " to " << through << " for loads" << std::endl;
#endif

        // 1. 获取从start可达through的块
        std::set<int> startToThrough;
        {
            std::queue<int> q;
            std::set<int>   visited;

            q.push(start);
            visited.insert(start);

            while (!q.empty())
            {
                int current = q.front();
                q.pop();

                startToThrough.insert(current);

                if (current != through)
                {  // 不要越过through块
                    auto* block = cfg->block_id_to_block[current];
                    for (int succ : cfg->G_id[block->block_id])
                    {
                        if (visited.count(succ) == 0)
                        {
                            visited.insert(succ);
                            q.push(succ);
                        }
                    }
                }
            }
        }

        // 2. 检查startToThrough中的块是否有load
        for (int blockId : startToThrough)
        {
            if (blockId == start) continue;

            auto* block = cfg->block_id_to_block[blockId];
            for (auto* inst : block->insts)
            {
                if (inst == newstore) break;  // 如果是新store，跳过它
                if (inst->opcode == IROpCode::LOAD)
                {
                    auto* loadInst = dynamic_cast<LoadInst*>(inst);
                    if (mayAlias(loadInst->ptr, ptr, cfg))
                    {
#ifdef DEBUG_DSE
                        std::cout << "      Found load in block " << blockId << " on path from start to through"
                                  << std::endl;
#endif
                        return false;
                    }
                }
                else if (inst->opcode == IROpCode::GETELEMENTPTR)
                {
                    auto* gepInst = dynamic_cast<GEPInst*>(inst);
                    if (mayAlias(gepInst->base_ptr, ptr, cfg))
                    {
#ifdef DEBUG_DSE
                        std::cout << "      Found GEP in block " << blockId << " on path from start to through"
                                  << std::endl;
#endif
                        return false;
                    }
                }
            }
        }

        // 没有找到load，所有路径都满足条件
#ifdef DEBUG_DSE
        std::cout << "      No loads found on any path, condition satisfied" << std::endl;
#endif
        return true;
    }

    // 检查指针是否可能指向全局或逃逸的内存
    bool DSEPass::pointsToGlobalOrEscapes(Operand* ptr, CFG* cfg)
    {
        // 如果是全局变量
        if (ptr->type == OperandType::GLOBAL)
        {
#ifdef DEBUG_DSE
            std::cout << "    Pointer " << ptr << " is global" << std::endl;
#endif
            return true;
        }

        // 使用别名分析中的逃逸分析结果
        if (ptr->type == OperandType::REG)
        {
            // 检查是否为参数
            if (params.count(ptr))
            {
#ifdef DEBUG_DSE
                std::cout << "    Pointer " << ptr << " is a parameter" << std::endl;
#endif
                return true;
            }

            // 检查是否为逃逸指针
            if (!ealias_analyser->isLocalPtr(cfg, ptr))
            {
#ifdef DEBUG_DSE
                std::cout << "    Pointer " << ptr << " escapes function" << std::endl;
#endif
                return true;
            }
        }

#ifdef DEBUG_DSE
        std::cout << "    Pointer " << ptr << " is local and doesn't escape" << std::endl;
#endif
        return false;
    }

    bool DSEPass::isDeadStore(CFG* cfg, Instruction* store)
    {
        auto*    storeInst    = dynamic_cast<StoreInst*>(store);
        int      storeBlockId = store->block_id;
        IRBlock* storeBlock   = cfg->block_id_to_block[storeBlockId];
        auto*    ptr          = storeInst->ptr;

#ifdef DEBUG_DSE
        std::cout << "    Analyzing store at block " << storeBlockId << ": " << storeInst->ptr << " = "
                  << storeInst->val << std::endl;
        std::cout << "    Step 1: Checking for global or escaping pointers" << std::endl;

#endif

        // 1. 检查全局逃逸情况
        // 如果store的指针可能指向全局变量或函数参数，保守地认为不是死代码
        if (pointsToGlobalOrEscapes(ptr, cfg))
        {
#ifdef DEBUG_DSE
            std::cout << "      Pointer escapes or is global, store is live" << std::endl;
#endif
            return false;
        }

#ifdef DEBUG_DSE
        std::cout << "    Step 2: Checking same basic block for redundant stores" << std::endl;
#endif
        // if (isInLoop(storeBlock, cfg))
        // {
        //     std::cout << "      Store is in a loop, conservatively assuming it's not dead" << std::endl;
        //     return false;  // 如果store在循环中，保守地认为不是死代码
        // }
        // 2. 检查同一基本块内的后续指令
        bool foundInSameBlock = false;
        auto it               = storeBlock->insts.begin();
        while (it != storeBlock->insts.end() && *it != store) { ++it; }
        if (it != storeBlock->insts.end())
        {
            ++it;  // 移动到下一个指令
            for (; it != storeBlock->insts.end(); ++it)
            {
                auto* inst = *it;

                // 如果遇到函数调用，如果是independent，那么我们继续处理
                if (inst->opcode == IROpCode::CALL)
                {
                    auto call_inst = dynamic_cast<CallInst*>(inst);
                    auto func_cfg  = get_cfg_by_name(ir, call_inst->func_name);
                    if (func_cfg)
                    {
                        if (!ealias_analyser->isNoSideEffect(func_cfg))
                        {
#ifdef DEBUG_DSE
                            std::cout << "      Found call with side effects, store is live" << std::endl;
#endif
                            return false;
                        }
#ifdef DEBUG_DSE
                        std::cout << "      Found call to " << call_inst->func_name
                                  << " - no side effects, continuing analysis" << std::endl;
#endif
                        // 还需要检查这里的call的函数参数是否与ptr别名是否能够正确处理
                        for (auto [dt, arg] : call_inst->args)
                        {
                            if (dt == DataType::PTR)
                            {
                                if (mayAlias(arg, storeInst->ptr, cfg))
                                {
#ifdef DEBUG_DSE
                                    std::cout << "      Found aliasing argument " << arg << " with store "
                                              << storeInst->ptr << std::endl;
#endif
                                    return false;
                                }
                            }
                        }
                    }
                }

                // 如果遇到对相同位置的load，不是死代码
                if (inst->opcode == IROpCode::LOAD)
                {
                    auto* loadInst = dynamic_cast<LoadInst*>(inst);
                    if (mayAlias(loadInst->ptr, storeInst->ptr, cfg))
                    {
#ifdef DEBUG_DSE
                        std::cout << "      Found load from same location, store is live" << std::endl;
#endif
                        return false;
                    }
                }

                // 如果遇到对相同位置的store，当前store是死代码
                if (inst->opcode == IROpCode::STORE)
                {
                    auto* nextStore = dynamic_cast<StoreInst*>(inst);

                    if (mustAlias(nextStore->ptr, storeInst->ptr, cfg))
                    {
#ifdef DEBUG_DSE
                        std::cout << "      Found later store to same location in same block, store is dead, current "
                                     "store is "
                                  << storeInst->ptr << " and next is " << nextStore->ptr << std::endl;
#endif
                        foundInSameBlock = true;
                        break;
                    }
                }
            }
        }

        // 如果在同一块内找到了覆盖store，不需要继续检查
        if (foundInSameBlock) return true;

#ifdef DEBUG_DSE
        std::cout << "    Step 3: Checking across basic blocks using dominator tree" << std::endl;
#endif

        // 3. 检查跨基本块的情况 - 这需要支配树和内存依赖分析

        // 获取从store所在块支配的所有块
        std::unordered_set<int> dominatedBlocks = getDominatedBlocks(ir, cfg, storeBlockId);
        // 检查所有被支配的块中是否有load或store操作
        for (int blockId : dominatedBlocks)
        {
            auto* block = cfg->block_id_to_block[blockId];
#ifdef DEBUG_DSE
            std::cout << "      Checking dominated block " << blockId << std::endl;
#endif

            for (auto* inst : block->insts)
            {
                // 如果遇到函数调用，保守地认为store不是死代码
                if (inst->opcode == IROpCode::CALL)
                {
                    auto call_inst = dynamic_cast<CallInst*>(inst);
                    auto func_cfg  = get_cfg_by_name(ir, call_inst->func_name);
                    if (func_cfg)
                    {
                        if (!ealias_analyser->isNoSideEffect(func_cfg))
                        {
#ifdef DEBUG_DSE
                            std::cout << "        Found call with side effects, store is live" << std::endl;
#endif
                            return false;
                        }
#ifdef DEBUG_DSE
                        std::cout << "        Found call to " << call_inst->func_name << " - no side effects"
                                  << std::endl;
#endif
                    }
                }

                // 如果遇到对相同位置的store，进一步检查是否所有路径都经过此store
                if (inst->opcode == IROpCode::STORE)
                {
                    auto* nextStore = dynamic_cast<StoreInst*>(inst);
                    if (mustAlias(nextStore->ptr, storeInst->ptr, cfg))
                    {
#ifdef DEBUG_DSE
                        std::cout << "        Found store to same location in block " << blockId << std::endl;
#endif
                        // 需要检查是否所有从当前store到此store的路径都没有load
                        if (allPathsGoThrough(cfg, storeBlockId, blockId, nextStore->block_id))
                        {
                            // 如果所有路径都经过nextStore，则当前store是死的
#ifdef DEBUG_DSE
                            std::cout << "        All paths go through overwriting store, current store is dead"
                                      << std::endl;
#endif
                            if (allPathsNoLoad(cfg, storeBlockId, nextStore->block_id, storeInst->ptr, inst))
                            {
                                return true;
                            }
                        }
#ifdef DEBUG_DSE
                        else
                        {
                            std::cout << "        Not all paths go through overwriting store, continuing analysis"
                                      << std::endl;
                        }
#endif
                    }
                }
            }
        }

#ifdef DEBUG_DSE
        std::cout << "    No conclusive evidence that store is dead, conservatively keeping it" << std::endl;
#endif
        return false;  // 默认保守处理
    }

    void DSEPass::GenerateElimination(CFG* cfg)
    {
#ifdef DEBUG_DSE
        std::cout << "    GenerateElimination: Analyzing all stores" << std::endl;
        int analyzed = 0, eliminated = 0;
#endif

        for (auto store : store_insts)
        {
#ifdef DEBUG_DSE
            analyzed++;
            if (analyzed % 20 == 0)
            {
                std::cout << "      Analyzed " << analyzed << " of " << store_insts.size() << " stores so far, found "
                          << eliminated << " dead stores" << std::endl;
            }
#endif

            if (isDeadStore(cfg, store))
            {
                erase_set[store->block_id].insert(store);
#ifdef DEBUG_DSE
                eliminated++;
                std::cout << "      Store at block " << store->block_id << " is dead" << std::endl;
#endif
            }
        }

#ifdef DEBUG_DSE
        std::cout << "    GenerateElimination: Completed analysis of " << analyzed << " stores, found " << eliminated
                  << " dead stores" << std::endl;
#endif
    }

    void DSEPass::EraseStoreInst(CFG* cfg)
    {
#ifdef DEBUG_DSE
        std::cout << "    EraseStoreInst: Removing dead stores" << std::endl;
        int total_removed = 0;
#endif
#ifdef LOG_DSE_REMOVAL
        static std::ofstream removal_log("dse_removal_details.txt", std::ios::app);
#endif

        for (auto [id, S] : erase_set)
        {
            auto                     block = cfg->block_id_to_block[id];
            std::deque<Instruction*> new_insts;
#ifdef DEBUG_DSE
            int removed_in_block = 0;
#endif

            for (auto inst : block->insts)
            {
                if (S.count(inst))
                {
#ifdef DEBUG_DSE
                    removed_in_block++;
#endif
#ifdef LOG_DSE_REMOVAL
                    auto* storeInst = dynamic_cast<StoreInst*>(inst);
                    removal_log << "Removed store in block " << id << ": " << storeInst->ptr << " = " << storeInst->val
                                << std::endl;
#endif
                    continue;
                }
                new_insts.push_back(inst);
            }

#ifdef DEBUG_DSE
            std::cout << "      Removed " << removed_in_block << " dead stores from block " << id << std::endl;
            total_removed += removed_in_block;
#endif
            block->insts = std::move(new_insts);
        }

#ifdef DEBUG_DSE
        std::cout << "    EraseStoreInst: Removed " << total_removed << " dead stores in total" << std::endl;
#endif
    }

    // 计算GEP指令链的总偏移量
    int DSEPass::calculateTotalOffset(LLVMIR::Operand* ptr, CFG* cfg, LLVMIR::Operand* alloca)
    {
        int total_offset = 0;

        // 防止循环引用
        std::set<int> visited_regs;

        bool hasreg      = false;
        auto alloca_inst = edef_use_analysis->getDef(cfg, alloca);
        if (!alloca_inst || alloca_inst->opcode != LLVMIR::IROpCode::ALLOCA) { return -1; }

        auto dims = static_cast<LLVMIR::AllocInst*>(alloca_inst)->dims;
        // 递归计算偏移量
        std::function<void(LLVMIR::Operand*)> calcOffset = [&](LLVMIR::Operand* current_ptr) {
            if (!current_ptr || current_ptr->type != LLVMIR::OperandType::REG) return;
            if (hasreg) return;  // 如果已经处理过寄存器，直接返回
            auto* reg_ptr = static_cast<LLVMIR::RegOperand*>(current_ptr);
            int   reg_num = reg_ptr->reg_num;

            // 防止循环
            if (visited_regs.count(reg_num)) return;
            visited_regs.insert(reg_num);

            // 查找定义
            LLVMIR::Instruction* def_inst = nullptr;
            if (edef_use_analysis) { def_inst = edef_use_analysis->getDef(cfg, current_ptr); }
            if (!def_inst || def_inst->opcode != LLVMIR::IROpCode::GETELEMENTPTR)
            {
                if (def_inst->opcode != IROpCode::ALLOCA)
                {
                    hasreg = true;  // 如果不是GEP或ALLOCA指令，说明无法计算偏移量
                }
                return;
            }
            auto* gep = static_cast<LLVMIR::GEPInst*>(def_inst);

            // 1. 先递归处理基指针
            calcOffset(gep->base_ptr);

            // 2. 计算当前GEP的偏移量
            if (gep->idxs.size() > 0)
            {
                // 多维数组GEP
                size_t start_idx = 0;

                // 跳过首个0索引(指向数组本身)
                if (gep->idxs.size() > 0 && gep->idxs[0]->type == LLVMIR::OperandType::IMMEI32)
                {
                    auto* imm = static_cast<LLVMIR::ImmeI32Operand*>(gep->idxs[0]);
                    if (imm->value == 0) start_idx = 1;
                }
                // 处理每个维度
                for (size_t i = start_idx; i < gep->idxs.size() && i - start_idx < dims.size(); ++i)
                {
                    auto* idx       = gep->idxs[i];
                    int   idx_value = 0;
                    // 提取索引值
                    if (idx->type == LLVMIR::OperandType::IMMEI32)
                    {
                        auto* imm = static_cast<LLVMIR::ImmeI32Operand*>(idx);
                        idx_value = imm->value;
                    }
                    else if (idx->type == LLVMIR::OperandType::REG)
                    {
                        hasreg = true;
                        return;
                    }
                    // 计算这个维度的乘数
                    int multiplier = 1;
                    for (size_t j = i - start_idx + 1; j < dims.size(); ++j) { multiplier *= dims[j]; }

                    total_offset += idx_value * multiplier;
                }
            }
        };
        // 开始计算
        calcOffset(ptr);
        if (hasreg)
        {
            total_offset = -1;  // 如果有寄存器参与计算，返回-1表示无法确定偏移量
        }
        return total_offset;
    }

    bool DSEPass::mayAlias(Operand* ptr1, Operand* ptr2, CFG* cfg)
    {
        auto alloca1 = arralias_analysis->traceToBase(cfg, ptr1);
        auto alloca2 = arralias_analysis->traceToBase(cfg, ptr2);
        if (!alloca1 || !alloca2) { return false; }
        if (alloca1 != alloca2) { return false; }
        auto offset1 = calculateTotalOffset(ptr1, cfg, alloca1);
        auto offset2 = calculateTotalOffset(ptr2, cfg, alloca2);
        if (offset1 == -1 || offset2 == -1)
        {
            return ealias_analyser->queryAlias(ptr1, ptr2, cfg) != EAliasAnalysis::EAliasAnalyser::AliasResult::NoAlias;
        }
        else
        {
#ifdef DEBUG_DSE
            std::cout << "Calculating may alias for " << ptr1 << " and " << ptr2 << std::endl;
            std::cout << "Offsets: " << offset1 << ", " << offset2 << std::endl;
#endif
            return offset1 == offset2;
        }
    }

    bool DSEPass::mustAlias(Operand* ptr1, Operand* ptr2, CFG* cfg)
    {
        auto alloca1 = arralias_analysis->traceToBase(cfg, ptr1);
        auto alloca2 = arralias_analysis->traceToBase(cfg, ptr2);
        if (!alloca1 || !alloca2) { return false; }
        if (alloca1 != alloca2) { return false; }
        auto offset1 = calculateTotalOffset(ptr1, cfg, alloca1);
        auto offset2 = calculateTotalOffset(ptr2, cfg, alloca2);
        if (offset1 == -1 || offset2 == -1)
        {
            return ealias_analyser->queryAlias(ptr1, ptr2, cfg) ==
                   EAliasAnalysis::EAliasAnalyser::AliasResult::MustAlias;
        }
        else
        {
#ifdef DEBUG_DSE
            std::cout << "Calculating must alias for " << ptr1 << " and " << ptr2 << std::endl;
            std::cout << "Offsets: " << offset1 << ", " << offset2 << std::endl;
#endif
            return offset1 == offset2;
        }
    }

    void DSEPass::NoUseStore(CFG* cfg)
    {
        auto                             dead_set = store_insts;
        std::unordered_set<Instruction*> used_stores;
        for (auto [id, block] : cfg->block_id_to_block)
        {
            for (auto inst : block->insts)
            {
                if (inst->opcode == IROpCode::LOAD)
                {
                    auto loadInst = dynamic_cast<LoadInst*>(inst);
                    for (auto store_inst : dead_set)
                    {
                        auto store = dynamic_cast<StoreInst*>(store_inst);
                        if (store && mayAlias(store->ptr, loadInst->ptr, cfg) && !used_stores.count(store_inst))
                        {
                            std::cout << "Find use of store: " << store_inst->toString() << " by "
                                      << loadInst->toString() << std::endl;
                            used_stores.insert(store_inst);
                        }
                    }
                }
                else if (inst->opcode == IROpCode::CALL)
                {
                    auto call_inst = dynamic_cast<CallInst*>(inst);
                    for (auto store_inst : dead_set)
                    {
                        auto store = dynamic_cast<StoreInst*>(store_inst);
                        if (store)
                        {
                            for (auto [ty, arg] : call_inst->args)
                            {
                                if (mayAlias(store->ptr, arg, cfg) && !used_stores.count(store_inst))
                                {
                                    used_stores.insert(store_inst);
                                    std::cout << "Find use of store: " << store_inst->toString() << " by "
                                              << call_inst->toString() << std::endl;
                                }
                            }
                        }
                    }
                }
                else if (inst->opcode == IROpCode::PHI)
                {
                    auto* phi_inst = dynamic_cast<PhiInst*>(inst);
                    for (auto store_inst : dead_set)
                    {
                        auto store = dynamic_cast<StoreInst*>(store_inst);
                        if (store)
                        {
                            for (auto [val, label] : phi_inst->vals_for_labels)
                            {
                                if (val == store->ptr && !used_stores.count(store_inst))
                                {
                                    used_stores.insert(store_inst);
                                    std::cout << "Find use of store: " << store_inst->toString() << " by "
                                              << phi_inst->toString() << std::endl;
                                    continue;
                                }
                                auto base_ptr1 = arralias_analysis->traceToBase(cfg, store->ptr);
                                if (!base_ptr1) continue;
                                auto base_ptr2 = arralias_analysis->traceToBase(cfg, val);
                                if (!base_ptr2) continue;
                                if (base_ptr1 == base_ptr2 && !used_stores.count(store_inst))
                                {
                                    used_stores.insert(store_inst);
                                    std::cout << "Find use of store: " << store_inst->toString() << " by "
                                              << phi_inst->toString() << std::endl;
                                }
                            }

                            if (store->ptr == phi_inst->res && !used_stores.count(store_inst))
                            {
                                used_stores.insert(store_inst);
                            }
                        }
                    }
                }
            }
            for (auto store_inst : used_stores)
            {
                dead_set.erase(store_inst);  // 从死代码集中移除被使用的store
            }
#ifdef DEBUG_DSE

            std::cout << "NoUseStore: Found " << used_stores.size() << " stores that are used by GEP instructions"
                      << std::endl;
#endif
        }
        std::unordered_map<int, Instruction*> to_remove;
        for (auto dead : dead_set)
        {
            auto store = dynamic_cast<StoreInst*>(dead);
            if (store)
            {
                erase_set[store->block_id].insert(dead);  // 将死代码存储到erase_set中
                                                          // #ifdef DEBUG_DSE
                std::cout << "NoUseStore: Found dead store in block " << store->block_id << ": " << store->ptr << " = "
                          << store->val << std::endl;
            }
        }
    }

    // bool DSEPass::isInLoop(LLVMIR::IRBlock* block, CFG* cfg)
    // {
    //     return false;
    //     // 检查block_id是否在循环中
    //     auto* loop_info = cfg->LoopForest;
    //     for (auto loop : loop_info->loop_set)
    //     {
    //         if (loop->loop_nodes.count(block))
    //         {
    //             return true;  // 如果block在循环中，返回true
    //         }
    //     }
    //     return false;
    // }

}  // namespace LLVMIR