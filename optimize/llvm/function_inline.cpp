#include "llvm/function_inline.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include "cfg.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <functional>
#include <queue>
#include <set>

namespace Transform
{

    FunctionInlineAnalyzer::FunctionInlineAnalyzer(LLVMIR::IR* ir) : ir(ir) {}

    void FunctionInlineAnalyzer::buildCallGraph()
    {
        call_graph.clear();
        reverse_call_graph.clear();
        all_call_sites.clear();

        for (auto func : ir->functions)
        {
            FunctionInfo& info = function_info[func];
            info.func          = func;

            for (auto block : func->blocks)
            {
                ++info.block_count;
                for (auto inst : block->insts)
                {
                    ++info.instruction_count;

                    if (inst->opcode == LLVMIR::IROpCode::CALL)
                    {
                        auto call_inst = static_cast<LLVMIR::CallInst*>(inst);
                        auto callee    = findFunction(call_inst->func_name);

                        if (callee)
                        {
                            info.calls_made.push_back(call_inst);
                            ++info.call_count;

                            call_graph[func].insert(callee);
                            reverse_call_graph[callee].insert(func);

                            function_info[callee].call_sites.push_back(call_inst);
                            function_info[callee].called_count++;

                            CallSiteInfo call_site;
                            call_site.call_inst           = call_inst;
                            call_site.caller              = func;
                            call_site.callee              = callee;
                            call_site.in_loop             = isInLoop(call_inst, func);
                            call_site.nesting_level       = calculateNestingLevel(call_inst);
                            call_site.estimated_frequency = estimateCallFrequency(call_inst);
                            call_site.has_pointer_args    = hasPointerArguments(call_inst);

                            all_call_sites.push_back(call_site);
                        }
                    }
                }
            }

            info.has_pointer_params = hasPointerParameters(func);
        }
    }

    void FunctionInlineAnalyzer::analyzeFunction(LLVMIR::IRFunction* func)
    {
        FunctionInfo& info = function_info[func];

        NaturalLoop* loop = getLoopForBlock(func, func->blocks.front());
        if (loop)
            info.has_loops = true;
        else
            info.has_loops = false;

        info.complexity_score = computeFunctionComplexity(func);
    }

    void FunctionInlineAnalyzer::detectRecursion()
    {
        for (auto& [func, info] : function_info)
        {
            for (auto call_inst : info.calls_made)
            {
                if (call_inst->func_name != func->func_def->func_name) continue;

                info.is_recursive = true;
                break;
            }
        }

        for (auto& [current_func, info] : function_info)
        {
            if (info.is_recursive) continue;

            std::set<LLVMIR::IRFunction*>   visited;
            std::queue<LLVMIR::IRFunction*> to_visit;

            if (call_graph.count(current_func))
                for (auto callee : call_graph[current_func]) to_visit.push(callee);

            while (!to_visit.empty() && !info.is_recursive)
            {
                auto node = to_visit.front();
                to_visit.pop();

                if (node == current_func)
                {
                    info.is_recursive = true;
                    break;
                }

                if (visited.count(node)) continue;
                visited.insert(node);

                if (!call_graph.count(node)) continue;

                for (auto callee : call_graph[node])
                    if (!visited.count(callee)) to_visit.push(callee);
            }
        }
    }

    void FunctionInlineAnalyzer::computeTopologicalOrder()
    {
        topo_order.clear();
        std::map<LLVMIR::IRFunction*, int> in_degree;

        for (auto func : ir->functions) in_degree[func] = 0;

        for (auto& [caller, callees] : call_graph)
            for (auto callee : callees) in_degree[callee]++;

        std::queue<LLVMIR::IRFunction*> queue;
        for (auto func : ir->functions)
            if (in_degree[func] == 0) queue.push(func);

        while (!queue.empty())
        {
            auto func = queue.front();
            queue.pop();
            topo_order.push_back(func);

            if (!call_graph.count(func)) continue;

            for (auto callee : call_graph[func])
            {
                --in_degree[callee];
                if (in_degree[callee] == 0) queue.push(callee);
            }
        }

        for (auto func : ir->functions)
            if (std::find(topo_order.begin(), topo_order.end(), func) == topo_order.end()) topo_order.push_back(func);
    }

