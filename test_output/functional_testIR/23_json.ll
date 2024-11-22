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

@pos = global i32 0
@buffer = global [50000000 x i32] zeroinitializer

define i32 @is_number(i32 %reg_0)
{
Block0:  ;Func define at line 16
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 16
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 48, 0
	%reg_4 = icmp sge i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block4
Block2:  ;If then at line 17
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 57, 0
	%reg_7 = icmp sle i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block5, label %Block7
Block3:  ;If end at line 17
	ret i32 0
Block4:  ;If else at line 17
	%reg_10 = add i32 0, 0
	ret i32 %reg_10
	br label %Block3
Block5:  ;If then at line 18
	%reg_8 = add i32 1, 0
	ret i32 %reg_8
	br label %Block6
Block6:  ;If end at line 18
	br label %Block3
Block7:  ;If else at line 18
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
	br label %Block6
}

define void @skip_space(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 28
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 28
	br label %Block2
Block2:  ;While condition at line 29
	%reg_3 = add i32 1, 0
	%reg_4 = icmp ne i32 %reg_3, 0
	br i1 %reg_4, label %Block3, label %Block4
Block3:  ;While body at line 29
	%reg_5 = load i32, ptr @pos
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = icmp sge i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block5, label %Block6
Block4:  ;While end at line 29
	ret void
Block5:  ;If then at line 30
	br label %Block4
Block6:  ;If end at line 30
	%reg_8 = load i32, ptr @pos
	%reg_9 = getelementptr i32, ptr %reg_0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = add i32 32, 0
	%reg_12 = icmp eq i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block8, label %Block10
Block7:  ;Break at line 31
	br label %Block6
Block8:  ;If then at line 33
	%reg_13 = load i32, ptr @pos
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr @pos
	br label %Block9
Block9:  ;If end at line 33
	br label %Block2
Block10:  ;If else at line 33
	%reg_16 = load i32, ptr @pos
	%reg_17 = getelementptr i32, ptr %reg_0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = add i32 9, 0
	%reg_20 = icmp eq i32 %reg_18, %reg_19
	br i1 %reg_20, label %Block11, label %Block13
Block11:  ;If then at line 35
	%reg_21 = load i32, ptr @pos
	%reg_22 = add i32 1, 0
	%reg_23 = add i32 %reg_21, %reg_22
	store i32 %reg_23, ptr @pos
	br label %Block12
Block12:  ;If end at line 35
	br label %Block9
Block13:  ;If else at line 35
	%reg_24 = load i32, ptr @pos
	%reg_25 = getelementptr i32, ptr %reg_0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = add i32 10, 0
	%reg_28 = icmp eq i32 %reg_26, %reg_27
	br i1 %reg_28, label %Block14, label %Block16
Block14:  ;If then at line 37
	%reg_29 = load i32, ptr @pos
	%reg_30 = add i32 1, 0
	%reg_31 = add i32 %reg_29, %reg_30
	store i32 %reg_31, ptr @pos
	br label %Block15
Block15:  ;If end at line 37
	br label %Block12
Block16:  ;If else at line 37
	%reg_32 = load i32, ptr @pos
	%reg_33 = getelementptr i32, ptr %reg_0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = add i32 13, 0
	%reg_36 = icmp eq i32 %reg_34, %reg_35
	br i1 %reg_36, label %Block17, label %Block19
Block17:  ;If then at line 39
	%reg_37 = load i32, ptr @pos
	%reg_38 = add i32 1, 0
	%reg_39 = add i32 %reg_37, %reg_38
	store i32 %reg_39, ptr @pos
	br label %Block18
Block18:  ;If end at line 39
	br label %Block15
Block19:  ;If else at line 39
	br label %Block4
Block20:  ;Break at line 42
	br label %Block18
}

define i32 @detect_item(i32 %reg_0, ptr %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 47
	%reg_552 = alloca [4 x i32]
	%reg_481 = alloca [5 x i32]
	%reg_422 = alloca [4 x i32]
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_2, ptr %reg_4
	br label %Block1
Block1:  ;Func body at line 47
	%reg_5 = load i32, ptr @pos
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = icmp sge i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
Block2:  ;If then at line 48
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 48
	%reg_9 = getelementptr i32, ptr %reg_1
	%reg_10 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_9, i32 %reg_10)
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = add i32 0, 0
	%reg_13 = icmp eq i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block4, label %Block6
Block4:  ;If then at line 53
	%reg_14 = load i32, ptr @pos
	%reg_15 = getelementptr i32, ptr %reg_1, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = add i32 123, 0
	%reg_18 = icmp eq i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block7, label %Block9
Block5:  ;If end at line 53
	%reg_609 = add i32 1, 0
	ret i32 %reg_609
Block6:  ;If else at line 53
	%reg_97 = load i32, ptr %reg_3
	%reg_98 = add i32 1, 0
	%reg_99 = icmp eq i32 %reg_97, %reg_98
	br i1 %reg_99, label %Block34, label %Block36
Block7:  ;If then at line 54
	%reg_19 = add i32 4, 0
	%reg_20 = getelementptr i32, ptr %reg_1
	%reg_21 = load i32, ptr %reg_4
	%reg_22 = call i32 @detect_item(i32 %reg_19, ptr %reg_20, i32 %reg_21)
	ret i32 %reg_22
	br label %Block8
Block8:  ;If end at line 54
	br label %Block5
Block9:  ;If else at line 54
	%reg_23 = load i32, ptr @pos
	%reg_24 = getelementptr i32, ptr %reg_1, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = add i32 91, 0
	%reg_27 = icmp eq i32 %reg_25, %reg_26
	br i1 %reg_27, label %Block10, label %Block12
Block10:  ;If then at line 56
	%reg_28 = add i32 3, 0
	%reg_29 = getelementptr i32, ptr %reg_1
	%reg_30 = load i32, ptr %reg_4
	%reg_31 = call i32 @detect_item(i32 %reg_28, ptr %reg_29, i32 %reg_30)
	ret i32 %reg_31
	br label %Block11
Block11:  ;If end at line 56
	br label %Block8
Block12:  ;If else at line 56
	%reg_32 = load i32, ptr @pos
	%reg_33 = getelementptr i32, ptr %reg_1, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = add i32 34, 0
	%reg_36 = icmp eq i32 %reg_34, %reg_35
	br i1 %reg_36, label %Block13, label %Block15
Block13:  ;If then at line 58
	%reg_37 = add i32 2, 0
	%reg_38 = getelementptr i32, ptr %reg_1
	%reg_39 = load i32, ptr %reg_4
	%reg_40 = call i32 @detect_item(i32 %reg_37, ptr %reg_38, i32 %reg_39)
	ret i32 %reg_40
	br label %Block14
Block14:  ;If end at line 58
	br label %Block11
Block15:  ;If else at line 58
	%reg_41 = load i32, ptr @pos
	%reg_42 = getelementptr i32, ptr %reg_1, i32 %reg_41
	%reg_43 = load i32, ptr %reg_42
	%reg_44 = call i32 @is_number(i32 %reg_43)
	%reg_45 = add i32 1, 0
	%reg_46 = icmp eq i32 %reg_44, %reg_45
	br i1 %reg_46, label %Block16, label %Block18
