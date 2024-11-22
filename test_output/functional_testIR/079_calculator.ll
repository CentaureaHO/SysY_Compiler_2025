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

@ints = global [10000 x i32] zeroinitializer
@intt = global i32 zeroinitializer
@chas = global [10000 x i32] zeroinitializer
@chat = global i32 zeroinitializer
@i = global i32 0
@ii = global i32 1
@c = global i32 zeroinitializer
@get = global [10000 x i32] zeroinitializer
@get2 = global [10000 x i32] zeroinitializer

define i32 @isdigit(i32 %reg_0)
{
Block0:  ;Func define at line 10
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 10
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 48, 0
	%reg_4 = icmp sge i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block4, label %Block3
Block2:  ;If then at line 11
	%reg_8 = add i32 1, 0
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 11
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
Block4:  ;And opertor at line 11
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 57, 0
	%reg_7 = icmp sle i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
}

define i32 @power(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 16
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 16
	%reg_5 = add i32 1, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 18
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = add i32 0, 0
	%reg_8 = icmp ne i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 18
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = mul i32 %reg_9, %reg_10
	store i32 %reg_11, ptr %reg_4
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = add i32 1, 0
	%reg_14 = sub i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_3
	br label %Block2
Block4:  ;While end at line 18
	%reg_15 = load i32, ptr %reg_4
	ret i32 %reg_15
}

define i32 @getstr(ptr %reg_0)
{
Block0:  ;Func define at line 25
	%reg_3 = alloca i32
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 25
	%reg_2 = call i32 @getch()
	store i32 %reg_2, ptr %reg_1
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 28
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 13, 0
	%reg_7 = icmp ne i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block5, label %Block4
Block3:  ;While body at line 28
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = getelementptr i32, ptr %reg_0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_1
	store i32 %reg_13, ptr %reg_12
	%reg_14 = load i32, ptr %reg_3
	%reg_15 = add i32 1, 0
	%reg_16 = add i32 %reg_14, %reg_15
	store i32 %reg_16, ptr %reg_3
	%reg_17 = call i32 @getch()
	store i32 %reg_17, ptr %reg_1
	br label %Block2
Block4:  ;While end at line 28
	%reg_18 = load i32, ptr %reg_3
	ret i32 %reg_18
Block5:  ;And opertor at line 28
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = add i32 10, 0
	%reg_10 = icmp ne i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
}

define void @intpush(i32 %reg_0)
{
Block0:  ;Func define at line 36
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 36
	%reg_2 = load i32, ptr @intt
	%reg_3 = add i32 1, 0
	%reg_4 = add i32 %reg_2, %reg_3
	store i32 %reg_4, ptr @intt
	%reg_5 = load i32, ptr @intt
	%reg_6 = getelementptr [10000 x i32], ptr @ints, i32 0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_1
	store i32 %reg_7, ptr %reg_6
	ret void
}

define void @chapush(i32 %reg_0)
{
Block0:  ;Func define at line 41
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 41
	%reg_2 = load i32, ptr @chat
	%reg_3 = add i32 1, 0
	%reg_4 = add i32 %reg_2, %reg_3
	store i32 %reg_4, ptr @chat
	%reg_5 = load i32, ptr @chat
	%reg_6 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_1
	store i32 %reg_7, ptr %reg_6
	ret void
}

define i32 @intpop()
{
Block0:  ;Func define at line 46
	br label %Block1
Block1:  ;Func body at line 46
	%reg_0 = load i32, ptr @intt
	%reg_1 = add i32 1, 0
	%reg_2 = sub i32 %reg_0, %reg_1
	store i32 %reg_2, ptr @intt
	%reg_3 = load i32, ptr @intt
	%reg_4 = add i32 1, 0
	%reg_5 = add i32 %reg_3, %reg_4
	%reg_6 = getelementptr [10000 x i32], ptr @ints, i32 0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	ret i32 %reg_7
}

define i32 @chapop()
{
Block0:  ;Func define at line 51
	br label %Block1
Block1:  ;Func body at line 51
	%reg_0 = load i32, ptr @chat
	%reg_1 = add i32 1, 0
	%reg_2 = sub i32 %reg_0, %reg_1
	store i32 %reg_2, ptr @chat
	%reg_3 = load i32, ptr @chat
	%reg_4 = add i32 1, 0
	%reg_5 = add i32 %reg_3, %reg_4
	%reg_6 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	ret i32 %reg_7
}

