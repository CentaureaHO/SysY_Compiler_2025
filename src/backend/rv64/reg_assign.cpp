#include <backend/rv64/reg_assign.h>
#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>
#include <queue>
using namespace Backend::RV64;
using namespace std;

size_t MAX_REGISTERS = 0;

Interval::Interval() : ref_cnt(0) {}
Interval::Interval(Register r) : reg(r), ref_cnt(0) {}
Interval::Segmant::Segmant(int s, int e) : start(s), end(e) {}

bool Interval::Segmant::inside(int ins_id) const { return start <= ins_id && ins_id < end; }
bool Interval::Segmant::intersect(Interval::Segmant s) const
{
    return inside(s.start) || inside((s.end - 1 > s.start) ? s.end - 1 : s.start) || s.inside(start) ||
           s.inside((end - 1 > start) ? end - 1 : start);
}

bool Interval::intersect(const Interval& i) const
{
    if (segs.empty() || i.segs.empty()) return false;

    if (reg.is_virtual && segs.size() > 1)
    {
        int min_start = segs.front().start;
        int max_end   = segs.back().end;

        for (const auto& seg2 : i.segs)
        {
            if (seg2.start < max_end && min_start < seg2.end) { return true; }
        }
        return false;
    }

    if (i.reg.is_virtual && i.segs.size() > 1)
    {
        int min_start = i.segs.front().start;
        int max_end   = i.segs.back().end;

        for (const auto& seg1 : segs)
        {
            if (seg1.start < max_end && min_start < seg1.end) { return true; }
        }
        return false;
    }

    auto it1 = segs.begin(), it2 = i.segs.begin();
    while (it1 != segs.end() && it2 != i.segs.end())
    {
        const Segmant& s1 = *it1;
        const Segmant& s2 = *it2;

        if (s1.intersect(s2)) return true;
        if (s1.end <= s2.start)
            ++it1;
        else if (s2.end <= s1.start)
            ++it2;
        else
            ++it1;
    }

    return false;
}

AssignRecord::AssignRecord() { phy_occupied.resize(64); }
void AssignRecord::clear()
{
    phy_occupied.clear();
    mem_occupied.clear();
    phy_occupied.resize(64);
}

void AssignRecord::occupyReg(int phy_id, Interval in) { phy_occupied[phy_id].push_back(in); }
void AssignRecord::occupyMem(int offset, int size, Interval in)
{
    size /= 4;
    for (int i = offset; i < offset + size; ++i)
    {
        while ((size_t)i >= mem_occupied.size()) mem_occupied.push_back({});
        mem_occupied[i].push_back(in);
    }
}

