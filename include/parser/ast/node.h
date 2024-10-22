#ifndef __PARSER_AST_NODE_H__
#define __PARSER_AST_NODE_H__

#include <variant>
#include <memory>
#include <parser/type_defs.h>
#include <parser/ast/symbol_table.h>

namespace AST
{
    class Node
    {
      private:
        std::shared_ptr<NodeAttribute> attribute;

      protected:
        unsigned int line_num;
    };

    
}  // namespace AST

#endif