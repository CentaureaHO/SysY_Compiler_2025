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


define i32 @func1(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 1
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 1
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = add i32 0, 0
	%reg_8 = icmp eq i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block2, label %Block4
Block2:  ;If then at line 2
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = load i32, ptr %reg_4
	%reg_11 = mul i32 %reg_9, %reg_10
	ret i32 %reg_11
	br label %Block3
Block3:  ;If end at line 2
	ret i32 0
Block4:  ;If else at line 2
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = load i32, ptr %reg_4
	%reg_14 = load i32, ptr %reg_5
	%reg_15 = sub i32 %reg_13, %reg_14
	%reg_16 = add i32 0, 0
	%reg_17 = call i32 @func1(i32 %reg_12, i32 %reg_15, i32 %reg_16)
	ret i32 %reg_17
	br label %Block3
}

define i32 @func2(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 10
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 10
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = icmp ne i32 %reg_4, 0
	br i1 %reg_5, label %Block2, label %Block4
Block2:  ;If then at line 11
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = srem i32 %reg_6, %reg_7
	%reg_9 = add i32 0, 0
	%reg_10 = call i32 @func2(i32 %reg_8, i32 %reg_9)
	ret i32 %reg_10
	br label %Block3
Block3:  ;If end at line 11
	ret i32 0
Block4:  ;If else at line 11
	%reg_11 = load i32, ptr %reg_2
	ret i32 %reg_11
	br label %Block3
}

define i32 @func3(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 19
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 19
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = add i32 0, 0
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 20
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = add i32 1, 0
	%reg_9 = add i32 %reg_7, %reg_8
	ret i32 %reg_9
	br label %Block3
Block3:  ;If end at line 20
	ret i32 0
Block4:  ;If else at line 20
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = add i32 %reg_10, %reg_11
	%reg_13 = add i32 0, 0
	%reg_14 = call i32 @func3(i32 %reg_12, i32 %reg_13)
	ret i32 %reg_14
	br label %Block3
}

define i32 @func4(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 28
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 28
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block2, label %Block4
Block2:  ;If then at line 29
	%reg_8 = load i32, ptr %reg_4
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 29
	ret i32 0
Block4:  ;If else at line 29
	%reg_9 = load i32, ptr %reg_5
	ret i32 %reg_9
	br label %Block3
}

define i32 @func5(i32 %reg_0)
{
Block0:  ;Func define at line 37
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 37
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = sub i32 0, %reg_2
	ret i32 %reg_3
}

define i32 @func6(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 41
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 41
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = icmp ne i32 %reg_4, 0
	br i1 %reg_5, label %Block5, label %Block4
Block2:  ;If then at line 42
	%reg_8 = add i32 1, 0
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 42
	ret i32 0
Block4:  ;If else at line 42
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
	br label %Block3
Block5:  ;And opertor at line 42
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block2, label %Block4
}

define i32 @func7(i32 %reg_0)
{
Block0:  ;Func define at line 50
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 50
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = icmp eq i32 %reg_2, 0
	br i1 %reg_3, label %Block2, label %Block4
Block2:  ;If then at line 51
	%reg_4 = add i32 1, 0
	ret i32 %reg_4
	br label %Block3
Block3:  ;If end at line 51
	ret i32 0
Block4:  ;If else at line 51
	%reg_5 = add i32 0, 0
	ret i32 %reg_5
	br label %Block3
}

