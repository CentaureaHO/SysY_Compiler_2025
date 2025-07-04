#include <backend/rv64/passes/optimize/control_flow/phi_destruction.h>
#include <backend/rv64/insts.h>
#include <queue>
#include <set>

using namespace std;

namespace Backend::RV64::Passes::Optimize::ControlFlow
{

    PhiDestructionPass::PhiDestructionPass(std::vector<Function*>& functions) : functions_(functions) {}

    bool PhiDestructionPass::run()
    {
        for (auto& func : functions_) { phiInstDestory(func); }
        return true;
    }

    void PhiDestructionPass::phiInstDestory(Function* func)
    {
        for (auto& [id, block] : func->cfg->blocks)
        {
            bool has_phi = false;
            for (auto inst : block->insts)
            {
                if (inst->inst_type == InstType::PHI)
                {
                    has_phi = true;
                    // assert(false);
                    break;
                }
            }
            if (!has_phi) continue;

            for (auto& predecessor : func->cfg->inv_graph[id])
            {
                if (func->cfg->graph[predecessor->label_num].size() <= 1) continue;

                int from = predecessor->label_num;
                int to   = id;

                Block* split_block = func->getNewBlock();
                int    mid_label   = split_block->label_num;

                func->cfg->removeEdge(from, to);
                func->cfg->makeEdge(from, mid_label);
                func->cfg->makeEdge(mid_label, to);

                // redirect branch inst
                redirectBranchInst(func, from, to, mid_label);
                split_block->insts.push_back(createJInst(RV64InstType::JAL, preg_x0, RV64Label(to)));

                // redirect phi inst
                for (auto& phi_inst : block->insts)
                {
                    if (phi_inst->inst_type != InstType::PHI) continue;

                    PhiInst*               phi = (PhiInst*)phi_inst;
                    vector<pair<int, int>> changedKeys;

                    for (auto& kv : phi->phi_list)
                        if (kv.first == from) changedKeys.push_back({kv.first, mid_label});

                    for (auto& ck : changedKeys)
                    {
                        int oldKey = ck.first;
                        int newKey = ck.second;

                        auto it = phi->phi_list.find(oldKey);
                        if (it == phi->phi_list.end()) continue;

                        Operand* val = it->second;
                        phi->phi_list.erase(it);
                        phi->phi_list.insert({newKey, val});
                    }
                }
            }

            for (auto it = block->insts.begin(); it != block->insts.end();)
            {
                Instruction* base_inst = *it;
                if (base_inst->inst_type != InstType::PHI)
                {
                    ++it;
                    continue;
                }

                it           = block->insts.erase(it);
                PhiInst* phi = (PhiInst*)base_inst;

                for (auto& [id, val] : phi->phi_list)
                {
                    if (val->operand_type == OperandType::REG)
                        if (((RegOperand*)val)->reg == phi->res_reg) continue;

                    Block* from_block                      = func->cfg->blocks[id];
                    from_block->phi_copy_map[phi->res_reg] = val;
                }

                phi->phi_list.clear();
                delete phi;
            }
        }

        for (auto& [id, block] : func->cfg->blocks)
        {
            map<Register, Operand*>& copy_map = block->phi_copy_map;
            if (copy_map.empty()) continue;

            map<Register, int> src_ref_count;
            for (auto& [dst, src] : copy_map) src_ref_count[dst] = 0;
            for (auto& [dst, src] : copy_map)
            {
                // assert(src != nullptr);
                if (src->operand_type != OperandType::REG) continue;
                assert((((RegOperand*)src)->reg).is_virtual);
                ++src_ref_count[((RegOperand*)src)->reg];
            }

            queue<Register> can_seq;
            for (auto& [reg, ref] : src_ref_count)
                if (ref == 0) can_seq.push(reg);

            auto insert_it = block->insts.end();
            if (!block->insts.empty())
            {
                --insert_it;
                auto jal_pos = insert_it;

                for (auto it = insert_it; it != --block->insts.begin(); --it)
                {
                    Instruction* base_inst = *it;
                    if (base_inst->inst_type == InstType::PHI) continue;
                    if (base_inst->inst_type != InstType::RV64)
                    {
                        insert_it = jal_pos;
                        break;
                    }

                    RV64Inst* rv64_inst = (RV64Inst*)base_inst;
                    if (rv64_inst->op == RV64InstType::JALR)
                    {
                        insert_it = it;
                        break;
                    }
                    if (rv64_inst->op == RV64InstType::JAL)
                    {
                        jal_pos = it;
                        continue;
                    }
                    if (opInfoTable[rv64_inst->op].type == RV64OpType::B)
                    {
                        insert_it = it;
                        break;
                    }
                    else
                    {
                        insert_it = jal_pos;
                        break;
                    }
                }
            }

            while (!copy_map.empty())
            {
                if (!can_seq.empty())
                {
                    Register dest = can_seq.front();
                    can_seq.pop();

                    if (copy_map.find(dest) == copy_map.end()) continue;
                    Operand* src_op = copy_map[dest];
                    copy_map.erase(dest);

                    if (src_op->operand_type == OperandType::REG)
                    {
                        Register src_reg = ((RegOperand*)src_op)->reg;
                        --src_ref_count[src_reg];
                        if (src_ref_count[src_reg] == 0) can_seq.push(src_reg);
                    }

                    if (src_op->operand_type == OperandType::REG)
                    {
                        Register src_reg = ((RegOperand*)src_op)->reg;
                        block->insts.insert(insert_it, createMoveInst(dest.data_type, dest, src_reg));
                    }
                    else if (src_op->operand_type == OperandType::IMMEI32)
                    {
                        int imme_val = ((ImmeI32Operand*)src_op)->val;
                        block->insts.insert(insert_it, createMoveInst(dest.data_type, dest, imme_val));
                    }
                    else if (src_op->operand_type == OperandType::IMMEF32)
                    {
                        float imme_val = ((ImmeF32Operand*)src_op)->val;
                        block->insts.insert(insert_it, createMoveInst(dest.data_type, dest, imme_val));
                    }
                    else { block->insts.insert(insert_it, new MoveInst(dest.data_type, src_op, new RegOperand(dest))); }
                    continue;
                }

                auto& [dest, src] = *copy_map.begin();
                Register mid_reg  = func->getNewReg(dest.data_type);

                if (src->operand_type == OperandType::REG)
                {
                    Register src_reg = ((RegOperand*)src)->reg;
                    block->insts.insert(insert_it, createMoveInst(dest.data_type, mid_reg, src_reg));
                }
                else if (src->operand_type == OperandType::IMMEI32)
                {
                    int imme_val = ((ImmeI32Operand*)src)->val;
                    block->insts.insert(insert_it, createMoveInst(dest.data_type, mid_reg, imme_val));
                }
                else if (src->operand_type == OperandType::IMMEF32)
                {
                    float imme_val = ((ImmeF32Operand*)src)->val;
                    block->insts.insert(insert_it, createMoveInst(dest.data_type, mid_reg, imme_val));
                }
                else { block->insts.insert(insert_it, new MoveInst(dest.data_type, src, new RegOperand(mid_reg))); }

                if (src->operand_type == OperandType::REG)
                {
                    Register src_reg = ((RegOperand*)src)->reg;
                    --src_ref_count[src_reg];
                    if (src_ref_count[src_reg] == 0) can_seq.push(src_reg);
                }
                ++src_ref_count[mid_reg];
                block->phi_copy_map[dest] = new RegOperand(mid_reg);
            }
        }
    }