Block16:  ;If then at line 60
	%reg_47 = add i32 1, 0
	%reg_48 = getelementptr i32, ptr %reg_1
	%reg_49 = load i32, ptr %reg_4
	%reg_50 = call i32 @detect_item(i32 %reg_47, ptr %reg_48, i32 %reg_49)
	ret i32 %reg_50
	br label %Block17
Block17:  ;If end at line 60
	br label %Block14
Block18:  ;If else at line 60
	%reg_51 = load i32, ptr @pos
	%reg_52 = getelementptr i32, ptr %reg_1, i32 %reg_51
	%reg_53 = load i32, ptr %reg_52
	%reg_54 = add i32 43, 0
	%reg_55 = icmp eq i32 %reg_53, %reg_54
	br i1 %reg_55, label %Block19, label %Block21
Block19:  ;If then at line 63
	%reg_56 = add i32 1, 0
	%reg_57 = getelementptr i32, ptr %reg_1
	%reg_58 = load i32, ptr %reg_4
	%reg_59 = call i32 @detect_item(i32 %reg_56, ptr %reg_57, i32 %reg_58)
	ret i32 %reg_59
	br label %Block20
Block20:  ;If end at line 63
	br label %Block17
Block21:  ;If else at line 63
	%reg_60 = load i32, ptr @pos
	%reg_61 = getelementptr i32, ptr %reg_1, i32 %reg_60
	%reg_62 = load i32, ptr %reg_61
	%reg_63 = add i32 45, 0
	%reg_64 = icmp eq i32 %reg_62, %reg_63
	br i1 %reg_64, label %Block22, label %Block24
Block22:  ;If then at line 66
	%reg_65 = add i32 1, 0
	%reg_66 = getelementptr i32, ptr %reg_1
	%reg_67 = load i32, ptr %reg_4
	%reg_68 = call i32 @detect_item(i32 %reg_65, ptr %reg_66, i32 %reg_67)
	ret i32 %reg_68
	br label %Block23
Block23:  ;If end at line 66
	br label %Block20
Block24:  ;If else at line 66
	%reg_69 = load i32, ptr @pos
	%reg_70 = getelementptr i32, ptr %reg_1, i32 %reg_69
	%reg_71 = load i32, ptr %reg_70
	%reg_72 = add i32 116, 0
	%reg_73 = icmp eq i32 %reg_71, %reg_72
	br i1 %reg_73, label %Block25, label %Block27
Block25:  ;If then at line 69
	%reg_74 = add i32 5, 0
	%reg_75 = getelementptr i32, ptr %reg_1
	%reg_76 = load i32, ptr %reg_4
	%reg_77 = call i32 @detect_item(i32 %reg_74, ptr %reg_75, i32 %reg_76)
	ret i32 %reg_77
	br label %Block26
Block26:  ;If end at line 69
	br label %Block23
Block27:  ;If else at line 69
	%reg_78 = load i32, ptr @pos
	%reg_79 = getelementptr i32, ptr %reg_1, i32 %reg_78
	%reg_80 = load i32, ptr %reg_79
	%reg_81 = add i32 102, 0
	%reg_82 = icmp eq i32 %reg_80, %reg_81
	br i1 %reg_82, label %Block28, label %Block30
Block28:  ;If then at line 72
	%reg_83 = add i32 6, 0
	%reg_84 = getelementptr i32, ptr %reg_1
	%reg_85 = load i32, ptr %reg_4
	%reg_86 = call i32 @detect_item(i32 %reg_83, ptr %reg_84, i32 %reg_85)
	ret i32 %reg_86
	br label %Block29
Block29:  ;If end at line 72
	br label %Block26
Block30:  ;If else at line 72
	%reg_87 = load i32, ptr @pos
	%reg_88 = getelementptr i32, ptr %reg_1, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	%reg_90 = add i32 110, 0
	%reg_91 = icmp eq i32 %reg_89, %reg_90
	br i1 %reg_91, label %Block31, label %Block33
Block31:  ;If then at line 75
	%reg_92 = add i32 7, 0
	%reg_93 = getelementptr i32, ptr %reg_1
	%reg_94 = load i32, ptr %reg_4
	%reg_95 = call i32 @detect_item(i32 %reg_92, ptr %reg_93, i32 %reg_94)
	ret i32 %reg_95
	br label %Block32
Block32:  ;If end at line 75
	br label %Block29
Block33:  ;If else at line 75
	%reg_96 = add i32 0, 0
	ret i32 %reg_96
	br label %Block32
Block34:  ;If then at line 81
	%reg_100 = load i32, ptr @pos
	%reg_101 = getelementptr i32, ptr %reg_1, i32 %reg_100
	%reg_102 = load i32, ptr %reg_101
	%reg_103 = add i32 43, 0
	%reg_104 = icmp eq i32 %reg_102, %reg_103
	br i1 %reg_104, label %Block37, label %Block39
Block35:  ;If end at line 81
	br label %Block5
Block36:  ;If else at line 81
	%reg_207 = load i32, ptr %reg_3
	%reg_208 = add i32 2, 0
	%reg_209 = icmp eq i32 %reg_207, %reg_208
	br i1 %reg_209, label %Block81, label %Block83
Block37:  ;If then at line 83
	%reg_105 = load i32, ptr @pos
	%reg_106 = add i32 1, 0
	%reg_107 = add i32 %reg_105, %reg_106
	store i32 %reg_107, ptr @pos
	br label %Block38
Block38:  ;If end at line 83
	%reg_116 = load i32, ptr @pos
	%reg_117 = load i32, ptr %reg_4
	%reg_118 = icmp sge i32 %reg_116, %reg_117
	br i1 %reg_118, label %Block42, label %Block44
Block39:  ;If else at line 83
	%reg_108 = load i32, ptr @pos
	%reg_109 = getelementptr i32, ptr %reg_1, i32 %reg_108
	%reg_110 = load i32, ptr %reg_109
	%reg_111 = add i32 45, 0
	%reg_112 = icmp eq i32 %reg_110, %reg_111
	br i1 %reg_112, label %Block40, label %Block41
Block40:  ;If then at line 88
	%reg_113 = load i32, ptr @pos
	%reg_114 = add i32 1, 0
	%reg_115 = add i32 %reg_113, %reg_114
	store i32 %reg_115, ptr @pos
	br label %Block41
Block41:  ;If end at line 88
	br label %Block38
Block42:  ;If then at line 95
	%reg_119 = add i32 0, 0
	ret i32 %reg_119
	br label %Block43
Block43:  ;If end at line 95
	br label %Block47
Block44:  ;If else at line 95
	%reg_120 = load i32, ptr @pos
	%reg_121 = getelementptr i32, ptr %reg_1, i32 %reg_120
	%reg_122 = load i32, ptr %reg_121
	%reg_123 = call i32 @is_number(i32 %reg_122)
	%reg_124 = add i32 0, 0
	%reg_125 = icmp eq i32 %reg_123, %reg_124
	br i1 %reg_125, label %Block45, label %Block46
Block45:  ;If then at line 100
	%reg_126 = add i32 0, 0
	ret i32 %reg_126
	br label %Block46
Block46:  ;If end at line 100
	br label %Block43
