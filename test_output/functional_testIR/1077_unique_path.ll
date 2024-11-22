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


define i32 @uniquePaths(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 1
	%reg_14 = alloca i32
	%reg_12 = alloca i32
	%reg_11 = alloca [9 x i32]
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 1
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = add i32 1, 0
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 2
	%reg_10 = add i32 1, 0
	ret i32 %reg_10
	br label %Block3
Block3:  ;If end at line 2
	call void @llvm.memset.p0.i32(ptr %reg_11, i8 0, i32 36, i1 1)
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_14
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_12
	br label %Block5
Block4:  ;Or opertor at line 2
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = add i32 1, 0
	%reg_9 = icmp eq i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block2, label %Block3
Block5:  ;While condition at line 9
	%reg_17 = load i32, ptr %reg_12
	%reg_18 = load i32, ptr %reg_2
	%reg_19 = icmp slt i32 %reg_17, %reg_18
	br i1 %reg_19, label %Block6, label %Block7
Block6:  ;While body at line 9
	%reg_20 = load i32, ptr %reg_12
	%reg_21 = add i32 3, 0
	%reg_22 = mul i32 %reg_20, %reg_21
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = add i32 %reg_22, %reg_23
	%reg_25 = add i32 1, 0
	%reg_26 = sub i32 %reg_24, %reg_25
	%reg_27 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_26
	%reg_28 = add i32 1, 0
	store i32 %reg_28, ptr %reg_27
	%reg_29 = load i32, ptr %reg_12
	%reg_30 = add i32 1, 0
	%reg_31 = add i32 %reg_29, %reg_30
	store i32 %reg_31, ptr %reg_12
	br label %Block5
Block7:  ;While end at line 9
	%reg_32 = add i32 0, 0
	store i32 %reg_32, ptr %reg_12
	br label %Block8
Block8:  ;While condition at line 14
	%reg_33 = load i32, ptr %reg_12
	%reg_34 = load i32, ptr %reg_3
	%reg_35 = icmp slt i32 %reg_33, %reg_34
	br i1 %reg_35, label %Block9, label %Block10
Block9:  ;While body at line 14
	%reg_36 = load i32, ptr %reg_2
	%reg_37 = add i32 1, 0
	%reg_38 = sub i32 %reg_36, %reg_37
	%reg_39 = add i32 3, 0
	%reg_40 = mul i32 %reg_38, %reg_39
	%reg_41 = load i32, ptr %reg_12
	%reg_42 = add i32 %reg_40, %reg_41
	%reg_43 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_42
	%reg_44 = add i32 1, 0
	store i32 %reg_44, ptr %reg_43
	%reg_45 = load i32, ptr %reg_12
	%reg_46 = add i32 1, 0
	%reg_47 = add i32 %reg_45, %reg_46
	store i32 %reg_47, ptr %reg_12
	br label %Block8
Block10:  ;While end at line 14
	%reg_48 = load i32, ptr %reg_2
	%reg_49 = add i32 2, 0
	%reg_50 = sub i32 %reg_48, %reg_49
	store i32 %reg_50, ptr %reg_12
	br label %Block11
Block11:  ;While condition at line 19
	%reg_51 = load i32, ptr %reg_12
	%reg_52 = add i32 1, 0
	%reg_53 = sub i32 0, %reg_52
	%reg_54 = icmp sgt i32 %reg_51, %reg_53
	br i1 %reg_54, label %Block12, label %Block13
Block12:  ;While body at line 19
	%reg_55 = load i32, ptr %reg_3
	%reg_56 = add i32 2, 0
	%reg_57 = sub i32 %reg_55, %reg_56
	store i32 %reg_57, ptr %reg_14
	br label %Block14
Block13:  ;While end at line 19
	%reg_93 = add i32 0, 0
	%reg_94 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_93
	%reg_95 = load i32, ptr %reg_94
	ret i32 %reg_95
Block14:  ;While condition at line 21
	%reg_58 = load i32, ptr %reg_14
	%reg_59 = add i32 1, 0
	%reg_60 = sub i32 0, %reg_59
	%reg_61 = icmp sgt i32 %reg_58, %reg_60
	br i1 %reg_61, label %Block15, label %Block16
Block15:  ;While body at line 21
	%reg_62 = load i32, ptr %reg_12
	%reg_63 = add i32 3, 0
	%reg_64 = mul i32 %reg_62, %reg_63
	%reg_65 = load i32, ptr %reg_14
	%reg_66 = add i32 %reg_64, %reg_65
	%reg_67 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_12
	%reg_69 = add i32 1, 0
	%reg_70 = add i32 %reg_68, %reg_69
	%reg_71 = add i32 3, 0
	%reg_72 = mul i32 %reg_70, %reg_71
	%reg_73 = load i32, ptr %reg_14
	%reg_74 = add i32 %reg_72, %reg_73
	%reg_75 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_74
	%reg_76 = load i32, ptr %reg_75
	%reg_77 = load i32, ptr %reg_12
	%reg_78 = add i32 3, 0
	%reg_79 = mul i32 %reg_77, %reg_78
	%reg_80 = load i32, ptr %reg_14
	%reg_81 = add i32 %reg_79, %reg_80
	%reg_82 = add i32 1, 0
	%reg_83 = add i32 %reg_81, %reg_82
	%reg_84 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_83
	%reg_85 = load i32, ptr %reg_84
	%reg_86 = add i32 %reg_76, %reg_85
	store i32 %reg_86, ptr %reg_67
	%reg_87 = load i32, ptr %reg_14
	%reg_88 = add i32 1, 0
	%reg_89 = sub i32 %reg_87, %reg_88
	store i32 %reg_89, ptr %reg_14
	br label %Block14
Block16:  ;While end at line 21
	%reg_90 = load i32, ptr %reg_12
	%reg_91 = add i32 1, 0
	%reg_92 = sub i32 %reg_90, %reg_91
	store i32 %reg_92, ptr %reg_12
	br label %Block11
}

define i32 @main()
{
Block0:  ;Func define at line 29
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 29
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = add i32 3, 0
	store i32 %reg_4, ptr %reg_2
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = call i32 @uniquePaths(i32 %reg_5, i32 %reg_6)
	store i32 %reg_7, ptr %reg_0
	%reg_8 = load i32, ptr %reg_0
	ret i32 %reg_8
}
