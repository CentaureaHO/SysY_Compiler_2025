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


define i32 @func(i32 %reg_0, ptr %reg_1, i32 %reg_2, ptr %reg_3, i32 %reg_4, i32 %reg_5, ptr %reg_6, i32 %reg_7, i32 %reg_8)
{
Block0:  ;Func define at line 2
	%reg_15 = alloca i32
	%reg_14 = alloca i32
	%reg_13 = alloca i32
	%reg_12 = alloca i32
	%reg_11 = alloca i32
	%reg_10 = alloca i32
	%reg_9 = alloca i32
	store i32 %reg_0, ptr %reg_9
	store i32 %reg_2, ptr %reg_10
	store i32 %reg_4, ptr %reg_11
	store i32 %reg_5, ptr %reg_12
	store i32 %reg_7, ptr %reg_13
	store i32 %reg_8, ptr %reg_14
	br label %Block1
Block1:  ;Func body at line 2
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	br label %Block2
Block2:  ;While condition at line 5
	%reg_17 = load i32, ptr %reg_15
	%reg_18 = add i32 10, 0
	%reg_19 = icmp slt i32 %reg_17, %reg_18
	br i1 %reg_19, label %Block3, label %Block4
Block3:  ;While body at line 5
	%reg_20 = load i32, ptr %reg_9
	%reg_21 = load i32, ptr %reg_15
	%reg_22 = getelementptr [59 x i32], ptr %reg_1, i32 %reg_20, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	call void @putint(i32 %reg_23)
	%reg_24 = load i32, ptr %reg_15
	%reg_25 = add i32 1, 0
	%reg_26 = add i32 %reg_24, %reg_25
	store i32 %reg_26, ptr %reg_15
	br label %Block2
Block4:  ;While end at line 5
	%reg_27 = add i32 10, 0
	call void @putch(i32 %reg_27)
	%reg_28 = load i32, ptr %reg_10
	%reg_29 = getelementptr i32, ptr %reg_3, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	call void @putint(i32 %reg_30)
	%reg_31 = add i32 10, 0
	call void @putch(i32 %reg_31)
	br label %Block5
Block5:  ;While condition at line 14
	%reg_32 = load i32, ptr %reg_14
	%reg_33 = add i32 10, 0
	%reg_34 = icmp slt i32 %reg_32, %reg_33
	br i1 %reg_34, label %Block6, label %Block7
Block6:  ;While body at line 14
	%reg_35 = load i32, ptr %reg_14
	%reg_36 = getelementptr i32, ptr %reg_6, i32 %reg_35
	%reg_37 = load i32, ptr %reg_13
	%reg_38 = add i32 128875, 0
	%reg_39 = mul i32 %reg_37, %reg_38
	%reg_40 = add i32 3724, 0
	%reg_41 = srem i32 %reg_39, %reg_40
	store i32 %reg_41, ptr %reg_36
	%reg_42 = load i32, ptr %reg_14
	%reg_43 = add i32 1, 0
	%reg_44 = add i32 %reg_42, %reg_43
	store i32 %reg_44, ptr %reg_14
	%reg_45 = load i32, ptr %reg_13
	%reg_46 = add i32 7, 0
	%reg_47 = add i32 %reg_45, %reg_46
	store i32 %reg_47, ptr %reg_13
	br label %Block5
Block7:  ;While end at line 14
	%reg_48 = load i32, ptr %reg_11
	%reg_49 = load i32, ptr %reg_12
	%reg_50 = add i32 %reg_48, %reg_49
	ret i32 %reg_50
}

