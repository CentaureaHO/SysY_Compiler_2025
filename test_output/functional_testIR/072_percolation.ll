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

@array = global [110 x i32] zeroinitializer
@n = global i32 zeroinitializer

define void @init(i32 %reg_0)
{
Block0:  ;Func define at line 3
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 3
	%reg_3 = add i32 1, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 5
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = load i32, ptr %reg_1
	%reg_7 = mul i32 %reg_5, %reg_6
	%reg_8 = add i32 1, 0
	%reg_9 = add i32 %reg_7, %reg_8
	%reg_10 = icmp sle i32 %reg_4, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 5
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_11
	%reg_13 = add i32 1, 0
	%reg_14 = sub i32 0, %reg_13
	store i32 %reg_14, ptr %reg_12
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_2
	br label %Block2
Block4:  ;While end at line 5
	ret void
}

define i32 @findfa(i32 %reg_0)
{
Block0:  ;Func define at line 11
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 11
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 12
	%reg_7 = load i32, ptr %reg_1
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 12
	ret i32 0
Block4:  ;If else at line 12
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	%reg_13 = call i32 @findfa(i32 %reg_12)
	store i32 %reg_13, ptr %reg_9
	%reg_14 = load i32, ptr %reg_1
	%reg_15 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	ret i32 %reg_16
	br label %Block3
}

define void @mmerge(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 19
	%reg_7 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 19
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = call i32 @findfa(i32 %reg_5)
	store i32 %reg_6, ptr %reg_4
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = call i32 @findfa(i32 %reg_8)
	store i32 %reg_9, ptr %reg_7
	%reg_10 = load i32, ptr %reg_4
	%reg_11 = load i32, ptr %reg_7
	%reg_12 = icmp ne i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block2, label %Block3
Block2:  ;If then at line 22
	%reg_13 = load i32, ptr %reg_4
	%reg_14 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_7
	store i32 %reg_15, ptr %reg_14
	br label %Block3
Block3:  ;If end at line 22
	ret void
}

