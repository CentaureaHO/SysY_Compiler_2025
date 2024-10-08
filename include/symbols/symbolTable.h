#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <defs/type.h>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <sstream>
#include <iostream>

/**
 * @brief 表示符号表中存储的值对象
 * 
 * 包含整数或浮点数值，并提供相关的类型检查和取值操作
 */
class Value
{
  private:
    union
    {
        int   intVal;    ///< 存储整数值
        float floatVal;  ///< 存储浮点值
    } num;
    Type* type;          ///< 值的类型

  public:
    /**
     * @brief 构造函数，初始化值
     * 
     * @param type 值的类型
     * @param val 字符串形式的值
     */
    Value(Type* type, std::string val);

    /**
     * @brief 判断值是否为整数类型
     * 
     * @return true 是整数类型
     * @return false 不是整数类型
     */
    bool isInt() const;

    /**
     * @brief 判断值是否为浮点类型
     * 
     * @return true 是浮点类型
     * @return false 不是浮点类型
     */
    bool isFloat() const;

    /**
     * @brief 获取整数值
     * 
     * @return int 整数值
     */
    int getInt() const;

    /**
     * @brief 获取浮点值
     * 
     * @return float 浮点值
     */
    float getFloat() const;

    /**
     * @brief 获取字符值
     * 
     * 字符值被存储为整数，转换时返回字符的 ASCII 值
     * @return int 字符的 ASCII 值
     */
    int getChar() const;

    /**
     * @brief 获取布尔值
     * 
     * 布尔值被存储为整数类型
     * @return int 布尔值（0 或 1）
     */
    int getBool() const;
};

/**
 * @brief 符号表中的符号类型枚举
 */
#define SymbolTypes       \
    X(CONSTANT, Constant) \
    X(VARIABLE, Variable) \
    X(FUNCTION, Function)

/**
 * @brief 变量的作用域类型枚举
 */
#define VariableTypes \
    X(GLOBAL, Global) \
    X(PARAM, Param)   \
    X(LOCAL, Local)

/**
 * @brief 表示符号表中的一个条目
 * 
 * 符号表条目可以表示常量、变量或函数
 */
class SymbolTableEntry
{
  private:
    int syType;  ///< 符号的类型

  protected:
    Type* varType; ///< 符号的类型（Type 类）
    enum SymbolType
    {
#define X(a, b) a,
        SymbolTypes
#undef X
    };

  public:
    /**
     * @brief 构造函数，初始化符号表条目
     * 
     * @param syType 符号类型
     * @param varType 符号的类型
     */
    SymbolTableEntry(int syType, Type* varType);

    /**
     * @brief 虚析构函数
     */
    virtual ~SymbolTableEntry();

    Type* getType() const;
    void  setType(Type* type);

    virtual Value* getValue() const;

    virtual std::string toString() const = 0;

    bool isValidType() const;

#define X(a, b) \
    /** @brief 判断符号是否为指定的类型 */ \
    bool is##b() const;
    SymbolTypes
#undef X
};

/**
 * @brief 表示常量条目
 * 
 * 继承自 SymbolTableEntry，保存常量的值
 */
class ConstantEntry : public SymbolTableEntry
{
  private:
    Value* value; ///< 常量的值

  public:
    /**
     * @brief 构造函数，初始化常量条目
     * 
     * @param type 常量的类型
     * @param value 常量的值
     */
    ConstantEntry(Type* type, Value* value);

    /**
     * @brief 析构函数，释放常量的值
     */
    ~ConstantEntry();

    Value* getValue() const override;
    std::string toString() const override;
};

/**
 * @brief 表示标识符条目
 * 
 * 继承自 SymbolTableEntry，表示变量的作用域和名称
 */
class IdentifierEntry : public SymbolTableEntry
{
  private:
    std::string  name;  ///< 变量的名称
    unsigned int scope; ///< 变量的作用域（全局、参数、局部）

  protected:
    enum VariableType
    {
#define X(a, b) a,
        VariableTypes
#undef X
    };

  public:
    /**
     * @brief 构造函数，初始化标识符条目
     * 
     * @param type 变量的类型
     * @param name 变量的名称
     * @param scope 变量的作用域
     */
    IdentifierEntry(Type* type, std::string name, unsigned int scope);

