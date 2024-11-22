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

@V = global [200 x [200 x i32]] zeroinitializer

define i32 @KnapSack(i32 %reg_0, ptr %reg_1, ptr %reg_2, ptr %reg_3, i32 %reg_4)
{
Block0:  ;Func define at line 2
	%reg_42 = alloca i32
	%reg_35 = alloca i32
	%reg_9 = alloca i32
	%reg_7 = alloca i32
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	store i32 %reg_0, ptr %reg_5
	store i32 %reg_4, ptr %reg_6
	br label %Block1
Block1:  ;Func body at line 2
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 1, 0
	store i32 %reg_11, ptr %reg_7
	br label %Block2
Block2:  ;While condition at line 6
	%reg_12 = load i32, ptr %reg_7
	%reg_13 = load i32, ptr %reg_5
	%reg_14 = icmp sle i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 6
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_9
	br label %Block5
Block4:  ;While end at line 6
	%reg_74 = load i32, ptr %reg_6
	store i32 %reg_74, ptr %reg_9
	%reg_75 = load i32, ptr %reg_5
	store i32 %reg_75, ptr %reg_7
	br label %Block14
Block5:  ;While condition at line 9
	%reg_16 = load i32, ptr %reg_9
	%reg_17 = load i32, ptr %reg_6
	%reg_18 = add i32 1, 0
	%reg_19 = add i32 %reg_17, %reg_18
	%reg_20 = icmp slt i32 %reg_16, %reg_19
	br i1 %reg_20, label %Block6, label %Block7
Block6:  ;While body at line 9
	%reg_21 = load i32, ptr %reg_9
	%reg_22 = load i32, ptr %reg_7
	%reg_23 = getelementptr i32, ptr %reg_1, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = icmp slt i32 %reg_21, %reg_24
	br i1 %reg_25, label %Block8, label %Block10
Block7:  ;While end at line 9
	%reg_71 = load i32, ptr %reg_7
	%reg_72 = add i32 1, 0
	%reg_73 = add i32 %reg_71, %reg_72
	store i32 %reg_73, ptr %reg_7
	br label %Block2
Block8:  ;If then at line 11
	%reg_26 = load i32, ptr %reg_7
	%reg_27 = load i32, ptr %reg_9
	%reg_28 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_26, i32 %reg_27
	%reg_29 = load i32, ptr %reg_7
	%reg_30 = add i32 1, 0
	%reg_31 = sub i32 %reg_29, %reg_30
	%reg_32 = load i32, ptr %reg_9
	%reg_33 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_31, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	store i32 %reg_34, ptr %reg_28
	br label %Block9
Block9:  ;If end at line 11
	%reg_68 = load i32, ptr %reg_9
	%reg_69 = add i32 1, 0
	%reg_70 = add i32 %reg_68, %reg_69
	store i32 %reg_70, ptr %reg_9
	br label %Block5
Block10:  ;If else at line 11
	%reg_36 = load i32, ptr %reg_7
	%reg_37 = add i32 1, 0
	%reg_38 = sub i32 %reg_36, %reg_37
	%reg_39 = load i32, ptr %reg_9
	%reg_40 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_38, i32 %reg_39
	%reg_41 = load i32, ptr %reg_40
	store i32 %reg_41, ptr %reg_35
	%reg_43 = load i32, ptr %reg_7
	%reg_44 = add i32 1, 0
	%reg_45 = sub i32 %reg_43, %reg_44
	%reg_46 = load i32, ptr %reg_9
	%reg_47 = load i32, ptr %reg_7
	%reg_48 = getelementptr i32, ptr %reg_1, i32 %reg_47
	%reg_49 = load i32, ptr %reg_48
	%reg_50 = sub i32 %reg_46, %reg_49
	%reg_51 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_45, i32 %reg_50
	%reg_52 = load i32, ptr %reg_51
	%reg_53 = load i32, ptr %reg_7
	%reg_54 = getelementptr i32, ptr %reg_2, i32 %reg_53
	%reg_55 = load i32, ptr %reg_54
	%reg_56 = add i32 %reg_52, %reg_55
	store i32 %reg_56, ptr %reg_42
	%reg_57 = load i32, ptr %reg_35
	%reg_58 = load i32, ptr %reg_42
	%reg_59 = icmp sgt i32 %reg_57, %reg_58
	br i1 %reg_59, label %Block11, label %Block13
Block11:  ;If then at line 17
	%reg_60 = load i32, ptr %reg_7
	%reg_61 = load i32, ptr %reg_9
	%reg_62 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_60, i32 %reg_61
	%reg_63 = load i32, ptr %reg_35
	store i32 %reg_63, ptr %reg_62
	br label %Block12
Block12:  ;If end at line 17
	br label %Block9
Block13:  ;If else at line 17
	%reg_64 = load i32, ptr %reg_7
	%reg_65 = load i32, ptr %reg_9
	%reg_66 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_64, i32 %reg_65
	%reg_67 = load i32, ptr %reg_42
	store i32 %reg_67, ptr %reg_66
	br label %Block12
Block14:  ;While condition at line 34
	%reg_76 = load i32, ptr %reg_7
	%reg_77 = add i32 1, 0
	%reg_78 = icmp sge i32 %reg_76, %reg_77
	br i1 %reg_78, label %Block15, label %Block16
Block15:  ;While body at line 34
	%reg_79 = load i32, ptr %reg_7
	%reg_80 = load i32, ptr %reg_9
	%reg_81 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_79, i32 %reg_80
	%reg_82 = load i32, ptr %reg_81
	%reg_83 = load i32, ptr %reg_7
	%reg_84 = add i32 1, 0
	%reg_85 = sub i32 %reg_83, %reg_84
	%reg_86 = load i32, ptr %reg_9
	%reg_87 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_85, i32 %reg_86
	%reg_88 = load i32, ptr %reg_87
	%reg_89 = icmp sgt i32 %reg_82, %reg_88
	br i1 %reg_89, label %Block17, label %Block19
Block16:  ;While end at line 34
	%reg_104 = load i32, ptr %reg_5
	%reg_105 = load i32, ptr %reg_6
	%reg_106 = getelementptr [200 x [200 x i32]], ptr @V, i32 0, i32 %reg_104, i32 %reg_105
	%reg_107 = load i32, ptr %reg_106
	ret i32 %reg_107
Block17:  ;If then at line 36
	%reg_90 = load i32, ptr %reg_7
	%reg_91 = getelementptr i32, ptr %reg_3, i32 %reg_90
	%reg_92 = add i32 1, 0
	store i32 %reg_92, ptr %reg_91
	%reg_93 = load i32, ptr %reg_9
	%reg_94 = load i32, ptr %reg_7
	%reg_95 = getelementptr i32, ptr %reg_1, i32 %reg_94
	%reg_96 = load i32, ptr %reg_95
	%reg_97 = sub i32 %reg_93, %reg_96
	store i32 %reg_97, ptr %reg_9
	br label %Block18
Block18:  ;If end at line 36
	%reg_101 = load i32, ptr %reg_7
	%reg_102 = add i32 1, 0
	%reg_103 = sub i32 %reg_101, %reg_102
	store i32 %reg_103, ptr %reg_7
	br label %Block14
Block19:  ;If else at line 36
	%reg_98 = load i32, ptr %reg_7
	%reg_99 = getelementptr i32, ptr %reg_3, i32 %reg_98
	%reg_100 = add i32 0, 0
	store i32 %reg_100, ptr %reg_99
	br label %Block18
}

