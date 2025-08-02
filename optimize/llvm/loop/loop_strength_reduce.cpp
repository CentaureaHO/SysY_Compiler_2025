#include "llvm/loop/loop_strength_reduce.h"
#include <assert.h>
#include <iostream>

namespace Transform
{

    using namespace LLVMIR;
    bool StrengthReduce::checkDom(int dom, int node)
    {

        auto dom_tree = this->ir->DomTrees[loop->cfg];
        if (dom == node) { return true; }
        if ((unsigned int)dom >= dom_tree->dom_tree.size()) { return false; }
        for (auto iter : dom_tree->dom_tree[dom])
        {
            if (checkDom(iter, node)) { return true; }
        }
        return false;
    }

    // 保证PHI在块首
    void insertAfterPHI(std::vector<Instruction*> ins_to_insert, LLVMIR::IRBlock* b)
    {
        assert(b != nullptr);
        auto it = b->insts.begin();
        while (it != b->insts.end() && (*it)->opcode == IROpCode::PHI) { ++it; }
        b->insts.insert(it, ins_to_insert.begin(), ins_to_insert.end());
    }

    void insertBeforeBr(std::vector<Instruction*> ins_to_insert, LLVMIR::IRBlock* b)
    {
        auto br = b->insts.back();
        b->insts.pop_back();
        for (auto iter : ins_to_insert) { b->insts.push_back(iter); }
        b->insts.push_back(br);
    }

    // 对循环中乘法做强度削弱操作
    /*
    %reg_12 = mul i32 10, %reg_9 //{10,+,10}
    ->

    header:
    %new_reg = phi[10,preheader],[reg_next,latch]

    latch:
    %reg_next = add %new_reg,10

    将reg_12重命名为new_reg

    */
    void StrengthReduce::doMulStrengthReduce()
    {
        auto dom_tree = this->ir->DomTrees[loop->cfg];
        assert(dom_tree != nullptr);

        int latch_num  = (*(loop->latches.begin()))->block_id;
        int header_num = loop->header->block_id;

        auto func_now = loop->cfg->func;
        // std::cout<<"reach before sr main loop"<<std::endl;
        // 要求指令为MUL 有合法SCEV表达式
        // for(auto iter:loop->loop_nodes){std::cout<<iter->block_id<<std::endl;}
        for (auto iter : loop->loop_nodes)
        {
            // std::cout<<"block here: "<<iter->block_id<<std::endl;
            if (!checkDom(iter->block_id, latch_num))
            {
                // std::cout<<"block jump: "<<iter->block_id<<std::endl;
                continue;  // 要求支配latch
            }

            for (auto ins : iter->insts)
            {
                // std::cout<<"ins now is with reg result: "<<ins->GetResultReg()<<std::endl;
                if (ins->opcode != IROpCode::MUL) { continue; }

                int res_no = ins->GetResultReg();
                if (res_no == -1) { continue; }

                auto scevexp = scev->getCR(loop, res_no);
                if (scevexp == nullptr) { continue; }
                if (scevexp->length() != 1) { continue; }  // 先只分析长度为2的，其他如何处理后面再分析 标记

                if (scevexp->operators[0] != Analysis::CROperator::ADD) { continue; }

                auto mul_ins = (ArithmeticInst*)ins;
                // std::cout<<"get reduced"<<std::endl;

                // 只处理operand为CONSTANT的情况，别的后面在说
                auto start = scevexp->operands[0];
                auto step  = scevexp->operands[1];

                Operand *irop_start, *irop_step;

                if (start.type == Analysis::CROperand::CHAIN_OF_RECURRENCES) { continue; }
                else if (start.type == Analysis::CROperand::CONSTANT)
                {
                    irop_start = getImmeI32Operand(start.const_val);
                }
                else if (start.type == Analysis::CROperand::LLVM_OPERAND)
                {
                    // continue;
                    if (start.llvm_op->GetRegNum() == -1 || !scev->isLoopInvariant(loop, start.llvm_op->GetRegNum()))
                    {
                        continue;
                    }
                    irop_start = start.llvm_op;
                }
                else { continue; }

                if (step.type == Analysis::CROperand::CHAIN_OF_RECURRENCES) { continue; }
                else if (step.type == Analysis::CROperand::CONSTANT) { irop_step = getImmeI32Operand(step.const_val); }
                else if (step.type == Analysis::CROperand::LLVM_OPERAND)
                {
                    // continue;
                    if (step.llvm_op->GetRegNum() == -1 || !scev->isLoopInvariant(loop, step.llvm_op->GetRegNum()))
                    {
                        continue;
                    }
                    irop_step = step.llvm_op;
                }
                else { continue; }

                // 构造phi指令
                auto new_phi = new PhiInst(mul_ins->type, getRegOperand(++func_now->max_reg));

                int new_phi_no = func_now->max_reg;

                // 初始化值
                new_phi->vals_for_labels.push_back(
                    std::pair<Operand*, Operand*>{irop_start, getLabelOperand(loop->preheader->block_id)});

                // 构造next值的计算，等于new_phi_no那个寄存器+step
                auto new_add_ins = new ArithmeticInst(IROpCode::ADD,
                    mul_ins->type,
                    getRegOperand(new_phi_no),
                    irop_step,
                    getRegOperand(++func_now->max_reg));

                int new_add_no = func_now->max_reg;

                // 加入phi
                new_phi->vals_for_labels.push_back(
                    std::pair<Operand*, Operand*>{getRegOperand(new_add_no), getLabelOperand(latch_num)});

                // 插入新指令
                loop->header->insts.push_front(new_phi);
                // func_now->blocks[latch_num]->insts.push_front(new_add_ins);
                new_add_insts.push_back(new_add_ins);

                // 记录重命名与删除
                replace[ins->GetResultReg()] = new_phi_no;
                todel[ins->GetResultReg()]   = 1;  // 后续交给dce去删除
            }
        }
        // for(auto iter:replace){
        //     std::cout<<iter.first<<' '<<iter.second<<std::endl;
        // }

        // func_now->blocks[latch_num]就是空指针，不知道是IR有什么毛病
        insertAfterPHI(new_add_insts, *(loop->latches.begin()));

        // 变量重命名
        for (auto iter : loop->cfg->func->blocks)
        {
            for (auto ins : iter->insts)
            {
                if (todel[ins->GetResultReg()]) { continue; }
                ins->Rename(replace);
            }
        }
    }

