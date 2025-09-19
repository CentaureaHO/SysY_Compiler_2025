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
		  needs_parallel_support_(false),
          cur_func_(nullptr),
          cur_block_(nullptr)
    {}

    bool CodeGenerationPass::run()
    {
        // Print assembly header
        out_ << "\t.text\n\t.globl main\n";
        out_ << "\t.attribute	4, 16\n";
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
        if (needs_parallel_support_) { printParallelSupportFunctions(); }
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
                            else if (func_name == "lsccll.lib.parallel.loop") { needs_parallel_support_ = true; }
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
        static const char memset_i8_asm[] = R"(
	.p2align	1
lsccll.lib.memset_i8:                   # @lsccll.lib.memset_i8
# %bb.0:                                # %entry
	tail	memset
.Lfunc_end0:

        )";

        out_ << memset_i8_asm << "\n";
    }

    void CodeGenerationPass::printMemsetI32Function()
    {
        static const char memset_i32_asm[] = R"(
    .p2align	1
lsccll.lib.memset_i32:                  # @lsccll.lib.memset_i32
# %bb.0:                                # %entry
	sext.w	a3, a2
	beqz	a3, .LBB1_13
# %bb.1:                                # %loop.preheader
	andi	a6, a2, 7
	srliw	a2, a2, 3
	beqz	a2, .LBB1_5
# %bb.2:                                # %loop.preheader.new
	slli	a2, a2, 3
	negw	t0, a2
	li	a7, 7
	li	a3, 7
.LBB1_3:                                # %loop
                                        # =>This Inner Loop Header: Depth=1
	addiw	a4, a3, -7
	slli	a4, a4, 2
	addiw	a5, a3, -6
	add	t1, a0, a4
	slli	a5, a5, 2
	add	a5, a5, a0
	addiw	a2, a3, -5
	slli	a2, a2, 2
	addiw	a4, a3, -4
	add	a2, a2, a0
	slli	a4, a4, 2
	sw	a1, 0(t1)
	add	a4, a4, a0
	sw	a1, 0(a5)
	sw	a1, 0(a2)
	addiw	a2, a3, -3
	sw	a1, 0(a4)
	slli	a2, a2, 2
	add	t1, a0, a2
	addiw	a4, a3, -2
	slli	a4, a4, 2
	addiw	a5, a3, -1
	add	a4, a4, a0
	slli	a5, a5, 2
	add	a5, a5, a0
	slli	a2, a3, 2
	sw	a1, 0(t1)
	add	a2, a2, a0
	sw	a1, 0(a4)
	addiw	a3, a3, 8
	sw	a1, 0(a5)
	addw	a4, t0, a3
	sw	a1, 0(a2)
	bne	a4, a7, .LBB1_3
# %bb.4:                                # %exit.loopexit.unr-lcssa.loopexit
	addiw	a2, a3, -7
.LBB1_5:                                # %exit.loopexit.unr-lcssa
	beqz	a6, .LBB1_13
# %bb.6:                                # %loop.epil
	slli	a3, a2, 2
	add	a3, a3, a0
	li	a4, 1
	sw	a1, 0(a3)
	beq	a6, a4, .LBB1_13
# %bb.7:                                # %loop.epil.1
	addiw	a3, a2, 1
	slli	a3, a3, 2
	add	a3, a3, a0
	li	a4, 2
	sw	a1, 0(a3)
	beq	a6, a4, .LBB1_13
# %bb.8:                                # %loop.epil.2
	addiw	a3, a2, 2
	slli	a3, a3, 2
	add	a3, a3, a0
	li	a4, 3
	sw	a1, 0(a3)
	beq	a6, a4, .LBB1_13
# %bb.9:                                # %loop.epil.3
	addiw	a3, a2, 3
	slli	a3, a3, 2
	add	a3, a3, a0
	li	a4, 4
	sw	a1, 0(a3)
	beq	a6, a4, .LBB1_13
# %bb.10:                               # %loop.epil.4
	addiw	a3, a2, 4
	slli	a3, a3, 2
	add	a3, a3, a0
	li	a4, 5
	sw	a1, 0(a3)
	beq	a6, a4, .LBB1_13
# %bb.11:                               # %loop.epil.5
	addiw	a3, a2, 5
	slli	a3, a3, 2
	add	a3, a3, a0
	li	a4, 6
	sw	a1, 0(a3)
	beq	a6, a4, .LBB1_13
# %bb.12:                               # %loop.epil.6
	addiw	a2, a2, 6
	slli	a2, a2, 2
	add	a0, a0, a2
	sw	a1, 0(a0)
.LBB1_13:                               # %exit
	ret
.Lfunc_end1:

        )";

        out_ << memset_i32_asm << "\n";
    }

    void CodeGenerationPass::printParallelSupportFunctions()
    {
        static const char parallel_support_asm[] = R"(
	.p2align	1
lsccll.lib.parallel.loop:               # @lsccll.lib.parallel.loop
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -208
	.cfi_def_cfa_offset 208
	sd	ra, 168(sp)                     # 8-byte Folded Spill
	sd	s0, 160(sp)                     # 8-byte Folded Spill
	sd	s1, 152(sp)                     # 8-byte Folded Spill
	sd	s2, 144(sp)                     # 8-byte Folded Spill
	sd	s3, 136(sp)                     # 8-byte Folded Spill
	sd	s4, 128(sp)                     # 8-byte Folded Spill
	sd	s5, 120(sp)                     # 8-byte Folded Spill
	sd	s6, 112(sp)                     # 8-byte Folded Spill
	sd	s7, 104(sp)                     # 8-byte Folded Spill
	sd	s8, 96(sp)                      # 8-byte Folded Spill
	sd	s9, 88(sp)                      # 8-byte Folded Spill
	sd	s10, 80(sp)                     # 8-byte Folded Spill
	sd	s11, 72(sp)                     # 8-byte Folded Spill
	.cfi_offset ra, -40
	.cfi_offset s0, -48
	.cfi_offset s1, -56
	.cfi_offset s2, -64
	.cfi_offset s3, -72
	.cfi_offset s4, -80
	.cfi_offset s5, -88
	.cfi_offset s6, -96
	.cfi_offset s7, -104
	.cfi_offset s8, -112
	.cfi_offset s9, -120
	.cfi_offset s10, -128
	.cfi_offset s11, -136
	addi	s0, sp, 176
	.cfi_def_cfa s0, 32
	mv	s7, a4
	slliw	s10, a3, 2
	slli	a4, a4, 3
	addw	a4, a4, s10
	addiw	s6, a4, 16
	slli	s1, s6, 32
	mv	s2, a0
	srli	s1, s1, 32
	mv	s8, sp
	addi	s1, s1, 15
	sd	a7, 24(s0)
	andi	s1, s1, -16
	sd	a6, 16(s0)
	sd	a5, 8(s0)
	sub	s11, sp, s1
	mv	sp, s11
	sub	s5, sp, s1
	mv	sp, s5
	sub	s4, sp, s1
	mv	sp, s4
	sub	s3, sp, s1
	mv	sp, s3
	sw	a1, 4(s11)
	li	a0, 1
	sw	zero, 0(s11)
	sw	a2, 8(s11)
	sw	a0, 12(s11)
	beqz	a4, .LBB2_34
# %bb.1:
	mv	s9, sp
	slli	a0, a3, 32
	srli	t0, a0, 32
	sext.w	a3, a3
	srli	a0, a0, 30
	addi	a0, a0, 15
	andi	a0, a0, -16
	sub	a1, sp, a0
	mv	sp, a1
	addi	a0, s0, 8
	sd	a0, -144(s0)
	blez	a3, .LBB2_17
# %bb.2:
	lw	t6, -144(s0)
	ld	t4, -128(s0)
	ld	ra, -136(s0)
	andi	a7, t0, 1
	li	t1, 1
	bne	a3, t1, .LBB2_7
# %bb.3:
	li	a6, 0
.LBB2_4:                                # %.loopexit
	beqz	a7, .LBB2_17
# %bb.5:
	li	a0, 41
	bgeu	t6, a0, .LBB2_15
# %bb.6:
	addi	a0, t6, 8
	add	ra, t4, t6
	sw	a0, -144(s0)
	j	.LBB2_16
.LBB2_7:
	lui	a0, 524288
	addiw	a0, a0, -2
	and	a6, t0, a0
	li	a2, 0
	neg	t5, a6
	li	t3, 41
	li	t2, 33
	mv	a4, a1
	li	a3, 1
	j	.LBB2_10
.LBB2_8:                                #   in Loop: Header=BB2_10 Depth=1
	addiw	t6, t6, 16
	add	a0, a0, t4
	mv	s1, a3
	sw	t6, -144(s0)
.LBB2_9:                                #   in Loop: Header=BB2_10 Depth=1
	lw	a0, 0(a0)
	slli	s1, s1, 2
	add	s1, s1, a1
	addi	a2, a2, 2
	addi	a3, a3, 2
	addi	a4, a4, 8
	add	a5, t5, a3
	sw	a0, 0(s1)
	beq	a5, t1, .LBB2_4
.LBB2_10:                               # =>This Inner Loop Header: Depth=1
	bgeu	t6, t3, .LBB2_13
# %bb.11:                               #   in Loop: Header=BB2_10 Depth=1
	addiw	a0, t6, 8
	add	a5, t4, t6
	sw	a0, -144(s0)
	lw	a5, 0(a5)
	sw	a5, 0(a4)
	bltu	t6, t2, .LBB2_8
# %bb.12:                               #   in Loop: Header=BB2_10 Depth=1
	ori	s1, a2, 1
	mv	t6, a0
	j	.LBB2_14
.LBB2_13:                               # %.thread
                                        #   in Loop: Header=BB2_10 Depth=1
	addi	a0, ra, 8
	sd	a0, -136(s0)
	lw	a5, 0(ra)
	mv	s1, a3
	mv	ra, a0
	sw	a5, 0(a4)
.LBB2_14:                               #   in Loop: Header=BB2_10 Depth=1
	mv	a0, ra
	addi	ra, ra, 8
	sd	ra, -136(s0)
	j	.LBB2_9
.LBB2_15:
	addi	a0, ra, 8
	sd	a0, -136(s0)
.LBB2_16:
	slli	a6, a6, 2
	lw	a0, 0(ra)
	add	a6, a6, a1
	sw	a0, 0(a6)
.LBB2_17:
	addi	a0, s11, 16
	slli	a2, t0, 2
	call	memcpy
	sext.w	s7, s7
	blez	s7, .LBB2_33
# %bb.18:
	lw	t4, -144(s0)
	add	a0, s11, s10
	ld	t1, -128(s0)
	addi	t3, a0, 16
	ld	a1, -136(s0)
	andi	a6, s7, 1
	li	a3, 1
	bne	s7, a3, .LBB2_20
# %bb.19:
	li	a3, 0
	bnez	a6, .LBB2_29
	j	.LBB2_33
.LBB2_20:
	li	a5, 0
	lui	a4, 524288
	slli	a3, a3, 32
	addiw	a4, a4, -2
	addi	t2, a3, -16
	and	s1, s7, a4
	li	a7, 41
	li	t0, 33
	j	.LBB2_23
.LBB2_21:                               #   in Loop: Header=BB2_23 Depth=1
	addiw	t4, t4, 16
	add	a3, a3, t1
	sw	t4, -144(s0)
.LBB2_22:                               #   in Loop: Header=BB2_23 Depth=1
	ld	a0, 0(a3)
	add	a4, a4, t3
	addi	a5, a5, 16
	addi	s1, s1, -2
	sd	a0, 8(a4)
	beqz	s1, .LBB2_28
.LBB2_23:                               # =>This Inner Loop Header: Depth=1
	bgeu	t4, a7, .LBB2_26
# %bb.24:                               #   in Loop: Header=BB2_23 Depth=1
	addiw	a3, t4, 8
	add	a0, t1, t4
	sw	a3, -144(s0)
	and	a4, a5, t2
	ld	a0, 0(a0)
	add	a2, t3, a4
	sd	a0, 0(a2)
	bltu	t4, t0, .LBB2_21
# %bb.25:                               #   in Loop: Header=BB2_23 Depth=1
	mv	t4, a3
	j	.LBB2_27
.LBB2_26:                               # %.thread30
                                        #   in Loop: Header=BB2_23 Depth=1
	addi	a3, a1, 8
	and	a4, a5, t2
	sd	a3, -136(s0)
	ld	a0, 0(a1)
	add	a2, t3, a4
	mv	a1, a3
	sd	a0, 0(a2)
.LBB2_27:                               #   in Loop: Header=BB2_23 Depth=1
	mv	a3, a1
	addi	a1, a1, 8
	sd	a1, -136(s0)
	j	.LBB2_22
.LBB2_28:
	slli	s7, s7, 3
	and	a3, s7, t2
	beqz	a6, .LBB2_33
.LBB2_29:
	li	a4, 41
	bgeu	t4, a4, .LBB2_31
# %bb.30:
	addi	a4, t4, 8
	add	a1, t1, t4
	sw	a4, -144(s0)
	j	.LBB2_32
.LBB2_31:
	addi	a2, a1, 8
	sd	a2, -136(s0)
.LBB2_32:
	ld	a1, 0(a1)
	add	a0, t3, a3
	sd	a1, 0(a0)
.LBB2_33:
	mv	sp, s9
.LBB2_34:
.Lpcrel_hi0:
	auipc	s7, %pcrel_hi(scheduler_initialized)
.Lpcrel_hi1:
	auipc	a0, %pcrel_hi(thread_list)
	addi	s10, a0, %pcrel_lo(.Lpcrel_hi1)
.Lpcrel_hi2:
	auipc	s9, %pcrel_hi(shared_mem)
	lbu	a0, %pcrel_lo(.Lpcrel_hi0)(s7)
	bnez	a0, .LBB2_37
# %bb.35:
	# 使用malloc替代mmap
    li	a0, 8                         # 分配共享数据结构的内存
    li	s1, -1
    call	malloc
    sd	a0, %pcrel_lo(.Lpcrel_hi2)(s9)
    beq	a0, s1, .LBB2_133
# %bb.36:
	sw	zero, 0(a0)
.Lpcrel_hi3:
	auipc	a1, %pcrel_hi(next_thread_id)
	li	a2, 1
	sw	zero, 4(a0)
	sd	zero, 0(s10)
	sw	a2, %pcrel_lo(.Lpcrel_hi3)(a1)
	sb	a2, %pcrel_lo(.Lpcrel_hi0)(s7)
.LBB2_37:
	mv	a0, s5
	mv	a1, s11
	mv	a2, s6
	call	memcpy
	mv	a0, s4
	mv	a1, s11
	mv	a2, s6
	call	memcpy
	mv	a0, s3
	mv	a1, s11
	mv	a2, s6
	call	memcpy
	li	a0, 1
	li	a1, 2
	li	a2, 3
	sw	a0, 0(s5)
	sw	a1, 0(s4)
	sw	a2, 0(s3)
	addi	a0, s0, -176
	mv	a2, s2
	mv	a3, s11
	call	lsccll.lib.parallel.thread_create
	addi	a0, s0, -168
	mv	a2, s2
	mv	a3, s5
	call	lsccll.lib.parallel.thread_create
	addi	a0, s0, -160
	mv	a2, s2
	mv	a3, s4
	call	lsccll.lib.parallel.thread_create
	addi	a0, s0, -152
	mv	a2, s2
	mv	a3, s3
	call	lsccll.lib.parallel.thread_create
	ld	s1, -176(s0)
	beqz	s1, .LBB2_60
# %bb.38:
	beqz s1, .LBB2_60    # 检查线程控制块是否为空
	lw t0, 4(s1)         # 加载线程状态
	li t1, 2             # 状态2表示已完成
	bne t0, t1, .LBB2_60 # 如果未完成，跳过清理逻辑
# %bb.39:
	ld	a1, 0(s10)
	beqz	a1, .LBB2_59
# %bb.40:
	beq	a1, s1, .LBB2_58
.LBB2_41:                               # %.preheader.i
                                        # =>This Inner Loop Header: Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_59
# %bb.42:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_57
# %bb.43:                               # %.preheader.i.1
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_59
# %bb.44:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_57
# %bb.45:                               # %.preheader.i.2
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_59
# %bb.46:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_57
# %bb.47:                               # %.preheader.i.3
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_59
# %bb.48:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_57
# %bb.49:                               # %.preheader.i.4
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_59
# %bb.50:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_57
# %bb.51:                               # %.preheader.i.5
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_59
# %bb.52:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_57
# %bb.53:                               # %.preheader.i.6
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_59
# %bb.54:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_57
# %bb.55:                               # %.preheader.i.7
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_59
# %bb.56:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a2
	bne	a1, s1, .LBB2_41
.LBB2_57:                               # %.loopexit4.i.loopexit
	addi	s10, a0, 40
.LBB2_58:                               # %.loopexit4.i
	ld	a0, 40(s1)
	sd	a0, 0(s10)
.LBB2_59:                               # %.loopexit.i
	mv	a0, s1
	call	free
	ld	a0, %pcrel_lo(.Lpcrel_hi2)(s9)
	li	a1, -1
	amoadd.w.aqrl	zero, a1, (a0)
.LBB2_60:                               # %lsccll.lib.parallel.thread_join.exit
	ld	s1, -168(s0)
	beqz	s1, .LBB2_83
# %bb.61:
	beqz s1, .LBB2_83    # 检查线程控制块是否为空
	lw t0, 4(s1)         # 加载线程状态
	li t1, 2             # 状态2表示已完成
	bne t0, t1, .LBB2_83 # 如果未完成，跳过清理逻辑
# %bb.62:
.Lpcrel_hi4:
	auipc	a0, %pcrel_hi(thread_list)
	addi	a0, a0, %pcrel_lo(.Lpcrel_hi4)
	ld	a1, 0(a0)
	beqz	a1, .LBB2_82
# %bb.63:
	beq	a1, s1, .LBB2_81
.LBB2_64:                               # %.preheader.i13
                                        # =>This Inner Loop Header: Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_82
# %bb.65:                               #   in Loop: Header=BB2_64 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_80
# %bb.66:                               # %.preheader.i13.1
                                        #   in Loop: Header=BB2_64 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_82
# %bb.67:                               #   in Loop: Header=BB2_64 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_80
# %bb.68:                               # %.preheader.i13.2
                                        #   in Loop: Header=BB2_64 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_82
# %bb.69:                               #   in Loop: Header=BB2_64 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_80
# %bb.70:                               # %.preheader.i13.3
                                        #   in Loop: Header=BB2_64 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_82
# %bb.71:                               #   in Loop: Header=BB2_64 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_80
# %bb.72:                               # %.preheader.i13.4
                                        #   in Loop: Header=BB2_64 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_82
# %bb.73:                               #   in Loop: Header=BB2_64 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_80
# %bb.74:                               # %.preheader.i13.5
                                        #   in Loop: Header=BB2_64 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_82
# %bb.75:                               #   in Loop: Header=BB2_64 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_80
# %bb.76:                               # %.preheader.i13.6
                                        #   in Loop: Header=BB2_64 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_82
# %bb.77:                               #   in Loop: Header=BB2_64 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_80
# %bb.78:                               # %.preheader.i13.7
                                        #   in Loop: Header=BB2_64 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_82
# %bb.79:                               #   in Loop: Header=BB2_64 Depth=1
	mv	a0, a2
	bne	a1, s1, .LBB2_64
.LBB2_80:                               # %.loopexit4.i15.loopexit
	addi	a0, a0, 40
.LBB2_81:                               # %.loopexit4.i15
	ld	a1, 40(s1)
	sd	a1, 0(a0)
.LBB2_82:                               # %.loopexit.i16
	mv	a0, s1
	call	free
	ld	a0, %pcrel_lo(.Lpcrel_hi2)(s9)
	li	a1, -1
	amoadd.w.aqrl	zero, a1, (a0)
.LBB2_83:                               # %lsccll.lib.parallel.thread_join.exit17
	ld	s1, -160(s0)
	beqz	s1, .LBB2_106
# %bb.84:
	beqz s1, .LBB2_106    # 检查线程控制块是否为空
	lw t0, 4(s1)          # 加载线程状态
	li t1, 2              # 状态2表示已完成
	bne t0, t1, .LBB2_106 # 如果未完成，跳过清理逻辑
# %bb.85:
.Lpcrel_hi5:
	auipc	a0, %pcrel_hi(thread_list)
	addi	a0, a0, %pcrel_lo(.Lpcrel_hi5)
	ld	a1, 0(a0)
	beqz	a1, .LBB2_105
# %bb.86:
	beq	a1, s1, .LBB2_104
.LBB2_87:                               # %.preheader.i18
                                        # =>This Inner Loop Header: Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_105
# %bb.88:                               #   in Loop: Header=BB2_87 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_103
# %bb.89:                               # %.preheader.i18.1
                                        #   in Loop: Header=BB2_87 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_105
# %bb.90:                               #   in Loop: Header=BB2_87 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_103
# %bb.91:                               # %.preheader.i18.2
                                        #   in Loop: Header=BB2_87 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_105
# %bb.92:                               #   in Loop: Header=BB2_87 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_103
# %bb.93:                               # %.preheader.i18.3
                                        #   in Loop: Header=BB2_87 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_105
# %bb.94:                               #   in Loop: Header=BB2_87 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_103
# %bb.95:                               # %.preheader.i18.4
                                        #   in Loop: Header=BB2_87 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_105
# %bb.96:                               #   in Loop: Header=BB2_87 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_103
# %bb.97:                               # %.preheader.i18.5
                                        #   in Loop: Header=BB2_87 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_105
# %bb.98:                               #   in Loop: Header=BB2_87 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_103
# %bb.99:                               # %.preheader.i18.6
                                        #   in Loop: Header=BB2_87 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_105
# %bb.100:                              #   in Loop: Header=BB2_87 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_103
# %bb.101:                              # %.preheader.i18.7
                                        #   in Loop: Header=BB2_87 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_105
# %bb.102:                              #   in Loop: Header=BB2_87 Depth=1
	mv	a0, a2
	bne	a1, s1, .LBB2_87
.LBB2_103:                              # %.loopexit4.i20.loopexit
	addi	a0, a0, 40
.LBB2_104:                              # %.loopexit4.i20
	ld	a1, 40(s1)
	sd	a1, 0(a0)
.LBB2_105:                              # %.loopexit.i21
	mv	a0, s1
	call	free
	ld	a0, %pcrel_lo(.Lpcrel_hi2)(s9)
	li	a1, -1
	amoadd.w.aqrl	zero, a1, (a0)
.LBB2_106:                              # %lsccll.lib.parallel.thread_join.exit22
	ld	s1, -152(s0)
	beqz	s1, .LBB2_129
# %bb.107:
	beqz s1, .LBB2_129    # 检查线程控制块是否为空
	lw t0, 4(s1)          # 加载线程状态
	li t1, 2              # 状态2表示已完成
	bne t0, t1, .LBB2_129 # 如果未完成，跳过清理逻辑
# %bb.108:
.Lpcrel_hi6:
	auipc	a0, %pcrel_hi(thread_list)
	addi	a0, a0, %pcrel_lo(.Lpcrel_hi6)
	ld	a1, 0(a0)
	beqz	a1, .LBB2_128
# %bb.109:
	beq	a1, s1, .LBB2_127
.LBB2_110:                              # %.preheader.i23
                                        # =>This Inner Loop Header: Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_128
# %bb.111:                              #   in Loop: Header=BB2_110 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_126
# %bb.112:                              # %.preheader.i23.1
                                        #   in Loop: Header=BB2_110 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_128
# %bb.113:                              #   in Loop: Header=BB2_110 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_126
# %bb.114:                              # %.preheader.i23.2
                                        #   in Loop: Header=BB2_110 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_128
# %bb.115:                              #   in Loop: Header=BB2_110 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_126
# %bb.116:                              # %.preheader.i23.3
                                        #   in Loop: Header=BB2_110 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_128
# %bb.117:                              #   in Loop: Header=BB2_110 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_126
# %bb.118:                              # %.preheader.i23.4
                                        #   in Loop: Header=BB2_110 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_128
# %bb.119:                              #   in Loop: Header=BB2_110 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_126
# %bb.120:                              # %.preheader.i23.5
                                        #   in Loop: Header=BB2_110 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_128
# %bb.121:                              #   in Loop: Header=BB2_110 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_126
# %bb.122:                              # %.preheader.i23.6
                                        #   in Loop: Header=BB2_110 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_128
# %bb.123:                              #   in Loop: Header=BB2_110 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_126
# %bb.124:                              # %.preheader.i23.7
                                        #   in Loop: Header=BB2_110 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_128
# %bb.125:                              #   in Loop: Header=BB2_110 Depth=1
	mv	a0, a2
	bne	a1, s1, .LBB2_110
.LBB2_126:                              # %.loopexit4.i25.loopexit
	addi	a0, a0, 40
.LBB2_127:                              # %.loopexit4.i25
	ld	a1, 40(s1)
	sd	a1, 0(a0)
.LBB2_128:                              # %.loopexit.i26
	mv	a0, s1
	call	free
	ld	a0, %pcrel_lo(.Lpcrel_hi2)(s9)
	li	a1, -1
	amoadd.w.aqrl	zero, a1, (a0)
.LBB2_129:                              # %lsccll.lib.parallel.thread_join.exit27
.Lpcrel_hi7:
	auipc	s2, %pcrel_hi(thread_list)
	ld	s1, %pcrel_lo(.Lpcrel_hi7)(s2)
	bnez	s1, .LBB2_135
.LBB2_130:                              # %.loopexit.i29
	ld	a0, %pcrel_lo(.Lpcrel_hi2)(s9)
	beqz	a0, .LBB2_132
# %bb.131:
	# 使用free替代munmap
    call	free
    sd	zero, %pcrel_lo(.Lpcrel_hi2)(s9)
.LBB2_132:                              # %lsccll.lib.parallel.thread_lib_cleanup.exit
	sd	zero, %pcrel_lo(.Lpcrel_hi7)(s2)
	sb	zero, %pcrel_lo(.Lpcrel_hi0)(s7)
.LBB2_133:                              # %lsccll.lib.parallel.thread_lib_init.exit
	mv	sp, s8
	addi	sp, s0, -176
	ld	ra, 168(sp)                     # 8-byte Folded Reload
	ld	s0, 160(sp)                     # 8-byte Folded Reload
	ld	s1, 152(sp)                     # 8-byte Folded Reload
	ld	s2, 144(sp)                     # 8-byte Folded Reload
	ld	s3, 136(sp)                     # 8-byte Folded Reload
	ld	s4, 128(sp)                     # 8-byte Folded Reload
	ld	s5, 120(sp)                     # 8-byte Folded Reload
	ld	s6, 112(sp)                     # 8-byte Folded Reload
	ld	s7, 104(sp)                     # 8-byte Folded Reload
	ld	s8, 96(sp)                      # 8-byte Folded Reload
	ld	s9, 88(sp)                      # 8-byte Folded Reload
	ld	s10, 80(sp)                     # 8-byte Folded Reload
	ld	s11, 72(sp)                     # 8-byte Folded Reload
	addi	sp, sp, 208
	ret
.LBB2_134:                              #   in Loop: Header=BB2_135 Depth=1
	mv	a0, s1
	call	free
	mv	s1, s3
	beqz	s3, .LBB2_130
.LBB2_135:                              # %.preheader.i28
                                        # =>This Inner Loop Header: Depth=1
	ld	s3, 40(s1)
	lw	a0, 0(s1)
	blez	a0, .LBB2_134
.LBB2_136:                              #   in Loop: Header=BB2_135 Depth=1
    lw	t0, 4(s1)          # 加载线程状态
    li	t1, 2              # 状态2表示已完成
    bne	t0, t1, .LBB2_134  # 如果状态不是已完成，继续清理下一个线程
    j	.LBB2_134          # 状态已完成，继续清理
.Lfunc_end2:
	.cfi_endproc
                                        # -- End function
	.p2align	1
lsccll.lib.parallel.thread_lib_init:    # @lsccll.lib.parallel.thread_lib_init
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)                       # 8-byte Folded Spill
	sd	s0, 0(sp)                       # 8-byte Folded Spill
	.cfi_offset ra, -8
	.cfi_offset s0, -16
