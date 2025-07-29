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
        if (indices.empty()) return result;

        result += "[";
        for (size_t i = 0; i < indices.size(); ++i)
        {
            if (i > 0) result += ",";
            result += std::to_string(indices[i]);
        }
        result += "]";

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

    TSCCPPass::TSCCPPass(LLVMIR::IR* ir, Analysis::AliasAnalyser* aa)
        : Pass(ir), current_cfg_(nullptr), alias_analyser_(aa)
    {
        instruction_visitor_ = std::make_unique<InstructionVisitor>(this);
    }

    TSCCPPass::~TSCCPPass() = default;

    void TSCCPPass::Execute()
    {
        for (const auto& [func_def, cfg] : ir->cfg)
        {
            current_cfg_ = cfg;

            value_map_.clear();
            use_chains_.clear();
            def_map_.clear();
            cfg_worklist_.clear();
            ssa_worklist_.clear();
            executable_edges_.clear();
            memory_state_.clear();

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
        for (const auto& [block_id, block] : cfg->block_id_to_block)
            for (auto* inst : block->insts) updateInstructionOperands(inst);
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
                if (reg_operand->reg_num == reg_num)
                {
                    DBGINFO("TSCCP: Replacing register ", reg_num, " with constant in instruction");
                    replaceWithConstant(operand, value);
                }
            }
        };

        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::PHI:
            {
                auto* phi = static_cast<LLVMIR::PhiInst*>(inst);
                for (auto& phi_pair : phi->vals_for_labels) replaceIfMatch(phi_pair.first);
                break;
            }
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
                auto* arith = static_cast<LLVMIR::ArithmeticInst*>(inst);
                replaceIfMatch(arith->lhs);
                replaceIfMatch(arith->rhs);
                break;
            }
            // 比较指令
            case LLVMIR::IROpCode::ICMP:
            case LLVMIR::IROpCode::FCMP:
            {
                auto* cmp = static_cast<LLVMIR::IcmpInst*>(inst);
                replaceIfMatch(cmp->lhs);
                replaceIfMatch(cmp->rhs);
                break;
            }
            // 分支指令
            case LLVMIR::IROpCode::BR_COND:
            {
                auto* br = static_cast<LLVMIR::BranchCondInst*>(inst);
                replaceIfMatch(br->cond);
                break;
            }
            // 内存指令
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
            case LLVMIR::IROpCode::GETELEMENTPTR:
            {
                auto* gep = static_cast<LLVMIR::GEPInst*>(inst);
                replaceIfMatch(gep->base_ptr);
                for (auto*& index : gep->idxs) replaceIfMatch(index);
                break;
            }
            // 类型转换指令
            case LLVMIR::IROpCode::ZEXT:
            {
                auto* zext = static_cast<LLVMIR::ZextInst*>(inst);
                replaceIfMatch(zext->src);
                break;
            }
            case LLVMIR::IROpCode::SITOFP:
            {
                auto* sitofp = static_cast<LLVMIR::SI2FPInst*>(inst);
                replaceIfMatch(sitofp->f_si);
                break;
            }
            case LLVMIR::IROpCode::FPTOSI:
            {
                auto* fptosi = static_cast<LLVMIR::FP2SIInst*>(inst);
                replaceIfMatch(fptosi->f_fp);
                break;
            }
            case LLVMIR::IROpCode::FPEXT:
            {
                auto* fpext = static_cast<LLVMIR::FPExtInst*>(inst);
                replaceIfMatch(fpext->src);
                break;
            }
            // 函数调用
            case LLVMIR::IROpCode::CALL:
            {
                auto* call = static_cast<LLVMIR::CallInst*>(inst);
                for (auto& arg_pair : call->args) replaceIfMatch(arg_pair.second);
                break;
            }
            // 返回指令
            case LLVMIR::IROpCode::RET:
            {
                auto* ret = static_cast<LLVMIR::RetInst*>(inst);
                if (ret->ret) replaceIfMatch(ret->ret);
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

    /**
     * 如果一条指令结果确定为常量，则理应在常量传播阶段将其引用全部替换
     * 则该指令的计算不再被需要，可以被删除
     */
    void TSCCPPass::eliminateDeadInstructions(CFG* cfg)
    {
        std::vector<LLVMIR::Instruction*> to_eliminate;

        for (auto& [block_id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                auto value            = getValue(inst);
                bool should_eliminate = false;

                if (value.isConstant())
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
                        // 比较指令
                        case LLVMIR::IROpCode::ICMP:
                        case LLVMIR::IROpCode::FCMP:
                        // 类型转换指令
                        case LLVMIR::IROpCode::SITOFP:
                        case LLVMIR::IROpCode::FPTOSI:
                        case LLVMIR::IROpCode::ZEXT:
                        case LLVMIR::IROpCode::FPEXT:
                        // 内存读取指令（需谨慎，可能有副作用）
                        case LLVMIR::IROpCode::LOAD:
                        // GEP指令
                        case LLVMIR::IROpCode::GETELEMENTPTR: should_eliminate = true; break;
                        // 函数调用不能消除，可能有副作用
                        case LLVMIR::IROpCode::CALL:
                        // 内存写入不能消除，有副作用
                        case LLVMIR::IROpCode::STORE:
                        // 控制流指令不能消除
                        case LLVMIR::IROpCode::BR_COND:
                        case LLVMIR::IROpCode::BR_UNCOND:
                        case LLVMIR::IROpCode::RET:
                        // PHI指令不能消除，影响控制流
                        case LLVMIR::IROpCode::PHI:
                        // 分配指令不能消除，影响内存布局
                        case LLVMIR::IROpCode::ALLOCA:
                        // 全局变量定义不能消除
                        case LLVMIR::IROpCode::GLOBAL_VAR:
                        case LLVMIR::IROpCode::GLOBAL_STR:
                        case LLVMIR::IROpCode::OTHER:
                        default: break;
                    }
                }

                if (should_eliminate) to_eliminate.push_back(inst);
            }
        }

        for (auto* dead_inst : to_eliminate)
        {
            int result_reg = dead_inst->GetResultReg();
            if (result_reg == -1) continue;

            auto dead_value = getValue(dead_inst);

            if (use_chains_.find(dead_inst) != use_chains_.end())
                for (auto* use_inst : use_chains_[dead_inst]) mapRegToConstant(use_inst, result_reg, dead_value);
        }

        std::set<LLVMIR::Instruction*> eliminate_set(to_eliminate.begin(), to_eliminate.end());

        for (auto& [block_id, block] : cfg->block_id_to_block)
        {
            std::deque<LLVMIR::Instruction*> new_insts;

            for (auto* inst : block->insts)
                if (eliminate_set.find(inst) == eliminate_set.end()) new_insts.push_back(inst);

            block->insts.clear();
            block->insts.insert(block->insts.end(), new_insts.begin(), new_insts.end());
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

                        // 失效所有与这个数组相关的内存位置
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

        if (!store_loc.isValid())
        {
            DBGINFO("\tSTORE with dynamic indices detected - using selective invalidation");

            LLVMIR::Operand* base_ptr = nullptr;
            if (store->ptr->type == LLVMIR::OperandType::REG)
            {
                auto* reg_operand = static_cast<LLVMIR::RegOperand*>(store->ptr);
                auto  it          = def_map_.find(reg_operand->reg_num);
                if (it != def_map_.end() && it->second->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                {
                    auto* gep = static_cast<LLVMIR::GEPInst*>(it->second);
                    base_ptr  = gep->base_ptr;
                }
            }

            if (base_ptr)
            {
                std::vector<MemoryLocation> to_remove;
                for (const auto& [loc, value] : memory_state_)
                {
                    if (reinterpret_cast<uintptr_t>(loc.base_ptr) == reinterpret_cast<uintptr_t>(base_ptr))
                    {
                        to_remove.push_back(loc);
                    }
                }
                for (const auto& loc : to_remove) { memory_state_.erase(loc); }
                DBGINFO("\tInvalidated ", to_remove.size(), " locations with same base pointer");
            }
            else
            {
                DBGINFO("\tCannot determine base pointer - clearing ALL memory state");
                memory_state_.clear();
            }
            return;
        }

        bool has_circular_dependency = false;
        if (store->val->type == LLVMIR::OperandType::REG)
        {
            auto* reg_operand = static_cast<LLVMIR::RegOperand*>(store->val);
            auto  def_it      = def_map_.find(reg_operand->reg_num);
            if (def_it != def_map_.end())
            {
                has_circular_dependency = hasCircularDependency(def_it->second, store_loc);
            }
        }

        DBGINFO("STORE: ",
            store_loc.toString(),
            " = ",
            (stored_value.isConstant() ? "CONSTANT" : "NON_CONSTANT"),
            (has_circular_dependency ? " (CIRCULAR_DEP)" : ""));

        for (auto it = memory_state_.begin(); it != memory_state_.end();)
        {
            if (it->first != store_loc && mayAlias(store_loc, it->first))
            {
                DBGINFO("\tINVALIDATING: ", it->first.toString(), " (alias conflict)");
                it = memory_state_.erase(it);
            }
            else
                ++it;
        }

        if (stored_value.isConstant() && !has_circular_dependency)
        {
            memory_state_[store_loc] = stored_value;
            DBGINFO("\tSTORED in memory state, total entries: ", memory_state_.size());
        }
        else
        {
            memory_state_.erase(store_loc);
            if (has_circular_dependency)
            {
                DBGINFO("\tERASED from memory state (circular dependency detected)");

                if (store->val->type == LLVMIR::OperandType::REG)
                {
                    auto* reg_operand = static_cast<LLVMIR::RegOperand*>(store->val);
                    auto  def_it      = def_map_.find(reg_operand->reg_num);
                    if (def_it != def_map_.end())
                    {
                        setValue(def_it->second, LatticeValue::createBottom());
                        DBGINFO("\tMarked stored value instruction as BOTTOM");
                    }
                }

                for (const auto& [block_id, block] : current_cfg_->block_id_to_block)
                {
                    for (auto* inst : block->insts)
                    {
                        if (inst->opcode == LLVMIR::IROpCode::LOAD)
                        {
                            auto*          load     = static_cast<LLVMIR::LoadInst*>(inst);
                            MemoryLocation load_loc = getMemoryLocation(load->ptr);
                            if (load_loc.isValid() && load_loc == store_loc)
                            {
                                setValue(load, LatticeValue::createBottom());
                                DBGINFO("\tMarked load from circular dependency location as BOTTOM");
                            }
                        }
                    }
                }
            }
            else
                DBGINFO("\tERASED from memory state");
        }
    }

    /**
     * 处理load指令
     *   1. 若`memory_state_`有相关记录 -> 顺序可达块上刚定义的store，可直接使用其值。
     *   2. 如果在`memory_state_`中找不到，说明值可能在之前的某个基本块中定义。
     *      此时调用`findDefiningStore`，利用支配树信息做后向查找。
     *   3. 如果两种方法都找不到，则该值不明确，返回BOTTOM
     */
    LatticeValue TSCCPPass::handleLoadInstruction(LLVMIR::LoadInst* load)
    {
        MemoryLocation load_loc = getMemoryLocation(load->ptr);

        if (!load_loc.isValid())
        {
            DBGINFO("LOAD with dynamic indices - cannot optimize, returning BOTTOM");
            return LatticeValue::createBottom();
        }

        DBGINFO("LOAD: ", load_loc.toString(), " from memory state with ", memory_state_.size(), " entries");

        auto it = memory_state_.find(load_loc);
        if (it != memory_state_.end())
        {
            DBGINFO("\tFOUND in memory state: ", (it->second.isConstant() ? "CONSTANT" : ""));
            return it->second;
        }

        DBGINFO("\tNOT FOUND in memory state, trying dominance-based search");

        LLVMIR::StoreInst* defining_store = nullptr;
        if (findDefiningStore(load, load_loc, defining_store))
        {
            LatticeValue stored_value = getValueForOperand(defining_store->val);
            DBGINFO("\tFOUND defining store via dominance analysis: ",
                (stored_value.isConstant() ? "CONSTANT" : "NON_CONSTANT"));
            return stored_value;
        }

        DBGINFO("\tCannot determine value safely, returning BOTTOM");
        return LatticeValue::createBottom();
    }

    /**
     * 解析指针操作数，获取内存位置
     *   递归地处理GEP指令，直到找到基址指针（通常是alloca）。
     *   将GEP的所有静态索引收集起来，形成一个精确的`MemoryLocation`。
     *   如果任何索引是动态的（非常量），则认为地址无法确定，返回无效位置。
     */
    MemoryLocation TSCCPPass::getMemoryLocation(LLVMIR::Operand* ptr) const
    {
        if (!ptr) return MemoryLocation();

        if (ptr->type == LLVMIR::OperandType::GLOBAL)
        {
            // 无法确定其他函数是否对全局变量进行修改，此处不处理
            DBGINFO("  GLOBAL variable detected, returning invalid location");
            return MemoryLocation();
        }

        if (ptr->type == LLVMIR::OperandType::REG)
        {
            auto* reg_operand = static_cast<LLVMIR::RegOperand*>(ptr);
            auto  it          = def_map_.find(reg_operand->reg_num);
            if (it != def_map_.end())
            {
                auto* defining_inst = it->second;

                if (dynamic_cast<LLVMIR::FuncDefInst*>(defining_inst))
                {
                    // TODO: Track pointer parameters if they point to stack-allocated memory that we can analyze
                    DBGINFO("  FUNCTION PARAMETER detected, returning invalid location");
                    return MemoryLocation();
                }

                if (defining_inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                {
                    auto* gep = static_cast<LLVMIR::GEPInst*>(defining_inst);

                    DBGINFO("\tGEP instruction found, base_ptr reg: ",
                        (gep->base_ptr->type == LLVMIR::OperandType::REG
                                ? static_cast<LLVMIR::RegOperand*>(gep->base_ptr)->reg_num
                                : -1));
                    DBGINFO("\tGEP has ", gep->idxs.size(), " indices");

                    MemoryLocation base_loc = getMemoryLocation(gep->base_ptr);
                    if (!base_loc.isValid())
                    {
                        DBGINFO("\tBase location invalid");
                        return MemoryLocation();
                    }

                    std::vector<int> indices = base_loc.indices;
                    for (size_t i = 0; i < gep->idxs.size(); ++i)
                    {
                        auto* operand = gep->idxs[i];
                        DBGINFO("\tIndex ", i, ": type=", static_cast<int>(operand->type));

                        if (operand->type == LLVMIR::OperandType::IMMEI32)
                        {
                            // 直接常量，可安全跟踪。初次运行pass时不会进入此分支
                            auto* imm = static_cast<LLVMIR::ImmeI32Operand*>(operand);
                            indices.push_back(imm->value);
                            DBGINFO("\t\timmediate_value=", imm->value);
                        }
                        else if (operand->type == LLVMIR::OperandType::REG)
                        {
                            // 对寄存器而言，需先检查其是否被确定为常量
                            auto* reg_operand = static_cast<LLVMIR::RegOperand*>(operand);
                            auto  def_it      = def_map_.find(reg_operand->reg_num);
                            if (def_it != def_map_.end())
                            {
                                auto*        defining_inst = def_it->second;
                                LatticeValue reg_value     = getValue(defining_inst);
                                if (reg_value.isConstant() && reg_value.getType() == LatticeValue::ValueType::INTEGER)
                                {
                                    indices.push_back(reg_value.getIntValue());
                                    DBGINFO("\t\tregister_constant_value=", reg_value.getIntValue());
                                }
                                else
                                {
                                    DBGINFO("\t\tregister_non_constant_or_unknown");
                                    DBGINFO("\tRejecting location due to dynamic index");
                                    return MemoryLocation();
                                }
                            }
                            else
                            {
                                DBGINFO("\t\tundefined_register");
                                DBGINFO("\tRejecting location due to undefined register");
                                return MemoryLocation();
                            }
                        }
                        else
                        {
                            DBGINFO("\t\tunsupported_operand_type");
                            DBGINFO("\tRejecting location due to unsupported operand type");
                            return MemoryLocation();
                        }
                    }

                    DBGINFO(
                        "\tFinal indices: [",
                        [&]() {
                            std::string result;
                            for (size_t i = 0; i < indices.size(); ++i)
                            {
                                if (i > 0) result += ",";
                                result += std::to_string(indices[i]);
                            }
                            return result;
                        }(),
                        "]");

                    return MemoryLocation(base_loc.base_ptr, indices);
                }

                if (defining_inst->opcode == LLVMIR::IROpCode::ALLOCA) return MemoryLocation(ptr);
            }
        }

        return MemoryLocation(ptr);
    }

    /**
     * 利用支配树查找定义store
     *   当load的值在当前内存状态中未知时，此函数被调用。
     *   1. 沿支配树从load所在块向上回溯。
     *   2. 在每个支配块中，从后向前搜索指令。
     *   3. 找到第一个写入相同`MemoryLocation`的store指令。
     *   4. 检查从该store到load的路径上是否跨越了循环(`hasLoopBetweenBlocks`)。
     *      如果跨越了循环，那么控制流不再是单向的，可能出现同前文提到循环引用的类似问题，因此不能使用该store的值
     *   5. 如果找到任何可能冲突的store（别名）或不安全的函数调用，则停止搜索。
     */
    bool TSCCPPass::findDefiningStore(
        LLVMIR::LoadInst* load, const MemoryLocation& loc, LLVMIR::StoreInst*& defining_store)
    {
        defining_store = nullptr;

        if (!loc.isValid())
        {
            DBGINFO("\tInvalid memory location for dominance search");
            return false;
        }

        auto dom_it = ir->DomTrees.find(current_cfg_);
        if (dom_it == ir->DomTrees.end())
        {
            DBGINFO("\tNo dominance tree available for current CFG");
            return false;
        }

        auto* dom_analyzer = dom_it->second;
        int   load_block   = load->block_id;

        DBGINFO("\tSearching for defining store using dominance analysis, load in block ", load_block);

        if (load_block < 0 || static_cast<size_t>(load_block) >= dom_analyzer->imm_dom.size())
        {
            DBGINFO("\tLoad block ", load_block, " is out of range for dominance tree");
            return false;
        }

        // 检查是否在循环中或跨循环边界 - 如果是，则无法确定是否存在类循环引用，保守处理
        if (isInLoopOrCrossesLoop(load_block))
        {
            DBGINFO("\tLoad is in loop, being conservative");
            return false;
        }

        std::vector<int> dominating_blocks;
        int              current_block = load_block;
        std::set<int>    visited_blocks;

        while (current_block != -1)
        {
            if (visited_blocks.count(current_block))
            {
                DBGINFO("\tDetected cycle in dominance tree at block ", current_block);
                break;
            }

            if (static_cast<size_t>(current_block) >= dom_analyzer->imm_dom.size())
            {
                DBGINFO("\tBlock ", current_block, " is out of range for dominance tree");
                break;
            }

            dominating_blocks.push_back(current_block);
            visited_blocks.insert(current_block);

            current_block = dom_analyzer->imm_dom[current_block];
            DBGINFO("\t\tBlock ", dominating_blocks.back(), " -> ", current_block);
        }

        DBGINFO("\tFound ", dominating_blocks.size(), " dominating blocks to search");

        for (int block_id : dominating_blocks)
        {
            auto block_it = current_cfg_->block_id_to_block.find(block_id);
            if (block_it == current_cfg_->block_id_to_block.end()) continue;

            auto* block = block_it->second;
            DBGINFO("\tSearching in dominating block ", block_id);

            if (block_id == load_block)
            {
                bool found_load = false;
                for (auto it = block->insts.rbegin(); it != block->insts.rend(); ++it)
                {
                    auto* inst = *it;

                    if (inst == load)
                    {
                        found_load = true;
                        continue;
                    }

                    if (!found_load) continue;

                    if (inst->opcode == LLVMIR::IROpCode::STORE)
                    {
                        auto*          store     = static_cast<LLVMIR::StoreInst*>(inst);
                        MemoryLocation store_loc = getMemoryLocation(store->ptr);

                        if (store_loc.isValid() && store_loc == loc)
                        {
                            DBGINFO("\t\tFound defining store in same block before load");
                            defining_store = store;
                            return true;
                        }

                        if (store_loc.isValid() && mayAlias(store_loc, loc))
                        {
                            DBGINFO("\t\tFound potentially aliasing store, stopping search");
                            return false;
                        }
                    }
                    else if (inst->opcode == LLVMIR::IROpCode::CALL)
                    {
                        // 无法确定函数调用中是否会对内存进行修改，因此返回false
                        auto* call = static_cast<LLVMIR::CallInst*>(inst);
                        if (!isSafeFunction(call->func_name))
                        {
                            DBGINFO("\t\tFound potentially unsafe function call, stopping search");
                            return false;
                        }
                    }
                }
            }
            else
            {
                // 不同块，需要检查从defining store到load的路径上是否有循环干扰

                // 先在块中寻找潜在的defining store
                LLVMIR::StoreInst* candidate_store = nullptr;
                for (auto it = block->insts.rbegin(); it != block->insts.rend(); ++it)
                {
                    auto* inst = *it;

                    if (inst->opcode == LLVMIR::IROpCode::STORE)
                    {
                        auto*          store     = static_cast<LLVMIR::StoreInst*>(inst);
                        MemoryLocation store_loc = getMemoryLocation(store->ptr);

                        if (store_loc.isValid() && store_loc == loc)
                        {
                            candidate_store = store;
                            break;
                        }

                        if (store_loc.isValid() && mayAlias(store_loc, loc))
                        {
                            DBGINFO("\t\tFound potentially aliasing store in block ", block_id, ", stopping search");
                            return false;
                        }
                    }
                    else if (inst->opcode == LLVMIR::IROpCode::CALL)
                    {
                        // 无法确定函数调用中是否会对内存进行修改，因此返回false
                        auto* call = static_cast<LLVMIR::CallInst*>(inst);
                        if (!isSafeFunction(call->func_name))
                        {
                            DBGINFO(
                                "\t\tFound potentially unsafe function call in block ", block_id, ", stopping search");
                            return false;
                        }
                    }
                }

                if (candidate_store)
                {
                    // 检查从candidate_store所在的块到load所在的块之间是否有循环干扰
                    if (hasLoopBetweenBlocks(block_id, load_block))
                    {
                        DBGINFO("\t\tFound defining store in block ",
                            block_id,
                            " but there are loops between store and load, being conservative");
                        return false;
                    }

                    DBGINFO("\t\tFound defining store in dominating block ", block_id, " with no loop interference");
                    defining_store = candidate_store;
                    return true;
                }
            }
        }

        DBGINFO("\tNo defining store found in dominating blocks");
        return false;
    }

    bool TSCCPPass::isInLoopOrCrossesLoop(int block_id) const
    {
        auto block_it = current_cfg_->block_id_to_block.find(block_id);
        if (block_it == current_cfg_->block_id_to_block.end()) return false;

        LLVMIR::IRBlock* block = block_it->second;

        if (!current_cfg_->LoopForest || current_cfg_->LoopForest->loop_set.empty())
        {
            DBGINFO("\tNo loop information available, being conservative and assuming in loop");
            return true;
        }

        for (auto* loop : current_cfg_->LoopForest->loop_set)
        {
            if (loop->loop_nodes.find(block) != loop->loop_nodes.end())
            {
                DBGINFO("\tBlock ",
                    block_id,
                    " is in loop ",
                    loop->loop_id,
                    " with header ",
                    (loop->header ? loop->header->block_id : -1));
                return true;
            }
        }

        DBGINFO("\tBlock ", block_id, " is not in any loop");
        return false;
    }

    /**
     * 检查从`from_block`到`to_block`的路径上是否有循环，保证找到的store通过单向控制流支配load
     */
    bool TSCCPPass::hasLoopBetweenBlocks(int from_block, int to_block) const
    {
        if (from_block == to_block) return false;

        std::queue<int> worklist;
        std::set<int>   visited;

        worklist.push(from_block);
        visited.insert(from_block);

        while (!worklist.empty())
        {
            int current = worklist.front();
            worklist.pop();

            // 如果到达目标块，检查这条路径上是否有循环
            if (current == to_block)
            {
                // 检查访问路径上的所有块是否在循环中
                for (int block_id : visited)
                {
                    if (block_id == from_block || block_id == to_block) continue;  // 跳过起点和终点

                    // 检查这个块是否在循环中
                    auto block_it = current_cfg_->block_id_to_block.find(block_id);
                    if (block_it == current_cfg_->block_id_to_block.end()) continue;

                    LLVMIR::IRBlock* block = block_it->second;
                    if (current_cfg_->LoopForest && !current_cfg_->LoopForest->loop_set.empty())
                    {
                        for (auto* loop : current_cfg_->LoopForest->loop_set)
                        {
                            if (loop->loop_nodes.find(block) != loop->loop_nodes.end())
                            {
                                DBGINFO("\tFound loop interference: block ",
                                    block_id,
                                    " is in loop ",
                                    loop->loop_id,
                                    " on path from ",
                                    from_block,
                                    " to ",
                                    to_block);
                                return true;
                            }
                        }
                    }
                }
                continue;  // 继续搜索其他路径
            }

            if (static_cast<size_t>(current) < current_cfg_->G_id.size())
            {
                for (int succ_id : current_cfg_->G_id[current])
                {
                    if (visited.find(succ_id) == visited.end())
                    {
                        visited.insert(succ_id);
                        worklist.push(succ_id);
                    }
                }
            }
        }

        DBGINFO("\tNo loop interference found on path from ", from_block, " to ", to_block);
        return false;
    }

    /**
     * 判断一个函数是否"安全"
     * "安全"意味着该函数不会修改任何通过指针传入的用户内存状态，或者其修改
     * 是可预测的。例如，`putint`是安全的，因为它只输出值，不修改内存。
     * 而`getarray`或一个未知的用户函数是不安全的。
     */
    bool TSCCPPass::isSafeFunction(const std::string& func_name) const
    {
        static const std::set<std::string> safe_functions = {
            "putint", "putch", "putarray", "putfloat", "putfarray", "_sysy_starttime", "_sysy_stoptime"};

        if (func_name.find("llvm.") == 0)
        {
            if (func_name.find("llvm.memset") == 0 || func_name.find("llvm.memcpy") == 0 ||
                func_name.find("llvm.memmove") == 0)
            {
                return false;
            }
            return true;
        }

        return safe_functions.count(func_name) > 0;
    }

    LLVMIR::Operand* TSCCPPass::getArrayBasePointer(LLVMIR::Operand* ptr) const
    {
        if (!ptr) return nullptr;

        // 如果是寄存器，查找定义它的指令
        if (ptr->type == LLVMIR::OperandType::REG)
        {
            auto* reg_operand = static_cast<LLVMIR::RegOperand*>(ptr);
            auto  it          = def_map_.find(reg_operand->reg_num);
            if (it != def_map_.end())
            {
                auto* defining_inst = it->second;

                // 如果是GEP指令，返回它的基础指针
                if (defining_inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                {
                    auto* gep = static_cast<LLVMIR::GEPInst*>(defining_inst);
                    return getArrayBasePointer(gep->base_ptr);
                }
                // 如果是alloca指令，这就是基础指针
                else if (defining_inst->opcode == LLVMIR::IROpCode::ALLOCA)
                    return ptr;
            }
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

                    // 两条 alloc 产生的结果不会为同一个
                    if (inst1->opcode == LLVMIR::IROpCode::ALLOCA && inst2->opcode == LLVMIR::IROpCode::ALLOCA)
                        return false;
                }
            }

            if (alias_analyser_)
                return alias_analyser_->queryAlias(loc1.base_ptr, loc2.base_ptr, current_cfg_) !=
                       Analysis::AliasAnalyser::NoAlias;
            return true;
        }

        if (loc1.indices == loc2.indices) return true;
        if (!loc1.indices.empty() && !loc2.indices.empty()) return false;

        return true;
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

}  // namespace Transform
