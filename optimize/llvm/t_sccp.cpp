#include "llvm/t_sccp.h"
#include "llvm_ir/instruction.h"
#include "cfg.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <queue>
#include <sstream>

// #define DBGMODE

#ifdef DBGMODE
template <typename... Args>
void dbg_impl(Args&&... args)
{
    ((std::cout << args), ...);
    std::cout << std::endl;
}
#define DBGINFO(...) dbg_impl(__VA_ARGS__)
#else
#define DBGINFO(...) \
    do {             \
    } while (0)
#endif

namespace Transform
{
    std::string MemoryLocation::toString() const
    {
        if (!isValid()) return "INVALID";

        std::string result = "Base:" + std::to_string(reinterpret_cast<uintptr_t>(base_ptr));
        result += "+";
        result += std::to_string(element_offset);

        return result;
    }

    /**
     * 格值meet操作的实现
     *   - TOP intersect X -> X (任何值都比未定义更精确)
     *   - BOTTOM intersect X -> BOTTOM (BOTTOM是最低状态，任何值与之相遇都变为BOTTOM)
     *   - CONST(a) intersect CONST(b) -> a==b ? CONST(a) : BOTTOM (常量相遇，值不同无法确定结果)
     */
    LatticeValue LatticeValue::meet(const LatticeValue& other) const
    {
        // TOP intersect X -> X
        if (this->isTop()) return other;
        if (other.isTop()) return *this;

        // BOTTOM intersect X -> BOTTOM
        if (this->isBottom() || other.isBottom()) return LatticeValue::createBottom();

        // CONST intersect CONST
        if (this->isConstant() && other.isConstant())
        {
            if (this->type_ != other.type_) return LatticeValue::createBottom();

            if (this->type_ == ValueType::INTEGER)
            {
                if (this->value_.int_val == other.value_.int_val)
                    return *this;
                else
                    return LatticeValue::createBottom();
            }
            else if (this->type_ == ValueType::FLOAT)
            {
                if (this->value_.float_val == other.value_.float_val)
                    return *this;
                else
                    return LatticeValue::createBottom();
            }
        }

        return LatticeValue::createBottom();
    }

    bool LatticeValue::operator==(const LatticeValue& other) const
    {
        if (status_ != other.status_) return false;
        if (type_ != other.type_) return false;

        // TOP/BOTTOM状态下，不关心具体值，直接认为相等
        if (!isConstant()) return true;

        if (type_ == ValueType::INTEGER)
            return value_.int_val == other.value_.int_val;
        else if (type_ == ValueType::FLOAT)
            return value_.float_val == other.value_.float_val;

        return true;
    }

    TSCCPPass::TSCCPPass(LLVMIR::IR* ir) : Pass(ir), current_cfg_(nullptr)
    {
        instruction_visitor_ = std::make_unique<InstructionVisitor>(this);
    }

    TSCCPPass::~TSCCPPass() = default;

    void TSCCPPass::Execute()
    {
        DBGINFO("=== TSCCP Pass Starting ===");
        for (const auto& [func_def, cfg] : ir->cfg)
        {
            DBGINFO("Processing function: ", func_def->func_name);
            current_cfg_ = cfg;

            value_map_.clear();
            use_chains_.clear();
            def_map_.clear();
            cfg_worklist_.clear();
            ssa_worklist_.clear();
            executable_edges_.clear();
            memory_state_.clear();
            memory_modified_in_loops_.clear();
            base_modified_in_loops_.clear();

            buildDefUseChains(cfg);

            for (const auto& [block_id, block] : cfg->block_id_to_block)
                for (auto* inst : block->insts) value_map_[inst] = LatticeValue();

            // 函数参数来源未知，设为BOTTOM；若前文执行过函数内联，内联部分参数可正常传播
            for (auto* arg_reg : cfg->func->func_def->arg_regs)
            {
                if (arg_reg->type != LLVMIR::OperandType::REG) continue;

                auto* reg_operand = static_cast<LLVMIR::RegOperand*>(arg_reg);
                auto  it          = def_map_.find(reg_operand->reg_num);
                if (it != def_map_.end()) value_map_[it->second] = LatticeValue::createBottom();
            }

            addToCFGWorklist(-1, 0);
            runSCCPAnalysis(cfg);
            propagateConstants(cfg);
            eliminateDeadInstructions(cfg);
        }
    }

    /**
     *   交替处理CFG工作列表和SSA工作列表，
     *   直到两个列表都为空，此时状态收敛。
     *   - CFG工作列表：处理控制流，确定可达的基本块和指令。
     *   - SSA工作列表：处理数据流，当一条指令的值改变时，将其使用者加入列表重新计算。
     */
    void TSCCPPass::runSCCPAnalysis(CFG* cfg)
    {
        size_t cfg_idx = 0;
        size_t ssa_idx = 0;

        while (cfg_idx < cfg_worklist_.size() || ssa_idx < ssa_worklist_.size())
        {
            // 优先处理控制流，确定可达代码
            while (cfg_idx < cfg_worklist_.size())
            {
                auto [from_bb, to_bb] = cfg_worklist_[cfg_idx++];

                if (from_bb != -1 && isEdgeExecutable(from_bb, to_bb)) continue;

                markEdgeExecutable(from_bb, to_bb);

                auto block_it = cfg->block_id_to_block.find(to_bb);
                if (block_it == cfg->block_id_to_block.end()) continue;

                // 访问新标记为可达的基本块中的所有指令
                for (auto* inst : block_it->second->insts) instruction_visitor_->visit(inst);
            }  // end while(cfg_idx < cfg_worklist_.size())

            // 处理因数据值变化而需要更新的指令
            while (ssa_idx < ssa_worklist_.size())
            {
                auto* inst = ssa_worklist_[ssa_idx++];

                // 仅当指令所在的基本块是可达的时，才访问它
                bool is_reachable = false;
                int  inst_block   = inst->block_id;

                if (inst_block == 0)
                    is_reachable = true;
                else if (inst_block < static_cast<int>(cfg->invG_id.size()))
                    for (int pred_id : cfg->invG_id[inst_block])
                    {
                        if (!isEdgeExecutable(pred_id, inst_block)) continue;

                        is_reachable = true;
                        break;
                    }  // end for

                if (is_reachable) instruction_visitor_->visit(inst);
            }  // end while(ssa_idx < ssa_worklist_.size())
        }  // end while (cfg_idx < cfg_worklist_.size() || ssa_idx < ssa_worklist_.size())
    }

