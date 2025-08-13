#include <backend/rv64/rv64_loop.h>
#include <backend/rv64/rv64_function.h>
#include <backend/rv64/insts.h>
#include <iostream>
#include <cassert>
#include <stack>
#include <queue>
#include <algorithm>

namespace Backend::RV64
{

    NaturalLoopForest::~NaturalLoopForest() { clear(); }

    void NaturalLoop::findExitNodes(CFG* cfg)
    {
        exit_nodes.clear();
        exiting_nodes.clear();

        for (const auto& node : loop_nodes)
        {
            if (node->insts.empty()) continue;

            int block_id = node->label_num;
            if (static_cast<size_t>(block_id) >= cfg->graph.size()) continue;

            bool has_exit = false;
            for (const auto& successor : cfg->graph[block_id])
            {
                if (successor && loop_nodes.find(successor) == loop_nodes.end())
                {
                    exit_nodes.insert(successor);
                    has_exit = true;
                }
            }

            if (has_exit) { exiting_nodes.insert(node); }
        }
    }

    void NaturalLoop::addPreheader(CFG* cfg, Function* func)
    {
        std::set<Block*> out_of_loop_predecessors;

        int header_id = header->label_num;
        if (static_cast<size_t>(header_id) < cfg->inv_graph.size())
        {
            for (const auto& pred : cfg->inv_graph[header_id])
            {
                if (pred && loop_nodes.find(pred) == loop_nodes.end()) { out_of_loop_predecessors.insert(pred); }
            }
        }

        if (out_of_loop_predecessors.empty()) { return; }

        if (out_of_loop_predecessors.size() == 1)
        {
            auto* candidate    = *out_of_loop_predecessors.begin();
            int   candidate_id = candidate->label_num;
            if (static_cast<size_t>(candidate_id) < cfg->graph.size() && cfg->graph[candidate_id].size() == 1)
            {
                preheader = candidate;
                return;
            }
        }

        preheader = insertTransferBlock(cfg, func, out_of_loop_predecessors, header);

        for (auto* current = fa_loop; current != nullptr; current = current->fa_loop)
        {
            current->loop_nodes.insert(preheader);
        }
    }

    bool NaturalLoop::isSimplified() const { return (latches.size() == 1) && (preheader != nullptr); }

    void NaturalLoopForest::combineSameHeadLoops()
    {
        std::set<Block*>       seen_headers;
        std::set<NaturalLoop*> loops_to_remove;

        for (auto* loop : loop_set)
        {
            if (seen_headers.find(loop->header) == seen_headers.end())
            {
                seen_headers.insert(loop->header);
                header_loop_map[loop->header] = loop;
                continue;
            }

            loops_to_remove.insert(loop);
            auto it = header_loop_map.find(loop->header);
            if (it == header_loop_map.end()) continue;

            auto* existing_loop = it->second;

            existing_loop->loop_nodes.insert(loop->loop_nodes.begin(), loop->loop_nodes.end());
            existing_loop->latches.insert(loop->latches.begin(), loop->latches.end());
        }

        for (auto* loop : loops_to_remove)
        {
            loop_set.erase(loop);
            delete loop;
        }
    }

    void NaturalLoopForest::buildHierarchy()
    {
        loopG.clear();
        loopG.resize(loop_cnt + 1);

        std::vector<std::vector<NaturalLoop*>>    temp_graph(loop_cnt + 1);
        std::vector<std::pair<int, NaturalLoop*>> in_degree(loop_cnt + 1);

        for (auto* loop : loop_set) in_degree[loop->loop_id].second = loop;

        for (auto* l1 : loop_set)
        {
            for (auto* l2 : loop_set)
            {
                if (l1 == l2) continue;

                if (judgeLoopContain(l1, l2))
                {
                    temp_graph[l1->loop_id].push_back(l2);
                    ++in_degree[l2->loop_id].first;
                }
            }
        }

        std::queue<NaturalLoop*> queue;

        for (const auto& [degree, loop] : in_degree)
        {
            if (degree == 0 && loop != nullptr) queue.push(loop);
        }

        while (!queue.empty())
        {
            auto* current = queue.front();
            queue.pop();

            for (auto* child : temp_graph[current->loop_id])
            {
                --in_degree[child->loop_id].first;
                if (in_degree[child->loop_id].first != 0) continue;

                loopG[current->loop_id].push_back(child);
                child->fa_loop = current;
                queue.push(child);
            }
        }
    }