.Lpcrel_hi8:
	auipc	s0, %pcrel_hi(scheduler_initialized)
	lbu	a0, %pcrel_lo(.Lpcrel_hi8)(s0)
	beqz	a0, .LBB3_2
# %bb.1:
	li	a1, 0
	j	.LBB3_4
.LBB3_2:
	# 使用malloc替代mmap
    li	a0, 8                         # 分配共享数据结构的内存
    call	malloc
    li	a1, -1
.Lpcrel_hi9:
	auipc	a2, %pcrel_hi(shared_mem)
	sd	a0, %pcrel_lo(.Lpcrel_hi9)(a2)
	beq	a0, a1, .LBB3_4
# %bb.3:
	sw	zero, 0(a0)
.Lpcrel_hi10:
	auipc	a2, %pcrel_hi(thread_list)
.Lpcrel_hi11:
	auipc	a3, %pcrel_hi(next_thread_id)
	li	a4, 1
	sw	zero, 4(a0)
	li	a1, 0
	sd	zero, %pcrel_lo(.Lpcrel_hi10)(a2)
	sw	a4, %pcrel_lo(.Lpcrel_hi11)(a3)
	sb	a4, %pcrel_lo(.Lpcrel_hi8)(s0)
.LBB3_4:
	mv	a0, a1
	ld	ra, 8(sp)                       # 8-byte Folded Reload
	ld	s0, 0(sp)                       # 8-byte Folded Reload
	addi	sp, sp, 16
	ret
