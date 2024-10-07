#ifndef __TYPE_H__
#define __TYPE_H__

#include <defs/type_base.h>
#include <defs/type_extended.h>

/**
 * @brief 使用别名定义常用的基本类型
 * 
 * 包含了常见的布尔类型、字符类型、整数类型、浮点类型和void类型
 */
using boolType  = IntTypeT<1>;
using charType  = IntTypeT<8>;
using shortType = IntTypeT<16>;
using intType   = IntTypeT<32>;
using longType  = IntTypeT<64>;

using floatType  = FloatTypeT<32>;
using doubleType = FloatTypeT<64>;

using voidType = VoidTypeT;

/**
 * @brief 全局常量类型对象
 * 
 * 用于表示常用类型的全局对象，如void、int、float
 */
extern voidType*  commonVoid;
extern intType*   commonInt;
extern floatType* commonFloat;

#endif