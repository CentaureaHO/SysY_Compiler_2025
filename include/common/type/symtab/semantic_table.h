#ifndef __COMMON_TYPE_SYMTAB_SEMANTICTABLE_H__
#define __COMMON_TYPE_SYMTAB_SEMANTICTABLE_H__

#include <map>
#include <vector>
#include <common/type/symtab/symbol_table.h>
#include <common/type/type_defs.h>

class FuncDeclStmt;

namespace SemanticTable
{
    class Table
    {
      public:
        Symbol::Table                                 symTable;
        std::map<const Symbol::Entry*, VarAttribute>  glbSymMap;
        std::map<const Symbol::Entry*, FuncDeclStmt*> funcDeclMap;

      public:
        Table();
        ~Table();

        void reg_funcs();
    };
}  // namespace SemanticTable

extern SemanticTable::Table* semTable;

#endif