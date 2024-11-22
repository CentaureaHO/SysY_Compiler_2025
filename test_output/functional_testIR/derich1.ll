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

@w = global i32 512
@h = global i32 270
@imgIn = global [512 x [270 x float]] zeroinitializer
@imgOut = global [512 x [270 x float]] zeroinitializer
@my_y1 = global [512 x [270 x float]] zeroinitializer
@my_y2 = global [512 x [270 x float]] zeroinitializer
@alpha = global float 0x3fd0000000000000

define float @newExp(float %reg_0)
{
Block0:  ;Func define at line 9
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 9
	%reg_2 = fadd float 0x3ff0000000000000, 0x0
	%reg_3 = load float, ptr %reg_1
	%reg_4 = add i32 256, 0
	%reg_5 = sitofp i32 %reg_4 to float
	%reg_6 = fdiv float %reg_3, %reg_5
	%reg_7 = fadd float %reg_2, %reg_6
	store float %reg_7, ptr %reg_1
	%reg_8 = load float, ptr %reg_1
	%reg_9 = load float, ptr %reg_1
	%reg_10 = fmul float %reg_8, %reg_9
	store float %reg_10, ptr %reg_1
	%reg_11 = load float, ptr %reg_1
	%reg_12 = load float, ptr %reg_1
	%reg_13 = fmul float %reg_11, %reg_12
	store float %reg_13, ptr %reg_1
	%reg_14 = load float, ptr %reg_1
	%reg_15 = load float, ptr %reg_1
	%reg_16 = fmul float %reg_14, %reg_15
	store float %reg_16, ptr %reg_1
	%reg_17 = load float, ptr %reg_1
	%reg_18 = load float, ptr %reg_1
	%reg_19 = fmul float %reg_17, %reg_18
	store float %reg_19, ptr %reg_1
	%reg_20 = load float, ptr %reg_1
	%reg_21 = load float, ptr %reg_1
	%reg_22 = fmul float %reg_20, %reg_21
	store float %reg_22, ptr %reg_1
	%reg_23 = load float, ptr %reg_1
	%reg_24 = load float, ptr %reg_1
	%reg_25 = fmul float %reg_23, %reg_24
	store float %reg_25, ptr %reg_1
	%reg_26 = load float, ptr %reg_1
	%reg_27 = load float, ptr %reg_1
	%reg_28 = fmul float %reg_26, %reg_27
	store float %reg_28, ptr %reg_1
	%reg_29 = load float, ptr %reg_1
	%reg_30 = load float, ptr %reg_1
	%reg_31 = fmul float %reg_29, %reg_30
	store float %reg_31, ptr %reg_1
	%reg_32 = load float, ptr %reg_1
	ret float %reg_32
}

define float @newPow(float %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 23
	%reg_3 = alloca i32
	%reg_2 = alloca float
	store float %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 23
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = add i32 0, 0
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 25
	%reg_7 = fadd float 0x3ff0000000000000, 0x0
	%reg_8 = load float, ptr %reg_2
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = sub i32 0, %reg_9
	%reg_11 = call float @newPow(float %reg_8, i32 %reg_10)
	%reg_12 = fdiv float %reg_7, %reg_11
	ret float %reg_12
	br label %Block3
Block3:  ;If end at line 25
	ret float 0x3fd3333340000000
Block4:  ;If else at line 25
	%reg_13 = load i32, ptr %reg_3
	%reg_14 = add i32 0, 0
	%reg_15 = icmp eq i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block5, label %Block7
Block5:  ;If then at line 27
	%reg_16 = fadd float 0x3ff0000000000000, 0x0
	ret float %reg_16
	br label %Block6
Block6:  ;If end at line 27
	br label %Block3
Block7:  ;If else at line 27
	%reg_17 = load i32, ptr %reg_3
	%reg_18 = add i32 0, 0
	%reg_19 = icmp sgt i32 %reg_17, %reg_18
	br i1 %reg_19, label %Block8, label %Block9
Block8:  ;If then at line 29
	%reg_20 = load float, ptr %reg_2
	%reg_21 = load float, ptr %reg_2
	%reg_22 = load i32, ptr %reg_3
	%reg_23 = add i32 1, 0
	%reg_24 = sub i32 %reg_22, %reg_23
	%reg_25 = call float @newPow(float %reg_21, i32 %reg_24)
	%reg_26 = fmul float %reg_20, %reg_25
	ret float %reg_26
	br label %Block9
Block9:  ;If end at line 29
	br label %Block6
}

