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

@maxn = global i32 18
@mod = global i32 1000000007
@dp = global [18 x [18 x [18 x [18 x [18 x [7 x i32]]]]]] zeroinitializer
@list = global [200 x i32] zeroinitializer
@cns = global [20 x i32] zeroinitializer

define i32 @equal(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 6
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 6
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 7
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 7
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
}

define i32 @dfs(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5)
{
Block0:  ;Func define at line 12
	%reg_43 = alloca i32
	%reg_11 = alloca i32
	%reg_10 = alloca i32
	%reg_9 = alloca i32
	%reg_8 = alloca i32
	%reg_7 = alloca i32
	%reg_6 = alloca i32
	store i32 %reg_0, ptr %reg_6
	store i32 %reg_1, ptr %reg_7
	store i32 %reg_2, ptr %reg_8
	store i32 %reg_3, ptr %reg_9
	store i32 %reg_4, ptr %reg_10
	store i32 %reg_5, ptr %reg_11
	br label %Block1
Block1:  ;Func body at line 12
	%reg_12 = load i32, ptr %reg_6
	%reg_13 = load i32, ptr %reg_7
	%reg_14 = load i32, ptr %reg_8
	%reg_15 = load i32, ptr %reg_9
	%reg_16 = load i32, ptr %reg_10
	%reg_17 = load i32, ptr %reg_11
	%reg_18 = getelementptr [18 x [18 x [18 x [18 x [18 x [7 x i32]]]]]], ptr @dp, i32 0, i32 %reg_12, i32 %reg_13, i32 %reg_14, i32 %reg_15, i32 %reg_16, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = add i32 1, 0
	%reg_21 = sub i32 0, %reg_20
	%reg_22 = icmp ne i32 %reg_19, %reg_21
	br i1 %reg_22, label %Block2, label %Block3
Block2:  ;If then at line 13
	%reg_23 = load i32, ptr %reg_6
	%reg_24 = load i32, ptr %reg_7
	%reg_25 = load i32, ptr %reg_8
	%reg_26 = load i32, ptr %reg_9
	%reg_27 = load i32, ptr %reg_10
	%reg_28 = load i32, ptr %reg_11
	%reg_29 = getelementptr [18 x [18 x [18 x [18 x [18 x [7 x i32]]]]]], ptr @dp, i32 0, i32 %reg_23, i32 %reg_24, i32 %reg_25, i32 %reg_26, i32 %reg_27, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	ret i32 %reg_30
	br label %Block3
Block3:  ;If end at line 13
	%reg_31 = load i32, ptr %reg_6
	%reg_32 = load i32, ptr %reg_7
	%reg_33 = add i32 %reg_31, %reg_32
	%reg_34 = load i32, ptr %reg_8
	%reg_35 = add i32 %reg_33, %reg_34
	%reg_36 = load i32, ptr %reg_9
	%reg_37 = add i32 %reg_35, %reg_36
	%reg_38 = load i32, ptr %reg_10
	%reg_39 = add i32 %reg_37, %reg_38
	%reg_40 = add i32 0, 0
	%reg_41 = icmp eq i32 %reg_39, %reg_40
	br i1 %reg_41, label %Block4, label %Block5
Block4:  ;If then at line 15
	%reg_42 = add i32 1, 0
	ret i32 %reg_42
	br label %Block5
Block5:  ;If end at line 15
	%reg_44 = add i32 0, 0
	store i32 %reg_44, ptr %reg_43
	%reg_45 = load i32, ptr %reg_6
	%reg_46 = icmp ne i32 %reg_45, 0
	br i1 %reg_46, label %Block6, label %Block7
Block6:  ;If then at line 18
	%reg_47 = load i32, ptr %reg_43
	%reg_48 = load i32, ptr %reg_6
	%reg_49 = load i32, ptr %reg_11
	%reg_50 = add i32 2, 0
	%reg_51 = call i32 @equal(i32 %reg_49, i32 %reg_50)
	%reg_52 = sub i32 %reg_48, %reg_51
	%reg_53 = load i32, ptr %reg_6
	%reg_54 = add i32 1, 0
	%reg_55 = sub i32 %reg_53, %reg_54
	%reg_56 = load i32, ptr %reg_7
	%reg_57 = load i32, ptr %reg_8
	%reg_58 = load i32, ptr %reg_9
	%reg_59 = load i32, ptr %reg_10
	%reg_60 = add i32 1, 0
	%reg_61 = call i32 @dfs(i32 %reg_55, i32 %reg_56, i32 %reg_57, i32 %reg_58, i32 %reg_59, i32 %reg_60)
	%reg_62 = mul i32 %reg_52, %reg_61
	%reg_63 = add i32 %reg_47, %reg_62
	%reg_64 = load i32, ptr @mod
	%reg_65 = srem i32 %reg_63, %reg_64
	store i32 %reg_65, ptr %reg_43
	br label %Block7
Block7:  ;If end at line 18
	%reg_66 = load i32, ptr %reg_7
	%reg_67 = icmp ne i32 %reg_66, 0
	br i1 %reg_67, label %Block8, label %Block9
Block8:  ;If then at line 19
	%reg_68 = load i32, ptr %reg_43
	%reg_69 = load i32, ptr %reg_7
	%reg_70 = load i32, ptr %reg_11
	%reg_71 = add i32 3, 0
	%reg_72 = call i32 @equal(i32 %reg_70, i32 %reg_71)
	%reg_73 = sub i32 %reg_69, %reg_72
	%reg_74 = load i32, ptr %reg_6
	%reg_75 = add i32 1, 0
	%reg_76 = add i32 %reg_74, %reg_75
	%reg_77 = load i32, ptr %reg_7
	%reg_78 = add i32 1, 0
	%reg_79 = sub i32 %reg_77, %reg_78
	%reg_80 = load i32, ptr %reg_8
	%reg_81 = load i32, ptr %reg_9
	%reg_82 = load i32, ptr %reg_10
	%reg_83 = add i32 2, 0
	%reg_84 = call i32 @dfs(i32 %reg_76, i32 %reg_79, i32 %reg_80, i32 %reg_81, i32 %reg_82, i32 %reg_83)
	%reg_85 = mul i32 %reg_73, %reg_84
	%reg_86 = add i32 %reg_68, %reg_85
	%reg_87 = load i32, ptr @mod
	%reg_88 = srem i32 %reg_86, %reg_87
	store i32 %reg_88, ptr %reg_43
	br label %Block9
Block9:  ;If end at line 19
	%reg_89 = load i32, ptr %reg_8
	%reg_90 = icmp ne i32 %reg_89, 0
	br i1 %reg_90, label %Block10, label %Block11
Block10:  ;If then at line 20
	%reg_91 = load i32, ptr %reg_43
	%reg_92 = load i32, ptr %reg_8
	%reg_93 = load i32, ptr %reg_11
	%reg_94 = add i32 4, 0
	%reg_95 = call i32 @equal(i32 %reg_93, i32 %reg_94)
	%reg_96 = sub i32 %reg_92, %reg_95
	%reg_97 = load i32, ptr %reg_6
	%reg_98 = load i32, ptr %reg_7
	%reg_99 = add i32 1, 0
	%reg_100 = add i32 %reg_98, %reg_99
	%reg_101 = load i32, ptr %reg_8
	%reg_102 = add i32 1, 0
	%reg_103 = sub i32 %reg_101, %reg_102
	%reg_104 = load i32, ptr %reg_9
	%reg_105 = load i32, ptr %reg_10
	%reg_106 = add i32 3, 0
	%reg_107 = call i32 @dfs(i32 %reg_97, i32 %reg_100, i32 %reg_103, i32 %reg_104, i32 %reg_105, i32 %reg_106)
	%reg_108 = mul i32 %reg_96, %reg_107
	%reg_109 = add i32 %reg_91, %reg_108
	%reg_110 = load i32, ptr @mod
	%reg_111 = srem i32 %reg_109, %reg_110
	store i32 %reg_111, ptr %reg_43
	br label %Block11
Block11:  ;If end at line 20
	%reg_112 = load i32, ptr %reg_9
	%reg_113 = icmp ne i32 %reg_112, 0
	br i1 %reg_113, label %Block12, label %Block13
Block12:  ;If then at line 21
	%reg_114 = load i32, ptr %reg_43
	%reg_115 = load i32, ptr %reg_9
	%reg_116 = load i32, ptr %reg_11
	%reg_117 = add i32 5, 0
	%reg_118 = call i32 @equal(i32 %reg_116, i32 %reg_117)
	%reg_119 = sub i32 %reg_115, %reg_118
	%reg_120 = load i32, ptr %reg_6
	%reg_121 = load i32, ptr %reg_7
	%reg_122 = load i32, ptr %reg_8
	%reg_123 = add i32 1, 0
	%reg_124 = add i32 %reg_122, %reg_123
	%reg_125 = load i32, ptr %reg_9
	%reg_126 = add i32 1, 0
	%reg_127 = sub i32 %reg_125, %reg_126
	%reg_128 = load i32, ptr %reg_10
	%reg_129 = add i32 4, 0
	%reg_130 = call i32 @dfs(i32 %reg_120, i32 %reg_121, i32 %reg_124, i32 %reg_127, i32 %reg_128, i32 %reg_129)
	%reg_131 = mul i32 %reg_119, %reg_130
	%reg_132 = add i32 %reg_114, %reg_131
	%reg_133 = load i32, ptr @mod
	%reg_134 = srem i32 %reg_132, %reg_133
	store i32 %reg_134, ptr %reg_43
	br label %Block13
Block13:  ;If end at line 21
	%reg_135 = load i32, ptr %reg_10
	%reg_136 = icmp ne i32 %reg_135, 0
	br i1 %reg_136, label %Block14, label %Block15
Block14:  ;If then at line 22
	%reg_137 = load i32, ptr %reg_43
	%reg_138 = load i32, ptr %reg_10
	%reg_139 = load i32, ptr %reg_6
	%reg_140 = load i32, ptr %reg_7
	%reg_141 = load i32, ptr %reg_8
	%reg_142 = load i32, ptr %reg_9
	%reg_143 = add i32 1, 0
	%reg_144 = add i32 %reg_142, %reg_143
	%reg_145 = load i32, ptr %reg_10
	%reg_146 = add i32 1, 0
	%reg_147 = sub i32 %reg_145, %reg_146
	%reg_148 = add i32 5, 0
	%reg_149 = call i32 @dfs(i32 %reg_139, i32 %reg_140, i32 %reg_141, i32 %reg_144, i32 %reg_147, i32 %reg_148)
	%reg_150 = mul i32 %reg_138, %reg_149
	%reg_151 = add i32 %reg_137, %reg_150
	%reg_152 = load i32, ptr @mod
	%reg_153 = srem i32 %reg_151, %reg_152
	store i32 %reg_153, ptr %reg_43
	br label %Block15
Block15:  ;If end at line 22
	%reg_154 = load i32, ptr %reg_6
	%reg_155 = load i32, ptr %reg_7
	%reg_156 = load i32, ptr %reg_8
	%reg_157 = load i32, ptr %reg_9
	%reg_158 = load i32, ptr %reg_10
	%reg_159 = load i32, ptr %reg_11
	%reg_160 = getelementptr [18 x [18 x [18 x [18 x [18 x [7 x i32]]]]]], ptr @dp, i32 0, i32 %reg_154, i32 %reg_155, i32 %reg_156, i32 %reg_157, i32 %reg_158, i32 %reg_159
	%reg_161 = load i32, ptr %reg_43
	%reg_162 = load i32, ptr @mod
	%reg_163 = srem i32 %reg_161, %reg_162
	store i32 %reg_163, ptr %reg_160
	%reg_164 = load i32, ptr %reg_6
	%reg_165 = load i32, ptr %reg_7
	%reg_166 = load i32, ptr %reg_8
	%reg_167 = load i32, ptr %reg_9
	%reg_168 = load i32, ptr %reg_10
	%reg_169 = load i32, ptr %reg_11
	%reg_170 = getelementptr [18 x [18 x [18 x [18 x [18 x [7 x i32]]]]]], ptr @dp, i32 0, i32 %reg_164, i32 %reg_165, i32 %reg_166, i32 %reg_167, i32 %reg_168, i32 %reg_169
	%reg_171 = load i32, ptr %reg_170
	ret i32 %reg_171
}