Block47:  ;While condition at line 105
	%reg_127 = load i32, ptr @pos
	%reg_128 = load i32, ptr %reg_4
	%reg_129 = icmp slt i32 %reg_127, %reg_128
	br i1 %reg_129, label %Block48, label %Block49
Block48:  ;While body at line 105
	%reg_130 = load i32, ptr @pos
	%reg_131 = getelementptr i32, ptr %reg_1, i32 %reg_130
	%reg_132 = load i32, ptr %reg_131
	%reg_133 = call i32 @is_number(i32 %reg_132)
	%reg_134 = add i32 1, 0
	%reg_135 = icmp ne i32 %reg_133, %reg_134
	br i1 %reg_135, label %Block50, label %Block51
Block49:  ;While end at line 105
	%reg_139 = load i32, ptr @pos
	%reg_140 = load i32, ptr %reg_4
	%reg_141 = icmp slt i32 %reg_139, %reg_140
	br i1 %reg_141, label %Block53, label %Block54
Block50:  ;If then at line 106
	br label %Block49
Block51:  ;If end at line 106
	%reg_136 = load i32, ptr @pos
	%reg_137 = add i32 1, 0
	%reg_138 = add i32 %reg_136, %reg_137
	store i32 %reg_138, ptr @pos
	br label %Block47
Block52:  ;Break at line 107
	br label %Block51
Block53:  ;If then at line 112
	%reg_142 = load i32, ptr @pos
	%reg_143 = getelementptr i32, ptr %reg_1, i32 %reg_142
	%reg_144 = load i32, ptr %reg_143
	%reg_145 = add i32 46, 0
	%reg_146 = icmp eq i32 %reg_144, %reg_145
	br i1 %reg_146, label %Block55, label %Block56
Block54:  ;If end at line 112
	%reg_162 = load i32, ptr @pos
	%reg_163 = load i32, ptr %reg_4
	%reg_164 = icmp slt i32 %reg_162, %reg_163
	br i1 %reg_164, label %Block63, label %Block64
Block55:  ;If then at line 113
	%reg_147 = load i32, ptr @pos
	%reg_148 = add i32 1, 0
	%reg_149 = add i32 %reg_147, %reg_148
	store i32 %reg_149, ptr @pos
	br label %Block57
Block56:  ;If end at line 113
	br label %Block54
Block57:  ;While condition at line 116
	%reg_150 = load i32, ptr @pos
	%reg_151 = load i32, ptr %reg_4
	%reg_152 = icmp slt i32 %reg_150, %reg_151
	br i1 %reg_152, label %Block58, label %Block59
Block58:  ;While body at line 116
	%reg_153 = load i32, ptr @pos
	%reg_154 = getelementptr i32, ptr %reg_1, i32 %reg_153
	%reg_155 = load i32, ptr %reg_154
	%reg_156 = call i32 @is_number(i32 %reg_155)
	%reg_157 = add i32 1, 0
	%reg_158 = icmp ne i32 %reg_156, %reg_157
	br i1 %reg_158, label %Block60, label %Block61
Block59:  ;While end at line 116
	br label %Block56
Block60:  ;If then at line 117
	br label %Block59
Block61:  ;If end at line 117
	%reg_159 = load i32, ptr @pos
	%reg_160 = add i32 1, 0
	%reg_161 = add i32 %reg_159, %reg_160
	store i32 %reg_161, ptr @pos
	br label %Block57
Block62:  ;Break at line 118
	br label %Block61
Block63:  ;If then at line 125
	%reg_165 = load i32, ptr @pos
	%reg_166 = getelementptr i32, ptr %reg_1, i32 %reg_165
	%reg_167 = load i32, ptr %reg_166
	%reg_168 = add i32 101, 0
	%reg_169 = icmp eq i32 %reg_167, %reg_168
	br i1 %reg_169, label %Block65, label %Block66
Block64:  ;If end at line 125
	br label %Block35
Block65:  ;If then at line 126
	%reg_170 = load i32, ptr @pos
	%reg_171 = add i32 1, 0
	%reg_172 = add i32 %reg_170, %reg_171
	store i32 %reg_172, ptr @pos
	%reg_173 = load i32, ptr @pos
	%reg_174 = load i32, ptr %reg_4
	%reg_175 = icmp slt i32 %reg_173, %reg_174
	br i1 %reg_175, label %Block67, label %Block68
Block66:  ;If end at line 126
	br label %Block64
Block67:  ;If then at line 129
	%reg_176 = load i32, ptr @pos
	%reg_177 = getelementptr i32, ptr %reg_1, i32 %reg_176
	%reg_178 = load i32, ptr %reg_177
	%reg_179 = add i32 43, 0
	%reg_180 = icmp eq i32 %reg_178, %reg_179
	br i1 %reg_180, label %Block69, label %Block70
Block68:  ;If end at line 129
	%reg_184 = load i32, ptr @pos
	%reg_185 = load i32, ptr %reg_4
	%reg_186 = icmp slt i32 %reg_184, %reg_185
	br i1 %reg_186, label %Block71, label %Block72
Block69:  ;If then at line 130
	%reg_181 = load i32, ptr @pos
	%reg_182 = add i32 1, 0
	%reg_183 = add i32 %reg_181, %reg_182
	store i32 %reg_183, ptr @pos
	br label %Block70
Block70:  ;If end at line 130
	br label %Block68
Block71:  ;If then at line 134
	%reg_187 = load i32, ptr @pos
	%reg_188 = getelementptr i32, ptr %reg_1, i32 %reg_187
	%reg_189 = load i32, ptr %reg_188
	%reg_190 = add i32 45, 0
	%reg_191 = icmp eq i32 %reg_189, %reg_190
	br i1 %reg_191, label %Block73, label %Block74
Block72:  ;If end at line 134
	br label %Block75
Block73:  ;If then at line 135
	%reg_192 = load i32, ptr @pos
	%reg_193 = add i32 1, 0
	%reg_194 = add i32 %reg_192, %reg_193
	store i32 %reg_194, ptr @pos
	br label %Block74
Block74:  ;If end at line 135
	br label %Block72
Block75:  ;While condition at line 141
	%reg_195 = load i32, ptr @pos
	%reg_196 = load i32, ptr %reg_4
	%reg_197 = icmp slt i32 %reg_195, %reg_196
	br i1 %reg_197, label %Block76, label %Block77
Block76:  ;While body at line 141
	%reg_198 = load i32, ptr @pos
	%reg_199 = getelementptr i32, ptr %reg_1, i32 %reg_198
	%reg_200 = load i32, ptr %reg_199
	%reg_201 = call i32 @is_number(i32 %reg_200)
	%reg_202 = add i32 1, 0
	%reg_203 = icmp ne i32 %reg_201, %reg_202
	br i1 %reg_203, label %Block78, label %Block79
Block77:  ;While end at line 141
	br label %Block66
Block78:  ;If then at line 142
	br label %Block77
Block79:  ;If end at line 142
	%reg_204 = load i32, ptr @pos
	%reg_205 = add i32 1, 0
	%reg_206 = add i32 %reg_204, %reg_205
	store i32 %reg_206, ptr @pos
	br label %Block75
Block80:  ;Break at line 143
	br label %Block79
