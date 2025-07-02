#include <backend/rv64/passes/optimize/move/immediate_integer_move_elimination.h>

namespace Backend::RV64::Passes::Optimize::Move
{

    ImmediateIntegerMoveEliminationPass::ImmediateIntegerMoveEliminationPass(std::vector<Function*>& functions)
        : functions_(functions)
    {}

    bool ImmediateIntegerMoveEliminationPass::run()
    {
        for (auto& func : functions_) { eliminateImmeIMoveInst(func); }
        return true;
    }

    void ImmediateIntegerMoveEliminationPass::eliminateImmeIMoveInst(Function* func)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                Instruction* base_inst = *it;
                if (base_inst->inst_type != InstType::MOVE) continue;

                MoveInst* move_inst = (MoveInst*)base_inst;
                if (move_inst->src->operand_type != OperandType::IMMEI32) continue;

                ImmeI32Operand* imme_op = (ImmeI32Operand*)move_inst->src;
                RegOperand*     dst_op  = (RegOperand*)move_inst->dst;
                assert(dst_op != nullptr);

                Register dst_reg = dst_op->reg;

                if (imme_op == nullptr)
                {
                    delete move_inst;
                    it = block->insts.erase(it);
                    block->insts.insert(it, createMoveInst(INT64, dst_reg, preg_x0));
                    --it;
                    continue;
                }

                int imme_val = imme_op->val;
                delete move_inst;

                if (imme_val >= -2048 && imme_val <= 2047)
                {
                    it = block->insts.erase(it);
                    block->insts.insert(it, createIInst(RV64InstType::ADDI, dst_reg, preg_x0, imme_val));
                    --it;
                }
                else if ((imme_val & 0xfff) == 0)
                {
                    it = block->insts.erase(it);
                    block->insts.insert(it, createUInst(RV64InstType::LUI, dst_reg, ((unsigned int)imme_val) >> 12));
                    --it;
                }
                else
                {
                    it = block->insts.erase(it);
                    block->insts.insert(
                        it, createUInst(RV64InstType::LUI, dst_reg, ((unsigned int)imme_val + 0x800) >> 12));
                    assert(((imme_val << 20) >> 20) <= 2047);
                    block->insts.insert(it, createIInst(RV64InstType::ADDI, dst_reg, dst_reg, (imme_val << 20) >> 20));
                    --it;
                }
            }
        }
    }

}  // namespace Backend::RV64::Passes::Optimize::Move
