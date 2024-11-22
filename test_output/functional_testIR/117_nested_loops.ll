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

@arr1 = global [10 x [2 x [3 x [4 x [5 x [6 x [2 x i32]]]]]]] zeroinitializer
@arr2 = global [10 x [2 x [3 x [2 x [4 x [8 x [7 x i32]]]]]]] zeroinitializer

define void @loop1(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 4
	%reg_16 = alloca i32
	%reg_14 = alloca i32
	%reg_12 = alloca i32
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 4
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_14
	%reg_17 = add i32 0, 0
	store i32 %reg_17, ptr %reg_16
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 7
	%reg_19 = load i32, ptr %reg_4
	%reg_20 = load i32, ptr %reg_2
	%reg_21 = icmp slt i32 %reg_19, %reg_20
	br i1 %reg_21, label %Block5, label %Block4
Block3:  ;While body at line 7
	%reg_25 = add i32 0, 0
	store i32 %reg_25, ptr %reg_6
	br label %Block6
Block4:  ;While end at line 7
	ret void
Block5:  ;And opertor at line 7
	%reg_22 = load i32, ptr %reg_4
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = icmp slt i32 %reg_22, %reg_23
	br i1 %reg_24, label %Block3, label %Block4
Block6:  ;While condition at line 9
	%reg_26 = load i32, ptr %reg_6
	%reg_27 = add i32 2, 0
	%reg_28 = icmp slt i32 %reg_26, %reg_27
	br i1 %reg_28, label %Block7, label %Block8
Block7:  ;While body at line 9
	%reg_29 = add i32 0, 0
	store i32 %reg_29, ptr %reg_8
	br label %Block9
Block8:  ;While end at line 9
	%reg_92 = load i32, ptr %reg_4
	%reg_93 = add i32 1, 0
	%reg_94 = add i32 %reg_92, %reg_93
	store i32 %reg_94, ptr %reg_4
	br label %Block2
Block9:  ;While condition at line 11
	%reg_30 = load i32, ptr %reg_8
	%reg_31 = add i32 3, 0
	%reg_32 = icmp slt i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block10, label %Block11
Block10:  ;While body at line 11
	%reg_33 = add i32 0, 0
	store i32 %reg_33, ptr %reg_10
	br label %Block12
Block11:  ;While end at line 11
	%reg_89 = load i32, ptr %reg_6
	%reg_90 = add i32 1, 0
	%reg_91 = add i32 %reg_89, %reg_90
	store i32 %reg_91, ptr %reg_6
	br label %Block6
Block12:  ;While condition at line 13
	%reg_34 = load i32, ptr %reg_10
	%reg_35 = add i32 4, 0
	%reg_36 = icmp slt i32 %reg_34, %reg_35
	br i1 %reg_36, label %Block13, label %Block14
Block13:  ;While body at line 13
	%reg_37 = add i32 0, 0
	store i32 %reg_37, ptr %reg_12
	br label %Block15
Block14:  ;While end at line 13
	%reg_86 = load i32, ptr %reg_8
	%reg_87 = add i32 1, 0
	%reg_88 = add i32 %reg_86, %reg_87
	store i32 %reg_88, ptr %reg_8
	br label %Block9
Block15:  ;While condition at line 15
	%reg_38 = load i32, ptr %reg_12
	%reg_39 = add i32 5, 0
	%reg_40 = icmp slt i32 %reg_38, %reg_39
	br i1 %reg_40, label %Block16, label %Block17
Block16:  ;While body at line 15
	%reg_41 = add i32 0, 0
	store i32 %reg_41, ptr %reg_14
	br label %Block18
Block17:  ;While end at line 15
	%reg_83 = load i32, ptr %reg_10
	%reg_84 = add i32 1, 0
	%reg_85 = add i32 %reg_83, %reg_84
	store i32 %reg_85, ptr %reg_10
	br label %Block12
Block18:  ;While condition at line 17
	%reg_42 = load i32, ptr %reg_14
	%reg_43 = add i32 6, 0
	%reg_44 = icmp slt i32 %reg_42, %reg_43
	br i1 %reg_44, label %Block19, label %Block20
Block19:  ;While body at line 17
	%reg_45 = add i32 0, 0
	store i32 %reg_45, ptr %reg_16
	br label %Block21
Block20:  ;While end at line 17
	%reg_80 = load i32, ptr %reg_12
	%reg_81 = add i32 1, 0
	%reg_82 = add i32 %reg_80, %reg_81
	store i32 %reg_82, ptr %reg_12
	br label %Block15
Block21:  ;While condition at line 19
	%reg_46 = load i32, ptr %reg_16
	%reg_47 = add i32 2, 0
	%reg_48 = icmp slt i32 %reg_46, %reg_47
	br i1 %reg_48, label %Block22, label %Block23
Block22:  ;While body at line 19
	%reg_49 = load i32, ptr %reg_4
	%reg_50 = load i32, ptr %reg_6
	%reg_51 = load i32, ptr %reg_8
	%reg_52 = load i32, ptr %reg_10
	%reg_53 = load i32, ptr %reg_12
	%reg_54 = load i32, ptr %reg_14
	%reg_55 = load i32, ptr %reg_16
	%reg_56 = getelementptr [10 x [2 x [3 x [4 x [5 x [6 x [2 x i32]]]]]]], ptr @arr1, i32 0, i32 %reg_49, i32 %reg_50, i32 %reg_51, i32 %reg_52, i32 %reg_53, i32 %reg_54, i32 %reg_55
	%reg_57 = load i32, ptr %reg_4
	%reg_58 = load i32, ptr %reg_6
	%reg_59 = add i32 %reg_57, %reg_58
	%reg_60 = load i32, ptr %reg_8
	%reg_61 = add i32 %reg_59, %reg_60
	%reg_62 = load i32, ptr %reg_10
	%reg_63 = add i32 %reg_61, %reg_62
	%reg_64 = load i32, ptr %reg_12
	%reg_65 = add i32 %reg_63, %reg_64
	%reg_66 = load i32, ptr %reg_14
	%reg_67 = add i32 %reg_65, %reg_66
	%reg_68 = load i32, ptr %reg_16
	%reg_69 = add i32 %reg_67, %reg_68
	%reg_70 = load i32, ptr %reg_2
	%reg_71 = add i32 %reg_69, %reg_70
	%reg_72 = load i32, ptr %reg_3
	%reg_73 = add i32 %reg_71, %reg_72
	store i32 %reg_73, ptr %reg_56
	%reg_74 = load i32, ptr %reg_16
	%reg_75 = add i32 1, 0
	%reg_76 = add i32 %reg_74, %reg_75
	store i32 %reg_76, ptr %reg_16
	br label %Block21
Block23:  ;While end at line 19
	%reg_77 = load i32, ptr %reg_14
	%reg_78 = add i32 1, 0
	%reg_79 = add i32 %reg_77, %reg_78
	store i32 %reg_79, ptr %reg_14
	br label %Block18
}

