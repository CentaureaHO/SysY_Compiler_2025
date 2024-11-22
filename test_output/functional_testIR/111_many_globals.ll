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

@a0 = global i32 zeroinitializer
@a1 = global i32 zeroinitializer
@a2 = global i32 zeroinitializer
@a3 = global i32 zeroinitializer
@a4 = global i32 zeroinitializer
@a5 = global i32 zeroinitializer
@a6 = global i32 zeroinitializer
@a7 = global i32 zeroinitializer
@a8 = global i32 zeroinitializer
@a9 = global i32 zeroinitializer
@a10 = global i32 zeroinitializer
@a11 = global i32 zeroinitializer
@a12 = global i32 zeroinitializer
@a13 = global i32 zeroinitializer
@a14 = global i32 zeroinitializer
@a15 = global i32 zeroinitializer
@a16 = global i32 zeroinitializer
@a17 = global i32 zeroinitializer
@a18 = global i32 zeroinitializer
@a19 = global i32 zeroinitializer
@a20 = global i32 zeroinitializer
@a21 = global i32 zeroinitializer
@a22 = global i32 zeroinitializer
@a23 = global i32 zeroinitializer
@a24 = global i32 zeroinitializer
@a25 = global i32 zeroinitializer
@a26 = global i32 zeroinitializer
@a27 = global i32 zeroinitializer
@a28 = global i32 zeroinitializer
@a29 = global i32 zeroinitializer
@a30 = global i32 zeroinitializer
@a31 = global i32 zeroinitializer
@a32 = global i32 zeroinitializer
@a33 = global i32 zeroinitializer
@a34 = global i32 zeroinitializer
@a35 = global i32 zeroinitializer
@a36 = global i32 zeroinitializer
@a37 = global i32 zeroinitializer
@a38 = global i32 zeroinitializer
@a39 = global i32 zeroinitializer

define i32 @testParam8(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, i32 %reg_7)
{
Block0:  ;Func define at line 45
	%reg_15 = alloca i32
	%reg_14 = alloca i32
	%reg_13 = alloca i32
	%reg_12 = alloca i32
	%reg_11 = alloca i32
	%reg_10 = alloca i32
	%reg_9 = alloca i32
	%reg_8 = alloca i32
	store i32 %reg_0, ptr %reg_8
	store i32 %reg_1, ptr %reg_9
	store i32 %reg_2, ptr %reg_10
	store i32 %reg_3, ptr %reg_11
	store i32 %reg_4, ptr %reg_12
	store i32 %reg_5, ptr %reg_13
	store i32 %reg_6, ptr %reg_14
	store i32 %reg_7, ptr %reg_15
	br label %Block1
Block1:  ;Func body at line 45
	%reg_16 = load i32, ptr %reg_8
	%reg_17 = load i32, ptr %reg_9
	%reg_18 = add i32 %reg_16, %reg_17
	%reg_19 = load i32, ptr %reg_10
	%reg_20 = add i32 %reg_18, %reg_19
	%reg_21 = load i32, ptr %reg_11
	%reg_22 = add i32 %reg_20, %reg_21
	%reg_23 = load i32, ptr %reg_12
	%reg_24 = add i32 %reg_22, %reg_23
	%reg_25 = load i32, ptr %reg_13
	%reg_26 = add i32 %reg_24, %reg_25
	%reg_27 = load i32, ptr %reg_14
	%reg_28 = add i32 %reg_26, %reg_27
	%reg_29 = load i32, ptr %reg_15
	%reg_30 = add i32 %reg_28, %reg_29
	ret i32 %reg_30
}

