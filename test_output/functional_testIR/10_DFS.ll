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
@maxn = global i32 1005
@maxm = global i32 5005
@to = global [5005 x i32] zeroinitializer
@next = global [5005 x i32] zeroinitializer
@head = global [1005 x i32] zeroinitializer
@cnt = global i32 0
@vis = global [1005 x i32] zeroinitializer

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

define void @add_edge(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 18
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 18
	%reg_4 = load i32, ptr @cnt
	%reg_5 = getelementptr [5005 x i32], ptr @to, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_3
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr @cnt
	%reg_8 = getelementptr [5005 x i32], ptr @next, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = getelementptr [1005 x i32], ptr @head, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	store i32 %reg_11, ptr %reg_8
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = getelementptr [1005 x i32], ptr @head, i32 0, i32 %reg_12
	%reg_14 = load i32, ptr @cnt
	store i32 %reg_14, ptr %reg_13
	%reg_15 = load i32, ptr @cnt
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr @cnt
	%reg_18 = load i32, ptr @cnt
	%reg_19 = getelementptr [5005 x i32], ptr @to, i32 0, i32 %reg_18
	%reg_20 = load i32, ptr %reg_2
	store i32 %reg_20, ptr %reg_19
	%reg_21 = load i32, ptr @cnt
	%reg_22 = getelementptr [5005 x i32], ptr @next, i32 0, i32 %reg_21
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = getelementptr [1005 x i32], ptr @head, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	store i32 %reg_25, ptr %reg_22
	%reg_26 = load i32, ptr %reg_3
	%reg_27 = getelementptr [1005 x i32], ptr @head, i32 0, i32 %reg_26
	%reg_28 = load i32, ptr @cnt
	store i32 %reg_28, ptr %reg_27
	%reg_29 = load i32, ptr @cnt
	%reg_30 = add i32 1, 0
	%reg_31 = add i32 %reg_29, %reg_30
	store i32 %reg_31, ptr @cnt
	ret void
}

define void @init()
{
Block0:  ;Func define at line 28
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 28
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 30
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = load i32, ptr @maxn
	%reg_4 = icmp slt i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 30
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = getelementptr [1005 x i32], ptr @head, i32 0, i32 %reg_5
	%reg_7 = add i32 1, 0
	%reg_8 = sub i32 0, %reg_7
	store i32 %reg_8, ptr %reg_6
	%reg_9 = load i32, ptr %reg_0
	%reg_10 = add i32 1, 0
	%reg_11 = add i32 %reg_9, %reg_10
	store i32 %reg_11, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 30
	ret void
}

define void @clear()
{
Block0:  ;Func define at line 36
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 36
	%reg_1 = add i32 1, 0
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 38
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = load i32, ptr @n
	%reg_4 = icmp sle i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 38
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = getelementptr [1005 x i32], ptr @vis, i32 0, i32 %reg_5
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_0
	%reg_9 = add i32 1, 0
	%reg_10 = add i32 %reg_8, %reg_9
	store i32 %reg_10, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 38
	ret void
}

