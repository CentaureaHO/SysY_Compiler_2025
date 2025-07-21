#include "llvm_ir/ir_builder.h"
#include <algorithm>

std::map<int, int> visited;

// 遍历CFG中块中代码，构建CFG
void dfs(CFG* cfg, int block_id)
{
    // std::cout<<"block id is:"<<block_id<<std::endl;
    auto block_now    = cfg->block_id_to_block[block_id];
    visited[block_id] = 1;

    // 遍历当前块，停止在第一条跳转/返回指令
    uint32_t i = 0;
    for (; i < block_now->insts.size(); i++)
    {
        auto op = block_now->insts[i]->opcode;
        if (op == LLVMIR::IROpCode::BR_COND || op == LLVMIR::IROpCode::BR_UNCOND || op == LLVMIR::IROpCode::RET)
        {
            break;
        }
    }

    // 清除后续无用指令
    block_now->insts.erase(block_now->insts.begin() + i + 1, block_now->insts.end());
    // std::cout<<"br/ret ins is: "<<i<<std::endl;
    // 根据这条指令填写CFG并递归
    auto op = block_now->insts[i]->opcode;
    if (op == LLVMIR::IROpCode::BR_COND)
    {
        LLVMIR::BranchCondInst* brinst  = (LLVMIR::BranchCondInst*)block_now->insts[i];
        int                     t_label = ((LLVMIR::LabelOperand*)(brinst->true_label))->label_num;
        int                     f_label = ((LLVMIR::LabelOperand*)(brinst->false_label))->label_num;

        cfg->G[block_id].push_back(cfg->block_id_to_block[t_label]);
        cfg->G_id[block_id].push_back(t_label);
        cfg->invG[t_label].push_back(block_now);
        cfg->invG_id[t_label].push_back(block_id);

        cfg->G[block_id].push_back(cfg->block_id_to_block[f_label]);
        cfg->G_id[block_id].push_back(f_label);
        cfg->invG[f_label].push_back(block_now);
        cfg->invG_id[f_label].push_back(block_id);

        // 递归
        if (!visited[t_label]) { dfs(cfg, t_label); }
        if (!visited[f_label]) { dfs(cfg, f_label); }
    }
    else if (op == LLVMIR::IROpCode::BR_UNCOND)
    {
        LLVMIR::BranchUncondInst* ubrinst = (LLVMIR::BranchUncondInst*)block_now->insts[i];
        int                       label   = ((LLVMIR::LabelOperand*)(ubrinst->target_label))->label_num;

        cfg->G[block_id].push_back(cfg->block_id_to_block[label]);

        cfg->G_id[block_id].push_back(label);
        cfg->invG[label].push_back(block_now);
        cfg->invG_id[label].push_back(block_id);

        if (!visited[label]) { dfs(cfg, label); }
    }
}
void CFG::BuildCFG()
{
    // 从入口块进行一次dfs，这里我们的入口块编号固定为0
    visited.clear();

    G.resize(func->max_label + 1);
    invG.resize(func->max_label + 1);
    G_id.resize(func->max_label + 1);
    invG_id.resize(func->max_label + 1);

    dfs(this, 0);
    // std::cout<<222<<std::endl;
    // 在当前cfg中删除不可达块
    for (auto iter = this->block_id_to_block.begin(); iter != this->block_id_to_block.end();)
    {
        if (!visited[iter->first]) { iter = this->block_id_to_block.erase(iter); }
        else
            iter++;
    }
    auto blocks_temp = this->func->blocks;
    this->func->blocks.clear();
    for (auto& iter : blocks_temp)
    {
        if (visited[iter->block_id]) this->func->blocks.push_back(iter);
    }

    // 清理G_id和invG_id中指向已删除块的边
    for (size_t i = 0; i < G_id.size(); ++i)
    {
        if (!visited[i]) continue;

        auto& edges = G_id[i];
        edges.erase(std::remove_if(edges.begin(), edges.end(), [&](int target_id) { return !visited[target_id]; }),
            edges.end());
    }

    for (size_t i = 0; i < invG_id.size(); ++i)
    {
        if (!visited[i]) continue;

        auto& edges = invG_id[i];
        edges.erase(std::remove_if(edges.begin(), edges.end(), [&](int source_id) { return !visited[source_id]; }),
            edges.end());
    }

    // std::cout<<"visiting condition: "<<std::endl;
    // for(auto iter:visited) std::cout<<iter.first<<' '<<iter.second<<std::endl;
    // // 打印CFG
    //  for(uint32_t i=0;i<G_id.size();i++){
    //      std::cout<<i<<" -> ";
    //      for(uint32_t j=0;j<G_id[i].size();j++){
    //          std::cout<<G_id[i][j]<<' ';
    //      }
    //      std::cout<<std::endl;
    //  }
    //  std::cout<<"-----------------------------"<<std::endl;
}
