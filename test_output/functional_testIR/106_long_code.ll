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

define i32 @bubblesort(ptr %reg_0)
{
Block0:  ;Func define at line 4
	%reg_28 = alloca i32
	%reg_3 = alloca i32
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 4
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_1
	br label %Block2
Block2:  ;While condition at line 8
	%reg_6 = load i32, ptr %reg_1
	%reg_7 = load i32, ptr @n
	%reg_8 = add i32 1, 0
	%reg_9 = sub i32 %reg_7, %reg_8
	%reg_10 = icmp slt i32 %reg_6, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_3
	br label %Block5
Block4:  ;While end at line 8
	%reg_51 = add i32 0, 0
	ret i32 %reg_51
Block5:  ;While condition at line 11
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = load i32, ptr @n
	%reg_14 = load i32, ptr %reg_1
	%reg_15 = sub i32 %reg_13, %reg_14
	%reg_16 = add i32 1, 0
	%reg_17 = sub i32 %reg_15, %reg_16
	%reg_18 = icmp slt i32 %reg_12, %reg_17
	br i1 %reg_18, label %Block6, label %Block7
Block6:  ;While body at line 11
	%reg_19 = load i32, ptr %reg_3
	%reg_20 = getelementptr i32, ptr %reg_0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = load i32, ptr %reg_3
	%reg_23 = add i32 1, 0
	%reg_24 = add i32 %reg_22, %reg_23
	%reg_25 = getelementptr i32, ptr %reg_0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = icmp sgt i32 %reg_21, %reg_26
	br i1 %reg_27, label %Block8, label %Block9
Block7:  ;While end at line 11
	%reg_48 = load i32, ptr %reg_1
	%reg_49 = add i32 1, 0
	%reg_50 = add i32 %reg_48, %reg_49
	store i32 %reg_50, ptr %reg_1
	br label %Block2
Block8:  ;If then at line 12
	%reg_29 = add i32 0, 0
	store i32 %reg_29, ptr %reg_28
	%reg_30 = load i32, ptr %reg_3
	%reg_31 = add i32 1, 0
	%reg_32 = add i32 %reg_30, %reg_31
	%reg_33 = getelementptr i32, ptr %reg_0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	store i32 %reg_34, ptr %reg_28
	%reg_35 = load i32, ptr %reg_3
	%reg_36 = add i32 1, 0
	%reg_37 = add i32 %reg_35, %reg_36
	%reg_38 = getelementptr i32, ptr %reg_0, i32 %reg_37
	%reg_39 = load i32, ptr %reg_3
	%reg_40 = getelementptr i32, ptr %reg_0, i32 %reg_39
	%reg_41 = load i32, ptr %reg_40
	store i32 %reg_41, ptr %reg_38
	%reg_42 = load i32, ptr %reg_3
	%reg_43 = getelementptr i32, ptr %reg_0, i32 %reg_42
	%reg_44 = load i32, ptr %reg_28
	store i32 %reg_44, ptr %reg_43
	br label %Block9
Block9:  ;If end at line 12
	%reg_45 = load i32, ptr %reg_3
	%reg_46 = add i32 1, 0
	%reg_47 = add i32 %reg_45, %reg_46
	store i32 %reg_47, ptr %reg_3
	br label %Block5
}

define i32 @insertsort(ptr %reg_0)
{
Block0:  ;Func define at line 26
	%reg_12 = alloca i32
	%reg_7 = alloca i32
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 26
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	%reg_3 = add i32 1, 0
	store i32 %reg_3, ptr %reg_1
	br label %Block2
Block2:  ;While condition at line 29
	%reg_4 = load i32, ptr %reg_1
	%reg_5 = load i32, ptr @n
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 29
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	%reg_9 = load i32, ptr %reg_1
	%reg_10 = getelementptr i32, ptr %reg_0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	store i32 %reg_11, ptr %reg_7
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = load i32, ptr %reg_1
	%reg_15 = add i32 1, 0
	%reg_16 = sub i32 %reg_14, %reg_15
	store i32 %reg_16, ptr %reg_12
	br label %Block5
Block4:  ;While end at line 29
	%reg_44 = add i32 0, 0
	ret i32 %reg_44
Block5:  ;While condition at line 35
	%reg_17 = load i32, ptr %reg_12
	%reg_18 = add i32 1, 0
	%reg_19 = sub i32 0, %reg_18
	%reg_20 = icmp sgt i32 %reg_17, %reg_19
	br i1 %reg_20, label %Block8, label %Block7
Block6:  ;While body at line 35
	%reg_26 = load i32, ptr %reg_12
	%reg_27 = add i32 1, 0
	%reg_28 = add i32 %reg_26, %reg_27
	%reg_29 = getelementptr i32, ptr %reg_0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_12
	%reg_31 = getelementptr i32, ptr %reg_0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	store i32 %reg_32, ptr %reg_29
	%reg_33 = load i32, ptr %reg_12
	%reg_34 = add i32 1, 0
	%reg_35 = sub i32 %reg_33, %reg_34
	store i32 %reg_35, ptr %reg_12
	br label %Block5
Block7:  ;While end at line 35
	%reg_36 = load i32, ptr %reg_12
	%reg_37 = add i32 1, 0
	%reg_38 = add i32 %reg_36, %reg_37
	%reg_39 = getelementptr i32, ptr %reg_0, i32 %reg_38
	%reg_40 = load i32, ptr %reg_7
	store i32 %reg_40, ptr %reg_39
	%reg_41 = load i32, ptr %reg_1
	%reg_42 = add i32 1, 0
	%reg_43 = add i32 %reg_41, %reg_42
	store i32 %reg_43, ptr %reg_1
	br label %Block2
Block8:  ;And opertor at line 35
	%reg_21 = load i32, ptr %reg_7
	%reg_22 = load i32, ptr %reg_12
	%reg_23 = getelementptr i32, ptr %reg_0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = icmp slt i32 %reg_21, %reg_24
	br i1 %reg_25, label %Block6, label %Block7
}