.Lfunc_end3:
	.cfi_endproc
                                        # -- End function
	.p2align	1
lsccll.lib.parallel.thread_lib_cleanup: # @lsccll.lib.parallel.thread_lib_cleanup
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -48
	.cfi_def_cfa_offset 48
	sd	ra, 40(sp)                      # 8-byte Folded Spill
	sd	s0, 32(sp)                      # 8-byte Folded Spill
	sd	s1, 24(sp)                      # 8-byte Folded Spill
	sd	s2, 16(sp)                      # 8-byte Folded Spill
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	.cfi_offset s2, -32
.Lpcrel_hi12:
	auipc	s2, %pcrel_hi(thread_list)
	ld	s0, %pcrel_lo(.Lpcrel_hi12)(s2)
	bnez	s0, .LBB4_5
.LBB4_1:                                # %.loopexit
.Lpcrel_hi13:
	auipc	s0, %pcrel_hi(shared_mem)
	ld	a0, %pcrel_lo(.Lpcrel_hi13)(s0)
	beqz	a0, .LBB4_3
# %bb.2:
	# 使用free代替munmap
    call	free
    sd	zero, %pcrel_lo(.Lpcrel_hi13)(s0)
.LBB4_3:
.Lpcrel_hi14:
	auipc	a0, %pcrel_hi(scheduler_initialized)
	sd	zero, %pcrel_lo(.Lpcrel_hi12)(s2)
	sb	zero, %pcrel_lo(.Lpcrel_hi14)(a0)
	ld	ra, 40(sp)                      # 8-byte Folded Reload
	ld	s0, 32(sp)                      # 8-byte Folded Reload
	ld	s1, 24(sp)                      # 8-byte Folded Reload
	ld	s2, 16(sp)                      # 8-byte Folded Reload
	addi	sp, sp, 48
	ret
