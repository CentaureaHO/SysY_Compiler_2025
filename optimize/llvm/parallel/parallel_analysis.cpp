#include "llvm/parallel/parallel_analysis.h"

namespace Analysis
{
    bool ParallelAnalysisPass::checkDependencies(NaturalLoop* loop)
    {
        std::cout << "分析循环 " << loop->loop_id << " 的依赖关系..." << std::endl;

        // 收集循环中的所有内存访问指令
        std::vector<LLVMIR::Instruction*> loads, stores;
        for (auto* block : loop->loop_nodes)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::LOAD) { loads.push_back(inst); }
                else if (inst->opcode == LLVMIR::IROpCode::STORE) { stores.push_back(inst); }
            }
        }

        // 检查存储-存储依赖
        for (size_t i = 0; i < stores.size(); i++)
        {
            for (size_t j = i + 1; j < stores.size(); j++)
            {
                if (alias_analyser->hasLoopCarriedDependency(loop, stores[i], stores[j]))
                {
                    loop_info[loop].reason = "存在循环间存储-存储依赖";
                    return false;
                }
            }
        }

        // 检查加载-存储和存储-加载依赖
        for (auto* load : loads)
        {
            for (auto* store : stores)
            {
                if (alias_analyser->hasLoopCarriedDependency(loop, load, store) ||
                    alias_analyser->hasLoopCarriedDependency(loop, store, load))
                {
                    loop_info[loop].reason = "存在循环间加载-存储依赖";
                    return false;
                }
            }
        }

        return true;
    }

    bool ParallelAnalysisPass::identifyReductions(NaturalLoop* loop)
    {
        auto& info = loop_info[loop];
        info.reduction_insts.clear();

        // 查找归约模式
        for (auto* block : loop->loop_nodes)
        {
            for (auto* inst : block->insts)
            {
                if (inst->opcode == LLVMIR::IROpCode::ADD || inst->opcode == LLVMIR::IROpCode::MUL)
                {

                    // 检查是否是归约模式: x = x op expr
                    auto* arith_inst = static_cast<LLVMIR::ArithmeticInst*>(inst);
                    int   result_reg = arith_inst->GetResultReg();

                    // 检查操作数是否包含结果寄存器
                    if ((arith_inst->lhs->type == LLVMIR::OperandType::REG &&
                            static_cast<LLVMIR::RegOperand*>(arith_inst->lhs)->reg_num == result_reg) ||
                        (arith_inst->rhs->type == LLVMIR::OperandType::REG &&
                            static_cast<LLVMIR::RegOperand*>(arith_inst->rhs)->reg_num == result_reg))
                    {

                        info.reduction_insts.push_back(inst);
                    }
                }
            }
        }

        return !info.reduction_insts.empty();
    }
}  // namespace Analysis