define i32 @testParam16(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, i32 %reg_7, i32 %reg_8, i32 %reg_9, i32 %reg_10, i32 %reg_11, i32 %reg_12, i32 %reg_13, i32 %reg_14, i32 %reg_15)
{
Block0:  ;Func define at line 50
	%reg_31 = alloca i32
	%reg_30 = alloca i32
	%reg_29 = alloca i32
	%reg_28 = alloca i32
	%reg_27 = alloca i32
	%reg_26 = alloca i32
	%reg_25 = alloca i32
	%reg_24 = alloca i32
	%reg_23 = alloca i32
	%reg_22 = alloca i32
	%reg_21 = alloca i32
	%reg_20 = alloca i32
	%reg_19 = alloca i32
	%reg_18 = alloca i32
	%reg_17 = alloca i32
	%reg_16 = alloca i32
	store i32 %reg_0, ptr %reg_16
	store i32 %reg_1, ptr %reg_17
	store i32 %reg_2, ptr %reg_18
	store i32 %reg_3, ptr %reg_19
	store i32 %reg_4, ptr %reg_20
	store i32 %reg_5, ptr %reg_21
	store i32 %reg_6, ptr %reg_22
	store i32 %reg_7, ptr %reg_23
	store i32 %reg_8, ptr %reg_24
	store i32 %reg_9, ptr %reg_25
	store i32 %reg_10, ptr %reg_26
	store i32 %reg_11, ptr %reg_27
	store i32 %reg_12, ptr %reg_28
	store i32 %reg_13, ptr %reg_29
	store i32 %reg_14, ptr %reg_30
	store i32 %reg_15, ptr %reg_31
	br label %Block1
Block1:  ;Func body at line 50
	%reg_32 = load i32, ptr %reg_16
	%reg_33 = load i32, ptr %reg_17
	%reg_34 = add i32 %reg_32, %reg_33
	%reg_35 = load i32, ptr %reg_18
	%reg_36 = add i32 %reg_34, %reg_35
	%reg_37 = load i32, ptr %reg_19
	%reg_38 = sub i32 %reg_36, %reg_37
	%reg_39 = load i32, ptr %reg_20
	%reg_40 = sub i32 %reg_38, %reg_39
	%reg_41 = load i32, ptr %reg_21
	%reg_42 = sub i32 %reg_40, %reg_41
	%reg_43 = load i32, ptr %reg_22
	%reg_44 = sub i32 %reg_42, %reg_43
	%reg_45 = load i32, ptr %reg_23
	%reg_46 = sub i32 %reg_44, %reg_45
	%reg_47 = load i32, ptr %reg_24
	%reg_48 = add i32 %reg_46, %reg_47
	%reg_49 = load i32, ptr %reg_25
	%reg_50 = add i32 %reg_48, %reg_49
	%reg_51 = load i32, ptr %reg_26
	%reg_52 = add i32 %reg_50, %reg_51
	%reg_53 = load i32, ptr %reg_27
	%reg_54 = add i32 %reg_52, %reg_53
	%reg_55 = load i32, ptr %reg_28
	%reg_56 = add i32 %reg_54, %reg_55
	%reg_57 = load i32, ptr %reg_29
	%reg_58 = add i32 %reg_56, %reg_57
	%reg_59 = load i32, ptr %reg_30
	%reg_60 = add i32 %reg_58, %reg_59
	%reg_61 = load i32, ptr %reg_31
	%reg_62 = add i32 %reg_60, %reg_61
	ret i32 %reg_62
}

