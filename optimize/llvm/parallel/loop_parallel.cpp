#include "llvm/parallel/loop_parallel.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include <iostream>
#include <algorithm>
#include <functional>

#define DBGMODE

#ifdef DBGMODE
template <typename... Args>
void dbg_impl(Args&&... args)
{
    ((std::cout << args), ...);
    std::cout << std::endl;
}
#define DBGINFO(...) dbg_impl(__VA_ARGS__)
#else
#define DBGINFO(...) \
    do {             \
    } while (0)
#endif

namespace Transform
{
    static int calculateLoopDepth(NaturalLoop* loop)
    {
        int depth = 0;
        while (!loop->fa_loop)
        {
            loop = loop->fa_loop;
            depth++;
        }
        return depth;
    }

    void LoopParallelizationPass::Execute()
    {
        DBGINFO("开始循环并行化分析...");
        processAllLoops();

        DBGINFO("并行化分析完成: ", loops_parallelized_, "/", loops_processed_, " 个循环被并行化");
    }

    void LoopParallelizationPass::processAllLoops()
    {
        // 类比LoopFullUnrollPass::processAllLoops()
        for (const auto& [func_def, cfg] : ir->cfg)
        {
            if (cfg && cfg->LoopForest && !cfg->LoopForest->loop_set.empty()) { processFunction(cfg); }
        }
    }

    void LoopParallelizationPass::processFunction(CFG* cfg)
    {
        // 完全类比LoopFullUnrollPass::processFunction()的DFS模式
        if (!cfg || !cfg->LoopForest) return;

        DBGINFO("处理函数中的循环...");

        std::function<void(NaturalLoop*)> dfs = [&](NaturalLoop* loop) {
            if (processLoop(cfg, loop)) { return; }
            for (auto* child_loop : cfg->LoopForest->loopG[loop->loop_id])
            {
                if (child_loop != nullptr) { dfs(child_loop); }
            }

            // 检查是否有子循环（判断是否为叶子节点）
            for (auto* child_loop : cfg->LoopForest->loopG[loop->loop_id])
            {
                if (child_loop != nullptr) { break; }
            }
        };

        auto root_loops = cfg->LoopForest->getRootLoops();
        for (auto* loop : root_loops) { dfs(loop); }
    }

    bool LoopParallelizationPass::processLoop(CFG* cfg, NaturalLoop* loop)
    {
        // 类比LoopFullUnrollPass::processLoop()
        DBGINFO("分析循环 ", loop->loop_id, " (深度: ", loop->loop_id, ")");

        // 为单个循环进行SCEV分析
        if (scev_analyser_) { scev_analyser_->analyzeSingleLoop(loop); }

        // 尝试并行化循环
        return tryParallelizeLoop(cfg, loop);
    }

    bool LoopParallelizationPass::tryParallelizeLoop(CFG* cfg, NaturalLoop* loop)
    {
        ++loops_processed_;

        ParallelizationInfo info;

        // 1. 基本结构检查（类比loop unroll的结构检查）
        if (!canParallelize(loop))
        {
            info.reason                 = "循环结构不支持并行化";
            parallelization_info_[loop] = info;
            logResult(loop, false, info.reason);
            return false;
        }

        // 2. 循环依赖分析（这是并行化的核心）
        if (!analyzeLoopDependencies(loop, info))
        {
            parallelization_info_[loop] = info;
            logResult(loop, false, info.reason);
            return false;
        }

        // 3. 估算并行化收益
        if (!isParallelizationProfitable(loop, info))
        {
            info.reason                 = "并行化收益不足";
            parallelization_info_[loop] = info;
            logResult(loop, false, info.reason);
            return false;
        }

        // 4. 处理嵌套循环冲突
        handleNestedLoopConflicts(loop);

        // 5. 检查是否因冲突被禁用
        auto& current_info = parallelization_info_[loop];
        if (current_info.conflicts_with_inner || current_info.conflicts_with_outer)
        {
            logResult(loop, false, "嵌套循环冲突");
            return false;
        }

        // 6. 执行并行化
        if (performParallelization(cfg, loop))
        {
            ++loops_parallelized_;
            current_info.should_parallelize   = true;
            current_info.priority             = calculateParallelizationPriority(loop);
            current_info.estimated_iterations = estimateIterationCount(loop);
            logResult(loop,
                true,
                "成功并行化 (迭代次数: " + std::to_string(current_info.estimated_iterations) +
                    ", 优先级: " + std::to_string(current_info.priority) + ")");
            return true;
        }
        else
        {
            current_info.reason = "并行化实施失败";
            logResult(loop, false, current_info.reason);
            return false;
        }
    }

