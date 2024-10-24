#include <common/type/symtab/symbol_table.h>
using namespace std;
using namespace Symbol;

/* Definition of Symbol::Entry: head */

unordered_map<string, Entry*> Entry::entryMap;

void Entry::clear()
{
    for (auto& [name, entry] : entryMap) delete entry;
    entryMap.clear();
}

Entry* Entry::getEntry(string name)
{
    if (entryMap.find(name) == entryMap.end()) entryMap[name] = new Entry(name);
    return entryMap[name];
}

Entry::Entry(string name) : name(name) {}

const string& Entry::getName() { return name; }

EntryDeleter::EntryDeleter() {}
EntryDeleter::~EntryDeleter() { Entry::clear(); }
EntryDeleter& EntryDeleter::getInstance()
{
    static EntryDeleter instance;
    return instance;
}
namespace
{
    EntryDeleter& instance = EntryDeleter::getInstance();
}

size_t EntryHasher::operator()(Entry* entry) const { return hash<string>()(entry->getName()); }
bool   EntryEqual::operator()(Entry* lhs, Entry* rhs) const { return lhs->getName() == rhs->getName(); }

/* Definition of Symbol::Entry: tail */
/* Definition of Symbol::Table: head */

Table::Scope::Scope(Scope* parent) : parent(parent), scopeLevel(parent ? parent->scopeLevel + 1 : -1) {}
Table::Scope::~Scope() { symbolMap.clear(); }

Table::Table() : currentScope(new Scope(nullptr)) {}
Table::~Table()
{
    Scope* scope = currentScope;
    while (scope)
    {
        Scope* parent = scope->parent;
        delete scope;
        scope = parent;
    }
}

void Table::setAsGlobal() { currentScope->scopeLevel = 0; }

VarAttribute* Table::getSymbol(Entry* entry)
{
    Scope* scope = currentScope;
    while (scope)
    {
        if (scope->symbolMap.find(entry) != scope->symbolMap.end()) return &scope->symbolMap[entry];
        scope = scope->parent;
    }
    return nullptr;
}

bool Table::enterScope()
{
    currentScope = new Scope(currentScope);
    return true;
}
bool Table::exitScope()
{
    if (currentScope == nullptr) return false;
    Scope* parent = currentScope->parent;
    delete currentScope;
    currentScope = parent;
    if (currentScope != nullptr && currentScope->scopeLevel <= 1) paramEntrys.clear();
    return true;
}

/* Definition of Symbol::Table: tail */