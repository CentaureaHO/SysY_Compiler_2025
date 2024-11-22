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

@maxn = global i32 10005
@n = global i32 zeroinitializer
@m = global i32 zeroinitializer
@f = global [10005 x [20 x i32]] zeroinitializer
@dep = global [10005 x i32] zeroinitializer
@to = global [10005 x i32] zeroinitializer
@next = global [10005 x i32] zeroinitializer
@head = global [10005 x i32] zeroinitializer
@cnt = global i32 0

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
Block0:  ;Func define at line 17
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 17
	%reg_4 = load i32, ptr @cnt
	%reg_5 = getelementptr [10005 x i32], ptr @to, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_3
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr @cnt
	%reg_8 = getelementptr [10005 x i32], ptr @next, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = getelementptr [10005 x i32], ptr @head, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	store i32 %reg_11, ptr %reg_8
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = getelementptr [10005 x i32], ptr @head, i32 0, i32 %reg_12
	%reg_14 = load i32, ptr @cnt
	store i32 %reg_14, ptr %reg_13
	%reg_15 = load i32, ptr @cnt
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr @cnt
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = add i32 0, 0
	%reg_20 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_18, i32 %reg_19
	%reg_21 = load i32, ptr %reg_2
	store i32 %reg_21, ptr %reg_20
	ret void
}

define void @init()
{
Block0:  ;Func define at line 24
	%reg_3 = alloca i32
	br label %Block1
Block1:  ;Func body at line 24
	%reg_0 = add i32 0, 0
	%reg_1 = getelementptr [10005 x i32], ptr @dep, i32 0, i32 %reg_0
	%reg_2 = add i32 1061109567, 0
	store i32 %reg_2, ptr %reg_1
	%reg_4 = add i32 1, 0
	store i32 %reg_4, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 27
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = load i32, ptr @n
	%reg_7 = icmp sle i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 27
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = getelementptr [10005 x i32], ptr @head, i32 0, i32 %reg_8
	%reg_10 = add i32 1, 0
	%reg_11 = sub i32 0, %reg_10
	store i32 %reg_11, ptr %reg_9
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = add i32 1, 0
	%reg_14 = add i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_3
	br label %Block2
Block4:  ;While end at line 27
	ret void
}

define void @tree(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 32
	%reg_36 = alloca i32
	%reg_7 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 32
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = getelementptr [10005 x i32], ptr @dep, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_3
	store i32 %reg_6, ptr %reg_5
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	br label %Block2
Block2:  ;While condition at line 35
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = load i32, ptr %reg_7
	%reg_11 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_9, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	%reg_13 = icmp ne i32 %reg_12, 0
	br i1 %reg_13, label %Block3, label %Block4
Block3:  ;While body at line 35
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = load i32, ptr %reg_7
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 %reg_15, %reg_16
	%reg_18 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_14, i32 %reg_17
	%reg_19 = load i32, ptr %reg_2
	%reg_20 = load i32, ptr %reg_7
	%reg_21 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_19, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	%reg_23 = load i32, ptr %reg_7
	%reg_24 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_22, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	store i32 %reg_25, ptr %reg_18
	%reg_26 = load i32, ptr %reg_7
	%reg_27 = add i32 1, 0
	%reg_28 = add i32 %reg_26, %reg_27
	store i32 %reg_28, ptr %reg_7
	br label %Block2
Block4:  ;While end at line 35
	%reg_29 = load i32, ptr %reg_2
	%reg_30 = getelementptr [10005 x i32], ptr @head, i32 0, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	store i32 %reg_31, ptr %reg_7
	br label %Block5
Block5:  ;While condition at line 40
	%reg_32 = load i32, ptr %reg_7
	%reg_33 = add i32 1, 0
	%reg_34 = sub i32 0, %reg_33
	%reg_35 = icmp ne i32 %reg_32, %reg_34
	br i1 %reg_35, label %Block6, label %Block7
Block6:  ;While body at line 40
	%reg_37 = load i32, ptr %reg_7
	%reg_38 = getelementptr [10005 x i32], ptr @to, i32 0, i32 %reg_37
	%reg_39 = load i32, ptr %reg_38
	store i32 %reg_39, ptr %reg_36
	%reg_40 = load i32, ptr %reg_36
	%reg_41 = load i32, ptr %reg_3
	%reg_42 = add i32 1, 0
	%reg_43 = add i32 %reg_41, %reg_42
	call void @tree(i32 %reg_40, i32 %reg_43)
	%reg_44 = load i32, ptr %reg_7
	%reg_45 = getelementptr [10005 x i32], ptr @next, i32 0, i32 %reg_44
	%reg_46 = load i32, ptr %reg_45
	store i32 %reg_46, ptr %reg_7
	br label %Block5
Block7:  ;While end at line 40
	ret void
}