    void NaturalLoopForest::clear()
    {
        for (auto* loop : loop_set) delete loop;

        loop_set.clear();
        header_loop_map.clear();
        loopG.clear();
        loop_cnt = 0;
    }

    std::vector<NaturalLoop*> NaturalLoopForest::getRootLoops() const
    {
        std::vector<NaturalLoop*> roots;

        for (auto* loop : loop_set)
        {
            if (loop->fa_loop == nullptr) roots.push_back(loop);
        }

        return roots;
    }

    Block* insertTransferBlock(CFG* cfg, Function* func, const std::set<Block*>& froms, Block* to)
    {
        assert(!froms.empty() && to != nullptr);

        Block* transfer_block = func->getNewBlock();
        cfg->addNewBlock(transfer_block->label_num, transfer_block);

        auto* jump_inst = createJInst(RV64InstType::JAL, preg_x0, RV64Label(to->label_num));
        transfer_block->insts.push_back(jump_inst);

        for (auto* from : froms)
        {
            for (auto* inst : from->insts)
            {
                if (inst->inst_type == InstType::RV64)
                {
                    auto* rv_inst = static_cast<RV64Inst*>(inst);

                    if (rv_inst->op == RV64InstType::JAL && rv_inst->rd.reg_num == static_cast<int>(REG::x0) &&
                        rv_inst->use_label && rv_inst->label.jmp_label == to->label_num)
                    {
                        rv_inst->label = RV64Label(transfer_block->label_num);
                    }
                    else if ((rv_inst->op == RV64InstType::BEQ || rv_inst->op == RV64InstType::BNE ||
                                 rv_inst->op == RV64InstType::BLT || rv_inst->op == RV64InstType::BGE ||
                                 rv_inst->op == RV64InstType::BLTU || rv_inst->op == RV64InstType::BGEU ||
                                 rv_inst->op == RV64InstType::BGT || rv_inst->op == RV64InstType::BLE) &&
                             rv_inst->use_label && rv_inst->label.jmp_label == to->label_num)
                    {
                        rv_inst->label = RV64Label(transfer_block->label_num);
                    }
                }
            }
        }

        for (auto* from : froms)
        {
            cfg->removeEdge(from->label_num, to->label_num);
            cfg->makeEdge(from->label_num, transfer_block->label_num);
        }
        cfg->makeEdge(transfer_block->label_num, to->label_num);

        return transfer_block;
    }

    std::set<Block*> findNodesInLoop(CFG* cfg, Block* latch, Block* header)
    {
        assert(latch != nullptr && header != nullptr);

        std::set<Block*>   loop_nodes;
        std::stack<Block*> worklist;

        loop_nodes.insert(latch);
        loop_nodes.insert(header);

        if (latch == header) return loop_nodes;

        worklist.push(latch);

        while (!worklist.empty())
        {
            auto* current = worklist.top();
            worklist.pop();

            int current_id = current->label_num;
            if (static_cast<size_t>(current_id) >= cfg->inv_graph.size()) continue;

            for (auto* predecessor : cfg->inv_graph[current_id])
            {
                if (predecessor && loop_nodes.find(predecessor) == loop_nodes.end())
                {
                    loop_nodes.insert(predecessor);
                    worklist.push(predecessor);
                }
            }
        }

        return loop_nodes;
    }

    bool judgeLoopContain(const NaturalLoop* outer, const NaturalLoop* inner)
    {
        if (!outer || !inner || outer == inner) return false;

        return std::all_of(inner->loop_nodes.begin(), inner->loop_nodes.end(), [&outer](const auto* node) {
            return outer->loop_nodes.find(const_cast<Block*>(node)) != outer->loop_nodes.end();
        });
    }

}  // namespace Backend::RV64