.LBB4_4:                                #   in Loop: Header=BB4_5 Depth=1
	mv	a0, s0
	call	free
	mv	s0, s1
	beqz	s1, .LBB4_1
.LBB4_5:                                # %.preheader
                                        # =>This Inner Loop Header: Depth=1
	ld	s1, 40(s0)
	lw	a0, 0(s0)
	blez	a0, .LBB4_4
# %bb.6:                                #   in Loop: Header=BB4_5 Depth=1
	lw t0, 4(s0)          # 加载线程状态
	li t1, 2              # 状态2表示已完成
	bne t0, t1, .LBB4_4   # 如果未完成，继续等待
	j	.LBB4_4
.Lfunc_end4:
	.cfi_endproc
                                        # -- End function
	.p2align	1
lsccll.lib.parallel.thread_create:      # @lsccll.lib.parallel.thread_create
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -48
	.cfi_def_cfa_offset 48
	sd	ra, 40(sp)                      # 8-byte Folded Spill
	sd	s0, 32(sp)                      # 8-byte Folded Spill
	sd	s1, 24(sp)                      # 8-byte Folded Spill
	sd	s2, 16(sp)                      # 8-byte Folded Spill
	sd	s3, 8(sp)                       # 8-byte Folded Spill
	sd	s4, 0(sp)                       # 8-byte Folded Spill
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	.cfi_offset s2, -32
	.cfi_offset s3, -40
	.cfi_offset s4, -48
