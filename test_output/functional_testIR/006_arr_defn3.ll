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
Block0:  ;Func define at line 2
	%reg_48 = alloca [4 x [2 x i32]]
	%reg_35 = alloca [4 x [2 x i32]]
	%reg_18 = alloca [4 x [2 x i32]]
	%reg_1 = alloca [4 x [2 x i32]]
	%reg_0 = alloca [4 x [2 x i32]]
	br label %Block1
Block1:  ;Func body at line 2
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 32, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 32, i1 1)
	%reg_2 = add i32 1, 0
	%reg_3 = getelementptr [4 x [2 x i32]], ptr %reg_1, i32 0, i32 0, i32 0
	store i32 %reg_2, ptr %reg_3
	%reg_4 = add i32 2, 0
	%reg_5 = getelementptr [4 x [2 x i32]], ptr %reg_1, i32 0, i32 0, i32 1
	store i32 %reg_4, ptr %reg_5
	%reg_6 = add i32 3, 0
	%reg_7 = getelementptr [4 x [2 x i32]], ptr %reg_1, i32 0, i32 1, i32 0
	store i32 %reg_6, ptr %reg_7
	%reg_8 = add i32 4, 0
	%reg_9 = getelementptr [4 x [2 x i32]], ptr %reg_1, i32 0, i32 1, i32 1
	store i32 %reg_8, ptr %reg_9
	%reg_10 = add i32 5, 0
	%reg_11 = getelementptr [4 x [2 x i32]], ptr %reg_1, i32 0, i32 2, i32 0
	store i32 %reg_10, ptr %reg_11
	%reg_12 = add i32 6, 0
	%reg_13 = getelementptr [4 x [2 x i32]], ptr %reg_1, i32 0, i32 2, i32 1
	store i32 %reg_12, ptr %reg_13
	%reg_14 = add i32 7, 0
	%reg_15 = getelementptr [4 x [2 x i32]], ptr %reg_1, i32 0, i32 3, i32 0
	store i32 %reg_14, ptr %reg_15
	%reg_16 = add i32 8, 0
	%reg_17 = getelementptr [4 x [2 x i32]], ptr %reg_1, i32 0, i32 3, i32 1
	store i32 %reg_16, ptr %reg_17
	call void @llvm.memset.p0.i32(ptr %reg_18, i8 0, i32 32, i1 1)
	%reg_19 = add i32 1, 0
	%reg_20 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 0, i32 0
	store i32 %reg_19, ptr %reg_20
	%reg_21 = add i32 2, 0
	%reg_22 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 0, i32 1
	store i32 %reg_21, ptr %reg_22
	%reg_23 = add i32 3, 0
	%reg_24 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 1, i32 0
	store i32 %reg_23, ptr %reg_24
	%reg_25 = add i32 4, 0
	%reg_26 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 1, i32 1
	store i32 %reg_25, ptr %reg_26
	%reg_27 = add i32 5, 0
	%reg_28 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 2, i32 0
	store i32 %reg_27, ptr %reg_28
	%reg_29 = add i32 6, 0
	%reg_30 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 2, i32 1
	store i32 %reg_29, ptr %reg_30
	%reg_31 = add i32 7, 0
	%reg_32 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 3, i32 0
	store i32 %reg_31, ptr %reg_32
	%reg_33 = add i32 8, 0
	%reg_34 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 3, i32 1
	store i32 %reg_33, ptr %reg_34
	call void @llvm.memset.p0.i32(ptr %reg_35, i8 0, i32 32, i1 1)
	%reg_36 = add i32 1, 0
	%reg_37 = getelementptr [4 x [2 x i32]], ptr %reg_35, i32 0, i32 0, i32 0
	store i32 %reg_36, ptr %reg_37
	%reg_38 = add i32 2, 0
	%reg_39 = getelementptr [4 x [2 x i32]], ptr %reg_35, i32 0, i32 0, i32 1
	store i32 %reg_38, ptr %reg_39
	%reg_40 = add i32 3, 0
	%reg_41 = getelementptr [4 x [2 x i32]], ptr %reg_35, i32 0, i32 1, i32 0
	store i32 %reg_40, ptr %reg_41
	%reg_42 = add i32 5, 0
	%reg_43 = getelementptr [4 x [2 x i32]], ptr %reg_35, i32 0, i32 2, i32 0
	store i32 %reg_42, ptr %reg_43
	%reg_44 = add i32 7, 0
	%reg_45 = getelementptr [4 x [2 x i32]], ptr %reg_35, i32 0, i32 3, i32 0
	store i32 %reg_44, ptr %reg_45
	%reg_46 = add i32 8, 0
	%reg_47 = getelementptr [4 x [2 x i32]], ptr %reg_35, i32 0, i32 3, i32 1
	store i32 %reg_46, ptr %reg_47
	call void @llvm.memset.p0.i32(ptr %reg_48, i8 0, i32 32, i1 1)
	%reg_49 = add i32 2, 0
	%reg_50 = add i32 1, 0
	%reg_51 = getelementptr [4 x [2 x i32]], ptr %reg_35, i32 0, i32 %reg_49, i32 %reg_50
	%reg_52 = load i32, ptr %reg_51
	%reg_53 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 0, i32 0
	store i32 %reg_52, ptr %reg_53
	%reg_54 = add i32 2, 0
	%reg_55 = add i32 1, 0
	%reg_56 = getelementptr [4 x [2 x i32]], ptr %reg_18, i32 0, i32 %reg_54, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 0, i32 1
	store i32 %reg_57, ptr %reg_58
	%reg_59 = add i32 3, 0
	%reg_60 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 1, i32 0
	store i32 %reg_59, ptr %reg_60
	%reg_61 = add i32 4, 0
	%reg_62 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 1, i32 1
	store i32 %reg_61, ptr %reg_62
	%reg_63 = add i32 5, 0
	%reg_64 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 2, i32 0
	store i32 %reg_63, ptr %reg_64
	%reg_65 = add i32 6, 0
	%reg_66 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 2, i32 1
	store i32 %reg_65, ptr %reg_66
	%reg_67 = add i32 7, 0
	%reg_68 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 3, i32 0
	store i32 %reg_67, ptr %reg_68
	%reg_69 = add i32 8, 0
	%reg_70 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 3, i32 1
	store i32 %reg_69, ptr %reg_70
	%reg_71 = add i32 3, 0
	%reg_72 = add i32 1, 0
	%reg_73 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 %reg_71, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	%reg_75 = add i32 0, 0
	%reg_76 = add i32 0, 0
	%reg_77 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 %reg_75, i32 %reg_76
	%reg_78 = load i32, ptr %reg_77
	%reg_79 = add i32 %reg_74, %reg_78
	%reg_80 = add i32 0, 0
	%reg_81 = add i32 1, 0
	%reg_82 = getelementptr [4 x [2 x i32]], ptr %reg_48, i32 0, i32 %reg_80, i32 %reg_81
	%reg_83 = load i32, ptr %reg_82
	%reg_84 = add i32 %reg_79, %reg_83
	%reg_85 = add i32 2, 0
	%reg_86 = add i32 0, 0
	%reg_87 = getelementptr [4 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_85, i32 %reg_86
	%reg_88 = load i32, ptr %reg_87
	%reg_89 = add i32 %reg_84, %reg_88
	ret i32 %reg_89
}
