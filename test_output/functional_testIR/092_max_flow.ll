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

@INF = global i32 1879048192
@size = global [10 x i32] zeroinitializer
@to = global [10 x [10 x i32]] zeroinitializer
@cap = global [10 x [10 x i32]] zeroinitializer
@rev = global [10 x [10 x i32]] zeroinitializer
@used = global [10 x i32] zeroinitializer

define void @my_memset(ptr %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 12
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_1, ptr %reg_3
	store i32 %reg_2, ptr %reg_4
	br label %Block1
Block1:  ;Func body at line 12
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	br label %Block2
Block2:  ;While condition at line 15
	%reg_7 = load i32, ptr %reg_5
	%reg_8 = load i32, ptr %reg_4
	%reg_9 = icmp slt i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block3, label %Block4
Block3:  ;While body at line 15
	%reg_10 = load i32, ptr %reg_5
	%reg_11 = getelementptr i32, ptr %reg_0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_3
	store i32 %reg_12, ptr %reg_11
	%reg_13 = load i32, ptr %reg_5
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_5
	br label %Block2
Block4:  ;While end at line 15
	ret void
}

define void @add_node(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 21
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 21
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = getelementptr [10 x [10 x i32]], ptr @to, i32 0, i32 %reg_6, i32 %reg_9
	%reg_11 = load i32, ptr %reg_4
	store i32 %reg_11, ptr %reg_10
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = load i32, ptr %reg_3
	%reg_14 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	%reg_16 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_12, i32 %reg_15
	%reg_17 = load i32, ptr %reg_5
	store i32 %reg_17, ptr %reg_16
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = load i32, ptr %reg_3
	%reg_20 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = getelementptr [10 x [10 x i32]], ptr @rev, i32 0, i32 %reg_18, i32 %reg_21
	%reg_23 = load i32, ptr %reg_4
	%reg_24 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	store i32 %reg_25, ptr %reg_22
	%reg_26 = load i32, ptr %reg_4
	%reg_27 = load i32, ptr %reg_4
	%reg_28 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_28
	%reg_30 = getelementptr [10 x [10 x i32]], ptr @to, i32 0, i32 %reg_26, i32 %reg_29
	%reg_31 = load i32, ptr %reg_3
	store i32 %reg_31, ptr %reg_30
	%reg_32 = load i32, ptr %reg_4
	%reg_33 = load i32, ptr %reg_4
	%reg_34 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_33
	%reg_35 = load i32, ptr %reg_34
	%reg_36 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_32, i32 %reg_35
	%reg_37 = add i32 0, 0
	store i32 %reg_37, ptr %reg_36
	%reg_38 = load i32, ptr %reg_4
	%reg_39 = load i32, ptr %reg_4
	%reg_40 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_39
	%reg_41 = load i32, ptr %reg_40
	%reg_42 = getelementptr [10 x [10 x i32]], ptr @rev, i32 0, i32 %reg_38, i32 %reg_41
	%reg_43 = load i32, ptr %reg_3
	%reg_44 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_43
	%reg_45 = load i32, ptr %reg_44
	store i32 %reg_45, ptr %reg_42
	%reg_46 = load i32, ptr %reg_3
	%reg_47 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_46
	%reg_48 = load i32, ptr %reg_3
	%reg_49 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	%reg_51 = add i32 1, 0
	%reg_52 = add i32 %reg_50, %reg_51
	store i32 %reg_52, ptr %reg_47
	%reg_53 = load i32, ptr %reg_4
	%reg_54 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_53
	%reg_55 = load i32, ptr %reg_4
	%reg_56 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = add i32 1, 0
	%reg_59 = add i32 %reg_57, %reg_58
	store i32 %reg_59, ptr %reg_54
	ret void
}

