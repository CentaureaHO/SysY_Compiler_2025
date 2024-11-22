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

@INF = global i32 65535
@e = global [16 x [16 x i32]] zeroinitializer
@book = global [16 x i32] zeroinitializer
@dis = global [16 x i32] zeroinitializer
@n = global i32 zeroinitializer
@m = global i32 zeroinitializer
@v1 = global i32 zeroinitializer
@v2 = global i32 zeroinitializer
@w = global i32 zeroinitializer

define void @Dijkstra()
{
Block0:  ;Func define at line 8
	%reg_58 = alloca i32
	%reg_33 = alloca i32
	%reg_31 = alloca i32
	%reg_29 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 8
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = add i32 1, 0
	store i32 %reg_4, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 13
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = load i32, ptr @n
	%reg_7 = icmp sle i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 13
	%reg_8 = load i32, ptr %reg_0
	%reg_9 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %reg_8
	%reg_10 = add i32 1, 0
	%reg_11 = load i32, ptr %reg_0
	%reg_12 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	store i32 %reg_13, ptr %reg_9
	%reg_14 = load i32, ptr %reg_0
	%reg_15 = getelementptr [16 x i32], ptr @book, i32 0, i32 %reg_14
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	%reg_17 = load i32, ptr %reg_0
	%reg_18 = add i32 1, 0
	%reg_19 = add i32 %reg_17, %reg_18
	store i32 %reg_19, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 13
	%reg_20 = add i32 1, 0
	%reg_21 = getelementptr [16 x i32], ptr @book, i32 0, i32 %reg_20
	%reg_22 = add i32 1, 0
	store i32 %reg_22, ptr %reg_21
	%reg_23 = add i32 1, 0
	store i32 %reg_23, ptr %reg_0
	br label %Block5
Block5:  ;While condition at line 21
	%reg_24 = load i32, ptr %reg_0
	%reg_25 = load i32, ptr @n
	%reg_26 = add i32 1, 0
	%reg_27 = sub i32 %reg_25, %reg_26
	%reg_28 = icmp sle i32 %reg_24, %reg_27
	br i1 %reg_28, label %Block6, label %Block7
Block6:  ;While body at line 21
	%reg_30 = load i32, ptr @INF
	store i32 %reg_30, ptr %reg_29
	%reg_32 = add i32 0, 0
	store i32 %reg_32, ptr %reg_31
	%reg_34 = add i32 1, 0
	store i32 %reg_34, ptr %reg_33
	br label %Block8
Block7:  ;While end at line 21
	ret void
Block8:  ;While condition at line 25
	%reg_35 = load i32, ptr %reg_33
	%reg_36 = load i32, ptr @n
	%reg_37 = icmp sle i32 %reg_35, %reg_36
	br i1 %reg_37, label %Block9, label %Block10
Block9:  ;While body at line 25
	%reg_38 = load i32, ptr %reg_29
	%reg_39 = load i32, ptr %reg_33
	%reg_40 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %reg_39
	%reg_41 = load i32, ptr %reg_40
	%reg_42 = icmp sgt i32 %reg_38, %reg_41
	br i1 %reg_42, label %Block13, label %Block12
Block10:  ;While end at line 25
	%reg_55 = load i32, ptr %reg_31
	%reg_56 = getelementptr [16 x i32], ptr @book, i32 0, i32 %reg_55
	%reg_57 = add i32 1, 0
	store i32 %reg_57, ptr %reg_56
	%reg_59 = add i32 1, 0
	store i32 %reg_59, ptr %reg_58
	br label %Block14
Block11:  ;If then at line 26
	%reg_48 = load i32, ptr %reg_33
	%reg_49 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	store i32 %reg_50, ptr %reg_29
	%reg_51 = load i32, ptr %reg_33
	store i32 %reg_51, ptr %reg_31
	br label %Block12
Block12:  ;If end at line 26
	%reg_52 = load i32, ptr %reg_33
	%reg_53 = add i32 1, 0
	%reg_54 = add i32 %reg_52, %reg_53
	store i32 %reg_54, ptr %reg_33
	br label %Block8
Block13:  ;And opertor at line 26
	%reg_43 = load i32, ptr %reg_33
	%reg_44 = getelementptr [16 x i32], ptr @book, i32 0, i32 %reg_43
	%reg_45 = load i32, ptr %reg_44
	%reg_46 = add i32 0, 0
	%reg_47 = icmp eq i32 %reg_45, %reg_46
	br i1 %reg_47, label %Block11, label %Block12
Block14:  ;While condition at line 34
	%reg_60 = load i32, ptr %reg_58
	%reg_61 = load i32, ptr @n
	%reg_62 = icmp sle i32 %reg_60, %reg_61
	br i1 %reg_62, label %Block15, label %Block16
Block15:  ;While body at line 34
	%reg_63 = load i32, ptr %reg_31
	%reg_64 = load i32, ptr %reg_58
	%reg_65 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %reg_63, i32 %reg_64
	%reg_66 = load i32, ptr %reg_65
	%reg_67 = load i32, ptr @INF
	%reg_68 = icmp slt i32 %reg_66, %reg_67
	br i1 %reg_68, label %Block17, label %Block18
Block16:  ;While end at line 34
	%reg_94 = load i32, ptr %reg_0
	%reg_95 = add i32 1, 0
	%reg_96 = add i32 %reg_94, %reg_95
	store i32 %reg_96, ptr %reg_0
	br label %Block5
Block17:  ;If then at line 35
	%reg_69 = load i32, ptr %reg_58
	%reg_70 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %reg_69
	%reg_71 = load i32, ptr %reg_70
	%reg_72 = load i32, ptr %reg_31
	%reg_73 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	%reg_75 = load i32, ptr %reg_31
	%reg_76 = load i32, ptr %reg_58
	%reg_77 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %reg_75, i32 %reg_76
	%reg_78 = load i32, ptr %reg_77
	%reg_79 = add i32 %reg_74, %reg_78
	%reg_80 = icmp sgt i32 %reg_71, %reg_79
	br i1 %reg_80, label %Block19, label %Block20
Block18:  ;If end at line 35
	%reg_91 = load i32, ptr %reg_58
	%reg_92 = add i32 1, 0
	%reg_93 = add i32 %reg_91, %reg_92
	store i32 %reg_93, ptr %reg_58
	br label %Block14
Block19:  ;If then at line 36
	%reg_81 = load i32, ptr %reg_58
	%reg_82 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %reg_81
	%reg_83 = load i32, ptr %reg_31
	%reg_84 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %reg_83
	%reg_85 = load i32, ptr %reg_84
	%reg_86 = load i32, ptr %reg_31
	%reg_87 = load i32, ptr %reg_58
	%reg_88 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %reg_86, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	%reg_90 = add i32 %reg_85, %reg_89
	store i32 %reg_90, ptr %reg_82
	br label %Block20
Block20:  ;If end at line 36
	br label %Block18
}

