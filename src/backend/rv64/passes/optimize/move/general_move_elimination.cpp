#include <backend/rv64/passes/optimize/move/general_move_elimination.h>

namespace Backend::RV64::Passes::Optimize::Move
{

    GeneralMoveEliminationPass::GeneralMoveEliminationPass(std::vector<Function*>& functions) : functions_(functions) {}

    bool GeneralMoveEliminationPass::run()
    {
        for (auto& func : functions_) { eliminateMoveInst(func); }
        return true;
    }

    void GeneralMoveEliminationPass::eliminateMoveInst(Function* func)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                setCanSchedule();

                Instruction* base_inst = *it;
                if (base_inst->inst_type != InstType::MOVE) continue;

                if (!base_inst->schedule_flag) setNoSchedule();

                MoveInst* move_inst = (MoveInst*)base_inst;
                assert(move_inst->src->operand_type == OperandType::REG);

                Register     dst_reg   = ((RegOperand*)move_inst->dst)->reg;
                Register     src_reg   = ((RegOperand*)move_inst->src)->reg;
                unsigned int data_type = src_reg.data_type->data_type;

                Instruction::delInst(move_inst);

                if (dst_reg == src_reg)
                {
                    it = block->insts.erase(it);
                    --it;
                    continue;
                }

                if (data_type == DataType::INT)
                {
                    it = block->insts.erase(it);
                    block->insts.insert(it, createRInst(RV64InstType::ADD, dst_reg, preg_x0, src_reg));
                    --it;
                }
                else if (data_type == DataType::FLOAT)
                {
                    it = block->insts.erase(it);
                    // block->insts.insert(it, createRInst(RV64InstType::FADD_S, dst_reg, preg_fa0, src_reg));
                    block->insts.insert(it, createR2Inst(RV64InstType::FMV_S, dst_reg, src_reg));
                    --it;
                }
                else
                    assert(false);
            }
        }

        setCanSchedule();
    }

}  // namespace Backend::RV64::Passes::Optimize::Move
