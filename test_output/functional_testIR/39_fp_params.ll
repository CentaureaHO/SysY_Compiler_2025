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

@k = global i32 zeroinitializer

define float @params_f40(float %reg_0, float %reg_1, float %reg_2, float %reg_3, float %reg_4, float %reg_5, float %reg_6, float %reg_7, float %reg_8, float %reg_9, float %reg_10, float %reg_11, float %reg_12, float %reg_13, float %reg_14, float %reg_15, float %reg_16, float %reg_17, float %reg_18, float %reg_19, float %reg_20, float %reg_21, float %reg_22, float %reg_23, float %reg_24, float %reg_25, float %reg_26, float %reg_27, float %reg_28, float %reg_29, float %reg_30, float %reg_31, float %reg_32, float %reg_33, float %reg_34, float %reg_35, float %reg_36, float %reg_37, float %reg_38, float %reg_39)
{
Block0:  ;Func define at line 3
	%reg_84 = alloca [10 x float]
	%reg_79 = alloca float
	%reg_78 = alloca float
	%reg_77 = alloca float
	%reg_76 = alloca float
	%reg_75 = alloca float
	%reg_74 = alloca float
	%reg_73 = alloca float
	%reg_72 = alloca float
	%reg_71 = alloca float
	%reg_70 = alloca float
	%reg_69 = alloca float
	%reg_68 = alloca float
	%reg_67 = alloca float
	%reg_66 = alloca float
	%reg_65 = alloca float
	%reg_64 = alloca float
	%reg_63 = alloca float
	%reg_62 = alloca float
	%reg_61 = alloca float
	%reg_60 = alloca float
	%reg_59 = alloca float
	%reg_58 = alloca float
	%reg_57 = alloca float
	%reg_56 = alloca float
	%reg_55 = alloca float
	%reg_54 = alloca float
	%reg_53 = alloca float
	%reg_52 = alloca float
	%reg_51 = alloca float
	%reg_50 = alloca float
	%reg_49 = alloca float
	%reg_48 = alloca float
	%reg_47 = alloca float
	%reg_46 = alloca float
	%reg_45 = alloca float
	%reg_44 = alloca float
	%reg_43 = alloca float
	%reg_42 = alloca float
	%reg_41 = alloca float
	%reg_40 = alloca float
	store float %reg_0, ptr %reg_40
	store float %reg_1, ptr %reg_41
	store float %reg_2, ptr %reg_42
	store float %reg_3, ptr %reg_43
	store float %reg_4, ptr %reg_44
	store float %reg_5, ptr %reg_45
	store float %reg_6, ptr %reg_46
	store float %reg_7, ptr %reg_47
	store float %reg_8, ptr %reg_48
	store float %reg_9, ptr %reg_49
	store float %reg_10, ptr %reg_50
	store float %reg_11, ptr %reg_51
	store float %reg_12, ptr %reg_52
	store float %reg_13, ptr %reg_53
	store float %reg_14, ptr %reg_54
	store float %reg_15, ptr %reg_55
	store float %reg_16, ptr %reg_56
	store float %reg_17, ptr %reg_57
	store float %reg_18, ptr %reg_58
	store float %reg_19, ptr %reg_59
	store float %reg_20, ptr %reg_60
	store float %reg_21, ptr %reg_61
	store float %reg_22, ptr %reg_62
	store float %reg_23, ptr %reg_63
	store float %reg_24, ptr %reg_64
	store float %reg_25, ptr %reg_65
	store float %reg_26, ptr %reg_66
	store float %reg_27, ptr %reg_67
	store float %reg_28, ptr %reg_68
	store float %reg_29, ptr %reg_69
	store float %reg_30, ptr %reg_70
	store float %reg_31, ptr %reg_71
	store float %reg_32, ptr %reg_72
	store float %reg_33, ptr %reg_73
	store float %reg_34, ptr %reg_74
	store float %reg_35, ptr %reg_75
	store float %reg_36, ptr %reg_76
	store float %reg_37, ptr %reg_77
	store float %reg_38, ptr %reg_78
	store float %reg_39, ptr %reg_79
	br label %Block1
Block1:  ;Func body at line 3
	%reg_80 = load float, ptr %reg_79
	%reg_81 = add i32 0, 0
	%reg_82 = sitofp i32 %reg_81 to float
	%reg_83 = fcmp one float %reg_80, %reg_82
	br i1 %reg_83, label %Block2, label %Block4
Block2:  ;If then at line 11
	call void @llvm.memset.p0.i32(ptr %reg_84, i8 0, i32 40, i1 1)
	%reg_85 = load float, ptr %reg_40
	%reg_86 = load float, ptr %reg_41
	%reg_87 = fadd float %reg_85, %reg_86
	%reg_88 = load float, ptr %reg_42
	%reg_89 = fadd float %reg_87, %reg_88
	%reg_90 = load float, ptr %reg_43
	%reg_91 = fadd float %reg_89, %reg_90
	%reg_92 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 0
	store float %reg_91, ptr %reg_92
	%reg_93 = load float, ptr %reg_44
	%reg_94 = load float, ptr %reg_45
	%reg_95 = fadd float %reg_93, %reg_94
	%reg_96 = load float, ptr %reg_46
	%reg_97 = fadd float %reg_95, %reg_96
	%reg_98 = load float, ptr %reg_47
	%reg_99 = fadd float %reg_97, %reg_98
	%reg_100 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 1
	store float %reg_99, ptr %reg_100
	%reg_101 = load float, ptr %reg_48
	%reg_102 = load float, ptr %reg_49
	%reg_103 = fadd float %reg_101, %reg_102
	%reg_104 = load float, ptr %reg_50
	%reg_105 = fadd float %reg_103, %reg_104
	%reg_106 = load float, ptr %reg_51
	%reg_107 = fadd float %reg_105, %reg_106
	%reg_108 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 2
	store float %reg_107, ptr %reg_108
	%reg_109 = load float, ptr %reg_52
	%reg_110 = load float, ptr %reg_53
	%reg_111 = fadd float %reg_109, %reg_110
	%reg_112 = load float, ptr %reg_54
	%reg_113 = fadd float %reg_111, %reg_112
	%reg_114 = load float, ptr %reg_55
	%reg_115 = fadd float %reg_113, %reg_114
	%reg_116 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 3
	store float %reg_115, ptr %reg_116
	%reg_117 = load float, ptr %reg_56
	%reg_118 = load float, ptr %reg_57
	%reg_119 = fadd float %reg_117, %reg_118
	%reg_120 = load float, ptr %reg_58
	%reg_121 = fadd float %reg_119, %reg_120
	%reg_122 = load float, ptr %reg_59
	%reg_123 = fadd float %reg_121, %reg_122
	%reg_124 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 4
	store float %reg_123, ptr %reg_124
	%reg_125 = load float, ptr %reg_60
	%reg_126 = load float, ptr %reg_61
	%reg_127 = fadd float %reg_125, %reg_126
	%reg_128 = load float, ptr %reg_62
	%reg_129 = fadd float %reg_127, %reg_128
	%reg_130 = load float, ptr %reg_63
	%reg_131 = fadd float %reg_129, %reg_130
	%reg_132 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 5
	store float %reg_131, ptr %reg_132
	%reg_133 = load float, ptr %reg_64
	%reg_134 = load float, ptr %reg_65
	%reg_135 = fadd float %reg_133, %reg_134
	%reg_136 = load float, ptr %reg_66
	%reg_137 = fadd float %reg_135, %reg_136
	%reg_138 = load float, ptr %reg_67
	%reg_139 = fadd float %reg_137, %reg_138
	%reg_140 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 6
	store float %reg_139, ptr %reg_140
	%reg_141 = load float, ptr %reg_68
	%reg_142 = load float, ptr %reg_69
	%reg_143 = fadd float %reg_141, %reg_142
	%reg_144 = load float, ptr %reg_70
	%reg_145 = fadd float %reg_143, %reg_144
	%reg_146 = load float, ptr %reg_71
	%reg_147 = fadd float %reg_145, %reg_146
	%reg_148 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 7
	store float %reg_147, ptr %reg_148
	%reg_149 = load float, ptr %reg_72
	%reg_150 = load float, ptr %reg_73
	%reg_151 = fadd float %reg_149, %reg_150
	%reg_152 = load float, ptr %reg_74
	%reg_153 = fadd float %reg_151, %reg_152
	%reg_154 = load float, ptr %reg_75
	%reg_155 = fadd float %reg_153, %reg_154
	%reg_156 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 8
	store float %reg_155, ptr %reg_156
	%reg_157 = load float, ptr %reg_76
	%reg_158 = load float, ptr %reg_77
	%reg_159 = fadd float %reg_157, %reg_158
	%reg_160 = load float, ptr %reg_78
	%reg_161 = fadd float %reg_159, %reg_160
	%reg_162 = load float, ptr %reg_79
	%reg_163 = fadd float %reg_161, %reg_162
	%reg_164 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 9
	store float %reg_163, ptr %reg_164
	%reg_165 = add i32 10, 0
	%reg_166 = getelementptr [10 x float], ptr %reg_84, i32 0
	call void @putfarray(i32 %reg_165, ptr %reg_166)
	%reg_167 = load i32, ptr @k
	%reg_168 = getelementptr [10 x float], ptr %reg_84, i32 0, i32 %reg_167
	%reg_169 = load float, ptr %reg_168
	ret float %reg_169
	br label %Block3
Block3:  ;If end at line 11
	ret float 0x3fd3333340000000
Block4:  ;If else at line 11
	%reg_170 = load float, ptr %reg_41
	%reg_171 = load float, ptr %reg_42
	%reg_172 = load float, ptr %reg_43
	%reg_173 = load float, ptr %reg_44
	%reg_174 = load float, ptr %reg_45
	%reg_175 = load float, ptr %reg_46
	%reg_176 = load float, ptr %reg_47
	%reg_177 = load float, ptr %reg_48
	%reg_178 = load float, ptr %reg_49
	%reg_179 = load float, ptr %reg_50
	%reg_180 = load float, ptr %reg_51
	%reg_181 = load float, ptr %reg_52
	%reg_182 = load float, ptr %reg_53
	%reg_183 = load float, ptr %reg_54
	%reg_184 = load float, ptr %reg_55
	%reg_185 = load float, ptr %reg_56
	%reg_186 = load float, ptr %reg_57
	%reg_187 = load float, ptr %reg_58
	%reg_188 = load float, ptr %reg_59
	%reg_189 = load float, ptr %reg_60
	%reg_190 = load float, ptr %reg_61
	%reg_191 = load float, ptr %reg_62
	%reg_192 = load float, ptr %reg_63
	%reg_193 = load float, ptr %reg_64
	%reg_194 = load float, ptr %reg_65
	%reg_195 = load float, ptr %reg_66
	%reg_196 = load float, ptr %reg_67
	%reg_197 = load float, ptr %reg_68
	%reg_198 = load float, ptr %reg_69
	%reg_199 = load float, ptr %reg_70
	%reg_200 = load float, ptr %reg_71
	%reg_201 = load float, ptr %reg_72
	%reg_202 = load float, ptr %reg_73
	%reg_203 = load float, ptr %reg_74
	%reg_204 = load float, ptr %reg_75
	%reg_205 = load float, ptr %reg_76
	%reg_206 = load float, ptr %reg_77
	%reg_207 = load float, ptr %reg_78
	%reg_208 = load float, ptr %reg_79
	%reg_209 = load float, ptr %reg_40
	%reg_210 = load float, ptr %reg_41
	%reg_211 = fadd float %reg_209, %reg_210
	%reg_212 = load float, ptr %reg_42
	%reg_213 = fadd float %reg_211, %reg_212
	%reg_214 = call float @params_f40(float %reg_170, float %reg_171, float %reg_172, float %reg_173, float %reg_174, float %reg_175, float %reg_176, float %reg_177, float %reg_178, float %reg_179, float %reg_180, float %reg_181, float %reg_182, float %reg_183, float %reg_184, float %reg_185, float %reg_186, float %reg_187, float %reg_188, float %reg_189, float %reg_190, float %reg_191, float %reg_192, float %reg_193, float %reg_194, float %reg_195, float %reg_196, float %reg_197, float %reg_198, float %reg_199, float %reg_200, float %reg_201, float %reg_202, float %reg_203, float %reg_204, float %reg_205, float %reg_206, float %reg_207, float %reg_208, float %reg_213)
	ret float %reg_214
	br label %Block3
}

