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


define i32 @MAX(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 2
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 2
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 4
	%reg_7 = load i32, ptr %reg_2
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 4
	ret i32 0
Block4:  ;If else at line 4
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = icmp sgt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block5, label %Block7
Block5:  ;If then at line 6
	%reg_11 = load i32, ptr %reg_2
	ret i32 %reg_11
	br label %Block6
Block6:  ;If end at line 6
	br label %Block3
Block7:  ;If else at line 6
	%reg_12 = load i32, ptr %reg_3
	ret i32 %reg_12
	br label %Block6
}

define i32 @max_sum_nonadjacent(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 12
	%reg_18 = alloca i32
	%reg_3 = alloca [16 x i32]
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 12
	call void @llvm.memset.p0.i32(ptr %reg_3, i8 0, i32 64, i1 1)
	%reg_4 = add i32 0, 0
	%reg_5 = getelementptr [16 x i32], ptr %reg_3, i32 0, i32 %reg_4
	%reg_6 = add i32 0, 0
	%reg_7 = getelementptr i32, ptr %reg_0, i32 %reg_6
	%reg_8 = load i32, ptr %reg_7
	store i32 %reg_8, ptr %reg_5
	%reg_9 = add i32 1, 0
	%reg_10 = getelementptr [16 x i32], ptr %reg_3, i32 0, i32 %reg_9
	%reg_11 = add i32 0, 0
	%reg_12 = getelementptr i32, ptr %reg_0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = add i32 1, 0
	%reg_15 = getelementptr i32, ptr %reg_0, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = call i32 @MAX(i32 %reg_13, i32 %reg_16)
	store i32 %reg_17, ptr %reg_10
	%reg_19 = add i32 2, 0
	store i32 %reg_19, ptr %reg_18
	br label %Block2
Block2:  ;While condition at line 18
	%reg_20 = load i32, ptr %reg_18
	%reg_21 = load i32, ptr %reg_2
	%reg_22 = icmp slt i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block3, label %Block4
Block3:  ;While body at line 18
	%reg_23 = load i32, ptr %reg_18
	%reg_24 = getelementptr [16 x i32], ptr %reg_3, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_18
	%reg_26 = add i32 2, 0
	%reg_27 = sub i32 %reg_25, %reg_26
	%reg_28 = getelementptr [16 x i32], ptr %reg_3, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_28
	%reg_30 = load i32, ptr %reg_18
	%reg_31 = getelementptr i32, ptr %reg_0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	%reg_33 = add i32 %reg_29, %reg_32
	%reg_34 = load i32, ptr %reg_18
	%reg_35 = add i32 1, 0
	%reg_36 = sub i32 %reg_34, %reg_35
	%reg_37 = getelementptr [16 x i32], ptr %reg_3, i32 0, i32 %reg_36
	%reg_38 = load i32, ptr %reg_37
	%reg_39 = call i32 @MAX(i32 %reg_33, i32 %reg_38)
	store i32 %reg_39, ptr %reg_24
	%reg_40 = load i32, ptr %reg_18
	%reg_41 = add i32 1, 0
	%reg_42 = add i32 %reg_40, %reg_41
	store i32 %reg_42, ptr %reg_18
	br label %Block2
Block4:  ;While end at line 18
	%reg_43 = load i32, ptr %reg_2
	%reg_44 = add i32 1, 0
	%reg_45 = sub i32 %reg_43, %reg_44
	%reg_46 = getelementptr [16 x i32], ptr %reg_3, i32 0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	ret i32 %reg_47
}