define i32 @same(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 43
	%reg_19 = alloca i32
	%reg_11 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 43
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = getelementptr [1005 x i32], ptr @vis, i32 0, i32 %reg_4
	%reg_6 = add i32 1, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = icmp eq i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block2, label %Block3
Block2:  ;If then at line 45
	%reg_10 = add i32 1, 0
	ret i32 %reg_10
	br label %Block3
Block3:  ;If end at line 45
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = getelementptr [1005 x i32], ptr @head, i32 0, i32 %reg_12
	%reg_14 = load i32, ptr %reg_13
	store i32 %reg_14, ptr %reg_11
	br label %Block4
Block4:  ;While condition at line 47
	%reg_15 = load i32, ptr %reg_11
	%reg_16 = add i32 1, 0
	%reg_17 = sub i32 0, %reg_16
	%reg_18 = icmp ne i32 %reg_15, %reg_17
	br i1 %reg_18, label %Block5, label %Block6
Block5:  ;While body at line 47
	%reg_20 = load i32, ptr %reg_11
	%reg_21 = getelementptr [5005 x i32], ptr @to, i32 0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	store i32 %reg_22, ptr %reg_19
	%reg_23 = load i32, ptr %reg_19
	%reg_24 = getelementptr [1005 x i32], ptr @vis, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = icmp eq i32 %reg_25, 0
	br i1 %reg_26, label %Block9, label %Block8
Block6:  ;While end at line 47
	%reg_35 = add i32 0, 0
	ret i32 %reg_35
Block7:  ;If then at line 49
	%reg_31 = add i32 1, 0
	ret i32 %reg_31
	br label %Block8
Block8:  ;If end at line 49
	%reg_32 = load i32, ptr %reg_11
	%reg_33 = getelementptr [5005 x i32], ptr @next, i32 0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	store i32 %reg_34, ptr %reg_11
	br label %Block4
Block9:  ;And opertor at line 49
	%reg_27 = load i32, ptr %reg_19
	%reg_28 = load i32, ptr %reg_3
	%reg_29 = call i32 @same(i32 %reg_27, i32 %reg_28)
	%reg_30 = icmp ne i32 %reg_29, 0
	br i1 %reg_30, label %Block7, label %Block8
}

define i32 @main()
{
Block0:  ;Func define at line 55
	%reg_26 = alloca i32
	%reg_24 = alloca i32
	%reg_18 = alloca i32
	%reg_16 = alloca i32
	%reg_4 = alloca i32
	br label %Block1
Block1:  ;Func body at line 55
	%reg_0 = call i32 @quick_read()
	store i32 %reg_0, ptr @n
	%reg_1 = call i32 @quick_read()
	store i32 %reg_1, ptr @m
	call void @init()
	br label %Block2
Block2:  ;While condition at line 58
	%reg_2 = load i32, ptr @m
	%reg_3 = icmp ne i32 %reg_2, 0
	br i1 %reg_3, label %Block3, label %Block4
Block3:  ;While body at line 58
	%reg_5 = call i32 @getch()
	store i32 %reg_5, ptr %reg_4
	br label %Block5
Block4:  ;While end at line 58
	%reg_33 = add i32 0, 0
	ret i32 %reg_33
Block5:  ;While condition at line 60
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = add i32 81, 0
	%reg_8 = icmp ne i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block8, label %Block7
Block6:  ;While body at line 60
	%reg_12 = call i32 @getch()
	store i32 %reg_12, ptr %reg_4
	br label %Block5
Block7:  ;While end at line 60
	%reg_13 = load i32, ptr %reg_4
	%reg_14 = add i32 81, 0
	%reg_15 = icmp eq i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block9, label %Block11
Block8:  ;And opertor at line 60
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = add i32 85, 0
	%reg_11 = icmp ne i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block6, label %Block7
Block9:  ;If then at line 63
	%reg_17 = call i32 @quick_read()
	store i32 %reg_17, ptr %reg_16
	%reg_19 = call i32 @quick_read()
	store i32 %reg_19, ptr %reg_18
	call void @clear()
	%reg_20 = load i32, ptr %reg_16
	%reg_21 = load i32, ptr %reg_18
	%reg_22 = call i32 @same(i32 %reg_20, i32 %reg_21)
	call void @putint(i32 %reg_22)
	%reg_23 = add i32 10, 0
	call void @putch(i32 %reg_23)
	br label %Block10
Block10:  ;If end at line 63
	%reg_30 = load i32, ptr @m
	%reg_31 = add i32 1, 0
	%reg_32 = sub i32 %reg_30, %reg_31
	store i32 %reg_32, ptr @m
	br label %Block2
Block11:  ;If else at line 63
	%reg_25 = call i32 @quick_read()
	store i32 %reg_25, ptr %reg_24
	%reg_27 = call i32 @quick_read()
	store i32 %reg_27, ptr %reg_26
	%reg_28 = load i32, ptr %reg_24
	%reg_29 = load i32, ptr %reg_26
	call void @add_edge(i32 %reg_28, i32 %reg_29)
	br label %Block10
}