Block81:  ;If then at line 149
	%reg_210 = load i32, ptr @pos
	%reg_211 = add i32 1, 0
	%reg_212 = add i32 %reg_210, %reg_211
	store i32 %reg_212, ptr @pos
	br label %Block84
Block82:  ;If end at line 149
	br label %Block35
Block83:  ;If else at line 149
	%reg_245 = load i32, ptr %reg_3
	%reg_246 = add i32 3, 0
	%reg_247 = icmp eq i32 %reg_245, %reg_246
	br i1 %reg_247, label %Block98, label %Block100
Block84:  ;While condition at line 153
	%reg_213 = load i32, ptr @pos
	%reg_214 = load i32, ptr %reg_4
	%reg_215 = icmp slt i32 %reg_213, %reg_214
	br i1 %reg_215, label %Block85, label %Block86
Block85:  ;While body at line 153
	%reg_216 = load i32, ptr @pos
	%reg_217 = getelementptr i32, ptr %reg_1, i32 %reg_216
	%reg_218 = load i32, ptr %reg_217
	%reg_219 = add i32 34, 0
	%reg_220 = icmp eq i32 %reg_218, %reg_219
	br i1 %reg_220, label %Block87, label %Block88
Block86:  ;While end at line 153
	%reg_232 = load i32, ptr @pos
	%reg_233 = load i32, ptr %reg_4
	%reg_234 = icmp sge i32 %reg_232, %reg_233
	br i1 %reg_234, label %Block93, label %Block95
Block87:  ;If then at line 154
	br label %Block86
Block88:  ;If end at line 154
	%reg_221 = load i32, ptr @pos
	%reg_222 = getelementptr i32, ptr %reg_1, i32 %reg_221
	%reg_223 = load i32, ptr %reg_222
	%reg_224 = add i32 92, 0
	%reg_225 = icmp eq i32 %reg_223, %reg_224
	br i1 %reg_225, label %Block90, label %Block92
Block89:  ;Break at line 155
	br label %Block88
Block90:  ;If then at line 157
	%reg_226 = load i32, ptr @pos
	%reg_227 = add i32 2, 0
	%reg_228 = add i32 %reg_226, %reg_227
	store i32 %reg_228, ptr @pos
	br label %Block91
Block91:  ;If end at line 157
	br label %Block84
Block92:  ;If else at line 157
	%reg_229 = load i32, ptr @pos
	%reg_230 = add i32 1, 0
	%reg_231 = add i32 %reg_229, %reg_230
	store i32 %reg_231, ptr @pos
	br label %Block91
Block93:  ;If then at line 165
	%reg_235 = add i32 0, 0
	ret i32 %reg_235
	br label %Block94
Block94:  ;If end at line 165
	%reg_242 = load i32, ptr @pos
	%reg_243 = add i32 1, 0
	%reg_244 = add i32 %reg_242, %reg_243
	store i32 %reg_244, ptr @pos
	br label %Block82
Block95:  ;If else at line 165
	%reg_236 = load i32, ptr @pos
	%reg_237 = getelementptr i32, ptr %reg_1, i32 %reg_236
	%reg_238 = load i32, ptr %reg_237
	%reg_239 = add i32 34, 0
	%reg_240 = icmp ne i32 %reg_238, %reg_239
	br i1 %reg_240, label %Block96, label %Block97
Block96:  ;If then at line 170
	%reg_241 = add i32 0, 0
	ret i32 %reg_241
	br label %Block97
Block97:  ;If end at line 170
	br label %Block94
Block98:  ;If then at line 176
	%reg_248 = load i32, ptr @pos
	%reg_249 = add i32 1, 0
	%reg_250 = add i32 %reg_248, %reg_249
	store i32 %reg_250, ptr @pos
	%reg_251 = getelementptr i32, ptr %reg_1
	%reg_252 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_251, i32 %reg_252)
	%reg_253 = load i32, ptr @pos
	%reg_254 = load i32, ptr %reg_4
	%reg_255 = icmp slt i32 %reg_253, %reg_254
	br i1 %reg_255, label %Block101, label %Block102
Block99:  ;If end at line 176
	br label %Block82
Block100:  ;If else at line 176
	%reg_308 = load i32, ptr %reg_3
	%reg_309 = add i32 4, 0
	%reg_310 = icmp eq i32 %reg_308, %reg_309
	br i1 %reg_310, label %Block116, label %Block118
Block101:  ;If then at line 182
	%reg_256 = load i32, ptr @pos
	%reg_257 = getelementptr i32, ptr %reg_1, i32 %reg_256
	%reg_258 = load i32, ptr %reg_257
	%reg_259 = add i32 93, 0
	%reg_260 = icmp eq i32 %reg_258, %reg_259
	br i1 %reg_260, label %Block103, label %Block104
Block102:  ;If end at line 182
	%reg_265 = add i32 0, 0
	%reg_266 = getelementptr i32, ptr %reg_1
	%reg_267 = load i32, ptr %reg_4
	%reg_268 = call i32 @detect_item(i32 %reg_265, ptr %reg_266, i32 %reg_267)
	%reg_269 = add i32 0, 0
	%reg_270 = icmp eq i32 %reg_268, %reg_269
	br i1 %reg_270, label %Block105, label %Block106
Block103:  ;If then at line 183
	%reg_261 = load i32, ptr @pos
	%reg_262 = add i32 1, 0
	%reg_263 = add i32 %reg_261, %reg_262
	store i32 %reg_263, ptr @pos
	%reg_264 = add i32 1, 0
	ret i32 %reg_264
	br label %Block104
Block104:  ;If end at line 183
	br label %Block102
Block105:  ;If then at line 191
	%reg_271 = add i32 0, 0
	ret i32 %reg_271
	br label %Block106
Block106:  ;If end at line 191
	%reg_272 = getelementptr i32, ptr %reg_1
	%reg_273 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_272, i32 %reg_273)
	br label %Block107
Block107:  ;While condition at line 197
	%reg_274 = load i32, ptr @pos
	%reg_275 = getelementptr i32, ptr %reg_1, i32 %reg_274
	%reg_276 = load i32, ptr %reg_275
	%reg_277 = add i32 44, 0
	%reg_278 = icmp eq i32 %reg_276, %reg_277
	br i1 %reg_278, label %Block108, label %Block109
Block108:  ;While body at line 197
	%reg_279 = load i32, ptr @pos
	%reg_280 = add i32 1, 0
	%reg_281 = add i32 %reg_279, %reg_280
	store i32 %reg_281, ptr @pos
	%reg_282 = getelementptr i32, ptr %reg_1
	%reg_283 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_282, i32 %reg_283)
	%reg_284 = add i32 0, 0
	%reg_285 = getelementptr i32, ptr %reg_1
	%reg_286 = load i32, ptr %reg_4
	%reg_287 = call i32 @detect_item(i32 %reg_284, ptr %reg_285, i32 %reg_286)
	%reg_288 = add i32 0, 0
	%reg_289 = icmp eq i32 %reg_287, %reg_288
	br i1 %reg_289, label %Block110, label %Block111
Block109:  ;While end at line 197
	%reg_293 = getelementptr i32, ptr %reg_1
	%reg_294 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_293, i32 %reg_294)
	%reg_295 = load i32, ptr @pos
	%reg_296 = load i32, ptr %reg_4
	%reg_297 = icmp sge i32 %reg_295, %reg_296
	br i1 %reg_297, label %Block112, label %Block113
