	.attribute	4, 16
	.attribute	5, "rv64i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zifencei2p0"
	.p2align	1
lsccll.lib.memset_i8:                   # @lsccll.lib.memset_i8
# %bb.0:                                # %entry
	tail	memset
.Lfunc_end0:
                                        # -- End function
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
                                        # -- End function
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
	lui	a0, %hi(scheduler_initialized)
	lbu	a0, %lo(scheduler_initialized)(a0)
	bnez	a0, .LBB2_37
# %bb.35:
	lui	a1, 1
	li	a2, 3
	li	a3, 33
	li	a4, -1
	li	a5, 0
	li	s1, -1
	call	mmap
	lui	a1, %hi(shared_mem)
	sd	a0, %lo(shared_mem)(a1)
	beq	a0, s1, .LBB2_137
# %bb.36:
	sw	zero, 0(a0)
	lui	a1, %hi(thread_list)
	lui	a2, %hi(next_thread_id)
	li	a3, 1
	sw	zero, 4(a0)
	lui	a0, %hi(scheduler_initialized)
	sd	zero, %lo(thread_list)(a1)
	sw	a3, %lo(next_thread_id)(a2)
	sb	a3, %lo(scheduler_initialized)(a0)
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
	beqz	s1, .LBB2_61
# %bb.38:
	lw	a0, 0(s1)
	addi	a1, s0, -108
	li	a2, 0
	call	waitpid
	sext.w	a0, a0
	li	a1, -1
	beq	a0, a1, .LBB2_61
# %bb.39:
	lui	a0, %hi(thread_list)
	ld	a1, %lo(thread_list)(a0)
	beqz	a1, .LBB2_60
# %bb.40:
	beq	a1, s1, .LBB2_58
.LBB2_41:                               # %.preheader.i
                                        # =>This Inner Loop Header: Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_60
# %bb.42:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_57
# %bb.43:                               # %.preheader.i.1
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_60
# %bb.44:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_57
# %bb.45:                               # %.preheader.i.2
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_60
# %bb.46:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_57
# %bb.47:                               # %.preheader.i.3
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_60
# %bb.48:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_57
# %bb.49:                               # %.preheader.i.4
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_60
# %bb.50:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_57
# %bb.51:                               # %.preheader.i.5
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_60
# %bb.52:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_57
# %bb.53:                               # %.preheader.i.6
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_60
# %bb.54:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_57
# %bb.55:                               # %.preheader.i.7
                                        #   in Loop: Header=BB2_41 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_60
# %bb.56:                               #   in Loop: Header=BB2_41 Depth=1
	mv	a0, a2
	bne	a1, s1, .LBB2_41
.LBB2_57:                               # %.loopexit4.i.loopexit
	addi	a0, a0, 40
	j	.LBB2_59
.LBB2_58:
	addi	a0, a0, %lo(thread_list)
.LBB2_59:                               # %.loopexit4.i
	ld	a1, 40(s1)
	sd	a1, 0(a0)
.LBB2_60:                               # %.loopexit.i
	mv	a0, s1
	call	free
	lui	a0, %hi(shared_mem)
	ld	a0, %lo(shared_mem)(a0)
	li	a1, -1
	amoadd.w.aqrl	zero, a1, (a0)
.LBB2_61:                               # %lsccll.lib.parallel.thread_join.exit
	ld	s1, -168(s0)
	beqz	s1, .LBB2_85
# %bb.62:
	lw	a0, 0(s1)
	addi	a1, s0, -108
	li	a2, 0
	call	waitpid
	sext.w	a0, a0
	li	a1, -1
	beq	a0, a1, .LBB2_85
# %bb.63:
	lui	a0, %hi(thread_list)
	ld	a1, %lo(thread_list)(a0)
	beqz	a1, .LBB2_84
# %bb.64:
	beq	a1, s1, .LBB2_82
.LBB2_65:                               # %.preheader.i13
                                        # =>This Inner Loop Header: Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_84
# %bb.66:                               #   in Loop: Header=BB2_65 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_81
# %bb.67:                               # %.preheader.i13.1
                                        #   in Loop: Header=BB2_65 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_84
