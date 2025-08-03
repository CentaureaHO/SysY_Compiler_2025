#include <backend/rv64/reg_assign.h>
#include <iostream>
#include <set>
#include <unordered_set>
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
bool Interval::Segmant::intersect(const Interval::Segmant& s) const
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

int Interval::getIntervalLength() const
{
    int total_length = 0;
    for (const auto& seg : segs) { total_length += (seg.end - seg.start); }
    return total_length;
}

double Interval::calculateSpillWeight() const
{
    int length = getIntervalLength();
    if (length == 0) return 0.0;
    return static_cast<double>(ref_cnt) / static_cast<double>(length);
}

void Interval::addSegment(int start, int end)
{
    if (start >= end) return;

    segs.emplace_back(start, end);
    std::sort(segs.begin(), segs.end());
    mergeSegments();
}

void Interval::mergeSegments()
{
    if (segs.size() <= 1) return;

    std::vector<Segmant> merged;
    merged.reserve(segs.size());

    for (const auto& seg : segs)
    {
        if (merged.empty() || merged.back().end < seg.start)
        {
            merged.push_back(seg);
        }
        else
        {
            merged.back().end = std::max(merged.back().end, seg.end);
        }
    }

    segs = std::move(merged);
}

Interval Interval::unionWith(const Interval& other) const
{
    Interval result(reg);
    result.ref_cnt = ref_cnt + other.ref_cnt;

    result.segs.reserve(segs.size() + other.segs.size());
    result.segs.insert(result.segs.end(), segs.begin(), segs.end());
    result.segs.insert(result.segs.end(), other.segs.begin(), other.segs.end());

    std::sort(result.segs.begin(), result.segs.end());
    result.mergeSegments();

    return result;
}

std::pair<Interval, Interval> Interval::splitAt(int split_point) const
{
    Interval left(reg), right(reg);

    for (const auto& seg : segs)
    {
        if (seg.end <= split_point)
        {
            left.segs.push_back(seg);
        }
        else if (seg.start >= split_point)
        {
            right.segs.push_back(seg);
        }
        else
        {
            if (seg.start < split_point) { left.segs.emplace_back(seg.start, split_point); }
            if (split_point < seg.end) { right.segs.emplace_back(split_point, seg.end); }
        }
    }

    int left_length  = left.getIntervalLength();
    int right_length = right.getIntervalLength();
    int total_length = left_length + right_length;

    if (total_length > 0)
    {
        left.ref_cnt  = (ref_cnt * left_length) / total_length;
        right.ref_cnt = ref_cnt - left.ref_cnt;
    }

    return {left, right};
}

