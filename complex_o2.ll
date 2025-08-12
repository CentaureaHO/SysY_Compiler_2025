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

@f = global [20 x [20 x [20 x [20 x [20 x i32]]]]] zeroinitializer

define i32 @main()
{
Block0:
	%reg_2 = alloca [5 x i32]
	br label %Block1
Block1: ; Loop 0 preheader
	%reg_1 = call i32 @getint()
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 20, i1 1)
	%reg_4 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 0
	store i32 1, ptr %reg_4
	br label %Block19
Block2: ; Loop 0 header
	%reg_240 = phi i32 [ 0, %Block19 ], [ %reg_243, %Block7 ]
	br label %Block3
Block3: ; Loop 0 body
	%reg_14 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 1
	store i32 2, ptr %reg_14
	br label %Block20
Block4: ; Loop 0 exit target
	%reg_242 = phi i32 [ %reg_243, %Block7 ], [ 0, %Block19 ]
	%reg_232 = getelementptr [5 x i32], ptr %reg_2, i32 0
	call void @putarray(i32 5, ptr %reg_232)
	call void @putint(i32 %reg_242)
	call void @putch(i32 10)
	ret i32 0
Block5: ; Loop 0 body
	%reg_239 = phi i32 [ %reg_240, %Block20 ], [ %reg_244, %Block10 ]
	br label %Block6
Block6: ; Loop 0 body
	%reg_22 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 2
	store i32 1, ptr %reg_22
	br label %Block21
Block7: ; Loop 0 latch
	%reg_243 = phi i32 [ %reg_244, %Block10 ], [ %reg_240, %Block20 ]
	%reg_228 = load i32, ptr %reg_4
	%reg_230 = add i32 %reg_228, 1
	store i32 %reg_230, ptr %reg_4
	%reg_255 = icmp slt i32 %reg_230, %reg_1
	br i1 %reg_255, label %Block2, label %Block4
Block8: ; Loop 0 body
	%reg_238 = phi i32 [ %reg_239, %Block21 ], [ %reg_245, %Block13 ]
	br label %Block9
Block9: ; Loop 0 body
	%reg_30 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 3
	store i32 3, ptr %reg_30
	br label %Block22
Block10: ; Loop 0 body
	%reg_244 = phi i32 [ %reg_245, %Block13 ], [ %reg_239, %Block21 ]
	%reg_221 = load i32, ptr %reg_14
	%reg_223 = add i32 %reg_221, 1
	store i32 %reg_223, ptr %reg_14
	%reg_264 = icmp slt i32 %reg_223, %reg_1
	br i1 %reg_264, label %Block5, label %Block7
Block11: ; Loop 0 body
	%reg_237 = phi i32 [ %reg_238, %Block22 ], [ %reg_246, %Block16 ]
	br label %Block12
Block12: ; Loop 0 body
	%reg_38 = getelementptr [5 x i32], ptr %reg_2, i32 0, i32 4
	store i32 1, ptr %reg_38
	br label %Block23
Block13: ; Loop 0 body
	%reg_245 = phi i32 [ %reg_246, %Block16 ], [ %reg_238, %Block22 ]
	%reg_214 = load i32, ptr %reg_22
	%reg_216 = add i32 %reg_214, 1
	store i32 %reg_216, ptr %reg_22
	%reg_273 = icmp slt i32 %reg_216, %reg_1
	br i1 %reg_273, label %Block8, label %Block10
Block14: ; Loop 0 body
	%reg_236 = phi i32 [ %reg_237, %Block23 ], [ %reg_241, %Block18 ]
	br label %Block15
