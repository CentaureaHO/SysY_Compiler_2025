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


define void @sort(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 1
	%reg_24 = alloca i32
	%reg_10 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 1
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 3
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = add i32 1, 0
	%reg_8 = sub i32 %reg_6, %reg_7
	%reg_9 = icmp slt i32 %reg_5, %reg_8
	br i1 %reg_9, label %Block3, label %Block4
Block3:  ;While body at line 3
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = add i32 1, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_10
	br label %Block5
Block4:  ;While end at line 3
	ret void
Block5:  ;While condition at line 5
	%reg_14 = load i32, ptr %reg_10
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block6, label %Block7
Block6:  ;While body at line 5
	%reg_17 = load i32, ptr %reg_3
	%reg_18 = getelementptr i32, ptr %reg_0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = load i32, ptr %reg_10
	%reg_21 = getelementptr i32, ptr %reg_0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	%reg_23 = icmp slt i32 %reg_19, %reg_22
	br i1 %reg_23, label %Block8, label %Block9
Block7:  ;While end at line 5
	%reg_39 = load i32, ptr %reg_3
	%reg_40 = add i32 1, 0
	%reg_41 = add i32 %reg_39, %reg_40
	store i32 %reg_41, ptr %reg_3
	br label %Block2
Block8:  ;If then at line 6
	%reg_25 = load i32, ptr %reg_3
	%reg_26 = getelementptr i32, ptr %reg_0, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	store i32 %reg_27, ptr %reg_24
	%reg_28 = load i32, ptr %reg_3
	%reg_29 = getelementptr i32, ptr %reg_0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_10
	%reg_31 = getelementptr i32, ptr %reg_0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	store i32 %reg_32, ptr %reg_29
	%reg_33 = load i32, ptr %reg_10
	%reg_34 = getelementptr i32, ptr %reg_0, i32 %reg_33
	%reg_35 = load i32, ptr %reg_24
	store i32 %reg_35, ptr %reg_34
	br label %Block9
Block9:  ;If end at line 6
	%reg_36 = load i32, ptr %reg_10
	%reg_37 = add i32 1, 0
	%reg_38 = add i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_10
	br label %Block5
}

define i32 @param32_rec(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, i32 %reg_7, i32 %reg_8, i32 %reg_9, i32 %reg_10, i32 %reg_11, i32 %reg_12, i32 %reg_13, i32 %reg_14, i32 %reg_15, i32 %reg_16, i32 %reg_17, i32 %reg_18, i32 %reg_19, i32 %reg_20, i32 %reg_21, i32 %reg_22, i32 %reg_23, i32 %reg_24, i32 %reg_25, i32 %reg_26, i32 %reg_27, i32 %reg_28, i32 %reg_29, i32 %reg_30, i32 %reg_31)
{
Block0:  ;Func define at line 18
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
Block1:  ;Func body at line 18
	%reg_64 = load i32, ptr %reg_32
	%reg_65 = add i32 0, 0
	%reg_66 = icmp eq i32 %reg_64, %reg_65
	br i1 %reg_66, label %Block2, label %Block4
Block2:  ;If then at line 23
	%reg_67 = load i32, ptr %reg_33
	ret i32 %reg_67
	br label %Block3
Block3:  ;If end at line 23
	ret i32 0
Block4:  ;If else at line 23
	%reg_68 = load i32, ptr %reg_32
	%reg_69 = add i32 1, 0
	%reg_70 = sub i32 %reg_68, %reg_69
	%reg_71 = load i32, ptr %reg_33
	%reg_72 = load i32, ptr %reg_34
	%reg_73 = add i32 %reg_71, %reg_72
	%reg_74 = add i32 998244353, 0
	%reg_75 = srem i32 %reg_73, %reg_74
	%reg_76 = load i32, ptr %reg_35
	%reg_77 = load i32, ptr %reg_36
	%reg_78 = load i32, ptr %reg_37
	%reg_79 = load i32, ptr %reg_38
	%reg_80 = load i32, ptr %reg_39
	%reg_81 = load i32, ptr %reg_40
	%reg_82 = load i32, ptr %reg_41
	%reg_83 = load i32, ptr %reg_42
	%reg_84 = load i32, ptr %reg_43
	%reg_85 = load i32, ptr %reg_44
	%reg_86 = load i32, ptr %reg_45
	%reg_87 = load i32, ptr %reg_46
	%reg_88 = load i32, ptr %reg_47
	%reg_89 = load i32, ptr %reg_48
	%reg_90 = load i32, ptr %reg_49
	%reg_91 = load i32, ptr %reg_50
	%reg_92 = load i32, ptr %reg_51
	%reg_93 = load i32, ptr %reg_52
	%reg_94 = load i32, ptr %reg_53
	%reg_95 = load i32, ptr %reg_54
	%reg_96 = load i32, ptr %reg_55
	%reg_97 = load i32, ptr %reg_56
	%reg_98 = load i32, ptr %reg_57
	%reg_99 = load i32, ptr %reg_58
	%reg_100 = load i32, ptr %reg_59
	%reg_101 = load i32, ptr %reg_60
	%reg_102 = load i32, ptr %reg_61
	%reg_103 = load i32, ptr %reg_62
	%reg_104 = load i32, ptr %reg_63
	%reg_105 = add i32 0, 0
	%reg_106 = call i32 @param32_rec(i32 %reg_70, i32 %reg_75, i32 %reg_76, i32 %reg_77, i32 %reg_78, i32 %reg_79, i32 %reg_80, i32 %reg_81, i32 %reg_82, i32 %reg_83, i32 %reg_84, i32 %reg_85, i32 %reg_86, i32 %reg_87, i32 %reg_88, i32 %reg_89, i32 %reg_90, i32 %reg_91, i32 %reg_92, i32 %reg_93, i32 %reg_94, i32 %reg_95, i32 %reg_96, i32 %reg_97, i32 %reg_98, i32 %reg_99, i32 %reg_100, i32 %reg_101, i32 %reg_102, i32 %reg_103, i32 %reg_104, i32 %reg_105)
	ret i32 %reg_106
	br label %Block3
}

