#include <common/type/symtab/symbol_table.h>
using namespace std;
using namespace Symbol;

void Entry::clear() {}

Entry* Entry::getEntry(string name)
{
    static std::map<std::string, std::unique_ptr<Entry>> entryMap;
    if (entryMap.find(name) == entryMap.end()) entryMap[name] = std::make_unique<Entry>(name);
    return entryMap[name].get();
}

Entry::Entry(string name) : name(name) {}

const string& Entry::getName() const { return name; }

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

Table::Scope::Scope(Scope* parent) : parent(parent), scopeLevel(parent ? parent->scopeLevel + 1 : 0) {}
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

Type* Table::getSymType(Entry* entry)
{
    VarAttribute* attr = getSymbol(entry);
    return attr ? attr->getType() : nullptr;
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
    return true;
}