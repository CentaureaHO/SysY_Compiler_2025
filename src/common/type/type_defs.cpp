#include <common/type/type_defs.h>
#include <iostream>
using namespace std;

namespace
{
    vector<string> typeStr = {
#define X(name, type, id) #name,
        VALUE_TYPES
#undef X
    };

    vector<string> opStr = {
#define X(name, op, id) #name,
        OPERATOR_TYPES
#undef X
    };
}  // namespace

const string& getOpStr(OpCode op) { return opStr[SIZE_T(op)]; }

/* Definition of Type: head */

bool Type::isInt() const { return getKind() == TypeKind::Int; }
bool Type::isLL() const { return getKind() == TypeKind::LL; }
bool Type::isFloat() const { return getKind() == TypeKind::Float; }
bool Type::isDouble() const { return getKind() == TypeKind::Double; }
bool Type::isBool() const { return getKind() == TypeKind::Bool; }
bool Type::isPointer() const { return getKind() == TypeKind::Ptr; }
bool Type::isArray() const { return getKind() == TypeKind::Arr; }
bool Type::isFunction() const { return getKind() == TypeKind::Func; }

/* Definition of Type: tail */
/* Definition of BasicType: head */

BasicType::BasicType(TypeKind kind) : kind(kind) {}

TypeKind BasicType::getKind() const { return kind; }
string   BasicType::getTypeName() const { return typeStr[static_cast<size_t>(kind)]; }

/* Definition of BasicType: tail */
/* Definition of PointerType: head */

PointerType::PointerType(Type* baseType) : baseType(baseType) {}

TypeKind PointerType::getKind() const { return TypeKind::Ptr; }
string   PointerType::getTypeName() const
{
    if (!baseType) return "VOID*";
    return baseType->getTypeName() + "*";
}

/* Definition of PointerType: tail */
/* Definition of ArrayType: head */

ArrayType::ArrayType(Type* baseType, size_t size) : baseType(baseType), size(size) {}

TypeKind ArrayType::getKind() const { return TypeKind::Arr; }
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

TypeKind FunctionType::getKind() const { return TypeKind::Func; }
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

TypeSystem::TypeSystem() {}
TypeSystem::~TypeSystem() { clear(); }

std::map<size_t, Type*> TypeSystem::typeMap;

size_t TypeSystem::generateHash(TypeKind kind) { return std::hash<int>{}(static_cast<int>(kind)); }
size_t TypeSystem::generateHash(TypeKind kind, Type* baseType)
{
    // 仍使用 std::hash 组合两个值生成键
    return std::hash<int>{}(static_cast<int>(kind)) ^ reinterpret_cast<size_t>(baseType);
}
size_t TypeSystem::generateHash(TypeKind kind, Type* returnType, const std::vector<Type*>& paramTypes)
{
    size_t hash = std::hash<int>{}(static_cast<int>(kind)) ^ reinterpret_cast<size_t>(returnType);
    for (auto param : paramTypes) { hash ^= reinterpret_cast<size_t>(param); }
    return hash;
}

Type* TypeSystem::getBasicType(TypeKind kind)
{
    static TypeSystem& ensure = TypeSystem::getInstance();
    size_t hash = generateHash(kind);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new BasicType(kind);
    return typeMap[hash];
}

Type* TypeSystem::getPointerType(Type* baseType)
{
    static TypeSystem& ensure = TypeSystem::getInstance();
    size_t hash = generateHash(TypeKind::Ptr, baseType);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new PointerType(baseType);
    return typeMap[hash];
}

Type* TypeSystem::getArrayType(Type* baseType, size_t size)
{
    static TypeSystem& ensure = TypeSystem::getInstance();
    size_t hash = generateHash(TypeKind::Arr, baseType);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new ArrayType(baseType, size);
    return typeMap[hash];
}

Type* TypeSystem::getFunctionType(Type* returnType, const std::vector<Type*>& paramTypes)
{
    static TypeSystem& ensure = TypeSystem::getInstance();
    size_t hash = generateHash(TypeKind::Func, returnType, paramTypes);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new FunctionType(returnType, paramTypes);
    return typeMap[hash];
}

void TypeSystem::clear()
{
    for (auto& [key, type] : typeMap) delete type;
    typeMap.clear();
}

TypeSystem& TypeSystem::getInstance()
{
    static TypeSystem instance;
    return instance;
}

namespace
{
    TypeSystem& typeSystemDeleter = TypeSystem::getInstance();
}

