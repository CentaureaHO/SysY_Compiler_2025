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

@n = global i32 zeroinitializer

define i32 @swap(ptr %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 2
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_1, ptr %reg_3
	store i32 %reg_2, ptr %reg_4
	br label %Block1
Block1:  ;Func body at line 2
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = getelementptr i32, ptr %reg_0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	store i32 %reg_9, ptr %reg_5
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = getelementptr i32, ptr %reg_0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_4
	%reg_13 = getelementptr i32, ptr %reg_0, i32 %reg_12
	%reg_14 = load i32, ptr %reg_13
	store i32 %reg_14, ptr %reg_11
	%reg_15 = load i32, ptr %reg_4
	%reg_16 = getelementptr i32, ptr %reg_0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_5
	store i32 %reg_17, ptr %reg_16
	%reg_18 = add i32 0, 0
	ret i32 %reg_18
}

define i32 @heap_ajust(ptr %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 9
	%reg_8 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_1, ptr %reg_3
	store i32 %reg_2, ptr %reg_4
	br label %Block1
Block1:  ;Func body at line 9
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr %reg_3
	store i32 %reg_7, ptr %reg_5
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	%reg_10 = load i32, ptr %reg_5
	%reg_11 = add i32 2, 0
	%reg_12 = mul i32 %reg_10, %reg_11
	%reg_13 = add i32 1, 0
	%reg_14 = add i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_8
	br label %Block2
Block2:  ;While condition at line 14
	%reg_15 = load i32, ptr %reg_8
	%reg_16 = load i32, ptr %reg_4
	%reg_17 = add i32 1, 0
	%reg_18 = add i32 %reg_16, %reg_17
	%reg_19 = icmp slt i32 %reg_15, %reg_18
	br i1 %reg_19, label %Block3, label %Block4
Block3:  ;While body at line 14
	%reg_20 = load i32, ptr %reg_8
	%reg_21 = load i32, ptr %reg_4
	%reg_22 = icmp slt i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block7, label %Block6
Block4:  ;While end at line 14
	%reg_53 = add i32 0, 0
	ret i32 %reg_53
Block5:  ;If then at line 15
	%reg_32 = load i32, ptr %reg_8
	%reg_33 = add i32 1, 0
	%reg_34 = add i32 %reg_32, %reg_33
	store i32 %reg_34, ptr %reg_8
	br label %Block6
Block6:  ;If end at line 15
	%reg_35 = load i32, ptr %reg_5
	%reg_36 = getelementptr i32, ptr %reg_0, i32 %reg_35
	%reg_37 = load i32, ptr %reg_36
	%reg_38 = load i32, ptr %reg_8
	%reg_39 = getelementptr i32, ptr %reg_0, i32 %reg_38
	%reg_40 = load i32, ptr %reg_39
	%reg_41 = icmp sgt i32 %reg_37, %reg_40
	br i1 %reg_41, label %Block8, label %Block10
Block7:  ;And opertor at line 15
	%reg_23 = load i32, ptr %reg_8
	%reg_24 = getelementptr i32, ptr %reg_0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = load i32, ptr %reg_8
	%reg_27 = add i32 1, 0
	%reg_28 = add i32 %reg_26, %reg_27
	%reg_29 = getelementptr i32, ptr %reg_0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	%reg_31 = icmp slt i32 %reg_25, %reg_30
	br i1 %reg_31, label %Block5, label %Block6
Block8:  ;If then at line 17
	%reg_42 = add i32 0, 0
	ret i32 %reg_42
	br label %Block9
Block9:  ;If end at line 17
	br label %Block2
Block10:  ;If else at line 17
	%reg_43 = getelementptr i32, ptr %reg_0
	%reg_44 = load i32, ptr %reg_5
	%reg_45 = load i32, ptr %reg_8
	%reg_46 = call i32 @swap(ptr %reg_43, i32 %reg_44, i32 %reg_45)
	store i32 %reg_46, ptr %reg_5
	%reg_47 = load i32, ptr %reg_8
	store i32 %reg_47, ptr %reg_5
	%reg_48 = load i32, ptr %reg_5
	%reg_49 = add i32 2, 0
	%reg_50 = mul i32 %reg_48, %reg_49
	%reg_51 = add i32 1, 0
	%reg_52 = add i32 %reg_50, %reg_51
	store i32 %reg_52, ptr %reg_8
	br label %Block9
}

