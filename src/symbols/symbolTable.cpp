#include <symbols/symbolTable.h>
#include <sstream>
using namespace std;

Value::Value(Type* type, string val) : type(type)
{
    if (type->isInt())
        num.intVal = stoi(val);
    else if (type->isFloat())
        num.floatVal = stof(val);
    else
        num.intVal = val[0];
}

bool Value::isInt() const { return type->isInt(); }
bool Value::isFloat() const { return type->isFloat(); }

int   Value::getInt() const { return num.intVal; }
float Value::getFloat() const { return num.floatVal; }
int   Value::getChar() const { return num.intVal; }
int   Value::getBool() const { return num.intVal; }

SymbolTableEntry::SymbolTableEntry(int syType, Type* varType)
    : syType(syType), varType(varType)
{}

SymbolTableEntry::~SymbolTableEntry() {}

Type* SymbolTableEntry::getType() const { return varType; }
void  SymbolTableEntry::setType(Type* type) { varType = type; }

Value* SymbolTableEntry::getValue() const { return nullptr; }

bool SymbolTableEntry::isValidType() const { return syType >= SymbolType::CONSTANT && syType <= SymbolType::FUNCTION; }

#define X(a, b) \
    bool SymbolTableEntry::is##b() const { return syType == SymbolType::a; }
SymbolTypes
#undef X

    ConstantEntry::ConstantEntry(Type* type, Value* value)
    : SymbolTableEntry(SymbolType::CONSTANT, type), value(value)
{}

ConstantEntry::~ConstantEntry() { delete value; }

Value* ConstantEntry::getValue() const { return value; }

string ConstantEntry::toString() const
{
    if (value->isInt())
        return "Int: " + to_string(value->getInt());
    else if (value->isFloat())
        return "Float: " + to_string(value->getFloat());
    return "Char: " + string(1, value->getChar());
}

IdentifierEntry::IdentifierEntry(Type* type, string name, unsigned int scope)
    : SymbolTableEntry(SymbolType::VARIABLE, type), name(name), scope(scope)
{}

bool IdentifierEntry::isGlobal() const { return scope == VariableType::GLOBAL; }
bool IdentifierEntry::isParam() const { return scope == VariableType::PARAM; }
bool IdentifierEntry::isLocal() const { return scope >= VariableType::LOCAL; }

int IdentifierEntry::getScope() const { return scope; }

string IdentifierEntry::toString() const
{
    stringstream ss;
    if (isGlobal())
        ss << "Global ";
    else if (isParam())
        ss << "Param ";
    else
        ss << "Local ";
    ss << name << ": " << this->getType()->typeStr();
    return ss.str();
}

FunctionEntry::FunctionEntry(FuncType* type, string name) : SymbolTableEntry(SymbolType::FUNCTION, type), name(name) {}

FunctionEntry::~FunctionEntry() { delete varType; }

string FunctionEntry::toString() const
{
    stringstream ss;
    ss << name << this->getType()->typeStr();
    return ss.str();
}

SymbolTable::Scope::Scope(Scope* pa) : parent(pa) { scopeLevel = parent ? parent->scopeLevel + 1 : 0; }

SymbolTable::Scope::~Scope()
{
    for (auto& entry : entries) { delete entry.second; }
}

SymbolTable::SymbolTable() { currentScope = new Scope(); }

SymbolTable::~SymbolTable()
{
    currentParams.clear();
    while (currentScope != nullptr)
    {
        Scope* temp  = currentScope;
        currentScope = currentScope->parent;
        delete temp;
    }
}

string SymbolTable::getConstantName(const Type* type)
{
    return "__anon_" + type->typeStr() + "_Const_" + to_string(count++) + "__";
}

string SymbolTable::getFuncname(const string& name, const vector<Type*>& paramTypes)
{
    stringstream ss;
    ss << name << "(";
    for (auto i = 0U; i < paramTypes.size(); i++)
    {
        ss << paramTypes[i]->typeStr();
        if (i != paramTypes.size() - 1) { ss << ", "; }
    }
    ss << ")";
    return ss.str();
}

void SymbolTable::enterScope() { currentScope = new Scope(currentScope); }

void SymbolTable::exitScope()
{
    if (currentScope->parent != nullptr)
    {
        Scope* temp  = currentScope;
        currentScope = currentScope->parent;
        delete temp;

        if (currentScope->scopeLevel <= 1) { currentParams.clear(); }
    }
}

void SymbolTable::addEntry(const string& name, SymbolTableEntry* entry)
{
    auto it = currentScope->entries.find(name);
    if (it != currentScope->entries.end())
    {
        stringstream ss;
        ss << "Redeclaration of " << name;
        if (!entry->getType()->isFunc())
        {
            ss << ": " << entry->getType()->typeStr() << " with previous " << name << ": "
               << it->second->getType()->typeStr();
        }
        throw runtime_error(ss.str());
    }
    auto it2 = currentParams.find(name);
    if (it2 != currentParams.end())
    {
        throw runtime_error("Redeclaration of " + name + ": " + entry->getType()->typeStr() + " as parameter");
    }
    currentScope->entries[name] = entry;
    if (currentScope->scopeLevel == 1) { currentParams.insert(name); }
}

string SymbolTable::addConstant(Type* type, string value)
{
    auto it = anonToSymTempMap.find(value);
    if (it != anonToSymTempMap.end()) return it->second;
    string constantName     = getConstantName(type);
    anonToSymTempMap[value] = constantName;
    addEntry(constantName, new ConstantEntry(type, new Value(type, value)));
    return constantName;
}

void SymbolTable::addConstant(const string& name, Type* type, string value)
{
    string ConstantName = (name == "") ? getConstantName(type) : name;
    addEntry(ConstantName, new ConstantEntry(type, new Value(type, value)));
}

void SymbolTable::addVariable(const string& name, Type* type)
{
    addEntry(name, new IdentifierEntry(type, name, currentScope->scopeLevel));
}

void SymbolTable::addFunction(const string& name, Type* retType, vector<Type*> paramType)
{
    if (currentScope->scopeLevel != 0) { throw runtime_error("Function declaration must be in global scope"); }
    string funcName = getFuncname(name, paramType);
    addEntry(funcName, new FunctionEntry(new FuncType(retType, paramType), name));
}

SymbolTableEntry* SymbolTable::findEntry(const string& name)
{
    Scope* scope = currentScope;
    while (scope != nullptr)
    {
        auto it = scope->entries.find(name);
        if (it != scope->entries.end()) { return it->second; }
        scope = scope->parent;
    }
    return nullptr;
}

int SymbolTable::getScopeLevel() const { return currentScope->scopeLevel; }

SymbolTable  globalSymbolTable;
SymbolTable* globalTable = &globalSymbolTable;
SymbolTable* scopeTable  = globalTable;

unordered_map<string, string> anonToSymTempMap;

int SymbolTable::count = 0;