    void FunctionInlineAnalyzer::analyzeCallSites()
    {
        for (auto& call_site : all_call_sites)
        {
            if (call_site.in_loop) call_site.estimated_frequency *= 10;
            if (call_site.nesting_level > 1) call_site.estimated_frequency *= call_site.nesting_level;
        }
    }

    void FunctionInlineAnalyzer::computeComplexityScores()
    {
        for (auto& [func, info] : function_info) info.complexity_score = computeFunctionComplexity(func);
    }

    NaturalLoop* FunctionInlineAnalyzer::getLoopForBlock(LLVMIR::IRFunction* func, LLVMIR::IRBlock* block)
    {
        for (auto& [func_def, cfg] : ir->cfg)
        {
            if (cfg->func != func && cfg->LoopForest) continue;
            if (!cfg->LoopForest) continue;

            auto it = cfg->LoopForest->header_loop_map.find(block);
            if (it != cfg->LoopForest->header_loop_map.end()) return it->second;

            for (auto loop : cfg->LoopForest->loop_set)
                if (loop->loop_nodes.count(block)) return loop;
        }
        return nullptr;
    }

    int FunctionInlineAnalyzer::getLoopDepth(LLVMIR::IRFunction* func, LLVMIR::IRBlock* block)
    {
        int          depth        = 0;
        NaturalLoop* current_loop = getLoopForBlock(func, block);

        while (current_loop != nullptr)
        {
            ++depth;
            current_loop = current_loop->fa_loop;
        }

        return depth;
    }

    int FunctionInlineAnalyzer::getControlFlowNesting(LLVMIR::IRFunction* func, LLVMIR::CallInst* call_inst)
    {
        LLVMIR::IRBlock* containing_block = nullptr;
        for (auto block : func->blocks)
        {
            for (auto inst : block->insts)
            {
                if (inst != call_inst) continue;

                containing_block = block;
                break;
            }
            if (containing_block) break;
        }

        if (!containing_block) return 1;

        int loop_depth    = getLoopDepth(func, containing_block);
        int dom_depth     = getDominatorDepth(func, containing_block);
        int total_nesting = dom_depth + loop_depth;

        return total_nesting > 0 ? total_nesting : 1;
    }

    int FunctionInlineAnalyzer::getDominatorDepth(LLVMIR::IRFunction* func, LLVMIR::IRBlock* block)
    {
        CFG* target_cfg = nullptr;
        for (auto& [func_def, cfg] : ir->cfg)
        {
            if (cfg->func != func) continue;

            target_cfg = cfg;
            break;
        }

        if (!target_cfg) return 1;

        auto dom_it = ir->DomTrees.find(target_cfg);
        if (dom_it == ir->DomTrees.end() || !dom_it->second) return 1;

        Cele::Algo::DomAnalyzer* dom_analyzer = dom_it->second;

        int block_id = block->block_id;
        int depth    = 0;

        while (block_id != -1 && static_cast<size_t>(block_id) < dom_analyzer->imm_dom.size())
        {
            int immediate_dominator = dom_analyzer->imm_dom[block_id];
            if (immediate_dominator == block_id) break;

            depth++;
            block_id = immediate_dominator;

            if (depth > 50) break;
        }

        return depth;
    }

    bool FunctionInlineAnalyzer::isInLoop(LLVMIR::CallInst* call_inst, LLVMIR::IRFunction* func)
    {
        LLVMIR::IRBlock* containing_block = nullptr;
        for (auto block : func->blocks)
        {
            for (auto inst : block->insts)
            {
                if (inst != call_inst) continue;

                containing_block = block;
                break;
            }
            if (containing_block) break;
        }

        if (!containing_block) return false;

        return getLoopForBlock(func, containing_block) != nullptr;
    }

    int FunctionInlineAnalyzer::estimateCallFrequency(LLVMIR::CallInst* call_inst)
    {
        LLVMIR::IRBlock* containing_block = nullptr;
        for (auto inst_func : ir->functions)
        {
            for (auto block : inst_func->blocks)
            {
                for (auto inst : block->insts)
                {
                    if (inst != call_inst) continue;

                    containing_block = block;
                    break;
                }
                if (containing_block) break;
            }
            if (!containing_block) continue;

            int loop_depth          = getLoopDepth(inst_func, containing_block);
            int estimated_frequency = 1;
            for (int i = 0; i < loop_depth; i++) estimated_frequency *= 10;

            return estimated_frequency;
        }

        return 1;
    }

