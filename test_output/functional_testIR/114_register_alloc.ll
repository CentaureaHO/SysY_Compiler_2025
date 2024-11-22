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

@a1 = global i32 1
@a2 = global i32 2
@a3 = global i32 3
@a4 = global i32 4
@a5 = global i32 5
@a6 = global i32 6
@a7 = global i32 7
@a8 = global i32 8
@a9 = global i32 9
@a10 = global i32 10
@a11 = global i32 11
@a12 = global i32 12
@a13 = global i32 13
@a14 = global i32 14
@a15 = global i32 15
@a16 = global i32 16
@a17 = global i32 1
@a18 = global i32 2
@a19 = global i32 3
@a20 = global i32 4
@a21 = global i32 5
@a22 = global i32 6
@a23 = global i32 7
@a24 = global i32 8
@a25 = global i32 9
@a26 = global i32 10
@a27 = global i32 11
@a28 = global i32 12
@a29 = global i32 13
@a30 = global i32 14
@a31 = global i32 15
@a32 = global i32 16

define i32 @func(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 34
	%reg_79 = alloca i32
	%reg_77 = alloca i32
	%reg_75 = alloca i32
	%reg_73 = alloca i32
	%reg_71 = alloca i32
	%reg_69 = alloca i32
	%reg_67 = alloca i32
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
	%reg_31 = alloca i32
	%reg_29 = alloca i32
	%reg_27 = alloca i32
	%reg_25 = alloca i32
	%reg_23 = alloca i32
	%reg_21 = alloca i32
	%reg_19 = alloca i32
	%reg_17 = alloca i32
	%reg_15 = alloca i32
	%reg_13 = alloca i32
	%reg_11 = alloca i32
	%reg_9 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 34
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = add i32 %reg_6, %reg_7
	store i32 %reg_8, ptr %reg_4
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_11
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_13
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_17
	%reg_20 = add i32 0, 0
	store i32 %reg_20, ptr %reg_19
	%reg_22 = add i32 0, 0
	store i32 %reg_22, ptr %reg_21
	%reg_24 = add i32 0, 0
	store i32 %reg_24, ptr %reg_23
	%reg_26 = add i32 0, 0
	store i32 %reg_26, ptr %reg_25
	%reg_28 = add i32 0, 0
	store i32 %reg_28, ptr %reg_27
	%reg_30 = add i32 0, 0
	store i32 %reg_30, ptr %reg_29
	%reg_32 = add i32 0, 0
	store i32 %reg_32, ptr %reg_31
	%reg_34 = add i32 0, 0
	store i32 %reg_34, ptr %reg_33
	%reg_36 = add i32 0, 0
	store i32 %reg_36, ptr %reg_35
	%reg_38 = add i32 0, 0
	store i32 %reg_38, ptr %reg_37
	%reg_40 = add i32 0, 0
	store i32 %reg_40, ptr %reg_39
	%reg_42 = add i32 0, 0
	store i32 %reg_42, ptr %reg_41
	%reg_44 = add i32 0, 0
	store i32 %reg_44, ptr %reg_43
	%reg_46 = add i32 0, 0
	store i32 %reg_46, ptr %reg_45
	%reg_48 = add i32 0, 0
	store i32 %reg_48, ptr %reg_47
	%reg_50 = add i32 0, 0
	store i32 %reg_50, ptr %reg_49
	%reg_52 = add i32 0, 0
	store i32 %reg_52, ptr %reg_51
	%reg_54 = add i32 0, 0
	store i32 %reg_54, ptr %reg_53
	%reg_56 = add i32 0, 0
	store i32 %reg_56, ptr %reg_55
	%reg_58 = add i32 0, 0
	store i32 %reg_58, ptr %reg_57
	%reg_60 = add i32 0, 0
	store i32 %reg_60, ptr %reg_59
	%reg_62 = add i32 0, 0
	store i32 %reg_62, ptr %reg_61
	%reg_64 = add i32 0, 0
	store i32 %reg_64, ptr %reg_63
	%reg_66 = add i32 0, 0
	store i32 %reg_66, ptr %reg_65
	%reg_68 = add i32 0, 0
	store i32 %reg_68, ptr %reg_67
	%reg_70 = add i32 0, 0
	store i32 %reg_70, ptr %reg_69
	%reg_72 = add i32 0, 0
	store i32 %reg_72, ptr %reg_71
	%reg_74 = add i32 0, 0
	store i32 %reg_74, ptr %reg_73
	%reg_76 = add i32 0, 0
	store i32 %reg_76, ptr %reg_75
	%reg_78 = add i32 0, 0
	store i32 %reg_78, ptr %reg_77
	%reg_80 = add i32 0, 0
	store i32 %reg_80, ptr %reg_79
	%reg_81 = call i32 @getint()
	store i32 %reg_81, ptr %reg_9
	%reg_82 = call i32 @getint()
	store i32 %reg_82, ptr %reg_11
	%reg_83 = call i32 @getint()
	store i32 %reg_83, ptr %reg_13
	%reg_84 = call i32 @getint()
	store i32 %reg_84, ptr %reg_15
	%reg_85 = add i32 1, 0
	%reg_86 = load i32, ptr %reg_9
	%reg_87 = add i32 %reg_85, %reg_86
	%reg_88 = load i32, ptr @a1
	%reg_89 = add i32 %reg_87, %reg_88
	store i32 %reg_89, ptr %reg_17
	%reg_90 = add i32 2, 0
	%reg_91 = load i32, ptr %reg_11
	%reg_92 = add i32 %reg_90, %reg_91
	%reg_93 = load i32, ptr @a2
	%reg_94 = add i32 %reg_92, %reg_93
	store i32 %reg_94, ptr %reg_19
	%reg_95 = add i32 3, 0
	%reg_96 = load i32, ptr %reg_13
	%reg_97 = add i32 %reg_95, %reg_96
	%reg_98 = load i32, ptr @a3
	%reg_99 = add i32 %reg_97, %reg_98
	store i32 %reg_99, ptr %reg_21
	%reg_100 = add i32 4, 0
	%reg_101 = load i32, ptr %reg_15
	%reg_102 = add i32 %reg_100, %reg_101
	%reg_103 = load i32, ptr @a4
	%reg_104 = add i32 %reg_102, %reg_103
	store i32 %reg_104, ptr %reg_23
	%reg_105 = add i32 1, 0
	%reg_106 = load i32, ptr %reg_17
	%reg_107 = add i32 %reg_105, %reg_106
	%reg_108 = load i32, ptr @a5
	%reg_109 = add i32 %reg_107, %reg_108
	store i32 %reg_109, ptr %reg_25
	%reg_110 = add i32 2, 0
	%reg_111 = load i32, ptr %reg_19
	%reg_112 = add i32 %reg_110, %reg_111
	%reg_113 = load i32, ptr @a6
	%reg_114 = add i32 %reg_112, %reg_113
	store i32 %reg_114, ptr %reg_27
	%reg_115 = add i32 3, 0
	%reg_116 = load i32, ptr %reg_21
	%reg_117 = add i32 %reg_115, %reg_116
	%reg_118 = load i32, ptr @a7
	%reg_119 = add i32 %reg_117, %reg_118
	store i32 %reg_119, ptr %reg_29
	%reg_120 = add i32 4, 0
	%reg_121 = load i32, ptr %reg_23
	%reg_122 = add i32 %reg_120, %reg_121
	%reg_123 = load i32, ptr @a8
	%reg_124 = add i32 %reg_122, %reg_123
	store i32 %reg_124, ptr %reg_31
	%reg_125 = add i32 1, 0
	%reg_126 = load i32, ptr %reg_25
	%reg_127 = add i32 %reg_125, %reg_126
	%reg_128 = load i32, ptr @a9
	%reg_129 = add i32 %reg_127, %reg_128
	store i32 %reg_129, ptr %reg_33
	%reg_130 = add i32 2, 0
	%reg_131 = load i32, ptr %reg_27
	%reg_132 = add i32 %reg_130, %reg_131
	%reg_133 = load i32, ptr @a10
	%reg_134 = add i32 %reg_132, %reg_133
	store i32 %reg_134, ptr %reg_35
	%reg_135 = add i32 3, 0
	%reg_136 = load i32, ptr %reg_29
	%reg_137 = add i32 %reg_135, %reg_136
	%reg_138 = load i32, ptr @a11
	%reg_139 = add i32 %reg_137, %reg_138
	store i32 %reg_139, ptr %reg_37
	%reg_140 = add i32 4, 0
	%reg_141 = load i32, ptr %reg_31
	%reg_142 = add i32 %reg_140, %reg_141
	%reg_143 = load i32, ptr @a12
	%reg_144 = add i32 %reg_142, %reg_143
	store i32 %reg_144, ptr %reg_39
	%reg_145 = add i32 1, 0
	%reg_146 = load i32, ptr %reg_33
	%reg_147 = add i32 %reg_145, %reg_146
	%reg_148 = load i32, ptr @a13
	%reg_149 = add i32 %reg_147, %reg_148
	store i32 %reg_149, ptr %reg_41
	%reg_150 = add i32 2, 0
	%reg_151 = load i32, ptr %reg_35
	%reg_152 = add i32 %reg_150, %reg_151
	%reg_153 = load i32, ptr @a14
	%reg_154 = add i32 %reg_152, %reg_153
	store i32 %reg_154, ptr %reg_43
	%reg_155 = add i32 3, 0
	%reg_156 = load i32, ptr %reg_37
	%reg_157 = add i32 %reg_155, %reg_156
	%reg_158 = load i32, ptr @a15
	%reg_159 = add i32 %reg_157, %reg_158
	store i32 %reg_159, ptr %reg_45
	%reg_160 = add i32 4, 0
	%reg_161 = load i32, ptr %reg_39
	%reg_162 = add i32 %reg_160, %reg_161
	%reg_163 = load i32, ptr @a16
	%reg_164 = add i32 %reg_162, %reg_163
	store i32 %reg_164, ptr %reg_47
	%reg_165 = add i32 1, 0
	%reg_166 = load i32, ptr %reg_41
	%reg_167 = add i32 %reg_165, %reg_166
	%reg_168 = load i32, ptr @a17
	%reg_169 = add i32 %reg_167, %reg_168
	store i32 %reg_169, ptr %reg_49
	%reg_170 = add i32 2, 0
	%reg_171 = load i32, ptr %reg_43
	%reg_172 = add i32 %reg_170, %reg_171
	%reg_173 = load i32, ptr @a18
	%reg_174 = add i32 %reg_172, %reg_173
	store i32 %reg_174, ptr %reg_51
	%reg_175 = add i32 3, 0
	%reg_176 = load i32, ptr %reg_45
	%reg_177 = add i32 %reg_175, %reg_176
	%reg_178 = load i32, ptr @a19
	%reg_179 = add i32 %reg_177, %reg_178
	store i32 %reg_179, ptr %reg_53
	%reg_180 = add i32 4, 0
	%reg_181 = load i32, ptr %reg_47
	%reg_182 = add i32 %reg_180, %reg_181
	%reg_183 = load i32, ptr @a20
	%reg_184 = add i32 %reg_182, %reg_183
	store i32 %reg_184, ptr %reg_55
	%reg_185 = add i32 1, 0
	%reg_186 = load i32, ptr %reg_49
	%reg_187 = add i32 %reg_185, %reg_186
	%reg_188 = load i32, ptr @a21
	%reg_189 = add i32 %reg_187, %reg_188
	store i32 %reg_189, ptr %reg_57
	%reg_190 = add i32 2, 0
	%reg_191 = load i32, ptr %reg_51
	%reg_192 = add i32 %reg_190, %reg_191
	%reg_193 = load i32, ptr @a22
	%reg_194 = add i32 %reg_192, %reg_193
	store i32 %reg_194, ptr %reg_59
	%reg_195 = add i32 3, 0
	%reg_196 = load i32, ptr %reg_53
	%reg_197 = add i32 %reg_195, %reg_196
	%reg_198 = load i32, ptr @a23
	%reg_199 = add i32 %reg_197, %reg_198
	store i32 %reg_199, ptr %reg_61
	%reg_200 = add i32 4, 0
	%reg_201 = load i32, ptr %reg_55
	%reg_202 = add i32 %reg_200, %reg_201
	%reg_203 = load i32, ptr @a24
	%reg_204 = add i32 %reg_202, %reg_203
	store i32 %reg_204, ptr %reg_63
	%reg_205 = add i32 1, 0
	%reg_206 = load i32, ptr %reg_57
	%reg_207 = add i32 %reg_205, %reg_206
	%reg_208 = load i32, ptr @a25
	%reg_209 = add i32 %reg_207, %reg_208
	store i32 %reg_209, ptr %reg_65
	%reg_210 = add i32 2, 0
	%reg_211 = load i32, ptr %reg_59
	%reg_212 = add i32 %reg_210, %reg_211
	%reg_213 = load i32, ptr @a26
	%reg_214 = add i32 %reg_212, %reg_213
	store i32 %reg_214, ptr %reg_67
	%reg_215 = add i32 3, 0
	%reg_216 = load i32, ptr %reg_61
	%reg_217 = add i32 %reg_215, %reg_216
	%reg_218 = load i32, ptr @a27
	%reg_219 = add i32 %reg_217, %reg_218
	store i32 %reg_219, ptr %reg_69
	%reg_220 = add i32 4, 0
	%reg_221 = load i32, ptr %reg_63
	%reg_222 = add i32 %reg_220, %reg_221
	%reg_223 = load i32, ptr @a28
	%reg_224 = add i32 %reg_222, %reg_223
	store i32 %reg_224, ptr %reg_71
	%reg_225 = add i32 1, 0
	%reg_226 = load i32, ptr %reg_65
	%reg_227 = add i32 %reg_225, %reg_226
	%reg_228 = load i32, ptr @a29
	%reg_229 = add i32 %reg_227, %reg_228
	store i32 %reg_229, ptr %reg_73
	%reg_230 = add i32 2, 0
	%reg_231 = load i32, ptr %reg_67
	%reg_232 = add i32 %reg_230, %reg_231
	%reg_233 = load i32, ptr @a30
	%reg_234 = add i32 %reg_232, %reg_233
	store i32 %reg_234, ptr %reg_75
	%reg_235 = add i32 3, 0
	%reg_236 = load i32, ptr %reg_69
	%reg_237 = add i32 %reg_235, %reg_236
	%reg_238 = load i32, ptr @a31
	%reg_239 = add i32 %reg_237, %reg_238
	store i32 %reg_239, ptr %reg_77
	%reg_240 = add i32 4, 0
	%reg_241 = load i32, ptr %reg_71
	%reg_242 = add i32 %reg_240, %reg_241
	%reg_243 = load i32, ptr @a32
	%reg_244 = add i32 %reg_242, %reg_243
	store i32 %reg_244, ptr %reg_79
	%reg_245 = load i32, ptr %reg_2
	%reg_246 = load i32, ptr %reg_3
	%reg_247 = sub i32 %reg_245, %reg_246
	%reg_248 = add i32 10, 0
	%reg_249 = add i32 %reg_247, %reg_248
	store i32 %reg_249, ptr %reg_4
	%reg_250 = add i32 1, 0
	%reg_251 = load i32, ptr %reg_65
	%reg_252 = add i32 %reg_250, %reg_251
	%reg_253 = load i32, ptr @a29
	%reg_254 = add i32 %reg_252, %reg_253
	store i32 %reg_254, ptr %reg_73
	%reg_255 = add i32 2, 0
	%reg_256 = load i32, ptr %reg_67
	%reg_257 = add i32 %reg_255, %reg_256
	%reg_258 = load i32, ptr @a30
	%reg_259 = add i32 %reg_257, %reg_258
	store i32 %reg_259, ptr %reg_75
	%reg_260 = add i32 3, 0
	%reg_261 = load i32, ptr %reg_69
	%reg_262 = add i32 %reg_260, %reg_261
	%reg_263 = load i32, ptr @a31
	%reg_264 = add i32 %reg_262, %reg_263
	store i32 %reg_264, ptr %reg_77
	%reg_265 = add i32 4, 0
	%reg_266 = load i32, ptr %reg_71
	%reg_267 = add i32 %reg_265, %reg_266
	%reg_268 = load i32, ptr @a32
	%reg_269 = add i32 %reg_267, %reg_268
	store i32 %reg_269, ptr %reg_79
	%reg_270 = add i32 1, 0
	%reg_271 = load i32, ptr %reg_57
	%reg_272 = add i32 %reg_270, %reg_271
	%reg_273 = load i32, ptr @a25
	%reg_274 = add i32 %reg_272, %reg_273
	store i32 %reg_274, ptr %reg_65
	%reg_275 = add i32 2, 0
	%reg_276 = load i32, ptr %reg_59
	%reg_277 = add i32 %reg_275, %reg_276
	%reg_278 = load i32, ptr @a26
	%reg_279 = add i32 %reg_277, %reg_278
	store i32 %reg_279, ptr %reg_67
	%reg_280 = add i32 3, 0
	%reg_281 = load i32, ptr %reg_61
	%reg_282 = add i32 %reg_280, %reg_281
	%reg_283 = load i32, ptr @a27
	%reg_284 = add i32 %reg_282, %reg_283
	store i32 %reg_284, ptr %reg_69
	%reg_285 = add i32 4, 0
	%reg_286 = load i32, ptr %reg_63
	%reg_287 = add i32 %reg_285, %reg_286
	%reg_288 = load i32, ptr @a28
	%reg_289 = add i32 %reg_287, %reg_288
	store i32 %reg_289, ptr %reg_71
	%reg_290 = add i32 1, 0
	%reg_291 = load i32, ptr %reg_49
	%reg_292 = add i32 %reg_290, %reg_291
	%reg_293 = load i32, ptr @a21
	%reg_294 = add i32 %reg_292, %reg_293
	store i32 %reg_294, ptr %reg_57
	%reg_295 = add i32 2, 0
	%reg_296 = load i32, ptr %reg_51
	%reg_297 = add i32 %reg_295, %reg_296
	%reg_298 = load i32, ptr @a22
	%reg_299 = add i32 %reg_297, %reg_298
	store i32 %reg_299, ptr %reg_59
	%reg_300 = add i32 3, 0
	%reg_301 = load i32, ptr %reg_53
	%reg_302 = add i32 %reg_300, %reg_301
	%reg_303 = load i32, ptr @a23
	%reg_304 = add i32 %reg_302, %reg_303
	store i32 %reg_304, ptr %reg_61
	%reg_305 = add i32 4, 0
	%reg_306 = load i32, ptr %reg_55
	%reg_307 = add i32 %reg_305, %reg_306
	%reg_308 = load i32, ptr @a24
	%reg_309 = add i32 %reg_307, %reg_308
	store i32 %reg_309, ptr %reg_63
	%reg_310 = add i32 1, 0
	%reg_311 = load i32, ptr %reg_41
	%reg_312 = add i32 %reg_310, %reg_311
	%reg_313 = load i32, ptr @a17
	%reg_314 = add i32 %reg_312, %reg_313
	store i32 %reg_314, ptr %reg_49
	%reg_315 = add i32 2, 0
	%reg_316 = load i32, ptr %reg_43
	%reg_317 = add i32 %reg_315, %reg_316
	%reg_318 = load i32, ptr @a18
	%reg_319 = add i32 %reg_317, %reg_318
	store i32 %reg_319, ptr %reg_51
	%reg_320 = add i32 3, 0
	%reg_321 = load i32, ptr %reg_45
	%reg_322 = add i32 %reg_320, %reg_321
	%reg_323 = load i32, ptr @a19
	%reg_324 = add i32 %reg_322, %reg_323
	store i32 %reg_324, ptr %reg_53
	%reg_325 = add i32 4, 0
	%reg_326 = load i32, ptr %reg_47
	%reg_327 = add i32 %reg_325, %reg_326
	%reg_328 = load i32, ptr @a20
	%reg_329 = add i32 %reg_327, %reg_328
	store i32 %reg_329, ptr %reg_55
	%reg_330 = add i32 1, 0
	%reg_331 = load i32, ptr %reg_33
	%reg_332 = add i32 %reg_330, %reg_331
	%reg_333 = load i32, ptr @a13
	%reg_334 = add i32 %reg_332, %reg_333
	store i32 %reg_334, ptr %reg_41
	%reg_335 = add i32 2, 0
	%reg_336 = load i32, ptr %reg_35
	%reg_337 = add i32 %reg_335, %reg_336
	%reg_338 = load i32, ptr @a14
	%reg_339 = add i32 %reg_337, %reg_338
	store i32 %reg_339, ptr %reg_43
	%reg_340 = add i32 3, 0
	%reg_341 = load i32, ptr %reg_37
	%reg_342 = add i32 %reg_340, %reg_341
	%reg_343 = load i32, ptr @a15
	%reg_344 = add i32 %reg_342, %reg_343
	store i32 %reg_344, ptr %reg_45
	%reg_345 = add i32 4, 0
	%reg_346 = load i32, ptr %reg_39
	%reg_347 = add i32 %reg_345, %reg_346
	%reg_348 = load i32, ptr @a16
	%reg_349 = add i32 %reg_347, %reg_348
	store i32 %reg_349, ptr %reg_47
	%reg_350 = add i32 1, 0
	%reg_351 = load i32, ptr %reg_25
	%reg_352 = add i32 %reg_350, %reg_351
	%reg_353 = load i32, ptr @a9
	%reg_354 = add i32 %reg_352, %reg_353
	store i32 %reg_354, ptr %reg_33
	%reg_355 = add i32 2, 0
	%reg_356 = load i32, ptr %reg_27
	%reg_357 = add i32 %reg_355, %reg_356
	%reg_358 = load i32, ptr @a10
	%reg_359 = add i32 %reg_357, %reg_358
	store i32 %reg_359, ptr %reg_35
	%reg_360 = add i32 3, 0
	%reg_361 = load i32, ptr %reg_29
	%reg_362 = add i32 %reg_360, %reg_361
	%reg_363 = load i32, ptr @a11
	%reg_364 = add i32 %reg_362, %reg_363
	store i32 %reg_364, ptr %reg_37
	%reg_365 = add i32 4, 0
	%reg_366 = load i32, ptr %reg_31
	%reg_367 = add i32 %reg_365, %reg_366
	%reg_368 = load i32, ptr @a12
	%reg_369 = add i32 %reg_367, %reg_368
	store i32 %reg_369, ptr %reg_39
	%reg_370 = add i32 1, 0
	%reg_371 = load i32, ptr %reg_17
	%reg_372 = add i32 %reg_370, %reg_371
	%reg_373 = load i32, ptr @a5
	%reg_374 = add i32 %reg_372, %reg_373
	store i32 %reg_374, ptr %reg_25
	%reg_375 = add i32 2, 0
	%reg_376 = load i32, ptr %reg_19
	%reg_377 = add i32 %reg_375, %reg_376
	%reg_378 = load i32, ptr @a6
	%reg_379 = add i32 %reg_377, %reg_378
	store i32 %reg_379, ptr %reg_27
	%reg_380 = add i32 3, 0
	%reg_381 = load i32, ptr %reg_21
	%reg_382 = add i32 %reg_380, %reg_381
	%reg_383 = load i32, ptr @a7
	%reg_384 = add i32 %reg_382, %reg_383
	store i32 %reg_384, ptr %reg_29
	%reg_385 = add i32 4, 0
	%reg_386 = load i32, ptr %reg_23
	%reg_387 = add i32 %reg_385, %reg_386
	%reg_388 = load i32, ptr @a8
	%reg_389 = add i32 %reg_387, %reg_388
	store i32 %reg_389, ptr %reg_31
	%reg_390 = add i32 1, 0
	%reg_391 = load i32, ptr %reg_9
	%reg_392 = add i32 %reg_390, %reg_391
	%reg_393 = load i32, ptr @a1
	%reg_394 = add i32 %reg_392, %reg_393
	store i32 %reg_394, ptr %reg_17
	%reg_395 = add i32 2, 0
	%reg_396 = load i32, ptr %reg_11
	%reg_397 = add i32 %reg_395, %reg_396
	%reg_398 = load i32, ptr @a2
	%reg_399 = add i32 %reg_397, %reg_398
	store i32 %reg_399, ptr %reg_19
	%reg_400 = add i32 3, 0
	%reg_401 = load i32, ptr %reg_13
	%reg_402 = add i32 %reg_400, %reg_401
	%reg_403 = load i32, ptr @a3
	%reg_404 = add i32 %reg_402, %reg_403
	store i32 %reg_404, ptr %reg_21
	%reg_405 = add i32 4, 0
	%reg_406 = load i32, ptr %reg_15
	%reg_407 = add i32 %reg_405, %reg_406
	%reg_408 = load i32, ptr @a4
	%reg_409 = add i32 %reg_407, %reg_408
	store i32 %reg_409, ptr %reg_23
	%reg_410 = add i32 1, 0
	%reg_411 = load i32, ptr %reg_9
	%reg_412 = add i32 %reg_410, %reg_411
	%reg_413 = load i32, ptr @a1
	%reg_414 = add i32 %reg_412, %reg_413
	store i32 %reg_414, ptr %reg_17
	%reg_415 = add i32 2, 0
	%reg_416 = load i32, ptr %reg_11
	%reg_417 = add i32 %reg_415, %reg_416
	%reg_418 = load i32, ptr @a2
	%reg_419 = add i32 %reg_417, %reg_418
	store i32 %reg_419, ptr %reg_19
	%reg_420 = add i32 3, 0
	%reg_421 = load i32, ptr %reg_13
	%reg_422 = add i32 %reg_420, %reg_421
	%reg_423 = load i32, ptr @a3
	%reg_424 = add i32 %reg_422, %reg_423
	store i32 %reg_424, ptr %reg_21
	%reg_425 = add i32 4, 0
	%reg_426 = load i32, ptr %reg_15
	%reg_427 = add i32 %reg_425, %reg_426
	%reg_428 = load i32, ptr @a4
	%reg_429 = add i32 %reg_427, %reg_428
	store i32 %reg_429, ptr %reg_23
	%reg_430 = load i32, ptr %reg_4
	%reg_431 = load i32, ptr %reg_9
	%reg_432 = add i32 %reg_430, %reg_431
	%reg_433 = load i32, ptr %reg_11
	%reg_434 = add i32 %reg_432, %reg_433
	%reg_435 = load i32, ptr %reg_13
	%reg_436 = add i32 %reg_434, %reg_435
	%reg_437 = load i32, ptr %reg_15
	%reg_438 = add i32 %reg_436, %reg_437
	%reg_439 = load i32, ptr %reg_17
	%reg_440 = sub i32 %reg_438, %reg_439
	%reg_441 = load i32, ptr %reg_19
	%reg_442 = sub i32 %reg_440, %reg_441
	%reg_443 = load i32, ptr %reg_21
	%reg_444 = sub i32 %reg_442, %reg_443
	%reg_445 = load i32, ptr %reg_23
	%reg_446 = sub i32 %reg_444, %reg_445
	%reg_447 = load i32, ptr %reg_25
	%reg_448 = add i32 %reg_446, %reg_447
	%reg_449 = load i32, ptr %reg_27
	%reg_450 = add i32 %reg_448, %reg_449
	%reg_451 = load i32, ptr %reg_29
	%reg_452 = add i32 %reg_450, %reg_451
	%reg_453 = load i32, ptr %reg_31
	%reg_454 = add i32 %reg_452, %reg_453
	%reg_455 = load i32, ptr %reg_33
	%reg_456 = sub i32 %reg_454, %reg_455
	%reg_457 = load i32, ptr %reg_35
	%reg_458 = sub i32 %reg_456, %reg_457
	%reg_459 = load i32, ptr %reg_37
	%reg_460 = sub i32 %reg_458, %reg_459
	%reg_461 = load i32, ptr %reg_39
	%reg_462 = sub i32 %reg_460, %reg_461
	%reg_463 = load i32, ptr %reg_41
	%reg_464 = add i32 %reg_462, %reg_463
	%reg_465 = load i32, ptr %reg_43
	%reg_466 = add i32 %reg_464, %reg_465
	%reg_467 = load i32, ptr %reg_45
	%reg_468 = add i32 %reg_466, %reg_467
	%reg_469 = load i32, ptr %reg_47
	%reg_470 = add i32 %reg_468, %reg_469
	%reg_471 = load i32, ptr %reg_49
	%reg_472 = sub i32 %reg_470, %reg_471
	%reg_473 = load i32, ptr %reg_51
	%reg_474 = sub i32 %reg_472, %reg_473
	%reg_475 = load i32, ptr %reg_53
	%reg_476 = sub i32 %reg_474, %reg_475
	%reg_477 = load i32, ptr %reg_55
	%reg_478 = sub i32 %reg_476, %reg_477
	%reg_479 = load i32, ptr %reg_57
	%reg_480 = add i32 %reg_478, %reg_479
	%reg_481 = load i32, ptr %reg_59
	%reg_482 = add i32 %reg_480, %reg_481
	%reg_483 = load i32, ptr %reg_61
	%reg_484 = add i32 %reg_482, %reg_483
	%reg_485 = load i32, ptr %reg_63
	%reg_486 = add i32 %reg_484, %reg_485
	%reg_487 = load i32, ptr %reg_65
	%reg_488 = sub i32 %reg_486, %reg_487
	%reg_489 = load i32, ptr %reg_67
	%reg_490 = sub i32 %reg_488, %reg_489
	%reg_491 = load i32, ptr %reg_69
	%reg_492 = sub i32 %reg_490, %reg_491
	%reg_493 = load i32, ptr %reg_71
	%reg_494 = sub i32 %reg_492, %reg_493
	%reg_495 = load i32, ptr %reg_73
	%reg_496 = add i32 %reg_494, %reg_495
	%reg_497 = load i32, ptr %reg_75
	%reg_498 = add i32 %reg_496, %reg_497
	%reg_499 = load i32, ptr %reg_77
	%reg_500 = add i32 %reg_498, %reg_499
	%reg_501 = load i32, ptr %reg_79
	%reg_502 = add i32 %reg_500, %reg_501
	%reg_503 = load i32, ptr @a1
	%reg_504 = add i32 %reg_502, %reg_503
	%reg_505 = load i32, ptr @a2
	%reg_506 = sub i32 %reg_504, %reg_505
	%reg_507 = load i32, ptr @a3
	%reg_508 = add i32 %reg_506, %reg_507
	%reg_509 = load i32, ptr @a4
	%reg_510 = sub i32 %reg_508, %reg_509
	%reg_511 = load i32, ptr @a5
	%reg_512 = add i32 %reg_510, %reg_511
	%reg_513 = load i32, ptr @a6
	%reg_514 = sub i32 %reg_512, %reg_513
	%reg_515 = load i32, ptr @a7
	%reg_516 = add i32 %reg_514, %reg_515
	%reg_517 = load i32, ptr @a8
	%reg_518 = sub i32 %reg_516, %reg_517
	%reg_519 = load i32, ptr @a9
	%reg_520 = add i32 %reg_518, %reg_519
	%reg_521 = load i32, ptr @a10
	%reg_522 = sub i32 %reg_520, %reg_521
	%reg_523 = load i32, ptr @a11
	%reg_524 = add i32 %reg_522, %reg_523
	%reg_525 = load i32, ptr @a12
	%reg_526 = sub i32 %reg_524, %reg_525
	%reg_527 = load i32, ptr @a13
	%reg_528 = add i32 %reg_526, %reg_527
	%reg_529 = load i32, ptr @a14
	%reg_530 = sub i32 %reg_528, %reg_529
	%reg_531 = load i32, ptr @a15
	%reg_532 = add i32 %reg_530, %reg_531
	%reg_533 = load i32, ptr @a16
	%reg_534 = sub i32 %reg_532, %reg_533
	%reg_535 = load i32, ptr @a17
	%reg_536 = add i32 %reg_534, %reg_535
	%reg_537 = load i32, ptr @a18
	%reg_538 = sub i32 %reg_536, %reg_537
	%reg_539 = load i32, ptr @a19
	%reg_540 = add i32 %reg_538, %reg_539
	%reg_541 = load i32, ptr @a20
	%reg_542 = sub i32 %reg_540, %reg_541
	%reg_543 = load i32, ptr @a21
	%reg_544 = add i32 %reg_542, %reg_543
	%reg_545 = load i32, ptr @a22
	%reg_546 = sub i32 %reg_544, %reg_545
	%reg_547 = load i32, ptr @a23
	%reg_548 = add i32 %reg_546, %reg_547
	%reg_549 = load i32, ptr @a24
	%reg_550 = sub i32 %reg_548, %reg_549
	%reg_551 = load i32, ptr @a25
	%reg_552 = add i32 %reg_550, %reg_551
	%reg_553 = load i32, ptr @a26
	%reg_554 = sub i32 %reg_552, %reg_553
	%reg_555 = load i32, ptr @a27
	%reg_556 = add i32 %reg_554, %reg_555
	%reg_557 = load i32, ptr @a28
	%reg_558 = sub i32 %reg_556, %reg_557
	%reg_559 = load i32, ptr @a29
	%reg_560 = add i32 %reg_558, %reg_559
	%reg_561 = load i32, ptr @a30
	%reg_562 = sub i32 %reg_560, %reg_561
	%reg_563 = load i32, ptr @a31
	%reg_564 = add i32 %reg_562, %reg_563
	%reg_565 = load i32, ptr @a32
	%reg_566 = sub i32 %reg_564, %reg_565
	ret i32 %reg_566
}

define i32 @main()
{
Block0:  ;Func define at line 86
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 86
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = call i32 @getint()
	store i32 %reg_4, ptr %reg_0
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = add i32 2, 0
	%reg_7 = add i32 9, 0
	%reg_8 = mul i32 %reg_6, %reg_7
	%reg_9 = add i32 %reg_5, %reg_8
	store i32 %reg_9, ptr %reg_2
	%reg_10 = load i32, ptr %reg_0
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = call i32 @func(i32 %reg_10, i32 %reg_11)
	store i32 %reg_12, ptr %reg_0
	%reg_13 = load i32, ptr %reg_0
	call void @putint(i32 %reg_13)
	%reg_14 = load i32, ptr %reg_0
	ret i32 %reg_14
}
