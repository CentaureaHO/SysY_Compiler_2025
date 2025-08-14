#include <backend/rv64/passes/code_generation.h>
#include <llvm_ir/instruction.h>
#include <cassert>
#include <set>

#define IS_IMMEI32(operand) (operand->type == LLVMIR::OperandType::IMMEI32)
#define IS_IMMEF32(operand) (operand->type == LLVMIR::OperandType::IMMEF32)

namespace Backend::RV64::Passes
{

    CodeGenerationPass::CodeGenerationPass(
        std::vector<Function*>& functions, std::vector<LLVMIR::Instruction*>& glb_defs, std::ostream& out)
        : functions_(functions),
          glb_defs_(glb_defs),
          out_(out),
          needs_memset_i8_(false),
          needs_memset_i32_(false),
          cur_func_(nullptr),
          cur_block_(nullptr)
    {}

    bool CodeGenerationPass::run()
    {
        // Print assembly header
        out_ << "\t.text\n\t.globl main\n";
        out_ << "\t.attribute arch, \"" << ::Backend::RV64::getRV64ArchString() << "\"\n\n";

        printFunctions();
        printASMLibFunctions();
        printGlobalDefinitions();

        return false;  // Does not modify the IR, just outputs code
    }

    void CodeGenerationPass::printASMLibFunctions()
    {
        if (needs_memset_i8_) { printMemsetI8Function(); }
        if (needs_memset_i32_) { printMemsetI32Function(); }
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
                    // Check for library function calls
                    if (inst->inst_type == InstType::RV64)
                    {
                        RV64Inst* rv64_inst = (RV64Inst*)inst;
                        if (rv64_inst->op == RV64InstType::CALL)
                        {
                            std::string func_name = rv64_inst->label.name;
                            if (func_name == "lsccll.lib.memset_i8") { needs_memset_i8_ = true; }
                            else if (func_name == "lsccll.lib.memset_i32") { needs_memset_i32_ = true; }
                        }
                    }

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
                    else if (inst->inst_type == InstType::SELECT)
                    {
                        out_ << "\t";
                        printASM((SelectInst*)inst);
                        out_ << "\n";
                    }
                    else
                        assert(false);
                }
            }
        }
    }

    void CodeGenerationPass::printMemsetI8Function()
    {
        out_ << "\n";
        out_ << "\t.p2align\t1\n";
        out_ << "lsccll.lib.memset_i8:\n";
        out_ << "\ttail\tmemset\n";
        out_ << ".Lfunc_end_memset_i8:\n";
        out_ << "\t\t\t\t\t\t# -- End function\n";
    }

    void CodeGenerationPass::printMemsetI32Function()
    {
        out_ << "\n";
        out_ << "\t.p2align\t1\n";
        out_ << "lsccll.lib.memset_i32:\n";
        out_ << "# %bb.0:\t\t\t\t\t\t# %entry\n";
        out_ << "\tsext.w\ta3, a2\n";
        out_ << "\tbeqz\ta3, .LBB1_13\n";
        out_ << "# %bb.1:\t\t\t\t\t\t# %loop.preheader\n";
        out_ << "\tandi\ta6, a2, 7\n";
        out_ << "\tsrliw\ta2, a2, 3\n";
        out_ << "\tbeqz\ta2, .LBB1_5\n";
        out_ << "# %bb.2:\t\t\t\t\t\t# %loop.preheader.new\n";
        out_ << "\tslli\ta2, a2, 3\n";
        out_ << "\tnegw\tt0, a2\n";
        out_ << "\tli\ta7, 7\n";
        out_ << "\tli\ta3, 7\n";
        out_ << ".LBB1_3:\t\t\t\t\t\t# %loop\n";
        out_ << "\t\t\t\t\t\t\t\t# =>This Inner Loop Header: Depth=1\n";
        out_ << "\taddiw\ta4, a3, -7\n";
        out_ << "\tslli\ta4, a4, 2\n";
        out_ << "\taddiw\ta5, a3, -6\n";
        out_ << "\tadd\tt1, a0, a4\n";
        out_ << "\tslli\ta5, a5, 2\n";
        out_ << "\tadd\ta5, a5, a0\n";
        out_ << "\taddiw\ta2, a3, -5\n";
        out_ << "\tslli\ta2, a2, 2\n";
        out_ << "\taddiw\ta4, a3, -4\n";
        out_ << "\tadd\ta2, a2, a0\n";
        out_ << "\tslli\ta4, a4, 2\n";
        out_ << "\tsw\ta1, 0(t1)\n";
        out_ << "\tadd\ta4, a4, a0\n";
        out_ << "\tsw\ta1, 0(a5)\n";
        out_ << "\tsw\ta1, 0(a2)\n";
        out_ << "\taddiw\ta2, a3, -3\n";
        out_ << "\tsw\ta1, 0(a4)\n";
        out_ << "\tslli\ta2, a2, 2\n";
        out_ << "\tadd\tt1, a0, a2\n";
        out_ << "\taddiw\ta4, a3, -2\n";
        out_ << "\tslli\ta4, a4, 2\n";
        out_ << "\taddiw\ta5, a3, -1\n";
        out_ << "\tadd\ta4, a4, a0\n";
        out_ << "\tslli\ta5, a5, 2\n";
        out_ << "\tadd\ta5, a5, a0\n";
        out_ << "\tslli\ta2, a3, 2\n";
        out_ << "\tsw\ta1, 0(t1)\n";
        out_ << "\tadd\ta2, a2, a0\n";
        out_ << "\tsw\ta1, 0(a4)\n";
        out_ << "\taddiw\ta3, a3, 8\n";
        out_ << "\tsw\ta1, 0(a5)\n";
        out_ << "\taddw\ta4, t0, a3\n";
        out_ << "\tsw\ta1, 0(a2)\n";
        out_ << "\tbne\ta4, a7, .LBB1_3\n";
        out_ << "# %bb.4:\t\t\t\t\t\t# %exit.loopexit.unr-lcssa.loopexit\n";
        out_ << "\taddiw\ta2, a3, -7\n";
        out_ << ".LBB1_5:\t\t\t\t\t\t# %exit.loopexit.unr-lcssa\n";
        out_ << "\tbeqz\ta6, .LBB1_13\n";
        out_ << "# %bb.6:\t\t\t\t\t\t# %loop.epil\n";
        out_ << "\tslli\ta3, a2, 2\n";
        out_ << "\tadd\ta3, a3, a0\n";
        out_ << "\tli\ta4, 1\n";
        out_ << "\tsw\ta1, 0(a3)\n";
        out_ << "\tbeq\ta6, a4, .LBB1_13\n";
        out_ << "# %bb.7:\t\t\t\t\t\t# %loop.epil.1\n";
        out_ << "\taddiw\ta3, a2, 1\n";
        out_ << "\tslli\ta3, a3, 2\n";
        out_ << "\tadd\ta3, a3, a0\n";
        out_ << "\tli\ta4, 2\n";
        out_ << "\tsw\ta1, 0(a3)\n";
        out_ << "\tbeq\ta6, a4, .LBB1_13\n";
        out_ << "# %bb.8:\t\t\t\t\t\t# %loop.epil.2\n";
        out_ << "\taddiw\ta3, a2, 2\n";
        out_ << "\tslli\ta3, a3, 2\n";
        out_ << "\tadd\ta3, a3, a0\n";
        out_ << "\tli\ta4, 3\n";
        out_ << "\tsw\ta1, 0(a3)\n";
        out_ << "\tbeq\ta6, a4, .LBB1_13\n";
        out_ << "# %bb.9:\t\t\t\t\t\t# %loop.epil.3\n";
        out_ << "\taddiw\ta3, a2, 3\n";
        out_ << "\tslli\ta3, a3, 2\n";
        out_ << "\tadd\ta3, a3, a0\n";
        out_ << "\tli\ta4, 4\n";
        out_ << "\tsw\ta1, 0(a3)\n";
        out_ << "\tbeq\ta6, a4, .LBB1_13\n";
        out_ << "# %bb.10:\t\t\t\t\t\t# %loop.epil.4\n";
        out_ << "\taddiw\ta3, a2, 4\n";
        out_ << "\tslli\ta3, a3, 2\n";
        out_ << "\tadd\ta3, a3, a0\n";
        out_ << "\tli\ta4, 5\n";
        out_ << "\tsw\ta1, 0(a3)\n";
        out_ << "\tbeq\ta6, a4, .LBB1_13\n";
        out_ << "# %bb.11:\t\t\t\t\t\t# %loop.epil.5\n";
        out_ << "\taddiw\ta3, a2, 5\n";
        out_ << "\tslli\ta3, a3, 2\n";
        out_ << "\tadd\ta3, a3, a0\n";
        out_ << "\tli\ta4, 6\n";
        out_ << "\tsw\ta1, 0(a3)\n";
        out_ << "\tbeq\ta6, a4, .LBB1_13\n";
        out_ << "# %bb.12:\t\t\t\t\t\t# %loop.epil.6\n";
        out_ << "\taddiw\ta2, a2, 6\n";
        out_ << "\tslli\ta2, a2, 2\n";
        out_ << "\tadd\ta0, a0, a2\n";
        out_ << "\tsw\ta1, 0(a0)\n";
        out_ << ".LBB1_13:\t\t\t\t\t\t# %exit\n";
        out_ << "\tret\n";
        out_ << ".Lfunc_end_memset_i32:\n";
        out_ << "\t\t\t\t\t\t# -- End function\n";
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

    void CodeGenerationPass::printASM(SelectInst* inst)
    {
        printOperand(inst->dst_reg);
        out_ << " = " << inst->dst_reg.data_type->toString() << " SELECT ";
        printOperand(inst->cond_reg);
        out_ << " ? ";
        printOperand(inst->true_val);
        out_ << " : ";
        printOperand(inst->false_val);
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