define i32 @QuickSort(ptr %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 46
	%reg_71 = alloca i32
	%reg_14 = alloca i32
	%reg_11 = alloca i32
	%reg_8 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_1, ptr %reg_3
	store i32 %reg_2, ptr %reg_4
	br label %Block1
Block1:  ;Func body at line 46
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = icmp slt i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
Block2:  ;If then at line 48
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	%reg_10 = load i32, ptr %reg_3
	store i32 %reg_10, ptr %reg_8
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_11
	%reg_13 = load i32, ptr %reg_4
	store i32 %reg_13, ptr %reg_11
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_14
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = getelementptr i32, ptr %reg_0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	store i32 %reg_18, ptr %reg_14
	br label %Block4
Block3:  ;If end at line 48
	%reg_87 = add i32 0, 0
	ret i32 %reg_87
Block4:  ;While condition at line 56
	%reg_19 = load i32, ptr %reg_8
	%reg_20 = load i32, ptr %reg_11
	%reg_21 = icmp slt i32 %reg_19, %reg_20
	br i1 %reg_21, label %Block5, label %Block6
Block5:  ;While body at line 56
	br label %Block7
Block6:  ;While end at line 56
	%reg_68 = load i32, ptr %reg_8
	%reg_69 = getelementptr i32, ptr %reg_0, i32 %reg_68
	%reg_70 = load i32, ptr %reg_14
	store i32 %reg_70, ptr %reg_69
	%reg_72 = add i32 0, 0
	store i32 %reg_72, ptr %reg_71
	%reg_73 = load i32, ptr %reg_8
	%reg_74 = add i32 1, 0
	%reg_75 = sub i32 %reg_73, %reg_74
	store i32 %reg_75, ptr %reg_71
	%reg_76 = getelementptr i32, ptr %reg_0
	%reg_77 = load i32, ptr %reg_3
	%reg_78 = load i32, ptr %reg_71
	%reg_79 = call i32 @QuickSort(ptr %reg_76, i32 %reg_77, i32 %reg_78)
	store i32 %reg_79, ptr %reg_71
	%reg_80 = load i32, ptr %reg_8
	%reg_81 = add i32 1, 0
	%reg_82 = add i32 %reg_80, %reg_81
	store i32 %reg_82, ptr %reg_71
	%reg_83 = getelementptr i32, ptr %reg_0
	%reg_84 = load i32, ptr %reg_71
	%reg_85 = load i32, ptr %reg_4
	%reg_86 = call i32 @QuickSort(ptr %reg_83, i32 %reg_84, i32 %reg_85)
	store i32 %reg_86, ptr %reg_71
	br label %Block3
Block7:  ;While condition at line 58
	%reg_22 = load i32, ptr %reg_8
	%reg_23 = load i32, ptr %reg_11
	%reg_24 = icmp slt i32 %reg_22, %reg_23
	br i1 %reg_24, label %Block10, label %Block9
Block8:  ;While body at line 58
	%reg_32 = load i32, ptr %reg_11
	%reg_33 = add i32 1, 0
	%reg_34 = sub i32 %reg_32, %reg_33
	store i32 %reg_34, ptr %reg_11
	br label %Block7
Block9:  ;While end at line 58
	%reg_35 = load i32, ptr %reg_8
	%reg_36 = load i32, ptr %reg_11
	%reg_37 = icmp slt i32 %reg_35, %reg_36
	br i1 %reg_37, label %Block11, label %Block12
Block10:  ;And opertor at line 58
	%reg_25 = load i32, ptr %reg_11
	%reg_26 = getelementptr i32, ptr %reg_0, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	%reg_28 = load i32, ptr %reg_14
	%reg_29 = add i32 1, 0
	%reg_30 = sub i32 %reg_28, %reg_29
	%reg_31 = icmp sgt i32 %reg_27, %reg_30
	br i1 %reg_31, label %Block8, label %Block9
Block11:  ;If then at line 63
	%reg_38 = load i32, ptr %reg_8
	%reg_39 = getelementptr i32, ptr %reg_0, i32 %reg_38
	%reg_40 = load i32, ptr %reg_11
	%reg_41 = getelementptr i32, ptr %reg_0, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	store i32 %reg_42, ptr %reg_39
	%reg_43 = load i32, ptr %reg_8
	%reg_44 = add i32 1, 0
	%reg_45 = add i32 %reg_43, %reg_44
	store i32 %reg_45, ptr %reg_8
	br label %Block12
Block12:  ;If end at line 63
	br label %Block13
Block13:  ;While condition at line 69
	%reg_46 = load i32, ptr %reg_8
	%reg_47 = load i32, ptr %reg_11
	%reg_48 = icmp slt i32 %reg_46, %reg_47
	br i1 %reg_48, label %Block16, label %Block15
Block14:  ;While body at line 69
	%reg_54 = load i32, ptr %reg_8
	%reg_55 = add i32 1, 0
	%reg_56 = add i32 %reg_54, %reg_55
	store i32 %reg_56, ptr %reg_8
	br label %Block13
Block15:  ;While end at line 69
	%reg_57 = load i32, ptr %reg_8
	%reg_58 = load i32, ptr %reg_11
	%reg_59 = icmp slt i32 %reg_57, %reg_58
	br i1 %reg_59, label %Block17, label %Block18
Block16:  ;And opertor at line 69
	%reg_49 = load i32, ptr %reg_8
	%reg_50 = getelementptr i32, ptr %reg_0, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = load i32, ptr %reg_14
	%reg_53 = icmp slt i32 %reg_51, %reg_52
	br i1 %reg_53, label %Block14, label %Block15
Block17:  ;If then at line 74
	%reg_60 = load i32, ptr %reg_11
	%reg_61 = getelementptr i32, ptr %reg_0, i32 %reg_60
	%reg_62 = load i32, ptr %reg_8
	%reg_63 = getelementptr i32, ptr %reg_0, i32 %reg_62
	%reg_64 = load i32, ptr %reg_63
	store i32 %reg_64, ptr %reg_61
	%reg_65 = load i32, ptr %reg_11
	%reg_66 = add i32 1, 0
	%reg_67 = sub i32 %reg_65, %reg_66
	store i32 %reg_67, ptr %reg_11
	br label %Block18
Block18:  ;If end at line 74
	br label %Block4
}