.Lpcrel_hi15:
	auipc	s0, %pcrel_hi(scheduler_initialized)
	mv	s3, a3
	mv	s4, a2
	mv	s2, a0
	lbu	a0, %pcrel_lo(.Lpcrel_hi15)(s0)
	beqz	a0, .LBB5_2
# %bb.1:
.Lpcrel_hi16:
	auipc	a0, %pcrel_hi(shared_mem)
	ld	s1, %pcrel_lo(.Lpcrel_hi16)(a0)
	j	.LBB5_4
.LBB5_2:
	# 使用malloc替代mmap
    li	a0, 8                         # 分配共享数据结构的内存
    li	s1, -1
    call	malloc
.Lpcrel_hi17:
	auipc	a1, %pcrel_hi(shared_mem)
	sd	a0, %pcrel_lo(.Lpcrel_hi17)(a1)
	beq	a0, s1, .LBB5_4
# %bb.3:
	sw	zero, 0(a0)
.Lpcrel_hi18:
	auipc	a1, %pcrel_hi(thread_list)
.Lpcrel_hi19:
	auipc	a2, %pcrel_hi(next_thread_id)
	li	a3, 1
	sw	zero, 4(a0)
	sd	zero, %pcrel_lo(.Lpcrel_hi18)(a1)
	mv	s1, a0
	sw	a3, %pcrel_lo(.Lpcrel_hi19)(a2)
	sb	a3, %pcrel_lo(.Lpcrel_hi15)(s0)
