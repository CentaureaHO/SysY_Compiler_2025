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

define i32 @counting_sort(ptr %reg_0, ptr %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 3
	%reg_9 = alloca i32
	%reg_7 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca [10 x i32]
	%reg_3 = alloca i32
	store i32 %reg_2, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 3
	call void @llvm.memset.p0.i32(ptr %reg_4, i8 0, i32 40, i1 1)
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_9
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_5
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_7
	br label %Block2
Block2:  ;While condition at line 11
	%reg_14 = load i32, ptr %reg_9
	%reg_15 = add i32 10, 0
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block3, label %Block4
Block3:  ;While body at line 11
	%reg_17 = load i32, ptr %reg_9
	%reg_18 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_17
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = load i32, ptr %reg_9
	%reg_21 = add i32 1, 0
	%reg_22 = add i32 %reg_20, %reg_21
	store i32 %reg_22, ptr %reg_9
	br label %Block2
Block4:  ;While end at line 11
	br label %Block5
Block5:  ;While condition at line 15
	%reg_23 = load i32, ptr %reg_5
	%reg_24 = load i32, ptr %reg_3
	%reg_25 = icmp slt i32 %reg_23, %reg_24
	br i1 %reg_25, label %Block6, label %Block7
Block6:  ;While body at line 15
	%reg_26 = load i32, ptr %reg_5
	%reg_27 = getelementptr i32, ptr %reg_0, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	%reg_29 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_5
	%reg_31 = getelementptr i32, ptr %reg_0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	%reg_33 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = add i32 1, 0
	%reg_36 = add i32 %reg_34, %reg_35
	store i32 %reg_36, ptr %reg_29
	%reg_37 = load i32, ptr %reg_5
	%reg_38 = add i32 1, 0
	%reg_39 = add i32 %reg_37, %reg_38
	store i32 %reg_39, ptr %reg_5
	br label %Block5
Block7:  ;While end at line 15
	%reg_40 = add i32 1, 0
	store i32 %reg_40, ptr %reg_9
	br label %Block8
Block8:  ;While condition at line 21
	%reg_41 = load i32, ptr %reg_9
	%reg_42 = add i32 10, 0
	%reg_43 = icmp slt i32 %reg_41, %reg_42
	br i1 %reg_43, label %Block9, label %Block10
Block9:  ;While body at line 21
	%reg_44 = load i32, ptr %reg_9
	%reg_45 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_44
	%reg_46 = load i32, ptr %reg_9
	%reg_47 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_46
	%reg_48 = load i32, ptr %reg_47
	%reg_49 = load i32, ptr %reg_9
	%reg_50 = add i32 1, 0
	%reg_51 = sub i32 %reg_49, %reg_50
	%reg_52 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_51
	%reg_53 = load i32, ptr %reg_52
	%reg_54 = add i32 %reg_48, %reg_53
	store i32 %reg_54, ptr %reg_45
	%reg_55 = load i32, ptr %reg_9
	%reg_56 = add i32 1, 0
	%reg_57 = add i32 %reg_55, %reg_56
	store i32 %reg_57, ptr %reg_9
	br label %Block8
Block10:  ;While end at line 21
	%reg_58 = load i32, ptr %reg_3
	store i32 %reg_58, ptr %reg_7
	br label %Block11
Block11:  ;While condition at line 26
	%reg_59 = load i32, ptr %reg_7
	%reg_60 = add i32 0, 0
	%reg_61 = icmp sgt i32 %reg_59, %reg_60
	br i1 %reg_61, label %Block12, label %Block13
Block12:  ;While body at line 26
	%reg_62 = load i32, ptr %reg_7
	%reg_63 = add i32 1, 0
	%reg_64 = sub i32 %reg_62, %reg_63
	%reg_65 = getelementptr i32, ptr %reg_0, i32 %reg_64
	%reg_66 = load i32, ptr %reg_65
	%reg_67 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_7
	%reg_69 = add i32 1, 0
	%reg_70 = sub i32 %reg_68, %reg_69
	%reg_71 = getelementptr i32, ptr %reg_0, i32 %reg_70
	%reg_72 = load i32, ptr %reg_71
	%reg_73 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	%reg_75 = add i32 1, 0
	%reg_76 = sub i32 %reg_74, %reg_75
	store i32 %reg_76, ptr %reg_67
	%reg_77 = load i32, ptr %reg_7
	%reg_78 = add i32 1, 0
	%reg_79 = sub i32 %reg_77, %reg_78
	%reg_80 = getelementptr i32, ptr %reg_0, i32 %reg_79
	%reg_81 = load i32, ptr %reg_80
	%reg_82 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_81
	%reg_83 = load i32, ptr %reg_82
	%reg_84 = getelementptr i32, ptr %reg_1, i32 %reg_83
	%reg_85 = load i32, ptr %reg_7
	%reg_86 = add i32 1, 0
	%reg_87 = sub i32 %reg_85, %reg_86
	%reg_88 = getelementptr i32, ptr %reg_0, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	store i32 %reg_89, ptr %reg_84
	%reg_90 = load i32, ptr %reg_7
	%reg_91 = add i32 1, 0
	%reg_92 = sub i32 %reg_90, %reg_91
	store i32 %reg_92, ptr %reg_7
	br label %Block11
Block13:  ;While end at line 26
	%reg_93 = add i32 0, 0
	ret i32 %reg_93
}