# %bb.68:                               #   in Loop: Header=BB2_65 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_81
# %bb.69:                               # %.preheader.i13.2
                                        #   in Loop: Header=BB2_65 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_84
# %bb.70:                               #   in Loop: Header=BB2_65 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_81
# %bb.71:                               # %.preheader.i13.3
                                        #   in Loop: Header=BB2_65 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_84
# %bb.72:                               #   in Loop: Header=BB2_65 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_81
# %bb.73:                               # %.preheader.i13.4
                                        #   in Loop: Header=BB2_65 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_84
# %bb.74:                               #   in Loop: Header=BB2_65 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_81
# %bb.75:                               # %.preheader.i13.5
                                        #   in Loop: Header=BB2_65 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_84
# %bb.76:                               #   in Loop: Header=BB2_65 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_81
# %bb.77:                               # %.preheader.i13.6
                                        #   in Loop: Header=BB2_65 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_84
# %bb.78:                               #   in Loop: Header=BB2_65 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_81
# %bb.79:                               # %.preheader.i13.7
                                        #   in Loop: Header=BB2_65 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_84
# %bb.80:                               #   in Loop: Header=BB2_65 Depth=1
	mv	a0, a2
	bne	a1, s1, .LBB2_65
.LBB2_81:                               # %.loopexit4.i15.loopexit
	addi	a0, a0, 40
	j	.LBB2_83
.LBB2_82:
	addi	a0, a0, %lo(thread_list)
.LBB2_83:                               # %.loopexit4.i15
	ld	a1, 40(s1)
	sd	a1, 0(a0)
.LBB2_84:                               # %.loopexit.i16
	mv	a0, s1
	call	free
	lui	a0, %hi(shared_mem)
	ld	a0, %lo(shared_mem)(a0)
	li	a1, -1
	amoadd.w.aqrl	zero, a1, (a0)
.LBB2_85:                               # %lsccll.lib.parallel.thread_join.exit17
	ld	s1, -160(s0)
	beqz	s1, .LBB2_109
# %bb.86:
	lw	a0, 0(s1)
	addi	a1, s0, -108
	li	a2, 0
	call	waitpid
	sext.w	a0, a0
	li	a1, -1
	beq	a0, a1, .LBB2_109
# %bb.87:
	lui	a0, %hi(thread_list)
	ld	a1, %lo(thread_list)(a0)
	beqz	a1, .LBB2_108
# %bb.88:
	beq	a1, s1, .LBB2_106
.LBB2_89:                               # %.preheader.i18
                                        # =>This Inner Loop Header: Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_108
# %bb.90:                               #   in Loop: Header=BB2_89 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_105
# %bb.91:                               # %.preheader.i18.1
                                        #   in Loop: Header=BB2_89 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_108
# %bb.92:                               #   in Loop: Header=BB2_89 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_105
# %bb.93:                               # %.preheader.i18.2
                                        #   in Loop: Header=BB2_89 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_108
# %bb.94:                               #   in Loop: Header=BB2_89 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_105
# %bb.95:                               # %.preheader.i18.3
                                        #   in Loop: Header=BB2_89 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_108
# %bb.96:                               #   in Loop: Header=BB2_89 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_105
# %bb.97:                               # %.preheader.i18.4
                                        #   in Loop: Header=BB2_89 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_108
# %bb.98:                               #   in Loop: Header=BB2_89 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_105
# %bb.99:                               # %.preheader.i18.5
                                        #   in Loop: Header=BB2_89 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_108
# %bb.100:                              #   in Loop: Header=BB2_89 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_105
# %bb.101:                              # %.preheader.i18.6
                                        #   in Loop: Header=BB2_89 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_108
# %bb.102:                              #   in Loop: Header=BB2_89 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_105
# %bb.103:                              # %.preheader.i18.7
                                        #   in Loop: Header=BB2_89 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_108
# %bb.104:                              #   in Loop: Header=BB2_89 Depth=1
	mv	a0, a2
	bne	a1, s1, .LBB2_89
.LBB2_105:                              # %.loopexit4.i20.loopexit
	addi	a0, a0, 40
	j	.LBB2_107
