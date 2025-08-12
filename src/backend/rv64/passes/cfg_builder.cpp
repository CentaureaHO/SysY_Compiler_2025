#include <backend/rv64/passes/cfg_builder.h>
#include <iostream>
#include <algorithm>

namespace Backend::RV64::Passes
{

    CFGBuilderPass::CFGBuilderPass(std::vector<Function*>& functions) : functions_(functions) {}

    bool CFGBuilderPass::run()
    {
        bool modified = false;
        for (auto* func : functions_)
        {
            if (buildCFGForFunction(func)) { modified = true; }
        }
        return modified;
    }

    bool CFGBuilderPass::buildCFGForFunction(Function* func)
    {
        if (func == nullptr || func->cfg == nullptr) return false;
        if (func->blocks.empty()) return false;

        CFG* cfg = func->cfg;

        clearCFGEdges(cfg);

        for (auto* block : func->blocks)
        {
            if (block != nullptr) { analyzeBlockSuccessors(block, cfg); }
        }

        addFallthroughEdges(func, cfg);

        setEntryAndExitBlocks(cfg);

        // std::cout << "CFG rebuilt for function " << func->name << " with " << cfg->blocks.size() << " blocks"
        //           << std::endl;

        return true;
    }

    void CFGBuilderPass::clearCFGEdges(CFG* cfg)
    {
        cfg->graph.clear();
        cfg->inv_graph.clear();

        cfg->graph.resize(cfg->max_label + 1);
        cfg->inv_graph.resize(cfg->max_label + 1);
    }

    void CFGBuilderPass::analyzeBlockSuccessors(Block* block, CFG* cfg)
    {
        if (block->insts.empty()) return;

        std::vector<int> targets = getBlockTargets(block);

        for (int target_id : targets)
        {
            if (cfg->blocks.find(target_id) != cfg->blocks.end()) { cfg->makeEdge(block->label_num, target_id); }
        }
    }

    void CFGBuilderPass::addFallthroughEdges(Function* func, CFG* cfg)
    {
        for (auto* block : func->blocks)
        {
            if (block == nullptr || block->insts.empty()) continue;

            auto* last_inst = block->insts.back();

            bool needs_fallthrough = false;

            if (isReturn(last_inst)) { needs_fallthrough = false; }
            else if (isUnconditionalBranch(last_inst)) { needs_fallthrough = false; }
            else if (isConditionalBranch(last_inst)) { needs_fallthrough = true; }
            else { needs_fallthrough = true; }

            if (needs_fallthrough)
            {
                Block* next_block = getNextBlockInLayout(func, block);
                if (next_block != nullptr) { cfg->makeEdge(block->label_num, next_block->label_num); }
            }
        }
    }

    Block* CFGBuilderPass::getNextBlockInLayout(Function* func, Block* current_block)
    {
        auto it = std::find(func->blocks.begin(), func->blocks.end(), current_block);
        if (it != func->blocks.end() && (it + 1) != func->blocks.end()) { return *(it + 1); }
        return nullptr;
    }

    std::vector<int> CFGBuilderPass::getBlockTargets(Block* block)
    {
        std::vector<int> targets;

        if (block->insts.empty()) return targets;

        auto* last_inst = block->insts.back();
        if (isReturn(last_inst)) { return targets; }

        if (hasConditionalBranch(block) && hasUnconditionalBranch(block))
        {
            auto [true_target, false_target] = analyzeConditionalJumpPattern(block);
            if (true_target >= 0) targets.push_back(true_target);
            if (false_target >= 0) targets.push_back(false_target);
        }
        else if (hasUnconditionalBranch(block))
        {
            int target = extractJumpTarget(block->insts.back());
            if (target >= 0) targets.push_back(target);
        }
        else if (isConditionalBranch(last_inst))
        {
            int target = extractJumpTarget(last_inst);
            if (target >= 0) targets.push_back(target);
        }

        return targets;
    }

    std::pair<int, int> CFGBuilderPass::analyzeConditionalJumpPattern(Block* block)
    {
        if (block->insts.size() < 2) return {-1, -1};

        auto* last_inst = block->insts.back();
        if (!isUnconditionalBranch(last_inst)) return {-1, -1};

        auto it = block->insts.end();
        --it;
        --it;
        auto* second_last_inst = *it;
        if (!isConditionalBranch(second_last_inst)) return {-1, -1};

        int true_target = extractJumpTarget(second_last_inst);

        int false_target = extractJumpTarget(last_inst);

        return {true_target, false_target};
    }

    bool CFGBuilderPass::hasConditionalBranch(Block* block)
    {
        if (block->insts.size() < 2) return false;

        auto it = block->insts.end();
        --it;
        --it;
        auto* second_last_inst = *it;
        return isConditionalBranch(second_last_inst);
    }

    bool CFGBuilderPass::hasUnconditionalBranch(Block* block)
    {
        if (block->insts.empty()) return false;

        auto* last_inst = block->insts.back();
        return isUnconditionalBranch(last_inst);
    }

    bool CFGBuilderPass::isConditionalBranch(Instruction* inst)
    {
        if (inst->inst_type != InstType::RV64) return false;

        auto* rv64_inst = static_cast<RV64Inst*>(inst);
        return (rv64_inst->op >= RV64InstType::BEQ && rv64_inst->op <= RV64InstType::BLEU);
    }

    bool CFGBuilderPass::isUnconditionalBranch(Instruction* inst)
    {
        if (inst->inst_type != InstType::RV64) return false;

        auto* rv64_inst = static_cast<RV64Inst*>(inst);
        return (rv64_inst->op == RV64InstType::JAL && rv64_inst->rd.reg_num == static_cast<int>(REG::x0));
    }

    bool CFGBuilderPass::isReturn(Instruction* inst)
    {
        if (inst->inst_type != InstType::RV64) return false;

        auto* rv64_inst = static_cast<RV64Inst*>(inst);
        return (rv64_inst->op == RV64InstType::JALR && rv64_inst->rd.reg_num == static_cast<int>(REG::x0) &&
                rv64_inst->rs1.reg_num == static_cast<int>(REG::x1));  // x1 is ra
    }

    int CFGBuilderPass::extractJumpTarget(Instruction* inst)
    {
        if (inst->inst_type != InstType::RV64) return -1;

        auto* rv64_inst = static_cast<RV64Inst*>(inst);

        if ((rv64_inst->op == RV64InstType::JAL ||
                (rv64_inst->op >= RV64InstType::BEQ && rv64_inst->op <= RV64InstType::BLEU)) &&
            rv64_inst->use_label)
        {
            return rv64_inst->label.jmp_label;
        }

        return -1;
    }

    void CFGBuilderPass::setEntryAndExitBlocks(CFG* cfg)
    {
        auto it = cfg->blocks.find(0);
        if (it != cfg->blocks.end()) { cfg->entry_block = it->second; }

        cfg->ret_block = nullptr;
        for (const auto& [id, block] : cfg->blocks)
        {
            if (block->insts.empty()) continue;

            auto* terminator = block->insts.back();
            if (isReturn(terminator))
            {
                cfg->ret_block = block;
                break;
            }
        }

        if (cfg->ret_block == nullptr)
        {
            for (const auto& [id, block] : cfg->blocks)
            {
                if (id < static_cast<int>(cfg->graph.size()) && cfg->graph[id].empty())
                {
                    cfg->ret_block = block;
                    break;
                }
            }
        }
    }

}  // namespace Backend::RV64::Passes