define void @loop2()
{
Block0:  ;Func define at line 37
	%reg_12 = alloca i32
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 37
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 40
	%reg_15 = load i32, ptr %reg_0
	%reg_16 = add i32 10, 0
	%reg_17 = icmp slt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block3, label %Block4
Block3:  ;While body at line 40
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_2
	br label %Block5
Block4:  ;While end at line 40
	ret void
Block5:  ;While condition at line 42
	%reg_19 = load i32, ptr %reg_2
	%reg_20 = add i32 2, 0
	%reg_21 = icmp slt i32 %reg_19, %reg_20
	br i1 %reg_21, label %Block6, label %Block7
Block6:  ;While body at line 42
	%reg_22 = add i32 0, 0
	store i32 %reg_22, ptr %reg_4
	br label %Block8
Block7:  ;While end at line 42
	%reg_75 = load i32, ptr %reg_0
	%reg_76 = add i32 1, 0
	%reg_77 = add i32 %reg_75, %reg_76
	store i32 %reg_77, ptr %reg_0
	br label %Block2
Block8:  ;While condition at line 44
	%reg_23 = load i32, ptr %reg_4
	%reg_24 = add i32 3, 0
	%reg_25 = icmp slt i32 %reg_23, %reg_24
	br i1 %reg_25, label %Block9, label %Block10
Block9:  ;While body at line 44
	%reg_26 = add i32 0, 0
	store i32 %reg_26, ptr %reg_6
	br label %Block11
Block10:  ;While end at line 44
	%reg_72 = load i32, ptr %reg_2
	%reg_73 = add i32 1, 0
	%reg_74 = add i32 %reg_72, %reg_73
	store i32 %reg_74, ptr %reg_2
	br label %Block5
Block11:  ;While condition at line 46
	%reg_27 = load i32, ptr %reg_6
	%reg_28 = add i32 2, 0
	%reg_29 = icmp slt i32 %reg_27, %reg_28
	br i1 %reg_29, label %Block12, label %Block13
Block12:  ;While body at line 46
	%reg_30 = add i32 0, 0
	store i32 %reg_30, ptr %reg_8
	br label %Block14
Block13:  ;While end at line 46
	%reg_69 = load i32, ptr %reg_4
	%reg_70 = add i32 1, 0
	%reg_71 = add i32 %reg_69, %reg_70
	store i32 %reg_71, ptr %reg_4
	br label %Block8
Block14:  ;While condition at line 48
	%reg_31 = load i32, ptr %reg_8
	%reg_32 = add i32 4, 0
	%reg_33 = icmp slt i32 %reg_31, %reg_32
	br i1 %reg_33, label %Block15, label %Block16
Block15:  ;While body at line 48
	%reg_34 = add i32 0, 0
	store i32 %reg_34, ptr %reg_10
	br label %Block17
Block16:  ;While end at line 48
	%reg_66 = load i32, ptr %reg_6
	%reg_67 = add i32 1, 0
	%reg_68 = add i32 %reg_66, %reg_67
	store i32 %reg_68, ptr %reg_6
	br label %Block11
Block17:  ;While condition at line 50
	%reg_35 = load i32, ptr %reg_10
	%reg_36 = add i32 8, 0
	%reg_37 = icmp slt i32 %reg_35, %reg_36
	br i1 %reg_37, label %Block18, label %Block19
Block18:  ;While body at line 50
	%reg_38 = add i32 0, 0
	store i32 %reg_38, ptr %reg_12
	br label %Block20
Block19:  ;While end at line 50
	%reg_63 = load i32, ptr %reg_8
	%reg_64 = add i32 1, 0
	%reg_65 = add i32 %reg_63, %reg_64
	store i32 %reg_65, ptr %reg_8
	br label %Block14
Block20:  ;While condition at line 52
	%reg_39 = load i32, ptr %reg_12
	%reg_40 = add i32 7, 0
	%reg_41 = icmp slt i32 %reg_39, %reg_40
	br i1 %reg_41, label %Block21, label %Block22
Block21:  ;While body at line 52
	%reg_42 = load i32, ptr %reg_0
	%reg_43 = load i32, ptr %reg_2
	%reg_44 = load i32, ptr %reg_4
	%reg_45 = load i32, ptr %reg_6
	%reg_46 = load i32, ptr %reg_8
	%reg_47 = load i32, ptr %reg_10
	%reg_48 = load i32, ptr %reg_12
	%reg_49 = getelementptr [10 x [2 x [3 x [2 x [4 x [8 x [7 x i32]]]]]]], ptr @arr2, i32 0, i32 %reg_42, i32 %reg_43, i32 %reg_44, i32 %reg_45, i32 %reg_46, i32 %reg_47, i32 %reg_48
	%reg_50 = load i32, ptr %reg_0
	%reg_51 = load i32, ptr %reg_2
	%reg_52 = add i32 %reg_50, %reg_51
	%reg_53 = load i32, ptr %reg_6
	%reg_54 = add i32 %reg_52, %reg_53
	%reg_55 = load i32, ptr %reg_12
	%reg_56 = add i32 %reg_54, %reg_55
	store i32 %reg_56, ptr %reg_49
	%reg_57 = load i32, ptr %reg_12
	%reg_58 = add i32 1, 0
	%reg_59 = add i32 %reg_57, %reg_58
	store i32 %reg_59, ptr %reg_12
	br label %Block20
Block22:  ;While end at line 52
	%reg_60 = load i32, ptr %reg_10
	%reg_61 = add i32 1, 0
	%reg_62 = add i32 %reg_60, %reg_61
	store i32 %reg_62, ptr %reg_10
	br label %Block17
}

