#include <backend/rv64/passes/optimize/instruction_schedule.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>

namespace Backend::RV64::Passes::Optimize
{
    const std::vector<RV64InstType> InstructionSchedulePass::MemOp = {RV64InstType::LW,
        RV64InstType::LD,
        RV64InstType::FLW,
        RV64InstType::FLD,
        RV64InstType::SW,
        RV64InstType::SD,
        RV64InstType::FSW,
        RV64InstType::FSD};

    const std::vector<RV64InstType> InstructionSchedulePass::BranchOp = {RV64InstType::BEQ,
        RV64InstType::BNE,
        RV64InstType::BLT,
        RV64InstType::BGE,
        RV64InstType::BLTU,
        RV64InstType::BGEU,
        RV64InstType::BGT,
        RV64InstType::BLE,
        RV64InstType::BGTU,
        RV64InstType::BLEU};

    const std::vector<RV64InstType> InstructionSchedulePass::FaluOp = {RV64InstType::FMV_W_X,
        RV64InstType::FMV_X_W,
        RV64InstType::FCVT_S_W,
        RV64InstType::FCVT_W_S,
        RV64InstType::FADD_S,
        RV64InstType::FSUB_S,
        RV64InstType::FMUL_S,
        RV64InstType::FDIV_S,
        RV64InstType::FMADD_S,
        RV64InstType::FMSUB_S,
        RV64InstType::FNMADD_S,
        RV64InstType::FNMSUB_S,
        RV64InstType::FEQ_S,
        RV64InstType::FLT_S,
        RV64InstType::FLE_S,
        RV64InstType::FNEG_S,
        RV64InstType::FMV_S,
        RV64InstType::FMV_D};

    const std::vector<RV64InstType> InstructionSchedulePass::MulDivOp = {RV64InstType::MUL,
        RV64InstType::MULW,
        RV64InstType::DIV,
        RV64InstType::DIVW,
        RV64InstType::REM,
        RV64InstType::REMW};

    int ExecState::getInstLatency(Instruction* ins)
    {
        if (ins->inst_type == InstType::RV64)
        {
            auto rv64_ins = static_cast<RV64Inst*>(ins);
            return opInfoTable[rv64_ins->op].latency;
        }
        return 1;
    }

    ExecState::FuncUnitType ExecState::GetFuncUnitType(RV64Inst* ins)
    {
        for (const auto& op : InstructionSchedulePass::MemOp)
        {
            if (ins->op == op) return MEM_UNIT;
        }
        for (const auto& op : InstructionSchedulePass::BranchOp)
        {
            if (ins->op == op) return BRANCH_UNIT;
        }
        for (const auto& op : InstructionSchedulePass::FaluOp)
        {
            if (ins->op == op) return FP_UNIT;
        }

        return INT_UNIT;
    }

    int ExecState::GetMaxIssueWidth(FuncUnitType unit_type)
    {
        switch (unit_type)
        {
            case MEM_UNIT: return TARGET_MEM_ISSUE_WIDTH;
            case INT_UNIT: return TARGET_INT_ISSUE_WIDTH;
            case FP_UNIT: return TARGET_FP_ISSUE_WIDTH;
            case BRANCH_UNIT: return TARGET_BRANCH_ISSUE_WIDTH;
            default: return 0;
        }
    }

    bool ExecState::HasFreeSlot(FuncUnitType unit_type)
    {
        switch (unit_type)
        {
            case MEM_UNIT: return mem_units_busy < GetMaxIssueWidth(MEM_UNIT);
            case INT_UNIT: return int_units_busy < GetMaxIssueWidth(INT_UNIT);
            case FP_UNIT: return fp_units_busy < GetMaxIssueWidth(FP_UNIT);
            case BRANCH_UNIT: return branch_units_busy < GetMaxIssueWidth(BRANCH_UNIT);
            default: return false;
        }
    }

    bool ExecState::CanIssue(Instruction* ins)
    {
        if (full()) return false;

        if (ins->inst_type != InstType::RV64) { return true; }

        auto         riscv_ins = static_cast<RV64Inst*>(ins);
        FuncUnitType unit_type = GetFuncUnitType(riscv_ins);
        return HasFreeSlot(unit_type);
    }

