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

@MAX_DIM_X = global i32 8
@MAX_DIM_Y = global i32 8
@test_block = global [8 x [8 x float]] zeroinitializer
@test_dct = global [8 x [8 x float]] zeroinitializer
@test_idct = global [8 x [8 x float]] zeroinitializer
@PI = global float 0x400921fb60000000
@TWO_PI = global float 0x401921fb60000000
@EPSILON = global float 0x3eb0c6f7a0000000

define float @my_fabs(float %reg_0)
{
Block0:  ;Func define at line 11
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 11
	%reg_2 = load float, ptr %reg_1
	%reg_3 = add i32 0, 0
	%reg_4 = sitofp i32 %reg_3 to float
	%reg_5 = fcmp ogt float %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 12
	%reg_6 = load float, ptr %reg_1
	ret float %reg_6
	br label %Block3
Block3:  ;If end at line 12
	%reg_7 = load float, ptr %reg_1
	%reg_8 = fsub float 0x0, %reg_7
	ret float %reg_8
}

define float @p(float %reg_0)
{
Block0:  ;Func define at line 16
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 16
	%reg_2 = add i32 3, 0
	%reg_3 = sitofp i32 %reg_2 to float
	%reg_4 = load float, ptr %reg_1
	%reg_5 = fmul float %reg_3, %reg_4
	%reg_6 = add i32 4, 0
	%reg_7 = sitofp i32 %reg_6 to float
	%reg_8 = load float, ptr %reg_1
	%reg_9 = fmul float %reg_7, %reg_8
	%reg_10 = load float, ptr %reg_1
	%reg_11 = fmul float %reg_9, %reg_10
	%reg_12 = load float, ptr %reg_1
	%reg_13 = fmul float %reg_11, %reg_12
	%reg_14 = fsub float %reg_5, %reg_13
	ret float %reg_14
}

define float @my_sin_impl(float %reg_0)
{
Block0:  ;Func define at line 18
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 18
	%reg_2 = load float, ptr %reg_1
	%reg_3 = call float @my_fabs(float %reg_2)
	%reg_4 = load float, ptr @EPSILON
	%reg_5 = fcmp ole float %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 19
	%reg_6 = load float, ptr %reg_1
	ret float %reg_6
	br label %Block3
Block3:  ;If end at line 19
	%reg_7 = load float, ptr %reg_1
	%reg_8 = fadd float 0x4008000000000000, 0x0
	%reg_9 = fdiv float %reg_7, %reg_8
	%reg_10 = call float @my_sin_impl(float %reg_9)
	%reg_11 = call float @p(float %reg_10)
	ret float %reg_11
}

define float @my_sin(float %reg_0)
{
Block0:  ;Func define at line 23
	%reg_9 = alloca i32
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 23
	%reg_2 = load float, ptr %reg_1
	%reg_3 = load float, ptr @TWO_PI
	%reg_4 = fcmp ogt float %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block4
Block2:  ;If then at line 24
	%reg_10 = load float, ptr %reg_1
	%reg_11 = load float, ptr @TWO_PI
	%reg_12 = fdiv float %reg_10, %reg_11
	%reg_13 = fptosi float %reg_12 to i32
	store i32 %reg_13, ptr %reg_9
	%reg_14 = load float, ptr %reg_1
	%reg_15 = load i32, ptr %reg_9
	%reg_16 = sitofp i32 %reg_15 to float
	%reg_17 = load float, ptr @TWO_PI
	%reg_18 = fmul float %reg_16, %reg_17
	%reg_19 = fsub float %reg_14, %reg_18
	store float %reg_19, ptr %reg_1
	br label %Block3
Block3:  ;If end at line 24
	%reg_20 = load float, ptr %reg_1
	%reg_21 = load float, ptr @PI
	%reg_22 = fcmp ogt float %reg_20, %reg_21
	br i1 %reg_22, label %Block5, label %Block6
Block4:  ;Or opertor at line 24
	%reg_5 = load float, ptr %reg_1
	%reg_6 = load float, ptr @TWO_PI
	%reg_7 = fsub float 0x0, %reg_6
	%reg_8 = fcmp olt float %reg_5, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block5:  ;If then at line 28
	%reg_23 = load float, ptr %reg_1
	%reg_24 = load float, ptr @TWO_PI
	%reg_25 = fsub float %reg_23, %reg_24
	store float %reg_25, ptr %reg_1
	br label %Block6
Block6:  ;If end at line 28
	%reg_26 = load float, ptr %reg_1
	%reg_27 = load float, ptr @PI
	%reg_28 = fsub float 0x0, %reg_27
	%reg_29 = fcmp olt float %reg_26, %reg_28
	br i1 %reg_29, label %Block7, label %Block8
Block7:  ;If then at line 29
	%reg_30 = load float, ptr %reg_1
	%reg_31 = load float, ptr @TWO_PI
	%reg_32 = fadd float %reg_30, %reg_31
	store float %reg_32, ptr %reg_1
	br label %Block8
Block8:  ;If end at line 29
	%reg_33 = load float, ptr %reg_1
	%reg_34 = call float @my_sin_impl(float %reg_33)
	ret float %reg_34
}