define float @params_f40_i24(i32 %reg_0, i32 %reg_1, i32 %reg_2, float %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6, float %reg_7, float %reg_8, float %reg_9, i32 %reg_10, float %reg_11, float %reg_12, i32 %reg_13, float %reg_14, i32 %reg_15, float %reg_16, float %reg_17, float %reg_18, float %reg_19, float %reg_20, float %reg_21, i32 %reg_22, float %reg_23, i32 %reg_24, i32 %reg_25, float %reg_26, float %reg_27, float %reg_28, float %reg_29, float %reg_30, i32 %reg_31, float %reg_32, i32 %reg_33, float %reg_34, float %reg_35, float %reg_36, float %reg_37, i32 %reg_38, i32 %reg_39, float %reg_40, float %reg_41, float %reg_42, i32 %reg_43, float %reg_44, i32 %reg_45, i32 %reg_46, float %reg_47, float %reg_48, float %reg_49, float %reg_50, i32 %reg_51, i32 %reg_52, i32 %reg_53, float %reg_54, float %reg_55, float %reg_56, float %reg_57, float %reg_58, float %reg_59, i32 %reg_60, float %reg_61, i32 %reg_62, float %reg_63)
{
Block0:  ;Func define at line 27
	%reg_265 = alloca i32
	%reg_212 = alloca [8 x i32]
	%reg_131 = alloca [10 x float]
	%reg_127 = alloca float
	%reg_126 = alloca i32
	%reg_125 = alloca float
	%reg_124 = alloca i32
	%reg_123 = alloca float
	%reg_122 = alloca float
	%reg_121 = alloca float
	%reg_120 = alloca float
	%reg_119 = alloca float
	%reg_118 = alloca float
	%reg_117 = alloca i32
	%reg_116 = alloca i32
	%reg_115 = alloca i32
	%reg_114 = alloca float
	%reg_113 = alloca float
	%reg_112 = alloca float
	%reg_111 = alloca float
	%reg_110 = alloca i32
	%reg_109 = alloca i32
	%reg_108 = alloca float
	%reg_107 = alloca i32
	%reg_106 = alloca float
	%reg_105 = alloca float
	%reg_104 = alloca float
	%reg_103 = alloca i32
	%reg_102 = alloca i32
	%reg_101 = alloca float
	%reg_100 = alloca float
	%reg_99 = alloca float
	%reg_98 = alloca float
	%reg_97 = alloca i32
	%reg_96 = alloca float
	%reg_95 = alloca i32
	%reg_94 = alloca float
	%reg_93 = alloca float
	%reg_92 = alloca float
	%reg_91 = alloca float
	%reg_90 = alloca float
	%reg_89 = alloca i32
	%reg_88 = alloca i32
	%reg_87 = alloca float
	%reg_86 = alloca i32
	%reg_85 = alloca float
	%reg_84 = alloca float
	%reg_83 = alloca float
	%reg_82 = alloca float
	%reg_81 = alloca float
	%reg_80 = alloca float
	%reg_79 = alloca i32
	%reg_78 = alloca float
	%reg_77 = alloca i32
	%reg_76 = alloca float
	%reg_75 = alloca float
	%reg_74 = alloca i32
	%reg_73 = alloca float
	%reg_72 = alloca float
	%reg_71 = alloca float
	%reg_70 = alloca i32
	%reg_69 = alloca i32
	%reg_68 = alloca i32
	%reg_67 = alloca float
	%reg_66 = alloca i32
	%reg_65 = alloca i32
	%reg_64 = alloca i32
	store i32 %reg_0, ptr %reg_64
	store i32 %reg_1, ptr %reg_65
	store i32 %reg_2, ptr %reg_66
	store float %reg_3, ptr %reg_67
	store i32 %reg_4, ptr %reg_68
	store i32 %reg_5, ptr %reg_69
	store i32 %reg_6, ptr %reg_70
	store float %reg_7, ptr %reg_71
	store float %reg_8, ptr %reg_72
	store float %reg_9, ptr %reg_73
	store i32 %reg_10, ptr %reg_74
	store float %reg_11, ptr %reg_75
	store float %reg_12, ptr %reg_76
	store i32 %reg_13, ptr %reg_77
	store float %reg_14, ptr %reg_78
	store i32 %reg_15, ptr %reg_79
	store float %reg_16, ptr %reg_80
	store float %reg_17, ptr %reg_81
	store float %reg_18, ptr %reg_82
	store float %reg_19, ptr %reg_83
	store float %reg_20, ptr %reg_84
	store float %reg_21, ptr %reg_85
	store i32 %reg_22, ptr %reg_86
	store float %reg_23, ptr %reg_87
	store i32 %reg_24, ptr %reg_88
	store i32 %reg_25, ptr %reg_89
	store float %reg_26, ptr %reg_90
	store float %reg_27, ptr %reg_91
	store float %reg_28, ptr %reg_92
	store float %reg_29, ptr %reg_93
	store float %reg_30, ptr %reg_94
	store i32 %reg_31, ptr %reg_95
	store float %reg_32, ptr %reg_96
	store i32 %reg_33, ptr %reg_97
	store float %reg_34, ptr %reg_98
	store float %reg_35, ptr %reg_99
	store float %reg_36, ptr %reg_100
	store float %reg_37, ptr %reg_101
	store i32 %reg_38, ptr %reg_102
	store i32 %reg_39, ptr %reg_103
	store float %reg_40, ptr %reg_104
	store float %reg_41, ptr %reg_105
	store float %reg_42, ptr %reg_106
	store i32 %reg_43, ptr %reg_107
	store float %reg_44, ptr %reg_108
	store i32 %reg_45, ptr %reg_109
	store i32 %reg_46, ptr %reg_110
	store float %reg_47, ptr %reg_111
	store float %reg_48, ptr %reg_112
	store float %reg_49, ptr %reg_113
	store float %reg_50, ptr %reg_114
	store i32 %reg_51, ptr %reg_115
	store i32 %reg_52, ptr %reg_116
	store i32 %reg_53, ptr %reg_117
	store float %reg_54, ptr %reg_118
	store float %reg_55, ptr %reg_119
	store float %reg_56, ptr %reg_120
	store float %reg_57, ptr %reg_121
	store float %reg_58, ptr %reg_122
	store float %reg_59, ptr %reg_123
	store i32 %reg_60, ptr %reg_124
	store float %reg_61, ptr %reg_125
	store i32 %reg_62, ptr %reg_126
	store float %reg_63, ptr %reg_127
	br label %Block1
Block1:  ;Func body at line 27
	%reg_128 = load i32, ptr %reg_64
	%reg_129 = add i32 0, 0
	%reg_130 = icmp ne i32 %reg_128, %reg_129
	br i1 %reg_130, label %Block2, label %Block4
Block2:  ;If then at line 39
	call void @llvm.memset.p0.i32(ptr %reg_131, i8 0, i32 40, i1 1)
	%reg_132 = load float, ptr %reg_106
	%reg_133 = load float, ptr %reg_82
	%reg_134 = fadd float %reg_132, %reg_133
	%reg_135 = load float, ptr %reg_125
	%reg_136 = fadd float %reg_134, %reg_135
	%reg_137 = load float, ptr %reg_75
	%reg_138 = fadd float %reg_136, %reg_137
	%reg_139 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 0
	store float %reg_138, ptr %reg_139
	%reg_140 = load float, ptr %reg_67
	%reg_141 = load float, ptr %reg_100
	%reg_142 = fadd float %reg_140, %reg_141
	%reg_143 = load float, ptr %reg_85
	%reg_144 = fadd float %reg_142, %reg_143
	%reg_145 = load float, ptr %reg_73
	%reg_146 = fadd float %reg_144, %reg_145
	%reg_147 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 1
	store float %reg_146, ptr %reg_147
	%reg_148 = load float, ptr %reg_71
	%reg_149 = load float, ptr %reg_93
	%reg_150 = fadd float %reg_148, %reg_149
	%reg_151 = load float, ptr %reg_105
	%reg_152 = fadd float %reg_150, %reg_151
	%reg_153 = load float, ptr %reg_83
	%reg_154 = fadd float %reg_152, %reg_153
	%reg_155 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 2
	store float %reg_154, ptr %reg_155
	%reg_156 = load float, ptr %reg_81
	%reg_157 = load float, ptr %reg_113
	%reg_158 = fadd float %reg_156, %reg_157
	%reg_159 = load float, ptr %reg_104
	%reg_160 = fadd float %reg_158, %reg_159
	%reg_161 = load float, ptr %reg_72
	%reg_162 = fadd float %reg_160, %reg_161
	%reg_163 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 3
	store float %reg_162, ptr %reg_163
	%reg_164 = load float, ptr %reg_99
	%reg_165 = load float, ptr %reg_123
	%reg_166 = fadd float %reg_164, %reg_165
	%reg_167 = load float, ptr %reg_118
	%reg_168 = fadd float %reg_166, %reg_167
	%reg_169 = load float, ptr %reg_119
	%reg_170 = fadd float %reg_168, %reg_169
	%reg_171 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 4
	store float %reg_170, ptr %reg_171
	%reg_172 = load float, ptr %reg_90
	%reg_173 = load float, ptr %reg_98
	%reg_174 = fadd float %reg_172, %reg_173
	%reg_175 = load float, ptr %reg_120
	%reg_176 = fadd float %reg_174, %reg_175
	%reg_177 = load float, ptr %reg_92
	%reg_178 = fadd float %reg_176, %reg_177
	%reg_179 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 5
	store float %reg_178, ptr %reg_179
	%reg_180 = load float, ptr %reg_112
	%reg_181 = load float, ptr %reg_94
	%reg_182 = fadd float %reg_180, %reg_181
	%reg_183 = load float, ptr %reg_121
	%reg_184 = fadd float %reg_182, %reg_183
	%reg_185 = load float, ptr %reg_111
	%reg_186 = fadd float %reg_184, %reg_185
	%reg_187 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 6
	store float %reg_186, ptr %reg_187
	%reg_188 = load float, ptr %reg_76
	%reg_189 = load float, ptr %reg_114
	%reg_190 = fadd float %reg_188, %reg_189
	%reg_191 = load float, ptr %reg_80
	%reg_192 = fadd float %reg_190, %reg_191
	%reg_193 = load float, ptr %reg_108
	%reg_194 = fadd float %reg_192, %reg_193
	%reg_195 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 7
	store float %reg_194, ptr %reg_195
	%reg_196 = load float, ptr %reg_87
	%reg_197 = load float, ptr %reg_91
	%reg_198 = fadd float %reg_196, %reg_197
	%reg_199 = load float, ptr %reg_101
	%reg_200 = fadd float %reg_198, %reg_199
	%reg_201 = load float, ptr %reg_127
	%reg_202 = fadd float %reg_200, %reg_201
	%reg_203 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 8
	store float %reg_202, ptr %reg_203
	%reg_204 = load float, ptr %reg_122
	%reg_205 = load float, ptr %reg_78
	%reg_206 = fadd float %reg_204, %reg_205
	%reg_207 = load float, ptr %reg_84
	%reg_208 = fadd float %reg_206, %reg_207
	%reg_209 = load float, ptr %reg_96
	%reg_210 = fadd float %reg_208, %reg_209
	%reg_211 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 9
	store float %reg_210, ptr %reg_211
	call void @llvm.memset.p0.i32(ptr %reg_212, i8 0, i32 32, i1 1)
	%reg_213 = load i32, ptr %reg_77
	%reg_214 = load i32, ptr %reg_68
	%reg_215 = add i32 %reg_213, %reg_214
	%reg_216 = load i32, ptr %reg_65
	%reg_217 = add i32 %reg_215, %reg_216
	%reg_218 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 0
	store i32 %reg_217, ptr %reg_218
	%reg_219 = load i32, ptr %reg_115
	%reg_220 = load i32, ptr %reg_69
	%reg_221 = add i32 %reg_219, %reg_220
	%reg_222 = load i32, ptr %reg_70
	%reg_223 = add i32 %reg_221, %reg_222
	%reg_224 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 1
	store i32 %reg_223, ptr %reg_224
	%reg_225 = load i32, ptr %reg_66
	%reg_226 = load i32, ptr %reg_86
	%reg_227 = add i32 %reg_225, %reg_226
	%reg_228 = load i32, ptr %reg_95
	%reg_229 = add i32 %reg_227, %reg_228
	%reg_230 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 2
	store i32 %reg_229, ptr %reg_230
	%reg_231 = load i32, ptr %reg_103
	%reg_232 = load i32, ptr %reg_88
	%reg_233 = add i32 %reg_231, %reg_232
	%reg_234 = load i32, ptr %reg_109
	%reg_235 = add i32 %reg_233, %reg_234
	%reg_236 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 3
	store i32 %reg_235, ptr %reg_236
	%reg_237 = load i32, ptr %reg_107
	%reg_238 = load i32, ptr %reg_89
	%reg_239 = add i32 %reg_237, %reg_238
	%reg_240 = load i32, ptr %reg_126
	%reg_241 = add i32 %reg_239, %reg_240
	%reg_242 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 4
	store i32 %reg_241, ptr %reg_242
	%reg_243 = load i32, ptr %reg_124
	%reg_244 = load i32, ptr %reg_110
	%reg_245 = add i32 %reg_243, %reg_244
	%reg_246 = load i32, ptr %reg_97
	%reg_247 = add i32 %reg_245, %reg_246
	%reg_248 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 5
	store i32 %reg_247, ptr %reg_248
	%reg_249 = load i32, ptr %reg_102
	%reg_250 = load i32, ptr %reg_79
	%reg_251 = add i32 %reg_249, %reg_250
	%reg_252 = load i32, ptr %reg_117
	%reg_253 = add i32 %reg_251, %reg_252
	%reg_254 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 6
	store i32 %reg_253, ptr %reg_254
	%reg_255 = load i32, ptr %reg_116
	%reg_256 = load i32, ptr %reg_74
	%reg_257 = add i32 %reg_255, %reg_256
	%reg_258 = load i32, ptr %reg_64
	%reg_259 = add i32 %reg_257, %reg_258
	%reg_260 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 7
	store i32 %reg_259, ptr %reg_260
	%reg_261 = add i32 10, 0
	%reg_262 = getelementptr [10 x float], ptr %reg_131, i32 0
	call void @putfarray(i32 %reg_261, ptr %reg_262)
	%reg_263 = add i32 8, 0
	%reg_264 = getelementptr [8 x i32], ptr %reg_212, i32 0
	call void @putarray(i32 %reg_263, ptr %reg_264)
	%reg_266 = add i32 0, 0
	store i32 %reg_266, ptr %reg_265
	br label %Block5
Block3:  ;If end at line 39
	ret float 0x3fd3333340000000
Block4:  ;If else at line 39
	%reg_288 = load i32, ptr %reg_68
	%reg_289 = load i32, ptr %reg_65
	%reg_290 = load i32, ptr %reg_66
	%reg_291 = load float, ptr %reg_67
	%reg_292 = load i32, ptr %reg_68
	%reg_293 = load i32, ptr %reg_69
	%reg_294 = load i32, ptr %reg_70
	%reg_295 = load float, ptr %reg_71
	%reg_296 = load float, ptr %reg_72
	%reg_297 = load float, ptr %reg_73
	%reg_298 = load i32, ptr %reg_74
	%reg_299 = load float, ptr %reg_75
	%reg_300 = load float, ptr %reg_76
	%reg_301 = load i32, ptr %reg_77
	%reg_302 = load float, ptr %reg_78
	%reg_303 = load i32, ptr %reg_79
	%reg_304 = load float, ptr %reg_80
	%reg_305 = load float, ptr %reg_81
	%reg_306 = load float, ptr %reg_82
	%reg_307 = load float, ptr %reg_83
	%reg_308 = load float, ptr %reg_84
	%reg_309 = load float, ptr %reg_85
	%reg_310 = load i32, ptr %reg_86
	%reg_311 = load float, ptr %reg_87
	%reg_312 = load i32, ptr %reg_88
	%reg_313 = load i32, ptr %reg_89
	%reg_314 = load float, ptr %reg_90
	%reg_315 = load float, ptr %reg_91
	%reg_316 = load float, ptr %reg_92
	%reg_317 = load float, ptr %reg_93
	%reg_318 = load float, ptr %reg_94
	%reg_319 = load i32, ptr %reg_95
	%reg_320 = load float, ptr %reg_96
	%reg_321 = load i32, ptr %reg_97
	%reg_322 = load float, ptr %reg_98
	%reg_323 = load float, ptr %reg_99
	%reg_324 = load float, ptr %reg_100
	%reg_325 = load float, ptr %reg_101
	%reg_326 = load i32, ptr %reg_102
	%reg_327 = load i32, ptr %reg_103
	%reg_328 = load float, ptr %reg_104
	%reg_329 = load float, ptr %reg_105
	%reg_330 = load float, ptr %reg_106
	%reg_331 = load i32, ptr %reg_107
	%reg_332 = load float, ptr %reg_108
	%reg_333 = load i32, ptr %reg_109
	%reg_334 = load i32, ptr %reg_110
	%reg_335 = load float, ptr %reg_111
	%reg_336 = load float, ptr %reg_112
	%reg_337 = load float, ptr %reg_113
	%reg_338 = load float, ptr %reg_114
	%reg_339 = load i32, ptr %reg_115
	%reg_340 = load i32, ptr %reg_116
	%reg_341 = load i32, ptr %reg_117
	%reg_342 = load float, ptr %reg_118
	%reg_343 = load float, ptr %reg_119
	%reg_344 = load float, ptr %reg_120
	%reg_345 = load float, ptr %reg_121
	%reg_346 = load float, ptr %reg_122
	%reg_347 = load float, ptr %reg_123
	%reg_348 = load i32, ptr %reg_124
	%reg_349 = load float, ptr %reg_125
	%reg_350 = load i32, ptr %reg_126
	%reg_351 = load float, ptr %reg_127
	%reg_352 = call float @params_f40_i24(i32 %reg_288, i32 %reg_289, i32 %reg_290, float %reg_291, i32 %reg_292, i32 %reg_293, i32 %reg_294, float %reg_295, float %reg_296, float %reg_297, i32 %reg_298, float %reg_299, float %reg_300, i32 %reg_301, float %reg_302, i32 %reg_303, float %reg_304, float %reg_305, float %reg_306, float %reg_307, float %reg_308, float %reg_309, i32 %reg_310, float %reg_311, i32 %reg_312, i32 %reg_313, float %reg_314, float %reg_315, float %reg_316, float %reg_317, float %reg_318, i32 %reg_319, float %reg_320, i32 %reg_321, float %reg_322, float %reg_323, float %reg_324, float %reg_325, i32 %reg_326, i32 %reg_327, float %reg_328, float %reg_329, float %reg_330, i32 %reg_331, float %reg_332, i32 %reg_333, i32 %reg_334, float %reg_335, float %reg_336, float %reg_337, float %reg_338, i32 %reg_339, i32 %reg_340, i32 %reg_341, float %reg_342, float %reg_343, float %reg_344, float %reg_345, float %reg_346, float %reg_347, i32 %reg_348, float %reg_349, i32 %reg_350, float %reg_351)
	ret float %reg_352
	br label %Block3
Block5:  ;While condition at line 51
	%reg_267 = load i32, ptr %reg_265
	%reg_268 = add i32 8, 0
	%reg_269 = icmp slt i32 %reg_267, %reg_268
	br i1 %reg_269, label %Block6, label %Block7
Block6:  ;While body at line 51
	%reg_270 = load i32, ptr %reg_265
	%reg_271 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 %reg_270
	%reg_272 = load i32, ptr %reg_265
	%reg_273 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 %reg_272
	%reg_274 = load i32, ptr %reg_273
	%reg_275 = sitofp i32 %reg_274 to float
	%reg_276 = load i32, ptr %reg_265
	%reg_277 = getelementptr [10 x float], ptr %reg_131, i32 0, i32 %reg_276
	%reg_278 = load float, ptr %reg_277
	%reg_279 = fsub float %reg_275, %reg_278
	%reg_280 = fptosi float %reg_279 to i32
	store i32 %reg_280, ptr %reg_271
	%reg_281 = load i32, ptr %reg_265
	%reg_282 = add i32 1, 0
	%reg_283 = add i32 %reg_281, %reg_282
	store i32 %reg_283, ptr %reg_265
	br label %Block5
Block7:  ;While end at line 51
	%reg_284 = load i32, ptr @k
	%reg_285 = getelementptr [8 x i32], ptr %reg_212, i32 0, i32 %reg_284
	%reg_286 = load i32, ptr %reg_285
	%reg_287 = sitofp i32 %reg_286 to float
	ret float %reg_287
	br label %Block3
}

