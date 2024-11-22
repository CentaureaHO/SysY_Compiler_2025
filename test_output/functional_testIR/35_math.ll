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

@e = global float 0x4005bf0a80000000

define float @my_fabs(float %reg_0)
{
Block0:  ;Func define at line 5
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 5
	%reg_2 = load float, ptr %reg_1
	%reg_3 = add i32 0, 0
	%reg_4 = sitofp i32 %reg_3 to float
	%reg_5 = fcmp ogt float %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 6
	%reg_6 = load float, ptr %reg_1
	ret float %reg_6
	br label %Block3
Block3:  ;If end at line 6
	%reg_7 = load float, ptr %reg_1
	%reg_8 = fsub float 0x0, %reg_7
	ret float %reg_8
}

define float @my_pow(float %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 10
	%reg_14 = alloca float
	%reg_3 = alloca i32
	%reg_2 = alloca float
	store float %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 10
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = add i32 0, 0
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 11
	%reg_7 = add i32 1, 0
	%reg_8 = sitofp i32 %reg_7 to float
	%reg_9 = load float, ptr %reg_2
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = sub i32 0, %reg_10
	%reg_12 = call float @my_pow(float %reg_9, i32 %reg_11)
	%reg_13 = fdiv float %reg_8, %reg_12
	ret float %reg_13
	br label %Block3
Block3:  ;If end at line 11
	%reg_15 = fadd float 0x3ff0000000000000, 0x0
	store float %reg_15, ptr %reg_14
	br label %Block4
Block4:  ;While condition at line 13
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = icmp ne i32 %reg_16, 0
	br i1 %reg_17, label %Block5, label %Block6
Block5:  ;While body at line 13
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = add i32 2, 0
	%reg_20 = srem i32 %reg_18, %reg_19
	%reg_21 = icmp ne i32 %reg_20, 0
	br i1 %reg_21, label %Block7, label %Block8
Block6:  ;While end at line 13
	%reg_31 = load float, ptr %reg_14
	ret float %reg_31
Block7:  ;If then at line 14
	%reg_22 = load float, ptr %reg_14
	%reg_23 = load float, ptr %reg_2
	%reg_24 = fmul float %reg_22, %reg_23
	store float %reg_24, ptr %reg_14
	br label %Block8
Block8:  ;If end at line 14
	%reg_25 = load float, ptr %reg_2
	%reg_26 = load float, ptr %reg_2
	%reg_27 = fmul float %reg_25, %reg_26
	store float %reg_27, ptr %reg_2
	%reg_28 = load i32, ptr %reg_3
	%reg_29 = add i32 2, 0
	%reg_30 = sdiv i32 %reg_28, %reg_29
	store i32 %reg_30, ptr %reg_3
	br label %Block4
}

define float @my_sqrt(float %reg_0)
{
Block0:  ;Func define at line 21
	%reg_30 = alloca i32
	%reg_13 = alloca float
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 21
	%reg_2 = load float, ptr %reg_1
	%reg_3 = add i32 100, 0
	%reg_4 = sitofp i32 %reg_3 to float
	%reg_5 = fcmp ogt float %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 22
	%reg_6 = fadd float 0x4024000000000000, 0x0
	%reg_7 = load float, ptr %reg_1
	%reg_8 = add i32 100, 0
	%reg_9 = sitofp i32 %reg_8 to float
	%reg_10 = fdiv float %reg_7, %reg_9
	%reg_11 = call float @my_sqrt(float %reg_10)
	%reg_12 = fmul float %reg_6, %reg_11
	ret float %reg_12
	br label %Block3
Block3:  ;If end at line 22
	%reg_14 = load float, ptr %reg_1
	%reg_15 = add i32 8, 0
	%reg_16 = sitofp i32 %reg_15 to float
	%reg_17 = fdiv float %reg_14, %reg_16
	%reg_18 = fadd float 0x3fe0000000000000, 0x0
	%reg_19 = fadd float %reg_17, %reg_18
	%reg_20 = add i32 2, 0
	%reg_21 = sitofp i32 %reg_20 to float
	%reg_22 = load float, ptr %reg_1
	%reg_23 = fmul float %reg_21, %reg_22
	%reg_24 = add i32 4, 0
	%reg_25 = sitofp i32 %reg_24 to float
	%reg_26 = load float, ptr %reg_1
	%reg_27 = fadd float %reg_25, %reg_26
	%reg_28 = fdiv float %reg_23, %reg_27
	%reg_29 = fadd float %reg_19, %reg_28
	store float %reg_29, ptr %reg_13
	%reg_31 = add i32 10, 0
	store i32 %reg_31, ptr %reg_30
	br label %Block4
Block4:  ;While condition at line 25
	%reg_32 = load i32, ptr %reg_30
	%reg_33 = icmp ne i32 %reg_32, 0
	br i1 %reg_33, label %Block5, label %Block6
Block5:  ;While body at line 25
	%reg_34 = load float, ptr %reg_13
	%reg_35 = load float, ptr %reg_1
	%reg_36 = load float, ptr %reg_13
	%reg_37 = fdiv float %reg_35, %reg_36
	%reg_38 = fadd float %reg_34, %reg_37
	%reg_39 = add i32 2, 0
	%reg_40 = sitofp i32 %reg_39 to float
	%reg_41 = fdiv float %reg_38, %reg_40
	store float %reg_41, ptr %reg_13
	%reg_42 = load i32, ptr %reg_30
	%reg_43 = add i32 1, 0
	%reg_44 = sub i32 %reg_42, %reg_43
	store i32 %reg_44, ptr %reg_30
	br label %Block4
Block6:  ;While end at line 25
	%reg_45 = load float, ptr %reg_13
	ret float %reg_45
}

