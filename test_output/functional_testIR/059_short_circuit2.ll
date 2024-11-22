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


define i32 @func(i32 %reg_0)
{
Block0:  ;Func define at line 1
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 1
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 50, 0
	%reg_4 = icmp sle i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block4
Block2:  ;If then at line 2
	%reg_5 = load i32, ptr %reg_1
	call void @putint(i32 %reg_5)
	%reg_6 = add i32 1, 0
	ret i32 %reg_6
	br label %Block3
Block3:  ;If end at line 2
	ret i32 0
Block4:  ;If else at line 2
	%reg_7 = load i32, ptr %reg_1
	call void @putint(i32 %reg_7)
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
	br label %Block3
}

define i32 @main()
{
Block0:  ;Func define at line 12
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 12
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = add i32 0, 0
	%reg_3 = call i32 @func(i32 %reg_2)
	%reg_4 = add i32 1, 0
	%reg_5 = icmp eq i32 %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block5
Block2:  ;If then at line 15
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_0
	br label %Block3
Block3:  ;If end at line 15
	%reg_16 = add i32 50, 0
	%reg_17 = call i32 @func(i32 %reg_16)
	%reg_18 = add i32 1, 0
	%reg_19 = icmp eq i32 %reg_17, %reg_18
	br i1 %reg_19, label %Block11, label %Block10
Block4:  ;If else at line 15
	%reg_15 = add i32 1, 0
	store i32 %reg_15, ptr %reg_0
	br label %Block3
Block5:  ;Or opertor at line 15
	%reg_6 = add i32 50, 0
	%reg_7 = call i32 @func(i32 %reg_6)
	%reg_8 = add i32 1, 0
	%reg_9 = icmp eq i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block6, label %Block4
Block6:  ;And opertor at line -1
	%reg_10 = add i32 100, 0
	%reg_11 = call i32 @func(i32 %reg_10)
	%reg_12 = add i32 0, 0
	%reg_13 = icmp eq i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block2, label %Block4
Block7:  ;If then at line 20
	%reg_28 = add i32 0, 0
	store i32 %reg_28, ptr %reg_0
	br label %Block8
Block8:  ;If end at line 20
	%reg_30 = add i32 0, 0
	ret i32 %reg_30
Block9:  ;If else at line 20
	%reg_29 = add i32 1, 0
	store i32 %reg_29, ptr %reg_0
	br label %Block8
Block10:  ;Or opertor at line 20
	%reg_24 = add i32 1, 0
	%reg_25 = call i32 @func(i32 %reg_24)
	%reg_26 = add i32 1, 0
	%reg_27 = icmp eq i32 %reg_25, %reg_26
	br i1 %reg_27, label %Block7, label %Block9
Block11:  ;And opertor at line -1
	%reg_20 = add i32 40, 0
	%reg_21 = call i32 @func(i32 %reg_20)
	%reg_22 = add i32 1, 0
	%reg_23 = icmp eq i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block7, label %Block10
}