define i32 @loop3(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6)
{
Block0:  ;Func define at line 70
	%reg_28 = alloca i32
	%reg_26 = alloca i32
	%reg_24 = alloca i32
	%reg_22 = alloca i32
	%reg_20 = alloca i32
	%reg_18 = alloca i32
	%reg_16 = alloca i32
	%reg_14 = alloca i32
	%reg_13 = alloca i32
	%reg_12 = alloca i32
	%reg_11 = alloca i32
	%reg_10 = alloca i32
	%reg_9 = alloca i32
	%reg_8 = alloca i32
	%reg_7 = alloca i32
	store i32 %reg_0, ptr %reg_7
	store i32 %reg_1, ptr %reg_8
	store i32 %reg_2, ptr %reg_9
	store i32 %reg_3, ptr %reg_10
	store i32 %reg_4, ptr %reg_11
	store i32 %reg_5, ptr %reg_12
	store i32 %reg_6, ptr %reg_13
	br label %Block1
Block1:  ;Func body at line 70
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_14
	%reg_17 = add i32 0, 0
	store i32 %reg_17, ptr %reg_16
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	%reg_21 = add i32 0, 0
	store i32 %reg_21, ptr %reg_20
	%reg_23 = add i32 0, 0
	store i32 %reg_23, ptr %reg_22
	%reg_25 = add i32 0, 0
	store i32 %reg_25, ptr %reg_24
	%reg_27 = add i32 0, 0
	store i32 %reg_27, ptr %reg_26
	%reg_29 = add i32 0, 0
	store i32 %reg_29, ptr %reg_28
	%reg_30 = add i32 0, 0
	store i32 %reg_30, ptr %reg_14
	br label %Block2
Block2:  ;While condition at line 74
	%reg_31 = load i32, ptr %reg_14
	%reg_32 = add i32 10, 0
	%reg_33 = icmp slt i32 %reg_31, %reg_32
	br i1 %reg_33, label %Block3, label %Block4
Block3:  ;While body at line 74
	%reg_34 = add i32 0, 0
	store i32 %reg_34, ptr %reg_16
	br label %Block5
Block4:  ;While end at line 74
	%reg_123 = load i32, ptr %reg_28
	ret i32 %reg_123
Block5:  ;While condition at line 76
	%reg_35 = load i32, ptr %reg_16
	%reg_36 = add i32 100, 0
	%reg_37 = icmp slt i32 %reg_35, %reg_36
	br i1 %reg_37, label %Block6, label %Block7
Block6:  ;While body at line 76
	%reg_38 = add i32 0, 0
	store i32 %reg_38, ptr %reg_18
	br label %Block8
Block7:  ;While end at line 76
	%reg_117 = load i32, ptr %reg_14
	%reg_118 = add i32 1, 0
	%reg_119 = add i32 %reg_117, %reg_118
	store i32 %reg_119, ptr %reg_14
	%reg_120 = load i32, ptr %reg_14
	%reg_121 = load i32, ptr %reg_7
	%reg_122 = icmp sge i32 %reg_120, %reg_121
	br i1 %reg_122, label %Block41, label %Block42
Block8:  ;While condition at line 78
	%reg_39 = load i32, ptr %reg_18
	%reg_40 = add i32 1000, 0
	%reg_41 = icmp slt i32 %reg_39, %reg_40
	br i1 %reg_41, label %Block9, label %Block10
Block9:  ;While body at line 78
	%reg_42 = add i32 0, 0
	store i32 %reg_42, ptr %reg_20
	br label %Block11
Block10:  ;While end at line 78
	%reg_111 = load i32, ptr %reg_16
	%reg_112 = add i32 1, 0
	%reg_113 = add i32 %reg_111, %reg_112
	store i32 %reg_113, ptr %reg_16
	%reg_114 = load i32, ptr %reg_16
	%reg_115 = load i32, ptr %reg_8
	%reg_116 = icmp sge i32 %reg_114, %reg_115
	br i1 %reg_116, label %Block38, label %Block39
Block11:  ;While condition at line 80
	%reg_43 = load i32, ptr %reg_20
	%reg_44 = add i32 10000, 0
	%reg_45 = icmp slt i32 %reg_43, %reg_44
	br i1 %reg_45, label %Block12, label %Block13
Block12:  ;While body at line 80
	%reg_46 = add i32 0, 0
	store i32 %reg_46, ptr %reg_22
	br label %Block14
Block13:  ;While end at line 80
	%reg_105 = load i32, ptr %reg_18
	%reg_106 = add i32 1, 0
	%reg_107 = add i32 %reg_105, %reg_106
	store i32 %reg_107, ptr %reg_18
	%reg_108 = load i32, ptr %reg_18
	%reg_109 = load i32, ptr %reg_9
	%reg_110 = icmp sge i32 %reg_108, %reg_109
	br i1 %reg_110, label %Block35, label %Block36
Block14:  ;While condition at line 82
	%reg_47 = load i32, ptr %reg_22
	%reg_48 = add i32 100000, 0
	%reg_49 = icmp slt i32 %reg_47, %reg_48
	br i1 %reg_49, label %Block15, label %Block16
Block15:  ;While body at line 82
	%reg_50 = add i32 0, 0
	store i32 %reg_50, ptr %reg_24
	br label %Block17
Block16:  ;While end at line 82
	%reg_99 = load i32, ptr %reg_20
	%reg_100 = add i32 1, 0
	%reg_101 = add i32 %reg_99, %reg_100
	store i32 %reg_101, ptr %reg_20
	%reg_102 = load i32, ptr %reg_20
	%reg_103 = load i32, ptr %reg_10
	%reg_104 = icmp sge i32 %reg_102, %reg_103
	br i1 %reg_104, label %Block32, label %Block33
Block17:  ;While condition at line 84
	%reg_51 = load i32, ptr %reg_24
	%reg_52 = add i32 1000000, 0
	%reg_53 = icmp slt i32 %reg_51, %reg_52
	br i1 %reg_53, label %Block18, label %Block19
Block18:  ;While body at line 84
	%reg_54 = add i32 0, 0
	store i32 %reg_54, ptr %reg_26
	br label %Block20
Block19:  ;While end at line 84
	%reg_93 = load i32, ptr %reg_22
	%reg_94 = add i32 1, 0
	%reg_95 = add i32 %reg_93, %reg_94
	store i32 %reg_95, ptr %reg_22
	%reg_96 = load i32, ptr %reg_22
	%reg_97 = load i32, ptr %reg_11
	%reg_98 = icmp sge i32 %reg_96, %reg_97
	br i1 %reg_98, label %Block29, label %Block30
Block20:  ;While condition at line 86
	%reg_55 = load i32, ptr %reg_26
	%reg_56 = add i32 10000000, 0
	%reg_57 = icmp slt i32 %reg_55, %reg_56
	br i1 %reg_57, label %Block21, label %Block22
Block21:  ;While body at line 86
	%reg_58 = load i32, ptr %reg_28
	%reg_59 = add i32 817, 0
	%reg_60 = srem i32 %reg_58, %reg_59
	%reg_61 = load i32, ptr %reg_14
	%reg_62 = load i32, ptr %reg_16
	%reg_63 = load i32, ptr %reg_18
	%reg_64 = load i32, ptr %reg_20
	%reg_65 = load i32, ptr %reg_22
	%reg_66 = load i32, ptr %reg_24
	%reg_67 = load i32, ptr %reg_26
	%reg_68 = getelementptr [10 x [2 x [3 x [4 x [5 x [6 x [2 x i32]]]]]]], ptr @arr1, i32 0, i32 %reg_61, i32 %reg_62, i32 %reg_63, i32 %reg_64, i32 %reg_65, i32 %reg_66, i32 %reg_67
	%reg_69 = load i32, ptr %reg_68
	%reg_70 = add i32 %reg_60, %reg_69
	%reg_71 = load i32, ptr %reg_14
	%reg_72 = load i32, ptr %reg_16
	%reg_73 = load i32, ptr %reg_18
	%reg_74 = load i32, ptr %reg_20
	%reg_75 = load i32, ptr %reg_22
	%reg_76 = load i32, ptr %reg_24
	%reg_77 = load i32, ptr %reg_26
	%reg_78 = getelementptr [10 x [2 x [3 x [2 x [4 x [8 x [7 x i32]]]]]]], ptr @arr2, i32 0, i32 %reg_71, i32 %reg_72, i32 %reg_73, i32 %reg_74, i32 %reg_75, i32 %reg_76, i32 %reg_77
	%reg_79 = load i32, ptr %reg_78
	%reg_80 = add i32 %reg_70, %reg_79
	store i32 %reg_80, ptr %reg_28
	%reg_81 = load i32, ptr %reg_26
	%reg_82 = add i32 1, 0
	%reg_83 = add i32 %reg_81, %reg_82
	store i32 %reg_83, ptr %reg_26
	%reg_84 = load i32, ptr %reg_26
	%reg_85 = load i32, ptr %reg_13
	%reg_86 = icmp sge i32 %reg_84, %reg_85
	br i1 %reg_86, label %Block23, label %Block24
Block22:  ;While end at line 86
	%reg_87 = load i32, ptr %reg_24
	%reg_88 = add i32 1, 0
	%reg_89 = add i32 %reg_87, %reg_88
	store i32 %reg_89, ptr %reg_24
	%reg_90 = load i32, ptr %reg_24
	%reg_91 = load i32, ptr %reg_12
	%reg_92 = icmp sge i32 %reg_90, %reg_91
	br i1 %reg_92, label %Block26, label %Block27
Block23:  ;If then at line 89
	br label %Block22
Block24:  ;If end at line 89
	br label %Block20
Block25:  ;Break at line 89
	br label %Block24
Block26:  ;If then at line 92
	br label %Block19
Block27:  ;If end at line 92
	br label %Block17
Block28:  ;Break at line 92
	br label %Block27
Block29:  ;If then at line 95
	br label %Block16
Block30:  ;If end at line 95
	br label %Block14
Block31:  ;Break at line 95
	br label %Block30
Block32:  ;If then at line 98
	br label %Block13
Block33:  ;If end at line 98
	br label %Block11
Block34:  ;Break at line 98
	br label %Block33
Block35:  ;If then at line 101
	br label %Block10
Block36:  ;If end at line 101
	br label %Block8
Block37:  ;Break at line 101
	br label %Block36
Block38:  ;If then at line 104
	br label %Block7
Block39:  ;If end at line 104
	br label %Block5
Block40:  ;Break at line 104
	br label %Block39
Block41:  ;If then at line 107
	br label %Block4
Block42:  ;If end at line 107
	br label %Block2
Block43:  ;Break at line 107
	br label %Block42
}

