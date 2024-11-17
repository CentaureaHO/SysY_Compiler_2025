#ifndef __COMMON_TYPE_SYMTAB_SEMANTICTABLE_H__
#define __COMMON_TYPE_SYMTAB_SEMANTICTABLE_H__

#include <unordered_map>
#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <common/type/type_defs.h>

class FuncDeclStmt;

namespace Semantic
{
    class Table
    {
      public:
        Symbol::Table                                                                                    symTable;
        std::unordered_map<const Symbol::Entry*, VarAttribute, Symbol::EntryHasher, Symbol::EntryEqual>  glbSymMap;
        std::unordered_map<const Symbol::Entry*, FuncDeclStmt*, Symbol::EntryHasher, Symbol::EntryEqual> funcDeclMap;

      public:
        Table();
        ~Table();
    };
}  // namespace Semantic

#endif