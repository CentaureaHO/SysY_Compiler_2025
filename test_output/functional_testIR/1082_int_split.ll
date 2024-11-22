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

@N = global i32 zeroinitializer
@newline = global i32 zeroinitializer

define i32 @split(i32 %reg_0, ptr %reg_1)
{
Block0:  ;Func define at line 5
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 5
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = load i32, ptr @N
	%reg_6 = add i32 1, 0
	%reg_7 = sub i32 %reg_5, %reg_6
	store i32 %reg_7, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 9
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = add i32 1, 0
	%reg_10 = sub i32 0, %reg_9
	%reg_11 = icmp ne i32 %reg_8, %reg_10
	br i1 %reg_11, label %Block3, label %Block4
Block3:  ;While body at line 9
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = getelementptr i32, ptr %reg_1, i32 %reg_12
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = add i32 10, 0
	%reg_16 = srem i32 %reg_14, %reg_15
	store i32 %reg_16, ptr %reg_13
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = add i32 10, 0
	%reg_19 = sdiv i32 %reg_17, %reg_18
	store i32 %reg_19, ptr %reg_2
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = add i32 1, 0
	%reg_22 = sub i32 %reg_20, %reg_21
	store i32 %reg_22, ptr %reg_3
	br label %Block2
Block4:  ;While end at line 9
	%reg_23 = add i32 0, 0
	ret i32 %reg_23
}

define i32 @main()
{
Block0:  ;Func define at line 20
	%reg_11 = alloca i32
	%reg_6 = alloca [4 x i32]
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	br label %Block1
Block1:  ;Func body at line 20
	%reg_0 = add i32 4, 0
	store i32 %reg_0, ptr @N
	%reg_1 = add i32 10, 0
	store i32 %reg_1, ptr @newline
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	call void @llvm.memset.p0.i32(ptr %reg_6, i8 0, i32 16, i1 1)
	%reg_7 = add i32 1478, 0
	store i32 %reg_7, ptr %reg_4
	%reg_8 = load i32, ptr %reg_4
	%reg_9 = getelementptr [4 x i32], ptr %reg_6, i32 0
	%reg_10 = call i32 @split(i32 %reg_8, ptr %reg_9)
	store i32 %reg_10, ptr %reg_4
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_11
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 31
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = add i32 4, 0
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block3, label %Block4
Block3:  ;While body at line 31
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = getelementptr [4 x i32], ptr %reg_6, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	store i32 %reg_19, ptr %reg_11
	%reg_20 = load i32, ptr %reg_11
	call void @putint(i32 %reg_20)
	%reg_21 = load i32, ptr @newline
	call void @putch(i32 %reg_21)
	%reg_22 = load i32, ptr %reg_2
	%reg_23 = add i32 1, 0
	%reg_24 = add i32 %reg_22, %reg_23
	store i32 %reg_24, ptr %reg_2
	br label %Block2
Block4:  ;While end at line 31
	%reg_25 = add i32 0, 0
	ret i32 %reg_25
}
