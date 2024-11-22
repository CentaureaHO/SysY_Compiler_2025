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

@N = global i32 10000

define i32 @long_array(i32 %reg_0)
{
Block0:  ;Func define at line 3
	%reg_103 = alloca i32
	%reg_80 = alloca i32
	%reg_60 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca [10000 x i32]
	%reg_3 = alloca [10000 x i32]
	%reg_2 = alloca [10000 x i32]
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 3
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 40000, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_3, i8 0, i32 40000, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_4, i8 0, i32 40000, i1 1)
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	br label %Block2
Block2:  ;While condition at line 8
	%reg_7 = load i32, ptr %reg_5
	%reg_8 = load i32, ptr @N
	%reg_9 = icmp slt i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block3, label %Block4
Block3:  ;While body at line 8
	%reg_10 = load i32, ptr %reg_5
	%reg_11 = getelementptr [10000 x i32], ptr %reg_2, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_5
	%reg_13 = load i32, ptr %reg_5
	%reg_14 = mul i32 %reg_12, %reg_13
	%reg_15 = add i32 10, 0
	%reg_16 = srem i32 %reg_14, %reg_15
	store i32 %reg_16, ptr %reg_11
	%reg_17 = load i32, ptr %reg_5
	%reg_18 = add i32 1, 0
	%reg_19 = add i32 %reg_17, %reg_18
	store i32 %reg_19, ptr %reg_5
	br label %Block2
Block4:  ;While end at line 8
	%reg_20 = add i32 0, 0
	store i32 %reg_20, ptr %reg_5
	br label %Block5
Block5:  ;While condition at line 13
	%reg_21 = load i32, ptr %reg_5
	%reg_22 = load i32, ptr @N
	%reg_23 = icmp slt i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block6, label %Block7
Block6:  ;While body at line 13
	%reg_24 = load i32, ptr %reg_5
	%reg_25 = getelementptr [10000 x i32], ptr %reg_3, i32 0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_5
	%reg_27 = getelementptr [10000 x i32], ptr %reg_2, i32 0, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	%reg_29 = load i32, ptr %reg_5
	%reg_30 = getelementptr [10000 x i32], ptr %reg_2, i32 0, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	%reg_32 = mul i32 %reg_28, %reg_31
	%reg_33 = add i32 10, 0
	%reg_34 = srem i32 %reg_32, %reg_33
	store i32 %reg_34, ptr %reg_25
	%reg_35 = load i32, ptr %reg_5
	%reg_36 = add i32 1, 0
	%reg_37 = add i32 %reg_35, %reg_36
	store i32 %reg_37, ptr %reg_5
	br label %Block5
Block7:  ;While end at line 13
	%reg_38 = add i32 0, 0
	store i32 %reg_38, ptr %reg_5
	br label %Block8
Block8:  ;While condition at line 18
	%reg_39 = load i32, ptr %reg_5
	%reg_40 = load i32, ptr @N
	%reg_41 = icmp slt i32 %reg_39, %reg_40
	br i1 %reg_41, label %Block9, label %Block10
Block9:  ;While body at line 18
	%reg_42 = load i32, ptr %reg_5
	%reg_43 = getelementptr [10000 x i32], ptr %reg_4, i32 0, i32 %reg_42
	%reg_44 = load i32, ptr %reg_5
	%reg_45 = getelementptr [10000 x i32], ptr %reg_3, i32 0, i32 %reg_44
	%reg_46 = load i32, ptr %reg_45
	%reg_47 = load i32, ptr %reg_5
	%reg_48 = getelementptr [10000 x i32], ptr %reg_3, i32 0, i32 %reg_47
	%reg_49 = load i32, ptr %reg_48
	%reg_50 = mul i32 %reg_46, %reg_49
	%reg_51 = add i32 100, 0
	%reg_52 = srem i32 %reg_50, %reg_51
	%reg_53 = load i32, ptr %reg_5
	%reg_54 = getelementptr [10000 x i32], ptr %reg_2, i32 0, i32 %reg_53
	%reg_55 = load i32, ptr %reg_54
	%reg_56 = add i32 %reg_52, %reg_55
	store i32 %reg_56, ptr %reg_43
	%reg_57 = load i32, ptr %reg_5
	%reg_58 = add i32 1, 0
	%reg_59 = add i32 %reg_57, %reg_58
	store i32 %reg_59, ptr %reg_5
	br label %Block8
