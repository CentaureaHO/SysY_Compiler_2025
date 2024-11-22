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

@ans = global [50 x i32] zeroinitializer
@sum = global i32 0
@n = global i32 zeroinitializer
@row = global [50 x i32] zeroinitializer
@line1 = global [50 x i32] zeroinitializer
@line2 = global [100 x i32] zeroinitializer

define void @printans()
{
Block0:  ;Func define at line 5
	%reg_3 = alloca i32
	br label %Block1
Block1:  ;Func body at line 5
	%reg_0 = load i32, ptr @sum
	%reg_1 = add i32 1, 0
	%reg_2 = add i32 %reg_0, %reg_1
	store i32 %reg_2, ptr @sum
	%reg_4 = add i32 1, 0
	store i32 %reg_4, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 9
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = load i32, ptr @n
	%reg_7 = icmp sle i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 9
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = getelementptr [50 x i32], ptr @ans, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	call void @putint(i32 %reg_10)
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = load i32, ptr @n
	%reg_13 = icmp eq i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block5, label %Block7
Block4:  ;While end at line 9
	ret void
Block5:  ;If then at line 11
	%reg_14 = add i32 10, 0
	call void @putch(i32 %reg_14)
	ret void
	br label %Block6
Block6:  ;If end at line 11
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = add i32 1, 0
	%reg_18 = add i32 %reg_16, %reg_17
	store i32 %reg_18, ptr %reg_3
	br label %Block2
Block7:  ;If else at line 11
	%reg_15 = add i32 32, 0
	call void @putch(i32 %reg_15)
	br label %Block6
}

define void @f(i32 %reg_0)
{
Block0:  ;Func define at line 20
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 20
	%reg_3 = add i32 1, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 23
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @n
	%reg_6 = icmp sle i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 23
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = getelementptr [50 x i32], ptr @row, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = add i32 1, 0
	%reg_11 = icmp ne i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block8, label %Block6
Block4:  ;While end at line 23
	ret void
Block5:  ;If then at line 24
	%reg_27 = load i32, ptr %reg_1
	%reg_28 = getelementptr [50 x i32], ptr @ans, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_2
	store i32 %reg_29, ptr %reg_28
	%reg_30 = load i32, ptr %reg_1
	%reg_31 = load i32, ptr @n
	%reg_32 = icmp eq i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block9, label %Block10
Block6:  ;If end at line 24
	%reg_66 = load i32, ptr %reg_2
	%reg_67 = add i32 1, 0
	%reg_68 = add i32 %reg_66, %reg_67
	store i32 %reg_68, ptr %reg_2
	br label %Block2
Block7:  ;And opertor at line 24
	%reg_19 = load i32, ptr @n
	%reg_20 = load i32, ptr %reg_1
	%reg_21 = add i32 %reg_19, %reg_20
	%reg_22 = load i32, ptr %reg_2
	%reg_23 = sub i32 %reg_21, %reg_22
	%reg_24 = getelementptr [100 x i32], ptr @line2, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = icmp eq i32 %reg_25, 0
	br i1 %reg_26, label %Block5, label %Block6
Block8:  ;And opertor at line -1
	%reg_12 = load i32, ptr %reg_1
	%reg_13 = load i32, ptr %reg_2
	%reg_14 = add i32 %reg_12, %reg_13
	%reg_15 = getelementptr [50 x i32], ptr @line1, i32 0, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = add i32 0, 0
	%reg_18 = icmp eq i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block7, label %Block6
Block9:  ;If then at line 26
	call void @printans()
	br label %Block10
Block10:  ;If end at line 26
	%reg_33 = load i32, ptr %reg_2
	%reg_34 = getelementptr [50 x i32], ptr @row, i32 0, i32 %reg_33
	%reg_35 = add i32 1, 0
	store i32 %reg_35, ptr %reg_34
	%reg_36 = load i32, ptr %reg_1
	%reg_37 = load i32, ptr %reg_2
	%reg_38 = add i32 %reg_36, %reg_37
	%reg_39 = getelementptr [50 x i32], ptr @line1, i32 0, i32 %reg_38
	%reg_40 = add i32 1, 0
	store i32 %reg_40, ptr %reg_39
	%reg_41 = load i32, ptr @n
	%reg_42 = load i32, ptr %reg_1
	%reg_43 = add i32 %reg_41, %reg_42
	%reg_44 = load i32, ptr %reg_2
	%reg_45 = sub i32 %reg_43, %reg_44
	%reg_46 = getelementptr [100 x i32], ptr @line2, i32 0, i32 %reg_45
	%reg_47 = add i32 1, 0
	store i32 %reg_47, ptr %reg_46
	%reg_48 = load i32, ptr %reg_1
	%reg_49 = add i32 1, 0
	%reg_50 = add i32 %reg_48, %reg_49
	call void @f(i32 %reg_50)
	%reg_51 = load i32, ptr %reg_2
	%reg_52 = getelementptr [50 x i32], ptr @row, i32 0, i32 %reg_51
	%reg_53 = add i32 0, 0
	store i32 %reg_53, ptr %reg_52
	%reg_54 = load i32, ptr %reg_1
	%reg_55 = load i32, ptr %reg_2
	%reg_56 = add i32 %reg_54, %reg_55
	%reg_57 = getelementptr [50 x i32], ptr @line1, i32 0, i32 %reg_56
	%reg_58 = add i32 0, 0
	store i32 %reg_58, ptr %reg_57
	%reg_59 = load i32, ptr @n
	%reg_60 = load i32, ptr %reg_1
	%reg_61 = add i32 %reg_59, %reg_60
	%reg_62 = load i32, ptr %reg_2
	%reg_63 = sub i32 %reg_61, %reg_62
	%reg_64 = getelementptr [100 x i32], ptr @line2, i32 0, i32 %reg_63
	%reg_65 = add i32 0, 0
	store i32 %reg_65, ptr %reg_64
	br label %Block6
}

define i32 @main()
{
Block0:  ;Func define at line 40
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 40
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 43
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = add i32 0, 0
	%reg_4 = icmp sgt i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 43
	%reg_5 = call i32 @getint()
	store i32 %reg_5, ptr @n
	%reg_6 = add i32 1, 0
	call void @f(i32 %reg_6)
	%reg_7 = load i32, ptr %reg_0
	%reg_8 = add i32 1, 0
	%reg_9 = sub i32 %reg_7, %reg_8
	store i32 %reg_9, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 43
	%reg_10 = load i32, ptr @sum
	ret i32 %reg_10
}