define float @F1(float %reg_0)
{
Block0:  ;Func define at line 32
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 32
	%reg_2 = add i32 1, 0
	%reg_3 = sitofp i32 %reg_2 to float
	%reg_4 = load float, ptr %reg_1
	%reg_5 = fdiv float %reg_3, %reg_4
	ret float %reg_5
}

define float @F2(float %reg_0)
{
Block0:  ;Func define at line 34
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 34
	%reg_2 = add i32 1, 0
	%reg_3 = sitofp i32 %reg_2 to float
	%reg_4 = add i32 1, 0
	%reg_5 = sitofp i32 %reg_4 to float
	%reg_6 = load float, ptr %reg_1
	%reg_7 = load float, ptr %reg_1
	%reg_8 = fmul float %reg_6, %reg_7
	%reg_9 = fsub float %reg_5, %reg_8
	%reg_10 = call float @my_sqrt(float %reg_9)
	%reg_11 = fdiv float %reg_3, %reg_10
	ret float %reg_11
}

define float @simpson(float %reg_0, float %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 36
	%reg_6 = alloca float
	%reg_5 = alloca i32
	%reg_4 = alloca float
	%reg_3 = alloca float
	store float %reg_0, ptr %reg_3
	store float %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 36
	%reg_7 = load float, ptr %reg_3
	%reg_8 = load float, ptr %reg_4
	%reg_9 = load float, ptr %reg_3
	%reg_10 = fsub float %reg_8, %reg_9
	%reg_11 = add i32 2, 0
	%reg_12 = sitofp i32 %reg_11 to float
	%reg_13 = fdiv float %reg_10, %reg_12
	%reg_14 = fadd float %reg_7, %reg_13
	store float %reg_14, ptr %reg_6
	%reg_15 = load i32, ptr %reg_5
	%reg_16 = add i32 1, 0
	%reg_17 = icmp eq i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block2, label %Block3
Block2:  ;If then at line 38
	%reg_18 = load float, ptr %reg_3
	%reg_19 = call float @F1(float %reg_18)
	%reg_20 = add i32 4, 0
	%reg_21 = sitofp i32 %reg_20 to float
	%reg_22 = load float, ptr %reg_6
	%reg_23 = call float @F1(float %reg_22)
	%reg_24 = fmul float %reg_21, %reg_23
	%reg_25 = fadd float %reg_19, %reg_24
	%reg_26 = load float, ptr %reg_4
	%reg_27 = call float @F1(float %reg_26)
	%reg_28 = fadd float %reg_25, %reg_27
	%reg_29 = load float, ptr %reg_4
	%reg_30 = load float, ptr %reg_3
	%reg_31 = fsub float %reg_29, %reg_30
	%reg_32 = fmul float %reg_28, %reg_31
	%reg_33 = add i32 6, 0
	%reg_34 = sitofp i32 %reg_33 to float
	%reg_35 = fdiv float %reg_32, %reg_34
	ret float %reg_35
	br label %Block3
Block3:  ;If end at line 38
	%reg_36 = load i32, ptr %reg_5
	%reg_37 = add i32 2, 0
	%reg_38 = icmp eq i32 %reg_36, %reg_37
	br i1 %reg_38, label %Block4, label %Block5
Block4:  ;If then at line 39
	%reg_39 = load float, ptr %reg_3
	%reg_40 = call float @F2(float %reg_39)
	%reg_41 = add i32 4, 0
	%reg_42 = sitofp i32 %reg_41 to float
	%reg_43 = load float, ptr %reg_6
	%reg_44 = call float @F2(float %reg_43)
	%reg_45 = fmul float %reg_42, %reg_44
	%reg_46 = fadd float %reg_40, %reg_45
	%reg_47 = load float, ptr %reg_4
	%reg_48 = call float @F2(float %reg_47)
	%reg_49 = fadd float %reg_46, %reg_48
	%reg_50 = load float, ptr %reg_4
	%reg_51 = load float, ptr %reg_3
	%reg_52 = fsub float %reg_50, %reg_51
	%reg_53 = fmul float %reg_49, %reg_52
	%reg_54 = add i32 6, 0
	%reg_55 = sitofp i32 %reg_54 to float
	%reg_56 = fdiv float %reg_53, %reg_55
	ret float %reg_56
	br label %Block5
Block5:  ;If end at line 39
	%reg_57 = add i32 0, 0
	%reg_58 = sitofp i32 %reg_57 to float
	ret float %reg_58
}