define i32 @getMid(ptr %reg_0)
{
Block0:  ;Func define at line 92
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 92
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	%reg_3 = load i32, ptr @n
	%reg_4 = add i32 2, 0
	%reg_5 = srem i32 %reg_3, %reg_4
	%reg_6 = add i32 0, 0
	%reg_7 = icmp eq i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block4
Block2:  ;If then at line 94
	%reg_8 = load i32, ptr @n
	%reg_9 = add i32 2, 0
	%reg_10 = sdiv i32 %reg_8, %reg_9
	store i32 %reg_10, ptr %reg_1
	%reg_11 = load i32, ptr %reg_1
	%reg_12 = getelementptr i32, ptr %reg_0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = load i32, ptr %reg_1
	%reg_15 = add i32 1, 0
	%reg_16 = sub i32 %reg_14, %reg_15
	%reg_17 = getelementptr i32, ptr %reg_0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = add i32 %reg_13, %reg_18
	%reg_20 = add i32 2, 0
	%reg_21 = sdiv i32 %reg_19, %reg_20
	ret i32 %reg_21
	br label %Block3
Block3:  ;If end at line 94
	ret i32 0
Block4:  ;If else at line 94
	%reg_22 = load i32, ptr @n
	%reg_23 = add i32 2, 0
	%reg_24 = sdiv i32 %reg_22, %reg_23
	store i32 %reg_24, ptr %reg_1
	%reg_25 = load i32, ptr %reg_1
	%reg_26 = getelementptr i32, ptr %reg_0, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	ret i32 %reg_27
	br label %Block3
}

define i32 @getMost(ptr %reg_0)
{
Block0:  ;Func define at line 103
	%reg_23 = alloca i32
	%reg_17 = alloca i32
	%reg_15 = alloca i32
	%reg_2 = alloca i32
	%reg_1 = alloca [1000 x i32]
	br label %Block1
Block1:  ;Func body at line 103
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 4000, i1 1)
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 107
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = add i32 1000, 0
	%reg_7 = icmp slt i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 107
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = getelementptr [1000 x i32], ptr %reg_1, i32 0, i32 %reg_8
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = add i32 1, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_2
	br label %Block2
Block4:  ;While end at line 107
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_2
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_17
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_15
	br label %Block5
Block5:  ;While condition at line 115
	%reg_20 = load i32, ptr %reg_2
	%reg_21 = load i32, ptr @n
	%reg_22 = icmp slt i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block6, label %Block7
Block6:  ;While body at line 115
	%reg_24 = add i32 0, 0
	store i32 %reg_24, ptr %reg_23
	%reg_25 = load i32, ptr %reg_2
	%reg_26 = getelementptr i32, ptr %reg_0, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	store i32 %reg_27, ptr %reg_23
	%reg_28 = load i32, ptr %reg_23
	%reg_29 = getelementptr [1000 x i32], ptr %reg_1, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_23
	%reg_31 = getelementptr [1000 x i32], ptr %reg_1, i32 0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	%reg_33 = add i32 1, 0
	%reg_34 = add i32 %reg_32, %reg_33
	store i32 %reg_34, ptr %reg_29
	%reg_35 = load i32, ptr %reg_23
	%reg_36 = getelementptr [1000 x i32], ptr %reg_1, i32 0, i32 %reg_35
	%reg_37 = load i32, ptr %reg_36
	%reg_38 = load i32, ptr %reg_15
	%reg_39 = icmp sgt i32 %reg_37, %reg_38
	br i1 %reg_39, label %Block8, label %Block9
