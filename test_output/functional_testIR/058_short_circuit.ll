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

@g = global i32 0

define i32 @func(i32 %reg_0)
{
Block0:  ;Func define at line 3
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 3
	%reg_2 = load i32, ptr @g
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = add i32 %reg_2, %reg_3
	store i32 %reg_4, ptr @g
	%reg_5 = load i32, ptr @g
	call void @putint(i32 %reg_5)
	%reg_6 = load i32, ptr @g
	ret i32 %reg_6
}

define i32 @main()
{
Block0:  ;Func define at line 9
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 9
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = call i32 @getint()
	store i32 %reg_2, ptr %reg_0
	%reg_3 = load i32, ptr %reg_0
	%reg_4 = add i32 10, 0
	%reg_5 = icmp sgt i32 %reg_3, %reg_4
	br i1 %reg_5, label %Block5, label %Block4
Block2:  ;If then at line 12
	%reg_9 = add i32 1, 0
	store i32 %reg_9, ptr %reg_0
	br label %Block3
Block3:  ;If end at line 12
	%reg_11 = call i32 @getint()
	store i32 %reg_11, ptr %reg_0
	%reg_12 = load i32, ptr %reg_0
	%reg_13 = add i32 11, 0
	%reg_14 = icmp sgt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block9, label %Block8
Block4:  ;If else at line 12
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_0
	br label %Block3
Block5:  ;And opertor at line 12
	%reg_6 = load i32, ptr %reg_0
	%reg_7 = call i32 @func(i32 %reg_6)
	%reg_8 = icmp ne i32 %reg_7, 0
	br i1 %reg_8, label %Block2, label %Block4
Block6:  ;If then at line 14
	%reg_18 = add i32 1, 0
	store i32 %reg_18, ptr %reg_0
	br label %Block7
Block7:  ;If end at line 14
	%reg_20 = call i32 @getint()
	store i32 %reg_20, ptr %reg_0
	%reg_21 = load i32, ptr %reg_0
	%reg_22 = add i32 99, 0
	%reg_23 = icmp sle i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block10, label %Block13
Block8:  ;If else at line 14
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_0
	br label %Block7
Block9:  ;And opertor at line 14
	%reg_15 = load i32, ptr %reg_0
	%reg_16 = call i32 @func(i32 %reg_15)
	%reg_17 = icmp ne i32 %reg_16, 0
	br i1 %reg_17, label %Block6, label %Block8
Block10:  ;If then at line 16
	%reg_27 = add i32 1, 0
	store i32 %reg_27, ptr %reg_0
	br label %Block11
Block11:  ;If end at line 16
	%reg_29 = call i32 @getint()
	store i32 %reg_29, ptr %reg_0
	%reg_30 = load i32, ptr %reg_0
	%reg_31 = add i32 100, 0
	%reg_32 = icmp sle i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block14, label %Block17
Block12:  ;If else at line 16
	%reg_28 = add i32 0, 0
	store i32 %reg_28, ptr %reg_0
	br label %Block11
Block13:  ;Or opertor at line 16
	%reg_24 = load i32, ptr %reg_0
	%reg_25 = call i32 @func(i32 %reg_24)
	%reg_26 = icmp ne i32 %reg_25, 0
	br i1 %reg_26, label %Block10, label %Block12
Block14:  ;If then at line 18
	%reg_36 = add i32 1, 0
	store i32 %reg_36, ptr %reg_0
	br label %Block15
Block15:  ;If end at line 18
	%reg_38 = add i32 99, 0
	%reg_39 = call i32 @func(i32 %reg_38)
	%reg_40 = icmp eq i32 %reg_39, 0
	br i1 %reg_40, label %Block21, label %Block20
Block16:  ;If else at line 18
	%reg_37 = add i32 0, 0
	store i32 %reg_37, ptr %reg_0
	br label %Block15
Block17:  ;Or opertor at line 18
	%reg_33 = load i32, ptr %reg_0
	%reg_34 = call i32 @func(i32 %reg_33)
	%reg_35 = icmp ne i32 %reg_34, 0
	br i1 %reg_35, label %Block14, label %Block16
Block18:  ;If then at line 19
	%reg_44 = add i32 1, 0
	store i32 %reg_44, ptr %reg_0
	br label %Block19
Block19:  ;If end at line 19
	%reg_46 = add i32 0, 0
	ret i32 %reg_46
Block20:  ;If else at line 19
	%reg_45 = add i32 0, 0
	store i32 %reg_45, ptr %reg_0
	br label %Block19
Block21:  ;And opertor at line 19
	%reg_41 = add i32 100, 0
	%reg_42 = call i32 @func(i32 %reg_41)
	%reg_43 = icmp ne i32 %reg_42, 0
	br i1 %reg_43, label %Block18, label %Block20
}
