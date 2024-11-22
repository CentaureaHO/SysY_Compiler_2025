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

@f = global [20 x [20 x [20 x [20 x [20 x i32]]]]] zeroinitializer

define i32 @main()
{
Block0:  ;Func define at line 2
	%reg_6 = alloca i32
	%reg_2 = alloca [5 x i32]
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 2
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 20, i1 1)
	%reg_3 = add i32 0, 0
	%reg_4 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_3
	%reg_5 = add i32 1, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	br label %Block2
Block2:  ;While condition at line 8
	%reg_8 = add i32 0, 0
	%reg_9 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = load i32, ptr %reg_0
	%reg_12 = icmp slt i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block3, label %Block4
Block3:  ;While body at line 8
	%reg_13 = add i32 1, 0
	%reg_14 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_13
	%reg_15 = add i32 2, 0
	store i32 %reg_15, ptr %reg_14
	br label %Block5
Block4:  ;While end at line 8
	%reg_231 = add i32 5, 0
	%reg_232 = getelementptr [5 x i32], ptr %reg_2, i32 0
	call void @putarray(i32 %reg_231, ptr %reg_232)
	%reg_233 = load i32, ptr %reg_6
	call void @putint(i32 %reg_233)
	%reg_234 = add i32 10, 0
	call void @putch(i32 %reg_234)
	%reg_235 = add i32 0, 0
	ret i32 %reg_235
Block5:  ;While condition at line 10
	%reg_16 = add i32 1, 0
	%reg_17 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = load i32, ptr %reg_0
	%reg_20 = icmp slt i32 %reg_18, %reg_19
	br i1 %reg_20, label %Block6, label %Block7
Block6:  ;While body at line 10
	%reg_21 = add i32 2, 0
	%reg_22 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_21
	%reg_23 = add i32 1, 0
	store i32 %reg_23, ptr %reg_22
	br label %Block8
Block7:  ;While end at line 10
	%reg_224 = add i32 0, 0
	%reg_225 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_224
	%reg_226 = add i32 0, 0
	%reg_227 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_226
	%reg_228 = load i32, ptr %reg_227
	%reg_229 = add i32 1, 0
	%reg_230 = add i32 %reg_228, %reg_229
	store i32 %reg_230, ptr %reg_225
	br label %Block2
Block8:  ;While condition at line 12
	%reg_24 = add i32 2, 0
	%reg_25 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = load i32, ptr %reg_0
	%reg_28 = icmp slt i32 %reg_26, %reg_27
	br i1 %reg_28, label %Block9, label %Block10
Block9:  ;While body at line 12
	%reg_29 = add i32 3, 0
	%reg_30 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_29
	%reg_31 = add i32 3, 0
	store i32 %reg_31, ptr %reg_30
	br label %Block11
Block10:  ;While end at line 12
	%reg_217 = add i32 1, 0
	%reg_218 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_217
	%reg_219 = add i32 1, 0
	%reg_220 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_219
	%reg_221 = load i32, ptr %reg_220
	%reg_222 = add i32 1, 0
	%reg_223 = add i32 %reg_221, %reg_222
	store i32 %reg_223, ptr %reg_218
	br label %Block5
Block11:  ;While condition at line 14
	%reg_32 = add i32 3, 0
	%reg_33 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = load i32, ptr %reg_0
	%reg_36 = icmp slt i32 %reg_34, %reg_35
	br i1 %reg_36, label %Block12, label %Block13
Block12:  ;While body at line 14
	%reg_37 = add i32 4, 0
	%reg_38 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_37
	%reg_39 = add i32 1, 0
	store i32 %reg_39, ptr %reg_38
	br label %Block14
Block13:  ;While end at line 14
	%reg_210 = add i32 2, 0
	%reg_211 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_210
	%reg_212 = add i32 2, 0
	%reg_213 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_212
	%reg_214 = load i32, ptr %reg_213
	%reg_215 = add i32 1, 0
	%reg_216 = add i32 %reg_214, %reg_215
	store i32 %reg_216, ptr %reg_211
	br label %Block8
