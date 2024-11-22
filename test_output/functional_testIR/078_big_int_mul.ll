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

@len = global i32 20

define i32 @main()
{
Block0:  ;Func define at line 3
	%reg_98 = alloca [40 x i32]
	%reg_97 = alloca [25 x i32]
	%reg_96 = alloca [25 x i32]
	%reg_94 = alloca i32
	%reg_92 = alloca i32
	%reg_51 = alloca [20 x i32]
	%reg_10 = alloca [20 x i32]
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 3
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	call void @llvm.memset.p0.i32(ptr %reg_10, i8 0, i32 80, i1 1)
	%reg_11 = add i32 1, 0
	%reg_12 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 0
	store i32 %reg_11, ptr %reg_12
	%reg_13 = add i32 2, 0
	%reg_14 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 1
	store i32 %reg_13, ptr %reg_14
	%reg_15 = add i32 3, 0
	%reg_16 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 2
	store i32 %reg_15, ptr %reg_16
	%reg_17 = add i32 4, 0
	%reg_18 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 3
	store i32 %reg_17, ptr %reg_18
	%reg_19 = add i32 5, 0
	%reg_20 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 4
	store i32 %reg_19, ptr %reg_20
	%reg_21 = add i32 6, 0
	%reg_22 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 5
	store i32 %reg_21, ptr %reg_22
	%reg_23 = add i32 7, 0
	%reg_24 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 6
	store i32 %reg_23, ptr %reg_24
	%reg_25 = add i32 8, 0
	%reg_26 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 7
	store i32 %reg_25, ptr %reg_26
	%reg_27 = add i32 9, 0
	%reg_28 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 8
	store i32 %reg_27, ptr %reg_28
	%reg_29 = add i32 0, 0
	%reg_30 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 9
	store i32 %reg_29, ptr %reg_30
	%reg_31 = add i32 1, 0
	%reg_32 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 10
	store i32 %reg_31, ptr %reg_32
	%reg_33 = add i32 2, 0
	%reg_34 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 11
	store i32 %reg_33, ptr %reg_34
	%reg_35 = add i32 3, 0
	%reg_36 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 12
	store i32 %reg_35, ptr %reg_36
	%reg_37 = add i32 4, 0
	%reg_38 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 13
	store i32 %reg_37, ptr %reg_38
	%reg_39 = add i32 5, 0
	%reg_40 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 14
	store i32 %reg_39, ptr %reg_40
	%reg_41 = add i32 6, 0
	%reg_42 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 15
	store i32 %reg_41, ptr %reg_42
	%reg_43 = add i32 7, 0
	%reg_44 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 16
	store i32 %reg_43, ptr %reg_44
	%reg_45 = add i32 8, 0
	%reg_46 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 17
	store i32 %reg_45, ptr %reg_46
	%reg_47 = add i32 9, 0
	%reg_48 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 18
	store i32 %reg_47, ptr %reg_48
	%reg_49 = add i32 0, 0
	%reg_50 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 19
	store i32 %reg_49, ptr %reg_50
	call void @llvm.memset.p0.i32(ptr %reg_51, i8 0, i32 80, i1 1)
	%reg_52 = add i32 2, 0
	%reg_53 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 0
	store i32 %reg_52, ptr %reg_53
	%reg_54 = add i32 3, 0
	%reg_55 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 1
	store i32 %reg_54, ptr %reg_55
	%reg_56 = add i32 4, 0
	%reg_57 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 2
	store i32 %reg_56, ptr %reg_57
	%reg_58 = add i32 2, 0
	%reg_59 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 3
	store i32 %reg_58, ptr %reg_59
	%reg_60 = add i32 5, 0
	%reg_61 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 4
	store i32 %reg_60, ptr %reg_61
	%reg_62 = add i32 7, 0
	%reg_63 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 5
	store i32 %reg_62, ptr %reg_63
	%reg_64 = add i32 9, 0
	%reg_65 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 6
	store i32 %reg_64, ptr %reg_65
	%reg_66 = add i32 9, 0
	%reg_67 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 7
	store i32 %reg_66, ptr %reg_67
	%reg_68 = add i32 0, 0
	%reg_69 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 8
	store i32 %reg_68, ptr %reg_69
	%reg_70 = add i32 1, 0
	%reg_71 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 9
	store i32 %reg_70, ptr %reg_71
	%reg_72 = add i32 9, 0
	%reg_73 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 10
	store i32 %reg_72, ptr %reg_73
	%reg_74 = add i32 8, 0
	%reg_75 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 11
	store i32 %reg_74, ptr %reg_75
	%reg_76 = add i32 7, 0
	%reg_77 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 12
	store i32 %reg_76, ptr %reg_77
	%reg_78 = add i32 6, 0
	%reg_79 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 13
	store i32 %reg_78, ptr %reg_79
	%reg_80 = add i32 4, 0
	%reg_81 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 14
	store i32 %reg_80, ptr %reg_81
	%reg_82 = add i32 3, 0
	%reg_83 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 15
	store i32 %reg_82, ptr %reg_83
	%reg_84 = add i32 2, 0
	%reg_85 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 16
	store i32 %reg_84, ptr %reg_85
	%reg_86 = add i32 1, 0
	%reg_87 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 17
	store i32 %reg_86, ptr %reg_87
	%reg_88 = add i32 2, 0
	%reg_89 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 18
	store i32 %reg_88, ptr %reg_89
	%reg_90 = add i32 2, 0
	%reg_91 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 19
	store i32 %reg_90, ptr %reg_91
	%reg_93 = load i32, ptr @len
	store i32 %reg_93, ptr %reg_92
	%reg_95 = load i32, ptr @len
	store i32 %reg_95, ptr %reg_94
	call void @llvm.memset.p0.i32(ptr %reg_96, i8 0, i32 100, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_97, i8 0, i32 100, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_98, i8 0, i32 160, i1 1)
	%reg_99 = add i32 0, 0
	store i32 %reg_99, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 15
	%reg_100 = load i32, ptr %reg_0
	%reg_101 = load i32, ptr %reg_92
	%reg_102 = icmp slt i32 %reg_100, %reg_101
	br i1 %reg_102, label %Block3, label %Block4
Block3:  ;While body at line 15
	%reg_103 = load i32, ptr %reg_0
	%reg_104 = getelementptr [25 x i32], ptr %reg_96, i32 0, i32 %reg_103
	%reg_105 = load i32, ptr %reg_0
	%reg_106 = getelementptr [20 x i32], ptr %reg_10, i32 0, i32 %reg_105
	%reg_107 = load i32, ptr %reg_106
	store i32 %reg_107, ptr %reg_104
	%reg_108 = load i32, ptr %reg_0
	%reg_109 = add i32 1, 0
	%reg_110 = add i32 %reg_108, %reg_109
	store i32 %reg_110, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 15
	%reg_111 = add i32 0, 0
	store i32 %reg_111, ptr %reg_0
	br label %Block5
Block5:  ;While condition at line 21
	%reg_112 = load i32, ptr %reg_0
	%reg_113 = load i32, ptr %reg_94
	%reg_114 = icmp slt i32 %reg_112, %reg_113
	br i1 %reg_114, label %Block6, label %Block7
Block6:  ;While body at line 21
	%reg_115 = load i32, ptr %reg_0
	%reg_116 = getelementptr [25 x i32], ptr %reg_97, i32 0, i32 %reg_115
	%reg_117 = load i32, ptr %reg_0
	%reg_118 = getelementptr [20 x i32], ptr %reg_51, i32 0, i32 %reg_117
	%reg_119 = load i32, ptr %reg_118
	store i32 %reg_119, ptr %reg_116
	%reg_120 = load i32, ptr %reg_0
	%reg_121 = add i32 1, 0
	%reg_122 = add i32 %reg_120, %reg_121
	store i32 %reg_122, ptr %reg_0
	br label %Block5
Block7:  ;While end at line 21
	%reg_123 = load i32, ptr %reg_92
	%reg_124 = load i32, ptr %reg_94
	%reg_125 = add i32 %reg_123, %reg_124
	%reg_126 = add i32 1, 0
	%reg_127 = sub i32 %reg_125, %reg_126
	store i32 %reg_127, ptr %reg_6
	%reg_128 = add i32 0, 0
	store i32 %reg_128, ptr %reg_0
	br label %Block8
Block8:  ;While condition at line 29
	%reg_129 = load i32, ptr %reg_0
	%reg_130 = load i32, ptr %reg_6
	%reg_131 = icmp sle i32 %reg_129, %reg_130
	br i1 %reg_131, label %Block9, label %Block10
Block9:  ;While body at line 29
	%reg_132 = load i32, ptr %reg_0
	%reg_133 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_132
	%reg_134 = add i32 0, 0
	store i32 %reg_134, ptr %reg_133
	%reg_135 = load i32, ptr %reg_0
	%reg_136 = add i32 1, 0
	%reg_137 = add i32 %reg_135, %reg_136
	store i32 %reg_137, ptr %reg_0
	br label %Block8
Block10:  ;While end at line 29
	%reg_138 = add i32 0, 0
	store i32 %reg_138, ptr %reg_8
	%reg_139 = load i32, ptr %reg_94
	%reg_140 = add i32 1, 0
	%reg_141 = sub i32 %reg_139, %reg_140
	store i32 %reg_141, ptr %reg_0
	br label %Block11
Block11:  ;While condition at line 37
	%reg_142 = load i32, ptr %reg_0
	%reg_143 = add i32 1, 0
	%reg_144 = sub i32 0, %reg_143
	%reg_145 = icmp sgt i32 %reg_142, %reg_144
	br i1 %reg_145, label %Block12, label %Block13
Block12:  ;While body at line 37
	%reg_146 = load i32, ptr %reg_0
	%reg_147 = getelementptr [25 x i32], ptr %reg_97, i32 0, i32 %reg_146
	%reg_148 = load i32, ptr %reg_147
	store i32 %reg_148, ptr %reg_4
	%reg_149 = load i32, ptr %reg_92
	%reg_150 = add i32 1, 0
	%reg_151 = sub i32 %reg_149, %reg_150
	store i32 %reg_151, ptr %reg_2
	br label %Block14
Block13:  ;While end at line 37
	%reg_201 = add i32 0, 0
	%reg_202 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_201
	%reg_203 = load i32, ptr %reg_202
	%reg_204 = add i32 0, 0
	%reg_205 = icmp ne i32 %reg_203, %reg_204
	br i1 %reg_205, label %Block20, label %Block21
Block14:  ;While condition at line 40
	%reg_152 = load i32, ptr %reg_2
	%reg_153 = add i32 1, 0
	%reg_154 = sub i32 0, %reg_153
	%reg_155 = icmp sgt i32 %reg_152, %reg_154
	br i1 %reg_155, label %Block15, label %Block16
Block15:  ;While body at line 40
	%reg_156 = load i32, ptr %reg_6
	%reg_157 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_156
	%reg_158 = load i32, ptr %reg_157
	%reg_159 = load i32, ptr %reg_4
	%reg_160 = load i32, ptr %reg_2
	%reg_161 = getelementptr [25 x i32], ptr %reg_96, i32 0, i32 %reg_160
	%reg_162 = load i32, ptr %reg_161
	%reg_163 = mul i32 %reg_159, %reg_162
	%reg_164 = add i32 %reg_158, %reg_163
	store i32 %reg_164, ptr %reg_8
	%reg_165 = load i32, ptr %reg_8
	%reg_166 = add i32 10, 0
	%reg_167 = icmp sge i32 %reg_165, %reg_166
	br i1 %reg_167, label %Block17, label %Block19
Block16:  ;While end at line 40
	%reg_193 = load i32, ptr %reg_6
	%reg_194 = load i32, ptr %reg_92
	%reg_195 = add i32 %reg_193, %reg_194
	%reg_196 = add i32 1, 0
	%reg_197 = sub i32 %reg_195, %reg_196
	store i32 %reg_197, ptr %reg_6
	%reg_198 = load i32, ptr %reg_0
	%reg_199 = add i32 1, 0
	%reg_200 = sub i32 %reg_198, %reg_199
	store i32 %reg_200, ptr %reg_0
	br label %Block11
Block17:  ;If then at line 42
	%reg_168 = load i32, ptr %reg_6
	%reg_169 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_168
	%reg_170 = load i32, ptr %reg_8
	store i32 %reg_170, ptr %reg_169
	%reg_171 = load i32, ptr %reg_6
	%reg_172 = add i32 1, 0
	%reg_173 = sub i32 %reg_171, %reg_172
	%reg_174 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_173
	%reg_175 = load i32, ptr %reg_6
	%reg_176 = add i32 1, 0
	%reg_177 = sub i32 %reg_175, %reg_176
	%reg_178 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_177
	%reg_179 = load i32, ptr %reg_178
	%reg_180 = load i32, ptr %reg_8
	%reg_181 = add i32 10, 0
	%reg_182 = sdiv i32 %reg_180, %reg_181
	%reg_183 = add i32 %reg_179, %reg_182
	store i32 %reg_183, ptr %reg_174
	br label %Block18
Block18:  ;If end at line 42
	%reg_187 = load i32, ptr %reg_2
	%reg_188 = add i32 1, 0
	%reg_189 = sub i32 %reg_187, %reg_188
	store i32 %reg_189, ptr %reg_2
	%reg_190 = load i32, ptr %reg_6
	%reg_191 = add i32 1, 0
	%reg_192 = sub i32 %reg_190, %reg_191
	store i32 %reg_192, ptr %reg_6
	br label %Block14
Block19:  ;If else at line 42
	%reg_184 = load i32, ptr %reg_6
	%reg_185 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_184
	%reg_186 = load i32, ptr %reg_8
	store i32 %reg_186, ptr %reg_185
	br label %Block18
Block20:  ;If then at line 55
	%reg_206 = add i32 0, 0
	%reg_207 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_206
	%reg_208 = load i32, ptr %reg_207
	call void @putint(i32 %reg_208)
	br label %Block21
Block21:  ;If end at line 55
	%reg_209 = add i32 1, 0
	store i32 %reg_209, ptr %reg_0
	br label %Block22
Block22:  ;While condition at line 59
	%reg_210 = load i32, ptr %reg_0
	%reg_211 = load i32, ptr %reg_92
	%reg_212 = load i32, ptr %reg_94
	%reg_213 = add i32 %reg_211, %reg_212
	%reg_214 = add i32 1, 0
	%reg_215 = sub i32 %reg_213, %reg_214
	%reg_216 = icmp sle i32 %reg_210, %reg_215
	br i1 %reg_216, label %Block23, label %Block24
Block23:  ;While body at line 59
	%reg_217 = load i32, ptr %reg_0
	%reg_218 = getelementptr [40 x i32], ptr %reg_98, i32 0, i32 %reg_217
	%reg_219 = load i32, ptr %reg_218
	call void @putint(i32 %reg_219)
	%reg_220 = load i32, ptr %reg_0
	%reg_221 = add i32 1, 0
	%reg_222 = add i32 %reg_220, %reg_221
	store i32 %reg_222, ptr %reg_0
	br label %Block22
Block24:  ;While end at line 59
	%reg_223 = add i32 0, 0
	ret i32 %reg_223
}
