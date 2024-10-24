#include <common/type/type_defs.h>
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

const string& getOpStr(OpCode op) { return opStr[static_cast<size_t>(op)]; }

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
    size_t hash = generateHash(TypeKind::Ptr, baseType);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new PointerType(baseType);
    return typeMap[hash];
}
Type* TypeSystem::getArrayType(Type* baseType, size_t size)
{
    size_t hash = generateHash(TypeKind::Arr, baseType);
    if (typeMap.find(hash) == typeMap.end()) typeMap[hash] = new ArrayType(baseType, size);
    return typeMap[hash];
}
Type* TypeSystem::getFunctionType(Type* returnType, const vector<Type*>& paramTypes)
{
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
/* Definition of LiteralValue: head */

LiteralValue::LiteralValue() : type(voidType), value(0) {}
LiteralValue::LiteralValue(int val) : type(intType), value(val) {}
LiteralValue::LiteralValue(long long val) : type(llType), value(val) {}
LiteralValue::LiteralValue(float val) : type(floatType), value(val) {}
LiteralValue::LiteralValue(double val) : type(doubleType), value(val) {}
LiteralValue::LiteralValue(bool val) : type(boolType), value(val) {}
LiteralValue::LiteralValue(string val) : type(strType), value(val) {}

string LiteralValue::getLiteralInfo()
{
    if (type == voidType) return "Literal undefined";

    return "Literal " + type->getTypeName() + ": " +
           visit(
               [](auto&& arg) -> string {
                   if constexpr (is_same_v<decay_t<decltype(arg)>, string>)
                       return arg;
                   else
                       return to_string(arg);
               },
               value);
}

/* Definition of LiteralValue: tail */
/* Definition of VarAttribute: head */

VarAttribute::VarAttribute(Type* type, bool isConst)
    : type(type), isConstVar(isConst), dims(vector<int>{}), initInts(vector<int>{}), initFloats(vector<float>{})
{}

Type* VarAttribute::getType() const { return type; }

/* Definition of VarAttribute: tail */
/* Definition of NodeAttribute: head */

NodeAttribute::NodeAttribute(OpCode op, std::shared_ptr<LiteralValue> literal, unsigned int line_num)
    : op(op), literal(literal), line_num(line_num)
{}

string NodeAttribute::getAttributeInfo()
{
    string res;
    if (op != OpCode::PlaceHolder) res = "Operator: " + opStr[static_cast<size_t>(op)] + " ";
    if (literal) res = literal->getLiteralInfo();
    return res;
}

/* Definition of NodeAttribute: tail */