Block10:  ;While end at line 18
	%reg_61 = add i32 0, 0
	store i32 %reg_61, ptr %reg_60
	%reg_62 = add i32 0, 0
	store i32 %reg_62, ptr %reg_5
	br label %Block11
Block11:  ;While condition at line 24
	%reg_63 = load i32, ptr %reg_5
	%reg_64 = load i32, ptr @N
	%reg_65 = icmp slt i32 %reg_63, %reg_64
	br i1 %reg_65, label %Block12, label %Block13
Block12:  ;While body at line 24
	%reg_66 = load i32, ptr %reg_5
	%reg_67 = add i32 10, 0
	%reg_68 = icmp slt i32 %reg_66, %reg_67
	br i1 %reg_68, label %Block14, label %Block16
Block13:  ;While end at line 24
	%reg_152 = load i32, ptr %reg_60
	ret i32 %reg_152
Block14:  ;If then at line 25
	%reg_69 = load i32, ptr %reg_60
	%reg_70 = load i32, ptr %reg_5
	%reg_71 = getelementptr [10000 x i32], ptr %reg_4, i32 0, i32 %reg_70
	%reg_72 = load i32, ptr %reg_71
	%reg_73 = add i32 %reg_69, %reg_72
	%reg_74 = add i32 1333, 0
	%reg_75 = srem i32 %reg_73, %reg_74
	store i32 %reg_75, ptr %reg_60
	%reg_76 = load i32, ptr %reg_60
	call void @putint(i32 %reg_76)
	br label %Block15
Block15:  ;If end at line 25
	%reg_149 = load i32, ptr %reg_5
	%reg_150 = add i32 1, 0
	%reg_151 = add i32 %reg_149, %reg_150
	store i32 %reg_151, ptr %reg_5
	br label %Block11
Block16:  ;If else at line 25
	%reg_77 = load i32, ptr %reg_5
	%reg_78 = add i32 20, 0
	%reg_79 = icmp slt i32 %reg_77, %reg_78
	br i1 %reg_79, label %Block17, label %Block19
Block17:  ;If then at line 29
	%reg_81 = load i32, ptr @N
	%reg_82 = add i32 2, 0
	%reg_83 = sdiv i32 %reg_81, %reg_82
	store i32 %reg_83, ptr %reg_80
	br label %Block20
Block18:  ;If end at line 29
	br label %Block15
Block19:  ;If else at line 29
	%reg_100 = load i32, ptr %reg_5
	%reg_101 = add i32 30, 0
	%reg_102 = icmp slt i32 %reg_100, %reg_101
	br i1 %reg_102, label %Block23, label %Block25
Block20:  ;While condition at line 31
	%reg_84 = load i32, ptr %reg_80
	%reg_85 = load i32, ptr @N
	%reg_86 = icmp slt i32 %reg_84, %reg_85
	br i1 %reg_86, label %Block21, label %Block22
Block21:  ;While body at line 31
	%reg_87 = load i32, ptr %reg_60
	%reg_88 = load i32, ptr %reg_5
	%reg_89 = getelementptr [10000 x i32], ptr %reg_4, i32 0, i32 %reg_88
	%reg_90 = load i32, ptr %reg_89
	%reg_91 = add i32 %reg_87, %reg_90
	%reg_92 = load i32, ptr %reg_80
	%reg_93 = getelementptr [10000 x i32], ptr %reg_2, i32 0, i32 %reg_92
	%reg_94 = load i32, ptr %reg_93
	%reg_95 = sub i32 %reg_91, %reg_94
	store i32 %reg_95, ptr %reg_60
	%reg_96 = load i32, ptr %reg_80
	%reg_97 = add i32 1, 0
	%reg_98 = add i32 %reg_96, %reg_97
	store i32 %reg_98, ptr %reg_80
	br label %Block20