    int FunctionInlineAnalyzer::calculateNestingLevel(LLVMIR::CallInst* call_inst)
    {
        for (auto func : ir->functions)
        {
            for (auto block : func->blocks)
            {
                for (auto inst : block->insts)
                {
                    if (inst == call_inst) return getControlFlowNesting(func, call_inst);
                }
            }
        }

        return 1;
    }

    bool FunctionInlineAnalyzer::hasPointerParameters(LLVMIR::IRFunction* func)
    {
        for (auto arg_type : func->func_def->arg_types)
            if (arg_type == LLVMIR::DataType::PTR) return true;

        return false;
    }

    bool FunctionInlineAnalyzer::hasPointerArguments(LLVMIR::CallInst* call_inst)
    {
        for (auto& arg : call_inst->args)
            if (arg.first == LLVMIR::DataType::PTR) return true;

        return false;
    }

    int FunctionInlineAnalyzer::computeFunctionComplexity(LLVMIR::IRFunction* func)
    {
        int complexity = 0;

        complexity += function_info[func].instruction_count;
        complexity += function_info[func].block_count * 2;
        complexity += function_info[func].call_count * 3;
        if (function_info[func].has_loops) complexity += 10;

        return complexity;
    }

    bool FunctionInlineAnalyzer::wouldCauseTooMuchGrowth(LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee)
    {
        const int MAX_FUNCTION_SIZE = 500;
        const int MAX_TOTAL_GROWTH  = 200;

        int caller_size = function_info[caller].instruction_count;
        int callee_size = function_info[callee].instruction_count;
        int call_count  = getCallCount(caller, callee);

        int estimated_growth = callee_size * call_count;

        return (caller_size + estimated_growth > MAX_FUNCTION_SIZE) || (estimated_growth > MAX_TOTAL_GROWTH);
    }

    LLVMIR::IRFunction* FunctionInlineAnalyzer::findFunction(const std::string& name)
    {
        for (auto func : ir->functions)
            if (func->func_def->func_name == name) return func;

        return nullptr;
    }

    bool FunctionInlineAnalyzer::isRecursive(LLVMIR::IRFunction* func)
    {
        auto it = function_info.find(func);
        return it != function_info.end() && it->second.is_recursive;
    }

    int FunctionInlineAnalyzer::getCallCount(LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee)
    {
        int  count = 0;
        auto it    = function_info.find(caller);
        if (it != function_info.end())
            for (auto call_inst : it->second.calls_made)
                if (call_inst->func_name == callee->func_def->func_name) ++count;

        return count;
    }

    const FunctionInlineAnalyzer::FunctionInfo& FunctionInlineAnalyzer::getFunctionInfo(LLVMIR::IRFunction* func) const
    {
        static FunctionInfo empty;
        auto                it = function_info.find(func);
        return it != function_info.end() ? it->second : empty;
    }

    std::string FunctionInlineAnalyzer::getInlineReason(
        LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee, LLVMIR::CallInst* call_inst)
    {
        const auto& caller_info = getFunctionInfo(caller);
        const auto& callee_info = getFunctionInfo(callee);

        if (caller == callee) return "Direct recursive call";
        if (callee_info.is_recursive) return "Callee is recursive";

        // 按照新的激进内联策略检查各个条件
        bool flag1 = callee_info.instruction_count <= 30;
        bool flag2 = (caller_info.instruction_count + callee_info.instruction_count) <= 200;
        bool flag3 = callee_info.has_pointer_params && (caller != callee);
        bool flag5 = callee_info.instruction_count <= 15;

        CallSiteInfo call_site_info;
        for (const auto& cs : all_call_sites)
            if (cs.call_inst == call_inst)
            {
                call_site_info = cs;
                break;
            }
        bool flag4 = call_site_info.in_loop && callee_info.instruction_count <= 50;

        if (flag5)
            return "Very small function (" + std::to_string(callee_info.instruction_count) + " instructions <= 15)";

        if (flag1) return "Small function (" + std::to_string(callee_info.instruction_count) + " instructions <= 30)";

        if (flag2)
            return "Combined size acceptable (" + std::to_string(caller_info.instruction_count) + " + " +
                   std::to_string(callee_info.instruction_count) + " = " +
                   std::to_string(caller_info.instruction_count + callee_info.instruction_count) + " <= 200)";

        if (flag3) return "Function has pointer parameters (good for optimization)";

        if (flag4)
            return "Call in loop with acceptable size (" + std::to_string(callee_info.instruction_count) +
                   " instructions <= 50, frequency x" + std::to_string(call_site_info.estimated_frequency) + ")";

        return "Does not meet aggressive inlining criteria";
    }

