#include <backend/rv64/passes/frame_lowering.h>

namespace Backend::RV64::Passes
{

    FrameLoweringPass::FrameLoweringPass(std::vector<Function*>& functions) : functions_(functions) {}

    bool FrameLoweringPass::run()
    {
        for (auto& func : functions_) { lowerFrame(func); }
        return true;
    }

    void FrameLoweringPass::lowerFrame(Function* func)
    {
        for (auto& block : func->blocks)
        {
            if (block->label_num != 0) continue;

            Register para_base = func->getNewReg(INT64);
            int      i32_cnt = 0, f32_cnt = 0, para_offset = 0;

            for (auto& param : func->params)
            {
                if (param.data_type->data_type == DataType::INT)
                {
                    if (i32_cnt < 8)
                    {
                        block->insts.push_front(createMoveInst(INT64, param, getPhyReg(preg_a0.reg_num + i32_cnt)));
                    }
                    else
                    {
                        if (para_offset >= -2048 && para_offset <= 2047)
                        {
                            block->insts.push_front(createIInst(RV64InstType::LD, param, preg_fp, para_offset));
                        }
                        else
                        {
                            Register offset_reg = func->getNewReg(INT64);
                            block->insts.push_front(createIInst(RV64InstType::LD, param, offset_reg, 0));
                            block->insts.push_front(createRInst(RV64InstType::ADD, offset_reg, preg_fp, offset_reg));
                            block->insts.push_front(createMoveInst(INT64, offset_reg, para_offset));
                        }
                        para_offset += 8;
                    }

                    ++i32_cnt;
                }
                else if (param.data_type->data_type == DataType::FLOAT)
                {
                    if (f32_cnt < 8)
                        block->insts.push_front(createMoveInst(FLOAT64, param, getPhyReg(preg_fa0.reg_num + f32_cnt)));
                    else
                    {
                        if (para_offset >= -2048 && para_offset <= 2047)
                        {
                            block->insts.push_front(createIInst(RV64InstType::FLD, param, preg_fp, para_offset));
                        }
                        else
                        {
                            Register offset_reg = func->getNewReg(INT64);
                            block->insts.push_front(createIInst(RV64InstType::FLD, param, offset_reg, 0));
                            block->insts.push_front(createRInst(RV64InstType::ADD, offset_reg, preg_fp, offset_reg));
                            block->insts.push_front(createMoveInst(INT64, offset_reg, para_offset));
                        }
                        para_offset += 8;
                    }

                    ++f32_cnt;
                }
                else
                    assert(false);
            }

            if (para_offset != 0)
            {
                func->has_stack_param = true;
                block->insts.push_front(createMoveInst(INT64, para_base, preg_fp));
            }
        }
    }

}  // namespace Backend::RV64::Passes