define void @kernel_deriche(i32 %reg_0, i32 %reg_1, float %reg_2, ptr %reg_3, ptr %reg_4, ptr %reg_5, ptr %reg_6)
{
Block0:  ;Func define at line 33
	%reg_58 = alloca float
	%reg_56 = alloca float
	%reg_54 = alloca float
	%reg_52 = alloca float
	%reg_50 = alloca float
	%reg_48 = alloca float
	%reg_46 = alloca float
	%reg_44 = alloca float
	%reg_42 = alloca float
	%reg_40 = alloca float
	%reg_38 = alloca float
	%reg_36 = alloca float
	%reg_34 = alloca float
	%reg_32 = alloca float
	%reg_30 = alloca float
	%reg_28 = alloca float
	%reg_26 = alloca float
	%reg_24 = alloca float
	%reg_22 = alloca float
	%reg_20 = alloca float
	%reg_18 = alloca float
	%reg_16 = alloca float
	%reg_14 = alloca float
	%reg_12 = alloca i32
	%reg_10 = alloca i32
	%reg_9 = alloca float
	%reg_8 = alloca i32
	%reg_7 = alloca i32
	store i32 %reg_0, ptr %reg_7
	store i32 %reg_1, ptr %reg_8
	store float %reg_2, ptr %reg_9
	br label %Block1
Block1:  ;Func body at line 33
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_15 = fadd float 0x0, 0x0
	store float %reg_15, ptr %reg_14
	%reg_17 = fadd float 0x0, 0x0
	store float %reg_17, ptr %reg_16
	%reg_19 = fadd float 0x0, 0x0
	store float %reg_19, ptr %reg_18
	%reg_21 = fadd float 0x0, 0x0
	store float %reg_21, ptr %reg_20
	%reg_23 = fadd float 0x0, 0x0
	store float %reg_23, ptr %reg_22
	%reg_25 = fadd float 0x0, 0x0
	store float %reg_25, ptr %reg_24
	%reg_27 = fadd float 0x0, 0x0
	store float %reg_27, ptr %reg_26
	%reg_29 = fadd float 0x0, 0x0
	store float %reg_29, ptr %reg_28
	%reg_31 = fadd float 0x0, 0x0
	store float %reg_31, ptr %reg_30
	%reg_33 = fadd float 0x0, 0x0
	store float %reg_33, ptr %reg_32
	%reg_35 = fadd float 0x0, 0x0
	store float %reg_35, ptr %reg_34
	%reg_37 = fadd float 0x0, 0x0
	store float %reg_37, ptr %reg_36
	%reg_39 = fadd float 0x0, 0x0
	store float %reg_39, ptr %reg_38
	%reg_41 = fadd float 0x0, 0x0
	store float %reg_41, ptr %reg_40
	%reg_43 = fadd float 0x0, 0x0
	store float %reg_43, ptr %reg_42
	%reg_45 = fadd float 0x0, 0x0
	store float %reg_45, ptr %reg_44
	%reg_47 = fadd float 0x0, 0x0
	store float %reg_47, ptr %reg_46
	%reg_49 = fadd float 0x0, 0x0
	store float %reg_49, ptr %reg_48
	%reg_51 = fadd float 0x0, 0x0
	store float %reg_51, ptr %reg_50
	%reg_53 = fadd float 0x0, 0x0
	store float %reg_53, ptr %reg_52
	%reg_55 = fadd float 0x0, 0x0
	store float %reg_55, ptr %reg_54
	%reg_57 = fadd float 0x0, 0x0
	store float %reg_57, ptr %reg_56
	%reg_59 = fadd float 0x0, 0x0
	store float %reg_59, ptr %reg_58
	%reg_60 = fadd float 0x3ff0000000000000, 0x0
	%reg_61 = load float, ptr %reg_9
	%reg_62 = fsub float 0x0, %reg_61
	%reg_63 = call float @newExp(float %reg_62)
	%reg_64 = fsub float %reg_60, %reg_63
	%reg_65 = fadd float 0x3ff0000000000000, 0x0
	%reg_66 = load float, ptr %reg_9
	%reg_67 = fsub float 0x0, %reg_66
	%reg_68 = call float @newExp(float %reg_67)
	%reg_69 = fsub float %reg_65, %reg_68
	%reg_70 = fmul float %reg_64, %reg_69
	%reg_71 = fadd float 0x3ff0000000000000, 0x0
	%reg_72 = fadd float 0x4000000000000000, 0x0
	%reg_73 = load float, ptr %reg_9
	%reg_74 = fmul float %reg_72, %reg_73
	%reg_75 = load float, ptr %reg_9
	%reg_76 = fsub float 0x0, %reg_75
	%reg_77 = call float @newExp(float %reg_76)
	%reg_78 = fmul float %reg_74, %reg_77
	%reg_79 = fadd float %reg_71, %reg_78
	%reg_80 = fadd float 0x4000000000000000, 0x0
	%reg_81 = load float, ptr %reg_9
	%reg_82 = fmul float %reg_80, %reg_81
	%reg_83 = call float @newExp(float %reg_82)
	%reg_84 = fsub float %reg_79, %reg_83
	%reg_85 = fdiv float %reg_70, %reg_84
	store float %reg_85, ptr %reg_34
	%reg_86 = load float, ptr %reg_34
	store float %reg_86, ptr %reg_36
	%reg_87 = load float, ptr %reg_34
	store float %reg_87, ptr %reg_44
	%reg_88 = load float, ptr %reg_34
	%reg_89 = load float, ptr %reg_9
	%reg_90 = fsub float 0x0, %reg_89
	%reg_91 = call float @newExp(float %reg_90)
	%reg_92 = fmul float %reg_88, %reg_91
	%reg_93 = load float, ptr %reg_9
	%reg_94 = fadd float 0x3ff0000000000000, 0x0
	%reg_95 = fsub float %reg_93, %reg_94
	%reg_96 = fmul float %reg_92, %reg_95
	store float %reg_96, ptr %reg_46
	%reg_97 = load float, ptr %reg_46
	store float %reg_97, ptr %reg_38
	%reg_98 = load float, ptr %reg_34
	%reg_99 = load float, ptr %reg_9
	%reg_100 = fsub float 0x0, %reg_99
	%reg_101 = call float @newExp(float %reg_100)
	%reg_102 = fmul float %reg_98, %reg_101
	%reg_103 = load float, ptr %reg_9
	%reg_104 = fadd float 0x3ff0000000000000, 0x0
	%reg_105 = fadd float %reg_103, %reg_104
	%reg_106 = fmul float %reg_102, %reg_105
	store float %reg_106, ptr %reg_48
	%reg_107 = load float, ptr %reg_48
	store float %reg_107, ptr %reg_40
	%reg_108 = load float, ptr %reg_34
	%reg_109 = fsub float 0x0, %reg_108
	%reg_110 = fadd float 0x4000000000000000, 0x0
	%reg_111 = fsub float 0x0, %reg_110
	%reg_112 = load float, ptr %reg_9
	%reg_113 = fmul float %reg_111, %reg_112
	%reg_114 = call float @newExp(float %reg_113)
	%reg_115 = fmul float %reg_109, %reg_114
	store float %reg_115, ptr %reg_50
	%reg_116 = load float, ptr %reg_50
	store float %reg_116, ptr %reg_42
	%reg_117 = fadd float 0x4000000000000000, 0x0
	%reg_118 = load float, ptr %reg_9
	%reg_119 = fsub float 0x0, %reg_118
	%reg_120 = fptosi float %reg_119 to i32
	%reg_121 = call float @newPow(float %reg_117, i32 %reg_120)
	store float %reg_121, ptr %reg_52
	%reg_122 = fadd float 0x4000000000000000, 0x0
	%reg_123 = fsub float 0x0, %reg_122
	%reg_124 = load float, ptr %reg_9
	%reg_125 = fmul float %reg_123, %reg_124
	%reg_126 = call float @newExp(float %reg_125)
	%reg_127 = fsub float 0x0, %reg_126
	store float %reg_127, ptr %reg_54
	%reg_128 = add i32 1, 0
	%reg_129 = sitofp i32 %reg_128 to float
	store float %reg_129, ptr %reg_56
	%reg_130 = add i32 1, 0
	%reg_131 = sitofp i32 %reg_130 to float
	store float %reg_131, ptr %reg_58
	%reg_132 = add i32 0, 0
	store i32 %reg_132, ptr %reg_10
	br label %Block2
Block2:  ;While condition at line 61
	%reg_133 = load i32, ptr %reg_10
	%reg_134 = load i32, ptr %reg_7
	%reg_135 = icmp slt i32 %reg_133, %reg_134
	br i1 %reg_135, label %Block3, label %Block4
Block3:  ;While body at line 61
	%reg_136 = fadd float 0x0, 0x0
	store float %reg_136, ptr %reg_18
	%reg_137 = fadd float 0x0, 0x0
	store float %reg_137, ptr %reg_20
	%reg_138 = fadd float 0x0, 0x0
	store float %reg_138, ptr %reg_14
	%reg_139 = add i32 0, 0
	store i32 %reg_139, ptr %reg_12
	br label %Block5
Block4:  ;While end at line 61
	%reg_179 = add i32 0, 0
	store i32 %reg_179, ptr %reg_10
	br label %Block8
Block5:  ;While condition at line 67
	%reg_140 = load i32, ptr %reg_12
	%reg_141 = load i32, ptr %reg_8
	%reg_142 = icmp slt i32 %reg_140, %reg_141
	br i1 %reg_142, label %Block6, label %Block7
Block6:  ;While body at line 67
	%reg_143 = load i32, ptr %reg_10
	%reg_144 = load i32, ptr %reg_12
	%reg_145 = getelementptr [270 x float], ptr %reg_5, i32 %reg_143, i32 %reg_144
	%reg_146 = load float, ptr %reg_36
	%reg_147 = load i32, ptr %reg_10
	%reg_148 = load i32, ptr %reg_12
	%reg_149 = getelementptr [270 x float], ptr %reg_3, i32 %reg_147, i32 %reg_148
	%reg_150 = load float, ptr %reg_149
	%reg_151 = fmul float %reg_146, %reg_150
	%reg_152 = load float, ptr %reg_38
	%reg_153 = load float, ptr %reg_14
	%reg_154 = fmul float %reg_152, %reg_153
	%reg_155 = fadd float %reg_151, %reg_154
	%reg_156 = load float, ptr %reg_52
	%reg_157 = load float, ptr %reg_18
	%reg_158 = fmul float %reg_156, %reg_157
	%reg_159 = fadd float %reg_155, %reg_158
	%reg_160 = load float, ptr %reg_54
	%reg_161 = load float, ptr %reg_20
	%reg_162 = fmul float %reg_160, %reg_161
	%reg_163 = fadd float %reg_159, %reg_162
	store float %reg_163, ptr %reg_145
	%reg_164 = load i32, ptr %reg_10
	%reg_165 = load i32, ptr %reg_12
	%reg_166 = getelementptr [270 x float], ptr %reg_3, i32 %reg_164, i32 %reg_165
	%reg_167 = load float, ptr %reg_166
	store float %reg_167, ptr %reg_14
	%reg_168 = load float, ptr %reg_18
	store float %reg_168, ptr %reg_20
	%reg_169 = load i32, ptr %reg_10
	%reg_170 = load i32, ptr %reg_12
	%reg_171 = getelementptr [270 x float], ptr %reg_5, i32 %reg_169, i32 %reg_170
	%reg_172 = load float, ptr %reg_171
	store float %reg_172, ptr %reg_18
	%reg_173 = load i32, ptr %reg_12
	%reg_174 = add i32 1, 0
	%reg_175 = add i32 %reg_173, %reg_174
	store i32 %reg_175, ptr %reg_12
	br label %Block5
Block7:  ;While end at line 67
	%reg_176 = load i32, ptr %reg_10
	%reg_177 = add i32 1, 0
	%reg_178 = add i32 %reg_176, %reg_177
	store i32 %reg_178, ptr %reg_10
	br label %Block2
Block8:  ;While condition at line 78
	%reg_180 = load i32, ptr %reg_10
	%reg_181 = load i32, ptr %reg_7
	%reg_182 = icmp slt i32 %reg_180, %reg_181
	br i1 %reg_182, label %Block9, label %Block10
Block9:  ;While body at line 78
	%reg_183 = fadd float 0x0, 0x0
	store float %reg_183, ptr %reg_30
	%reg_184 = fadd float 0x0, 0x0
	store float %reg_184, ptr %reg_32
	%reg_185 = fadd float 0x0, 0x0
	store float %reg_185, ptr %reg_22
	%reg_186 = fadd float 0x0, 0x0
	store float %reg_186, ptr %reg_24
	%reg_187 = load i32, ptr %reg_8
	%reg_188 = add i32 1, 0
	%reg_189 = sub i32 %reg_187, %reg_188
	store i32 %reg_189, ptr %reg_12
	br label %Block11
Block10:  ;While end at line 78
	%reg_227 = add i32 0, 0
	store i32 %reg_227, ptr %reg_10
	br label %Block14
Block11:  ;While condition at line 85
	%reg_190 = load i32, ptr %reg_12
	%reg_191 = add i32 0, 0
	%reg_192 = icmp sge i32 %reg_190, %reg_191
	br i1 %reg_192, label %Block12, label %Block13
Block12:  ;While body at line 85
	%reg_193 = load i32, ptr %reg_10
	%reg_194 = load i32, ptr %reg_12
	%reg_195 = getelementptr [270 x float], ptr %reg_6, i32 %reg_193, i32 %reg_194
	%reg_196 = load float, ptr %reg_40
	%reg_197 = load float, ptr %reg_22
	%reg_198 = fmul float %reg_196, %reg_197
	%reg_199 = load float, ptr %reg_42
	%reg_200 = load float, ptr %reg_24
	%reg_201 = fmul float %reg_199, %reg_200
	%reg_202 = fadd float %reg_198, %reg_201
	%reg_203 = load float, ptr %reg_52
	%reg_204 = load float, ptr %reg_30
	%reg_205 = fmul float %reg_203, %reg_204
	%reg_206 = fadd float %reg_202, %reg_205
	%reg_207 = load float, ptr %reg_54
	%reg_208 = load float, ptr %reg_32
	%reg_209 = fmul float %reg_207, %reg_208
	%reg_210 = fadd float %reg_206, %reg_209
	store float %reg_210, ptr %reg_195
	%reg_211 = load float, ptr %reg_22
	store float %reg_211, ptr %reg_24
	%reg_212 = load i32, ptr %reg_10
	%reg_213 = load i32, ptr %reg_12
	%reg_214 = getelementptr [270 x float], ptr %reg_3, i32 %reg_212, i32 %reg_213
	%reg_215 = load float, ptr %reg_214
	store float %reg_215, ptr %reg_22
	%reg_216 = load float, ptr %reg_30
	store float %reg_216, ptr %reg_32
	%reg_217 = load i32, ptr %reg_10
	%reg_218 = load i32, ptr %reg_12
	%reg_219 = getelementptr [270 x float], ptr %reg_6, i32 %reg_217, i32 %reg_218
	%reg_220 = load float, ptr %reg_219
	store float %reg_220, ptr %reg_30
	%reg_221 = load i32, ptr %reg_12
	%reg_222 = add i32 1, 0
	%reg_223 = sub i32 %reg_221, %reg_222
	store i32 %reg_223, ptr %reg_12
	br label %Block11
Block13:  ;While end at line 85
	%reg_224 = load i32, ptr %reg_10
	%reg_225 = add i32 1, 0
	%reg_226 = add i32 %reg_224, %reg_225
	store i32 %reg_226, ptr %reg_10
	br label %Block8
Block14:  ;While condition at line 97
	%reg_228 = load i32, ptr %reg_10
	%reg_229 = load i32, ptr %reg_7
	%reg_230 = icmp slt i32 %reg_228, %reg_229
	br i1 %reg_230, label %Block15, label %Block16
Block15:  ;While body at line 97
	%reg_231 = add i32 0, 0
	store i32 %reg_231, ptr %reg_12
	br label %Block17
Block16:  ;While end at line 97
	%reg_255 = add i32 0, 0
	store i32 %reg_255, ptr %reg_12
	br label %Block20
Block17:  ;While condition at line 99
	%reg_232 = load i32, ptr %reg_12
	%reg_233 = load i32, ptr %reg_8
	%reg_234 = icmp slt i32 %reg_232, %reg_233
	br i1 %reg_234, label %Block18, label %Block19
Block18:  ;While body at line 99
	%reg_235 = load i32, ptr %reg_10
	%reg_236 = load i32, ptr %reg_12
	%reg_237 = getelementptr [270 x float], ptr %reg_4, i32 %reg_235, i32 %reg_236
	%reg_238 = load float, ptr %reg_56
	%reg_239 = load i32, ptr %reg_10
	%reg_240 = load i32, ptr %reg_12
	%reg_241 = getelementptr [270 x float], ptr %reg_5, i32 %reg_239, i32 %reg_240
	%reg_242 = load float, ptr %reg_241
	%reg_243 = load i32, ptr %reg_10
	%reg_244 = load i32, ptr %reg_12
	%reg_245 = getelementptr [270 x float], ptr %reg_6, i32 %reg_243, i32 %reg_244
	%reg_246 = load float, ptr %reg_245
	%reg_247 = fadd float %reg_242, %reg_246
	%reg_248 = fmul float %reg_238, %reg_247
	store float %reg_248, ptr %reg_237
	%reg_249 = load i32, ptr %reg_12
	%reg_250 = add i32 1, 0
	%reg_251 = add i32 %reg_249, %reg_250
	store i32 %reg_251, ptr %reg_12
	br label %Block17
Block19:  ;While end at line 99
	%reg_252 = load i32, ptr %reg_10
	%reg_253 = add i32 1, 0
	%reg_254 = add i32 %reg_252, %reg_253
	store i32 %reg_254, ptr %reg_10
	br label %Block14
Block20:  ;While condition at line 107
	%reg_256 = load i32, ptr %reg_12
	%reg_257 = load i32, ptr %reg_8
	%reg_258 = icmp slt i32 %reg_256, %reg_257
	br i1 %reg_258, label %Block21, label %Block22
Block21:  ;While body at line 107
	%reg_259 = fadd float 0x0, 0x0
	store float %reg_259, ptr %reg_16
	%reg_260 = fadd float 0x0, 0x0
	store float %reg_260, ptr %reg_18
	%reg_261 = fadd float 0x0, 0x0
	store float %reg_261, ptr %reg_20
	%reg_262 = add i32 0, 0
	store i32 %reg_262, ptr %reg_10
	br label %Block23
Block22:  ;While end at line 107
	%reg_302 = add i32 0, 0
	store i32 %reg_302, ptr %reg_12
	br label %Block26
Block23:  ;While condition at line 113
	%reg_263 = load i32, ptr %reg_10
	%reg_264 = load i32, ptr %reg_7
	%reg_265 = icmp slt i32 %reg_263, %reg_264
	br i1 %reg_265, label %Block24, label %Block25
Block24:  ;While body at line 113
	%reg_266 = load i32, ptr %reg_10
	%reg_267 = load i32, ptr %reg_12
	%reg_268 = getelementptr [270 x float], ptr %reg_5, i32 %reg_266, i32 %reg_267
	%reg_269 = load float, ptr %reg_44
	%reg_270 = load i32, ptr %reg_10
	%reg_271 = load i32, ptr %reg_12
	%reg_272 = getelementptr [270 x float], ptr %reg_4, i32 %reg_270, i32 %reg_271
	%reg_273 = load float, ptr %reg_272
	%reg_274 = fmul float %reg_269, %reg_273
	%reg_275 = load float, ptr %reg_46
	%reg_276 = load float, ptr %reg_16
	%reg_277 = fmul float %reg_275, %reg_276
	%reg_278 = fadd float %reg_274, %reg_277
	%reg_279 = load float, ptr %reg_52
	%reg_280 = load float, ptr %reg_18
	%reg_281 = fmul float %reg_279, %reg_280
	%reg_282 = fadd float %reg_278, %reg_281
	%reg_283 = load float, ptr %reg_54
	%reg_284 = load float, ptr %reg_20
	%reg_285 = fmul float %reg_283, %reg_284
	%reg_286 = fadd float %reg_282, %reg_285
	store float %reg_286, ptr %reg_268
	%reg_287 = load i32, ptr %reg_10
	%reg_288 = load i32, ptr %reg_12
	%reg_289 = getelementptr [270 x float], ptr %reg_4, i32 %reg_287, i32 %reg_288
	%reg_290 = load float, ptr %reg_289
	store float %reg_290, ptr %reg_16
	%reg_291 = load float, ptr %reg_18
	store float %reg_291, ptr %reg_20
	%reg_292 = load i32, ptr %reg_10
	%reg_293 = load i32, ptr %reg_12
	%reg_294 = getelementptr [270 x float], ptr %reg_5, i32 %reg_292, i32 %reg_293
	%reg_295 = load float, ptr %reg_294
	store float %reg_295, ptr %reg_18
	%reg_296 = load i32, ptr %reg_10
	%reg_297 = add i32 1, 0
	%reg_298 = add i32 %reg_296, %reg_297
	store i32 %reg_298, ptr %reg_10
	br label %Block23
Block25:  ;While end at line 113
	%reg_299 = load i32, ptr %reg_12
	%reg_300 = add i32 1, 0
	%reg_301 = add i32 %reg_299, %reg_300
	store i32 %reg_301, ptr %reg_12
	br label %Block20
Block26:  ;While condition at line 124
	%reg_303 = load i32, ptr %reg_12
	%reg_304 = load i32, ptr %reg_8
	%reg_305 = icmp slt i32 %reg_303, %reg_304
	br i1 %reg_305, label %Block27, label %Block28
Block27:  ;While body at line 124
	%reg_306 = fadd float 0x0, 0x0
	store float %reg_306, ptr %reg_26
	%reg_307 = fadd float 0x0, 0x0
	store float %reg_307, ptr %reg_28
	%reg_308 = fadd float 0x0, 0x0
	store float %reg_308, ptr %reg_30
	%reg_309 = fadd float 0x0, 0x0
	store float %reg_309, ptr %reg_32
	%reg_310 = load i32, ptr %reg_7
	%reg_311 = add i32 1, 0
	%reg_312 = sub i32 %reg_310, %reg_311
	store i32 %reg_312, ptr %reg_10
	br label %Block29
Block28:  ;While end at line 124
	%reg_350 = add i32 0, 0
	store i32 %reg_350, ptr %reg_10
	br label %Block32
Block29:  ;While condition at line 131
	%reg_313 = load i32, ptr %reg_10
	%reg_314 = add i32 0, 0
	%reg_315 = icmp sge i32 %reg_313, %reg_314
	br i1 %reg_315, label %Block30, label %Block31
Block30:  ;While body at line 131
	%reg_316 = load i32, ptr %reg_10
	%reg_317 = load i32, ptr %reg_12
	%reg_318 = getelementptr [270 x float], ptr %reg_6, i32 %reg_316, i32 %reg_317
	%reg_319 = load float, ptr %reg_48
	%reg_320 = load float, ptr %reg_26
	%reg_321 = fmul float %reg_319, %reg_320
	%reg_322 = load float, ptr %reg_50
	%reg_323 = load float, ptr %reg_28
	%reg_324 = fmul float %reg_322, %reg_323
	%reg_325 = fadd float %reg_321, %reg_324
	%reg_326 = load float, ptr %reg_52
	%reg_327 = load float, ptr %reg_30
	%reg_328 = fmul float %reg_326, %reg_327
	%reg_329 = fadd float %reg_325, %reg_328
	%reg_330 = load float, ptr %reg_54
	%reg_331 = load float, ptr %reg_32
	%reg_332 = fmul float %reg_330, %reg_331
	%reg_333 = fadd float %reg_329, %reg_332
	store float %reg_333, ptr %reg_318
	%reg_334 = load float, ptr %reg_26
	store float %reg_334, ptr %reg_28
	%reg_335 = load i32, ptr %reg_10
	%reg_336 = load i32, ptr %reg_12
	%reg_337 = getelementptr [270 x float], ptr %reg_4, i32 %reg_335, i32 %reg_336
	%reg_338 = load float, ptr %reg_337
	store float %reg_338, ptr %reg_26
	%reg_339 = load float, ptr %reg_30
	store float %reg_339, ptr %reg_32
	%reg_340 = load i32, ptr %reg_10
	%reg_341 = load i32, ptr %reg_12
	%reg_342 = getelementptr [270 x float], ptr %reg_6, i32 %reg_340, i32 %reg_341
	%reg_343 = load float, ptr %reg_342
	store float %reg_343, ptr %reg_30
	%reg_344 = load i32, ptr %reg_10
	%reg_345 = add i32 1, 0
	%reg_346 = sub i32 %reg_344, %reg_345
	store i32 %reg_346, ptr %reg_10
	br label %Block29
Block31:  ;While end at line 131
	%reg_347 = load i32, ptr %reg_12
	%reg_348 = add i32 1, 0
	%reg_349 = add i32 %reg_347, %reg_348
	store i32 %reg_349, ptr %reg_12
	br label %Block26
Block32:  ;While condition at line 143
	%reg_351 = load i32, ptr %reg_10
	%reg_352 = load i32, ptr %reg_7
	%reg_353 = icmp slt i32 %reg_351, %reg_352
	br i1 %reg_353, label %Block33, label %Block34
Block33:  ;While body at line 143
	%reg_354 = add i32 0, 0
	store i32 %reg_354, ptr %reg_12
	br label %Block35
Block34:  ;While end at line 143
	ret void
Block35:  ;While condition at line 145
	%reg_355 = load i32, ptr %reg_12
	%reg_356 = load i32, ptr %reg_8
	%reg_357 = icmp slt i32 %reg_355, %reg_356
	br i1 %reg_357, label %Block36, label %Block37
Block36:  ;While body at line 145
	%reg_358 = load i32, ptr %reg_10
	%reg_359 = load i32, ptr %reg_12
	%reg_360 = getelementptr [270 x float], ptr %reg_4, i32 %reg_358, i32 %reg_359
	%reg_361 = load float, ptr %reg_58
	%reg_362 = load i32, ptr %reg_10
	%reg_363 = load i32, ptr %reg_12
	%reg_364 = getelementptr [270 x float], ptr %reg_5, i32 %reg_362, i32 %reg_363
	%reg_365 = load float, ptr %reg_364
	%reg_366 = load i32, ptr %reg_10
	%reg_367 = load i32, ptr %reg_12
	%reg_368 = getelementptr [270 x float], ptr %reg_6, i32 %reg_366, i32 %reg_367
	%reg_369 = load float, ptr %reg_368
	%reg_370 = fadd float %reg_365, %reg_369
	%reg_371 = fmul float %reg_361, %reg_370
	store float %reg_371, ptr %reg_360
	%reg_372 = load i32, ptr %reg_12
	%reg_373 = add i32 1, 0
	%reg_374 = add i32 %reg_372, %reg_373
	store i32 %reg_374, ptr %reg_12
	br label %Block35
Block37:  ;While end at line 145
	%reg_375 = load i32, ptr %reg_10
	%reg_376 = add i32 1, 0
	%reg_377 = add i32 %reg_375, %reg_376
	store i32 %reg_377, ptr %reg_10
	br label %Block32
}