define i32 @longest_common_subseq(ptr %reg_0, i32 %reg_1, ptr %reg_2, i32 %reg_3)
{
Block0:  ;Func define at line 25
	%reg_9 = alloca i32
	%reg_7 = alloca i32
	%reg_6 = alloca [16 x [16 x i32]]
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_3, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 25
	call void @llvm.memset.p0.i32(ptr %reg_6, i8 0, i32 1024, i1 1)
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 1, 0
	store i32 %reg_11, ptr %reg_7
	br label %Block2
Block2:  ;While condition at line 31
	%reg_12 = load i32, ptr %reg_7
	%reg_13 = load i32, ptr %reg_4
	%reg_14 = icmp sle i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 31
	%reg_15 = add i32 1, 0
	store i32 %reg_15, ptr %reg_9
	br label %Block5
Block4:  ;While end at line 31
	%reg_65 = load i32, ptr %reg_4
	%reg_66 = load i32, ptr %reg_5
	%reg_67 = getelementptr [16 x [16 x i32]], ptr %reg_6, i32 0, i32 %reg_65, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	ret i32 %reg_68
Block5:  ;While condition at line 33
	%reg_16 = load i32, ptr %reg_9
	%reg_17 = load i32, ptr %reg_5
	%reg_18 = icmp sle i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block6, label %Block7
Block6:  ;While body at line 33
	%reg_19 = load i32, ptr %reg_7
	%reg_20 = add i32 1, 0
	%reg_21 = sub i32 %reg_19, %reg_20
	%reg_22 = getelementptr i32, ptr %reg_0, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	%reg_24 = load i32, ptr %reg_9
	%reg_25 = add i32 1, 0
	%reg_26 = sub i32 %reg_24, %reg_25
	%reg_27 = getelementptr i32, ptr %reg_2, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	%reg_29 = icmp eq i32 %reg_23, %reg_28
	br i1 %reg_29, label %Block8, label %Block10
Block7:  ;While end at line 33
	%reg_62 = load i32, ptr %reg_7
	%reg_63 = add i32 1, 0
	%reg_64 = add i32 %reg_62, %reg_63
	store i32 %reg_64, ptr %reg_7
	br label %Block2
Block8:  ;If then at line 34
	%reg_30 = load i32, ptr %reg_7
	%reg_31 = load i32, ptr %reg_9
	%reg_32 = getelementptr [16 x [16 x i32]], ptr %reg_6, i32 0, i32 %reg_30, i32 %reg_31
	%reg_33 = load i32, ptr %reg_7
	%reg_34 = add i32 1, 0
	%reg_35 = sub i32 %reg_33, %reg_34
	%reg_36 = load i32, ptr %reg_9
	%reg_37 = add i32 1, 0
	%reg_38 = sub i32 %reg_36, %reg_37
	%reg_39 = getelementptr [16 x [16 x i32]], ptr %reg_6, i32 0, i32 %reg_35, i32 %reg_38
	%reg_40 = load i32, ptr %reg_39
	%reg_41 = add i32 1, 0
	%reg_42 = add i32 %reg_40, %reg_41
	store i32 %reg_42, ptr %reg_32
	br label %Block9
Block9:  ;If end at line 34
	%reg_59 = load i32, ptr %reg_9
	%reg_60 = add i32 1, 0
	%reg_61 = add i32 %reg_59, %reg_60
	store i32 %reg_61, ptr %reg_9
	br label %Block5
Block10:  ;If else at line 34
	%reg_43 = load i32, ptr %reg_7
	%reg_44 = load i32, ptr %reg_9
	%reg_45 = getelementptr [16 x [16 x i32]], ptr %reg_6, i32 0, i32 %reg_43, i32 %reg_44
	%reg_46 = load i32, ptr %reg_7
	%reg_47 = add i32 1, 0
	%reg_48 = sub i32 %reg_46, %reg_47
	%reg_49 = load i32, ptr %reg_9
	%reg_50 = getelementptr [16 x [16 x i32]], ptr %reg_6, i32 0, i32 %reg_48, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = load i32, ptr %reg_7
	%reg_53 = load i32, ptr %reg_9
	%reg_54 = add i32 1, 0
	%reg_55 = sub i32 %reg_53, %reg_54
	%reg_56 = getelementptr [16 x [16 x i32]], ptr %reg_6, i32 0, i32 %reg_52, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = call i32 @MAX(i32 %reg_51, i32 %reg_57)
	store i32 %reg_58, ptr %reg_45
	br label %Block9
}

