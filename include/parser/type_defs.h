#ifndef __PARSER_TYPE_DEFS_H__
#define __PARSER_TYPE_DEFS_H__

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

#define VALUE_TYPES            \
    X(Void, std::monostate, 0) \
    X(Int, int, 1)             \
    X(LL, long long, 2)        \
    X(Float, float, 3)         \
    X(Double, double, 4)       \
    X(Bool, bool, 5)           \
    X(Ptr, void*, 6)           \
    X(Arr, void*, 7)           \
    X(Func, void*, 8)          \
    X(CStr, const char*, 9)    \
    X(Char, char, 10)          \
    X(Str, std::string, 11)

#define OPERATOR_TYPES              \
    X(PlaceHolder, PlaceHolder, -1) \
    X(Add, +, 0)                    \
    X(Sub, -, 1)                    \
    X(Mul, *, 2)                    \
    X(Div, /, 3)                    \
    X(Gt, >, 4)                     \
    X(Ge, >=, 5)                    \
    X(Lt, <, 6)                     \
    X(Le, <=, 7)                    \
    X(Eq, ==, 8)                    \
    X(Mod, %, 9)                    \
    X(Neq, !=, 10)                  \
    X(Not, !, 11)                   \
    X(BitOr, |, 12)                 \
    X(BitAnd, &, 13)                \
    X(And, &&, 14)                  \
    X(Or, ||, 15)

enum class TypeKind
{
#define X(name, type, id) name = id,
    VALUE_TYPES
#undef X
};

enum class OpCode
{
#define X(name, op, id) name = id,
    OPERATOR_TYPES
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
    BasicType(TypeKind kind = TypeKind::Void);

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
    TypeSystem();
    ~TypeSystem();

    static std::unordered_map<size_t, Type*> typeMap;

    static size_t generateHash(TypeKind kind);
    static size_t generateHash(TypeKind kind, Type* baseType);
    static size_t generateHash(TypeKind kind, Type* returnType, const std::vector<Type*>& paramTypes);

    static void clear();

  public:
    TypeSystem& operator=(const TypeSystem&) = delete;
    TypeSystem(const TypeSystem&)            = delete;

    static TypeSystem& getInstance();

    static Type* getBasicType(TypeKind kind);
    static Type* getPointerType(Type* baseType);
    static Type* getArrayType(Type* baseType, size_t size);
    static Type* getFunctionType(Type* returnType, const std::vector<Type*>& paramTypes);
};

extern Type* voidType;
extern Type* intType;
extern Type* llType;
extern Type* floatType;
extern Type* doubleType;
extern Type* boolType;
extern Type* strType;

class LiteralValue
{
  private:
    Type*                                                          type;
    std::variant<int, long long, float, double, bool, std::string> value;

  public:
    LiteralValue();
    LiteralValue(int val);
    LiteralValue(long long val);
    LiteralValue(float val);
    LiteralValue(double val);
    LiteralValue(bool val);
    LiteralValue(std::string val);

    std::string getLiteralInfo();
};

class VarAttribute
{
  private:
    Type* type;
    bool  isConstVar;

    std::vector<int>   dims;
    std::vector<int>   initInts;
    std::vector<float> initFloats;

  public:
    VarAttribute(Type* type = voidType, bool isConstVar = false);
};

class NodeAttribute
{
  private:
    OpCode                        op;
    std::shared_ptr<LiteralValue> literal;

    unsigned int line_num;

  public:
    NodeAttribute(
        OpCode op = OpCode::PlaceHolder, std::shared_ptr<LiteralValue> literal = nullptr, unsigned int line_num = 0);

    std::string getAttributeInfo();
};

#endif