define float @my_cos(float %reg_0)
{
Block0:  ;Func define at line 33
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 33
	%reg_2 = load float, ptr %reg_1
	%reg_3 = load float, ptr @PI
	%reg_4 = add i32 2, 0
	%reg_5 = sitofp i32 %reg_4 to float
	%reg_6 = fdiv float %reg_3, %reg_5
	%reg_7 = fadd float %reg_2, %reg_6
	%reg_8 = call float @my_sin(float %reg_7)
	ret float %reg_8
}

define void @write_mat(ptr %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 35
	%reg_14 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_1, ptr %reg_3
	store i32 %reg_2, ptr %reg_4
	br label %Block1
Block1:  ;Func body at line 35
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	br label %Block2
Block2:  ;While condition at line 37
	%reg_7 = load i32, ptr %reg_5
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = icmp slt i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block3, label %Block4
Block3:  ;While body at line 37
	%reg_10 = load i32, ptr %reg_5
	%reg_11 = add i32 0, 0
	%reg_12 = getelementptr [8 x float], ptr %reg_0, i32 %reg_10, i32 %reg_11
	%reg_13 = load float, ptr %reg_12
	call void @putfloat(float %reg_13)
	%reg_15 = add i32 1, 0
	store i32 %reg_15, ptr %reg_14
	br label %Block5
Block4:  ;While end at line 37
	%reg_31 = add i32 10, 0
	call void @putch(i32 %reg_31)
	ret void
Block5:  ;While condition at line 40
	%reg_16 = load i32, ptr %reg_14
	%reg_17 = load i32, ptr %reg_4
	%reg_18 = icmp slt i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block6, label %Block7
Block6:  ;While body at line 40
	%reg_19 = add i32 32, 0
	call void @putch(i32 %reg_19)
	%reg_20 = load i32, ptr %reg_5
	%reg_21 = load i32, ptr %reg_14
	%reg_22 = getelementptr [8 x float], ptr %reg_0, i32 %reg_20, i32 %reg_21
	%reg_23 = load float, ptr %reg_22
	call void @putfloat(float %reg_23)
	%reg_24 = load i32, ptr %reg_14
	%reg_25 = add i32 1, 0
	%reg_26 = add i32 %reg_24, %reg_25
	store i32 %reg_26, ptr %reg_14
	br label %Block5
Block7:  ;While end at line 40
	%reg_27 = add i32 10, 0
	call void @putch(i32 %reg_27)
	%reg_28 = load i32, ptr %reg_5
	%reg_29 = add i32 1, 0
	%reg_30 = add i32 %reg_28, %reg_29
	store i32 %reg_30, ptr %reg_5
	br label %Block2
}

