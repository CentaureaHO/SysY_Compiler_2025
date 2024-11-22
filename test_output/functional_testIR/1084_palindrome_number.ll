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


define i32 @palindrome(i32 %reg_0)
{
Block0:  ;Func define at line 1
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca [4 x i32]
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 1
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 16, i1 1)
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 7
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = add i32 4, 0
	%reg_10 = icmp slt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 7
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = getelementptr [4 x i32], ptr %reg_2, i32 0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = add i32 10, 0
	%reg_15 = srem i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_12
	%reg_16 = load i32, ptr %reg_1
	%reg_17 = add i32 10, 0
	%reg_18 = sdiv i32 %reg_16, %reg_17
	store i32 %reg_18, ptr %reg_1
	%reg_19 = load i32, ptr %reg_3
	%reg_20 = add i32 1, 0
	%reg_21 = add i32 %reg_19, %reg_20
	store i32 %reg_21, ptr %reg_3
	br label %Block2
Block4:  ;While end at line 7
	%reg_22 = add i32 0, 0
	%reg_23 = getelementptr [4 x i32], ptr %reg_2, i32 0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = add i32 3, 0
	%reg_26 = getelementptr [4 x i32], ptr %reg_2, i32 0, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	%reg_28 = icmp eq i32 %reg_24, %reg_27
	br i1 %reg_28, label %Block8, label %Block7
Block5:  ;If then at line 14
	%reg_36 = add i32 1, 0
	store i32 %reg_36, ptr %reg_5
	br label %Block6
Block6:  ;If end at line 14
	%reg_38 = load i32, ptr %reg_5
	ret i32 %reg_38
Block7:  ;If else at line 14
	%reg_37 = add i32 0, 0
	store i32 %reg_37, ptr %reg_5
	br label %Block6
Block8:  ;And opertor at line 14
	%reg_29 = add i32 1, 0
	%reg_30 = getelementptr [4 x i32], ptr %reg_2, i32 0, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	%reg_32 = add i32 2, 0
	%reg_33 = getelementptr [4 x i32], ptr %reg_2, i32 0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = icmp eq i32 %reg_31, %reg_34
	br i1 %reg_35, label %Block5, label %Block7
}

define i32 @main()
{
Block0:  ;Func define at line 23
	%reg_3 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 23
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = add i32 1221, 0
	store i32 %reg_2, ptr %reg_0
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = call i32 @palindrome(i32 %reg_5)
	store i32 %reg_6, ptr %reg_3
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = add i32 1, 0
	%reg_9 = icmp eq i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block2, label %Block4
Block2:  ;If then at line 29
	%reg_10 = load i32, ptr %reg_0
	call void @putint(i32 %reg_10)
	br label %Block3
Block3:  ;If end at line 29
	%reg_13 = add i32 10, 0
	store i32 %reg_13, ptr %reg_3
	%reg_14 = load i32, ptr %reg_3
	call void @putch(i32 %reg_14)
	%reg_15 = add i32 0, 0
	ret i32 %reg_15
Block4:  ;If else at line 29
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_3
	%reg_12 = load i32, ptr %reg_3
	call void @putint(i32 %reg_12)
	br label %Block3
}
