#include <backend/rv64/passes/optimize/redundant_arithmetic_elimination.h>
#include <backend/rv64/insts.h>

namespace Backend::RV64::Passes::Optimize
{

    RedundantArithmeticEliminationPass::RedundantArithmeticEliminationPass(std::vector<Function*>& functions)
        : functions_(functions)
    {}

    bool RedundantArithmeticEliminationPass::run()
    {
        for (auto& func : functions_) { eliminateRedundantArithmetic(func); }
        return true;
    }

    void RedundantArithmeticEliminationPass::eliminateRedundantArithmetic(Function* func)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                Instruction* base_inst = *it;
                if (base_inst->inst_type != InstType::RV64) continue;

                RV64Inst* rv64_inst = (RV64Inst*)base_inst;

                if (isRedundantInstruction(rv64_inst))
                {
                    Instruction::delInst(rv64_inst);
                    it = block->insts.erase(it);
                    --it;
                    continue;
                }
            }
        }

        setCanSchedule();
    }

    bool RedundantArithmeticEliminationPass::isRedundantInstruction(RV64Inst* inst)
    {
        if (inst->op == RV64InstType::ADD || inst->op == RV64InstType::ADDW)
        {
            if (inst->rs2 == preg_x0 && inst->rd == inst->rs1) { return true; }
            if (inst->rs1 == preg_x0 && inst->rd == inst->rs2) { return true; }
        }

        if (inst->op == RV64InstType::ADDI || inst->op == RV64InstType::ADDIW)
        {
            if (!inst->use_label && inst->imme == 0 && inst->rd == inst->rs1) return true;
        }

        if (inst->op == RV64InstType::FADD_S)
        {
            // TODO
        }

        return false;
    }

}  // namespace Backend::RV64::Passes::Optimize
