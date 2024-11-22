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


define i32 @FourWhile()
{
Block0:  ;Func define at line 1
	%reg_9 = alloca i32
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 1
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = add i32 5, 0
	store i32 %reg_2, ptr %reg_0
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = add i32 6, 0
	store i32 %reg_7, ptr %reg_3
	%reg_8 = add i32 7, 0
	store i32 %reg_8, ptr %reg_5
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 10, 0
	store i32 %reg_11, ptr %reg_9
	br label %Block2
Block2:  ;While condition at line 10
	%reg_12 = load i32, ptr %reg_0
	%reg_13 = add i32 20, 0
	%reg_14 = icmp slt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 10
	%reg_15 = load i32, ptr %reg_0
	%reg_16 = add i32 3, 0
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_0
	br label %Block5
Block4:  ;While end at line 10
	%reg_45 = load i32, ptr %reg_0
	%reg_46 = load i32, ptr %reg_3
	%reg_47 = load i32, ptr %reg_9
	%reg_48 = add i32 %reg_46, %reg_47
	%reg_49 = add i32 %reg_45, %reg_48
	%reg_50 = load i32, ptr %reg_5
	%reg_51 = add i32 %reg_49, %reg_50
	ret i32 %reg_51
Block5:  ;While condition at line 12
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = add i32 10, 0
	%reg_20 = icmp slt i32 %reg_18, %reg_19
	br i1 %reg_20, label %Block6, label %Block7
Block6:  ;While body at line 12
	%reg_21 = load i32, ptr %reg_3
	%reg_22 = add i32 1, 0
	%reg_23 = add i32 %reg_21, %reg_22
	store i32 %reg_23, ptr %reg_3
	br label %Block8
Block7:  ;While end at line 12
	%reg_42 = load i32, ptr %reg_3
	%reg_43 = add i32 2, 0
	%reg_44 = sub i32 %reg_42, %reg_43
	store i32 %reg_44, ptr %reg_3
	br label %Block2
Block8:  ;While condition at line 14
	%reg_24 = load i32, ptr %reg_5
	%reg_25 = add i32 7, 0
	%reg_26 = icmp eq i32 %reg_24, %reg_25
	br i1 %reg_26, label %Block9, label %Block10
Block9:  ;While body at line 14
	%reg_27 = load i32, ptr %reg_5
	%reg_28 = add i32 1, 0
	%reg_29 = sub i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_5
	br label %Block11
Block10:  ;While end at line 14
	%reg_39 = load i32, ptr %reg_5
	%reg_40 = add i32 1, 0
	%reg_41 = add i32 %reg_39, %reg_40
	store i32 %reg_41, ptr %reg_5
	br label %Block5
Block11:  ;While condition at line 16
	%reg_30 = load i32, ptr %reg_9
	%reg_31 = add i32 20, 0
	%reg_32 = icmp slt i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block12, label %Block13
Block12:  ;While body at line 16
	%reg_33 = load i32, ptr %reg_9
	%reg_34 = add i32 3, 0
	%reg_35 = add i32 %reg_33, %reg_34
	store i32 %reg_35, ptr %reg_9
	br label %Block11
Block13:  ;While end at line 16
	%reg_36 = load i32, ptr %reg_9
	%reg_37 = add i32 1, 0
	%reg_38 = sub i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_9
	br label %Block8
}

define i32 @main()
{
Block0:  ;Func define at line 29
	br label %Block1
Block1:  ;Func body at line 29
	%reg_0 = call i32 @FourWhile()
	ret i32 %reg_0
}