define i32 @main()
{
Block0:  ;Func define at line 59
	%reg_20 = alloca i32
	%reg_9 = alloca i32
	%reg_8 = alloca [10 x i32]
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 59
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	%reg_3 = call i32 @getint()
	store i32 %reg_3, ptr %reg_2
	%reg_5 = call i32 @getint()
	store i32 %reg_5, ptr %reg_4
	%reg_7 = call i32 @getint()
	store i32 %reg_7, ptr %reg_6
	call void @llvm.memset.p0.i32(ptr %reg_8, i8 0, i32 40, i1 1)
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block2
Block2:  ;While condition at line 63
	%reg_11 = load i32, ptr %reg_9
	%reg_12 = add i32 10, 0
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block3, label %Block4
Block3:  ;While body at line 63
	%reg_14 = load i32, ptr %reg_9
	%reg_15 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_14
	%reg_16 = call i32 @getint()
	store i32 %reg_16, ptr %reg_15
	%reg_17 = load i32, ptr %reg_9
	%reg_18 = add i32 1, 0
	%reg_19 = add i32 %reg_17, %reg_18
	store i32 %reg_19, ptr %reg_9
	br label %Block2
Block4:  ;While end at line 63
	%reg_21 = load i32, ptr %reg_0
	%reg_22 = call i32 @func7(i32 %reg_21)
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = call i32 @func5(i32 %reg_23)
	%reg_25 = call i32 @func6(i32 %reg_22, i32 %reg_24)
	%reg_26 = load i32, ptr %reg_4
	%reg_27 = call i32 @func2(i32 %reg_25, i32 %reg_26)
	%reg_28 = load i32, ptr %reg_6
	%reg_29 = call i32 @func3(i32 %reg_27, i32 %reg_28)
	%reg_30 = call i32 @func5(i32 %reg_29)
	%reg_31 = add i32 0, 0
	%reg_32 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_31
	%reg_33 = load i32, ptr %reg_32
	%reg_34 = add i32 1, 0
	%reg_35 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_34
	%reg_36 = load i32, ptr %reg_35
	%reg_37 = call i32 @func5(i32 %reg_36)
	%reg_38 = add i32 2, 0
	%reg_39 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_38
	%reg_40 = load i32, ptr %reg_39
	%reg_41 = add i32 3, 0
	%reg_42 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_41
	%reg_43 = load i32, ptr %reg_42
	%reg_44 = call i32 @func7(i32 %reg_43)
	%reg_45 = call i32 @func6(i32 %reg_40, i32 %reg_44)
	%reg_46 = add i32 4, 0
	%reg_47 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_46
	%reg_48 = load i32, ptr %reg_47
	%reg_49 = add i32 5, 0
	%reg_50 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = call i32 @func7(i32 %reg_51)
	%reg_53 = call i32 @func2(i32 %reg_48, i32 %reg_52)
	%reg_54 = call i32 @func4(i32 %reg_37, i32 %reg_45, i32 %reg_53)
	%reg_55 = add i32 6, 0
	%reg_56 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = call i32 @func3(i32 %reg_54, i32 %reg_57)
	%reg_59 = add i32 7, 0
	%reg_60 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_59
	%reg_61 = load i32, ptr %reg_60
	%reg_62 = call i32 @func2(i32 %reg_58, i32 %reg_61)
	%reg_63 = add i32 8, 0
	%reg_64 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_63
	%reg_65 = load i32, ptr %reg_64
	%reg_66 = add i32 9, 0
	%reg_67 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	%reg_69 = call i32 @func7(i32 %reg_68)
	%reg_70 = call i32 @func3(i32 %reg_65, i32 %reg_69)
	%reg_71 = load i32, ptr %reg_0
	%reg_72 = call i32 @func1(i32 %reg_62, i32 %reg_70, i32 %reg_71)
	%reg_73 = call i32 @func4(i32 %reg_30, i32 %reg_33, i32 %reg_72)
	%reg_74 = load i32, ptr %reg_2
	%reg_75 = load i32, ptr %reg_4
	%reg_76 = call i32 @func7(i32 %reg_75)
	%reg_77 = load i32, ptr %reg_6
	%reg_78 = call i32 @func3(i32 %reg_76, i32 %reg_77)
	%reg_79 = call i32 @func2(i32 %reg_74, i32 %reg_78)
	%reg_80 = call i32 @func3(i32 %reg_73, i32 %reg_79)
	%reg_81 = add i32 0, 0
	%reg_82 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_81
	%reg_83 = load i32, ptr %reg_82
	%reg_84 = add i32 1, 0
	%reg_85 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_84
	%reg_86 = load i32, ptr %reg_85
	%reg_87 = call i32 @func1(i32 %reg_80, i32 %reg_83, i32 %reg_86)
	%reg_88 = add i32 2, 0
	%reg_89 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_88
	%reg_90 = load i32, ptr %reg_89
	%reg_91 = call i32 @func2(i32 %reg_87, i32 %reg_90)
	%reg_92 = add i32 3, 0
	%reg_93 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_92
	%reg_94 = load i32, ptr %reg_93
	%reg_95 = add i32 4, 0
	%reg_96 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_95
	%reg_97 = load i32, ptr %reg_96
	%reg_98 = add i32 5, 0
	%reg_99 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_98
	%reg_100 = load i32, ptr %reg_99
	%reg_101 = call i32 @func5(i32 %reg_100)
	%reg_102 = call i32 @func3(i32 %reg_97, i32 %reg_101)
	%reg_103 = add i32 6, 0
	%reg_104 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_103
	%reg_105 = load i32, ptr %reg_104
	%reg_106 = call i32 @func5(i32 %reg_105)
	%reg_107 = call i32 @func2(i32 %reg_102, i32 %reg_106)
	%reg_108 = add i32 7, 0
	%reg_109 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_108
	%reg_110 = load i32, ptr %reg_109
	%reg_111 = add i32 8, 0
	%reg_112 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_111
	%reg_113 = load i32, ptr %reg_112
	%reg_114 = call i32 @func7(i32 %reg_113)
	%reg_115 = call i32 @func1(i32 %reg_107, i32 %reg_110, i32 %reg_114)
	%reg_116 = add i32 9, 0
	%reg_117 = getelementptr [10 x i32], ptr %reg_8, i32 0, i32 %reg_116
	%reg_118 = load i32, ptr %reg_117
	%reg_119 = call i32 @func5(i32 %reg_118)
	%reg_120 = call i32 @func2(i32 %reg_115, i32 %reg_119)
	%reg_121 = load i32, ptr %reg_0
	%reg_122 = call i32 @func3(i32 %reg_120, i32 %reg_121)
	%reg_123 = call i32 @func1(i32 %reg_91, i32 %reg_94, i32 %reg_122)
	store i32 %reg_123, ptr %reg_20
	%reg_124 = load i32, ptr %reg_20
	ret i32 %reg_124
}
