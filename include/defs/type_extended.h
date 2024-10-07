#ifndef __TYPE_EXTENDED_H__
#define __TYPE_EXTENDED_H__

#include <defs/type_base.h>

/**
 * @brief 模板类，表示不同大小的整数类型
 *
 * 根据模板参数 `Size` 表示不同位数的整数类型
 * @tparam Size 整数类型的位数
 */
template <unsigned int Size>
class IntTypeT : public IntType
{
  public:
    /**
     * @brief 构造函数，初始化指定大小的整数类型
     */
    IntTypeT() : IntType(Size) {}

    /**
     * @brief 返回整数类型的字符串表示
     *
     * @return std::string 整数类型的字符串表示
     */
    std::string typeStr() const override
    {
        if constexpr (Size == 1)
            return "bool";
        else if (Size == 8)
            return "char";
        else if (Size == 16)
            return "short";
        else if (Size == 32)
            return "int";
        else if (Size == 64)
            return "long";
        return IntType::typeStr();
    }
};

/**
 * @brief 模板类，表示不同大小的浮点类型
 *
 * 根据模板参数 `Size` 表示不同位数的浮点类型
 * @tparam Size 浮点类型的位数
 */
template <unsigned int Size>
class FloatTypeT : public FloatType
{
  public:
    /**
     * @brief 构造函数，初始化指定大小的浮点类型
     */
    FloatTypeT() : FloatType(Size) {}

    /**
     * @brief 返回浮点类型的字符串表示
     *
     * @return std::string 浮点类型的字符串表示
     */
    std::string typeStr() const override
    {
        if constexpr (Size == 32)
            return "float";
        else if (Size == 64)
            return "double";
        return FloatType::typeStr();
    }
};

/**
 * @brief 表示void类型的类
 *
 * 继承自 `VoidType`，用于描述void类型
 */
class VoidTypeT : public VoidType
{
  public:
    /**
     * @brief 构造函数，初始化void类型
     */
    VoidTypeT() : VoidType() {}

    /**
     * @brief 返回void类型的字符串表示
     *
     * @return std::string void类型的字符串表示
     */
    std::string typeStr() const override { return "void"; }
};

#endif