define i32 @main()
{
Block0:  ;Func define at line 51
	%reg_31 = alloca i32
	%reg_29 = alloca i32
	%reg_28 = alloca [6 x i32]
	%reg_15 = alloca [6 x i32]
	%reg_2 = alloca [6 x i32]
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 51
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 24, i1 1)
	%reg_3 = add i32 0, 0
	%reg_4 = getelementptr [6 x i32], ptr %reg_2, i32 0, i32 0
	store i32 %reg_3, ptr %reg_4
	%reg_5 = add i32 2, 0
	%reg_6 = getelementptr [6 x i32], ptr %reg_2, i32 0, i32 1
	store i32 %reg_5, ptr %reg_6
	%reg_7 = add i32 2, 0
	%reg_8 = getelementptr [6 x i32], ptr %reg_2, i32 0, i32 2
	store i32 %reg_7, ptr %reg_8
	%reg_9 = add i32 6, 0
	%reg_10 = getelementptr [6 x i32], ptr %reg_2, i32 0, i32 3
	store i32 %reg_9, ptr %reg_10
	%reg_11 = add i32 5, 0
	%reg_12 = getelementptr [6 x i32], ptr %reg_2, i32 0, i32 4
	store i32 %reg_11, ptr %reg_12
	%reg_13 = add i32 4, 0
	%reg_14 = getelementptr [6 x i32], ptr %reg_2, i32 0, i32 5
	store i32 %reg_13, ptr %reg_14
	call void @llvm.memset.p0.i32(ptr %reg_15, i8 0, i32 24, i1 1)
	%reg_16 = add i32 0, 0
	%reg_17 = getelementptr [6 x i32], ptr %reg_15, i32 0, i32 0
	store i32 %reg_16, ptr %reg_17
	%reg_18 = add i32 6, 0
	%reg_19 = getelementptr [6 x i32], ptr %reg_15, i32 0, i32 1
	store i32 %reg_18, ptr %reg_19
	%reg_20 = add i32 3, 0
	%reg_21 = getelementptr [6 x i32], ptr %reg_15, i32 0, i32 2
	store i32 %reg_20, ptr %reg_21
	%reg_22 = add i32 5, 0
	%reg_23 = getelementptr [6 x i32], ptr %reg_15, i32 0, i32 3
	store i32 %reg_22, ptr %reg_23
	%reg_24 = add i32 4, 0
	%reg_25 = getelementptr [6 x i32], ptr %reg_15, i32 0, i32 4
	store i32 %reg_24, ptr %reg_25
	%reg_26 = add i32 6, 0
	%reg_27 = getelementptr [6 x i32], ptr %reg_15, i32 0, i32 5
	store i32 %reg_26, ptr %reg_27
	call void @llvm.memset.p0.i32(ptr %reg_28, i8 0, i32 24, i1 1)
	%reg_30 = add i32 5, 0
	store i32 %reg_30, ptr %reg_29
	%reg_32 = add i32 10, 0
	store i32 %reg_32, ptr %reg_31
	%reg_33 = load i32, ptr %reg_29
	%reg_34 = getelementptr [6 x i32], ptr %reg_2, i32 0
	%reg_35 = getelementptr [6 x i32], ptr %reg_15, i32 0
	%reg_36 = getelementptr [6 x i32], ptr %reg_28, i32 0
	%reg_37 = load i32, ptr %reg_31
	%reg_38 = call i32 @KnapSack(i32 %reg_33, ptr %reg_34, ptr %reg_35, ptr %reg_36, i32 %reg_37)
	store i32 %reg_38, ptr %reg_0
	%reg_39 = load i32, ptr %reg_0
	call void @putint(i32 %reg_39)
	%reg_40 = add i32 0, 0
	ret i32 %reg_40
}
