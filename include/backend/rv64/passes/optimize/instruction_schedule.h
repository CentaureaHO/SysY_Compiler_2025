#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_INSTRUCTION_SCHEDULE_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_INSTRUCTION_SCHEDULE_H__

#include <backend/base_pass.h>
#include <backend/rv64/rv64_function.h>
#include <backend/rv64/rv64_defs.h>
#include <backend/rv64/reg_assign.h>
#include <vector>
#include <map>
#include <set>
#include <string>

namespace Backend::RV64::Passes::Optimize
{
#define RISCV_INT_ARCH_REGS 32
#define RISCV_FP_ARCH_REGS 32

#define TARGET_ROB_ENTRIES 64

#define TARGET_MEM_ISSUE_WIDTH 1
#define TARGET_INT_ISSUE_WIDTH 2
#define TARGET_FP_ISSUE_WIDTH 1
#define TARGET_BRANCH_ISSUE_WIDTH 1

#define TARGET_MEM_RS_ENTRIES 12
#define TARGET_INT_RS_ENTRIES 20
#define TARGET_FP_RS_ENTRIES 16

#define INT_REG_RESERVE_MARGIN 4
#define FP_REG_RESERVE_MARGIN 4
#define ROB_SAFETY_FACTOR 2

#define I32_PRESSURE_LIMIT (RISCV_INT_ARCH_REGS - INT_REG_RESERVE_MARGIN)
#define F32_PRESSURE_LIMIT (RISCV_FP_ARCH_REGS - FP_REG_RESERVE_MARGIN)

#define SCHEDULER_ROB_LIMIT (TARGET_ROB_ENTRIES / ROB_SAFETY_FACTOR)

    struct ActiveRegSet
    {
        std::set<Register> i32_active;
        std::set<Register> f32_active;

        void remove(Register reg)
        {
            if (reg.data_type->data_type == DataType::INT)
            {
                i32_active.erase(reg);
                Assert(f32_active.find(reg) == f32_active.end());
            }
            else if (reg.data_type->data_type == DataType::FLOAT)
            {
                f32_active.erase(reg);
                Assert(i32_active.find(reg) == i32_active.end());
            }
            else { ERROR("Unknown data type"); }
        }

        void add(Register reg)
        {
            if (reg.data_type->data_type == DataType::INT) { i32_active.insert(reg); }
            else if (reg.data_type->data_type == DataType::FLOAT) { f32_active.insert(reg); }
            else { ERROR("Unknown data type"); }
        }

        void addins(Instruction* ins, std::map<Register, int>& kill_countdown)
        {
            for (auto reg : ins->getReadRegs())
            {
                kill_countdown[*reg] = kill_countdown[*reg] - 1;
                Assert(kill_countdown[*reg] >= 0);
                if (kill_countdown[*reg] <= 0) { this->remove(*reg); }
            }
            for (auto reg : ins->getWriteRegs()) { this->add(*reg); }
        }

        bool judgeSpill(Instruction* ins, std::map<Register, int>& kill_countdown)
        {
            int i_pressure = i32_active.size();
            int f_pressure = f32_active.size();
            for (auto reg : ins->getReadRegs())
            {
                if (kill_countdown[*reg] == 1)
                {
                    if (reg->data_type->data_type == DataType::INT) { i_pressure--; }
                    else if (reg->data_type->data_type == DataType::FLOAT) { f_pressure--; }
                }
            }
            for (auto reg : ins->getWriteRegs())
            {
                if (reg->data_type->data_type == DataType::INT) { i_pressure++; }
                else if (reg->data_type->data_type == DataType::FLOAT) { f_pressure++; }
            }
            return i_pressure > I32_PRESSURE_LIMIT || f_pressure > F32_PRESSURE_LIMIT;
        }
    };

    struct ExecState
    {
        int                         mem_units_busy;
        int                         int_units_busy;
        int                         fp_units_busy;
        int                         branch_units_busy;
        std::map<Instruction*, int> inflight;

        std::vector<Instruction*> nextCycle()
        {
            std::vector<Instruction*> ret;
            for (auto& [ins, latency] : inflight)
            {
                if (latency == 0) { ret.push_back(ins); }
                else { inflight[ins] = latency - 1; }
            }

            return ret;
        }

        bool full() { return inflight.size() >= SCHEDULER_ROB_LIMIT; }

        bool empty() { return inflight.empty(); }

        ExecState() : mem_units_busy(0), int_units_busy(0), fp_units_busy(0), branch_units_busy(0) {}

        bool CanIssue(Instruction* ins);
        bool Issue(Instruction* ins);
        bool Retire(Instruction* ins);

        enum FuncUnitType
        {
            MEM_UNIT,
            INT_UNIT,
            FP_UNIT,
            BRANCH_UNIT
        };
        FuncUnitType GetFuncUnitType(RV64Inst* ins);
        bool         HasFreeSlot(FuncUnitType unit_type);

        int GetMaxIssueWidth(FuncUnitType unit_type);
        int getInstLatency(Instruction* ins);
    };

    struct InsPrioEntry
    {
        Instruction* ins;
        int          prio;
        InsPrioEntry(Instruction* ins, int prio) : ins(ins), prio(prio) {}
        bool operator<(const InsPrioEntry& other) const
        {
            if (prio != other.prio) return prio > other.prio;
            return (long long)ins < (long long)other.ins;
        }
        bool operator==(const InsPrioEntry& other) const { return ins == other.ins && prio == other.prio; }
    };

    class InstructionSchedulePass : public Backend::BasePass
    {
      public:
        // Static vectors for instruction classification
        static const std::vector<RV64InstType> MemOp;
        static const std::vector<RV64InstType> BranchOp;
        static const std::vector<RV64InstType> FaluOp;
        static const std::vector<RV64InstType> MulDivOp;

      private:
        std::vector<Backend::RV64::Function*>& functions_;
        Backend::RV64::Function*               current_func;
        Backend::RV64::Block*                  cur_block;
        Liveness*                              liveness;

        void ExecuteInFunction(Backend::RV64::Function* func);
        void ExecuteInBlock();
        void ExecuteInList(std::vector<Instruction*>& list, std::vector<Instruction*>& res, ActiveRegSet& active_set,
            std::map<Register, int>& kill_countdown);

      public:
        InstructionSchedulePass(std::vector<Backend::RV64::Function*>& functions)
            : functions_(functions), current_func(nullptr), cur_block(nullptr), liveness(nullptr)
        {}

        bool        run() override;
        const char* getName() const override { return "InstructionSchedulePass"; }
    };

}  // namespace Backend::RV64::Passes::Optimize

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_INSTRUCTION_SCHEDULE_H__
