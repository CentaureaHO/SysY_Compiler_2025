#include <parser/type_defs.h>
using namespace std;

namespace
{
    vector<string> typeStr = {
#define X(name, type, id) #name,
        VALUE_TYPES
#undef X
    };
}  // namespace

/* Definition of Type: head */

bool Type::isInt() const { return getKind() == TypeKind::INT; }
bool Type::isLL() const { return getKind() == TypeKind::LL; }
bool Type::isFloat() const { return getKind() == TypeKind::FLOAT; }
bool Type::isDouble() const { return getKind() == TypeKind::DOUBLE; }
bool Type::isBool() const { return getKind() == TypeKind::BOOL; }
bool Type::isPointer() const { return getKind() == TypeKind::PTR; }
bool Type::isArray() const { return getKind() == TypeKind::ARR; }
bool Type::isFunction() const { return getKind() == TypeKind::FUNC; }

/* Definition of Type: tail */
/* Definition of BasicType: head */

BasicType::BasicType(TypeKind kind) : kind(kind) {}

TypeKind BasicType::getKind() const { return kind; }
string   BasicType::getTypeName() const { return typeStr[static_cast<size_t>(kind)]; }

/* Definition of BasicType: tail */
/* Definition of PointerType: head */

PointerType::PointerType(Type* baseType) : baseType(baseType) {}

TypeKind PointerType::getKind() const { return TypeKind::PTR; }
string   PointerType::getTypeName() const
{
    if (!baseType) return "VOID*";
    return baseType->getTypeName() + "*";
}

/* Definition of PointerType: tail */
/* Definition of ArrayType: head */

ArrayType::ArrayType(Type* baseType, size_t size) : baseType(baseType), size(size) {}

TypeKind ArrayType::getKind() const { return TypeKind::ARR; }
string   ArrayType::getTypeName() const
{
    if (!baseType) return "VOID[]";
    return baseType->getTypeName() + "[" + to_string(size) + "]";
}

/* Definition of ArrayType: tail */
/* Definition of FunctionType: head */

FunctionType::FunctionType(Type* returnType, vector<Type*> paramTypes)
    : returnType(returnType), paramTypes(std::move(paramTypes))
{}

TypeKind FunctionType::getKind() const { return TypeKind::FUNC; }
string   FunctionType::getTypeName() const
{
    string res = returnType->getTypeName() + "(";
    for (size_t i = 0; i < paramTypes.size(); i++)
    {
        res += paramTypes[i]->getTypeName();
        if (i != paramTypes.size() - 1) res += ", ";
    }
    res += ")";
    return res;
}

/* Definition of FunctionType: tail */
/* Definition of TypeSystem: head */

unordered_map<size_t, Type*> TypeSystem::typeMap;

size_t TypeSystem::generateHash(TypeKind kind) { return std::hash<int>{}(static_cast<int>(kind)); }
size_t TypeSystem::generateHash(TypeKind kind, Type* baseType)
{
    return std::hash<int>{}(static_cast<int>(kind)) ^ std::hash<Type*>{}(baseType);
}
size_t TypeSystem::generateHash(TypeKind kind, Type* returnType, const std::vector<Type*>& paramTypes)
{
    size_t hash = std::hash<int>{}(static_cast<int>(kind)) ^ std::hash<Type*>{}(returnType);
    for (auto param : paramTypes) { hash ^= std::hash<Type*>{}(param); }
    return hash;
}

Type* TypeSystem::getBasicType(TypeKind kind)
{
    size_t hash = generateHash(kind);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new BasicType(kind);
    return typeMap[hash];
}

Type* TypeSystem::getPointerType(Type* baseType)
{
    size_t hash = generateHash(TypeKind::PTR, baseType);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new PointerType(baseType);
    return typeMap[hash];
}

Type* TypeSystem::getArrayType(Type* baseType, size_t size)
{
    size_t hash = generateHash(TypeKind::ARR, baseType);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new ArrayType(baseType, size);
    return typeMap[hash];
}

Type* TypeSystem::getFunctionType(Type* returnType, const vector<Type*>& paramTypes)
{
    size_t hash = generateHash(TypeKind::FUNC, returnType, paramTypes);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new FunctionType(returnType, paramTypes);
    return typeMap[hash];
}

void TypeSystem::clear()
{
    for (auto& [key, type] : typeMap) delete type;
    typeMap.clear();
}

Type* voidType   = TypeSystem::getBasicType(TypeKind::VOID);
Type* intType    = TypeSystem::getBasicType(TypeKind::INT);
Type* llType     = TypeSystem::getBasicType(TypeKind::LL);
Type* floatType  = TypeSystem::getBasicType(TypeKind::FLOAT);
Type* doubleType = TypeSystem::getBasicType(TypeKind::DOUBLE);
Type* boolType   = TypeSystem::getBasicType(TypeKind::BOOL);

/* Definition of TypeSystem: tail */