#include <backend/rv64/rv64_cfg.h>
#include <algorithm>
using namespace Backend::RV64;
using namespace std;

CFG::CFG() : max_label(0), entry_block(nullptr), ret_block(nullptr), dom_tree(nullptr), post_dom_tree(nullptr) {}

void CFG::addNewBlock(int id, Block* b)
{
    blocks[id] = b;
    if (id > max_label) max_label = id;

    while (graph.size() <= (size_t)id) graph.push_back({});
    while (inv_graph.size() <= (size_t)id) inv_graph.push_back({});
}

void CFG::makeEdge(int from, int to)
{
    assert(blocks.find(from) != blocks.end());
    assert(blocks.find(to) != blocks.end());
    assert(blocks[from] != nullptr);
    assert(blocks[to] != nullptr);

    graph[from].push_back(blocks[to]);
    inv_graph[to].push_back(blocks[from]);
}

void CFG::removeEdge(int from, int to)
{
    assert(blocks.find(from) != blocks.end());
    assert(blocks.find(to) != blocks.end());
    assert(blocks[from] != nullptr);
    assert(blocks[to] != nullptr);

    auto it = find(graph[from].begin(), graph[from].end(), blocks[to]);
    if (it != graph[from].end())
        graph[from].erase(it);
    else
        assert(false);

    it = find(inv_graph[to].begin(), inv_graph[to].end(), blocks[from]);
    if (it != inv_graph[to].end())
        inv_graph[to].erase(it);
    else
        assert(false);
}

std::vector<std::vector<int>> CFG::buildGraphAdjacencyList() const
{
    std::vector<std::vector<int>> adj_list(max_label + 1);

    for (const auto& [block_id, block] : blocks)
    {
        if (block_id < graph.size())
        {
            for (Block* succ_block : graph[block_id])
            {
                for (const auto& [succ_id, succ_ptr] : blocks)
                {
                    if (succ_ptr == succ_block)
                    {
                        adj_list[block_id].push_back(succ_id);
                        break;
                    }
                }
            }
        }
    }

    return adj_list;
}