define void @intadd(i32 %reg_0)
{
Block0:  ;Func define at line 56
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 56
	%reg_2 = load i32, ptr @intt
	%reg_3 = getelementptr [10000 x i32], ptr @ints, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr @intt
	%reg_5 = getelementptr [10000 x i32], ptr @ints, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = add i32 10, 0
	%reg_8 = mul i32 %reg_6, %reg_7
	store i32 %reg_8, ptr %reg_3
	%reg_9 = load i32, ptr @intt
	%reg_10 = getelementptr [10000 x i32], ptr @ints, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr @intt
	%reg_12 = getelementptr [10000 x i32], ptr @ints, i32 0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = load i32, ptr %reg_1
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_10
	ret void
}

define i32 @find()
{
Block0:  ;Func define at line 62
	br label %Block1
Block1:  ;Func body at line 62
	%reg_0 = call i32 @chapop()
	store i32 %reg_0, ptr @c
	%reg_1 = load i32, ptr @ii
	%reg_2 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_1
	%reg_3 = add i32 32, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = load i32, ptr @ii
	%reg_5 = add i32 1, 0
	%reg_6 = add i32 %reg_4, %reg_5
	%reg_7 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_6
	%reg_8 = load i32, ptr @c
	store i32 %reg_8, ptr %reg_7
	%reg_9 = load i32, ptr @ii
	%reg_10 = add i32 2, 0
	%reg_11 = add i32 %reg_9, %reg_10
	store i32 %reg_11, ptr @ii
	%reg_12 = load i32, ptr @chat
	%reg_13 = add i32 0, 0
	%reg_14 = icmp eq i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block2, label %Block3
Block2:  ;If then at line 68
	%reg_15 = add i32 0, 0
	ret i32 %reg_15
	br label %Block3
Block3:  ;If end at line 68
	%reg_16 = add i32 1, 0
	ret i32 %reg_16
}