    void PhiDestructionPass::redirectBranchInst(Function* func, int from, int origin_to, int new_to)
    {
        Block* from_block = func->cfg->blocks[from];
        bool   jal_got    = false;

        for (auto it = from_block->insts.rbegin(); it != from_block->insts.rend(); ++it)
        {
            Instruction* inst = *it;

            if (inst->inst_type == InstType::PHI) continue;
            if (inst->inst_type == InstType::MOVE)
            {
                if (jal_got) break;
                continue;
            }

            RV64Inst* rv64_inst = (RV64Inst*)inst;
            if (rv64_inst->op == RV64InstType::JALR) break;
            if (rv64_inst->op == RV64InstType::JAL)
            {
                if (rv64_inst->label.jmp_label == origin_to) rv64_inst->label = RV64Label(new_to);
                jal_got = true;
                continue;
            }
            if (opInfoTable[rv64_inst->op].type == RV64OpType::B)
            {
                if (rv64_inst->label.jmp_label == origin_to)
                    rv64_inst->label = RV64Label(new_to, rv64_inst->label.seq_label);
                else if (rv64_inst->label.seq_label == origin_to)
                    rv64_inst->label = RV64Label(rv64_inst->label.jmp_label, new_to);
                break;
            }
            if (jal_got) break;
        }
    }

}  // namespace Backend::RV64::Passes::Optimize::ControlFlow