define void @dct(ptr %reg_0, ptr %reg_1, i32 %reg_2, i32 %reg_3)
{
Block0:  ;Func define at line 51
	%reg_26 = alloca i32
	%reg_21 = alloca i32
	%reg_11 = alloca i32
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	store i32 %reg_2, ptr %reg_4
	store i32 %reg_3, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 51
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	br label %Block2
Block2:  ;While condition at line 53
	%reg_8 = load i32, ptr %reg_6
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = icmp slt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 53
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_11
	br label %Block5
Block4:  ;While end at line 53
	ret void
Block5:  ;While condition at line 55
	%reg_13 = load i32, ptr %reg_11
	%reg_14 = load i32, ptr %reg_5
	%reg_15 = icmp slt i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block6, label %Block7
Block6:  ;While body at line 55
	%reg_16 = load i32, ptr %reg_6
	%reg_17 = load i32, ptr %reg_11
	%reg_18 = getelementptr [8 x float], ptr %reg_0, i32 %reg_16, i32 %reg_17
	%reg_19 = add i32 0, 0
	%reg_20 = sitofp i32 %reg_19 to float
	store float %reg_20, ptr %reg_18
	%reg_22 = add i32 0, 0
	store i32 %reg_22, ptr %reg_21
	br label %Block8
Block7:  ;While end at line 55
	%reg_84 = load i32, ptr %reg_6
	%reg_85 = add i32 1, 0
	%reg_86 = add i32 %reg_84, %reg_85
	store i32 %reg_86, ptr %reg_6
	br label %Block2
Block8:  ;While condition at line 58
	%reg_23 = load i32, ptr %reg_21
	%reg_24 = load i32, ptr %reg_4
	%reg_25 = icmp slt i32 %reg_23, %reg_24
	br i1 %reg_25, label %Block9, label %Block10
Block9:  ;While body at line 58
	%reg_27 = add i32 0, 0
	store i32 %reg_27, ptr %reg_26
	br label %Block11
Block10:  ;While end at line 58
	%reg_81 = load i32, ptr %reg_11
	%reg_82 = add i32 1, 0
	%reg_83 = add i32 %reg_81, %reg_82
	store i32 %reg_83, ptr %reg_11
	br label %Block5
Block11:  ;While condition at line 60
	%reg_28 = load i32, ptr %reg_26
	%reg_29 = load i32, ptr %reg_5
	%reg_30 = icmp slt i32 %reg_28, %reg_29
	br i1 %reg_30, label %Block12, label %Block13
Block12:  ;While body at line 60
	%reg_31 = load i32, ptr %reg_6
	%reg_32 = load i32, ptr %reg_11
	%reg_33 = getelementptr [8 x float], ptr %reg_0, i32 %reg_31, i32 %reg_32
	%reg_34 = load i32, ptr %reg_6
	%reg_35 = load i32, ptr %reg_11
	%reg_36 = getelementptr [8 x float], ptr %reg_0, i32 %reg_34, i32 %reg_35
	%reg_37 = load float, ptr %reg_36
	%reg_38 = load i32, ptr %reg_21
	%reg_39 = load i32, ptr %reg_26
	%reg_40 = getelementptr [8 x float], ptr %reg_1, i32 %reg_38, i32 %reg_39
	%reg_41 = load float, ptr %reg_40
	%reg_42 = load float, ptr @PI
	%reg_43 = load i32, ptr %reg_4
	%reg_44 = sitofp i32 %reg_43 to float
	%reg_45 = fdiv float %reg_42, %reg_44
	%reg_46 = load i32, ptr %reg_21
	%reg_47 = sitofp i32 %reg_46 to float
	%reg_48 = fadd float 0x3ff0000000000000, 0x0
	%reg_49 = fadd float 0x4000000000000000, 0x0
	%reg_50 = fdiv float %reg_48, %reg_49
	%reg_51 = fadd float %reg_47, %reg_50
	%reg_52 = fmul float %reg_45, %reg_51
	%reg_53 = load i32, ptr %reg_6
	%reg_54 = sitofp i32 %reg_53 to float
	%reg_55 = fmul float %reg_52, %reg_54
	%reg_56 = call float @my_cos(float %reg_55)
	%reg_57 = fmul float %reg_41, %reg_56
	%reg_58 = load float, ptr @PI
	%reg_59 = load i32, ptr %reg_5
	%reg_60 = sitofp i32 %reg_59 to float
	%reg_61 = fdiv float %reg_58, %reg_60
	%reg_62 = load i32, ptr %reg_26
	%reg_63 = sitofp i32 %reg_62 to float
	%reg_64 = fadd float 0x3ff0000000000000, 0x0
	%reg_65 = fadd float 0x4000000000000000, 0x0
	%reg_66 = fdiv float %reg_64, %reg_65
	%reg_67 = fadd float %reg_63, %reg_66
	%reg_68 = fmul float %reg_61, %reg_67
	%reg_69 = load i32, ptr %reg_11
	%reg_70 = sitofp i32 %reg_69 to float
	%reg_71 = fmul float %reg_68, %reg_70
	%reg_72 = call float @my_cos(float %reg_71)
	%reg_73 = fmul float %reg_57, %reg_72
	%reg_74 = fadd float %reg_37, %reg_73
	store float %reg_74, ptr %reg_33
	%reg_75 = load i32, ptr %reg_26
	%reg_76 = add i32 1, 0
	%reg_77 = add i32 %reg_75, %reg_76
	store i32 %reg_77, ptr %reg_26
	br label %Block11
Block13:  ;While end at line 60
	%reg_78 = load i32, ptr %reg_21
	%reg_79 = add i32 1, 0
	%reg_80 = add i32 %reg_78, %reg_79
	store i32 %reg_80, ptr %reg_21
	br label %Block8
}

