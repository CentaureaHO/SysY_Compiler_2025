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


define i32 @main()
{
Block0:  ;Func define at line 1
	%reg_47 = alloca [4 x [2 x [1 x i32]]]
	%reg_31 = alloca [4 x [2 x i32]]
	%reg_14 = alloca [4 x [2 x i32]]
	%reg_13 = alloca [4 x [2 x i32]]
	%reg_11 = alloca i32
	%reg_0 = alloca [4 x [2 x i32]]
	br label %Block1
Block1:  ;Func body at line 1
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 32, i1 1)
	%reg_1 = add i32 1, 0
	%reg_2 = getelementptr [4 x [2 x i32]], ptr %reg_0, i32 0, i32 0, i32 0
	store i32 %reg_1, ptr %reg_2
	%reg_3 = add i32 2, 0
	%reg_4 = getelementptr [4 x [2 x i32]], ptr %reg_0, i32 0, i32 0, i32 1
	store i32 %reg_3, ptr %reg_4
	%reg_5 = add i32 3, 0
	%reg_6 = getelementptr [4 x [2 x i32]], ptr %reg_0, i32 0, i32 1, i32 0
	store i32 %reg_5, ptr %reg_6
	%reg_7 = add i32 4, 0
	%reg_8 = getelementptr [4 x [2 x i32]], ptr %reg_0, i32 0, i32 1, i32 1
	store i32 %reg_7, ptr %reg_8
	%reg_9 = add i32 7, 0
	%reg_10 = getelementptr [4 x [2 x i32]], ptr %reg_0, i32 0, i32 3, i32 0
	store i32 %reg_9, ptr %reg_10
	%reg_12 = add i32 3, 0
	store i32 %reg_12, ptr %reg_11
	call void @llvm.memset.p0.i32(ptr %reg_13, i8 0, i32 32, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_14, i8 0, i32 32, i1 1)
	%reg_15 = add i32 1, 0
	%reg_16 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 0, i32 0
	store i32 %reg_15, ptr %reg_16
	%reg_17 = add i32 2, 0
	%reg_18 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 0, i32 1
	store i32 %reg_17, ptr %reg_18
	%reg_19 = add i32 3, 0
	%reg_20 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 1, i32 0
	store i32 %reg_19, ptr %reg_20
	%reg_21 = add i32 4, 0
	%reg_22 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 1, i32 1
	store i32 %reg_21, ptr %reg_22
	%reg_23 = add i32 5, 0
	%reg_24 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 2, i32 0
	store i32 %reg_23, ptr %reg_24
	%reg_25 = add i32 6, 0
	%reg_26 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 2, i32 1
	store i32 %reg_25, ptr %reg_26
	%reg_27 = add i32 7, 0
	%reg_28 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 3, i32 0
	store i32 %reg_27, ptr %reg_28
	%reg_29 = add i32 8, 0
	%reg_30 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 3, i32 1
	store i32 %reg_29, ptr %reg_30
	call void @llvm.memset.p0.i32(ptr %reg_31, i8 0, i32 32, i1 1)
	%reg_32 = add i32 1, 0
	%reg_33 = getelementptr [4 x [2 x i32]], ptr %reg_31, i32 0, i32 0, i32 0
	store i32 %reg_32, ptr %reg_33
	%reg_34 = add i32 2, 0
	%reg_35 = getelementptr [4 x [2 x i32]], ptr %reg_31, i32 0, i32 0, i32 1
	store i32 %reg_34, ptr %reg_35
	%reg_36 = add i32 3, 0
	%reg_37 = getelementptr [4 x [2 x i32]], ptr %reg_31, i32 0, i32 1, i32 0
	store i32 %reg_36, ptr %reg_37
	%reg_38 = add i32 5, 0
	%reg_39 = getelementptr [4 x [2 x i32]], ptr %reg_31, i32 0, i32 2, i32 0
	store i32 %reg_38, ptr %reg_39
	%reg_40 = add i32 3, 0
	%reg_41 = add i32 0, 0
	%reg_42 = getelementptr [4 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_40, i32 %reg_41
	%reg_43 = load i32, ptr %reg_42
	%reg_44 = getelementptr [4 x [2 x i32]], ptr %reg_31, i32 0, i32 3, i32 0
	store i32 %reg_43, ptr %reg_44
	%reg_45 = add i32 8, 0
	%reg_46 = getelementptr [4 x [2 x i32]], ptr %reg_31, i32 0, i32 3, i32 1
	store i32 %reg_45, ptr %reg_46
	call void @llvm.memset.p0.i32(ptr %reg_47, i8 0, i32 32, i1 1)
	%reg_48 = add i32 2, 0
	%reg_49 = add i32 1, 0
	%reg_50 = getelementptr [4 x [2 x i32]], ptr %reg_31, i32 0, i32 %reg_48, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 0, i32 0, i32 0
	store i32 %reg_51, ptr %reg_52
	%reg_53 = add i32 2, 0
	%reg_54 = add i32 1, 0
	%reg_55 = getelementptr [4 x [2 x i32]], ptr %reg_14, i32 0, i32 %reg_53, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	%reg_57 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 0, i32 1, i32 0
	store i32 %reg_56, ptr %reg_57
	%reg_58 = add i32 3, 0
	%reg_59 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 1, i32 0, i32 0
	store i32 %reg_58, ptr %reg_59
	%reg_60 = add i32 4, 0
	%reg_61 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 1, i32 1, i32 0
	store i32 %reg_60, ptr %reg_61
	%reg_62 = add i32 5, 0
	%reg_63 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 2, i32 0, i32 0
	store i32 %reg_62, ptr %reg_63
	%reg_64 = add i32 6, 0
	%reg_65 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 2, i32 1, i32 0
	store i32 %reg_64, ptr %reg_65
	%reg_66 = add i32 7, 0
	%reg_67 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 3, i32 0, i32 0
	store i32 %reg_66, ptr %reg_67
	%reg_68 = add i32 8, 0
	%reg_69 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 3, i32 1, i32 0
	store i32 %reg_68, ptr %reg_69
	%reg_70 = add i32 3, 0
	%reg_71 = add i32 1, 0
	%reg_72 = add i32 0, 0
	%reg_73 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 %reg_70, i32 %reg_71, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	%reg_75 = add i32 0, 0
	%reg_76 = add i32 0, 0
	%reg_77 = add i32 0, 0
	%reg_78 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 %reg_75, i32 %reg_76, i32 %reg_77
	%reg_79 = load i32, ptr %reg_78
	%reg_80 = add i32 %reg_74, %reg_79
	%reg_81 = add i32 0, 0
	%reg_82 = add i32 1, 0
	%reg_83 = add i32 0, 0
	%reg_84 = getelementptr [4 x [2 x [1 x i32]]], ptr %reg_47, i32 0, i32 %reg_81, i32 %reg_82, i32 %reg_83
	%reg_85 = load i32, ptr %reg_84
	%reg_86 = add i32 %reg_80, %reg_85
	%reg_87 = add i32 3, 0
	%reg_88 = add i32 0, 0
	%reg_89 = getelementptr [4 x [2 x i32]], ptr %reg_31, i32 0, i32 %reg_87, i32 %reg_88
	%reg_90 = load i32, ptr %reg_89
	%reg_91 = add i32 %reg_86, %reg_90
	ret i32 %reg_91
}