    bool FunctionInlineAnalyzer::shouldInline(
        LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee, LLVMIR::CallInst* call_inst)
    {
        const auto& caller_info = getFunctionInfo(caller);
        const auto& callee_info = getFunctionInfo(callee);

        if (caller == callee) return false;
        if (callee_info.is_recursive) return false;

        bool flag1 = callee_info.instruction_count <= 30;
        bool flag2 = (caller_info.instruction_count + callee_info.instruction_count) <= 200;
        bool flag3 = callee_info.has_pointer_params && (caller != callee);

        CallSiteInfo call_site_info;
        for (const auto& cs : all_call_sites)
            if (cs.call_inst == call_inst)
            {
                call_site_info = cs;
                break;
            }
        bool flag4 = call_site_info.in_loop && callee_info.instruction_count <= 50;

        bool flag5 = callee_info.instruction_count <= 15;

        return flag1 || flag2 || flag3 || flag4 || flag5;
    }

    void FunctionInlineAnalyzer::analyze()
    {
        buildCallGraph();

        for (auto func : ir->functions) analyzeFunction(func);

        detectRecursion();
        computeTopologicalOrder();
        analyzeCallSites();
        computeComplexityScores();
    }

    void FunctionInlineAnalyzer::printAnalysisReport() const
    {
        std::cout << "\n=== Function Inline Analysis Report ===" << std::endl;

        for (const auto& [func, info] : function_info)
        {
            std::cout << "Function: " << func->func_def->func_name << std::endl;
            std::cout << "  Instructions: " << info.instruction_count << std::endl;
            std::cout << "  Blocks: " << info.block_count << std::endl;
            std::cout << "  Calls made: " << info.call_count << std::endl;
            std::cout << "  Called: " << info.called_count << " times" << std::endl;
            std::cout << "  Recursive: " << (info.is_recursive ? "Yes" : "No") << std::endl;
            std::cout << "  Has loops: " << (info.has_loops ? "Yes" : "No") << std::endl;
            std::cout << "  Has pointer params: " << (info.has_pointer_params ? "Yes" : "No") << std::endl;
            std::cout << "  Complexity score: " << info.complexity_score << std::endl;
            std::cout << std::endl;
        }

        std::cout << "Call sites analyzed: " << all_call_sites.size() << std::endl;

        for (const auto& cs : all_call_sites)
        {
            std::cout << "  " << cs.caller->func_def->func_name << " -> " << cs.callee->func_def->func_name;
            if (cs.in_loop) std::cout << " (in loop, freq x" << cs.estimated_frequency << ")";
            if (cs.nesting_level > 1) std::cout << " (nesting " << cs.nesting_level << ")";
            if (cs.has_pointer_args) std::cout << " (ptr args)";
            std::cout << std::endl;
        }

        std::cout << "Processing order: ";
        for (size_t i = 0; i < topo_order.size(); ++i)
        {
            if (i > 0) std::cout << " -> ";
            std::cout << topo_order[i]->func_def->func_name;
        }
        std::cout << std::endl;
    }

    FunctionInlinePass::FunctionInlinePass(LLVMIR::IR* ir) : Pass(ir) { analyzer = new FunctionInlineAnalyzer(ir); }

    FunctionInlinePass::~FunctionInlinePass() { delete analyzer; }

    LLVMIR::IRFunction* FunctionInlinePass::findFunction(const std::string& name)
    {
        for (auto func : ir->functions)
            if (func->func_def->func_name == name) return func;

        return nullptr;
    }

    LLVMIR::Instruction* FunctionInlinePass::copyInstruction(LLVMIR::Instruction* inst,
        const std::map<int, int>& reg_map, const std::map<int, int>& label_map, LLVMIR::IRFunction* caller)
    {
        if (!inst) return nullptr;

        return inst->CloneWithMapping(reg_map, label_map);
    }