define i32 @main()
{
Block0:  ;Func define at line 24
	%reg_139 = alloca i32
	%reg_34 = alloca i32
	%reg_21 = alloca i32
	%reg_18 = alloca i32
	%reg_16 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 24
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = add i32 1, 0
	store i32 %reg_8, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 28
	%reg_9 = load i32, ptr %reg_0
	%reg_10 = icmp ne i32 %reg_9, 0
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 28
	%reg_11 = load i32, ptr %reg_0
	%reg_12 = add i32 1, 0
	%reg_13 = sub i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_0
	%reg_14 = add i32 4, 0
	store i32 %reg_14, ptr @n
	%reg_15 = add i32 10, 0
	store i32 %reg_15, ptr %reg_2
	%reg_17 = add i32 0, 0
	store i32 %reg_17, ptr %reg_16
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = load i32, ptr @n
	call void @init(i32 %reg_20)
	%reg_22 = load i32, ptr @n
	%reg_23 = load i32, ptr @n
	%reg_24 = mul i32 %reg_22, %reg_23
	%reg_25 = add i32 1, 0
	%reg_26 = add i32 %reg_24, %reg_25
	store i32 %reg_26, ptr %reg_21
	br label %Block5
Block4:  ;While end at line 28
	%reg_153 = add i32 0, 0
	ret i32 %reg_153
Block5:  ;While condition at line 37
	%reg_27 = load i32, ptr %reg_16
	%reg_28 = load i32, ptr %reg_2
	%reg_29 = icmp slt i32 %reg_27, %reg_28
	br i1 %reg_29, label %Block6, label %Block7
Block6:  ;While body at line 37
	%reg_30 = call i32 @getint()
	store i32 %reg_30, ptr %reg_4
	%reg_31 = call i32 @getint()
	store i32 %reg_31, ptr %reg_6
	%reg_32 = load i32, ptr %reg_18
	%reg_33 = icmp eq i32 %reg_32, 0
	br i1 %reg_33, label %Block8, label %Block9
Block7:  ;While end at line 37
	%reg_148 = load i32, ptr %reg_18
	%reg_149 = icmp eq i32 %reg_148, 0
	br i1 %reg_149, label %Block30, label %Block31
Block8:  ;If then at line 41
	%reg_35 = load i32, ptr @n
	%reg_36 = load i32, ptr %reg_4
	%reg_37 = add i32 1, 0
	%reg_38 = sub i32 %reg_36, %reg_37
	%reg_39 = mul i32 %reg_35, %reg_38
	%reg_40 = load i32, ptr %reg_6
	%reg_41 = add i32 %reg_39, %reg_40
	store i32 %reg_41, ptr %reg_34
	%reg_42 = load i32, ptr %reg_34
	%reg_43 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_42
	%reg_44 = load i32, ptr %reg_34
	store i32 %reg_44, ptr %reg_43
	%reg_45 = load i32, ptr %reg_4
	%reg_46 = add i32 1, 0
	%reg_47 = icmp eq i32 %reg_45, %reg_46
	br i1 %reg_47, label %Block10, label %Block11
Block9:  ;If end at line 41
	%reg_145 = load i32, ptr %reg_16
	%reg_146 = add i32 1, 0
	%reg_147 = add i32 %reg_145, %reg_146
	store i32 %reg_147, ptr %reg_16
	br label %Block5
Block10:  ;If then at line 45
	%reg_48 = add i32 0, 0
	%reg_49 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_48
	%reg_50 = add i32 0, 0
	store i32 %reg_50, ptr %reg_49
	%reg_51 = load i32, ptr %reg_34
	%reg_52 = add i32 0, 0
	call void @mmerge(i32 %reg_51, i32 %reg_52)
	br label %Block11
Block11:  ;If end at line 45
	%reg_53 = load i32, ptr %reg_4
	%reg_54 = load i32, ptr @n
	%reg_55 = icmp eq i32 %reg_53, %reg_54
	br i1 %reg_55, label %Block12, label %Block13
Block12:  ;If then at line 49
	%reg_56 = load i32, ptr %reg_21
	%reg_57 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_56
	%reg_58 = load i32, ptr %reg_21
	store i32 %reg_58, ptr %reg_57
	%reg_59 = load i32, ptr %reg_34
	%reg_60 = load i32, ptr %reg_21
	call void @mmerge(i32 %reg_59, i32 %reg_60)
	br label %Block13
Block13:  ;If end at line 49
	%reg_61 = load i32, ptr %reg_6
	%reg_62 = load i32, ptr @n
	%reg_63 = icmp slt i32 %reg_61, %reg_62
	br i1 %reg_63, label %Block16, label %Block15
Block14:  ;If then at line 53
	%reg_72 = load i32, ptr %reg_34
	%reg_73 = load i32, ptr %reg_34
	%reg_74 = add i32 1, 0
	%reg_75 = add i32 %reg_73, %reg_74
	call void @mmerge(i32 %reg_72, i32 %reg_75)
	br label %Block15
Block15:  ;If end at line 53
	%reg_76 = load i32, ptr %reg_6
	%reg_77 = add i32 1, 0
	%reg_78 = icmp sgt i32 %reg_76, %reg_77
	br i1 %reg_78, label %Block19, label %Block18
Block16:  ;And opertor at line 53
	%reg_64 = load i32, ptr %reg_34
	%reg_65 = add i32 1, 0
	%reg_66 = add i32 %reg_64, %reg_65
	%reg_67 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	%reg_69 = add i32 1, 0
	%reg_70 = sub i32 0, %reg_69
	%reg_71 = icmp ne i32 %reg_68, %reg_70
	br i1 %reg_71, label %Block14, label %Block15
Block17:  ;If then at line 56
	%reg_87 = load i32, ptr %reg_34
	%reg_88 = load i32, ptr %reg_34
	%reg_89 = add i32 1, 0
	%reg_90 = sub i32 %reg_88, %reg_89
	call void @mmerge(i32 %reg_87, i32 %reg_90)
	br label %Block18
Block18:  ;If end at line 56
	%reg_91 = load i32, ptr %reg_4
	%reg_92 = load i32, ptr @n
	%reg_93 = icmp slt i32 %reg_91, %reg_92
	br i1 %reg_93, label %Block22, label %Block21
Block19:  ;And opertor at line 56
	%reg_79 = load i32, ptr %reg_34
	%reg_80 = add i32 1, 0
	%reg_81 = sub i32 %reg_79, %reg_80
	%reg_82 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_81
	%reg_83 = load i32, ptr %reg_82
	%reg_84 = add i32 1, 0
	%reg_85 = sub i32 0, %reg_84
	%reg_86 = icmp ne i32 %reg_83, %reg_85
	br i1 %reg_86, label %Block17, label %Block18
Block20:  ;If then at line 59
	%reg_102 = load i32, ptr %reg_34
	%reg_103 = load i32, ptr %reg_34
	%reg_104 = load i32, ptr @n
	%reg_105 = add i32 %reg_103, %reg_104
	call void @mmerge(i32 %reg_102, i32 %reg_105)
	br label %Block21
Block21:  ;If end at line 59
	%reg_106 = load i32, ptr %reg_4
	%reg_107 = add i32 1, 0
	%reg_108 = icmp sgt i32 %reg_106, %reg_107
	br i1 %reg_108, label %Block25, label %Block24
Block22:  ;And opertor at line 59
	%reg_94 = load i32, ptr %reg_34
	%reg_95 = load i32, ptr @n
	%reg_96 = add i32 %reg_94, %reg_95
	%reg_97 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_96
	%reg_98 = load i32, ptr %reg_97
	%reg_99 = add i32 1, 0
	%reg_100 = sub i32 0, %reg_99
	%reg_101 = icmp ne i32 %reg_98, %reg_100
	br i1 %reg_101, label %Block20, label %Block21
Block23:  ;If then at line 62
	%reg_117 = load i32, ptr %reg_34
	%reg_118 = load i32, ptr %reg_34
	%reg_119 = load i32, ptr @n
	%reg_120 = sub i32 %reg_118, %reg_119
	call void @mmerge(i32 %reg_117, i32 %reg_120)
	br label %Block24
Block24:  ;If end at line 62
	%reg_121 = add i32 0, 0
	%reg_122 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_121
	%reg_123 = load i32, ptr %reg_122
	%reg_124 = add i32 1, 0
	%reg_125 = sub i32 0, %reg_124
	%reg_126 = icmp ne i32 %reg_123, %reg_125
	br i1 %reg_126, label %Block29, label %Block27
Block25:  ;And opertor at line 62
	%reg_109 = load i32, ptr %reg_34
	%reg_110 = load i32, ptr @n
	%reg_111 = sub i32 %reg_109, %reg_110
	%reg_112 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_111
	%reg_113 = load i32, ptr %reg_112
	%reg_114 = add i32 1, 0
	%reg_115 = sub i32 0, %reg_114
	%reg_116 = icmp ne i32 %reg_113, %reg_115
	br i1 %reg_116, label %Block23, label %Block24
Block26:  ;If then at line 66
	%reg_138 = add i32 1, 0
	store i32 %reg_138, ptr %reg_18
	%reg_140 = load i32, ptr %reg_16
	%reg_141 = add i32 1, 0
	%reg_142 = add i32 %reg_140, %reg_141
	store i32 %reg_142, ptr %reg_139
	%reg_143 = load i32, ptr %reg_139
	call void @putint(i32 %reg_143)
	%reg_144 = add i32 10, 0
	call void @putch(i32 %reg_144)
	br label %Block27
Block27:  ;If end at line 66
	br label %Block9
Block28:  ;And opertor at line 66
	%reg_133 = add i32 0, 0
	%reg_134 = call i32 @findfa(i32 %reg_133)
	%reg_135 = load i32, ptr %reg_21
	%reg_136 = call i32 @findfa(i32 %reg_135)
	%reg_137 = icmp eq i32 %reg_134, %reg_136
	br i1 %reg_137, label %Block26, label %Block27
Block29:  ;And opertor at line -1
	%reg_127 = load i32, ptr %reg_21
	%reg_128 = getelementptr [110 x i32], ptr @array, i32 0, i32 %reg_127
	%reg_129 = load i32, ptr %reg_128
	%reg_130 = add i32 1, 0
	%reg_131 = sub i32 0, %reg_130
	%reg_132 = icmp ne i32 %reg_129, %reg_131
	br i1 %reg_132, label %Block28, label %Block27
Block30:  ;If then at line 76
	%reg_150 = add i32 1, 0
	%reg_151 = sub i32 0, %reg_150
	call void @putint(i32 %reg_151)
	%reg_152 = add i32 10, 0
	call void @putch(i32 %reg_152)
	br label %Block31
Block31:  ;If end at line 76
	br label %Block2
}