define i32 @main()
{
Block0:  ;Func define at line 29
	%reg_80 = alloca i32
	%reg_27 = alloca i32
	%reg_22 = alloca i32
	%reg_17 = alloca i32
	%reg_12 = alloca i32
	%reg_7 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 29
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 32
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @maxn
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 32
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	br label %Block5
Block4:  ;While end at line 32
	%reg_59 = add i32 0, 0
	store i32 %reg_59, ptr %reg_2
	br label %Block20
Block5:  ;While condition at line 34
	%reg_9 = load i32, ptr %reg_7
	%reg_10 = load i32, ptr @maxn
	%reg_11 = icmp slt i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block6, label %Block7
Block6:  ;While body at line 34
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	br label %Block8
Block7:  ;While end at line 34
	%reg_56 = load i32, ptr %reg_2
	%reg_57 = add i32 1, 0
	%reg_58 = add i32 %reg_56, %reg_57
	store i32 %reg_58, ptr %reg_2
	br label %Block2
Block8:  ;While condition at line 36
	%reg_14 = load i32, ptr %reg_12
	%reg_15 = load i32, ptr @maxn
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block9, label %Block10
Block9:  ;While body at line 36
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_17
	br label %Block11
Block10:  ;While end at line 36
	%reg_53 = load i32, ptr %reg_7
	%reg_54 = add i32 1, 0
	%reg_55 = add i32 %reg_53, %reg_54
	store i32 %reg_55, ptr %reg_7
	br label %Block5
Block11:  ;While condition at line 38
	%reg_19 = load i32, ptr %reg_17
	%reg_20 = load i32, ptr @maxn
	%reg_21 = icmp slt i32 %reg_19, %reg_20
	br i1 %reg_21, label %Block12, label %Block13
Block12:  ;While body at line 38
	%reg_23 = add i32 0, 0
	store i32 %reg_23, ptr %reg_22
	br label %Block14
Block13:  ;While end at line 38
	%reg_50 = load i32, ptr %reg_12
	%reg_51 = add i32 1, 0
	%reg_52 = add i32 %reg_50, %reg_51
	store i32 %reg_52, ptr %reg_12
	br label %Block8
Block14:  ;While condition at line 40
	%reg_24 = load i32, ptr %reg_22
	%reg_25 = load i32, ptr @maxn
	%reg_26 = icmp slt i32 %reg_24, %reg_25
	br i1 %reg_26, label %Block15, label %Block16
Block15:  ;While body at line 40
	%reg_28 = add i32 0, 0
	store i32 %reg_28, ptr %reg_27
	br label %Block17
Block16:  ;While end at line 40
	%reg_47 = load i32, ptr %reg_17
	%reg_48 = add i32 1, 0
	%reg_49 = add i32 %reg_47, %reg_48
	store i32 %reg_49, ptr %reg_17
	br label %Block11
Block17:  ;While condition at line 42
	%reg_29 = load i32, ptr %reg_27
	%reg_30 = add i32 7, 0
	%reg_31 = icmp slt i32 %reg_29, %reg_30
	br i1 %reg_31, label %Block18, label %Block19
Block18:  ;While body at line 42
	%reg_32 = load i32, ptr %reg_2
	%reg_33 = load i32, ptr %reg_7
	%reg_34 = load i32, ptr %reg_12
	%reg_35 = load i32, ptr %reg_17
	%reg_36 = load i32, ptr %reg_22
	%reg_37 = load i32, ptr %reg_27
	%reg_38 = getelementptr [18 x [18 x [18 x [18 x [18 x [7 x i32]]]]]], ptr @dp, i32 0, i32 %reg_32, i32 %reg_33, i32 %reg_34, i32 %reg_35, i32 %reg_36, i32 %reg_37
	%reg_39 = add i32 1, 0
	%reg_40 = sub i32 0, %reg_39
	store i32 %reg_40, ptr %reg_38
	%reg_41 = load i32, ptr %reg_27
	%reg_42 = add i32 1, 0
	%reg_43 = add i32 %reg_41, %reg_42
	store i32 %reg_43, ptr %reg_27
	br label %Block17
Block19:  ;While end at line 42
	%reg_44 = load i32, ptr %reg_22
	%reg_45 = add i32 1, 0
	%reg_46 = add i32 %reg_44, %reg_45
	store i32 %reg_46, ptr %reg_22
	br label %Block14
Block20:  ;While condition at line 58
	%reg_60 = load i32, ptr %reg_2
	%reg_61 = load i32, ptr %reg_0
	%reg_62 = icmp slt i32 %reg_60, %reg_61
	br i1 %reg_62, label %Block21, label %Block22
Block21:  ;While body at line 58
	%reg_63 = load i32, ptr %reg_2
	%reg_64 = getelementptr [200 x i32], ptr @list, i32 0, i32 %reg_63
	%reg_65 = call i32 @getint()
	store i32 %reg_65, ptr %reg_64
	%reg_66 = load i32, ptr %reg_2
	%reg_67 = getelementptr [200 x i32], ptr @list, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	%reg_69 = getelementptr [20 x i32], ptr @cns, i32 0, i32 %reg_68
	%reg_70 = load i32, ptr %reg_2
	%reg_71 = getelementptr [200 x i32], ptr @list, i32 0, i32 %reg_70
	%reg_72 = load i32, ptr %reg_71
	%reg_73 = getelementptr [20 x i32], ptr @cns, i32 0, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	%reg_75 = add i32 1, 0
	%reg_76 = add i32 %reg_74, %reg_75
	store i32 %reg_76, ptr %reg_69
	%reg_77 = load i32, ptr %reg_2
	%reg_78 = add i32 1, 0
	%reg_79 = add i32 %reg_77, %reg_78
	store i32 %reg_79, ptr %reg_2
	br label %Block20
Block22:  ;While end at line 58
	%reg_81 = add i32 1, 0
	%reg_82 = getelementptr [20 x i32], ptr @cns, i32 0, i32 %reg_81
	%reg_83 = load i32, ptr %reg_82
	%reg_84 = add i32 2, 0
	%reg_85 = getelementptr [20 x i32], ptr @cns, i32 0, i32 %reg_84
	%reg_86 = load i32, ptr %reg_85
	%reg_87 = add i32 3, 0
	%reg_88 = getelementptr [20 x i32], ptr @cns, i32 0, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	%reg_90 = add i32 4, 0
	%reg_91 = getelementptr [20 x i32], ptr @cns, i32 0, i32 %reg_90
	%reg_92 = load i32, ptr %reg_91
	%reg_93 = add i32 5, 0
	%reg_94 = getelementptr [20 x i32], ptr @cns, i32 0, i32 %reg_93
	%reg_95 = load i32, ptr %reg_94
	%reg_96 = add i32 0, 0
	%reg_97 = call i32 @dfs(i32 %reg_83, i32 %reg_86, i32 %reg_89, i32 %reg_92, i32 %reg_95, i32 %reg_96)
	store i32 %reg_97, ptr %reg_80
	%reg_98 = load i32, ptr %reg_80
	call void @putint(i32 %reg_98)
	%reg_99 = load i32, ptr %reg_80
	ret i32 %reg_99
}