std::vector<int> AssignRecord::getValidRegs(Interval in, bool save)
{
    if (in.reg.data_type->data_type == DataType::INT)
    {
        static bool        init = false;
        static vector<int> simple_res;
        static vector<int> save_res;
        if (init == false)
        {
            init = true;

#define X(name, alias, saver)                                           \
    {                                                                   \
        if (preg_##alias.reg_num <= preg_t6.reg_num)                    \
        {                                                               \
            if (saver == 0) simple_res.push_back(preg_##alias.reg_num); \
            if (saver == 1)                                             \
            {                                                           \
                simple_res.push_back(preg_##alias.reg_num);             \
                save_res.push_back(preg_##alias.reg_num);               \
            }                                                           \
        }                                                               \
    }
            RV64_REGS
#undef X

            int ra = preg_ra.reg_num;
            int fp = preg_fp.reg_num;
            simple_res.erase(remove(simple_res.begin(), simple_res.end(), ra), simple_res.end());
            simple_res.erase(remove(simple_res.begin(), simple_res.end(), fp), simple_res.end());
            save_res.erase(remove(save_res.begin(), save_res.end(), fp), save_res.end());
        }

        // return save_res;
        if (save) return save_res;
        return simple_res;
    }
    else if (in.reg.data_type->data_type == DataType::FLOAT)
    {
        static bool        init = false;
        static vector<int> res;
        if (init == false)
        {
            init = true;
            for (int i = static_cast<int>(REG::f0); i <= static_cast<int>(REG::f31); ++i) res.push_back(i);
        }

        return res;
    }
    assert(false);
    return std::vector<int>();
}

int AssignRecord::getValidMem(Interval in)
{
    vector<uint8_t> ok(mem_occupied.size(), 1);

    for (size_t i = 0; i < mem_occupied.size(); ++i)
    {
        for (auto& oti : mem_occupied[i])
        {
            // cerr << "CKPT: mem_occupied size " << mem_occupied.size() << " i " << i << " oti " << oti.reg.reg_num
            //      << endl;
            if (in.intersect(oti))
            {
                ok[i] = 0;
                break;
            }
        }
    }

    int free_cnt = 0;
    int size     = in.reg.data_type->getDataWidth() / 4;
    for (size_t offset = 0; offset < ok.size(); ++offset)
    {
        if (ok[offset])
            ++free_cnt;
        else
            free_cnt = 0;

        if (free_cnt == size) return offset - free_cnt + 1;
    }

    return static_cast<int>(mem_occupied.size()) - free_cnt;
}

void BaseRegisterAssigner::tagBFSID()
{
    // map<int, bool> visited;
    vector<bool> visited(cur_func->cfg->max_label + 1, false);
    queue<int>   worklist;
    worklist.push(0);

    int cnt = 0;

    while (!worklist.empty())
    {
        int cur = worklist.front();
        worklist.pop();

        if (visited[cur]) continue;
        visited[cur] = true;

        cur_block = cur_func->cfg->blocks[cur];
        for (auto& to : cur_func->cfg->graph[cur])
        {
            if (!visited[to->label_num]) worklist.push(to->label_num);
        }

        insmap[cnt++] = nullptr;
        for (auto& inst : cur_block->insts)
        {
            assert(inst->inst_type == InstType::RV64);
            inst->ins_id         = cnt++;
            insmap[inst->ins_id] = inst;
        }
    }
}

void BaseRegisterAssigner::getInterval()
{
    Liveness          liveness(cur_func);
    std::vector<bool> visited(cur_func->cfg->max_label + 1, false);
    std::queue<int>   worklist;
    std::vector<int>  bfs_order;
    worklist.push(0);

    while (!worklist.empty())
    {
        int cur = worklist.front();
        worklist.pop();

        if (visited[cur]) continue;
        visited[cur] = true;

        bfs_order.push_back(cur);
        for (auto& to : cur_func->cfg->graph[cur])
        {
            if (!visited[to->label_num]) worklist.push(to->label_num);
        }
    }

    for (int block_id : bfs_order)
    {
        Cele::dynamic_bitset in_set  = liveness.GetIN(block_id);
        Cele::dynamic_bitset out_set = liveness.GetOUT(block_id);

        for (size_t i = 0; i < MAX_REGISTERS; ++i)
        {
            if (in_set.test(i)) { Register reg = liveness.reverse_mapping[i]; }
        }

        for (size_t i = 0; i < MAX_REGISTERS; ++i)
        {
            if (out_set.test(i)) { Register reg = liveness.reverse_mapping[i]; }
        }
    }

    std::map<Register, int> last_def, last_use;
    for (auto it = bfs_order.rbegin(); it != bfs_order.rend(); ++it)
    {
        Block* block = cur_func->cfg->blocks[*it];
        int    id    = block->label_num;

        Cele::dynamic_bitset out = liveness.GetOUT(id);
        for (size_t i = 0; i < MAX_REGISTERS; ++i)
        {
            if (out.test(i))
            {
                Register reg = liveness.reverse_mapping[i];

                if (intervals.find(reg) == intervals.end()) intervals[reg] = Interval(reg);
                if (last_use.find(reg) == last_use.end())
                {
                    int in_ins_id  = block->insts.front()->ins_id - 1;
                    int out_ins_id = block->insts.back()->ins_id;
                    intervals[reg].segs.emplace_back(in_ins_id, out_ins_id);
                }
                else
                {
                    int in_ins_id  = block->insts.front()->ins_id - 1;
                    int out_ins_id = block->insts.back()->ins_id;
                    intervals[reg].segs.emplace_back(in_ins_id, out_ins_id);
                }
                last_use[reg] = block->insts.back()->ins_id;
            }
        }

        for (auto rit = block->insts.rbegin(); rit != block->insts.rend(); ++rit)
        {
            Instruction* inst = *rit;

            for (auto& reg : inst->getWriteRegs())
            {
                last_def[*reg] = inst->ins_id;
                if (intervals.find(*reg) == intervals.end()) intervals[*reg] = Interval(*reg);

                if (last_use.find(*reg) != last_use.end())
                {
                    last_use.erase(*reg);

                    if (!intervals[*reg].segs.empty()) { intervals[*reg].segs.back().start = inst->ins_id; }
                }
                else { intervals[*reg].segs.emplace_back(inst->ins_id, inst->ins_id + 1); }

                intervals[*reg].ref_cnt++;
            }
            for (auto& reg : inst->getReadRegs())
            {
                if (intervals.find(*reg) == intervals.end()) intervals[*reg] = Interval(*reg);
                if (last_use.find(*reg) == last_use.end())
                {
                    int in_ins_id = inst->ins_id - 1;
                    intervals[*reg].segs.emplace_back(in_ins_id, inst->ins_id);
                }
                last_use[*reg] = inst->ins_id;

                intervals[*reg].ref_cnt++;
            }
        }
    }

    last_def.clear();
    last_use.clear();

    for (auto& [reg, interval] : intervals)
    {
        std::sort(interval.segs.begin(),
            interval.segs.end(),
            [](const Interval::Segmant& a, const Interval::Segmant& b) { return a.start < b.start; });
    }
}

Register BaseRegisterAssigner::genReadCode(std::list<Instruction*>::iterator& it, int raw_stk_offset, DataType* dt)
{
    Register read_mid_reg = cur_func->getNewReg(dt);
    int      offset       = raw_stk_offset + cur_func->stack_size;

    if (offset <= 2047 && offset >= -2048)
    {
        if (dt == INT64)
            cur_block->insts.insert(it, createIInst(RV64InstType::LD, read_mid_reg, preg_sp, offset));
        else if (dt == FLOAT64)
            cur_block->insts.insert(it, createIInst(RV64InstType::FLD, read_mid_reg, preg_sp, offset));
        else
            assert(false);

        return read_mid_reg;
    }

    Register imme_reg       = cur_func->getNewReg(INT64);
    Register offset_mid_reg = cur_func->getNewReg(INT64);
    cur_block->insts.insert(it, createUInst(RV64InstType::LI, imme_reg, offset));
    cur_block->insts.insert(it, createRInst(RV64InstType::ADD, offset_mid_reg, preg_sp, imme_reg));

    if (dt == INT64)
        cur_block->insts.insert(it, createIInst(RV64InstType::LD, read_mid_reg, offset_mid_reg, 0));
    else if (dt == FLOAT64)
        cur_block->insts.insert(it, createIInst(RV64InstType::FLD, read_mid_reg, offset_mid_reg, 0));
    else
        assert(false);

    return read_mid_reg;
}

Register BaseRegisterAssigner::genWriteCode(std::list<Instruction*>::iterator& it, int raw_stk_offset, DataType* dt)
{
    Register write_mid_reg = cur_func->getNewReg(dt);
    int      offset        = raw_stk_offset + cur_func->stack_size;
    ++it;

    if (offset <= 2047 && offset >= -2048)
    {
        if (dt == INT64)
            cur_block->insts.insert(it, createSInst(RV64InstType::SD, write_mid_reg, preg_sp, offset));
        else if (dt == FLOAT64)
            cur_block->insts.insert(it, createSInst(RV64InstType::FSD, write_mid_reg, preg_sp, offset));
        else
            assert(false);
    }
    else
    {
        Register imme_reg       = cur_func->getNewReg(INT64);
        Register offset_mid_reg = cur_func->getNewReg(INT64);
        cur_block->insts.insert(it, createUInst(RV64InstType::LI, imme_reg, offset));
        cur_block->insts.insert(it, createRInst(RV64InstType::ADD, offset_mid_reg, preg_sp, imme_reg));
        if (dt == INT64)
            cur_block->insts.insert(it, createSInst(RV64InstType::SD, write_mid_reg, offset_mid_reg, 0));
        else if (dt == FLOAT64)
            cur_block->insts.insert(it, createSInst(RV64InstType::FSD, write_mid_reg, offset_mid_reg, 0));
        else
            assert(false);
    }

    --it;
    return write_mid_reg;
}

void LinearScanRegisterAssigner::assignRegisters(std::vector<Function*>& functions)
{
    queue<Function*> worklist;
    for (auto& func : functions) worklist.push(func);

    while (!worklist.empty())
    {
        insmap.clear();
        intervals.clear();
        dfsOrderBlocks.clear();
        regAlloc.clear();
        phy_regs.clear();

        cur_func = worklist.front();
        worklist.pop();

        // calcIntervals();
        tagBFSID();
        getInterval();
        for (auto& [reg, interval] : intervals)
            sort(interval.segs.begin(),
                interval.segs.end(),
                [](const Interval::Segmant& a, const Interval::Segmant& b) { return a.start < b.start; });

        if (tryAssignRegister())
        {
            // replace

            for (auto& [id, block] : cur_func->cfg->blocks)
            {
                for (auto& inst : block->insts)
                {
                    for (auto& reg : inst->getReadRegs())
                    {
                        if (!reg->is_virtual) continue;

                        auto& [in_mem, id] = regAlloc[*reg];
                        assert(!in_mem);
                        reg->is_virtual = false;
                        // cerr << "Replace " << reg->reg_num << " with " << ar.phy_reg_no << endl;
                        reg->reg_num = id;
                    }
                    for (auto& reg : inst->getWriteRegs())
                    {
                        if (!reg->is_virtual) continue;

                        auto& [in_mem, id] = regAlloc[*reg];
                        assert(!in_mem);
                        reg->is_virtual = false;
                        // cerr << "Replace " << reg->reg_num << " with " << ar.phy_reg_no << endl;
                        reg->reg_num = id;
                    }
                }
            }

            continue;
        }

        for (auto& [id, block] : cur_func->cfg->blocks)
        {
            cur_block = block;
            for (auto ins_it = block->insts.begin(); ins_it != block->insts.end(); ++ins_it)
            {
                Instruction* inst = *ins_it;

                for (auto& reg : inst->getReadRegs())
                {
                    if (!reg->is_virtual) continue;

                    auto& [in_mem, id] = regAlloc[*reg];
                    if (!in_mem) continue;

                    *reg = genReadCode(ins_it, id * 4, reg->data_type);
                }
                for (auto& reg : inst->getWriteRegs())
                {
                    if (!reg->is_virtual) continue;

                    auto& [in_mem, id] = regAlloc[*reg];
                    if (!in_mem) continue;

                    *reg = genWriteCode(ins_it, id * 4, reg->data_type);
                }
            }
        }

        cur_func->stack_size += ((int)phy_regs.mem_occupied.size()) * 4;
        worklist.push(cur_func);
    }

    return;
}

void LinearScanRegisterAssigner::intervalDfs(Block* cur, vector<vector<Block*>> to, vector<uint8_t>& visited, int& dfn)
{
    int label = cur->label_num;
    if (visited[label]) return;

    visited[label] = 1;
    dfsOrderBlocks.push_back(cur);
    insmap[dfn++] = nullptr;
    for (auto& inst : cur->insts)
    {
        assert(inst->inst_type == InstType::RV64);
        inst->ins_id         = dfn++;
        insmap[inst->ins_id] = inst;
    }

    for (auto& block : to[label]) intervalDfs(block, to, visited, dfn);
}

// #define PRINT_INST_ID
void LinearScanRegisterAssigner::calcIntervals()
{
    int             dfn       = 0;
    int             max_label = cur_func->cfg->max_label;
    vector<uint8_t> visited(max_label + 1, 0);
    intervalDfs(cur_func->cfg->blocks[0], cur_func->cfg->graph, visited, dfn);

#ifdef PRINT_INST_ID
    cerr << "Function " << cur_func->name << ":\n";
    for (auto& [id, block] : cur_func->cfg->blocks)
    {
        cerr << "Block " << id << ":\n";
        for (auto& inst : block->insts)
            cerr << "\t" << opInfoTable[((RV64Inst*)inst)->op]._asm << " " << inst->ins_id << endl;
    }
#endif

    map<int, set<Register>> IN, OUT, DEF, USE;
    for (auto& [id, block] : cur_func->cfg->blocks)
    {
        DEF[id] = set<Register>();
        USE[id] = set<Register>();

        set<Register>& def = DEF[id];
        set<Register>& use = USE[id];

        for (auto& inst : block->insts)
        {
            for (auto& reg : inst->getReadRegs()) use.insert(*reg);
            for (auto& reg : inst->getWriteRegs()) def.insert(*reg);
        }
    }

    bool changed = false;
    while (true)
    {
        changed = false;
        for (auto& [id, block] : cur_func->cfg->blocks)
        {
            set<Register> in, out;
            for (auto& Block : cur_func->cfg->graph[id])
                out.insert(IN[Block->label_num].begin(), IN[Block->label_num].end());

            if (OUT[id] != out) OUT[id] = out;
            set<Register> diff_tmp;
            set_difference(
                OUT[id].begin(), OUT[id].end(), DEF[id].begin(), DEF[id].end(), inserter(diff_tmp, diff_tmp.begin()));
            set_union(USE[id].begin(), USE[id].end(), diff_tmp.begin(), diff_tmp.end(), inserter(in, in.begin()));

            if (in != IN[id])
            {
                changed = true;
                IN[id]  = in;
            }
        }

        if (!changed) break;
    }

    map<Register, int> last_def, last_use;
    for (auto it = dfsOrderBlocks.rbegin(); it != dfsOrderBlocks.rend(); ++it)
    {
        Block* block = *it;
        int    id    = block->label_num;

        for (auto& reg : OUT[id])
        {
            if (intervals.find(reg) == intervals.end()) intervals[reg] = Interval(reg);
            if (last_use.find(reg) == last_use.end())
            {
                int in_ins_id  = block->insts.front()->ins_id - 1;
                int out_ins_id = block->insts.back()->ins_id;
                intervals[reg].segs.emplace_back(in_ins_id, out_ins_id);
            }
            else
            {
                int in_ins_id = 0, out_ins_id = 0;
                in_ins_id  = block->insts.front()->ins_id - 1;
                out_ins_id = block->insts.back()->ins_id;
                intervals[reg].segs.emplace_back(in_ins_id, out_ins_id);
            }
            last_use[reg] = block->insts.back()->ins_id;
        }

        for (auto it = block->insts.rbegin(); it != block->insts.rend(); ++it)
        {
            assert((*it)->inst_type == InstType::RV64);
            RV64Inst* inst = (RV64Inst*)*it;

            for (auto& reg : inst->getWriteRegs())
            {
                last_def[*reg] = inst->ins_id;
                if (intervals.find(*reg) == intervals.end()) intervals[*reg] = Interval(*reg);
                if (last_use.find(*reg) == last_use.end())
                {
                    intervals[*reg].segs.emplace_back(inst->ins_id, inst->ins_id);
                }
                else
                {
                    last_use.erase(*reg);

                    if (!intervals[*reg].segs.empty()) { intervals[*reg].segs.back().start = inst->ins_id; }
                }
                ++intervals[*reg].ref_cnt;
            }
            for (auto& reg : inst->getReadRegs())
            {
                if (intervals.find(*reg) == intervals.end()) intervals[*reg] = Interval(*reg);

                if (last_use.find(*reg) == last_use.end())
                {
                    int in_ins_id = 0, out_ins_id = 0;
                    in_ins_id = block->insts.front()->ins_id - 1;
                    intervals[*reg].segs.emplace_back(in_ins_id, inst->ins_id);
                }
                last_use[*reg] = inst->ins_id;
                ++intervals[*reg].ref_cnt;
            }
        }

        last_def.clear();
        last_use.clear();
    }

    for (auto& [reg, interval] : intervals)
    {
        std::sort(interval.segs.begin(),
            interval.segs.end(),
            [](const Interval::Segmant& a, const Interval::Segmant& b) { return a.start < b.start; });
    }

    // for (auto& kv : intervals)
    // {
    //     cerr << "[Reg " << kv.first.reg_num << "] segs: ";
    //     for (auto& seg : kv.second.segs) { cerr << "[" << seg.start << "," << seg.end << ") "; }
    //     cerr << " ref_cnt=" << kv.second.ref_cnt << endl;
    // }

    // int i = 0;
}

namespace
{
    bool intervalCmp(Interval a, Interval b) { return a.segs.front().start < b.segs.front().start; }
}  // namespace

bool LinearScanRegisterAssigner::tryAssignRegister()
{
    static int try_cnt = 0;

    struct IntervalPtrCmp
    {
        bool operator()(const Interval* a, const Interval* b) const
        {
            return a->segs.front().start < b->segs.front().start;
        }
    };

    std::priority_queue<const Interval*, std::vector<const Interval*>, IntervalPtrCmp> unalloc_queue;

    bool spilled = false;

    for (auto& [reg, interval] : intervals)
    {
        if (reg.is_virtual) { unalloc_queue.push(&interval); }
        else { phy_regs.occupyReg(reg.reg_num, interval); }
    }

    while (!unalloc_queue.empty())
    {
        const Interval* in_ptr = unalloc_queue.top();
        unalloc_queue.pop();

        const Interval& in = *in_ptr;

        Register vreg       = in.reg;
        int      phy_reg_id = -1;

        bool in_param = false;
        if (!cur_func->params.empty())
        {
            for (auto& param : cur_func->params)
            {
                if (param.reg_num == in.reg.reg_num)
                {
                    in_param = true;
                    break;
                }
            }
        }

        const auto& valid_regs = phy_regs.getValidRegs(in, in_param);

        for (int reg : valid_regs)
        {
            bool        conflict = false;
            const auto& occupied = phy_regs.phy_occupied[reg];

            if (occupied.empty())
            {
                phy_reg_id = reg;
                break;
            }

            for (const auto& oti : occupied)
            {
                if (in.intersect(oti))
                {
                    conflict = true;
                    break;
                }
            }

            if (!conflict)
            {
                phy_reg_id = reg;
                break;
            }
        }

        if (phy_reg_id >= 0)
        {
            phy_regs.occupyReg(phy_reg_id, in);
            regAlloc[vreg] = {false, phy_reg_id};
            continue;
        }

        // assert(false);
        spilled = true;
        int mem = phy_regs.getValidMem(in);
        phy_regs.occupyMem(mem, in.reg.data_type->getDataWidth(), in);
        regAlloc[vreg] = {true, mem};
    }

    return !spilled;
}

// GraphColoringRegisterAssigner实现
// 暂时直接使用父类LinearScanRegisterAssigner的实现

// Liveness实现
Cele::dynamic_bitset Liveness::GetIN(int bid) { return IN[bid]; }
Cele::dynamic_bitset Liveness::GetOUT(int bid) { return OUT[bid]; }

void Liveness::UpdateDefUse()
{
    DEF.clear();
    USE.clear();
    register_to_index.clear();
    reverse_mapping.clear();

    size_t reg_index = 0;
    MAX_REGISTERS    = 0;

    for (auto& [id, block] : current_func->cfg->blocks)
        for (auto& inst : block->insts) MAX_REGISTERS += inst->getReadRegs().size() + inst->getWriteRegs().size();

    for (auto& [id, block] : current_func->cfg->blocks)
    {
        DEF[id].resize(MAX_REGISTERS);
        USE[id].resize(MAX_REGISTERS);

        for (auto& inst : block->insts)
        {
            for (auto& reg_r : inst->getReadRegs())
            {
                if (register_to_index.find(*reg_r) == register_to_index.end())
                {
                    register_to_index[*reg_r] = reg_index++;
                    reverse_mapping.push_back(*reg_r);
                }
                size_t index = register_to_index[*reg_r];
                if (!DEF[id].test(index)) { USE[id].set(index); }
            }
            for (auto& reg_w : inst->getWriteRegs())
            {
                if (register_to_index.find(*reg_w) == register_to_index.end())
                {
                    register_to_index[*reg_w] = reg_index++;
                    reverse_mapping.push_back(*reg_w);
                }
                size_t index = register_to_index[*reg_w];
                if (!USE[id].test(index)) { DEF[id].set(index); }
            }
        }
    }
}

void Liveness::Execute()
{
    UpdateDefUse();

    OUT.clear();
    IN.clear();

    for (auto& [id, block] : current_func->cfg->blocks)
    {
        OUT[id].resize(MAX_REGISTERS);
        IN[id].resize(MAX_REGISTERS);
    }

    bool changed = true;

    while (changed)
    {
        changed = false;

        for (auto& [id, block] : current_func->cfg->blocks)
        {
            auto& cur_block = block;
            auto  cur_id    = cur_block->label_num;

            Cele::dynamic_bitset out(MAX_REGISTERS);
            for (auto& succ : current_func->cfg->graph[cur_id]) { out |= IN[succ->label_num]; }
            if (out != OUT[cur_id])
            {
                changed     = true;
                OUT[cur_id] = out;
            }

            Cele::dynamic_bitset in = USE[cur_id] | (OUT[cur_id] & ~DEF[cur_id]);
            if (in != IN[cur_id])
            {
                changed    = true;
                IN[cur_id] = in;
            }
        }
    }
}
