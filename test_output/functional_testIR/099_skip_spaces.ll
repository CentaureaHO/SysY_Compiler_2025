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


define i32 @main()
{
Block0:  ;Func define at line 11
	%reg_3 = alloca i32
	%reg_1 = alloca i32
	%reg_0 = alloca [100 x i32]
	br label %Block1
Block1:  ;Func body at line 11
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 400, i1 1)
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 13
	%reg_5 = call i32 @getint()
	%reg_6 = icmp ne i32 %reg_5, 0
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 13
	%reg_7 = load i32, ptr %reg_1
	%reg_8 = getelementptr [100 x i32], ptr %reg_0, i32 0, i32 %reg_7
	%reg_9 = call i32 @getint()
	store i32 %reg_9, ptr %reg_8
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = add i32 1, 0
	%reg_12 = add i32 %reg_10, %reg_11
	store i32 %reg_12, ptr %reg_1
	br label %Block2
Block4:  ;While end at line 13
	br label %Block5
Block5:  ;While condition at line 17
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = icmp ne i32 %reg_13, 0
	br i1 %reg_14, label %Block6, label %Block7
Block6:  ;While body at line 17
	%reg_15 = load i32, ptr %reg_1
	%reg_16 = add i32 1, 0
	%reg_17 = sub i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_1
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = load i32, ptr %reg_1
	%reg_20 = getelementptr [100 x i32], ptr %reg_0, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = add i32 %reg_18, %reg_21
	store i32 %reg_22, ptr %reg_3
	br label %Block5
Block7:  ;While end at line 17
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = add i32 79, 0
	%reg_25 = srem i32 %reg_23, %reg_24
	ret i32 %reg_25
}