Block110:  ;If then at line 202
	%reg_290 = add i32 0, 0
	ret i32 %reg_290
	br label %Block111
Block111:  ;If end at line 202
	%reg_291 = getelementptr i32, ptr %reg_1
	%reg_292 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_291, i32 %reg_292)
	br label %Block107
Block112:  ;If then at line 212
	%reg_298 = add i32 0, 0
	ret i32 %reg_298
	br label %Block113
Block113:  ;If end at line 212
	%reg_299 = load i32, ptr @pos
	%reg_300 = getelementptr i32, ptr %reg_1, i32 %reg_299
	%reg_301 = load i32, ptr %reg_300
	%reg_302 = add i32 93, 0
	%reg_303 = icmp ne i32 %reg_301, %reg_302
	br i1 %reg_303, label %Block114, label %Block115
Block114:  ;If then at line 215
	%reg_304 = add i32 0, 0
	ret i32 %reg_304
	br label %Block115
Block115:  ;If end at line 215
	%reg_305 = load i32, ptr @pos
	%reg_306 = add i32 1, 0
	%reg_307 = add i32 %reg_305, %reg_306
	store i32 %reg_307, ptr @pos
	br label %Block99
Block116:  ;If then at line 219
	%reg_311 = load i32, ptr @pos
	%reg_312 = add i32 1, 0
	%reg_313 = add i32 %reg_311, %reg_312
	store i32 %reg_313, ptr @pos
	%reg_314 = getelementptr i32, ptr %reg_1
	%reg_315 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_314, i32 %reg_315)
	%reg_316 = load i32, ptr @pos
	%reg_317 = load i32, ptr %reg_4
	%reg_318 = icmp slt i32 %reg_316, %reg_317
	br i1 %reg_318, label %Block119, label %Block120
Block117:  ;If end at line 219
	br label %Block99
Block118:  ;If else at line 219
	%reg_419 = load i32, ptr %reg_3
	%reg_420 = add i32 5, 0
	%reg_421 = icmp eq i32 %reg_419, %reg_420
	br i1 %reg_421, label %Block147, label %Block149
Block119:  ;If then at line 225
	%reg_319 = load i32, ptr @pos
	%reg_320 = getelementptr i32, ptr %reg_1, i32 %reg_319
	%reg_321 = load i32, ptr %reg_320
	%reg_322 = add i32 125, 0
	%reg_323 = icmp eq i32 %reg_321, %reg_322
	br i1 %reg_323, label %Block121, label %Block122
Block120:  ;If end at line 225
	%reg_328 = add i32 2, 0
	%reg_329 = getelementptr i32, ptr %reg_1
	%reg_330 = load i32, ptr %reg_4
	%reg_331 = call i32 @detect_item(i32 %reg_328, ptr %reg_329, i32 %reg_330)
	%reg_332 = add i32 0, 0
	%reg_333 = icmp eq i32 %reg_331, %reg_332
	br i1 %reg_333, label %Block123, label %Block124
Block121:  ;If then at line 226
	%reg_324 = load i32, ptr @pos
	%reg_325 = add i32 1, 0
	%reg_326 = add i32 %reg_324, %reg_325
	store i32 %reg_326, ptr @pos
	%reg_327 = add i32 1, 0
	ret i32 %reg_327
	br label %Block122
Block122:  ;If end at line 226
	br label %Block120
Block123:  ;If then at line 234
	%reg_334 = add i32 0, 0
	ret i32 %reg_334
	br label %Block124
Block124:  ;If end at line 234
	%reg_335 = getelementptr i32, ptr %reg_1
	%reg_336 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_335, i32 %reg_336)
	%reg_337 = load i32, ptr @pos
	%reg_338 = load i32, ptr %reg_4
	%reg_339 = icmp sge i32 %reg_337, %reg_338
	br i1 %reg_339, label %Block125, label %Block126
Block125:  ;If then at line 240
	%reg_340 = add i32 0, 0
	ret i32 %reg_340
	br label %Block126
Block126:  ;If end at line 240
	%reg_341 = load i32, ptr @pos
	%reg_342 = getelementptr i32, ptr %reg_1, i32 %reg_341
	%reg_343 = load i32, ptr %reg_342
	%reg_344 = add i32 58, 0
	%reg_345 = icmp ne i32 %reg_343, %reg_344
	br i1 %reg_345, label %Block127, label %Block128
Block127:  ;If then at line 243
	%reg_346 = add i32 0, 0
	ret i32 %reg_346
	br label %Block128
Block128:  ;If end at line 243
	%reg_347 = load i32, ptr @pos
	%reg_348 = add i32 1, 0
	%reg_349 = add i32 %reg_347, %reg_348
	store i32 %reg_349, ptr @pos
	%reg_350 = getelementptr i32, ptr %reg_1
	%reg_351 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_350, i32 %reg_351)
	%reg_352 = add i32 0, 0
	%reg_353 = getelementptr i32, ptr %reg_1
	%reg_354 = load i32, ptr %reg_4
	%reg_355 = call i32 @detect_item(i32 %reg_352, ptr %reg_353, i32 %reg_354)
	%reg_356 = add i32 0, 0
	%reg_357 = icmp eq i32 %reg_355, %reg_356
	br i1 %reg_357, label %Block129, label %Block130
Block129:  ;If then at line 250
	%reg_358 = add i32 0, 0
	ret i32 %reg_358
	br label %Block130
Block130:  ;If end at line 250
	%reg_359 = getelementptr i32, ptr %reg_1
	%reg_360 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_359, i32 %reg_360)
	br label %Block131
Block131:  ;While condition at line 256
	%reg_361 = load i32, ptr @pos
	%reg_362 = getelementptr i32, ptr %reg_1, i32 %reg_361
	%reg_363 = load i32, ptr %reg_362
	%reg_364 = add i32 44, 0
	%reg_365 = icmp eq i32 %reg_363, %reg_364
	br i1 %reg_365, label %Block132, label %Block133
Block132:  ;While body at line 256
	%reg_366 = load i32, ptr @pos
	%reg_367 = add i32 1, 0
	%reg_368 = add i32 %reg_366, %reg_367
	store i32 %reg_368, ptr @pos
	%reg_369 = getelementptr i32, ptr %reg_1
	%reg_370 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_369, i32 %reg_370)
	%reg_371 = add i32 2, 0
	%reg_372 = getelementptr i32, ptr %reg_1
	%reg_373 = load i32, ptr %reg_4
	%reg_374 = call i32 @detect_item(i32 %reg_371, ptr %reg_372, i32 %reg_373)
	%reg_375 = add i32 0, 0
	%reg_376 = icmp eq i32 %reg_374, %reg_375
	br i1 %reg_376, label %Block134, label %Block135
Block133:  ;While end at line 256
	%reg_404 = getelementptr i32, ptr %reg_1
	%reg_405 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_404, i32 %reg_405)
	%reg_406 = load i32, ptr @pos
	%reg_407 = load i32, ptr %reg_4
	%reg_408 = icmp sge i32 %reg_406, %reg_407
	br i1 %reg_408, label %Block142, label %Block144
