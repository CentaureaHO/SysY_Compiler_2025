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
declare void @llvm.memset.p0.i32(ptr, i8, i32, i1)

@V = global i32 4
@space = global i32 32
@LF = global i32 10

define void @printSolution(ptr %reg_0)
{
Block0:  ;Func define at line 5
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 5
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	br label %Block2
Block2:  ;While condition at line 7
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = load i32, ptr @V
	%reg_5 = icmp slt i32 %reg_3, %reg_4
	br i1 %reg_5, label %Block3, label %Block4
Block3:  ;While body at line 7
	%reg_6 = load i32, ptr %reg_1
	%reg_7 = getelementptr i32, ptr %reg_0, i32 %reg_6
	%reg_8 = load i32, ptr %reg_7
	call void @putint(i32 %reg_8)
	%reg_9 = load i32, ptr @space
	call void @putch(i32 %reg_9)
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = add i32 1, 0
	%reg_12 = add i32 %reg_10, %reg_11
	store i32 %reg_12, ptr %reg_1
	br label %Block2
Block4:  ;While end at line 7
	%reg_13 = load i32, ptr @LF
	call void @putch(i32 %reg_13)
	ret void
}

define void @printMessage()
{
Block0:  ;Func define at line 15
	br label %Block1
Block1:  ;Func body at line 15
	%reg_0 = add i32 78, 0
	call void @putch(i32 %reg_0)
	%reg_1 = add i32 111, 0
	call void @putch(i32 %reg_1)
	%reg_2 = add i32 116, 0
	call void @putch(i32 %reg_2)
	%reg_3 = load i32, ptr @space
	call void @putch(i32 %reg_3)
	%reg_4 = add i32 101, 0
	call void @putch(i32 %reg_4)
	%reg_5 = add i32 120, 0
	call void @putch(i32 %reg_5)
	%reg_6 = add i32 105, 0
	call void @putch(i32 %reg_6)
	%reg_7 = add i32 115, 0
	call void @putch(i32 %reg_7)
	%reg_8 = add i32 116, 0
	call void @putch(i32 %reg_8)
	ret void
}

define i32 @isSafe(ptr %reg_0, ptr %reg_1)
{
Block0:  ;Func define at line 21
	%reg_7 = alloca i32
	%reg_2 = alloca i32
	br label %Block1
Block1:  ;Func body at line 21
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 23
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @V
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 23
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = add i32 1, 0
	%reg_10 = add i32 %reg_8, %reg_9
	store i32 %reg_10, ptr %reg_7
	br label %Block5
Block4:  ;While end at line 23
	%reg_33 = add i32 1, 0
	ret i32 %reg_33
Block5:  ;While condition at line 25
	%reg_11 = load i32, ptr %reg_7
	%reg_12 = load i32, ptr @V
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block6, label %Block7
Block6:  ;While body at line 25
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = load i32, ptr %reg_7
	%reg_16 = getelementptr [4 x i32], ptr %reg_0, i32 %reg_14, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	%reg_18 = icmp ne i32 %reg_17, 0
	br i1 %reg_18, label %Block10, label %Block9
Block7:  ;While end at line 25
	%reg_30 = load i32, ptr %reg_2
	%reg_31 = add i32 1, 0
	%reg_32 = add i32 %reg_30, %reg_31
	store i32 %reg_32, ptr %reg_2
	br label %Block2
Block8:  ;If then at line 26
	%reg_26 = add i32 0, 0
	ret i32 %reg_26
	br label %Block9
Block9:  ;If end at line 26
	%reg_27 = load i32, ptr %reg_7
	%reg_28 = add i32 1, 0
	%reg_29 = add i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_7
	br label %Block5
Block10:  ;And opertor at line 26
	%reg_19 = load i32, ptr %reg_7
	%reg_20 = getelementptr i32, ptr %reg_1, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = load i32, ptr %reg_2
	%reg_23 = getelementptr i32, ptr %reg_1, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = icmp eq i32 %reg_21, %reg_24
	br i1 %reg_25, label %Block8, label %Block9
}

define i32 @graphColoring(ptr %reg_0, i32 %reg_1, i32 %reg_2, ptr %reg_3)
{
Block0:  ;Func define at line 35
	%reg_16 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 35
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = load i32, ptr @V
	%reg_8 = icmp eq i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block2:  ;If then at line 36
	%reg_9 = getelementptr [4 x i32], ptr %reg_0
	%reg_10 = getelementptr i32, ptr %reg_3
	%reg_11 = call i32 @isSafe(ptr %reg_9, ptr %reg_10)
	%reg_12 = icmp ne i32 %reg_11, 0
	br i1 %reg_12, label %Block4, label %Block5
Block3:  ;If end at line 36
	%reg_17 = add i32 1, 0
	store i32 %reg_17, ptr %reg_16
	br label %Block6
Block4:  ;If then at line 37
	%reg_13 = getelementptr i32, ptr %reg_3
	call void @printSolution(ptr %reg_13)
	%reg_14 = add i32 1, 0
	ret i32 %reg_14
	br label %Block5
Block5:  ;If end at line 37
	%reg_15 = add i32 0, 0
	ret i32 %reg_15
	br label %Block3
Block6:  ;While condition at line 44
	%reg_18 = load i32, ptr %reg_16
	%reg_19 = load i32, ptr %reg_4
	%reg_20 = icmp sle i32 %reg_18, %reg_19
	br i1 %reg_20, label %Block7, label %Block8
Block7:  ;While body at line 44
	%reg_21 = load i32, ptr %reg_5
	%reg_22 = getelementptr i32, ptr %reg_3, i32 %reg_21
	%reg_23 = load i32, ptr %reg_16
	store i32 %reg_23, ptr %reg_22
	%reg_24 = getelementptr [4 x i32], ptr %reg_0
	%reg_25 = load i32, ptr %reg_4
	%reg_26 = load i32, ptr %reg_5
	%reg_27 = add i32 1, 0
	%reg_28 = add i32 %reg_26, %reg_27
	%reg_29 = getelementptr i32, ptr %reg_3
	%reg_30 = call i32 @graphColoring(ptr %reg_24, i32 %reg_25, i32 %reg_28, ptr %reg_29)
	%reg_31 = icmp ne i32 %reg_30, 0
	br i1 %reg_31, label %Block9, label %Block10
Block8:  ;While end at line 44
	%reg_39 = add i32 0, 0
	ret i32 %reg_39
Block9:  ;If then at line 46
	%reg_32 = add i32 1, 0
	ret i32 %reg_32
	br label %Block10
Block10:  ;If end at line 46
	%reg_33 = load i32, ptr %reg_5
	%reg_34 = getelementptr i32, ptr %reg_3, i32 %reg_33
	%reg_35 = add i32 0, 0
	store i32 %reg_35, ptr %reg_34
	%reg_36 = load i32, ptr %reg_16
	%reg_37 = add i32 1, 0
	%reg_38 = add i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_16
	br label %Block6
}

