#include <common/type/symtab/semantic_table.h>
#include <ast/helper.h>
#include <ast/expression.h>
#include <ast/statement.h>
using namespace std;
using namespace SemanticTable;
using SymTab = Symbol::Table;
using SymEnt = Symbol::Entry;

namespace
{
    // int getint(), getch(), getarray(int a[]);
    static const SymEnt* getint   = SymEnt::getEntry("getint");
    static const SymEnt* getch    = SymEnt::getEntry("getch");
    static const SymEnt* getarray = SymEnt::getEntry("getarray");

    // float getfloat();
    static const SymEnt* getfloat = SymEnt::getEntry("getfloat");

    // int getfarray(float a[]);
    static const SymEnt* getfarray = SymEnt::getEntry("getfarray");

    // void putint(int a), putch(int a), putarray(int n, int a[]);
    static const SymEnt* putint   = SymEnt::getEntry("putint");
    static const SymEnt* putch    = SymEnt::getEntry("putch");
    static const SymEnt* putarray = SymEnt::getEntry("putarray");

    // void putfloat(float a);
    static const SymEnt* putfloat = SymEnt::getEntry("putfloat");

    // void putfarray(int n, float a[]);
    static const SymEnt* putfarray = SymEnt::getEntry("putfarray");

    // void starttime(), stoptime();
    static const SymEnt* _sysy_starttime = SymEnt::getEntry("_sysy_starttime");
    static const SymEnt* _sysy_stoptime  = SymEnt::getEntry("_sysy_stoptime");

    // void putf(char a[], ...)
    // 暂不实现

    /*  暂不实现
    struct timeval _sysy_start, _sysy_end;
    #define starttime() _sysy_starttime(__LINE__)
    #define stoptime() _sysy_stoptime(__LINE__)
    #define _SYSY_N 1024
    int                             _sysy_l1[_SYSY_N], _sysy_l2[_SYSY_N];
    int                             _sysy_h[_SYSY_N], _sysy_m[_SYSY_N], _sysy_s[_SYSY_N], _sysy_us[_SYSY_N];
    int                             _sysy_idx;
    __attribute((constructor)) void before_main();
    __attribute((destructor)) void  after_main();
    void                            _sysy_starttime(int lineno);
    void                            _sysy_stoptime(int lineno);
     */
}  // namespace

Table::Table() : symTable(), glbSymMap(), funcDeclMap()
{
    // int getint()
    funcDeclMap[getint] = new FuncDeclStmt(getint, TypeSystem::getBasicType(TypeKind::Int));

    // int getch()
    funcDeclMap[getch] = new FuncDeclStmt(getch, TypeSystem::getBasicType(TypeKind::Int));

    // int getarray(int a[])
    funcDeclMap[getarray] = new FuncDeclStmt(getarray,
        TypeSystem::getBasicType(TypeKind::Int),
        new std::vector<FuncParamDefNode*>{new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Int),
            SymEnt::getEntry("a"),
            new std::vector<ExprNode*>{new ConstExpr(-1)})});

    // float getfloat()
    funcDeclMap[getfloat] = new FuncDeclStmt(getfloat, TypeSystem::getBasicType(TypeKind::Float));

    // int getfarray(float a[])
    funcDeclMap[getfarray] = new FuncDeclStmt(getfarray,
        TypeSystem::getBasicType(TypeKind::Int),
        new std::vector<FuncParamDefNode*>{new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Float),
            SymEnt::getEntry("a"),
            new std::vector<ExprNode*>{new ConstExpr(-1)})});

    // void putint(int a)
    funcDeclMap[putint] = new FuncDeclStmt(putint,
        TypeSystem::getBasicType(TypeKind::Void),
        new std::vector<FuncParamDefNode*>{
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Int), SymEnt::getEntry("a"))});

    // void putch(int a)
    funcDeclMap[putch] = new FuncDeclStmt(putch,
        TypeSystem::getBasicType(TypeKind::Void),
        new std::vector<FuncParamDefNode*>{
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Int), SymEnt::getEntry("a"))});

    // void putarray(int n, int a[])
    funcDeclMap[putarray] = new FuncDeclStmt(putarray,
        TypeSystem::getBasicType(TypeKind::Void),
        new std::vector<FuncParamDefNode*>{
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Int), SymEnt::getEntry("n")),
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Int),
                SymEnt::getEntry("a"),
                new std::vector<ExprNode*>{new ConstExpr(-1)})});

    // void putfloat(float a)
    funcDeclMap[putfloat] = new FuncDeclStmt(putfloat,
        TypeSystem::getBasicType(TypeKind::Void),
        new std::vector<FuncParamDefNode*>{
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Float), SymEnt::getEntry("a"))});

    // void putfarray(int n, float a[])
    funcDeclMap[putfarray] = new FuncDeclStmt(putfarray,
        TypeSystem::getBasicType(TypeKind::Void),
        new std::vector<FuncParamDefNode*>{
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Int), SymEnt::getEntry("n")),
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Float),
                SymEnt::getEntry("a"),
                new std::vector<ExprNode*>{new ConstExpr(-1)})});

    // void _sysy_starttime(int lineno)
    funcDeclMap[_sysy_starttime] = new FuncDeclStmt(_sysy_starttime,
        TypeSystem::getBasicType(TypeKind::Void),
        new std::vector<FuncParamDefNode*>{
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Int), SymEnt::getEntry("lineno"))});

    // void stoptime(int lineno)
    funcDeclMap[_sysy_stoptime] = new FuncDeclStmt(_sysy_stoptime,
        TypeSystem::getBasicType(TypeKind::Void),
        new std::vector<FuncParamDefNode*>{
            new FuncParamDefNode(TypeSystem::getBasicType(TypeKind::Int), SymEnt::getEntry("lineno"))});

    // for (auto& [_, decl] : funcDeclMap) { decl->typeCheck(); }
}

Table::~Table()
{
    for (auto& [_, decl] : funcDeclMap)
    {
        if (_ == getint || _ == getch || _ == getarray || _ == getfloat || _ == getfarray || _ == putint ||
            _ == putch || _ == putarray || _ == putfloat || _ == putfarray || _ == _sysy_starttime ||
            _ == _sysy_stoptime)
            delete decl;
    }
}

void Table::reg_funcs()
{
    for (auto& [_, decl] : funcDeclMap) { decl->typeCheck(); }
}

namespace
{
    class Cleaner
    {
      public:
        ~Cleaner()
        {
            if (semTable)
            {
                delete semTable;
                semTable = nullptr;
            }
        }
    } cleaner;
}  // namespace

Table* semTable = nullptr;