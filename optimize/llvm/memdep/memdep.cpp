#include "llvm/memdep/memdep.h"
#include <queue>
#include <set>
#include <algorithm>
#include <iostream>
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include "cfg.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include "dom_analyzer.h"

namespace Analysis
{
    static CFG* getCfgByName(LLVMIR::IR* ir, const std::string& name)
    {
        for (auto& func : ir->functions)
        {
            if (func->func_def->func_name == name)
            {
                if (ir->cfg.count(func->func_def)) return ir->cfg.at(func->func_def);
            }
        }
        return nullptr;
    }

    MemorySSA::MemorySSA(LLVMIR::IR* ir, AliasAnalyser* aa) : ir(ir), alias_analyser(aa) {}

    void MemorySSA::run()
    {
        for (auto const& [func_def, cfg] : ir->cfg)
        {
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                for (auto inst : bb->insts) inst->block_id = bb->block_id;
            }
            buildMemorySSA(cfg);
        }
    }

    void MemorySSA::buildMemorySSA(CFG* cfg)
    {
        bb_memory_accesses[cfg].clear();
        bb_memory_phis[cfg].clear();
        location_accesses[cfg].clear();

        for (auto const& [id, bb] : cfg->block_id_to_block)
        {
            for (int i = 0; i < (int)bb->insts.size(); ++i)
            {
                auto inst = bb->insts[i];
                if (inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    auto access      = new MemoryUse(inst);
                    access->location = getMemoryLocation(inst);
                    access->block_id = bb->block_id;
                    access->inst_id  = i;
                    bb_memory_accesses[cfg][bb->block_id].push_back(access);
                    location_accesses[cfg][access->location].push_back(access);
                    inst_to_access[inst] = access;
                }
                else if (inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto access      = new MemoryDef(inst);
                    access->location = getMemoryLocation(inst);
                    access->block_id = bb->block_id;
                    access->inst_id  = i;
                    bb_memory_accesses[cfg][bb->block_id].push_back(access);
                    location_accesses[cfg][access->location].push_back(access);
                    inst_to_access[inst] = access;
                }
                else if (inst->opcode == LLVMIR::IROpCode::CALL)
                {
                    auto call_inst = dynamic_cast<LLVMIR::CallInst*>(inst);
                    auto call_cfg  = getCfgByName(ir, call_inst->func_name);
                    if (!call_cfg || !alias_analyser->isNoSideEffect(call_cfg))
                    {
                        auto access      = new MemoryDef(inst);
                        access->location = MemoryLocation{nullptr, LLVMIR::DataType::VOID};
                        access->block_id = bb->block_id;
                        access->inst_id  = i;
                        bb_memory_accesses[cfg][bb->block_id].push_back(access);
                        inst_to_access[inst] = access;
                    }
                }
            }
        }

        insertMemoryPhis(cfg);
        renameMemoryAccesses(cfg);
    }

    void MemorySSA::insertMemoryPhis(CFG* cfg)
    {
        std::set<MemoryLocation> all_locations;
        for (auto const& [loc, accesses] : location_accesses[cfg]) { all_locations.insert(loc); }

        for (const auto& location : all_locations)
        {
            std::set<int> defs;
            for (auto access : location_accesses[cfg][location])
            {
                if (access->type == DEF) { defs.insert(access->block_id); }
            }

            if (ir->DomTrees.count(cfg))
            {
                auto          dom_analyzer = ir->DomTrees.at(cfg);
                std::set<int> phi_blocks;

                for (int def_block : defs)
                {
                    if ((unsigned)def_block < dom_analyzer->dom_frontier.size())
                    {
                        for (int frontier_block : dom_analyzer->dom_frontier[def_block])
                        {
                            phi_blocks.insert(frontier_block);
                        }
                    }
                }

                for (int phi_block : phi_blocks)
                {
                    if (bb_memory_phis[cfg].find(phi_block) == bb_memory_phis[cfg].end())
                    {
                        bb_memory_phis[cfg][phi_block] = new MemoryPhi(phi_block);
                    }
                }
            }
        }
    }

    void MemorySSA::renameMemoryAccesses(CFG* cfg)
    {
        std::map<MemoryLocation, MemoryAccess*> current_def;

        for (const auto& location : location_accesses[cfg]) { current_def[location.first] = nullptr; }

        if (ir->DomTrees.count(cfg)) { processBlock(cfg, 0, current_def); }
    }

    void MemorySSA::processBlock(CFG* cfg, int bb_id, std::map<MemoryLocation, MemoryAccess*>& current_def)
    {
        auto saved_defs = current_def;

        if (bb_memory_phis[cfg].count(bb_id))
        {
            auto phi = bb_memory_phis[cfg][bb_id];
            for (const auto& [location, accesses] : location_accesses[cfg])
            {
                bool has_def_in_block = false;
                for (auto access : accesses)
                {
                    if (access->block_id == bb_id && access->type == DEF)
                    {
                        has_def_in_block = true;
                        break;
                    }
                }
                if (has_def_in_block) { current_def[location] = phi; }
            }
        }

        if (bb_memory_accesses[cfg].count(bb_id))
        {
            for (auto access : bb_memory_accesses[cfg][bb_id])
            {
                if (access->type == USE)
                {
                    access->defining_access = current_def[access->location];
                    if (access->defining_access) { access->defining_access->uses.push_back(access); }
                }
                else if (access->type == DEF) { current_def[access->location] = access; }
            }
        }

        if (ir->DomTrees.count(cfg))
        {
            auto dom_analyzer = ir->DomTrees.at(cfg);
            if ((unsigned)bb_id < dom_analyzer->dom_tree.size())
            {
                for (int child : dom_analyzer->dom_tree[bb_id])
                {
                    if (child != bb_id) { processBlock(cfg, child, current_def); }
                }
            }
        }

        current_def = saved_defs;
    }

    MemorySSA::MemoryLocation MemorySSA::getMemoryLocation(LLVMIR::Instruction* inst)
    {
        MemoryLocation loc;
        if (inst->opcode == LLVMIR::IROpCode::LOAD)
        {
            auto load_inst = dynamic_cast<LLVMIR::LoadInst*>(inst);
            loc.ptr        = load_inst->ptr;
            loc.size       = load_inst->type;
        }
        else if (inst->opcode == LLVMIR::IROpCode::STORE)
        {
            auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(inst);
            loc.ptr         = store_inst->ptr;
            loc.size        = store_inst->type;
        }
        return loc;
    }

    bool MemorySSA::mayAlias(const MemoryLocation& loc1, const MemoryLocation& loc2, CFG* cfg)
    {
        if (loc1.ptr == nullptr || loc2.ptr == nullptr) return true;
        return alias_analyser->queryAlias(loc1.ptr, loc2.ptr, cfg) != AliasAnalyser::NoAlias;
    }

    MemorySSA::MemoryAccess* MemorySSA::getDefiningAccess(LLVMIR::Instruction* inst, CFG* cfg)
    {
        if (inst_to_access.count(inst)) { return inst_to_access[inst]->defining_access; }
        return nullptr;
    }

    bool MemorySSA::isLoadSameMemory(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg)
    {
        if (inst1->opcode != LLVMIR::IROpCode::LOAD || inst2->opcode != LLVMIR::IROpCode::LOAD) return false;

        auto load1 = dynamic_cast<LLVMIR::LoadInst*>(inst1);
        auto load2 = dynamic_cast<LLVMIR::LoadInst*>(inst2);

        if (!load1 || !load2) return false;

        if (alias_analyser->queryAlias(load1->ptr, load2->ptr, cfg) != AliasAnalyser::MustAlias) return false;
        if (inst1->block_id != inst2->block_id) return false;

        auto bb   = cfg->block_id_to_block.at(inst1->block_id);
        int  idx1 = -1, idx2 = -1;

        for (int i = 0; i < (int)bb->insts.size(); ++i)
        {
            if (bb->insts[i] == inst1) idx1 = i;
            if (bb->insts[i] == inst2) idx2 = i;
        }

        if (idx1 == -1 || idx2 == -1) return false;

        if (idx1 > idx2) return false;

        for (int i = idx1 + 1; i < idx2; ++i)
        {
            auto inst = bb->insts[i];
            if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(inst);
                if (alias_analyser->queryAlias(load1->ptr, store_inst->ptr, cfg) != AliasAnalyser::NoAlias)
                    return false;
            }
            else if (inst->opcode == LLVMIR::IROpCode::CALL)
            {
                auto call_inst = dynamic_cast<LLVMIR::CallInst*>(inst);
                auto call_cfg  = getCfgByName(ir, call_inst->func_name);
                if (!call_cfg || !alias_analyser->isNoSideEffect(call_cfg)) return false;
            }
        }

        return true;
    }

    bool MemorySSA::isDepend(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg)
    {
        if (inst1->block_id != inst2->block_id) return true;

        if (inst1->opcode != LLVMIR::IROpCode::LOAD && inst1->opcode != LLVMIR::IROpCode::STORE) return false;
        if (inst2->opcode != LLVMIR::IROpCode::LOAD && inst2->opcode != LLVMIR::IROpCode::STORE) return false;

        auto bb   = cfg->block_id_to_block.at(inst1->block_id);
        int  idx1 = -1, idx2 = -1;

        for (int i = 0; i < (int)bb->insts.size(); ++i)
        {
            if (bb->insts[i] == inst1) idx1 = i;
            if (bb->insts[i] == inst2) idx2 = i;
        }

        if (idx1 == -1 || idx2 == -1 || idx1 >= idx2) return true;

        LLVMIR::Operand* p1;
        if (inst1->opcode == LLVMIR::IROpCode::LOAD)
            p1 = dynamic_cast<LLVMIR::LoadInst*>(inst1)->ptr;
        else if (inst1->opcode == LLVMIR::IROpCode::STORE)
            p1 = dynamic_cast<LLVMIR::StoreInst*>(inst1)->ptr;
        else
            return false;

        for (int i = idx1 + 1; i < idx2; ++i)
        {
            auto inst = bb->insts[i];
            if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                auto p2 = dynamic_cast<LLVMIR::StoreInst*>(inst)->ptr;
                if (alias_analyser->queryAlias(p1, p2, cfg) != AliasAnalyser::NoAlias) return true;
            }
            else if (inst->opcode == LLVMIR::IROpCode::CALL)
            {
                auto call_cfg = getCfgByName(ir, dynamic_cast<LLVMIR::CallInst*>(inst)->func_name);
                if (!call_cfg || !alias_analyser->isNoSideEffect(call_cfg)) return true;
            }
        }
        return false;
    }

    bool MemorySSA::haveMemAccessBetween(LLVMIR::Instruction* start, LLVMIR::Instruction* end, CFG* cfg)
    {
        if (start->block_id != end->block_id) return true;

        int  idx1 = -1, idx2 = -1;
        auto bb = cfg->block_id_to_block.at(start->block_id);
        for (int i = 0; i < (int)bb->insts.size(); ++i)
        {
            if (bb->insts[i] == start) idx1 = i;
            if (bb->insts[i] == end) idx2 = i;
        }

        if (idx1 == -1 || idx2 == -1 || idx1 >= idx2) return true;

        for (int i = idx1 + 1; i < idx2; ++i)
        {
            auto inst = bb->insts[i];
            if (inst->opcode == LLVMIR::IROpCode::LOAD || inst->opcode == LLVMIR::IROpCode::STORE ||
                inst->opcode == LLVMIR::IROpCode::CALL)
                return true;
        }
        return false;
    }

    MemoryDependenceAnalyser::MemoryDependenceAnalyser(LLVMIR::IR* ir, AliasAnalyser* aa) : ir(ir)
    {
        memory_ssa = new MemorySSA(ir, aa);
    }

    MemoryDependenceAnalyser::~MemoryDependenceAnalyser() { delete memory_ssa; }

    void MemoryDependenceAnalyser::run() { memory_ssa->run(); }

    bool MemoryDependenceAnalyser::isDepend(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg)
    {
        return memory_ssa->isDepend(inst1, inst2, cfg);
    }

    bool MemoryDependenceAnalyser::haveMemAccessBetween(LLVMIR::Instruction* start, LLVMIR::Instruction* end, CFG* cfg)
    {
        return memory_ssa->haveMemAccessBetween(start, end, cfg);
    }

    bool MemoryDependenceAnalyser::isLoadSameMemory(LLVMIR::Instruction* inst1, LLVMIR::Instruction* inst2, CFG* cfg)
    {
        return memory_ssa->isLoadSameMemory(inst1, inst2, cfg);
    }

}  // namespace Analysis
