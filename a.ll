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
declare void @_sysy_starttime(i32)
declare void @_sysy_stoptime(i32)
declare void @llvm.memset.p0.i32(ptr, i8, i32, i1)

@base = global i32 16
@a = global [30000010 x i32] zeroinitializer
@ans = global i32 zeroinitializer

define i32 @getMaxNum(i32 %reg_0, ptr %reg_1)
{
Block0:  ;Func define at line 3
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 3
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	br label %Block2
Block2:  ;While condition at line 6
	%reg_7 = load i32, ptr %reg_5
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = icmp slt i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block3, label %Block4
Block3:  ;While body at line 6
	%reg_10 = load i32, ptr %reg_5
	%reg_11 = getelementptr i32, ptr %reg_1, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	%reg_13 = load i32, ptr %reg_3
	%reg_14 = icmp sgt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block5, label %Block6
Block4:  ;While end at line 6
	%reg_21 = load i32, ptr %reg_3
	ret i32 %reg_21
Block5:  ;If then at line 7
	%reg_15 = load i32, ptr %reg_5
	%reg_16 = getelementptr i32, ptr %reg_1, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	store i32 %reg_17, ptr %reg_3
	br label %Block6
Block6:  ;If end at line 7
	%reg_18 = load i32, ptr %reg_5
	%reg_19 = add i32 1, 0
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_5
	br label %Block2
}

define i32 @getNumPos(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 13
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 13
	%reg_5 = add i32 1, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	br label %Block2
Block2:  ;While condition at line 16
	%reg_8 = load i32, ptr %reg_6
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = icmp slt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 16
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = load i32, ptr @base
	%reg_13 = sdiv i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_2
	%reg_14 = load i32, ptr %reg_6
	%reg_15 = add i32 1, 0
	%reg_16 = add i32 %reg_14, %reg_15
	store i32 %reg_16, ptr %reg_6
	br label %Block2
Block4:  ;While end at line 16
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = load i32, ptr @base
	%reg_19 = srem i32 %reg_17, %reg_18
	ret i32 %reg_19
}

