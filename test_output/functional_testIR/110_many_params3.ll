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


define i32 @testParam8(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, i32 %reg_7)
{
Block0:  ;Func define at line 1
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
Block1:  ;Func body at line 1
	%reg_16 = load i32, ptr %reg_8
	%reg_17 = load i32, ptr %reg_9
	%reg_18 = sub i32 %reg_16, %reg_17
	%reg_19 = load i32, ptr %reg_10
	%reg_20 = add i32 %reg_18, %reg_19
	%reg_21 = load i32, ptr %reg_11
	%reg_22 = sub i32 %reg_20, %reg_21
	%reg_23 = load i32, ptr %reg_12
	%reg_24 = sub i32 %reg_22, %reg_23
	%reg_25 = load i32, ptr %reg_13
	%reg_26 = sub i32 %reg_24, %reg_25
	%reg_27 = load i32, ptr %reg_14
	%reg_28 = add i32 %reg_26, %reg_27
	%reg_29 = load i32, ptr %reg_15
	%reg_30 = add i32 %reg_28, %reg_29
	ret i32 %reg_30
}

define i32 @testParam16(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, i32 %reg_7, i32 %reg_8, i32 %reg_9, i32 %reg_10, i32 %reg_11, i32 %reg_12, i32 %reg_13, i32 %reg_14, i32 %reg_15)
{
Block0:  ;Func define at line 6
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
Block1:  ;Func body at line 6
	%reg_32 = load i32, ptr %reg_16
	%reg_33 = load i32, ptr %reg_17
	%reg_34 = add i32 %reg_32, %reg_33
	%reg_35 = load i32, ptr %reg_18
	%reg_36 = add i32 %reg_34, %reg_35
	%reg_37 = load i32, ptr %reg_19
	%reg_38 = add i32 %reg_36, %reg_37
	%reg_39 = load i32, ptr %reg_20
	%reg_40 = sub i32 %reg_38, %reg_39
	%reg_41 = load i32, ptr %reg_21
	%reg_42 = add i32 %reg_40, %reg_41
	%reg_43 = load i32, ptr %reg_22
	%reg_44 = add i32 %reg_42, %reg_43
	%reg_45 = load i32, ptr %reg_23
	%reg_46 = add i32 %reg_44, %reg_45
	%reg_47 = load i32, ptr %reg_24
	%reg_48 = sub i32 %reg_46, %reg_47
	%reg_49 = load i32, ptr %reg_25
	%reg_50 = add i32 %reg_48, %reg_49
	%reg_51 = load i32, ptr %reg_26
	%reg_52 = sub i32 %reg_50, %reg_51
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
Block0:  ;Func define at line 14
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
Block1:  ;Func body at line 14
	%reg_64 = load i32, ptr %reg_32
	%reg_65 = load i32, ptr %reg_33
	%reg_66 = load i32, ptr %reg_34
	%reg_67 = mul i32 %reg_65, %reg_66
	%reg_68 = add i32 %reg_64, %reg_67
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
	%reg_88 = sub i32 %reg_86, %reg_87
	%reg_89 = load i32, ptr %reg_45
	%reg_90 = sub i32 %reg_88, %reg_89
	%reg_91 = load i32, ptr %reg_46
	%reg_92 = sub i32 %reg_90, %reg_91
	%reg_93 = load i32, ptr %reg_47
	%reg_94 = sub i32 %reg_92, %reg_93
	%reg_95 = load i32, ptr %reg_48
	%reg_96 = sub i32 %reg_94, %reg_95
	%reg_97 = load i32, ptr %reg_49
	%reg_98 = sub i32 %reg_96, %reg_97
	%reg_99 = load i32, ptr %reg_50
	%reg_100 = sub i32 %reg_98, %reg_99
	%reg_101 = load i32, ptr %reg_51
	%reg_102 = sub i32 %reg_100, %reg_101
	%reg_103 = load i32, ptr %reg_52
	%reg_104 = sub i32 %reg_102, %reg_103
	%reg_105 = load i32, ptr %reg_53
	%reg_106 = sub i32 %reg_104, %reg_105
	%reg_107 = load i32, ptr %reg_54
	%reg_108 = add i32 %reg_106, %reg_107
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
Block0:  ;Func define at line 28
	%reg_62 = alloca i32
	%reg_60 = alloca i32
	%reg_58 = alloca i32
	%reg_56 = alloca i32
	%reg_54 = alloca i32
	%reg_52 = alloca i32
	%reg_50 = alloca i32
	%reg_48 = alloca i32
	%reg_46 = alloca i32
	%reg_44 = alloca i32
	%reg_42 = alloca i32
	%reg_40 = alloca i32
	%reg_38 = alloca i32
	%reg_36 = alloca i32
	%reg_34 = alloca i32
	%reg_32 = alloca i32
	%reg_30 = alloca i32
	%reg_28 = alloca i32
	%reg_26 = alloca i32
	%reg_24 = alloca i32
	%reg_22 = alloca i32
	%reg_20 = alloca i32
	%reg_18 = alloca i32
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
Block1:  ;Func body at line 28
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
	%reg_31 = add i32 0, 0
	store i32 %reg_31, ptr %reg_30
	%reg_33 = add i32 0, 0
	store i32 %reg_33, ptr %reg_32
	%reg_35 = add i32 0, 0
	store i32 %reg_35, ptr %reg_34
	%reg_37 = add i32 0, 0
	store i32 %reg_37, ptr %reg_36
	%reg_39 = add i32 0, 0
	store i32 %reg_39, ptr %reg_38
	%reg_41 = add i32 0, 0
	store i32 %reg_41, ptr %reg_40
	%reg_43 = add i32 0, 0
	store i32 %reg_43, ptr %reg_42
	%reg_45 = add i32 0, 0
	store i32 %reg_45, ptr %reg_44
	%reg_47 = add i32 0, 0
	store i32 %reg_47, ptr %reg_46
	%reg_49 = add i32 0, 0
	store i32 %reg_49, ptr %reg_48
	%reg_51 = add i32 0, 0
	store i32 %reg_51, ptr %reg_50
	%reg_53 = add i32 0, 0
	store i32 %reg_53, ptr %reg_52
	%reg_55 = add i32 0, 0
	store i32 %reg_55, ptr %reg_54
	%reg_57 = add i32 0, 0
	store i32 %reg_57, ptr %reg_56
	%reg_59 = add i32 0, 0
	store i32 %reg_59, ptr %reg_58
	%reg_61 = add i32 0, 0
	store i32 %reg_61, ptr %reg_60
	%reg_63 = add i32 0, 0
	store i32 %reg_63, ptr %reg_62
	%reg_64 = add i32 0, 0
	store i32 %reg_64, ptr %reg_0
	%reg_65 = add i32 1, 0
	store i32 %reg_65, ptr %reg_2
	%reg_66 = add i32 2, 0
	store i32 %reg_66, ptr %reg_4
	%reg_67 = add i32 3, 0
	store i32 %reg_67, ptr %reg_6
	%reg_68 = add i32 4, 0
	store i32 %reg_68, ptr %reg_8
	%reg_69 = add i32 5, 0
	store i32 %reg_69, ptr %reg_10
	%reg_70 = add i32 6, 0
	store i32 %reg_70, ptr %reg_12
	%reg_71 = add i32 7, 0
	store i32 %reg_71, ptr %reg_14
	%reg_72 = add i32 8, 0
	store i32 %reg_72, ptr %reg_16
	%reg_73 = add i32 9, 0
	store i32 %reg_73, ptr %reg_18
	%reg_74 = add i32 0, 0
	store i32 %reg_74, ptr %reg_20
	%reg_75 = add i32 1, 0
	store i32 %reg_75, ptr %reg_22
	%reg_76 = add i32 2, 0
	store i32 %reg_76, ptr %reg_24
	%reg_77 = add i32 3, 0
	store i32 %reg_77, ptr %reg_26
	%reg_78 = add i32 4, 0
	store i32 %reg_78, ptr %reg_28
	%reg_79 = add i32 5, 0
	store i32 %reg_79, ptr %reg_30
	%reg_80 = add i32 6, 0
	store i32 %reg_80, ptr %reg_32
	%reg_81 = add i32 7, 0
	store i32 %reg_81, ptr %reg_34
	%reg_82 = add i32 8, 0
	store i32 %reg_82, ptr %reg_36
	%reg_83 = add i32 9, 0
	store i32 %reg_83, ptr %reg_38
	%reg_84 = add i32 0, 0
	store i32 %reg_84, ptr %reg_40
	%reg_85 = add i32 1, 0
	store i32 %reg_85, ptr %reg_42
	%reg_86 = add i32 2, 0
	store i32 %reg_86, ptr %reg_44
	%reg_87 = add i32 3, 0
	store i32 %reg_87, ptr %reg_46
	%reg_88 = add i32 4, 0
	store i32 %reg_88, ptr %reg_48
	%reg_89 = add i32 5, 0
	store i32 %reg_89, ptr %reg_50
	%reg_90 = add i32 6, 0
	store i32 %reg_90, ptr %reg_52
	%reg_91 = add i32 7, 0
	store i32 %reg_91, ptr %reg_54
	%reg_92 = add i32 8, 0
	store i32 %reg_92, ptr %reg_56
	%reg_93 = add i32 9, 0
	store i32 %reg_93, ptr %reg_58
	%reg_94 = add i32 0, 0
	store i32 %reg_94, ptr %reg_60
	%reg_95 = add i32 1, 0
	store i32 %reg_95, ptr %reg_62
	%reg_96 = load i32, ptr %reg_0
	%reg_97 = load i32, ptr %reg_2
	%reg_98 = load i32, ptr %reg_4
	%reg_99 = load i32, ptr %reg_6
	%reg_100 = load i32, ptr %reg_8
	%reg_101 = load i32, ptr %reg_10
	%reg_102 = load i32, ptr %reg_12
	%reg_103 = load i32, ptr %reg_14
	%reg_104 = call i32 @testParam8(i32 %reg_96, i32 %reg_97, i32 %reg_98, i32 %reg_99, i32 %reg_100, i32 %reg_101, i32 %reg_102, i32 %reg_103)
	%reg_105 = load i32, ptr %reg_2
	%reg_106 = load i32, ptr %reg_4
	%reg_107 = load i32, ptr %reg_6
	%reg_108 = load i32, ptr %reg_8
	%reg_109 = load i32, ptr %reg_10
	%reg_110 = load i32, ptr %reg_12
	%reg_111 = load i32, ptr %reg_14
	%reg_112 = load i32, ptr %reg_16
	%reg_113 = load i32, ptr %reg_18
	%reg_114 = load i32, ptr %reg_20
	%reg_115 = load i32, ptr %reg_22
	%reg_116 = load i32, ptr %reg_24
	%reg_117 = load i32, ptr %reg_26
	%reg_118 = load i32, ptr %reg_28
	%reg_119 = load i32, ptr %reg_30
	%reg_120 = call i32 @testParam16(i32 %reg_104, i32 %reg_105, i32 %reg_106, i32 %reg_107, i32 %reg_108, i32 %reg_109, i32 %reg_110, i32 %reg_111, i32 %reg_112, i32 %reg_113, i32 %reg_114, i32 %reg_115, i32 %reg_116, i32 %reg_117, i32 %reg_118, i32 %reg_119)
	%reg_121 = load i32, ptr %reg_2
	%reg_122 = load i32, ptr %reg_4
	%reg_123 = load i32, ptr %reg_6
	%reg_124 = load i32, ptr %reg_8
	%reg_125 = load i32, ptr %reg_10
	%reg_126 = load i32, ptr %reg_12
	%reg_127 = load i32, ptr %reg_14
	%reg_128 = load i32, ptr %reg_16
	%reg_129 = load i32, ptr %reg_18
	%reg_130 = load i32, ptr %reg_20
	%reg_131 = load i32, ptr %reg_22
	%reg_132 = load i32, ptr %reg_24
	%reg_133 = load i32, ptr %reg_26
	%reg_134 = load i32, ptr %reg_28
	%reg_135 = load i32, ptr %reg_30
	%reg_136 = load i32, ptr %reg_32
	%reg_137 = load i32, ptr %reg_34
	%reg_138 = load i32, ptr %reg_36
	%reg_139 = load i32, ptr %reg_38
	%reg_140 = load i32, ptr %reg_40
	%reg_141 = load i32, ptr %reg_42
	%reg_142 = load i32, ptr %reg_44
	%reg_143 = load i32, ptr %reg_46
	%reg_144 = load i32, ptr %reg_48
	%reg_145 = load i32, ptr %reg_50
	%reg_146 = load i32, ptr %reg_52
	%reg_147 = load i32, ptr %reg_54
	%reg_148 = load i32, ptr %reg_56
	%reg_149 = load i32, ptr %reg_58
	%reg_150 = load i32, ptr %reg_60
	%reg_151 = load i32, ptr %reg_62
	%reg_152 = call i32 @testParam32(i32 %reg_120, i32 %reg_121, i32 %reg_122, i32 %reg_123, i32 %reg_124, i32 %reg_125, i32 %reg_126, i32 %reg_127, i32 %reg_128, i32 %reg_129, i32 %reg_130, i32 %reg_131, i32 %reg_132, i32 %reg_133, i32 %reg_134, i32 %reg_135, i32 %reg_136, i32 %reg_137, i32 %reg_138, i32 %reg_139, i32 %reg_140, i32 %reg_141, i32 %reg_142, i32 %reg_143, i32 %reg_144, i32 %reg_145, i32 %reg_146, i32 %reg_147, i32 %reg_148, i32 %reg_149, i32 %reg_150, i32 %reg_151)
	store i32 %reg_152, ptr %reg_0
	%reg_153 = load i32, ptr %reg_0
	call void @putint(i32 %reg_153)
	%reg_154 = add i32 0, 0
	ret i32 %reg_154
}
