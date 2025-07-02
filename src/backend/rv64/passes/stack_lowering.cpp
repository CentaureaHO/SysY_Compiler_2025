#include <backend/rv64/passes/stack_lowering.h>
#include <vector>
#include <set>

namespace Backend::RV64::Passes
{

    StackLoweringPass::StackLoweringPass(std::vector<Function*>& functions) : functions_(functions) {}

    bool StackLoweringPass::run()
    {
        for (auto& func : functions_) { lowerStack(func); }
        return true;
    }

    void StackLoweringPass::gatherRegsToSave(Function* func, MAT2(int) & reg_def_blocks, MAT2(int) & reg_access_blocks)
    {
        reg_def_blocks.clear();
        reg_access_blocks.clear();
        reg_def_blocks.resize(64);
        reg_access_blocks.resize(64);

        bool need_save[64];

        for (auto& block : func->blocks)
        {
#define X(name, alias, saver)                   \
    if (saver == 1)                             \
        need_save[preg_##alias.reg_num] = true; \
    else                                        \
        need_save[preg_##alias.reg_num] = false;
            RV64_REGS
#undef X
            need_save[preg_ra.reg_num] = true;

            for (auto& inst : block->insts)
            {
                for (auto& reg : inst->getWriteRegs())
                {
                    assert(reg->is_virtual == false);
                    if (need_save[reg->reg_num])
                    {
                        need_save[reg->reg_num] = false;
                        reg_def_blocks[reg->reg_num].push_back(block->label_num);
                        reg_access_blocks[reg->reg_num].push_back(block->label_num);
                    }
                }
                for (auto& reg : inst->getReadRegs())
                {
                    assert(reg->is_virtual == false);
                    if (need_save[reg->reg_num])
                    {
                        need_save[reg->reg_num] = false;
                        reg_access_blocks[reg->reg_num].push_back(block->label_num);
                    }
                }
            }

            if (func->has_stack_param)
            {
                reg_def_blocks[preg_fp.reg_num].push_back(block->label_num);
                reg_access_blocks[preg_fp.reg_num].push_back(block->label_num);
            }
        }
    }

    void StackLoweringPass::lowerStack(Function* func)
    {
        std::vector<int> sd_blocks(64, 0);
        std::vector<int> ld_blocks(64, 0);
        std::vector<int> restore_offset(64, 0);
        int              saveregnum = 0, cur_restore_offset = 0;

        MAT2(int) reg_def_blocks, reg_access_blocks;
        gatherRegsToSave(func, reg_def_blocks, reg_access_blocks);

        for (size_t i = 0; i < reg_def_blocks.size(); ++i)
        {
            if (reg_access_blocks[i].empty()) continue;
            ++saveregnum;
        }

        func->stack_size += saveregnum * 8;

        for (auto& block : func->blocks)
        {
            if (block->label_num == 0)
            {
                if (func->stack_size <= 2048)
                    block->insts.push_front(createIInst(RV64InstType::ADDI, preg_sp, preg_sp, -func->stack_size));
                else
                {
                    block->insts.push_front(createRInst(RV64InstType::SUB, preg_sp, preg_sp, preg_t0));
                    block->insts.push_front(createUInst(RV64InstType::LI, preg_t0, func->stack_size));
                }

                if (func->has_stack_param)
                    block->insts.push_front(createRInst(RV64InstType::ADD, preg_fp, preg_sp, preg_x0));

                int offset = 0;
                for (int i = 0; i < 64; ++i)
                {
                    if (reg_def_blocks[i].empty()) continue;

                    offset -= 8;
                    if (static_cast<int>(REG::x0) <= i && i <= static_cast<int>(REG::x31))
                    {
                        block->insts.push_front(createSInst(RV64InstType::SD, getPhyReg(i), preg_sp, offset));
                    }
                    else { block->insts.push_front(createSInst(RV64InstType::FSD, getPhyReg(i), preg_sp, offset)); }
                }
            }

            if (block->insts.empty()) continue;
            Instruction* last_inst = block->insts.back();

            RV64Inst* rv64_inst = (RV64Inst*)last_inst;
            if (rv64_inst->op != RV64InstType::JALR) continue;
            if (!(rv64_inst->rd == preg_x0)) continue;
            if (!(rv64_inst->rs1 == preg_ra)) continue;

            block->insts.pop_back();
            if (func->stack_size <= 2047)
                block->insts.push_back(createIInst(RV64InstType::ADDI, preg_sp, preg_sp, func->stack_size));
            else
            {
                block->insts.push_back(createUInst(RV64InstType::LI, preg_t0, func->stack_size));
                block->insts.push_back(createRInst(RV64InstType::ADD, preg_sp, preg_sp, preg_t0));
            }

            int offset = 0;
            for (int i = 0; i < 32; ++i)
            {
                if (reg_def_blocks[i].empty()) continue;
                offset -= 8;
                block->insts.push_back(createIInst(RV64InstType::LD, getPhyReg(i), preg_sp, offset));
            }
            for (int i = 32; i < 64; ++i)
            {
                if (reg_def_blocks[i].empty()) continue;
                offset -= 8;
                block->insts.push_back(createIInst(RV64InstType::FLD, getPhyReg(i), preg_sp, offset));
            }

            block->insts.push_back(rv64_inst);
        }
    }

}  // namespace Backend::RV64::Passes