define float @params_fa40(ptr %reg_0, ptr %reg_1, ptr %reg_2, ptr %reg_3, ptr %reg_4, ptr %reg_5, ptr %reg_6, ptr %reg_7, ptr %reg_8, ptr %reg_9, ptr %reg_10, ptr %reg_11, ptr %reg_12, ptr %reg_13, ptr %reg_14, ptr %reg_15, ptr %reg_16, ptr %reg_17, ptr %reg_18, ptr %reg_19, ptr %reg_20, ptr %reg_21, ptr %reg_22, ptr %reg_23, ptr %reg_24, ptr %reg_25, ptr %reg_26, ptr %reg_27, ptr %reg_28, ptr %reg_29, ptr %reg_30, ptr %reg_31, ptr %reg_32, ptr %reg_33, ptr %reg_34, ptr %reg_35, ptr %reg_36, ptr %reg_37, ptr %reg_38, ptr %reg_39)
{
Block0:  ;Func define at line 66
	%reg_40 = alloca [10 x float]
	br label %Block1
Block1:  ;Func body at line 66
	call void @llvm.memset.p0.i32(ptr %reg_40, i8 0, i32 40, i1 1)
	%reg_41 = load i32, ptr @k
	%reg_42 = getelementptr float, ptr %reg_0, i32 %reg_41
	%reg_43 = load float, ptr %reg_42
	%reg_44 = load i32, ptr @k
	%reg_45 = getelementptr float, ptr %reg_1, i32 %reg_44
	%reg_46 = load float, ptr %reg_45
	%reg_47 = fadd float %reg_43, %reg_46
	%reg_48 = load i32, ptr @k
	%reg_49 = getelementptr float, ptr %reg_2, i32 %reg_48
	%reg_50 = load float, ptr %reg_49
	%reg_51 = fadd float %reg_47, %reg_50
	%reg_52 = load i32, ptr @k
	%reg_53 = getelementptr float, ptr %reg_3, i32 %reg_52
	%reg_54 = load float, ptr %reg_53
	%reg_55 = fadd float %reg_51, %reg_54
	%reg_56 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 0
	store float %reg_55, ptr %reg_56
	%reg_57 = load i32, ptr @k
	%reg_58 = getelementptr float, ptr %reg_4, i32 %reg_57
	%reg_59 = load float, ptr %reg_58
	%reg_60 = load i32, ptr @k
	%reg_61 = getelementptr float, ptr %reg_5, i32 %reg_60
	%reg_62 = load float, ptr %reg_61
	%reg_63 = fadd float %reg_59, %reg_62
	%reg_64 = load i32, ptr @k
	%reg_65 = getelementptr float, ptr %reg_6, i32 %reg_64
	%reg_66 = load float, ptr %reg_65
	%reg_67 = fadd float %reg_63, %reg_66
	%reg_68 = load i32, ptr @k
	%reg_69 = getelementptr float, ptr %reg_7, i32 %reg_68
	%reg_70 = load float, ptr %reg_69
	%reg_71 = fadd float %reg_67, %reg_70
	%reg_72 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 1
	store float %reg_71, ptr %reg_72
	%reg_73 = load i32, ptr @k
	%reg_74 = getelementptr float, ptr %reg_8, i32 %reg_73
	%reg_75 = load float, ptr %reg_74
	%reg_76 = load i32, ptr @k
	%reg_77 = getelementptr float, ptr %reg_9, i32 %reg_76
	%reg_78 = load float, ptr %reg_77
	%reg_79 = fadd float %reg_75, %reg_78
	%reg_80 = load i32, ptr @k
	%reg_81 = getelementptr float, ptr %reg_10, i32 %reg_80
	%reg_82 = load float, ptr %reg_81
	%reg_83 = fadd float %reg_79, %reg_82
	%reg_84 = load i32, ptr @k
	%reg_85 = getelementptr float, ptr %reg_11, i32 %reg_84
	%reg_86 = load float, ptr %reg_85
	%reg_87 = fadd float %reg_83, %reg_86
	%reg_88 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 2
	store float %reg_87, ptr %reg_88
	%reg_89 = load i32, ptr @k
	%reg_90 = getelementptr float, ptr %reg_12, i32 %reg_89
	%reg_91 = load float, ptr %reg_90
	%reg_92 = load i32, ptr @k
	%reg_93 = getelementptr float, ptr %reg_13, i32 %reg_92
	%reg_94 = load float, ptr %reg_93
	%reg_95 = fadd float %reg_91, %reg_94
	%reg_96 = load i32, ptr @k
	%reg_97 = getelementptr float, ptr %reg_14, i32 %reg_96
	%reg_98 = load float, ptr %reg_97
	%reg_99 = fadd float %reg_95, %reg_98
	%reg_100 = load i32, ptr @k
	%reg_101 = getelementptr float, ptr %reg_15, i32 %reg_100
	%reg_102 = load float, ptr %reg_101
	%reg_103 = fadd float %reg_99, %reg_102
	%reg_104 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 3
	store float %reg_103, ptr %reg_104
	%reg_105 = load i32, ptr @k
	%reg_106 = getelementptr float, ptr %reg_16, i32 %reg_105
	%reg_107 = load float, ptr %reg_106
	%reg_108 = load i32, ptr @k
	%reg_109 = getelementptr float, ptr %reg_17, i32 %reg_108
	%reg_110 = load float, ptr %reg_109
	%reg_111 = fadd float %reg_107, %reg_110
	%reg_112 = load i32, ptr @k
	%reg_113 = getelementptr float, ptr %reg_18, i32 %reg_112
	%reg_114 = load float, ptr %reg_113
	%reg_115 = fadd float %reg_111, %reg_114
	%reg_116 = load i32, ptr @k
	%reg_117 = getelementptr float, ptr %reg_19, i32 %reg_116
	%reg_118 = load float, ptr %reg_117
	%reg_119 = fadd float %reg_115, %reg_118
	%reg_120 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 4
	store float %reg_119, ptr %reg_120
	%reg_121 = load i32, ptr @k
	%reg_122 = getelementptr float, ptr %reg_20, i32 %reg_121
	%reg_123 = load float, ptr %reg_122
	%reg_124 = load i32, ptr @k
	%reg_125 = getelementptr float, ptr %reg_21, i32 %reg_124
	%reg_126 = load float, ptr %reg_125
	%reg_127 = fadd float %reg_123, %reg_126
	%reg_128 = load i32, ptr @k
	%reg_129 = getelementptr float, ptr %reg_22, i32 %reg_128
	%reg_130 = load float, ptr %reg_129
	%reg_131 = fadd float %reg_127, %reg_130
	%reg_132 = load i32, ptr @k
	%reg_133 = getelementptr float, ptr %reg_23, i32 %reg_132
	%reg_134 = load float, ptr %reg_133
	%reg_135 = fadd float %reg_131, %reg_134
	%reg_136 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 5
	store float %reg_135, ptr %reg_136
	%reg_137 = load i32, ptr @k
	%reg_138 = getelementptr float, ptr %reg_24, i32 %reg_137
	%reg_139 = load float, ptr %reg_138
	%reg_140 = load i32, ptr @k
	%reg_141 = getelementptr float, ptr %reg_25, i32 %reg_140
	%reg_142 = load float, ptr %reg_141
	%reg_143 = fadd float %reg_139, %reg_142
	%reg_144 = load i32, ptr @k
	%reg_145 = getelementptr float, ptr %reg_26, i32 %reg_144
	%reg_146 = load float, ptr %reg_145
	%reg_147 = fadd float %reg_143, %reg_146
	%reg_148 = load i32, ptr @k
	%reg_149 = getelementptr float, ptr %reg_27, i32 %reg_148
	%reg_150 = load float, ptr %reg_149
	%reg_151 = fadd float %reg_147, %reg_150
	%reg_152 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 6
	store float %reg_151, ptr %reg_152
	%reg_153 = load i32, ptr @k
	%reg_154 = getelementptr float, ptr %reg_28, i32 %reg_153
	%reg_155 = load float, ptr %reg_154
	%reg_156 = load i32, ptr @k
	%reg_157 = getelementptr float, ptr %reg_29, i32 %reg_156
	%reg_158 = load float, ptr %reg_157
	%reg_159 = fadd float %reg_155, %reg_158
	%reg_160 = load i32, ptr @k
	%reg_161 = getelementptr float, ptr %reg_30, i32 %reg_160
	%reg_162 = load float, ptr %reg_161
	%reg_163 = fadd float %reg_159, %reg_162
	%reg_164 = load i32, ptr @k
	%reg_165 = getelementptr float, ptr %reg_31, i32 %reg_164
	%reg_166 = load float, ptr %reg_165
	%reg_167 = fadd float %reg_163, %reg_166
	%reg_168 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 7
	store float %reg_167, ptr %reg_168
	%reg_169 = load i32, ptr @k
	%reg_170 = getelementptr float, ptr %reg_32, i32 %reg_169
	%reg_171 = load float, ptr %reg_170
	%reg_172 = load i32, ptr @k
	%reg_173 = getelementptr float, ptr %reg_33, i32 %reg_172
	%reg_174 = load float, ptr %reg_173
	%reg_175 = fadd float %reg_171, %reg_174
	%reg_176 = load i32, ptr @k
	%reg_177 = getelementptr float, ptr %reg_34, i32 %reg_176
	%reg_178 = load float, ptr %reg_177
	%reg_179 = fadd float %reg_175, %reg_178
	%reg_180 = load i32, ptr @k
	%reg_181 = getelementptr float, ptr %reg_35, i32 %reg_180
	%reg_182 = load float, ptr %reg_181
	%reg_183 = fadd float %reg_179, %reg_182
	%reg_184 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 8
	store float %reg_183, ptr %reg_184
	%reg_185 = load i32, ptr @k
	%reg_186 = getelementptr float, ptr %reg_36, i32 %reg_185
	%reg_187 = load float, ptr %reg_186
	%reg_188 = load i32, ptr @k
	%reg_189 = getelementptr float, ptr %reg_37, i32 %reg_188
	%reg_190 = load float, ptr %reg_189
	%reg_191 = fadd float %reg_187, %reg_190
	%reg_192 = load i32, ptr @k
	%reg_193 = getelementptr float, ptr %reg_38, i32 %reg_192
	%reg_194 = load float, ptr %reg_193
	%reg_195 = fadd float %reg_191, %reg_194
	%reg_196 = load i32, ptr @k
	%reg_197 = getelementptr float, ptr %reg_39, i32 %reg_196
	%reg_198 = load float, ptr %reg_197
	%reg_199 = fadd float %reg_195, %reg_198
	%reg_200 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 9
	store float %reg_199, ptr %reg_200
	%reg_201 = load i32, ptr @k
	%reg_202 = getelementptr float, ptr %reg_39, i32 %reg_201
	%reg_203 = load float, ptr %reg_202
	%reg_204 = add i32 0, 0
	%reg_205 = sitofp i32 %reg_204 to float
	%reg_206 = fcmp one float %reg_203, %reg_205
	%reg_207 = zext i1 %reg_206 to i32
	%reg_208 = add i32 0, 0
	%reg_209 = icmp ne i32 %reg_207, %reg_208
	%reg_210 = zext i1 %reg_209 to i32
	%reg_211 = sitofp i32 %reg_210 to float
	%reg_212 = fadd float 0x0, 0x0
	%reg_213 = fcmp one float %reg_211, %reg_212
	%reg_214 = zext i1 %reg_213 to i32
	%reg_215 = sitofp i32 %reg_214 to float
	%reg_216 = fadd float 0x0, 0x0
	%reg_217 = fcmp one float %reg_215, %reg_216
	br i1 %reg_217, label %Block2, label %Block4
Block2:  ;If then at line 82
	%reg_218 = add i32 10, 0
	%reg_219 = getelementptr [10 x float], ptr %reg_40, i32 0
	call void @putfarray(i32 %reg_218, ptr %reg_219)
	%reg_220 = load i32, ptr @k
	%reg_221 = getelementptr [10 x float], ptr %reg_40, i32 0, i32 %reg_220
	%reg_222 = load float, ptr %reg_221
	ret float %reg_222
	br label %Block3
Block3:  ;If end at line 82
	ret float 0x3fd3333340000000
Block4:  ;If else at line 82
	%reg_223 = getelementptr float, ptr %reg_1
	%reg_224 = getelementptr float, ptr %reg_2
	%reg_225 = getelementptr float, ptr %reg_3
	%reg_226 = getelementptr float, ptr %reg_4
	%reg_227 = getelementptr float, ptr %reg_5
	%reg_228 = getelementptr float, ptr %reg_6
	%reg_229 = getelementptr float, ptr %reg_7
	%reg_230 = getelementptr float, ptr %reg_8
	%reg_231 = getelementptr float, ptr %reg_9
	%reg_232 = getelementptr float, ptr %reg_10
	%reg_233 = getelementptr float, ptr %reg_11
	%reg_234 = getelementptr float, ptr %reg_12
	%reg_235 = getelementptr float, ptr %reg_13
	%reg_236 = getelementptr float, ptr %reg_14
	%reg_237 = getelementptr float, ptr %reg_15
	%reg_238 = getelementptr float, ptr %reg_16
	%reg_239 = getelementptr float, ptr %reg_17
	%reg_240 = getelementptr float, ptr %reg_18
	%reg_241 = getelementptr float, ptr %reg_19
	%reg_242 = getelementptr float, ptr %reg_20
	%reg_243 = getelementptr float, ptr %reg_21
	%reg_244 = getelementptr float, ptr %reg_22
	%reg_245 = getelementptr float, ptr %reg_23
	%reg_246 = getelementptr float, ptr %reg_24
	%reg_247 = getelementptr float, ptr %reg_25
	%reg_248 = getelementptr float, ptr %reg_26
	%reg_249 = getelementptr float, ptr %reg_27
	%reg_250 = getelementptr float, ptr %reg_28
	%reg_251 = getelementptr float, ptr %reg_29
	%reg_252 = getelementptr float, ptr %reg_30
	%reg_253 = getelementptr float, ptr %reg_31
	%reg_254 = getelementptr float, ptr %reg_32
	%reg_255 = getelementptr float, ptr %reg_33
	%reg_256 = getelementptr float, ptr %reg_34
	%reg_257 = getelementptr float, ptr %reg_35
	%reg_258 = getelementptr float, ptr %reg_36
	%reg_259 = getelementptr float, ptr %reg_37
	%reg_260 = getelementptr float, ptr %reg_38
	%reg_261 = getelementptr float, ptr %reg_39
	%reg_262 = getelementptr [10 x float], ptr %reg_40, i32 0
	%reg_263 = call float @params_fa40(ptr %reg_223, ptr %reg_224, ptr %reg_225, ptr %reg_226, ptr %reg_227, ptr %reg_228, ptr %reg_229, ptr %reg_230, ptr %reg_231, ptr %reg_232, ptr %reg_233, ptr %reg_234, ptr %reg_235, ptr %reg_236, ptr %reg_237, ptr %reg_238, ptr %reg_239, ptr %reg_240, ptr %reg_241, ptr %reg_242, ptr %reg_243, ptr %reg_244, ptr %reg_245, ptr %reg_246, ptr %reg_247, ptr %reg_248, ptr %reg_249, ptr %reg_250, ptr %reg_251, ptr %reg_252, ptr %reg_253, ptr %reg_254, ptr %reg_255, ptr %reg_256, ptr %reg_257, ptr %reg_258, ptr %reg_259, ptr %reg_260, ptr %reg_261, ptr %reg_262)
	ret float %reg_263
	br label %Block3
}