define i32 @param32_arr(ptr %reg_0, ptr %reg_1, ptr %reg_2, ptr %reg_3, ptr %reg_4, ptr %reg_5, ptr %reg_6, ptr %reg_7, ptr %reg_8, ptr %reg_9, ptr %reg_10, ptr %reg_11, ptr %reg_12, ptr %reg_13, ptr %reg_14, ptr %reg_15, ptr %reg_16, ptr %reg_17, ptr %reg_18, ptr %reg_19, ptr %reg_20, ptr %reg_21, ptr %reg_22, ptr %reg_23, ptr %reg_24, ptr %reg_25, ptr %reg_26, ptr %reg_27, ptr %reg_28, ptr %reg_29, ptr %reg_30, ptr %reg_31)
{
Block0:  ;Func define at line 34
	%reg_32 = alloca i32
	br label %Block1
Block1:  ;Func body at line 34
	%reg_33 = add i32 0, 0
	%reg_34 = getelementptr i32, ptr %reg_0, i32 %reg_33
	%reg_35 = load i32, ptr %reg_34
	%reg_36 = add i32 1, 0
	%reg_37 = getelementptr i32, ptr %reg_0, i32 %reg_36
	%reg_38 = load i32, ptr %reg_37
	%reg_39 = add i32 %reg_35, %reg_38
	store i32 %reg_39, ptr %reg_32
	%reg_40 = load i32, ptr %reg_32
	%reg_41 = add i32 0, 0
	%reg_42 = getelementptr i32, ptr %reg_1, i32 %reg_41
	%reg_43 = load i32, ptr %reg_42
	%reg_44 = add i32 %reg_40, %reg_43
	%reg_45 = add i32 1, 0
	%reg_46 = getelementptr i32, ptr %reg_1, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	%reg_48 = add i32 %reg_44, %reg_47
	store i32 %reg_48, ptr %reg_32
	%reg_49 = load i32, ptr %reg_32
	%reg_50 = add i32 0, 0
	%reg_51 = getelementptr i32, ptr %reg_2, i32 %reg_50
	%reg_52 = load i32, ptr %reg_51
	%reg_53 = add i32 %reg_49, %reg_52
	%reg_54 = add i32 1, 0
	%reg_55 = getelementptr i32, ptr %reg_2, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	%reg_57 = add i32 %reg_53, %reg_56
	store i32 %reg_57, ptr %reg_32
	%reg_58 = load i32, ptr %reg_32
	%reg_59 = add i32 0, 0
	%reg_60 = getelementptr i32, ptr %reg_3, i32 %reg_59
	%reg_61 = load i32, ptr %reg_60
	%reg_62 = add i32 %reg_58, %reg_61
	%reg_63 = add i32 1, 0
	%reg_64 = getelementptr i32, ptr %reg_3, i32 %reg_63
	%reg_65 = load i32, ptr %reg_64
	%reg_66 = add i32 %reg_62, %reg_65
	store i32 %reg_66, ptr %reg_32
	%reg_67 = load i32, ptr %reg_32
	%reg_68 = add i32 0, 0
	%reg_69 = getelementptr i32, ptr %reg_4, i32 %reg_68
	%reg_70 = load i32, ptr %reg_69
	%reg_71 = add i32 %reg_67, %reg_70
	%reg_72 = add i32 1, 0
	%reg_73 = getelementptr i32, ptr %reg_4, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	%reg_75 = add i32 %reg_71, %reg_74
	store i32 %reg_75, ptr %reg_32
	%reg_76 = load i32, ptr %reg_32
	%reg_77 = add i32 0, 0
	%reg_78 = getelementptr i32, ptr %reg_5, i32 %reg_77
	%reg_79 = load i32, ptr %reg_78
	%reg_80 = add i32 %reg_76, %reg_79
	%reg_81 = add i32 1, 0
	%reg_82 = getelementptr i32, ptr %reg_5, i32 %reg_81
	%reg_83 = load i32, ptr %reg_82
	%reg_84 = add i32 %reg_80, %reg_83
	store i32 %reg_84, ptr %reg_32
	%reg_85 = load i32, ptr %reg_32
	%reg_86 = add i32 0, 0
	%reg_87 = getelementptr i32, ptr %reg_6, i32 %reg_86
	%reg_88 = load i32, ptr %reg_87
	%reg_89 = add i32 %reg_85, %reg_88
	%reg_90 = add i32 1, 0
	%reg_91 = getelementptr i32, ptr %reg_6, i32 %reg_90
	%reg_92 = load i32, ptr %reg_91
	%reg_93 = add i32 %reg_89, %reg_92
	store i32 %reg_93, ptr %reg_32
	%reg_94 = load i32, ptr %reg_32
	%reg_95 = add i32 0, 0
	%reg_96 = getelementptr i32, ptr %reg_7, i32 %reg_95
	%reg_97 = load i32, ptr %reg_96
	%reg_98 = add i32 %reg_94, %reg_97
	%reg_99 = add i32 1, 0
	%reg_100 = getelementptr i32, ptr %reg_7, i32 %reg_99
	%reg_101 = load i32, ptr %reg_100
	%reg_102 = add i32 %reg_98, %reg_101
	store i32 %reg_102, ptr %reg_32
	%reg_103 = load i32, ptr %reg_32
	%reg_104 = add i32 0, 0
	%reg_105 = getelementptr i32, ptr %reg_8, i32 %reg_104
	%reg_106 = load i32, ptr %reg_105
	%reg_107 = add i32 %reg_103, %reg_106
	%reg_108 = add i32 1, 0
	%reg_109 = getelementptr i32, ptr %reg_8, i32 %reg_108
	%reg_110 = load i32, ptr %reg_109
	%reg_111 = add i32 %reg_107, %reg_110
	store i32 %reg_111, ptr %reg_32
	%reg_112 = load i32, ptr %reg_32
	%reg_113 = add i32 0, 0
	%reg_114 = getelementptr i32, ptr %reg_9, i32 %reg_113
	%reg_115 = load i32, ptr %reg_114
	%reg_116 = add i32 %reg_112, %reg_115
	%reg_117 = add i32 1, 0
	%reg_118 = getelementptr i32, ptr %reg_9, i32 %reg_117
	%reg_119 = load i32, ptr %reg_118
	%reg_120 = add i32 %reg_116, %reg_119
	store i32 %reg_120, ptr %reg_32
	%reg_121 = load i32, ptr %reg_32
	%reg_122 = add i32 0, 0
	%reg_123 = getelementptr i32, ptr %reg_10, i32 %reg_122
	%reg_124 = load i32, ptr %reg_123
	%reg_125 = add i32 %reg_121, %reg_124
	%reg_126 = add i32 1, 0
	%reg_127 = getelementptr i32, ptr %reg_10, i32 %reg_126
	%reg_128 = load i32, ptr %reg_127
	%reg_129 = add i32 %reg_125, %reg_128
	store i32 %reg_129, ptr %reg_32
	%reg_130 = load i32, ptr %reg_32
	%reg_131 = add i32 0, 0
	%reg_132 = getelementptr i32, ptr %reg_11, i32 %reg_131
	%reg_133 = load i32, ptr %reg_132
	%reg_134 = add i32 %reg_130, %reg_133
	%reg_135 = add i32 1, 0
	%reg_136 = getelementptr i32, ptr %reg_11, i32 %reg_135
	%reg_137 = load i32, ptr %reg_136
	%reg_138 = add i32 %reg_134, %reg_137
	store i32 %reg_138, ptr %reg_32
	%reg_139 = load i32, ptr %reg_32
	%reg_140 = add i32 0, 0
	%reg_141 = getelementptr i32, ptr %reg_12, i32 %reg_140
	%reg_142 = load i32, ptr %reg_141
	%reg_143 = add i32 %reg_139, %reg_142
	%reg_144 = add i32 1, 0
	%reg_145 = getelementptr i32, ptr %reg_12, i32 %reg_144
	%reg_146 = load i32, ptr %reg_145
	%reg_147 = add i32 %reg_143, %reg_146
	store i32 %reg_147, ptr %reg_32
	%reg_148 = load i32, ptr %reg_32
	%reg_149 = add i32 0, 0
	%reg_150 = getelementptr i32, ptr %reg_13, i32 %reg_149
	%reg_151 = load i32, ptr %reg_150
	%reg_152 = add i32 %reg_148, %reg_151
	%reg_153 = add i32 1, 0
	%reg_154 = getelementptr i32, ptr %reg_13, i32 %reg_153
	%reg_155 = load i32, ptr %reg_154
	%reg_156 = add i32 %reg_152, %reg_155
	store i32 %reg_156, ptr %reg_32
	%reg_157 = load i32, ptr %reg_32
	%reg_158 = add i32 0, 0
	%reg_159 = getelementptr i32, ptr %reg_14, i32 %reg_158
	%reg_160 = load i32, ptr %reg_159
	%reg_161 = add i32 %reg_157, %reg_160
	%reg_162 = add i32 1, 0
	%reg_163 = getelementptr i32, ptr %reg_14, i32 %reg_162
	%reg_164 = load i32, ptr %reg_163
	%reg_165 = add i32 %reg_161, %reg_164
	store i32 %reg_165, ptr %reg_32
	%reg_166 = load i32, ptr %reg_32
	%reg_167 = add i32 0, 0
	%reg_168 = getelementptr i32, ptr %reg_15, i32 %reg_167
	%reg_169 = load i32, ptr %reg_168
	%reg_170 = add i32 %reg_166, %reg_169
	%reg_171 = add i32 1, 0
	%reg_172 = getelementptr i32, ptr %reg_15, i32 %reg_171
	%reg_173 = load i32, ptr %reg_172
	%reg_174 = add i32 %reg_170, %reg_173
	store i32 %reg_174, ptr %reg_32
	%reg_175 = load i32, ptr %reg_32
	%reg_176 = add i32 0, 0
	%reg_177 = getelementptr i32, ptr %reg_16, i32 %reg_176
	%reg_178 = load i32, ptr %reg_177
	%reg_179 = add i32 %reg_175, %reg_178
	%reg_180 = add i32 1, 0
	%reg_181 = getelementptr i32, ptr %reg_16, i32 %reg_180
	%reg_182 = load i32, ptr %reg_181
	%reg_183 = add i32 %reg_179, %reg_182
	store i32 %reg_183, ptr %reg_32
	%reg_184 = load i32, ptr %reg_32
	%reg_185 = add i32 0, 0
	%reg_186 = getelementptr i32, ptr %reg_17, i32 %reg_185
	%reg_187 = load i32, ptr %reg_186
	%reg_188 = add i32 %reg_184, %reg_187
	%reg_189 = add i32 1, 0
	%reg_190 = getelementptr i32, ptr %reg_17, i32 %reg_189
	%reg_191 = load i32, ptr %reg_190
	%reg_192 = add i32 %reg_188, %reg_191
	store i32 %reg_192, ptr %reg_32
	%reg_193 = load i32, ptr %reg_32
	%reg_194 = add i32 0, 0
	%reg_195 = getelementptr i32, ptr %reg_18, i32 %reg_194
	%reg_196 = load i32, ptr %reg_195
	%reg_197 = add i32 %reg_193, %reg_196
	%reg_198 = add i32 1, 0
	%reg_199 = getelementptr i32, ptr %reg_18, i32 %reg_198
	%reg_200 = load i32, ptr %reg_199
	%reg_201 = add i32 %reg_197, %reg_200
	store i32 %reg_201, ptr %reg_32
	%reg_202 = load i32, ptr %reg_32
	%reg_203 = add i32 0, 0
	%reg_204 = getelementptr i32, ptr %reg_19, i32 %reg_203
	%reg_205 = load i32, ptr %reg_204
	%reg_206 = add i32 %reg_202, %reg_205
	%reg_207 = add i32 1, 0
	%reg_208 = getelementptr i32, ptr %reg_19, i32 %reg_207
	%reg_209 = load i32, ptr %reg_208
	%reg_210 = add i32 %reg_206, %reg_209
	store i32 %reg_210, ptr %reg_32
	%reg_211 = load i32, ptr %reg_32
	%reg_212 = add i32 0, 0
	%reg_213 = getelementptr i32, ptr %reg_20, i32 %reg_212
	%reg_214 = load i32, ptr %reg_213
	%reg_215 = add i32 %reg_211, %reg_214
	%reg_216 = add i32 1, 0
	%reg_217 = getelementptr i32, ptr %reg_20, i32 %reg_216
	%reg_218 = load i32, ptr %reg_217
	%reg_219 = add i32 %reg_215, %reg_218
	store i32 %reg_219, ptr %reg_32
	%reg_220 = load i32, ptr %reg_32
	%reg_221 = add i32 0, 0
	%reg_222 = getelementptr i32, ptr %reg_21, i32 %reg_221
	%reg_223 = load i32, ptr %reg_222
	%reg_224 = add i32 %reg_220, %reg_223
	%reg_225 = add i32 1, 0
	%reg_226 = getelementptr i32, ptr %reg_21, i32 %reg_225
	%reg_227 = load i32, ptr %reg_226
	%reg_228 = add i32 %reg_224, %reg_227
	store i32 %reg_228, ptr %reg_32
	%reg_229 = load i32, ptr %reg_32
	%reg_230 = add i32 0, 0
	%reg_231 = getelementptr i32, ptr %reg_22, i32 %reg_230
	%reg_232 = load i32, ptr %reg_231
	%reg_233 = add i32 %reg_229, %reg_232
	%reg_234 = add i32 1, 0
	%reg_235 = getelementptr i32, ptr %reg_22, i32 %reg_234
	%reg_236 = load i32, ptr %reg_235
	%reg_237 = add i32 %reg_233, %reg_236
	store i32 %reg_237, ptr %reg_32
	%reg_238 = load i32, ptr %reg_32
	%reg_239 = add i32 0, 0
	%reg_240 = getelementptr i32, ptr %reg_23, i32 %reg_239
	%reg_241 = load i32, ptr %reg_240
	%reg_242 = add i32 %reg_238, %reg_241
	%reg_243 = add i32 1, 0
	%reg_244 = getelementptr i32, ptr %reg_23, i32 %reg_243
	%reg_245 = load i32, ptr %reg_244
	%reg_246 = add i32 %reg_242, %reg_245
	store i32 %reg_246, ptr %reg_32
	%reg_247 = load i32, ptr %reg_32
	%reg_248 = add i32 0, 0
	%reg_249 = getelementptr i32, ptr %reg_24, i32 %reg_248
	%reg_250 = load i32, ptr %reg_249
	%reg_251 = add i32 %reg_247, %reg_250
	%reg_252 = add i32 1, 0
	%reg_253 = getelementptr i32, ptr %reg_24, i32 %reg_252
	%reg_254 = load i32, ptr %reg_253
	%reg_255 = add i32 %reg_251, %reg_254
	store i32 %reg_255, ptr %reg_32
	%reg_256 = load i32, ptr %reg_32
	%reg_257 = add i32 0, 0
	%reg_258 = getelementptr i32, ptr %reg_25, i32 %reg_257
	%reg_259 = load i32, ptr %reg_258
	%reg_260 = add i32 %reg_256, %reg_259
	%reg_261 = add i32 1, 0
	%reg_262 = getelementptr i32, ptr %reg_25, i32 %reg_261
	%reg_263 = load i32, ptr %reg_262
	%reg_264 = add i32 %reg_260, %reg_263
	store i32 %reg_264, ptr %reg_32
	%reg_265 = load i32, ptr %reg_32
	%reg_266 = add i32 0, 0
	%reg_267 = getelementptr i32, ptr %reg_26, i32 %reg_266
	%reg_268 = load i32, ptr %reg_267
	%reg_269 = add i32 %reg_265, %reg_268
	%reg_270 = add i32 1, 0
	%reg_271 = getelementptr i32, ptr %reg_26, i32 %reg_270
	%reg_272 = load i32, ptr %reg_271
	%reg_273 = add i32 %reg_269, %reg_272
	store i32 %reg_273, ptr %reg_32
	%reg_274 = load i32, ptr %reg_32
	%reg_275 = add i32 0, 0
	%reg_276 = getelementptr i32, ptr %reg_27, i32 %reg_275
	%reg_277 = load i32, ptr %reg_276
	%reg_278 = add i32 %reg_274, %reg_277
	%reg_279 = add i32 1, 0
	%reg_280 = getelementptr i32, ptr %reg_27, i32 %reg_279
	%reg_281 = load i32, ptr %reg_280
	%reg_282 = add i32 %reg_278, %reg_281
	store i32 %reg_282, ptr %reg_32
	%reg_283 = load i32, ptr %reg_32
	%reg_284 = add i32 0, 0
	%reg_285 = getelementptr i32, ptr %reg_28, i32 %reg_284
	%reg_286 = load i32, ptr %reg_285
	%reg_287 = add i32 %reg_283, %reg_286
	%reg_288 = add i32 1, 0
	%reg_289 = getelementptr i32, ptr %reg_28, i32 %reg_288
	%reg_290 = load i32, ptr %reg_289
	%reg_291 = add i32 %reg_287, %reg_290
	store i32 %reg_291, ptr %reg_32
	%reg_292 = load i32, ptr %reg_32
	%reg_293 = add i32 0, 0
	%reg_294 = getelementptr i32, ptr %reg_29, i32 %reg_293
	%reg_295 = load i32, ptr %reg_294
	%reg_296 = add i32 %reg_292, %reg_295
	%reg_297 = add i32 1, 0
	%reg_298 = getelementptr i32, ptr %reg_29, i32 %reg_297
	%reg_299 = load i32, ptr %reg_298
	%reg_300 = add i32 %reg_296, %reg_299
	store i32 %reg_300, ptr %reg_32
	%reg_301 = load i32, ptr %reg_32
	%reg_302 = add i32 0, 0
	%reg_303 = getelementptr i32, ptr %reg_30, i32 %reg_302
	%reg_304 = load i32, ptr %reg_303
	%reg_305 = add i32 %reg_301, %reg_304
	%reg_306 = add i32 1, 0
	%reg_307 = getelementptr i32, ptr %reg_30, i32 %reg_306
	%reg_308 = load i32, ptr %reg_307
	%reg_309 = add i32 %reg_305, %reg_308
	store i32 %reg_309, ptr %reg_32
	%reg_310 = load i32, ptr %reg_32
	%reg_311 = add i32 0, 0
	%reg_312 = getelementptr i32, ptr %reg_31, i32 %reg_311
	%reg_313 = load i32, ptr %reg_312
	%reg_314 = add i32 %reg_310, %reg_313
	%reg_315 = add i32 1, 0
	%reg_316 = getelementptr i32, ptr %reg_31, i32 %reg_315
	%reg_317 = load i32, ptr %reg_316
	%reg_318 = add i32 %reg_314, %reg_317
	store i32 %reg_318, ptr %reg_32
	%reg_319 = load i32, ptr %reg_32
	ret i32 %reg_319
}