define i32 @heap_sort(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 27
	%reg_32 = alloca i32
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 27
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = add i32 2, 0
	%reg_9 = sdiv i32 %reg_7, %reg_8
	%reg_10 = add i32 1, 0
	%reg_11 = sub i32 %reg_9, %reg_10
	store i32 %reg_11, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 31
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = add i32 1, 0
	%reg_14 = sub i32 0, %reg_13
	%reg_15 = icmp sgt i32 %reg_12, %reg_14
	br i1 %reg_15, label %Block3, label %Block4
Block3:  ;While body at line 31
	%reg_16 = load i32, ptr %reg_2
	%reg_17 = add i32 1, 0
	%reg_18 = sub i32 %reg_16, %reg_17
	store i32 %reg_18, ptr %reg_5
	%reg_19 = getelementptr i32, ptr %reg_0
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = load i32, ptr %reg_5
	%reg_22 = call i32 @heap_ajust(ptr %reg_19, i32 %reg_20, i32 %reg_21)
	store i32 %reg_22, ptr %reg_5
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = add i32 1, 0
	%reg_25 = sub i32 %reg_23, %reg_24
	store i32 %reg_25, ptr %reg_3
	br label %Block2
Block4:  ;While end at line 31
	%reg_26 = load i32, ptr %reg_2
	%reg_27 = add i32 1, 0
	%reg_28 = sub i32 %reg_26, %reg_27
	store i32 %reg_28, ptr %reg_3
	br label %Block5
Block5:  ;While condition at line 37
	%reg_29 = load i32, ptr %reg_3
	%reg_30 = add i32 0, 0
	%reg_31 = icmp sgt i32 %reg_29, %reg_30
	br i1 %reg_31, label %Block6, label %Block7
Block6:  ;While body at line 37
	%reg_33 = add i32 0, 0
	store i32 %reg_33, ptr %reg_32
	%reg_34 = add i32 0, 0
	store i32 %reg_34, ptr %reg_32
	%reg_35 = getelementptr i32, ptr %reg_0
	%reg_36 = load i32, ptr %reg_32
	%reg_37 = load i32, ptr %reg_3
	%reg_38 = call i32 @swap(ptr %reg_35, i32 %reg_36, i32 %reg_37)
	store i32 %reg_38, ptr %reg_5
	%reg_39 = load i32, ptr %reg_3
	%reg_40 = add i32 1, 0
	%reg_41 = sub i32 %reg_39, %reg_40
	store i32 %reg_41, ptr %reg_5
	%reg_42 = getelementptr i32, ptr %reg_0
	%reg_43 = load i32, ptr %reg_32
	%reg_44 = load i32, ptr %reg_5
	%reg_45 = call i32 @heap_ajust(ptr %reg_42, i32 %reg_43, i32 %reg_44)
	store i32 %reg_45, ptr %reg_5
	%reg_46 = load i32, ptr %reg_3
	%reg_47 = add i32 1, 0
	%reg_48 = sub i32 %reg_46, %reg_47
	store i32 %reg_48, ptr %reg_3
	br label %Block5
Block7:  ;While end at line 37
	%reg_49 = add i32 0, 0
	ret i32 %reg_49
}

define i32 @main()
{
Block0:  ;Func define at line 48
	%reg_41 = alloca i32
	%reg_32 = alloca i32
	%reg_1 = alloca [10 x i32]
	br label %Block1
Block1:  ;Func body at line 48
	%reg_0 = add i32 10, 0
	store i32 %reg_0, ptr @n
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 40, i1 1)
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_2
	%reg_4 = add i32 4, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = add i32 1, 0
	%reg_6 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_5
	%reg_7 = add i32 3, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = add i32 2, 0
	%reg_9 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_8
	%reg_10 = add i32 9, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 3, 0
	%reg_12 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_11
	%reg_13 = add i32 2, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = add i32 4, 0
	%reg_15 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_14
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	%reg_17 = add i32 5, 0
	%reg_18 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_17
	%reg_19 = add i32 1, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = add i32 6, 0
	%reg_21 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_20
	%reg_22 = add i32 6, 0
	store i32 %reg_22, ptr %reg_21
	%reg_23 = add i32 7, 0
	%reg_24 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_23
	%reg_25 = add i32 5, 0
	store i32 %reg_25, ptr %reg_24
	%reg_26 = add i32 8, 0
	%reg_27 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_26
	%reg_28 = add i32 7, 0
	store i32 %reg_28, ptr %reg_27
	%reg_29 = add i32 9, 0
	%reg_30 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_29
	%reg_31 = add i32 8, 0
	store i32 %reg_31, ptr %reg_30
	%reg_33 = add i32 0, 0
	store i32 %reg_33, ptr %reg_32
	%reg_34 = add i32 0, 0
	store i32 %reg_34, ptr %reg_32
	%reg_35 = getelementptr [10 x i32], ptr %reg_1, i32 0
	%reg_36 = load i32, ptr @n
	%reg_37 = call i32 @heap_sort(ptr %reg_35, i32 %reg_36)
	store i32 %reg_37, ptr %reg_32
	br label %Block2
Block2:  ;While condition at line 56
	%reg_38 = load i32, ptr %reg_32
	%reg_39 = load i32, ptr @n
	%reg_40 = icmp slt i32 %reg_38, %reg_39
	br i1 %reg_40, label %Block3, label %Block4
Block3:  ;While body at line 56
	%reg_42 = add i32 0, 0
	store i32 %reg_42, ptr %reg_41
	%reg_43 = load i32, ptr %reg_32
	%reg_44 = getelementptr [10 x i32], ptr %reg_1, i32 0, i32 %reg_43
	%reg_45 = load i32, ptr %reg_44
	store i32 %reg_45, ptr %reg_41
	%reg_46 = load i32, ptr %reg_41
	call void @putint(i32 %reg_46)
	%reg_47 = add i32 10, 0
	store i32 %reg_47, ptr %reg_41
	%reg_48 = load i32, ptr %reg_41
	call void @putch(i32 %reg_48)
	%reg_49 = load i32, ptr %reg_32
	%reg_50 = add i32 1, 0
	%reg_51 = add i32 %reg_49, %reg_50
	store i32 %reg_51, ptr %reg_32
	br label %Block2
Block4:  ;While end at line 56
	%reg_52 = add i32 0, 0
	ret i32 %reg_52
}