define i32 @params_mix(float %reg_0, ptr %reg_1, i32 %reg_2, ptr %reg_3, float %reg_4, i32 %reg_5, float %reg_6, float %reg_7, ptr %reg_8, ptr %reg_9, i32 %reg_10, i32 %reg_11, ptr %reg_12, ptr %reg_13, ptr %reg_14, i32 %reg_15, ptr %reg_16, ptr %reg_17, float %reg_18, float %reg_19, float %reg_20, ptr %reg_21, i32 %reg_22, float %reg_23, float %reg_24, float %reg_25, ptr %reg_26, ptr %reg_27, ptr %reg_28, ptr %reg_29, ptr %reg_30, float %reg_31, float %reg_32, ptr %reg_33, i32 %reg_34, ptr %reg_35, ptr %reg_36, float %reg_37, float %reg_38, ptr %reg_39, ptr %reg_40, i32 %reg_41, i32 %reg_42, float %reg_43, float %reg_44, ptr %reg_45, i32 %reg_46, ptr %reg_47, i32 %reg_48, ptr %reg_49, ptr %reg_50, float %reg_51, float %reg_52, ptr %reg_53, i32 %reg_54, ptr %reg_55, ptr %reg_56, float %reg_57, i32 %reg_58, float %reg_59, ptr %reg_60, ptr %reg_61, float %reg_62, i32 %reg_63)
{
Block0:  ;Func define at line 93
	%reg_294 = alloca i32
	%reg_202 = alloca [10 x i32]
	%reg_99 = alloca [10 x float]
	%reg_98 = alloca i32
	%reg_97 = alloca float
	%reg_96 = alloca float
	%reg_95 = alloca i32
	%reg_94 = alloca float
	%reg_93 = alloca i32
	%reg_92 = alloca float
	%reg_91 = alloca float
	%reg_90 = alloca i32
	%reg_89 = alloca i32
	%reg_88 = alloca float
	%reg_87 = alloca float
	%reg_86 = alloca i32
	%reg_85 = alloca i32
	%reg_84 = alloca float
	%reg_83 = alloca float
	%reg_82 = alloca i32
	%reg_81 = alloca float
	%reg_80 = alloca float
	%reg_79 = alloca float
	%reg_78 = alloca float
	%reg_77 = alloca float
	%reg_76 = alloca i32
	%reg_75 = alloca float
	%reg_74 = alloca float
	%reg_73 = alloca float
	%reg_72 = alloca i32
	%reg_71 = alloca i32
	%reg_70 = alloca i32
	%reg_69 = alloca float
	%reg_68 = alloca float
	%reg_67 = alloca i32
	%reg_66 = alloca float
	%reg_65 = alloca i32
	%reg_64 = alloca float
	store float %reg_0, ptr %reg_64
	store i32 %reg_2, ptr %reg_65
	store float %reg_4, ptr %reg_66
	store i32 %reg_5, ptr %reg_67
	store float %reg_6, ptr %reg_68
	store float %reg_7, ptr %reg_69
	store i32 %reg_10, ptr %reg_70
	store i32 %reg_11, ptr %reg_71
	store i32 %reg_15, ptr %reg_72
	store float %reg_18, ptr %reg_73
	store float %reg_19, ptr %reg_74
	store float %reg_20, ptr %reg_75
	store i32 %reg_22, ptr %reg_76
	store float %reg_23, ptr %reg_77
	store float %reg_24, ptr %reg_78
	store float %reg_25, ptr %reg_79
	store float %reg_31, ptr %reg_80
	store float %reg_32, ptr %reg_81
	store i32 %reg_34, ptr %reg_82
	store float %reg_37, ptr %reg_83
	store float %reg_38, ptr %reg_84
	store i32 %reg_41, ptr %reg_85
	store i32 %reg_42, ptr %reg_86
	store float %reg_43, ptr %reg_87
	store float %reg_44, ptr %reg_88
	store i32 %reg_46, ptr %reg_89
	store i32 %reg_48, ptr %reg_90
	store float %reg_51, ptr %reg_91
	store float %reg_52, ptr %reg_92
	store i32 %reg_54, ptr %reg_93
	store float %reg_57, ptr %reg_94
	store i32 %reg_58, ptr %reg_95
	store float %reg_59, ptr %reg_96
	store float %reg_62, ptr %reg_97
	store i32 %reg_63, ptr %reg_98
	br label %Block1
Block1:  ;Func body at line 93
	call void @llvm.memset.p0.i32(ptr %reg_99, i8 0, i32 40, i1 1)
	%reg_100 = load float, ptr %reg_64
	%reg_101 = load i32, ptr @k
	%reg_102 = getelementptr float, ptr %reg_3, i32 %reg_101
	%reg_103 = load float, ptr %reg_102
	%reg_104 = fadd float %reg_100, %reg_103
	%reg_105 = load float, ptr %reg_66
	%reg_106 = fadd float %reg_104, %reg_105
	%reg_107 = load float, ptr %reg_68
	%reg_108 = fadd float %reg_106, %reg_107
	%reg_109 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 0
	store float %reg_108, ptr %reg_109
	%reg_110 = load float, ptr %reg_69
	%reg_111 = load i32, ptr @k
	%reg_112 = getelementptr float, ptr %reg_8, i32 %reg_111
	%reg_113 = load float, ptr %reg_112
	%reg_114 = fadd float %reg_110, %reg_113
	%reg_115 = load i32, ptr @k
	%reg_116 = getelementptr float, ptr %reg_12, i32 %reg_115
	%reg_117 = load float, ptr %reg_116
	%reg_118 = fadd float %reg_114, %reg_117
	%reg_119 = load i32, ptr @k
	%reg_120 = getelementptr float, ptr %reg_16, i32 %reg_119
	%reg_121 = load float, ptr %reg_120
	%reg_122 = fadd float %reg_118, %reg_121
	%reg_123 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 1
	store float %reg_122, ptr %reg_123
	%reg_124 = load i32, ptr @k
	%reg_125 = getelementptr float, ptr %reg_17, i32 %reg_124
	%reg_126 = load float, ptr %reg_125
	%reg_127 = load float, ptr %reg_73
	%reg_128 = fadd float %reg_126, %reg_127
	%reg_129 = load float, ptr %reg_74
	%reg_130 = fadd float %reg_128, %reg_129
	%reg_131 = load float, ptr %reg_75
	%reg_132 = fadd float %reg_130, %reg_131
	%reg_133 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 2
	store float %reg_132, ptr %reg_133
	%reg_134 = load i32, ptr @k
	%reg_135 = getelementptr float, ptr %reg_21, i32 %reg_134
	%reg_136 = load float, ptr %reg_135
	%reg_137 = load float, ptr %reg_77
	%reg_138 = fadd float %reg_136, %reg_137
	%reg_139 = load float, ptr %reg_78
	%reg_140 = fadd float %reg_138, %reg_139
	%reg_141 = load float, ptr %reg_79
	%reg_142 = fadd float %reg_140, %reg_141
	%reg_143 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 3
	store float %reg_142, ptr %reg_143
	%reg_144 = load i32, ptr @k
	%reg_145 = getelementptr float, ptr %reg_27, i32 %reg_144
	%reg_146 = load float, ptr %reg_145
	%reg_147 = load i32, ptr @k
	%reg_148 = getelementptr float, ptr %reg_30, i32 %reg_147
	%reg_149 = load float, ptr %reg_148
	%reg_150 = fadd float %reg_146, %reg_149
	%reg_151 = load float, ptr %reg_80
	%reg_152 = fadd float %reg_150, %reg_151
	%reg_153 = load float, ptr %reg_81
	%reg_154 = fadd float %reg_152, %reg_153
	%reg_155 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 4
	store float %reg_154, ptr %reg_155
	%reg_156 = load i32, ptr @k
	%reg_157 = getelementptr float, ptr %reg_35, i32 %reg_156
	%reg_158 = load float, ptr %reg_157
	%reg_159 = load i32, ptr @k
	%reg_160 = getelementptr float, ptr %reg_36, i32 %reg_159
	%reg_161 = load float, ptr %reg_160
	%reg_162 = fadd float %reg_158, %reg_161
	%reg_163 = load float, ptr %reg_83
	%reg_164 = fadd float %reg_162, %reg_163
	%reg_165 = load float, ptr %reg_84
	%reg_166 = fadd float %reg_164, %reg_165
	%reg_167 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 5
	store float %reg_166, ptr %reg_167
	%reg_168 = load float, ptr %reg_87
	%reg_169 = load float, ptr %reg_88
	%reg_170 = fadd float %reg_168, %reg_169
	%reg_171 = load i32, ptr @k
	%reg_172 = getelementptr float, ptr %reg_47, i32 %reg_171
	%reg_173 = load float, ptr %reg_172
	%reg_174 = fadd float %reg_170, %reg_173
	%reg_175 = load float, ptr %reg_91
	%reg_176 = fadd float %reg_174, %reg_175
	%reg_177 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 6
	store float %reg_176, ptr %reg_177
	%reg_178 = load float, ptr %reg_92
	%reg_179 = load i32, ptr @k
	%reg_180 = getelementptr float, ptr %reg_53, i32 %reg_179
	%reg_181 = load float, ptr %reg_180
	%reg_182 = fadd float %reg_178, %reg_181
	%reg_183 = load i32, ptr @k
	%reg_184 = getelementptr float, ptr %reg_56, i32 %reg_183
	%reg_185 = load float, ptr %reg_184
	%reg_186 = fadd float %reg_182, %reg_185
	%reg_187 = load float, ptr %reg_94
	%reg_188 = fadd float %reg_186, %reg_187
	%reg_189 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 7
	store float %reg_188, ptr %reg_189
	%reg_190 = load float, ptr %reg_96
	%reg_191 = load i32, ptr @k
	%reg_192 = getelementptr float, ptr %reg_60, i32 %reg_191
	%reg_193 = load float, ptr %reg_192
	%reg_194 = fadd float %reg_190, %reg_193
	%reg_195 = load i32, ptr @k
	%reg_196 = getelementptr float, ptr %reg_61, i32 %reg_195
	%reg_197 = load float, ptr %reg_196
	%reg_198 = fadd float %reg_194, %reg_197
	%reg_199 = load float, ptr %reg_97
	%reg_200 = fadd float %reg_198, %reg_199
	%reg_201 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 8
	store float %reg_200, ptr %reg_201
	call void @llvm.memset.p0.i32(ptr %reg_202, i8 0, i32 40, i1 1)
	%reg_203 = load i32, ptr @k
	%reg_204 = getelementptr i32, ptr %reg_1, i32 %reg_203
	%reg_205 = load i32, ptr %reg_204
	%reg_206 = load i32, ptr %reg_65
	%reg_207 = add i32 %reg_205, %reg_206
	%reg_208 = load i32, ptr %reg_67
	%reg_209 = add i32 %reg_207, %reg_208
	%reg_210 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 0
	store i32 %reg_209, ptr %reg_210
	%reg_211 = load i32, ptr @k
	%reg_212 = getelementptr i32, ptr %reg_9, i32 %reg_211
	%reg_213 = load i32, ptr %reg_212
	%reg_214 = load i32, ptr %reg_70
	%reg_215 = add i32 %reg_213, %reg_214
	%reg_216 = load i32, ptr %reg_71
	%reg_217 = add i32 %reg_215, %reg_216
	%reg_218 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 1
	store i32 %reg_217, ptr %reg_218
	%reg_219 = load i32, ptr @k
	%reg_220 = getelementptr i32, ptr %reg_13, i32 %reg_219
	%reg_221 = load i32, ptr %reg_220
	%reg_222 = load i32, ptr @k
	%reg_223 = getelementptr i32, ptr %reg_14, i32 %reg_222
	%reg_224 = load i32, ptr %reg_223
	%reg_225 = add i32 %reg_221, %reg_224
	%reg_226 = load i32, ptr %reg_72
	%reg_227 = add i32 %reg_225, %reg_226
	%reg_228 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 2
	store i32 %reg_227, ptr %reg_228
	%reg_229 = load i32, ptr %reg_76
	%reg_230 = load i32, ptr @k
	%reg_231 = getelementptr i32, ptr %reg_26, i32 %reg_230
	%reg_232 = load i32, ptr %reg_231
	%reg_233 = add i32 %reg_229, %reg_232
	%reg_234 = load i32, ptr @k
	%reg_235 = getelementptr i32, ptr %reg_28, i32 %reg_234
	%reg_236 = load i32, ptr %reg_235
	%reg_237 = add i32 %reg_233, %reg_236
	%reg_238 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 3
	store i32 %reg_237, ptr %reg_238
	%reg_239 = load i32, ptr @k
	%reg_240 = getelementptr i32, ptr %reg_29, i32 %reg_239
	%reg_241 = load i32, ptr %reg_240
	%reg_242 = load i32, ptr @k
	%reg_243 = getelementptr i32, ptr %reg_33, i32 %reg_242
	%reg_244 = load i32, ptr %reg_243
	%reg_245 = add i32 %reg_241, %reg_244
	%reg_246 = load i32, ptr %reg_82
	%reg_247 = add i32 %reg_245, %reg_246
	%reg_248 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 4
	store i32 %reg_247, ptr %reg_248
	%reg_249 = load i32, ptr @k
	%reg_250 = getelementptr i32, ptr %reg_39, i32 %reg_249
	%reg_251 = load i32, ptr %reg_250
	%reg_252 = load i32, ptr @k
	%reg_253 = getelementptr i32, ptr %reg_40, i32 %reg_252
	%reg_254 = load i32, ptr %reg_253
	%reg_255 = add i32 %reg_251, %reg_254
	%reg_256 = load i32, ptr %reg_85
	%reg_257 = add i32 %reg_255, %reg_256
	%reg_258 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 5
	store i32 %reg_257, ptr %reg_258
	%reg_259 = load i32, ptr %reg_86
	%reg_260 = load i32, ptr @k
	%reg_261 = getelementptr i32, ptr %reg_45, i32 %reg_260
	%reg_262 = load i32, ptr %reg_261
	%reg_263 = add i32 %reg_259, %reg_262
	%reg_264 = load i32, ptr %reg_89
	%reg_265 = add i32 %reg_263, %reg_264
	%reg_266 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 6
	store i32 %reg_265, ptr %reg_266
	%reg_267 = load i32, ptr %reg_90
	%reg_268 = load i32, ptr @k
	%reg_269 = getelementptr i32, ptr %reg_49, i32 %reg_268
	%reg_270 = load i32, ptr %reg_269
	%reg_271 = add i32 %reg_267, %reg_270
	%reg_272 = load i32, ptr @k
	%reg_273 = getelementptr i32, ptr %reg_50, i32 %reg_272
	%reg_274 = load i32, ptr %reg_273
	%reg_275 = add i32 %reg_271, %reg_274
	%reg_276 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 7
	store i32 %reg_275, ptr %reg_276
	%reg_277 = load i32, ptr %reg_93
	%reg_278 = load i32, ptr @k
	%reg_279 = getelementptr i32, ptr %reg_55, i32 %reg_278
	%reg_280 = load i32, ptr %reg_279
	%reg_281 = add i32 %reg_277, %reg_280
	%reg_282 = load i32, ptr %reg_95
	%reg_283 = add i32 %reg_281, %reg_282
	%reg_284 = load i32, ptr %reg_98
	%reg_285 = add i32 %reg_283, %reg_284
	%reg_286 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 8
	store i32 %reg_285, ptr %reg_286
	%reg_287 = load i32, ptr %reg_98
	%reg_288 = add i32 0, 0
	%reg_289 = icmp ne i32 %reg_287, %reg_288
	br i1 %reg_289, label %Block2, label %Block4
Block2:  ;If then at line 114
	%reg_290 = add i32 10, 0
	%reg_291 = getelementptr [10 x float], ptr %reg_99, i32 0
	call void @putfarray(i32 %reg_290, ptr %reg_291)
	%reg_292 = add i32 10, 0
	%reg_293 = getelementptr [10 x i32], ptr %reg_202, i32 0
	call void @putarray(i32 %reg_292, ptr %reg_293)
	%reg_295 = add i32 0, 0
	store i32 %reg_295, ptr %reg_294
	br label %Block5
Block3:  ;If end at line 114
	ret i32 0
Block4:  ;If else at line 114
	%reg_322 = load float, ptr %reg_64
	%reg_323 = getelementptr [10 x i32], ptr %reg_202, i32 0
	%reg_324 = load i32, ptr %reg_65
	%reg_325 = getelementptr [10 x float], ptr %reg_99, i32 0
	%reg_326 = load float, ptr %reg_66
	%reg_327 = load i32, ptr %reg_67
	%reg_328 = load float, ptr %reg_68
	%reg_329 = load float, ptr %reg_69
	%reg_330 = getelementptr float, ptr %reg_8
	%reg_331 = getelementptr i32, ptr %reg_9
	%reg_332 = load i32, ptr %reg_70
	%reg_333 = load i32, ptr %reg_71
	%reg_334 = getelementptr float, ptr %reg_12
	%reg_335 = getelementptr i32, ptr %reg_13
	%reg_336 = getelementptr i32, ptr %reg_14
	%reg_337 = load i32, ptr %reg_72
	%reg_338 = getelementptr float, ptr %reg_16
	%reg_339 = getelementptr float, ptr %reg_17
	%reg_340 = load float, ptr %reg_73
	%reg_341 = load float, ptr %reg_74
	%reg_342 = load float, ptr %reg_75
	%reg_343 = getelementptr float, ptr %reg_21
	%reg_344 = load i32, ptr %reg_76
	%reg_345 = load float, ptr %reg_77
	%reg_346 = load float, ptr %reg_78
	%reg_347 = load float, ptr %reg_79
	%reg_348 = getelementptr i32, ptr %reg_26
	%reg_349 = getelementptr float, ptr %reg_27
	%reg_350 = getelementptr i32, ptr %reg_28
	%reg_351 = getelementptr i32, ptr %reg_29
	%reg_352 = getelementptr float, ptr %reg_30
	%reg_353 = load float, ptr %reg_80
	%reg_354 = load float, ptr %reg_81
	%reg_355 = getelementptr i32, ptr %reg_33
	%reg_356 = load i32, ptr %reg_82
	%reg_357 = getelementptr float, ptr %reg_35
	%reg_358 = getelementptr float, ptr %reg_36
	%reg_359 = load float, ptr %reg_83
	%reg_360 = load float, ptr %reg_84
	%reg_361 = getelementptr i32, ptr %reg_39
	%reg_362 = getelementptr i32, ptr %reg_40
	%reg_363 = load i32, ptr %reg_85
	%reg_364 = load i32, ptr %reg_86
	%reg_365 = load float, ptr %reg_87
	%reg_366 = load float, ptr %reg_88
	%reg_367 = getelementptr i32, ptr %reg_45
	%reg_368 = load i32, ptr %reg_89
	%reg_369 = getelementptr float, ptr %reg_47
	%reg_370 = load i32, ptr %reg_90
	%reg_371 = getelementptr i32, ptr %reg_49
	%reg_372 = getelementptr i32, ptr %reg_50
	%reg_373 = load float, ptr %reg_91
	%reg_374 = load float, ptr %reg_92
	%reg_375 = getelementptr float, ptr %reg_53
	%reg_376 = load i32, ptr %reg_93
	%reg_377 = getelementptr i32, ptr %reg_55
	%reg_378 = getelementptr float, ptr %reg_56
	%reg_379 = load float, ptr %reg_94
	%reg_380 = load i32, ptr %reg_95
	%reg_381 = load float, ptr %reg_96
	%reg_382 = getelementptr float, ptr %reg_60
	%reg_383 = getelementptr float, ptr %reg_61
	%reg_384 = load i32, ptr %reg_98
	%reg_385 = sitofp i32 %reg_384 to float
	%reg_386 = load float, ptr %reg_97
	%reg_387 = fptosi float %reg_386 to i32
	%reg_388 = call i32 @params_mix(float %reg_322, ptr %reg_323, i32 %reg_324, ptr %reg_325, float %reg_326, i32 %reg_327, float %reg_328, float %reg_329, ptr %reg_330, ptr %reg_331, i32 %reg_332, i32 %reg_333, ptr %reg_334, ptr %reg_335, ptr %reg_336, i32 %reg_337, ptr %reg_338, ptr %reg_339, float %reg_340, float %reg_341, float %reg_342, ptr %reg_343, i32 %reg_344, float %reg_345, float %reg_346, float %reg_347, ptr %reg_348, ptr %reg_349, ptr %reg_350, ptr %reg_351, ptr %reg_352, float %reg_353, float %reg_354, ptr %reg_355, i32 %reg_356, ptr %reg_357, ptr %reg_358, float %reg_359, float %reg_360, ptr %reg_361, ptr %reg_362, i32 %reg_363, i32 %reg_364, float %reg_365, float %reg_366, ptr %reg_367, i32 %reg_368, ptr %reg_369, i32 %reg_370, ptr %reg_371, ptr %reg_372, float %reg_373, float %reg_374, ptr %reg_375, i32 %reg_376, ptr %reg_377, ptr %reg_378, float %reg_379, i32 %reg_380, float %reg_381, ptr %reg_382, ptr %reg_383, float %reg_385, i32 %reg_387)
	ret i32 %reg_388
	br label %Block3
Block5:  ;While condition at line 118
	%reg_296 = load i32, ptr %reg_294
	%reg_297 = add i32 10, 0
	%reg_298 = icmp slt i32 %reg_296, %reg_297
	br i1 %reg_298, label %Block6, label %Block7
Block6:  ;While body at line 118
	%reg_299 = load i32, ptr %reg_294
	%reg_300 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 %reg_299
	%reg_301 = load i32, ptr %reg_294
	%reg_302 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 %reg_301
	%reg_303 = load i32, ptr %reg_302
	%reg_304 = sitofp i32 %reg_303 to float
	%reg_305 = load i32, ptr %reg_294
	%reg_306 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 %reg_305
	%reg_307 = load float, ptr %reg_306
	%reg_308 = fsub float %reg_304, %reg_307
	%reg_309 = fptosi float %reg_308 to i32
	store i32 %reg_309, ptr %reg_300
	%reg_310 = load i32, ptr %reg_294
	%reg_311 = add i32 1, 0
	%reg_312 = add i32 %reg_310, %reg_311
	store i32 %reg_312, ptr %reg_294
	br label %Block5
Block7:  ;While end at line 118
	%reg_313 = load i32, ptr @k
	%reg_314 = getelementptr [10 x i32], ptr %reg_202, i32 0, i32 %reg_313
	%reg_315 = load i32, ptr %reg_314
	%reg_316 = sitofp i32 %reg_315 to float
	%reg_317 = add i32 8, 0
	%reg_318 = getelementptr [10 x float], ptr %reg_99, i32 0, i32 %reg_317
	%reg_319 = load float, ptr %reg_318
	%reg_320 = fmul float %reg_316, %reg_319
	%reg_321 = fptosi float %reg_320 to i32
	ret i32 %reg_321
	br label %Block3
}