define i32 @LCA(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 46
	%reg_15 = alloca i32
	%reg_11 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 46
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = getelementptr [10005 x i32], ptr @dep, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = getelementptr [10005 x i32], ptr @dep, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = icmp slt i32 %reg_6, %reg_9
	br i1 %reg_10, label %Block2, label %Block3
Block2:  ;If then at line 47
	%reg_12 = load i32, ptr %reg_2
	store i32 %reg_12, ptr %reg_11
	%reg_13 = load i32, ptr %reg_3
	store i32 %reg_13, ptr %reg_2
	%reg_14 = load i32, ptr %reg_11
	store i32 %reg_14, ptr %reg_3
	br label %Block3
Block3:  ;If end at line 47
	%reg_16 = add i32 19, 0
	store i32 %reg_16, ptr %reg_15
	br label %Block4
Block4:  ;While condition at line 53
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = getelementptr [10005 x i32], ptr @dep, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = getelementptr [10005 x i32], ptr @dep, i32 0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	%reg_23 = icmp sgt i32 %reg_19, %reg_22
	br i1 %reg_23, label %Block5, label %Block6
Block5:  ;While body at line 53
	%reg_24 = load i32, ptr %reg_2
	%reg_25 = load i32, ptr %reg_15
	%reg_26 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_24, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	%reg_28 = icmp ne i32 %reg_27, 0
	br i1 %reg_28, label %Block9, label %Block8
Block6:  ;While end at line 53
	%reg_46 = load i32, ptr %reg_2
	%reg_47 = load i32, ptr %reg_3
	%reg_48 = icmp eq i32 %reg_46, %reg_47
	br i1 %reg_48, label %Block10, label %Block11
Block7:  ;If then at line 54
	%reg_39 = load i32, ptr %reg_2
	%reg_40 = load i32, ptr %reg_15
	%reg_41 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_39, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	store i32 %reg_42, ptr %reg_2
	br label %Block8
Block8:  ;If end at line 54
	%reg_43 = load i32, ptr %reg_15
	%reg_44 = add i32 1, 0
	%reg_45 = sub i32 %reg_43, %reg_44
	store i32 %reg_45, ptr %reg_15
	br label %Block4
Block9:  ;And opertor at line 54
	%reg_29 = load i32, ptr %reg_2
	%reg_30 = load i32, ptr %reg_15
	%reg_31 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_29, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	%reg_33 = getelementptr [10005 x i32], ptr @dep, i32 0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = load i32, ptr %reg_3
	%reg_36 = getelementptr [10005 x i32], ptr @dep, i32 0, i32 %reg_35
	%reg_37 = load i32, ptr %reg_36
	%reg_38 = icmp sge i32 %reg_34, %reg_37
	br i1 %reg_38, label %Block7, label %Block8
Block10:  ;If then at line 58
	%reg_49 = load i32, ptr %reg_2
	ret i32 %reg_49
	br label %Block11
Block11:  ;If end at line 58
	%reg_50 = add i32 19, 0
	store i32 %reg_50, ptr %reg_15
	br label %Block12
Block12:  ;While condition at line 60
	%reg_51 = load i32, ptr %reg_15
	%reg_52 = add i32 0, 0
	%reg_53 = icmp sge i32 %reg_51, %reg_52
	br i1 %reg_53, label %Block13, label %Block14
Block13:  ;While body at line 60
	%reg_54 = load i32, ptr %reg_2
	%reg_55 = load i32, ptr %reg_15
	%reg_56 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_54, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = load i32, ptr %reg_3
	%reg_59 = load i32, ptr %reg_15
	%reg_60 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_58, i32 %reg_59
	%reg_61 = load i32, ptr %reg_60
	%reg_62 = icmp ne i32 %reg_57, %reg_61
	br i1 %reg_62, label %Block15, label %Block16
Block14:  ;While end at line 60
	%reg_74 = load i32, ptr %reg_2
	%reg_75 = add i32 0, 0
	%reg_76 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_74, i32 %reg_75
	%reg_77 = load i32, ptr %reg_76
	ret i32 %reg_77
Block15:  ;If then at line 61
	%reg_63 = load i32, ptr %reg_2
	%reg_64 = load i32, ptr %reg_15
	%reg_65 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_63, i32 %reg_64
	%reg_66 = load i32, ptr %reg_65
	store i32 %reg_66, ptr %reg_2
	%reg_67 = load i32, ptr %reg_3
	%reg_68 = load i32, ptr %reg_15
	%reg_69 = getelementptr [10005 x [20 x i32]], ptr @f, i32 0, i32 %reg_67, i32 %reg_68
	%reg_70 = load i32, ptr %reg_69
	store i32 %reg_70, ptr %reg_3
	br label %Block16
Block16:  ;If end at line 61
	%reg_71 = load i32, ptr %reg_15
	%reg_72 = add i32 1, 0
	%reg_73 = sub i32 %reg_71, %reg_72
	store i32 %reg_73, ptr %reg_15
	br label %Block12
}