define i32 @param16(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, i32 %reg_7, i32 %reg_8, i32 %reg_9, i32 %reg_10, i32 %reg_11, i32 %reg_12, i32 %reg_13, i32 %reg_14, i32 %reg_15)
{
Block0:  ;Func define at line 75
	%reg_32 = alloca [16 x i32]
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
Block1:  ;Func body at line 75
	call void @llvm.memset.p0.i32(ptr %reg_32, i8 0, i32 64, i1 1)
	%reg_33 = load i32, ptr %reg_16
	%reg_34 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 0
	store i32 %reg_33, ptr %reg_34
	%reg_35 = load i32, ptr %reg_17
	%reg_36 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 1
	store i32 %reg_35, ptr %reg_36
	%reg_37 = load i32, ptr %reg_18
	%reg_38 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 2
	store i32 %reg_37, ptr %reg_38
	%reg_39 = load i32, ptr %reg_19
	%reg_40 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 3
	store i32 %reg_39, ptr %reg_40
	%reg_41 = load i32, ptr %reg_20
	%reg_42 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 4
	store i32 %reg_41, ptr %reg_42
	%reg_43 = load i32, ptr %reg_21
	%reg_44 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 5
	store i32 %reg_43, ptr %reg_44
	%reg_45 = load i32, ptr %reg_22
	%reg_46 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 6
	store i32 %reg_45, ptr %reg_46
	%reg_47 = load i32, ptr %reg_23
	%reg_48 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 7
	store i32 %reg_47, ptr %reg_48
	%reg_49 = load i32, ptr %reg_24
	%reg_50 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 8
	store i32 %reg_49, ptr %reg_50
	%reg_51 = load i32, ptr %reg_25
	%reg_52 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 9
	store i32 %reg_51, ptr %reg_52
	%reg_53 = load i32, ptr %reg_26
	%reg_54 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 10
	store i32 %reg_53, ptr %reg_54
	%reg_55 = load i32, ptr %reg_27
	%reg_56 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 11
	store i32 %reg_55, ptr %reg_56
	%reg_57 = load i32, ptr %reg_28
	%reg_58 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 12
	store i32 %reg_57, ptr %reg_58
	%reg_59 = load i32, ptr %reg_29
	%reg_60 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 13
	store i32 %reg_59, ptr %reg_60
	%reg_61 = load i32, ptr %reg_30
	%reg_62 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 14
	store i32 %reg_61, ptr %reg_62
	%reg_63 = load i32, ptr %reg_31
	%reg_64 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 15
	store i32 %reg_63, ptr %reg_64
	%reg_65 = getelementptr [16 x i32], ptr %reg_32, i32 0
	%reg_66 = add i32 16, 0
	call void @sort(ptr %reg_65, i32 %reg_66)
	%reg_67 = add i32 0, 0
	%reg_68 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_67
	%reg_69 = load i32, ptr %reg_68
	%reg_70 = add i32 1, 0
	%reg_71 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_70
	%reg_72 = load i32, ptr %reg_71
	%reg_73 = add i32 2, 0
	%reg_74 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_73
	%reg_75 = load i32, ptr %reg_74
	%reg_76 = add i32 3, 0
	%reg_77 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_76
	%reg_78 = load i32, ptr %reg_77
	%reg_79 = add i32 4, 0
	%reg_80 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_79
	%reg_81 = load i32, ptr %reg_80
	%reg_82 = add i32 5, 0
	%reg_83 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_82
	%reg_84 = load i32, ptr %reg_83
	%reg_85 = add i32 6, 0
	%reg_86 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_85
	%reg_87 = load i32, ptr %reg_86
	%reg_88 = add i32 7, 0
	%reg_89 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_88
	%reg_90 = load i32, ptr %reg_89
	%reg_91 = add i32 8, 0
	%reg_92 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_91
	%reg_93 = load i32, ptr %reg_92
	%reg_94 = add i32 9, 0
	%reg_95 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_94
	%reg_96 = load i32, ptr %reg_95
	%reg_97 = add i32 10, 0
	%reg_98 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_97
	%reg_99 = load i32, ptr %reg_98
	%reg_100 = add i32 11, 0
	%reg_101 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_100
	%reg_102 = load i32, ptr %reg_101
	%reg_103 = add i32 12, 0
	%reg_104 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_103
	%reg_105 = load i32, ptr %reg_104
	%reg_106 = add i32 13, 0
	%reg_107 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_106
	%reg_108 = load i32, ptr %reg_107
	%reg_109 = add i32 14, 0
	%reg_110 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_109
	%reg_111 = load i32, ptr %reg_110
	%reg_112 = add i32 15, 0
	%reg_113 = getelementptr [16 x i32], ptr %reg_32, i32 0, i32 %reg_112
	%reg_114 = load i32, ptr %reg_113
	%reg_115 = load i32, ptr %reg_16
	%reg_116 = load i32, ptr %reg_17
	%reg_117 = load i32, ptr %reg_18
	%reg_118 = load i32, ptr %reg_19
	%reg_119 = load i32, ptr %reg_20
	%reg_120 = load i32, ptr %reg_21
	%reg_121 = load i32, ptr %reg_22
	%reg_122 = load i32, ptr %reg_23
	%reg_123 = load i32, ptr %reg_24
	%reg_124 = load i32, ptr %reg_25
	%reg_125 = load i32, ptr %reg_26
	%reg_126 = load i32, ptr %reg_27
	%reg_127 = load i32, ptr %reg_28
	%reg_128 = load i32, ptr %reg_29
	%reg_129 = load i32, ptr %reg_30
	%reg_130 = load i32, ptr %reg_31
	%reg_131 = call i32 @param32_rec(i32 %reg_69, i32 %reg_72, i32 %reg_75, i32 %reg_78, i32 %reg_81, i32 %reg_84, i32 %reg_87, i32 %reg_90, i32 %reg_93, i32 %reg_96, i32 %reg_99, i32 %reg_102, i32 %reg_105, i32 %reg_108, i32 %reg_111, i32 %reg_114, i32 %reg_115, i32 %reg_116, i32 %reg_117, i32 %reg_118, i32 %reg_119, i32 %reg_120, i32 %reg_121, i32 %reg_122, i32 %reg_123, i32 %reg_124, i32 %reg_125, i32 %reg_126, i32 %reg_127, i32 %reg_128, i32 %reg_129, i32 %reg_130)
	ret i32 %reg_131
}