define i32 @main()
{
Block0:  ;Func define at line 133
	%reg_527 = alloca i32
	%reg_445 = alloca float
	%reg_187 = alloca float
	%reg_25 = alloca float
	%reg_2 = alloca i32
	%reg_1 = alloca [24 x [3 x i32]]
	%reg_0 = alloca [40 x [3 x float]]
	br label %Block1
Block1:  ;Func body at line 133
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 480, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 288, i1 1)
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = call i32 @getint()
	store i32 %reg_4, ptr @k
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 139
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = add i32 40, 0
	%reg_8 = icmp slt i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 139
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_9
	%reg_11 = call i32 @getfarray(ptr %reg_10)
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = add i32 1, 0
	%reg_14 = add i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_2
	br label %Block2
Block4:  ;While end at line 139
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_2
	br label %Block5
Block5:  ;While condition at line 144
	%reg_16 = load i32, ptr %reg_2
	%reg_17 = add i32 24, 0
	%reg_18 = icmp slt i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block6, label %Block7
Block6:  ;While body at line 144
	%reg_19 = load i32, ptr %reg_2
	%reg_20 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_19
	%reg_21 = call i32 @getarray(ptr %reg_20)
	%reg_22 = load i32, ptr %reg_2
	%reg_23 = add i32 1, 0
	%reg_24 = add i32 %reg_22, %reg_23
	store i32 %reg_24, ptr %reg_2
	br label %Block5
