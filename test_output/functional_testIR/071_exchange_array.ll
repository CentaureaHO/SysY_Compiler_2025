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
	%reg_57 = alloca i32
	%reg_55 = alloca i32
	%reg_53 = alloca i32
	%reg_51 = alloca i32
	%reg_0 = alloca [5 x [5 x i32]]
	br label %Block1
Block1:  ;Func body at line 1
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 100, i1 1)
	%reg_1 = add i32 1, 0
	%reg_2 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 0, i32 0
	store i32 %reg_1, ptr %reg_2
	%reg_3 = add i32 1, 0
	%reg_4 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 0, i32 1
	store i32 %reg_3, ptr %reg_4
	%reg_5 = add i32 1, 0
	%reg_6 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 0, i32 2
	store i32 %reg_5, ptr %reg_6
	%reg_7 = add i32 1, 0
	%reg_8 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 0, i32 3
	store i32 %reg_7, ptr %reg_8
	%reg_9 = add i32 1, 0
	%reg_10 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 0, i32 4
	store i32 %reg_9, ptr %reg_10
	%reg_11 = add i32 2, 0
	%reg_12 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 1, i32 0
	store i32 %reg_11, ptr %reg_12
	%reg_13 = add i32 2, 0
	%reg_14 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 1, i32 1
	store i32 %reg_13, ptr %reg_14
	%reg_15 = add i32 2, 0
	%reg_16 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 1, i32 2
	store i32 %reg_15, ptr %reg_16
	%reg_17 = add i32 2, 0
	%reg_18 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 1, i32 3
	store i32 %reg_17, ptr %reg_18
	%reg_19 = add i32 2, 0
	%reg_20 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 1, i32 4
	store i32 %reg_19, ptr %reg_20
	%reg_21 = add i32 3, 0
	%reg_22 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 2, i32 0
	store i32 %reg_21, ptr %reg_22
	%reg_23 = add i32 3, 0
	%reg_24 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 2, i32 1
	store i32 %reg_23, ptr %reg_24
	%reg_25 = add i32 3, 0
	%reg_26 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 2, i32 2
	store i32 %reg_25, ptr %reg_26
	%reg_27 = add i32 3, 0
	%reg_28 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 2, i32 3
	store i32 %reg_27, ptr %reg_28
	%reg_29 = add i32 3, 0
	%reg_30 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 2, i32 4
	store i32 %reg_29, ptr %reg_30
	%reg_31 = add i32 4, 0
	%reg_32 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 3, i32 0
	store i32 %reg_31, ptr %reg_32
	%reg_33 = add i32 4, 0
	%reg_34 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 3, i32 1
	store i32 %reg_33, ptr %reg_34
	%reg_35 = add i32 4, 0
	%reg_36 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 3, i32 2
	store i32 %reg_35, ptr %reg_36
	%reg_37 = add i32 4, 0
	%reg_38 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 3, i32 3
	store i32 %reg_37, ptr %reg_38
	%reg_39 = add i32 4, 0
	%reg_40 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 3, i32 4
	store i32 %reg_39, ptr %reg_40
	%reg_41 = add i32 5, 0
	%reg_42 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 4, i32 0
	store i32 %reg_41, ptr %reg_42
	%reg_43 = add i32 5, 0
	%reg_44 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 4, i32 1
	store i32 %reg_43, ptr %reg_44
	%reg_45 = add i32 5, 0
	%reg_46 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 4, i32 2
	store i32 %reg_45, ptr %reg_46
	%reg_47 = add i32 5, 0
	%reg_48 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 4, i32 3
	store i32 %reg_47, ptr %reg_48
	%reg_49 = add i32 5, 0
	%reg_50 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 4, i32 4
	store i32 %reg_49, ptr %reg_50
	%reg_52 = add i32 0, 0
	store i32 %reg_52, ptr %reg_51
	%reg_54 = add i32 0, 0
	store i32 %reg_54, ptr %reg_53
	%reg_56 = add i32 0, 0
	store i32 %reg_56, ptr %reg_55
	%reg_58 = add i32 0, 0
	store i32 %reg_58, ptr %reg_57
	%reg_59 = add i32 2, 0
	store i32 %reg_59, ptr %reg_51
	%reg_60 = add i32 3, 0
	store i32 %reg_60, ptr %reg_53
	%reg_61 = add i32 0, 0
	store i32 %reg_61, ptr %reg_57
	br label %Block2
Block2:  ;While condition at line 7
	%reg_62 = load i32, ptr %reg_57
	%reg_63 = add i32 5, 0
	%reg_64 = icmp slt i32 %reg_62, %reg_63
	br i1 %reg_64, label %Block3, label %Block4
Block3:  ;While body at line 7
	%reg_65 = load i32, ptr %reg_51
	%reg_66 = add i32 1, 0
	%reg_67 = sub i32 %reg_65, %reg_66
	%reg_68 = load i32, ptr %reg_57
	%reg_69 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 %reg_67, i32 %reg_68
	%reg_70 = load i32, ptr %reg_69
	store i32 %reg_70, ptr %reg_55
	%reg_71 = load i32, ptr %reg_51
	%reg_72 = add i32 1, 0
	%reg_73 = sub i32 %reg_71, %reg_72
	%reg_74 = load i32, ptr %reg_57
	%reg_75 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 %reg_73, i32 %reg_74
	%reg_76 = load i32, ptr %reg_53
	%reg_77 = add i32 1, 0
	%reg_78 = sub i32 %reg_76, %reg_77
	%reg_79 = load i32, ptr %reg_57
	%reg_80 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 %reg_78, i32 %reg_79
	%reg_81 = load i32, ptr %reg_80
	store i32 %reg_81, ptr %reg_75
	%reg_82 = load i32, ptr %reg_53
	%reg_83 = add i32 1, 0
	%reg_84 = sub i32 %reg_82, %reg_83
	%reg_85 = load i32, ptr %reg_57
	%reg_86 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 %reg_84, i32 %reg_85
	%reg_87 = load i32, ptr %reg_55
	store i32 %reg_87, ptr %reg_86
	%reg_88 = load i32, ptr %reg_57
	%reg_89 = add i32 1, 0
	%reg_90 = add i32 %reg_88, %reg_89
	store i32 %reg_90, ptr %reg_57
	br label %Block2
Block4:  ;While end at line 7
	%reg_91 = add i32 2, 0
	%reg_92 = add i32 0, 0
	%reg_93 = getelementptr [5 x [5 x i32]], ptr %reg_0, i32 0, i32 %reg_91, i32 %reg_92
	%reg_94 = load i32, ptr %reg_93
	ret i32 %reg_94
}