define i32 @main()
{
Block0:  ;Func define at line 87
	%reg_21 = alloca i32
	%reg_0 = alloca [32 x [2 x i32]]
	br label %Block1
Block1:  ;Func body at line 87
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 256, i1 1)
	%reg_1 = call i32 @getint()
	%reg_2 = call i32 @getint()
	%reg_3 = call i32 @getint()
	%reg_4 = call i32 @getint()
	%reg_5 = call i32 @getint()
	%reg_6 = call i32 @getint()
	%reg_7 = call i32 @getint()
	%reg_8 = call i32 @getint()
	%reg_9 = call i32 @getint()
	%reg_10 = call i32 @getint()
	%reg_11 = call i32 @getint()
	%reg_12 = call i32 @getint()
	%reg_13 = call i32 @getint()
	%reg_14 = call i32 @getint()
	%reg_15 = call i32 @getint()
	%reg_16 = call i32 @getint()
	%reg_17 = call i32 @param16(i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, i32 %reg_7, i32 %reg_8, i32 %reg_9, i32 %reg_10, i32 %reg_11, i32 %reg_12, i32 %reg_13, i32 %reg_14, i32 %reg_15, i32 %reg_16)
	%reg_18 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 0, i32 0
	store i32 %reg_17, ptr %reg_18
	%reg_19 = add i32 8848, 0
	%reg_20 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 0, i32 1
	store i32 %reg_19, ptr %reg_20
	%reg_22 = add i32 1, 0
	store i32 %reg_22, ptr %reg_21
	br label %Block2