define float @asr5(float %reg_0, float %reg_1, float %reg_2, float %reg_3, i32 %reg_4)
{
Block0:  ;Func define at line 43
	%reg_24 = alloca float
	%reg_19 = alloca float
	%reg_10 = alloca float
	%reg_9 = alloca i32
	%reg_8 = alloca float
	%reg_7 = alloca float
	%reg_6 = alloca float
	%reg_5 = alloca float
	store float %reg_0, ptr %reg_5
	store float %reg_1, ptr %reg_6
	store float %reg_2, ptr %reg_7
	store float %reg_3, ptr %reg_8
	store i32 %reg_4, ptr %reg_9
	br label %Block1
Block1:  ;Func body at line 43
	%reg_11 = load float, ptr %reg_5
	%reg_12 = load float, ptr %reg_6
	%reg_13 = load float, ptr %reg_5
	%reg_14 = fsub float %reg_12, %reg_13
	%reg_15 = add i32 2, 0
	%reg_16 = sitofp i32 %reg_15 to float
	%reg_17 = fdiv float %reg_14, %reg_16
	%reg_18 = fadd float %reg_11, %reg_17
	store float %reg_18, ptr %reg_10
	%reg_20 = load float, ptr %reg_5
	%reg_21 = load float, ptr %reg_10
	%reg_22 = load i32, ptr %reg_9
	%reg_23 = call float @simpson(float %reg_20, float %reg_21, i32 %reg_22)
	store float %reg_23, ptr %reg_19
	%reg_25 = load float, ptr %reg_10
	%reg_26 = load float, ptr %reg_6
	%reg_27 = load i32, ptr %reg_9
	%reg_28 = call float @simpson(float %reg_25, float %reg_26, i32 %reg_27)
	store float %reg_28, ptr %reg_24
	%reg_29 = load float, ptr %reg_19
	%reg_30 = load float, ptr %reg_24
	%reg_31 = fadd float %reg_29, %reg_30
	%reg_32 = load float, ptr %reg_8
	%reg_33 = fsub float %reg_31, %reg_32
	%reg_34 = call float @my_fabs(float %reg_33)
	%reg_35 = add i32 15, 0
	%reg_36 = sitofp i32 %reg_35 to float
	%reg_37 = load float, ptr %reg_7
	%reg_38 = fmul float %reg_36, %reg_37
	%reg_39 = fcmp ole float %reg_34, %reg_38
	br i1 %reg_39, label %Block2, label %Block3
Block2:  ;If then at line 46
	%reg_40 = load float, ptr %reg_19
	%reg_41 = load float, ptr %reg_24
	%reg_42 = fadd float %reg_40, %reg_41
	%reg_43 = load float, ptr %reg_19
	%reg_44 = load float, ptr %reg_24
	%reg_45 = fadd float %reg_43, %reg_44
	%reg_46 = load float, ptr %reg_8
	%reg_47 = fsub float %reg_45, %reg_46
	%reg_48 = fadd float 0x402e000000000000, 0x0
	%reg_49 = fdiv float %reg_47, %reg_48
	%reg_50 = fadd float %reg_42, %reg_49
	ret float %reg_50
	br label %Block3
Block3:  ;If end at line 46
	%reg_51 = load float, ptr %reg_5
	%reg_52 = load float, ptr %reg_10
	%reg_53 = load float, ptr %reg_7
	%reg_54 = add i32 2, 0
	%reg_55 = sitofp i32 %reg_54 to float
	%reg_56 = fdiv float %reg_53, %reg_55
	%reg_57 = load float, ptr %reg_19
	%reg_58 = load i32, ptr %reg_9
	%reg_59 = call float @asr5(float %reg_51, float %reg_52, float %reg_56, float %reg_57, i32 %reg_58)
	%reg_60 = load float, ptr %reg_10
	%reg_61 = load float, ptr %reg_6
	%reg_62 = load float, ptr %reg_7
	%reg_63 = add i32 2, 0
	%reg_64 = sitofp i32 %reg_63 to float
	%reg_65 = fdiv float %reg_62, %reg_64
	%reg_66 = load float, ptr %reg_24
	%reg_67 = load i32, ptr %reg_9
	%reg_68 = call float @asr5(float %reg_60, float %reg_61, float %reg_65, float %reg_66, i32 %reg_67)
	%reg_69 = fadd float %reg_59, %reg_68
	ret float %reg_69
}

