#ifndef __COMMON_TYPE_SYMTAB_SYMBOLTABLE_H__
#define __COMMON_TYPE_SYMTAB_SYMBOLTABLE_H__

#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <common/type/type_defs.h>

namespace Symbol
{
    class Entry
    {
        friend class EntryDeleter;

      private:
        static std::unordered_map<std::string, Entry*> entryMap;
        static void                                    clear();

      public:
        static Entry* getEntry(std::string name);

      private:
        Entry(std::string name = "NULL");
        std::string name;

      public:
        const std::string& getName() const;
    };

    class EntryDeleter
    {
      private:
        EntryDeleter();
        ~EntryDeleter();

      public:
        EntryDeleter(const EntryDeleter&)                   = delete;
        EntryDeleter&        operator=(const EntryDeleter&) = delete;
        static EntryDeleter& getInstance();
    };

    struct EntryHasher
    {
        std::size_t operator()(const Entry* entry) const;
    };

    struct EntryEqual
    {
        bool operator()(const Entry* lhs, const Entry* rhs) const;
    };

    class Table
    {
      private:
        struct Scope
        {
            std::unordered_map<Entry*, VarAttribute, EntryHasher, EntryEqual> symbolMap;
            Scope*                                                            parent;
            /*
             *  -1  : unknown
             *  0   : global
             *  1   : parameter
             *  >= 2: local
             */
            int scopeLevel;

            Scope(Scope* parent = nullptr);
            ~Scope();
        };

        Scope* currentScope;

      public:
        Table();
        ~Table();

        void setAsGlobal();

        /*
         * 0: success
         * 1: exist in current scope
         * 2: exist in param list
         */
        template <typename T>
        int           addSymbol(Entry* entry, T&& attribute);
        VarAttribute* getSymbol(Entry* entry);
        Type*         getSymType(Entry* entry);

        bool enterScope();
        bool exitScope();
    };
}  // namespace Symbol

#include <common/type/symtab/symbol_table.tpp>

#endif