define i32 @main()
{
Block0:  ;Func define at line 72
	%reg_288 = alloca i32
	%reg_286 = alloca i32
	%reg_284 = alloca i32
	%reg_232 = alloca i32
	%reg_2 = alloca i32
	br label %Block1
Block1:  ;Func body at line 72
	%reg_0 = add i32 0, 0
	store i32 %reg_0, ptr @intt
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr @chat
	%reg_3 = getelementptr [10000 x i32], ptr @get, i32 0
	%reg_4 = call i32 @getstr(ptr %reg_3)
	store i32 %reg_4, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 77
	%reg_5 = load i32, ptr @i
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = icmp slt i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 77
	%reg_8 = load i32, ptr @i
	%reg_9 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = call i32 @isdigit(i32 %reg_10)
	%reg_12 = add i32 1, 0
	%reg_13 = icmp eq i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block5, label %Block7
Block4:  ;While end at line 77
	br label %Block76
Block5:  ;If then at line 79
	%reg_14 = load i32, ptr @ii
	%reg_15 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_14
	%reg_16 = load i32, ptr @i
	%reg_17 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	store i32 %reg_18, ptr %reg_15
	%reg_19 = load i32, ptr @ii
	%reg_20 = add i32 1, 0
	%reg_21 = add i32 %reg_19, %reg_20
	store i32 %reg_21, ptr @ii
	br label %Block6
Block6:  ;If end at line 79
	%reg_226 = load i32, ptr @i
	%reg_227 = add i32 1, 0
	%reg_228 = add i32 %reg_226, %reg_227
	store i32 %reg_228, ptr @i
	br label %Block2
Block7:  ;If else at line 79
	%reg_22 = load i32, ptr @i
	%reg_23 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = add i32 40, 0
	%reg_26 = icmp eq i32 %reg_24, %reg_25
	br i1 %reg_26, label %Block8, label %Block9
Block8:  ;If then at line 86
	%reg_27 = add i32 40, 0
	call void @chapush(i32 %reg_27)
	br label %Block9
Block9:  ;If end at line 86
	%reg_28 = load i32, ptr @i
	%reg_29 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	%reg_31 = add i32 94, 0
	%reg_32 = icmp eq i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block10, label %Block11
Block10:  ;If then at line 87
	%reg_33 = add i32 94, 0
	call void @chapush(i32 %reg_33)
	br label %Block11
Block11:  ;If end at line 87
	%reg_34 = load i32, ptr @i
	%reg_35 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_34
	%reg_36 = load i32, ptr %reg_35
	%reg_37 = add i32 41, 0
	%reg_38 = icmp eq i32 %reg_36, %reg_37
	br i1 %reg_38, label %Block12, label %Block13
Block12:  ;If then at line 88
	%reg_39 = call i32 @chapop()
	store i32 %reg_39, ptr @c
	br label %Block14
Block13:  ;If end at line 88
	%reg_55 = load i32, ptr @i
	%reg_56 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = add i32 43, 0
	%reg_59 = icmp eq i32 %reg_57, %reg_58
	br i1 %reg_59, label %Block17, label %Block18
Block14:  ;While condition at line 91
	%reg_40 = load i32, ptr @c
	%reg_41 = add i32 40, 0
	%reg_42 = icmp ne i32 %reg_40, %reg_41
	br i1 %reg_42, label %Block15, label %Block16
Block15:  ;While body at line 91
	%reg_43 = load i32, ptr @ii
	%reg_44 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_43
	%reg_45 = add i32 32, 0
	store i32 %reg_45, ptr %reg_44
	%reg_46 = load i32, ptr @ii
	%reg_47 = add i32 1, 0
	%reg_48 = add i32 %reg_46, %reg_47
	%reg_49 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr @c
	store i32 %reg_50, ptr %reg_49
	%reg_51 = load i32, ptr @ii
	%reg_52 = add i32 2, 0
	%reg_53 = add i32 %reg_51, %reg_52
	store i32 %reg_53, ptr @ii
	%reg_54 = call i32 @chapop()
	store i32 %reg_54, ptr @c
	br label %Block14
Block16:  ;While end at line 91
	br label %Block13
Block17:  ;If then at line 99
	br label %Block19
Block18:  ;If end at line 99
	%reg_94 = load i32, ptr @i
	%reg_95 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_94
	%reg_96 = load i32, ptr %reg_95
	%reg_97 = add i32 45, 0
	%reg_98 = icmp eq i32 %reg_96, %reg_97
	br i1 %reg_98, label %Block30, label %Block31
Block19:  ;While condition at line 101
	%reg_60 = load i32, ptr @chat
	%reg_61 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_60
	%reg_62 = load i32, ptr %reg_61
	%reg_63 = add i32 43, 0
	%reg_64 = icmp eq i32 %reg_62, %reg_63
	br i1 %reg_64, label %Block20, label %Block26
Block20:  ;While body at line 101
	%reg_90 = call i32 @find()
	%reg_91 = add i32 0, 0
	%reg_92 = icmp eq i32 %reg_90, %reg_91
	br i1 %reg_92, label %Block27, label %Block28
Block21:  ;While end at line 101
	%reg_93 = add i32 43, 0
	call void @chapush(i32 %reg_93)
	br label %Block18
Block22:  ;Or opertor at line 101
	%reg_85 = load i32, ptr @chat
	%reg_86 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_85
	%reg_87 = load i32, ptr %reg_86
	%reg_88 = add i32 94, 0
	%reg_89 = icmp eq i32 %reg_87, %reg_88
	br i1 %reg_89, label %Block20, label %Block21
Block23:  ;Or opertor at line -1
	%reg_80 = load i32, ptr @chat
	%reg_81 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_80
	%reg_82 = load i32, ptr %reg_81
	%reg_83 = add i32 37, 0
	%reg_84 = icmp eq i32 %reg_82, %reg_83
	br i1 %reg_84, label %Block20, label %Block22
Block24:  ;Or opertor at line -1
	%reg_75 = load i32, ptr @chat
	%reg_76 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_75
	%reg_77 = load i32, ptr %reg_76
	%reg_78 = add i32 47, 0
	%reg_79 = icmp eq i32 %reg_77, %reg_78
	br i1 %reg_79, label %Block20, label %Block23
Block25:  ;Or opertor at line -1
	%reg_70 = load i32, ptr @chat
	%reg_71 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_70
	%reg_72 = load i32, ptr %reg_71
	%reg_73 = add i32 42, 0
	%reg_74 = icmp eq i32 %reg_72, %reg_73
	br i1 %reg_74, label %Block20, label %Block24
Block26:  ;Or opertor at line -1
	%reg_65 = load i32, ptr @chat
	%reg_66 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_65
	%reg_67 = load i32, ptr %reg_66
	%reg_68 = add i32 45, 0
	%reg_69 = icmp eq i32 %reg_67, %reg_68
	br i1 %reg_69, label %Block20, label %Block25
Block27:  ;If then at line 103
	br label %Block21
Block28:  ;If end at line 103
	br label %Block19
Block29:  ;Break at line 103
	br label %Block28
Block30:  ;If then at line 107
	br label %Block32
Block31:  ;If end at line 107
	%reg_133 = load i32, ptr @i
	%reg_134 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_133
	%reg_135 = load i32, ptr %reg_134
	%reg_136 = add i32 42, 0
	%reg_137 = icmp eq i32 %reg_135, %reg_136
	br i1 %reg_137, label %Block43, label %Block44
Block32:  ;While condition at line 109
	%reg_99 = load i32, ptr @chat
	%reg_100 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_99
	%reg_101 = load i32, ptr %reg_100
	%reg_102 = add i32 43, 0
	%reg_103 = icmp eq i32 %reg_101, %reg_102
	br i1 %reg_103, label %Block33, label %Block39
Block33:  ;While body at line 109
	%reg_129 = call i32 @find()
	%reg_130 = add i32 0, 0
	%reg_131 = icmp eq i32 %reg_129, %reg_130
	br i1 %reg_131, label %Block40, label %Block41
Block34:  ;While end at line 109
	%reg_132 = add i32 45, 0
	call void @chapush(i32 %reg_132)
	br label %Block31
Block35:  ;Or opertor at line 109
	%reg_124 = load i32, ptr @chat
	%reg_125 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_124
	%reg_126 = load i32, ptr %reg_125
	%reg_127 = add i32 94, 0
	%reg_128 = icmp eq i32 %reg_126, %reg_127
	br i1 %reg_128, label %Block33, label %Block34
Block36:  ;Or opertor at line -1
	%reg_119 = load i32, ptr @chat
	%reg_120 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_119
	%reg_121 = load i32, ptr %reg_120
	%reg_122 = add i32 37, 0
	%reg_123 = icmp eq i32 %reg_121, %reg_122
	br i1 %reg_123, label %Block33, label %Block35
Block37:  ;Or opertor at line -1
	%reg_114 = load i32, ptr @chat
	%reg_115 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_114
	%reg_116 = load i32, ptr %reg_115
	%reg_117 = add i32 47, 0
	%reg_118 = icmp eq i32 %reg_116, %reg_117
	br i1 %reg_118, label %Block33, label %Block36
Block38:  ;Or opertor at line -1
	%reg_109 = load i32, ptr @chat
	%reg_110 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_109
	%reg_111 = load i32, ptr %reg_110
	%reg_112 = add i32 42, 0
	%reg_113 = icmp eq i32 %reg_111, %reg_112
	br i1 %reg_113, label %Block33, label %Block37
Block39:  ;Or opertor at line -1
	%reg_104 = load i32, ptr @chat
	%reg_105 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_104
	%reg_106 = load i32, ptr %reg_105
	%reg_107 = add i32 45, 0
	%reg_108 = icmp eq i32 %reg_106, %reg_107
	br i1 %reg_108, label %Block33, label %Block38
Block40:  ;If then at line 111
	br label %Block34
Block41:  ;If end at line 111
	br label %Block32
Block42:  ;Break at line 111
	br label %Block41
Block43:  ;If then at line 115
	br label %Block45
Block44:  ;If end at line 115
	%reg_162 = load i32, ptr @i
	%reg_163 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_162
	%reg_164 = load i32, ptr %reg_163
	%reg_165 = add i32 47, 0
	%reg_166 = icmp eq i32 %reg_164, %reg_165
	br i1 %reg_166, label %Block54, label %Block55
Block45:  ;While condition at line 117
	%reg_138 = load i32, ptr @chat
	%reg_139 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_138
	%reg_140 = load i32, ptr %reg_139
	%reg_141 = add i32 42, 0
	%reg_142 = icmp eq i32 %reg_140, %reg_141
	br i1 %reg_142, label %Block46, label %Block50
Block46:  ;While body at line 117
	%reg_158 = call i32 @find()
	%reg_159 = add i32 0, 0
	%reg_160 = icmp eq i32 %reg_158, %reg_159
	br i1 %reg_160, label %Block51, label %Block52
Block47:  ;While end at line 117
	%reg_161 = add i32 42, 0
	call void @chapush(i32 %reg_161)
	br label %Block44
Block48:  ;Or opertor at line 117
	%reg_153 = load i32, ptr @chat
	%reg_154 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_153
	%reg_155 = load i32, ptr %reg_154
	%reg_156 = add i32 94, 0
	%reg_157 = icmp eq i32 %reg_155, %reg_156
	br i1 %reg_157, label %Block46, label %Block47
Block49:  ;Or opertor at line -1
	%reg_148 = load i32, ptr @chat
	%reg_149 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_148
	%reg_150 = load i32, ptr %reg_149
	%reg_151 = add i32 37, 0
	%reg_152 = icmp eq i32 %reg_150, %reg_151
	br i1 %reg_152, label %Block46, label %Block48
Block50:  ;Or opertor at line -1
	%reg_143 = load i32, ptr @chat
	%reg_144 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_143
	%reg_145 = load i32, ptr %reg_144
	%reg_146 = add i32 47, 0
	%reg_147 = icmp eq i32 %reg_145, %reg_146
	br i1 %reg_147, label %Block46, label %Block49
Block51:  ;If then at line 119
	br label %Block47
Block52:  ;If end at line 119
	br label %Block45
Block53:  ;Break at line 119
	br label %Block52
Block54:  ;If then at line 123
	br label %Block56
Block55:  ;If end at line 123
	%reg_191 = load i32, ptr @i
	%reg_192 = getelementptr [10000 x i32], ptr @get, i32 0, i32 %reg_191
	%reg_193 = load i32, ptr %reg_192
	%reg_194 = add i32 37, 0
	%reg_195 = icmp eq i32 %reg_193, %reg_194
	br i1 %reg_195, label %Block65, label %Block66
Block56:  ;While condition at line 125
	%reg_167 = load i32, ptr @chat
	%reg_168 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_167
	%reg_169 = load i32, ptr %reg_168
	%reg_170 = add i32 42, 0
	%reg_171 = icmp eq i32 %reg_169, %reg_170
	br i1 %reg_171, label %Block57, label %Block61
Block57:  ;While body at line 125
	%reg_187 = call i32 @find()
	%reg_188 = add i32 0, 0
	%reg_189 = icmp eq i32 %reg_187, %reg_188
	br i1 %reg_189, label %Block62, label %Block63
Block58:  ;While end at line 125
	%reg_190 = add i32 47, 0
	call void @chapush(i32 %reg_190)
	br label %Block55
Block59:  ;Or opertor at line 125
	%reg_182 = load i32, ptr @chat
	%reg_183 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_182
	%reg_184 = load i32, ptr %reg_183
	%reg_185 = add i32 94, 0
	%reg_186 = icmp eq i32 %reg_184, %reg_185
	br i1 %reg_186, label %Block57, label %Block58
Block60:  ;Or opertor at line -1
	%reg_177 = load i32, ptr @chat
	%reg_178 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_177
	%reg_179 = load i32, ptr %reg_178
	%reg_180 = add i32 37, 0
	%reg_181 = icmp eq i32 %reg_179, %reg_180
	br i1 %reg_181, label %Block57, label %Block59
Block61:  ;Or opertor at line -1
	%reg_172 = load i32, ptr @chat
	%reg_173 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_172
	%reg_174 = load i32, ptr %reg_173
	%reg_175 = add i32 47, 0
	%reg_176 = icmp eq i32 %reg_174, %reg_175
	br i1 %reg_176, label %Block57, label %Block60
Block62:  ;If then at line 127
	br label %Block58
Block63:  ;If end at line 127
	br label %Block56
Block64:  ;Break at line 127
	br label %Block63
Block65:  ;If then at line 131
	br label %Block67
Block66:  ;If end at line 131
	%reg_220 = load i32, ptr @ii
	%reg_221 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_220
	%reg_222 = add i32 32, 0
	store i32 %reg_222, ptr %reg_221
	%reg_223 = load i32, ptr @ii
	%reg_224 = add i32 1, 0
	%reg_225 = add i32 %reg_223, %reg_224
	store i32 %reg_225, ptr @ii
	br label %Block6
Block67:  ;While condition at line 133
	%reg_196 = load i32, ptr @chat
	%reg_197 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_196
	%reg_198 = load i32, ptr %reg_197
	%reg_199 = add i32 42, 0
	%reg_200 = icmp eq i32 %reg_198, %reg_199
	br i1 %reg_200, label %Block68, label %Block72
Block68:  ;While body at line 133
	%reg_216 = call i32 @find()
	%reg_217 = add i32 0, 0
	%reg_218 = icmp eq i32 %reg_216, %reg_217
	br i1 %reg_218, label %Block73, label %Block74
Block69:  ;While end at line 133
	%reg_219 = add i32 37, 0
	call void @chapush(i32 %reg_219)
	br label %Block66
Block70:  ;Or opertor at line 133
	%reg_211 = load i32, ptr @chat
	%reg_212 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_211
	%reg_213 = load i32, ptr %reg_212
	%reg_214 = add i32 94, 0
	%reg_215 = icmp eq i32 %reg_213, %reg_214
	br i1 %reg_215, label %Block68, label %Block69
Block71:  ;Or opertor at line -1
	%reg_206 = load i32, ptr @chat
	%reg_207 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_206
	%reg_208 = load i32, ptr %reg_207
	%reg_209 = add i32 37, 0
	%reg_210 = icmp eq i32 %reg_208, %reg_209
	br i1 %reg_210, label %Block68, label %Block70
Block72:  ;Or opertor at line -1
	%reg_201 = load i32, ptr @chat
	%reg_202 = getelementptr [10000 x i32], ptr @chas, i32 0, i32 %reg_201
	%reg_203 = load i32, ptr %reg_202
	%reg_204 = add i32 47, 0
	%reg_205 = icmp eq i32 %reg_203, %reg_204
	br i1 %reg_205, label %Block68, label %Block71
Block73:  ;If then at line 135
	br label %Block69
Block74:  ;If end at line 135
	br label %Block67
Block75:  ;Break at line 135
	br label %Block74
Block76:  ;While condition at line 144
	%reg_229 = load i32, ptr @chat
	%reg_230 = add i32 0, 0
	%reg_231 = icmp sgt i32 %reg_229, %reg_230
	br i1 %reg_231, label %Block77, label %Block78
Block77:  ;While body at line 144
	%reg_233 = call i32 @chapop()
	store i32 %reg_233, ptr %reg_232
	%reg_234 = load i32, ptr @ii
	%reg_235 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_234
	%reg_236 = add i32 32, 0
	store i32 %reg_236, ptr %reg_235
	%reg_237 = load i32, ptr @ii
	%reg_238 = add i32 1, 0
	%reg_239 = add i32 %reg_237, %reg_238
	%reg_240 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_239
	%reg_241 = load i32, ptr %reg_232
	store i32 %reg_241, ptr %reg_240
	%reg_242 = load i32, ptr @ii
	%reg_243 = add i32 2, 0
	%reg_244 = add i32 %reg_242, %reg_243
	store i32 %reg_244, ptr @ii
	br label %Block76
Block78:  ;While end at line 144
	%reg_245 = load i32, ptr @ii
	%reg_246 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_245
	%reg_247 = add i32 64, 0
	store i32 %reg_247, ptr %reg_246
	%reg_248 = add i32 1, 0
	store i32 %reg_248, ptr @i
	br label %Block79
Block79:  ;While condition at line 153
	%reg_249 = load i32, ptr @i
	%reg_250 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_249
	%reg_251 = load i32, ptr %reg_250
	%reg_252 = add i32 64, 0
	%reg_253 = icmp ne i32 %reg_251, %reg_252
	br i1 %reg_253, label %Block80, label %Block81
Block80:  ;While body at line 153
	%reg_254 = load i32, ptr @i
	%reg_255 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_254
	%reg_256 = load i32, ptr %reg_255
	%reg_257 = add i32 43, 0
	%reg_258 = icmp eq i32 %reg_256, %reg_257
	br i1 %reg_258, label %Block82, label %Block89
Block81:  ;While end at line 153
	%reg_375 = add i32 1, 0
	%reg_376 = getelementptr [10000 x i32], ptr @ints, i32 0, i32 %reg_375
	%reg_377 = load i32, ptr %reg_376
	call void @putint(i32 %reg_377)
	%reg_378 = add i32 0, 0
	ret i32 %reg_378
Block82:  ;If then at line 155
	%reg_285 = call i32 @intpop()
	store i32 %reg_285, ptr %reg_284
	%reg_287 = call i32 @intpop()
	store i32 %reg_287, ptr %reg_286
	%reg_289 = add i32 0, 0
	store i32 %reg_289, ptr %reg_288
	%reg_290 = load i32, ptr @i
	%reg_291 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_290
	%reg_292 = load i32, ptr %reg_291
	%reg_293 = add i32 43, 0
	%reg_294 = icmp eq i32 %reg_292, %reg_293
	br i1 %reg_294, label %Block90, label %Block91
Block83:  ;If end at line 155
	%reg_372 = load i32, ptr @i
	%reg_373 = add i32 1, 0
	%reg_374 = add i32 %reg_372, %reg_373
	store i32 %reg_374, ptr @i
	br label %Block79
Block84:  ;If else at line 155
	%reg_339 = load i32, ptr @i
	%reg_340 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_339
	%reg_341 = load i32, ptr %reg_340
	%reg_342 = add i32 32, 0
	%reg_343 = icmp ne i32 %reg_341, %reg_342
	br i1 %reg_343, label %Block102, label %Block103
Block85:  ;Or opertor at line 155
	%reg_279 = load i32, ptr @i
	%reg_280 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_279
	%reg_281 = load i32, ptr %reg_280
	%reg_282 = add i32 94, 0
	%reg_283 = icmp eq i32 %reg_281, %reg_282
	br i1 %reg_283, label %Block82, label %Block84
Block86:  ;Or opertor at line -1
	%reg_274 = load i32, ptr @i
	%reg_275 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_274
	%reg_276 = load i32, ptr %reg_275
	%reg_277 = add i32 37, 0
	%reg_278 = icmp eq i32 %reg_276, %reg_277
	br i1 %reg_278, label %Block82, label %Block85
Block87:  ;Or opertor at line -1
	%reg_269 = load i32, ptr @i
	%reg_270 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_269
	%reg_271 = load i32, ptr %reg_270
	%reg_272 = add i32 47, 0
	%reg_273 = icmp eq i32 %reg_271, %reg_272
	br i1 %reg_273, label %Block82, label %Block86
Block88:  ;Or opertor at line -1
	%reg_264 = load i32, ptr @i
	%reg_265 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_264
	%reg_266 = load i32, ptr %reg_265
	%reg_267 = add i32 42, 0
	%reg_268 = icmp eq i32 %reg_266, %reg_267
	br i1 %reg_268, label %Block82, label %Block87
Block89:  ;Or opertor at line -1
	%reg_259 = load i32, ptr @i
	%reg_260 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_259
	%reg_261 = load i32, ptr %reg_260
	%reg_262 = add i32 45, 0
	%reg_263 = icmp eq i32 %reg_261, %reg_262
	br i1 %reg_263, label %Block82, label %Block88
Block90:  ;If then at line 158
	%reg_295 = load i32, ptr %reg_284
	%reg_296 = load i32, ptr %reg_286
	%reg_297 = add i32 %reg_295, %reg_296
	store i32 %reg_297, ptr %reg_288
	br label %Block91
Block91:  ;If end at line 158
	%reg_298 = load i32, ptr @i
	%reg_299 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_298
	%reg_300 = load i32, ptr %reg_299
	%reg_301 = add i32 45, 0
	%reg_302 = icmp eq i32 %reg_300, %reg_301
	br i1 %reg_302, label %Block92, label %Block93
Block92:  ;If then at line 159
	%reg_303 = load i32, ptr %reg_286
	%reg_304 = load i32, ptr %reg_284
	%reg_305 = sub i32 %reg_303, %reg_304
	store i32 %reg_305, ptr %reg_288
	br label %Block93
Block93:  ;If end at line 159
	%reg_306 = load i32, ptr @i
	%reg_307 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_306
	%reg_308 = load i32, ptr %reg_307
	%reg_309 = add i32 42, 0
	%reg_310 = icmp eq i32 %reg_308, %reg_309
	br i1 %reg_310, label %Block94, label %Block95
Block94:  ;If then at line 160
	%reg_311 = load i32, ptr %reg_284
	%reg_312 = load i32, ptr %reg_286
	%reg_313 = mul i32 %reg_311, %reg_312
	store i32 %reg_313, ptr %reg_288
	br label %Block95
Block95:  ;If end at line 160
	%reg_314 = load i32, ptr @i
	%reg_315 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_314
	%reg_316 = load i32, ptr %reg_315
	%reg_317 = add i32 47, 0
	%reg_318 = icmp eq i32 %reg_316, %reg_317
	br i1 %reg_318, label %Block96, label %Block97
Block96:  ;If then at line 161
	%reg_319 = load i32, ptr %reg_286
	%reg_320 = load i32, ptr %reg_284
	%reg_321 = sdiv i32 %reg_319, %reg_320
	store i32 %reg_321, ptr %reg_288
	br label %Block97
Block97:  ;If end at line 161
	%reg_322 = load i32, ptr @i
	%reg_323 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_322
	%reg_324 = load i32, ptr %reg_323
	%reg_325 = add i32 37, 0
	%reg_326 = icmp eq i32 %reg_324, %reg_325
	br i1 %reg_326, label %Block98, label %Block99
Block98:  ;If then at line 162
	%reg_327 = load i32, ptr %reg_286
	%reg_328 = load i32, ptr %reg_284
	%reg_329 = srem i32 %reg_327, %reg_328
	store i32 %reg_329, ptr %reg_288
	br label %Block99
Block99:  ;If end at line 162
	%reg_330 = load i32, ptr @i
	%reg_331 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_330
	%reg_332 = load i32, ptr %reg_331
	%reg_333 = add i32 94, 0
	%reg_334 = icmp eq i32 %reg_332, %reg_333
	br i1 %reg_334, label %Block100, label %Block101
Block100:  ;If then at line 163
	%reg_335 = load i32, ptr %reg_286
	%reg_336 = load i32, ptr %reg_284
	%reg_337 = call i32 @power(i32 %reg_335, i32 %reg_336)
	store i32 %reg_337, ptr %reg_288
	br label %Block101
Block101:  ;If end at line 163
	%reg_338 = load i32, ptr %reg_288
	call void @intpush(i32 %reg_338)
	br label %Block83
Block102:  ;If then at line 168
	%reg_344 = load i32, ptr @i
	%reg_345 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_344
	%reg_346 = load i32, ptr %reg_345
	%reg_347 = add i32 48, 0
	%reg_348 = sub i32 %reg_346, %reg_347
	call void @intpush(i32 %reg_348)
	%reg_349 = add i32 1, 0
	store i32 %reg_349, ptr @ii
	br label %Block104
Block103:  ;If end at line 168
	br label %Block83
Block104:  ;While condition at line 172
	%reg_350 = load i32, ptr @i
	%reg_351 = load i32, ptr @ii
	%reg_352 = add i32 %reg_350, %reg_351
	%reg_353 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_352
	%reg_354 = load i32, ptr %reg_353
	%reg_355 = add i32 32, 0
	%reg_356 = icmp ne i32 %reg_354, %reg_355
	br i1 %reg_356, label %Block105, label %Block106
Block105:  ;While body at line 172
	%reg_357 = load i32, ptr @i
	%reg_358 = load i32, ptr @ii
	%reg_359 = add i32 %reg_357, %reg_358
	%reg_360 = getelementptr [10000 x i32], ptr @get2, i32 0, i32 %reg_359
	%reg_361 = load i32, ptr %reg_360
	%reg_362 = add i32 48, 0
	%reg_363 = sub i32 %reg_361, %reg_362
	call void @intadd(i32 %reg_363)
	%reg_364 = load i32, ptr @ii
	%reg_365 = add i32 1, 0
	%reg_366 = add i32 %reg_364, %reg_365
	store i32 %reg_366, ptr @ii
	br label %Block104
Block106:  ;While end at line 172
	%reg_367 = load i32, ptr @i
	%reg_368 = load i32, ptr @ii
	%reg_369 = add i32 %reg_367, %reg_368
	%reg_370 = add i32 1, 0
	%reg_371 = sub i32 %reg_369, %reg_370
	store i32 %reg_371, ptr @i
	br label %Block103
}
