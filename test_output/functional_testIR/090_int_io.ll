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

@ascii_0 = global i32 48

define i32 @my_getint()
{
Block0:  ;Func define at line 3
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 3
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 7
	%reg_4 = add i32 1, 0
	%reg_5 = icmp ne i32 %reg_4, 0
	br i1 %reg_5, label %Block3, label %Block4
Block3:  ;While body at line 7
	%reg_6 = call i32 @getch()
	%reg_7 = load i32, ptr @ascii_0
	%reg_8 = sub i32 %reg_6, %reg_7
	store i32 %reg_8, ptr %reg_2
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = add i32 0, 0
	%reg_11 = icmp slt i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block5, label %Block8
Block4:  ;While end at line 7
	%reg_15 = load i32, ptr %reg_2
	store i32 %reg_15, ptr %reg_0
	br label %Block11
Block5:  ;If then at line 9
	br label %Block2
Block6:  ;If end at line 9
	br label %Block2
Block7:  ;If else at line 9
	br label %Block4
Block8:  ;Or opertor at line 9
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = add i32 9, 0
	%reg_14 = icmp sgt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block5, label %Block7
Block9:  ;Continue at line 10
	br label %Block6
Block10:  ;Break at line 12
	br label %Block6
Block11:  ;While condition at line 17
	%reg_16 = add i32 1, 0
	%reg_17 = icmp ne i32 %reg_16, 0
	br i1 %reg_17, label %Block12, label %Block13
Block12:  ;While body at line 17
	%reg_18 = call i32 @getch()
	%reg_19 = load i32, ptr @ascii_0
	%reg_20 = sub i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_2
	%reg_21 = load i32, ptr %reg_2
	%reg_22 = add i32 0, 0
	%reg_23 = icmp sge i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block17, label %Block16
Block13:  ;While end at line 17
	%reg_32 = load i32, ptr %reg_0
	ret i32 %reg_32
Block14:  ;If then at line 19
	%reg_27 = load i32, ptr %reg_0
	%reg_28 = add i32 10, 0
	%reg_29 = mul i32 %reg_27, %reg_28
	%reg_30 = load i32, ptr %reg_2
	%reg_31 = add i32 %reg_29, %reg_30
	store i32 %reg_31, ptr %reg_0
	br label %Block15
Block15:  ;If end at line 19
	br label %Block11
Block16:  ;If else at line 19
	br label %Block13
Block17:  ;And opertor at line 19
	%reg_24 = load i32, ptr %reg_2
	%reg_25 = add i32 9, 0
	%reg_26 = icmp sle i32 %reg_24, %reg_25
	br i1 %reg_26, label %Block14, label %Block16
Block18:  ;Break at line 22
	br label %Block15
}

define void @my_putint(i32 %reg_0)
{
Block0:  ;Func define at line 29
	%reg_3 = alloca i32
	%reg_2 = alloca [16 x i32]
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 29
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 64, i1 1)
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 32
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 0, 0
	%reg_7 = icmp sgt i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 32
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = getelementptr [16 x i32], ptr %reg_2, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = add i32 10, 0
	%reg_12 = srem i32 %reg_10, %reg_11
	%reg_13 = load i32, ptr @ascii_0
	%reg_14 = add i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_9
	%reg_15 = load i32, ptr %reg_1
	%reg_16 = add i32 10, 0
	%reg_17 = sdiv i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_1
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = add i32 1, 0
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_3
	br label %Block2
Block4:  ;While end at line 32
	br label %Block5
Block5:  ;While condition at line 37
	%reg_21 = load i32, ptr %reg_3
	%reg_22 = add i32 0, 0
	%reg_23 = icmp sgt i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block6, label %Block7
Block6:  ;While body at line 37
	%reg_24 = load i32, ptr %reg_3
	%reg_25 = add i32 1, 0
	%reg_26 = sub i32 %reg_24, %reg_25
	store i32 %reg_26, ptr %reg_3
	%reg_27 = load i32, ptr %reg_3
	%reg_28 = getelementptr [16 x i32], ptr %reg_2, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_28
	call void @putch(i32 %reg_29)
	br label %Block5
Block7:  ;While end at line 37
	ret void
}

define i32 @main()
{
Block0:  ;Func define at line 43
	%reg_5 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 43
	%reg_1 = call i32 @my_getint()
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 46
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = add i32 0, 0
	%reg_4 = icmp sgt i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 46
	%reg_6 = call i32 @my_getint()
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr %reg_5
	call void @my_putint(i32 %reg_7)
	%reg_8 = add i32 10, 0
	call void @putch(i32 %reg_8)
	%reg_9 = load i32, ptr %reg_0
	%reg_10 = add i32 1, 0
	%reg_11 = sub i32 %reg_9, %reg_10
	store i32 %reg_11, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 46
	%reg_12 = add i32 0, 0
	ret i32 %reg_12
}
