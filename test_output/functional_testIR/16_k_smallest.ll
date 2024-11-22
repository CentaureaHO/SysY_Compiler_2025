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

@maxN = global i32 1000
@space = global i32 32
@array = global [1000 x i32] zeroinitializer

define void @swap(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 5
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 5
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = getelementptr [1000 x i32], ptr @array, i32 0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	store i32 %reg_7, ptr %reg_4
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = getelementptr [1000 x i32], ptr @array, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = getelementptr [1000 x i32], ptr @array, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	store i32 %reg_12, ptr %reg_9
	%reg_13 = load i32, ptr %reg_3
	%reg_14 = getelementptr [1000 x i32], ptr @array, i32 0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_4
	store i32 %reg_15, ptr %reg_14
	ret void
}

define i32 @findPivot(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 11
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 11
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = getelementptr [1000 x i32], ptr @array, i32 0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	store i32 %reg_7, ptr %reg_4
	%reg_9 = load i32, ptr %reg_2
	store i32 %reg_9, ptr %reg_8
	%reg_11 = load i32, ptr %reg_2
	store i32 %reg_11, ptr %reg_10
	br label %Block2
Block2:  ;While condition at line 15
	%reg_12 = load i32, ptr %reg_10
	%reg_13 = load i32, ptr %reg_3
	%reg_14 = icmp slt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 15
	%reg_15 = load i32, ptr %reg_10
	%reg_16 = getelementptr [1000 x i32], ptr @array, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	%reg_18 = load i32, ptr %reg_4
	%reg_19 = icmp sle i32 %reg_17, %reg_18
	br i1 %reg_19, label %Block5, label %Block6
Block4:  ;While end at line 15
	%reg_28 = load i32, ptr %reg_8
	%reg_29 = load i32, ptr %reg_3
	call void @swap(i32 %reg_28, i32 %reg_29)
	%reg_30 = load i32, ptr %reg_8
	ret i32 %reg_30
Block5:  ;If then at line 16
	%reg_20 = load i32, ptr %reg_10
	%reg_21 = load i32, ptr %reg_8
	call void @swap(i32 %reg_20, i32 %reg_21)
	%reg_22 = load i32, ptr %reg_8
	%reg_23 = add i32 1, 0
	%reg_24 = add i32 %reg_22, %reg_23
	store i32 %reg_24, ptr %reg_8
	br label %Block6
Block6:  ;If end at line 16
	%reg_25 = load i32, ptr %reg_10
	%reg_26 = add i32 1, 0
	%reg_27 = add i32 %reg_25, %reg_26
	store i32 %reg_27, ptr %reg_10
	br label %Block2
}

define void @findSmallest(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3)
{
Block0:  ;Func define at line 26
	%reg_18 = alloca i32
	%reg_11 = alloca i32
	%reg_7 = alloca i32
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	store i32 %reg_0, ptr %reg_4
	store i32 %reg_1, ptr %reg_5
	store i32 %reg_2, ptr %reg_6
	store i32 %reg_3, ptr %reg_7
	br label %Block1
Block1:  ;Func body at line 26
	%reg_8 = load i32, ptr %reg_4
	%reg_9 = load i32, ptr %reg_5
	%reg_10 = icmp eq i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block2, label %Block4
Block2:  ;If then at line 27
	ret void
	br label %Block3
Block3:  ;If end at line 27
	ret void
Block4:  ;If else at line 27
	%reg_12 = load i32, ptr %reg_4
	%reg_13 = load i32, ptr %reg_5
	%reg_14 = call i32 @findPivot(i32 %reg_12, i32 %reg_13)
	store i32 %reg_14, ptr %reg_11
	%reg_15 = load i32, ptr %reg_6
	%reg_16 = load i32, ptr %reg_11
	%reg_17 = icmp eq i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block5, label %Block7
Block5:  ;If then at line 31
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	br label %Block8
Block6:  ;If end at line 31
	br label %Block3
Block7:  ;If else at line 31
	%reg_30 = load i32, ptr %reg_6
	%reg_31 = load i32, ptr %reg_11
	%reg_32 = icmp slt i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block11, label %Block13
Block8:  ;While condition at line 33
	%reg_20 = load i32, ptr %reg_18
	%reg_21 = load i32, ptr %reg_11
	%reg_22 = icmp slt i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block9, label %Block10
Block9:  ;While body at line 33
	%reg_23 = load i32, ptr %reg_18
	%reg_24 = getelementptr [1000 x i32], ptr @array, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	call void @putint(i32 %reg_25)
	%reg_26 = load i32, ptr @space
	call void @putch(i32 %reg_26)
	%reg_27 = load i32, ptr %reg_18
	%reg_28 = add i32 1, 0
	%reg_29 = add i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_18
	br label %Block8
Block10:  ;While end at line 33
	br label %Block6
Block11:  ;If then at line 39
	%reg_33 = load i32, ptr %reg_4
	%reg_34 = load i32, ptr %reg_11
	%reg_35 = add i32 1, 0
	%reg_36 = sub i32 %reg_34, %reg_35
	%reg_37 = load i32, ptr %reg_6
	%reg_38 = load i32, ptr %reg_7
	call void @findSmallest(i32 %reg_33, i32 %reg_36, i32 %reg_37, i32 %reg_38)
	br label %Block12
Block12:  ;If end at line 39
	br label %Block6
Block13:  ;If else at line 39
	%reg_39 = load i32, ptr %reg_11
	%reg_40 = add i32 1, 0
	%reg_41 = add i32 %reg_39, %reg_40
	%reg_42 = load i32, ptr %reg_5
	%reg_43 = load i32, ptr %reg_6
	%reg_44 = load i32, ptr %reg_7
	call void @findSmallest(i32 %reg_41, i32 %reg_42, i32 %reg_43, i32 %reg_44)
	br label %Block12
}

define i32 @main()
{
Block0:  ;Func define at line 48
	%reg_17 = alloca i32
	%reg_15 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 48
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	%reg_3 = call i32 @getint()
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 51
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = load i32, ptr %reg_0
	%reg_8 = icmp slt i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 51
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = getelementptr [1000 x i32], ptr @array, i32 0, i32 %reg_9
	%reg_11 = call i32 @getint()
	store i32 %reg_11, ptr %reg_10
	%reg_12 = load i32, ptr %reg_4
	%reg_13 = add i32 1, 0
	%reg_14 = add i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_4
	br label %Block2
Block4:  ;While end at line 51
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	%reg_18 = load i32, ptr %reg_0
	%reg_19 = add i32 1, 0
	%reg_20 = sub i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_17
	%reg_21 = load i32, ptr %reg_15
	%reg_22 = load i32, ptr %reg_17
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = load i32, ptr %reg_0
	call void @findSmallest(i32 %reg_21, i32 %reg_22, i32 %reg_23, i32 %reg_24)
	%reg_25 = add i32 0, 0
	ret i32 %reg_25
}
