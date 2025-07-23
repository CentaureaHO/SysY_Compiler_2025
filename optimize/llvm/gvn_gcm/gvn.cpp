#pragma once
#include "gvn.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_block.h"
#include <queue>
#include <algorithm>

namespace LLVMIR
{

    GVN::GVN(IR* ir, Cele::Algo::DomAnalyzer* domAnalyzer, Analysis::AliasAnalyser* aliasAnalyser)
        : ir(ir), domAnalyzer(domAnalyzer), aliasAnalyser(aliasAnalyser)
    {}

    GVN::~GVN() {}

    std::unique_ptr<Expression> GVN::buildExpression(Instruction* inst)
    {
        std::vector<Operand*> ops;

        switch (inst->opcode)
        {
            // 二元算术指令
            case IROpCode::ADD:
            case IROpCode::MUL:
            case IROpCode::FADD:
            case IROpCode::FMUL:
            case IROpCode::BITAND:
            case IROpCode::BITXOR:
            {
                auto* binInst = static_cast<ArithmeticInst*>(inst);
                ops.push_back(binInst->lhs);
                ops.push_back(binInst->rhs);
                return std::make_unique<CommutativeBinaryExpression>(inst->opcode, ops);
            }

            case IROpCode::SUB:
            case IROpCode::DIV:
            case IROpCode::FSUB:
            case IROpCode::FDIV:
            case IROpCode::MOD:
            case IROpCode::SHL:
            case IROpCode::ASHR:
            case IROpCode::LSHR:
            case IROpCode::ICMP:
            case IROpCode::FCMP:
            {
                auto* binInst = static_cast<ArithmeticInst*>(inst);
                ops.push_back(binInst->lhs);
                ops.push_back(binInst->rhs);
                return std::make_unique<BinaryExpression>(inst->opcode, ops);
            }

            case IROpCode::LOAD:
            {
                auto* loadInst = static_cast<LoadInst*>(inst);
                ops.push_back(loadInst->ptr);
                return std::make_unique<LoadExpression>(inst->opcode, ops);
            }

            case IROpCode::CALL:
            {
                auto* callInst = static_cast<CallInst*>(inst);
                for (auto& arg : callInst->args) { ops.push_back(arg.second); }
                return std::make_unique<CallExpression>(inst->opcode, ops);
            }

            default:
                // 不支持其他类型的指令
                return nullptr;
        }
    }

    

}  // namespace LLVMIR