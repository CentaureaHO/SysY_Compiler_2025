declare i32 @getint()
declare i32 @getch()
declare i32 @getarray(ptr)
declare float @getfloat()
declare i32 @getfarray(ptr)
declare void @putint(i32)
declare void @putch(i32)
declare void @putarray(i32, ptr)
declare void @putfloat(float)
declare void @putfarray(i32, ptr)
declare void @_sysy_starttime(i32)
declare void @_sysy_stoptime(i32)
declare void @llvm.memset.p0.i32(ptr, i8, i32, i1)


define i32 @main()
{
Block0:
	br label %Block1
Block1:
	br label %Block10
Block2: ; Loop 0 header
	%reg_51 = phi i32 [ %reg_57, %Block7 ], [ 0, %Block14 ]
	%reg_49 = phi i32 [ %reg_47, %Block7 ], [ 0, %Block14 ]
	br label %Block3
Block3: ; Loop 0 body
	br label %Block11
Block4: ; Loop 0 exit target
	ret i32 0
Block5: ; Loop 1 header (nested depth: 1)
	%reg_50 = phi i32 [ %reg_44, %Block9 ], [ %reg_51, %Block12 ]
	br label %Block6
Block6: ; Loop 1 body (nested depth: 1)
	%reg_21 = sub i32 100, %reg_49
	%reg_23 = sub i32 %reg_21, %reg_50
	%reg_26 = mul i32 5, %reg_49
	%reg_29 = add i32 %reg_50, 0
	%reg_30 = add i32 %reg_26, %reg_29
	%reg_77 = ashr i32 %reg_23, 31
	%reg_78 = and i32 %reg_77, 1
	%reg_79 = add i32 %reg_23, %reg_78
	%reg_33 = ashr i32 %reg_79, 1
	%reg_34 = add i32 %reg_30, %reg_33
	%reg_36 = icmp eq i32 %reg_34, 100
	br i1 %reg_36, label %Block8, label %Block9
Block7: ; Loop 0 latch
	%reg_57 = phi i32 [ %reg_51, %Block11 ], [ %reg_44, %Block13 ]
	%reg_47 = add i32 %reg_49, 1
	%reg_67 = icmp slt i32 %reg_47, 21
	br i1 %reg_67, label %Block2, label %Block15
Block8: ; Loop 1 body (nested depth: 1)
	call void @putint(i32 %reg_49)
	call void @putint(i32 %reg_50)
	call void @putint(i32 %reg_23)
	call void @putch(i32 10)
	br label %Block9
Block9: ; Loop 1 latch (nested depth: 1)
	%reg_44 = add i32 %reg_50, 1
	%reg_76 = icmp slt i32 %reg_44, %reg_72
	br i1 %reg_76, label %Block5, label %Block13
Block10:
	br i1 1, label %Block14, label %Block4
Block11: ; Loop 0 body
	%reg_72 = sub i32 101, %reg_49
	%reg_73 = icmp slt i32 %reg_51, %reg_72
	br i1 %reg_73, label %Block12, label %Block7
Block12: ; Loop 1 preheader (nested depth: 1)
	br label %Block5 ; Transfer control to target block
Block13: ; Loop 1 dedicated exit (nested depth: 1)
	br label %Block7 ; Transfer control to target block
Block14: ; Loop 0 preheader
	br label %Block2 ; Transfer control to target block
Block15: ; Loop 0 dedicated exit
	br label %Block4 ; Transfer control to target block
}
