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

@A = global [100 x i32] zeroinitializer
@B = global [100 x i32] zeroinitializer
@C = global [100 x i32] zeroinitializer
@D = global [100 x i32] zeroinitializer
@p = global i32 234145

define void @f(ptr %reg_0, ptr %reg_1, ptr %reg_2, ptr %reg_3, i32 %reg_4)
{
Block0:  ;Func define at line 6
	%reg_9 = alloca i32
	%reg_5 = alloca i32
	store i32 %reg_4, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 6
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = add i32 0, 0
	%reg_8 = icmp eq i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block2:  ;If then at line 8
	ret void
	br label %Block3
Block3:  ;If end at line 8
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block4
Block4:  ;While condition at line 12
	%reg_11 = load i32, ptr %reg_9
	%reg_12 = add i32 100, 0
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block5, label %Block6
Block5:  ;While body at line 12
	%reg_14 = load i32, ptr %reg_9
	%reg_15 = getelementptr i32, ptr %reg_0, i32 %reg_14
	%reg_16 = load i32, ptr %reg_9
	%reg_17 = getelementptr i32, ptr %reg_2, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = load i32, ptr %reg_9
	%reg_20 = getelementptr i32, ptr %reg_1, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = add i32 %reg_18, %reg_21
	%reg_23 = load i32, ptr %reg_9
	%reg_24 = getelementptr i32, ptr %reg_3, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = add i32 %reg_22, %reg_25
	store i32 %reg_26, ptr %reg_15
	%reg_27 = load i32, ptr %reg_9
	%reg_28 = getelementptr i32, ptr %reg_1, i32 %reg_27
	%reg_29 = load i32, ptr %reg_9
	%reg_30 = getelementptr i32, ptr %reg_0, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	%reg_32 = load i32, ptr %reg_9
	%reg_33 = getelementptr i32, ptr %reg_2, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = add i32 %reg_31, %reg_34
	%reg_36 = load i32, ptr %reg_9
	%reg_37 = getelementptr i32, ptr %reg_3, i32 %reg_36
	%reg_38 = load i32, ptr %reg_37
	%reg_39 = add i32 %reg_35, %reg_38
	store i32 %reg_39, ptr %reg_28
	%reg_40 = load i32, ptr %reg_9
	%reg_41 = getelementptr i32, ptr %reg_2, i32 %reg_40
	%reg_42 = load i32, ptr %reg_9
	%reg_43 = getelementptr i32, ptr %reg_1, i32 %reg_42
	%reg_44 = load i32, ptr %reg_43
	%reg_45 = load i32, ptr %reg_9
	%reg_46 = getelementptr i32, ptr %reg_0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	%reg_48 = add i32 %reg_44, %reg_47
	%reg_49 = load i32, ptr %reg_9
	%reg_50 = getelementptr i32, ptr %reg_3, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = add i32 %reg_48, %reg_51
	store i32 %reg_52, ptr %reg_41
	%reg_53 = load i32, ptr %reg_9
	%reg_54 = getelementptr i32, ptr %reg_0, i32 %reg_53
	%reg_55 = load i32, ptr %reg_9
	%reg_56 = getelementptr i32, ptr %reg_0, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = load i32, ptr @p
	%reg_59 = srem i32 %reg_57, %reg_58
	store i32 %reg_59, ptr %reg_54
	%reg_60 = load i32, ptr %reg_9
	%reg_61 = getelementptr i32, ptr %reg_1, i32 %reg_60
	%reg_62 = load i32, ptr %reg_9
	%reg_63 = getelementptr i32, ptr %reg_1, i32 %reg_62
	%reg_64 = load i32, ptr %reg_63
	%reg_65 = load i32, ptr @p
	%reg_66 = srem i32 %reg_64, %reg_65
	store i32 %reg_66, ptr %reg_61
	%reg_67 = load i32, ptr %reg_9
	%reg_68 = getelementptr i32, ptr %reg_2, i32 %reg_67
	%reg_69 = load i32, ptr %reg_9
	%reg_70 = getelementptr i32, ptr %reg_2, i32 %reg_69
	%reg_71 = load i32, ptr %reg_70
	%reg_72 = load i32, ptr @p
	%reg_73 = srem i32 %reg_71, %reg_72
	store i32 %reg_73, ptr %reg_68
	%reg_74 = load i32, ptr %reg_9
	%reg_75 = add i32 1, 0
	%reg_76 = add i32 %reg_74, %reg_75
	store i32 %reg_76, ptr %reg_9
	br label %Block4
Block6:  ;While end at line 12
	%reg_77 = getelementptr i32, ptr %reg_1
	%reg_78 = getelementptr i32, ptr %reg_2
	%reg_79 = getelementptr i32, ptr %reg_0
	%reg_80 = getelementptr i32, ptr %reg_3
	%reg_81 = load i32, ptr %reg_5
	%reg_82 = add i32 1, 0
	%reg_83 = sub i32 %reg_81, %reg_82
	call void @f(ptr %reg_77, ptr %reg_78, ptr %reg_79, ptr %reg_80, i32 %reg_83)
	ret void
}