define i32 @main()
{
Block0:  ;Func define at line 153
	br label %Block1
Block1:  ;Func body at line 153
	%reg_0 = getelementptr [512 x [270 x float]], ptr @imgIn, i32 0
	%reg_1 = call i32 @getfarray(ptr %reg_0)
	%reg_2 = load i32, ptr @w
	%reg_3 = load i32, ptr @h
	%reg_4 = load float, ptr @alpha
	%reg_5 = getelementptr [512 x [270 x float]], ptr @imgIn, i32 0
	%reg_6 = getelementptr [512 x [270 x float]], ptr @imgOut, i32 0
	%reg_7 = getelementptr [512 x [270 x float]], ptr @my_y1, i32 0
	%reg_8 = getelementptr [512 x [270 x float]], ptr @my_y2, i32 0
	call void @kernel_deriche(i32 %reg_2, i32 %reg_3, float %reg_4, ptr %reg_5, ptr %reg_6, ptr %reg_7, ptr %reg_8)
	%reg_9 = load i32, ptr @w
	%reg_10 = load i32, ptr @h
	%reg_11 = mul i32 %reg_9, %reg_10
	%reg_12 = getelementptr [512 x [270 x float]], ptr @imgOut, i32 0
	call void @putfarray(i32 %reg_11, ptr %reg_12)
	%reg_13 = add i32 0, 0
	ret i32 %reg_13
}
