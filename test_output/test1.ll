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

@V = global i32 4
@space = global i32 32
@LF = global i32 10

define void @printSolution(ptr %reg_0)
{
Block0:
	br label %Block1
Block1:
	br label %Block5
Block2: ; Loop 0 header
	%reg_24 = phi ptr [ %reg_23, %Block6 ], [ %reg_25, %Block3 ]
	%reg_14 = phi i32 [ %reg_12, %Block3 ], [ 0, %Block6 ]
	br label %Block3
Block3: ; Loop 0 latch
	%reg_8 = load i32, ptr %reg_24
	call void @putint(i32 %reg_8)
	%reg_9 = load i32, ptr @space
	call void @putch(i32 %reg_9)
	%reg_12 = add i32 %reg_14, 1
	%reg_18 = load i32, ptr @V
	%reg_19 = icmp slt i32 %reg_12, %reg_18
	%reg_25 = getelementptr i32, ptr %reg_24, i32 1
	br i1 %reg_19, label %Block2, label %Block7
Block4:
	%reg_13 = load i32, ptr @LF
	call void @putch(i32 %reg_13)
	ret void
Block5:
	%reg_16 = load i32, ptr @V
	%reg_17 = icmp slt i32 0, %reg_16
	br i1 %reg_17, label %Block6, label %Block4
Block6: ; Loop 0 preheader
	%reg_23 = getelementptr i32, ptr %reg_0, i32 0
	br label %Block2
Block7: ; Loop 0 exit target
	br label %Block4
}

define void @printMessage()
{
Block0:
	br label %Block1
Block1:
	call void @putch(i32 78)
	call void @putch(i32 111)
	call void @putch(i32 116)
	%reg_3 = load i32, ptr @space
	call void @putch(i32 %reg_3)
	call void @putch(i32 101)
	call void @putch(i32 120)
	call void @putch(i32 105)
	call void @putch(i32 115)
	call void @putch(i32 116)
	ret void
}

define i32 @isSafe(ptr %reg_0, ptr %reg_1)
{
Block0:
	br label %Block1
Block1: ; Loop 0 preheader
	br label %Block2
Block2: ; Loop 0 header
	%reg_35 = phi i32 [ 0, %Block1 ], [ %reg_10, %Block7 ]
	%reg_5 = load i32, ptr @V
	%reg_6 = icmp slt i32 %reg_35, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3: ; Loop 0 body
	%reg_10 = add i32 %reg_35, 1
	br label %Block5
Block4: ; Loop 0 exit target
	br label %Block11
Block5: ; Loop 0 body
	%reg_37 = phi i32 [ %reg_10, %Block3 ], [ %reg_29, %Block9 ]
	%reg_38 = phi i32 [ 0, %Block3 ], [ %reg_39, %Block9 ]
	%reg_12 = load i32, ptr @V
	%reg_13 = icmp slt i32 %reg_37, %reg_12
	br i1 %reg_13, label %Block6, label %Block7
Block6: ; Loop 0 body
	%reg_16 = getelementptr [4 x i32], ptr %reg_0, i32 %reg_35, i32 %reg_37
	%reg_17 = load i32, ptr %reg_16
	%reg_18 = icmp ne i32 %reg_17, 0
	br i1 %reg_18, label %Block10, label %Block9
Block7: ; Loop 0 latch
	br label %Block2
Block8: ; Loop 0 exit target
	br label %Block11
Block9: ; Loop 0 body
	%reg_29 = add i32 %reg_37, 1
	%reg_39 = add i32 %reg_38, 1
	br label %Block5
Block10: ; Loop 0 exiting block
	%reg_20 = getelementptr i32, ptr %reg_1, i32 %reg_37
	%reg_21 = load i32, ptr %reg_20
	%reg_23 = getelementptr i32, ptr %reg_1, i32 %reg_35
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = icmp eq i32 %reg_21, %reg_24
	br i1 %reg_25, label %Block8, label %Block9
Block11:
	%reg_34 = phi i32 [ 1, %Block4 ], [ 0, %Block8 ]
	ret i32 %reg_34
}

