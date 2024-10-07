#include <defs/type_base.h>
#include <sstream>
using namespace std;

Type::Type(int type) : type(type) {}
Type::~Type() {}

string Type::typeStr() const
{
    switch (type)
    {
#define X(a, b) \
    case VarType::a: return #b;
        VarTypes
#undef X
            default : return "Unknown";
    }
}

bool Type::isValidType() const { return type > VarType::BEGIN_CTRL && type < VarType::END_CTRL; }

#define X(a, b) \
    bool Type::is##b() const { return type == VarType::a; }
VarTypes
#undef X

    IntType::IntType(unsigned int s)
    : Type(Type::INT), size(s)
{}

string IntType::typeStr() const { return "i" + to_string(size); }

FloatType::FloatType(unsigned int s) : Type(Type::FLOAT), size(s) {}

string FloatType::typeStr() const { return "f" + to_string(size); }

VoidType::VoidType() : Type(Type::VOID) {}

PtrType::PtrType(Type* baseType) : Type(Type::PTR), baseType(baseType) {}

PtrType::~PtrType() {}

FuncType::FuncType(Type* returnType, vector<Type*> paramTypes)
    : Type(Type::FUNC), retType(returnType), paramTypes(paramTypes)
{}

FuncType::~FuncType() {}

string FuncType::typeStr() const
{
    string str = "(";
    for (auto i = 0u; i < paramTypes.size(); i++)
    {
        str += paramTypes[i]->typeStr();
        if (i != paramTypes.size() - 1) str += ", ";
    }
    str += ") -> " + retType->typeStr();
    return str;
}