define float @asr4(float %reg_0, float %reg_1, float %reg_2, i32 %reg_3)
{
Block0:  ;Func define at line 50
	%reg_7 = alloca i32
	%reg_6 = alloca float
	%reg_5 = alloca float
	%reg_4 = alloca float
	store float %reg_0, ptr %reg_4
	store float %reg_1, ptr %reg_5
	store float %reg_2, ptr %reg_6
	store i32 %reg_3, ptr %reg_7
	br label %Block1
Block1:  ;Func body at line 50
	%reg_8 = load float, ptr %reg_4
	%reg_9 = load float, ptr %reg_5
	%reg_10 = load float, ptr %reg_6
	%reg_11 = load float, ptr %reg_4
	%reg_12 = load float, ptr %reg_5
	%reg_13 = load i32, ptr %reg_7
	%reg_14 = call float @simpson(float %reg_11, float %reg_12, i32 %reg_13)
	%reg_15 = load i32, ptr %reg_7
	%reg_16 = call float @asr5(float %reg_8, float %reg_9, float %reg_10, float %reg_14, i32 %reg_15)
	ret float %reg_16
}

define float @eee(float %reg_0)
{
Block0:  ;Func define at line 54
	%reg_5 = alloca float
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 54
	%reg_2 = load float, ptr %reg_1
	%reg_3 = fadd float 0x3f50624de0000000, 0x0
	%reg_4 = fcmp ogt float %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block3
Block2:  ;If then at line 55
	%reg_6 = load float, ptr %reg_1
	%reg_7 = add i32 2, 0
	%reg_8 = sitofp i32 %reg_7 to float
	%reg_9 = fdiv float %reg_6, %reg_8
	%reg_10 = call float @eee(float %reg_9)
	store float %reg_10, ptr %reg_5
	%reg_11 = load float, ptr %reg_5
	%reg_12 = load float, ptr %reg_5
	%reg_13 = fmul float %reg_11, %reg_12
	ret float %reg_13
	br label %Block3
Block3:  ;If end at line 55
	%reg_14 = add i32 1, 0
	%reg_15 = sitofp i32 %reg_14 to float
	%reg_16 = load float, ptr %reg_1
	%reg_17 = fadd float %reg_15, %reg_16
	%reg_18 = load float, ptr %reg_1
	%reg_19 = load float, ptr %reg_1
	%reg_20 = fmul float %reg_18, %reg_19
	%reg_21 = add i32 2, 0
	%reg_22 = sitofp i32 %reg_21 to float
	%reg_23 = fdiv float %reg_20, %reg_22
	%reg_24 = fadd float %reg_17, %reg_23
	%reg_25 = load float, ptr %reg_1
	%reg_26 = add i32 3, 0
	%reg_27 = call float @my_pow(float %reg_25, i32 %reg_26)
	%reg_28 = add i32 6, 0
	%reg_29 = sitofp i32 %reg_28 to float
	%reg_30 = fdiv float %reg_27, %reg_29
	%reg_31 = fadd float %reg_24, %reg_30
	%reg_32 = load float, ptr %reg_1
	%reg_33 = add i32 4, 0
	%reg_34 = call float @my_pow(float %reg_32, i32 %reg_33)
	%reg_35 = add i32 24, 0
	%reg_36 = sitofp i32 %reg_35 to float
	%reg_37 = fdiv float %reg_34, %reg_36
	%reg_38 = fadd float %reg_31, %reg_37
	%reg_39 = load float, ptr %reg_1
	%reg_40 = add i32 5, 0
	%reg_41 = call float @my_pow(float %reg_39, i32 %reg_40)
	%reg_42 = add i32 120, 0
	%reg_43 = sitofp i32 %reg_42 to float
	%reg_44 = fdiv float %reg_41, %reg_43
	%reg_45 = fadd float %reg_38, %reg_44
	ret float %reg_45
}

