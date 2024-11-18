template <typename T>
int Symbol::Table::addSymbol(Entry* entry, T&& attribute)
{
    if (currentScope->symbolMap.find(entry) != currentScope->symbolMap.end()) return 1;
    currentScope->symbolMap.emplace(entry, std::forward<T>(attribute));
    return 0;
}