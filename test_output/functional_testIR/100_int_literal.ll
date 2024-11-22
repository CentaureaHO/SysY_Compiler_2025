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

@s = global i32 0

define i32 @get_ans_se(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 3
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 3
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_4
	%reg_9 = load i32, ptr %reg_5
	%reg_10 = icmp eq i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block2, label %Block3
Block2:  ;If then at line 5
	%reg_11 = add i32 1, 0
	store i32 %reg_11, ptr %reg_6
	br label %Block3
Block3:  ;If end at line 5
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = add i32 2, 0
	%reg_14 = mul i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_3
	%reg_15 = load i32, ptr %reg_3
	%reg_16 = load i32, ptr %reg_6
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_3
	%reg_18 = load i32, ptr @s
	%reg_19 = load i32, ptr %reg_3
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr @s
	%reg_21 = load i32, ptr %reg_3
	ret i32 %reg_21
}

define i32 @get_ans(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 12
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 12
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_4
	%reg_9 = load i32, ptr %reg_5
	%reg_10 = icmp eq i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block2, label %Block3
Block2:  ;If then at line 14
	%reg_11 = add i32 1, 0
	store i32 %reg_11, ptr %reg_6
	br label %Block3
Block3:  ;If end at line 14
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = add i32 2, 0
	%reg_14 = mul i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_3
	%reg_15 = load i32, ptr %reg_3
	%reg_16 = load i32, ptr %reg_6
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_3
	%reg_18 = load i32, ptr %reg_3
	ret i32 %reg_18
}