.LBB5_4:
	li	s0, -1
	lw	a0, 0(s1)
	li	a1, 127
	blt	a1, a0, .LBB5_10
# %bb.5:
	li	a0, 48
	call	malloc
	beqz	a0, .LBB5_10
# %bb.6:
	mv	s1, a0
.Lpcrel_hi20:
	auipc	a0, %pcrel_hi(next_thread_id)
	lw	a1, %pcrel_lo(.Lpcrel_hi20)(a0)
	addi	a2, a1, 1
	sw	a2, %pcrel_lo(.Lpcrel_hi20)(a0)
	li t0, -1
    sw t0, 0(s1)         # 初始化线程ID为-1
    sw	a1, 8(s1)
	sw	a1, 8(s1)
.Lpcrel_hi21:
	auipc	a0, %pcrel_hi(thread_list)
	sw	zero, 4(s1)
	sd	s4, 16(s1)
	ld	a1, %pcrel_lo(.Lpcrel_hi21)(a0)
	sd	s3, 24(s1)
	sd	zero, 32(s1)
	sd	a1, 40(s1)
	sd	s1, %pcrel_lo(.Lpcrel_hi21)(a0)
	# 分配栈空间
	li	 a0, 8192                   # 8KB栈大小
	call	malloc
	mv	 s5, a0                     # 保存栈指针
	beqz	 a0, .LBB5_9              # 如果栈分配失败，跳转到错误处理

	# 处理8192的栈大小
	li t0, 8192             # 先加载8192到临时寄存器
	add a1, a0, t0          # 然后相加

	# 设置clone参数的正确方式
	li a0, 0x00000100       # CLONE_VM
	li t0, 0x00000200       # CLONE_FS
	or a0, a0, t0
	li t0, 0x00000400       # CLONE_FILES
	or a0, a0, t0
	li t0, 0x00000800       # CLONE_SIGHAND
	or a0, a0, t0
	li t0, 0x00010000       # CLONE_THREAD
	or a0, a0, t0
	li t0, 0x00040000       # CLONE_SYSVSEM
	or a0, a0, t0

	# 将参数保存到栈上，供子线程使用
	addi a1, a1, -16              # 为参数预留栈空间
	sd s4, 0(a1)                  # 存储函数指针
	sd s3, 8(a1)                  # 存储参数

	# 调用clone
	li a2, 0                      # parent_tidptr = NULL
	li a3, 0                      # child_tidptr = NULL
	li a4, 0                      # tls = NULL
	li a7, 220                    # clone系统调用号
	ecall

	# 检查结果
	sext.w a1, a0
	li s0, -1
	beq a1, s0, .LBB5_9
