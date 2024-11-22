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


define i32 @concat(ptr %reg_0, ptr %reg_1, ptr %reg_2)
{
Block0:  ;Func define at line 2
	%reg_17 = alloca i32
	%reg_3 = alloca i32
	br label %Block1
Block1:  ;Func body at line 2
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 6
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = add i32 3, 0
	%reg_8 = icmp slt i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 6
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = getelementptr i32, ptr %reg_2, i32 %reg_9
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = getelementptr i32, ptr %reg_0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	store i32 %reg_13, ptr %reg_10
	%reg_14 = load i32, ptr %reg_3
	%reg_15 = add i32 1, 0
	%reg_16 = add i32 %reg_14, %reg_15
	store i32 %reg_16, ptr %reg_3
	br label %Block2
Block4:  ;While end at line 6
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_17
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_17
	br label %Block5
Block5:  ;While condition at line 14
	%reg_20 = load i32, ptr %reg_17
	%reg_21 = add i32 3, 0
	%reg_22 = icmp slt i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block6, label %Block7
Block6:  ;While body at line 14
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = getelementptr i32, ptr %reg_2, i32 %reg_23
	%reg_25 = load i32, ptr %reg_17
	%reg_26 = getelementptr i32, ptr %reg_1, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	store i32 %reg_27, ptr %reg_24
	%reg_28 = load i32, ptr %reg_3
	%reg_29 = add i32 1, 0
	%reg_30 = add i32 %reg_28, %reg_29
	store i32 %reg_30, ptr %reg_3
	%reg_31 = load i32, ptr %reg_17
	%reg_32 = add i32 1, 0
	%reg_33 = add i32 %reg_31, %reg_32
	store i32 %reg_33, ptr %reg_17
	br label %Block5
Block7:  ;While end at line 14
	%reg_34 = add i32 0, 0
	ret i32 %reg_34
}

define i32 @main()
{
Block0:  ;Func define at line 25
	%reg_40 = alloca i32
	%reg_9 = alloca i32
	%reg_8 = alloca [3 x i32]
	%reg_7 = alloca [3 x i32]
	%reg_6 = alloca [6 x i32]
	%reg_5 = alloca [3 x i32]
	%reg_4 = alloca [3 x i32]
	%reg_3 = alloca [3 x i32]
	%reg_2 = alloca [3 x i32]
	%reg_1 = alloca [3 x i32]
	%reg_0 = alloca [3 x i32]
	br label %Block1
Block1:  ;Func body at line 25
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_3, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_4, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_5, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_6, i8 0, i32 24, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_7, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_8, i8 0, i32 12, i1 1)
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_9
	br label %Block2
Block2:  ;While condition at line 30
	%reg_12 = load i32, ptr %reg_9
	%reg_13 = add i32 3, 0
	%reg_14 = icmp slt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 30
	%reg_15 = load i32, ptr %reg_9
	%reg_16 = getelementptr [3 x i32], ptr %reg_0, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_9
	store i32 %reg_17, ptr %reg_16
	%reg_18 = load i32, ptr %reg_9
	%reg_19 = getelementptr [3 x i32], ptr %reg_1, i32 0, i32 %reg_18
	%reg_20 = load i32, ptr %reg_9
	store i32 %reg_20, ptr %reg_19
	%reg_21 = load i32, ptr %reg_9
	%reg_22 = getelementptr [3 x i32], ptr %reg_2, i32 0, i32 %reg_21
	%reg_23 = load i32, ptr %reg_9
	store i32 %reg_23, ptr %reg_22
	%reg_24 = load i32, ptr %reg_9
	%reg_25 = getelementptr [3 x i32], ptr %reg_3, i32 0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_9
	store i32 %reg_26, ptr %reg_25
	%reg_27 = load i32, ptr %reg_9
	%reg_28 = getelementptr [3 x i32], ptr %reg_4, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_9
	store i32 %reg_29, ptr %reg_28
	%reg_30 = load i32, ptr %reg_9
	%reg_31 = getelementptr [3 x i32], ptr %reg_5, i32 0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_9
	store i32 %reg_32, ptr %reg_31
	%reg_33 = load i32, ptr %reg_9
	%reg_34 = add i32 1, 0
	%reg_35 = add i32 %reg_33, %reg_34
	store i32 %reg_35, ptr %reg_9
	br label %Block2
Block4:  ;While end at line 30
	%reg_36 = getelementptr [3 x i32], ptr %reg_0, i32 0
	%reg_37 = getelementptr [3 x i32], ptr %reg_3, i32 0
	%reg_38 = getelementptr [6 x i32], ptr %reg_6, i32 0
	%reg_39 = call i32 @concat(ptr %reg_36, ptr %reg_37, ptr %reg_38)
	store i32 %reg_39, ptr %reg_9
	%reg_41 = add i32 0, 0
	store i32 %reg_41, ptr %reg_40
	br label %Block5
Block5:  ;While condition at line 42
	%reg_42 = load i32, ptr %reg_9
	%reg_43 = add i32 6, 0
	%reg_44 = icmp slt i32 %reg_42, %reg_43
	br i1 %reg_44, label %Block6, label %Block7
Block6:  ;While body at line 42
	%reg_45 = load i32, ptr %reg_9
	%reg_46 = getelementptr [6 x i32], ptr %reg_6, i32 0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	store i32 %reg_47, ptr %reg_40
	%reg_48 = load i32, ptr %reg_40
	call void @putint(i32 %reg_48)
	%reg_49 = load i32, ptr %reg_9
	%reg_50 = add i32 1, 0
	%reg_51 = add i32 %reg_49, %reg_50
	store i32 %reg_51, ptr %reg_9
	br label %Block5
Block7:  ;While end at line 42
	%reg_52 = add i32 10, 0
	store i32 %reg_52, ptr %reg_40
	%reg_53 = load i32, ptr %reg_40
	call void @putch(i32 %reg_53)
	%reg_54 = add i32 0, 0
	ret i32 %reg_54
}
