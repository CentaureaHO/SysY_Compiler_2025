#include "llvm/loop/loop_strength_reduce.h"
#include <assert.h>
#include <iostream>

namespace Transform
{

using namespace LLVMIR;
bool StrengthReduce::checkDom(int dom,int node){
    
    auto dom_tree=this->ir->DomTrees[loop->cfg];
    if(dom==node){return true;}
    if((unsigned int)dom>=dom_tree->dom_tree.size()){return false;}
    for(auto iter:dom_tree->dom_tree[dom]){
        if(checkDom(iter,node)){return true;}
    }
    return false;
}

//对循环中乘法做强度削弱操作
/* 
%reg_12 = mul i32 10, %reg_9 //{10,+,10}
->

header:
%new_reg = phi[10,preheader],[reg_next,latch]

latch:
%reg_next = add %new_reg,10

将reg_12重命名为new_reg

*/
void StrengthReduce::doMulStrengthReduce(){
    auto dom_tree=this->ir->DomTrees[loop->cfg];
    assert(dom_tree!=nullptr);


    int latch_num=(*(loop->latches.begin()))->block_id;
    int header_num=loop->header->block_id;

    auto func_now=loop->cfg->func;
    //std::cout<<"reach before sr main loop"<<std::endl;
    //要求指令为MUL 有合法SCEV表达式 支配latch块
    //for(auto iter:loop->loop_nodes){std::cout<<iter->block_id<<std::endl;}
    for(auto iter:loop->loop_nodes){
        //std::cout<<"block here: "<<iter->block_id<<std::endl;
        if(!checkDom(iter->block_id,latch_num)){
            //std::cout<<"block jump: "<<iter->block_id<<std::endl;
            continue;//要求支配latch
        }
        
        for(auto ins:iter->insts){
            //std::cout<<"ins now is with reg result: "<<ins->GetResultReg()<<std::endl;
            if(ins->opcode!=IROpCode::MUL){
                continue;
            }

            int res_no=ins->GetResultReg();
            if(res_no==-1){continue;}

            auto scevexp=scev->getCR(loop,res_no);
            if(scevexp==nullptr){continue;}
            if(scevexp->length()!=1){continue;}//先只分析长度为2的，其他如何处理后面再分析 标记
            
            if(scevexp->operators[0]!=Analysis::CROperator::ADD){continue;}

            auto mul_ins=(ArithmeticInst*) ins;
            //std::cout<<"get reduced"<<std::endl;

            //只处理operand为CONSTANT的情况，别的后面在说
            auto start=scevexp->operands[0];
            auto step=scevexp->operands[1];

            Operand* irop_start,*irop_step;

            if(start.type==Analysis::CROperand::CHAIN_OF_RECURRENCES){continue;}
            else if(start.type==Analysis::CROperand::CONSTANT){
                irop_start = new ImmeI32Operand(start.const_val);
            }else if(start.type==Analysis::CROperand::LLVM_OPERAND){
                continue;
            }else{continue;}

            if(step.type==Analysis::CROperand::CHAIN_OF_RECURRENCES){continue;}
            else if(step.type==Analysis::CROperand::CONSTANT){
                irop_step = new ImmeI32Operand(step.const_val);
            }else if(step.type==Analysis::CROperand::LLVM_OPERAND){
                continue;
            }else{continue;}

            //构造phi指令
            auto new_phi=new PhiInst(
                mul_ins->type,
                new RegOperand(++func_now->max_reg)
            );

            int new_phi_no=func_now->max_reg;

            //初始化值
            new_phi->vals_for_labels.push_back(
                std::pair<Operand*,Operand*>{irop_start,new LabelOperand(loop->preheader->block_id)}
            );

            //构造next值的计算，等于new_phi_no那个寄存器+step
            auto new_add_ins= new ArithmeticInst(
                IROpCode::ADD,
                mul_ins->type,
                new RegOperand(new_phi_no),
                irop_step,
                new RegOperand(++func_now->max_reg)
            );

            int new_add_no=func_now->max_reg;

            //加入phi
            new_phi->vals_for_labels.push_back(
                std::pair<Operand*,Operand*>{new RegOperand(new_add_no) ,new LabelOperand(latch_num)}
            );

            //插入新指令
            loop->header->insts.push_front(new_phi);
            func_now->blocks[latch_num]->insts.push_front(new_add_ins);

            //记录重命名与删除
            replace[ins->GetResultReg()]=new_phi_no;
            todel[ins->GetResultReg()]=1;//后续交给dce去删除

        }
    }
    // for(auto iter:replace){
    //     std::cout<<iter.first<<' '<<iter.second<<std::endl;
    // }
    //变量重命名
    for(auto iter:loop->cfg->func->blocks){
        for(auto ins:iter->insts){
            if(todel[ins->GetResultReg()]){continue;}
            ins->Rename(replace);
        }
    }

}



void StrengthReducePass::Execute(){
    //std::cout<<"----------------INFO ABOUT StrengthReducePass-------------------"<<std::endl;
    for(auto [func,cfg_now]:ir->cfg){
        for(auto [block,loop]:cfg_now->LoopForest->header_loop_map){
            //std::cout<<"on loop "<<loop->loop_id<<":"<<std::endl;
            StrengthReduce s{this->scev,loop,ir};
            s.doMulStrengthReduce();
        }
    }
}

}  // namespace Transform