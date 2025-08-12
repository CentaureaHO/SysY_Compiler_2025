#include "gep_strength_reduce.h"
#include <iostream>
#include <algorithm>
#include <cmath>

namespace Transform
{
    GEPStrengthReduce::GEPStrengthReduce(LLVMIR::IR* ir) : Pass(ir) {}

    void GEPStrengthReduce::Execute()
    {
        cur_cfg = nullptr;

        for (auto* func : ir->functions) runOnFunction(func);
    }

    void GEPStrengthReduce::runOnFunction(LLVMIR::IRFunction* func)
    {
        cur_func = func;

        cur_cfg = nullptr;
        for (const auto& [func_def, cfg_ptr] : ir->cfg)
        {
            if (func_def == func->func_def)
            {
                cur_cfg = cfg_ptr;
                break;
            }
        }

        if (!cur_cfg) return;

        auto dom_tree_it = ir->DomTrees.find(cur_cfg);
        if (dom_tree_it == ir->DomTrees.end()) return;

        auto* dom_analyzer = dom_tree_it->second;
        if (!dom_analyzer) return;

        std::vector<GEPInfo>     available_geps;
        std::map<int, ArithInfo> arithmetic_defs;
        std::set<int>            visited;

        processDominatedBlock(0, available_geps, arithmetic_defs, visited);
    }

    void GEPStrengthReduce::processDominatedBlock(int block_id, std::vector<GEPInfo>& available_geps,
        std::map<int, ArithInfo>& arithmetic_defs, std::set<int>& visited)
    {
        if (visited.find(block_id) != visited.end()) return;
        visited.insert(block_id);

        auto block_it = cur_cfg->block_id_to_block.find(block_id);
        if (block_it == cur_cfg->block_id_to_block.end()) return;

        LLVMIR::IRBlock* block = block_it->second;

        size_t                   original_gep_count  = available_geps.size();
        std::map<int, ArithInfo> original_arith_defs = arithmetic_defs;
        std::vector<GEPInfo>     current_geps;
        collectArithmeticDefs(block, arithmetic_defs);
        collectGEPInstructions(block, current_geps);

        for (auto& target_gep : current_geps)
        {
            bool optimized = false;

            for (auto it = available_geps.rbegin(); it != available_geps.rend() && !optimized; ++it)
            {
                const auto& base_gep = *it;
                int         offset   = 0;

                if (canReduceGEP(base_gep, target_gep, arithmetic_defs, offset))
                {
                    if (isValidImmediateOffset(offset))
                    {
                        optimizeGEP(target_gep.gep_inst, base_gep.gep_inst, offset);
                        optimized = true;
                    }
                }
            }

            available_geps.push_back(target_gep);
        }
        auto dom_tree_it = ir->DomTrees.find(cur_cfg);
        if (dom_tree_it != ir->DomTrees.end())
        {
            auto* dom_analyzer = dom_tree_it->second;

            if (static_cast<size_t>(block_id) < dom_analyzer->dom_tree.size())
            {
                for (int dominated_block : dom_analyzer->dom_tree[block_id])
                {
                    processDominatedBlock(dominated_block, available_geps, arithmetic_defs, visited);
                }
            }
        }

        available_geps.resize(original_gep_count);
        arithmetic_defs = original_arith_defs;
        visited.erase(block_id);
    }