void Interval::print() const
{
    std::cerr << "Interval for reg " << reg.reg_num << (reg.is_virtual ? " (virtual)" : " (physical)")
              << " ref_cnt=" << ref_cnt << " segments: ";
    for (const auto& seg : segs) { std::cerr << "[" << seg.start << "," << seg.end << ") "; }
    std::cerr << std::endl;
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

std::vector<Interval> AssignRecord::getConflictIntervals(const Interval& interval) const
{
    std::vector<Interval> result;

    for (const auto& phy_intervals : phy_occupied)
    {
        for (const auto& other_interval : phy_intervals)
        {
            if (interval.reg.data_type->data_type == other_interval.reg.data_type->data_type &&
                interval.intersect(other_interval))
            {
                result.push_back(other_interval);
            }
        }
    }

    return result;
}

bool AssignRecord::releaseReg(int phy_id, const Interval& interval)
{
    auto& intervals = phy_occupied[phy_id];
    for (auto it = intervals.begin(); it != intervals.end(); ++it)
    {
        if (it->reg.reg_num == interval.reg.reg_num && it->reg.is_virtual == interval.reg.is_virtual)
        {
            intervals.erase(it);
            return true;
        }
    }
    return false;
}

bool AssignRecord::releaseMem(int offset, int size, const Interval& interval)
{
    size /= 4;
    bool released = false;

    for (int i = offset; i < offset + size; ++i)
    {
        if (i < static_cast<int>(mem_occupied.size()))
        {
            auto& intervals = mem_occupied[i];
            for (auto it = intervals.begin(); it != intervals.end(); ++it)
            {
                if (it->reg.reg_num == interval.reg.reg_num && it->reg.is_virtual == interval.reg.is_virtual)
                {
                    intervals.erase(it);
                    released = true;
                    break;
                }
            }
        }
    }

    return released;
}

int AssignRecord::getIdleReg(
    const Interval& interval, const std::vector<int>& preferred_regs, const std::vector<int>& noprefer_regs, bool save)
{
    auto isRegAvailable = [&](int reg_id) -> bool {
        const auto& occupied = phy_occupied[reg_id];

        if (occupied.empty()) { return true; }

        for (const auto& other_interval : occupied)
        {
            if (interval.intersect(other_interval)) { return false; }
        }
        return true;
    };

    for (int reg_id : preferred_regs)
    {
        if (isRegAvailable(reg_id)) { return reg_id; }
    }

    const auto&             valid_regs = getValidRegs(interval, save);
    std::unordered_set<int> preferred_set(preferred_regs.begin(), preferred_regs.end());
    std::unordered_set<int> noprefer_set(noprefer_regs.begin(), noprefer_regs.end());

    for (int reg_id : valid_regs)
    {
        if (preferred_set.count(reg_id)) continue;
        if (noprefer_set.count(reg_id)) continue;
        if (isRegAvailable(reg_id)) { return reg_id; }
    }

    for (int reg_id : noprefer_regs)
    {
        if (std::find(valid_regs.begin(), valid_regs.end(), reg_id) == valid_regs.end()) { continue; }

        if (isRegAvailable(reg_id)) { return reg_id; }
    }

    return -1;
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
        // cerr << "\tTag block " << cur << " with ID " << cnt << endl;

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
        copy_sources.clear();

        cur_func = worklist.front();
        worklist.pop();

        // cerr << "Reg alloc processing function " << cur_func->name << endl;

        // calcIntervals();
        tagBFSID();
        getInterval();
        for (auto& [reg, interval] : intervals)
            sort(interval.segs.begin(),
                interval.segs.end(),
                [](const Interval::Segmant& a, const Interval::Segmant& b) { return a.start < b.start; });

        collectCopyInformation();
        coalesceRegisters();

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
        if (reg.is_virtual)
            unalloc_queue.push(&interval);
        else
            phy_regs.occupyReg(reg.reg_num, interval);
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

        std::vector<int> preferred_regs;
        std::vector<int> noprefer_regs;

        for (const auto& copy_source : copy_sources[vreg])
        {
            if (!copy_source.is_virtual)
            {
                preferred_regs.push_back(copy_source.reg_num);
            }
            else
            {
                if (regAlloc.find(copy_source) != regAlloc.end() && !regAlloc[copy_source].first)
                {
                    preferred_regs.push_back(regAlloc[copy_source].second);
                }
            }
        }

        for (const auto& seg : in.segs)
        {
            int def_point = seg.start;

            auto inst_it = insmap.find(def_point);
            if (inst_it == insmap.end() || inst_it->second == nullptr) continue;

            Instruction* current_inst = inst_it->second;
            if (current_inst->inst_type != InstType::RV64) continue;

            RV64Inst* rv_inst = static_cast<RV64Inst*>(current_inst);
            int       latency = rv_inst->getLatency();

            const int look_back = 5;
            for (int i = 1; i <= look_back; ++i)
            {
                auto prev_it = insmap.find(def_point - i);
                if (prev_it == insmap.end()) break;

                if (prev_it->second == nullptr) break;

                Instruction* prev_inst = prev_it->second;
                if (prev_inst->inst_type != InstType::RV64) continue;

                RV64Inst* prev_rv_inst = static_cast<RV64Inst*>(prev_inst);
                int       prev_latency = prev_rv_inst->getLatency();

                if (prev_latency >= i)
                {
                    auto write_regs = prev_inst->getWriteRegs();
                    if (write_regs.size() == 1)
                    {
                        Register* written_reg = write_regs[0];
                        if (!written_reg->is_virtual)
                        {
                            if (written_reg->reg_num != static_cast<int>(REG::x0))
                            {
                                noprefer_regs.push_back(written_reg->reg_num);
                            }
                        }
                        else if (regAlloc.find(*written_reg) != regAlloc.end() && !regAlloc[*written_reg].first)
                        {
                            noprefer_regs.push_back(regAlloc[*written_reg].second);
                        }
                    }
                }
            }

            for (int i = 1; i <= latency && i <= 3; ++i)
            {
                auto next_it = insmap.find(def_point + i);
                if (next_it == insmap.end()) break;

                if (next_it->second == nullptr) break;

                Instruction* next_inst = next_it->second;
                if (next_inst->inst_type != InstType::RV64) continue;

                auto write_regs = next_inst->getWriteRegs();
                if (write_regs.size() == 1)
                {
                    Register* written_reg = write_regs[0];
                    if (!written_reg->is_virtual)
                    {
                        if (written_reg->reg_num != static_cast<int>(REG::x0))
                        {
                            noprefer_regs.push_back(written_reg->reg_num);
                        }
                    }
                    else if (regAlloc.find(*written_reg) != regAlloc.end() && !regAlloc[*written_reg].first)
                    {
                        noprefer_regs.push_back(regAlloc[*written_reg].second);
                    }
                }
            }
        }

        phy_reg_id = phy_regs.getIdleReg(in, preferred_regs, noprefer_regs, in_param);

        if (phy_reg_id >= 0)
        {
            phy_regs.occupyReg(phy_reg_id, in);
            regAlloc[vreg] = {false, phy_reg_id};
            continue;
        }

        spilled = true;

        int mem = phy_regs.getValidMem(in);
        phy_regs.occupyMem(mem, in.reg.data_type->getDataWidth(), in);
        regAlloc[vreg] = {true, mem};

/*
        if (intervals.size() > 200)
            continue;*/

        double current_spill_weight = in.calculateSpillWeight();

        const Interval* best_victim         = nullptr;
        int             best_victim_phy_reg = -1;
        double          best_victim_weight  = current_spill_weight;

        const auto& valid_regs = phy_regs.getValidRegs(in, in_param);

        for (int reg_id : valid_regs)
        {
            const auto& occupied = phy_regs.phy_occupied[reg_id];

            for (const auto& other_interval : occupied)
            {
                if (!other_interval.reg.is_virtual) continue;

                if (in.intersect(other_interval))
                {
                    double conflict_weight = other_interval.calculateSpillWeight();
                    if (conflict_weight < best_victim_weight)
                    {
                        best_victim_weight  = conflict_weight;
                        best_victim         = &other_interval;
                        best_victim_phy_reg = reg_id;
                    }
                }
            }
        }

        if (best_victim != nullptr && best_victim_phy_reg >= 0)
        {
            swapRegisterSpill(*best_victim, best_victim_phy_reg, in, mem);
        }
    }

    return !spilled;
}