    bool LoopParallelizationPass::canParallelize(NaturalLoop* loop) const
    {
        // 基本结构检查（类比loop unroll的结构要求）
        if (!loop || !loop->header || !loop->preheader) { return false; }

        // 检查latch数量（简化要求，只支持单latch）
        if (loop->latches.size() != 1) { return false; }

        // 检查是否是简单的for循环
        if (!isSimpleForLoop(loop)) { return false; }

        return true;
    }

    bool LoopParallelizationPass::analyzeLoopDependencies(NaturalLoop* loop, ParallelizationInfo& info)
    {
        DBGINFO("  检查循环依赖...");

        // 手动进行内存依赖分析
        if (hasLoopCarriedMemoryDependency(loop))
        {
            info.reason                      = "检测到循环携带内存依赖";
            info.has_loop_carried_dependency = true;
            return false;
        }

        info.can_parallelize = true;
        return true;
    }

    bool LoopParallelizationPass::hasLoopCarriedMemoryDependency(NaturalLoop* loop)
    {
        // 收集所有内存操作
        std::vector<LLVMIR::Instruction*> memory_ops = collectMemoryInstructions(loop);

        DBGINFO("    检查 ", memory_ops.size(), " 个内存操作");

        // 两两检查内存操作是否存在依赖
        for (size_t i = 0; i < memory_ops.size(); i++)
        {
            for (size_t j = i + 1; j < memory_ops.size(); j++)
            {
                if (checkInstructionDependency(loop, memory_ops[i], memory_ops[j]))
                {
                    DBGINFO("    ✗ 发现内存依赖");
                    return true;
                }
            }
        }

        DBGINFO("    ✓ 未发现循环携带依赖");
        return false;
    }

    bool LoopParallelizationPass::checkInstructionDependency(
        NaturalLoop* loop, LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2)
    {
        // 检查两个指令是否都是内存操作
        bool inst1_is_memory = (inst1->opcode == LLVMIR::IROpCode::LOAD || inst1->opcode == LLVMIR::IROpCode::STORE);
        bool inst2_is_memory = (inst2->opcode == LLVMIR::IROpCode::LOAD || inst2->opcode == LLVMIR::IROpCode::STORE);

        if (!inst1_is_memory || !inst2_is_memory) { return false; }

        // 如果至少有一个是写操作，需要检查别名
        bool has_write = (inst1->opcode == LLVMIR::IROpCode::STORE || inst2->opcode == LLVMIR::IROpCode::STORE);

        if (!has_write)
        {
            return false;  // 两个读操作不会产生依赖
        }

        // 获取内存地址
        LLVMIR::Operand* ptr1 = nullptr;
        LLVMIR::Operand* ptr2 = nullptr;

        if (inst1->opcode == LLVMIR::IROpCode::LOAD) { ptr1 = static_cast<LLVMIR::LoadInst*>(inst1)->ptr; }
        else if (inst1->opcode == LLVMIR::IROpCode::STORE) { ptr1 = static_cast<LLVMIR::StoreInst*>(inst1)->ptr; }

        if (inst2->opcode == LLVMIR::IROpCode::LOAD) { ptr2 = static_cast<LLVMIR::LoadInst*>(inst2)->ptr; }
        else if (inst2->opcode == LLVMIR::IROpCode::STORE) { ptr2 = static_cast<LLVMIR::StoreInst*>(inst2)->ptr; }

        if (!ptr1 || !ptr2)
        {
            return true;  // 保守估计
        }

        // 使用别名分析检查
        if (alias_analysis_)
        {
            Analysis::AliasAnalyser::AliasResult result = alias_analysis_->queryAlias(ptr1, ptr2, loop->cfg);
            return result != Analysis::AliasAnalyser::NoAlias;
        }

        // 简单启发式：如果都是基于归纳变量的数组访问，可能安全
        return true;  // 保守估计有依赖
    }

