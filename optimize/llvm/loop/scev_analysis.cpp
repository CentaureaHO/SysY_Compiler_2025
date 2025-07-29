#include "llvm/loop/scev_analysis.h"
#include "llvm_ir/ir_builder.h"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <functional>
#include <cmath>

namespace Analysis
{
    LLVMIR::IcmpCond getInverseIcmpCond(LLVMIR::IcmpCond cond);
    LLVMIR::IcmpCond getSwapIcmpCond(LLVMIR::IcmpCond cond);

    CROperand::CROperand(std::unique_ptr<ChainOfRecurrences> cr) : type(CHAIN_OF_RECURRENCES), nested_cr(std::move(cr))
    {}

    CROperand::CROperand(const CROperand& other) : type(other.type)
    {
        switch (type)
        {
            case CONSTANT: const_val = other.const_val; break;
            case LLVM_OPERAND: llvm_op = other.llvm_op; break;
            case CHAIN_OF_RECURRENCES:
                if (other.nested_cr) { nested_cr = std::make_unique<ChainOfRecurrences>(*other.nested_cr); }
                break;
        }
    }

    CROperand& CROperand::operator=(const CROperand& other)
    {
        if (this != &other)
        {
            type = other.type;
            switch (type)
            {
                case CONSTANT: const_val = other.const_val; break;
                case LLVM_OPERAND: llvm_op = other.llvm_op; break;
                case CHAIN_OF_RECURRENCES:
                    if (other.nested_cr) { nested_cr = std::make_unique<ChainOfRecurrences>(*other.nested_cr); }
                    else { nested_cr.reset(); }
                    break;
            }
        }
        return *this;
    }

    CROperand::CROperand(CROperand&& other) noexcept : type(other.type)
    {
        switch (type)
        {
            case CONSTANT: const_val = other.const_val; break;
            case LLVM_OPERAND: llvm_op = other.llvm_op; break;
            case CHAIN_OF_RECURRENCES: nested_cr = std::move(other.nested_cr); break;
        }
    }

    CROperand& CROperand::operator=(CROperand&& other) noexcept
    {
        if (this != &other)
        {
            type = other.type;
            switch (type)
            {
                case CONSTANT: const_val = other.const_val; break;
                case LLVM_OPERAND: llvm_op = other.llvm_op; break;
                case CHAIN_OF_RECURRENCES: nested_cr = std::move(other.nested_cr); break;
            }
        }
        return *this;
    }

    std::optional<int32_t> CROperand::getConstantValue() const
    {
        switch (type)
        {
            case CONSTANT: return const_val;
            case LLVM_OPERAND:
                if (llvm_op && llvm_op->type == LLVMIR::OperandType::IMMEI32)
                {
                    auto* imm = static_cast<LLVMIR::ImmeI32Operand*>(llvm_op);
                    return imm->value;
                }
                break;
            case CHAIN_OF_RECURRENCES: break;
        }
        return std::nullopt;
    }