.LBB2_106:
	addi	a0, a0, %lo(thread_list)
.LBB2_107:                              # %.loopexit4.i20
	ld	a1, 40(s1)
	sd	a1, 0(a0)
.LBB2_108:                              # %.loopexit.i21
	mv	a0, s1
	call	free
	lui	a0, %hi(shared_mem)
	ld	a0, %lo(shared_mem)(a0)
	li	a1, -1
	amoadd.w.aqrl	zero, a1, (a0)
.LBB2_109:                              # %lsccll.lib.parallel.thread_join.exit22
	ld	s1, -152(s0)
	beqz	s1, .LBB2_133
# %bb.110:
	lw	a0, 0(s1)
	addi	a1, s0, -108
	li	a2, 0
	call	waitpid
	sext.w	a0, a0
	li	a1, -1
	beq	a0, a1, .LBB2_133
# %bb.111:
	lui	a0, %hi(thread_list)
	ld	a1, %lo(thread_list)(a0)
	beqz	a1, .LBB2_132
# %bb.112:
	beq	a1, s1, .LBB2_130
.LBB2_113:                              # %.preheader.i23
                                        # =>This Inner Loop Header: Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_132
# %bb.114:                              #   in Loop: Header=BB2_113 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_129
# %bb.115:                              # %.preheader.i23.1
                                        #   in Loop: Header=BB2_113 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_132
# %bb.116:                              #   in Loop: Header=BB2_113 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_129
# %bb.117:                              # %.preheader.i23.2
                                        #   in Loop: Header=BB2_113 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_132
# %bb.118:                              #   in Loop: Header=BB2_113 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_129
# %bb.119:                              # %.preheader.i23.3
                                        #   in Loop: Header=BB2_113 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_132
# %bb.120:                              #   in Loop: Header=BB2_113 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_129
# %bb.121:                              # %.preheader.i23.4
                                        #   in Loop: Header=BB2_113 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_132
# %bb.122:                              #   in Loop: Header=BB2_113 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_129
# %bb.123:                              # %.preheader.i23.5
                                        #   in Loop: Header=BB2_113 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_132
# %bb.124:                              #   in Loop: Header=BB2_113 Depth=1
	mv	a0, a2
	beq	a1, s1, .LBB2_129
# %bb.125:                              # %.preheader.i23.6
                                        #   in Loop: Header=BB2_113 Depth=1
	ld	a2, 40(a1)
	beqz	a2, .LBB2_132
# %bb.126:                              #   in Loop: Header=BB2_113 Depth=1
	mv	a0, a1
	beq	a2, s1, .LBB2_129
# %bb.127:                              # %.preheader.i23.7
                                        #   in Loop: Header=BB2_113 Depth=1
	ld	a1, 40(a2)
	beqz	a1, .LBB2_132
# %bb.128:                              #   in Loop: Header=BB2_113 Depth=1
	mv	a0, a2
	bne	a1, s1, .LBB2_113
.LBB2_129:                              # %.loopexit4.i25.loopexit
	addi	a0, a0, 40
	j	.LBB2_131
.LBB2_130:
	addi	a0, a0, %lo(thread_list)
.LBB2_131:                              # %.loopexit4.i25
	ld	a1, 40(s1)
	sd	a1, 0(a0)
.LBB2_132:                              # %.loopexit.i26
	mv	a0, s1
	call	free
	lui	a0, %hi(shared_mem)
	ld	a0, %lo(shared_mem)(a0)
	li	a1, -1
	amoadd.w.aqrl	zero, a1, (a0)
.LBB2_133:                              # %lsccll.lib.parallel.thread_join.exit27
	lui	a0, %hi(thread_list)
	ld	s1, %lo(thread_list)(a0)
	bnez	s1, .LBB2_139
.LBB2_134:                              # %.loopexit.i29
	lui	s1, %hi(shared_mem)
	ld	a0, %lo(shared_mem)(s1)
	beqz	a0, .LBB2_136
# %bb.135:
	lui	a1, 1
	call	munmap
	sd	zero, %lo(shared_mem)(s1)
