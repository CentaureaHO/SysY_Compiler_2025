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


define i32 @f(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 1
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 1
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = mul i32 %reg_4, %reg_5
	ret i32 %reg_6
}

define i32 @g(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 5
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 5
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = srem i32 %reg_4, %reg_5
	ret i32 %reg_6
}

define i32 @h(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 9
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 9
	%reg_4 = add i32 2, 0
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = call i32 @g(i32 %reg_5, i32 %reg_6)
	%reg_8 = call i32 @f(i32 %reg_4, i32 %reg_7)
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = call i32 @f(i32 %reg_9, i32 %reg_10)
	%reg_12 = add i32 4, 0
	%reg_13 = call i32 @g(i32 %reg_11, i32 %reg_12)
	%reg_14 = call i32 @f(i32 %reg_8, i32 %reg_13)
	ret i32 %reg_14
}

define i32 @main()
{
Block0:  ;Func define at line 13
	br label %Block1
Block1:  ;Func body at line 13
	%reg_0 = add i32 11, 0
	%reg_1 = add i32 3, 0
	%reg_2 = call i32 @h(i32 %reg_0, i32 %reg_1)
	call void @putint(i32 %reg_2)
	%reg_3 = add i32 0, 0
	ret i32 %reg_3
}