    void FunctionInlinePass::updateRegAndLabelMaps(LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee,
        LLVMIR::CallInst* call_inst, std::map<int, int>& reg_map, std::map<int, int>& label_map)
    {
        for (int i = call_inst->args.size(); i <= callee->max_reg; ++i) reg_map[i] = ++caller->max_reg;
        for (int i = 1; i <= callee->max_label; i++) label_map[i] = ++caller->max_label;
    }

    LLVMIR::IRBlock* FunctionInlinePass::findCallBlock(LLVMIR::IRFunction* func, LLVMIR::CallInst* call_inst)
    {
        for (auto block : func->blocks)
            for (auto inst : block->insts)
                if (inst == call_inst) return block;

        return nullptr;
    }

    void FunctionInlinePass::splitBlockAtCall(LLVMIR::IRFunction* caller, LLVMIR::IRBlock* call_block,
        LLVMIR::CallInst* call_inst, LLVMIR::IRBlock*& continue_block)
    {
        continue_block = caller->createBlock();

        auto call_iter = std::find(call_block->insts.begin(), call_block->insts.end(), call_inst);
        if (call_iter == call_block->insts.end()) return;

        auto next_iter = call_iter;
        ++next_iter;

        for (auto iter = next_iter; iter != call_block->insts.end(); ++iter)
        {
            (*iter)->block_id = continue_block->block_id;
            continue_block->insts.push_back(*iter);
        }

        call_block->insts.erase(call_iter, call_block->insts.end());

        updatePhiInstructions(caller, call_block, continue_block);
    }

