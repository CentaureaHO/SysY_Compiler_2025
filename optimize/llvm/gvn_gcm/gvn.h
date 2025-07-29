#pragma once
#include "./Expressin/Binary.h"
#include "./Expressin/Call.h"
#include "./Expressin/Commutative.h"
#include "./Expressin/Load.h"
#include "Valuemap.h"
#include "dom_analyzer.h"
#include "llvm_ir/ir_block.h"
#include "llvm_ir/ir_builder.h"
#include "llvm/alias_analysis/alias_analysis.h"
#include <unordered_map>
#include <memory>

namespace LLVMIR
{

    class GVN
    {
      private:
        IR*                                                           ir;
        Cele::Algo::DomAnalyzer*                                      domAnalyzer;
        Analysis::AliasAnalyser*                                      aliasAnalyser;
        ValueMap                                                      valueMap;
        std::unordered_map<Instruction*, std::unique_ptr<Expression>> instrToExpr;

      public:
        GVN(IR* ir, Cele::Algo::DomAnalyzer* domAnalyzer = nullptr, Analysis::AliasAnalyser* aliasAnalyser = nullptr);
        ~GVN();

        // 主优化入口
        bool optimize();

      private:
        // 构造表达式
        std::unique_ptr<Expression> buildExpression(Instruction* inst);

        // 处理各种指令
        bool processInstruction(Instruction* inst);
        bool processArithmeticInst(Instruction* inst);
        bool processLoadInst(LoadInst* inst);
        bool processCallInst(CallInst* inst);

        // 检查两条指令之间是否有内存干扰
        bool hasMemoryInterference(Instruction* from, Instruction* to, Operand* memLoc);

        // 处理基本块
        bool processBlock(IRBlock* bb);

        // 遍历支配树
        void processDominatorTree();

        // 替换指令
        bool replaceInstruction(Instruction* from, Operand* to);
    };

}  // namespace LLVMIR