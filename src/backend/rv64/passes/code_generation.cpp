#include <backend/rv64/passes/code_generation.h>
#include <llvm_ir/instruction.h>
#include <cassert>

#define IS_IMMEI32(operand) (operand->type == LLVMIR::OperandType::IMMEI32)
#define IS_IMMEF32(operand) (operand->type == LLVMIR::OperandType::IMMEF32)

namespace Backend::RV64::Passes
{

    CodeGenerationPass::CodeGenerationPass(
        std::vector<Function*>& functions, std::vector<LLVMIR::Instruction*>& glb_defs, std::ostream& out)
        : functions_(functions), glb_defs_(glb_defs), out_(out), cur_func_(nullptr), cur_block_(nullptr)
    {}

    bool CodeGenerationPass::run()
    {
        // Print assembly header
        out_ << "\t.text\n\t.globl main\n";
        out_ << "\t.attribute arch, \"rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0_zba1p0_zbb1p0\"\n\n";

        printFunctions();
        printGlobalDefinitions();

        return false;  // Does not modify the IR, just outputs code
    }

    void CodeGenerationPass::printFunctions()
    {
        for (auto& func : functions_)
        {
            cur_func_ = func;
            out_ << func->name << ":\n";

            for (auto& block : func->blocks)
            {
                cur_block_ = block;
                int bid    = block->label_num;

                out_ << "." << func->name << "_" << bid << ":\n";

                for (auto& inst : block->insts)
                {
                    if (inst->inst_type == InstType::RV64)
                    {
                        out_ << "\t";
                        printASM((RV64Inst*)inst);
                        out_ << "\n";
                    }
                    else if (inst->inst_type == InstType::MOVE)
                    {
                        out_ << "\t";
                        printASM((MoveInst*)inst);
                        out_ << "\n";
                    }
                    else if (inst->inst_type == InstType::PHI)
                    {
                        out_ << "\t";
                        printASM((PhiInst*)inst);
                        out_ << "\n";
                    }
                    else
                        assert(false);
                }
            }
        }
    }

    void CodeGenerationPass::printGlobalDefinitions()
    {
        out_ << "\t.data\n";
        for (auto& glb_def : glb_defs_)
        {
            if (glb_def->opcode != LLVMIR::IROpCode::GLOBAL_VAR) assert(false);

            LLVMIR::GlbvarDefInst* inst = (LLVMIR::GlbvarDefInst*)glb_def;
            out_ << inst->name << ":\n";

            if (inst->type == LLVMIR::DataType::I32)
            {
                if (inst->arr_init.dims.empty())
                {
                    if (inst->init)
                    {
                        assert(IS_IMMEI32(inst->init));
                        out_ << "\t.word\t" << ((LLVMIR::ImmeI32Operand*)inst->init)->value << "\n";
                    }
                    else
                        out_ << "\t.word\t0\n";
                }
                else
                {
                    int zero_cum = 0;

                    for (auto& val : inst->arr_init.initVals)
                    {
                        int v = val.int_val;
                        if (v == 0)
                        {
                            zero_cum += 4;
                            continue;
                        }

                        if (zero_cum)
                        {
                            out_ << "\t.zero\t" << zero_cum << "\n";
                            zero_cum = 0;
                        }
                        out_ << "\t.word\t" << v << "\n";
                    }
                    if (inst->arr_init.initVals.empty())
                    {
                        int prod = 4;
                        for (auto dim : inst->arr_init.dims) prod *= dim;
                        out_ << "\t.zero\t" << prod * 4 << "\n";
                    }
                    if (zero_cum)
                    {
                        out_ << "\t.zero\t" << zero_cum << "\n";
                        zero_cum = 0;
                    }
                }
            }
            else if (inst->type == LLVMIR::DataType::F32)
            {
                if (inst->arr_init.dims.empty())
                {
                    if (inst->init)
                    {
                        assert(IS_IMMEF32(inst->init));
                        float f = ((LLVMIR::ImmeF32Operand*)inst->init)->value;
                        out_ << "\t.word\t" << *(int*)&f << "\n";
                    }
                    else
                        out_ << "\t.word\t0\n";
                }
                else
                {
                    int zero_cum = 0;
                    for (auto& val : inst->arr_init.initVals)
                    {
                        float f = val.float_val;
                        if (f == 0)
                            zero_cum += 4;
                        else
                        {
                            if (zero_cum)
                            {
                                out_ << "\t.zero\t" << zero_cum << "\n";
                                zero_cum = 0;
                            }
                            out_ << "\t.word\t" << *(int*)&f << "\n";
                        }
                    }
                    if (inst->arr_init.initVals.empty())
                    {
                        int prod = 4;
                        for (auto dim : inst->arr_init.dims) prod *= dim;
                        out_ << "\t.zero\t" << prod << "\n";
                    }
                    if (zero_cum)
                    {
                        out_ << "\t.zero\t" << zero_cum << "\n";
                        zero_cum = 0;
                    }
                }
            }
            else if (inst->type == LLVMIR::DataType::I64)
            {
                // Not implemented yet
                assert(false);
            }
            else
                assert(false);
        }
    }