Block2:  ;While condition at line 94
	%reg_23 = load i32, ptr %reg_21
	%reg_24 = add i32 32, 0
	%reg_25 = icmp slt i32 %reg_23, %reg_24
	br i1 %reg_25, label %Block3, label %Block4
Block3:  ;While body at line 94
	%reg_26 = load i32, ptr %reg_21
	%reg_27 = add i32 0, 0
	%reg_28 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_26, i32 %reg_27
	%reg_29 = load i32, ptr %reg_21
	%reg_30 = add i32 1, 0
	%reg_31 = sub i32 %reg_29, %reg_30
	%reg_32 = add i32 1, 0
	%reg_33 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_31, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = add i32 1, 0
	%reg_36 = sub i32 %reg_34, %reg_35
	store i32 %reg_36, ptr %reg_28
	%reg_37 = load i32, ptr %reg_21
	%reg_38 = add i32 1, 0
	%reg_39 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_37, i32 %reg_38
	%reg_40 = load i32, ptr %reg_21
	%reg_41 = add i32 1, 0
	%reg_42 = sub i32 %reg_40, %reg_41
	%reg_43 = add i32 0, 0
	%reg_44 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_42, i32 %reg_43
	%reg_45 = load i32, ptr %reg_44
	%reg_46 = add i32 2, 0
	%reg_47 = sub i32 %reg_45, %reg_46
	store i32 %reg_47, ptr %reg_39
	%reg_48 = load i32, ptr %reg_21
	%reg_49 = add i32 1, 0
	%reg_50 = add i32 %reg_48, %reg_49
	store i32 %reg_50, ptr %reg_21
	br label %Block2
