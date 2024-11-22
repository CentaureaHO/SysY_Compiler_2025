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

@N = global i32 zeroinitializer

define i32 @insert(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 3
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 3
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 9
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = getelementptr i32, ptr %reg_0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = icmp sgt i32 %reg_8, %reg_11
	br i1 %reg_12, label %Block5, label %Block4
Block3:  ;While body at line 9
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = add i32 1, 0
	%reg_18 = add i32 %reg_16, %reg_17
	store i32 %reg_18, ptr %reg_3
	br label %Block2
Block4:  ;While end at line 9
	%reg_19 = load i32, ptr @N
	store i32 %reg_19, ptr %reg_5
	br label %Block6
Block5:  ;And opertor at line 9
	%reg_13 = load i32, ptr %reg_3
	%reg_14 = load i32, ptr @N
	%reg_15 = icmp slt i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block3, label %Block4
Block6:  ;While condition at line 13
	%reg_20 = load i32, ptr %reg_5
	%reg_21 = load i32, ptr %reg_3
	%reg_22 = icmp sgt i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block7, label %Block8
Block7:  ;While body at line 13
	%reg_23 = load i32, ptr %reg_5
	%reg_24 = getelementptr i32, ptr %reg_0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_5
	%reg_26 = add i32 1, 0
	%reg_27 = sub i32 %reg_25, %reg_26
	%reg_28 = getelementptr i32, ptr %reg_0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_28
	store i32 %reg_29, ptr %reg_24
	%reg_30 = load i32, ptr %reg_3
	%reg_31 = getelementptr i32, ptr %reg_0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_2
	store i32 %reg_32, ptr %reg_31
	%reg_33 = load i32, ptr %reg_5
	%reg_34 = add i32 1, 0
	%reg_35 = sub i32 %reg_33, %reg_34
	store i32 %reg_35, ptr %reg_5
	br label %Block6
Block8:  ;While end at line 13
	%reg_36 = add i32 0, 0
	ret i32 %reg_36
}

define i32 @main()
{
Block0:  ;Func define at line 24
	%reg_34 = alloca i32
	%reg_32 = alloca i32
	%reg_1 = alloca [11 x i32]
	br label %Block1
Block1:  ;Func body at line 24
	%reg_0 = add i32 10, 0
	store i32 %reg_0, ptr @N
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 44, i1 1)
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_2
	%reg_4 = add i32 1, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = add i32 1, 0
	%reg_6 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_5
	%reg_7 = add i32 3, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = add i32 2, 0
	%reg_9 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_8
	%reg_10 = add i32 4, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 3, 0
	%reg_12 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_11
	%reg_13 = add i32 7, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = add i32 4, 0
	%reg_15 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_14
	%reg_16 = add i32 8, 0
	store i32 %reg_16, ptr %reg_15
	%reg_17 = add i32 5, 0
	%reg_18 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_17
	%reg_19 = add i32 11, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = add i32 6, 0
	%reg_21 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_20
	%reg_22 = add i32 13, 0
	store i32 %reg_22, ptr %reg_21
	%reg_23 = add i32 7, 0
	%reg_24 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_23
	%reg_25 = add i32 18, 0
	store i32 %reg_25, ptr %reg_24
	%reg_26 = add i32 8, 0
	%reg_27 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_26
	%reg_28 = add i32 56, 0
	store i32 %reg_28, ptr %reg_27
	%reg_29 = add i32 9, 0
	%reg_30 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_29
	%reg_31 = add i32 78, 0
	store i32 %reg_31, ptr %reg_30
	%reg_33 = add i32 0, 0
	store i32 %reg_33, ptr %reg_32
	%reg_35 = add i32 0, 0
	store i32 %reg_35, ptr %reg_34
	%reg_36 = add i32 0, 0
	store i32 %reg_36, ptr %reg_34
	%reg_37 = call i32 @getint()
	store i32 %reg_37, ptr %reg_32
	%reg_38 = getelementptr [11 x i32], ptr %reg_1, i32 0
	%reg_39 = load i32, ptr %reg_32
	%reg_40 = call i32 @insert(ptr %reg_38, i32 %reg_39)
	store i32 %reg_40, ptr %reg_32
	br label %Block2
Block2:  ;While condition at line 45
	%reg_41 = load i32, ptr %reg_34
	%reg_42 = load i32, ptr @N
	%reg_43 = icmp slt i32 %reg_41, %reg_42
	br i1 %reg_43, label %Block3, label %Block4
Block3:  ;While body at line 45
	%reg_44 = load i32, ptr %reg_34
	%reg_45 = getelementptr [11 x i32], ptr %reg_1, i32 0, i32 %reg_44
	%reg_46 = load i32, ptr %reg_45
	store i32 %reg_46, ptr %reg_32
	%reg_47 = load i32, ptr %reg_32
	call void @putint(i32 %reg_47)
	%reg_48 = add i32 10, 0
	store i32 %reg_48, ptr %reg_32
	%reg_49 = load i32, ptr %reg_32
	call void @putch(i32 %reg_49)
	%reg_50 = load i32, ptr %reg_34
	%reg_51 = add i32 1, 0
	%reg_52 = add i32 %reg_50, %reg_51
	store i32 %reg_52, ptr %reg_34
	br label %Block2
Block4:  ;While end at line 45
	%reg_53 = add i32 0, 0
	ret i32 %reg_53
}