Block7:  ;While end at line 115
	%reg_47 = load i32, ptr %reg_17
	ret i32 %reg_47
Block8:  ;If then at line 119
	%reg_40 = load i32, ptr %reg_23
	%reg_41 = getelementptr [1000 x i32], ptr %reg_1, i32 0, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	store i32 %reg_42, ptr %reg_15
	%reg_43 = load i32, ptr %reg_23
	store i32 %reg_43, ptr %reg_17
	br label %Block9
Block9:  ;If end at line 119
	%reg_44 = load i32, ptr %reg_2
	%reg_45 = add i32 1, 0
	%reg_46 = add i32 %reg_44, %reg_45
	store i32 %reg_46, ptr %reg_2
	br label %Block5
}

define i32 @revert(ptr %reg_0)
{
Block0:  ;Func define at line 128
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 128
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_3
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_5
	br label %Block2
Block2:  ;While condition at line 134
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = load i32, ptr %reg_5
	%reg_11 = icmp slt i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block3, label %Block4
Block3:  ;While body at line 134
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = getelementptr i32, ptr %reg_0, i32 %reg_12
	%reg_14 = load i32, ptr %reg_13
	store i32 %reg_14, ptr %reg_1
	%reg_15 = load i32, ptr %reg_3
	%reg_16 = getelementptr i32, ptr %reg_0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_5
	%reg_18 = getelementptr i32, ptr %reg_0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	store i32 %reg_19, ptr %reg_16
	%reg_20 = load i32, ptr %reg_5
	%reg_21 = getelementptr i32, ptr %reg_0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_1
	store i32 %reg_22, ptr %reg_21
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = add i32 1, 0
	%reg_25 = add i32 %reg_23, %reg_24
	store i32 %reg_25, ptr %reg_3
	%reg_26 = load i32, ptr %reg_5
	%reg_27 = add i32 1, 0
	%reg_28 = sub i32 %reg_26, %reg_27
	store i32 %reg_28, ptr %reg_5
	br label %Block2
Block4:  ;While end at line 134
	%reg_29 = add i32 0, 0
	ret i32 %reg_29
}

define i32 @arrCopy(ptr %reg_0, ptr %reg_1)
{
Block0:  ;Func define at line 144
	%reg_2 = alloca i32
	br label %Block1
Block1:  ;Func body at line 144
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 147
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = load i32, ptr @n
	%reg_7 = icmp slt i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 147
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = getelementptr i32, ptr %reg_1, i32 %reg_8
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = getelementptr i32, ptr %reg_0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	store i32 %reg_12, ptr %reg_9
	%reg_13 = load i32, ptr %reg_2
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_2
	br label %Block2
Block4:  ;While end at line 147
	%reg_16 = add i32 0, 0
	ret i32 %reg_16
}

define i32 @calSum(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 154
	%reg_6 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 154
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_3
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_6
	br label %Block2
Block2:  ;While condition at line 159
	%reg_9 = load i32, ptr %reg_6
	%reg_10 = load i32, ptr @n
	%reg_11 = icmp slt i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block3, label %Block4
Block3:  ;While body at line 159
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = load i32, ptr %reg_6
	%reg_14 = getelementptr i32, ptr %reg_0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	%reg_16 = add i32 %reg_12, %reg_15
	store i32 %reg_16, ptr %reg_3
	%reg_17 = load i32, ptr %reg_6
	%reg_18 = load i32, ptr %reg_2
	%reg_19 = srem i32 %reg_17, %reg_18
	%reg_20 = load i32, ptr %reg_2
	%reg_21 = add i32 1, 0
	%reg_22 = sub i32 %reg_20, %reg_21
	%reg_23 = icmp ne i32 %reg_19, %reg_22
	br i1 %reg_23, label %Block5, label %Block7
Block4:  ;While end at line 159
	%reg_34 = add i32 0, 0
	ret i32 %reg_34
Block5:  ;If then at line 161
	%reg_24 = load i32, ptr %reg_6
	%reg_25 = getelementptr i32, ptr %reg_0, i32 %reg_24
	%reg_26 = add i32 0, 0
	store i32 %reg_26, ptr %reg_25
	br label %Block6
Block6:  ;If end at line 161
	%reg_31 = load i32, ptr %reg_6
	%reg_32 = add i32 1, 0
	%reg_33 = add i32 %reg_31, %reg_32
	store i32 %reg_33, ptr %reg_6
	br label %Block2
Block7:  ;If else at line 161
	%reg_27 = load i32, ptr %reg_6
	%reg_28 = getelementptr i32, ptr %reg_0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_3
	store i32 %reg_29, ptr %reg_28
	%reg_30 = add i32 0, 0
	store i32 %reg_30, ptr %reg_3
	br label %Block6
}

