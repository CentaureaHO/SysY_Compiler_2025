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
	.addrsig
