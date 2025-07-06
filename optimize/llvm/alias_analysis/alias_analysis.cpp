#include "llvm/alias_analysis/alias_analysis.h"
#include <queue>
#include <vector>
#include <map>
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include "cfg.h"

namespace Analyser
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

    static PtrRegMemInfo getPtrInfo(LLVMIR::Operand* ptr, std::map<int, PtrRegMemInfo>& ptr_map)
    {
        if (ptr->type == LLVMIR::OperandType::REG)
        {
            auto reg_num = dynamic_cast<LLVMIR::RegOperand*>(ptr)->reg_num;
            if (ptr_map.find(reg_num) != ptr_map.end())
                return ptr_map.at(reg_num);
            else
            {
                // Treat an unknown pointer as pointing to unknown memory
                PtrRegMemInfo tmp;
                tmp.is_full_mem = true;
                tmp.possible_ptrs.push_back(ptr);
                return tmp;
            }
        }
        else if (ptr->type == LLVMIR::OperandType::GLOBAL)
        {
            PtrRegMemInfo tmp;
            tmp.possible_ptrs.push_back(ptr);
            return tmp;
        }
        else { assert(false); }
    }

    static bool isAlias(PtrRegMemInfo& ptr_info1, PtrRegMemInfo& ptr_info2)
    {
        if (ptr_info1.is_full_mem || ptr_info2.is_full_mem) return true;

        for (auto p1 : ptr_info1.possible_ptrs)
        {
            for (auto p2 : ptr_info2.possible_ptrs)
            {
                if (p1->getName() == p2->getName()) return true;
            }
        }
        return false;
    }

    AliasAnalyser::AliasAnalyser(LLVMIR::IR* ir) : ir(ir) {}

    bool AliasAnalyser::isSamePtrWithDiffConstIndex(LLVMIR::Operand* p1, LLVMIR::Operand* p2, CFG* cfg)
    {
        if (p1->type != LLVMIR::OperandType::REG || p2->type != LLVMIR::OperandType::REG) return false;

        auto r1 = dynamic_cast<LLVMIR::RegOperand*>(p1)->reg_num;
        auto r2 = dynamic_cast<LLVMIR::RegOperand*>(p2)->reg_num;

        if (cfg_result_map[cfg].find(r1) == cfg_result_map[cfg].end() ||
            cfg_result_map[cfg].find(r2) == cfg_result_map[cfg].end())
            return false;

        auto I1 = cfg_result_map[cfg][r1];
        auto I2 = cfg_result_map[cfg][r2];
        if (I1 == nullptr || I2 == nullptr || I1->opcode != LLVMIR::IROpCode::GETELEMENTPTR ||
            I2->opcode != LLVMIR::IROpCode::GETELEMENTPTR)
            return false;

        auto gep_inst1 = dynamic_cast<LLVMIR::GEPInst*>(I1);
        auto gep_inst2 = dynamic_cast<LLVMIR::GEPInst*>(I2);
        if (gep_inst1->idxs.size() != gep_inst2->idxs.size() || gep_inst1->idxs.empty()) return false;

        auto ptr1 = gep_inst1->base_ptr;
        auto ptr2 = gep_inst2->base_ptr;
        if (ptr1->getName() != ptr2->getName()) return false;

        auto idx1_op = gep_inst1->idxs.back();
        auto idx2_op = gep_inst2->idxs.back();

        if (idx1_op->type == LLVMIR::OperandType::IMMEI32 && idx2_op->type == LLVMIR::OperandType::IMMEI32)
        {
            auto const_idx1 = dynamic_cast<LLVMIR::ImmeI32Operand*>(idx1_op)->value;
            auto const_idx2 = dynamic_cast<LLVMIR::ImmeI32Operand*>(idx2_op)->value;
            if (const_idx1 != const_idx2) return true;
        }
        return false;
    }

    AliasAnalyser::AliasResult AliasAnalyser::queryAlias(LLVMIR::Operand* op1, LLVMIR::Operand* op2, CFG* cfg)
    {
        auto& ptr_map = ptr_reg_mem_map[cfg];

        auto ptr_info1 = getPtrInfo(op1, ptr_map);
        auto ptr_info2 = getPtrInfo(op2, ptr_map);

        if (ptr_info1.possible_ptrs.size() == 1 && ptr_info2.possible_ptrs.size() == 1)
        {
            if (isSamePtrWithDiffConstIndex(op1, op2, cfg)) return NoAlias;
        }

        if (isAlias(ptr_info1, ptr_info2)) return MustAlias;

        return NoAlias;
    }

    AliasAnalyser::ModRefResult AliasAnalyser::queryInstModRef(LLVMIR::Instruction* inst, LLVMIR::Operand* op, CFG* cfg)
    {
        auto& ptr_map = ptr_reg_mem_map.at(cfg);

        if (inst->opcode == LLVMIR::IROpCode::LOAD)
        {
            auto ptr       = dynamic_cast<LLVMIR::LoadInst*>(inst)->ptr;
            auto ptr_info1 = getPtrInfo(ptr, ptr_map);
            auto ptr_info2 = getPtrInfo(op, ptr_map);
            if (isAlias(ptr_info1, ptr_info2)) return ModRefResult::Ref;
        }
        else if (inst->opcode == LLVMIR::IROpCode::STORE)
        {
            auto ptr       = dynamic_cast<LLVMIR::StoreInst*>(inst)->ptr;
            auto ptr_info1 = getPtrInfo(ptr, ptr_map);
            auto ptr_info2 = getPtrInfo(op, ptr_map);
            if (isAlias(ptr_info1, ptr_info2)) return ModRefResult::Mod;
        }
        else if (inst->opcode == LLVMIR::IROpCode::CALL)
        {
            auto call_inst = dynamic_cast<LLVMIR::CallInst*>(inst);
            auto call_name = call_inst->func_name;
            auto call_cfg  = getCfgByName(ir, call_name);

            if (call_cfg == nullptr) return ModRefResult::ModRef;

            auto& rw_info = cfg_mem_rw_map.at(call_cfg);
            if (rw_info.have_external_call) return ModRefResult::ModRef;
            if (rw_info.isIndependent()) return ModRefResult::NoModRef;

            bool is_mod = false, is_ref = false;
            auto ptr_info1 = getPtrInfo(op, ptr_map);

            for (auto ptr2 : rw_info.read_ptrs)
            {
                PtrRegMemInfo real_ptr_info2;
                if (ptr2->type == LLVMIR::OperandType::GLOBAL) { real_ptr_info2 = getPtrInfo(ptr2, ptr_map); }
                else if (ptr2->type == LLVMIR::OperandType::REG)
                {
                    int ptr2_reg_no       = dynamic_cast<LLVMIR::RegOperand*>(ptr2)->reg_num;
                    auto [type, real_ptr] = call_inst->args.at(ptr2_reg_no);
                    real_ptr_info2        = getPtrInfo(real_ptr, ptr_map);
                }
                if (isAlias(ptr_info1, real_ptr_info2))
                {
                    is_ref = true;
                    break;
                }
            }

            for (auto ptr2 : rw_info.write_ptrs)
            {
                PtrRegMemInfo real_ptr_info2;
                if (ptr2->type == LLVMIR::OperandType::GLOBAL) { real_ptr_info2 = getPtrInfo(ptr2, ptr_map); }
                else if (ptr2->type == LLVMIR::OperandType::REG)
                {
                    int ptr2_reg_no       = dynamic_cast<LLVMIR::RegOperand*>(ptr2)->reg_num;
                    auto [type, real_ptr] = call_inst->args.at(ptr2_reg_no);
                    real_ptr_info2        = getPtrInfo(real_ptr, ptr_map);
                }
                if (isAlias(ptr_info1, real_ptr_info2))
                {
                    is_mod = true;
                    break;
                }
            }

            if (is_mod && is_ref) return ModRefResult::ModRef;
            if (is_mod) return ModRefResult::Mod;
            if (is_ref) return ModRefResult::Ref;
        }
        return ModRefResult::NoModRef;
    }

    static bool isFunctionArg(LLVMIR::IRFunction* func, LLVMIR::Operand* op)
    {
        if (op->type == LLVMIR::OperandType::REG)
        {
            auto reg_op = dynamic_cast<LLVMIR::RegOperand*>(op);
            for (const auto& arg : func->func_def->arg_regs)
            {
                if (dynamic_cast<LLVMIR::RegOperand*>(arg)->reg_num == reg_op->reg_num) return true;
            }
        }
        return false;
    }

    bool PtrRegMemInfo::pushPossiblePtr(LLVMIR::Operand* op)
    {
        for (auto ptr : possible_ptrs)
        {
            if (ptr->getName() == op->getName()) return false;
        }
        possible_ptrs.push_back(op);
        return true;
    }

    bool PtrRegMemInfo::insertNewPtrs(LLVMIR::Operand* op, std::map<int, PtrRegMemInfo>& ptr_map, CFG* cfg)
    {
        if (op->type == LLVMIR::OperandType::GLOBAL)
        {
            is_local = false;
            return pushPossiblePtr(op);
        }
        else if (op->type == LLVMIR::OperandType::REG)
        {
            if (isFunctionArg(cfg->func, op))
            {
                is_local    = false;
                is_full_mem = true;
                return pushPossiblePtr(op);
            }
            else
            {
                int reg_no = dynamic_cast<LLVMIR::RegOperand*>(op)->reg_num;
                if (ptr_map.count(reg_no) == 0)
                {
                    is_full_mem = true;
                    return pushPossiblePtr(op);
                }
                auto ptr_info = ptr_map.at(reg_no);
                bool changed  = false;
                for (auto ptr : ptr_info.possible_ptrs) changed |= pushPossiblePtr(ptr);
                is_full_mem |= ptr_info.is_full_mem;
                is_local &= ptr_info.is_local;
                return changed;
            }
        }
        return false;
    }

    bool FunctionMemRWInfo::insertNewReadPtrs(LLVMIR::Operand* op)
    {
        for (auto ptr : read_ptrs)
        {
            if (ptr->getName() == op->getName()) return false;
        }
        read_ptrs.push_back(op);
        return true;
    }

    bool FunctionMemRWInfo::insertNewWritePtrs(LLVMIR::Operand* op)
    {
        for (auto ptr : write_ptrs)
        {
            if (ptr->getName() == op->getName()) return false;
        }
        write_ptrs.push_back(op);
        return true;
    }

    bool FunctionMemRWInfo::insertNewReadPtrs(std::vector<LLVMIR::Operand*> ops)
    {
        bool changed = false;
        for (auto ptr : ops) changed |= insertNewReadPtrs(ptr);
        return changed;
    }

    bool FunctionMemRWInfo::insertNewWritePtrs(std::vector<LLVMIR::Operand*> ops)
    {
        bool changed = false;
        for (auto ptr : ops) changed |= insertNewWritePtrs(ptr);
        return changed;
    }

    bool FunctionMemRWInfo::mergeCall(
        LLVMIR::CallInst* call_inst, FunctionMemRWInfo& rw_info, std::map<int, PtrRegMemInfo>& ptr_map)
    {
        bool changed = false;

        for (auto op : rw_info.read_ptrs)
        {
            if (op->type == LLVMIR::OperandType::GLOBAL) { changed |= insertNewReadPtrs(op); }
            else if (op->type == LLVMIR::OperandType::REG)
            {
                int reg_no = dynamic_cast<LLVMIR::RegOperand*>(op)->reg_num;
                assert(reg_no < (int)call_inst->args.size());

                auto [type, ptr] = call_inst->args.at(reg_no);
                assert(ptr->type == LLVMIR::OperandType::REG);

                auto ptr_reg_no = dynamic_cast<LLVMIR::RegOperand*>(ptr)->reg_num;
                if (ptr_map.at(ptr_reg_no).is_local) continue;
                changed |= insertNewReadPtrs(ptr_map.at(ptr_reg_no).possible_ptrs);
            }
        }

        for (auto op : rw_info.write_ptrs)
        {
            if (op->type == LLVMIR::OperandType::GLOBAL) { changed |= insertNewWritePtrs(op); }
            else if (op->type == LLVMIR::OperandType::REG)
            {
                int reg_no = dynamic_cast<LLVMIR::RegOperand*>(op)->reg_num;
                assert(reg_no < (int)call_inst->args.size());

                auto [type, ptr] = call_inst->args.at(reg_no);
                assert(ptr->type == LLVMIR::OperandType::REG);

                auto ptr_reg_no = dynamic_cast<LLVMIR::RegOperand*>(ptr)->reg_num;
                if (ptr_map.at(ptr_reg_no).is_local) continue;
                changed |= insertNewWritePtrs(ptr_map.at(ptr_reg_no).possible_ptrs);
            }
        }
        return changed;
    }

    void AliasAnalyser::run()
    {
        cfg_result_map.clear();
        for (auto const& [func_def, cfg] : ir->cfg)
        {
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                for (auto inst : bb->insts)
                {
                    int v = inst->GetResultReg();
                    if (v != -1) cfg_result_map[cfg][v] = inst;
                }
            }
        }

        ptr_reg_mem_map.clear();
        cfg_mem_rw_map.clear();
        for (auto const& [func_def, cfg] : ir->cfg) analyzeCFG(cfg);

        std::map<CFG*, std::vector<LLVMIR::CallInst*>> call_map;
        for (auto const& [func_def, cfg] : ir->cfg)
        {
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                for (auto inst : bb->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::CALL)
                        call_map[cfg].push_back(dynamic_cast<LLVMIR::CallInst*>(inst));
                }
            }
        }

        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto const& [func_def, cfg] : ir->cfg)
            {
                if (cfg_mem_rw_map[cfg].have_external_call) continue;

                auto& ptr_map = ptr_reg_mem_map[cfg];

                if (call_map.find(cfg) == call_map.end()) continue;

                for (auto call_inst : call_map.at(cfg))
                {
                    std::string call_name = call_inst->func_name;
                    auto        call_cfg  = getCfgByName(ir, call_name);
                    if (call_cfg == nullptr)
                    {
                        cfg_mem_rw_map[cfg].have_external_call = true;
                        changed                                = true;
                        continue;
                    }

                    auto& call_info = cfg_mem_rw_map.at(call_cfg);
                    if (call_info.have_external_call)
                    {
                        cfg_mem_rw_map[cfg].have_external_call = true;
                        changed                                = true;
                        continue;
                    }
                    changed |= cfg_mem_rw_map[cfg].mergeCall(call_inst, call_info, ptr_map);
                }
            }
        }
    }

    void AliasAnalyser::analyzeCFG(CFG* cfg)
    {
        std::map<int, PtrRegMemInfo> ptr_map;
        FunctionMemRWInfo            rw_info;
        LLVMIR::FuncDefInst*         func_def = cfg->func->func_def;

        for (unsigned i = 0; i < func_def->arg_regs.size(); ++i)
        {
            if (func_def->arg_types[i] == LLVMIR::DataType::PTR)
            {
                auto arg_op                 = func_def->arg_regs[i];
                int  reg_no                 = dynamic_cast<LLVMIR::RegOperand*>(arg_op)->reg_num;
                ptr_map[reg_no].is_full_mem = true;
                ptr_map[reg_no].is_local    = false;
                ptr_map[reg_no].possible_ptrs.push_back(arg_op);
            }
        }

        for (auto inst : cfg->block_id_to_block.at(0)->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::ALLOCA)
            {
                auto alloca_inst = dynamic_cast<LLVMIR::AllocInst*>(inst);
                int  def_reg     = alloca_inst->GetResultReg();
                ptr_map[def_reg].possible_ptrs.push_back(alloca_inst->res);
            }
        }

        bool changed = true;
        while (changed)
        {
            changed = false;
            std::queue<LLVMIR::IRBlock*>     q;
            std::map<LLVMIR::IRBlock*, bool> vis;
            q.push(cfg->block_id_to_block.at(0));

            while (!q.empty())
            {
                auto bb = q.front();
                q.pop();
                if (vis[bb]) continue;
                vis[bb] = true;

                for (auto inst : bb->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                    {
                        auto gep_inst      = dynamic_cast<LLVMIR::GEPInst*>(inst);
                        auto addr          = gep_inst->base_ptr;
                        auto result_reg_no = gep_inst->GetResultReg();
                        changed |= ptr_map[result_reg_no].insertNewPtrs(addr, ptr_map, cfg);
                    }
                    else if (inst->opcode == LLVMIR::IROpCode::PHI)
                    {
                        auto phi_inst      = dynamic_cast<LLVMIR::PhiInst*>(inst);
                        auto result_reg_no = phi_inst->GetResultReg();
                        if (phi_inst->type == LLVMIR::DataType::PTR)
                        {
                            for (auto const& [val, label] : phi_inst->vals_for_labels)
                                changed |= ptr_map[result_reg_no].insertNewPtrs(val, ptr_map, cfg);
                        }
                    }
                }
                for (auto bb_succ : cfg->G[bb->block_id]) q.push(bb_succ);
            }
        }

        for (auto const& [id, bb] : cfg->block_id_to_block)
        {
            for (auto inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto store_inst = dynamic_cast<LLVMIR::StoreInst*>(inst);
                    auto ptr        = store_inst->ptr;
                    if (ptr->type == LLVMIR::OperandType::GLOBAL) { rw_info.insertNewWritePtrs(ptr); }
                    else if (ptr->type == LLVMIR::OperandType::REG)
                    {
                        auto ptr_reg_no = dynamic_cast<LLVMIR::RegOperand*>(ptr)->reg_num;
                        if (ptr_map.count(ptr_reg_no) && ptr_map.at(ptr_reg_no).is_local) continue;
                        if (ptr_map.count(ptr_reg_no)) rw_info.insertNewWritePtrs(ptr_map.at(ptr_reg_no).possible_ptrs);
                    }
                    else { assert(false); }
                }
                else if (inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    auto load_inst = dynamic_cast<LLVMIR::LoadInst*>(inst);
                    auto ptr       = load_inst->ptr;
                    if (ptr->type == LLVMIR::OperandType::GLOBAL) { rw_info.insertNewReadPtrs(ptr); }
                    else if (ptr->type == LLVMIR::OperandType::REG)
                    {
                        auto ptr_reg_no = dynamic_cast<LLVMIR::RegOperand*>(ptr)->reg_num;
                        if (ptr_map.count(ptr_reg_no) && ptr_map.at(ptr_reg_no).is_local) continue;
                        if (ptr_map.count(ptr_reg_no)) rw_info.insertNewReadPtrs(ptr_map.at(ptr_reg_no).possible_ptrs);
                    }
                    else { assert(false); }
                }
            }
        }

        ptr_reg_mem_map[cfg] = ptr_map;
        cfg_mem_rw_map[cfg]  = rw_info;
    }

    bool AliasAnalyser::isLocalPtr(CFG* cfg, LLVMIR::Operand* ptr)
    {
        if (ptr->type == LLVMIR::OperandType::REG)
        {
            auto reg_no = dynamic_cast<LLVMIR::RegOperand*>(ptr)->reg_num;
            if (ptr_reg_mem_map.at(cfg).count(reg_no)) return ptr_reg_mem_map.at(cfg).at(reg_no).is_local;
        }
        return false;
    }

}  // namespace Analyser
