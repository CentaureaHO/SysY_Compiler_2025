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

@n = global i32 zeroinitializer
@m = global i32 zeroinitializer
@fa = global [100005 x i32] zeroinitializer

define i32 @quick_read()
{
Block0:  ;Func define at line 1
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 1
	%reg_1 = call i32 @getch()
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 3
	%reg_6 = load i32, ptr %reg_0
	%reg_7 = add i32 48, 0
	%reg_8 = icmp slt i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block5
Block3:  ;While body at line 3
	%reg_12 = load i32, ptr %reg_0
	%reg_13 = add i32 45, 0
	%reg_14 = icmp eq i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block6, label %Block7
Block4:  ;While end at line 3
	br label %Block8
Block5:  ;Or opertor at line 3
	%reg_9 = load i32, ptr %reg_0
	%reg_10 = add i32 57, 0
	%reg_11 = icmp sgt i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block3, label %Block4
Block6:  ;If then at line 4
	%reg_15 = add i32 1, 0
	store i32 %reg_15, ptr %reg_4
	br label %Block7
Block7:  ;If end at line 4
	%reg_16 = call i32 @getch()
	store i32 %reg_16, ptr %reg_0
	br label %Block2
Block8:  ;While condition at line 7
	%reg_17 = load i32, ptr %reg_0
	%reg_18 = add i32 48, 0
	%reg_19 = icmp sge i32 %reg_17, %reg_18
	br i1 %reg_19, label %Block11, label %Block10
Block9:  ;While body at line 7
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = add i32 10, 0
	%reg_25 = mul i32 %reg_23, %reg_24
	%reg_26 = load i32, ptr %reg_0
	%reg_27 = add i32 %reg_25, %reg_26
	%reg_28 = add i32 48, 0
	%reg_29 = sub i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_2
	%reg_30 = call i32 @getch()
	store i32 %reg_30, ptr %reg_0
	br label %Block8
Block10:  ;While end at line 7
	%reg_31 = load i32, ptr %reg_4
	%reg_32 = icmp ne i32 %reg_31, 0
	br i1 %reg_32, label %Block12, label %Block14
Block11:  ;And opertor at line 7
	%reg_20 = load i32, ptr %reg_0
	%reg_21 = add i32 57, 0
	%reg_22 = icmp sle i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block9, label %Block10
Block12:  ;If then at line 11
	%reg_33 = load i32, ptr %reg_2
	%reg_34 = sub i32 0, %reg_33
	ret i32 %reg_34
	br label %Block13
Block13:  ;If end at line 11
	ret i32 0
Block14:  ;If else at line 11
	%reg_35 = load i32, ptr %reg_2
	ret i32 %reg_35
	br label %Block13
}

define void @init()
{
Block0:  ;Func define at line 15
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 15
	%reg_1 = add i32 1, 0
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 17
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = load i32, ptr @n
	%reg_4 = icmp sle i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 17
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_0
	%reg_9 = add i32 1, 0
	%reg_10 = add i32 %reg_8, %reg_9
	store i32 %reg_10, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 17
	ret void
}

define i32 @find(i32 %reg_0)
{
Block0:  ;Func define at line 22
	%reg_8 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 22
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 23
	%reg_7 = load i32, ptr %reg_1
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 23
	ret i32 0
Block4:  ;If else at line 23
	%reg_9 = load i32, ptr %reg_1
	%reg_10 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = call i32 @find(i32 %reg_11)
	store i32 %reg_12, ptr %reg_8
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_8
	store i32 %reg_15, ptr %reg_14
	%reg_16 = load i32, ptr %reg_8
	ret i32 %reg_16
	br label %Block3
}

define i32 @same(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 30
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 30
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = call i32 @find(i32 %reg_4)
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = call i32 @find(i32 %reg_6)
	%reg_8 = icmp eq i32 %reg_5, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block2:  ;If then at line 31
	%reg_9 = add i32 1, 0
	ret i32 %reg_9
	br label %Block3
Block3:  ;If end at line 31
	%reg_10 = add i32 0, 0
	ret i32 %reg_10
}

define i32 @main()
{
Block0:  ;Func define at line 34
	%reg_27 = alloca i32
	%reg_24 = alloca i32
	%reg_18 = alloca i32
	%reg_16 = alloca i32
	%reg_4 = alloca i32
	br label %Block1
Block1:  ;Func body at line 34
	%reg_0 = call i32 @quick_read()
	store i32 %reg_0, ptr @n
	%reg_1 = call i32 @quick_read()
	store i32 %reg_1, ptr @m
	call void @init()
	br label %Block2
Block2:  ;While condition at line 37
	%reg_2 = load i32, ptr @m
	%reg_3 = icmp ne i32 %reg_2, 0
	br i1 %reg_3, label %Block3, label %Block4
Block3:  ;While body at line 37
	%reg_5 = call i32 @getch()
	store i32 %reg_5, ptr %reg_4
	br label %Block5
Block4:  ;While end at line 37
	%reg_36 = add i32 0, 0
	ret i32 %reg_36
Block5:  ;While condition at line 39
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = add i32 81, 0
	%reg_8 = icmp ne i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block8, label %Block7
Block6:  ;While body at line 39
	%reg_12 = call i32 @getch()
	store i32 %reg_12, ptr %reg_4
	br label %Block5
Block7:  ;While end at line 39
	%reg_13 = load i32, ptr %reg_4
	%reg_14 = add i32 81, 0
	%reg_15 = icmp eq i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block9, label %Block11
Block8:  ;And opertor at line 39
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = add i32 85, 0
	%reg_11 = icmp ne i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block6, label %Block7
Block9:  ;If then at line 42
	%reg_17 = call i32 @quick_read()
	store i32 %reg_17, ptr %reg_16
	%reg_19 = call i32 @quick_read()
	store i32 %reg_19, ptr %reg_18
	%reg_20 = load i32, ptr %reg_16
	%reg_21 = load i32, ptr %reg_18
	%reg_22 = call i32 @same(i32 %reg_20, i32 %reg_21)
	call void @putint(i32 %reg_22)
	%reg_23 = add i32 10, 0
	call void @putch(i32 %reg_23)
	br label %Block10
Block10:  ;If end at line 42
	%reg_33 = load i32, ptr @m
	%reg_34 = add i32 1, 0
	%reg_35 = sub i32 %reg_33, %reg_34
	store i32 %reg_35, ptr @m
	br label %Block2
Block11:  ;If else at line 42
	%reg_25 = call i32 @quick_read()
	%reg_26 = call i32 @find(i32 %reg_25)
	store i32 %reg_26, ptr %reg_24
	%reg_28 = call i32 @quick_read()
	%reg_29 = call i32 @find(i32 %reg_28)
	store i32 %reg_29, ptr %reg_27
	%reg_30 = load i32, ptr %reg_24
	%reg_31 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_27
	store i32 %reg_32, ptr %reg_31
	br label %Block10
}
