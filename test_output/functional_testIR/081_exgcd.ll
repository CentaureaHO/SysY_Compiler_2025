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


define i32 @exgcd(i32 %reg_0, i32 %reg_1, ptr %reg_2, ptr %reg_3)
{
Block0:  ;Func define at line 1
	%reg_24 = alloca i32
	%reg_16 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	store i32 %reg_0, ptr %reg_4
	store i32 %reg_1, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 1
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = add i32 0, 0
	%reg_8 = icmp eq i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block2, label %Block4
Block2:  ;If then at line 2
	%reg_9 = add i32 0, 0
	%reg_10 = getelementptr i32, ptr %reg_2, i32 %reg_9
	%reg_11 = add i32 1, 0
	store i32 %reg_11, ptr %reg_10
	%reg_12 = add i32 0, 0
	%reg_13 = getelementptr i32, ptr %reg_3, i32 %reg_12
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_13
	%reg_15 = load i32, ptr %reg_4
	ret i32 %reg_15
	br label %Block3
Block3:  ;If end at line 2
	ret i32 0
Block4:  ;If else at line 2
	%reg_17 = load i32, ptr %reg_5
	%reg_18 = load i32, ptr %reg_4
	%reg_19 = load i32, ptr %reg_5
	%reg_20 = srem i32 %reg_18, %reg_19
	%reg_21 = getelementptr i32, ptr %reg_2
	%reg_22 = getelementptr i32, ptr %reg_3
	%reg_23 = call i32 @exgcd(i32 %reg_17, i32 %reg_20, ptr %reg_21, ptr %reg_22)
	store i32 %reg_23, ptr %reg_16
	%reg_25 = add i32 0, 0
	%reg_26 = getelementptr i32, ptr %reg_2, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	store i32 %reg_27, ptr %reg_24
	%reg_28 = add i32 0, 0
	%reg_29 = getelementptr i32, ptr %reg_2, i32 %reg_28
	%reg_30 = add i32 0, 0
	%reg_31 = getelementptr i32, ptr %reg_3, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	store i32 %reg_32, ptr %reg_29
	%reg_33 = add i32 0, 0
	%reg_34 = getelementptr i32, ptr %reg_3, i32 %reg_33
	%reg_35 = load i32, ptr %reg_24
	%reg_36 = load i32, ptr %reg_4
	%reg_37 = load i32, ptr %reg_5
	%reg_38 = sdiv i32 %reg_36, %reg_37
	%reg_39 = add i32 0, 0
	%reg_40 = getelementptr i32, ptr %reg_3, i32 %reg_39
	%reg_41 = load i32, ptr %reg_40
	%reg_42 = mul i32 %reg_38, %reg_41
	%reg_43 = sub i32 %reg_35, %reg_42
	store i32 %reg_43, ptr %reg_34
	%reg_44 = load i32, ptr %reg_16
	ret i32 %reg_44
	br label %Block3
}

define i32 @main()
{
Block0:  ;Func define at line 16
	%reg_7 = alloca [1 x i32]
	%reg_4 = alloca [1 x i32]
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 16
	%reg_1 = add i32 7, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 15, 0
	store i32 %reg_3, ptr %reg_2
	call void @llvm.memset.p0.i32(ptr %reg_4, i8 0, i32 4, i1 1)
	%reg_5 = add i32 1, 0
	%reg_6 = getelementptr [1 x i32], ptr %reg_4, i32 0, i32 0
	store i32 %reg_5, ptr %reg_6
	call void @llvm.memset.p0.i32(ptr %reg_7, i8 0, i32 4, i1 1)
	%reg_8 = add i32 1, 0
	%reg_9 = getelementptr [1 x i32], ptr %reg_7, i32 0, i32 0
	store i32 %reg_8, ptr %reg_9
	%reg_10 = load i32, ptr %reg_0
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = getelementptr [1 x i32], ptr %reg_4, i32 0
	%reg_13 = getelementptr [1 x i32], ptr %reg_7, i32 0
	%reg_14 = call i32 @exgcd(i32 %reg_10, i32 %reg_11, ptr %reg_12, ptr %reg_13)
	%reg_15 = add i32 0, 0
	%reg_16 = getelementptr [1 x i32], ptr %reg_4, i32 0, i32 %reg_15
	%reg_17 = add i32 0, 0
	%reg_18 = getelementptr [1 x i32], ptr %reg_4, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = load i32, ptr %reg_2
	%reg_21 = srem i32 %reg_19, %reg_20
	%reg_22 = load i32, ptr %reg_2
	%reg_23 = add i32 %reg_21, %reg_22
	%reg_24 = load i32, ptr %reg_2
	%reg_25 = srem i32 %reg_23, %reg_24
	store i32 %reg_25, ptr %reg_16
	%reg_26 = add i32 0, 0
	%reg_27 = getelementptr [1 x i32], ptr %reg_4, i32 0, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	call void @putint(i32 %reg_28)
	%reg_29 = add i32 0, 0
	ret i32 %reg_29
}