define void @idct(ptr %reg_0, ptr %reg_1, i32 %reg_2, i32 %reg_3)
{
Block0:  ;Func define at line 74
	%reg_30 = alloca i32
	%reg_28 = alloca i32
	%reg_11 = alloca i32
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	store i32 %reg_2, ptr %reg_4
	store i32 %reg_3, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 74
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	br label %Block2
Block2:  ;While condition at line 76
	%reg_8 = load i32, ptr %reg_6
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = icmp slt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 76
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_11
	br label %Block5
Block4:  ;While end at line 76
	ret void
Block5:  ;While condition at line 78
	%reg_13 = load i32, ptr %reg_11
	%reg_14 = load i32, ptr %reg_5
	%reg_15 = icmp slt i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block6, label %Block7
Block6:  ;While body at line 78
	%reg_16 = load i32, ptr %reg_6
	%reg_17 = load i32, ptr %reg_11
	%reg_18 = getelementptr [8 x float], ptr %reg_0, i32 %reg_16, i32 %reg_17
	%reg_19 = add i32 1, 0
	%reg_20 = sitofp i32 %reg_19 to float
	%reg_21 = fadd float 0x4010000000000000, 0x0
	%reg_22 = fdiv float %reg_20, %reg_21
	%reg_23 = add i32 0, 0
	%reg_24 = add i32 0, 0
	%reg_25 = getelementptr [8 x float], ptr %reg_1, i32 %reg_23, i32 %reg_24
	%reg_26 = load float, ptr %reg_25
	%reg_27 = fmul float %reg_22, %reg_26
	store float %reg_27, ptr %reg_18
	%reg_29 = add i32 0, 0
	store i32 %reg_29, ptr %reg_28
	%reg_31 = add i32 0, 0
	store i32 %reg_31, ptr %reg_30
	%reg_32 = add i32 1, 0
	store i32 %reg_32, ptr %reg_28
	br label %Block8
Block7:  ;While end at line 78
	%reg_158 = load i32, ptr %reg_6
	%reg_159 = add i32 1, 0
	%reg_160 = add i32 %reg_158, %reg_159
	store i32 %reg_160, ptr %reg_6
	br label %Block2
Block8:  ;While condition at line 83
	%reg_33 = load i32, ptr %reg_28
	%reg_34 = load i32, ptr %reg_4
	%reg_35 = icmp slt i32 %reg_33, %reg_34
	br i1 %reg_35, label %Block9, label %Block10
Block9:  ;While body at line 83
	%reg_36 = load i32, ptr %reg_6
	%reg_37 = load i32, ptr %reg_11
	%reg_38 = getelementptr [8 x float], ptr %reg_0, i32 %reg_36, i32 %reg_37
	%reg_39 = load i32, ptr %reg_6
	%reg_40 = load i32, ptr %reg_11
	%reg_41 = getelementptr [8 x float], ptr %reg_0, i32 %reg_39, i32 %reg_40
	%reg_42 = load float, ptr %reg_41
	%reg_43 = add i32 1, 0
	%reg_44 = sitofp i32 %reg_43 to float
	%reg_45 = fadd float 0x4000000000000000, 0x0
	%reg_46 = fdiv float %reg_44, %reg_45
	%reg_47 = load i32, ptr %reg_28
	%reg_48 = add i32 0, 0
	%reg_49 = getelementptr [8 x float], ptr %reg_1, i32 %reg_47, i32 %reg_48
	%reg_50 = load float, ptr %reg_49
	%reg_51 = fmul float %reg_46, %reg_50
	%reg_52 = fadd float %reg_42, %reg_51
	store float %reg_52, ptr %reg_38
	%reg_53 = load i32, ptr %reg_28
	%reg_54 = add i32 1, 0
	%reg_55 = add i32 %reg_53, %reg_54
	store i32 %reg_55, ptr %reg_28
	br label %Block8
Block10:  ;While end at line 83
	%reg_56 = add i32 1, 0
	store i32 %reg_56, ptr %reg_30
	br label %Block11
Block11:  ;While condition at line 88
	%reg_57 = load i32, ptr %reg_30
	%reg_58 = load i32, ptr %reg_5
	%reg_59 = icmp slt i32 %reg_57, %reg_58
	br i1 %reg_59, label %Block12, label %Block13
Block12:  ;While body at line 88
	%reg_60 = load i32, ptr %reg_6
	%reg_61 = load i32, ptr %reg_11
	%reg_62 = getelementptr [8 x float], ptr %reg_0, i32 %reg_60, i32 %reg_61
	%reg_63 = load i32, ptr %reg_6
	%reg_64 = load i32, ptr %reg_11
	%reg_65 = getelementptr [8 x float], ptr %reg_0, i32 %reg_63, i32 %reg_64
	%reg_66 = load float, ptr %reg_65
	%reg_67 = add i32 1, 0
	%reg_68 = sitofp i32 %reg_67 to float
	%reg_69 = fadd float 0x4000000000000000, 0x0
	%reg_70 = fdiv float %reg_68, %reg_69
	%reg_71 = add i32 0, 0
	%reg_72 = load i32, ptr %reg_30
	%reg_73 = getelementptr [8 x float], ptr %reg_1, i32 %reg_71, i32 %reg_72
	%reg_74 = load float, ptr %reg_73
	%reg_75 = fmul float %reg_70, %reg_74
	%reg_76 = fadd float %reg_66, %reg_75
	store float %reg_76, ptr %reg_62
	%reg_77 = load i32, ptr %reg_30
	%reg_78 = add i32 1, 0
	%reg_79 = add i32 %reg_77, %reg_78
	store i32 %reg_79, ptr %reg_30
	br label %Block11
Block13:  ;While end at line 88
	%reg_80 = add i32 1, 0
	store i32 %reg_80, ptr %reg_28
	br label %Block14
Block14:  ;While condition at line 94
	%reg_81 = load i32, ptr %reg_28
	%reg_82 = load i32, ptr %reg_4
	%reg_83 = icmp slt i32 %reg_81, %reg_82
	br i1 %reg_83, label %Block15, label %Block16
Block15:  ;While body at line 94
	%reg_84 = add i32 1, 0
	store i32 %reg_84, ptr %reg_30
	br label %Block17
Block16:  ;While end at line 94
	%reg_138 = load i32, ptr %reg_6
	%reg_139 = load i32, ptr %reg_11
	%reg_140 = getelementptr [8 x float], ptr %reg_0, i32 %reg_138, i32 %reg_139
	%reg_141 = load i32, ptr %reg_6
	%reg_142 = load i32, ptr %reg_11
	%reg_143 = getelementptr [8 x float], ptr %reg_0, i32 %reg_141, i32 %reg_142
	%reg_144 = load float, ptr %reg_143
	%reg_145 = fadd float 0x4000000000000000, 0x0
	%reg_146 = fmul float %reg_144, %reg_145
	%reg_147 = load i32, ptr %reg_4
	%reg_148 = sitofp i32 %reg_147 to float
	%reg_149 = fdiv float %reg_146, %reg_148
	%reg_150 = fadd float 0x4000000000000000, 0x0
	%reg_151 = fmul float %reg_149, %reg_150
	%reg_152 = load i32, ptr %reg_5
	%reg_153 = sitofp i32 %reg_152 to float
	%reg_154 = fdiv float %reg_151, %reg_153
	store float %reg_154, ptr %reg_140
	%reg_155 = load i32, ptr %reg_11
	%reg_156 = add i32 1, 0
	%reg_157 = add i32 %reg_155, %reg_156
	store i32 %reg_157, ptr %reg_11
	br label %Block5
Block17:  ;While condition at line 96
	%reg_85 = load i32, ptr %reg_30
	%reg_86 = load i32, ptr %reg_5
	%reg_87 = icmp slt i32 %reg_85, %reg_86
	br i1 %reg_87, label %Block18, label %Block19
Block18:  ;While body at line 96
	%reg_88 = load i32, ptr %reg_6
	%reg_89 = load i32, ptr %reg_11
	%reg_90 = getelementptr [8 x float], ptr %reg_0, i32 %reg_88, i32 %reg_89
	%reg_91 = load i32, ptr %reg_6
	%reg_92 = load i32, ptr %reg_11
	%reg_93 = getelementptr [8 x float], ptr %reg_0, i32 %reg_91, i32 %reg_92
	%reg_94 = load float, ptr %reg_93
	%reg_95 = load i32, ptr %reg_28
	%reg_96 = load i32, ptr %reg_30
	%reg_97 = getelementptr [8 x float], ptr %reg_1, i32 %reg_95, i32 %reg_96
	%reg_98 = load float, ptr %reg_97
	%reg_99 = load float, ptr @PI
	%reg_100 = load i32, ptr %reg_4
	%reg_101 = sitofp i32 %reg_100 to float
	%reg_102 = fdiv float %reg_99, %reg_101
	%reg_103 = load i32, ptr %reg_6
	%reg_104 = sitofp i32 %reg_103 to float
	%reg_105 = fadd float 0x3ff0000000000000, 0x0
	%reg_106 = fadd float 0x4000000000000000, 0x0
	%reg_107 = fdiv float %reg_105, %reg_106
	%reg_108 = fadd float %reg_104, %reg_107
	%reg_109 = fmul float %reg_102, %reg_108
	%reg_110 = load i32, ptr %reg_28
	%reg_111 = sitofp i32 %reg_110 to float
	%reg_112 = fmul float %reg_109, %reg_111
	%reg_113 = call float @my_cos(float %reg_112)
	%reg_114 = fmul float %reg_98, %reg_113
	%reg_115 = load float, ptr @PI
	%reg_116 = load i32, ptr %reg_5
	%reg_117 = sitofp i32 %reg_116 to float
	%reg_118 = fdiv float %reg_115, %reg_117
	%reg_119 = load i32, ptr %reg_11
	%reg_120 = sitofp i32 %reg_119 to float
	%reg_121 = fadd float 0x3ff0000000000000, 0x0
	%reg_122 = fadd float 0x4000000000000000, 0x0
	%reg_123 = fdiv float %reg_121, %reg_122
	%reg_124 = fadd float %reg_120, %reg_123
	%reg_125 = fmul float %reg_118, %reg_124
	%reg_126 = load i32, ptr %reg_30
	%reg_127 = sitofp i32 %reg_126 to float
	%reg_128 = fmul float %reg_125, %reg_127
	%reg_129 = call float @my_cos(float %reg_128)
	%reg_130 = fmul float %reg_114, %reg_129
	%reg_131 = fadd float %reg_94, %reg_130
	store float %reg_131, ptr %reg_90
	%reg_132 = load i32, ptr %reg_30
	%reg_133 = add i32 1, 0
	%reg_134 = add i32 %reg_132, %reg_133
	store i32 %reg_134, ptr %reg_30
	br label %Block17
Block19:  ;While end at line 96
	%reg_135 = load i32, ptr %reg_28
	%reg_136 = add i32 1, 0
	%reg_137 = add i32 %reg_135, %reg_136
	store i32 %reg_137, ptr %reg_28
	br label %Block14
}