# %bb.7:
	beqz	a1, .LBB5_11
# %bb.8:
.Lpcrel_hi22:
    auipc a1, %pcrel_hi(shared_mem)
    li a2, 1
    ld a1, %pcrel_lo(.Lpcrel_hi22)(a1)
    sw a0, 0(s1)                # 存储线程ID
    sw a2, 4(s1)                # 设置线程状态
    li s0, 0
    sd s5, 32(s1)               # 存储栈指针，用于后续清理
    amoadd.w.aqrl zero, a2, (a1)
    sd s1, 0(s2)
	j	.LBB5_10
.LBB5_9:
    # 如果s5不为零,表示已分配栈,需要释放
    beqz s5, .LBB5_9_skip_free
    mv a0, s5
    call free
.LBB5_9_skip_free:
    mv a0, s1
    call free
.LBB5_10:
	mv	a0, s0
	ld	ra, 40(sp)                      # 8-byte Folded Reload
	ld	s0, 32(sp)                      # 8-byte Folded Reload
	ld	s1, 24(sp)                      # 8-byte Folded Reload
	ld	s2, 16(sp)                      # 8-byte Folded Reload
	ld	s3, 8(sp)                       # 8-byte Folded Reload
	ld	s4, 0(sp)                       # 8-byte Folded Reload
	addi	sp, sp, 48
	ret
.LBB5_11:
	mv	a0, s4
	mv	a1, s3
	call	lsccll.lib.parallel.thread_process_main
.Lfunc_end5:
	.cfi_endproc
                                        # -- End function
	.p2align	1                               # -- Begin function lsccll.lib.parallel.thread_process_main
lsccll.lib.parallel.thread_process_main: # @lsccll.lib.parallel.thread_process_main
	.cfi_startproc
# %bb.0:
    addi sp, sp, -32
    .cfi_def_cfa_offset 32
    sd ra, 24(sp)               # 8-byte Folded Spill
    sd s0, 16(sp)               # 8-byte Folded Spill
    sd s1, 8(sp)                # 8-byte Folded Spill
    .cfi_offset ra, -8
    .cfi_offset s0, -16
    .cfi_offset s1, -24
    
    # 从栈上加载参数和函数指针
    ld a0, 8(sp)               # 加载参数
    ld a2, 0(sp)               # 加载函数指针
    jalr a2                    # 调用函数
    
    # 保存返回值
    mv s1, a0                  # 保存函数返回值
.Lpcrel_hi23:
	auipc	a1, %pcrel_hi(shared_mem)
	ld	a1, %pcrel_lo(.Lpcrel_hi23)(a1)

	# 遍历线程列表，找到当前线程