    bool isGlobal() const;
    bool isParam() const;
    bool isLocal() const;

    int getScope() const;

    std::string toString() const override;
};

/**
 * @brief 表示函数条目
 * 
 * 继承自 SymbolTableEntry，保存函数的返回类型和名称
 */
class FunctionEntry : public SymbolTableEntry
{
  private:
    std::string name; ///< 函数名称

  public:
    /**
     * @brief 构造函数，初始化函数条目
     * 
     * @param type 函数类型
     * @param name 函数名称
     */
    FunctionEntry(FuncType* type, std::string name);

    /**
     * @brief 析构函数，释放函数类型
     */
    ~FunctionEntry();

    std::string toString() const override;
};

/**
 * @brief 匿名符号与符号表条目映射表
 * 
 * 保存匿名常量对应的符号表条目
 */
extern std::unordered_map<std::string, std::string> anonToSymTempMap;

/**
 * @brief 表示符号表
 * 
 * 管理作用域、符号表条目，并支持符号的查找和作用域管理
 */
class SymbolTable
{
  private:
    /**
     * @brief 表示符号表中的作用域
     */
    struct Scope
    {
        std::unordered_map<std::string, SymbolTableEntry*> entries; ///< 当前作用域中的符号条目
        Scope*                                             parent;  ///< 父作用域
        int                                                scopeLevel; ///< 作用域的深度

        /**
         * @brief 构造函数，初始化作用域
         * 
         * @param pa 父作用域指针
         */
        Scope(Scope* pa = nullptr);

        /**
         * @brief 析构函数，释放当前作用域中的所有符号条目
         */
        ~Scope();
    };

    Scope*                          currentScope; ///< 当前作用域
    std::unordered_set<std::string> currentParams; ///< 当前作用域中的参数集合
    static int                      count;        ///< 常量名称计数器

  public:
    /**
     * @brief 构造函数，初始化符号表
     */
    SymbolTable();

    /**
     * @brief 析构函数，释放所有作用域
     */
    ~SymbolTable();

    /**
     * @brief 生成常量的名称
     * 
     * @param type 常量的类型
     * @return std::string 常量的名称
     */
    static std::string getConstantName(const Type* type);

    /**
     * @brief 生成函数的名称
     * 
     * @param name 函数名称
     * @param paramTypes 参数类型列表
     * @return std::string 带参数类型的函数名称
     */
    static std::string getFuncname(const std::string& name, const std::vector<Type*>& paramTypes);

    /**
     * @brief 进入一个新的作用域
     */
    void enterScope();

    /**
     * @brief 退出当前作用域
     */
    void exitScope();

    /**
     * @brief 向当前作用域中添加符号条目
     * 
     * @param name 符号名称
     * @param entry 符号条目
     */
    void addEntry(const std::string& name, SymbolTableEntry* entry);

    /**
     * @brief 添加常量条目
     * 
     * @param type 常量类型
     * @param value 常量的值
     * @return std::string 常量的符号名称
     */
    std::string addConstant(Type* type, std::string value);

    /**
     * @brief 添加命名的常量条目
     * 
     * @param name 常量名称
     * @param type 常量类型
     * @param value 常量的值
     */
    void addConstant(const std::string& name, Type* type, std::string value);

    /**
     * @brief 添加变量条目
     * 
     * @param name 变量名称
     * @param type 变量类型
     */
    void addVariable(const std::string& name, Type* type);

    /**
     * @brief 添加函数条目
     * 
     * @param name 函数名称
     * @param retType 返回类型
     * @param paramType 参数类型列表
     */
    void addFunction(const std::string& name, Type* retType, std::vector<Type*> paramType);

    /**
     * @brief 查找符号条目
     * 
     * @param name 符号名称
     * @return SymbolTableEntry* 符号条目指针
     */
    SymbolTableEntry* findEntry(const std::string& name);

    /**
     * @brief 获取当前作用域的层级
     * 
     * @return int 当前作用域的层级
     */
    int getScopeLevel() const;
};

/**
 * @brief 全局符号表指针
 */
extern SymbolTable* globalTable;

/**
 * @brief 当前作用域符号表指针
 */
extern SymbolTable* scopeTable;

#endif