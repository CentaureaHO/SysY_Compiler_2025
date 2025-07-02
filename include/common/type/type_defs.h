#ifndef __COMMON_TYPE_TYPEDEFS_H__
#define __COMMON_TYPE_TYPEDEFS_H__

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <map>

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

#define OPERATOR_TYPES             \
    X(PlaceHolder, PlaceHolder, 0) \
    X(Add, +, 1)                   \
    X(Sub, -, 2)                   \
    X(Mul, *, 3)                   \
    X(Div, /, 4)                   \
    X(Gt, >, 5)                    \
    X(Ge, >=, 6)                   \
    X(Lt, <, 7)                    \
    X(Le, <=, 8)                   \
    X(Eq, ==, 9)                   \
    X(Mod, %, 10)                  \
    X(Neq, !=, 11)                 \
    X(Not, !, 12)                  \
    X(BitOr, |, 13)                \
    X(BitAnd, &, 14)               \
    X(And, &&, 15)                 \
    X(Or, ||, 16)                  \
    X(Assign, =, 17)

#define SIZE_T(x) static_cast<size_t>(x)

#define FLOAT_TO_DOUBLE_BITS(f)                            \
    ([](float value) {                                     \
        double             d = static_cast<double>(value); \
        unsigned long long dbytes;                         \
        std::memcpy(&dbytes, &d, sizeof(double));          \
        return static_cast<long long>(dbytes);             \
    }(f))

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

const std::string& getOpStr(OpCode op);

class Type
{
    friend class TypeSystem;

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
    friend class TypeSystem;

  private:
    BasicType(TypeKind kind = TypeKind::Void);
    TypeKind kind;

  public:
    TypeKind    getKind() const override;
    std::string getTypeName() const override;
};

class PointerType : public Type
{
    friend class TypeSystem;

  public:
    PointerType(Type* baseType = nullptr);
    Type* baseType;

  public:
    TypeKind    getKind() const override;
    std::string getTypeName() const override;
};

class ArrayType : public Type
{
    friend class TypeSystem;

  private:
    ArrayType(Type* baseType = nullptr, size_t size = 0);
    Type*  baseType;
    size_t size;

  public:
    TypeKind    getKind() const override;
    std::string getTypeName() const override;
};

class FunctionType : public Type
{
    friend class TypeSystem;

  private:
    FunctionType(Type* returnType = nullptr, std::vector<Type*> paramTypes = {});
    Type*              returnType;
    std::vector<Type*> paramTypes;

  public:
    TypeKind    getKind() const override;
    std::string getTypeName() const override;
};

class TypeSystem
{
  private:
    TypeSystem();
    ~TypeSystem();

    static std::map<size_t, Type*> typeMap;

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

struct VarValue
{
    union
    {
        int                           int_val;
        long long                     ll_val;
        float                         float_val;
        double                        double_val;
        bool                          bool_val;
        std::shared_ptr<std::string>* str_ptr;
    };
    enum class Type
    {
        Int,
        LL,
        Float,
        Double,
        Bool,
        Str
    } type;

    VarValue() : int_val(0), type(Type::Int) {}
    VarValue(int val) : int_val(val), type(Type::Int) {}
    VarValue(long long val) : ll_val(val), type(Type::LL) {}
    VarValue(float val) : float_val(val), type(Type::Float) {}
    VarValue(double val) : double_val(val), type(Type::Double) {}
    VarValue(bool val) : bool_val(val), type(Type::Bool) {}
    VarValue(std::shared_ptr<std::string>* val) : str_ptr(val), type(Type::Str) {}
    VarValue(const VarValue& other) : type(other.type)
    {
        switch (type)
        {
            case Type::Int: int_val = other.int_val; break;
            case Type::LL: ll_val = other.ll_val; break;
            case Type::Float: float_val = other.float_val; break;
            case Type::Double: double_val = other.double_val; break;
            case Type::Bool: bool_val = other.bool_val; break;
            case Type::Str: str_ptr = new std::shared_ptr<std::string>(*other.str_ptr); break;
        }
    }