define i32 @main()
{
Block0:  ;Func define at line 46
	%reg_60 = alloca i32
	%reg_58 = alloca i32
	%reg_31 = alloca [13 x i32]
	%reg_0 = alloca [15 x i32]
	br label %Block1
Block1:  ;Func body at line 46
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 60, i1 1)
	%reg_1 = add i32 8, 0
	%reg_2 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 0
	store i32 %reg_1, ptr %reg_2
	%reg_3 = add i32 7, 0
	%reg_4 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 1
	store i32 %reg_3, ptr %reg_4
	%reg_5 = add i32 4, 0
	%reg_6 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 2
	store i32 %reg_5, ptr %reg_6
	%reg_7 = add i32 1, 0
	%reg_8 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 3
	store i32 %reg_7, ptr %reg_8
	%reg_9 = add i32 2, 0
	%reg_10 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 4
	store i32 %reg_9, ptr %reg_10
	%reg_11 = add i32 7, 0
	%reg_12 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 5
	store i32 %reg_11, ptr %reg_12
	%reg_13 = add i32 0, 0
	%reg_14 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 6
	store i32 %reg_13, ptr %reg_14
	%reg_15 = add i32 1, 0
	%reg_16 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 7
	store i32 %reg_15, ptr %reg_16
	%reg_17 = add i32 9, 0
	%reg_18 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 8
	store i32 %reg_17, ptr %reg_18
	%reg_19 = add i32 3, 0
	%reg_20 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 9
	store i32 %reg_19, ptr %reg_20
	%reg_21 = add i32 4, 0
	%reg_22 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 10
	store i32 %reg_21, ptr %reg_22
	%reg_23 = add i32 8, 0
	%reg_24 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 11
	store i32 %reg_23, ptr %reg_24
	%reg_25 = add i32 3, 0
	%reg_26 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 12
	store i32 %reg_25, ptr %reg_26
	%reg_27 = add i32 7, 0
	%reg_28 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 13
	store i32 %reg_27, ptr %reg_28
	%reg_29 = add i32 0, 0
	%reg_30 = getelementptr [15 x i32], ptr %reg_0, i32 0, i32 14
	store i32 %reg_29, ptr %reg_30
	call void @llvm.memset.p0.i32(ptr %reg_31, i8 0, i32 52, i1 1)
	%reg_32 = add i32 3, 0
	%reg_33 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 0
	store i32 %reg_32, ptr %reg_33
	%reg_34 = add i32 9, 0
	%reg_35 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 1
	store i32 %reg_34, ptr %reg_35
	%reg_36 = add i32 7, 0
	%reg_37 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 2
	store i32 %reg_36, ptr %reg_37
	%reg_38 = add i32 1, 0
	%reg_39 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 3
	store i32 %reg_38, ptr %reg_39
	%reg_40 = add i32 4, 0
	%reg_41 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 4
	store i32 %reg_40, ptr %reg_41
	%reg_42 = add i32 2, 0
	%reg_43 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 5
	store i32 %reg_42, ptr %reg_43
	%reg_44 = add i32 4, 0
	%reg_45 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 6
	store i32 %reg_44, ptr %reg_45
	%reg_46 = add i32 3, 0
	%reg_47 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 7
	store i32 %reg_46, ptr %reg_47
	%reg_48 = add i32 6, 0
	%reg_49 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 8
	store i32 %reg_48, ptr %reg_49
	%reg_50 = add i32 8, 0
	%reg_51 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 9
	store i32 %reg_50, ptr %reg_51
	%reg_52 = add i32 0, 0
	%reg_53 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 10
	store i32 %reg_52, ptr %reg_53
	%reg_54 = add i32 1, 0
	%reg_55 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 11
	store i32 %reg_54, ptr %reg_55
	%reg_56 = add i32 5, 0
	%reg_57 = getelementptr [13 x i32], ptr %reg_31, i32 0, i32 12
	store i32 %reg_56, ptr %reg_57
	%reg_59 = add i32 0, 0
	store i32 %reg_59, ptr %reg_58
	%reg_61 = add i32 0, 0
	store i32 %reg_61, ptr %reg_60
	%reg_62 = getelementptr [15 x i32], ptr %reg_0, i32 0
	%reg_63 = add i32 15, 0
	%reg_64 = call i32 @max_sum_nonadjacent(ptr %reg_62, i32 %reg_63)
	call void @putint(i32 %reg_64)
	%reg_65 = add i32 10, 0
	call void @putch(i32 %reg_65)
	%reg_66 = getelementptr [15 x i32], ptr %reg_0, i32 0
	%reg_67 = add i32 15, 0
	%reg_68 = getelementptr [13 x i32], ptr %reg_31, i32 0
	%reg_69 = add i32 13, 0
	%reg_70 = call i32 @longest_common_subseq(ptr %reg_66, i32 %reg_67, ptr %reg_68, i32 %reg_69)
	call void @putint(i32 %reg_70)
	%reg_71 = add i32 10, 0
	call void @putch(i32 %reg_71)
	%reg_72 = add i32 0, 0
	ret i32 %reg_72
}
