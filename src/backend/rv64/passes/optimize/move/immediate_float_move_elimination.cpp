#include <backend/rv64/passes/optimize/move/immediate_float_move_elimination.h>

namespace Backend::RV64::Passes::Optimize::Move
{

    ImmediateFloatMoveEliminationPass::ImmediateFloatMoveEliminationPass(std::vector<Function*>& functions)
        : functions_(functions)
    {}

    bool ImmediateFloatMoveEliminationPass::run()
    {
        for (auto& func : functions_) { eliminateImmeFMoveInst(func); }
        return true;
    }

    void ImmediateFloatMoveEliminationPass::eliminateImmeFMoveInst(Function* func)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                Instruction* base_inst = *it;
                if (base_inst->inst_type != InstType::MOVE) continue;

                MoveInst* move_inst = (MoveInst*)base_inst;
                if (move_inst->src->operand_type != OperandType::IMMEF32) continue;

                float    src     = ((ImmeF32Operand*)move_inst->src)->val;
                Register dst_reg = ((RegOperand*)move_inst->dst)->reg;

                delete move_inst;
                it               = block->insts.erase(it);
                Register tmp_reg = func->getNewReg(INT64);
                block->insts.insert(it, createMoveInst(INT64, tmp_reg, *(int*)&src));
                block->insts.insert(it, createR2Inst(RV64InstType::FMV_W_X, dst_reg, tmp_reg));
                --it;
            }
        }
    }

}  // namespace Backend::RV64::Passes::Optimize::Move