define i32 @main()
{
Block0:  ;Func define at line 20
	%reg_21 = alloca i32
	%reg_19 = alloca i32
	%reg_17 = alloca i32
	%reg_15 = alloca i32
	%reg_11 = alloca i32
	%reg_9 = alloca i32
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 20
	%reg_1 = add i32 -2147483648, 0
	%reg_2 = sub i32 0, %reg_1
	store i32 %reg_2, ptr %reg_0
	%reg_4 = add i32 -2147483648, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 -2147483648, 0
	%reg_7 = add i32 1, 0
	%reg_8 = add i32 %reg_6, %reg_7
	store i32 %reg_8, ptr %reg_5
	%reg_10 = add i32 2147483647, 0
	store i32 %reg_10, ptr %reg_9
	%reg_12 = add i32 2147483647, 0
	%reg_13 = add i32 1, 0
	%reg_14 = sub i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_11
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_17
	%reg_20 = add i32 0, 0
	store i32 %reg_20, ptr %reg_19
	%reg_22 = add i32 0, 0
	store i32 %reg_22, ptr %reg_21
	%reg_23 = add i32 0, 0
	%reg_24 = load i32, ptr %reg_0
	%reg_25 = load i32, ptr %reg_3
	%reg_26 = call i32 @get_ans(i32 %reg_23, i32 %reg_24, i32 %reg_25)
	store i32 %reg_26, ptr %reg_15
	%reg_27 = load i32, ptr %reg_15
	%reg_28 = load i32, ptr %reg_0
	%reg_29 = add i32 1, 0
	%reg_30 = add i32 %reg_28, %reg_29
	%reg_31 = load i32, ptr %reg_5
	%reg_32 = call i32 @get_ans(i32 %reg_27, i32 %reg_30, i32 %reg_31)
	store i32 %reg_32, ptr %reg_15
	%reg_33 = load i32, ptr %reg_15
	%reg_34 = load i32, ptr %reg_0
	%reg_35 = load i32, ptr %reg_9
	%reg_36 = sub i32 0, %reg_35
	%reg_37 = add i32 1, 0
	%reg_38 = sub i32 %reg_36, %reg_37
	%reg_39 = call i32 @get_ans(i32 %reg_33, i32 %reg_34, i32 %reg_38)
	store i32 %reg_39, ptr %reg_15
	%reg_40 = load i32, ptr %reg_15
	%reg_41 = load i32, ptr %reg_0
	%reg_42 = load i32, ptr %reg_11
	%reg_43 = add i32 1, 0
	%reg_44 = add i32 %reg_42, %reg_43
	%reg_45 = call i32 @get_ans(i32 %reg_40, i32 %reg_41, i32 %reg_44)
	store i32 %reg_45, ptr %reg_15
	%reg_46 = load i32, ptr %reg_15
	%reg_47 = load i32, ptr %reg_3
	%reg_48 = add i32 2, 0
	%reg_49 = sdiv i32 %reg_47, %reg_48
	%reg_50 = load i32, ptr %reg_5
	%reg_51 = add i32 2, 0
	%reg_52 = sdiv i32 %reg_50, %reg_51
	%reg_53 = call i32 @get_ans(i32 %reg_46, i32 %reg_49, i32 %reg_52)
	store i32 %reg_53, ptr %reg_15
	%reg_54 = load i32, ptr %reg_15
	%reg_55 = load i32, ptr %reg_3
	%reg_56 = load i32, ptr %reg_9
	%reg_57 = sub i32 0, %reg_56
	%reg_58 = add i32 1, 0
	%reg_59 = sub i32 %reg_57, %reg_58
	%reg_60 = call i32 @get_ans(i32 %reg_54, i32 %reg_55, i32 %reg_59)
	store i32 %reg_60, ptr %reg_15
	%reg_61 = load i32, ptr %reg_15
	%reg_62 = load i32, ptr %reg_3
	%reg_63 = load i32, ptr %reg_11
	%reg_64 = add i32 1, 0
	%reg_65 = add i32 %reg_63, %reg_64
	%reg_66 = call i32 @get_ans(i32 %reg_61, i32 %reg_62, i32 %reg_65)
	store i32 %reg_66, ptr %reg_15
	%reg_67 = add i32 0, 0
	%reg_68 = load i32, ptr %reg_5
	%reg_69 = load i32, ptr %reg_9
	%reg_70 = call i32 @get_ans(i32 %reg_67, i32 %reg_68, i32 %reg_69)
	store i32 %reg_70, ptr %reg_17
	%reg_71 = load i32, ptr %reg_17
	%reg_72 = load i32, ptr %reg_5
	%reg_73 = load i32, ptr %reg_11
	%reg_74 = call i32 @get_ans(i32 %reg_71, i32 %reg_72, i32 %reg_73)
	store i32 %reg_74, ptr %reg_17
	%reg_75 = load i32, ptr %reg_17
	%reg_76 = load i32, ptr %reg_9
	%reg_77 = load i32, ptr %reg_11
	%reg_78 = call i32 @get_ans(i32 %reg_75, i32 %reg_76, i32 %reg_77)
	store i32 %reg_78, ptr %reg_17
	%reg_79 = load i32, ptr %reg_17
	%reg_80 = load i32, ptr %reg_0
	%reg_81 = add i32 2, 0
	%reg_82 = sdiv i32 %reg_80, %reg_81
	%reg_83 = load i32, ptr %reg_3
	%reg_84 = add i32 2, 0
	%reg_85 = sdiv i32 %reg_83, %reg_84
	%reg_86 = call i32 @get_ans(i32 %reg_79, i32 %reg_82, i32 %reg_85)
	store i32 %reg_86, ptr %reg_17
	%reg_87 = add i32 0, 0
	%reg_88 = load i32, ptr %reg_0
	%reg_89 = load i32, ptr %reg_3
	%reg_90 = call i32 @get_ans_se(i32 %reg_87, i32 %reg_88, i32 %reg_89)
	store i32 %reg_90, ptr %reg_19
	%reg_91 = load i32, ptr %reg_19
	%reg_92 = load i32, ptr %reg_0
	%reg_93 = add i32 1, 0
	%reg_94 = add i32 %reg_92, %reg_93
	%reg_95 = load i32, ptr %reg_5
	%reg_96 = call i32 @get_ans_se(i32 %reg_91, i32 %reg_94, i32 %reg_95)
	store i32 %reg_96, ptr %reg_19
	%reg_97 = load i32, ptr %reg_19
	%reg_98 = load i32, ptr %reg_0
	%reg_99 = load i32, ptr %reg_9
	%reg_100 = sub i32 0, %reg_99
	%reg_101 = add i32 1, 0
	%reg_102 = sub i32 %reg_100, %reg_101
	%reg_103 = call i32 @get_ans_se(i32 %reg_97, i32 %reg_98, i32 %reg_102)
	store i32 %reg_103, ptr %reg_19
	%reg_104 = load i32, ptr %reg_19
	%reg_105 = load i32, ptr %reg_0
	%reg_106 = load i32, ptr %reg_11
	%reg_107 = add i32 1, 0
	%reg_108 = add i32 %reg_106, %reg_107
	%reg_109 = call i32 @get_ans_se(i32 %reg_104, i32 %reg_105, i32 %reg_108)
	store i32 %reg_109, ptr %reg_19
	%reg_110 = load i32, ptr %reg_19
	%reg_111 = load i32, ptr %reg_3
	%reg_112 = add i32 2, 0
	%reg_113 = sdiv i32 %reg_111, %reg_112
	%reg_114 = load i32, ptr %reg_5
	%reg_115 = add i32 2, 0
	%reg_116 = sdiv i32 %reg_114, %reg_115
	%reg_117 = call i32 @get_ans_se(i32 %reg_110, i32 %reg_113, i32 %reg_116)
	store i32 %reg_117, ptr %reg_19
	%reg_118 = load i32, ptr %reg_19
	%reg_119 = load i32, ptr %reg_3
	%reg_120 = load i32, ptr %reg_9
	%reg_121 = sub i32 0, %reg_120
	%reg_122 = add i32 1, 0
	%reg_123 = sub i32 %reg_121, %reg_122
	%reg_124 = call i32 @get_ans_se(i32 %reg_118, i32 %reg_119, i32 %reg_123)
	store i32 %reg_124, ptr %reg_19
	%reg_125 = load i32, ptr %reg_19
	%reg_126 = load i32, ptr %reg_3
	%reg_127 = load i32, ptr %reg_11
	%reg_128 = add i32 1, 0
	%reg_129 = add i32 %reg_127, %reg_128
	%reg_130 = call i32 @get_ans_se(i32 %reg_125, i32 %reg_126, i32 %reg_129)
	store i32 %reg_130, ptr %reg_19
	%reg_131 = add i32 0, 0
	%reg_132 = load i32, ptr %reg_5
	%reg_133 = load i32, ptr %reg_9
	%reg_134 = call i32 @get_ans_se(i32 %reg_131, i32 %reg_132, i32 %reg_133)
	store i32 %reg_134, ptr %reg_21
	%reg_135 = load i32, ptr %reg_21
	%reg_136 = load i32, ptr %reg_5
	%reg_137 = load i32, ptr %reg_11
	%reg_138 = call i32 @get_ans_se(i32 %reg_135, i32 %reg_136, i32 %reg_137)
	store i32 %reg_138, ptr %reg_21
	%reg_139 = load i32, ptr %reg_21
	%reg_140 = load i32, ptr %reg_9
	%reg_141 = load i32, ptr %reg_11
	%reg_142 = call i32 @get_ans_se(i32 %reg_139, i32 %reg_140, i32 %reg_141)
	store i32 %reg_142, ptr %reg_21
	%reg_143 = load i32, ptr %reg_21
	%reg_144 = load i32, ptr %reg_0
	%reg_145 = add i32 2, 0
	%reg_146 = sdiv i32 %reg_144, %reg_145
	%reg_147 = load i32, ptr %reg_3
	%reg_148 = add i32 2, 0
	%reg_149 = sdiv i32 %reg_147, %reg_148
	%reg_150 = call i32 @get_ans_se(i32 %reg_143, i32 %reg_146, i32 %reg_149)
	store i32 %reg_150, ptr %reg_21
	%reg_151 = load i32, ptr %reg_15
	%reg_152 = load i32, ptr %reg_17
	%reg_153 = add i32 %reg_151, %reg_152
	%reg_154 = load i32, ptr %reg_19
	%reg_155 = add i32 %reg_153, %reg_154
	%reg_156 = load i32, ptr %reg_21
	%reg_157 = add i32 %reg_155, %reg_156
	ret i32 %reg_157
}