    void TSCCPPass::buildDefUseChains(CFG* cfg)
    {
        for (auto* arg_reg : cfg->func->func_def->arg_regs)
        {
            if (arg_reg->type != LLVMIR::OperandType::REG) continue;

            auto* reg_operand              = static_cast<LLVMIR::RegOperand*>(arg_reg);
            def_map_[reg_operand->reg_num] = cfg->func->func_def;
        }

        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                inst->block_id = block_id;

                int result_reg = inst->GetResultReg();
                if (result_reg != -1) def_map_[result_reg] = inst;
            }
        }

        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                std::vector<int> used_regs = inst->GetUsedRegs();
                for (int reg_num : used_regs)
                {
                    auto it = def_map_.find(reg_num);
                    if (it != def_map_.end()) use_chains_[it->second].push_back(inst);
                }
            }
        }
    }

    void TSCCPPass::addToSSAWorklist(LLVMIR::Instruction* inst) { ssa_worklist_.push_back(inst); }

    void TSCCPPass::addToCFGWorklist(int from_bb, int to_bb) { cfg_worklist_.emplace_back(from_bb, to_bb); }

    bool TSCCPPass::isEdgeExecutable(int from_bb, int to_bb) const
    {
        return executable_edges_.count({from_bb, to_bb}) > 0;
    }

    void TSCCPPass::markEdgeExecutable(int from_bb, int to_bb)
    {
        if (from_bb != -1) executable_edges_.insert({from_bb, to_bb});
    }

    /**
     * 获取一个操作数的格值
     *
     *   - 立即数 -> 对应的常量格值。
     *   - 寄存器 -> 查找定义该寄存器的指令，并返回该指令的格值。
     *   - 其他情况（如全局变量、函数参数）返回BOTTOM
     */
    LatticeValue TSCCPPass::getValueForOperand(LLVMIR::Operand* operand) const
    {
        if (!operand) return LatticeValue::createBottom();

        switch (operand->type)
        {
            case LLVMIR::OperandType::IMMEI32:
            {
                auto* imm = static_cast<LLVMIR::ImmeI32Operand*>(operand);
                return LatticeValue(imm->value);
            }
            case LLVMIR::OperandType::IMMEF32:
            {
                auto* imm = static_cast<LLVMIR::ImmeF32Operand*>(operand);
                return LatticeValue(imm->value);
            }
            case LLVMIR::OperandType::REG:
            {
                auto* reg = static_cast<LLVMIR::RegOperand*>(operand);
                auto  it  = def_map_.find(reg->reg_num);
                if (it != def_map_.end()) return getValue(it->second);
                return LatticeValue::createBottom();
            }
            default: return LatticeValue::createBottom();
        }
    }

    /**
     * 设置一条指令的格值
     *   用新值与旧值进行meet操作。如果值发生改变，即可能确定新的常量，也可能是某值变得不确定
     *   则将所有使用该指令结果的指令加入SSA工作列表，以便重新计算
     */
    void TSCCPPass::setValue(LLVMIR::Instruction* inst, const LatticeValue& value)
    {
        auto old_value = getValue(inst);
        auto new_value = old_value.meet(value);

        if (new_value == old_value) return;

        value_map_[inst] = new_value;

        auto it = use_chains_.find(inst);
        if (it != use_chains_.end())
            for (auto* user : it->second) addToSSAWorklist(user);
    }

    LatticeValue TSCCPPass::getValue(LLVMIR::Instruction* inst) const
    {
        auto it = value_map_.find(inst);
        if (it != value_map_.end()) return it->second;
        return LatticeValue();
    }

    void TSCCPPass::propagateConstants(CFG* cfg)
    {
        current_cfg_ = cfg;

        for (auto& [block_id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                updateInstructionOperands(inst);

                if (inst->opcode == LLVMIR::IROpCode::LOAD)
                {
                    auto value = getValue(inst);
                    if (value.isConstant())
                    {
                        auto dead_value = getValue(inst);
                        int  result_reg = inst->GetResultReg();
                        if (result_reg == -1) continue;

                        for (auto* use_inst : use_chains_[inst]) mapRegToConstant(use_inst, result_reg, dead_value);
                    }
                }
            }
        }
    }

    void TSCCPPass::updateInstructionOperands(LLVMIR::Instruction* inst)
    {
        switch (inst->opcode)
        {
            // 算术指令
            case LLVMIR::IROpCode::ADD:
            case LLVMIR::IROpCode::SUB:
            case LLVMIR::IROpCode::MUL:
            case LLVMIR::IROpCode::DIV:
            case LLVMIR::IROpCode::MOD:
            case LLVMIR::IROpCode::FADD:
            case LLVMIR::IROpCode::FSUB:
            case LLVMIR::IROpCode::FMUL:
            case LLVMIR::IROpCode::FDIV:
            case LLVMIR::IROpCode::BITXOR:
            case LLVMIR::IROpCode::BITAND:
            case LLVMIR::IROpCode::SHL:
            case LLVMIR::IROpCode::ASHR:
            case LLVMIR::IROpCode::LSHR:
            {
                auto* arith   = static_cast<LLVMIR::ArithmeticInst*>(inst);
                auto  lhs_val = getValueForOperand(arith->lhs);
                auto  rhs_val = getValueForOperand(arith->rhs);
                if (lhs_val.isConstant()) replaceWithConstant(arith->lhs, lhs_val);
                if (rhs_val.isConstant()) replaceWithConstant(arith->rhs, rhs_val);
                break;
            }
            // 比较指令
            case LLVMIR::IROpCode::ICMP:
            {
                auto* icmp    = static_cast<LLVMIR::IcmpInst*>(inst);
                auto  lhs_val = getValueForOperand(icmp->lhs);
                auto  rhs_val = getValueForOperand(icmp->rhs);
                if (lhs_val.isConstant()) replaceWithConstant(icmp->lhs, lhs_val);
                if (rhs_val.isConstant()) replaceWithConstant(icmp->rhs, rhs_val);
                break;
            }
            case LLVMIR::IROpCode::FCMP:
            {
                auto* fcmp    = static_cast<LLVMIR::FcmpInst*>(inst);
                auto  lhs_val = getValueForOperand(fcmp->lhs);
                auto  rhs_val = getValueForOperand(fcmp->rhs);
                if (lhs_val.isConstant()) replaceWithConstant(fcmp->lhs, lhs_val);
                if (rhs_val.isConstant()) replaceWithConstant(fcmp->rhs, rhs_val);
                break;
            }
            // 分支指令
            case LLVMIR::IROpCode::BR_COND:
            {
                auto* br       = static_cast<LLVMIR::BranchCondInst*>(inst);
                auto  cond_val = getValueForOperand(br->cond);
                if (cond_val.isConstant()) replaceWithConstant(br->cond, cond_val);
                break;
            }
            // 内存指令
            case LLVMIR::IROpCode::LOAD:
            {
                auto* load    = static_cast<LLVMIR::LoadInst*>(inst);
                auto  ptr_val = getValueForOperand(load->ptr);
                if (ptr_val.isConstant()) replaceWithConstant(load->ptr, ptr_val);
                break;
            }
            case LLVMIR::IROpCode::STORE:
            {
                auto* store = static_cast<LLVMIR::StoreInst*>(inst);
                auto  val   = getValueForOperand(store->val);
                auto  ptr   = getValueForOperand(store->ptr);
                if (val.isConstant()) replaceWithConstant(store->val, val);
                if (ptr.isConstant()) replaceWithConstant(store->ptr, ptr);
                break;
            }
            case LLVMIR::IROpCode::GETELEMENTPTR:
            {
                auto* gep = static_cast<LLVMIR::GEPInst*>(inst);
                auto  ptr = getValueForOperand(gep->base_ptr);
                if (ptr.isConstant()) replaceWithConstant(gep->base_ptr, ptr);
                for (auto*& idx : gep->idxs)
                {
                    auto idx_val = getValueForOperand(idx);
                    if (idx_val.isConstant()) replaceWithConstant(idx, idx_val);
                }
                break;
            }
            // 类型转换指令
            case LLVMIR::IROpCode::ZEXT:
            {
                auto* zext    = static_cast<LLVMIR::ZextInst*>(inst);
                auto  src_val = getValueForOperand(zext->src);
                if (src_val.isConstant()) replaceWithConstant(zext->src, src_val);
                break;
            }
            case LLVMIR::IROpCode::SITOFP:
            {
                auto* sitofp  = static_cast<LLVMIR::SI2FPInst*>(inst);
                auto  src_val = getValueForOperand(sitofp->f_si);
                if (src_val.isConstant()) replaceWithConstant(sitofp->f_si, src_val);
                break;
            }
            case LLVMIR::IROpCode::FPTOSI:
            {
                auto* fptosi  = static_cast<LLVMIR::FP2SIInst*>(inst);
                auto  src_val = getValueForOperand(fptosi->f_fp);
                if (src_val.isConstant()) replaceWithConstant(fptosi->f_fp, src_val);
                break;
            }
            case LLVMIR::IROpCode::FPEXT:
            {
                auto* fpext   = static_cast<LLVMIR::FPExtInst*>(inst);
                auto  src_val = getValueForOperand(fpext->src);
                if (src_val.isConstant()) replaceWithConstant(fpext->src, src_val);
                break;
            }
            // 函数调用
            case LLVMIR::IROpCode::CALL:
            {
                auto* call = static_cast<LLVMIR::CallInst*>(inst);
                for (auto& arg_pair : call->args)
                {
                    auto arg_val = getValueForOperand(arg_pair.second);
                    if (arg_val.isConstant()) replaceWithConstant(arg_pair.second, arg_val);
                }
                break;
            }
            // 返回指令
            case LLVMIR::IROpCode::RET:
            {
                auto* ret = static_cast<LLVMIR::RetInst*>(inst);
                if (ret->ret)
                {
                    auto ret_val = getValueForOperand(ret->ret);
                    if (ret_val.isConstant()) replaceWithConstant(ret->ret, ret_val);
                }
                break;
            }
            // PHI指令
            case LLVMIR::IROpCode::PHI:
            {
                auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                for (auto& phi_pair : phi->vals_for_labels)
                {
                    auto val = getValueForOperand(phi_pair.first);
                    if (val.isConstant()) replaceWithConstant(phi_pair.first, val);
                }
                break;
            }
            // 其他指令暂不处理
            case LLVMIR::IROpCode::BR_UNCOND:
            case LLVMIR::IROpCode::ALLOCA:
            case LLVMIR::IROpCode::GLOBAL_VAR:
            case LLVMIR::IROpCode::GLOBAL_STR:
            case LLVMIR::IROpCode::OTHER:
            default: break;
        }
    }

    void TSCCPPass::replaceWithConstant(LLVMIR::Operand*& operand, const LatticeValue& value)
    {
        if (!value.isConstant()) return;

        DBGINFO("TSCCP: Replacing operand with constant: ",
            (value.getType() == LatticeValue::ValueType::INTEGER ? value.getIntValue() : (int)value.getFloatValue()));

        if (value.getType() == LatticeValue::ValueType::INTEGER)
            operand = getImmeI32Operand(value.getIntValue());
        else if (value.getType() == LatticeValue::ValueType::FLOAT)
            operand = getImmeF32Operand(value.getFloatValue());
    }

    void TSCCPPass::mapRegToConstant(LLVMIR::Instruction* inst, int reg_num, const LatticeValue& value)
    {
        if (!value.isConstant()) return;

        DBGINFO("TSCCP: Mapping register ",
            reg_num,
            " to constant: ",
            (value.getType() == LatticeValue::ValueType::INTEGER ? value.getIntValue() : (int)value.getFloatValue()));

        auto replaceIfMatch = [&](LLVMIR::Operand*& operand) {
            if (operand && operand->type == LLVMIR::OperandType::REG)
            {
                auto* reg_operand = static_cast<LLVMIR::RegOperand*>(operand);
                if (reg_operand->reg_num == reg_num) replaceWithConstant(operand, value);
            }
        };

        std::vector<LLVMIR::Operand*> operands = inst->GetUsedOperands();
        for (auto*& operand : operands)
        {
            if (operand && operand->type == LLVMIR::OperandType::REG)
            {
                auto* reg_operand = static_cast<LLVMIR::RegOperand*>(operand);
                if (reg_operand->reg_num == reg_num) replaceWithConstant(operand, value);
            }
        }

        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::ADD:
            case LLVMIR::IROpCode::SUB:
            case LLVMIR::IROpCode::MUL:
            case LLVMIR::IROpCode::DIV:
            case LLVMIR::IROpCode::MOD:
            case LLVMIR::IROpCode::FADD:
            case LLVMIR::IROpCode::FSUB:
            case LLVMIR::IROpCode::FMUL:
            case LLVMIR::IROpCode::FDIV:
            case LLVMIR::IROpCode::BITXOR:
            case LLVMIR::IROpCode::BITAND:
            case LLVMIR::IROpCode::SHL:
            case LLVMIR::IROpCode::ASHR:
            case LLVMIR::IROpCode::LSHR:
            {
                auto* arith = static_cast<LLVMIR::ArithmeticInst*>(inst);
                replaceIfMatch(arith->lhs);
                replaceIfMatch(arith->rhs);
                break;
            }
            case LLVMIR::IROpCode::ICMP:
            case LLVMIR::IROpCode::FCMP:
            {
                auto* cmp = static_cast<LLVMIR::IcmpInst*>(inst);
                replaceIfMatch(cmp->lhs);
                replaceIfMatch(cmp->rhs);
                break;
            }
            case LLVMIR::IROpCode::STORE:
            {
                auto* store = static_cast<LLVMIR::StoreInst*>(inst);
                replaceIfMatch(store->val);
                replaceIfMatch(store->ptr);
                break;
            }
            case LLVMIR::IROpCode::LOAD:
            {
                auto* load = static_cast<LLVMIR::LoadInst*>(inst);
                replaceIfMatch(load->ptr);
                break;
            }
            case LLVMIR::IROpCode::BR_COND:
            {
                auto* br = static_cast<LLVMIR::BranchCondInst*>(inst);
                replaceIfMatch(br->cond);
                break;
            }
            case LLVMIR::IROpCode::RET:
            {
                auto* ret = static_cast<LLVMIR::RetInst*>(inst);
                if (ret->ret) replaceIfMatch(ret->ret);
                break;
            }
            case LLVMIR::IROpCode::PHI:
            {
                auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                for (auto& [val, label] : phi->vals_for_labels) replaceIfMatch(val);
                break;
            }
            case LLVMIR::IROpCode::GETELEMENTPTR:
            {
                auto* gep = static_cast<LLVMIR::GEPInst*>(inst);
                replaceIfMatch(gep->base_ptr);
                for (auto*& idx : gep->idxs) replaceIfMatch(idx);
                break;
            }
            case LLVMIR::IROpCode::CALL:
            {
                auto* call = static_cast<LLVMIR::CallInst*>(inst);
                for (auto& [type, operand] : call->args) replaceIfMatch(operand);
                break;
            }
            default: break;
        }
    }

    /**
     * 如果一条指令结果确定为常量，则理应在常量传播阶段将其引用全部替换
     * 则该指令的计算不再被需要，可以被删除
     */
    void TSCCPPass::eliminateDeadInstructions(CFG* cfg)
    {
        std::vector<LLVMIR::Instruction*> dead_instructions;
        for (auto& [block_id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                auto value = getValue(inst);
                if (value.isConstant())
                {
                    auto dead_value = getValue(inst);
                    int  result_reg = inst->GetResultReg();
                    if (result_reg == -1) continue;

                    for (auto* use_inst : use_chains_[inst]) mapRegToConstant(use_inst, result_reg, dead_value);

                    dead_instructions.push_back(inst);
                }
            }
        }

        for (auto* dead_inst : dead_instructions)
        {
            auto block_it = cfg->block_id_to_block.find(dead_inst->block_id);
            if (block_it != cfg->block_id_to_block.end())
            {
                auto& insts = block_it->second->insts;
                insts.erase(std::remove(insts.begin(), insts.end(), dead_inst), insts.end());
                delete dead_inst;
            }
        }
    }

    void InstructionVisitor::visit(LLVMIR::Instruction* inst)
    {
        current_inst_ = inst;

        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::PHI: visitPhi(static_cast<LLVMIR::PhiInst*>(inst)); break;
            case LLVMIR::IROpCode::BR_COND:
            case LLVMIR::IROpCode::BR_UNCOND: visitBranch(inst); break;
            // 算术指令
            case LLVMIR::IROpCode::ADD:
            case LLVMIR::IROpCode::SUB:
            case LLVMIR::IROpCode::MUL:
            case LLVMIR::IROpCode::DIV:
            case LLVMIR::IROpCode::MOD:
            case LLVMIR::IROpCode::FADD:
            case LLVMIR::IROpCode::FSUB:
            case LLVMIR::IROpCode::FMUL:
            case LLVMIR::IROpCode::FDIV:
            case LLVMIR::IROpCode::BITXOR:
            case LLVMIR::IROpCode::BITAND:
            case LLVMIR::IROpCode::SHL:
            case LLVMIR::IROpCode::ASHR:
            case LLVMIR::IROpCode::LSHR: visitArithmetic(static_cast<LLVMIR::ArithmeticInst*>(inst)); break;
            // 比较指令
            case LLVMIR::IROpCode::ICMP:
            case LLVMIR::IROpCode::FCMP: visitComparison(inst); break;
            // 类型转换指令
            case LLVMIR::IROpCode::SITOFP:
            case LLVMIR::IROpCode::FPTOSI:
            case LLVMIR::IROpCode::ZEXT:
            case LLVMIR::IROpCode::FPEXT: visitConversion(inst); break;
            // 内存指令
            case LLVMIR::IROpCode::LOAD: visitLoad(static_cast<LLVMIR::LoadInst*>(inst)); break;
            case LLVMIR::IROpCode::STORE: visitStore(static_cast<LLVMIR::StoreInst*>(inst)); break;
            // 函数调用
            case LLVMIR::IROpCode::CALL: visitCall(static_cast<LLVMIR::CallInst*>(inst)); break;
            // 其他指令
            case LLVMIR::IROpCode::ALLOCA:
            case LLVMIR::IROpCode::GETELEMENTPTR:
            case LLVMIR::IROpCode::RET:
            case LLVMIR::IROpCode::GLOBAL_VAR:
            case LLVMIR::IROpCode::GLOBAL_STR:
            case LLVMIR::IROpCode::OTHER:
            default: visitOther(inst); break;
        }
    }

    void InstructionVisitor::visitPhi(LLVMIR::PhiInst* phi)
    {
        LatticeValue result;
        bool         first                      = true;
        bool         is_potential_induction_var = false;

        for (const auto& [value_op, label_op] : phi->vals_for_labels)
        {
            if (value_op->type != LLVMIR::OperandType::REG) continue;

            auto* reg_op = static_cast<LLVMIR::RegOperand*>(value_op);
            auto  def_it = pass_->def_map_.find(reg_op->reg_num);
            if (def_it == pass_->def_map_.end()) continue;

            auto* def_inst = def_it->second;
            if (def_inst->opcode == LLVMIR::IROpCode::ADD || def_inst->opcode == LLVMIR::IROpCode::SUB ||
                def_inst->opcode == LLVMIR::IROpCode::PHI)
            {
                is_potential_induction_var = true;
                break;
            }
        }

        for (const auto& [value_op, label_op] : phi->vals_for_labels)
        {
            auto* label      = static_cast<LLVMIR::LabelOperand*>(label_op);
            int   pred_block = label->label_num;
            int   curr_block = phi->block_id;

            if (!pass_->isEdgeExecutable(pred_block, curr_block)) continue;

            LatticeValue operand_value = pass_->getValueForOperand(value_op);
            if (first)
            {
                result = operand_value;
                first  = false;
            }
            else
                result = result.meet(operand_value);
        }

        if (is_potential_induction_var && result.isConstant()) result = LatticeValue::createBottom();

        if (!first) pass_->setValue(phi, result);
    }

    void InstructionVisitor::visitBranch(LLVMIR::Instruction* br)
    {
        int current_block = br->block_id;

        if (br->opcode == LLVMIR::IROpCode::BR_UNCOND)
        {
            auto* br_uncond    = static_cast<LLVMIR::BranchUncondInst*>(br);
            auto* target_label = static_cast<LLVMIR::LabelOperand*>(br_uncond->target_label);
            pass_->addToCFGWorklist(current_block, target_label->label_num);
        }
        else if (br->opcode == LLVMIR::IROpCode::BR_COND)
        {
            auto* br_cond    = static_cast<LLVMIR::BranchCondInst*>(br);
            auto  cond_value = pass_->getValueForOperand(br_cond->cond);

            auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br_cond->true_label);
            auto* false_label = static_cast<LLVMIR::LabelOperand*>(br_cond->false_label);

            if (cond_value.isConstant() && cond_value.getType() == LatticeValue::ValueType::INTEGER)
            {
                if (cond_value.getIntValue() != 0)
                    pass_->addToCFGWorklist(current_block, true_label->label_num);
                else
                    pass_->addToCFGWorklist(current_block, false_label->label_num);
            }
            else if (cond_value.isBottom())
            {
                pass_->addToCFGWorklist(current_block, true_label->label_num);
                pass_->addToCFGWorklist(current_block, false_label->label_num);
            }
        }
    }

    void InstructionVisitor::visitArithmetic(LLVMIR::ArithmeticInst* arith)
    {
        LatticeValue lhs    = pass_->getValueForOperand(arith->lhs);
        LatticeValue rhs    = pass_->getValueForOperand(arith->rhs);
        LatticeValue result = foldBinaryOperation(arith->opcode, lhs, rhs);
        pass_->setValue(arith, result);
    }

    void InstructionVisitor::visitComparison(LLVMIR::Instruction* cmp)
    {
        LatticeValue result;

        if (cmp->opcode == LLVMIR::IROpCode::ICMP)
        {
            auto*        icmp = static_cast<LLVMIR::IcmpInst*>(cmp);
            LatticeValue lhs  = pass_->getValueForOperand(icmp->lhs);
            LatticeValue rhs  = pass_->getValueForOperand(icmp->rhs);
            result            = foldComparisonOperation(cmp->opcode, icmp->cond, LLVMIR::FcmpCond::OEQ, lhs, rhs);
        }
        else if (cmp->opcode == LLVMIR::IROpCode::FCMP)
        {
            auto*        fcmp = static_cast<LLVMIR::FcmpInst*>(cmp);
            LatticeValue lhs  = pass_->getValueForOperand(fcmp->lhs);
            LatticeValue rhs  = pass_->getValueForOperand(fcmp->rhs);
            result            = foldComparisonOperation(cmp->opcode, LLVMIR::IcmpCond::EQ, fcmp->cond, lhs, rhs);
        }

        pass_->setValue(cmp, result);
    }

    void InstructionVisitor::visitConversion(LLVMIR::Instruction* conv)
    {
        LatticeValue operand_value;

        if (conv->opcode == LLVMIR::IROpCode::SITOFP)
        {
            auto* sitofp  = static_cast<LLVMIR::SI2FPInst*>(conv);
            operand_value = pass_->getValueForOperand(sitofp->f_si);
        }
        else if (conv->opcode == LLVMIR::IROpCode::FPTOSI)
        {
            auto* fptosi  = static_cast<LLVMIR::FP2SIInst*>(conv);
            operand_value = pass_->getValueForOperand(fptosi->f_fp);
        }
        else if (conv->opcode == LLVMIR::IROpCode::ZEXT)
        {
            auto* zext    = static_cast<LLVMIR::ZextInst*>(conv);
            operand_value = pass_->getValueForOperand(zext->src);
        }

        LatticeValue result = foldConversionOperation(conv->opcode, operand_value);
        pass_->setValue(conv, result);
    }

    void InstructionVisitor::visitCall(LLVMIR::CallInst* call)
    {
        pass_->setValue(call, LatticeValue::createBottom());
        bool is_system_function = false;

        std::vector<std::string> system_functions = {"getint",
            "getch",
            "getarray",
            "getfloat",
            "getfarray",
            "putint",
            "putch",
            "putarray",
            "putfloat",
            "putfarray",
            "_sysy_starttime",
            "_sysy_stoptime"};

        if (call->func_name.find("llvm.") == 0) is_system_function = true;

        for (const auto& sys_func : system_functions)
            if (call->func_name == sys_func)
            {
                is_system_function = true;
                break;
            }

        if (!is_system_function)
        {
            pass_->memory_state_.clear();

            for (auto& [block_id, block] : pass_->current_cfg_->block_id_to_block)
                for (auto* inst : block->insts)
                {
                    if (inst->opcode != LLVMIR::IROpCode::LOAD) continue;

                    pass_->addToSSAWorklist(inst);
                    pass_->setValue(inst, LatticeValue::createBottom());
                }
        }
        else
        {
            if (call->func_name == "getarray" || call->func_name == "getfarray")
            {
                if (!call->args.empty())
                {
                    auto* arg = call->args[0].second;
                    DBGINFO("TSCCP: Processing ", call->func_name, " call with argument");

                    LLVMIR::Operand* base_array = pass_->getArrayBasePointer(arg);
                    if (base_array)
                    {
                        DBGINFO("\tFound base array pointer, invalidating all related memory locations");

                        std::vector<MemoryLocation> to_invalidate;
                        for (auto& [mem_loc, value] : pass_->memory_state_)
                        {
                            if (pass_->isRelatedToArray(mem_loc, base_array))
                            {
                                to_invalidate.push_back(mem_loc);
                                DBGINFO("\t\tInvalidating: ", mem_loc.toString());
                            }
                        }

                        for (const auto& invalid_loc : to_invalidate) { pass_->memory_state_.erase(invalid_loc); }

                        DBGINFO("\tInvalidated ", to_invalidate.size(), " memory locations for ", call->func_name);
                    }
                    else
                    {
                        DBGINFO("\tCould not determine base array pointer, clearing all memory state");
                        pass_->memory_state_.clear();
                    }
                }
            }
            else if (call->func_name.find("llvm.memset") == 0)
            {
                if (!call->args.empty())
                {
                    auto*                       ptr_arg = call->args[0].second;
                    std::vector<MemoryLocation> to_invalidate;
                    for (auto& [mem_loc, value] : pass_->memory_state_)
                        if (reinterpret_cast<uintptr_t>(mem_loc.base_ptr) == reinterpret_cast<uintptr_t>(ptr_arg))
                            to_invalidate.push_back(mem_loc);

                    for (const auto& invalid_loc : to_invalidate) pass_->memory_state_.erase(invalid_loc);

                    for (auto& [block_id, block] : pass_->current_cfg_->block_id_to_block)
                        for (auto* inst : block->insts)
                        {
                            if (inst->opcode != LLVMIR::IROpCode::LOAD) continue;

                            auto* load = static_cast<LLVMIR::LoadInst*>(inst);
                            if (reinterpret_cast<uintptr_t>(load->ptr) == reinterpret_cast<uintptr_t>(ptr_arg))
                            {
                                pass_->addToSSAWorklist(inst);
                                pass_->setValue(inst, LatticeValue::createBottom());
                            }
                        }
                }
            }
            else
                DBGINFO("TSCCP: Unhandled system function: ", call->func_name);
        }
    }

    void InstructionVisitor::visitOther(LLVMIR::Instruction* inst)
    {
        pass_->setValue(inst, LatticeValue::createBottom());
    }

    LatticeValue InstructionVisitor::foldBinaryOperation(
        LLVMIR::IROpCode opcode, const LatticeValue& lhs, const LatticeValue& rhs)
    {
        if (!lhs.isConstant() || !rhs.isConstant())
        {
            return lhs.isBottom() || rhs.isBottom() ? LatticeValue::createBottom() : LatticeValue();
        }

        if (lhs.getType() == LatticeValue::ValueType::INTEGER && rhs.getType() == LatticeValue::ValueType::INTEGER)
        {
            int l = lhs.getIntValue();
            int r = rhs.getIntValue();

            switch (opcode)
            {
                // 基本算术运算
                case LLVMIR::IROpCode::ADD: return LatticeValue(l + r);
                case LLVMIR::IROpCode::SUB: return LatticeValue(l - r);
                case LLVMIR::IROpCode::MUL: return LatticeValue(l * r);
                case LLVMIR::IROpCode::DIV: return r != 0 ? LatticeValue(l / r) : LatticeValue::createBottom();
                case LLVMIR::IROpCode::MOD: return r != 0 ? LatticeValue(l % r) : LatticeValue::createBottom();

                // 位运算
                case LLVMIR::IROpCode::BITXOR: return LatticeValue(l ^ r);
                case LLVMIR::IROpCode::BITAND: return LatticeValue(l & r);

                // 移位运算（需要检查移位量的合理性）
                case LLVMIR::IROpCode::SHL:
                    return (r >= 0 && r < 32) ? LatticeValue(l << r) : LatticeValue::createBottom();
                case LLVMIR::IROpCode::ASHR:
                    return (r >= 0 && r < 32) ? LatticeValue(l >> r) : LatticeValue::createBottom();
                case LLVMIR::IROpCode::LSHR:
                    return (r >= 0 && r < 32) ? LatticeValue(static_cast<int>(static_cast<unsigned>(l) >> r))
                                              : LatticeValue::createBottom();

                default: return LatticeValue::createBottom();
            }
        }
        else if (lhs.getType() == LatticeValue::ValueType::FLOAT && rhs.getType() == LatticeValue::ValueType::FLOAT)
        {
            float l = lhs.getFloatValue();
            float r = rhs.getFloatValue();

            switch (opcode)
            {
                case LLVMIR::IROpCode::FADD: return LatticeValue(l + r);
                case LLVMIR::IROpCode::FSUB: return LatticeValue(l - r);
                case LLVMIR::IROpCode::FMUL: return LatticeValue(l * r);
                case LLVMIR::IROpCode::FDIV:
                {
                    // 浮点除法，检查是否除零或无穷大
                    if (r == 0.0f) return LatticeValue::createBottom();
                    float result = l / r;
                    // 检查结果是否为有限数
                    if (std::isfinite(result)) return LatticeValue(result);
                    return LatticeValue::createBottom();
                }
                default: break;
            }
        }

        return LatticeValue::createBottom();
    }

    LatticeValue InstructionVisitor::foldComparisonOperation(LLVMIR::IROpCode opcode, LLVMIR::IcmpCond icond,
        LLVMIR::FcmpCond fcond, const LatticeValue& lhs, const LatticeValue& rhs)
    {
        if (!lhs.isConstant() || !rhs.isConstant())
        {
            return lhs.isBottom() || rhs.isBottom() ? LatticeValue::createBottom() : LatticeValue();
        }

        if (opcode == LLVMIR::IROpCode::ICMP && lhs.getType() == LatticeValue::ValueType::INTEGER &&
            rhs.getType() == LatticeValue::ValueType::INTEGER)
        {
            int l = lhs.getIntValue();
            int r = rhs.getIntValue();

            switch (icond)
            {
                // 相等性比较
                case LLVMIR::IcmpCond::EQ: return LatticeValue(l == r ? 1 : 0);
                case LLVMIR::IcmpCond::NE: return LatticeValue(l != r ? 1 : 0);

                // 有符号比较
                case LLVMIR::IcmpCond::SGT: return LatticeValue(l > r ? 1 : 0);
                case LLVMIR::IcmpCond::SGE: return LatticeValue(l >= r ? 1 : 0);
                case LLVMIR::IcmpCond::SLT: return LatticeValue(l < r ? 1 : 0);
                case LLVMIR::IcmpCond::SLE: return LatticeValue(l <= r ? 1 : 0);

                // 无符号比较
                case LLVMIR::IcmpCond::UGT:
                    return LatticeValue(static_cast<unsigned>(l) > static_cast<unsigned>(r) ? 1 : 0);
                case LLVMIR::IcmpCond::UGE:
                    return LatticeValue(static_cast<unsigned>(l) >= static_cast<unsigned>(r) ? 1 : 0);
                case LLVMIR::IcmpCond::ULT:
                    return LatticeValue(static_cast<unsigned>(l) < static_cast<unsigned>(r) ? 1 : 0);
                case LLVMIR::IcmpCond::ULE:
                    return LatticeValue(static_cast<unsigned>(l) <= static_cast<unsigned>(r) ? 1 : 0);

                default: return LatticeValue::createBottom();
            }
        }
        else if (opcode == LLVMIR::IROpCode::FCMP && lhs.getType() == LatticeValue::ValueType::FLOAT &&
                 rhs.getType() == LatticeValue::ValueType::FLOAT)
        {
            float l = lhs.getFloatValue();
            float r = rhs.getFloatValue();

            // 检查NaN值，NaN比较总是返回false（除了UNO）
            bool l_is_nan = std::isnan(l);
            bool r_is_nan = std::isnan(r);
            bool any_nan  = l_is_nan || r_is_nan;

            switch (fcond)
            {
                // 常量比较结果
                case LLVMIR::FcmpCond::FALSE: return LatticeValue(0);
                case LLVMIR::FcmpCond::TRUE: return LatticeValue(1);

                // 有序比较（ordered）- 如果有NaN则返回false
                case LLVMIR::FcmpCond::OEQ: return LatticeValue(any_nan ? 0 : (l == r ? 1 : 0));
                case LLVMIR::FcmpCond::OGT: return LatticeValue(any_nan ? 0 : (l > r ? 1 : 0));
                case LLVMIR::FcmpCond::OGE: return LatticeValue(any_nan ? 0 : (l >= r ? 1 : 0));
                case LLVMIR::FcmpCond::OLT: return LatticeValue(any_nan ? 0 : (l < r ? 1 : 0));
                case LLVMIR::FcmpCond::OLE: return LatticeValue(any_nan ? 0 : (l <= r ? 1 : 0));
                case LLVMIR::FcmpCond::ONE: return LatticeValue(any_nan ? 0 : (l != r ? 1 : 0));
                case LLVMIR::FcmpCond::ORD: return LatticeValue(any_nan ? 0 : 1);

                // 无序比较（unordered）- 如果有NaN则返回true
                case LLVMIR::FcmpCond::UEQ: return LatticeValue(any_nan ? 1 : (l == r ? 1 : 0));
                case LLVMIR::FcmpCond::UGT: return LatticeValue(any_nan ? 1 : (l > r ? 1 : 0));
                case LLVMIR::FcmpCond::UGE: return LatticeValue(any_nan ? 1 : (l >= r ? 1 : 0));
                case LLVMIR::FcmpCond::ULT: return LatticeValue(any_nan ? 1 : (l < r ? 1 : 0));
                case LLVMIR::FcmpCond::ULE: return LatticeValue(any_nan ? 1 : (l <= r ? 1 : 0));
                case LLVMIR::FcmpCond::UNE: return LatticeValue(any_nan ? 1 : (l != r ? 1 : 0));
                case LLVMIR::FcmpCond::UNO: return LatticeValue(any_nan ? 1 : 0);

                default: return LatticeValue::createBottom();
            }
        }

        return LatticeValue::createBottom();
    }

    LatticeValue InstructionVisitor::foldConversionOperation(LLVMIR::IROpCode opcode, const LatticeValue& operand)
    {
        if (!operand.isConstant()) { return operand.isBottom() ? LatticeValue::createBottom() : LatticeValue(); }

        switch (opcode)
        {
            case LLVMIR::IROpCode::SITOFP:
                // 有符号整数转浮点数
                if (operand.getType() == LatticeValue::ValueType::INTEGER)
                {
                    return LatticeValue(static_cast<float>(operand.getIntValue()));
                }
                break;

            case LLVMIR::IROpCode::FPTOSI:
                // 浮点数转有符号整数
                if (operand.getType() == LatticeValue::ValueType::FLOAT)
                {
                    float val = operand.getFloatValue();
                    // 检查是否为有限数
                    if (std::isfinite(val))
                    {
                        // 检查是否在int范围内
                        if (val >= static_cast<float>(INT_MIN) && val <= static_cast<float>(INT_MAX))
                        {
                            return LatticeValue(static_cast<int>(val));
                        }
                    }
                    // 如果不在范围内或不是有限数，返回bottom
                    return LatticeValue::createBottom();
                }
                break;

            case LLVMIR::IROpCode::ZEXT:
                // 零扩展（通常用于布尔值或小整数类型扩展到更大的整数类型）
                if (operand.getType() == LatticeValue::ValueType::INTEGER)
                {
                    // 对于我们的实现，假设都是32位整数，所以zext实际上不改变值
                    return LatticeValue(operand.getIntValue());
                }
                break;

            case LLVMIR::IROpCode::FPEXT:
                // 浮点数精度扩展（如float到double）
                if (operand.getType() == LatticeValue::ValueType::FLOAT)
                {
                    // 在我们的实现中，假设只有float类型，所以fpext不改变值
                    return LatticeValue(operand.getFloatValue());
                }
                break;

            default:
                // 未支持的类型转换操作
                break;
        }

        return LatticeValue::createBottom();
    }

    void InstructionVisitor::visitLoad(LLVMIR::LoadInst* load)
    {
        MemoryLocation load_loc = pass_->getMemoryLocation(load->ptr);

        if (load_loc.isValid() && pass_->current_cfg_ && pass_->current_cfg_->LoopForest)
        {
            for (auto* loop : pass_->current_cfg_->LoopForest->loop_set)
            {
                if (pass_->isModifiedInLoop(load_loc, loop))
                {
                    pass_->setValue(load, LatticeValue::createBottom());
                    return;
                }
            }
        }

        LatticeValue result = pass_->handleLoadInstruction(load);
        pass_->setValue(load, result);
    }

    void InstructionVisitor::visitStore(LLVMIR::StoreInst* store) { pass_->handleStoreInstruction(store); }

    /**
     * 处理store指令
     *   1. 解析出内存位置`MemoryLocation`。
     *   2. 如果地址是动态的（无法在编译时确定），则无法确定该store是否有效，清空所有相关的内存状态。
     *   3. 检查是否存在循环依赖（如 a[i] = a[i] + 1），若存在则不明确该store是否有效，清空所有相关的内存状态。
     *   4. 更新`memory_state_`，将当前内存位置的值设为store的值。
     *   5. 使任何可能与当前位置冲突（alias）的其他内存位置失效
     *   6.
     * 上文中提到的"有效"指该store指令的值可以被load指令获取，对于循环依赖的store无法定位最后一次store，因此无法确定该store是否有效
     */
    void TSCCPPass::handleStoreInstruction(LLVMIR::StoreInst* store)
    {
        LatticeValue   stored_value = getValueForOperand(store->val);
        MemoryLocation store_loc    = getMemoryLocation(store->ptr);

        NaturalLoop* store_loop = getLoopForInstruction(store);
        if (store_loop)
        {
            if (store_loc.isValid()) { recordMemoryModificationInLoop(store_loc, store_loop); }

            LLVMIR::Operand* base_ptr = getArrayBasePointer(store->ptr);
            if (base_ptr) { recordBaseModificationInLoop(base_ptr, store_loop); }
        }

        if (!store_loc.isValid())
        {
            DBGINFO("\tSTORE with dynamic indices detected - clearing ALL memory state");
            memory_state_.clear();
            return;
        }

        bool has_circular_dependency = false;
        if (store->val->type == LLVMIR::OperandType::REG)
        {
            auto* reg_operand = static_cast<LLVMIR::RegOperand*>(store->val);
            auto  def_it      = def_map_.find(reg_operand->reg_num);
            if (def_it != def_map_.end()) has_circular_dependency = hasCircularDependency(def_it->second, store_loc);
        }

        for (auto it = memory_state_.begin(); it != memory_state_.end();)
        {
            if (it->first != store_loc && mayAlias(store_loc, it->first))
                it = memory_state_.erase(it);
            else
                ++it;
        }

        if (stored_value.isConstant() && !has_circular_dependency)
            memory_state_[store_loc] = stored_value;
        else
            memory_state_.erase(store_loc);
    }

    LatticeValue TSCCPPass::handleLoadInstruction(LLVMIR::LoadInst* load)
    {
        MemoryLocation load_loc = getMemoryLocation(load->ptr);

        if (!load_loc.isValid()) return LatticeValue::createBottom();

        if (isMemoryModifiedInAnyLoop(load_loc))
        {
            // 跳过缓存使用reaching stores分析
        }
        else
        {
            auto it = memory_state_.find(load_loc);
            if (it != memory_state_.end()) { return it->second; }
        }

        std::vector<LLVMIR::StoreInst*> reaching_stores = collectReachingStoresWithLoopAwareness(load, load_loc);

        if (reaching_stores.empty()) { return LatticeValue::createBottom(); }

        LatticeValue result;
        bool         first = true;

        for (auto* store : reaching_stores)
        {
            LatticeValue store_value = getValueForOperand(store->val);

            if (first)
            {
                result = store_value;
                first  = false;
            }
            else { result = result.meet(store_value); }
        }

        return result;
    }

    MemoryLocation TSCCPPass::getMemoryLocation(LLVMIR::Operand* ptr) const
    {
        if (!ptr) return MemoryLocation();

        if (ptr->type == LLVMIR::OperandType::GLOBAL) return MemoryLocation();

        if (ptr->type == LLVMIR::OperandType::REG)
        {
            auto* reg_operand = static_cast<LLVMIR::RegOperand*>(ptr);
            auto  it          = def_map_.find(reg_operand->reg_num);
            if (it != def_map_.end())
            {
                auto* defining_inst = it->second;

                if (defining_inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                {
                    auto* gep = static_cast<LLVMIR::GEPInst*>(defining_inst);

                    MemoryLocation base_loc = getMemoryLocation(gep->base_ptr);
                    if (!base_loc.isValid()) return MemoryLocation();

                    int current_offset = 0;

                    if (gep->dims.size() > 0)
                    {
                        size_t start_idx = 0;

                        if (gep->idxs.size() > 0)
                        {
                            auto* first_operand = gep->idxs[0];
                            if (first_operand->type == LLVMIR::OperandType::IMMEI32)
                            {
                                auto* imm = static_cast<LLVMIR::ImmeI32Operand*>(first_operand);
                                if (imm->value == 0) start_idx = 1;
                            }
                        }

                        for (size_t i = start_idx; i < gep->idxs.size(); ++i)
                        {
                            auto* operand     = gep->idxs[i];
                            int   index_value = 0;

                            if (operand->type == LLVMIR::OperandType::IMMEI32)
                            {
                                auto* imm   = static_cast<LLVMIR::ImmeI32Operand*>(operand);
                                index_value = imm->value;
                            }
                            else if (operand->type == LLVMIR::OperandType::REG)
                            {
                                auto* reg_operand = static_cast<LLVMIR::RegOperand*>(operand);
                                auto  def_it      = def_map_.find(reg_operand->reg_num);
                                if (def_it != def_map_.end())
                                {
                                    auto*        defining_inst = def_it->second;
                                    LatticeValue reg_value     = getValue(defining_inst);
                                    if (reg_value.isConstant() &&
                                        reg_value.getType() == LatticeValue::ValueType::INTEGER)
                                    {
                                        index_value = reg_value.getIntValue();
                                    }
                                    else { return MemoryLocation(); }
                                }
                                else { return MemoryLocation(); }
                            }
                            else { return MemoryLocation(); }

                            int    dimension_multiplier = 1;
                            size_t dim_idx              = i - start_idx;

                            for (size_t d = dim_idx + 1; d < gep->dims.size(); ++d)
                            {
                                dimension_multiplier *= gep->dims[d];
                            }

                            current_offset += index_value * dimension_multiplier;
                        }
                    }
                    else
                    {
                        for (size_t i = 0; i < gep->idxs.size(); ++i)
                        {
                            auto* operand     = gep->idxs[i];
                            int   index_value = 0;

                            if (operand->type == LLVMIR::OperandType::IMMEI32)
                            {
                                auto* imm   = static_cast<LLVMIR::ImmeI32Operand*>(operand);
                                index_value = imm->value;
                            }
                            else if (operand->type == LLVMIR::OperandType::REG)
                            {
                                auto* reg_operand = static_cast<LLVMIR::RegOperand*>(operand);
                                auto  def_it      = def_map_.find(reg_operand->reg_num);
                                if (def_it != def_map_.end())
                                {
                                    auto*        defining_inst = def_it->second;
                                    LatticeValue reg_value     = getValue(defining_inst);
                                    if (reg_value.isConstant() &&
                                        reg_value.getType() == LatticeValue::ValueType::INTEGER)
                                    {
                                        index_value = reg_value.getIntValue();
                                    }
                                    else
                                        return MemoryLocation();
                                }
                                else
                                    return MemoryLocation();
                            }
                            else
                                return MemoryLocation();

                            current_offset += index_value;
                        }
                    }

                    int total_offset = base_loc.element_offset + current_offset;
                    return MemoryLocation(base_loc.base_ptr, total_offset);
                }

                if (defining_inst->opcode == LLVMIR::IROpCode::ALLOCA) return MemoryLocation(ptr, 0);
            }
        }

        return MemoryLocation(ptr, 0);
    }

    bool TSCCPPass::isSafeFunction(const std::string& func_name) const
    {
        static const std::set<std::string> safe_functions = {
            "putint", "putch", "putarray", "putfloat", "putfarray", "_sysy_starttime", "_sysy_stoptime"};

        if (func_name.find("llvm.") == 0)
        {
            if (func_name.find("llvm.memcpy") == 0 || func_name.find("llvm.memmove") == 0) { return false; }
            return true;
        }

        return safe_functions.count(func_name) > 0;
    }

    LLVMIR::Operand* TSCCPPass::getArrayBasePointer(LLVMIR::Operand* ptr) const
    {
        if (!ptr) return nullptr;

        std::set<LLVMIR::Operand*> visited;
        while (ptr && visited.find(ptr) == visited.end())
        {
            visited.insert(ptr);

            if (ptr->type == LLVMIR::OperandType::GLOBAL) return ptr;

            if (ptr->type == LLVMIR::OperandType::REG)
            {
                auto* reg = static_cast<LLVMIR::RegOperand*>(ptr);
                auto  it  = def_map_.find(reg->reg_num);
                if (it == def_map_.end()) break;

                auto* def = it->second;
                if (def->opcode == LLVMIR::IROpCode::ALLOCA) return ptr;

                if (def->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                {
                    auto* gep = static_cast<LLVMIR::GEPInst*>(def);
                    ptr       = gep->base_ptr;
                    continue;
                }

                break;
            }

            break;
        }

        return ptr;
    }

    bool TSCCPPass::isRelatedToArray(const MemoryLocation& loc, LLVMIR::Operand* array_base) const
    {
        if (!loc.isValid() || !array_base) return false;

        LLVMIR::Operand* loc_base    = getArrayBasePointer(loc.base_ptr);
        LLVMIR::Operand* target_base = getArrayBasePointer(array_base);

        return isSameArray(loc_base, target_base);
    }

    bool TSCCPPass::isSameArray(LLVMIR::Operand* ptr1, LLVMIR::Operand* ptr2) const
    {
        if (!ptr1 || !ptr2) return false;

        LLVMIR::Operand* base1 = getArrayBasePointer(ptr1);
        LLVMIR::Operand* base2 = getArrayBasePointer(ptr2);

        if (!base1 || !base2) return false;
        if (base1 == base2) return true;

        if (base1->type == LLVMIR::OperandType::REG && base2->type == LLVMIR::OperandType::REG)
        {
            auto* reg1 = static_cast<LLVMIR::RegOperand*>(base1);
            auto* reg2 = static_cast<LLVMIR::RegOperand*>(base2);
            return reg1->reg_num == reg2->reg_num;
        }

        if (base1->type == LLVMIR::OperandType::GLOBAL && base2->type == LLVMIR::OperandType::GLOBAL)
        {
            auto* global1 = static_cast<LLVMIR::GlobalOperand*>(base1);
            auto* global2 = static_cast<LLVMIR::GlobalOperand*>(base2);
            return global1->global_name == global2->global_name;
        }

        return false;
    }

    bool TSCCPPass::mayAlias(const MemoryLocation& loc1, const MemoryLocation& loc2) const
    {
        if (!loc1.isValid() || !loc2.isValid()) return true;

        if (loc1.base_ptr != loc2.base_ptr)
        {
            if (loc1.base_ptr->type == LLVMIR::OperandType::REG && loc2.base_ptr->type == LLVMIR::OperandType::REG)
            {
                auto* reg1 = static_cast<LLVMIR::RegOperand*>(loc1.base_ptr);
                auto* reg2 = static_cast<LLVMIR::RegOperand*>(loc2.base_ptr);

                auto it1 = def_map_.find(reg1->reg_num);
                auto it2 = def_map_.find(reg2->reg_num);

                if (it1 != def_map_.end() && it2 != def_map_.end())
                {
                    auto* inst1 = it1->second;
                    auto* inst2 = it2->second;

                    if (inst1->opcode == LLVMIR::IROpCode::ALLOCA && inst2->opcode == LLVMIR::IROpCode::ALLOCA)
                        return false;
                }
            }

            return true;
        }

        return loc1.element_offset == loc2.element_offset;
    }

    bool TSCCPPass::hasCircularDependency(LLVMIR::Instruction* inst, const MemoryLocation& store_loc) const
    {
        if (!inst) return false;

        std::set<LLVMIR::Instruction*> visited;
        return hasCircularDependencyHelper(inst, store_loc, visited);
    }

    bool TSCCPPass::hasCircularDependencyHelper(
        LLVMIR::Instruction* inst, const MemoryLocation& store_loc, std::set<LLVMIR::Instruction*>& visited) const
    {
        if (!inst || visited.count(inst)) return false;
        visited.insert(inst);

        if (inst->opcode == LLVMIR::IROpCode::LOAD)
        {
            auto*          load     = static_cast<LLVMIR::LoadInst*>(inst);
            MemoryLocation load_loc = getMemoryLocation(load->ptr);
            if (load_loc.isValid() && load_loc == store_loc) return true;
        }

        std::vector<int> used_regs = inst->GetUsedRegs();
        for (int reg_num : used_regs)
        {
            auto def_it = def_map_.find(reg_num);
            if (def_it == def_map_.end()) continue;
            if (hasCircularDependencyHelper(def_it->second, store_loc, visited)) return true;
        }

        return false;
    }

    std::vector<LLVMIR::StoreInst*> TSCCPPass::collectReachingStores(LLVMIR::LoadInst* load, const MemoryLocation& loc)
    {
        std::vector<LLVMIR::StoreInst*> reaching_stores;

        auto dom_it = ir->DomTrees.find(current_cfg_);
        if (dom_it == ir->DomTrees.end()) return reaching_stores;

        auto* dom_analyzer = dom_it->second;
        findReachingStoresWithDominatorTree(loc, load, dom_analyzer, reaching_stores);

        return reaching_stores;
    }

    void TSCCPPass::findReachingStoresWithDominatorTree(const MemoryLocation& loc, LLVMIR::LoadInst* target_load,
        Cele::Algo::DomAnalyzer* dom_analyzer, std::vector<LLVMIR::StoreInst*>& reaching_stores)
    {
        int current_block = target_load->block_id;

        while (current_block != -1)
        {
            auto block_it = current_cfg_->block_id_to_block.find(current_block);
            if (block_it == current_cfg_->block_id_to_block.end()) break;

            auto* block = block_it->second;

            auto start_it = block->insts.rbegin();
            auto end_it   = block->insts.rend();

            if (current_block == target_load->block_id)
            {
                auto load_pos = std::find(block->insts.begin(), block->insts.end(), target_load);
                if (load_pos != block->insts.end())
                {
                    start_it = std::reverse_iterator<std::deque<LLVMIR::Instruction*>::iterator>(load_pos);
                    end_it   = block->insts.rend();
                }
            }

            for (auto it = start_it; it != end_it; ++it)
            {
                auto* inst = *it;

                if (inst->opcode == LLVMIR::IROpCode::STORE)
                {
                    auto*          store     = static_cast<LLVMIR::StoreInst*>(inst);
                    MemoryLocation store_loc = getMemoryLocation(store->ptr);

                    if (store_loc.isValid() && store_loc == loc)
                    {
                        reaching_stores.push_back(store);
                        return;
                    }
                    else if (store_loc.isValid())
                    {
                        if (isSameArray(store_loc.base_ptr, loc.base_ptr)) { return; }
                        else if (mayAlias(store_loc, loc)) { return; }
                    }
                    else { return; }
                }
                else if (inst->opcode == LLVMIR::IROpCode::CALL)
                {
                    auto* call = static_cast<LLVMIR::CallInst*>(inst);
                    if (!isSafeFunction(call->func_name)) return;
                }
            }

            if (static_cast<size_t>(current_block) >= dom_analyzer->imm_dom.size()) break;

            int imm_dom = dom_analyzer->imm_dom[current_block];
            if (imm_dom == current_block) break;

            current_block = imm_dom;
        }
    }

    NaturalLoop* TSCCPPass::getLoopForInstruction(LLVMIR::Instruction* inst) const
    {
        if (!current_cfg_ || !current_cfg_->LoopForest) return nullptr;

        int  block_id = inst->block_id;
        auto block_it = current_cfg_->block_id_to_block.find(block_id);
        if (block_it == current_cfg_->block_id_to_block.end()) return nullptr;

        LLVMIR::IRBlock* block = block_it->second;

        NaturalLoop* innermost_loop = nullptr;
        for (auto* loop : current_cfg_->LoopForest->loop_set)
        {
            if (loop->loop_nodes.count(block) > 0)
            {
                if (!innermost_loop || (innermost_loop->loop_nodes.size() > loop->loop_nodes.size()))
                {
                    innermost_loop = loop;
                }
            }
        }

        return innermost_loop;
    }

    bool TSCCPPass::isModifiedInLoop(const MemoryLocation& loc, NaturalLoop* loop) const
    {
        if (!loop || !loc.isValid()) return false;

        for (auto* block : loop->loop_nodes)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::STORE) continue;

                auto*          store     = static_cast<LLVMIR::StoreInst*>(inst);
                MemoryLocation store_loc = getMemoryLocation(store->ptr);

                if (store_loc.isValid() && store_loc == loc) return true;
            }
        }

        return false;
    }

    std::vector<LLVMIR::StoreInst*> TSCCPPass::collectReachingStoresWithLoopAwareness(
        LLVMIR::LoadInst* load, const MemoryLocation& loc)
    {
        std::vector<LLVMIR::StoreInst*> reaching_stores;

        if (!current_cfg_ || !current_cfg_->LoopForest) { return collectReachingStores(load, loc); }

        NaturalLoop* load_loop = getLoopForInstruction(load);

        if (load_loop)
        {
            if (isModifiedInLoop(loc, load_loop)) { return reaching_stores; }
        }
        else
        {
            for (auto* loop : current_cfg_->LoopForest->loop_set)
            {
                if (isModifiedInLoop(loc, loop)) { return reaching_stores; }
            }
        }

        return collectReachingStores(load, loc);
    }

    bool TSCCPPass::isMemoryModifiedInAnyLoop(const MemoryLocation& loc) const
    {
        auto it = memory_modified_in_loops_.find(loc);
        if (it != memory_modified_in_loops_.end() && !it->second.empty()) { return true; }

        if (loc.isValid() && loc.base_ptr)
        {
            auto base_it = base_modified_in_loops_.find(loc.base_ptr);
            if (base_it != base_modified_in_loops_.end() && !base_it->second.empty()) { return true; }
        }

        return false;
    }

    void TSCCPPass::recordMemoryModificationInLoop(const MemoryLocation& loc, NaturalLoop* loop)
    {
        if (loop && loc.isValid()) { memory_modified_in_loops_[loc].insert(loop); }
    }

    void TSCCPPass::recordBaseModificationInLoop(LLVMIR::Operand* base_ptr, NaturalLoop* loop)
    {
        if (loop && base_ptr) { base_modified_in_loops_[base_ptr].insert(loop); }
    }

}  // namespace Transform