define i32 @main()
{
Block0:  ;Func define at line 69
	%reg_22 = alloca i32
	%reg_20 = alloca i32
	%reg_9 = alloca i32
	%reg_7 = alloca i32
	%reg_2 = alloca i32
	br label %Block1
Block1:  ;Func body at line 69
	%reg_0 = call i32 @quick_read()
	store i32 %reg_0, ptr @n
	%reg_1 = call i32 @quick_read()
	store i32 %reg_1, ptr @m
	call void @init()
	%reg_3 = add i32 1, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 73
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @n
	%reg_6 = icmp ne i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 73
	%reg_8 = call i32 @quick_read()
	store i32 %reg_8, ptr %reg_7
	%reg_10 = call i32 @quick_read()
	store i32 %reg_10, ptr %reg_9
	%reg_11 = load i32, ptr %reg_7
	%reg_12 = load i32, ptr %reg_9
	call void @add_edge(i32 %reg_11, i32 %reg_12)
	%reg_13 = load i32, ptr %reg_2
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_2
	br label %Block2
Block4:  ;While end at line 73
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 1, 0
	call void @tree(i32 %reg_16, i32 %reg_17)
	br label %Block5
Block5:  ;While condition at line 79
	%reg_18 = load i32, ptr @m
	%reg_19 = icmp ne i32 %reg_18, 0
	br i1 %reg_19, label %Block6, label %Block7
Block6:  ;While body at line 79
	%reg_21 = call i32 @quick_read()
	store i32 %reg_21, ptr %reg_20
	%reg_23 = call i32 @quick_read()
	store i32 %reg_23, ptr %reg_22
	%reg_24 = load i32, ptr %reg_20
	%reg_25 = load i32, ptr %reg_22
	%reg_26 = call i32 @LCA(i32 %reg_24, i32 %reg_25)
	call void @putint(i32 %reg_26)
	%reg_27 = add i32 10, 0
	call void @putch(i32 %reg_27)
	%reg_28 = load i32, ptr @m
	%reg_29 = add i32 1, 0
	%reg_30 = sub i32 %reg_28, %reg_29
	store i32 %reg_30, ptr @m
	br label %Block5
Block7:  ;While end at line 79
	%reg_31 = add i32 0, 0
	ret i32 %reg_31
}
