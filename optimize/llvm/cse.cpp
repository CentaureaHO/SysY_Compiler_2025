#include "llvm/cse.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"
#include <functional>
#include <algorithm>
#include "llvm_ir/instruction.h"

namespace Transform
{
    static CFG* get_cfg_by_name(LLVMIR::IR* ir, const std::string& name)
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

    CSEPass::CSEPass(LLVMIR::IR* ir, Analysis::AliasAnalyser* aa, Analysis::MemoryDependenceAnalyser* md)
        : Pass(ir), alias_analyser(aa), memdep_analyser(md)
    {}

    InstCSEInfo CSEPass::getCSEInfo(LLVMIR::Instruction* inst)
    {
        InstCSEInfo ans;
        ans.opcode = inst->opcode;

        // 获取CSE操作数列表
        auto operands = inst->GetCSEOperands();

        // 处理需要额外信息的指令类型
        if (inst->opcode == LLVMIR::IROpCode::ICMP)
        {
            auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(inst);
            ans.icmp_cond  = icmp_inst->cond;
        }
        else if (inst->opcode == LLVMIR::IROpCode::FCMP)
        {
            auto fcmp_inst = dynamic_cast<LLVMIR::FcmpInst*>(inst);
            ans.fcmp_cond  = fcmp_inst->cond;
        }
        else if (inst->opcode == LLVMIR::IROpCode::CALL)
        {
            auto call_inst = dynamic_cast<LLVMIR::CallInst*>(inst);
            ans.operand_list.push_back(call_inst->func_name);
        }

        for (auto op : operands) ans.operand_list.push_back(op->getName());

        if (inst->opcode == LLVMIR::IROpCode::ADD || inst->opcode == LLVMIR::IROpCode::FMUL ||
            inst->opcode == LLVMIR::IROpCode::MUL || inst->opcode == LLVMIR::IROpCode::FADD)
        {
            std::sort(ans.operand_list.begin(), ans.operand_list.end());
        }
        else if (inst->opcode == LLVMIR::IROpCode::ICMP)
        {
            if (ans.operand_list.size() == 2 && ans.operand_list[0] > ans.operand_list[1])
            {
                std::swap(ans.operand_list[0], ans.operand_list[1]);
                auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(inst);
                switch (icmp_inst->cond)
                {
                    case LLVMIR::IcmpCond::SGT: ans.icmp_cond = LLVMIR::IcmpCond::SLT; break;
                    case LLVMIR::IcmpCond::SGE: ans.icmp_cond = LLVMIR::IcmpCond::SLE; break;
                    case LLVMIR::IcmpCond::SLT: ans.icmp_cond = LLVMIR::IcmpCond::SGT; break;
                    case LLVMIR::IcmpCond::SLE: ans.icmp_cond = LLVMIR::IcmpCond::SGE; break;
                    default: break;  // EQ, NE are symmetric
                }
            }
        }
        else if (inst->opcode == LLVMIR::IROpCode::FCMP)
        {
            if (ans.operand_list.size() == 2 && ans.operand_list[0] > ans.operand_list[1])
            {
                std::swap(ans.operand_list[0], ans.operand_list[1]);
                auto fcmp_inst = dynamic_cast<LLVMIR::FcmpInst*>(inst);
                switch (fcmp_inst->cond)
                {
                    case LLVMIR::FcmpCond::OGT: ans.fcmp_cond = LLVMIR::FcmpCond::OLT; break;
                    case LLVMIR::FcmpCond::OGE: ans.fcmp_cond = LLVMIR::FcmpCond::OLE; break;
                    case LLVMIR::FcmpCond::OLT: ans.fcmp_cond = LLVMIR::FcmpCond::OGT; break;
                    case LLVMIR::FcmpCond::OLE: ans.fcmp_cond = LLVMIR::FcmpCond::OGE; break;
                    default: break;  // OEQ, ONE, UNO are symmetric
                }
            }
        }

        return ans;
    }