define i32 @main()
{
Block0:  ;Func define at line 35
	%reg_43 = alloca i32
	%reg_35 = alloca [10 x i32]
	%reg_32 = alloca i32
	%reg_1 = alloca [10 x i32]
	br label %Block1
Block1:  ;Func body at line 35
	%reg_0 = add i32 10, 0
	store i32 %reg_0, ptr @n
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 40, i1 1)
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_2
	%reg_4 = add i32 4, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = add i32 1, 0
	%reg_6 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_5
	%reg_7 = add i32 3, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = add i32 2, 0
	%reg_9 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_8
	%reg_10 = add i32 9, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 3, 0
	%reg_12 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_11
	%reg_13 = add i32 2, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = add i32 4, 0
	%reg_15 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_14
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	%reg_17 = add i32 5, 0
	%reg_18 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_17
	%reg_19 = add i32 1, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = add i32 6, 0
	%reg_21 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_20
	%reg_22 = add i32 6, 0
	store i32 %reg_22, ptr %reg_21
	%reg_23 = add i32 7, 0
	%reg_24 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_23
	%reg_25 = add i32 5, 0
	store i32 %reg_25, ptr %reg_24
	%reg_26 = add i32 8, 0
	%reg_27 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_26
	%reg_28 = add i32 7, 0
	store i32 %reg_28, ptr %reg_27
	%reg_29 = add i32 9, 0
	%reg_30 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_29
	%reg_31 = add i32 8, 0
	store i32 %reg_31, ptr %reg_30
	%reg_33 = add i32 0, 0
	store i32 %reg_33, ptr %reg_32
	%reg_34 = add i32 0, 0
	store i32 %reg_34, ptr %reg_32
	call void @llvm.memset.p0.i32(ptr %reg_35, i8 0, i32 40, i1 1)
	%reg_36 = getelementptr [10 x i32], ptr %reg_1, i32 0
	%reg_37 = getelementptr [10 x i32], ptr %reg_35, i32 0
	%reg_38 = load i32, ptr @n
	%reg_39 = call i32 @counting_sort(ptr %reg_36, ptr %reg_37, i32 %reg_38)
	store i32 %reg_39, ptr %reg_32
	br label %Block2
Block2:  ;While condition at line 44
	%reg_40 = load i32, ptr %reg_32
	%reg_41 = load i32, ptr @n
	%reg_42 = icmp slt i32 %reg_40, %reg_41
	br i1 %reg_42, label %Block3, label %Block4
Block3:  ;While body at line 44
	%reg_44 = add i32 0, 0
	store i32 %reg_44, ptr %reg_43
	%reg_45 = load i32, ptr %reg_32
	%reg_46 = getelementptr [10 x i32], ptr %reg_35, i32 0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	store i32 %reg_47, ptr %reg_43
	%reg_48 = load i32, ptr %reg_43
	call void @putint(i32 %reg_48)
	%reg_49 = add i32 10, 0
	store i32 %reg_49, ptr %reg_43
	%reg_50 = load i32, ptr %reg_43
	call void @putch(i32 %reg_50)
	%reg_51 = load i32, ptr %reg_32
	%reg_52 = add i32 1, 0
	%reg_53 = add i32 %reg_51, %reg_52
	store i32 %reg_53, ptr %reg_32
	br label %Block2
Block4:  ;While end at line 44
	%reg_54 = add i32 0, 0
	ret i32 %reg_54
}