define i32 @main()
{
Block0:  ;Func define at line 46
	%reg_36 = alloca i32
	%reg_34 = alloca i32
	%reg_8 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 46
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = call i32 @getint()
	store i32 %reg_2, ptr @n
	%reg_3 = call i32 @getint()
	store i32 %reg_3, ptr @m
	%reg_4 = add i32 1, 0
	store i32 %reg_4, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 53
	%reg_5 = load i32, ptr %reg_0
	%reg_6 = load i32, ptr @n
	%reg_7 = icmp sle i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 53
	%reg_9 = add i32 1, 0
	store i32 %reg_9, ptr %reg_8
	br label %Block5
Block4:  ;While end at line 53
	%reg_30 = add i32 1, 0
	store i32 %reg_30, ptr %reg_0
	br label %Block11
Block5:  ;While condition at line 55
	%reg_10 = load i32, ptr %reg_8
	%reg_11 = load i32, ptr @n
	%reg_12 = icmp sle i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block6, label %Block7
Block6:  ;While body at line 55
	%reg_13 = load i32, ptr %reg_0
	%reg_14 = load i32, ptr %reg_8
	%reg_15 = icmp eq i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block8, label %Block10
Block7:  ;While end at line 55
	%reg_27 = load i32, ptr %reg_0
	%reg_28 = add i32 1, 0
	%reg_29 = add i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_0
	br label %Block2
Block8:  ;If then at line 56
	%reg_16 = load i32, ptr %reg_0
	%reg_17 = load i32, ptr %reg_8
	%reg_18 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %reg_16, i32 %reg_17
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	br label %Block9
Block9:  ;If end at line 56
	%reg_24 = load i32, ptr %reg_8
	%reg_25 = add i32 1, 0
	%reg_26 = add i32 %reg_24, %reg_25
	store i32 %reg_26, ptr %reg_8
	br label %Block5
Block10:  ;If else at line 56
	%reg_20 = load i32, ptr %reg_0
	%reg_21 = load i32, ptr %reg_8
	%reg_22 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %reg_20, i32 %reg_21
	%reg_23 = load i32, ptr @INF
	store i32 %reg_23, ptr %reg_22
	br label %Block9
Block11:  ;While condition at line 66
	%reg_31 = load i32, ptr %reg_0
	%reg_32 = load i32, ptr @m
	%reg_33 = icmp sle i32 %reg_31, %reg_32
	br i1 %reg_33, label %Block12, label %Block13
Block12:  ;While body at line 66
	%reg_35 = call i32 @getint()
	store i32 %reg_35, ptr %reg_34
	%reg_37 = call i32 @getint()
	store i32 %reg_37, ptr %reg_36
	%reg_38 = load i32, ptr %reg_34
	%reg_39 = load i32, ptr %reg_36
	%reg_40 = getelementptr [16 x [16 x i32]], ptr @e, i32 0, i32 %reg_38, i32 %reg_39
	%reg_41 = call i32 @getint()
	store i32 %reg_41, ptr %reg_40
	%reg_42 = load i32, ptr %reg_0
	%reg_43 = add i32 1, 0
	%reg_44 = add i32 %reg_42, %reg_43
	store i32 %reg_44, ptr %reg_0
	br label %Block11
Block13:  ;While end at line 66
	call void @Dijkstra()
	%reg_45 = add i32 1, 0
	store i32 %reg_45, ptr %reg_0
	br label %Block14
Block14:  ;While condition at line 75
	%reg_46 = load i32, ptr %reg_0
	%reg_47 = load i32, ptr @n
	%reg_48 = icmp sle i32 %reg_46, %reg_47
	br i1 %reg_48, label %Block15, label %Block16
Block15:  ;While body at line 75
	%reg_49 = load i32, ptr %reg_0
	%reg_50 = getelementptr [16 x i32], ptr @dis, i32 0, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	call void @putint(i32 %reg_51)
	%reg_52 = add i32 32, 0
	call void @putch(i32 %reg_52)
	%reg_53 = load i32, ptr %reg_0
	%reg_54 = add i32 1, 0
	%reg_55 = add i32 %reg_53, %reg_54
	store i32 %reg_55, ptr %reg_0
	br label %Block14
Block16:  ;While end at line 75
	%reg_56 = add i32 10, 0
	call void @putch(i32 %reg_56)
	%reg_57 = add i32 0, 0
	ret i32 %reg_57
}
