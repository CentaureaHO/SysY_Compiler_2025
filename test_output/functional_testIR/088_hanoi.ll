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


define void @move(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 2
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 2
	%reg_4 = load i32, ptr %reg_2
	call void @putint(i32 %reg_4)
	%reg_5 = add i32 32, 0
	call void @putch(i32 %reg_5)
	%reg_6 = load i32, ptr %reg_3
	call void @putint(i32 %reg_6)
	%reg_7 = add i32 44, 0
	call void @putch(i32 %reg_7)
	%reg_8 = add i32 32, 0
	call void @putch(i32 %reg_8)
	ret void
}

define void @hanoi(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3)
{
Block0:  ;Func define at line 7
	%reg_7 = alloca i32
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	store i32 %reg_0, ptr %reg_4
	store i32 %reg_1, ptr %reg_5
	store i32 %reg_2, ptr %reg_6
	store i32 %reg_3, ptr %reg_7
	br label %Block1
Block1:  ;Func body at line 7
	%reg_8 = load i32, ptr %reg_4
	%reg_9 = add i32 1, 0
	%reg_10 = icmp eq i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block2, label %Block4
Block2:  ;If then at line 9
	%reg_11 = load i32, ptr %reg_5
	%reg_12 = load i32, ptr %reg_7
	call void @move(i32 %reg_11, i32 %reg_12)
	br label %Block3
Block3:  ;If end at line 9
	ret void
Block4:  ;If else at line 9
	%reg_13 = load i32, ptr %reg_4
	%reg_14 = add i32 1, 0
	%reg_15 = sub i32 %reg_13, %reg_14
	%reg_16 = load i32, ptr %reg_5
	%reg_17 = load i32, ptr %reg_7
	%reg_18 = load i32, ptr %reg_6
	call void @hanoi(i32 %reg_15, i32 %reg_16, i32 %reg_17, i32 %reg_18)
	%reg_19 = load i32, ptr %reg_5
	%reg_20 = load i32, ptr %reg_7
	call void @move(i32 %reg_19, i32 %reg_20)
	%reg_21 = load i32, ptr %reg_4
	%reg_22 = add i32 1, 0
	%reg_23 = sub i32 %reg_21, %reg_22
	%reg_24 = load i32, ptr %reg_6
	%reg_25 = load i32, ptr %reg_5
	%reg_26 = load i32, ptr %reg_7
	call void @hanoi(i32 %reg_23, i32 %reg_24, i32 %reg_25, i32 %reg_26)
	br label %Block3
}

define i32 @main()
{
Block0:  ;Func define at line 18
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 18
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 21
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = add i32 0, 0
	%reg_4 = icmp sgt i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 21
	%reg_5 = call i32 @getint()
	%reg_6 = add i32 1, 0
	%reg_7 = add i32 2, 0
	%reg_8 = add i32 3, 0
	call void @hanoi(i32 %reg_5, i32 %reg_6, i32 %reg_7, i32 %reg_8)
	%reg_9 = add i32 10, 0
	call void @putch(i32 %reg_9)
	%reg_10 = load i32, ptr %reg_0
	%reg_11 = add i32 1, 0
	%reg_12 = sub i32 %reg_10, %reg_11
	store i32 %reg_12, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 21
	%reg_13 = add i32 0, 0
	ret i32 %reg_13
}
