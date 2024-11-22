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

@a = global [4096 x i32] zeroinitializer

define i32 @f1(ptr %reg_0)
{
Block0:  ;Func define at line 3
	br label %Block1
Block1:  ;Func body at line 3
	%reg_1 = add i32 5, 0
	%reg_2 = getelementptr [4096 x i32], ptr @a, i32 0, i32 %reg_1
	%reg_3 = add i32 4000, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = add i32 4000, 0
	%reg_5 = getelementptr [4096 x i32], ptr @a, i32 0, i32 %reg_4
	%reg_6 = add i32 3, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = add i32 4095, 0
	%reg_8 = getelementptr [4096 x i32], ptr @a, i32 0, i32 %reg_7
	%reg_9 = add i32 7, 0
	store i32 %reg_9, ptr %reg_8
	%reg_10 = add i32 4095, 0
	%reg_11 = getelementptr [4096 x i32], ptr @a, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	%reg_13 = getelementptr i32, ptr %reg_0, i32 %reg_12
	%reg_14 = add i32 2216, 0
	%reg_15 = getelementptr [4096 x i32], ptr @a, i32 0, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = add i32 9, 0
	%reg_18 = add i32 %reg_16, %reg_17
	store i32 %reg_18, ptr %reg_13
	%reg_19 = add i32 5, 0
	%reg_20 = getelementptr [4096 x i32], ptr @a, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = getelementptr [4096 x i32], ptr @a, i32 0, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	ret i32 %reg_23
}

define i32 @main()
{
Block0:  ;Func define at line 12
	%reg_13 = alloca [1024 x [4 x i32]]
	%reg_0 = alloca [4 x [1024 x i32]]
	br label %Block1
Block1:  ;Func body at line 12
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 16384, i1 1)
	%reg_1 = add i32 1, 0
	%reg_2 = getelementptr [4 x [1024 x i32]], ptr %reg_0, i32 0, i32 1, i32 0
	store i32 %reg_1, ptr %reg_2
	%reg_3 = add i32 2, 0
	%reg_4 = getelementptr [4 x [1024 x i32]], ptr %reg_0, i32 0, i32 2, i32 0
	store i32 %reg_3, ptr %reg_4
	%reg_5 = add i32 3, 0
	%reg_6 = getelementptr [4 x [1024 x i32]], ptr %reg_0, i32 0, i32 2, i32 1
	store i32 %reg_5, ptr %reg_6
	%reg_7 = add i32 4, 0
	%reg_8 = getelementptr [4 x [1024 x i32]], ptr %reg_0, i32 0, i32 3, i32 0
	store i32 %reg_7, ptr %reg_8
	%reg_9 = add i32 5, 0
	%reg_10 = getelementptr [4 x [1024 x i32]], ptr %reg_0, i32 0, i32 3, i32 1
	store i32 %reg_9, ptr %reg_10
	%reg_11 = add i32 6, 0
	%reg_12 = getelementptr [4 x [1024 x i32]], ptr %reg_0, i32 0, i32 3, i32 2
	store i32 %reg_11, ptr %reg_12
	call void @llvm.memset.p0.i32(ptr %reg_13, i8 0, i32 16384, i1 1)
	%reg_14 = add i32 1, 0
	%reg_15 = getelementptr [1024 x [4 x i32]], ptr %reg_13, i32 0, i32 0, i32 0
	store i32 %reg_14, ptr %reg_15
	%reg_16 = add i32 2, 0
	%reg_17 = getelementptr [1024 x [4 x i32]], ptr %reg_13, i32 0, i32 0, i32 1
	store i32 %reg_16, ptr %reg_17
	%reg_18 = add i32 3, 0
	%reg_19 = getelementptr [1024 x [4 x i32]], ptr %reg_13, i32 0, i32 1, i32 0
	store i32 %reg_18, ptr %reg_19
	%reg_20 = add i32 4, 0
	%reg_21 = getelementptr [1024 x [4 x i32]], ptr %reg_13, i32 0, i32 1, i32 1
	store i32 %reg_20, ptr %reg_21
	%reg_22 = add i32 0, 0
	%reg_23 = getelementptr [1024 x [4 x i32]], ptr %reg_13, i32 0, i32 %reg_22
	%reg_24 = call i32 @f1(ptr %reg_23)
	call void @putint(i32 %reg_24)
	%reg_25 = add i32 10, 0
	call void @putch(i32 %reg_25)
	%reg_26 = add i32 2, 0
	%reg_27 = add i32 0, 0
	%reg_28 = getelementptr [1024 x [4 x i32]], ptr %reg_13, i32 0, i32 %reg_26, i32 %reg_27
	%reg_29 = load i32, ptr %reg_28
	ret i32 %reg_29
}