define i32 @testParam32(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, i32 %reg_7, i32 %reg_8, i32 %reg_9, i32 %reg_10, i32 %reg_11, i32 %reg_12, i32 %reg_13, i32 %reg_14, i32 %reg_15, i32 %reg_16, i32 %reg_17, i32 %reg_18, i32 %reg_19, i32 %reg_20, i32 %reg_21, i32 %reg_22, i32 %reg_23, i32 %reg_24, i32 %reg_25, i32 %reg_26, i32 %reg_27, i32 %reg_28, i32 %reg_29, i32 %reg_30, i32 %reg_31)
{
Block0:  ;Func define at line 58
	%reg_63 = alloca i32
	%reg_62 = alloca i32
	%reg_61 = alloca i32
	%reg_60 = alloca i32
	%reg_59 = alloca i32
	%reg_58 = alloca i32
	%reg_57 = alloca i32
	%reg_56 = alloca i32
	%reg_55 = alloca i32
	%reg_54 = alloca i32
	%reg_53 = alloca i32
	%reg_52 = alloca i32
	%reg_51 = alloca i32
	%reg_50 = alloca i32
	%reg_49 = alloca i32
	%reg_48 = alloca i32
	%reg_47 = alloca i32
	%reg_46 = alloca i32
	%reg_45 = alloca i32
	%reg_44 = alloca i32
	%reg_43 = alloca i32
	%reg_42 = alloca i32
	%reg_41 = alloca i32
	%reg_40 = alloca i32
	%reg_39 = alloca i32
	%reg_38 = alloca i32
	%reg_37 = alloca i32
	%reg_36 = alloca i32
	%reg_35 = alloca i32
	%reg_34 = alloca i32
	%reg_33 = alloca i32
	%reg_32 = alloca i32
	store i32 %reg_0, ptr %reg_32
	store i32 %reg_1, ptr %reg_33
	store i32 %reg_2, ptr %reg_34
	store i32 %reg_3, ptr %reg_35
	store i32 %reg_4, ptr %reg_36
	store i32 %reg_5, ptr %reg_37
	store i32 %reg_6, ptr %reg_38
	store i32 %reg_7, ptr %reg_39
	store i32 %reg_8, ptr %reg_40
	store i32 %reg_9, ptr %reg_41
	store i32 %reg_10, ptr %reg_42
	store i32 %reg_11, ptr %reg_43
	store i32 %reg_12, ptr %reg_44
	store i32 %reg_13, ptr %reg_45
	store i32 %reg_14, ptr %reg_46
	store i32 %reg_15, ptr %reg_47
	store i32 %reg_16, ptr %reg_48
	store i32 %reg_17, ptr %reg_49
	store i32 %reg_18, ptr %reg_50
	store i32 %reg_19, ptr %reg_51
	store i32 %reg_20, ptr %reg_52
	store i32 %reg_21, ptr %reg_53
	store i32 %reg_22, ptr %reg_54
	store i32 %reg_23, ptr %reg_55
	store i32 %reg_24, ptr %reg_56
	store i32 %reg_25, ptr %reg_57
	store i32 %reg_26, ptr %reg_58
	store i32 %reg_27, ptr %reg_59
	store i32 %reg_28, ptr %reg_60
	store i32 %reg_29, ptr %reg_61
	store i32 %reg_30, ptr %reg_62
	store i32 %reg_31, ptr %reg_63
	br label %Block1
Block1:  ;Func body at line 58
	%reg_64 = load i32, ptr %reg_32
	%reg_65 = load i32, ptr %reg_33
	%reg_66 = add i32 %reg_64, %reg_65
	%reg_67 = load i32, ptr %reg_34
	%reg_68 = add i32 %reg_66, %reg_67
	%reg_69 = load i32, ptr %reg_35
	%reg_70 = add i32 %reg_68, %reg_69
	%reg_71 = load i32, ptr %reg_36
	%reg_72 = add i32 %reg_70, %reg_71
	%reg_73 = load i32, ptr %reg_37
	%reg_74 = add i32 %reg_72, %reg_73
	%reg_75 = load i32, ptr %reg_38
	%reg_76 = add i32 %reg_74, %reg_75
	%reg_77 = load i32, ptr %reg_39
	%reg_78 = add i32 %reg_76, %reg_77
	%reg_79 = load i32, ptr %reg_40
	%reg_80 = add i32 %reg_78, %reg_79
	%reg_81 = load i32, ptr %reg_41
	%reg_82 = add i32 %reg_80, %reg_81
	%reg_83 = load i32, ptr %reg_42
	%reg_84 = add i32 %reg_82, %reg_83
	%reg_85 = load i32, ptr %reg_43
	%reg_86 = add i32 %reg_84, %reg_85
	%reg_87 = load i32, ptr %reg_44
	%reg_88 = add i32 %reg_86, %reg_87
	%reg_89 = load i32, ptr %reg_45
	%reg_90 = add i32 %reg_88, %reg_89
	%reg_91 = load i32, ptr %reg_46
	%reg_92 = add i32 %reg_90, %reg_91
	%reg_93 = load i32, ptr %reg_47
	%reg_94 = add i32 %reg_92, %reg_93
	%reg_95 = load i32, ptr %reg_48
	%reg_96 = add i32 %reg_94, %reg_95
	%reg_97 = load i32, ptr %reg_49
	%reg_98 = add i32 %reg_96, %reg_97
	%reg_99 = load i32, ptr %reg_50
	%reg_100 = sub i32 %reg_98, %reg_99
	%reg_101 = load i32, ptr %reg_51
	%reg_102 = sub i32 %reg_100, %reg_101
	%reg_103 = load i32, ptr %reg_52
	%reg_104 = sub i32 %reg_102, %reg_103
	%reg_105 = load i32, ptr %reg_53
	%reg_106 = sub i32 %reg_104, %reg_105
	%reg_107 = load i32, ptr %reg_54
	%reg_108 = sub i32 %reg_106, %reg_107
	%reg_109 = load i32, ptr %reg_55
	%reg_110 = add i32 %reg_108, %reg_109
	%reg_111 = load i32, ptr %reg_56
	%reg_112 = add i32 %reg_110, %reg_111
	%reg_113 = load i32, ptr %reg_57
	%reg_114 = add i32 %reg_112, %reg_113
	%reg_115 = load i32, ptr %reg_58
	%reg_116 = add i32 %reg_114, %reg_115
	%reg_117 = load i32, ptr %reg_59
	%reg_118 = add i32 %reg_116, %reg_117
	%reg_119 = load i32, ptr %reg_60
	%reg_120 = add i32 %reg_118, %reg_119
	%reg_121 = load i32, ptr %reg_61
	%reg_122 = add i32 %reg_120, %reg_121
	%reg_123 = load i32, ptr %reg_62
	%reg_124 = add i32 %reg_122, %reg_123
	%reg_125 = load i32, ptr %reg_63
	%reg_126 = add i32 %reg_124, %reg_125
	ret i32 %reg_126
}

