#include "readonly.h"
#include "llvm_ir/defs.h"
#include "llvm_ir/instruction.h"

namespace Analysis
{
    static CFG* getCfgByName(LLVMIR::IR* ir, const std::string& name)
    {
        for (auto& func : ir->functions)
        {
            if (func->func_def->func_name == name)
            {
                if (ir->cfg.count(func->func_def)) return ir->cfg.at(func->func_def);
            }
        }
        return nullptr;
    }

    ReadOnlyGlobalAnalysis::ReadOnlyGlobalAnalysis(LLVMIR::IR* ir, AliasAnalyser* alias_analyser)
        : ir(ir), alias_analyser(alias_analyser)
    {}

    void ReadOnlyGlobalAnalysis::run()
    {
        buildGlobalAliasPtrSet();
        scanForWrites();
    }

    bool ReadOnlyGlobalAnalysis::isReadOnly(LLVMIR::Operand* global) { return readonly_globals.count(global) > 0; }

    const std::unordered_set<LLVMIR::Operand*>& ReadOnlyGlobalAnalysis::getReadOnlyGlobals()
    {
        return readonly_globals;
    }

    void ReadOnlyGlobalAnalysis::buildGlobalAliasPtrSet()
    {
        for (auto& [func, cfg] : ir->cfg)
        {
            for (auto& [id, block] : cfg->block_id_to_block)
            {
                for (auto* inst : block->insts)
                {
                    if (inst->opcode == LLVMIR::IROpCode::GETELEMENTPTR)
                    {
                        auto* gep_inst = static_cast<LLVMIR::GEPInst*>(inst);
                        // 考虑 GEP 的 base_ptr 是否是全局变量或可能的别名寄存器
                        if (gep_inst->base_ptr->type == LLVMIR::OperandType::GLOBAL ||
                            maybe_alias_regs.count(gep_inst->base_ptr))
                        {
#ifdef DEBUG
                            // 如果是全局变量的 GEP，加入到可能的别名集合中
                            std::cout << "GEP base pointer is global: " << gep_inst->base_ptr->getName()
                                      << " Result op is " << gep_inst->GetResultOperand()->getName() << std::endl;
#endif
                            maybe_alias_regs[gep_inst->GetResultOperand()] = gep_inst->base_ptr;
                        }
                    }
                    if (inst->opcode == LLVMIR::IROpCode::LOAD)
                    {
                        auto* load_inst = static_cast<LLVMIR::LoadInst*>(inst);
                        if (load_inst->ptr->type == LLVMIR::OperandType::GLOBAL)
                        {
                            maybe_alias_regs[load_inst->GetResultOperand()] = load_inst->ptr;
                        }
                    }
                }
            }
        }
    }

    LLVMIR::Operand* ReadOnlyGlobalAnalysis::traceToGlobal(LLVMIR::Operand* op)
    {
        std::unordered_set<LLVMIR::Operand*> visited;
        while (maybe_alias_regs.count(op))
        {
#ifdef DEBUG
            std::cout << "op is " << op->getName() << " and type is "
                      << ((op->type == LLVMIR::OperandType::GLOBAL) ? "Global" : "Other") << std::endl;
#endif
            if (op->type == LLVMIR::OperandType::GLOBAL) return op;  // 如果已经是全局变量，直接返回
            if (visited.count(op)) break;                            // 防止 alias 循环
            visited.insert(op);
            op = maybe_alias_regs[op];
#ifdef DEBUG
            std::cout << "op is " << op->getName() << " and type is "
                      << ((op->type == LLVMIR::OperandType::GLOBAL) ? "Global" : "Other") << std::endl;
#endif
        }

        if (op->type == LLVMIR::OperandType::GLOBAL) return op;
        return nullptr;
    }

    void ReadOnlyGlobalAnalysis::scanForWrites()
    {
        for (auto& [func, cfg] : ir->cfg)
        {
            for (auto& [id, block] : cfg->block_id_to_block)
            {
                for (auto* inst : block->insts)
                {
                    // 检查 store
                    if (inst->opcode == LLVMIR::IROpCode::STORE)
                    {
                        auto ptr = inst->GetUsedOperands().at(0);  // store第一个是目标
#ifdef DEBUG
                        std::cout << "Store to global: " << ptr->getName() << std::endl;
#endif
                        auto global = traceToGlobal(ptr);
                        if (global)
                        {
#ifdef DEBUG
                            std::cout << "Found write to global: " << global->getName() << std::endl;
#endif
                            // 就算是global，但是我们还是不能确定就一定写入
                            auto writes = alias_analyser->getWritePtrs(getCfgByName(ir, func->func_name));
                            for (auto write : writes)
                            {
                                if (write == global && !maybe_written_globals.count(global))
                                {
#ifdef DEBUG
                                    std::cout << "Truly! Writing to global: " << global->getName() << std::endl;
#endif
                                    maybe_written_globals.insert(global);
                                }
                            }
                        }
                    }

                    // 检查 call 的指针参数是否有全局变量
                    if (inst->opcode == LLVMIR::IROpCode::CALL)
                    {
                        auto call_inst = dynamic_cast<LLVMIR::CallInst*>(inst);
                        auto call_cfg  = getCfgByName(ir, call_inst->func_name);
                        if (call_cfg && alias_analyser->isNoSideEffect(call_cfg))
                        {
                            // 没有副作用的函数调用
                            // std::cout << "Call to function without side effects: " << call_inst->func_name <<
                            // std::endl;
                        }
                        else
                        {
                            // 有副作用，需要考虑，加入所有在这里被写入的
                            auto writes = alias_analyser->getWritePtrs(call_cfg);
                            for (auto write : writes)
                            {
#ifdef DEBUG
                                std::cout << "Write to global: " << write->getName() << std::endl;
#endif
                                if (write->type == LLVMIR::OperandType::GLOBAL) { maybe_written_globals.insert(write); }
                            }
                            // 此外检查所有的参数
                            for (auto& [ty, arg] : call_inst->args)
                            {
                                if (maybe_written_globals.count(arg))
                                {
                                    auto arg_global = traceToGlobal(arg);
                                    if (arg_global && !maybe_written_globals.count(arg_global))
                                    {
                                        maybe_written_globals.insert(traceToGlobal(arg));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // 所有未出现过写入的全局变量视为只读
        for (auto* global : ir->global_def)
        {
            auto global_def = dynamic_cast<LLVMIR::GlbvarDefInst*>(global);
            auto gloabl_op  = getGlobalOperand(global_def->name);
#ifdef DEBUG
            std::cout << "Global variable: " << gloabl_op->getName() << std::endl;
#endif
            if (maybe_written_globals.count(gloabl_op) == 0)
            {
#ifdef DEBUG
                std::cout << "In Global variable: " << gloabl_op->getName() << std::endl;
#endif
                readonly_globals.insert(gloabl_op);
            }
        }
    }
}  // namespace Analysis