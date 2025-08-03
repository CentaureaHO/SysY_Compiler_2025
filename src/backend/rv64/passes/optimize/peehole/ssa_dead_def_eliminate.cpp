#include <backend/rv64/passes/optimize/peehole/ssa_dead_def_eliminate.h>
#include <backend/rv64/rv64_block.h>

namespace Backend::RV64::Passes::Optimize::Peehole
{

    SSADeadDefEliminatePass::SSADeadDefEliminatePass(std::vector<Function*>& functions) : functions_(functions) {}

    bool SSADeadDefEliminatePass::run()
    {
        for (auto& func : functions_) { eliminateFunction(func); }
        return true;
    }

    void SSADeadDefEliminatePass::eliminateFunction(Function* func)
    {
        std::map<int, int> vreg_refcnt;

        countRegisterReferences(func, vreg_refcnt);
        removeDeadDefinitions(func, vreg_refcnt);
    }

    void SSADeadDefEliminatePass::countRegisterReferences(Function* func, std::map<int, int>& vreg_refcnt)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto& inst : block->insts)
            {
                auto read_regs = inst->getReadRegs();
                for (auto reg : read_regs)
                {
                    if (reg->is_virtual) { vreg_refcnt[reg->reg_num] = vreg_refcnt[reg->reg_num] + 1; }
                }
            }
        }
    }

    void SSADeadDefEliminatePass::removeDeadDefinitions(Function* func, const std::map<int, int>& vreg_refcnt)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end();)
            {
                auto inst       = *it;
                auto write_regs = inst->getWriteRegs();

                if (write_regs.size() == 1)
                {
                    auto rd = write_regs[0];
                    if (rd->is_virtual)
                    {
                        auto refcnt_it = vreg_refcnt.find(rd->reg_num);
                        int  refcnt    = (refcnt_it != vreg_refcnt.end()) ? refcnt_it->second : 0;

                        if (refcnt == 0)
                        {
                            it = block->insts.erase(it);
                            continue;
                        }
                    }
                }
                ++it;
            }
        }
    }

}  // namespace Backend::RV64::Passes::Optimize::Peehole