    LLVMIR::Instruction* CROperand::generateInstruction(CFG* cfg) const
    {
        switch (type)
        {
            case CONSTANT:
                return new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                    LLVMIR::DataType::I32,
                    getImmeI32Operand(const_val),
                    getImmeI32Operand(0),
                    getRegOperand(++cfg->func->max_reg));
            case LLVM_OPERAND:
                return new LLVMIR::ArithmeticInst(LLVMIR::IROpCode::ADD,
                    LLVMIR::DataType::I32,
                    llvm_op,
                    getImmeI32Operand(0),
                    getRegOperand(++cfg->func->max_reg));
            case CHAIN_OF_RECURRENCES:
                if (nested_cr)
                {
                    auto insts = nested_cr->generateInstructions(cfg);
                    return insts.empty() ? nullptr : insts.back();
                }
                break;
        }
        return nullptr;
    }

    bool CROperand::isLoopInvariant(const std::set<int>& invariant_set) const
    {
        switch (type)
        {
            case CONSTANT: return true;
            case LLVM_OPERAND:
                if (llvm_op && llvm_op->type == LLVMIR::OperandType::REG)
                {
                    auto* reg = static_cast<LLVMIR::RegOperand*>(llvm_op);
                    return invariant_set.find(reg->reg_num) != invariant_set.end();
                }
                return true;
            case CHAIN_OF_RECURRENCES: return false;
        }
        return false;
    }

    void CROperand::print() const
    {
        switch (type)
        {
            case CONSTANT: std::cout << const_val; break;
            case LLVM_OPERAND:
                if (llvm_op) { std::cout << llvm_op->getName(); }
                else { std::cout << "null"; }
                break;
            case CHAIN_OF_RECURRENCES:
                if (nested_cr) { nested_cr->print(); }
                else { std::cout << "null_cr"; }
                break;
        }
    }

    bool CROperand::operator==(const CROperand& other) const
    {
        if (type != other.type) return false;

        switch (type)
        {
            case CONSTANT: return const_val == other.const_val;
            case LLVM_OPERAND: return llvm_op == other.llvm_op;
            case CHAIN_OF_RECURRENCES: return (nested_cr == nullptr) == (other.nested_cr == nullptr);
        }
        return false;
    }

    ChainOfRecurrences::ChainOfRecurrences(const CROperand& start, CROperator op, const CROperand& step)
    {
        operands  = {start, step};
        operators = {op};
    }

    ChainOfRecurrences::ChainOfRecurrences(const std::vector<CROperand>& ops, const std::vector<CROperator>& opers)
        : operands(ops), operators(opers)
    {
        assert(operands.size() == operators.size() + 1);
    }

    bool ChainOfRecurrences::isPureSum() const
    {
        return std::all_of(operators.begin(), operators.end(), [](CROperator op) { return op == CROperator::ADD; });
    }

    bool ChainOfRecurrences::isPureProduct() const
    {
        return std::all_of(operators.begin(), operators.end(), [](CROperator op) { return op == CROperator::MUL; });
    }

    bool ChainOfRecurrences::isSimple() const
    {
        if (operands.empty()) return false;
        return operands.back().type == CROperand::CONSTANT ||
               (operands.back().type == CROperand::LLVM_OPERAND &&
                   operands.back().llvm_op->type == LLVMIR::OperandType::IMMEI32);
    }

    int ChainOfRecurrences::getCostIndex() const
    {
        int cost = static_cast<int>(operators.size());
        for (const auto& operand : operands)
        {
            if (operand.type == CROperand::CHAIN_OF_RECURRENCES && operand.nested_cr)
            {
                cost += operand.nested_cr->getCostIndex();
            }
        }
        return cost;
    }

    ChainOfRecurrences ChainOfRecurrences::operator+(const ChainOfRecurrences& other) const
    {
        return *CRBuilder::crAdd(*this, other);
    }

    ChainOfRecurrences ChainOfRecurrences::operator*(const ChainOfRecurrences& other) const
    {
        return *CRBuilder::crMul(*this, other);
    }

    ChainOfRecurrences ChainOfRecurrences::operator+(const CROperand& constant) const
    {
        if (operators.empty()) { return ChainOfRecurrences({operands[0], constant}, {CROperator::ADD}); }

        if (operators[0] == CROperator::ADD)
        {
            ChainOfRecurrences result       = *this;
            auto               result_val   = result.operands[0].getConstantValue();
            auto               constant_val = constant.getConstantValue();
            if (result_val && constant_val) { result.operands[0] = CROperand(*result_val + *constant_val); }
            return result;
        }

        return *this;
    }

    ChainOfRecurrences ChainOfRecurrences::operator*(const CROperand& constant) const
    {
        if (isPureSum())
        {
            ChainOfRecurrences result       = *this;
            auto               constant_val = constant.getConstantValue();
            if (constant_val)
            {
                for (auto& operand : result.operands)
                {
                    auto operand_val = operand.getConstantValue();
                    if (operand_val) { operand = CROperand(*operand_val * *constant_val); }
                }
            }
            return result;
        }

        if (operators.size() > 0 && operators[0] == CROperator::MUL)
        {
            ChainOfRecurrences result       = *this;
            auto               result_val   = result.operands[0].getConstantValue();
            auto               constant_val = constant.getConstantValue();
            if (result_val && constant_val) { result.operands[0] = CROperand(*result_val * *constant_val); }
            return result;
        }

        return *this;
    }

    CROperand ChainOfRecurrences::evaluateClosedForm(int iteration) const
    {
        bool all_constants = true;
        for (const auto& operand : operands)
        {
            if (!operand.getConstantValue())
            {
                all_constants = false;
                break;
            }
        }

        if (!all_constants)
        {
            // 如果包含非常量操作数，返回0
            return CROperand(0);
        }

        if (isPureSum())
        {
            int result = 0;

            for (size_t j = 0; j < operands.size(); ++j)
            {
                auto const_val      = operands[j].getConstantValue();
                int  coeff          = *const_val;
                int  binomial_coeff = 1;

                for (size_t k = 0; k < j; ++k)
                {
                    binomial_coeff *= (iteration - k);
                    binomial_coeff /= (k + 1);
                }

                result += coeff * binomial_coeff;
            }

            return CROperand(result);
        }
        else if (isPureProduct())
        {
            if (operands.size() == 2)
            {
                auto phi0_val = operands[0].getConstantValue();
                auto phi1_val = operands[1].getConstantValue();

                if (phi0_val && phi1_val)
                {
                    int result = *phi0_val;
                    for (int i = 0; i < iteration; ++i) { result *= *phi1_val; }
                    return CROperand(result);
                }
            }

            return CROperand(0);
        }
        else { return CROperand(0); }
    }

    void ChainOfRecurrences::print() const
    {
        std::cout << "{";
        for (size_t i = 0; i < operands.size(); ++i)
        {
            operands[i].print();
            if (i < operators.size()) { std::cout << (operators[i] == CROperator::ADD ? ",+," : ",*,"); }
        }
        std::cout << "}";
    }

    std::vector<LLVMIR::Instruction*> ChainOfRecurrences::generateInstructions(CFG* cfg) const
    {
        std::vector<LLVMIR::Instruction*> instructions;
        for (const auto& operand : operands)
        {
            auto* inst = operand.generateInstruction(cfg);
            if (inst) { instructions.push_back(inst); }
        }
        return instructions;
    }

    std::unique_ptr<ChainOfRecurrences> CRBuilder::createFromBasicInductionVar(
        LLVMIR::Operand* start, LLVMIR::Operand* step, CROperator op)
    {
        CROperand start_op(start);
        CROperand step_op(step);
        return std::make_unique<ChainOfRecurrences>(start_op, op, step_op);
    }

    std::unique_ptr<ChainOfRecurrences> CRBuilder::createFromArithmeticExpr(
        LLVMIR::ArithmeticInst* inst, const std::map<int, std::unique_ptr<ChainOfRecurrences>>& cr_map)
    {
        if (!inst) return nullptr;

        auto lhs_cr = [&]() -> std::unique_ptr<ChainOfRecurrences> {
            if (inst->lhs->type == LLVMIR::OperandType::REG)
            {
                auto* reg = static_cast<LLVMIR::RegOperand*>(inst->lhs);
                auto  it  = cr_map.find(reg->reg_num);
                if (it != cr_map.end()) { return std::make_unique<ChainOfRecurrences>(*it->second); }
            }
            return std::make_unique<ChainOfRecurrences>(
                std::vector<CROperand>{CROperand(inst->lhs)}, std::vector<CROperator>{});
        }();

        auto rhs_cr = [&]() -> std::unique_ptr<ChainOfRecurrences> {
            if (inst->rhs->type == LLVMIR::OperandType::REG)
            {
                auto* reg = static_cast<LLVMIR::RegOperand*>(inst->rhs);
                auto  it  = cr_map.find(reg->reg_num);
                if (it != cr_map.end()) { return std::make_unique<ChainOfRecurrences>(*it->second); }
            }
            return std::make_unique<ChainOfRecurrences>(
                std::vector<CROperand>{CROperand(inst->rhs)}, std::vector<CROperator>{});
        }();

        if (!lhs_cr || !rhs_cr) return nullptr;

        switch (inst->opcode)
        {
            case LLVMIR::IROpCode::ADD: return crAdd(*lhs_cr, *rhs_cr);
            case LLVMIR::IROpCode::MUL: return crMul(*lhs_cr, *rhs_cr);
            default:
                // TODO: 处理其他运算符
                return nullptr;
        }
    }

    std::unique_ptr<ChainOfRecurrences> CRBuilder::crAdd(const ChainOfRecurrences& lhs, const ChainOfRecurrences& rhs)
    {
        if (lhs.operators.empty() && rhs.operators.empty())
        {
            auto lhs_val = lhs.operands[0].getConstantValue();
            auto rhs_val = rhs.operands[0].getConstantValue();
            if (lhs_val && rhs_val)
            {
                return std::make_unique<ChainOfRecurrences>(
                    std::vector<CROperand>{CROperand(*lhs_val + *rhs_val)}, std::vector<CROperator>{});
            }
        }

        if (lhs.isPureSum() && rhs.isPureSum())
        {
            size_t                  max_len = std::max(lhs.operands.size(), rhs.operands.size());
            std::vector<CROperand>  result_operands;
            std::vector<CROperator> result_operators;

            for (size_t i = 0; i < max_len; ++i)
            {
                CROperand result_op(0);

                if (i < lhs.operands.size() && i < rhs.operands.size())
                {
                    auto lhs_val = lhs.operands[i].getConstantValue();
                    auto rhs_val = rhs.operands[i].getConstantValue();
                    if (lhs_val && rhs_val) { result_op = CROperand(*lhs_val + *rhs_val); }
                    else { result_op = lhs.operands[i]; }
                }
                else if (i < lhs.operands.size()) { result_op = lhs.operands[i]; }
                else if (i < rhs.operands.size()) { result_op = rhs.operands[i]; }

                result_operands.push_back(result_op);
                if (i < max_len - 1) { result_operators.push_back(CROperator::ADD); }
            }

            return std::make_unique<ChainOfRecurrences>(result_operands, result_operators);
        }

        return nullptr;
    }

    std::unique_ptr<ChainOfRecurrences> CRBuilder::crMul(const ChainOfRecurrences& lhs, const ChainOfRecurrences& rhs)
    {
        if (rhs.operators.empty())
        {
            auto rhs_val = rhs.operands[0].getConstantValue();
            if (rhs_val) { return std::make_unique<ChainOfRecurrences>(lhs * CROperand(*rhs_val)); }
        }

        if (lhs.operators.empty())
        {
            auto lhs_val = lhs.operands[0].getConstantValue();
            if (lhs_val) { return std::make_unique<ChainOfRecurrences>(rhs * CROperand(*lhs_val)); }
        }

        if (lhs.isPureProduct() && rhs.isPureProduct())
        {
            size_t                  max_len = std::max(lhs.operands.size(), rhs.operands.size());
            std::vector<CROperand>  result_operands;
            std::vector<CROperator> result_operators;

            for (size_t i = 0; i < max_len; ++i)
            {
                CROperand result_op(1);

                if (i < lhs.operands.size() && i < rhs.operands.size())
                {
                    auto lhs_val = lhs.operands[i].getConstantValue();
                    auto rhs_val = rhs.operands[i].getConstantValue();
                    if (lhs_val && rhs_val) { result_op = CROperand(*lhs_val * *rhs_val); }
                }
                else if (i < lhs.operands.size()) { result_op = lhs.operands[i]; }
                else if (i < rhs.operands.size()) { result_op = rhs.operands[i]; }

                result_operands.push_back(result_op);
                if (i < max_len - 1) { result_operators.push_back(CROperator::MUL); }
            }

            return std::make_unique<ChainOfRecurrences>(result_operands, result_operators);
        }

        if (lhs.isPureSum() && rhs.isPureSum()) { return crProd(lhs, rhs); }

        return nullptr;
    }

    std::unique_ptr<ChainOfRecurrences> CRBuilder::crProd(const ChainOfRecurrences& phi, const ChainOfRecurrences& psi)
    {
        if (!phi.isPureSum() || !psi.isPureSum()) { return nullptr; }

        size_t k = phi.length();
        size_t l = psi.length();

        if (k < l) { return crProd(psi, phi); }
        if (l == 0)
        {
            auto psi_val = psi.operands[0].getConstantValue();
            if (psi_val) { return std::make_unique<ChainOfRecurrences>(phi * CROperand(*psi_val)); }
            return nullptr;
        }
        if (l == 1)
        {
            auto psi0 = psi.operands[0].getConstantValue();
            auto psi1 = psi.operands[1].getConstantValue();

            if (psi0 && psi1)
            {
                std::vector<CROperand>  result_operands;
                std::vector<CROperator> result_operators;

                auto phi0_val = phi.operands[0].getConstantValue();
                if (phi0_val) { result_operands.push_back(CROperand(*phi0_val * *psi0)); }
                else
                {
                    return nullptr;  // 此处需要常量
                }

                for (size_t i = 1; i <= k; ++i)
                {
                    auto phi_i_minus_1_val = (i > 1) ? phi.operands[i - 1].getConstantValue() : phi0_val;
                    auto phi_i_val         = phi.operands[i].getConstantValue();

                    if (phi_i_minus_1_val && phi_i_val)
                    {
                        int term1 = i * (*phi_i_minus_1_val) * (*psi1);
                        int term2 = (*psi0 + i * (*psi1)) * (*phi_i_val);
                        result_operands.push_back(CROperand(term1 + term2));
                        result_operators.push_back(CROperator::ADD);
                    }
                    else { return nullptr; }
                }

                auto phi_k_val = phi.operands[k].getConstantValue();
                if (phi_k_val)
                {
                    result_operands.push_back(CROperand((k + 1) * (*phi_k_val) * (*psi1)));
                    result_operators.push_back(CROperator::ADD);
                }

                return std::make_unique<ChainOfRecurrences>(result_operands, result_operators);
            }
        }

        std::vector<CROperand>  f1_operands(phi.operands.begin() + 1, phi.operands.end());
        std::vector<CROperator> f1_operators(phi.operators.begin() + 1, phi.operators.end());
        ChainOfRecurrences      f1(f1_operands, f1_operators);

        std::vector<CROperand>  g1_operands(psi.operands.begin() + 1, psi.operands.end());
        std::vector<CROperator> g1_operators(psi.operators.begin() + 1, psi.operators.end());
        ChainOfRecurrences      g1(g1_operands, g1_operators);

        ChainOfRecurrences psi_prime = psi + g1;

        auto xi_prime        = crProd(phi, g1);
        auto xi_double_prime = crProd(f1, psi_prime);

        if (!xi_prime || !xi_double_prime) return nullptr;

        auto phi0_val = phi.operands[0].getConstantValue();
        auto psi0_val = psi.operands[0].getConstantValue();

        if (!phi0_val || !psi0_val) return nullptr;

        std::vector<CROperand>  result_operands;
        std::vector<CROperator> result_operators;

        result_operands.push_back(CROperand(*phi0_val * *psi0_val));

        size_t max_len = std::max(xi_prime->operands.size(), xi_double_prime->operands.size());
        for (size_t i = 1; i < max_len; ++i)
        {
            auto xi_p_val =
                (i < xi_prime->operands.size()) ? xi_prime->operands[i].getConstantValue() : std::optional<int>(0);
            auto xi_pp_val = (i < xi_double_prime->operands.size()) ? xi_double_prime->operands[i].getConstantValue()
                                                                    : std::optional<int>(0);

            if (xi_p_val && xi_pp_val)
            {
                result_operands.push_back(CROperand(*xi_p_val + *xi_pp_val));
                result_operators.push_back(CROperator::ADD);
            }
        }

        return std::make_unique<ChainOfRecurrences>(result_operands, result_operators);
    }

    std::unique_ptr<ChainOfRecurrences> CRBuilder::crExpt(
        const ChainOfRecurrences& base, const ChainOfRecurrences& exponent)
    {
        return nullptr;
    }

    std::unique_ptr<ChainOfRecurrences> CRBuilder::simplify(std::unique_ptr<ChainOfRecurrences> cr)
    {
        if (!cr) return nullptr;
        cr = constantFold(std::move(cr));
        return cr;
    }

    std::unique_ptr<ChainOfRecurrences> CRBuilder::constantFold(std::unique_ptr<ChainOfRecurrences> cr)
    {
        if (!cr) return nullptr;

        for (size_t i = 0; i < cr->operands.size(); ++i)
        {
            if (cr->operands[i].type == CROperand::CONSTANT)
            {
                auto val = cr->operands[i].getConstantValue();
                if (val && *val == 0 && i > 0)
                {
                    cr->operands.erase(cr->operands.begin() + i);
                    if (i < cr->operators.size()) { cr->operators.erase(cr->operators.begin() + i - 1); }
                    break;
                }
            }
        }

        return cr;
    }

    ChainOfRecurrences* LoopCRInfo::getCR(int reg_num) const
    {
        auto it = cr_map.find(reg_num);
        return (it != cr_map.end()) ? it->second.get() : nullptr;
    }

    bool LoopCRInfo::canUnroll() const { return is_simple_loop && getConstantTripCount().has_value(); }

    std::optional<int> LoopCRInfo::getConstantTripCount() const
    {
        if (!is_simple_loop) return std::nullopt;

        auto lower    = loop_info.lowerbound.getConstantValue();
        auto upper    = loop_info.upperbound.getConstantValue();
        auto step_val = loop_info.step.getConstantValue();
        std::cout << "lower: " << *lower << ", upper: " << *upper << ", step_val: " << *step_val << std::endl;

        if (lower && upper && step_val && *step_val > 0)
        {
            // 使用保存的选中归纳变量CR来判断是否为乘法递增
            bool is_multiplicative = loop_info.isMultiplicativeInductionVar();

            int count = 0;
            if (is_multiplicative && *step_val > 1)
            {
                // 乘法递增：计算 start * step^n >= upper 的最小n
                // 即：n = ceil(log(upper/start) / log(step))
                double ratio    = static_cast<double>(*upper) / static_cast<double>(*lower);
                double log_step = std::log(static_cast<double>(*step_val));

                if (log_step > 0)
                {
                    double n = std::log(ratio) / log_step;
                    count    = static_cast<int>(std::ceil(n));
                }
                else { return std::nullopt; }
            }
            else
            {
                // 加法递增：修正的计算方式
                // 对于 i < upper: count = floor((upper - lower) / step) + 1
                // 对于 i <= upper: count = floor((upper - lower) / step) + 1
                count    = 0;
                int lb   = *lower;
                int ub   = *upper;
                int step = *step_val;
                while (lb < ub)
                {
                    lb += step;
                    ++count;
                }

                if (loop_info.is_upperbound_closed && lb == ub) ++count;
            }

            // 对于闭合上界，不需要额外调整（已经在上面的公式中处理）
            return count;
        }

        return std::nullopt;
    }

    void LoopCRInfo::printInfo() const
    {
        std::cout << "\n=== CR Analysis for Loop " << loop->loop_id << " (header: Block" << loop->header->block_id
                  << ") in function " << loop->cfg->func->func_def->func_name << " ===" << std::endl;

        std::cout << "Invariant Variables: ";
        for (int reg : invariant_regs) std::cout << "reg_" << reg << " ";
        std::cout << std::endl;

        std::cout << "Chain of Recurrences:" << std::endl;
        for (const auto& [reg_num, cr] : cr_map)
        {
            if (cr->length() == 0) continue;
            std::cout << "\treg_" << reg_num << " = ";
            cr->print();
            std::cout << " (length: " << cr->length() << ", cost: " << cr->getCostIndex()
                      << ", pure-sum: " << (cr->isPureSum() ? "yes" : "no")
                      << ", pure-product: " << (cr->isPureProduct() ? "yes" : "no")
                      << ", simple: " << (cr->isSimple() ? "yes" : "no") << ")" << std::endl;
        }

        std::cout << "Closed-form evaluations (first 5 iterations):" << std::endl;
        for (const auto& [reg, cr] : cr_map)
        {
            bool has_non_constant = false;
            for (const auto& operand : cr->operands)
            {
                if (!operand.getConstantValue())
                {
                    has_non_constant = true;
                    break;
                }
            }

            if (has_non_constant)
            {
                // std::cout << "0, 0, 0, 0, 0 (contains non-constant operand)" << std::endl;
            }
            else
            {
                std::cout << "\treg_" << reg << ": ";
                for (int i = 0; i < 5; ++i)
                {
                    auto result = cr->evaluateClosedForm(i);
                    auto value  = result.getConstantValue();
                    if (value) { std::cout << *value; }
                    else { std::cout << "?"; }
                    if (i < 4) std::cout << ", ";
                }
                std::cout << std::endl;
            }
        }

        if (is_simple_loop)
        {
            std::cout << "Loop Analysis:" << std::endl;
            std::cout << "\tLoop structure recognized" << std::endl;
            auto trip_count = getConstantTripCount();
            if (trip_count) { std::cout << "\tConstant trip count: " << *trip_count << std::endl; }
        }
        else
            std::cout << "\tLoop structure not recognized" << std::endl;

        std::cout << "======================================" << std::endl;
    }

    void SCEVAnalyser::run()
    {
        clear();

        for (const auto& [func_def, cfg] : ir_->cfg)
        {
            if (!cfg->LoopForest) continue;

            buildResultMap(cfg);

            for (auto* loop : cfg->LoopForest->loop_set) { analyzeLoop(cfg, loop); }
        }
    }

    void SCEVAnalyser::clear()
    {
        loop_analysis_.clear();
        result_map_.clear();
    }

    LoopCRInfo* SCEVAnalyser::getLoopInfo(NaturalLoop* loop) const
    {
        auto it = loop_analysis_.find(loop);
        return (it != loop_analysis_.end()) ? it->second.get() : nullptr;
    }

    ChainOfRecurrences* SCEVAnalyser::getCR(NaturalLoop* loop, int reg_num) const
    {
        auto* info = getLoopInfo(loop);
        return info ? info->getCR(reg_num) : nullptr;
    }

    bool SCEVAnalyser::isLoopInvariant(NaturalLoop* loop, int reg_num) const
    {
        auto* info = getLoopInfo(loop);
        return info ? info->isInvariant(reg_num) : false;
    }

    std::vector<NaturalLoop*> SCEVAnalyser::getAnalyzedLoops() const
    {
        std::vector<NaturalLoop*> loops;
        for (const auto& [loop, info] : loop_analysis_) { loops.push_back(loop); }
        return loops;
    }

    bool SCEVAnalyser::canFullyUnrollLoop(NaturalLoop* loop) const
    {
        auto* info = getLoopInfo(loop);
        return info ? info->canUnroll() : false;
    }

    std::optional<int> SCEVAnalyser::getLoopTripCount(NaturalLoop* loop) const
    {
        auto* info = getLoopInfo(loop);
        return info ? info->getConstantTripCount() : std::nullopt;
    }

    void SCEVAnalyser::printAllResults() const
    {
        std::cout << "\n=== SCEV Analysis Results ===" << std::endl;
        for (const auto& [loop, info] : loop_analysis_) { info->printInfo(); }
        std::cout << "============================" << std::endl;
    }

    void SCEVAnalyser::analyzeLoop(CFG* cfg, NaturalLoop* loop)
    {
        auto info = std::make_unique<LoopCRInfo>(loop);

        findInvariantVars(info.get(), cfg);
        findBasicInductionVars(info.get(), cfg);
        findCRRecurrences(info.get(), cfg);
        analyzeSimpleLoop(info.get(), cfg);

        loop_analysis_[loop] = std::move(info);
    }

    void SCEVAnalyser::buildResultMap(CFG* cfg)
    {
        result_map_.clear();

        for (const auto& [block_id, block] : cfg->block_id_to_block)
        {
            for (auto* inst : block->insts)
            {
                int result_reg = inst->GetResultReg();
                if (result_reg != -1) { result_map_[result_reg] = inst; }
            }
        }

        for (size_t i = 0; i < cfg->func->func_def->arg_regs.size(); ++i)
        {
            auto* arg_reg = cfg->func->func_def->arg_regs[i];
            if (arg_reg->type == LLVMIR::OperandType::REG)
            {
                auto* reg                 = static_cast<LLVMIR::RegOperand*>(arg_reg);
                result_map_[reg->reg_num] = cfg->func->func_def;
            }
        }
    }

    void SCEVAnalyser::findInvariantVars(LoopCRInfo* info, CFG* cfg)
    {
        for (const auto& [id, bb] : cfg->block_id_to_block)
        {
            if (info->loop->loop_nodes.find(bb) != info->loop->loop_nodes.end()) continue;

            for (auto* inst : bb->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::PHI &&
                    info->loop->exit_nodes.find(bb) != info->loop->exit_nodes.end())
                    continue;

                int result_reg = inst->GetResultReg();
                if (result_reg != -1) info->invariant_regs.insert(result_reg);
            }
        }

        for (size_t i = 0; i < cfg->func->func_def->arg_regs.size(); ++i)
        {
            auto* arg_reg = cfg->func->func_def->arg_regs[i];
            if (arg_reg->type == LLVMIR::OperandType::REG)
            {
                auto* reg = static_cast<LLVMIR::RegOperand*>(arg_reg);
                info->invariant_regs.insert(reg->reg_num);
            }
        }
    }

    void SCEVAnalyser::findBasicInductionVars(LoopCRInfo* info, CFG* cfg)
    {
        assert(info->loop->latches.size() == 1);
        auto* latch = *info->loop->latches.begin();

        for (auto* inst : info->loop->header->insts)
        {
            if (inst->opcode != LLVMIR::IROpCode::PHI) break;

            auto* phi_inst = static_cast<LLVMIR::PhiInst*>(inst);
            if (phi_inst->vals_for_labels.size() != 2) continue;
            if (phi_inst->type != LLVMIR::DataType::I32) continue;

            LLVMIR::Operand* start_val = nullptr;
            LLVMIR::Operand* loop_val  = nullptr;

            for (const auto& [val, label] : phi_inst->vals_for_labels)
            {
                auto* label_op = static_cast<LLVMIR::LabelOperand*>(label);
                if (label_op->label_num == info->loop->preheader->block_id) { start_val = val; }
                else if (label_op->label_num == latch->block_id) { loop_val = val; }
            }

            if (!start_val || !isI32Invariant(start_val, info->invariant_regs)) continue;
            if (!loop_val || loop_val->type != LLVMIR::OperandType::REG) continue;

            auto* loop_reg      = static_cast<LLVMIR::RegOperand*>(loop_val);
            auto [step, opcode] = findBasicIndVarCycleVarDef(info, phi_inst->GetResultReg(), loop_reg->reg_num);

            if (step)
            {
                if (opcode == LLVMIR::IROpCode::ADD)
                {
                    info->cr_map[phi_inst->GetResultReg()] =
                        CRBuilder::createFromBasicInductionVar(start_val, step, CROperator::ADD);
                }
                else if (opcode == LLVMIR::IROpCode::SUB)
                {
                    if (step->type == LLVMIR::OperandType::IMMEI32)
                    {
                        auto* imm_step = static_cast<LLVMIR::ImmeI32Operand*>(step);
                        auto* neg_step = getImmeI32Operand(-imm_step->value);
                        info->cr_map[phi_inst->GetResultReg()] =
                            CRBuilder::createFromBasicInductionVar(start_val, neg_step, CROperator::ADD);
                    }
                }
                else if (opcode == LLVMIR::IROpCode::MUL)
                {
                    info->cr_map[phi_inst->GetResultReg()] =
                        CRBuilder::createFromBasicInductionVar(start_val, step, CROperator::MUL);
                }
            }
        }

        for (int reg : info->invariant_regs)
        {
            if (info->cr_map.find(reg) == info->cr_map.end())
            {
                info->cr_map[reg] = std::make_unique<ChainOfRecurrences>(
                    std::vector<CROperand>{CROperand(getRegOperand(reg))}, std::vector<CROperator>{});
            }
        }
    }

    void SCEVAnalyser::findCRRecurrences(LoopCRInfo* info, CFG* cfg)
    {
        bool changed = true;
        while (changed)
        {
            changed = false;
            for (auto* bb : info->loop->loop_nodes)
            {
                for (auto* inst : bb->insts)
                {
                    int result_reg = inst->GetResultReg();
                    if (result_reg == -1) continue;
                    if (info->cr_map.find(result_reg) != info->cr_map.end()) continue;

                    if (inst->opcode == LLVMIR::IROpCode::ADD || inst->opcode == LLVMIR::IROpCode::MUL ||
                        inst->opcode == LLVMIR::IROpCode::SUB)
                    {
                        auto* arith_inst = static_cast<LLVMIR::ArithmeticInst*>(inst);
                        auto  result_cr  = CRBuilder::createFromArithmeticExpr(arith_inst, info->cr_map);

                        if (result_cr)
                        {
                            result_cr = CRBuilder::simplify(std::move(result_cr));
                            if (result_cr)
                            {
                                changed                  = true;
                                info->cr_map[result_reg] = std::move(result_cr);
                            }
                        }
                    }
                }
            }
        }
    }

    void SCEVAnalyser::analyzeSimpleLoop(LoopCRInfo* info, CFG* cfg)
    {
        info->is_simple_loop = false;

        if (info->loop->exit_nodes.size() != 1) return;
        if (info->loop->exiting_nodes.size() != 1) return;

        auto* exit_node    = *info->loop->exit_nodes.begin();
        auto* exiting_node = *info->loop->exiting_nodes.begin();
        auto* latch        = *info->loop->latches.begin();

        bool is_latch_eq_exiting = (exiting_node == latch);
        if (!is_latch_eq_exiting) return;
        if (exiting_node->insts.size() < 2) return;

        auto* cmp_inst = exiting_node->insts[exiting_node->insts.size() - 2];
        auto* br_inst  = exiting_node->insts[exiting_node->insts.size() - 1];

        if (cmp_inst->opcode == LLVMIR::IROpCode::FCMP) return;
        if (cmp_inst->opcode != LLVMIR::IROpCode::ICMP) return;
        assert(br_inst->opcode == LLVMIR::IROpCode::BR_COND);

        auto* icmp_inst    = static_cast<LLVMIR::IcmpInst*>(cmp_inst);
        auto* br_cond_inst = static_cast<LLVMIR::BranchCondInst*>(br_inst);

        auto* false_label = static_cast<LLVMIR::LabelOperand*>(br_cond_inst->false_label);
        auto* true_label  = static_cast<LLVMIR::LabelOperand*>(br_cond_inst->true_label);

        bool exit_on_true = (true_label->label_num == exit_node->block_id);

        auto  cond = icmp_inst->cond;
        auto* op1  = icmp_inst->lhs;
        auto* op2  = icmp_inst->rhs;

        auto getCR = [&](LLVMIR::Operand* operand) -> ChainOfRecurrences* {
            if (operand->type == LLVMIR::OperandType::REG)
            {
                auto* reg = static_cast<LLVMIR::RegOperand*>(operand);
                auto  it  = info->cr_map.find(reg->reg_num);
                if (it != info->cr_map.end()) { return it->second.get(); }
            }
            return nullptr;
        };

        auto* cr1 = getCR(op1);
        auto* cr2 = getCR(op2);

        if (!cr1 && !cr2) return;

        bool cr1_is_iv = (cr1 && cr1->length() > 0);
        bool cr2_is_iv = (cr2 && cr2->length() > 0);

        ChainOfRecurrences* selected_cr = nullptr;
        CROperand           upperbound_operand;
        LLVMIR::IcmpCond    normalized_cond = cond;

        if (cr1_is_iv && !cr2_is_iv)
        {
            selected_cr        = cr1;
            upperbound_operand = CROperand(op2);
        }
        else if (!cr1_is_iv && cr2_is_iv)
        {
            selected_cr        = cr2;
            upperbound_operand = CROperand(op1);
            normalized_cond    = getSwapIcmpCond(cond);
        }
        else if (cr1_is_iv && cr2_is_iv)
        {
            auto cr1_start = cr1->operands[0].getConstantValue();
            auto cr2_start = cr2->operands[0].getConstantValue();

            if (cr1_start && cr2_start)
            {
                if (*cr1_start <= *cr2_start)
                {
                    selected_cr        = cr1;
                    upperbound_operand = CROperand(op2);
                }
                else
                {
                    selected_cr        = cr2;
                    upperbound_operand = CROperand(op1);
                    normalized_cond    = getSwapIcmpCond(cond);
                }
            }
            else if (cr1_start && !cr2_start)
            {
                selected_cr        = cr1;
                upperbound_operand = CROperand(op2);
            }
            else if (!cr1_start && cr2_start)
            {
                selected_cr        = cr2;
                upperbound_operand = CROperand(op1);
                normalized_cond    = getSwapIcmpCond(cond);
            }
            else { return; }
        }
        else { return; }

        if (!selected_cr) return;

        CROperand true_lowerbound = selected_cr->operands[0];
        CROperand step_operand    = (selected_cr->operands.size() > 1) ? selected_cr->operands[1] : CROperand(1);

        // 对 do-while 形式循环，真正的下界为比较语句使用的归纳变量再前推一步的值
        // do-while的特征：latch块有条件跳转到header
        // 其它形式循环：latch块无条件跳转到header
        bool is_do_while_form = true;

        if (!latch->insts.empty())
        {
            auto* last_inst = latch->insts.back();

            if (last_inst->opcode == LLVMIR::IROpCode::BR_UNCOND)
            {
                auto* br_uncond    = static_cast<LLVMIR::BranchUncondInst*>(last_inst);
                auto* target_label = static_cast<LLVMIR::LabelOperand*>(br_uncond->target_label);
                if (target_label->label_num == info->loop->header->block_id) { is_do_while_form = false; }
            }
        }

        auto current_start = true_lowerbound.getConstantValue();
        auto step_val      = step_operand.getConstantValue();

        if (is_do_while_form && current_start && step_val)
        {
            if (selected_cr->isPureSum()) { true_lowerbound = CROperand(*current_start - *step_val); }
            else if (selected_cr->isPureProduct()) { true_lowerbound = CROperand(*current_start / *step_val); }
        }

        info->loop_info.lowerbound     = true_lowerbound;
        info->loop_info.upperbound     = upperbound_operand;
        info->loop_info.step           = step_operand;
        info->loop_info.cond           = exit_on_true ? getInverseIcmpCond(normalized_cond) : normalized_cond;
        info->loop_info.selected_iv_cr = selected_cr;
        info->is_simple_loop           = true;
        info->loop_info.is_upperbound_closed =
            (info->loop_info.cond == LLVMIR::IcmpCond::SLE || info->loop_info.cond == LLVMIR::IcmpCond::SGE);
    }

    bool SCEVAnalyser::isI32Invariant(LLVMIR::Operand* operand, const std::set<int>& invariant_set) const
    {
        if (operand->type == LLVMIR::OperandType::IMMEI32) return true;

        if (operand->type == LLVMIR::OperandType::REG)
        {
            auto* reg = static_cast<LLVMIR::RegOperand*>(operand);
            auto  it  = result_map_.find(reg->reg_num);
            if (it != result_map_.end())
            {
                return (invariant_set.find(reg->reg_num) != invariant_set.end()) &&
                       (it->second->GetResultType() == LLVMIR::DataType::I32);
            }
        }

        return false;
    }

    std::pair<LLVMIR::Operand*, LLVMIR::IROpCode> SCEVAnalyser::findBasicIndVarCycleVarDef(
        LoopCRInfo* info, int start_reg, int target_reg)
    {
        auto it = result_map_.find(target_reg);
        if (it == result_map_.end()) return {nullptr, LLVMIR::IROpCode::OTHER};

        auto* inst = it->second;

        if (inst->opcode != LLVMIR::IROpCode::ADD && inst->opcode != LLVMIR::IROpCode::SUB &&
            inst->opcode != LLVMIR::IROpCode::MUL)
            return {nullptr, LLVMIR::IROpCode::OTHER};

        auto* arith_inst = static_cast<LLVMIR::ArithmeticInst*>(inst);
        auto* op1        = arith_inst->lhs;
        auto* op2        = arith_inst->rhs;

        bool t1 = isI32Invariant(op1, info->invariant_regs);
        bool t2 = isI32Invariant(op2, info->invariant_regs);

        if ((t1 && t2) || (!t1 && !t2)) return {nullptr, LLVMIR::IROpCode::OTHER};

        if (t1) std::swap(op1, op2);

        assert(op1->type == LLVMIR::OperandType::REG);
        auto* reg_op = static_cast<LLVMIR::RegOperand*>(op1);
        if (reg_op->reg_num == start_reg) { return {op2, inst->opcode}; }

        return {nullptr, LLVMIR::IROpCode::OTHER};
    }

    LLVMIR::IcmpCond getInverseIcmpCond(LLVMIR::IcmpCond cond)
    {
        switch (cond)
        {
            case LLVMIR::IcmpCond::EQ: return LLVMIR::IcmpCond::NE;
            case LLVMIR::IcmpCond::NE: return LLVMIR::IcmpCond::EQ;
            case LLVMIR::IcmpCond::SLE: return LLVMIR::IcmpCond::SGT;
            case LLVMIR::IcmpCond::SGT: return LLVMIR::IcmpCond::SLE;
            case LLVMIR::IcmpCond::SLT: return LLVMIR::IcmpCond::SGE;
            case LLVMIR::IcmpCond::SGE: return LLVMIR::IcmpCond::SLT;
            default: return cond;
        }
    }

    LLVMIR::IcmpCond getSwapIcmpCond(LLVMIR::IcmpCond cond)
    {
        switch (cond)
        {
            case LLVMIR::IcmpCond::EQ: return LLVMIR::IcmpCond::EQ;
            case LLVMIR::IcmpCond::NE: return LLVMIR::IcmpCond::NE;
            case LLVMIR::IcmpCond::SLE: return LLVMIR::IcmpCond::SGE;
            case LLVMIR::IcmpCond::SGT: return LLVMIR::IcmpCond::SLT;
            case LLVMIR::IcmpCond::SLT: return LLVMIR::IcmpCond::SGT;
            case LLVMIR::IcmpCond::SGE: return LLVMIR::IcmpCond::SLE;
            default: return cond;
        }
    }

}  // namespace Analysis
