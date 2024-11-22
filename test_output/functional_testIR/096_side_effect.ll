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

@a = global i32 -1
@b = global i32 1

define i32 @inc_a()
{
Block0:  ;Func define at line 4
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 4
	%reg_1 = load i32, ptr @a
	store i32 %reg_1, ptr %reg_0
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = add i32 1, 0
	%reg_4 = add i32 %reg_2, %reg_3
	store i32 %reg_4, ptr %reg_0
	%reg_5 = load i32, ptr %reg_0
	store i32 %reg_5, ptr @a
	%reg_6 = load i32, ptr @a
	ret i32 %reg_6
}

define i32 @main()
{
Block0:  ;Func define at line 12
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 12
	%reg_1 = add i32 5, 0
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 15
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = add i32 0, 0
	%reg_4 = icmp sge i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 15
	%reg_5 = call i32 @inc_a()
	%reg_6 = icmp ne i32 %reg_5, 0
	br i1 %reg_6, label %Block8, label %Block6
Block4:  ;While end at line 15
	%reg_35 = load i32, ptr @a
	call void @putint(i32 %reg_35)
	%reg_36 = add i32 32, 0
	call void @putch(i32 %reg_36)
	%reg_37 = load i32, ptr @b
	call void @putint(i32 %reg_37)
	%reg_38 = add i32 10, 0
	call void @putch(i32 %reg_38)
	%reg_39 = load i32, ptr @a
	ret i32 %reg_39
Block5:  ;If then at line 16
	%reg_11 = load i32, ptr @a
	call void @putint(i32 %reg_11)
	%reg_12 = add i32 32, 0
	call void @putch(i32 %reg_12)
	%reg_13 = load i32, ptr @b
	call void @putint(i32 %reg_13)
	%reg_14 = add i32 10, 0
	call void @putch(i32 %reg_14)
	br label %Block6
Block6:  ;If end at line 16
	%reg_15 = call i32 @inc_a()
	%reg_16 = add i32 14, 0
	%reg_17 = icmp slt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block9, label %Block12
Block7:  ;And opertor at line 16
	%reg_9 = call i32 @inc_a()
	%reg_10 = icmp ne i32 %reg_9, 0
	br i1 %reg_10, label %Block5, label %Block6
Block8:  ;And opertor at line -1
	%reg_7 = call i32 @inc_a()
	%reg_8 = icmp ne i32 %reg_7, 0
	br i1 %reg_8, label %Block7, label %Block6
Block9:  ;If then at line 19
	%reg_26 = load i32, ptr @a
	call void @putint(i32 %reg_26)
	%reg_27 = add i32 10, 0
	call void @putch(i32 %reg_27)
	%reg_28 = load i32, ptr @b
	%reg_29 = add i32 2, 0
	%reg_30 = mul i32 %reg_28, %reg_29
	store i32 %reg_30, ptr @b
	br label %Block10
Block10:  ;If end at line 19
	%reg_32 = load i32, ptr %reg_0
	%reg_33 = add i32 1, 0
	%reg_34 = sub i32 %reg_32, %reg_33
	store i32 %reg_34, ptr %reg_0
	br label %Block2
Block11:  ;If else at line 19
	%reg_31 = call i32 @inc_a()
	br label %Block10
Block12:  ;Or opertor at line 19
	%reg_18 = call i32 @inc_a()
	%reg_19 = icmp ne i32 %reg_18, 0
	br i1 %reg_19, label %Block13, label %Block11
Block13:  ;And opertor at line -1
	%reg_20 = call i32 @inc_a()
	%reg_21 = call i32 @inc_a()
	%reg_22 = sub i32 %reg_20, %reg_21
	%reg_23 = add i32 1, 0
	%reg_24 = add i32 %reg_22, %reg_23
	%reg_25 = icmp ne i32 %reg_24, 0
	br i1 %reg_25, label %Block9, label %Block11
}