define i32 @main()
{
Block0:  ;Func define at line 54
	%reg_36 = alloca i32
	%reg_35 = alloca [4 x i32]
	%reg_33 = alloca i32
	%reg_0 = alloca [4 x [4 x i32]]
	br label %Block1
Block1:  ;Func body at line 54
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 64, i1 1)
	%reg_1 = add i32 0, 0
	%reg_2 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 0, i32 0
	store i32 %reg_1, ptr %reg_2
	%reg_3 = add i32 1, 0
	%reg_4 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 0, i32 1
	store i32 %reg_3, ptr %reg_4
	%reg_5 = add i32 1, 0
	%reg_6 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 0, i32 2
	store i32 %reg_5, ptr %reg_6
	%reg_7 = add i32 1, 0
	%reg_8 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 0, i32 3
	store i32 %reg_7, ptr %reg_8
	%reg_9 = add i32 1, 0
	%reg_10 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 1, i32 0
	store i32 %reg_9, ptr %reg_10
	%reg_11 = add i32 0, 0
	%reg_12 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 1, i32 1
	store i32 %reg_11, ptr %reg_12
	%reg_13 = add i32 1, 0
	%reg_14 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 1, i32 2
	store i32 %reg_13, ptr %reg_14
	%reg_15 = add i32 0, 0
	%reg_16 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 1, i32 3
	store i32 %reg_15, ptr %reg_16
	%reg_17 = add i32 1, 0
	%reg_18 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 2, i32 0
	store i32 %reg_17, ptr %reg_18
	%reg_19 = add i32 1, 0
	%reg_20 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 2, i32 1
	store i32 %reg_19, ptr %reg_20
	%reg_21 = add i32 0, 0
	%reg_22 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 2, i32 2
	store i32 %reg_21, ptr %reg_22
	%reg_23 = add i32 1, 0
	%reg_24 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 2, i32 3
	store i32 %reg_23, ptr %reg_24
	%reg_25 = add i32 1, 0
	%reg_26 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 3, i32 0
	store i32 %reg_25, ptr %reg_26
	%reg_27 = add i32 0, 0
	%reg_28 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 3, i32 1
	store i32 %reg_27, ptr %reg_28
	%reg_29 = add i32 1, 0
	%reg_30 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 3, i32 2
	store i32 %reg_29, ptr %reg_30
	%reg_31 = add i32 0, 0
	%reg_32 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0, i32 3, i32 3
	store i32 %reg_31, ptr %reg_32
	%reg_34 = add i32 3, 0
	store i32 %reg_34, ptr %reg_33
	call void @llvm.memset.p0.i32(ptr %reg_35, i8 0, i32 16, i1 1)
	%reg_37 = add i32 0, 0
	store i32 %reg_37, ptr %reg_36
	br label %Block2
Block2:  ;While condition at line 62
	%reg_38 = load i32, ptr %reg_36
	%reg_39 = load i32, ptr @V
	%reg_40 = icmp slt i32 %reg_38, %reg_39
	br i1 %reg_40, label %Block3, label %Block4
Block3:  ;While body at line 62
	%reg_41 = load i32, ptr %reg_36
	%reg_42 = getelementptr [4 x i32], ptr %reg_35, i32 0, i32 %reg_41
	%reg_43 = add i32 0, 0
	store i32 %reg_43, ptr %reg_42
	%reg_44 = load i32, ptr %reg_36
	%reg_45 = add i32 1, 0
	%reg_46 = add i32 %reg_44, %reg_45
	store i32 %reg_46, ptr %reg_36
	br label %Block2
Block4:  ;While end at line 62
	%reg_47 = getelementptr [4 x [4 x i32]], ptr %reg_0, i32 0
	%reg_48 = load i32, ptr %reg_33
	%reg_49 = add i32 0, 0
	%reg_50 = getelementptr [4 x i32], ptr %reg_35, i32 0
	%reg_51 = call i32 @graphColoring(ptr %reg_47, i32 %reg_48, i32 %reg_49, ptr %reg_50)
	%reg_52 = icmp eq i32 %reg_51, 0
	br i1 %reg_52, label %Block5, label %Block6
Block5:  ;If then at line 66
	call void @printMessage()
	br label %Block6
Block6:  ;If end at line 66
	%reg_53 = add i32 0, 0
	ret i32 %reg_53
}