.LBB2_136:                              # %lsccll.lib.parallel.thread_lib_cleanup.exit
	lui	a0, %hi(thread_list)
	lui	a1, %hi(scheduler_initialized)
	sd	zero, %lo(thread_list)(a0)
	sb	zero, %lo(scheduler_initialized)(a1)
.LBB2_137:                              # %lsccll.lib.parallel.thread_lib_init.exit
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
.LBB2_138:                              #   in Loop: Header=BB2_139 Depth=1
	mv	a0, s1
	call	free
	mv	s1, s2
	beqz	s2, .LBB2_134
.LBB2_139:                              # %.preheader.i28
                                        # =>This Inner Loop Header: Depth=1
	ld	s2, 40(s1)
	lw	a0, 0(s1)
	blez	a0, .LBB2_138
# %bb.140:                              #   in Loop: Header=BB2_139 Depth=1
	addi	a1, s0, -108
	li	a2, 0
	call	waitpid
	j	.LBB2_138
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
	lui	a0, %hi(scheduler_initialized)
	lbu	a0, %lo(scheduler_initialized)(a0)
	beqz	a0, .LBB3_2
# %bb.1:
	li	s0, 0
	j	.LBB3_4
.LBB3_2:
	lui	a1, 1
	li	a2, 3
	li	a3, 33
	li	a4, -1
	li	a5, 0
	li	s0, -1
	call	mmap
	lui	a1, %hi(shared_mem)
	sd	a0, %lo(shared_mem)(a1)
	beq	a0, s0, .LBB3_4
# %bb.3:
	lui	a1, %hi(next_thread_id)
	li	a2, 1
	sw	zero, 0(a0)
	lui	a3, %hi(scheduler_initialized)
	sw	zero, 4(a0)
	lui	a0, %hi(thread_list)
	li	s0, 0
	sw	a2, %lo(next_thread_id)(a1)
	sb	a2, %lo(scheduler_initialized)(a3)
	sd	zero, %lo(thread_list)(a0)
.LBB3_4:
	mv	a0, s0
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
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	sd	ra, 24(sp)                      # 8-byte Folded Spill
	sd	s0, 16(sp)                      # 8-byte Folded Spill
	sd	s1, 8(sp)                       # 8-byte Folded Spill
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	lui	a0, %hi(thread_list)
	ld	s0, %lo(thread_list)(a0)
	bnez	s0, .LBB4_5
.LBB4_1:                                # %.loopexit
	lui	s0, %hi(shared_mem)
	ld	a0, %lo(shared_mem)(s0)
	beqz	a0, .LBB4_3
# %bb.2:
	lui	a1, 1
	call	munmap
	sd	zero, %lo(shared_mem)(s0)
.LBB4_3:
	lui	a0, %hi(thread_list)
	lui	a1, %hi(scheduler_initialized)
	sd	zero, %lo(thread_list)(a0)
	sb	zero, %lo(scheduler_initialized)(a1)
	ld	ra, 24(sp)                      # 8-byte Folded Reload
	ld	s0, 16(sp)                      # 8-byte Folded Reload
	ld	s1, 8(sp)                       # 8-byte Folded Reload
	addi	sp, sp, 32
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
	addi	a1, sp, 4
	li	a2, 0
	call	waitpid
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
	lui	a1, %hi(scheduler_initialized)
	mv	s3, a3
	mv	s4, a2
	mv	s2, a0
	lbu	a0, %lo(scheduler_initialized)(a1)
	beqz	a0, .LBB5_2
# %bb.1:
	lui	a0, %hi(shared_mem)
	ld	s1, %lo(shared_mem)(a0)
	j	.LBB5_4
.LBB5_2:
	lui	a1, 1
	li	a2, 3
	li	a3, 33
	li	a4, -1
	li	a5, 0
	li	s1, -1
	call	mmap
	lui	a1, %hi(shared_mem)
	sd	a0, %lo(shared_mem)(a1)
	beq	a0, s1, .LBB5_4
