#ifndef __BACKEND_RV64_CFG_H__
#define __BACKEND_RV64_CFG_H__

#include "rv64_block.h"
#include <unordered_map>
#include <map>
#include <vector>

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

      public:
        CFG();

      public:
        void addNewBlock(int id, Block* b);
        void makeEdge(int from, int to);
        void removeEdge(int from, int to);
    };
}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_CFG_H__
