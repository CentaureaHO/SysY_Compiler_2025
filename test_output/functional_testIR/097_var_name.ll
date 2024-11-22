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


define i32 @main()
{
Block0:  ;Func define at line 1
	%reg_9 = alloca i32
	%reg_4 = alloca [20 x i32]
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 1
	%reg_1 = add i32 2, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 20, 0
	store i32 %reg_3, ptr %reg_2
	call void @llvm.memset.p0.i32(ptr %reg_4, i8 0, i32 80, i1 1)
	%reg_5 = add i32 1, 0
	%reg_6 = getelementptr [20 x i32], ptr %reg_4, i32 0, i32 0
	store i32 %reg_5, ptr %reg_6
	%reg_7 = add i32 2, 0
	%reg_8 = getelementptr [20 x i32], ptr %reg_4, i32 0, i32 1
	store i32 %reg_7, ptr %reg_8
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block2
Block2:  ;While condition at line 6
	%reg_11 = load i32, ptr %reg_0
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block3, label %Block4
Block3:  ;While body at line 6
	%reg_14 = load i32, ptr %reg_0
	%reg_15 = getelementptr [20 x i32], ptr %reg_4, i32 0, i32 %reg_14
	%reg_16 = load i32, ptr %reg_0
	%reg_17 = getelementptr [20 x i32], ptr %reg_4, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = load i32, ptr %reg_0
	%reg_20 = add i32 1, 0
	%reg_21 = sub i32 %reg_19, %reg_20
	%reg_22 = getelementptr [20 x i32], ptr %reg_4, i32 0, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	%reg_24 = add i32 %reg_18, %reg_23
	%reg_25 = load i32, ptr %reg_0
	%reg_26 = add i32 2, 0
	%reg_27 = sub i32 %reg_25, %reg_26
	%reg_28 = getelementptr [20 x i32], ptr %reg_4, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_28
	%reg_30 = add i32 %reg_24, %reg_29
	store i32 %reg_30, ptr %reg_15
	%reg_31 = load i32, ptr %reg_9
	%reg_32 = load i32, ptr %reg_0
	%reg_33 = getelementptr [20 x i32], ptr %reg_4, i32 0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = add i32 %reg_31, %reg_34
	store i32 %reg_35, ptr %reg_9
	%reg_36 = load i32, ptr %reg_0
	%reg_37 = getelementptr [20 x i32], ptr %reg_4, i32 0, i32 %reg_36
	%reg_38 = load i32, ptr %reg_37
	call void @putint(i32 %reg_38)
	%reg_39 = add i32 10, 0
	call void @putch(i32 %reg_39)
	%reg_40 = load i32, ptr %reg_0
	%reg_41 = add i32 1, 0
	%reg_42 = add i32 %reg_40, %reg_41
	store i32 %reg_42, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 6
	%reg_43 = load i32, ptr %reg_9
	ret i32 %reg_43
}
