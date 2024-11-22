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

@M = global i32 zeroinitializer
@L = global i32 zeroinitializer
@N = global i32 zeroinitializer

define i32 @mul(ptr %reg_0, ptr %reg_1, ptr %reg_2, ptr %reg_3, ptr %reg_4, ptr %reg_5, ptr %reg_6, ptr %reg_7, ptr %reg_8)
{
Block0:  ;Func define at line 6
	%reg_9 = alloca i32
	br label %Block1
Block1:  ;Func body at line 6
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_9
	%reg_12 = add i32 0, 0
	%reg_13 = getelementptr float, ptr %reg_6, i32 %reg_12
	%reg_14 = add i32 0, 0
	%reg_15 = getelementptr float, ptr %reg_0, i32 %reg_14
	%reg_16 = load float, ptr %reg_15
	%reg_17 = add i32 0, 0
	%reg_18 = getelementptr float, ptr %reg_3, i32 %reg_17
	%reg_19 = load float, ptr %reg_18
	%reg_20 = fmul float %reg_16, %reg_19
	%reg_21 = add i32 1, 0
	%reg_22 = getelementptr float, ptr %reg_0, i32 %reg_21
	%reg_23 = load float, ptr %reg_22
	%reg_24 = add i32 0, 0
	%reg_25 = getelementptr float, ptr %reg_4, i32 %reg_24
	%reg_26 = load float, ptr %reg_25
	%reg_27 = fmul float %reg_23, %reg_26
	%reg_28 = fadd float %reg_20, %reg_27
	%reg_29 = add i32 2, 0
	%reg_30 = getelementptr float, ptr %reg_0, i32 %reg_29
	%reg_31 = load float, ptr %reg_30
	%reg_32 = add i32 0, 0
	%reg_33 = getelementptr float, ptr %reg_5, i32 %reg_32
	%reg_34 = load float, ptr %reg_33
	%reg_35 = fmul float %reg_31, %reg_34
	%reg_36 = fadd float %reg_28, %reg_35
	store float %reg_36, ptr %reg_13
	%reg_37 = add i32 1, 0
	%reg_38 = getelementptr float, ptr %reg_6, i32 %reg_37
	%reg_39 = add i32 0, 0
	%reg_40 = getelementptr float, ptr %reg_0, i32 %reg_39
	%reg_41 = load float, ptr %reg_40
	%reg_42 = add i32 1, 0
	%reg_43 = getelementptr float, ptr %reg_3, i32 %reg_42
	%reg_44 = load float, ptr %reg_43
	%reg_45 = fmul float %reg_41, %reg_44
	%reg_46 = add i32 1, 0
	%reg_47 = getelementptr float, ptr %reg_0, i32 %reg_46
	%reg_48 = load float, ptr %reg_47
	%reg_49 = add i32 1, 0
	%reg_50 = getelementptr float, ptr %reg_4, i32 %reg_49
	%reg_51 = load float, ptr %reg_50
	%reg_52 = fmul float %reg_48, %reg_51
	%reg_53 = fadd float %reg_45, %reg_52
	%reg_54 = add i32 2, 0
	%reg_55 = getelementptr float, ptr %reg_0, i32 %reg_54
	%reg_56 = load float, ptr %reg_55
	%reg_57 = add i32 1, 0
	%reg_58 = getelementptr float, ptr %reg_5, i32 %reg_57
	%reg_59 = load float, ptr %reg_58
	%reg_60 = fmul float %reg_56, %reg_59
	%reg_61 = fadd float %reg_53, %reg_60
	store float %reg_61, ptr %reg_38
	%reg_62 = add i32 2, 0
	%reg_63 = getelementptr float, ptr %reg_6, i32 %reg_62
	%reg_64 = add i32 0, 0
	%reg_65 = getelementptr float, ptr %reg_0, i32 %reg_64
	%reg_66 = load float, ptr %reg_65
	%reg_67 = add i32 2, 0
	%reg_68 = getelementptr float, ptr %reg_3, i32 %reg_67
	%reg_69 = load float, ptr %reg_68
	%reg_70 = fmul float %reg_66, %reg_69
	%reg_71 = add i32 1, 0
	%reg_72 = getelementptr float, ptr %reg_0, i32 %reg_71
	%reg_73 = load float, ptr %reg_72
	%reg_74 = add i32 2, 0
	%reg_75 = getelementptr float, ptr %reg_4, i32 %reg_74
	%reg_76 = load float, ptr %reg_75
	%reg_77 = fmul float %reg_73, %reg_76
	%reg_78 = fadd float %reg_70, %reg_77
	%reg_79 = add i32 2, 0
	%reg_80 = getelementptr float, ptr %reg_0, i32 %reg_79
	%reg_81 = load float, ptr %reg_80
	%reg_82 = add i32 2, 0
	%reg_83 = getelementptr float, ptr %reg_5, i32 %reg_82
	%reg_84 = load float, ptr %reg_83
	%reg_85 = fmul float %reg_81, %reg_84
	%reg_86 = fadd float %reg_78, %reg_85
	store float %reg_86, ptr %reg_63
	%reg_87 = add i32 0, 0
	%reg_88 = getelementptr float, ptr %reg_7, i32 %reg_87
	%reg_89 = add i32 0, 0
	%reg_90 = getelementptr float, ptr %reg_1, i32 %reg_89
	%reg_91 = load float, ptr %reg_90
	%reg_92 = add i32 0, 0
	%reg_93 = getelementptr float, ptr %reg_3, i32 %reg_92
	%reg_94 = load float, ptr %reg_93
	%reg_95 = fmul float %reg_91, %reg_94
	%reg_96 = add i32 1, 0
	%reg_97 = getelementptr float, ptr %reg_1, i32 %reg_96
	%reg_98 = load float, ptr %reg_97
	%reg_99 = add i32 0, 0
	%reg_100 = getelementptr float, ptr %reg_4, i32 %reg_99
	%reg_101 = load float, ptr %reg_100
	%reg_102 = fmul float %reg_98, %reg_101
	%reg_103 = fadd float %reg_95, %reg_102
	%reg_104 = add i32 2, 0
	%reg_105 = getelementptr float, ptr %reg_1, i32 %reg_104
	%reg_106 = load float, ptr %reg_105
	%reg_107 = add i32 0, 0
	%reg_108 = getelementptr float, ptr %reg_5, i32 %reg_107
	%reg_109 = load float, ptr %reg_108
	%reg_110 = fmul float %reg_106, %reg_109
	%reg_111 = fadd float %reg_103, %reg_110
	store float %reg_111, ptr %reg_88
	%reg_112 = add i32 1, 0
	%reg_113 = getelementptr float, ptr %reg_7, i32 %reg_112
	%reg_114 = add i32 0, 0
	%reg_115 = getelementptr float, ptr %reg_1, i32 %reg_114
	%reg_116 = load float, ptr %reg_115
	%reg_117 = add i32 1, 0
	%reg_118 = getelementptr float, ptr %reg_3, i32 %reg_117
	%reg_119 = load float, ptr %reg_118
	%reg_120 = fmul float %reg_116, %reg_119
	%reg_121 = add i32 1, 0
	%reg_122 = getelementptr float, ptr %reg_1, i32 %reg_121
	%reg_123 = load float, ptr %reg_122
	%reg_124 = add i32 1, 0
	%reg_125 = getelementptr float, ptr %reg_4, i32 %reg_124
	%reg_126 = load float, ptr %reg_125
	%reg_127 = fmul float %reg_123, %reg_126
	%reg_128 = fadd float %reg_120, %reg_127
	%reg_129 = add i32 2, 0
	%reg_130 = getelementptr float, ptr %reg_1, i32 %reg_129
	%reg_131 = load float, ptr %reg_130
	%reg_132 = add i32 1, 0
	%reg_133 = getelementptr float, ptr %reg_5, i32 %reg_132
	%reg_134 = load float, ptr %reg_133
	%reg_135 = fmul float %reg_131, %reg_134
	%reg_136 = fadd float %reg_128, %reg_135
	store float %reg_136, ptr %reg_113
	%reg_137 = add i32 2, 0
	%reg_138 = getelementptr float, ptr %reg_7, i32 %reg_137
	%reg_139 = add i32 0, 0
	%reg_140 = getelementptr float, ptr %reg_1, i32 %reg_139
	%reg_141 = load float, ptr %reg_140
	%reg_142 = add i32 2, 0
	%reg_143 = getelementptr float, ptr %reg_3, i32 %reg_142
	%reg_144 = load float, ptr %reg_143
	%reg_145 = fmul float %reg_141, %reg_144
	%reg_146 = add i32 1, 0
	%reg_147 = getelementptr float, ptr %reg_1, i32 %reg_146
	%reg_148 = load float, ptr %reg_147
	%reg_149 = add i32 2, 0
	%reg_150 = getelementptr float, ptr %reg_4, i32 %reg_149
	%reg_151 = load float, ptr %reg_150
	%reg_152 = fmul float %reg_148, %reg_151
	%reg_153 = fadd float %reg_145, %reg_152
	%reg_154 = add i32 2, 0
	%reg_155 = getelementptr float, ptr %reg_1, i32 %reg_154
	%reg_156 = load float, ptr %reg_155
	%reg_157 = add i32 2, 0
	%reg_158 = getelementptr float, ptr %reg_5, i32 %reg_157
	%reg_159 = load float, ptr %reg_158
	%reg_160 = fmul float %reg_156, %reg_159
	%reg_161 = fadd float %reg_153, %reg_160
	store float %reg_161, ptr %reg_138
	%reg_162 = add i32 0, 0
	%reg_163 = getelementptr float, ptr %reg_8, i32 %reg_162
	%reg_164 = add i32 0, 0
	%reg_165 = getelementptr float, ptr %reg_2, i32 %reg_164
	%reg_166 = load float, ptr %reg_165
	%reg_167 = add i32 0, 0
	%reg_168 = getelementptr float, ptr %reg_3, i32 %reg_167
	%reg_169 = load float, ptr %reg_168
	%reg_170 = fmul float %reg_166, %reg_169
	%reg_171 = add i32 1, 0
	%reg_172 = getelementptr float, ptr %reg_2, i32 %reg_171
	%reg_173 = load float, ptr %reg_172
	%reg_174 = add i32 0, 0
	%reg_175 = getelementptr float, ptr %reg_4, i32 %reg_174
	%reg_176 = load float, ptr %reg_175
	%reg_177 = fmul float %reg_173, %reg_176
	%reg_178 = fadd float %reg_170, %reg_177
	%reg_179 = add i32 2, 0
	%reg_180 = getelementptr float, ptr %reg_2, i32 %reg_179
	%reg_181 = load float, ptr %reg_180
	%reg_182 = add i32 0, 0
	%reg_183 = getelementptr float, ptr %reg_5, i32 %reg_182
	%reg_184 = load float, ptr %reg_183
	%reg_185 = fmul float %reg_181, %reg_184
	%reg_186 = fadd float %reg_178, %reg_185
	store float %reg_186, ptr %reg_163
	%reg_187 = add i32 1, 0
	%reg_188 = getelementptr float, ptr %reg_8, i32 %reg_187
	%reg_189 = add i32 0, 0
	%reg_190 = getelementptr float, ptr %reg_2, i32 %reg_189
	%reg_191 = load float, ptr %reg_190
	%reg_192 = add i32 1, 0
	%reg_193 = getelementptr float, ptr %reg_3, i32 %reg_192
	%reg_194 = load float, ptr %reg_193
	%reg_195 = fmul float %reg_191, %reg_194
	%reg_196 = add i32 1, 0
	%reg_197 = getelementptr float, ptr %reg_2, i32 %reg_196
	%reg_198 = load float, ptr %reg_197
	%reg_199 = add i32 1, 0
	%reg_200 = getelementptr float, ptr %reg_4, i32 %reg_199
	%reg_201 = load float, ptr %reg_200
	%reg_202 = fmul float %reg_198, %reg_201
	%reg_203 = fadd float %reg_195, %reg_202
	%reg_204 = add i32 2, 0
	%reg_205 = getelementptr float, ptr %reg_2, i32 %reg_204
	%reg_206 = load float, ptr %reg_205
	%reg_207 = add i32 1, 0
	%reg_208 = getelementptr float, ptr %reg_5, i32 %reg_207
	%reg_209 = load float, ptr %reg_208
	%reg_210 = fmul float %reg_206, %reg_209
	%reg_211 = fadd float %reg_203, %reg_210
	store float %reg_211, ptr %reg_188
	%reg_212 = add i32 2, 0
	%reg_213 = getelementptr float, ptr %reg_8, i32 %reg_212
	%reg_214 = add i32 0, 0
	%reg_215 = getelementptr float, ptr %reg_2, i32 %reg_214
	%reg_216 = load float, ptr %reg_215
	%reg_217 = add i32 2, 0
	%reg_218 = getelementptr float, ptr %reg_3, i32 %reg_217
	%reg_219 = load float, ptr %reg_218
	%reg_220 = fmul float %reg_216, %reg_219
	%reg_221 = add i32 1, 0
	%reg_222 = getelementptr float, ptr %reg_2, i32 %reg_221
	%reg_223 = load float, ptr %reg_222
	%reg_224 = add i32 2, 0
	%reg_225 = getelementptr float, ptr %reg_4, i32 %reg_224
	%reg_226 = load float, ptr %reg_225
	%reg_227 = fmul float %reg_223, %reg_226
	%reg_228 = fadd float %reg_220, %reg_227
	%reg_229 = add i32 2, 0
	%reg_230 = getelementptr float, ptr %reg_2, i32 %reg_229
	%reg_231 = load float, ptr %reg_230
	%reg_232 = add i32 2, 0
	%reg_233 = getelementptr float, ptr %reg_5, i32 %reg_232
	%reg_234 = load float, ptr %reg_233
	%reg_235 = fmul float %reg_231, %reg_234
	%reg_236 = fadd float %reg_228, %reg_235
	store float %reg_236, ptr %reg_213
	%reg_237 = add i32 0, 0
	ret i32 %reg_237
}