    std::pair<GEPInst*, Operand*> StrengthReduce::checkGEP(GEPInst* ins)
    {
        // 要求最终能化为确定start和确定step的形式
        // 对step非imm或已经存在的operand的情况，不考虑
        // 返回创建初始地址的GEP指令和step Operand

        // 要求最多一个SCEV变量 {start,+,step}

        // std::cout<<"call checkGEP"<<std::endl;
        int      invar_cnt = 0;
        Operand* iv        = nullptr;
        int      iv_idx    = -1;

        for (unsigned int i = 0; i < ins->idxs.size(); ++i)
        {
            auto iter = ins->idxs[i];
            if (iter->type == OperandType::IMMEI32) { continue; }
            if (iter->type == OperandType::REG)
            {
                if (!scev->isLoopInvariant(loop, iter->GetRegNum()))
                {
                    invar_cnt++;
                    iv     = iter;
                    iv_idx = i;
                }
            }
            else { assert(0 && "Unexpected operand type in GEP index"); }
        }
        if (invar_cnt != 1)
        {
            return {nullptr, nullptr};  // 此处为0时，纯常量GEP也可考虑提到循环外，但现在先不考虑
        }

        // 获取并检查SCEV表达式
        auto scevexp = scev->getCR(loop, iv->GetRegNum());
        if (scevexp == nullptr || scevexp->length() != 1 || scevexp->operators[0] != Analysis::CROperator::ADD)
        {
            return {nullptr, nullptr};
        }

        auto start = scevexp->operands[0];
        auto step  = scevexp->operands[1];

        // std::cout<<ins->GetResultReg()<<std::endl;
        // if(step.type!=Analysis::CROperand::Type::CONSTANT){return {nullptr,nullptr};}
        int step_val;
        if (step.type == Analysis::CROperand::Type::CONSTANT) { step_val = step.const_val; }
        else if (step.type == Analysis::CROperand::Type::LLVM_OPERAND)
        {
            if (step.llvm_op->type == OperandType::IMMEI32) { step_val = ((ImmeI32Operand*)step.llvm_op)->value; }
            else { return {nullptr, nullptr}; }
        }
        else { return {nullptr, nullptr}; }
        // std::cout<<"reach 111"<<std::endl;

        // 获取需要的start和step

        Operand* start_irop = nullptr;
        if (start.type == Analysis::CROperand::Type::CONSTANT) { start_irop = getImmeI32Operand(start.const_val); }
        else if (start.type == Analysis::CROperand::Type::LLVM_OPERAND)
        {

            if (  // 不知道为什么循环不变量也不行 标记
                  //(start.llvm_op->type==OperandType::REG&&scev->isLoopInvariant(loop,start.llvm_op->GetRegNum()))||
                start.llvm_op->type == OperandType::IMMEI32)
            {
                start_irop = start.llvm_op;
            }
            else { return {nullptr, nullptr}; }
        }
        else { return {nullptr, nullptr}; }

        // 构造新的GEP，其中result reg应当替换掉，iv对应的维度替换为start
        auto new_gep = new GEPInst(
            ins->type, ins->idx_type, ins->base_ptr, getRegOperand(++loop->cfg->func->max_reg), ins->dims, ins->idxs);
        new_gep->idxs[iv_idx] = start_irop;

        // std::cout<<"dims for "<<ins->GetResultReg()<<std::endl;
        // for(auto iter:ins->dims){std::cout<<iter<<' ';}
        // std::cout<<std::endl;

        for (unsigned int i = iv_idx; i < ins->dims.size(); i++) { step_val *= ins->dims[i]; }

        auto new_imm_step = getImmeI32Operand(step_val);

        return {new_gep, new_imm_step};
    }