Block4:  ;While end at line 94
	%reg_51 = add i32 0, 0
	%reg_52 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_51
	%reg_53 = add i32 1, 0
	%reg_54 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_53
	%reg_55 = add i32 2, 0
	%reg_56 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_55
	%reg_57 = add i32 3, 0
	%reg_58 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_57
	%reg_59 = add i32 4, 0
	%reg_60 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_59
	%reg_61 = add i32 5, 0
	%reg_62 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_61
	%reg_63 = add i32 6, 0
	%reg_64 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_63
	%reg_65 = add i32 7, 0
	%reg_66 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_65
	%reg_67 = add i32 8, 0
	%reg_68 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_67
	%reg_69 = add i32 9, 0
	%reg_70 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_69
	%reg_71 = add i32 10, 0
	%reg_72 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_71
	%reg_73 = add i32 11, 0
	%reg_74 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_73
	%reg_75 = add i32 12, 0
	%reg_76 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_75
	%reg_77 = add i32 13, 0
	%reg_78 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_77
	%reg_79 = add i32 14, 0
	%reg_80 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_79
	%reg_81 = add i32 15, 0
	%reg_82 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_81
	%reg_83 = add i32 16, 0
	%reg_84 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_83
	%reg_85 = add i32 17, 0
	%reg_86 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_85
	%reg_87 = add i32 18, 0
	%reg_88 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_87
	%reg_89 = add i32 19, 0
	%reg_90 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_89
	%reg_91 = add i32 20, 0
	%reg_92 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_91
	%reg_93 = add i32 21, 0
	%reg_94 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_93
	%reg_95 = add i32 22, 0
	%reg_96 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_95
	%reg_97 = add i32 23, 0
	%reg_98 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_97
	%reg_99 = add i32 24, 0
	%reg_100 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_99
	%reg_101 = add i32 25, 0
	%reg_102 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_101
	%reg_103 = add i32 26, 0
	%reg_104 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_103
	%reg_105 = add i32 27, 0
	%reg_106 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_105
	%reg_107 = add i32 28, 0
	%reg_108 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_107
	%reg_109 = add i32 29, 0
	%reg_110 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_109
	%reg_111 = add i32 30, 0
	%reg_112 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_111
	%reg_113 = add i32 31, 0
	%reg_114 = getelementptr [32 x [2 x i32]], ptr %reg_0, i32 0, i32 %reg_113
	%reg_115 = call i32 @param32_arr(ptr %reg_52, ptr %reg_54, ptr %reg_56, ptr %reg_58, ptr %reg_60, ptr %reg_62, ptr %reg_64, ptr %reg_66, ptr %reg_68, ptr %reg_70, ptr %reg_72, ptr %reg_74, ptr %reg_76, ptr %reg_78, ptr %reg_80, ptr %reg_82, ptr %reg_84, ptr %reg_86, ptr %reg_88, ptr %reg_90, ptr %reg_92, ptr %reg_94, ptr %reg_96, ptr %reg_98, ptr %reg_100, ptr %reg_102, ptr %reg_104, ptr %reg_106, ptr %reg_108, ptr %reg_110, ptr %reg_112, ptr %reg_114)
	call void @putint(i32 %reg_115)
	%reg_116 = add i32 10, 0
	call void @putch(i32 %reg_116)
	%reg_117 = add i32 0, 0
	ret i32 %reg_117
}
