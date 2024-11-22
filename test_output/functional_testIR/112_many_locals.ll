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


define i32 @foo()
{
Block0:  ;Func define at line 2
	%reg_81 = alloca i32
	%reg_65 = alloca i32
	%reg_63 = alloca i32
	%reg_61 = alloca i32
	%reg_59 = alloca i32
	%reg_57 = alloca i32
	%reg_55 = alloca i32
	%reg_53 = alloca i32
	%reg_51 = alloca i32
	%reg_49 = alloca i32
	%reg_47 = alloca i32
	%reg_45 = alloca i32
	%reg_43 = alloca i32
	%reg_41 = alloca i32
	%reg_39 = alloca i32
	%reg_37 = alloca i32
	%reg_35 = alloca i32
	%reg_33 = alloca i32
	%reg_0 = alloca [16 x i32]
	br label %Block1
Block1:  ;Func body at line 2
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 64, i1 1)
	%reg_1 = add i32 0, 0
	%reg_2 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 0
	store i32 %reg_1, ptr %reg_2
	%reg_3 = add i32 1, 0
	%reg_4 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 1
	store i32 %reg_3, ptr %reg_4
	%reg_5 = add i32 2, 0
	%reg_6 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 2
	store i32 %reg_5, ptr %reg_6
	%reg_7 = add i32 3, 0
	%reg_8 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 3
	store i32 %reg_7, ptr %reg_8
	%reg_9 = add i32 0, 0
	%reg_10 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 4
	store i32 %reg_9, ptr %reg_10
	%reg_11 = add i32 1, 0
	%reg_12 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 5
	store i32 %reg_11, ptr %reg_12
	%reg_13 = add i32 2, 0
	%reg_14 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 6
	store i32 %reg_13, ptr %reg_14
	%reg_15 = add i32 3, 0
	%reg_16 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 7
	store i32 %reg_15, ptr %reg_16
	%reg_17 = add i32 0, 0
	%reg_18 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 8
	store i32 %reg_17, ptr %reg_18
	%reg_19 = add i32 1, 0
	%reg_20 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 9
	store i32 %reg_19, ptr %reg_20
	%reg_21 = add i32 2, 0
	%reg_22 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 10
	store i32 %reg_21, ptr %reg_22
	%reg_23 = add i32 3, 0
	%reg_24 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 11
	store i32 %reg_23, ptr %reg_24
	%reg_25 = add i32 0, 0
	%reg_26 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 12
	store i32 %reg_25, ptr %reg_26
	%reg_27 = add i32 1, 0
	%reg_28 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 13
	store i32 %reg_27, ptr %reg_28
	%reg_29 = add i32 2, 0
	%reg_30 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 14
	store i32 %reg_29, ptr %reg_30
	%reg_31 = add i32 3, 0
	%reg_32 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 15
	store i32 %reg_31, ptr %reg_32
	%reg_34 = add i32 3, 0
	store i32 %reg_34, ptr %reg_33
	%reg_36 = add i32 7, 0
	store i32 %reg_36, ptr %reg_35
	%reg_38 = add i32 5, 0
	store i32 %reg_38, ptr %reg_37
	%reg_40 = add i32 6, 0
	store i32 %reg_40, ptr %reg_39
	%reg_42 = add i32 1, 0
	store i32 %reg_42, ptr %reg_41
	%reg_44 = add i32 0, 0
	store i32 %reg_44, ptr %reg_43
	%reg_46 = add i32 3, 0
	store i32 %reg_46, ptr %reg_45
	%reg_48 = add i32 5, 0
	store i32 %reg_48, ptr %reg_47
	%reg_50 = add i32 4, 0
	store i32 %reg_50, ptr %reg_49
	%reg_52 = add i32 2, 0
	store i32 %reg_52, ptr %reg_51
	%reg_54 = add i32 7, 0
	store i32 %reg_54, ptr %reg_53
	%reg_56 = add i32 9, 0
	store i32 %reg_56, ptr %reg_55
	%reg_58 = add i32 8, 0
	store i32 %reg_58, ptr %reg_57
	%reg_60 = add i32 1, 0
	store i32 %reg_60, ptr %reg_59
	%reg_62 = add i32 4, 0
	store i32 %reg_62, ptr %reg_61
	%reg_64 = add i32 6, 0
	store i32 %reg_64, ptr %reg_63
	%reg_66 = load i32, ptr %reg_33
	%reg_67 = load i32, ptr %reg_35
	%reg_68 = add i32 %reg_66, %reg_67
	%reg_69 = load i32, ptr %reg_37
	%reg_70 = add i32 %reg_68, %reg_69
	%reg_71 = load i32, ptr %reg_39
	%reg_72 = add i32 %reg_70, %reg_71
	%reg_73 = load i32, ptr %reg_41
	%reg_74 = add i32 %reg_72, %reg_73
	%reg_75 = load i32, ptr %reg_43
	%reg_76 = add i32 %reg_74, %reg_75
	%reg_77 = load i32, ptr %reg_45
	%reg_78 = add i32 %reg_76, %reg_77
	%reg_79 = load i32, ptr %reg_47
	%reg_80 = add i32 %reg_78, %reg_79
	store i32 %reg_80, ptr %reg_65
	%reg_82 = load i32, ptr %reg_49
	%reg_83 = load i32, ptr %reg_51
	%reg_84 = add i32 %reg_82, %reg_83
	%reg_85 = load i32, ptr %reg_53
	%reg_86 = add i32 %reg_84, %reg_85
	%reg_87 = load i32, ptr %reg_55
	%reg_88 = add i32 %reg_86, %reg_87
	%reg_89 = load i32, ptr %reg_57
	%reg_90 = add i32 %reg_88, %reg_89
	%reg_91 = load i32, ptr %reg_59
	%reg_92 = add i32 %reg_90, %reg_91
	%reg_93 = load i32, ptr %reg_61
	%reg_94 = add i32 %reg_92, %reg_93
	%reg_95 = load i32, ptr %reg_63
	%reg_96 = add i32 %reg_94, %reg_95
	store i32 %reg_96, ptr %reg_81
	%reg_97 = load i32, ptr %reg_65
	%reg_98 = load i32, ptr %reg_81
	%reg_99 = add i32 %reg_97, %reg_98
	%reg_100 = load i32, ptr %reg_33
	%reg_101 = getelementptr [16 x i32], ptr %reg_0, i32 0, i32 %reg_100
	%reg_102 = load i32, ptr %reg_101
	%reg_103 = add i32 %reg_99, %reg_102
	ret i32 %reg_103
}