Block14:  ;While condition at line 16
	%reg_40 = add i32 4, 0
	%reg_41 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	%reg_43 = load i32, ptr %reg_0
	%reg_44 = icmp slt i32 %reg_42, %reg_43
	br i1 %reg_44, label %Block15, label %Block16
Block15:  ;While body at line 16
	%reg_45 = add i32 0, 0
	%reg_46 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	%reg_48 = add i32 1, 0
	%reg_49 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	%reg_51 = add i32 2, 0
	%reg_52 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_51
	%reg_53 = load i32, ptr %reg_52
	%reg_54 = add i32 3, 0
	%reg_55 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	%reg_57 = add i32 4, 0
	%reg_58 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_57
	%reg_59 = load i32, ptr %reg_58
	%reg_60 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_47, i32 %reg_50, i32 %reg_53, i32 %reg_56, i32 %reg_59
	%reg_61 = add i32 0, 0
	%reg_62 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_61
	%reg_63 = load i32, ptr %reg_62
	%reg_64 = add i32 1, 0
	%reg_65 = sub i32 %reg_63, %reg_64
	%reg_66 = add i32 1, 0
	%reg_67 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	%reg_69 = add i32 2, 0
	%reg_70 = sub i32 %reg_68, %reg_69
	%reg_71 = add i32 2, 0
	%reg_72 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_71
	%reg_73 = load i32, ptr %reg_72
	%reg_74 = add i32 1, 0
	%reg_75 = sub i32 %reg_73, %reg_74
	%reg_76 = add i32 3, 0
	%reg_77 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_76
	%reg_78 = load i32, ptr %reg_77
	%reg_79 = add i32 3, 0
	%reg_80 = sub i32 %reg_78, %reg_79
	%reg_81 = add i32 4, 0
	%reg_82 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_81
	%reg_83 = load i32, ptr %reg_82
	%reg_84 = add i32 1, 0
	%reg_85 = sub i32 %reg_83, %reg_84
	%reg_86 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_65, i32 %reg_70, i32 %reg_75, i32 %reg_80, i32 %reg_85
	%reg_87 = load i32, ptr %reg_86
	%reg_88 = add i32 0, 0
	%reg_89 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_88
	%reg_90 = load i32, ptr %reg_89
	%reg_91 = add i32 1, 0
	%reg_92 = sub i32 %reg_90, %reg_91
	%reg_93 = add i32 1, 0
	%reg_94 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_93
	%reg_95 = load i32, ptr %reg_94
	%reg_96 = add i32 1, 0
	%reg_97 = sub i32 %reg_95, %reg_96
	%reg_98 = add i32 2, 0
	%reg_99 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_98
	%reg_100 = load i32, ptr %reg_99
	%reg_101 = add i32 1, 0
	%reg_102 = sub i32 %reg_100, %reg_101
	%reg_103 = add i32 3, 0
	%reg_104 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_103
	%reg_105 = load i32, ptr %reg_104
	%reg_106 = add i32 1, 0
	%reg_107 = sub i32 %reg_105, %reg_106
	%reg_108 = add i32 4, 0
	%reg_109 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_108
	%reg_110 = load i32, ptr %reg_109
	%reg_111 = add i32 1, 0
	%reg_112 = sub i32 %reg_110, %reg_111
	%reg_113 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_92, i32 %reg_97, i32 %reg_102, i32 %reg_107, i32 %reg_112
	%reg_114 = load i32, ptr %reg_113
	%reg_115 = add i32 %reg_87, %reg_114
	%reg_116 = add i32 0, 0
	%reg_117 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_116
	%reg_118 = load i32, ptr %reg_117
	%reg_119 = add i32 1, 0
	%reg_120 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_119
	%reg_121 = load i32, ptr %reg_120
	%reg_122 = mul i32 %reg_118, %reg_121
	%reg_123 = add i32 %reg_115, %reg_122
	%reg_124 = add i32 2, 0
	%reg_125 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_124
	%reg_126 = load i32, ptr %reg_125
	%reg_127 = add i32 %reg_123, %reg_126
	%reg_128 = add i32 3, 0
	%reg_129 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_128
	%reg_130 = load i32, ptr %reg_129
	%reg_131 = add i32 %reg_127, %reg_130
	%reg_132 = add i32 4, 0
	%reg_133 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_132
	%reg_134 = load i32, ptr %reg_133
	%reg_135 = add i32 %reg_131, %reg_134
	store i32 %reg_135, ptr %reg_60
	%reg_136 = add i32 0, 0
	%reg_137 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_136
	%reg_138 = load i32, ptr %reg_137
	%reg_139 = add i32 1, 0
	%reg_140 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_139
	%reg_141 = load i32, ptr %reg_140
	%reg_142 = add i32 2, 0
	%reg_143 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_142
	%reg_144 = load i32, ptr %reg_143
	%reg_145 = add i32 3, 0
	%reg_146 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_145
	%reg_147 = load i32, ptr %reg_146
	%reg_148 = add i32 4, 0
	%reg_149 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_148
	%reg_150 = load i32, ptr %reg_149
	%reg_151 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_138, i32 %reg_141, i32 %reg_144, i32 %reg_147, i32 %reg_150
	%reg_152 = add i32 0, 0
	%reg_153 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_152
	%reg_154 = load i32, ptr %reg_153
	%reg_155 = add i32 1, 0
	%reg_156 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_155
	%reg_157 = load i32, ptr %reg_156
	%reg_158 = add i32 2, 0
	%reg_159 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_158
	%reg_160 = load i32, ptr %reg_159
	%reg_161 = add i32 3, 0
	%reg_162 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_161
	%reg_163 = load i32, ptr %reg_162
	%reg_164 = add i32 4, 0
	%reg_165 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_164
	%reg_166 = load i32, ptr %reg_165
	%reg_167 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_154, i32 %reg_157, i32 %reg_160, i32 %reg_163, i32 %reg_166
	%reg_168 = load i32, ptr %reg_167
	%reg_169 = add i32 998244353, 0
	%reg_170 = srem i32 %reg_168, %reg_169
	store i32 %reg_170, ptr %reg_151
	%reg_171 = load i32, ptr %reg_6
	%reg_172 = add i32 0, 0
	%reg_173 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_172
	%reg_174 = load i32, ptr %reg_173
	%reg_175 = add i32 1, 0
	%reg_176 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_175
	%reg_177 = load i32, ptr %reg_176
	%reg_178 = add i32 2, 0
	%reg_179 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_178
	%reg_180 = load i32, ptr %reg_179
	%reg_181 = add i32 3, 0
	%reg_182 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_181
	%reg_183 = load i32, ptr %reg_182
	%reg_184 = add i32 4, 0
	%reg_185 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_184
	%reg_186 = load i32, ptr %reg_185
	%reg_187 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_174, i32 %reg_177, i32 %reg_180, i32 %reg_183, i32 %reg_186
	%reg_188 = load i32, ptr %reg_187
	%reg_189 = add i32 %reg_171, %reg_188
	store i32 %reg_189, ptr %reg_6
	%reg_190 = load i32, ptr %reg_6
	%reg_191 = add i32 998244353, 0
	%reg_192 = icmp sgt i32 %reg_190, %reg_191
	br i1 %reg_192, label %Block17, label %Block18
Block16:  ;While end at line 16
	%reg_203 = add i32 3, 0
	%reg_204 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_203
	%reg_205 = add i32 3, 0
	%reg_206 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_205
	%reg_207 = load i32, ptr %reg_206
	%reg_208 = add i32 1, 0
	%reg_209 = add i32 %reg_207, %reg_208
	store i32 %reg_209, ptr %reg_204
	br label %Block11
Block17:  ;If then at line 23
	%reg_193 = load i32, ptr %reg_6
	%reg_194 = add i32 998244353, 0
	%reg_195 = sub i32 %reg_193, %reg_194
	store i32 %reg_195, ptr %reg_6
	br label %Block18
Block18:  ;If end at line 23
	%reg_196 = add i32 4, 0
	%reg_197 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_196
	%reg_198 = add i32 4, 0
	%reg_199 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 %reg_198
	%reg_200 = load i32, ptr %reg_199
	%reg_201 = add i32 1, 0
	%reg_202 = add i32 %reg_200, %reg_201
	store i32 %reg_202, ptr %reg_197
	br label %Block14
}