Block7:  ;While end at line 144
	%reg_26 = add i32 0, 0
	%reg_27 = load i32, ptr @k
	%reg_28 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_26, i32 %reg_27
	%reg_29 = load float, ptr %reg_28
	%reg_30 = add i32 1, 0
	%reg_31 = load i32, ptr @k
	%reg_32 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_30, i32 %reg_31
	%reg_33 = load float, ptr %reg_32
	%reg_34 = add i32 2, 0
	%reg_35 = load i32, ptr @k
	%reg_36 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_34, i32 %reg_35
	%reg_37 = load float, ptr %reg_36
	%reg_38 = add i32 3, 0
	%reg_39 = load i32, ptr @k
	%reg_40 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_38, i32 %reg_39
	%reg_41 = load float, ptr %reg_40
	%reg_42 = add i32 4, 0
	%reg_43 = load i32, ptr @k
	%reg_44 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_42, i32 %reg_43
	%reg_45 = load float, ptr %reg_44
	%reg_46 = add i32 5, 0
	%reg_47 = load i32, ptr @k
	%reg_48 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_46, i32 %reg_47
	%reg_49 = load float, ptr %reg_48
	%reg_50 = add i32 6, 0
	%reg_51 = load i32, ptr @k
	%reg_52 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_50, i32 %reg_51
	%reg_53 = load float, ptr %reg_52
	%reg_54 = add i32 7, 0
	%reg_55 = load i32, ptr @k
	%reg_56 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_54, i32 %reg_55
	%reg_57 = load float, ptr %reg_56
	%reg_58 = add i32 8, 0
	%reg_59 = load i32, ptr @k
	%reg_60 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_58, i32 %reg_59
	%reg_61 = load float, ptr %reg_60
	%reg_62 = add i32 9, 0
	%reg_63 = load i32, ptr @k
	%reg_64 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_62, i32 %reg_63
	%reg_65 = load float, ptr %reg_64
	%reg_66 = add i32 10, 0
	%reg_67 = load i32, ptr @k
	%reg_68 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_66, i32 %reg_67
	%reg_69 = load float, ptr %reg_68
	%reg_70 = add i32 11, 0
	%reg_71 = load i32, ptr @k
	%reg_72 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_70, i32 %reg_71
	%reg_73 = load float, ptr %reg_72
	%reg_74 = add i32 12, 0
	%reg_75 = load i32, ptr @k
	%reg_76 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_74, i32 %reg_75
	%reg_77 = load float, ptr %reg_76
	%reg_78 = add i32 13, 0
	%reg_79 = load i32, ptr @k
	%reg_80 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_78, i32 %reg_79
	%reg_81 = load float, ptr %reg_80
	%reg_82 = add i32 14, 0
	%reg_83 = load i32, ptr @k
	%reg_84 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_82, i32 %reg_83
	%reg_85 = load float, ptr %reg_84
	%reg_86 = add i32 15, 0
	%reg_87 = load i32, ptr @k
	%reg_88 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_86, i32 %reg_87
	%reg_89 = load float, ptr %reg_88
	%reg_90 = add i32 16, 0
	%reg_91 = load i32, ptr @k
	%reg_92 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_90, i32 %reg_91
	%reg_93 = load float, ptr %reg_92
	%reg_94 = add i32 17, 0
	%reg_95 = load i32, ptr @k
	%reg_96 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_94, i32 %reg_95
	%reg_97 = load float, ptr %reg_96
	%reg_98 = add i32 18, 0
	%reg_99 = load i32, ptr @k
	%reg_100 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_98, i32 %reg_99
	%reg_101 = load float, ptr %reg_100
	%reg_102 = add i32 19, 0
	%reg_103 = load i32, ptr @k
	%reg_104 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_102, i32 %reg_103
	%reg_105 = load float, ptr %reg_104
	%reg_106 = add i32 20, 0
	%reg_107 = load i32, ptr @k
	%reg_108 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_106, i32 %reg_107
	%reg_109 = load float, ptr %reg_108
	%reg_110 = add i32 21, 0
	%reg_111 = load i32, ptr @k
	%reg_112 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_110, i32 %reg_111
	%reg_113 = load float, ptr %reg_112
	%reg_114 = add i32 22, 0
	%reg_115 = load i32, ptr @k
	%reg_116 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_114, i32 %reg_115
	%reg_117 = load float, ptr %reg_116
	%reg_118 = add i32 23, 0
	%reg_119 = load i32, ptr @k
	%reg_120 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_118, i32 %reg_119
	%reg_121 = load float, ptr %reg_120
	%reg_122 = add i32 24, 0
	%reg_123 = load i32, ptr @k
	%reg_124 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_122, i32 %reg_123
	%reg_125 = load float, ptr %reg_124
	%reg_126 = add i32 25, 0
	%reg_127 = load i32, ptr @k
	%reg_128 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_126, i32 %reg_127
	%reg_129 = load float, ptr %reg_128
	%reg_130 = add i32 26, 0
	%reg_131 = load i32, ptr @k
	%reg_132 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_130, i32 %reg_131
	%reg_133 = load float, ptr %reg_132
	%reg_134 = add i32 27, 0
	%reg_135 = load i32, ptr @k
	%reg_136 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_134, i32 %reg_135
	%reg_137 = load float, ptr %reg_136
	%reg_138 = add i32 28, 0
	%reg_139 = load i32, ptr @k
	%reg_140 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_138, i32 %reg_139
	%reg_141 = load float, ptr %reg_140
	%reg_142 = add i32 29, 0
	%reg_143 = load i32, ptr @k
	%reg_144 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_142, i32 %reg_143
	%reg_145 = load float, ptr %reg_144
	%reg_146 = add i32 30, 0
	%reg_147 = load i32, ptr @k
	%reg_148 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_146, i32 %reg_147
	%reg_149 = load float, ptr %reg_148
	%reg_150 = add i32 31, 0
	%reg_151 = load i32, ptr @k
	%reg_152 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_150, i32 %reg_151
	%reg_153 = load float, ptr %reg_152
	%reg_154 = add i32 32, 0
	%reg_155 = load i32, ptr @k
	%reg_156 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_154, i32 %reg_155
	%reg_157 = load float, ptr %reg_156
	%reg_158 = add i32 33, 0
	%reg_159 = load i32, ptr @k
	%reg_160 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_158, i32 %reg_159
	%reg_161 = load float, ptr %reg_160
	%reg_162 = add i32 34, 0
	%reg_163 = load i32, ptr @k
	%reg_164 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_162, i32 %reg_163
	%reg_165 = load float, ptr %reg_164
	%reg_166 = add i32 35, 0
	%reg_167 = load i32, ptr @k
	%reg_168 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_166, i32 %reg_167
	%reg_169 = load float, ptr %reg_168
	%reg_170 = add i32 36, 0
	%reg_171 = load i32, ptr @k
	%reg_172 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_170, i32 %reg_171
	%reg_173 = load float, ptr %reg_172
	%reg_174 = add i32 37, 0
	%reg_175 = load i32, ptr @k
	%reg_176 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_174, i32 %reg_175
	%reg_177 = load float, ptr %reg_176
	%reg_178 = add i32 38, 0
	%reg_179 = load i32, ptr @k
	%reg_180 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_178, i32 %reg_179
	%reg_181 = load float, ptr %reg_180
	%reg_182 = add i32 39, 0
	%reg_183 = load i32, ptr @k
	%reg_184 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_182, i32 %reg_183
	%reg_185 = load float, ptr %reg_184
	%reg_186 = call float @params_f40(float %reg_29, float %reg_33, float %reg_37, float %reg_41, float %reg_45, float %reg_49, float %reg_53, float %reg_57, float %reg_61, float %reg_65, float %reg_69, float %reg_73, float %reg_77, float %reg_81, float %reg_85, float %reg_89, float %reg_93, float %reg_97, float %reg_101, float %reg_105, float %reg_109, float %reg_113, float %reg_117, float %reg_121, float %reg_125, float %reg_129, float %reg_133, float %reg_137, float %reg_141, float %reg_145, float %reg_149, float %reg_153, float %reg_157, float %reg_161, float %reg_165, float %reg_169, float %reg_173, float %reg_177, float %reg_181, float %reg_185)
	store float %reg_186, ptr %reg_25
	%reg_188 = add i32 23, 0
	%reg_189 = load i32, ptr @k
	%reg_190 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_188, i32 %reg_189
	%reg_191 = load i32, ptr %reg_190
	%reg_192 = add i32 2, 0
	%reg_193 = load i32, ptr @k
	%reg_194 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_192, i32 %reg_193
	%reg_195 = load i32, ptr %reg_194
	%reg_196 = add i32 6, 0
	%reg_197 = load i32, ptr @k
	%reg_198 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_196, i32 %reg_197
	%reg_199 = load i32, ptr %reg_198
	%reg_200 = add i32 4, 0
	%reg_201 = load i32, ptr @k
	%reg_202 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_200, i32 %reg_201
	%reg_203 = load float, ptr %reg_202
	%reg_204 = add i32 1, 0
	%reg_205 = load i32, ptr @k
	%reg_206 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_204, i32 %reg_205
	%reg_207 = load i32, ptr %reg_206
	%reg_208 = add i32 4, 0
	%reg_209 = load i32, ptr @k
	%reg_210 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_208, i32 %reg_209
	%reg_211 = load i32, ptr %reg_210
	%reg_212 = add i32 5, 0
	%reg_213 = load i32, ptr @k
	%reg_214 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_212, i32 %reg_213
	%reg_215 = load i32, ptr %reg_214
	%reg_216 = add i32 8, 0
	%reg_217 = load i32, ptr @k
	%reg_218 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_216, i32 %reg_217
	%reg_219 = load float, ptr %reg_218
	%reg_220 = add i32 15, 0
	%reg_221 = load i32, ptr @k
	%reg_222 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_220, i32 %reg_221
	%reg_223 = load float, ptr %reg_222
	%reg_224 = add i32 7, 0
	%reg_225 = load i32, ptr @k
	%reg_226 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_224, i32 %reg_225
	%reg_227 = load float, ptr %reg_226
	%reg_228 = add i32 22, 0
	%reg_229 = load i32, ptr @k
	%reg_230 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_228, i32 %reg_229
	%reg_231 = load i32, ptr %reg_230
	%reg_232 = add i32 3, 0
	%reg_233 = load i32, ptr @k
	%reg_234 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_232, i32 %reg_233
	%reg_235 = load float, ptr %reg_234
	%reg_236 = add i32 28, 0
	%reg_237 = load i32, ptr @k
	%reg_238 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_236, i32 %reg_237
	%reg_239 = load float, ptr %reg_238
	%reg_240 = add i32 0, 0
	%reg_241 = load i32, ptr @k
	%reg_242 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_240, i32 %reg_241
	%reg_243 = load i32, ptr %reg_242
	%reg_244 = add i32 37, 0
	%reg_245 = load i32, ptr @k
	%reg_246 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_244, i32 %reg_245
	%reg_247 = load float, ptr %reg_246
	%reg_248 = add i32 19, 0
	%reg_249 = load i32, ptr @k
	%reg_250 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_248, i32 %reg_249
	%reg_251 = load i32, ptr %reg_250
	%reg_252 = add i32 30, 0
	%reg_253 = load i32, ptr @k
	%reg_254 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_252, i32 %reg_253
	%reg_255 = load float, ptr %reg_254
	%reg_256 = add i32 12, 0
	%reg_257 = load i32, ptr @k
	%reg_258 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_256, i32 %reg_257
	%reg_259 = load float, ptr %reg_258
	%reg_260 = add i32 1, 0
	%reg_261 = load i32, ptr @k
	%reg_262 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_260, i32 %reg_261
	%reg_263 = load float, ptr %reg_262
	%reg_264 = add i32 11, 0
	%reg_265 = load i32, ptr @k
	%reg_266 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_264, i32 %reg_265
	%reg_267 = load float, ptr %reg_266
	%reg_268 = add i32 38, 0
	%reg_269 = load i32, ptr @k
	%reg_270 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_268, i32 %reg_269
	%reg_271 = load float, ptr %reg_270
	%reg_272 = add i32 6, 0
	%reg_273 = load i32, ptr @k
	%reg_274 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_272, i32 %reg_273
	%reg_275 = load float, ptr %reg_274
	%reg_276 = add i32 7, 0
	%reg_277 = load i32, ptr @k
	%reg_278 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_276, i32 %reg_277
	%reg_279 = load i32, ptr %reg_278
	%reg_280 = add i32 32, 0
	%reg_281 = load i32, ptr @k
	%reg_282 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_280, i32 %reg_281
	%reg_283 = load float, ptr %reg_282
	%reg_284 = add i32 10, 0
	%reg_285 = load i32, ptr @k
	%reg_286 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_284, i32 %reg_285
	%reg_287 = load i32, ptr %reg_286
	%reg_288 = add i32 13, 0
	%reg_289 = load i32, ptr @k
	%reg_290 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_288, i32 %reg_289
	%reg_291 = load i32, ptr %reg_290
	%reg_292 = add i32 20, 0
	%reg_293 = load i32, ptr @k
	%reg_294 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_292, i32 %reg_293
	%reg_295 = load float, ptr %reg_294
	%reg_296 = add i32 33, 0
	%reg_297 = load i32, ptr @k
	%reg_298 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_296, i32 %reg_297
	%reg_299 = load float, ptr %reg_298
	%reg_300 = add i32 23, 0
	%reg_301 = load i32, ptr @k
	%reg_302 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_300, i32 %reg_301
	%reg_303 = load float, ptr %reg_302
	%reg_304 = add i32 9, 0
	%reg_305 = load i32, ptr @k
	%reg_306 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_304, i32 %reg_305
	%reg_307 = load float, ptr %reg_306
	%reg_308 = add i32 25, 0
	%reg_309 = load i32, ptr @k
	%reg_310 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_308, i32 %reg_309
	%reg_311 = load float, ptr %reg_310
	%reg_312 = add i32 8, 0
	%reg_313 = load i32, ptr @k
	%reg_314 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_312, i32 %reg_313
	%reg_315 = load i32, ptr %reg_314
	%reg_316 = add i32 39, 0
	%reg_317 = load i32, ptr @k
	%reg_318 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_316, i32 %reg_317
	%reg_319 = load float, ptr %reg_318
	%reg_320 = add i32 17, 0
	%reg_321 = load i32, ptr @k
	%reg_322 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_320, i32 %reg_321
	%reg_323 = load i32, ptr %reg_322
	%reg_324 = add i32 21, 0
	%reg_325 = load i32, ptr @k
	%reg_326 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_324, i32 %reg_325
	%reg_327 = load float, ptr %reg_326
	%reg_328 = add i32 16, 0
	%reg_329 = load i32, ptr @k
	%reg_330 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_328, i32 %reg_329
	%reg_331 = load float, ptr %reg_330
	%reg_332 = add i32 5, 0
	%reg_333 = load i32, ptr @k
	%reg_334 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_332, i32 %reg_333
	%reg_335 = load float, ptr %reg_334
	%reg_336 = add i32 34, 0
	%reg_337 = load i32, ptr @k
	%reg_338 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_336, i32 %reg_337
	%reg_339 = load float, ptr %reg_338
	%reg_340 = add i32 18, 0
	%reg_341 = load i32, ptr @k
	%reg_342 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_340, i32 %reg_341
	%reg_343 = load i32, ptr %reg_342
	%reg_344 = add i32 9, 0
	%reg_345 = load i32, ptr @k
	%reg_346 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_344, i32 %reg_345
	%reg_347 = load i32, ptr %reg_346
	%reg_348 = add i32 14, 0
	%reg_349 = load i32, ptr @k
	%reg_350 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_348, i32 %reg_349
	%reg_351 = load float, ptr %reg_350
	%reg_352 = add i32 10, 0
	%reg_353 = load i32, ptr @k
	%reg_354 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_352, i32 %reg_353
	%reg_355 = load float, ptr %reg_354
	%reg_356 = add i32 0, 0
	%reg_357 = load i32, ptr @k
	%reg_358 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_356, i32 %reg_357
	%reg_359 = load float, ptr %reg_358
	%reg_360 = add i32 12, 0
	%reg_361 = load i32, ptr @k
	%reg_362 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_360, i32 %reg_361
	%reg_363 = load i32, ptr %reg_362
	%reg_364 = add i32 31, 0
	%reg_365 = load i32, ptr @k
	%reg_366 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_364, i32 %reg_365
	%reg_367 = load float, ptr %reg_366
	%reg_368 = add i32 11, 0
	%reg_369 = load i32, ptr @k
	%reg_370 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_368, i32 %reg_369
	%reg_371 = load i32, ptr %reg_370
	%reg_372 = add i32 16, 0
	%reg_373 = load i32, ptr @k
	%reg_374 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_372, i32 %reg_373
	%reg_375 = load i32, ptr %reg_374
	%reg_376 = add i32 27, 0
	%reg_377 = load i32, ptr @k
	%reg_378 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_376, i32 %reg_377
	%reg_379 = load float, ptr %reg_378
	%reg_380 = add i32 24, 0
	%reg_381 = load i32, ptr @k
	%reg_382 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_380, i32 %reg_381
	%reg_383 = load float, ptr %reg_382
	%reg_384 = add i32 13, 0
	%reg_385 = load i32, ptr @k
	%reg_386 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_384, i32 %reg_385
	%reg_387 = load float, ptr %reg_386
	%reg_388 = add i32 29, 0
	%reg_389 = load i32, ptr @k
	%reg_390 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_388, i32 %reg_389
	%reg_391 = load float, ptr %reg_390
	%reg_392 = add i32 3, 0
	%reg_393 = load i32, ptr @k
	%reg_394 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_392, i32 %reg_393
	%reg_395 = load i32, ptr %reg_394
	%reg_396 = add i32 21, 0
	%reg_397 = load i32, ptr @k
	%reg_398 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_396, i32 %reg_397
	%reg_399 = load i32, ptr %reg_398
	%reg_400 = add i32 20, 0
	%reg_401 = load i32, ptr @k
	%reg_402 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_400, i32 %reg_401
	%reg_403 = load i32, ptr %reg_402
	%reg_404 = add i32 18, 0
	%reg_405 = load i32, ptr @k
	%reg_406 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_404, i32 %reg_405
	%reg_407 = load float, ptr %reg_406
	%reg_408 = add i32 19, 0
	%reg_409 = load i32, ptr @k
	%reg_410 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_408, i32 %reg_409
	%reg_411 = load float, ptr %reg_410
	%reg_412 = add i32 22, 0
	%reg_413 = load i32, ptr @k
	%reg_414 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_412, i32 %reg_413
	%reg_415 = load float, ptr %reg_414
	%reg_416 = add i32 26, 0
	%reg_417 = load i32, ptr @k
	%reg_418 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_416, i32 %reg_417
	%reg_419 = load float, ptr %reg_418
	%reg_420 = add i32 36, 0
	%reg_421 = load i32, ptr @k
	%reg_422 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_420, i32 %reg_421
	%reg_423 = load float, ptr %reg_422
	%reg_424 = add i32 17, 0
	%reg_425 = load i32, ptr @k
	%reg_426 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_424, i32 %reg_425
	%reg_427 = load float, ptr %reg_426
	%reg_428 = add i32 15, 0
	%reg_429 = load i32, ptr @k
	%reg_430 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_428, i32 %reg_429
	%reg_431 = load i32, ptr %reg_430
	%reg_432 = add i32 2, 0
	%reg_433 = load i32, ptr @k
	%reg_434 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_432, i32 %reg_433
	%reg_435 = load float, ptr %reg_434
	%reg_436 = add i32 14, 0
	%reg_437 = load i32, ptr @k
	%reg_438 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_436, i32 %reg_437
	%reg_439 = load i32, ptr %reg_438
	%reg_440 = add i32 35, 0
	%reg_441 = load i32, ptr @k
	%reg_442 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_440, i32 %reg_441
	%reg_443 = load float, ptr %reg_442
	%reg_444 = call float @params_f40_i24(i32 %reg_191, i32 %reg_195, i32 %reg_199, float %reg_203, i32 %reg_207, i32 %reg_211, i32 %reg_215, float %reg_219, float %reg_223, float %reg_227, i32 %reg_231, float %reg_235, float %reg_239, i32 %reg_243, float %reg_247, i32 %reg_251, float %reg_255, float %reg_259, float %reg_263, float %reg_267, float %reg_271, float %reg_275, i32 %reg_279, float %reg_283, i32 %reg_287, i32 %reg_291, float %reg_295, float %reg_299, float %reg_303, float %reg_307, float %reg_311, i32 %reg_315, float %reg_319, i32 %reg_323, float %reg_327, float %reg_331, float %reg_335, float %reg_339, i32 %reg_343, i32 %reg_347, float %reg_351, float %reg_355, float %reg_359, i32 %reg_363, float %reg_367, i32 %reg_371, i32 %reg_375, float %reg_379, float %reg_383, float %reg_387, float %reg_391, i32 %reg_395, i32 %reg_399, i32 %reg_403, float %reg_407, float %reg_411, float %reg_415, float %reg_419, float %reg_423, float %reg_427, i32 %reg_431, float %reg_435, i32 %reg_439, float %reg_443)
	store float %reg_444, ptr %reg_187
	%reg_446 = add i32 0, 0
	%reg_447 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_446
	%reg_448 = add i32 1, 0
	%reg_449 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_448
	%reg_450 = add i32 2, 0
	%reg_451 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_450
	%reg_452 = add i32 3, 0
	%reg_453 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_452
	%reg_454 = add i32 4, 0
	%reg_455 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_454
	%reg_456 = add i32 5, 0
	%reg_457 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_456
	%reg_458 = add i32 6, 0
	%reg_459 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_458
	%reg_460 = add i32 7, 0
	%reg_461 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_460
	%reg_462 = add i32 8, 0
	%reg_463 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_462
	%reg_464 = add i32 9, 0
	%reg_465 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_464
	%reg_466 = add i32 10, 0
	%reg_467 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_466
	%reg_468 = add i32 11, 0
	%reg_469 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_468
	%reg_470 = add i32 12, 0
	%reg_471 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_470
	%reg_472 = add i32 13, 0
	%reg_473 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_472
	%reg_474 = add i32 14, 0
	%reg_475 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_474
	%reg_476 = add i32 15, 0
	%reg_477 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_476
	%reg_478 = add i32 16, 0
	%reg_479 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_478
	%reg_480 = add i32 17, 0
	%reg_481 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_480
	%reg_482 = add i32 18, 0
	%reg_483 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_482
	%reg_484 = add i32 19, 0
	%reg_485 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_484
	%reg_486 = add i32 20, 0
	%reg_487 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_486
	%reg_488 = add i32 21, 0
	%reg_489 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_488
	%reg_490 = add i32 22, 0
	%reg_491 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_490
	%reg_492 = add i32 23, 0
	%reg_493 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_492
	%reg_494 = add i32 24, 0
	%reg_495 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_494
	%reg_496 = add i32 25, 0
	%reg_497 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_496
	%reg_498 = add i32 26, 0
	%reg_499 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_498
	%reg_500 = add i32 27, 0
	%reg_501 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_500
	%reg_502 = add i32 28, 0
	%reg_503 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_502
	%reg_504 = add i32 29, 0
	%reg_505 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_504
	%reg_506 = add i32 30, 0
	%reg_507 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_506
	%reg_508 = add i32 31, 0
	%reg_509 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_508
	%reg_510 = add i32 32, 0
	%reg_511 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_510
	%reg_512 = add i32 33, 0
	%reg_513 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_512
	%reg_514 = add i32 34, 0
	%reg_515 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_514
	%reg_516 = add i32 35, 0
	%reg_517 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_516
	%reg_518 = add i32 36, 0
	%reg_519 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_518
	%reg_520 = add i32 37, 0
	%reg_521 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_520
	%reg_522 = add i32 38, 0
	%reg_523 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_522
	%reg_524 = add i32 39, 0
	%reg_525 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_524
	%reg_526 = call float @params_fa40(ptr %reg_447, ptr %reg_449, ptr %reg_451, ptr %reg_453, ptr %reg_455, ptr %reg_457, ptr %reg_459, ptr %reg_461, ptr %reg_463, ptr %reg_465, ptr %reg_467, ptr %reg_469, ptr %reg_471, ptr %reg_473, ptr %reg_475, ptr %reg_477, ptr %reg_479, ptr %reg_481, ptr %reg_483, ptr %reg_485, ptr %reg_487, ptr %reg_489, ptr %reg_491, ptr %reg_493, ptr %reg_495, ptr %reg_497, ptr %reg_499, ptr %reg_501, ptr %reg_503, ptr %reg_505, ptr %reg_507, ptr %reg_509, ptr %reg_511, ptr %reg_513, ptr %reg_515, ptr %reg_517, ptr %reg_519, ptr %reg_521, ptr %reg_523, ptr %reg_525)
	store float %reg_526, ptr %reg_445
	%reg_528 = add i32 0, 0
	%reg_529 = load i32, ptr @k
	%reg_530 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_528, i32 %reg_529
	%reg_531 = load float, ptr %reg_530
	%reg_532 = add i32 0, 0
	%reg_533 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_532
	%reg_534 = add i32 1, 0
	%reg_535 = load i32, ptr @k
	%reg_536 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_534, i32 %reg_535
	%reg_537 = load i32, ptr %reg_536
	%reg_538 = add i32 1, 0
	%reg_539 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_538
	%reg_540 = add i32 2, 0
	%reg_541 = load i32, ptr @k
	%reg_542 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_540, i32 %reg_541
	%reg_543 = load float, ptr %reg_542
	%reg_544 = add i32 2, 0
	%reg_545 = load i32, ptr @k
	%reg_546 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_544, i32 %reg_545
	%reg_547 = load i32, ptr %reg_546
	%reg_548 = add i32 3, 0
	%reg_549 = load i32, ptr @k
	%reg_550 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_548, i32 %reg_549
	%reg_551 = load float, ptr %reg_550
	%reg_552 = add i32 4, 0
	%reg_553 = load i32, ptr @k
	%reg_554 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_552, i32 %reg_553
	%reg_555 = load float, ptr %reg_554
	%reg_556 = add i32 5, 0
	%reg_557 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_556
	%reg_558 = add i32 3, 0
	%reg_559 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_558
	%reg_560 = add i32 4, 0
	%reg_561 = load i32, ptr @k
	%reg_562 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_560, i32 %reg_561
	%reg_563 = load i32, ptr %reg_562
	%reg_564 = add i32 5, 0
	%reg_565 = load i32, ptr @k
	%reg_566 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_564, i32 %reg_565
	%reg_567 = load i32, ptr %reg_566
	%reg_568 = add i32 6, 0
	%reg_569 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_568
	%reg_570 = add i32 6, 0
	%reg_571 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_570
	%reg_572 = add i32 7, 0
	%reg_573 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_572
	%reg_574 = add i32 8, 0
	%reg_575 = load i32, ptr @k
	%reg_576 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_574, i32 %reg_575
	%reg_577 = load i32, ptr %reg_576
	%reg_578 = add i32 7, 0
	%reg_579 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_578
	%reg_580 = add i32 8, 0
	%reg_581 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_580
	%reg_582 = add i32 9, 0
	%reg_583 = load i32, ptr @k
	%reg_584 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_582, i32 %reg_583
	%reg_585 = load float, ptr %reg_584
	%reg_586 = add i32 10, 0
	%reg_587 = load i32, ptr @k
	%reg_588 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_586, i32 %reg_587
	%reg_589 = load float, ptr %reg_588
	%reg_590 = add i32 11, 0
	%reg_591 = load i32, ptr @k
	%reg_592 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_590, i32 %reg_591
	%reg_593 = load float, ptr %reg_592
	%reg_594 = add i32 12, 0
	%reg_595 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_594
	%reg_596 = add i32 9, 0
	%reg_597 = load i32, ptr @k
	%reg_598 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_596, i32 %reg_597
	%reg_599 = load i32, ptr %reg_598
	%reg_600 = add i32 13, 0
	%reg_601 = load i32, ptr @k
	%reg_602 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_600, i32 %reg_601
	%reg_603 = load float, ptr %reg_602
	%reg_604 = add i32 14, 0
	%reg_605 = load i32, ptr @k
	%reg_606 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_604, i32 %reg_605
	%reg_607 = load float, ptr %reg_606
	%reg_608 = add i32 15, 0
	%reg_609 = load i32, ptr @k
	%reg_610 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_608, i32 %reg_609
	%reg_611 = load float, ptr %reg_610
	%reg_612 = add i32 10, 0
	%reg_613 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_612
	%reg_614 = add i32 16, 0
	%reg_615 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_614
	%reg_616 = add i32 11, 0
	%reg_617 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_616
	%reg_618 = add i32 12, 0
	%reg_619 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_618
	%reg_620 = add i32 17, 0
	%reg_621 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_620
	%reg_622 = add i32 18, 0
	%reg_623 = load i32, ptr @k
	%reg_624 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_622, i32 %reg_623
	%reg_625 = load float, ptr %reg_624
	%reg_626 = add i32 19, 0
	%reg_627 = load i32, ptr @k
	%reg_628 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_626, i32 %reg_627
	%reg_629 = load float, ptr %reg_628
	%reg_630 = add i32 13, 0
	%reg_631 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_630
	%reg_632 = add i32 14, 0
	%reg_633 = load i32, ptr @k
	%reg_634 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_632, i32 %reg_633
	%reg_635 = load i32, ptr %reg_634
	%reg_636 = add i32 20, 0
	%reg_637 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_636
	%reg_638 = add i32 21, 0
	%reg_639 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_638
	%reg_640 = add i32 22, 0
	%reg_641 = load i32, ptr @k
	%reg_642 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_640, i32 %reg_641
	%reg_643 = load float, ptr %reg_642
	%reg_644 = add i32 23, 0
	%reg_645 = load i32, ptr @k
	%reg_646 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_644, i32 %reg_645
	%reg_647 = load float, ptr %reg_646
	%reg_648 = add i32 15, 0
	%reg_649 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_648
	%reg_650 = add i32 16, 0
	%reg_651 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_650
	%reg_652 = add i32 17, 0
	%reg_653 = load i32, ptr @k
	%reg_654 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_652, i32 %reg_653
	%reg_655 = load i32, ptr %reg_654
	%reg_656 = add i32 18, 0
	%reg_657 = load i32, ptr @k
	%reg_658 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_656, i32 %reg_657
	%reg_659 = load i32, ptr %reg_658
	%reg_660 = add i32 24, 0
	%reg_661 = load i32, ptr @k
	%reg_662 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_660, i32 %reg_661
	%reg_663 = load float, ptr %reg_662
	%reg_664 = add i32 25, 0
	%reg_665 = load i32, ptr @k
	%reg_666 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_664, i32 %reg_665
	%reg_667 = load float, ptr %reg_666
	%reg_668 = add i32 19, 0
	%reg_669 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_668
	%reg_670 = add i32 20, 0
	%reg_671 = load i32, ptr @k
	%reg_672 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_670, i32 %reg_671
	%reg_673 = load i32, ptr %reg_672
	%reg_674 = add i32 26, 0
	%reg_675 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_674
	%reg_676 = add i32 21, 0
	%reg_677 = load i32, ptr @k
	%reg_678 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_676, i32 %reg_677
	%reg_679 = load i32, ptr %reg_678
	%reg_680 = add i32 22, 0
	%reg_681 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_680
	%reg_682 = add i32 23, 0
	%reg_683 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_682
	%reg_684 = add i32 27, 0
	%reg_685 = load i32, ptr @k
	%reg_686 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_684, i32 %reg_685
	%reg_687 = load float, ptr %reg_686
	%reg_688 = add i32 28, 0
	%reg_689 = load i32, ptr @k
	%reg_690 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_688, i32 %reg_689
	%reg_691 = load float, ptr %reg_690
	%reg_692 = add i32 29, 0
	%reg_693 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_692
	%reg_694 = add i32 0, 0
	%reg_695 = load i32, ptr @k
	%reg_696 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_694, i32 %reg_695
	%reg_697 = load i32, ptr %reg_696
	%reg_698 = add i32 1, 0
	%reg_699 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_698
	%reg_700 = add i32 30, 0
	%reg_701 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_700
	%reg_702 = add i32 31, 0
	%reg_703 = load i32, ptr @k
	%reg_704 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_702, i32 %reg_703
	%reg_705 = load float, ptr %reg_704
	%reg_706 = add i32 2, 0
	%reg_707 = load i32, ptr @k
	%reg_708 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_706, i32 %reg_707
	%reg_709 = load i32, ptr %reg_708
	%reg_710 = add i32 32, 0
	%reg_711 = load i32, ptr @k
	%reg_712 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_710, i32 %reg_711
	%reg_713 = load float, ptr %reg_712
	%reg_714 = add i32 33, 0
	%reg_715 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_714
	%reg_716 = add i32 34, 0
	%reg_717 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_716
	%reg_718 = add i32 35, 0
	%reg_719 = load i32, ptr @k
	%reg_720 = getelementptr [40 x [3 x float]], ptr %reg_0, i32 0, i32 %reg_718, i32 %reg_719
	%reg_721 = load float, ptr %reg_720
	%reg_722 = add i32 3, 0
	%reg_723 = load i32, ptr @k
	%reg_724 = getelementptr [24 x [3 x i32]], ptr %reg_1, i32 0, i32 %reg_722, i32 %reg_723
	%reg_725 = load i32, ptr %reg_724
	%reg_726 = call i32 @params_mix(float %reg_531, ptr %reg_533, i32 %reg_537, ptr %reg_539, float %reg_543, i32 %reg_547, float %reg_551, float %reg_555, ptr %reg_557, ptr %reg_559, i32 %reg_563, i32 %reg_567, ptr %reg_569, ptr %reg_571, ptr %reg_573, i32 %reg_577, ptr %reg_579, ptr %reg_581, float %reg_585, float %reg_589, float %reg_593, ptr %reg_595, i32 %reg_599, float %reg_603, float %reg_607, float %reg_611, ptr %reg_613, ptr %reg_615, ptr %reg_617, ptr %reg_619, ptr %reg_621, float %reg_625, float %reg_629, ptr %reg_631, i32 %reg_635, ptr %reg_637, ptr %reg_639, float %reg_643, float %reg_647, ptr %reg_649, ptr %reg_651, i32 %reg_655, i32 %reg_659, float %reg_663, float %reg_667, ptr %reg_669, i32 %reg_673, ptr %reg_675, i32 %reg_679, ptr %reg_681, ptr %reg_683, float %reg_687, float %reg_691, ptr %reg_693, i32 %reg_697, ptr %reg_699, ptr %reg_701, float %reg_705, i32 %reg_709, float %reg_713, ptr %reg_715, ptr %reg_717, float %reg_721, i32 %reg_725)
	store i32 %reg_726, ptr %reg_527
	%reg_727 = load float, ptr %reg_25
	call void @putfloat(float %reg_727)
	%reg_728 = add i32 10, 0
	call void @putch(i32 %reg_728)
	%reg_729 = load float, ptr %reg_187
	call void @putfloat(float %reg_729)
	%reg_730 = add i32 10, 0
	call void @putch(i32 %reg_730)
	%reg_731 = load float, ptr %reg_445
	call void @putfloat(float %reg_731)
	%reg_732 = add i32 10, 0
	call void @putch(i32 %reg_732)
	%reg_733 = load i32, ptr %reg_527
	call void @putint(i32 %reg_733)
	%reg_734 = add i32 10, 0
	call void @putch(i32 %reg_734)
	%reg_735 = add i32 0, 0
	ret i32 %reg_735
}