    // 对GEP做强度削弱
    // 要求GEP的块支配latch，且step可静态确定
    void StrengthReduce::doGEPStrengthReduce()
    {
        std::vector<LLVMIR::Instruction*> update_insts_latch;
        std::vector<LLVMIR::Instruction*> gep_insts_preheader;

        auto dom_tree = this->ir->DomTrees[loop->cfg];
        assert(dom_tree != nullptr);

        int latch_num  = (*(loop->latches.begin()))->block_id;
        int header_num = loop->header->block_id;

        auto func_now = loop->cfg->func;

        for (auto iter : loop->loop_nodes)
        {
            if (!checkDom(iter->block_id, latch_num))
            {
                continue;  // 要求支配latch
            }

            for (auto ins : iter->insts)
            {
                if (ins->opcode != IROpCode::GETELEMENTPTR) { continue; }
                auto     lsr_gep  = checkGEP((GEPInst*)ins);
                GEPInst* new_gep  = lsr_gep.first;
                Operand* step_imm = lsr_gep.second;

                if (!new_gep || !step_imm) { continue; }

                // 插入新gep到preheader
                gep_insts_preheader.push_back(new_gep);

                RegOperand* new_gep_in_loop = getRegOperand(++loop->cfg->func->max_reg);
                RegOperand* update_op       = getRegOperand(++loop->cfg->func->max_reg);

                // 插入更新指令，使用gep来更新 %2 = gep %1, step
                GEPInst* update_inst =
                    new GEPInst(new_gep->type, new_gep->idx_type, new_gep_in_loop, update_op, {}, {step_imm});

                update_insts_latch.push_back(update_inst);

                // 在header插入phi指令
                auto new_phi = new PhiInst(
                    LLVMIR::DataType::PTR, new_gep_in_loop, new std::vector<std::pair<Operand*, Operand*>>{});

                new_phi->vals_for_labels.push_back(
                    {new_gep->GetResultOperand(), getLabelOperand(loop->preheader->block_id)});

                new_phi->vals_for_labels.push_back({update_inst->GetResultOperand(), getLabelOperand(latch_num)});

                loop->header->insts.push_front(new_phi);

                // 记录替换与删除
                this->replace[ins->GetResultReg()] = new_gep_in_loop->reg_num;
                todel[ins->GetResultReg()]         = 1;
            }
        }

        insertBeforeBr(update_insts_latch, *(loop->latches.begin()));
        insertAfterPHI(gep_insts_preheader, loop->preheader);

        // 变量重命名
        for (auto iter : loop->cfg->func->blocks)
        {
            for (auto ins : iter->insts)
            {
                if (todel[ins->GetResultReg()]) { continue; }
                ins->Rename(replace);
            }
        }
    }

    void StrengthReducePass::Execute()
    {
        // std::cout<<"----------------INFO ABOUT StrengthReducePass-------------------"<<std::endl;
        for (auto [func, cfg_now] : ir->cfg)
        {
            for (auto [block, loop] : cfg_now->LoopForest->header_loop_map)
            {
                // std::cout<<"on loop "<<loop->loop_id<<":"<<std::endl;
                StrengthReduce s{this->scev, loop, ir};
                s.doMulStrengthReduce();
                s.doGEPStrengthReduce();
            }
        }
    }

}  // namespace Transform