# %bb.3:
	sw	zero, 0(a0)
	lui	a1, %hi(thread_list)
	lui	a2, %hi(next_thread_id)
	li	a3, 1
	sw	zero, 4(a0)
	lui	a4, %hi(scheduler_initialized)
	sd	zero, %lo(thread_list)(a1)
	mv	s1, a0
	sw	a3, %lo(next_thread_id)(a2)
	sb	a3, %lo(scheduler_initialized)(a4)
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
	lui	a0, %hi(next_thread_id)
	lw	a1, %lo(next_thread_id)(a0)
	addi	a2, a1, 1
	sw	a2, %lo(next_thread_id)(a0)
	sw	a1, 8(s1)
	lui	a0, %hi(thread_list)
	sw	zero, 4(s1)
	sd	s4, 16(s1)
	ld	a1, %lo(thread_list)(a0)
	sd	s3, 24(s1)
	sd	zero, 32(s1)
	sd	a1, 40(s1)
	sd	s1, %lo(thread_list)(a0)
	call	fork
	sext.w	a1, a0
	li	s0, -1
	beq	a1, s0, .LBB5_9
# %bb.7:
	beqz	a1, .LBB5_11
# %bb.8:
	lui	a1, %hi(shared_mem)
	li	a2, 1
	ld	a1, %lo(shared_mem)(a1)
	sw	a0, 0(s1)
	sw	a2, 4(s1)
	li	s0, 0
	amoadd.w.aqrl	zero, a2, (a1)
	sd	s1, 0(s2)
	j	.LBB5_10
.LBB5_9:
	mv	a0, s1
	call	free
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
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)                       # 8-byte Folded Spill
	.cfi_offset ra, -8
	mv	a2, a0
	mv	a0, a1
	jalr	a2
	lui	a1, %hi(shared_mem)
	ld	a1, %lo(shared_mem)(a1)
	beqz	a1, .LBB6_2
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
	beqz	a0, .LBB7_23
# %bb.1:
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	sd	ra, 24(sp)                      # 8-byte Folded Spill
	sd	s0, 16(sp)                      # 8-byte Folded Spill
	sd	s1, 8(sp)                       # 8-byte Folded Spill
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	mv	s0, a1
	mv	s1, a0
	lw	a0, 0(a0)
	addi	a1, sp, 4
	li	a2, 0
	call	waitpid
	sext.w	a1, a0
	li	a0, -1
	beq	a1, a0, .LBB7_27
# %bb.2:
	beqz	s0, .LBB7_4
# %bb.3:
	li	a0, 0
	lw	a1, 4(sp)
	andi	a2, a1, 127
	slli	a1, a1, 48
	bnez	a2, .LBB7_29
# %bb.28:
	srli	a0, a1, 56
.LBB7_29:
	sd	a0, 0(s0)
.LBB7_4:
	lui	a1, %hi(thread_list)
	ld	a2, %lo(thread_list)(a1)
	mv	a0, s1
	beqz	a2, .LBB7_26
# %bb.5:
	beq	a2, a0, .LBB7_24
.LBB7_6:                                # %.preheader
                                        # =>This Inner Loop Header: Depth=1
	ld	a3, 40(a2)
	beqz	a3, .LBB7_26
# %bb.7:                                #   in Loop: Header=BB7_6 Depth=1
	mv	a1, a2
	beq	a3, a0, .LBB7_22
# %bb.8:                                # %.preheader.1
                                        #   in Loop: Header=BB7_6 Depth=1
	ld	a2, 40(a3)
	beqz	a2, .LBB7_26
# %bb.9:                                #   in Loop: Header=BB7_6 Depth=1
	mv	a1, a3
	beq	a2, a0, .LBB7_22
# %bb.10:                               # %.preheader.2
                                        #   in Loop: Header=BB7_6 Depth=1
	ld	a3, 40(a2)
	beqz	a3, .LBB7_26
# %bb.11:                               #   in Loop: Header=BB7_6 Depth=1
	mv	a1, a2
	beq	a3, a0, .LBB7_22
# %bb.12:                               # %.preheader.3
                                        #   in Loop: Header=BB7_6 Depth=1
	ld	a2, 40(a3)
	beqz	a2, .LBB7_26