define i32 @avgPooling(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 172
	%reg_9 = alloca i32
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 172
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_5
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_3
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block2
Block2:  ;While condition at line 178
	%reg_11 = load i32, ptr %reg_5
	%reg_12 = load i32, ptr @n
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block3, label %Block4
Block3:  ;While body at line 178
	%reg_14 = load i32, ptr %reg_5
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = add i32 1, 0
	%reg_17 = sub i32 %reg_15, %reg_16
	%reg_18 = icmp slt i32 %reg_14, %reg_17
	br i1 %reg_18, label %Block5, label %Block7
Block4:  ;While end at line 178
	%reg_63 = load i32, ptr @n
	%reg_64 = load i32, ptr %reg_2
	%reg_65 = sub i32 %reg_63, %reg_64
	%reg_66 = add i32 1, 0
	%reg_67 = add i32 %reg_65, %reg_66
	store i32 %reg_67, ptr %reg_5
	br label %Block11
Block5:  ;If then at line 179
	%reg_19 = load i32, ptr %reg_3
	%reg_20 = load i32, ptr %reg_5
	%reg_21 = getelementptr i32, ptr %reg_0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	%reg_23 = add i32 %reg_19, %reg_22
	store i32 %reg_23, ptr %reg_3
	br label %Block6
Block6:  ;If end at line 179
	%reg_60 = load i32, ptr %reg_5
	%reg_61 = add i32 1, 0
	%reg_62 = add i32 %reg_60, %reg_61
	store i32 %reg_62, ptr %reg_5
	br label %Block2
Block7:  ;If else at line 179
	%reg_24 = load i32, ptr %reg_5
	%reg_25 = load i32, ptr %reg_2
	%reg_26 = add i32 1, 0
	%reg_27 = sub i32 %reg_25, %reg_26
	%reg_28 = icmp eq i32 %reg_24, %reg_27
	br i1 %reg_28, label %Block8, label %Block10
Block8:  ;If then at line 181
	%reg_29 = add i32 0, 0
	%reg_30 = getelementptr i32, ptr %reg_0, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	store i32 %reg_31, ptr %reg_9
	%reg_32 = add i32 0, 0
	%reg_33 = getelementptr i32, ptr %reg_0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_3
	%reg_35 = load i32, ptr %reg_2
	%reg_36 = sdiv i32 %reg_34, %reg_35
	store i32 %reg_36, ptr %reg_33
	br label %Block9
Block9:  ;If end at line 181
	br label %Block6
Block10:  ;If else at line 181
	%reg_37 = load i32, ptr %reg_3
	%reg_38 = load i32, ptr %reg_5
	%reg_39 = getelementptr i32, ptr %reg_0, i32 %reg_38
	%reg_40 = load i32, ptr %reg_39
	%reg_41 = add i32 %reg_37, %reg_40
	%reg_42 = load i32, ptr %reg_9
	%reg_43 = sub i32 %reg_41, %reg_42
	store i32 %reg_43, ptr %reg_3
	%reg_44 = load i32, ptr %reg_5
	%reg_45 = load i32, ptr %reg_2
	%reg_46 = sub i32 %reg_44, %reg_45
	%reg_47 = add i32 1, 0
	%reg_48 = add i32 %reg_46, %reg_47
	%reg_49 = getelementptr i32, ptr %reg_0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	store i32 %reg_50, ptr %reg_9
	%reg_51 = load i32, ptr %reg_5
	%reg_52 = load i32, ptr %reg_2
	%reg_53 = sub i32 %reg_51, %reg_52
	%reg_54 = add i32 1, 0
	%reg_55 = add i32 %reg_53, %reg_54
	%reg_56 = getelementptr i32, ptr %reg_0, i32 %reg_55
	%reg_57 = load i32, ptr %reg_3
	%reg_58 = load i32, ptr %reg_2
	%reg_59 = sdiv i32 %reg_57, %reg_58
	store i32 %reg_59, ptr %reg_56
	br label %Block9
Block11:  ;While condition at line 192
	%reg_68 = load i32, ptr %reg_5
	%reg_69 = load i32, ptr @n
	%reg_70 = icmp slt i32 %reg_68, %reg_69
	br i1 %reg_70, label %Block12, label %Block13
Block12:  ;While body at line 192
	%reg_71 = load i32, ptr %reg_5
	%reg_72 = getelementptr i32, ptr %reg_0, i32 %reg_71
	%reg_73 = add i32 0, 0
	store i32 %reg_73, ptr %reg_72
	%reg_74 = load i32, ptr %reg_5
	%reg_75 = add i32 1, 0
	%reg_76 = add i32 %reg_74, %reg_75
	store i32 %reg_76, ptr %reg_5
	br label %Block11
Block13:  ;While end at line 192
	%reg_77 = add i32 0, 0
	ret i32 %reg_77
}