define i32 @dfs(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 35
	%reg_52 = alloca i32
	%reg_39 = alloca i32
	%reg_13 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 35
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = load i32, ptr %reg_4
	%reg_8 = icmp eq i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block2:  ;If then at line 37
	%reg_9 = load i32, ptr %reg_5
	ret i32 %reg_9
	br label %Block3
Block3:  ;If end at line 37
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = getelementptr [10 x i32], ptr @used, i32 0, i32 %reg_10
	%reg_12 = add i32 1, 0
	store i32 %reg_12, ptr %reg_11
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_13
	br label %Block4
Block4:  ;While condition at line 42
	%reg_15 = load i32, ptr %reg_13
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = getelementptr [10 x i32], ptr @size, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = icmp slt i32 %reg_15, %reg_18
	br i1 %reg_19, label %Block5, label %Block6
Block5:  ;While body at line 42
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = load i32, ptr %reg_13
	%reg_22 = getelementptr [10 x [10 x i32]], ptr @to, i32 0, i32 %reg_20, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	%reg_24 = getelementptr [10 x i32], ptr @used, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = icmp ne i32 %reg_25, 0
	br i1 %reg_26, label %Block7, label %Block8
Block6:  ;While end at line 42
	%reg_97 = add i32 0, 0
	ret i32 %reg_97
Block7:  ;If then at line 43
	%reg_27 = load i32, ptr %reg_13
	%reg_28 = add i32 1, 0
	%reg_29 = add i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_13
	br label %Block4
Block8:  ;If end at line 43
	%reg_30 = load i32, ptr %reg_3
	%reg_31 = load i32, ptr %reg_13
	%reg_32 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_30, i32 %reg_31
	%reg_33 = load i32, ptr %reg_32
	%reg_34 = add i32 0, 0
	%reg_35 = icmp sle i32 %reg_33, %reg_34
	br i1 %reg_35, label %Block10, label %Block11
Block9:  ;Continue at line 43
	br label %Block8
Block10:  ;If then at line 44
	%reg_36 = load i32, ptr %reg_13
	%reg_37 = add i32 1, 0
	%reg_38 = add i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_13
	br label %Block4
Block11:  ;If end at line 44
	%reg_40 = add i32 0, 0
	store i32 %reg_40, ptr %reg_39
	%reg_41 = load i32, ptr %reg_5
	%reg_42 = load i32, ptr %reg_3
	%reg_43 = load i32, ptr %reg_13
	%reg_44 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_42, i32 %reg_43
	%reg_45 = load i32, ptr %reg_44
	%reg_46 = icmp slt i32 %reg_41, %reg_45
	br i1 %reg_46, label %Block13, label %Block15
Block12:  ;Continue at line 44
	br label %Block11
Block13:  ;If then at line 47
	%reg_47 = load i32, ptr %reg_5
	store i32 %reg_47, ptr %reg_39
	br label %Block14
Block14:  ;If end at line 47
	%reg_53 = load i32, ptr %reg_3
	%reg_54 = load i32, ptr %reg_13
	%reg_55 = getelementptr [10 x [10 x i32]], ptr @to, i32 0, i32 %reg_53, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	%reg_57 = load i32, ptr %reg_4
	%reg_58 = load i32, ptr %reg_39
	%reg_59 = call i32 @dfs(i32 %reg_56, i32 %reg_57, i32 %reg_58)
	store i32 %reg_59, ptr %reg_52
	%reg_60 = load i32, ptr %reg_52
	%reg_61 = add i32 0, 0
	%reg_62 = icmp sgt i32 %reg_60, %reg_61
	br i1 %reg_62, label %Block16, label %Block17
Block15:  ;If else at line 47
	%reg_48 = load i32, ptr %reg_3
	%reg_49 = load i32, ptr %reg_13
	%reg_50 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_48, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	store i32 %reg_51, ptr %reg_39
	br label %Block14
Block16:  ;If then at line 53
	%reg_63 = load i32, ptr %reg_3
	%reg_64 = load i32, ptr %reg_13
	%reg_65 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_63, i32 %reg_64
	%reg_66 = load i32, ptr %reg_3
	%reg_67 = load i32, ptr %reg_13
	%reg_68 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_66, i32 %reg_67
	%reg_69 = load i32, ptr %reg_68
	%reg_70 = load i32, ptr %reg_52
	%reg_71 = sub i32 %reg_69, %reg_70
	store i32 %reg_71, ptr %reg_65
	%reg_72 = load i32, ptr %reg_3
	%reg_73 = load i32, ptr %reg_13
	%reg_74 = getelementptr [10 x [10 x i32]], ptr @to, i32 0, i32 %reg_72, i32 %reg_73
	%reg_75 = load i32, ptr %reg_74
	%reg_76 = load i32, ptr %reg_3
	%reg_77 = load i32, ptr %reg_13
	%reg_78 = getelementptr [10 x [10 x i32]], ptr @rev, i32 0, i32 %reg_76, i32 %reg_77
	%reg_79 = load i32, ptr %reg_78
	%reg_80 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_75, i32 %reg_79
	%reg_81 = load i32, ptr %reg_3
	%reg_82 = load i32, ptr %reg_13
	%reg_83 = getelementptr [10 x [10 x i32]], ptr @to, i32 0, i32 %reg_81, i32 %reg_82
	%reg_84 = load i32, ptr %reg_83
	%reg_85 = load i32, ptr %reg_3
	%reg_86 = load i32, ptr %reg_13
	%reg_87 = getelementptr [10 x [10 x i32]], ptr @rev, i32 0, i32 %reg_85, i32 %reg_86
	%reg_88 = load i32, ptr %reg_87
	%reg_89 = getelementptr [10 x [10 x i32]], ptr @cap, i32 0, i32 %reg_84, i32 %reg_88
	%reg_90 = load i32, ptr %reg_89
	%reg_91 = load i32, ptr %reg_52
	%reg_92 = add i32 %reg_90, %reg_91
	store i32 %reg_92, ptr %reg_80
	%reg_93 = load i32, ptr %reg_52
	ret i32 %reg_93
	br label %Block17
Block17:  ;If end at line 53
	%reg_94 = load i32, ptr %reg_13
	%reg_95 = add i32 1, 0
	%reg_96 = add i32 %reg_94, %reg_95
	store i32 %reg_96, ptr %reg_13
	br label %Block4
}

define i32 @max_flow(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 63
	%reg_11 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 63
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 67
	%reg_6 = add i32 1, 0
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 67
	%reg_8 = getelementptr [10 x i32], ptr @used, i32 0
	%reg_9 = add i32 0, 0
	%reg_10 = add i32 10, 0
	call void @my_memset(ptr %reg_8, i32 %reg_9, i32 %reg_10)
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = load i32, ptr %reg_3
	%reg_14 = load i32, ptr @INF
	%reg_15 = call i32 @dfs(i32 %reg_12, i32 %reg_13, i32 %reg_14)
	store i32 %reg_15, ptr %reg_11
	%reg_16 = load i32, ptr %reg_11
	%reg_17 = add i32 0, 0
	%reg_18 = icmp eq i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block5, label %Block6
Block4:  ;While end at line 67
	ret i32 0
Block5:  ;If then at line 71
	%reg_19 = load i32, ptr %reg_4
	ret i32 %reg_19
	br label %Block6
Block6:  ;If end at line 71
	%reg_20 = load i32, ptr %reg_4
	%reg_21 = load i32, ptr %reg_11
	%reg_22 = add i32 %reg_20, %reg_21
	store i32 %reg_22, ptr %reg_4
	br label %Block2
}

define i32 @main()
{
Block0:  ;Func define at line 77
	%reg_18 = alloca i32
	%reg_14 = alloca i32
	%reg_12 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 77
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = call i32 @getint()
	store i32 %reg_4, ptr %reg_0
	%reg_5 = call i32 @getint()
	store i32 %reg_5, ptr %reg_2
	%reg_6 = getelementptr [10 x i32], ptr @size, i32 0
	%reg_7 = add i32 0, 0
	%reg_8 = add i32 10, 0
	call void @my_memset(ptr %reg_6, i32 %reg_7, i32 %reg_8)
	br label %Block2
Block2:  ;While condition at line 84
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = add i32 0, 0
	%reg_11 = icmp sgt i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block3, label %Block4
Block3:  ;While body at line 84
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_14
	%reg_16 = call i32 @getint()
	store i32 %reg_16, ptr %reg_12
	%reg_17 = call i32 @getint()
	store i32 %reg_17, ptr %reg_14
	%reg_19 = call i32 @getint()
	store i32 %reg_19, ptr %reg_18
	%reg_20 = load i32, ptr %reg_12
	%reg_21 = load i32, ptr %reg_14
	%reg_22 = load i32, ptr %reg_18
	call void @add_node(i32 %reg_20, i32 %reg_21, i32 %reg_22)
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = add i32 1, 0
	%reg_25 = sub i32 %reg_23, %reg_24
	store i32 %reg_25, ptr %reg_2
	br label %Block2
Block4:  ;While end at line 84
	%reg_26 = add i32 1, 0
	%reg_27 = load i32, ptr %reg_0
	%reg_28 = call i32 @max_flow(i32 %reg_26, i32 %reg_27)
	call void @putint(i32 %reg_28)
	%reg_29 = add i32 10, 0
	call void @putch(i32 %reg_29)
	%reg_30 = add i32 0, 0
	ret i32 %reg_30
}