    VarValue& operator=(const VarValue& other)
    {
        if (this == &other) return *this;

        type = other.type;
        switch (type)
        {
            case Type::Int: int_val = other.int_val; break;
            case Type::LL: ll_val = other.ll_val; break;
            case Type::Float: float_val = other.float_val; break;
            case Type::Double: double_val = other.double_val; break;
            case Type::Bool: bool_val = other.bool_val; break;
            case Type::Str:
                delete str_ptr;     // Free the old pointer
                str_ptr = nullptr;  // Initialize to nullptr
                str_ptr = new std::shared_ptr<std::string>(*other.str_ptr);
                break;
        }
        return *this;
    }
};

#include <cassert>

inline int cast_to_int(const VarValue& var)
{
    switch (var.type)
    {
        case VarValue::Type::Int: return var.int_val;
        case VarValue::Type::LL: return static_cast<int>(var.ll_val);
        case VarValue::Type::Float: return static_cast<int>(var.float_val);
        case VarValue::Type::Double: return static_cast<int>(var.double_val);
        case VarValue::Type::Bool: return static_cast<int>(var.bool_val);
        default: assert(false); return 0;
    }
}

inline long long cast_to_ll(const VarValue& var)
{
    switch (var.type)
    {
        case VarValue::Type::Int: return static_cast<long long>(var.int_val);
        case VarValue::Type::LL: return var.ll_val;
        case VarValue::Type::Float: return static_cast<long long>(var.float_val);
        case VarValue::Type::Double: return static_cast<long long>(var.double_val);
        case VarValue::Type::Bool: return static_cast<long long>(var.bool_val);
        default: assert(false); return 0;
    }
}

inline bool cast_to_bool(const VarValue& var)
{
    switch (var.type)
    {
        case VarValue::Type::Int: return static_cast<bool>(var.int_val);
        case VarValue::Type::LL: return static_cast<bool>(var.ll_val);
        case VarValue::Type::Float: return static_cast<bool>(var.float_val);
        case VarValue::Type::Double: return static_cast<bool>(var.double_val);
        case VarValue::Type::Bool: return var.bool_val;
        default: assert(false); return false;
    }
}

inline double cast_to_double(const VarValue& var)
{
    switch (var.type)
    {
        case VarValue::Type::Int: return static_cast<double>(var.int_val);
        case VarValue::Type::LL: return static_cast<double>(var.ll_val);
        case VarValue::Type::Float: return static_cast<double>(var.float_val);
        case VarValue::Type::Double: return var.double_val;
        case VarValue::Type::Bool: return static_cast<double>(var.bool_val);
        default: assert(false); return 0.0;
    }
}

inline float cast_to_float(const VarValue& var)
{
    switch (var.type)
    {
        case VarValue::Type::Int: return static_cast<float>(var.int_val);
        case VarValue::Type::LL: return static_cast<float>(var.ll_val);
        case VarValue::Type::Float: return var.float_val;
        case VarValue::Type::Double: return static_cast<float>(var.double_val);
        case VarValue::Type::Bool: return static_cast<float>(var.bool_val);
        default: assert(false); return 0.0f;
    }
}

#define TO_INT(x) cast_to_int(x)
#define TO_FLOAT(x) cast_to_float(x)
#define TO_LL(x) cast_to_ll(x)
#define TO_BOOL(x) cast_to_bool(x)
#define TO_DOUBLE(x) cast_to_double(x)

class ConstValue
{
  public:
    Type*    type;
    VarValue value;
    bool     isConst;

  public:
    ConstValue();
    ConstValue(int val);
    ConstValue(long long val);
    ConstValue(float val);
    ConstValue(double val);
    ConstValue(bool val);
    ConstValue(std::shared_ptr<std::string> val);
};

class VarAttribute
{
  public:
    Type* type;
    bool  isConst;
    int   scope;

    std::vector<int>      dims;
    std::vector<VarValue> initVals;

  public:
    VarAttribute(Type* type = voidType, bool isConst = false);

    Type* getType() const;
};

class NodeAttribute
{
  public:
    OpCode     op;
    ConstValue val;

    unsigned int line_num;

  public:
    NodeAttribute(OpCode op = OpCode::PlaceHolder, ConstValue v = ConstValue(), unsigned int line_num = 0);

    OpCode&     getOp();
    ConstValue& getVal();
};

#define IF_IS_POLY(p, T) (dynamic_cast<T*>(p) != nullptr)

#endif