define i32 @main()
{
Block0:  ;Func define at line 199
	%reg_106 = alloca i32
	%reg_99 = alloca i32
	%reg_2 = alloca [32 x i32]
	%reg_1 = alloca [32 x i32]
	br label %Block1
Block1:  ;Func body at line 199
	%reg_0 = add i32 32, 0
	store i32 %reg_0, ptr @n
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 128, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 128, i1 1)
	%reg_3 = add i32 0, 0
	%reg_4 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_3
	%reg_5 = add i32 7, 0
	store i32 %reg_5, ptr %reg_4
	%reg_6 = add i32 1, 0
	%reg_7 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_6
	%reg_8 = add i32 23, 0
	store i32 %reg_8, ptr %reg_7
	%reg_9 = add i32 2, 0
	%reg_10 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_9
	%reg_11 = add i32 89, 0
	store i32 %reg_11, ptr %reg_10
	%reg_12 = add i32 3, 0
	%reg_13 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_12
	%reg_14 = add i32 26, 0
	store i32 %reg_14, ptr %reg_13
	%reg_15 = add i32 4, 0
	%reg_16 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_15
	%reg_17 = add i32 282, 0
	store i32 %reg_17, ptr %reg_16
	%reg_18 = add i32 5, 0
	%reg_19 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_18
	%reg_20 = add i32 254, 0
	store i32 %reg_20, ptr %reg_19
	%reg_21 = add i32 6, 0
	%reg_22 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_21
	%reg_23 = add i32 27, 0
	store i32 %reg_23, ptr %reg_22
	%reg_24 = add i32 7, 0
	%reg_25 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_24
	%reg_26 = add i32 5, 0
	store i32 %reg_26, ptr %reg_25
	%reg_27 = add i32 8, 0
	%reg_28 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_27
	%reg_29 = add i32 83, 0
	store i32 %reg_29, ptr %reg_28
	%reg_30 = add i32 9, 0
	%reg_31 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_30
	%reg_32 = add i32 273, 0
	store i32 %reg_32, ptr %reg_31
	%reg_33 = add i32 10, 0
	%reg_34 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_33
	%reg_35 = add i32 574, 0
	store i32 %reg_35, ptr %reg_34
	%reg_36 = add i32 11, 0
	%reg_37 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_36
	%reg_38 = add i32 905, 0
	store i32 %reg_38, ptr %reg_37
	%reg_39 = add i32 12, 0
	%reg_40 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_39
	%reg_41 = add i32 354, 0
	store i32 %reg_41, ptr %reg_40
	%reg_42 = add i32 13, 0
	%reg_43 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_42
	%reg_44 = add i32 657, 0
	store i32 %reg_44, ptr %reg_43
	%reg_45 = add i32 14, 0
	%reg_46 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_45
	%reg_47 = add i32 935, 0
	store i32 %reg_47, ptr %reg_46
	%reg_48 = add i32 15, 0
	%reg_49 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_48
	%reg_50 = add i32 264, 0
	store i32 %reg_50, ptr %reg_49
	%reg_51 = add i32 16, 0
	%reg_52 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_51
	%reg_53 = add i32 639, 0
	store i32 %reg_53, ptr %reg_52
	%reg_54 = add i32 17, 0
	%reg_55 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_54
	%reg_56 = add i32 459, 0
	store i32 %reg_56, ptr %reg_55
	%reg_57 = add i32 18, 0
	%reg_58 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_57
	%reg_59 = add i32 29, 0
	store i32 %reg_59, ptr %reg_58
	%reg_60 = add i32 19, 0
	%reg_61 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_60
	%reg_62 = add i32 68, 0
	store i32 %reg_62, ptr %reg_61
	%reg_63 = add i32 20, 0
	%reg_64 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_63
	%reg_65 = add i32 929, 0
	store i32 %reg_65, ptr %reg_64
	%reg_66 = add i32 21, 0
	%reg_67 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_66
	%reg_68 = add i32 756, 0
	store i32 %reg_68, ptr %reg_67
	%reg_69 = add i32 22, 0
	%reg_70 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_69
	%reg_71 = add i32 452, 0
	store i32 %reg_71, ptr %reg_70
	%reg_72 = add i32 23, 0
	%reg_73 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_72
	%reg_74 = add i32 279, 0
	store i32 %reg_74, ptr %reg_73
	%reg_75 = add i32 24, 0
	%reg_76 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_75
	%reg_77 = add i32 58, 0
	store i32 %reg_77, ptr %reg_76
	%reg_78 = add i32 25, 0
	%reg_79 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_78
	%reg_80 = add i32 87, 0
	store i32 %reg_80, ptr %reg_79
	%reg_81 = add i32 26, 0
	%reg_82 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_81
	%reg_83 = add i32 96, 0
	store i32 %reg_83, ptr %reg_82
	%reg_84 = add i32 27, 0
	%reg_85 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_84
	%reg_86 = add i32 36, 0
	store i32 %reg_86, ptr %reg_85
	%reg_87 = add i32 28, 0
	%reg_88 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_87
	%reg_89 = add i32 39, 0
	store i32 %reg_89, ptr %reg_88
	%reg_90 = add i32 29, 0
	%reg_91 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_90
	%reg_92 = add i32 28, 0
	store i32 %reg_92, ptr %reg_91
	%reg_93 = add i32 30, 0
	%reg_94 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_93
	%reg_95 = add i32 1, 0
	store i32 %reg_95, ptr %reg_94
	%reg_96 = add i32 31, 0
	%reg_97 = getelementptr [32 x i32], ptr %reg_1, i32 0, i32 %reg_96
	%reg_98 = add i32 290, 0
	store i32 %reg_98, ptr %reg_97
	%reg_100 = add i32 0, 0
	store i32 %reg_100, ptr %reg_99
	%reg_101 = getelementptr [32 x i32], ptr %reg_1, i32 0
	%reg_102 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_103 = call i32 @arrCopy(ptr %reg_101, ptr %reg_102)
	store i32 %reg_103, ptr %reg_99
	%reg_104 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_105 = call i32 @revert(ptr %reg_104)
	store i32 %reg_105, ptr %reg_99
	%reg_107 = add i32 0, 0
	store i32 %reg_107, ptr %reg_106
	%reg_108 = add i32 0, 0
	store i32 %reg_108, ptr %reg_106
	br label %Block2
