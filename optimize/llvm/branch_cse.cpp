#include "llvm/branch_cse.h"
#include "llvm_ir/instruction.h"
#include <queue>
#include <algorithm>

namespace StructuralTransform
{
    BranchCSEPass::BranchCSEPass(LLVMIR::IR* ir) : Pass(ir) {}

    CmpInstCSEInfo BranchCSEPass::getCSEInfo(LLVMIR::Instruction* inst)
    {
        CmpInstCSEInfo ans;
        ans.opcode = inst->opcode;

        std::vector<LLVMIR::Operand*> operands;

        if (inst->opcode == LLVMIR::IROpCode::ICMP)
        {
            auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(inst);
            ans.cond       = static_cast<int>(icmp_inst->cond);
            operands.push_back(icmp_inst->lhs);
            operands.push_back(icmp_inst->rhs);
        }
        else if (inst->opcode == LLVMIR::IROpCode::FCMP)
        {
            auto fcmp_inst = dynamic_cast<LLVMIR::FcmpInst*>(inst);
            ans.cond       = static_cast<int>(fcmp_inst->cond);
            operands.push_back(fcmp_inst->lhs);
            operands.push_back(fcmp_inst->rhs);
        }

        if (inst->opcode == LLVMIR::IROpCode::ADD || inst->opcode == LLVMIR::IROpCode::MUL)
        {
            if (operands.size() == 2)
            {
                auto op1 = operands[0], op2 = operands[1];
                if (op1->getName() > op2->getName()) std::swap(op1, op2);
                ans.operand_list.push_back(op1->getName());
                ans.operand_list.push_back(op2->getName());
            }
        }
        else
        {
            for (auto op : operands) ans.operand_list.push_back(op->getName());
        }

        return ans;
    }

    bool BranchCSEPass::canReach(int bb1_id, int bb2_id, CFG* cfg)
    {
        std::vector<bool> vis(cfg->block_id_to_block.size(), false);
        std::queue<int>   q;
        q.push(bb1_id);

        while (!q.empty())
        {
            auto x = q.front();
            q.pop();

            if (x == bb2_id) return true;
            if (vis[x]) continue;

            vis[x] = true;

            if ((unsigned)x < cfg->G.size())
            {
                for (auto bb : cfg->G[x]) q.push(bb->block_id);
            }
        }
        return false;
    }

    bool BranchCSEPass::canJump(bool is_left, int x1_id, int x2_id, CFG* cfg)
    {
        if (!ir->DomTrees.count(cfg)) return false;

        auto&                         dom_tree      = ir->DomTrees.at(cfg)->dom_tree;
        std::function<bool(int, int)> checkDominate = [&](int dominator, int node) -> bool {
            if (dominator == node) return true;
            if ((unsigned)dominator >= dom_tree.size()) return false;

            for (int child : dom_tree[dominator])
            {
                if (checkDominate(child, node)) return true;
            }
            return false;
        };

        bool dominates = checkDominate(x1_id, x2_id);
        if (!dominates) return false;

        auto x1 = cfg->block_id_to_block.at(x1_id);
        auto x2 = cfg->block_id_to_block.at(x2_id);

        if (x1->insts.empty()) return false;

        auto last_inst = x1->insts.back();
        if (last_inst->opcode != LLVMIR::IROpCode::BR_COND) return false;

        auto br_inst1    = dynamic_cast<LLVMIR::BranchCondInst*>(last_inst);
        auto true_label  = dynamic_cast<LLVMIR::LabelOperand*>(br_inst1->true_label);
        auto false_label = dynamic_cast<LLVMIR::LabelOperand*>(br_inst1->false_label);

        if (!true_label || !false_label) return false;

        int x_t_id = true_label->label_num;
        int x_f_id = false_label->label_num;

        if (is_left)
        {
            if (!canReach(x_t_id, x2_id, cfg) && canReach(x_f_id, x2_id, cfg))
            {
                // Replace with constant true comparison
                // Then, may adjust to jump to x2's true label without condition
                if (x2->insts.size() >= 2)
                {
                    auto tmp_inst1 = x2->insts.back();
                    auto tmp_inst2 = *(x2->insts.end() - 2);
                    x2->insts.pop_back();
                    x2->insts.pop_back();

                    auto result_reg = getRegOperand(tmp_inst2->GetResultReg());
                    auto new_icmp   = new LLVMIR::IcmpInst(LLVMIR::DataType::I32,
                        LLVMIR::IcmpCond::EQ,
                        getImmeI32Operand(0),
                        getImmeI32Operand(1),
                        result_reg);
                    x2->insts.push_back(new_icmp);
                    x2->insts.push_back(tmp_inst1);
                    return true;
                }
            }
        }
        else
        {
            if (canReach(x_t_id, x2_id, cfg) && !canReach(x_f_id, x2_id, cfg))
            {
                // Replace with constant false comparison
                // Then, may adjust to jump to x2's false label without condition
                if (x2->insts.size() >= 2)
                {
                    auto tmp_inst1 = x2->insts.back();
                    auto tmp_inst2 = *(x2->insts.end() - 2);
                    x2->insts.pop_back();
                    x2->insts.pop_back();

                    auto result_reg = getRegOperand(tmp_inst2->GetResultReg());
                    auto new_icmp   = new LLVMIR::IcmpInst(LLVMIR::DataType::I32,
                        LLVMIR::IcmpCond::EQ,
                        getImmeI32Operand(1),
                        getImmeI32Operand(1),
                        result_reg);
                    x2->insts.push_back(new_icmp);
                    x2->insts.push_back(tmp_inst1);
                    return true;
                }
            }
        }
        return false;
    }

