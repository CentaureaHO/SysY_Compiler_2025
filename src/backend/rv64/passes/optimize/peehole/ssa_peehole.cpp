#include <backend/rv64/passes/optimize/peehole/ssa_peehole.h>
#include <backend/rv64/insts.h>

namespace Backend::RV64::Passes::Optimize::Peehole
{

    SSAPeepholePass::SSAPeepholePass(std::vector<Function*>& functions) : functions_(functions) {}

    bool SSAPeepholePass::run()
    {
        for (auto& func : functions_) { optimizeFunction(func); }
        return true;
    }

    void SSAPeepholePass::optimizeFunction(Function* func)
    {
        std::map<Register, Register> negative_map;

        for (auto& [id, block] : func->cfg->blocks)
        {
            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                if ((*it)->inst_type == InstType::RV64)
                {
                    RV64Inst* rv_inst = (RV64Inst*)*it;
                    if (rv_inst->op == RV64InstType::FNEG_S)
                    {
                        Register rd  = rv_inst->rd;
                        Register rs1 = rv_inst->rs1;
                        if (rd.is_virtual && rs1.is_virtual) { negative_map[rd] = rs1; }
                    }
                }

                auto jt = it;
                for (int i = 1; i <= WINDOW_LENGTH; i++)
                {
                    if (jt == block->insts.begin()) break;
                    --jt;

                    if (tryAddi(jt, it, block)) { break; }
                    if (tryFmla(jt, it, block))
                    {
                        assert(false && "tryFmla");
                        break;
                    }
#if RV64_ENABLE_ZBA
                    if (tryShxAdd(jt, it, block)) { break; }
                    if (tryConstShxAdd(jt, it, block, func)) { break; }
#endif
                    if (tryMemOffset(jt, it, block)) { break; }
                }
            }

            for (auto it = block->insts.begin(); it != block->insts.end(); ++it)
            {
                if ((*it)->inst_type == InstType::RV64)
                {
                    RV64Inst* rv_inst = (RV64Inst*)*it;
                    if (inFMList(rv_inst->op))
                    {
                        Register rs1 = rv_inst->rs1;
                        Register rs2 = rv_inst->rs2;
                        Register rs3 = rv_inst->rs3;

                        bool mul_neg = false;
                        bool add_neg = false;

                        if (negative_map.find(rs1) != negative_map.end())
                        {
                            rv_inst->rs1 = negative_map[rs1];
                            mul_neg      = !mul_neg;
                        }
                        if (negative_map.find(rs2) != negative_map.end())
                        {
                            rv_inst->rs2 = negative_map[rs2];
                            mul_neg      = !mul_neg;
                        }
                        if (negative_map.find(rs3) != negative_map.end())
                        {
                            rv_inst->rs3 = negative_map[rs3];
                            add_neg      = !add_neg;
                        }

                        if (mul_neg) { rv_inst->op = reverseMulOp(rv_inst->op); }
                        if (add_neg) { rv_inst->op = reverseAddOp(rv_inst->op); }
                    }
                }
            }
        }
    }

    bool SSAPeepholePass::tryAddi(
        std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block)
    {
        Instruction* pre_inst = *pre;
        Instruction* cur_inst = *cur;

        if (pre_inst->inst_type == InstType::MOVE && cur_inst->inst_type == InstType::RV64)
        {
            MoveInst* move_inst = (MoveInst*)pre_inst;
            RV64Inst* rv_inst   = (RV64Inst*)cur_inst;

            if (move_inst->src->operand_type == OperandType::IMMEI32 &&
                move_inst->dst->operand_type == OperandType::REG)
            {

                ImmeI32Operand* src_imm    = (ImmeI32Operand*)move_inst->src;
                RegOperand*     dst_reg_op = (RegOperand*)move_inst->dst;
                Register        dst_reg    = dst_reg_op->reg;

                if (src_imm->val >= -2048 && src_imm->val <= 2047)
                {
                    if (rv_inst->op == RV64InstType::ADD)
                    {
                        if (rv_inst->rs2 == dst_reg)
                        {
                            cur = block->insts.erase(cur);
                            RV64Inst* new_inst =
                                createIInst(RV64InstType::ADDI, rv_inst->rd, rv_inst->rs1, src_imm->val);
                            block->insts.insert(cur, new_inst);
                            --cur;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool SSAPeepholePass::tryFmla(
        std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block)
    {
        return false;  // 精度问题，本地无法通过测试

        Instruction* pre_inst = *pre;
        Instruction* cur_inst = *cur;

        if (pre_inst->inst_type == InstType::RV64 && cur_inst->inst_type == InstType::RV64)
        {
            RV64Inst* pre_rv = (RV64Inst*)pre_inst;
            RV64Inst* cur_rv = (RV64Inst*)cur_inst;

            // FMUL_S + FADD_S → FMADD_S
            if (pre_rv->op == RV64InstType::FMUL_S && cur_rv->op == RV64InstType::FADD_S)
            {
                if (!pre_rv->rd.is_virtual) return false;
                if (pre_rv->rd == cur_rv->rs1 || pre_rv->rd == cur_rv->rs2)
                {
                    Register diff_op = cur_rv->rs1;
                    if (pre_rv->rd == cur_rv->rs1)
                    {
                        diff_op = cur_rv->rs2;

                        cur = block->insts.erase(cur);

                        RV64Inst* new_inst =
                            createR4Inst(RV64InstType::FMADD_S, cur_rv->rd, pre_rv->rs1, pre_rv->rs2, diff_op);
                        block->insts.insert(cur, new_inst);
                        --cur;
                        return true;
                    }
                    else if (pre_rv->rd == cur_rv->rs2)
                    {
                        diff_op = cur_rv->rs1;
                        // TODO
                    }
                }
            }

            // FMUL_S + FSUB_S → FMSUB_S
            if (pre_rv->op == RV64InstType::FMUL_S && cur_rv->op == RV64InstType::FSUB_S)
            {
                if (!pre_rv->rd.is_virtual) return false;
                if (pre_rv->rd == cur_rv->rs1)
                {
                    Register diff_op = cur_rv->rs2;

                    cur = block->insts.erase(cur);

                    RV64Inst* new_inst =
                        createR4Inst(RV64InstType::FMSUB_S, cur_rv->rd, pre_rv->rs1, pre_rv->rs2, diff_op);
                    block->insts.insert(cur, new_inst);
                    --cur;
                    return true;
                }
            }
        }
        return false;
    }

#if RV64_ENABLE_ZBA
    bool SSAPeepholePass::tryShxAdd(
        std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block)
    {
        Instruction* pre_inst = *pre;
        Instruction* cur_inst = *cur;

        if (pre_inst->inst_type == InstType::RV64 && cur_inst->inst_type == InstType::RV64)
        {
            RV64Inst* pre_rv = (RV64Inst*)pre_inst;
            RV64Inst* cur_rv = (RV64Inst*)cur_inst;

            if ((pre_rv->op == RV64InstType::SLLI && cur_rv->op == RV64InstType::ADD) ||
                (pre_rv->op == RV64InstType::SLLIW && cur_rv->op == RV64InstType::ADDW))
            {

                if (!pre_rv->rd.is_virtual) return false;
                if (pre_rv->imme > 3 || pre_rv->imme <= 0) return false;
                if (pre_rv->rd == cur_rv->rs1 || pre_rv->rd == cur_rv->rs2)
                {
                    Register     diff_op = cur_rv->rs1;
                    RV64InstType new_op;

                    if (pre_rv->op == RV64InstType::SLLI)
                    {
                        // SLLI + ADD  →  SHxADD
                        switch (pre_rv->imme)
                        {
                            case 1: new_op = RV64InstType::SH1ADD; break;
                            case 2: new_op = RV64InstType::SH2ADD; break;
                            case 3: new_op = RV64InstType::SH3ADD; break;
                            default: return false;
                        }
                    }
                    else
                    {
                        // SLLIW + ADDW  →  SHxADDUW
                        switch (pre_rv->imme)
                        {
                            case 1: new_op = RV64InstType::SH1ADDUW; break;
                            case 2: new_op = RV64InstType::SH2ADDUW; break;
                            case 3: new_op = RV64InstType::SH3ADDUW; break;
                            default: return false;
                        }
                    }

                    if (pre_rv->rd == cur_rv->rs1) { diff_op = cur_rv->rs2; }
                    else if (pre_rv->rd == cur_rv->rs2) { diff_op = cur_rv->rs1; }

                    cur                = block->insts.erase(cur);
                    RV64Inst* new_inst = createRInst(new_op, cur_rv->rd, pre_rv->rs1, diff_op);
                    block->insts.insert(cur, new_inst);
                    --cur;
                    return true;
                }
            }
        }
        return false;
    }

    bool SSAPeepholePass::tryConstShxAdd(
        std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block, Function* func)
    {
        Instruction* pre_inst = *pre;
        Instruction* cur_inst = *cur;

        if (pre_inst->inst_type == InstType::MOVE && cur_inst->inst_type == InstType::RV64)
        {
            MoveInst* move_inst = (MoveInst*)pre_inst;
            RV64Inst* rv_inst   = (RV64Inst*)cur_inst;

            if (move_inst->src->operand_type == OperandType::IMMEI32)
            {
                ImmeI32Operand* src_imm = (ImmeI32Operand*)move_inst->src;
                int             pre_imm = src_imm->val;

                if (pre_imm <= 2047) return false;
                if (rv_inst->op == RV64InstType::ADD)
                {
                    RegOperand* dst_reg_op = (RegOperand*)move_inst->dst;
                    Register    pre_rd     = dst_reg_op->reg;

                    if (pre_rd == rv_inst->rs1 || pre_rd == rv_inst->rs2)
                    {
                        int ctz = __builtin_ctz(pre_imm);
                        if (ctz > 3) ctz = 3;
                        if (ctz == 0) return false;

                        int after_imm = pre_imm >> ctz;
                        if (after_imm <= 2047 && after_imm >= -2048)
                        {
                            Register mid_reg = func->getNewReg(INT64);
                            Register diff_op = rv_inst->rs1;
                            if (rv_inst->rs1 == pre_rd) { diff_op = rv_inst->rs2; }
                            else { diff_op = rv_inst->rs1; }

                            RV64InstType base_op;
                            if (rv_inst->op == RV64InstType::ADDW)
                            {
                                switch (ctz)
                                {
                                    case 1: base_op = RV64InstType::SH1ADDUW; break;
                                    case 2: base_op = RV64InstType::SH2ADDUW; break;
                                    case 3: base_op = RV64InstType::SH3ADDUW; break;
                                    default: return false;
                                }
                            }
                            else
                            {
                                switch (ctz)
                                {
                                    case 1: base_op = RV64InstType::SH1ADD; break;
                                    case 2: base_op = RV64InstType::SH2ADD; break;
                                    case 3: base_op = RV64InstType::SH3ADD; break;
                                    default: return false;
                                }
                            }

                            cur                = block->insts.erase(cur);
                            MoveInst* new_move = createMoveInst(INT64, mid_reg, after_imm);
                            RV64Inst* new_inst = createRInst(base_op, rv_inst->rd, mid_reg, diff_op);
                            block->insts.insert(cur, new_move);
                            block->insts.insert(cur, new_inst);
                            --cur;
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
#endif

    bool SSAPeepholePass::tryMemOffset(
        std::list<Instruction*>::iterator& pre, std::list<Instruction*>::iterator& cur, Block* block)
    {
        Instruction* pre_inst = *pre;
        Instruction* cur_inst = *cur;

        if (pre_inst->inst_type == InstType::RV64 && cur_inst->inst_type == InstType::RV64)
        {
            RV64Inst* pre_rv = (RV64Inst*)pre_inst;
            RV64Inst* cur_rv = (RV64Inst*)cur_inst;

            if (pre_rv->op == RV64InstType::ADDI)
            {
                if (!cur_rv->use_label)
                {
                    if (cur_rv->op == RV64InstType::LD || cur_rv->op == RV64InstType::LW ||
                        cur_rv->op == RV64InstType::FLW || cur_rv->op == RV64InstType::FLD)
                    {

                        if (pre_rv->rd == cur_rv->rs1)
                        {
                            if (!pre_rv->use_label)
                            {
                                int new_imm = pre_rv->imme + cur_rv->imme;
                                if (new_imm >= -2048 && new_imm <= 2047)
                                {
                                    cur_rv->imme = new_imm;
                                    cur_rv->rs1  = pre_rv->rs1;
                                    return true;
                                }
                            }
                            else if (cur_rv->imme == 0)
                            {
                                cur_rv->use_label = true;
                                cur_rv->label     = pre_rv->label;
                                cur_rv->rs1       = pre_rv->rs1;
                                return true;
                            }
                        }
                    }
                    else if (cur_rv->op == RV64InstType::SD || cur_rv->op == RV64InstType::SW ||
                             cur_rv->op == RV64InstType::FSD || cur_rv->op == RV64InstType::FSW)
                    {

                        if (pre_rv->rd == cur_rv->rs2)
                        {
                            if (!pre_rv->use_label)
                            {
                                int new_imm = pre_rv->imme + cur_rv->imme;
                                if (new_imm >= -2048 && new_imm <= 2047)
                                {
                                    cur_rv->imme = new_imm;
                                    cur_rv->rs2  = pre_rv->rs1;
                                    return true;
                                }
                            }
                            else if (cur_rv->imme == 0)
                            {
                                cur_rv->use_label = true;
                                cur_rv->label     = pre_rv->label;
                                cur_rv->rs2       = pre_rv->rs1;
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    bool SSAPeepholePass::inFMList(RV64InstType opcode)
    {
        return opcode == RV64InstType::FMADD_S || opcode == RV64InstType::FMSUB_S || opcode == RV64InstType::FNMADD_S ||
               opcode == RV64InstType::FNMSUB_S;
    }

    RV64InstType SSAPeepholePass::reverseMulOp(RV64InstType opcode)
    {
        switch (opcode)
        {
            case RV64InstType::FMADD_S: return RV64InstType::FNMADD_S;
            case RV64InstType::FMSUB_S: return RV64InstType::FNMSUB_S;
            case RV64InstType::FNMADD_S: return RV64InstType::FMADD_S;
            case RV64InstType::FNMSUB_S: return RV64InstType::FMSUB_S;
            default: return opcode;
        }
    }

    RV64InstType SSAPeepholePass::reverseAddOp(RV64InstType opcode)
    {
        switch (opcode)
        {
            case RV64InstType::FMADD_S: return RV64InstType::FMSUB_S;
            case RV64InstType::FMSUB_S: return RV64InstType::FMADD_S;
            case RV64InstType::FNMADD_S: return RV64InstType::FNMSUB_S;
            case RV64InstType::FNMSUB_S: return RV64InstType::FNMADD_S;
            default: return opcode;
        }
    }

}  // namespace Backend::RV64::Passes::Optimize::Peehole