# %bb.13:                               #   in Loop: Header=BB7_6 Depth=1
	mv	a1, a3
	beq	a2, a0, .LBB7_22
# %bb.14:                               # %.preheader.4
                                        #   in Loop: Header=BB7_6 Depth=1
	ld	a3, 40(a2)
	beqz	a3, .LBB7_26
# %bb.15:                               #   in Loop: Header=BB7_6 Depth=1
	mv	a1, a2
	beq	a3, a0, .LBB7_22
# %bb.16:                               # %.preheader.5
                                        #   in Loop: Header=BB7_6 Depth=1
	ld	a2, 40(a3)
	beqz	a2, .LBB7_26
# %bb.17:                               #   in Loop: Header=BB7_6 Depth=1
	mv	a1, a3
	beq	a2, a0, .LBB7_22
# %bb.18:                               # %.preheader.6
                                        #   in Loop: Header=BB7_6 Depth=1
	ld	a3, 40(a2)
	beqz	a3, .LBB7_26
# %bb.19:                               #   in Loop: Header=BB7_6 Depth=1
	mv	a1, a2
	beq	a3, a0, .LBB7_22
# %bb.20:                               # %.preheader.7
                                        #   in Loop: Header=BB7_6 Depth=1
	ld	a2, 40(a3)
	beqz	a2, .LBB7_26
# %bb.21:                               #   in Loop: Header=BB7_6 Depth=1
	mv	a1, a3
	bne	a2, a0, .LBB7_6
.LBB7_22:                               # %.loopexit4.loopexit
	addi	a1, a1, 40
	j	.LBB7_25
.LBB7_23:
	li	a0, -1
	ret
.LBB7_24:
	addi	a1, a1, %lo(thread_list)
.LBB7_25:                               # %.loopexit4
	ld	a2, 40(a0)
	sd	a2, 0(a1)
.LBB7_26:                               # %.loopexit
	call	free
	lui	a0, %hi(shared_mem)
	ld	a1, %lo(shared_mem)(a0)
	li	a2, -1
	li	a0, 0
	amoadd.w.aqrl	zero, a2, (a1)
.LBB7_27:
	ld	ra, 24(sp)                      # 8-byte Folded Reload
	ld	s0, 16(sp)                      # 8-byte Folded Reload
	ld	s1, 8(sp)                       # 8-byte Folded Reload
	addi	sp, sp, 32
	ret
.Lfunc_end7:
	.cfi_endproc
                                        # -- End function
	.p2align	1
lsccll.lib.parallel.thread_exit:        # @lsccll.lib.parallel.thread_exit
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)                       # 8-byte Folded Spill
	.cfi_offset ra, -8
	andi	a0, a0, 255
	call	exit
.Lfunc_end8:
	.cfi_endproc
                                        # -- End function
	.p2align	1
lsccll.lib.parallel.thread_self:        # @lsccll.lib.parallel.thread_self
	.cfi_startproc
# %bb.0:
	li	a0, 0
	ret
.Lfunc_end9:
	.cfi_endproc
                                        # -- End function
	.p2align	1
lsccll.lib.parallel.thread_yield:       # @lsccll.lib.parallel.thread_yield
	.cfi_startproc
# %bb.0:
	addi	sp, sp, -16
	.cfi_def_cfa_offset 16
	sd	ra, 8(sp)                       # 8-byte Folded Spill
	.cfi_offset ra, -8
	call	sched_yield
	sext.w	a0, a0
	ld	ra, 8(sp)                       # 8-byte Folded Reload
	addi	sp, sp, 16
	ret
.Lfunc_end10:
	.cfi_endproc
                                        # -- End function
	.p2align	1
lsccll.lib.parallel.get_thread_count:   # @lsccll.lib.parallel.get_thread_count
	.cfi_startproc
# %bb.0:
	lui	a0, %hi(shared_mem)
	ld	a0, %lo(shared_mem)(a0)
	beqz	a0, .LBB11_2
# %bb.1:
	lw	a0, 0(a0)
.LBB11_2:
	ret
.Lfunc_end11:
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
	.addrsig