Block134:  ;If then at line 262
	%reg_377 = add i32 0, 0
	ret i32 %reg_377
	br label %Block135
Block135:  ;If end at line 262
	%reg_378 = getelementptr i32, ptr %reg_1
	%reg_379 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_378, i32 %reg_379)
	%reg_380 = load i32, ptr @pos
	%reg_381 = load i32, ptr %reg_4
	%reg_382 = icmp sge i32 %reg_380, %reg_381
	br i1 %reg_382, label %Block136, label %Block137
Block136:  ;If then at line 268
	%reg_383 = add i32 0, 0
	ret i32 %reg_383
	br label %Block137
Block137:  ;If end at line 268
	%reg_384 = load i32, ptr @pos
	%reg_385 = getelementptr i32, ptr %reg_1, i32 %reg_384
	%reg_386 = load i32, ptr %reg_385
	%reg_387 = add i32 58, 0
	%reg_388 = icmp ne i32 %reg_386, %reg_387
	br i1 %reg_388, label %Block138, label %Block139
Block138:  ;If then at line 271
	%reg_389 = add i32 0, 0
	ret i32 %reg_389
	br label %Block139
Block139:  ;If end at line 271
	%reg_390 = load i32, ptr @pos
	%reg_391 = add i32 1, 0
	%reg_392 = add i32 %reg_390, %reg_391
	store i32 %reg_392, ptr @pos
	%reg_393 = getelementptr i32, ptr %reg_1
	%reg_394 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_393, i32 %reg_394)
	%reg_395 = add i32 0, 0
	%reg_396 = getelementptr i32, ptr %reg_1
	%reg_397 = load i32, ptr %reg_4
	%reg_398 = call i32 @detect_item(i32 %reg_395, ptr %reg_396, i32 %reg_397)
	%reg_399 = add i32 0, 0
	%reg_400 = icmp eq i32 %reg_398, %reg_399
	br i1 %reg_400, label %Block140, label %Block141
Block140:  ;If then at line 278
	%reg_401 = add i32 0, 0
	ret i32 %reg_401
	br label %Block141
Block141:  ;If end at line 278
	%reg_402 = getelementptr i32, ptr %reg_1
	%reg_403 = load i32, ptr %reg_4
	call void @skip_space(ptr %reg_402, i32 %reg_403)
	br label %Block131
Block142:  ;If then at line 286
	%reg_409 = add i32 0, 0
	ret i32 %reg_409
	br label %Block143
Block143:  ;If end at line 286
	%reg_416 = load i32, ptr @pos
	%reg_417 = add i32 1, 0
	%reg_418 = add i32 %reg_416, %reg_417
	store i32 %reg_418, ptr @pos
	br label %Block117
Block144:  ;If else at line 286
	%reg_410 = load i32, ptr @pos
	%reg_411 = getelementptr i32, ptr %reg_1, i32 %reg_410
	%reg_412 = load i32, ptr %reg_411
	%reg_413 = add i32 125, 0
	%reg_414 = icmp ne i32 %reg_412, %reg_413
	br i1 %reg_414, label %Block145, label %Block146
Block145:  ;If then at line 289
	%reg_415 = add i32 0, 0
	ret i32 %reg_415
	br label %Block146
Block146:  ;If end at line 289
	br label %Block143
Block147:  ;If then at line 294
	call void @llvm.memset.p0.i32(ptr %reg_422, i8 0, i32 16, i1 1)
	%reg_423 = add i32 116, 0
	%reg_424 = getelementptr [4 x i32], ptr %reg_422, i32 0, i32 0
	store i32 %reg_423, ptr %reg_424
	%reg_425 = add i32 114, 0
	%reg_426 = getelementptr [4 x i32], ptr %reg_422, i32 0, i32 1
	store i32 %reg_425, ptr %reg_426
	%reg_427 = add i32 117, 0
	%reg_428 = getelementptr [4 x i32], ptr %reg_422, i32 0, i32 2
	store i32 %reg_427, ptr %reg_428
	%reg_429 = add i32 101, 0
	%reg_430 = getelementptr [4 x i32], ptr %reg_422, i32 0, i32 3
	store i32 %reg_429, ptr %reg_430
	%reg_431 = load i32, ptr @pos
	%reg_432 = add i32 3, 0
	%reg_433 = add i32 %reg_431, %reg_432
	%reg_434 = load i32, ptr %reg_4
	%reg_435 = icmp sge i32 %reg_433, %reg_434
	br i1 %reg_435, label %Block150, label %Block152
Block148:  ;If end at line 294
	br label %Block117
Block149:  ;If else at line 294
	%reg_478 = load i32, ptr %reg_3
	%reg_479 = add i32 6, 0
	%reg_480 = icmp eq i32 %reg_478, %reg_479
	br i1 %reg_480, label %Block164, label %Block166
Block150:  ;If then at line 297
	%reg_436 = add i32 0, 0
	ret i32 %reg_436
	br label %Block151
Block151:  ;If end at line 297
	%reg_475 = load i32, ptr @pos
	%reg_476 = add i32 4, 0
	%reg_477 = add i32 %reg_475, %reg_476
	store i32 %reg_477, ptr @pos
	br label %Block148
Block152:  ;If else at line 297
	%reg_437 = load i32, ptr @pos
	%reg_438 = getelementptr i32, ptr %reg_1, i32 %reg_437
	%reg_439 = load i32, ptr %reg_438
	%reg_440 = add i32 0, 0
	%reg_441 = getelementptr [4 x i32], ptr %reg_422, i32 0, i32 %reg_440
	%reg_442 = load i32, ptr %reg_441
	%reg_443 = icmp ne i32 %reg_439, %reg_442
	br i1 %reg_443, label %Block153, label %Block155
Block153:  ;If then at line 301
	%reg_444 = add i32 0, 0
	ret i32 %reg_444
	br label %Block154
Block154:  ;If end at line 301
	br label %Block151
Block155:  ;If else at line 301
	%reg_445 = load i32, ptr @pos
	%reg_446 = add i32 1, 0
	%reg_447 = add i32 %reg_445, %reg_446
	%reg_448 = getelementptr i32, ptr %reg_1, i32 %reg_447
	%reg_449 = load i32, ptr %reg_448
	%reg_450 = add i32 1, 0
	%reg_451 = getelementptr [4 x i32], ptr %reg_422, i32 0, i32 %reg_450
	%reg_452 = load i32, ptr %reg_451
	%reg_453 = icmp ne i32 %reg_449, %reg_452
	br i1 %reg_453, label %Block156, label %Block158
Block156:  ;If then at line 303
	%reg_454 = add i32 0, 0
	ret i32 %reg_454
	br label %Block157
Block157:  ;If end at line 303
	br label %Block154
Block158:  ;If else at line 303
	%reg_455 = load i32, ptr @pos
	%reg_456 = add i32 2, 0
	%reg_457 = add i32 %reg_455, %reg_456
	%reg_458 = getelementptr i32, ptr %reg_1, i32 %reg_457
	%reg_459 = load i32, ptr %reg_458
	%reg_460 = add i32 2, 0
	%reg_461 = getelementptr [4 x i32], ptr %reg_422, i32 0, i32 %reg_460
	%reg_462 = load i32, ptr %reg_461
	%reg_463 = icmp ne i32 %reg_459, %reg_462
	br i1 %reg_463, label %Block159, label %Block161