define float @my_exp(float %reg_0)
{
Block0:  ;Func define at line 63
	%reg_23 = alloca float
	%reg_19 = alloca float
	%reg_12 = alloca i32
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 63
	%reg_2 = load float, ptr %reg_1
	%reg_3 = add i32 0, 0
	%reg_4 = sitofp i32 %reg_3 to float
	%reg_5 = fcmp olt float %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 64
	%reg_6 = add i32 1, 0
	%reg_7 = sitofp i32 %reg_6 to float
	%reg_8 = load float, ptr %reg_1
	%reg_9 = fsub float 0x0, %reg_8
	%reg_10 = call float @my_exp(float %reg_9)
	%reg_11 = fdiv float %reg_7, %reg_10
	ret float %reg_11
	br label %Block3
Block3:  ;If end at line 64
	%reg_13 = load float, ptr %reg_1
	%reg_14 = fptosi float %reg_13 to i32
	store i32 %reg_14, ptr %reg_12
	%reg_15 = load float, ptr %reg_1
	%reg_16 = load i32, ptr %reg_12
	%reg_17 = sitofp i32 %reg_16 to float
	%reg_18 = fsub float %reg_15, %reg_17
	store float %reg_18, ptr %reg_1
	%reg_20 = load float, ptr @e
	%reg_21 = load i32, ptr %reg_12
	%reg_22 = call float @my_pow(float %reg_20, i32 %reg_21)
	store float %reg_22, ptr %reg_19
	%reg_24 = load float, ptr %reg_1
	%reg_25 = call float @eee(float %reg_24)
	store float %reg_25, ptr %reg_23
	%reg_26 = load float, ptr %reg_19
	%reg_27 = load float, ptr %reg_23
	%reg_28 = fmul float %reg_26, %reg_27
	ret float %reg_28
}

define float @my_ln(float %reg_0)
{
Block0:  ;Func define at line 72
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 72
	%reg_2 = add i32 1, 0
	%reg_3 = sitofp i32 %reg_2 to float
	%reg_4 = load float, ptr %reg_1
	%reg_5 = fadd float 0x3e45798ee0000000, 0x0
	%reg_6 = add i32 1, 0
	%reg_7 = call float @asr4(float %reg_3, float %reg_4, float %reg_5, i32 %reg_6)
	ret float %reg_7
}

define float @my_log(float %reg_0, float %reg_1)
{
Block0:  ;Func define at line 74
	%reg_3 = alloca float
	%reg_2 = alloca float
	store float %reg_0, ptr %reg_2
	store float %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 74
	%reg_4 = load float, ptr %reg_3
	%reg_5 = call float @my_ln(float %reg_4)
	%reg_6 = load float, ptr %reg_2
	%reg_7 = call float @my_ln(float %reg_6)
	%reg_8 = fdiv float %reg_5, %reg_7
	ret float %reg_8
}

define float @my_powf(float %reg_0, float %reg_1)
{
Block0:  ;Func define at line 76
	%reg_3 = alloca float
	%reg_2 = alloca float
	store float %reg_0, ptr %reg_2
	store float %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 76
	%reg_4 = load float, ptr %reg_3
	%reg_5 = load float, ptr %reg_2
	%reg_6 = call float @my_ln(float %reg_5)
	%reg_7 = fmul float %reg_4, %reg_6
	%reg_8 = call float @my_exp(float %reg_7)
	ret float %reg_8
}