.Lfind_thread:
    beqz a1, .Lexit           # 如果没找到，直接退出
    lw t0, 0(a1)              # 加载线程ID
    li t1, -1                 # 特殊值-1,表示当前线程
    beq t0, t1, .Lfound_thread
    ld a1, 40(a1)             # 下一个线程
    j .Lfind_thread
    
.Lfound_thread:
    # 找到当前线程，更新状态和返回值
    li t0, 2                  # 状态2表示线程已结束
    sw t0, 4(a1)              # 更新状态字段
    sw s1, 12(a1)             # 存储返回值(使用偏移量12)

.Lexit:
    auipc a1, %pcrel_hi(shared_mem)
    ld a1, %pcrel_lo(.Lexit)(a1)
    beqz a1, .LBB6_2
# %bb.1:
	addi	a1, a1, 4
	li	a2, 1
	amoadd.w.aqrl	zero, a2, (a1)
.LBB6_2:
	andi	a0, a0, 255
	call	exit
.Lfunc_end6:
	.cfi_endproc
                                        # -- End function
    .p2align	1
lsccll.lib.parallel.thread_join:        # @lsccll.lib.parallel.thread_join
    .cfi_startproc
# %bb.0:
    beqz	a0, .LBB7_14       # 如果线程指针为NULL,返回错误
# %bb.1:
    addi	sp, sp, -32
    .cfi_def_cfa_offset 32
    sd	ra, 24(sp)           # 8-byte Folded Spill
    sd	s0, 16(sp)           # 8-byte Folded Spill
    sd	s1, 8(sp)            # 8-byte Folded Spill
    .cfi_offset ra, -8
    .cfi_offset s0, -16
    .cfi_offset s1, -24
    mv	s0, a1               # 保存返回值指针
    mv	s1, a0               # 保存线程控制块指针

# 自旋等待线程完成
.Lwait_loop:
    lw	t0, 4(s1)            # 加载线程状态
    li	t1, 2                # 状态2表示线程已结束
    beq	t0, t1, .Lthread_done # 如果线程已结束，处理清理工作
    
    # 可以加入一个短暂的延时以减少CPU使用
    li	a0, 1000
.Ldelay_loop:
    addi	a0, a0, -1
    bnez	a0, .Ldelay_loop
    j	.Lwait_loop

.Lthread_done:
    # 线程已经完成，获取返回值
    beqz	s0, .LBB7_cleanup  # 如果不需要返回值，跳过

    # 从线程控制块中获取返回值
    lw	a0, 12(s1)           # 从偏移量12加载返回值
    sd	a0, 0(s0)            # 存储到用户提供的位置

.LBB7_cleanup:
    # 从线程列表中移除节点
.Lpcrel_hi24:
    auipc	a0, %pcrel_hi(thread_list)
    addi	a1, a0, %pcrel_lo(.Lpcrel_hi24)
    ld	a2, 0(a1)
    mv	a0, s1
    beqz	a2, .LBB7_11       # 如果线程列表为空，直接清理
    
    # 下面的代码与原先相同，寻找线程控制块在链表中的位置
    beq	a2, a0, .LBB7_10
.LBB7_3:                               # %.preheader
    ld	a3, 40(a2)
    beqz	a3, .LBB7_11
    mv	a1, a2
    beq	a3, a0, .LBB7_9
    ld	a2, 40(a3)
    beqz	a2, .LBB7_11
    # 剩下的链表遍历代码...与原来相同
    
.LBB7_9:                              # %.loopexit4.loopexit
    addi	a1, a1, 40
.LBB7_10:                             # %.loopexit4
    ld	a2, 40(a0)
    sd	a2, 0(a1)           # 从链表中移除节点

.LBB7_11:                             # %.loopexit
    # 释放线程栈
    ld a1, 32(a0)            # 加载栈指针
    beqz a1, .LBB7_12_skip_stack
    mv s2, a0                # 临时保存线程控制块指针
    mv a0, a1
    call free
    mv a0, s2               # 恢复线程控制块指针
.LBB7_12_skip_stack:
    call	free            # 释放线程控制块
.Lpcrel_hi25:
    auipc	a0, %pcrel_hi(shared_mem)
    ld	a1, %pcrel_lo(.Lpcrel_hi25)(a0)
    li	a2, -1
    li	a0, 0             # 成功返回0
    amoadd.w.aqrl	zero, a2, (a1)
    
    ld	ra, 24(sp)        # 8-byte Folded Reload
    ld	s0, 16(sp)        # 8-byte Folded Reload
    ld	s1, 8(sp)         # 8-byte Folded Reload
    addi	sp, sp, 32
    ret
.LBB7_14:
    li	a0, -1           # 返回错误
    ret
.Lfunc_end7:
    .cfi_endproc
                                        # -- End function
	.p2align	2, 0x0
scheduler_initialized:
	.byte	0                               # 0x0
	.p2align	3, 0x0
shared_mem:
	.quad	0
	.p2align	3, 0x0
thread_list:
	.quad	0
	.p2align	2, 0x0
next_thread_id:
	.word	1                               # 0x1

        )";

        out_ << parallel_support_asm << "\n";
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

        if (!inst->comment.empty()) out_ << " # " << inst->comment;
    }

    void CodeGenerationPass::printASM(MoveInst* inst)
    {
        out_ << "move ";
        printOperand(inst->dst);
        out_ << ", ";
        printOperand(inst->src);
        out_ << ", " << inst->data_type->toString();

        if (!inst->comment.empty()) out_ << " # " << inst->comment;
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

        if (!inst->comment.empty()) out_ << " # " << inst->comment;
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

        if (!inst->comment.empty()) out_ << " # " << inst->comment;
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