Block159:  ;If then at line 305
	%reg_464 = add i32 0, 0
	ret i32 %reg_464
	br label %Block160
Block160:  ;If end at line 305
	br label %Block157
Block161:  ;If else at line 305
	%reg_465 = load i32, ptr @pos
	%reg_466 = add i32 3, 0
	%reg_467 = add i32 %reg_465, %reg_466
	%reg_468 = getelementptr i32, ptr %reg_1, i32 %reg_467
	%reg_469 = load i32, ptr %reg_468
	%reg_470 = add i32 3, 0
	%reg_471 = getelementptr [4 x i32], ptr %reg_422, i32 0, i32 %reg_470
	%reg_472 = load i32, ptr %reg_471
	%reg_473 = icmp ne i32 %reg_469, %reg_472
	br i1 %reg_473, label %Block162, label %Block163
Block162:  ;If then at line 307
	%reg_474 = add i32 0, 0
	ret i32 %reg_474
	br label %Block163
Block163:  ;If end at line 307
	br label %Block160
Block164:  ;If then at line 311
	call void @llvm.memset.p0.i32(ptr %reg_481, i8 0, i32 20, i1 1)
	%reg_482 = add i32 102, 0
	%reg_483 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 0
	store i32 %reg_482, ptr %reg_483
	%reg_484 = add i32 97, 0
	%reg_485 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 1
	store i32 %reg_484, ptr %reg_485
	%reg_486 = add i32 108, 0
	%reg_487 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 2
	store i32 %reg_486, ptr %reg_487
	%reg_488 = add i32 115, 0
	%reg_489 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 3
	store i32 %reg_488, ptr %reg_489
	%reg_490 = add i32 101, 0
	%reg_491 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 4
	store i32 %reg_490, ptr %reg_491
	%reg_492 = load i32, ptr @pos
	%reg_493 = add i32 4, 0
	%reg_494 = add i32 %reg_492, %reg_493
	%reg_495 = load i32, ptr %reg_4
	%reg_496 = icmp sge i32 %reg_494, %reg_495
	br i1 %reg_496, label %Block167, label %Block169
Block165:  ;If end at line 311
	br label %Block148
Block166:  ;If else at line 311
	%reg_549 = load i32, ptr %reg_3
	%reg_550 = add i32 7, 0
	%reg_551 = icmp eq i32 %reg_549, %reg_550
	br i1 %reg_551, label %Block184, label %Block186
Block167:  ;If then at line 314
	%reg_497 = add i32 0, 0
	ret i32 %reg_497
	br label %Block168
Block168:  ;If end at line 314
	%reg_546 = load i32, ptr @pos
	%reg_547 = add i32 5, 0
	%reg_548 = add i32 %reg_546, %reg_547
	store i32 %reg_548, ptr @pos
	br label %Block165
Block169:  ;If else at line 314
	%reg_498 = load i32, ptr @pos
	%reg_499 = getelementptr i32, ptr %reg_1, i32 %reg_498
	%reg_500 = load i32, ptr %reg_499
	%reg_501 = add i32 0, 0
	%reg_502 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 %reg_501
	%reg_503 = load i32, ptr %reg_502
	%reg_504 = icmp ne i32 %reg_500, %reg_503
	br i1 %reg_504, label %Block170, label %Block172
Block170:  ;If then at line 319
	%reg_505 = add i32 0, 0
	ret i32 %reg_505
	br label %Block171
Block171:  ;If end at line 319
	br label %Block168
Block172:  ;If else at line 319
	%reg_506 = load i32, ptr @pos
	%reg_507 = add i32 1, 0
	%reg_508 = add i32 %reg_506, %reg_507
	%reg_509 = getelementptr i32, ptr %reg_1, i32 %reg_508
	%reg_510 = load i32, ptr %reg_509
	%reg_511 = add i32 1, 0
	%reg_512 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 %reg_511
	%reg_513 = load i32, ptr %reg_512
	%reg_514 = icmp ne i32 %reg_510, %reg_513
	br i1 %reg_514, label %Block173, label %Block175
Block173:  ;If then at line 321
	%reg_515 = add i32 0, 0
	ret i32 %reg_515
	br label %Block174
Block174:  ;If end at line 321
	br label %Block171
Block175:  ;If else at line 321
	%reg_516 = load i32, ptr @pos
	%reg_517 = add i32 2, 0
	%reg_518 = add i32 %reg_516, %reg_517
	%reg_519 = getelementptr i32, ptr %reg_1, i32 %reg_518
	%reg_520 = load i32, ptr %reg_519
	%reg_521 = add i32 2, 0
	%reg_522 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 %reg_521
	%reg_523 = load i32, ptr %reg_522
	%reg_524 = icmp ne i32 %reg_520, %reg_523
	br i1 %reg_524, label %Block176, label %Block178
Block176:  ;If then at line 323
	%reg_525 = add i32 0, 0
	ret i32 %reg_525
	br label %Block177
Block177:  ;If end at line 323
	br label %Block174
Block178:  ;If else at line 323
	%reg_526 = load i32, ptr @pos
	%reg_527 = add i32 3, 0
	%reg_528 = add i32 %reg_526, %reg_527
	%reg_529 = getelementptr i32, ptr %reg_1, i32 %reg_528
	%reg_530 = load i32, ptr %reg_529
	%reg_531 = add i32 3, 0
	%reg_532 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 %reg_531
	%reg_533 = load i32, ptr %reg_532
	%reg_534 = icmp ne i32 %reg_530, %reg_533
	br i1 %reg_534, label %Block179, label %Block181
Block179:  ;If then at line 325
	%reg_535 = add i32 0, 0
	ret i32 %reg_535
	br label %Block180
Block180:  ;If end at line 325
	br label %Block177
Block181:  ;If else at line 325
	%reg_536 = load i32, ptr @pos
	%reg_537 = add i32 4, 0
	%reg_538 = add i32 %reg_536, %reg_537
	%reg_539 = getelementptr i32, ptr %reg_1, i32 %reg_538
	%reg_540 = load i32, ptr %reg_539
	%reg_541 = add i32 4, 0
	%reg_542 = getelementptr [5 x i32], ptr %reg_481, i32 0, i32 %reg_541
	%reg_543 = load i32, ptr %reg_542
	%reg_544 = icmp ne i32 %reg_540, %reg_543
	br i1 %reg_544, label %Block182, label %Block183
Block182:  ;If then at line 327
	%reg_545 = add i32 0, 0
	ret i32 %reg_545
	br label %Block183
Block183:  ;If end at line 327
	br label %Block180