define i32 @graphColoring(ptr %reg_0, i32 %reg_1, i32 %reg_2, ptr %reg_3)
{
Block0:
	br label %Block1
Block1:
	%reg_7 = load i32, ptr @V
	%reg_8 = icmp eq i32 %reg_2, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block2:
	%reg_9 = getelementptr [4 x i32], ptr %reg_0
	%reg_10 = getelementptr i32, ptr %reg_3
	%reg_11 = call i32 @isSafe(ptr %reg_9, ptr %reg_10)
	%reg_12 = icmp ne i32 %reg_11, 0
	br i1 %reg_12, label %Block4, label %Block5
Block3: ; Loop 0 preheader
	br label %Block6
Block4:
	br label %Block13
Block5:
	br label %Block11
Block6: ; Loop 0 header
	%reg_64 = phi i32 [ 0, %Block3 ], [ %reg_65, %Block10 ]
	%reg_66 = add i32 %reg_64, 1
	%reg_20 = icmp sle i32 %reg_66, %reg_1
	br i1 %reg_20, label %Block7, label %Block8
Block7: ; Loop 0 exiting block
	%reg_22 = getelementptr i32, ptr %reg_3, i32 %reg_2
	store i32 %reg_66, ptr %reg_22
	%reg_24 = getelementptr [4 x i32], ptr %reg_0
	%reg_28 = add i32 %reg_2, 1
	%reg_29 = getelementptr i32, ptr %reg_3
	%reg_30 = call i32 @graphColoring(ptr %reg_24, i32 %reg_1, i32 %reg_28, ptr %reg_29)
	%reg_31 = icmp ne i32 %reg_30, 0
	br i1 %reg_31, label %Block9, label %Block10
Block8: ; Loop 0 exit target
	br label %Block11
Block9: ; Loop 0 exit target
	br label %Block11
Block10: ; Loop 0 latch
	store i32 0, ptr %reg_22
	%reg_65 = add i32 %reg_64, 1
	br label %Block6
Block11:
	%reg_40 = phi i32 [ 0, %Block5 ], [ 0, %Block8 ], [ 1, %Block9 ], [ 1, %Block12 ]
	ret i32 %reg_40
Block12:
	br label %Block11
Block13:
	br label %Block21
Block14: ; Loop 1 header
	%reg_68 = phi ptr [ %reg_67, %Block22 ], [ %reg_69, %Block15 ]
	%reg_55 = phi i32 [ %reg_53, %Block15 ], [ 0, %Block22 ]
	br label %Block15
Block15: ; Loop 1 latch
	%reg_49 = load i32, ptr %reg_68
	call void @putint(i32 %reg_49)
	%reg_50 = load i32, ptr @space
	call void @putch(i32 %reg_50)
	%reg_53 = add i32 %reg_55, 1
	%reg_59 = load i32, ptr @V
	%reg_60 = icmp slt i32 %reg_53, %reg_59
	%reg_69 = getelementptr i32, ptr %reg_68, i32 1
	br i1 %reg_60, label %Block14, label %Block23
Block16:
	%reg_54 = load i32, ptr @LF
	call void @putch(i32 %reg_54)
	br label %Block12
Block21:
	%reg_57 = load i32, ptr @V
	%reg_58 = icmp slt i32 0, %reg_57
	br i1 %reg_58, label %Block22, label %Block16
Block22: ; Loop 1 preheader
	%reg_67 = getelementptr i32, ptr %reg_10, i32 0
	br label %Block14
Block23: ; Loop 1 exit target
	br label %Block16
}

