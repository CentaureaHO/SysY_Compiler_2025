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

@buf = global [2 x [100 x i32]] zeroinitializer

define void @merge_sort(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 4
	%reg_23 = alloca i32
	%reg_21 = alloca i32
	%reg_19 = alloca i32
	%reg_9 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 4
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = add i32 1, 0
	%reg_6 = add i32 %reg_4, %reg_5
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = icmp sge i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block2:  ;If then at line 6
	ret void
	br label %Block3
Block3:  ;If end at line 6
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = add i32 %reg_10, %reg_11
	%reg_13 = add i32 2, 0
	%reg_14 = sdiv i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_9
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = load i32, ptr %reg_9
	call void @merge_sort(i32 %reg_15, i32 %reg_16)
	%reg_17 = load i32, ptr %reg_9
	%reg_18 = load i32, ptr %reg_3
	call void @merge_sort(i32 %reg_17, i32 %reg_18)
	%reg_20 = load i32, ptr %reg_2
	store i32 %reg_20, ptr %reg_19
	%reg_22 = load i32, ptr %reg_9
	store i32 %reg_22, ptr %reg_21
	%reg_24 = load i32, ptr %reg_2
	store i32 %reg_24, ptr %reg_23
	br label %Block4
Block4:  ;While condition at line 14
	%reg_25 = load i32, ptr %reg_19
	%reg_26 = load i32, ptr %reg_9
	%reg_27 = icmp slt i32 %reg_25, %reg_26
	br i1 %reg_27, label %Block7, label %Block6
Block5:  ;While body at line 14
	%reg_31 = add i32 0, 0
	%reg_32 = load i32, ptr %reg_19
	%reg_33 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_31, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = add i32 0, 0
	%reg_36 = load i32, ptr %reg_21
	%reg_37 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_35, i32 %reg_36
	%reg_38 = load i32, ptr %reg_37
	%reg_39 = icmp slt i32 %reg_34, %reg_38
	br i1 %reg_39, label %Block8, label %Block10
Block6:  ;While end at line 14
	br label %Block11
Block7:  ;And opertor at line 14
	%reg_28 = load i32, ptr %reg_21
	%reg_29 = load i32, ptr %reg_3
	%reg_30 = icmp slt i32 %reg_28, %reg_29
	br i1 %reg_30, label %Block5, label %Block6
Block8:  ;If then at line 15
	%reg_40 = add i32 1, 0
	%reg_41 = load i32, ptr %reg_23
	%reg_42 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_40, i32 %reg_41
	%reg_43 = add i32 0, 0
	%reg_44 = load i32, ptr %reg_19
	%reg_45 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_43, i32 %reg_44
	%reg_46 = load i32, ptr %reg_45
	store i32 %reg_46, ptr %reg_42
	%reg_47 = load i32, ptr %reg_19
	%reg_48 = add i32 1, 0
	%reg_49 = add i32 %reg_47, %reg_48
	store i32 %reg_49, ptr %reg_19
	br label %Block9
Block9:  ;If end at line 15
	%reg_60 = load i32, ptr %reg_23
	%reg_61 = add i32 1, 0
	%reg_62 = add i32 %reg_60, %reg_61
	store i32 %reg_62, ptr %reg_23
	br label %Block4
Block10:  ;If else at line 15
	%reg_50 = add i32 1, 0
	%reg_51 = load i32, ptr %reg_23
	%reg_52 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_50, i32 %reg_51
	%reg_53 = add i32 0, 0
	%reg_54 = load i32, ptr %reg_21
	%reg_55 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_53, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	store i32 %reg_56, ptr %reg_52
	%reg_57 = load i32, ptr %reg_21
	%reg_58 = add i32 1, 0
	%reg_59 = add i32 %reg_57, %reg_58
	store i32 %reg_59, ptr %reg_21
	br label %Block9
Block11:  ;While condition at line 24
	%reg_63 = load i32, ptr %reg_19
	%reg_64 = load i32, ptr %reg_9
	%reg_65 = icmp slt i32 %reg_63, %reg_64
	br i1 %reg_65, label %Block12, label %Block13
Block12:  ;While body at line 24
	%reg_66 = add i32 1, 0
	%reg_67 = load i32, ptr %reg_23
	%reg_68 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_66, i32 %reg_67
	%reg_69 = add i32 0, 0
	%reg_70 = load i32, ptr %reg_19
	%reg_71 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_69, i32 %reg_70
	%reg_72 = load i32, ptr %reg_71
	store i32 %reg_72, ptr %reg_68
	%reg_73 = load i32, ptr %reg_19
	%reg_74 = add i32 1, 0
	%reg_75 = add i32 %reg_73, %reg_74
	store i32 %reg_75, ptr %reg_19
	%reg_76 = load i32, ptr %reg_23
	%reg_77 = add i32 1, 0
	%reg_78 = add i32 %reg_76, %reg_77
	store i32 %reg_78, ptr %reg_23
	br label %Block11
Block13:  ;While end at line 24
	br label %Block14
Block14:  ;While condition at line 29
	%reg_79 = load i32, ptr %reg_21
	%reg_80 = load i32, ptr %reg_3
	%reg_81 = icmp slt i32 %reg_79, %reg_80
	br i1 %reg_81, label %Block15, label %Block16
Block15:  ;While body at line 29
	%reg_82 = add i32 1, 0
	%reg_83 = load i32, ptr %reg_23
	%reg_84 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_82, i32 %reg_83
	%reg_85 = add i32 0, 0
	%reg_86 = load i32, ptr %reg_21
	%reg_87 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_85, i32 %reg_86
	%reg_88 = load i32, ptr %reg_87
	store i32 %reg_88, ptr %reg_84
	%reg_89 = load i32, ptr %reg_21
	%reg_90 = add i32 1, 0
	%reg_91 = add i32 %reg_89, %reg_90
	store i32 %reg_91, ptr %reg_21
	%reg_92 = load i32, ptr %reg_23
	%reg_93 = add i32 1, 0
	%reg_94 = add i32 %reg_92, %reg_93
	store i32 %reg_94, ptr %reg_23
	br label %Block14
Block16:  ;While end at line 29
	br label %Block17
Block17:  ;While condition at line 35
	%reg_95 = load i32, ptr %reg_2
	%reg_96 = load i32, ptr %reg_3
	%reg_97 = icmp slt i32 %reg_95, %reg_96
	br i1 %reg_97, label %Block18, label %Block19
Block18:  ;While body at line 35
	%reg_98 = add i32 0, 0
	%reg_99 = load i32, ptr %reg_2
	%reg_100 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_98, i32 %reg_99
	%reg_101 = add i32 1, 0
	%reg_102 = load i32, ptr %reg_2
	%reg_103 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_101, i32 %reg_102
	%reg_104 = load i32, ptr %reg_103
	store i32 %reg_104, ptr %reg_100
	%reg_105 = load i32, ptr %reg_2
	%reg_106 = add i32 1, 0
	%reg_107 = add i32 %reg_105, %reg_106
	store i32 %reg_107, ptr %reg_2
	br label %Block17
Block19:  ;While end at line 35
	ret void
}

define i32 @main()
{
Block0:  ;Func define at line 41
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 41
	%reg_1 = add i32 0, 0
	%reg_2 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_1
	%reg_3 = call i32 @getarray(ptr %reg_2)
	store i32 %reg_3, ptr %reg_0
	%reg_4 = add i32 0, 0
	%reg_5 = load i32, ptr %reg_0
	call void @merge_sort(i32 %reg_4, i32 %reg_5)
	%reg_6 = load i32, ptr %reg_0
	%reg_7 = add i32 0, 0
	%reg_8 = getelementptr [2 x [100 x i32]], ptr @buf, i32 0, i32 %reg_7
	call void @putarray(i32 %reg_6, ptr %reg_8)
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
}