Block2:  ;While condition at line 240
	%reg_109 = load i32, ptr %reg_106
	%reg_110 = add i32 32, 0
	%reg_111 = icmp slt i32 %reg_109, %reg_110
	br i1 %reg_111, label %Block3, label %Block4
Block3:  ;While body at line 240
	%reg_112 = load i32, ptr %reg_106
	%reg_113 = getelementptr [32 x i32], ptr %reg_2, i32 0, i32 %reg_112
	%reg_114 = load i32, ptr %reg_113
	store i32 %reg_114, ptr %reg_99
	%reg_115 = load i32, ptr %reg_99
	call void @putint(i32 %reg_115)
	%reg_116 = load i32, ptr %reg_106
	%reg_117 = add i32 1, 0
	%reg_118 = add i32 %reg_116, %reg_117
	store i32 %reg_118, ptr %reg_106
	br label %Block2
Block4:  ;While end at line 240
	%reg_119 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_120 = call i32 @bubblesort(ptr %reg_119)
	store i32 %reg_120, ptr %reg_99
	%reg_121 = add i32 0, 0
	store i32 %reg_121, ptr %reg_106
	br label %Block5
Block5:  ;While condition at line 247
	%reg_122 = load i32, ptr %reg_106
	%reg_123 = add i32 32, 0
	%reg_124 = icmp slt i32 %reg_122, %reg_123
	br i1 %reg_124, label %Block6, label %Block7
Block6:  ;While body at line 247
	%reg_125 = load i32, ptr %reg_106
	%reg_126 = getelementptr [32 x i32], ptr %reg_2, i32 0, i32 %reg_125
	%reg_127 = load i32, ptr %reg_126
	store i32 %reg_127, ptr %reg_99
	%reg_128 = load i32, ptr %reg_99
	call void @putint(i32 %reg_128)
	%reg_129 = load i32, ptr %reg_106
	%reg_130 = add i32 1, 0
	%reg_131 = add i32 %reg_129, %reg_130
	store i32 %reg_131, ptr %reg_106
	br label %Block5
Block7:  ;While end at line 247
	%reg_132 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_133 = call i32 @getMid(ptr %reg_132)
	store i32 %reg_133, ptr %reg_99
	%reg_134 = load i32, ptr %reg_99
	call void @putint(i32 %reg_134)
	%reg_135 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_136 = call i32 @getMost(ptr %reg_135)
	store i32 %reg_136, ptr %reg_99
	%reg_137 = load i32, ptr %reg_99
	call void @putint(i32 %reg_137)
	%reg_138 = getelementptr [32 x i32], ptr %reg_1, i32 0
	%reg_139 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_140 = call i32 @arrCopy(ptr %reg_138, ptr %reg_139)
	store i32 %reg_140, ptr %reg_99
	%reg_141 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_142 = call i32 @bubblesort(ptr %reg_141)
	store i32 %reg_142, ptr %reg_99
	%reg_143 = add i32 0, 0
	store i32 %reg_143, ptr %reg_106
	br label %Block8
Block8:  ;While condition at line 260
	%reg_144 = load i32, ptr %reg_106
	%reg_145 = add i32 32, 0
	%reg_146 = icmp slt i32 %reg_144, %reg_145
	br i1 %reg_146, label %Block9, label %Block10
Block9:  ;While body at line 260
	%reg_147 = load i32, ptr %reg_106
	%reg_148 = getelementptr [32 x i32], ptr %reg_2, i32 0, i32 %reg_147
	%reg_149 = load i32, ptr %reg_148
	store i32 %reg_149, ptr %reg_99
	%reg_150 = load i32, ptr %reg_99
	call void @putint(i32 %reg_150)
	%reg_151 = load i32, ptr %reg_106
	%reg_152 = add i32 1, 0
	%reg_153 = add i32 %reg_151, %reg_152
	store i32 %reg_153, ptr %reg_106
	br label %Block8
Block10:  ;While end at line 260
	%reg_154 = getelementptr [32 x i32], ptr %reg_1, i32 0
	%reg_155 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_156 = call i32 @arrCopy(ptr %reg_154, ptr %reg_155)
	store i32 %reg_156, ptr %reg_99
	%reg_157 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_158 = call i32 @insertsort(ptr %reg_157)
	store i32 %reg_158, ptr %reg_99
	%reg_159 = add i32 0, 0
	store i32 %reg_159, ptr %reg_106
	br label %Block11
Block11:  ;While condition at line 269
	%reg_160 = load i32, ptr %reg_106
	%reg_161 = add i32 32, 0
	%reg_162 = icmp slt i32 %reg_160, %reg_161
	br i1 %reg_162, label %Block12, label %Block13
