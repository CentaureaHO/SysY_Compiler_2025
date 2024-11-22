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

@a = global i32 zeroinitializer
@b = global i32 zeroinitializer
@d = global i32 zeroinitializer

define i32 @set_a(i32 %reg_0)
{
Block0:  ;Func define at line 3
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 3
	%reg_2 = load i32, ptr %reg_1
	store i32 %reg_2, ptr @a
	%reg_3 = load i32, ptr @a
	ret i32 %reg_3
}

define i32 @set_b(i32 %reg_0)
{
Block0:  ;Func define at line 4
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 4
	%reg_2 = load i32, ptr %reg_1
	store i32 %reg_2, ptr @b
	%reg_3 = load i32, ptr @b
	ret i32 %reg_3
}

define i32 @set_d(i32 %reg_0)
{
Block0:  ;Func define at line 5
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 5
	%reg_2 = load i32, ptr %reg_1
	store i32 %reg_2, ptr @d
	%reg_3 = load i32, ptr @d
	ret i32 %reg_3
}

define i32 @main()
{
Block0:  ;Func define at line 7
	%reg_102 = alloca i32
	%reg_100 = alloca i32
	%reg_98 = alloca i32
	%reg_96 = alloca i32
	%reg_94 = alloca i32
	%reg_24 = alloca i32
	br label %Block1
Block1:  ;Func body at line 7
	%reg_0 = add i32 2, 0
	store i32 %reg_0, ptr @a
	%reg_1 = add i32 3, 0
	store i32 %reg_1, ptr @b
	%reg_2 = add i32 0, 0
	%reg_3 = call i32 @set_a(i32 %reg_2)
	%reg_4 = icmp ne i32 %reg_3, 0
	br i1 %reg_4, label %Block4, label %Block3
Block2:  ;If then at line 10
	br label %Block3
Block3:  ;If end at line 10
	%reg_8 = load i32, ptr @a
	call void @putint(i32 %reg_8)
	%reg_9 = add i32 32, 0
	call void @putch(i32 %reg_9)
	%reg_10 = load i32, ptr @b
	call void @putint(i32 %reg_10)
	%reg_11 = add i32 32, 0
	call void @putch(i32 %reg_11)
	%reg_12 = add i32 2, 0
	store i32 %reg_12, ptr @a
	%reg_13 = add i32 3, 0
	store i32 %reg_13, ptr @b
	%reg_14 = add i32 0, 0
	%reg_15 = call i32 @set_a(i32 %reg_14)
	%reg_16 = icmp ne i32 %reg_15, 0
	br i1 %reg_16, label %Block7, label %Block6
Block4:  ;And opertor at line 10
	%reg_5 = add i32 1, 0
	%reg_6 = call i32 @set_b(i32 %reg_5)
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block2, label %Block3
Block5:  ;If then at line 15
	br label %Block6
Block6:  ;If end at line 15
	%reg_20 = load i32, ptr @a
	call void @putint(i32 %reg_20)
	%reg_21 = add i32 32, 0
	call void @putch(i32 %reg_21)
	%reg_22 = load i32, ptr @b
	call void @putint(i32 %reg_22)
	%reg_23 = add i32 10, 0
	call void @putch(i32 %reg_23)
	%reg_25 = add i32 1, 0
	store i32 %reg_25, ptr %reg_24
	%reg_26 = add i32 2, 0
	store i32 %reg_26, ptr @d
	%reg_27 = load i32, ptr %reg_24
	%reg_28 = add i32 1, 0
	%reg_29 = icmp sge i32 %reg_27, %reg_28
	br i1 %reg_29, label %Block10, label %Block9
Block7:  ;And opertor at line 15
	%reg_17 = add i32 1, 0
	%reg_18 = call i32 @set_b(i32 %reg_17)
	%reg_19 = icmp ne i32 %reg_18, 0
	br i1 %reg_19, label %Block5, label %Block6
Block8:  ;If then at line 21
	br label %Block9
Block9:  ;If end at line 21
	%reg_33 = load i32, ptr @d
	call void @putint(i32 %reg_33)
	%reg_34 = add i32 32, 0
	call void @putch(i32 %reg_34)
	%reg_35 = load i32, ptr %reg_24
	%reg_36 = add i32 1, 0
	%reg_37 = icmp sle i32 %reg_35, %reg_36
	br i1 %reg_37, label %Block11, label %Block13
Block10:  ;And opertor at line 21
	%reg_30 = add i32 3, 0
	%reg_31 = call i32 @set_d(i32 %reg_30)
	%reg_32 = icmp ne i32 %reg_31, 0
	br i1 %reg_32, label %Block8, label %Block9
Block11:  ;If then at line 23
	br label %Block12
Block12:  ;If end at line 23
	%reg_41 = load i32, ptr @d
	call void @putint(i32 %reg_41)
	%reg_42 = add i32 10, 0
	call void @putch(i32 %reg_42)
	%reg_43 = add i32 16, 0
	%reg_44 = add i32 3, 0
	%reg_45 = add i32 2, 0
	%reg_46 = add i32 1, 0
	%reg_47 = add i32 %reg_45, %reg_46
	%reg_48 = sub i32 %reg_44, %reg_47
	%reg_49 = icmp sge i32 %reg_43, %reg_48
	br i1 %reg_49, label %Block14, label %Block15
Block13:  ;Or opertor at line 23
	%reg_38 = add i32 4, 0
	%reg_39 = call i32 @set_d(i32 %reg_38)
	%reg_40 = icmp ne i32 %reg_39, 0
	br i1 %reg_40, label %Block11, label %Block12
Block14:  ;If then at line 26
	%reg_50 = add i32 65, 0
	call void @putch(i32 %reg_50)
	br label %Block15
Block15:  ;If end at line 26
	%reg_51 = add i32 25, 0
	%reg_52 = add i32 7, 0
	%reg_53 = sub i32 %reg_51, %reg_52
	%reg_54 = add i32 36, 0
	%reg_55 = add i32 6, 0
	%reg_56 = add i32 3, 0
	%reg_57 = mul i32 %reg_55, %reg_56
	%reg_58 = sub i32 %reg_54, %reg_57
	%reg_59 = icmp ne i32 %reg_53, %reg_58
	br i1 %reg_59, label %Block16, label %Block17
Block16:  ;If then at line 27
	%reg_60 = add i32 66, 0
	call void @putch(i32 %reg_60)
	br label %Block17
Block17:  ;If end at line 27
	%reg_61 = add i32 1, 0
	%reg_62 = add i32 8, 0
	%reg_63 = icmp slt i32 %reg_61, %reg_62
	%reg_64 = zext i1 %reg_63 to i32
	%reg_65 = add i32 7, 0
	%reg_66 = add i32 2, 0
	%reg_67 = srem i32 %reg_65, %reg_66
	%reg_68 = icmp ne i32 %reg_64, %reg_67
	br i1 %reg_68, label %Block18, label %Block19
Block18:  ;If then at line 28
	%reg_69 = add i32 67, 0
	call void @putch(i32 %reg_69)
	br label %Block19
Block19:  ;If end at line 28
	%reg_70 = add i32 3, 0
	%reg_71 = add i32 4, 0
	%reg_72 = icmp sgt i32 %reg_70, %reg_71
	%reg_73 = zext i1 %reg_72 to i32
	%reg_74 = add i32 0, 0
	%reg_75 = icmp eq i32 %reg_73, %reg_74
	br i1 %reg_75, label %Block20, label %Block21
Block20:  ;If then at line 29
	%reg_76 = add i32 68, 0
	call void @putch(i32 %reg_76)
	br label %Block21
Block21:  ;If end at line 29
	%reg_77 = add i32 1, 0
	%reg_78 = add i32 102, 0
	%reg_79 = add i32 63, 0
	%reg_80 = icmp sle i32 %reg_78, %reg_79
	%reg_81 = zext i1 %reg_80 to i32
	%reg_82 = icmp eq i32 %reg_77, %reg_81
	br i1 %reg_82, label %Block22, label %Block23
Block22:  ;If then at line 30
	%reg_83 = add i32 69, 0
	call void @putch(i32 %reg_83)
	br label %Block23
Block23:  ;If end at line 30
	%reg_84 = add i32 5, 0
	%reg_85 = add i32 6, 0
	%reg_86 = sub i32 %reg_84, %reg_85
	%reg_87 = add i32 0, 0
	%reg_88 = icmp eq i32 %reg_87, 0
	%reg_89 = zext i1 %reg_88 to i32
	%reg_90 = sub i32 0, %reg_89
	%reg_91 = icmp eq i32 %reg_86, %reg_90
	br i1 %reg_91, label %Block24, label %Block25
Block24:  ;If then at line 31
	%reg_92 = add i32 70, 0
	call void @putch(i32 %reg_92)
	br label %Block25
Block25:  ;If end at line 31
	%reg_93 = add i32 10, 0
	call void @putch(i32 %reg_93)
	%reg_95 = add i32 0, 0
	store i32 %reg_95, ptr %reg_94
	%reg_97 = add i32 1, 0
	store i32 %reg_97, ptr %reg_96
	%reg_99 = add i32 2, 0
	store i32 %reg_99, ptr %reg_98
	%reg_101 = add i32 3, 0
	store i32 %reg_101, ptr %reg_100
	%reg_103 = add i32 4, 0
	store i32 %reg_103, ptr %reg_102
	br label %Block26
Block26:  ;While condition at line 35
	%reg_104 = load i32, ptr %reg_94
	%reg_105 = icmp ne i32 %reg_104, 0
	br i1 %reg_105, label %Block29, label %Block28
Block27:  ;While body at line 35
	%reg_108 = add i32 32, 0
	call void @putch(i32 %reg_108)
	br label %Block26
Block28:  ;While end at line 35
	%reg_109 = load i32, ptr %reg_94
	%reg_110 = icmp ne i32 %reg_109, 0
	br i1 %reg_110, label %Block30, label %Block32
Block29:  ;And opertor at line 35
	%reg_106 = load i32, ptr %reg_96
	%reg_107 = icmp ne i32 %reg_106, 0
	br i1 %reg_107, label %Block27, label %Block28
Block30:  ;If then at line 36
	%reg_113 = add i32 67, 0
	call void @putch(i32 %reg_113)
	br label %Block31
Block31:  ;If end at line 36
	%reg_114 = load i32, ptr %reg_94
	%reg_115 = load i32, ptr %reg_96
	%reg_116 = icmp sge i32 %reg_114, %reg_115
	br i1 %reg_116, label %Block33, label %Block35
Block32:  ;Or opertor at line 36
	%reg_111 = load i32, ptr %reg_96
	%reg_112 = icmp ne i32 %reg_111, 0
	br i1 %reg_112, label %Block30, label %Block31
Block33:  ;If then at line 37
	%reg_120 = add i32 72, 0
	call void @putch(i32 %reg_120)
	br label %Block34
Block34:  ;If end at line 37
	%reg_121 = load i32, ptr %reg_98
	%reg_122 = load i32, ptr %reg_96
	%reg_123 = icmp sge i32 %reg_121, %reg_122
	br i1 %reg_123, label %Block38, label %Block37
Block35:  ;Or opertor at line 37
	%reg_117 = load i32, ptr %reg_96
	%reg_118 = load i32, ptr %reg_94
	%reg_119 = icmp sle i32 %reg_117, %reg_118
	br i1 %reg_119, label %Block33, label %Block34
Block36:  ;If then at line 38
	%reg_127 = add i32 73, 0
	call void @putch(i32 %reg_127)
	br label %Block37
Block37:  ;If end at line 38
	%reg_128 = load i32, ptr %reg_94
	%reg_129 = load i32, ptr %reg_96
	%reg_130 = icmp eq i32 %reg_129, 0
	%reg_131 = zext i1 %reg_130 to i32
	%reg_132 = icmp eq i32 %reg_128, %reg_131
	br i1 %reg_132, label %Block42, label %Block41
Block38:  ;And opertor at line 38
	%reg_124 = load i32, ptr %reg_102
	%reg_125 = load i32, ptr %reg_100
	%reg_126 = icmp ne i32 %reg_124, %reg_125
	br i1 %reg_126, label %Block36, label %Block37
Block39:  ;If then at line 39
	%reg_139 = add i32 74, 0
	call void @putch(i32 %reg_139)
	br label %Block40
Block40:  ;If end at line 39
	%reg_140 = load i32, ptr %reg_94
	%reg_141 = load i32, ptr %reg_96
	%reg_142 = icmp eq i32 %reg_141, 0
	%reg_143 = zext i1 %reg_142 to i32
	%reg_144 = icmp eq i32 %reg_140, %reg_143
	br i1 %reg_144, label %Block43, label %Block45
Block41:  ;Or opertor at line 39
	%reg_136 = load i32, ptr %reg_102
	%reg_137 = load i32, ptr %reg_102
	%reg_138 = icmp sge i32 %reg_136, %reg_137
	br i1 %reg_138, label %Block39, label %Block40
Block42:  ;And opertor at line -1
	%reg_133 = load i32, ptr %reg_100
	%reg_134 = load i32, ptr %reg_100
	%reg_135 = icmp slt i32 %reg_133, %reg_134
	br i1 %reg_135, label %Block39, label %Block41
Block43:  ;If then at line 40
	%reg_151 = add i32 75, 0
	call void @putch(i32 %reg_151)
	br label %Block44
Block44:  ;If end at line 40
	%reg_152 = add i32 10, 0
	call void @putch(i32 %reg_152)
	%reg_153 = add i32 0, 0
	ret i32 %reg_153
Block45:  ;Or opertor at line 40
	%reg_145 = load i32, ptr %reg_100
	%reg_146 = load i32, ptr %reg_100
	%reg_147 = icmp slt i32 %reg_145, %reg_146
	br i1 %reg_147, label %Block46, label %Block44
Block46:  ;And opertor at line -1
	%reg_148 = load i32, ptr %reg_102
	%reg_149 = load i32, ptr %reg_102
	%reg_150 = icmp sge i32 %reg_148, %reg_149
	br i1 %reg_150, label %Block43, label %Block44
}