Type* voidType   = TypeSystem::getBasicType(TypeKind::Void);
Type* intType    = TypeSystem::getBasicType(TypeKind::Int);
Type* llType     = TypeSystem::getBasicType(TypeKind::LL);
Type* floatType  = TypeSystem::getBasicType(TypeKind::Float);
Type* doubleType = TypeSystem::getBasicType(TypeKind::Double);
Type* boolType   = TypeSystem::getBasicType(TypeKind::Bool);
Type* strType    = TypeSystem::getBasicType(TypeKind::Str);

/* Definition of TypeSystem: tail */
/* Definition of ConstValue: head */

ConstValue::ConstValue() : type(voidType), value(0), isConst(false) {}
ConstValue::ConstValue(int val) : type(intType), value(val), isConst(false) {}
ConstValue::ConstValue(long long val) : type(llType), value(val), isConst(false) {}
ConstValue::ConstValue(float val) : type(floatType), value(val), isConst(false) {}
ConstValue::ConstValue(double val) : type(doubleType), value(val), isConst(false) {}
ConstValue::ConstValue(bool val) : type(boolType), value(val), isConst(false) {}
ConstValue::ConstValue(shared_ptr<string> val) : type(strType), value(val), isConst(false) {}

/* Definition of ConstValue: tail */
/* Definition of VarAttribute: head */

VarAttribute::VarAttribute(Type* type, bool isConst)
    : type(type), isConst(isConst), scope(0), dims(vector<int>{}), initVals()
{}

Type* VarAttribute::getType() const { return type; }

/* Definition of VarAttribute: tail */
/* Definition of NodeAttribute: head */

NodeAttribute::NodeAttribute(OpCode op, ConstValue v, unsigned int line_num) : op(op), val(v), line_num(line_num) {}

OpCode&     NodeAttribute::getOp() { return op; }
ConstValue& NodeAttribute::getVal() { return val; }

/* Definition of NodeAttribute: tail */

bool safe_cast_to_bool(const std::variant<int, long long, float, double, bool, std::shared_ptr<std::string>>& value)
{
    return std::visit(
        [](auto&& value) -> bool {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, bool>)
                return static_cast<bool>(value);
            else if constexpr (std::is_same_v<T, int>)
                return static_cast<bool>(value);
            else if constexpr (std::is_same_v<T, long long>)
                return static_cast<bool>(value);
            else if constexpr (std::is_same_v<T, float>)
                return static_cast<bool>(value);
            else
            {
                std::cerr << "Unexpected type: " << typeid(T).name() << '\n';
                throw std::bad_variant_access();
            }
        },
        value);
}

int safe_cast_to_int(const std::variant<int, long long, float, double, bool, std::shared_ptr<std::string>>& value)
{
    return std::visit(
        [](auto&& value) -> int {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, bool>)
                return static_cast<int>(value);
            else if constexpr (std::is_same_v<T, int>)
                return static_cast<int>(value);
            else if constexpr (std::is_same_v<T, long long>)
                return static_cast<int>(value);
            else if constexpr (std::is_same_v<T, float>)
                return static_cast<int>(value);
            else
            {
                std::cerr << "Unexpected type: " << typeid(T).name() << '\n';
                throw std::bad_variant_access();
            }
        },
        value);
}

long long safe_cast_to_ll(const std::variant<int, long long, float, double, bool, std::shared_ptr<std::string>>& value)
{
    return std::visit(
        [](auto&& value) -> long long {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, bool>)
                return static_cast<long long>(value);
            else if constexpr (std::is_same_v<T, int>)
                return static_cast<long long>(value);
            else if constexpr (std::is_same_v<T, long long>)
                return static_cast<long long>(value);
            else if constexpr (std::is_same_v<T, float>)
                return static_cast<long long>(value);
            else
            {
                std::cerr << "Unexpected type: " << typeid(T).name() << '\n';
                throw std::bad_variant_access();
            }
        },
        value);
}

float safe_cast_to_float(const std::variant<int, long long, float, double, bool, std::shared_ptr<std::string>>& value)
{
    return std::visit(
        [](auto&& value) -> float {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, bool>)
                return static_cast<float>(value);
            else if constexpr (std::is_same_v<T, int>)
                return static_cast<float>(value);
            else if constexpr (std::is_same_v<T, long long>)
                return static_cast<float>(value);
            else if constexpr (std::is_same_v<T, float>)
                return static_cast<float>(value);
            else
            {
                std::cerr << "Unexpected type: " << typeid(T).name() << '\n';
                throw std::bad_variant_access();
            }
        },
        value);
}