Block12:  ;While body at line 269
	%reg_163 = load i32, ptr %reg_106
	%reg_164 = getelementptr [32 x i32], ptr %reg_2, i32 0, i32 %reg_163
	%reg_165 = load i32, ptr %reg_164
	store i32 %reg_165, ptr %reg_99
	%reg_166 = load i32, ptr %reg_99
	call void @putint(i32 %reg_166)
	%reg_167 = load i32, ptr %reg_106
	%reg_168 = add i32 1, 0
	%reg_169 = add i32 %reg_167, %reg_168
	store i32 %reg_169, ptr %reg_106
	br label %Block11
Block13:  ;While end at line 269
	%reg_170 = getelementptr [32 x i32], ptr %reg_1, i32 0
	%reg_171 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_172 = call i32 @arrCopy(ptr %reg_170, ptr %reg_171)
	store i32 %reg_172, ptr %reg_99
	%reg_173 = add i32 0, 0
	store i32 %reg_173, ptr %reg_106
	%reg_174 = add i32 31, 0
	store i32 %reg_174, ptr %reg_99
	%reg_175 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_176 = load i32, ptr %reg_106
	%reg_177 = load i32, ptr %reg_99
	%reg_178 = call i32 @QuickSort(ptr %reg_175, i32 %reg_176, i32 %reg_177)
	store i32 %reg_178, ptr %reg_99
	br label %Block14
Block14:  ;While condition at line 279
	%reg_179 = load i32, ptr %reg_106
	%reg_180 = add i32 32, 0
	%reg_181 = icmp slt i32 %reg_179, %reg_180
	br i1 %reg_181, label %Block15, label %Block16
Block15:  ;While body at line 279
	%reg_182 = load i32, ptr %reg_106
	%reg_183 = getelementptr [32 x i32], ptr %reg_2, i32 0, i32 %reg_182
	%reg_184 = load i32, ptr %reg_183
	store i32 %reg_184, ptr %reg_99
	%reg_185 = load i32, ptr %reg_99
	call void @putint(i32 %reg_185)
	%reg_186 = load i32, ptr %reg_106
	%reg_187 = add i32 1, 0
	%reg_188 = add i32 %reg_186, %reg_187
	store i32 %reg_188, ptr %reg_106
	br label %Block14
Block16:  ;While end at line 279
	%reg_189 = getelementptr [32 x i32], ptr %reg_1, i32 0
	%reg_190 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_191 = call i32 @arrCopy(ptr %reg_189, ptr %reg_190)
	store i32 %reg_191, ptr %reg_99
	%reg_192 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_193 = add i32 4, 0
	%reg_194 = call i32 @calSum(ptr %reg_192, i32 %reg_193)
	store i32 %reg_194, ptr %reg_99
	%reg_195 = add i32 0, 0
	store i32 %reg_195, ptr %reg_106
	br label %Block17
Block17:  ;While condition at line 288
	%reg_196 = load i32, ptr %reg_106
	%reg_197 = add i32 32, 0
	%reg_198 = icmp slt i32 %reg_196, %reg_197
	br i1 %reg_198, label %Block18, label %Block19
Block18:  ;While body at line 288
	%reg_199 = load i32, ptr %reg_106
	%reg_200 = getelementptr [32 x i32], ptr %reg_2, i32 0, i32 %reg_199
	%reg_201 = load i32, ptr %reg_200
	store i32 %reg_201, ptr %reg_99
	%reg_202 = load i32, ptr %reg_99
	call void @putint(i32 %reg_202)
	%reg_203 = load i32, ptr %reg_106
	%reg_204 = add i32 1, 0
	%reg_205 = add i32 %reg_203, %reg_204
	store i32 %reg_205, ptr %reg_106
	br label %Block17
Block19:  ;While end at line 288
	%reg_206 = getelementptr [32 x i32], ptr %reg_1, i32 0
	%reg_207 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_208 = call i32 @arrCopy(ptr %reg_206, ptr %reg_207)
	store i32 %reg_208, ptr %reg_99
	%reg_209 = getelementptr [32 x i32], ptr %reg_2, i32 0
	%reg_210 = add i32 3, 0
	%reg_211 = call i32 @avgPooling(ptr %reg_209, i32 %reg_210)
	store i32 %reg_211, ptr %reg_99
	%reg_212 = add i32 0, 0
	store i32 %reg_212, ptr %reg_106
	br label %Block20
Block20:  ;While condition at line 297
	%reg_213 = load i32, ptr %reg_106
	%reg_214 = add i32 32, 0
	%reg_215 = icmp slt i32 %reg_213, %reg_214
	br i1 %reg_215, label %Block21, label %Block22
Block21:  ;While body at line 297
	%reg_216 = load i32, ptr %reg_106
	%reg_217 = getelementptr [32 x i32], ptr %reg_2, i32 0, i32 %reg_216
	%reg_218 = load i32, ptr %reg_217
	store i32 %reg_218, ptr %reg_99
	%reg_219 = load i32, ptr %reg_99
	call void @putint(i32 %reg_219)
	%reg_220 = load i32, ptr %reg_106
	%reg_221 = add i32 1, 0
	%reg_222 = add i32 %reg_220, %reg_221
	store i32 %reg_222, ptr %reg_106
	br label %Block20
Block22:  ;While end at line 297
	%reg_223 = add i32 0, 0
	ret i32 %reg_223
}
