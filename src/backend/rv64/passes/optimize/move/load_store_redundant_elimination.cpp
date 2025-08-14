#include <backend/rv64/passes/optimize/move/load_store_redundant_elimination.h>
#include <backend/rv64/insts.h>

namespace Backend::RV64::Passes::Optimize::Move
{

    LoadStoreRedundantEliminationPass::LoadStoreRedundantEliminationPass(std::vector<Function*>& functions)
        : functions_(functions)
    {}

    bool LoadStoreRedundantEliminationPass::run()
    {
        for (auto& func : functions_) { eliminateLoadStoreRedundant(func); }
        return true;
    }

    void LoadStoreRedundantEliminationPass::eliminateLoadStoreRedundant(Function* func)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                Instruction* base_inst = *it;
                if (base_inst->inst_type != InstType::RV64) continue;

                RV64Inst* rv64_inst = (RV64Inst*)base_inst;

                if (!isLoadInstruction(rv64_inst)) continue;

                Register load_target_reg = rv64_inst->rd;
                auto     next_it         = it;
                ++next_it;

                Register load_base_reg = rv64_inst->rs1;

                while (next_it != block->insts.end())
                {
                    Instruction* next_base_inst = *next_it;

                    assert(next_base_inst->inst_type == InstType::RV64);

                    RV64Inst* next_inst = (RV64Inst*)next_base_inst;

                    if (isRegisterWritten(next_inst, load_target_reg)) break;
                    if (isRegisterWritten(next_inst, load_base_reg)) break;
                    if (isConditionalBranchInstruction(next_inst) || isJalrInstruction(next_inst)) break;
                    if (isStoreInstruction(next_inst) && isMemoryLocationSame(rv64_inst, next_inst) &&
                        isStoringSameRegister(rv64_inst, next_inst))
                    {
                        Instruction::delInst(next_inst);
                        next_it = block->insts.erase(next_it);
                        continue;
                    }

                    ++next_it;
                }
            }
        }

        setCanSchedule();
    }

    bool LoadStoreRedundantEliminationPass::isLoadInstruction(RV64Inst* inst)
    {
        return inst->op == RV64InstType::LW || inst->op == RV64InstType::LD || inst->op == RV64InstType::FLW ||
               inst->op == RV64InstType::FLD;
    }

    bool LoadStoreRedundantEliminationPass::isStoreInstruction(RV64Inst* inst)
    {
        return inst->op == RV64InstType::SW || inst->op == RV64InstType::SD || inst->op == RV64InstType::FSW ||
               inst->op == RV64InstType::FSD;
    }

    bool LoadStoreRedundantEliminationPass::isBranchInstruction(RV64Inst* inst)
    {
        return inst->op == RV64InstType::BEQ || inst->op == RV64InstType::BNE || inst->op == RV64InstType::BLT ||
               inst->op == RV64InstType::BGE || inst->op == RV64InstType::BLTU || inst->op == RV64InstType::BGEU ||
               inst->op == RV64InstType::BGT || inst->op == RV64InstType::BLE || inst->op == RV64InstType::BGTU ||
               inst->op == RV64InstType::BLEU || inst->op == RV64InstType::JAL || inst->op == RV64InstType::JALR;
    }

    bool LoadStoreRedundantEliminationPass::isJalrInstruction(RV64Inst* inst) { return inst->op == RV64InstType::JALR; }

    bool LoadStoreRedundantEliminationPass::isConditionalBranchInstruction(RV64Inst* inst)
    {
        return inst->op == RV64InstType::BEQ || inst->op == RV64InstType::BNE || inst->op == RV64InstType::BLT ||
               inst->op == RV64InstType::BGE || inst->op == RV64InstType::BLTU || inst->op == RV64InstType::BGEU ||
               inst->op == RV64InstType::BGT || inst->op == RV64InstType::BLE || inst->op == RV64InstType::BGTU ||
               inst->op == RV64InstType::BLEU;
    }

    bool LoadStoreRedundantEliminationPass::isMemoryLocationSame(RV64Inst* load_inst, RV64Inst* store_inst)
    {
        if (!(load_inst->rs1 == store_inst->rs2)) return false;

        if (load_inst->use_label != store_inst->use_label) return false;

        if (load_inst->use_label)
        {
            return load_inst->label.name == store_inst->label.name &&
                   load_inst->label.is_data_addr == store_inst->label.is_data_addr &&
                   load_inst->label.is_hi == store_inst->label.is_hi &&
                   load_inst->label.jmp_label == store_inst->label.jmp_label &&
                   load_inst->label.seq_label == store_inst->label.seq_label;
        }
        else { return load_inst->imme == store_inst->imme; }
    }

    bool LoadStoreRedundantEliminationPass::isStoringSameRegister(RV64Inst* load_inst, RV64Inst* store_inst)
    {
        return load_inst->rd == store_inst->rs1;
    }

    bool LoadStoreRedundantEliminationPass::isRegisterWritten(RV64Inst* inst, Register target_reg)
    {
        auto write_regs = inst->getWriteRegs();
        for (auto* reg : write_regs)
        {
            if (*reg == target_reg) return true;
        }
        return false;
    }

    bool LoadStoreRedundantEliminationPass::isRegisterUsed(RV64Inst* inst, Register target_reg)
    {
        auto read_regs = inst->getReadRegs();
        for (auto* reg : read_regs)
        {
            if (*reg == target_reg) return true;
        }
        return false;
    }

}  // namespace Backend::RV64::Passes::Optimize::Move