Block22:  ;While end at line 31
	%reg_99 = load i32, ptr %reg_60
	call void @putint(i32 %reg_99)
	br label %Block18
Block23:  ;If then at line 37
	%reg_104 = load i32, ptr @N
	%reg_105 = add i32 2, 0
	%reg_106 = sdiv i32 %reg_104, %reg_105
	store i32 %reg_106, ptr %reg_103
	br label %Block26
Block24:  ;If end at line 37
	br label %Block18
Block25:  ;If else at line 37
	%reg_140 = load i32, ptr %reg_60
	%reg_141 = load i32, ptr %reg_5
	%reg_142 = getelementptr [10000 x i32], ptr %reg_4, i32 0, i32 %reg_141
	%reg_143 = load i32, ptr %reg_142
	%reg_144 = load i32, ptr %reg_1
	%reg_145 = mul i32 %reg_143, %reg_144
	%reg_146 = add i32 %reg_140, %reg_145
	%reg_147 = add i32 99988, 0
	%reg_148 = srem i32 %reg_146, %reg_147
	store i32 %reg_148, ptr %reg_60
	br label %Block24
Block26:  ;While condition at line 39
	%reg_107 = load i32, ptr %reg_103
	%reg_108 = load i32, ptr @N
	%reg_109 = icmp slt i32 %reg_107, %reg_108
	br i1 %reg_109, label %Block27, label %Block28
Block27:  ;While body at line 39
	%reg_110 = load i32, ptr %reg_103
	%reg_111 = add i32 2233, 0
	%reg_112 = icmp sgt i32 %reg_110, %reg_111
	br i1 %reg_112, label %Block29, label %Block31
Block28:  ;While end at line 39
	%reg_139 = load i32, ptr %reg_60
	call void @putint(i32 %reg_139)
	br label %Block24
Block29:  ;If then at line 40
	%reg_113 = load i32, ptr %reg_60
	%reg_114 = load i32, ptr %reg_5
	%reg_115 = getelementptr [10000 x i32], ptr %reg_3, i32 0, i32 %reg_114
	%reg_116 = load i32, ptr %reg_115
	%reg_117 = add i32 %reg_113, %reg_116
	%reg_118 = load i32, ptr %reg_103
	%reg_119 = getelementptr [10000 x i32], ptr %reg_2, i32 0, i32 %reg_118
	%reg_120 = load i32, ptr %reg_119
	%reg_121 = sub i32 %reg_117, %reg_120
	store i32 %reg_121, ptr %reg_60
	%reg_122 = load i32, ptr %reg_103
	%reg_123 = add i32 1, 0
	%reg_124 = add i32 %reg_122, %reg_123
	store i32 %reg_124, ptr %reg_103
	br label %Block30
Block30:  ;If end at line 40
	br label %Block26
Block31:  ;If else at line 40
	%reg_125 = load i32, ptr %reg_60
	%reg_126 = load i32, ptr %reg_5
	%reg_127 = getelementptr [10000 x i32], ptr %reg_2, i32 0, i32 %reg_126
	%reg_128 = load i32, ptr %reg_127
	%reg_129 = add i32 %reg_125, %reg_128
	%reg_130 = load i32, ptr %reg_103
	%reg_131 = getelementptr [10000 x i32], ptr %reg_4, i32 0, i32 %reg_130
	%reg_132 = load i32, ptr %reg_131
	%reg_133 = add i32 %reg_129, %reg_132
	%reg_134 = add i32 13333, 0
	%reg_135 = srem i32 %reg_133, %reg_134
	store i32 %reg_135, ptr %reg_60
	%reg_136 = load i32, ptr %reg_103
	%reg_137 = add i32 2, 0
	%reg_138 = add i32 %reg_136, %reg_137
	store i32 %reg_138, ptr %reg_103
	br label %Block30
}

define i32 @main()
{
Block0:  ;Func define at line 59
	br label %Block1
Block1:  ;Func body at line 59
	%reg_0 = add i32 9, 0
	%reg_1 = call i32 @long_array(i32 %reg_0)
	ret i32 %reg_1
}