define i32 @main()
{
Block0:  ;Func define at line 111
	%reg_9 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 111
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	%reg_3 = call i32 @getint()
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 115
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = load i32, ptr %reg_0
	%reg_8 = icmp slt i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 115
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block5
Block4:  ;While end at line 115
	%reg_24 = getelementptr [8 x [8 x float]], ptr @test_dct, i32 0
	%reg_25 = getelementptr [8 x [8 x float]], ptr @test_block, i32 0
	%reg_26 = load i32, ptr %reg_0
	%reg_27 = load i32, ptr %reg_2
	call void @dct(ptr %reg_24, ptr %reg_25, i32 %reg_26, i32 %reg_27)
	%reg_28 = getelementptr [8 x [8 x float]], ptr @test_dct, i32 0
	%reg_29 = load i32, ptr %reg_0
	%reg_30 = load i32, ptr %reg_2
	call void @write_mat(ptr %reg_28, i32 %reg_29, i32 %reg_30)
	%reg_31 = getelementptr [8 x [8 x float]], ptr @test_idct, i32 0
	%reg_32 = getelementptr [8 x [8 x float]], ptr @test_dct, i32 0
	%reg_33 = load i32, ptr %reg_0
	%reg_34 = load i32, ptr %reg_2
	call void @idct(ptr %reg_31, ptr %reg_32, i32 %reg_33, i32 %reg_34)
	%reg_35 = getelementptr [8 x [8 x float]], ptr @test_idct, i32 0
	%reg_36 = load i32, ptr %reg_0
	%reg_37 = load i32, ptr %reg_2
	call void @write_mat(ptr %reg_35, i32 %reg_36, i32 %reg_37)
	%reg_38 = add i32 0, 0
	ret i32 %reg_38
Block5:  ;While condition at line 117
	%reg_11 = load i32, ptr %reg_9
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block6, label %Block7
Block6:  ;While body at line 117
	%reg_14 = load i32, ptr %reg_4
	%reg_15 = load i32, ptr %reg_9
	%reg_16 = getelementptr [8 x [8 x float]], ptr @test_block, i32 0, i32 %reg_14, i32 %reg_15
	%reg_17 = call float @getfloat()
	store float %reg_17, ptr %reg_16
	%reg_18 = load i32, ptr %reg_9
	%reg_19 = add i32 1, 0
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_9
	br label %Block5
Block7:  ;While end at line 117
	%reg_21 = load i32, ptr %reg_4
	%reg_22 = add i32 1, 0
	%reg_23 = add i32 %reg_21, %reg_22
	store i32 %reg_23, ptr %reg_4
	br label %Block2
}
