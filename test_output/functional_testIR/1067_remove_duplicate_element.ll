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


define i32 @removeElement(ptr %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 1
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_1, ptr %reg_3
	store i32 %reg_2, ptr %reg_4
	br label %Block1
Block1:  ;Func body at line 1
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_5
	br label %Block2
Block2:  ;While condition at line 4
	%reg_8 = load i32, ptr %reg_5
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = icmp slt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 4
	%reg_11 = load i32, ptr %reg_5
	%reg_12 = getelementptr i32, ptr %reg_0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = load i32, ptr %reg_4
	%reg_15 = icmp eq i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block5, label %Block7
Block4:  ;While end at line 4
	%reg_29 = load i32, ptr %reg_3
	ret i32 %reg_29
Block5:  ;If then at line 5
	%reg_16 = load i32, ptr %reg_5
	%reg_17 = getelementptr i32, ptr %reg_0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = add i32 1, 0
	%reg_20 = sub i32 %reg_18, %reg_19
	%reg_21 = getelementptr i32, ptr %reg_0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	store i32 %reg_22, ptr %reg_17
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = add i32 1, 0
	%reg_25 = sub i32 %reg_23, %reg_24
	store i32 %reg_25, ptr %reg_3
	br label %Block6
Block6:  ;If end at line 5
	br label %Block2
Block7:  ;If else at line 5
	%reg_26 = load i32, ptr %reg_5
	%reg_27 = add i32 1, 0
	%reg_28 = add i32 %reg_26, %reg_27
	store i32 %reg_28, ptr %reg_5
	br label %Block6
}

define i32 @main()
{
Block0:  ;Func define at line 14
	%reg_34 = alloca i32
	%reg_2 = alloca [10 x i32]
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 14
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 40, i1 1)
	%reg_3 = add i32 0, 0
	%reg_4 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_3
	%reg_5 = add i32 3, 0
	store i32 %reg_5, ptr %reg_4
	%reg_6 = add i32 1, 0
	%reg_7 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_6
	%reg_8 = add i32 3, 0
	store i32 %reg_8, ptr %reg_7
	%reg_9 = add i32 2, 0
	%reg_10 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_9
	%reg_11 = add i32 9, 0
	store i32 %reg_11, ptr %reg_10
	%reg_12 = add i32 3, 0
	%reg_13 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_12
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_13
	%reg_15 = add i32 4, 0
	%reg_16 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_15
	%reg_17 = add i32 0, 0
	store i32 %reg_17, ptr %reg_16
	%reg_18 = add i32 5, 0
	%reg_19 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_18
	%reg_20 = add i32 1, 0
	store i32 %reg_20, ptr %reg_19
	%reg_21 = add i32 6, 0
	%reg_22 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_21
	%reg_23 = add i32 1, 0
	store i32 %reg_23, ptr %reg_22
	%reg_24 = add i32 7, 0
	%reg_25 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_24
	%reg_26 = add i32 5, 0
	store i32 %reg_26, ptr %reg_25
	%reg_27 = add i32 8, 0
	%reg_28 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_27
	%reg_29 = add i32 7, 0
	store i32 %reg_29, ptr %reg_28
	%reg_30 = add i32 9, 0
	%reg_31 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_30
	%reg_32 = add i32 8, 0
	store i32 %reg_32, ptr %reg_31
	%reg_33 = add i32 10, 0
	store i32 %reg_33, ptr %reg_0
	%reg_35 = add i32 0, 0
	store i32 %reg_35, ptr %reg_34
	%reg_36 = add i32 3, 0
	store i32 %reg_36, ptr %reg_34
	%reg_37 = getelementptr [10 x i32], ptr %reg_2, i32 0
	%reg_38 = load i32, ptr %reg_0
	%reg_39 = load i32, ptr %reg_34
	%reg_40 = call i32 @removeElement(ptr %reg_37, i32 %reg_38, i32 %reg_39)
	store i32 %reg_40, ptr %reg_0
	%reg_41 = load i32, ptr %reg_0
	ret i32 %reg_41
}