define void @radixSort(i32 %reg_0, ptr %reg_1, i32 %reg_2, i32 %reg_3)
{
Block0:  ;Func define at line 23
	%reg_139 = alloca i32
	%reg_97 = alloca i32
	%reg_86 = alloca i32
	%reg_19 = alloca i32
	%reg_9 = alloca [16 x i32]
	%reg_8 = alloca [16 x i32]
	%reg_7 = alloca [16 x i32]
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	store i32 %reg_0, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	store i32 %reg_3, ptr %reg_6
	br label %Block1
Block1:  ;Func body at line 23
	call void @llvm.memset.p0.i32(ptr %reg_7, i8 0, i32 64, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_8, i8 0, i32 64, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_9, i8 0, i32 64, i1 1)
	%reg_10 = load i32, ptr %reg_4
	%reg_11 = add i32 1, 0
	%reg_12 = sub i32 0, %reg_11
	%reg_13 = icmp eq i32 %reg_10, %reg_12
	br i1 %reg_13, label %Block2, label %Block4
Block2:  ;If then at line 28
	ret void
	br label %Block3
Block3:  ;If end at line 28
	%reg_20 = load i32, ptr %reg_5
	store i32 %reg_20, ptr %reg_19
	br label %Block5
Block4:  ;Or opertor at line 28
	%reg_14 = load i32, ptr %reg_5
	%reg_15 = add i32 1, 0
	%reg_16 = add i32 %reg_14, %reg_15
	%reg_17 = load i32, ptr %reg_6
	%reg_18 = icmp sge i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block2, label %Block3
Block5:  ;While condition at line 33
	%reg_21 = load i32, ptr %reg_19
	%reg_22 = load i32, ptr %reg_6
	%reg_23 = icmp slt i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block6, label %Block7
Block6:  ;While body at line 33
	%reg_24 = load i32, ptr %reg_19
	%reg_25 = getelementptr i32, ptr %reg_1, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = load i32, ptr %reg_4
	%reg_28 = call i32 @getNumPos(i32 %reg_26, i32 %reg_27)
	%reg_29 = getelementptr [16 x i32], ptr %reg_9, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_19
	%reg_31 = getelementptr i32, ptr %reg_1, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	%reg_33 = load i32, ptr %reg_4
	%reg_34 = call i32 @getNumPos(i32 %reg_32, i32 %reg_33)
	%reg_35 = getelementptr [16 x i32], ptr %reg_9, i32 0, i32 %reg_34
	%reg_36 = load i32, ptr %reg_35
	%reg_37 = add i32 1, 0
	%reg_38 = add i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_29
	%reg_39 = load i32, ptr %reg_19
	%reg_40 = add i32 1, 0
	%reg_41 = add i32 %reg_39, %reg_40
	store i32 %reg_41, ptr %reg_19
	br label %Block5
Block7:  ;While end at line 33
	%reg_42 = add i32 0, 0
	%reg_43 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_42
	%reg_44 = load i32, ptr %reg_5
	store i32 %reg_44, ptr %reg_43
	%reg_45 = add i32 0, 0
	%reg_46 = getelementptr [16 x i32], ptr %reg_8, i32 0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_5
	%reg_48 = add i32 0, 0
	%reg_49 = getelementptr [16 x i32], ptr %reg_9, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	%reg_51 = add i32 %reg_47, %reg_50
	store i32 %reg_51, ptr %reg_46
	%reg_52 = add i32 1, 0
	store i32 %reg_52, ptr %reg_19
	br label %Block8
Block8:  ;While condition at line 42
	%reg_53 = load i32, ptr %reg_19
	%reg_54 = load i32, ptr @base
	%reg_55 = icmp slt i32 %reg_53, %reg_54
	br i1 %reg_55, label %Block9, label %Block10
Block9:  ;While body at line 42
	%reg_56 = load i32, ptr %reg_19
	%reg_57 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_56
	%reg_58 = load i32, ptr %reg_19
	%reg_59 = add i32 1, 0
	%reg_60 = sub i32 %reg_58, %reg_59
	%reg_61 = getelementptr [16 x i32], ptr %reg_8, i32 0, i32 %reg_60
	%reg_62 = load i32, ptr %reg_61
	store i32 %reg_62, ptr %reg_57
	%reg_63 = load i32, ptr %reg_19
	%reg_64 = getelementptr [16 x i32], ptr %reg_8, i32 0, i32 %reg_63
	%reg_65 = load i32, ptr %reg_19
	%reg_66 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_65
	%reg_67 = load i32, ptr %reg_66
	%reg_68 = load i32, ptr %reg_19
	%reg_69 = getelementptr [16 x i32], ptr %reg_9, i32 0, i32 %reg_68
	%reg_70 = load i32, ptr %reg_69
	%reg_71 = add i32 %reg_67, %reg_70
	store i32 %reg_71, ptr %reg_64
	%reg_72 = load i32, ptr %reg_19
	%reg_73 = add i32 1, 0
	%reg_74 = add i32 %reg_72, %reg_73
	store i32 %reg_74, ptr %reg_19
	br label %Block8
Block10:  ;While end at line 42
	%reg_75 = add i32 0, 0
	store i32 %reg_75, ptr %reg_19
	br label %Block11
Block11:  ;While condition at line 48
	%reg_76 = load i32, ptr %reg_19
	%reg_77 = load i32, ptr @base
	%reg_78 = icmp slt i32 %reg_76, %reg_77
	br i1 %reg_78, label %Block12, label %Block13
Block12:  ;While body at line 48
	br label %Block14
Block13:  ;While end at line 48
	%reg_140 = load i32, ptr %reg_5
	store i32 %reg_140, ptr %reg_139
	%reg_141 = add i32 0, 0
	%reg_142 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_141
	%reg_143 = load i32, ptr %reg_5
	store i32 %reg_143, ptr %reg_142
	%reg_144 = add i32 0, 0
	%reg_145 = getelementptr [16 x i32], ptr %reg_8, i32 0, i32 %reg_144
	%reg_146 = load i32, ptr %reg_5
	%reg_147 = add i32 0, 0
	%reg_148 = getelementptr [16 x i32], ptr %reg_9, i32 0, i32 %reg_147
	%reg_149 = load i32, ptr %reg_148
	%reg_150 = add i32 %reg_146, %reg_149
	store i32 %reg_150, ptr %reg_145
	%reg_151 = add i32 0, 0
	store i32 %reg_151, ptr %reg_139
	br label %Block20
Block14:  ;While condition at line 49
	%reg_79 = load i32, ptr %reg_19
	%reg_80 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_79
	%reg_81 = load i32, ptr %reg_80
	%reg_82 = load i32, ptr %reg_19
	%reg_83 = getelementptr [16 x i32], ptr %reg_8, i32 0, i32 %reg_82
	%reg_84 = load i32, ptr %reg_83
	%reg_85 = icmp slt i32 %reg_81, %reg_84
	br i1 %reg_85, label %Block15, label %Block16
Block15:  ;While body at line 49
	%reg_87 = load i32, ptr %reg_19
	%reg_88 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	%reg_90 = getelementptr i32, ptr %reg_1, i32 %reg_89
	%reg_91 = load i32, ptr %reg_90
	store i32 %reg_91, ptr %reg_86
	br label %Block17
Block16:  ;While end at line 49
	%reg_136 = load i32, ptr %reg_19
	%reg_137 = add i32 1, 0
	%reg_138 = add i32 %reg_136, %reg_137
	store i32 %reg_138, ptr %reg_19
	br label %Block11
Block17:  ;While condition at line 51
	%reg_92 = load i32, ptr %reg_86
	%reg_93 = load i32, ptr %reg_4
	%reg_94 = call i32 @getNumPos(i32 %reg_92, i32 %reg_93)
	%reg_95 = load i32, ptr %reg_19
	%reg_96 = icmp ne i32 %reg_94, %reg_95
	br i1 %reg_96, label %Block18, label %Block19
Block18:  ;While body at line 51
	%reg_98 = load i32, ptr %reg_86
	store i32 %reg_98, ptr %reg_97
	%reg_99 = load i32, ptr %reg_97
	%reg_100 = load i32, ptr %reg_4
	%reg_101 = call i32 @getNumPos(i32 %reg_99, i32 %reg_100)
	%reg_102 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_101
	%reg_103 = load i32, ptr %reg_102
	%reg_104 = getelementptr i32, ptr %reg_1, i32 %reg_103
	%reg_105 = load i32, ptr %reg_104
	store i32 %reg_105, ptr %reg_86
	%reg_106 = load i32, ptr %reg_97
	%reg_107 = load i32, ptr %reg_4
	%reg_108 = call i32 @getNumPos(i32 %reg_106, i32 %reg_107)
	%reg_109 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_108
	%reg_110 = load i32, ptr %reg_109
	%reg_111 = getelementptr i32, ptr %reg_1, i32 %reg_110
	%reg_112 = load i32, ptr %reg_97
	store i32 %reg_112, ptr %reg_111
	%reg_113 = load i32, ptr %reg_97
	%reg_114 = load i32, ptr %reg_4
	%reg_115 = call i32 @getNumPos(i32 %reg_113, i32 %reg_114)
	%reg_116 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_115
	%reg_117 = load i32, ptr %reg_97
	%reg_118 = load i32, ptr %reg_4
	%reg_119 = call i32 @getNumPos(i32 %reg_117, i32 %reg_118)
	%reg_120 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_119
	%reg_121 = load i32, ptr %reg_120
	%reg_122 = add i32 1, 0
	%reg_123 = add i32 %reg_121, %reg_122
	store i32 %reg_123, ptr %reg_116
	br label %Block17
Block19:  ;While end at line 51
	%reg_124 = load i32, ptr %reg_19
	%reg_125 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_124
	%reg_126 = load i32, ptr %reg_125
	%reg_127 = getelementptr i32, ptr %reg_1, i32 %reg_126
	%reg_128 = load i32, ptr %reg_86
	store i32 %reg_128, ptr %reg_127
	%reg_129 = load i32, ptr %reg_19
	%reg_130 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_129
	%reg_131 = load i32, ptr %reg_19
	%reg_132 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_131
	%reg_133 = load i32, ptr %reg_132
	%reg_134 = add i32 1, 0
	%reg_135 = add i32 %reg_133, %reg_134
	store i32 %reg_135, ptr %reg_130
	br label %Block14
Block20:  ;While condition at line 71
	%reg_152 = load i32, ptr %reg_139
	%reg_153 = load i32, ptr @base
	%reg_154 = icmp slt i32 %reg_152, %reg_153
	br i1 %reg_154, label %Block21, label %Block22
Block21:  ;While body at line 71
	%reg_155 = load i32, ptr %reg_139
	%reg_156 = add i32 0, 0
	%reg_157 = icmp sgt i32 %reg_155, %reg_156
	br i1 %reg_157, label %Block23, label %Block24
Block22:  ;While end at line 71
	ret void
Block23:  ;If then at line 72
	%reg_158 = load i32, ptr %reg_139
	%reg_159 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_158
	%reg_160 = load i32, ptr %reg_139
	%reg_161 = add i32 1, 0
	%reg_162 = sub i32 %reg_160, %reg_161
	%reg_163 = getelementptr [16 x i32], ptr %reg_8, i32 0, i32 %reg_162
	%reg_164 = load i32, ptr %reg_163
	store i32 %reg_164, ptr %reg_159
	%reg_165 = load i32, ptr %reg_139
	%reg_166 = getelementptr [16 x i32], ptr %reg_8, i32 0, i32 %reg_165
	%reg_167 = load i32, ptr %reg_139
	%reg_168 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_167
	%reg_169 = load i32, ptr %reg_168
	%reg_170 = load i32, ptr %reg_139
	%reg_171 = getelementptr [16 x i32], ptr %reg_9, i32 0, i32 %reg_170
	%reg_172 = load i32, ptr %reg_171
	%reg_173 = add i32 %reg_169, %reg_172
	store i32 %reg_173, ptr %reg_166
	br label %Block24
Block24:  ;If end at line 72
	%reg_174 = load i32, ptr %reg_4
	%reg_175 = add i32 1, 0
	%reg_176 = sub i32 %reg_174, %reg_175
	%reg_177 = getelementptr i32, ptr %reg_1
	%reg_178 = load i32, ptr %reg_139
	%reg_179 = getelementptr [16 x i32], ptr %reg_7, i32 0, i32 %reg_178
	%reg_180 = load i32, ptr %reg_179
	%reg_181 = load i32, ptr %reg_139
	%reg_182 = getelementptr [16 x i32], ptr %reg_8, i32 0, i32 %reg_181
	%reg_183 = load i32, ptr %reg_182
	call void @radixSort(i32 %reg_176, ptr %reg_177, i32 %reg_180, i32 %reg_183)
	%reg_184 = load i32, ptr %reg_139
	%reg_185 = add i32 1, 0
	%reg_186 = add i32 %reg_184, %reg_185
	store i32 %reg_186, ptr %reg_139
	br label %Block20
}