define i32 @main()
{
Block0:
	%reg_35 = alloca [4 x i32]
	%reg_0 = alloca [4 x [4 x i32]]
	br label %Block1
Block1:
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 64, i1 1)
	%reg_2 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 0, i32 0
	store i32 0, ptr %reg_2
	%reg_4 = getelementptr i32, ptr %reg_2, i32 1
	store i32 1, ptr %reg_4
	%reg_6 = getelementptr i32, ptr %reg_4, i32 1
	store i32 1, ptr %reg_6
	%reg_8 = getelementptr i32, ptr %reg_6, i32 1
	store i32 1, ptr %reg_8
	%reg_10 = getelementptr i32, ptr %reg_8, i32 1
	store i32 1, ptr %reg_10
	%reg_12 = getelementptr i32, ptr %reg_10, i32 1
	store i32 0, ptr %reg_12
	%reg_14 = getelementptr i32, ptr %reg_12, i32 1
	store i32 1, ptr %reg_14
	%reg_16 = getelementptr i32, ptr %reg_14, i32 1
	store i32 0, ptr %reg_16
	%reg_18 = getelementptr i32, ptr %reg_16, i32 1
	store i32 1, ptr %reg_18
	%reg_20 = getelementptr i32, ptr %reg_18, i32 1
	store i32 1, ptr %reg_20
	%reg_22 = getelementptr i32, ptr %reg_20, i32 1
	store i32 0, ptr %reg_22
	%reg_24 = getelementptr i32, ptr %reg_22, i32 1
	store i32 1, ptr %reg_24
	%reg_26 = getelementptr i32, ptr %reg_24, i32 1
	store i32 1, ptr %reg_26
	%reg_28 = getelementptr i32, ptr %reg_26, i32 1
	store i32 0, ptr %reg_28
	%reg_30 = getelementptr i32, ptr %reg_28, i32 1
	store i32 1, ptr %reg_30
	%reg_32 = getelementptr i32, ptr %reg_30, i32 1
	store i32 0, ptr %reg_32
	call void @llvm.memset.p0.i32(ptr %reg_35, i8 0, i32 16, i1 1)
	br label %Block10
Block2: ; Loop 0 header
	%reg_73 = phi ptr [ %reg_72, %Block11 ], [ %reg_74, %Block3 ]
	%reg_54 = phi i32 [ %reg_46, %Block3 ], [ 0, %Block11 ]
	br label %Block3
Block3: ; Loop 0 latch
	store i32 0, ptr %reg_73
	%reg_46 = add i32 %reg_54, 1
	%reg_67 = load i32, ptr @V
	%reg_68 = icmp slt i32 %reg_46, %reg_67
	%reg_74 = getelementptr i32, ptr %reg_73, i32 1
	br i1 %reg_68, label %Block2, label %Block12
Block4:
	%reg_47 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0
	%reg_50 = getelementptr [4 x i32], ptr %reg_35, i32 0
	%reg_51 = call i32 @graphColoring(ptr %reg_47, i32 3, i32 0, ptr %reg_50)
	%reg_52 = icmp eq i32 %reg_51, 0
	br i1 %reg_52, label %Block5, label %Block6
Block5:
	br label %Block8
Block6:
	ret i32 0
Block7:
	br label %Block6
Block8:
	call void @putch(i32 78)
	call void @putch(i32 111)
	call void @putch(i32 116)
	%reg_58 = load i32, ptr @space
	call void @putch(i32 %reg_58)
	call void @putch(i32 101)
	call void @putch(i32 120)
	call void @putch(i32 105)
	call void @putch(i32 115)
	call void @putch(i32 116)
	br label %Block7
Block10:
	%reg_65 = load i32, ptr @V
	%reg_66 = icmp slt i32 0, %reg_65
	br i1 %reg_66, label %Block11, label %Block4
Block11: ; Loop 0 preheader
	%reg_72 = getelementptr [4 x i32], ptr %reg_35, i32 0, i32 0
	br label %Block2
Block12: ; Loop 0 exit target
	br label %Block4
}
