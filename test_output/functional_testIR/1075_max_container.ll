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


define i32 @maxArea(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 1
	%reg_18 = alloca i32
	%reg_11 = alloca i32
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 1
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_3
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = add i32 1, 0
	%reg_10 = sub i32 %reg_8, %reg_9
	store i32 %reg_10, ptr %reg_5
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_11
	%reg_13 = add i32 1, 0
	%reg_14 = sub i32 0, %reg_13
	store i32 %reg_14, ptr %reg_11
	br label %Block2
Block2:  ;While condition at line 8
	%reg_15 = load i32, ptr %reg_3
	%reg_16 = load i32, ptr %reg_5
	%reg_17 = icmp slt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block3, label %Block4
Block3:  ;While body at line 8
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = getelementptr i32, ptr %reg_0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	%reg_23 = load i32, ptr %reg_5
	%reg_24 = getelementptr i32, ptr %reg_0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = icmp slt i32 %reg_22, %reg_25
	br i1 %reg_26, label %Block5, label %Block7
Block4:  ;While end at line 8
	%reg_58 = load i32, ptr %reg_11
	ret i32 %reg_58
Block5:  ;If then at line 10
	%reg_27 = load i32, ptr %reg_5
	%reg_28 = load i32, ptr %reg_3
	%reg_29 = sub i32 %reg_27, %reg_28
	%reg_30 = load i32, ptr %reg_3
	%reg_31 = getelementptr i32, ptr %reg_0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	%reg_33 = mul i32 %reg_29, %reg_32
	store i32 %reg_33, ptr %reg_18
	br label %Block6
Block6:  ;If end at line 10
	%reg_41 = load i32, ptr %reg_18
	%reg_42 = load i32, ptr %reg_11
	%reg_43 = icmp sgt i32 %reg_41, %reg_42
	br i1 %reg_43, label %Block8, label %Block9
Block7:  ;If else at line 10
	%reg_34 = load i32, ptr %reg_5
	%reg_35 = load i32, ptr %reg_3
	%reg_36 = sub i32 %reg_34, %reg_35
	%reg_37 = load i32, ptr %reg_5
	%reg_38 = getelementptr i32, ptr %reg_0, i32 %reg_37
	%reg_39 = load i32, ptr %reg_38
	%reg_40 = mul i32 %reg_36, %reg_39
	store i32 %reg_40, ptr %reg_18
	br label %Block6
Block8:  ;If then at line 14
	%reg_44 = load i32, ptr %reg_18
	store i32 %reg_44, ptr %reg_11
	br label %Block9
Block9:  ;If end at line 14
	%reg_45 = load i32, ptr %reg_3
	%reg_46 = getelementptr i32, ptr %reg_0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	%reg_48 = load i32, ptr %reg_5
	%reg_49 = getelementptr i32, ptr %reg_0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	%reg_51 = icmp sgt i32 %reg_47, %reg_50
	br i1 %reg_51, label %Block10, label %Block12
Block10:  ;If then at line 17
	%reg_52 = load i32, ptr %reg_5
	%reg_53 = add i32 1, 0
	%reg_54 = sub i32 %reg_52, %reg_53
	store i32 %reg_54, ptr %reg_5
	br label %Block11
Block11:  ;If end at line 17
	br label %Block2
Block12:  ;If else at line 17
	%reg_55 = load i32, ptr %reg_3
	%reg_56 = add i32 1, 0
	%reg_57 = add i32 %reg_55, %reg_56
	store i32 %reg_57, ptr %reg_3
	br label %Block11
}

define i32 @main()
{
Block0:  ;Func define at line 25
	%reg_2 = alloca [10 x i32]
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 25
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
	%reg_34 = getelementptr [10 x i32], ptr %reg_2, i32 0
	%reg_35 = load i32, ptr %reg_0
	%reg_36 = call i32 @maxArea(ptr %reg_34, i32 %reg_35)
	store i32 %reg_36, ptr %reg_0
	%reg_37 = load i32, ptr %reg_0
	ret i32 %reg_37
}