void LinearScanRegisterAssigner::swapRegisterSpill(
    const Interval& victim_interval, int victim_phy_reg, const Interval& current_interval, int mem_offset)
{
    phy_regs.releaseReg(victim_phy_reg, victim_interval);
    phy_regs.releaseMem(mem_offset, current_interval.reg.data_type->getDataWidth(), current_interval);

    phy_regs.occupyReg(victim_phy_reg, current_interval);
    regAlloc[current_interval.reg] = {false, victim_phy_reg};

    int victim_mem = phy_regs.getValidMem(victim_interval);
    phy_regs.occupyMem(victim_mem, victim_interval.reg.data_type->getDataWidth(), victim_interval);
    regAlloc[victim_interval.reg] = {true, victim_mem};
}

Register LinearScanRegisterAssigner::findCoalescingRoot(std::map<Register, Register>& coal_result, Register vreg)
{
    Register ret = vreg;
    while (!(ret.reg_num == coal_result[ret].reg_num && ret.is_virtual == coal_result[ret].is_virtual))
    {
        ret = coal_result[ret];
    }
    return coal_result[vreg] = ret;
}

void LinearScanRegisterAssigner::collectCopyInformation()
{
    copy_sources.clear();

    for (auto& [block_id, block] : cur_func->cfg->blocks)
    {
        for (auto& inst : block->insts)
        {
            if (inst->inst_type == InstType::RV64)
            {
                RV64Inst* rv_inst = static_cast<RV64Inst*>(inst);

                if (rv_inst->op == RV64InstType::ADD)
                {
                    auto read_regs  = inst->getReadRegs();
                    auto write_regs = inst->getWriteRegs();

                    if (read_regs.size() == 2 && write_regs.size() == 1)
                    {
                        Register* src1 = read_regs[0];
                        Register* src2 = read_regs[1];
                        Register* dst  = write_regs[0];

                        if ((!src1->is_virtual && src1->reg_num == static_cast<int>(REG::x0)) ||
                            (!src2->is_virtual && src2->reg_num == static_cast<int>(REG::x0)))
                        {
                            Register* real_src =
                                (src1->is_virtual || src1->reg_num != static_cast<int>(REG::x0)) ? src1 : src2;

                            copy_sources[*dst].push_back(*real_src);
                            copy_sources[*real_src].push_back(*dst);
                        }
                    }
                }
            }
        }
    }
}