    void LoopParallelizationPass::handleNestedLoopConflicts(NaturalLoop* loop)
    {
        auto& info = parallelization_info_[loop];

        // 检查是否有内层循环已经被并行化
        if (hasConflictingInnerLoops(loop))
        {
            info.conflicts_with_inner = true;
            info.reason               = "内层循环已被并行化";
            info.should_parallelize   = false;
            DBGINFO("    因内层循环冲突，跳过外层并行化");
            return;
        }

        // 如果当前循环被标记为可并行化，标记可能冲突的外层循环
        if (info.can_parallelize) { markConflictingLoops(loop); }
    }

    bool LoopParallelizationPass::hasConflictingInnerLoops(NaturalLoop* loop)
    {
        // 检查直接子循环
        for (auto* child_loop : loop->cfg->LoopForest->loopG[loop->loop_id])
        {
            if (child_loop != nullptr)
            {
                auto it = parallelization_info_.find(child_loop);
                if (it != parallelization_info_.end() && it->second.should_parallelize) { return true; }
            }
        }
        return false;
    }

    void LoopParallelizationPass::markConflictingLoops(NaturalLoop* loop)
    {
        // 标记所有外层循环为冲突（简化实现）
        // 在真实实现中，可能需要更精细的冲突分析
        auto& info    = parallelization_info_[loop];
        info.priority = calculateParallelizationPriority(loop);
    }

    bool LoopParallelizationPass::isParallelizationProfitable(NaturalLoop* loop, const ParallelizationInfo& info) const
    {
        // 估算迭代次数
        int iterations = estimateIterationCount(loop);
        if (iterations < MIN_ITERATIONS_FOR_PARALLEL) { return false; }

        // 检查循环体大小
        int loop_size = 0;
        for (auto* block : loop->loop_nodes) { loop_size += block->insts.size(); }

        if (loop_size < MIN_LOOP_SIZE_FOR_PARALLEL) { return false; }

        return true;
    }