define i32 @sum(ptr %reg_0)
{
Block0:  ;Func define at line 23
	%reg_3 = alloca i32
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 23
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 27
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 100, 0
	%reg_7 = icmp slt i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 27
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = load i32, ptr %reg_1
	%reg_10 = getelementptr i32, ptr %reg_0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = add i32 %reg_8, %reg_11
	store i32 %reg_12, ptr %reg_3
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_1
	br label %Block2
Block4:  ;While end at line 27
	%reg_16 = load i32, ptr %reg_3
	ret i32 %reg_16
}

define i32 @main()
{
Block0:  ;Func define at line 33
	%reg_34 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 33
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 36
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = add i32 100, 0
	%reg_4 = icmp slt i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 36
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = getelementptr [100 x i32], ptr @A, i32 0, i32 %reg_5
	%reg_7 = add i32 1, 0
	%reg_8 = load i32, ptr %reg_0
	%reg_9 = add i32 4, 0
	%reg_10 = sdiv i32 %reg_8, %reg_9
	%reg_11 = add i32 %reg_7, %reg_10
	store i32 %reg_11, ptr %reg_6
	%reg_12 = load i32, ptr %reg_0
	%reg_13 = getelementptr [100 x i32], ptr @B, i32 0, i32 %reg_12
	%reg_14 = add i32 2, 0
	%reg_15 = load i32, ptr %reg_0
	%reg_16 = add i32 3, 0
	%reg_17 = sdiv i32 %reg_15, %reg_16
	%reg_18 = add i32 %reg_14, %reg_17
	store i32 %reg_18, ptr %reg_13
	%reg_19 = load i32, ptr %reg_0
	%reg_20 = getelementptr [100 x i32], ptr @C, i32 0, i32 %reg_19
	%reg_21 = add i32 3, 0
	%reg_22 = load i32, ptr %reg_0
	%reg_23 = add i32 2, 0
	%reg_24 = sdiv i32 %reg_22, %reg_23
	%reg_25 = add i32 %reg_21, %reg_24
	store i32 %reg_25, ptr %reg_20
	%reg_26 = load i32, ptr %reg_0
	%reg_27 = getelementptr [100 x i32], ptr @D, i32 0, i32 %reg_26
	%reg_28 = add i32 4, 0
	%reg_29 = load i32, ptr %reg_0
	%reg_30 = add i32 %reg_28, %reg_29
	store i32 %reg_30, ptr %reg_27
	%reg_31 = load i32, ptr %reg_0
	%reg_32 = add i32 1, 0
	%reg_33 = add i32 %reg_31, %reg_32
	store i32 %reg_33, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 36
	%reg_35 = add i32 0, 0
	store i32 %reg_35, ptr %reg_34
	%reg_36 = load i32, ptr %reg_34
	%reg_37 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_38 = call i32 @sum(ptr %reg_37)
	%reg_39 = add i32 %reg_36, %reg_38
	store i32 %reg_39, ptr %reg_34
	%reg_40 = getelementptr [100 x i32], ptr @A, i32 0
	%reg_41 = getelementptr [100 x i32], ptr @B, i32 0
	%reg_42 = getelementptr [100 x i32], ptr @C, i32 0
	%reg_43 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_44 = add i32 10, 0
	call void @f(ptr %reg_40, ptr %reg_41, ptr %reg_42, ptr %reg_43, i32 %reg_44)
	%reg_45 = load i32, ptr %reg_34
	%reg_46 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_47 = call i32 @sum(ptr %reg_46)
	%reg_48 = add i32 %reg_45, %reg_47
	store i32 %reg_48, ptr %reg_34
	%reg_49 = getelementptr [100 x i32], ptr @A, i32 0
	%reg_50 = getelementptr [100 x i32], ptr @A, i32 0
	%reg_51 = getelementptr [100 x i32], ptr @A, i32 0
	%reg_52 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_53 = add i32 10, 0
	call void @f(ptr %reg_49, ptr %reg_50, ptr %reg_51, ptr %reg_52, i32 %reg_53)
	%reg_54 = load i32, ptr %reg_34
	%reg_55 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_56 = call i32 @sum(ptr %reg_55)
	%reg_57 = add i32 %reg_54, %reg_56
	store i32 %reg_57, ptr %reg_34
	%reg_58 = load i32, ptr %reg_34
	%reg_59 = getelementptr [100 x i32], ptr @A, i32 0
	%reg_60 = call i32 @sum(ptr %reg_59)
	%reg_61 = add i32 %reg_58, %reg_60
	store i32 %reg_61, ptr %reg_34
	%reg_62 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_63 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_64 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_65 = getelementptr [100 x i32], ptr @A, i32 0
	%reg_66 = add i32 10, 0
	call void @f(ptr %reg_62, ptr %reg_63, ptr %reg_64, ptr %reg_65, i32 %reg_66)
	%reg_67 = load i32, ptr %reg_34
	%reg_68 = getelementptr [100 x i32], ptr @A, i32 0
	%reg_69 = call i32 @sum(ptr %reg_68)
	%reg_70 = add i32 %reg_67, %reg_69
	store i32 %reg_70, ptr %reg_34
	%reg_71 = load i32, ptr %reg_34
	%reg_72 = getelementptr [100 x i32], ptr @D, i32 0
	%reg_73 = call i32 @sum(ptr %reg_72)
	%reg_74 = add i32 %reg_71, %reg_73
	store i32 %reg_74, ptr %reg_34
	%reg_75 = load i32, ptr %reg_34
	call void @putint(i32 %reg_75)
	%reg_76 = add i32 10, 0
	call void @putch(i32 %reg_76)
	%reg_77 = add i32 100, 0
	%reg_78 = getelementptr [100 x i32], ptr @A, i32 0
	call void @putarray(i32 %reg_77, ptr %reg_78)
	%reg_79 = add i32 100, 0
	%reg_80 = getelementptr [100 x i32], ptr @B, i32 0
	call void @putarray(i32 %reg_79, ptr %reg_80)
	%reg_81 = add i32 100, 0
	%reg_82 = getelementptr [100 x i32], ptr @C, i32 0
	call void @putarray(i32 %reg_81, ptr %reg_82)
	%reg_83 = add i32 100, 0
	%reg_84 = getelementptr [100 x i32], ptr @D, i32 0
	call void @putarray(i32 %reg_83, ptr %reg_84)
	%reg_85 = add i32 0, 0
	ret i32 %reg_85
}