define i32 @main()
{
Block0:  ;Func define at line 78
	%reg_6 = alloca float
	%reg_4 = alloca float
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 78
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 80
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = icmp ne i32 %reg_2, 0
	br i1 %reg_3, label %Block3, label %Block4
Block3:  ;While body at line 80
	%reg_5 = call float @getfloat()
	store float %reg_5, ptr %reg_4
	%reg_7 = call float @getfloat()
	store float %reg_7, ptr %reg_6
	%reg_8 = load float, ptr %reg_4
	%reg_9 = call float @my_fabs(float %reg_8)
	call void @putfloat(float %reg_9)
	%reg_10 = add i32 32, 0
	call void @putch(i32 %reg_10)
	%reg_11 = load float, ptr %reg_4
	%reg_12 = add i32 2, 0
	%reg_13 = call float @my_pow(float %reg_11, i32 %reg_12)
	call void @putfloat(float %reg_13)
	%reg_14 = add i32 32, 0
	call void @putch(i32 %reg_14)
	%reg_15 = load float, ptr %reg_4
	%reg_16 = call float @my_sqrt(float %reg_15)
	call void @putfloat(float %reg_16)
	%reg_17 = add i32 32, 0
	call void @putch(i32 %reg_17)
	%reg_18 = load float, ptr %reg_4
	%reg_19 = call float @my_exp(float %reg_18)
	call void @putfloat(float %reg_19)
	%reg_20 = add i32 32, 0
	call void @putch(i32 %reg_20)
	%reg_21 = load float, ptr %reg_4
	%reg_22 = add i32 0, 0
	%reg_23 = sitofp i32 %reg_22 to float
	%reg_24 = fcmp ogt float %reg_21, %reg_23
	br i1 %reg_24, label %Block5, label %Block7
Block4:  ;While end at line 80
	%reg_54 = add i32 0, 0
	ret i32 %reg_54
Block5:  ;If then at line 90
	%reg_25 = load float, ptr %reg_4
	%reg_26 = call float @my_ln(float %reg_25)
	call void @putfloat(float %reg_26)
	br label %Block6
Block6:  ;If end at line 90
	%reg_28 = add i32 32, 0
	call void @putch(i32 %reg_28)
	%reg_29 = load float, ptr %reg_4
	%reg_30 = add i32 0, 0
	%reg_31 = sitofp i32 %reg_30 to float
	%reg_32 = fcmp ogt float %reg_29, %reg_31
	br i1 %reg_32, label %Block11, label %Block10
Block7:  ;If else at line 90
	%reg_27 = add i32 45, 0
	call void @putch(i32 %reg_27)
	br label %Block6
Block8:  ;If then at line 96
	%reg_37 = load float, ptr %reg_4
	%reg_38 = load float, ptr %reg_6
	%reg_39 = call float @my_log(float %reg_37, float %reg_38)
	call void @putfloat(float %reg_39)
	br label %Block9
Block9:  ;If end at line 96
	%reg_41 = add i32 32, 0
	call void @putch(i32 %reg_41)
	%reg_42 = load float, ptr %reg_4
	%reg_43 = add i32 0, 0
	%reg_44 = sitofp i32 %reg_43 to float
	%reg_45 = fcmp ogt float %reg_42, %reg_44
	br i1 %reg_45, label %Block12, label %Block14
Block10:  ;If else at line 96
	%reg_40 = add i32 45, 0
	call void @putch(i32 %reg_40)
	br label %Block9
Block11:  ;And opertor at line 96
	%reg_33 = load float, ptr %reg_6
	%reg_34 = add i32 0, 0
	%reg_35 = sitofp i32 %reg_34 to float
	%reg_36 = fcmp ogt float %reg_33, %reg_35
	br i1 %reg_36, label %Block8, label %Block10
Block12:  ;If then at line 102
	%reg_46 = load float, ptr %reg_4
	%reg_47 = load float, ptr %reg_6
	%reg_48 = call float @my_powf(float %reg_46, float %reg_47)
	call void @putfloat(float %reg_48)
	br label %Block13
Block13:  ;If end at line 102
	%reg_50 = add i32 10, 0
	call void @putch(i32 %reg_50)
	%reg_51 = load i32, ptr %reg_0
	%reg_52 = add i32 1, 0
	%reg_53 = sub i32 %reg_51, %reg_52
	store i32 %reg_53, ptr %reg_0
	br label %Block2
Block14:  ;If else at line 102
	%reg_49 = add i32 45, 0
	call void @putch(i32 %reg_49)
	br label %Block13
}
