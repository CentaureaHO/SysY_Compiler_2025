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


define void @get_next(ptr %reg_0, ptr %reg_1)
{
Block0:  ;Func define at line 2
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	br label %Block1
Block1:  ;Func body at line 2
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr i32, ptr %reg_1, i32 %reg_2
	%reg_4 = add i32 1, 0
	%reg_5 = sub i32 0, %reg_4
	store i32 %reg_5, ptr %reg_3
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = add i32 1, 0
	%reg_10 = sub i32 0, %reg_9
	store i32 %reg_10, ptr %reg_8
	br label %Block2
Block2:  ;While condition at line 6
	%reg_11 = load i32, ptr %reg_6
	%reg_12 = getelementptr i32, ptr %reg_0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = icmp ne i32 %reg_13, 0
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 6
	%reg_15 = load i32, ptr %reg_8
	%reg_16 = add i32 1, 0
	%reg_17 = sub i32 0, %reg_16
	%reg_18 = icmp eq i32 %reg_15, %reg_17
	br i1 %reg_18, label %Block5, label %Block8
Block4:  ;While end at line 6
	ret void
Block5:  ;If then at line 7
	%reg_26 = load i32, ptr %reg_8
	%reg_27 = add i32 1, 0
	%reg_28 = add i32 %reg_26, %reg_27
	store i32 %reg_28, ptr %reg_8
	%reg_29 = load i32, ptr %reg_6
	%reg_30 = add i32 1, 0
	%reg_31 = add i32 %reg_29, %reg_30
	store i32 %reg_31, ptr %reg_6
	%reg_32 = load i32, ptr %reg_6
	%reg_33 = getelementptr i32, ptr %reg_1, i32 %reg_32
	%reg_34 = load i32, ptr %reg_8
	store i32 %reg_34, ptr %reg_33
	br label %Block6
Block6:  ;If end at line 7
	br label %Block2
Block7:  ;If else at line 7
	%reg_35 = load i32, ptr %reg_8
	%reg_36 = getelementptr i32, ptr %reg_1, i32 %reg_35
	%reg_37 = load i32, ptr %reg_36
	store i32 %reg_37, ptr %reg_8
	br label %Block6
Block8:  ;Or opertor at line 7
	%reg_19 = load i32, ptr %reg_6
	%reg_20 = getelementptr i32, ptr %reg_0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = load i32, ptr %reg_8
	%reg_23 = getelementptr i32, ptr %reg_0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = icmp eq i32 %reg_21, %reg_24
	br i1 %reg_25, label %Block5, label %Block7
}

define i32 @KMP(ptr %reg_0, ptr %reg_1)
{
Block0:  ;Func define at line 17
	%reg_7 = alloca i32
	%reg_5 = alloca i32
	%reg_2 = alloca [4096 x i32]
	br label %Block1
Block1:  ;Func body at line 17
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 16384, i1 1)
	%reg_3 = getelementptr i32, ptr %reg_0
	%reg_4 = getelementptr [4096 x i32], ptr %reg_2, i32 0
	call void @get_next(ptr %reg_3, ptr %reg_4)
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	br label %Block2
Block2:  ;While condition at line 23
	%reg_9 = load i32, ptr %reg_7
	%reg_10 = getelementptr i32, ptr %reg_1, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = icmp ne i32 %reg_11, 0
	br i1 %reg_12, label %Block3, label %Block4
Block3:  ;While body at line 23
	%reg_13 = load i32, ptr %reg_5
	%reg_14 = getelementptr i32, ptr %reg_0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	%reg_16 = load i32, ptr %reg_7
	%reg_17 = getelementptr i32, ptr %reg_1, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = icmp eq i32 %reg_15, %reg_18
	br i1 %reg_19, label %Block5, label %Block7
Block4:  ;While end at line 23
	%reg_44 = add i32 1, 0
	%reg_45 = sub i32 0, %reg_44
	ret i32 %reg_45
