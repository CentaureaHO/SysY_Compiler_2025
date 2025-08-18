#include "llvm/parallel/loop_parallel.h"
#include "common/type/type_defs.h"
#include "llvm_ir/build/type_trans.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/function.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
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
        if (i32_vars.size() + float_vars.size() >= 16)
        {
            DBGINFO("    循环使用过多外部变量，跳过并行化");
            return false;
        }

        // 3. 生成循环的并行版本函数
        std::string func_name     = generateParallelFunctionName(cfg, loop);
        auto*       parallel_func = createParallelFunction(func_name, i32_vars, float_vars);

        // 4. 复制循环体到新函数
        if (!copyLoopBodyToFunction(cfg, loop, parallel_func, i32_vars, float_vars)) { return false; }

        return true;
    }

    std::string LoopParallelizationPass::generateParallelFunctionName(CFG* cfg, NaturalLoop* loop) const
    {
        // 生成唯一的函数名
        return "parallel_loop_" + cfg->func->func_def->func_name + "_" + std::to_string(loop->loop_id);
    }

    LLVMIR::IRFunction* LoopParallelizationPass::createParallelFunction(
        const std::string& func_name, const std::set<int>& i32_vars, const std::set<int>& float_vars)
    {
        // 创建并行函数声明
        auto* fd            = new LLVMIR::FuncDefInst(LLVMIR::DataType::VOID, func_name);
        auto* parallel_func = new LLVMIR::IRFunction(TypeSystem::getBasicType(TypeKind::Void), fd);

        // 添加参数类型
        fd->arg_types.push_back(LLVMIR::DataType::PTR);  // function pointer
        fd->arg_regs.push_back(getRegOperand(0));

        ir->functions.push_back(parallel_func);
        return parallel_func;
    }

    bool LoopParallelizationPass::copyLoopBodyToFunction(CFG* cfg, NaturalLoop* loop, LLVMIR::IRFunction* parallel_func,
        const std::set<int>& i32_vars, const std::set<int>& float_vars)
    {
        DBGINFO("    开始复制循环体到并行函数...");

        // 1. 准备寄存器和标签映射
        std::map<int, int> reg_replace_map;
        std::map<int, int> label_replace_map;

        // 2. 设置函数入口和参数解包
        if (parallel_func->blocks.empty()) { parallel_func->createBlock(); }
        int max_reg   = ++parallel_func->max_reg;
        int max_label = parallel_func->max_label;
        std::cout << "    准备并行函数入口..." << std::endl;
        std::cout << "max reg is " << max_reg << std::endl;
        std::cout << "max label is " << max_label << std::endl;
        auto* entry_block = parallel_func->blocks[0];
        entry_block->insts.clear();  // 清空默认的ret指令

        if (!setupFunctionEntry(entry_block, i32_vars, float_vars, reg_replace_map, max_reg)) { return false; }

        // 3. 创建新的循环结构
        LLVMIR::IRBlock* new_header = nullptr;
        LLVMIR::IRBlock* new_latch  = nullptr;
        LLVMIR::IRBlock* new_exit   = nullptr;

        if (!createNewLoopStructure(parallel_func, loop, label_replace_map, max_label, new_header, new_latch, new_exit))
        {
            return false;
        }

        // 4. 复制循环体指令
        if (!copyLoopInstructions(cfg, loop, parallel_func, reg_replace_map, label_replace_map, max_reg))
        {
            return false;
        }

        // 5. 设置循环控制和边界
        if (!setupLoopControl(cfg, loop, entry_block, new_header, new_latch, new_exit, reg_replace_map, max_reg))
        {
            return false;
        }

        // 6. 连接控制流
        connectControlFlow(entry_block, new_header, new_exit);

        DBGINFO("    循环体复制完成");
        return true;
    }

    // 设置函数入口和参数解包
    bool LoopParallelizationPass::setupFunctionEntry(LLVMIR::IRBlock* entry_block, const std::set<int>& i32_vars,
        const std::set<int>& float_vars, std::map<int, int>& reg_replace_map, int& max_reg)
    {
        // 参数解包：从传入的结构体中提取数据
        // struct { int thread_id; int start; int end; int i32_vars[]; float float_vars[]; }

        auto* param_ptr = getRegOperand(0);  // 参数指针

        // 1. 加载 thread_id (offset 0)
        auto* gep_thread =
            new LLVMIR::GEPInst(LLVMIR::DataType::I32, LLVMIR::DataType::I32, param_ptr, getRegOperand(++max_reg));
        gep_thread->idxs.emplace_back(getImmeI32Operand(0));
        entry_block->insts.push_back(gep_thread);

        auto* load_thread = new LLVMIR::LoadInst(LLVMIR::DataType::I32, gep_thread->res, getRegOperand(++max_reg));
        entry_block->insts.push_back(load_thread);
        int thread_id_reg = max_reg;

        // 2. 加载 start (offset 1)
        auto* gep_start =
            new LLVMIR::GEPInst(LLVMIR::DataType::I32, LLVMIR::DataType::I32, param_ptr, getRegOperand(++max_reg));
        gep_start->idxs.emplace_back(getImmeI32Operand(1));
        entry_block->insts.push_back(gep_start);

        auto* load_start = new LLVMIR::LoadInst(LLVMIR::DataType::I32, gep_start->res, getRegOperand(++max_reg));
        entry_block->insts.push_back(load_start);
        int start_reg = max_reg;

        // 3. 加载 end (offset 2)
        auto* gep_end =
            new LLVMIR::GEPInst(LLVMIR::DataType::I32, LLVMIR::DataType::I32, param_ptr, getRegOperand(++max_reg));
        gep_end->idxs.emplace_back(getImmeI32Operand(2));
        entry_block->insts.push_back(gep_end);

        auto* load_end = new LLVMIR::LoadInst(LLVMIR::DataType::I32, gep_end->res, getRegOperand(++max_reg));
        entry_block->insts.push_back(load_end);
        int end_reg = max_reg;

        // 4. 计算当前线程的循环边界
        // part = (end - start) / 4
        auto* sub_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::SUB,
            LLVMIR::DataType::I32,
            getRegOperand(end_reg),
            getRegOperand(start_reg),
            getRegOperand(++max_reg));
        entry_block->insts.push_back(sub_inst);

        auto* div_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::DIV,
            LLVMIR::DataType::I32,
            sub_inst->res,
            getImmeI32Operand(4),
            getRegOperand(++max_reg));
        entry_block->insts.push_back(div_inst);
        int part_reg = max_reg;

        // ST = part * thread_id + start
        auto* mul_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::MUL,
            LLVMIR::DataType::I32,
            getRegOperand(part_reg),
            getRegOperand(thread_id_reg),
            getRegOperand(++max_reg));
        entry_block->insts.push_back(mul_inst);

        auto* add_start_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
            LLVMIR::DataType::I32,
            mul_inst->res,
            getRegOperand(start_reg),
            getRegOperand(++max_reg));
        entry_block->insts.push_back(add_start_inst);
        int local_start_reg = max_reg;

        // ED = part * (thread_id + 1) + start
        auto* thread_add_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
            LLVMIR::DataType::I32,
            getRegOperand(thread_id_reg),
            getImmeI32Operand(1),
            getRegOperand(++max_reg));
        entry_block->insts.push_back(thread_add_inst);

        auto* mul_end_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::MUL,
            LLVMIR::DataType::I32,
            getRegOperand(part_reg),
            thread_add_inst->res,
            getRegOperand(++max_reg));
        entry_block->insts.push_back(mul_end_inst);

        auto* add_end_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
            LLVMIR::DataType::I32,
            mul_end_inst->res,
            getRegOperand(start_reg),
            getRegOperand(++max_reg));
        entry_block->insts.push_back(add_end_inst);
        int local_end_reg = max_reg;

        // 5. 加载外部变量
        int offset = 3;  // 跳过 thread_id, start, end

        // 加载 i32 变量
        for (int old_reg : i32_vars)
        {
            auto* gep_var =
                new LLVMIR::GEPInst(LLVMIR::DataType::I32, LLVMIR::DataType::I32, getRegOperand(++max_reg), param_ptr);
            gep_var->idxs.emplace_back(getImmeI32Operand(offset++));
            entry_block->insts.push_back(gep_var);

            auto* load_var = new LLVMIR::LoadInst(LLVMIR::DataType::I32, gep_var->res, getRegOperand(++max_reg));
            entry_block->insts.push_back(load_var);

            reg_replace_map[old_reg] = max_reg;
        }

        // 加载 float 变量
        for (int old_reg : float_vars)
        {
            auto* gep_var =
                new LLVMIR::GEPInst(LLVMIR::DataType::F32, LLVMIR::DataType::I32, getRegOperand(++max_reg), param_ptr);
            gep_var->idxs.emplace_back(getImmeI32Operand(offset++));
            entry_block->insts.push_back(gep_var);

            auto* load_var = new LLVMIR::LoadInst(LLVMIR::DataType::F32, gep_var->res, getRegOperand(++max_reg));
            entry_block->insts.push_back(load_var);

            reg_replace_map[old_reg] = max_reg;
        }

        // 保存循环边界寄存器
        loop_start_reg_ = local_start_reg;
        loop_end_reg_   = local_end_reg;

        return true;
    }

    // 创建新的循环结构
    bool LoopParallelizationPass::createNewLoopStructure(LLVMIR::IRFunction* parallel_func, NaturalLoop* loop,
        std::map<int, int>& label_replace_map, int& max_label, LLVMIR::IRBlock*& new_header,
        LLVMIR::IRBlock*& new_latch, LLVMIR::IRBlock*& new_exit)
    {
        if (loop->latches.size() != 1)
        {
            DBGINFO("    循环 latches 数量不为 1，无法创建并行结构");
            return false;
        }

        if (loop->preheader) { label_replace_map[loop->preheader->block_id] = 0; }
        else
        {
            DBGINFO("    循环没有 preheader，无法创建并行结构");
            return false;
        }
        // 为原循环的每个基本块创建新的基本块
        for (auto* old_block : loop->loop_nodes)
        {
            // std::cout << std::endl;
            // std::cout << "      处理基本块: " << old_block->block_id << std::endl;
            auto* new_block = parallel_func->createBlock();
            // std::cout << "      新建基本块: " << new_block->block_id << std::endl;
            // std::cout << std::endl;
            label_replace_map[old_block->block_id] = new_block->block_id;

            if (old_block == loop->header) { new_header = new_block; }
            if (loop->latches.count(old_block)) { new_latch = new_block; }
        }

        // 创建退出块
        new_exit = parallel_func->createBlock();

        return new_header != nullptr && new_latch != nullptr && new_exit != nullptr;
    }

    // 复制循环体指令
    bool LoopParallelizationPass::copyLoopInstructions(CFG* cfg, NaturalLoop* loop, LLVMIR::IRFunction* parallel_func,
        std::map<int, int>& reg_replace_map, std::map<int, int>& label_replace_map, int& max_reg)
    {
        // 遍历原循环的所有基本块
        for (auto* old_block : loop->loop_nodes)
        {
            int   new_block_id = label_replace_map[old_block->block_id];
            auto* new_block    = parallel_func->getBlock(new_block_id);

            // 复制每条指令
            for (auto* old_inst : old_block->insts)
            {
                auto* new_inst = copyInstruction(old_inst, reg_replace_map, label_replace_map, max_reg);
                if (!new_inst)
                {
                    DBGINFO("      复制指令失败");
                    return false;
                }
                new_block->insts.push_back(new_inst);
            }
        }

        return true;
    }

    // 复制单条指令
    LLVMIR::Instruction* LoopParallelizationPass::copyInstruction(LLVMIR::Instruction* old_inst,
        std::map<int, int>& reg_replace_map, std::map<int, int>& label_replace_map, int& max_reg)
    {
        // 创建新指令的副本
        auto* new_inst = old_inst->Clone();

        // 更新结果寄存器
        if (old_inst->GetResultReg() != -1)
        {
            int new_reg                               = ++max_reg;
            reg_replace_map[old_inst->GetResultReg()] = new_reg;
            std::cout << std::endl;
            std::cout << "      复制指令: " << old_inst->GetResultReg() << " -> " << new_reg << std::endl;
            new_inst->ReplaceAllOperands(reg_replace_map);
            std::cout << "      复制指令: " << old_inst->toString() << " -> " << new_inst->toString() << std::endl;
            std::cout << std::endl;
        }

        // 更新操作数中的寄存器
        auto used_regs = old_inst->GetUsedRegs();
        for (int old_reg : used_regs)
        {
            if (reg_replace_map.count(old_reg))
            {
                new_inst->ReplaceAllOperands(reg_replace_map);
                break;
            }
        }

        // 更新跳转指令中的标签
        if (old_inst->opcode == LLVMIR::IROpCode::BR_COND || old_inst->opcode == LLVMIR::IROpCode::BR_UNCOND ||
            old_inst->opcode == LLVMIR::IROpCode::PHI)
        {
            updateJumpTargets(new_inst, label_replace_map);
        }

        return new_inst;
    }

    // 设置循环控制
    bool LoopParallelizationPass::setupLoopControl(CFG* cfg, NaturalLoop* loop, LLVMIR::IRBlock* entry_block,
        LLVMIR::IRBlock* new_header, LLVMIR::IRBlock* new_latch, LLVMIR::IRBlock* new_exit,
        std::map<int, int>& reg_replace_map, int& max_reg)
    {
        // 1. 修改header中的phi指令，使用新的循环边界
        for (auto* inst : new_header->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI)
            {
                auto* phi = static_cast<LLVMIR::PhiInst*>(inst);

                // 找到来自preheader的值，替换为local_start
                for (auto& [val, label] : phi->vals_for_labels)
                {
                    if (static_cast<LLVMIR::LabelOperand*>(label)->label_num == loop->preheader->block_id)
                    {
                        if (val->type == LLVMIR::OperandType::REG)
                        {
                            std::map<int, int> replace_map = {{val->GetRegNum(), loop_start_reg_}};
                            phi->ReplaceAllOperands(replace_map);
                        }
                        break;
                    }
                }
            }
        }

        // 2. 修改latch中的条件跳转，使用新的结束条件
        auto* latch_last_inst = new_latch->insts.back();
        if (latch_last_inst->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br_cond = static_cast<LLVMIR::BranchCondInst*>(latch_last_inst);

            // 查找条件比较指令并修改
            for (auto it = new_latch->insts.rbegin(); it != new_latch->insts.rend(); ++it)
            {
                if ((*it)->opcode == LLVMIR::IROpCode::ICMP)
                {
                    auto* icmp = static_cast<LLVMIR::IcmpInst*>(*it);

                    // 将原来的结束条件替换为 loop_end_reg
                    std::map<int, int> replace_map = {{icmp->rhs->GetRegNum(), loop_end_reg_}};
                    icmp->ReplaceAllOperands(replace_map);
                    break;
                }
            }

            // 修改跳转目标：原来跳到exit的地方改为跳到new_exit
            if (br_cond->false_label->type == LLVMIR::OperandType::LABEL)
            {
                br_cond->false_label = getLabelOperand(new_exit->block_id);
            }
        }

        return true;
    }

    // 连接控制流
    void LoopParallelizationPass::connectControlFlow(
        LLVMIR::IRBlock* entry_block, LLVMIR::IRBlock* new_header, LLVMIR::IRBlock* new_exit)
    {
        // entry_block -> new_header
        auto* br_to_header = new LLVMIR::BranchUncondInst(getLabelOperand(new_header->block_id));
        entry_block->insts.push_back(br_to_header);

        // new_exit -> return
        auto* ret_inst = new LLVMIR::RetInst(LLVMIR::DataType::VOID, nullptr);
        new_exit->insts.push_back(ret_inst);
    }

    // 更新跳转目标
    void LoopParallelizationPass::updateJumpTargets(LLVMIR::Instruction* inst, std::map<int, int>& label_replace_map)
    {
        if (inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* br = static_cast<LLVMIR::BranchUncondInst*>(inst);
            if (br->target_label->type == LLVMIR::OperandType::LABEL)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(br->target_label);
                if (label_replace_map.count(label_op->label_num))
                {
                    br->target_label = getLabelOperand(label_replace_map[label_op->label_num]);
                }
            }
        }
        else if (inst->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br = static_cast<LLVMIR::BranchCondInst*>(inst);

            if (br->true_label->type == LLVMIR::OperandType::LABEL)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(br->true_label);
                if (label_replace_map.count(label_op->label_num))
                {
                    br->true_label = getLabelOperand(label_replace_map[label_op->label_num]);
                }
            }

            if (br->false_label->type == LLVMIR::OperandType::LABEL)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(br->false_label);
                if (label_replace_map.count(label_op->label_num))
                {
                    br->false_label = getLabelOperand(label_replace_map[label_op->label_num]);
                }
            }
        }
        else if (inst->opcode == LLVMIR::IROpCode::PHI)
        {
            auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
            for (auto& [val, label] : phi->vals_for_labels)
            {
                if (label->type == LLVMIR::OperandType::LABEL)
                {
                    auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                    if (label_replace_map.count(label_op->label_num))
                    {
                        label = getLabelOperand(label_replace_map[label_op->label_num]);
                    }
                }
            }
        }
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