void LinearScanRegisterAssigner::coalesceRegisters()
{
    std::map<Register, Register> coal_result;

    for (auto& [reg, interval] : intervals)
    {
        if (reg.is_virtual) { coal_result[reg] = reg; }
    }

    for (auto& [reg, interval] : intervals)
    {
        if (!reg.is_virtual) continue;

        for (auto& other : copy_sources[reg])
        {
            if (!other.is_virtual) continue;

            if (coal_result.find(reg) == coal_result.end()) continue;
            if (coal_result.find(other) == coal_result.end()) continue;

            Register root_reg       = findCoalescingRoot(coal_result, reg);
            Register other_root_reg = findCoalescingRoot(coal_result, other);

            if (root_reg.reg_num == other_root_reg.reg_num && root_reg.is_virtual == other_root_reg.is_virtual)
                continue;

            if (intervals[root_reg].intersect(intervals[other_root_reg])) continue;

            for (const auto& seg : intervals[other_root_reg].segs) { intervals[root_reg].segs.push_back(seg); }
            intervals[root_reg].ref_cnt += intervals[other_root_reg].ref_cnt;

            std::sort(intervals[root_reg].segs.begin(),
                intervals[root_reg].segs.end(),
                [](const Interval::Segmant& a, const Interval::Segmant& b) { return a.start < b.start; });

            for (auto& src : copy_sources[other_root_reg])
            {
                if (!src.is_virtual)
                {
                    copy_sources[src].push_back(root_reg);
                    copy_sources[root_reg].push_back(src);
                }
            }

            coal_result[other_root_reg] = root_reg;

            intervals.erase(other_root_reg);
        }
    }

    for (auto& [block_id, block] : cur_func->cfg->blocks)
    {
        for (auto& inst : block->insts)
        {
            for (auto& reg : inst->getReadRegs())
            {
                if (reg->is_virtual && coal_result.find(*reg) != coal_result.end())
                {
                    *reg = findCoalescingRoot(coal_result, *reg);
                }
            }
            for (auto& reg : inst->getWriteRegs())
            {
                if (reg->is_virtual && coal_result.find(*reg) != coal_result.end())
                {
                    *reg = findCoalescingRoot(coal_result, *reg);
                }
            }
        }
    }
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

    std::unordered_set<Register> unique_registers;
    for (auto& [id, block] : current_func->cfg->blocks)
    {
        for (auto& inst : block->insts)
        {
            for (auto& reg_r : inst->getReadRegs()) unique_registers.insert(*reg_r);
            for (auto& reg_w : inst->getWriteRegs()) unique_registers.insert(*reg_w);
        }
    }

    MAX_REGISTERS = unique_registers.size();

    for (const auto& reg : unique_registers)
    {
        register_to_index[reg] = reg_index++;
        reverse_mapping.push_back(reg);
    }

    for (auto& [id, block] : current_func->cfg->blocks)
    {
        DEF[id].resize(MAX_REGISTERS);
        USE[id].resize(MAX_REGISTERS);

        for (auto& inst : block->insts)
        {
            for (auto& reg_r : inst->getReadRegs())
            {
                size_t index = register_to_index[*reg_r];
                if (!DEF[id].test(index)) { USE[id].set(index); }
            }
            for (auto& reg_w : inst->getWriteRegs())
            {
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

    Cele::dynamic_bitset temp_out(MAX_REGISTERS);
    Cele::dynamic_bitset temp_in(MAX_REGISTERS);
    Cele::dynamic_bitset temp_def_complement(MAX_REGISTERS);

    while (changed)
    {
        changed = false;

        for (auto& [id, block] : current_func->cfg->blocks)
        {
            auto& cur_block = block;
            auto  cur_id    = cur_block->label_num;

            temp_out.reset();
            for (auto& succ : current_func->cfg->graph[cur_id]) { temp_out |= IN[succ->label_num]; }

            if (temp_out != OUT[cur_id])
            {
                changed     = true;
                OUT[cur_id] = temp_out;
            }

            temp_def_complement = DEF[cur_id];
            temp_def_complement.flip();
            temp_in = OUT[cur_id];
            temp_in &= temp_def_complement;
            temp_in |= USE[cur_id];

            if (temp_in != IN[cur_id])
            {
                changed    = true;
                IN[cur_id] = temp_in;
            }
        }
    }
}