    bool BranchCSEPass::blockDefNoUseCheck(CFG* cfg, int bb_id, int st_id)
    {
        auto          bb = cfg->block_id_to_block.at(bb_id);
        std::set<int> defs;

        for (auto inst : bb->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI) return false;

            int result_reg = inst->GetResultReg();
            if (result_reg != -1) defs.insert(result_reg);
        }

        for (auto inst : bb->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::STORE || inst->opcode == LLVMIR::IROpCode::CALL) return false;
        }

        auto bb2 = cfg->block_id_to_block.at(st_id);
        for (auto inst : bb2->insts)
        {
            if (inst->opcode == LLVMIR::IROpCode::PHI) return false;
        }

        std::vector<bool> vis(cfg->block_id_to_block.size(), false);
        std::queue<int>   q;
        q.push(st_id);

        while (!q.empty())
        {
            auto x = q.front();
            q.pop();

            if (vis[x]) continue;
            vis[x] = true;

            if (x == bb_id) continue;

            auto bb_x = cfg->block_id_to_block.at(x);
            for (auto inst : bb_x->insts)
            {
                auto used_regs = inst->GetUsedRegs();
                for (auto reg : used_regs)
                {
                    if (defs.count(reg)) return false;
                }
            }

            if ((unsigned)x < cfg->G.size())
            {
                for (auto bb : cfg->G[x]) q.push(bb->block_id);
            }
        }
        return true;
    }

    bool BranchCSEPass::emptyBlockJumping(CFG* cfg)
    {
        bool flag = false;

        for (auto const& [id, bb] : cfg->block_id_to_block)
        {
            if (bb->insts.size() < 2) continue;

            auto inst = *(bb->insts.end() - 2);
            if (inst->opcode != LLVMIR::IROpCode::ICMP && inst->opcode != LLVMIR::IROpCode::FCMP) continue;

            LLVMIR::Operand* op1_1 = nullptr;
            LLVMIR::Operand* op1_2 = nullptr;
            int              cond1 = 0;

            if (inst->opcode == LLVMIR::IROpCode::ICMP)
            {
                auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(inst);
                op1_1          = icmp_inst->lhs;
                op1_2          = icmp_inst->rhs;
                cond1          = static_cast<int>(icmp_inst->cond);
            }
            else if (inst->opcode == LLVMIR::IROpCode::FCMP)
            {
                auto fcmp_inst = dynamic_cast<LLVMIR::FcmpInst*>(inst);
                op1_1          = fcmp_inst->lhs;
                op1_2          = fcmp_inst->rhs;
                cond1          = static_cast<int>(fcmp_inst->cond);
            }

            if ((unsigned)id >= cfg->G.size()) continue;

            for (auto bb2 : cfg->G[id])
            {
                bool dominates = false;
                if (ir->DomTrees.count(cfg))
                {
                    auto&                         dom_tree      = ir->DomTrees.at(cfg)->dom_tree;
                    std::function<bool(int, int)> checkDominate = [&](int dominator, int node) -> bool {
                        if (dominator == node) return true;
                        if ((unsigned)dominator >= dom_tree.size()) return false;

                        for (int child : dom_tree[dominator])
                        {
                            if (checkDominate(child, node)) return true;
                        }
                        return false;
                    };
                    dominates = checkDominate(bb->block_id, bb2->block_id);
                }

                if (!dominates) continue;
                if (bb2 == bb || bb2->insts.size() < 2) continue;

                auto inst2 = *(bb2->insts.end() - 2);
                if (inst->opcode != inst2->opcode) continue;

                LLVMIR::Operand* op2_1 = nullptr;
                LLVMIR::Operand* op2_2 = nullptr;
                int              cond2 = 0;

                if (inst2->opcode == LLVMIR::IROpCode::ICMP)
                {
                    auto icmp_inst2 = dynamic_cast<LLVMIR::IcmpInst*>(inst2);
                    op2_1           = icmp_inst2->lhs;
                    op2_2           = icmp_inst2->rhs;
                    cond2           = static_cast<int>(icmp_inst2->cond);
                }
                else if (inst2->opcode == LLVMIR::IROpCode::FCMP)
                {
                    auto fcmp_inst2 = dynamic_cast<LLVMIR::FcmpInst*>(inst2);
                    op2_1           = fcmp_inst2->lhs;
                    op2_2           = fcmp_inst2->rhs;
                    cond2           = static_cast<int>(fcmp_inst2->cond);
                }

                if (op1_1->getName() != op2_1->getName() || op1_2->getName() != op2_2->getName() || cond1 != cond2)
                    continue;

                auto br_inst1 = dynamic_cast<LLVMIR::BranchCondInst*>(bb->insts.back());
                auto br_inst2 = dynamic_cast<LLVMIR::BranchCondInst*>(bb2->insts.back());

                if (!br_inst1 || !br_inst2) continue;

                auto true_label1  = dynamic_cast<LLVMIR::LabelOperand*>(br_inst1->true_label);
                auto true_label2  = dynamic_cast<LLVMIR::LabelOperand*>(br_inst2->true_label);
                auto false_label2 = dynamic_cast<LLVMIR::LabelOperand*>(br_inst2->false_label);

                if (!true_label1 || !true_label2 || !false_label2) continue;

                if (true_label1->label_num == bb2->block_id)
                {
                    if (!blockDefNoUseCheck(cfg, bb2->block_id, true_label2->label_num)) continue;

                    br_inst1->true_label = true_label2;
                    bb2->insts.pop_back();
                    bb2->insts.pop_back();

                    auto result_reg = getRegOperand(inst2->GetResultReg());
                    auto new_icmp   = new LLVMIR::IcmpInst(LLVMIR::DataType::I32,
                        LLVMIR::IcmpCond::EQ,
                        getImmeI32Operand(1),
                        getImmeI32Operand(0),
                        result_reg);
                    bb2->insts.push_back(new_icmp);
                    bb2->insts.push_back(br_inst2);
                    flag = true;
                }
                else
                {
                    if (!blockDefNoUseCheck(cfg, bb2->block_id, false_label2->label_num)) continue;

                    br_inst1->false_label = false_label2;
                    bb2->insts.pop_back();
                    bb2->insts.pop_back();

                    auto result_reg2 = getRegOperand(inst2->GetResultReg());
                    auto new_icmp    = new LLVMIR::IcmpInst(LLVMIR::DataType::I32,
                        LLVMIR::IcmpCond::EQ,
                        getImmeI32Operand(1),
                        getImmeI32Operand(1),
                        result_reg2);
                    bb2->insts.push_back(new_icmp);
                    bb2->insts.push_back(br_inst2);
                    flag = true;
                }
            }
        }
        return flag;
    }

    void BranchCSEPass::dfs(int now_id, CFG* cfg, std::map<CmpInstCSEInfo, std::vector<LLVMIR::Instruction*>>& cmp_map)
    {
        auto                     now = cfg->block_id_to_block.at(now_id);
        std::set<CmpInstCSEInfo> tmp_cse_set;

        if (now->insts.size() >= 2)
        {
            auto inst = *(now->insts.end() - 2);
            if (inst->opcode == LLVMIR::IROpCode::FCMP || inst->opcode == LLVMIR::IROpCode::ICMP)
            {
                auto info         = getCSEInfo(inst);
                bool is_const_cmp = false;

                if (inst->opcode == LLVMIR::IROpCode::ICMP)
                {
                    auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(inst);
                    if (icmp_inst->lhs->type == LLVMIR::OperandType::IMMEI32 &&
                        icmp_inst->rhs->type == LLVMIR::OperandType::IMMEI32)
                        is_const_cmp = true;
                }

                bool is_cse = false;
                if (!is_const_cmp && cmp_map.count(info))
                {
                    for (auto inst2 : cmp_map[info])
                    {
                        inst2->block_id = inst2->block_id;
                        if (canJump(true, inst2->block_id, inst->block_id, cfg) ||
                            canJump(false, inst2->block_id, inst->block_id, cfg))
                        {
                            is_cse = true;
                            break;
                        }
                    }
                }

                if (!is_cse && !is_const_cmp)
                {
                    tmp_cse_set.insert(info);
                    cmp_map[info].push_back(inst);
                }
            }
        }

        if (ir->DomTrees.count(cfg))
        {
            auto& dom_tree = ir->DomTrees.at(cfg)->dom_tree;
            if ((unsigned)now_id < dom_tree.size())
            {
                for (auto child_id : dom_tree[now_id])
                {
                    if (child_id != now_id) dfs(child_id, cfg, cmp_map);
                }
            }
        }

        for (auto info : tmp_cse_set)
        {
            cmp_map[info].pop_back();
            if (cmp_map[info].empty()) cmp_map.erase(info);
        }
    }

    bool BranchCSEPass::compareInstructionCSE(CFG* cfg)
    {
        bool                                           changed = false;
        std::map<CmpInstCSEInfo, LLVMIR::Instruction*> cmp_map;
        std::set<LLVMIR::Instruction*>                 erase_set;
        std::map<int, int>                             reg_replace_map;

        if (!ir->DomTrees.count(cfg)) return false;

        auto&                         dom_tree      = ir->DomTrees.at(cfg)->dom_tree;
        std::function<bool(int, int)> checkDominate = [&](int dominator, int node) -> bool {
            if (dominator == node) return true;
            if ((unsigned)dominator >= dom_tree.size()) return false;

            for (int child : dom_tree[dominator])
            {
                if (checkDominate(child, node)) return true;
            }
            return false;
        };

        for (auto const& [id, bb] : cfg->block_id_to_block)
        {
            for (auto inst : bb->insts)
            {
                if (inst->opcode != LLVMIR::IROpCode::ICMP && inst->opcode != LLVMIR::IROpCode::FCMP) continue;

                auto info = getCSEInfo(inst);

                // Check if this is a constant comparison (skip CSE for these)
                bool is_const_cmp = false;
                if (inst->opcode == LLVMIR::IROpCode::ICMP)
                {
                    auto icmp_inst = dynamic_cast<LLVMIR::IcmpInst*>(inst);
                    if (icmp_inst->lhs->type == LLVMIR::OperandType::IMMEI32 &&
                        icmp_inst->rhs->type == LLVMIR::OperandType::IMMEI32)
                        is_const_cmp = true;
                }

                if (!is_const_cmp && cmp_map.count(info))
                {
                    auto* prev_inst = cmp_map[info];
                    if (checkDominate(prev_inst->block_id, inst->block_id))
                    {
                        erase_set.insert(inst);
                        reg_replace_map[inst->GetResultReg()] = prev_inst->GetResultReg();
                        changed                               = true;
                    }
                    else if (checkDominate(inst->block_id, prev_inst->block_id)) { cmp_map[info] = inst; }
                }
                else if (!is_const_cmp) { cmp_map[info] = inst; }
            }
        }

        // Apply the changes
        if (changed)
        {
            // Remove redundant instructions
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                bb->insts.erase(
                    std::remove_if(
                        bb->insts.begin(), bb->insts.end(), [&](LLVMIR::Instruction* i) { return erase_set.count(i); }),
                    bb->insts.end());
            }

            // Rename registers
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                for (auto inst : bb->insts) inst->Rename(reg_replace_map);
            }
        }

        return changed;
    }

    void BranchCSEPass::Execute()
    {
        for (auto const& [func_def, cfg] : ir->cfg)
        {
            for (auto const& [id, bb] : cfg->block_id_to_block)
            {
                for (auto inst : bb->insts) inst->block_id = id;
            }

            std::map<CmpInstCSEInfo, std::vector<LLVMIR::Instruction*>> cmp_map;
            bool                                                        changed = true;

            while (changed)
            {
                cmp_map.clear();
                changed = false;

                // First do basic comparison instruction CSE
                changed |= compareInstructionCSE(cfg);

                // Then do branch-specific optimizations
                dfs(0, cfg, cmp_map);
                changed |= emptyBlockJumping(cfg);
            }
        }
    }

}  // namespace StructuralTransform