Block184:  ;If then at line 331
	call void @llvm.memset.p0.i32(ptr %reg_552, i8 0, i32 16, i1 1)
	%reg_553 = add i32 110, 0
	%reg_554 = getelementptr [4 x i32], ptr %reg_552, i32 0, i32 0
	store i32 %reg_553, ptr %reg_554
	%reg_555 = add i32 117, 0
	%reg_556 = getelementptr [4 x i32], ptr %reg_552, i32 0, i32 1
	store i32 %reg_555, ptr %reg_556
	%reg_557 = add i32 108, 0
	%reg_558 = getelementptr [4 x i32], ptr %reg_552, i32 0, i32 2
	store i32 %reg_557, ptr %reg_558
	%reg_559 = add i32 108, 0
	%reg_560 = getelementptr [4 x i32], ptr %reg_552, i32 0, i32 3
	store i32 %reg_559, ptr %reg_560
	%reg_561 = load i32, ptr @pos
	%reg_562 = add i32 3, 0
	%reg_563 = add i32 %reg_561, %reg_562
	%reg_564 = load i32, ptr %reg_4
	%reg_565 = icmp sge i32 %reg_563, %reg_564
	br i1 %reg_565, label %Block187, label %Block189
Block185:  ;If end at line 331
	br label %Block165
Block186:  ;If else at line 331
	%reg_608 = add i32 0, 0
	ret i32 %reg_608
	br label %Block185
Block187:  ;If then at line 334
	%reg_566 = add i32 0, 0
	ret i32 %reg_566
	br label %Block188
Block188:  ;If end at line 334
	%reg_605 = load i32, ptr @pos
	%reg_606 = add i32 4, 0
	%reg_607 = add i32 %reg_605, %reg_606
	store i32 %reg_607, ptr @pos
	br label %Block185
Block189:  ;If else at line 334
	%reg_567 = load i32, ptr @pos
	%reg_568 = getelementptr i32, ptr %reg_1, i32 %reg_567
	%reg_569 = load i32, ptr %reg_568
	%reg_570 = add i32 0, 0
	%reg_571 = getelementptr [4 x i32], ptr %reg_552, i32 0, i32 %reg_570
	%reg_572 = load i32, ptr %reg_571
	%reg_573 = icmp ne i32 %reg_569, %reg_572
	br i1 %reg_573, label %Block190, label %Block192
Block190:  ;If then at line 338
	%reg_574 = add i32 0, 0
	ret i32 %reg_574
	br label %Block191
Block191:  ;If end at line 338
	br label %Block188
Block192:  ;If else at line 338
	%reg_575 = load i32, ptr @pos
	%reg_576 = add i32 1, 0
	%reg_577 = add i32 %reg_575, %reg_576
	%reg_578 = getelementptr i32, ptr %reg_1, i32 %reg_577
	%reg_579 = load i32, ptr %reg_578
	%reg_580 = add i32 1, 0
	%reg_581 = getelementptr [4 x i32], ptr %reg_552, i32 0, i32 %reg_580
	%reg_582 = load i32, ptr %reg_581
	%reg_583 = icmp ne i32 %reg_579, %reg_582
	br i1 %reg_583, label %Block193, label %Block195
Block193:  ;If then at line 340
	%reg_584 = add i32 0, 0
	ret i32 %reg_584
	br label %Block194
Block194:  ;If end at line 340
	br label %Block191
Block195:  ;If else at line 340
	%reg_585 = load i32, ptr @pos
	%reg_586 = add i32 2, 0
	%reg_587 = add i32 %reg_585, %reg_586
	%reg_588 = getelementptr i32, ptr %reg_1, i32 %reg_587
	%reg_589 = load i32, ptr %reg_588
	%reg_590 = add i32 2, 0
	%reg_591 = getelementptr [4 x i32], ptr %reg_552, i32 0, i32 %reg_590
	%reg_592 = load i32, ptr %reg_591
	%reg_593 = icmp ne i32 %reg_589, %reg_592
	br i1 %reg_593, label %Block196, label %Block198
Block196:  ;If then at line 342
	%reg_594 = add i32 0, 0
	ret i32 %reg_594
	br label %Block197
Block197:  ;If end at line 342
	br label %Block194
Block198:  ;If else at line 342
	%reg_595 = load i32, ptr @pos
	%reg_596 = add i32 3, 0
	%reg_597 = add i32 %reg_595, %reg_596
	%reg_598 = getelementptr i32, ptr %reg_1, i32 %reg_597
	%reg_599 = load i32, ptr %reg_598
	%reg_600 = add i32 3, 0
	%reg_601 = getelementptr [4 x i32], ptr %reg_552, i32 0, i32 %reg_600
	%reg_602 = load i32, ptr %reg_601
	%reg_603 = icmp ne i32 %reg_599, %reg_602
	br i1 %reg_603, label %Block199, label %Block200
Block199:  ;If then at line 344
	%reg_604 = add i32 0, 0
	ret i32 %reg_604
	br label %Block200
Block200:  ;If end at line 344
	br label %Block197
}

define i32 @main()
{
Block0:  ;Func define at line 357
	%reg_16 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 357
	%reg_1 = call i32 @getch()
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 360
	%reg_4 = load i32, ptr %reg_0
	%reg_5 = add i32 35, 0
	%reg_6 = icmp ne i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 360
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = getelementptr [50000000 x i32], ptr @buffer, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_0
	store i32 %reg_9, ptr %reg_8
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = add i32 1, 0
	%reg_12 = add i32 %reg_10, %reg_11
	store i32 %reg_12, ptr %reg_2
	%reg_13 = call i32 @getch()
	store i32 %reg_13, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 360
	%reg_14 = getelementptr [50000000 x i32], ptr @buffer, i32 0
	%reg_15 = load i32, ptr %reg_2
	call void @skip_space(ptr %reg_14, i32 %reg_15)
	%reg_17 = add i32 0, 0
	%reg_18 = getelementptr [50000000 x i32], ptr @buffer, i32 0
	%reg_19 = load i32, ptr %reg_2
	%reg_20 = call i32 @detect_item(i32 %reg_17, ptr %reg_18, i32 %reg_19)
	store i32 %reg_20, ptr %reg_16
	%reg_21 = getelementptr [50000000 x i32], ptr @buffer, i32 0
	%reg_22 = load i32, ptr %reg_2
	call void @skip_space(ptr %reg_21, i32 %reg_22)
	%reg_23 = load i32, ptr %reg_16
	%reg_24 = add i32 0, 0
	%reg_25 = icmp ne i32 %reg_23, %reg_24
	br i1 %reg_25, label %Block5, label %Block7
Block5:  ;If then at line 368
	%reg_26 = add i32 111, 0
	call void @putch(i32 %reg_26)
	%reg_27 = add i32 107, 0
	call void @putch(i32 %reg_27)
	%reg_28 = add i32 10, 0
	call void @putch(i32 %reg_28)
	%reg_29 = add i32 0, 0
	ret i32 %reg_29
	br label %Block6
Block6:  ;If end at line 368
	ret i32 0
Block7:  ;If else at line 368
	%reg_30 = add i32 110, 0
	call void @putch(i32 %reg_30)
	%reg_31 = add i32 111, 0
	call void @putch(i32 %reg_31)
	%reg_32 = add i32 116, 0
	call void @putch(i32 %reg_32)
	%reg_33 = add i32 32, 0
	call void @putch(i32 %reg_33)
	%reg_34 = add i32 111, 0
	call void @putch(i32 %reg_34)
	%reg_35 = add i32 107, 0
	call void @putch(i32 %reg_35)
	%reg_36 = add i32 10, 0
	call void @putch(i32 %reg_36)
	%reg_37 = add i32 1, 0
	ret i32 %reg_37
	br label %Block6
}