define i32 @main()
{
Block0:  ;Func define at line 15
	%reg_110 = alloca i32
	%reg_94 = alloca i32
	%reg_81 = alloca i32
	%reg_79 = alloca i32
	%reg_77 = alloca i32
	%reg_75 = alloca i32
	%reg_73 = alloca i32
	%reg_71 = alloca i32
	%reg_69 = alloca i32
	%reg_67 = alloca i32
	%reg_48 = alloca i32
	%reg_32 = alloca i32
	%reg_30 = alloca i32
	%reg_28 = alloca i32
	%reg_26 = alloca i32
	%reg_24 = alloca i32
	%reg_22 = alloca i32
	%reg_20 = alloca i32
	%reg_18 = alloca i32
	%reg_16 = alloca i32
	%reg_14 = alloca i32
	%reg_12 = alloca i32
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 15
	%reg_1 = add i32 3, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 7, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 5, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 6, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = add i32 1, 0
	store i32 %reg_9, ptr %reg_8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	%reg_13 = add i32 3, 0
	store i32 %reg_13, ptr %reg_12
	%reg_15 = add i32 5, 0
	store i32 %reg_15, ptr %reg_14
	%reg_17 = add i32 4, 0
	store i32 %reg_17, ptr %reg_16
	%reg_19 = add i32 2, 0
	store i32 %reg_19, ptr %reg_18
	%reg_21 = add i32 7, 0
	store i32 %reg_21, ptr %reg_20
	%reg_23 = add i32 9, 0
	store i32 %reg_23, ptr %reg_22
	%reg_25 = add i32 8, 0
	store i32 %reg_25, ptr %reg_24
	%reg_27 = add i32 1, 0
	store i32 %reg_27, ptr %reg_26
	%reg_29 = add i32 4, 0
	store i32 %reg_29, ptr %reg_28
	%reg_31 = add i32 6, 0
	store i32 %reg_31, ptr %reg_30
	%reg_33 = load i32, ptr %reg_0
	%reg_34 = load i32, ptr %reg_2
	%reg_35 = add i32 %reg_33, %reg_34
	%reg_36 = load i32, ptr %reg_4
	%reg_37 = add i32 %reg_35, %reg_36
	%reg_38 = load i32, ptr %reg_6
	%reg_39 = add i32 %reg_37, %reg_38
	%reg_40 = load i32, ptr %reg_8
	%reg_41 = add i32 %reg_39, %reg_40
	%reg_42 = load i32, ptr %reg_10
	%reg_43 = add i32 %reg_41, %reg_42
	%reg_44 = load i32, ptr %reg_12
	%reg_45 = add i32 %reg_43, %reg_44
	%reg_46 = load i32, ptr %reg_14
	%reg_47 = add i32 %reg_45, %reg_46
	store i32 %reg_47, ptr %reg_32
	%reg_49 = load i32, ptr %reg_16
	%reg_50 = load i32, ptr %reg_18
	%reg_51 = add i32 %reg_49, %reg_50
	%reg_52 = load i32, ptr %reg_20
	%reg_53 = add i32 %reg_51, %reg_52
	%reg_54 = load i32, ptr %reg_22
	%reg_55 = add i32 %reg_53, %reg_54
	%reg_56 = load i32, ptr %reg_24
	%reg_57 = add i32 %reg_55, %reg_56
	%reg_58 = load i32, ptr %reg_26
	%reg_59 = add i32 %reg_57, %reg_58
	%reg_60 = load i32, ptr %reg_28
	%reg_61 = add i32 %reg_59, %reg_60
	%reg_62 = load i32, ptr %reg_30
	%reg_63 = add i32 %reg_61, %reg_62
	store i32 %reg_63, ptr %reg_48
	%reg_64 = load i32, ptr %reg_32
	%reg_65 = call i32 @foo()
	%reg_66 = add i32 %reg_64, %reg_65
	store i32 %reg_66, ptr %reg_32
	%reg_68 = add i32 4, 0
	store i32 %reg_68, ptr %reg_67
	%reg_70 = add i32 7, 0
	store i32 %reg_70, ptr %reg_69
	%reg_72 = add i32 2, 0
	store i32 %reg_72, ptr %reg_71
	%reg_74 = add i32 5, 0
	store i32 %reg_74, ptr %reg_73
	%reg_76 = add i32 8, 0
	store i32 %reg_76, ptr %reg_75
	%reg_78 = add i32 0, 0
	store i32 %reg_78, ptr %reg_77
	%reg_80 = add i32 6, 0
	store i32 %reg_80, ptr %reg_79
	%reg_82 = add i32 3, 0
	store i32 %reg_82, ptr %reg_81
	%reg_83 = load i32, ptr %reg_48
	%reg_84 = call i32 @foo()
	%reg_85 = add i32 %reg_83, %reg_84
	store i32 %reg_85, ptr %reg_48
	%reg_86 = load i32, ptr %reg_16
	store i32 %reg_86, ptr %reg_0
	%reg_87 = load i32, ptr %reg_18
	store i32 %reg_87, ptr %reg_2
	%reg_88 = load i32, ptr %reg_20
	store i32 %reg_88, ptr %reg_4
	%reg_89 = load i32, ptr %reg_22
	store i32 %reg_89, ptr %reg_6
	%reg_90 = load i32, ptr %reg_24
	store i32 %reg_90, ptr %reg_8
	%reg_91 = load i32, ptr %reg_26
	store i32 %reg_91, ptr %reg_10
	%reg_92 = load i32, ptr %reg_28
	store i32 %reg_92, ptr %reg_12
	%reg_93 = load i32, ptr %reg_30
	store i32 %reg_93, ptr %reg_14
	%reg_95 = load i32, ptr %reg_67
	%reg_96 = load i32, ptr %reg_69
	%reg_97 = add i32 %reg_95, %reg_96
	%reg_98 = load i32, ptr %reg_71
	%reg_99 = add i32 %reg_97, %reg_98
	%reg_100 = load i32, ptr %reg_73
	%reg_101 = add i32 %reg_99, %reg_100
	%reg_102 = load i32, ptr %reg_75
	%reg_103 = add i32 %reg_101, %reg_102
	%reg_104 = load i32, ptr %reg_77
	%reg_105 = add i32 %reg_103, %reg_104
	%reg_106 = load i32, ptr %reg_79
	%reg_107 = add i32 %reg_105, %reg_106
	%reg_108 = load i32, ptr %reg_81
	%reg_109 = add i32 %reg_107, %reg_108
	store i32 %reg_109, ptr %reg_94
	%reg_111 = load i32, ptr %reg_32
	%reg_112 = load i32, ptr %reg_48
	%reg_113 = add i32 %reg_111, %reg_112
	%reg_114 = load i32, ptr %reg_94
	%reg_115 = add i32 %reg_113, %reg_114
	store i32 %reg_115, ptr %reg_110
	%reg_116 = load i32, ptr %reg_110
	call void @putint(i32 %reg_116)
	%reg_117 = add i32 10, 0
	call void @putch(i32 %reg_117)
	%reg_118 = add i32 0, 0
	ret i32 %reg_118
}
