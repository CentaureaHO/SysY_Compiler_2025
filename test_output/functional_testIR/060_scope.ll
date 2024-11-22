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

@a = global i32 7

define i32 @func()
{
Block0:  ;Func define at line 3
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 3
	%reg_1 = load i32, ptr @a
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 1, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 6
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = add i32 1, 0
	%reg_9 = add i32 %reg_7, %reg_8
	store i32 %reg_9, ptr %reg_2
	%reg_10 = add i32 1, 0
	ret i32 %reg_10
	br label %Block3
Block3:  ;If end at line 6
	ret i32 0
Block4:  ;If else at line 6
	%reg_11 = add i32 0, 0
	ret i32 %reg_11
	br label %Block3
}

define i32 @main()
{
Block0:  ;Func define at line 14
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 14
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 17
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = add i32 100, 0
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 17
	%reg_7 = call i32 @func()
	%reg_8 = add i32 1, 0
	%reg_9 = icmp eq i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block5, label %Block6
Block4:  ;While end at line 17
	%reg_16 = load i32, ptr %reg_0
	%reg_17 = add i32 100, 0
	%reg_18 = icmp slt i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block7, label %Block9
Block5:  ;If then at line 18
	%reg_10 = load i32, ptr %reg_0
	%reg_11 = add i32 1, 0
	%reg_12 = add i32 %reg_10, %reg_11
	store i32 %reg_12, ptr %reg_0
	br label %Block6
Block6:  ;If end at line 18
	%reg_13 = load i32, ptr %reg_2
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_2
	br label %Block2
Block7:  ;If then at line 22
	%reg_19 = add i32 1, 0
	call void @putint(i32 %reg_19)
	br label %Block8
Block8:  ;If end at line 22
	%reg_21 = add i32 0, 0
	ret i32 %reg_21
Block9:  ;If else at line 22
	%reg_20 = add i32 0, 0
	call void @putint(i32 %reg_20)
	br label %Block8
}