    bool ExecState::Issue(Instruction* ins)
    {
        if (!CanIssue(ins)) return false;

        inflight[ins] = getInstLatency(ins);

        if (ins->inst_type == InstType::RV64)
        {
            auto         riscv_ins = static_cast<RV64Inst*>(ins);
            FuncUnitType unit_type = GetFuncUnitType(riscv_ins);

            switch (unit_type)
            {
                case MEM_UNIT: mem_units_busy++; break;
                case INT_UNIT: int_units_busy++; break;
                case FP_UNIT: fp_units_busy++; break;
                case BRANCH_UNIT: branch_units_busy++; break;
            }
        }

        return true;
    }

    bool ExecState::Retire(Instruction* ins)
    {
        Assert(inflight.find(ins) != inflight.end() && inflight[ins] == 0);
        inflight.erase(ins);

        if (ins->inst_type == InstType::RV64)
        {
            auto         riscv_ins = static_cast<RV64Inst*>(ins);
            FuncUnitType unit_type = GetFuncUnitType(riscv_ins);

            switch (unit_type)
            {
                case MEM_UNIT:
                    mem_units_busy--;
                    Assert(mem_units_busy >= 0);
                    break;
                case INT_UNIT:
                    int_units_busy--;
                    Assert(int_units_busy >= 0);
                    break;
                case FP_UNIT:
                    fp_units_busy--;
                    Assert(fp_units_busy >= 0);
                    break;
                case BRANCH_UNIT:
                    branch_units_busy--;
                    Assert(branch_units_busy >= 0);
                    break;
            }
        }

        return true;
    }

    bool InstructionSchedulePass::run()
    {
        bool changed = false;
        for (auto* func : functions_)
        {
            ExecuteInFunction(func);
            changed = true;
        }
        return changed;
    }

    void InstructionSchedulePass::ExecuteInFunction(Backend::RV64::Function* func)
    {
        current_func = func;
        liveness     = new Liveness(current_func);
        for (auto block : func->blocks)
        {
            cur_block = block;
            ExecuteInBlock();
        }
        delete liveness;
    }

    void InstructionSchedulePass::ExecuteInBlock()
    {
        std::vector<Instruction*> schedule_batch;
        std::vector<Instruction*> result;

        std::map<Register, int> kill_countdown;
        ActiveRegSet            active_set;

        auto out_set = liveness->GetOUT(cur_block->label_num);
        for (size_t i = 0; i < out_set.size(); ++i)
        {
            if (out_set[i])
            {
                Register reg        = liveness->reverse_mapping[i];
                kill_countdown[reg] = 1;
            }
        }

        auto in_set = liveness->GetIN(cur_block->label_num);
        for (size_t i = 0; i < in_set.size(); ++i)
        {
            if (in_set[i])
            {
                Register reg = liveness->reverse_mapping[i];
                active_set.add(reg);
            }
        }

        for (auto ins : cur_block->insts)
        {
            for (auto reg : ins->getReadRegs()) { kill_countdown[*reg] = kill_countdown[*reg] + 1; }
        }

        for (auto ins : cur_block->insts)
        {
            if (ins->inst_type == InstType::PHI)
            {
                for (auto reg : ins->getWriteRegs()) { active_set.add(*reg); }
            }
            if (ins->schedule_flag) { schedule_batch.push_back(ins); }
            else
            {
                if (!schedule_batch.empty())
                {
                    std::vector<Instruction*> schedule_result;
                    ExecuteInList(schedule_batch, schedule_result, active_set, kill_countdown);
                    for (auto scheduled_ins : schedule_result) { result.push_back(scheduled_ins); }
                    schedule_batch.clear();
                }
                result.push_back(ins);
            }
        }

        if (!schedule_batch.empty())
        {
            std::vector<Instruction*> schedule_result;
            ExecuteInList(schedule_batch, schedule_result, active_set, kill_countdown);
            for (auto scheduled_ins : schedule_result) { result.push_back(scheduled_ins); }
            schedule_batch.clear();
        }

        cur_block->insts.clear();
        for (auto ins : result) { cur_block->insts.push_back(ins); }
    }