Block15: ; Loop 0 body
	%reg_47 = load i32, ptr %reg_4
	%reg_50 = load i32, ptr %reg_14
	%reg_53 = load i32, ptr %reg_22
	%reg_56 = load i32, ptr %reg_30
	%reg_59 = load i32, ptr %reg_38
	%reg_60 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_47, i32 %reg_50, i32 %reg_53, i32 %reg_56, i32 %reg_59
	%reg_65 = sub i32 %reg_47, 1
	%reg_70 = sub i32 %reg_50, 2
	%reg_75 = sub i32 %reg_53, 1
	%reg_80 = sub i32 %reg_56, 3
	%reg_85 = sub i32 %reg_59, 1
	%reg_86 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_65, i32 %reg_70, i32 %reg_75, i32 %reg_80, i32 %reg_85
	%reg_87 = load i32, ptr %reg_86
	%reg_97 = sub i32 %reg_50, 1
	%reg_107 = sub i32 %reg_56, 1
	%reg_113 = getelementptr [20 x [20 x [20 x [20 x [20 x i32]]]]], ptr @f, i32 0, i32 %reg_65, i32 %reg_97, i32 %reg_75, i32 %reg_107, i32 %reg_85
	%reg_114 = load i32, ptr %reg_113
	%reg_115 = add i32 %reg_87, %reg_114
	%reg_122 = mul i32 %reg_47, %reg_50
	%reg_123 = add i32 %reg_115, %reg_122
	%reg_127 = add i32 %reg_123, %reg_53
	%reg_131 = add i32 %reg_127, %reg_56
	%reg_135 = add i32 %reg_131, %reg_59
	store i32 %reg_135, ptr %reg_60
	%reg_170 = srem i32 %reg_135, 998244353
	store i32 %reg_170, ptr %reg_60
	%reg_189 = add i32 %reg_236, %reg_170
	%reg_192 = icmp sgt i32 %reg_189, 998244353
	br i1 %reg_192, label %Block17, label %Block18
Block16: ; Loop 0 body
	%reg_246 = phi i32 [ %reg_241, %Block18 ], [ %reg_237, %Block23 ]
	%reg_207 = load i32, ptr %reg_30
	%reg_209 = add i32 %reg_207, 1
	store i32 %reg_209, ptr %reg_30
	%reg_282 = icmp slt i32 %reg_209, %reg_1
	br i1 %reg_282, label %Block11, label %Block13
Block17: ; Loop 0 body
	%reg_195 = sub i32 %reg_189, 998244353
	br label %Block18
Block18: ; Loop 0 body
	%reg_241 = phi i32 [ %reg_189, %Block15 ], [ %reg_195, %Block17 ]
	%reg_200 = load i32, ptr %reg_38
	%reg_202 = add i32 %reg_200, 1
	store i32 %reg_202, ptr %reg_38
	%reg_291 = icmp slt i32 %reg_202, %reg_1
	br i1 %reg_291, label %Block14, label %Block16
Block19: ; LoopRotateGuardBlock for loop 0
	%reg_247 = phi i32 [ 0, %Block1 ]
	%reg_250 = load i32, ptr %reg_4
	%reg_251 = icmp slt i32 %reg_250, %reg_1
	br i1 %reg_251, label %Block2, label %Block4
Block20: ; LoopRotateGuardBlock for loop 1
	%reg_256 = phi i32 [ %reg_240, %Block3 ]
	%reg_259 = load i32, ptr %reg_14
	%reg_260 = icmp slt i32 %reg_259, %reg_1
	br i1 %reg_260, label %Block5, label %Block7
Block21: ; LoopRotateGuardBlock for loop 2
	%reg_265 = phi i32 [ %reg_239, %Block6 ]
	%reg_268 = load i32, ptr %reg_22
	%reg_269 = icmp slt i32 %reg_268, %reg_1
	br i1 %reg_269, label %Block8, label %Block10
Block22: ; LoopRotateGuardBlock for loop 3
	%reg_274 = phi i32 [ %reg_238, %Block9 ]
	%reg_277 = load i32, ptr %reg_30
	%reg_278 = icmp slt i32 %reg_277, %reg_1
	br i1 %reg_278, label %Block11, label %Block13
Block23: ; LoopRotateGuardBlock for loop 4
	%reg_283 = phi i32 [ %reg_237, %Block12 ]
	%reg_286 = load i32, ptr %reg_38
	%reg_287 = icmp slt i32 %reg_286, %reg_1
	br i1 %reg_287, label %Block14, label %Block16
}
