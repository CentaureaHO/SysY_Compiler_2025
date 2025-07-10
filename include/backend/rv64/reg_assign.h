#ifndef __BACKEND_RV64_REG_ASSIGN_H__
#define __BACKEND_RV64_REG_ASSIGN_H__

#include "rv64_cfg.h"
#include "rv64_function.h"
#include "rv64_block.h"
#include <dynamic_bitset.h>
#include <bitset>
#include <functional>
#include <vector>
#include <set>
#include <memory>

namespace std
{
    template <>
    struct hash<Backend::RV64::Register>
    {
        std::size_t operator()(const Backend::RV64::Register& reg) const noexcept
        {
            return std::hash<int>()(reg.reg_num);
        }
    };
}  // namespace std

namespace Backend::RV64
{
    struct Interval
    {
        struct Segmant
        {
            int start, end;

            Segmant(int s, int e);

            bool inside(int ins_id) const;
            bool intersect(Interval::Segmant i) const;
        };

        Register             reg;
        std::vector<Segmant> segs;
        int                  ref_cnt;

        Interval();
        Interval(Register r);

        bool intersect(const Interval& i) const;
    };

    class AssignRecord
    {
      public:
        MAT2(Interval) phy_occupied;
        MAT2(Interval) mem_occupied;

      public:
        AssignRecord();

      public:
        void clear();
        void occupyReg(int phy_id, Interval i);
        void occupyMem(int offset, int size, Interval i);

        std::vector<int> getValidRegs(Interval i, bool save = false);
        int              getValidMem(Interval i);
    };

    class Liveness
    {
      public:
        Function* current_func;
        void      UpdateDefUse();

        std::unordered_map<int, Cele::dynamic_bitset> IN, OUT, DEF, USE;
        std::unordered_map<Register, size_t>          register_to_index;
        std::vector<Register>                         reverse_mapping;

      public:
        void Execute();
        Liveness(Function* mfun, bool calculate = true) : current_func(mfun)
        {
            if (calculate) { Execute(); }
        }

        Cele::dynamic_bitset GetIN(int bid);
        Cele::dynamic_bitset GetOUT(int bid);
    };

    class BaseRegisterAssigner
    {
      public:
        virtual ~BaseRegisterAssigner() = default;

        virtual void assignRegisters(std::vector<Function*>& functions) = 0;

      protected:
        Function*    cur_func;
        Block*       cur_block;
        AssignRecord phy_regs;

        std::map<int, Instruction*>              insmap;
        std::map<Register, Interval>             intervals;
        std::vector<Block*>                      dfsOrderBlocks;
        std::map<Register, std::pair<bool, int>> regAlloc;

        void     tagBFSID();
        void     getInterval();
        Register genReadCode(std::list<Instruction*>::iterator& it, int raw_stk_offset, DataType* dt);
        Register genWriteCode(std::list<Instruction*>::iterator& it, int raw_stk_offset, DataType* dt);
    };

    // 线性扫描寄存器分配器
    class LinearScanRegisterAssigner : public BaseRegisterAssigner
    {
      public:
        LinearScanRegisterAssigner()          = default;
        virtual ~LinearScanRegisterAssigner() = default;

        void assignRegisters(std::vector<Function*>& functions) override;

      private:
        void calcIntervals();
        void intervalDfs(Block* cur, MAT2(Block*) to, std::vector<uint8_t>& visited, int& dfn);
        bool tryAssignRegister();
    };

    class GraphColoringRegisterAssigner : public LinearScanRegisterAssigner
    {
      public:
        GraphColoringRegisterAssigner()          = default;
        virtual ~GraphColoringRegisterAssigner() = default;

        // 暂时使用父类的实现
        // void assignRegisters(std::vector<Function*>& functions) override;

      private:
        // TODO: 实现图着色算法相关的私有方法
    };

    using RegisterAssigner = LinearScanRegisterAssigner;

}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_REG_ASSIGN_H__