define i32 @main()
{
Block0:  ;Func define at line 112
	%reg_16 = alloca i32
	%reg_14 = alloca i32
	%reg_12 = alloca i32
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 112
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	%reg_3 = call i32 @getint()
	store i32 %reg_3, ptr %reg_2
	%reg_5 = call i32 @getint()
	store i32 %reg_5, ptr %reg_4
	%reg_7 = call i32 @getint()
	store i32 %reg_7, ptr %reg_6
	%reg_9 = call i32 @getint()
	store i32 %reg_9, ptr %reg_8
	%reg_11 = call i32 @getint()
	store i32 %reg_11, ptr %reg_10
	%reg_13 = call i32 @getint()
	store i32 %reg_13, ptr %reg_12
	%reg_15 = call i32 @getint()
	store i32 %reg_15, ptr %reg_14
	%reg_17 = call i32 @getint()
	store i32 %reg_17, ptr %reg_16
	%reg_18 = load i32, ptr %reg_0
	%reg_19 = load i32, ptr %reg_2
	call void @loop1(i32 %reg_18, i32 %reg_19)
	call void @loop2()
	%reg_20 = load i32, ptr %reg_4
	%reg_21 = load i32, ptr %reg_6
	%reg_22 = load i32, ptr %reg_8
	%reg_23 = load i32, ptr %reg_10
	%reg_24 = load i32, ptr %reg_12
	%reg_25 = load i32, ptr %reg_14
	%reg_26 = load i32, ptr %reg_16
	%reg_27 = call i32 @loop3(i32 %reg_20, i32 %reg_21, i32 %reg_22, i32 %reg_23, i32 %reg_24, i32 %reg_25, i32 %reg_26)
	ret i32 %reg_27
}
