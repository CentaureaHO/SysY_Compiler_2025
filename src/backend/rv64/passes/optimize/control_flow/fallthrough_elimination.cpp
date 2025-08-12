#include <backend/rv64/passes/optimize/control_flow/fallthrough_elimination.h>
#include <iostream>
#include <algorithm>

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    FallthroughEliminationPass::FallthroughEliminationPass(std::vector<Function*>& functions)
        : functions_(functions), eliminated_jumps_count_(0)
    {}

    bool FallthroughEliminationPass::run()
    {
        bool modified           = false;
        eliminated_jumps_count_ = 0;

        for (auto* func : functions_)
        {
            if (optimizeFunction(func)) { modified = true; }
        }

        if (eliminated_jumps_count_ > 0)
        {
            // std::cout << "Eliminated " << eliminated_jumps_count_ << " fallthrough jumps" << std::endl;
        }

        return modified;
    }

    bool FallthroughEliminationPass::optimizeFunction(Function* func)
    {
        if (func == nullptr || func->blocks.empty()) return false;

        return eliminateFallthroughJumps(func);
    }

    bool FallthroughEliminationPass::eliminateFallthroughJumps(Function* func)
    {
        bool modified = false;

        for (size_t i = 0; i < func->blocks.size() - 1; ++i)
        {
            Block* current_block = func->blocks[i];
            Block* next_block    = func->blocks[i + 1];

            if (current_block == nullptr || next_block == nullptr) continue;

            if (isFallthroughJump(current_block, next_block))
            {
                bool was_conditional_pattern = hasConditionalJumpPattern(current_block);

                removeLastInstruction(current_block);
                eliminated_jumps_count_++;
                modified = true;

                // std::cout << "Eliminated fallthrough jump from block " << current_block->label_num << " to block "
                //           << next_block->label_num;

                // if (was_conditional_pattern) std::cout << " (false branch of conditional jump)";

                // std::cout << std::endl;
            }
        }

        return modified;
    }

    bool FallthroughEliminationPass::isFallthroughJump(Block* block, Block* next_block)
    {
        if (block->insts.empty()) return false;

        auto* last_inst = block->insts.back();

        if (!isUnconditionalJump(last_inst)) return false;

        int target = getJumpTarget(last_inst);
        return (target == next_block->label_num);
    }

    bool FallthroughEliminationPass::isUnconditionalJump(Instruction* inst)
    {
        if (inst->inst_type != InstType::RV64) return false;

        auto* rv64_inst = static_cast<RV64Inst*>(inst);
        return (rv64_inst->op == RV64InstType::JAL && rv64_inst->rd.reg_num == static_cast<int>(REG::x0));
    }

    bool FallthroughEliminationPass::isConditionalBranch(Instruction* inst)
    {
        if (inst->inst_type != InstType::RV64) return false;

        auto* rv64_inst = static_cast<RV64Inst*>(inst);
        return (rv64_inst->op >= RV64InstType::BEQ && rv64_inst->op <= RV64InstType::BLEU);
    }

    int FallthroughEliminationPass::getJumpTarget(Instruction* inst)
    {
        if (inst->inst_type != InstType::RV64) return -1;

        auto* rv64_inst = static_cast<RV64Inst*>(inst);

        if (rv64_inst->use_label) { return rv64_inst->label.jmp_label; }

        return -1;
    }

    void FallthroughEliminationPass::removeLastInstruction(Block* block)
    {
        if (!block->insts.empty())
        {
            auto* last_inst = block->insts.back();
            block->insts.pop_back();
            Instruction::delInst(last_inst);
        }
    }

    bool FallthroughEliminationPass::hasConditionalJumpPattern(Block* block)
    {
        if (block->insts.size() < 2) return false;

        auto* last_inst = block->insts.back();
        if (!isUnconditionalJump(last_inst)) return false;

        auto it = block->insts.end();
        --it;
        --it;
        auto* second_last_inst = *it;

        return isConditionalBranch(second_last_inst);
    }

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow
