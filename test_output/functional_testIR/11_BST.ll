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

@space = global i32 32
@LF = global i32 10
@maxNode = global i32 10000
@value = global [10000 x i32] zeroinitializer
@left_child = global [10000 x i32] zeroinitializer
@right_child = global [10000 x i32] zeroinitializer
@now = global i32 zeroinitializer

define i32 @search(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 9
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 9
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = add i32 1, 0
	%reg_6 = sub i32 0, %reg_5
	%reg_7 = icmp eq i32 %reg_4, %reg_6
	br i1 %reg_7, label %Block2, label %Block5
Block2:  ;If then at line 10
	%reg_13 = load i32, ptr %reg_2
	ret i32 %reg_13
	br label %Block3
Block3:  ;If end at line 10
	ret i32 0
Block4:  ;If else at line 10
	%reg_14 = load i32, ptr %reg_3
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	%reg_18 = icmp sgt i32 %reg_14, %reg_17
	br i1 %reg_18, label %Block6, label %Block8
Block5:  ;Or opertor at line 10
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = icmp eq i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block2, label %Block4
Block6:  ;If then at line 12
	%reg_19 = load i32, ptr %reg_2
	%reg_20 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = load i32, ptr %reg_3
	%reg_23 = call i32 @search(i32 %reg_21, i32 %reg_22)
	ret i32 %reg_23
	br label %Block7
Block7:  ;If end at line 12
	br label %Block3
Block8:  ;If else at line 12
	%reg_24 = load i32, ptr %reg_2
	%reg_25 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = load i32, ptr %reg_3
	%reg_28 = call i32 @search(i32 %reg_26, i32 %reg_27)
	ret i32 %reg_28
	br label %Block7
}

define i32 @find_minimum(i32 %reg_0)
{
Block0:  ;Func define at line 18
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 18
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 1, 0
	%reg_4 = sub i32 0, %reg_3
	%reg_5 = icmp eq i32 %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block4
Block2:  ;If then at line 19
	%reg_6 = add i32 1, 0
	%reg_7 = sub i32 0, %reg_6
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 19
	%reg_18 = load i32, ptr %reg_1
	ret i32 %reg_18
Block4:  ;If else at line 19
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = add i32 1, 0
	%reg_12 = sub i32 0, %reg_11
	%reg_13 = icmp ne i32 %reg_10, %reg_12
	br i1 %reg_13, label %Block5, label %Block6
Block5:  ;If then at line 22
	%reg_14 = load i32, ptr %reg_1
	%reg_15 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = call i32 @find_minimum(i32 %reg_16)
	ret i32 %reg_17
	br label %Block6
Block6:  ;If end at line 22
	br label %Block3
}

define i32 @new_node(i32 %reg_0)
{
Block0:  ;Func define at line 28
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 28
	%reg_2 = load i32, ptr @now
	%reg_3 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_1
	store i32 %reg_4, ptr %reg_3
	%reg_5 = load i32, ptr @now
	%reg_6 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_5
	%reg_7 = add i32 1, 0
	%reg_8 = sub i32 0, %reg_7
	store i32 %reg_8, ptr %reg_6
	%reg_9 = load i32, ptr @now
	%reg_10 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_9
	%reg_11 = add i32 1, 0
	%reg_12 = sub i32 0, %reg_11
	store i32 %reg_12, ptr %reg_10
	%reg_13 = load i32, ptr @now
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr @now
	%reg_16 = load i32, ptr @now
	%reg_17 = add i32 1, 0
	%reg_18 = sub i32 %reg_16, %reg_17
	ret i32 %reg_18
}

define i32 @insert(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 36
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 36
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = add i32 1, 0
	%reg_6 = sub i32 0, %reg_5
	%reg_7 = icmp eq i32 %reg_4, %reg_6
	br i1 %reg_7, label %Block2, label %Block4
Block2:  ;If then at line 37
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = call i32 @new_node(i32 %reg_8)
	ret i32 %reg_9
	br label %Block3
Block3:  ;If end at line 37
	%reg_29 = load i32, ptr %reg_2
	ret i32 %reg_29
Block4:  ;If else at line 37
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = icmp sgt i32 %reg_10, %reg_13
	br i1 %reg_14, label %Block5, label %Block7
Block5:  ;If then at line 40
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = call i32 @insert(i32 %reg_19, i32 %reg_20)
	store i32 %reg_21, ptr %reg_16
	br label %Block6
Block6:  ;If end at line 40
	br label %Block3
Block7:  ;If else at line 40
	%reg_22 = load i32, ptr %reg_2
	%reg_23 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_2
	%reg_25 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = load i32, ptr %reg_3
	%reg_28 = call i32 @insert(i32 %reg_26, i32 %reg_27)
	store i32 %reg_28, ptr %reg_23
	br label %Block6
}

define i32 @delete(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 48
	%reg_72 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 48
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = add i32 1, 0
	%reg_6 = sub i32 0, %reg_5
	%reg_7 = icmp eq i32 %reg_4, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
Block2:  ;If then at line 49
	%reg_8 = add i32 1, 0
	%reg_9 = sub i32 0, %reg_8
	ret i32 %reg_9
	br label %Block3
Block3:  ;If end at line 49
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = icmp sgt i32 %reg_10, %reg_13
	br i1 %reg_14, label %Block4, label %Block6
Block4:  ;If then at line 50
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = call i32 @delete(i32 %reg_19, i32 %reg_20)
	store i32 %reg_21, ptr %reg_16
	br label %Block5
Block5:  ;If end at line 50
	%reg_91 = load i32, ptr %reg_2
	ret i32 %reg_91
Block6:  ;If else at line 50
	%reg_22 = load i32, ptr %reg_3
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = icmp slt i32 %reg_22, %reg_25
	br i1 %reg_26, label %Block7, label %Block9
Block7:  ;If then at line 51
	%reg_27 = load i32, ptr %reg_2
	%reg_28 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_2
	%reg_30 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	%reg_32 = load i32, ptr %reg_3
	%reg_33 = call i32 @delete(i32 %reg_31, i32 %reg_32)
	store i32 %reg_33, ptr %reg_28
	br label %Block8
Block8:  ;If end at line 51
	br label %Block5
Block9:  ;If else at line 51
	%reg_34 = load i32, ptr %reg_2
	%reg_35 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_34
	%reg_36 = load i32, ptr %reg_35
	%reg_37 = add i32 1, 0
	%reg_38 = sub i32 0, %reg_37
	%reg_39 = icmp eq i32 %reg_36, %reg_38
	br i1 %reg_39, label %Block13, label %Block12
Block10:  ;If then at line 53
	%reg_46 = add i32 1, 0
	%reg_47 = sub i32 0, %reg_46
	ret i32 %reg_47
	br label %Block11
Block11:  ;If end at line 53
	br label %Block8
Block12:  ;If else at line 53
	%reg_48 = load i32, ptr %reg_2
	%reg_49 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	%reg_51 = add i32 1, 0
	%reg_52 = sub i32 0, %reg_51
	%reg_53 = icmp eq i32 %reg_50, %reg_52
	br i1 %reg_53, label %Block14, label %Block17
Block13:  ;And opertor at line 53
	%reg_40 = load i32, ptr %reg_2
	%reg_41 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	%reg_43 = add i32 1, 0
	%reg_44 = sub i32 0, %reg_43
	%reg_45 = icmp eq i32 %reg_42, %reg_44
	br i1 %reg_45, label %Block10, label %Block12
Block14:  ;If then at line 54
	%reg_60 = load i32, ptr %reg_2
	%reg_61 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_60
	%reg_62 = load i32, ptr %reg_61
	%reg_63 = add i32 1, 0
	%reg_64 = sub i32 0, %reg_63
	%reg_65 = icmp eq i32 %reg_62, %reg_64
	br i1 %reg_65, label %Block18, label %Block20
Block15:  ;If end at line 54
	br label %Block11
Block16:  ;If else at line 54
	%reg_73 = load i32, ptr %reg_2
	%reg_74 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_73
	%reg_75 = load i32, ptr %reg_74
	%reg_76 = call i32 @find_minimum(i32 %reg_75)
	store i32 %reg_76, ptr %reg_72
	%reg_77 = load i32, ptr %reg_2
	%reg_78 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_77
	%reg_79 = load i32, ptr %reg_72
	%reg_80 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_79
	%reg_81 = load i32, ptr %reg_80
	store i32 %reg_81, ptr %reg_78
	%reg_82 = load i32, ptr %reg_2
	%reg_83 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_82
	%reg_84 = load i32, ptr %reg_2
	%reg_85 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_84
	%reg_86 = load i32, ptr %reg_85
	%reg_87 = load i32, ptr %reg_72
	%reg_88 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	%reg_90 = call i32 @delete(i32 %reg_86, i32 %reg_89)
	store i32 %reg_90, ptr %reg_83
	br label %Block15
Block17:  ;Or opertor at line 54
	%reg_54 = load i32, ptr %reg_2
	%reg_55 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	%reg_57 = add i32 1, 0
	%reg_58 = sub i32 0, %reg_57
	%reg_59 = icmp eq i32 %reg_56, %reg_58
	br i1 %reg_59, label %Block14, label %Block16
Block18:  ;If then at line 55
	%reg_66 = load i32, ptr %reg_2
	%reg_67 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	ret i32 %reg_68
	br label %Block19
Block19:  ;If end at line 55
	br label %Block15
Block20:  ;If else at line 55
	%reg_69 = load i32, ptr %reg_2
	%reg_70 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_69
	%reg_71 = load i32, ptr %reg_70
	ret i32 %reg_71
	br label %Block19
}

define void @inorder(i32 %reg_0)
{
Block0:  ;Func define at line 67
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 67
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 1, 0
	%reg_4 = sub i32 0, %reg_3
	%reg_5 = icmp ne i32 %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 68
	%reg_6 = load i32, ptr %reg_1
	%reg_7 = getelementptr [10000 x i32], ptr @left_child, i32 0, i32 %reg_6
	%reg_8 = load i32, ptr %reg_7
	call void @inorder(i32 %reg_8)
	%reg_9 = load i32, ptr %reg_1
	%reg_10 = getelementptr [10000 x i32], ptr @value, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	call void @putint(i32 %reg_11)
	%reg_12 = load i32, ptr @space
	call void @putch(i32 %reg_12)
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = getelementptr [10000 x i32], ptr @right_child, i32 0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	call void @inorder(i32 %reg_15)
	br label %Block3
Block3:  ;If end at line 68
	ret void
}

define i32 @main()
{
Block0:  ;Func define at line 76
	%reg_9 = alloca i32
	%reg_6 = alloca i32
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 76
	%reg_0 = add i32 0, 0
	store i32 %reg_0, ptr @now
	%reg_2 = call i32 @getint()
	store i32 %reg_2, ptr %reg_1
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = icmp eq i32 %reg_3, 0
	br i1 %reg_4, label %Block2, label %Block3
Block2:  ;If then at line 79
	%reg_5 = add i32 0, 0
	ret i32 %reg_5
	br label %Block3
Block3:  ;If end at line 79
	%reg_7 = call i32 @getint()
	%reg_8 = call i32 @new_node(i32 %reg_7)
	store i32 %reg_8, ptr %reg_6
	%reg_10 = add i32 1, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block4
Block4:  ;While condition at line 81
	%reg_11 = load i32, ptr %reg_9
	%reg_12 = load i32, ptr %reg_1
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block5, label %Block6
Block5:  ;While body at line 81
	%reg_14 = load i32, ptr %reg_6
	%reg_15 = call i32 @getint()
	%reg_16 = call i32 @insert(i32 %reg_14, i32 %reg_15)
	%reg_17 = load i32, ptr %reg_9
	%reg_18 = add i32 1, 0
	%reg_19 = add i32 %reg_17, %reg_18
	store i32 %reg_19, ptr %reg_9
	br label %Block4
Block6:  ;While end at line 81
	%reg_20 = load i32, ptr %reg_6
	call void @inorder(i32 %reg_20)
	%reg_21 = load i32, ptr @LF
	call void @putch(i32 %reg_21)
	%reg_22 = call i32 @getint()
	store i32 %reg_22, ptr %reg_1
	%reg_23 = add i32 0, 0
	store i32 %reg_23, ptr %reg_9
	br label %Block7
Block7:  ;While condition at line 88
	%reg_24 = load i32, ptr %reg_9
	%reg_25 = load i32, ptr %reg_1
	%reg_26 = icmp slt i32 %reg_24, %reg_25
	br i1 %reg_26, label %Block8, label %Block9
Block8:  ;While body at line 88
	%reg_27 = load i32, ptr %reg_6
	%reg_28 = call i32 @getint()
	%reg_29 = call i32 @delete(i32 %reg_27, i32 %reg_28)
	store i32 %reg_29, ptr %reg_6
	%reg_30 = load i32, ptr %reg_9
	%reg_31 = add i32 1, 0
	%reg_32 = add i32 %reg_30, %reg_31
	store i32 %reg_32, ptr %reg_9
	br label %Block7
Block9:  ;While end at line 88
	%reg_33 = load i32, ptr %reg_6
	call void @inorder(i32 %reg_33)
	%reg_34 = load i32, ptr @LF
	call void @putch(i32 %reg_34)
	%reg_35 = add i32 0, 0
	ret i32 %reg_35
}