Block5:  ;If then at line 24
	%reg_20 = load i32, ptr %reg_5
	%reg_21 = add i32 1, 0
	%reg_22 = add i32 %reg_20, %reg_21
	store i32 %reg_22, ptr %reg_5
	%reg_23 = load i32, ptr %reg_7
	%reg_24 = add i32 1, 0
	%reg_25 = add i32 %reg_23, %reg_24
	store i32 %reg_25, ptr %reg_7
	%reg_26 = load i32, ptr %reg_5
	%reg_27 = getelementptr i32, ptr %reg_0, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	%reg_29 = icmp eq i32 %reg_28, 0
	br i1 %reg_29, label %Block8, label %Block9
Block6:  ;If end at line 24
	br label %Block2
Block7:  ;If else at line 24
	%reg_31 = load i32, ptr %reg_5
	%reg_32 = getelementptr [4096 x i32], ptr %reg_2, i32 0, i32 %reg_31
	%reg_33 = load i32, ptr %reg_32
	store i32 %reg_33, ptr %reg_5
	%reg_34 = load i32, ptr %reg_5
	%reg_35 = add i32 1, 0
	%reg_36 = sub i32 0, %reg_35
	%reg_37 = icmp eq i32 %reg_34, %reg_36
	br i1 %reg_37, label %Block10, label %Block11
Block8:  ;If then at line 27
	%reg_30 = load i32, ptr %reg_7
	ret i32 %reg_30
	br label %Block9
Block9:  ;If end at line 27
	br label %Block6
Block10:  ;If then at line 32
	%reg_38 = load i32, ptr %reg_5
	%reg_39 = add i32 1, 0
	%reg_40 = add i32 %reg_38, %reg_39
	store i32 %reg_40, ptr %reg_5
	%reg_41 = load i32, ptr %reg_7
	%reg_42 = add i32 1, 0
	%reg_43 = add i32 %reg_41, %reg_42
	store i32 %reg_43, ptr %reg_7
	br label %Block11
Block11:  ;If end at line 32
	br label %Block6
}

define i32 @read_str(ptr %reg_0)
{
Block0:  ;Func define at line 41
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 41
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	br label %Block2
Block2:  ;While condition at line 44
	%reg_3 = add i32 1, 0
	%reg_4 = icmp ne i32 %reg_3, 0
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 44
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = getelementptr i32, ptr %reg_0, i32 %reg_5
	%reg_7 = call i32 @getch()
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = getelementptr i32, ptr %reg_0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = add i32 10, 0
	%reg_12 = icmp eq i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block5, label %Block6
Block4:  ;While end at line 44
	%reg_16 = load i32, ptr %reg_1
	%reg_17 = getelementptr i32, ptr %reg_0, i32 %reg_16
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_17
	%reg_19 = load i32, ptr %reg_1
	ret i32 %reg_19
Block5:  ;If then at line 46
	br label %Block4
Block6:  ;If end at line 46
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_1
	br label %Block2
Block7:  ;Break at line 47
	br label %Block6
}

define i32 @main()
{
Block0:  ;Func define at line 54
	%reg_1 = alloca [4096 x i32]
	%reg_0 = alloca [4096 x i32]
	br label %Block1
Block1:  ;Func body at line 54
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 16384, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 16384, i1 1)
	%reg_2 = getelementptr [4096 x i32], ptr %reg_0, i32 0
	%reg_3 = call i32 @read_str(ptr %reg_2)
	%reg_4 = getelementptr [4096 x i32], ptr %reg_1, i32 0
	%reg_5 = call i32 @read_str(ptr %reg_4)
	%reg_6 = getelementptr [4096 x i32], ptr %reg_0, i32 0
	%reg_7 = getelementptr [4096 x i32], ptr %reg_1, i32 0
	%reg_8 = call i32 @KMP(ptr %reg_6, ptr %reg_7)
	call void @putint(i32 %reg_8)
	%reg_9 = add i32 10, 0
	call void @putch(i32 %reg_9)
	%reg_10 = add i32 0, 0
	ret i32 %reg_10
}
