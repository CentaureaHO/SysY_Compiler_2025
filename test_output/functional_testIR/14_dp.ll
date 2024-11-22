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

@t = global [1005 x [2 x i32]] zeroinitializer
@dp = global [1005 x [35 x i32]] zeroinitializer

define i32 @main()
{
Block0:  ;Func define at line 2
	%reg_125 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 2
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
	%reg_10 = call i32 @getint()
	store i32 %reg_10, ptr %reg_0
	%reg_11 = call i32 @getint()
	store i32 %reg_11, ptr %reg_2
	%reg_12 = add i32 1, 0
	store i32 %reg_12, ptr %reg_6
	br label %Block2
Block2:  ;While condition at line 8
	%reg_13 = load i32, ptr %reg_6
	%reg_14 = load i32, ptr %reg_0
	%reg_15 = icmp sle i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block3, label %Block4
Block3:  ;While body at line 8
	%reg_16 = call i32 @getint()
	store i32 %reg_16, ptr %reg_4
	%reg_17 = load i32, ptr %reg_6
	%reg_18 = load i32, ptr %reg_4
	%reg_19 = add i32 2, 0
	%reg_20 = srem i32 %reg_18, %reg_19
	%reg_21 = getelementptr [1005 x [2 x i32]], ptr @t, i32 0, i32 %reg_17, i32 %reg_20
	%reg_22 = add i32 1, 0
	store i32 %reg_22, ptr %reg_21
	%reg_23 = load i32, ptr %reg_6
	%reg_24 = add i32 0, 0
	%reg_25 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_23, i32 %reg_24
	%reg_26 = load i32, ptr %reg_6
	%reg_27 = add i32 1, 0
	%reg_28 = sub i32 %reg_26, %reg_27
	%reg_29 = add i32 0, 0
	%reg_30 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_28, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	%reg_32 = load i32, ptr %reg_6
	%reg_33 = add i32 1, 0
	%reg_34 = getelementptr [1005 x [2 x i32]], ptr @t, i32 0, i32 %reg_32, i32 %reg_33
	%reg_35 = load i32, ptr %reg_34
	%reg_36 = add i32 %reg_31, %reg_35
	store i32 %reg_36, ptr %reg_25
	%reg_37 = load i32, ptr %reg_6
	%reg_38 = add i32 1, 0
	%reg_39 = add i32 %reg_37, %reg_38
	store i32 %reg_39, ptr %reg_6
	br label %Block2
Block4:  ;While end at line 8
	%reg_40 = add i32 1, 0
	store i32 %reg_40, ptr %reg_6
	br label %Block5
Block5:  ;While condition at line 16
	%reg_41 = load i32, ptr %reg_6
	%reg_42 = load i32, ptr %reg_0
	%reg_43 = icmp sle i32 %reg_41, %reg_42
	br i1 %reg_43, label %Block6, label %Block7
Block6:  ;While body at line 16
	%reg_44 = add i32 1, 0
	store i32 %reg_44, ptr %reg_8
	br label %Block8
Block7:  ;While end at line 16
	%reg_126 = add i32 0, 0
	store i32 %reg_126, ptr %reg_125
	%reg_127 = add i32 0, 0
	store i32 %reg_127, ptr %reg_8
	br label %Block14
Block8:  ;While condition at line 18
	%reg_45 = load i32, ptr %reg_8
	%reg_46 = load i32, ptr %reg_2
	%reg_47 = icmp sle i32 %reg_45, %reg_46
	br i1 %reg_47, label %Block9, label %Block10
Block9:  ;While body at line 18
	%reg_48 = load i32, ptr %reg_6
	%reg_49 = add i32 1, 0
	%reg_50 = sub i32 %reg_48, %reg_49
	%reg_51 = load i32, ptr %reg_8
	%reg_52 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_50, i32 %reg_51
	%reg_53 = load i32, ptr %reg_52
	%reg_54 = load i32, ptr %reg_6
	%reg_55 = load i32, ptr %reg_8
	%reg_56 = add i32 1, 0
	%reg_57 = add i32 %reg_55, %reg_56
	%reg_58 = add i32 2, 0
	%reg_59 = srem i32 %reg_57, %reg_58
	%reg_60 = getelementptr [1005 x [2 x i32]], ptr @t, i32 0, i32 %reg_54, i32 %reg_59
	%reg_61 = load i32, ptr %reg_60
	%reg_62 = add i32 %reg_53, %reg_61
	%reg_63 = load i32, ptr %reg_6
	%reg_64 = add i32 1, 0
	%reg_65 = sub i32 %reg_63, %reg_64
	%reg_66 = load i32, ptr %reg_8
	%reg_67 = add i32 1, 0
	%reg_68 = sub i32 %reg_66, %reg_67
	%reg_69 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_65, i32 %reg_68
	%reg_70 = load i32, ptr %reg_69
	%reg_71 = load i32, ptr %reg_6
	%reg_72 = load i32, ptr %reg_8
	%reg_73 = add i32 1, 0
	%reg_74 = add i32 %reg_72, %reg_73
	%reg_75 = add i32 2, 0
	%reg_76 = srem i32 %reg_74, %reg_75
	%reg_77 = getelementptr [1005 x [2 x i32]], ptr @t, i32 0, i32 %reg_71, i32 %reg_76
	%reg_78 = load i32, ptr %reg_77
	%reg_79 = add i32 %reg_70, %reg_78
	%reg_80 = icmp sgt i32 %reg_62, %reg_79
	br i1 %reg_80, label %Block11, label %Block13
Block10:  ;While end at line 18
	%reg_122 = load i32, ptr %reg_6
	%reg_123 = add i32 1, 0
	%reg_124 = add i32 %reg_122, %reg_123
	store i32 %reg_124, ptr %reg_6
	br label %Block5
Block11:  ;If then at line 19
	%reg_81 = load i32, ptr %reg_6
	%reg_82 = load i32, ptr %reg_8
	%reg_83 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_81, i32 %reg_82
	%reg_84 = load i32, ptr %reg_6
	%reg_85 = add i32 1, 0
	%reg_86 = sub i32 %reg_84, %reg_85
	%reg_87 = load i32, ptr %reg_8
	%reg_88 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_86, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	%reg_90 = load i32, ptr %reg_6
	%reg_91 = load i32, ptr %reg_8
	%reg_92 = add i32 1, 0
	%reg_93 = add i32 %reg_91, %reg_92
	%reg_94 = add i32 2, 0
	%reg_95 = srem i32 %reg_93, %reg_94
	%reg_96 = getelementptr [1005 x [2 x i32]], ptr @t, i32 0, i32 %reg_90, i32 %reg_95
	%reg_97 = load i32, ptr %reg_96
	%reg_98 = add i32 %reg_89, %reg_97
	store i32 %reg_98, ptr %reg_83
	br label %Block12
Block12:  ;If end at line 19
	%reg_119 = load i32, ptr %reg_8
	%reg_120 = add i32 1, 0
	%reg_121 = add i32 %reg_119, %reg_120
	store i32 %reg_121, ptr %reg_8
	br label %Block8
Block13:  ;If else at line 19
	%reg_99 = load i32, ptr %reg_6
	%reg_100 = load i32, ptr %reg_8
	%reg_101 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_99, i32 %reg_100
	%reg_102 = load i32, ptr %reg_6
	%reg_103 = add i32 1, 0
	%reg_104 = sub i32 %reg_102, %reg_103
	%reg_105 = load i32, ptr %reg_8
	%reg_106 = add i32 1, 0
	%reg_107 = sub i32 %reg_105, %reg_106
	%reg_108 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_104, i32 %reg_107
	%reg_109 = load i32, ptr %reg_108
	%reg_110 = load i32, ptr %reg_6
	%reg_111 = load i32, ptr %reg_8
	%reg_112 = add i32 1, 0
	%reg_113 = add i32 %reg_111, %reg_112
	%reg_114 = add i32 2, 0
	%reg_115 = srem i32 %reg_113, %reg_114
	%reg_116 = getelementptr [1005 x [2 x i32]], ptr @t, i32 0, i32 %reg_110, i32 %reg_115
	%reg_117 = load i32, ptr %reg_116
	%reg_118 = add i32 %reg_109, %reg_117
	store i32 %reg_118, ptr %reg_101
	br label %Block12
Block14:  ;While condition at line 32
	%reg_128 = load i32, ptr %reg_8
	%reg_129 = load i32, ptr %reg_2
	%reg_130 = icmp sle i32 %reg_128, %reg_129
	br i1 %reg_130, label %Block15, label %Block16
Block15:  ;While body at line 32
	%reg_131 = load i32, ptr %reg_125
	%reg_132 = load i32, ptr %reg_0
	%reg_133 = load i32, ptr %reg_8
	%reg_134 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_132, i32 %reg_133
	%reg_135 = load i32, ptr %reg_134
	%reg_136 = icmp slt i32 %reg_131, %reg_135
	br i1 %reg_136, label %Block17, label %Block18
Block16:  ;While end at line 32
	%reg_144 = load i32, ptr %reg_125
	ret i32 %reg_144
Block17:  ;If then at line 33
	%reg_137 = load i32, ptr %reg_0
	%reg_138 = load i32, ptr %reg_8
	%reg_139 = getelementptr [1005 x [35 x i32]], ptr @dp, i32 0, i32 %reg_137, i32 %reg_138
	%reg_140 = load i32, ptr %reg_139
	store i32 %reg_140, ptr %reg_125
	br label %Block18
Block18:  ;If end at line 33
	%reg_141 = load i32, ptr %reg_8
	%reg_142 = add i32 1, 0
	%reg_143 = add i32 %reg_141, %reg_142
	store i32 %reg_143, ptr %reg_8
	br label %Block14
}