    void InstructionSchedulePass::ExecuteInList(std::vector<Instruction*>& list, std::vector<Instruction*>& res,
        ActiveRegSet& active_set, std::map<Register, int>& kill_countdown)
    {
        res.clear();
        std::map<Instruction*, std::vector<Instruction*>> data_pre_graph;
        std::map<Instruction*, int>                       in_degree;
        std::map<Instruction*, std::vector<Instruction*>> data_dep_graph;
        std::map<Register, Instruction*>                  last_def_ins;
        Instruction*                                      last_store = nullptr;
        std::vector<Instruction*>                         last_loads;

        std::vector<Instruction*>   ready;
        std::map<Instruction*, int> ins_prio;

        std::set<InsPrioEntry> ready_set;

        for (auto ins : list)
        {
            if (ins->inst_type == InstType::RV64)
            {
                auto riscv_ins = static_cast<RV64Inst*>(ins);
                if (riscv_ins->op == RV64InstType::LW || riscv_ins->op == RV64InstType::LD ||
                    riscv_ins->op == RV64InstType::FLW || riscv_ins->op == RV64InstType::FLD)
                {
                    if (last_store != nullptr)
                    {
                        data_pre_graph[last_store].push_back(ins);
                        in_degree[ins] = in_degree[ins] + 1;
                        data_dep_graph[ins].push_back(last_store);
                    }
                    last_loads.push_back(ins);
                }
                if (riscv_ins->op == RV64InstType::SW || riscv_ins->op == RV64InstType::SD ||
                    riscv_ins->op == RV64InstType::FSW || riscv_ins->op == RV64InstType::FSD)
                {
                    if (!last_loads.empty())
                    {
                        for (auto load : last_loads)
                        {
                            data_pre_graph[load].push_back(ins);
                            in_degree[ins] = in_degree[ins] + 1;
                            data_dep_graph[ins].push_back(load);
                        }
                        last_loads.clear();
                    }
                    else if (last_store != nullptr)
                    {
                        data_pre_graph[last_store].push_back(ins);
                        in_degree[ins] = in_degree[ins] + 1;
                        data_dep_graph[ins].push_back(last_store);
                    }
                    last_store = ins;
                }
            }

            for (auto reg : ins->getReadRegs())
            {
                if (last_def_ins.find(*reg) != last_def_ins.end())
                {
                    data_pre_graph[last_def_ins[*reg]].push_back(ins);
                    in_degree[ins] = in_degree[ins] + 1;
                    data_dep_graph[ins].push_back(last_def_ins[*reg]);
                }
            }
            for (auto reg : ins->getWriteRegs()) { last_def_ins[*reg] = ins; }
            if (in_degree[ins] == 0) { ready.push_back(ins); }
        }

        for (auto it = list.rbegin(); it != list.rend(); ++it)
        {
            auto ins             = *it;
            int  longest_latency = 0;
            for (auto succ : data_pre_graph[ins])
            {
                if (longest_latency < ins_prio[succ]) { longest_latency = ins_prio[succ]; }
            }
            int latency = 1;
            if (ins->inst_type == InstType::RV64)
            {
                auto riscv_ins = static_cast<RV64Inst*>(ins);
                latency        = opInfoTable[riscv_ins->op].latency;
            }
            ins_prio[ins] = longest_latency + latency;
        }

        for (auto ins : ready) { ready_set.insert(InsPrioEntry(ins, ins_prio[ins])); }

        ExecState state;

        while (!ready_set.empty() || !state.empty())
        {
            std::vector<InsPrioEntry> issued_this_cycle;

            for (auto it = ready_set.begin(); it != ready_set.end() && !state.full();)
            {
                auto entry = *it;
                if (!active_set.judgeSpill(entry.ins, kill_countdown))
                {
                    if (state.CanIssue(entry.ins))
                    {
                        state.Issue(entry.ins);
                        issued_this_cycle.push_back(entry);
                        it = ready_set.erase(it);
                        active_set.addins(entry.ins, kill_countdown);
                    }
                    else { ++it; }
                }
                else { ++it; }
            }

            if (!state.full())
            {
                for (auto it = ready_set.begin(); it != ready_set.end() && !state.full();)
                {
                    auto entry = *it;
                    if (state.CanIssue(entry.ins))
                    {
                        state.Issue(entry.ins);
                        issued_this_cycle.push_back(entry);
                        it = ready_set.erase(it);
                        active_set.addins(entry.ins, kill_countdown);
                    }
                    else { ++it; }
                }
            }

            for (const auto& entry : issued_this_cycle) { res.push_back(entry.ins); }

            auto retirelist = state.nextCycle();
            for (auto ins : retirelist)
            {
                state.Retire(ins);
                for (auto succ : data_pre_graph[ins])
                {
                    in_degree[succ] = in_degree[succ] - 1;
                    Assert(in_degree[succ] >= 0);
                    if (in_degree[succ] == 0) { ready_set.insert(InsPrioEntry(succ, ins_prio[succ])); }
                }
            }
        }

        Assert(list.size() == res.size());
    }

}  // namespace Backend::RV64::Passes::Optimize