    // 辅助方法实现
    std::vector<LLVMIR::Instruction*> LoopParallelizationPass::collectMemoryInstructions(NaturalLoop* loop)
    {
        std::vector<LLVMIR::Instruction*> memory_ops;

        for (auto* block : loop->loop_nodes)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::LOAD || inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    memory_ops.push_back(inst);
                }
            }
        }

        return memory_ops;
    }

    bool LoopParallelizationPass::isSimpleForLoop(NaturalLoop* loop) const
    {
        // 检查循环是否有规律的归纳变量
        if (scev_analyser_)
        {
            auto* scev_info = scev_analyser_->getLoopInfo(loop);
            return scev_info && scev_info->is_simple_loop;
        }

        // 简单启发式检查
        return loop->loop_nodes.size() <= 2;  // header + latch
    }

    int LoopParallelizationPass::estimateIterationCount(NaturalLoop* loop) const
    {
        if (scev_analyser_)
        {
            auto trip_count_opt = scev_analyser_->getLoopTripCount(loop);
            if (trip_count_opt) { return *trip_count_opt; }
        }

        // 默认估计
        return 100;
    }

    int LoopParallelizationPass::calculateParallelizationPriority(NaturalLoop* loop) const
    {
        int priority = 10;  // 基础分数

        // // 基于循环深度（深层循环执行频率更高）
        priority += calculateLoopDepth(loop) * 5;

        // 基于循环大小
        int loop_size = 0;
        for (auto* block : loop->loop_nodes) { loop_size += block->insts.size(); }
        priority += std::min(loop_size, 20);

        // 基于迭代次数
        int iterations = estimateIterationCount(loop);
        if (iterations >= 1000) { priority += 20; }
        else if (iterations >= 100) { priority += 10; }

        return priority;
    }

    bool LoopParallelizationPass::performParallelization(CFG* cfg, NaturalLoop* loop)
    {
        // 1. 生成并行函数
        if (!generateParallelFunction(cfg, loop)) { return false; }

        // 2. 添加动态检查逻辑
        if (!insertDynamicParallelCheck(cfg, loop)) { return false; }

        return true;
    }

    bool LoopParallelizationPass::generateParallelFunction(CFG* cfg, NaturalLoop* loop)
    {
        // 1. 创建并行函数声明
        addParallelLibraryFunctions();

        // 2. 分析循环外定义的变量
        auto [i32_vars, float_vars] = analyzeLoopExternalVariables(cfg, loop);

        // 3. 生成循环的并行版本函数
        // std::string func_name     = generateParallelFunctionName(cfg, loop);
        // auto*       parallel_func = createParallelFunction(func_name, i32_vars, float_vars);

        // 4. 复制循环体到新函数
        // if (!copyLoopBodyToFunction(cfg, loop, parallel_func, i32_vars, float_vars)) { return false; }

        return true;
    }

    void LoopParallelizationPass::addParallelLibraryFunctions()
    {
        // 添加运行时并行库函数
        auto* thread_func = new LLVMIR::FuncDeclareInst(LLVMIR::DataType::VOID, "___parallel_loop_");

        thread_func->arg_types.push_back(LLVMIR::DataType::PTR);  // function pointer
        thread_func->arg_types.push_back(LLVMIR::DataType::I32);  // start
        thread_func->arg_types.push_back(LLVMIR::DataType::I32);  // end
        thread_func->arg_types.push_back(LLVMIR::DataType::I32);  // i32_count
        thread_func->arg_types.push_back(LLVMIR::DataType::I32);  // f32_count

        ir->function_declare.push_back(thread_func);
    }

    bool LoopParallelizationPass::insertDynamicParallelCheck(CFG* cfg, NaturalLoop* loop)
    {
        // 1. 计算循环深度
        int loop_depth = calculateLoopDepth(loop);

        // 2. 估算指令数量
        int instruction_count = 0;
        for (auto* block : loop->loop_nodes) { instruction_count += block->insts.size(); }

        // 3. 创建检查基本块
        auto* check_block = cfg->func->createBlock();

        // 4. 插入迭代次数检查
        insertIterationCountCheck(cfg, loop, check_block, loop_depth, instruction_count);

        // 5. 修改控制流：preheader -> check_block -> [parallel_call | original_loop]
        // redirectControlFlow(cfg, loop, check_block);

        return true;
    }

    void LoopParallelizationPass::insertIterationCountCheck(
        CFG* cfg, NaturalLoop* loop, LLVMIR::IRBlock* check_block, int loop_depth, int inst_count)
    {
        // 实现您的动态检查逻辑
        int depth_constants[] = {0, 1, 500, 5000};

        // 计算迭代次数阈值
        int threshold = (loop_depth < 4) ? std::max(20, 4000000 / inst_count / depth_constants[loop_depth]) : 20;

        // 生成检查代码...
    }

    std::tuple<std::set<int>, std::set<int>> LoopParallelizationPass::analyzeLoopExternalVariables(
        CFG* cfg, NaturalLoop* loop)
    {
        std::set<int> i32_vars, float_vars;

        std::map<int, LLVMIR::DataType> var_types;

        // 分析循环中使用但在循环外定义的变量
        for (auto* block : loop->loop_nodes)
        {
            if (loop->loop_nodes.count(block)) { continue; }
            for (auto inst : block->insts)
            {
                if (inst->GetResultReg() != -1)
                {
                    // 记录
                    var_types[inst->GetResultReg()] = inst->GetResultType();
                }
            }
        }

        for (auto node : loop->loop_nodes)
        {
            for (auto inst : node->insts)
            {
                for (auto use : inst->GetUsedRegs())
                {
                    if (var_types.count(use))
                    {
                        auto type = var_types[use];
                        if (type == LLVMIR::DataType::I32) { i32_vars.insert(use); }
                        else if (type == LLVMIR::DataType::F32) { float_vars.insert(use); }
                    }
                }
            }
        }

        return {i32_vars, float_vars};
    }

    void LoopParallelizationPass::logResult(NaturalLoop* loop, bool success, const std::string& reason) const
    {
#ifdef DBGMODE
        std::cout << "Loop " << loop->loop_id << ": " << (success ? "PARALLELIZED" : "SKIPPED") << " - " << reason
                  << std::endl;
#endif
    }

}  // namespace Transform