    void FunctionInlinePass::updatePhiInstructions(
        LLVMIR::IRFunction* caller, LLVMIR::IRBlock* old_block, LLVMIR::IRBlock* new_block)
    {
        for (auto block : caller->blocks)
        {
            for (auto inst : block->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::PHI) continue;

                auto phi_inst = static_cast<LLVMIR::PhiInst*>(inst);

                for (auto& val_label : phi_inst->vals_for_labels)
                {
                    if (val_label.second->type != LLVMIR::OperandType::LABEL) continue;

                    auto label_op = static_cast<LLVMIR::LabelOperand*>(val_label.second);
                    if (label_op->label_num == old_block->block_id)
                        val_label.second = getLabelOperand(new_block->block_id);
                }
            }
        }
    }

    void FunctionInlinePass::inlineFunction(
        LLVMIR::IRFunction* caller, LLVMIR::IRFunction* callee, LLVMIR::CallInst* call_inst)
    {
        std::map<int, int> reg_map;
        std::map<int, int> label_map;

        LLVMIR::IRBlock* call_block = findCallBlock(caller, call_inst);
        if (!call_block) return;

        LLVMIR::IRBlock* continue_block = nullptr;
        splitBlockAtCall(caller, call_block, call_inst, continue_block);

        updateRegAndLabelMaps(caller, callee, call_inst, reg_map, label_map);

        label_map[0] = call_block->block_id;
        for (size_t i = 0; i < call_inst->args.size(); i++)
        {
            auto& arg = call_inst->args[i];
            if (arg.second->type == LLVMIR::OperandType::REG)
            {
                auto reg_op = static_cast<LLVMIR::RegOperand*>(arg.second);
                reg_map[i]  = reg_op->reg_num;
            }
            else
            {
                int new_reg = ++caller->max_reg;
                reg_map[i]  = new_reg;

                LLVMIR::Instruction* assign_inst = nullptr;
                if (arg.first == LLVMIR::DataType::I32)
                {
                    assign_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                        LLVMIR::DataType::I32,
                        arg.second,
                        getImmeI32Operand(0),
                        getRegOperand(new_reg));
                }
                else if (arg.first == LLVMIR::DataType::F32)
                {
                    assign_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::FADD,
                        LLVMIR::DataType::F32,
                        arg.second,
                        getImmeF32Operand(0.0f),
                        getRegOperand(new_reg));
                }

                if (assign_inst) call_block->insts.push_back(assign_inst);
            }
        }

        std::map<int, LLVMIR::IRBlock*> block_map;

        for (auto callee_block : callee->blocks)
        {
            LLVMIR::IRBlock* target_block = nullptr;

            if (callee_block->block_id == 0)
            {
                target_block = call_block;
                block_map[0] = call_block;
            }
            else
            {
                target_block = caller->createBlock();
                if (label_map.find(callee_block->block_id) != label_map.end())
                {
                    target_block->block_id = label_map[callee_block->block_id];
                }
                block_map[callee_block->block_id] = target_block;
            }

            for (auto inst : callee_block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ALLOCA)
                {
                    auto new_inst = copyInstruction(inst, reg_map, label_map, caller);
                    if (new_inst) caller->blocks[0]->insts.insert(caller->blocks[0]->insts.begin(), new_inst);
                }
                else if (inst->opcode == LLVMIR::IROpCode::RET)
                {
                    auto ret_inst = static_cast<LLVMIR::RetInst*>(inst);

                    if (ret_inst->ret && call_inst->res)
                    {
                        LLVMIR::Instruction* assign_inst = nullptr;
                        if (ret_inst->ret_type == LLVMIR::DataType::I32)
                        {
                            assign_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                                LLVMIR::DataType::I32,
                                copyOperand(ret_inst->ret, reg_map, label_map),
                                getImmeI32Operand(0),
                                call_inst->res);
                        }
                        else if (ret_inst->ret_type == LLVMIR::DataType::F32)
                        {
                            assign_inst = new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::FADD,
                                LLVMIR::DataType::F32,
                                copyOperand(ret_inst->ret, reg_map, label_map),
                                getImmeF32Operand(0.0f),
                                call_inst->res);
                        }

                        if (assign_inst) target_block->insts.push_back(assign_inst);
                    }

                    auto br_inst = new LLVMIR::BranchUncondInst(getLabelOperand(continue_block->block_id));
                    target_block->insts.push_back(br_inst);
                }
                else
                {
                    auto new_inst = copyInstruction(inst, reg_map, label_map, caller);
                    if (new_inst) target_block->insts.push_back(new_inst);
                }
            }
        }

        if (!call_block->insts.empty())
        {
            auto last_inst = call_block->insts.back();
            if (last_inst->opcode != LLVMIR::IROpCode::BR_UNCOND && last_inst->opcode != LLVMIR::IROpCode::BR_COND &&
                last_inst->opcode != LLVMIR::IROpCode::RET)
            {
                auto br_inst = new LLVMIR::BranchUncondInst(getLabelOperand(continue_block->block_id));
                call_block->insts.push_back(br_inst);
            }
        }
        else
        {
            auto br_inst = new LLVMIR::BranchUncondInst(getLabelOperand(continue_block->block_id));
            call_block->insts.push_back(br_inst);
        }
    }

    void FunctionInlinePass::Execute()
    {
        analyzer->analyze();

        // analyzer->printAnalysisReport();

        bool      changed        = true;
        int       iteration      = 0;
        const int MAX_ITERATIONS = 5;

        while (changed && iteration < MAX_ITERATIONS)
        {
            changed = false;
            ++iteration;

            auto processing_order = analyzer->getProcessingOrder();

            for (auto func : processing_order)
            {
                std::vector<LLVMIR::CallInst*> calls_to_inline;

                for (auto block : func->blocks)
                {
                    for (auto inst : block->insts)
                    {
                        if (inst->opcode != LLVMIR::IROpCode::CALL) continue;

                        auto call_inst = static_cast<LLVMIR::CallInst*>(inst);
                        auto callee    = findFunction(call_inst->func_name);

                        if (callee)
                        {
                            std::string reason = analyzer->getInlineReason(func, callee, call_inst);

                            if (analyzer->shouldInline(func, callee, call_inst))
                            {
                                // std::cout << "Accept inlining: " << callee->func_def->func_name << " in "
                                //           << func->func_def->func_name << " - " << reason << std::endl;
                                calls_to_inline.push_back(call_inst);
                            }
                            else
                            {
                                // std::cout << "Reject inlining: " << callee->func_def->func_name << " in "
                                //           << func->func_def->func_name << " - " << reason << std::endl;
                            }
                        }
                    }
                }

                for (auto call_inst : calls_to_inline)
                {
                    auto callee = findFunction(call_inst->func_name);
                    if (callee)
                    {
                        inlineFunction(func, callee, call_inst);
                        changed = true;
                    }
                }

                if (changed && calls_to_inline.size() > 0)
                {
                    analyzer->analyze();
                    break;
                }
            }
        }

        if (iteration >= MAX_ITERATIONS)
        {
            std::cout << "Function inlining stopped after " << MAX_ITERATIONS << " iterations" << std::endl;
        }
    }
}  // namespace Transform