    bool CSEPass::isNotCSE(LLVMIR::Instruction* inst)
    {
        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::PHI:
            case LLVMIR::IROpCode::BR_COND:
            case LLVMIR::IROpCode::BR_UNCOND:
            case LLVMIR::IROpCode::ALLOCA:
            case LLVMIR::IROpCode::RET:
            case LLVMIR::IROpCode::ICMP:  // ICMP/FCMP are handled by branch CSE
            case LLVMIR::IROpCode::FCMP: return true;
            default: return false;
        }
    }

    void CSEPass::init()
    {
        for (auto const& [func_def, cfg] : ir->cfg)
        {
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                auto& insts = bb->insts;
                for (size_t i = 0; i < insts.size(); ++i)
                {
                    if (insts[i]->opcode != LLVMIR::IROpCode::STORE) continue;
                    auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(insts[i]);
                    if (store_inst->val->type == LLVMIR::OperandType::IMMEI32)
                    {
                        auto imme_val = dynamic_cast<LLVMIR::ImmeI32Operand*>(store_inst->val);
                        auto new_reg  = getRegOperand(cfg->func->max_reg++);
                        auto new_add  = new LLVMIR::ArithmeticInst(
                            LLVMIR::IROpCode::ADD, LLVMIR::DataType::I32, imme_val, getImmeI32Operand(0), new_reg);
                        insts.insert(insts.begin() + i, new_add);
                        store_inst->val = new_reg;
                        ++i;
                    }
                    else if (store_inst->val->type == LLVMIR::OperandType::IMMEF32)
                    {
                        auto imme_val = dynamic_cast<LLVMIR::ImmeF32Operand*>(store_inst->val);
                        auto new_reg  = getRegOperand(cfg->func->max_reg++);
                        auto new_fadd = new LLVMIR::ArithmeticInst(
                            LLVMIR::IROpCode::FADD, LLVMIR::DataType::F32, imme_val, getImmeF32Operand(0.0), new_reg);
                        insts.insert(insts.begin() + i, new_fadd);
                        store_inst->val = new_reg;
                        ++i;
                    }
                }
            }
        }
    }

    void CSEPass::callKill(LLVMIR::Instruction* inst, std::map<InstCSEInfo, int>& call_inst_map,
        std::map<InstCSEInfo, int>& load_inst_map, std::set<LLVMIR::Instruction*>& call_inst_set,
        std::set<LLVMIR::Instruction*>& load_inst_set, CFG* cfg)
    {
        auto call_inst = dynamic_cast<LLVMIR::CallInst*>(inst);
        auto call_cfg  = get_cfg_by_name(ir, call_inst->func_name);

        if (!call_cfg || alias_analyser->haveExternalCall(call_cfg))
        {
            load_inst_map.clear();
            load_inst_set.clear();
            call_inst_map.clear();
            call_inst_set.clear();
            return;
        }

        for (auto it = load_inst_set.begin(); it != load_inst_set.end();)
        {
            auto load_inst = dynamic_cast<LLVMIR::LoadInst*>(*it);
            auto ptr       = load_inst->ptr;
            auto result    = alias_analyser->queryInstModRef(inst, ptr, cfg);
            if (result == Analysis::AliasAnalyser::Mod || result == Analysis::AliasAnalyser::ModRef)
            {
                load_inst_map.erase(getCSEInfo(*it));
                it = load_inst_set.erase(it);
            }
            else
                ++it;
        }

        auto write_ptrs = alias_analyser->getWritePtrs(call_cfg);

        for (auto it = call_inst_set.begin(); it != call_inst_set.end();)
        {
            bool killed = false;
            for (auto ptr : write_ptrs)
            {
                if (alias_analyser->queryInstModRef(*it, ptr, cfg) != Analysis::AliasAnalyser::NoModRef)
                {
                    killed = true;
                    break;
                }
            }
            if (killed)
            {
                call_inst_map.erase(getCSEInfo(*it));
                it = call_inst_set.erase(it);
            }
            else
                ++it;
        }
    }

    void CSEPass::storeKill(LLVMIR::Instruction* inst, std::map<InstCSEInfo, int>& call_inst_map,
        std::map<InstCSEInfo, int>& load_inst_map, std::set<LLVMIR::Instruction*>& call_inst_set,
        std::set<LLVMIR::Instruction*>& load_inst_set, CFG* cfg)
    {
        auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(inst);
        auto ptr        = store_inst->ptr;

        for (auto it = load_inst_set.begin(); it != load_inst_set.end();)
        {
            if (alias_analyser->queryAlias(ptr, dynamic_cast<LLVMIR::LoadInst*>(*it)->ptr, cfg) !=
                Analysis::AliasAnalyser::NoAlias)
            {
                load_inst_map.erase(getCSEInfo(*it));
                it = load_inst_set.erase(it);
            }
            else
                ++it;
        }

        for (auto it = call_inst_set.begin(); it != call_inst_set.end();)
        {
            if (alias_analyser->queryInstModRef(*it, ptr, cfg) != Analysis::AliasAnalyser::NoModRef)
            {
                call_inst_map.erase(getCSEInfo(*it));
                it = call_inst_set.erase(it);
            }
            else
                ++it;
        }
    }

    bool CSEPass::basicBlockCSE(
        LLVMIR::IRBlock* bb, std::map<int, int>& reg_replace_map, std::set<LLVMIR::Instruction*>& erase_set, CFG* cfg)
    {
        bool                           changed = false;
        std::map<InstCSEInfo, int>     load_inst_map;
        std::map<InstCSEInfo, int>     call_inst_map;
        std::map<InstCSEInfo, int>     inst_map;
        std::set<LLVMIR::Instruction*> load_inst_set;
        std::set<LLVMIR::Instruction*> call_inst_set;

        for (auto inst : bb->insts)
        {
            if (isNotCSE(inst)) continue;

            if (inst->opcode == LLVMIR::IROpCode::CALL)
            {
                auto call_inst = dynamic_cast<LLVMIR::CallInst*>(inst);
                auto call_cfg  = get_cfg_by_name(ir, call_inst->func_name);
                if (call_cfg && alias_analyser->isNoSideEffect(call_cfg))
                {
                    auto info = getCSEInfo(inst);
                    if (call_inst_map.count(info))
                    {
                        erase_set.insert(inst);
                        reg_replace_map[inst->GetResultReg()] = call_inst_map[info];
                        changed                               = true;
                    }
                    else
                    {
                        call_inst_set.insert(inst);
                        call_inst_map[info] = inst->GetResultReg();
                    }
                }
                else
                    callKill(inst, call_inst_map, load_inst_map, call_inst_set, load_inst_set, cfg);
            }
            else if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                storeKill(inst, call_inst_map, load_inst_map, call_inst_set, load_inst_set, cfg);
                auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(inst);
                // Value propagation from store to load
                if (auto val_reg = dynamic_cast<LLVMIR::RegOperand*>(store_inst->val))
                {
                    int val_reg_num = val_reg->reg_num;
                    if (reg_replace_map.count(val_reg_num)) val_reg_num = reg_replace_map.at(val_reg_num);
                    auto load_equivalent =
                        new LLVMIR::LoadInst(store_inst->type, store_inst->ptr, getRegOperand(val_reg_num));
                    auto info           = getCSEInfo(load_equivalent);
                    load_inst_map[info] = val_reg_num;
                    load_inst_set.insert(load_equivalent);
                }
            }
            else if (inst->opcode == LLVMIR::IROpCode::LOAD)
            {
                auto info = getCSEInfo(inst);
                if (load_inst_map.count(info))
                {
                    erase_set.insert(inst);
                    reg_replace_map[inst->GetResultReg()] = load_inst_map[info];
                    changed                               = true;
                }
                else
                {
                    load_inst_set.insert(inst);
                    load_inst_map[info] = inst->GetResultReg();
                }
            }
            else
            {
                // Other instructions

                auto info = getCSEInfo(inst);
                if (inst_map.count(info))
                {
                    erase_set.insert(inst);
                    reg_replace_map[inst->GetResultReg()] = inst_map[info];
                    changed                               = true;
                }
                else
                    inst_map[info] = inst->GetResultReg();
            }
        }
        return changed;
    }

    void CSEPass::domTreeWalkCSE(CFG* cfg)
    {
        current_cfg  = cfg;
        bool changed = true;
        while (changed)
        {
            changed = false;
            erase_set.clear();
            inst_cse_map.clear();
            load_cse_map.clear();
            reg_replace_map.clear();

            if (ir->DomTrees.count(cfg)) dfsDomTree(0);

            if (!erase_set.empty())
            {
                changed = true;
                for (auto const& [id, bb] : cfg->block_id_to_block)
                {
                    bb->insts.erase(std::remove_if(bb->insts.begin(),
                                        bb->insts.end(),
                                        [&](LLVMIR::Instruction* i) { return erase_set.count(i); }),
                        bb->insts.end());
                }
                for (auto const& [id, bb] : cfg->block_id_to_block)
                {
                    for (auto inst : bb->insts) inst->Rename(reg_replace_map);
                }
            }
        }
    }

    void CSEPass::dfsDomTree(int bbid)
    {
        auto                       now = current_cfg->block_id_to_block.at(bbid);
        std::vector<InstCSEInfo>   tmp_cse;
        std::map<InstCSEInfo, int> tmp_load;

        for (auto inst : now->insts)
        {
            if (isNotCSE(inst)) continue;

            if (inst->opcode == LLVMIR::IROpCode::LOAD)
            {
                auto info      = getCSEInfo(inst);
                bool cse_found = false;
                if (load_cse_map.count(info))
                {
                    for (auto inst2 : load_cse_map[info])
                    {
                        if (!memdep_analyser->isLoadSameMemory(inst, inst2, current_cfg)) continue;

                        erase_set.insert(inst);
                        if (inst2->opcode == LLVMIR::IROpCode::STORE)
                        {
                            auto store_inst2 = dynamic_cast<LLVMIR::StoreInst*>(inst2);
                            reg_replace_map[inst->GetResultReg()] =
                                dynamic_cast<LLVMIR::RegOperand*>(store_inst2->val)->reg_num;
                        }
                        else
                            reg_replace_map[inst->GetResultReg()] = inst2->GetResultReg();
                        cse_found = true;
                        break;
                    }
                }
                if (!cse_found)
                {
                    load_cse_map[info].push_back(inst);
                    tmp_load[info]++;
                }
            }
            else if (inst->opcode == LLVMIR::IROpCode::STORE)
            {
                auto info = getCSEInfo(inst);  // Store as source for load CSE
                load_cse_map[info].push_back(inst);
                tmp_load[info]++;
            }
            else if (inst->opcode == LLVMIR::IROpCode::CALL)
            {

                auto call_inst = dynamic_cast<LLVMIR::CallInst*>(inst);
                auto call_cfg  = get_cfg_by_name(ir, call_inst->func_name);
                if (call_cfg && alias_analyser->isIndependent(call_cfg))
                {
                    auto info = getCSEInfo(inst);
                    if (inst_cse_map.count(info))
                    {
                        erase_set.insert(inst);
                        reg_replace_map[inst->GetResultReg()] = inst_cse_map[info];
                    }
                    else
                    {
                        inst_cse_map[info] = inst->GetResultReg();
                        tmp_cse.push_back(info);
                    }
                }
            }
            else
            {
                // Other instructions

                auto info = getCSEInfo(inst);
                if (inst_cse_map.count(info))
                {
                    erase_set.insert(inst);
                    reg_replace_map[inst->GetResultReg()] = inst_cse_map[info];
                }
                else
                {
                    inst_cse_map[info] = inst->GetResultReg();
                    tmp_cse.push_back(info);
                }
            }
        }

        if (ir->DomTrees.count(current_cfg))
        {
            auto& dom_tree = ir->DomTrees.at(current_cfg)->dom_tree;
            if ((unsigned)bbid < dom_tree.size())
            {
                for (auto child_bbid : dom_tree[bbid])
                {
                    if (child_bbid != bbid) dfsDomTree(child_bbid);
                }
            }
        }

        for (auto const& info : tmp_cse) inst_cse_map.erase(info);
        for (auto const& [info, count] : tmp_load)
        {
            for (int i = 0; i < count; ++i) load_cse_map[info].pop_back();
            if (load_cse_map[info].empty()) load_cse_map.erase(info);
        }
    }

    void CSEPass::Execute()
    {
        init();
        for (auto const& [func_def, cfg] : ir->cfg)
        {
            bool changed = true;
            while (changed)
            {
                changed = false;
                std::map<int, int>             reg_replace_map;
                std::set<LLVMIR::Instruction*> erase_set;
                for (auto const& [id, bb] : cfg->block_id_to_block)
                {
                    changed |= basicBlockCSE(bb, reg_replace_map, erase_set, cfg);
                }

                if (changed)
                {
                    for (auto const& [id, bb] : cfg->block_id_to_block)
                    {
                        bb->insts.erase(std::remove_if(bb->insts.begin(),
                                            bb->insts.end(),
                                            [&](LLVMIR::Instruction* i) { return erase_set.count(i); }),
                            bb->insts.end());
                    }
                    for (auto const& [id, bb] : cfg->block_id_to_block)
                    {
                        for (auto inst : bb->insts) inst->Rename(reg_replace_map);
                    }
                }
            }
            domTreeWalkCSE(cfg);
        }
    }

}  // namespace Transform
