#ifndef __BACKEND_RV64_CFG_H__
#define __BACKEND_RV64_CFG_H__

#include "rv64_block.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <memory>
#include <dom_analyzer.h>

namespace Backend::RV64
{
    class CFG
    {
      public:
        std::map<int, Block*> blocks;
        MAT2(Block*) graph, inv_graph;
        int max_label;

        Block* entry_block;
        Block* ret_block;

        std::unique_ptr<Cele::Algo::DomAnalyzer> dom_tree;
        std::unique_ptr<Cele::Algo::DomAnalyzer> post_dom_tree;

      public:
        CFG();

      public:
        void addNewBlock(int id, Block* b);
        void makeEdge(int from, int to);
        void removeEdge(int from, int to);

        std::vector<std::vector<int>> buildGraphAdjacencyList() const;
    };
}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_CFG_H__
