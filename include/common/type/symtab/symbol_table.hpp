template <typename T>
int Symbol::Table::addSymbol(Entry* entry, T&& attribute)
{
    if (paramEntrys.find(entry) != paramEntrys.end()) return 2;
    if (currentScope->symbolMap.find(entry) != currentScope->symbolMap.end()) return 1;
    currentScope->symbolMap.emplace(entry, std::forward<T>(attribute));
    if (currentScope->scopeLevel == 1) paramEntrys.insert(entry);
    return 0;
}