    void GEPStrengthReduce::collectArithmeticDefs(LLVMIR::IRBlock* block, std::map<int, ArithInfo>& arith_defs)
    {
        for (auto* inst : block->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::ADD || inst->opcode == LLVMIR::IROpCode::SUB)
            {
                auto* arith_inst = dynamic_cast<LLVMIR::ArithmeticInst*>(inst);
                if (arith_inst && arith_inst->type == LLVMIR::DataType::I32)
                {
                    int result_reg = arith_inst->GetResultReg();
                    if (result_reg != -1) { arith_defs[result_reg] = ArithInfo(arith_inst); }
                }
            }
        }
    }

    void GEPStrengthReduce::collectGEPInstructions(LLVMIR::IRBlock* block, std::vector<GEPInfo>& gep_infos)
    {
        for (auto* inst : block->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
            {
                auto* gep_inst = dynamic_cast<LLVMIR::GEPInst*>(inst);
                if (gep_inst) { gep_infos.emplace_back(gep_inst); }
            }
        }
    }

    bool GEPStrengthReduce::canReduceGEP(const GEPInfo& base_gep, const GEPInfo& target_gep,
        const std::map<int, ArithInfo>& arith_defs, int& offset) const
    {
        // 必须有相同的基指针
        if (!operandsEqual(base_gep.base_ptr, target_gep.base_ptr)) return false;
        // 必须有相同的维度信息
        if (base_gep.dims != target_gep.dims) return false;
        // 索引数量必须相同
        if (base_gep.indices.size() != target_gep.indices.size()) return false;

        assert(base_gep.gep_inst->idx_type == target_gep.gep_inst->idx_type);

        return calculateOffset(base_gep, target_gep, arith_defs, offset);
    }

    bool GEPStrengthReduce::calculateOffset(const GEPInfo& base_gep, const GEPInfo& target_gep,
        const std::map<int, ArithInfo>& arith_defs, int& offset) const
    {
        offset = 0;

        if (base_gep.indices.empty()) return false;

        std::vector<int> element_sizes;
        if (!base_gep.dims.empty())
        {
            int current_size = 1;
            element_sizes.push_back(current_size);

            for (int i = base_gep.dims.size() - 1; i >= 0; --i)
            {
                current_size *= base_gep.dims[i];
                element_sizes.push_back(current_size);
            }
            std::reverse(element_sizes.begin(), element_sizes.end());
        }
        else
            element_sizes.assign(base_gep.indices.size(), 1);

        for (size_t i = 0; i < base_gep.indices.size(); ++i)
        {
            LLVMIR::Operand* base_idx   = base_gep.indices[i];
            LLVMIR::Operand* target_idx = target_gep.indices[i];

            if (operandsEqual(base_idx, target_idx)) continue;

            int  index_diff = 0;
            bool found_diff = false;

            int base_val, target_val;
            if (isImmediateOperand(base_idx, base_val) && isImmediateOperand(target_idx, target_val))
            {
                index_diff = target_val - base_val;
                found_diff = true;
            }
            else
            {
                // target -> base + constant || base - constant
                LLVMIR::Operand* arith_base;
                int              constant_offset;
                if (isRegisterDefinedByArith(target_idx, arith_defs, arith_base, constant_offset))
                {
                    if (operandsEqual(arith_base, base_idx))
                    {
                        index_diff = constant_offset;
                        found_diff = true;
                    }
                }

                // base -> target + constant || target - constant
                if (!found_diff && isRegisterDefinedByArith(base_idx, arith_defs, arith_base, constant_offset))
                {
                    if (operandsEqual(arith_base, target_idx))
                    {
                        index_diff = -constant_offset;
                        found_diff = true;
                    }
                }
            }

            if (!found_diff) return false;

            if (i < element_sizes.size())
                offset += index_diff * element_sizes[i];
            else
                offset += index_diff;
        }

        return true;
    }

    bool GEPStrengthReduce::operandsEqual(LLVMIR::Operand* op1, LLVMIR::Operand* op2) const
    {
        if (!op1 || !op2) return op1 == op2;

        if (op1->type != op2->type) return false;

        switch (op1->type)
        {
            case LLVMIR::OperandType::REG:
            {
                auto* reg1 = dynamic_cast<LLVMIR::RegOperand*>(op1);
                auto* reg2 = dynamic_cast<LLVMIR::RegOperand*>(op2);
                return reg1 && reg2 && reg1->reg_num == reg2->reg_num;
            }
            case LLVMIR::OperandType::IMMEI32:
            {
                auto* imm1 = dynamic_cast<LLVMIR::ImmeI32Operand*>(op1);
                auto* imm2 = dynamic_cast<LLVMIR::ImmeI32Operand*>(op2);
                return imm1 && imm2 && imm1->value == imm2->value;
            }
            case LLVMIR::OperandType::GLOBAL:
            {
                auto* glob1 = dynamic_cast<LLVMIR::GlobalOperand*>(op1);
                auto* glob2 = dynamic_cast<LLVMIR::GlobalOperand*>(op2);
                return glob1 && glob2 && glob1->global_name == glob2->global_name;
            }
            default: return false;
        }
    }

    bool GEPStrengthReduce::isImmediateOperand(LLVMIR::Operand* op, int& value) const
    {
        if (!op || op->type != LLVMIR::OperandType::IMMEI32) return false;

        auto* imm_op = dynamic_cast<LLVMIR::ImmeI32Operand*>(op);
        if (!imm_op) return false;

        value = imm_op->value;
        return true;
    }

    bool GEPStrengthReduce::isRegisterDefinedByArith(LLVMIR::Operand* op, const std::map<int, ArithInfo>& arith_defs,
        LLVMIR::Operand*& base_operand, int& constant_offset) const
    {
        if (!op || op->type != LLVMIR::OperandType::REG) return false;

        auto* reg_op = dynamic_cast<LLVMIR::RegOperand*>(op);
        if (!reg_op) return false;

        auto arith_it = arith_defs.find(reg_op->reg_num);
        if (arith_it == arith_defs.end()) return false;

        const ArithInfo& arith = arith_it->second;

        if (arith.opcode == LLVMIR::IROpCode::ADD)
        {
            int imm_val;
            if (arith.operand1->type == LLVMIR::OperandType::REG && isImmediateOperand(arith.operand2, imm_val))
            {
                base_operand    = arith.operand1;
                constant_offset = imm_val;
                return true;
            }
            if (arith.operand2->type == LLVMIR::OperandType::REG && isImmediateOperand(arith.operand1, imm_val))
            {
                base_operand    = arith.operand2;
                constant_offset = imm_val;
                return true;
            }
        }
        else if (arith.opcode == LLVMIR::IROpCode::SUB)
        {
            int imm_val;
            if (arith.operand1->type == LLVMIR::OperandType::REG && isImmediateOperand(arith.operand2, imm_val))
            {
                base_operand    = arith.operand1;
                constant_offset = -imm_val;
                return true;
            }
        }

        return false;
    }

    void GEPStrengthReduce::optimizeGEP(LLVMIR::GEPInst* target_gep, LLVMIR::GEPInst* base_gep, int offset)
    {
        if (offset == 0) return;

        target_gep->base_ptr = base_gep->res;

        target_gep->idxs.clear();
        target_gep->idxs.push_back(getImmeI32Operand(offset));

        target_gep->type     = base_gep->type;
        target_gep->idx_type = base_gep->idx_type;

        target_gep->dims.clear();
    }

    int GEPStrengthReduce::calculateDimensionSize(const std::vector<int>& dims, int dim_index) const
    {
        if (dim_index >= static_cast<int>(dims.size())) return 1;

        int size = 1;
        for (int i = dim_index + 1; i < static_cast<int>(dims.size()); ++i) { size *= dims[i]; }
        return size;
    }

}  // namespace Transform