    void CodeGenerationPass::printASM(RV64Inst* inst)
    {
        // Helper function
        auto isMemInst = [](RV64InstType type) {
            return type == RV64InstType::LW || type == RV64InstType::LD || type == RV64InstType::FLW ||
                   type == RV64InstType::FLD || type == RV64InstType::FSW || type == RV64InstType::FSD;
        };

        OpInfo& opinfo = opInfoTable[inst->op];
        out_ << opinfo._asm << "\t\t";
        if (opinfo._asm.length() <= 3) out_ << '\t';

        switch (opinfo.type)
        {
            case RV64OpType::R:
            {
                printOperand(inst->rd);
                out_ << ", ";
                printOperand(inst->rs1);
                out_ << ", ";
                printOperand(inst->rs2);
                break;
            }
            case RV64OpType::R2:
            {
                printOperand(inst->rd);
                out_ << ", ";
                printOperand(inst->rs1);
                if (inst->op == RV64InstType::FCVT_W_S) out_ << ", rtz";
                break;
            }
            case RV64OpType::I:
            {
                printOperand(inst->rd);
                out_ << ", ";

                if (isMemInst(inst->op))
                {
                    if (inst->use_label)
                        printOperand(inst->label);
                    else
                        out_ << inst->imme;
                    out_ << "(";
                    printOperand(inst->rs1);
                    out_ << ")";
                }
                else
                {
                    printOperand(inst->rs1);
                    out_ << ", ";
                    if (inst->use_label)
                        printOperand(inst->label);
                    else
                        out_ << inst->imme;
                }

                break;
            }
            case RV64OpType::S:
            {
                printOperand(inst->rs1);
                out_ << ", ";
                if (inst->use_label)
                    printOperand(inst->label);
                else { out_ << inst->imme; }
                out_ << "(";
                printOperand(inst->rs2);
                out_ << ")";
                break;
            }
            case RV64OpType::B:
            {
                printOperand(inst->rs1);
                out_ << ", ";
                printOperand(inst->rs2);
                out_ << ", ";
                if (inst->use_label)
                    printOperand(inst->label);
                else
                    out_ << inst->imme;
                break;
            }
            case RV64OpType::U:
            {
                printOperand(inst->rd);
                out_ << ", ";
                if (inst->use_label)
                    printOperand(inst->label);
                else
                    out_ << inst->imme;
                break;
            }
            case RV64OpType::J:
            {
                printOperand(inst->rd);
                out_ << ", ";
                if (inst->use_label)
                    printOperand(inst->label);
                else
                    out_ << inst->imme;
                break;
            }
            case RV64OpType::CALL:
            {
                out_ << inst->label.name;
                break;
            }
            case RV64OpType::R4:
            {
                printOperand(inst->rd);
                out_ << ", ";
                printOperand(inst->rs1);
                out_ << ", ";
                printOperand(inst->rs2);
                out_ << ", ";
                printOperand(inst->rs3);
                break;
            }
            default: assert(false);
        }
    }

    void CodeGenerationPass::printASM(MoveInst* inst)
    {
        out_ << "move ";
        printOperand(inst->dst);
        out_ << ", ";
        printOperand(inst->src);
        out_ << ", " << inst->data_type->toString();
    }

    void CodeGenerationPass::printASM(PhiInst* inst)
    {
        printOperand(inst->res_reg);
        out_ << " = " << inst->res_reg.data_type->toString() << " PHI ";
        for (auto& [label, val] : inst->phi_list)
        {
            out_ << "[";
            printOperand(val);
            out_ << ", %" << cur_func_->name << "_" << label << "] ";
        }
    }

    void CodeGenerationPass::printOperand(Register r)
    {
        if (r.is_virtual)
            out_ << "v_" << r.reg_num;
        else
            out_ << rv64_reg_name_map[r.reg_num];
    }

    void CodeGenerationPass::printOperand(Register* r)
    {
        if (r->is_virtual)
            out_ << "v_" << r->reg_num;
        else
            out_ << rv64_reg_name_map[r->reg_num];
    }

    void CodeGenerationPass::printOperand(RV64Label l)
    {
        if (l.is_data_addr)
        {
            if (l.is_la)
                out_ << l.name;
            else if (l.is_hi)
                out_ << "%hi(" << l.name << ")";
            else
                out_ << "%lo(" << l.name << ")";
        }
        else
            out_ << "." << cur_func_->name << "_" << l.jmp_label;
    }

    void CodeGenerationPass::printOperand(RV64Label* l)
    {
        if (l->is_data_addr)
        {
            if (l->is_hi)
                out_ << "%hi(" << l->name << ")";
            else
                out_ << "%lo(" << l->name << ")";
        }
        else
            out_ << "." << cur_func_->name << "_" << l->jmp_label;
    }

    void CodeGenerationPass::printOperand(Operand* op)
    {
        if (op->operand_type == OperandType::REG)
            printOperand(((RegOperand*)op)->reg);
        else if (op->operand_type == OperandType::IMMEI32)
            out_ << ((ImmeI32Operand*)op)->val;
        else if (op->operand_type == OperandType::IMMEF32)
            out_ << ((ImmeF32Operand*)op)->val;
        else
            assert(false);
    }

}  // namespace Backend::RV64::Passes
