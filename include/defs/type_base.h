#ifndef __TYPE_BASE_H__
#define __TYPE_BASE_H__

#include <string>
#include <vector>

#define VarTypes      \
    X(BEGIN_CTRL, BC) \
    X(INT, Int)       \
    X(FLOAT, Float)   \
    X(VOID, Void)     \
    X(PTR, Ptr)       \
    X(FUNC, Func)     \
    X(END_CTRL, EC)

/**
 * @brief 表示基础类型的抽象基类
 * 
 * 定义了一个虚函数 `typeStr()`，用于返回类型的字符串表示
 * 提供基本的类型检查功能
 */
class Type
{
  private:
    int type;  // 用于存储类型的内部表示

  protected:
    enum VarType
    {
#define X(a, b) a,
        VarTypes
#undef X
    };

  public:
    /**
     * @brief 构造函数，初始化类型
     * 
     * @param type 类型标识
     */
    Type(int type);

    /**
     * @brief 虚析构函数
     */
    virtual ~Type();

    /**
     * @brief 返回类型的字符串表示
     * 
     * @return std::string 类型的字符串表示
     */
    virtual std::string typeStr() const;

    /**
     * @brief 检查类型是否有效
     * 
     * @return true 有效类型
     * @return false 无效类型
     */
    bool isValidType() const;

#define X(a, b) \
    /** @brief 检查是否为指定的类型 */ \
    bool is##b() const;
    VarTypes
#undef X
};

/**
 * @brief 表示整数类型的类
 * 
 * 继承自 `Type`，增加了对整数大小的支持
 */
class IntType : public Type
{
  private:
    unsigned int size;  // 整数的大小，单位为bit

  public:
    /**
     * @brief 构造函数，初始化整数类型
     * 
     * @param s 整数的大小
     */
    IntType(unsigned int s);

    /**
     * @brief 返回整数类型的字符串表示
     * 
     * @return std::string 整数类型的字符串表示
     */
    std::string typeStr() const override;
};

/**
 * @brief 表示浮点类型的类
 * 
 * 继承自 `Type`，增加了对浮点数大小的支持
 */
class FloatType : public Type
{
  private:
    unsigned int size;  // 浮点数的大小，单位为bit

  public:
    /**
     * @brief 构造函数，初始化浮点类型
     * 
     * @param s 浮点数的大小
     */
    FloatType(unsigned int s);

    /**
     * @brief 返回浮点类型的字符串表示
     * 
     * @return std::string 浮点类型的字符串表示
     */
    std::string typeStr() const override;
};

/**
 * @brief 表示空类型的类
 * 
 * 继承自 `Type`，表示void类型
 */
class VoidType : public Type
{
  public:
    /**
     * @brief 构造函数，初始化void类型
     */
    VoidType();
};

/**
 * @brief 表示指针类型的类
 * 
 * 继承自 `Type`，表示指向某一类型的指针
 */
class PtrType : public Type
{
  private:
    Type* baseType;  // 指针所指向的基础类型

  public:
    /**
     * @brief 构造函数，初始化指针类型
     * 
     * @param baseType 指针指向的基础类型
     */
    PtrType(Type* baseType);

    /**
     * @brief 析构函数
     */
    ~PtrType();
};

/**
 * @brief 表示函数类型的类
 * 
 * 继承自 `Type`，用于描述具有返回类型和参数类型的函数
 */
class FuncType : public Type
{
  private:
    Type*              retType;    // 函数的返回类型
    std::vector<Type*> paramTypes; // 函数的参数类型列表

  public:
    /**
     * @brief 构造函数，初始化函数类型
     * 
     * @param returnType 函数的返回类型
     * @param paramTypes 函数的参数类型列表
     */
    FuncType(Type* returnType, std::vector<Type*> paramTypes);

    /**
     * @brief 析构函数
     */
    ~FuncType();

    /**
     * @brief 返回函数类型的字符串表示
     * 
     * @return std::string 函数类型的字符串表示
     */
    std::string typeStr() const override;
};

#endif