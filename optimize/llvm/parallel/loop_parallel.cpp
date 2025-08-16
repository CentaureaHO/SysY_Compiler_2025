#include "llvm/parallel/loop_parallel.h"

namespace Transform
{
    bool LoopParallelizer::convertToOpenMP(NaturalLoop* loop)
    {
        auto& loop_info = parallel_analysis->getParallelInfo(loop);
        if (!loop_info.is_parallelizable) return false;

        auto* header    = loop->header;
        auto* preheader = loop->preheader;
        if (!preheader) return false;

        // 1. 在preheader中插入OpenMP并行指令
        std::string omp_pragma = "#pragma omp parallel for";

        // 添加归约子句
        if (!loop_info.reduction_insts.empty())
        {
            omp_pragma += " reduction(";
            for (size_t i = 0; i < loop_info.reduction_insts.size(); i++)
            {
                auto* inst = loop_info.reduction_insts[i];
                char  op   = '+';  // 默认加法归约

                if (inst->opcode == LLVMIR::IROpCode::MUL) op = '*';

                if (i > 0) omp_pragma += ",";
                omp_pragma += op;
                omp_pragma += ":reg_" + std::to_string(inst->GetResultReg());
            }
            omp_pragma += ")";
        }

        // 添加私有变量子句
        std::set<int> private_regs;
        for (auto* block : loop->loop_nodes)
        {
            for (auto* inst : block->insts)
            {
                if (inst->GetResultReg() != -1) { private_regs.insert(inst->GetResultReg()); }
            }
        }

        if (!private_regs.empty())
        {
            omp_pragma += " private(";
            bool first = true;
            for (int reg : private_regs)
            {
                if (!first) omp_pragma += ",";
                omp_pragma += "reg_" + std::to_string(reg);
                first = false;
            }
            omp_pragma += ")";
        }

        // 2. 创建pragma指令并插入preheader
        auto* pragma_inst     = new LLVMIR::PragmaInst(omp_pragma);
        pragma_inst->block_id = preheader->block_id;
        preheader->insts.insert(preheader->insts.end() - 1, pragma_inst);

        std::cout << "为循环 " << loop->loop_id << " 生成OpenMP指令: " << omp_pragma << std::endl;
        return true;
    }
}  // namespace Transform