define i32 @main()
{
Block0:  ;Func define at line 24
	%reg_55 = alloca i32
	%reg_12 = alloca i32
	%reg_11 = alloca [3 x float]
	%reg_10 = alloca [3 x float]
	%reg_9 = alloca [6 x float]
	%reg_8 = alloca [3 x float]
	%reg_7 = alloca [3 x float]
	%reg_6 = alloca [3 x float]
	%reg_5 = alloca [3 x float]
	%reg_4 = alloca [3 x float]
	%reg_3 = alloca [3 x float]
	br label %Block1
Block1:  ;Func body at line 24
	%reg_0 = add i32 3, 0
	store i32 %reg_0, ptr @N
	%reg_1 = add i32 3, 0
	store i32 %reg_1, ptr @M
	%reg_2 = add i32 3, 0
	store i32 %reg_2, ptr @L
	call void @llvm.memset.p0.i32(ptr %reg_3, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_4, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_5, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_6, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_7, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_8, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_9, i8 0, i32 24, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_10, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_11, i8 0, i32 12, i1 1)
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_12
	br label %Block2
Block2:  ;While condition at line 32
	%reg_15 = load i32, ptr %reg_12
	%reg_16 = load i32, ptr @M
	%reg_17 = icmp slt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block3, label %Block4
Block3:  ;While body at line 32
	%reg_18 = load i32, ptr %reg_12
	%reg_19 = getelementptr [3 x float], ptr %reg_3, i32 0, i32 %reg_18
	%reg_20 = load i32, ptr %reg_12
	%reg_21 = sitofp i32 %reg_20 to float
	store float %reg_21, ptr %reg_19
	%reg_22 = load i32, ptr %reg_12
	%reg_23 = getelementptr [3 x float], ptr %reg_4, i32 0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_12
	%reg_25 = sitofp i32 %reg_24 to float
	store float %reg_25, ptr %reg_23
	%reg_26 = load i32, ptr %reg_12
	%reg_27 = getelementptr [3 x float], ptr %reg_5, i32 0, i32 %reg_26
	%reg_28 = load i32, ptr %reg_12
	%reg_29 = sitofp i32 %reg_28 to float
	store float %reg_29, ptr %reg_27
	%reg_30 = load i32, ptr %reg_12
	%reg_31 = getelementptr [3 x float], ptr %reg_6, i32 0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_12
	%reg_33 = sitofp i32 %reg_32 to float
	store float %reg_33, ptr %reg_31
	%reg_34 = load i32, ptr %reg_12
	%reg_35 = getelementptr [3 x float], ptr %reg_7, i32 0, i32 %reg_34
	%reg_36 = load i32, ptr %reg_12
	%reg_37 = sitofp i32 %reg_36 to float
	store float %reg_37, ptr %reg_35
	%reg_38 = load i32, ptr %reg_12
	%reg_39 = getelementptr [3 x float], ptr %reg_8, i32 0, i32 %reg_38
	%reg_40 = load i32, ptr %reg_12
	%reg_41 = sitofp i32 %reg_40 to float
	store float %reg_41, ptr %reg_39
	%reg_42 = load i32, ptr %reg_12
	%reg_43 = add i32 1, 0
	%reg_44 = add i32 %reg_42, %reg_43
	store i32 %reg_44, ptr %reg_12
	br label %Block2
Block4:  ;While end at line 32
	%reg_45 = getelementptr [3 x float], ptr %reg_3, i32 0
	%reg_46 = getelementptr [3 x float], ptr %reg_4, i32 0
	%reg_47 = getelementptr [3 x float], ptr %reg_5, i32 0
	%reg_48 = getelementptr [3 x float], ptr %reg_6, i32 0
	%reg_49 = getelementptr [3 x float], ptr %reg_7, i32 0
	%reg_50 = getelementptr [3 x float], ptr %reg_8, i32 0
	%reg_51 = getelementptr [6 x float], ptr %reg_9, i32 0
	%reg_52 = getelementptr [3 x float], ptr %reg_10, i32 0
	%reg_53 = getelementptr [3 x float], ptr %reg_11, i32 0
	%reg_54 = call i32 @mul(ptr %reg_45, ptr %reg_46, ptr %reg_47, ptr %reg_48, ptr %reg_49, ptr %reg_50, ptr %reg_51, ptr %reg_52, ptr %reg_53)
	store i32 %reg_54, ptr %reg_12
	%reg_56 = add i32 0, 0
	store i32 %reg_56, ptr %reg_55
	br label %Block5
Block5:  ;While condition at line 44
	%reg_57 = load i32, ptr %reg_12
	%reg_58 = load i32, ptr @N
	%reg_59 = icmp slt i32 %reg_57, %reg_58
	br i1 %reg_59, label %Block6, label %Block7
Block6:  ;While body at line 44
	%reg_60 = load i32, ptr %reg_12
	%reg_61 = getelementptr [6 x float], ptr %reg_9, i32 0, i32 %reg_60
	%reg_62 = load float, ptr %reg_61
	%reg_63 = fptosi float %reg_62 to i32
	store i32 %reg_63, ptr %reg_55
	%reg_64 = load i32, ptr %reg_55
	call void @putint(i32 %reg_64)
	%reg_65 = load i32, ptr %reg_12
	%reg_66 = add i32 1, 0
	%reg_67 = add i32 %reg_65, %reg_66
	store i32 %reg_67, ptr %reg_12
	br label %Block5
Block7:  ;While end at line 44
	%reg_68 = add i32 10, 0
	store i32 %reg_68, ptr %reg_55
	%reg_69 = add i32 0, 0
	store i32 %reg_69, ptr %reg_12
	%reg_70 = load i32, ptr %reg_55
	call void @putch(i32 %reg_70)
	br label %Block8
Block8:  ;While condition at line 53
	%reg_71 = load i32, ptr %reg_12
	%reg_72 = load i32, ptr @N
	%reg_73 = icmp slt i32 %reg_71, %reg_72
	br i1 %reg_73, label %Block9, label %Block10
Block9:  ;While body at line 53
	%reg_74 = load i32, ptr %reg_12
	%reg_75 = getelementptr [3 x float], ptr %reg_10, i32 0, i32 %reg_74
	%reg_76 = load float, ptr %reg_75
	%reg_77 = fptosi float %reg_76 to i32
	store i32 %reg_77, ptr %reg_55
	%reg_78 = load i32, ptr %reg_55
	call void @putint(i32 %reg_78)
	%reg_79 = load i32, ptr %reg_12
	%reg_80 = add i32 1, 0
	%reg_81 = add i32 %reg_79, %reg_80
	store i32 %reg_81, ptr %reg_12
	br label %Block8
Block10:  ;While end at line 53
	%reg_82 = add i32 10, 0
	store i32 %reg_82, ptr %reg_55
	%reg_83 = add i32 0, 0
	store i32 %reg_83, ptr %reg_12
	%reg_84 = load i32, ptr %reg_55
	call void @putch(i32 %reg_84)
	br label %Block11
Block11:  ;While condition at line 63
	%reg_85 = load i32, ptr %reg_12
	%reg_86 = load i32, ptr @N
	%reg_87 = icmp slt i32 %reg_85, %reg_86
	br i1 %reg_87, label %Block12, label %Block13
Block12:  ;While body at line 63
	%reg_88 = load i32, ptr %reg_12
	%reg_89 = getelementptr [3 x float], ptr %reg_11, i32 0, i32 %reg_88
	%reg_90 = load float, ptr %reg_89
	%reg_91 = fptosi float %reg_90 to i32
	store i32 %reg_91, ptr %reg_55
	%reg_92 = load i32, ptr %reg_55
	call void @putint(i32 %reg_92)
	%reg_93 = load i32, ptr %reg_12
	%reg_94 = add i32 1, 0
	%reg_95 = add i32 %reg_93, %reg_94
	store i32 %reg_95, ptr %reg_12
	br label %Block11
Block13:  ;While end at line 63
	%reg_96 = add i32 10, 0
	store i32 %reg_96, ptr %reg_55
	%reg_97 = load i32, ptr %reg_55
	call void @putch(i32 %reg_97)
	%reg_98 = add i32 0, 0
	ret i32 %reg_98
}
