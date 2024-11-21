/*
 * @ref: https://github.com/yuhuifishash/NKU-Compilers2024-RV64GC
 *
 * @参考范围：
 *      1. IR模块设计，包括 IR(LLVMIR)，
 *                        IRBlock(BasicBlock)，
 *                        IRFunction(无参照，考虑日后修改为囊括IRTable的功能)，
 *                        Instruction(Instruction)；
 *      2. IR指令，包括一些llvm的内建函数，如memset；
 *      3. float类型在IR中的表示。预备实验撰写IR时曾发现在IR中若直接调用putf输出float类型数字，
 *         实际输出未知结果。当时采取的做法为转换为double后再输出。
 *         参考内容即为IR中的float表示形式，转换为double的二进制表示即可正常输出，且相较于运行时
 *         类型转换而言少一条指令；
 *      4. 多维数组的初始化规则与初始化方法。
 */
#ifndef __LLVMIR_IRBUILDER_H__
#define __LLVMIR_IRBUILDER_H__

#include <vector>
#include <map>
#include <llvm_ir/instruction.h>
#include <llvm_ir/block.h>
#include <llvm_ir/function.h>

namespace Symbol
{
    class RegTable;
}

namespace LLVMIR
{
    class IRTable
    {
      public:
        Symbol::RegTable*           symTab;
        std::map<int, VarAttribute> regMap;
        std::map<int, bool>         formalArrTab;

        IRTable();
    };

    class IR
    {
      public:
        std::vector<Instruction*> global_def;
        std::vector<Instruction*> function_declare;
        std::vector<IRFunction*>  functions;

        IRFunction* cur_func;

      public:
        IR();
        ~IR();

        void registerLibraryFunctions();

        void enterFunc(IRFunction* func);

        IRBlock* createBlock();
        IRBlock* getBlock(int label);

        void printIR(std::ostream& s);
    };
}  // namespace LLVMIR

#endif