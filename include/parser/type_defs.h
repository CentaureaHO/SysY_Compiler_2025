#ifndef __PARSER_TYPE_DEFS_H__
#define __PARSER_TYPE_DEFS_H__

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#define VALUE_TYPES            \
    X(VOID, std::monostate, 0) \
    X(INT, int, 1)             \
    X(LL, long long, 2)        \
    X(FLOAT, float, 3)         \
    X(DOUBLE, double, 4)       \
    X(BOOL, bool, 5)           \
    X(PTR, void*, 6)           \
    X(ARR, void*, 7)           \
    X(FUNC, void*, 8)          \
    X(CSTR, const char*, 9)    \
    X(CHAR, char, 10)

enum class TypeKind
{
#define X(name, type, id) name = id,
    VALUE_TYPES
#undef X
};

class Type
{
  public:
    virtual TypeKind    getKind() const     = 0;
    virtual std::string getTypeName() const = 0;
    virtual ~Type()                         = default;

    bool isInt() const;
    bool isLL() const;
    bool isFloat() const;
    bool isDouble() const;
    bool isBool() const;
    bool isPointer() const;
    bool isArray() const;
    bool isFunction() const;
};

class BasicType : public Type
{
  private:
    TypeKind kind;

  public:
    BasicType(TypeKind kind = TypeKind::VOID);

    TypeKind    getKind() const override;
    std::string getTypeName() const override;
};

class PointerType : public Type
{
  private:
    Type* baseType;

  public:
    PointerType(Type* baseType = nullptr);

    TypeKind    getKind() const override;
    std::string getTypeName() const override;
};

class ArrayType : public Type
{
  private:
    Type*  baseType;
    size_t size;

  public:
    ArrayType(Type* baseType = nullptr, size_t size = 0);

    TypeKind    getKind() const override;
    std::string getTypeName() const override;
};

class FunctionType : public Type
{
  private:
    Type*              returnType;
    std::vector<Type*> paramTypes;

  public:
    FunctionType(Type* returnType = nullptr, std::vector<Type*> paramTypes = {});

    TypeKind    getKind() const override;
    std::string getTypeName() const override;
};

class TypeSystem
{
  private:
    static std::unordered_map<size_t, Type*> typeMap;

    static size_t generateHash(TypeKind kind);
    static size_t generateHash(TypeKind kind, Type* baseType);
    static size_t generateHash(TypeKind kind, Type* returnType, const std::vector<Type*>& paramTypes);

  public:
    static Type* getBasicType(TypeKind kind);
    static Type* getPointerType(Type* baseType);
    static Type* getArrayType(Type* baseType, size_t size);
    static Type* getFunctionType(Type* returnType, const std::vector<Type*>& paramTypes);
    static void  clear();
};

extern Type* voidType;
extern Type* intType;
extern Type* llType;
extern Type* floatType;
extern Type* doubleType;
extern Type* boolType;

#endif