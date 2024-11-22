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


define i32 @maxSubArray(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 1
	%reg_21 = alloca i32
	%reg_18 = alloca i32
	%reg_13 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 1
	%reg_3 = load i32, ptr %reg_2
	%reg_4 = add i32 0, 0
	%reg_5 = icmp eq i32 %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 2
	%reg_6 = add i32 0, 0
	ret i32 %reg_6
	br label %Block3
Block3:  ;If end at line 2
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = add i32 1, 0
	%reg_9 = icmp eq i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block4, label %Block5
Block4:  ;If then at line 4
	%reg_10 = add i32 0, 0
	%reg_11 = getelementptr i32, ptr %reg_0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	ret i32 %reg_12
	br label %Block5
Block5:  ;If end at line 4
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_13
	%reg_15 = add i32 0, 0
	%reg_16 = getelementptr i32, ptr %reg_0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	store i32 %reg_17, ptr %reg_13
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = load i32, ptr %reg_13
	store i32 %reg_20, ptr %reg_18
	%reg_22 = add i32 0, 0
	store i32 %reg_22, ptr %reg_21
	%reg_23 = add i32 1, 0
	store i32 %reg_23, ptr %reg_21
	br label %Block6
Block6:  ;While condition at line 12
	%reg_24 = load i32, ptr %reg_21
	%reg_25 = load i32, ptr %reg_2
	%reg_26 = icmp slt i32 %reg_24, %reg_25
	br i1 %reg_26, label %Block7, label %Block8
Block7:  ;While body at line 12
	%reg_27 = load i32, ptr %reg_13
	%reg_28 = add i32 0, 0
	%reg_29 = icmp slt i32 %reg_27, %reg_28
	br i1 %reg_29, label %Block9, label %Block10
Block8:  ;While end at line 12
	%reg_43 = load i32, ptr %reg_18
	ret i32 %reg_43
Block9:  ;If then at line 13
	%reg_30 = add i32 0, 0
	store i32 %reg_30, ptr %reg_13
	br label %Block10
Block10:  ;If end at line 13
	%reg_31 = load i32, ptr %reg_13
	%reg_32 = load i32, ptr %reg_21
	%reg_33 = getelementptr i32, ptr %reg_0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = add i32 %reg_31, %reg_34
	store i32 %reg_35, ptr %reg_13
	%reg_36 = load i32, ptr %reg_18
	%reg_37 = load i32, ptr %reg_13
	%reg_38 = icmp slt i32 %reg_36, %reg_37
	br i1 %reg_38, label %Block11, label %Block12
Block11:  ;If then at line 16
	%reg_39 = load i32, ptr %reg_13
	store i32 %reg_39, ptr %reg_18
	br label %Block12
Block12:  ;If end at line 16
	%reg_40 = load i32, ptr %reg_21
	%reg_41 = add i32 1, 0
	%reg_42 = add i32 %reg_40, %reg_41
	store i32 %reg_42, ptr %reg_21
	br label %Block6
}

define i32 @main()
{
Block0:  ;Func define at line 22
	%reg_2 = alloca [10 x i32]
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 22
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 40, i1 1)
	%reg_3 = add i32 0, 0
	%reg_4 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_3
	%reg_5 = add i32 4, 0
	%reg_6 = sub i32 0, %reg_5
	store i32 %reg_6, ptr %reg_4
	%reg_7 = add i32 1, 0
	%reg_8 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_7
	%reg_9 = add i32 3, 0
	store i32 %reg_9, ptr %reg_8
	%reg_10 = add i32 2, 0
	%reg_11 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_10
	%reg_12 = add i32 9, 0
	store i32 %reg_12, ptr %reg_11
	%reg_13 = add i32 3, 0
	%reg_14 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_13
	%reg_15 = add i32 2, 0
	%reg_16 = sub i32 0, %reg_15
	store i32 %reg_16, ptr %reg_14
	%reg_17 = add i32 4, 0
	%reg_18 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_17
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = add i32 5, 0
	%reg_21 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_20
	%reg_22 = add i32 1, 0
	store i32 %reg_22, ptr %reg_21
	%reg_23 = add i32 6, 0
	%reg_24 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_23
	%reg_25 = add i32 6, 0
	%reg_26 = sub i32 0, %reg_25
	store i32 %reg_26, ptr %reg_24
	%reg_27 = add i32 7, 0
	%reg_28 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_27
	%reg_29 = add i32 5, 0
	store i32 %reg_29, ptr %reg_28
	%reg_30 = add i32 8, 0
	%reg_31 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_30
	%reg_32 = add i32 7, 0
	store i32 %reg_32, ptr %reg_31
	%reg_33 = add i32 9, 0
	%reg_34 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_33
	%reg_35 = add i32 8, 0
	store i32 %reg_35, ptr %reg_34
	%reg_36 = add i32 10, 0
	store i32 %reg_36, ptr %reg_0
	%reg_37 = getelementptr [10 x i32], ptr %reg_2, i32 0
	%reg_38 = load i32, ptr %reg_0
	%reg_39 = call i32 @maxSubArray(ptr %reg_37, i32 %reg_38)
	store i32 %reg_39, ptr %reg_0
	%reg_40 = load i32, ptr %reg_0
	ret i32 %reg_40
}