define i32 @main()
{
Block0:  ;Func define at line 23
	%reg_38 = alloca i32
	%reg_1 = alloca [53 x [59 x i32]]
	%reg_0 = alloca [61 x [67 x i32]]
	br label %Block1
Block1:  ;Func body at line 23
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 16348, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 12508, i1 1)
	%reg_2 = add i32 17, 0
	%reg_3 = add i32 1, 0
	%reg_4 = getelementptr [61 x [67 x i32]], ptr %reg_0, i32 0, i32 %reg_2, i32 %reg_3
	%reg_5 = add i32 6, 0
	store i32 %reg_5, ptr %reg_4
	%reg_6 = add i32 17, 0
	%reg_7 = add i32 3, 0
	%reg_8 = getelementptr [61 x [67 x i32]], ptr %reg_0, i32 0, i32 %reg_6, i32 %reg_7
	%reg_9 = add i32 7, 0
	store i32 %reg_9, ptr %reg_8
	%reg_10 = add i32 17, 0
	%reg_11 = add i32 4, 0
	%reg_12 = getelementptr [61 x [67 x i32]], ptr %reg_0, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = add i32 4, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = add i32 17, 0
	%reg_15 = add i32 7, 0
	%reg_16 = getelementptr [61 x [67 x i32]], ptr %reg_0, i32 0, i32 %reg_14, i32 %reg_15
	%reg_17 = add i32 9, 0
	store i32 %reg_17, ptr %reg_16
	%reg_18 = add i32 17, 0
	%reg_19 = add i32 11, 0
	%reg_20 = getelementptr [61 x [67 x i32]], ptr %reg_0, i32 0, i32 %reg_18, i32 %reg_19
	%reg_21 = add i32 11, 0
	store i32 %reg_21, ptr %reg_20
	%reg_22 = add i32 6, 0
	%reg_23 = add i32 1, 0
	%reg_24 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_22, i32 %reg_23
	%reg_25 = add i32 1, 0
	store i32 %reg_25, ptr %reg_24
	%reg_26 = add i32 6, 0
	%reg_27 = add i32 2, 0
	%reg_28 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_26, i32 %reg_27
	%reg_29 = add i32 2, 0
	store i32 %reg_29, ptr %reg_28
	%reg_30 = add i32 6, 0
	%reg_31 = add i32 3, 0
	%reg_32 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_30, i32 %reg_31
	%reg_33 = add i32 3, 0
	store i32 %reg_33, ptr %reg_32
	%reg_34 = add i32 6, 0
	%reg_35 = add i32 9, 0
	%reg_36 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_34, i32 %reg_35
	%reg_37 = add i32 9, 0
	store i32 %reg_37, ptr %reg_36
	%reg_39 = add i32 0, 0
	store i32 %reg_39, ptr %reg_38
	%reg_40 = add i32 17, 0
	%reg_41 = add i32 1, 0
	%reg_42 = getelementptr [61 x [67 x i32]], ptr %reg_0, i32 0, i32 %reg_40, i32 %reg_41
	%reg_43 = load i32, ptr %reg_42
	%reg_44 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0
	%reg_45 = add i32 17, 0
	%reg_46 = add i32 3, 0
	%reg_47 = getelementptr [61 x [67 x i32]], ptr %reg_0, i32 0, i32 %reg_45, i32 %reg_46
	%reg_48 = load i32, ptr %reg_47
	%reg_49 = add i32 17, 0
	%reg_50 = getelementptr [61 x [67 x i32]], ptr %reg_0, i32 0, i32 %reg_49
	%reg_51 = add i32 6, 0
	%reg_52 = add i32 3, 0
	%reg_53 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_51, i32 %reg_52
	%reg_54 = load i32, ptr %reg_53
	%reg_55 = add i32 6, 0
	%reg_56 = add i32 0, 0
	%reg_57 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_55, i32 %reg_56
	%reg_58 = load i32, ptr %reg_57
	%reg_59 = add i32 6, 0
	%reg_60 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_59
	%reg_61 = add i32 34, 0
	%reg_62 = add i32 4, 0
	%reg_63 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_61, i32 %reg_62
	%reg_64 = load i32, ptr %reg_63
	%reg_65 = add i32 51, 0
	%reg_66 = add i32 18, 0
	%reg_67 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_65, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	%reg_69 = call i32 @func(i32 %reg_43, ptr %reg_44, i32 %reg_48, ptr %reg_50, i32 %reg_54, i32 %reg_58, ptr %reg_60, i32 %reg_64, i32 %reg_68)
	%reg_70 = add i32 3, 0
	%reg_71 = mul i32 %reg_69, %reg_70
	store i32 %reg_71, ptr %reg_38
	br label %Block2
Block2:  ;While condition at line 42
	%reg_72 = load i32, ptr %reg_38
	%reg_73 = add i32 0, 0
	%reg_74 = icmp sge i32 %reg_72, %reg_73
	br i1 %reg_74, label %Block3, label %Block4
Block3:  ;While body at line 42
	%reg_75 = add i32 6, 0
	%reg_76 = load i32, ptr %reg_38
	%reg_77 = getelementptr [53 x [59 x i32]], ptr %reg_1, i32 0, i32 %reg_75, i32 %reg_76
	%reg_78 = load i32, ptr %reg_77
	call void @putint(i32 %reg_78)
	%reg_79 = add i32 32, 0
	call void @putch(i32 %reg_79)
	%reg_80 = load i32, ptr %reg_38
	%reg_81 = add i32 1, 0
	%reg_82 = sub i32 %reg_80, %reg_81
	store i32 %reg_82, ptr %reg_38
	br label %Block2
Block4:  ;While end at line 42
	%reg_83 = add i32 10, 0
	call void @putch(i32 %reg_83)
	%reg_84 = add i32 0, 0
	ret i32 %reg_84
}