define i32 @main()
{
Block0:  ;Func define at line 87
	%reg_8 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 87
	%reg_1 = getelementptr [30000010 x i32], ptr @a, i32 0
	%reg_2 = call i32 @getarray(ptr %reg_1)
	store i32 %reg_2, ptr %reg_0
	%reg_3 = add i32 90, 0
	call void @_sysy_starttime(i32 %reg_3)
	%reg_4 = add i32 8, 0
	%reg_5 = getelementptr [30000010 x i32], ptr @a, i32 0
	%reg_6 = add i32 0, 0
	%reg_7 = load i32, ptr %reg_0
	call void @radixSort(i32 %reg_4, ptr %reg_5, i32 %reg_6, i32 %reg_7)
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	br label %Block2
Block2:  ;While condition at line 95
	%reg_10 = load i32, ptr %reg_8
	%reg_11 = load i32, ptr %reg_0
	%reg_12 = icmp slt i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block3, label %Block4
Block3:  ;While body at line 95
	%reg_13 = load i32, ptr @ans
	%reg_14 = load i32, ptr %reg_8
	%reg_15 = load i32, ptr %reg_8
	%reg_16 = getelementptr [30000010 x i32], ptr @a, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	%reg_18 = add i32 2, 0
	%reg_19 = load i32, ptr %reg_8
	%reg_20 = add i32 %reg_18, %reg_19
	%reg_21 = srem i32 %reg_17, %reg_20
	%reg_22 = mul i32 %reg_14, %reg_21
	%reg_23 = add i32 %reg_13, %reg_22
	store i32 %reg_23, ptr @ans
	%reg_24 = load i32, ptr %reg_8
	%reg_25 = add i32 1, 0
	%reg_26 = add i32 %reg_24, %reg_25
	store i32 %reg_26, ptr %reg_8
	br label %Block2
Block4:  ;While end at line 95
	%reg_27 = load i32, ptr @ans
	%reg_28 = add i32 0, 0
	%reg_29 = icmp slt i32 %reg_27, %reg_28
	br i1 %reg_29, label %Block5, label %Block6
Block5:  ;If then at line 100
	%reg_30 = load i32, ptr @ans
	%reg_31 = sub i32 0, %reg_30
	store i32 %reg_31, ptr @ans
	br label %Block6
Block6:  ;If end at line 100
	%reg_32 = add i32 102, 0
	call void @_sysy_stoptime(i32 %reg_32)
	%reg_33 = load i32, ptr @ans
	call void @putint(i32 %reg_33)
	%reg_34 = add i32 10, 0
	call void @putch(i32 %reg_34)
	%reg_35 = add i32 0, 0
	ret i32 %reg_35
}