define i32 @main()
{
Block0:  ;Func define at line 72
	br label %Block1
Block1:  ;Func body at line 72
	%reg_0 = add i32 0, 0
	store i32 %reg_0, ptr @a0
	%reg_1 = add i32 1, 0
	store i32 %reg_1, ptr @a1
	%reg_2 = add i32 2, 0
	store i32 %reg_2, ptr @a2
	%reg_3 = add i32 3, 0
	store i32 %reg_3, ptr @a3
	%reg_4 = add i32 4, 0
	store i32 %reg_4, ptr @a4
	%reg_5 = add i32 5, 0
	store i32 %reg_5, ptr @a5
	%reg_6 = add i32 6, 0
	store i32 %reg_6, ptr @a6
	%reg_7 = add i32 7, 0
	store i32 %reg_7, ptr @a7
	%reg_8 = add i32 8, 0
	store i32 %reg_8, ptr @a8
	%reg_9 = add i32 9, 0
	store i32 %reg_9, ptr @a9
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr @a10
	%reg_11 = add i32 1, 0
	store i32 %reg_11, ptr @a11
	%reg_12 = add i32 2, 0
	store i32 %reg_12, ptr @a12
	%reg_13 = add i32 3, 0
	store i32 %reg_13, ptr @a13
	%reg_14 = add i32 4, 0
	store i32 %reg_14, ptr @a14
	%reg_15 = add i32 5, 0
	store i32 %reg_15, ptr @a15
	%reg_16 = add i32 6, 0
	store i32 %reg_16, ptr @a16
	%reg_17 = add i32 7, 0
	store i32 %reg_17, ptr @a17
	%reg_18 = add i32 8, 0
	store i32 %reg_18, ptr @a18
	%reg_19 = add i32 9, 0
	store i32 %reg_19, ptr @a19
	%reg_20 = add i32 0, 0
	store i32 %reg_20, ptr @a20
	%reg_21 = add i32 1, 0
	store i32 %reg_21, ptr @a21
	%reg_22 = add i32 2, 0
	store i32 %reg_22, ptr @a22
	%reg_23 = add i32 3, 0
	store i32 %reg_23, ptr @a23
	%reg_24 = add i32 4, 0
	store i32 %reg_24, ptr @a24
	%reg_25 = add i32 5, 0
	store i32 %reg_25, ptr @a25
	%reg_26 = add i32 6, 0
	store i32 %reg_26, ptr @a26
	%reg_27 = add i32 7, 0
	store i32 %reg_27, ptr @a27
	%reg_28 = add i32 8, 0
	store i32 %reg_28, ptr @a28
	%reg_29 = add i32 9, 0
	store i32 %reg_29, ptr @a29
	%reg_30 = add i32 0, 0
	store i32 %reg_30, ptr @a30
	%reg_31 = add i32 1, 0
	store i32 %reg_31, ptr @a31
	%reg_32 = add i32 4, 0
	store i32 %reg_32, ptr @a32
	%reg_33 = add i32 5, 0
	store i32 %reg_33, ptr @a33
	%reg_34 = add i32 6, 0
	store i32 %reg_34, ptr @a34
	%reg_35 = add i32 7, 0
	store i32 %reg_35, ptr @a35
	%reg_36 = add i32 8, 0
	store i32 %reg_36, ptr @a36
	%reg_37 = add i32 9, 0
	store i32 %reg_37, ptr @a37
	%reg_38 = add i32 0, 0
	store i32 %reg_38, ptr @a38
	%reg_39 = add i32 1, 0
	store i32 %reg_39, ptr @a39
	%reg_40 = load i32, ptr @a0
	%reg_41 = load i32, ptr @a1
	%reg_42 = load i32, ptr @a2
	%reg_43 = load i32, ptr @a3
	%reg_44 = load i32, ptr @a4
	%reg_45 = load i32, ptr @a5
	%reg_46 = load i32, ptr @a6
	%reg_47 = load i32, ptr @a7
	%reg_48 = call i32 @testParam8(i32 %reg_40, i32 %reg_41, i32 %reg_42, i32 %reg_43, i32 %reg_44, i32 %reg_45, i32 %reg_46, i32 %reg_47)
	store i32 %reg_48, ptr @a0
	%reg_49 = load i32, ptr @a0
	call void @putint(i32 %reg_49)
	%reg_50 = load i32, ptr @a32
	%reg_51 = load i32, ptr @a33
	%reg_52 = load i32, ptr @a34
	%reg_53 = load i32, ptr @a35
	%reg_54 = load i32, ptr @a36
	%reg_55 = load i32, ptr @a37
	%reg_56 = load i32, ptr @a38
	%reg_57 = load i32, ptr @a39
	%reg_58 = load i32, ptr @a8
	%reg_59 = load i32, ptr @a9
	%reg_60 = load i32, ptr @a10
	%reg_61 = load i32, ptr @a11
	%reg_62 = load i32, ptr @a12
	%reg_63 = load i32, ptr @a13
	%reg_64 = load i32, ptr @a14
	%reg_65 = load i32, ptr @a15
	%reg_66 = call i32 @testParam16(i32 %reg_50, i32 %reg_51, i32 %reg_52, i32 %reg_53, i32 %reg_54, i32 %reg_55, i32 %reg_56, i32 %reg_57, i32 %reg_58, i32 %reg_59, i32 %reg_60, i32 %reg_61, i32 %reg_62, i32 %reg_63, i32 %reg_64, i32 %reg_65)
	store i32 %reg_66, ptr @a0
	%reg_67 = load i32, ptr @a0
	call void @putint(i32 %reg_67)
	%reg_68 = load i32, ptr @a0
	%reg_69 = load i32, ptr @a1
	%reg_70 = load i32, ptr @a2
	%reg_71 = load i32, ptr @a3
	%reg_72 = load i32, ptr @a4
	%reg_73 = load i32, ptr @a5
	%reg_74 = load i32, ptr @a6
	%reg_75 = load i32, ptr @a7
	%reg_76 = load i32, ptr @a8
	%reg_77 = load i32, ptr @a9
	%reg_78 = load i32, ptr @a10
	%reg_79 = load i32, ptr @a11
	%reg_80 = load i32, ptr @a12
	%reg_81 = load i32, ptr @a13
	%reg_82 = load i32, ptr @a14
	%reg_83 = load i32, ptr @a15
	%reg_84 = load i32, ptr @a16
	%reg_85 = load i32, ptr @a17
	%reg_86 = load i32, ptr @a18
	%reg_87 = load i32, ptr @a19
	%reg_88 = load i32, ptr @a20
	%reg_89 = load i32, ptr @a21
	%reg_90 = load i32, ptr @a22
	%reg_91 = load i32, ptr @a23
	%reg_92 = load i32, ptr @a24
	%reg_93 = load i32, ptr @a25
	%reg_94 = load i32, ptr @a26
	%reg_95 = load i32, ptr @a27
	%reg_96 = load i32, ptr @a28
	%reg_97 = load i32, ptr @a29
	%reg_98 = load i32, ptr @a30
	%reg_99 = load i32, ptr @a31
	%reg_100 = call i32 @testParam32(i32 %reg_68, i32 %reg_69, i32 %reg_70, i32 %reg_71, i32 %reg_72, i32 %reg_73, i32 %reg_74, i32 %reg_75, i32 %reg_76, i32 %reg_77, i32 %reg_78, i32 %reg_79, i32 %reg_80, i32 %reg_81, i32 %reg_82, i32 %reg_83, i32 %reg_84, i32 %reg_85, i32 %reg_86, i32 %reg_87, i32 %reg_88, i32 %reg_89, i32 %reg_90, i32 %reg_91, i32 %reg_92, i32 %reg_93, i32 %reg_94, i32 %reg_95, i32 %reg_96, i32 %reg_97, i32 %reg_98, i32 %reg_99)
	store i32 %reg_100, ptr @a0
	%reg_101 = load i32, ptr @a0
	call void @putint(i32 %reg_101)
	%reg_102 = add i32 0, 0
	ret i32 %reg_102
}
