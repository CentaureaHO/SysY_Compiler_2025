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

@W = global i32 192
@H = global i32 192
@N = global i32 24
@PI = global float 0x400921fb60000000
@TWO_PI = global float 0x401921fb60000000
@MAX_STEP = global i32 10
@MAX_DISTANCE = global float 0x4000000000000000
@EPSILON = global float 0x3eb0c6f7a0000000
@RAND_MAX = global i32 100000006
@seed = global i32 0

define i32 @rand()
{
Block0:  ;Func define at line 13
	br label %Block1
Block1:  ;Func body at line 13
	%reg_0 = load i32, ptr @seed
	%reg_1 = add i32 19980130, 0
	%reg_2 = mul i32 %reg_0, %reg_1
	%reg_3 = add i32 23333, 0
	%reg_4 = add i32 %reg_2, %reg_3
	%reg_5 = add i32 100000007, 0
	%reg_6 = srem i32 %reg_4, %reg_5
	store i32 %reg_6, ptr @seed
	%reg_7 = load i32, ptr @seed
	%reg_8 = add i32 0, 0
	%reg_9 = icmp slt i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block2, label %Block3
Block2:  ;If then at line 15
	%reg_10 = load i32, ptr @seed
	%reg_11 = add i32 100000007, 0
	%reg_12 = add i32 %reg_10, %reg_11
	store i32 %reg_12, ptr @seed
	br label %Block3
Block3:  ;If end at line 15
	%reg_13 = load i32, ptr @seed
	ret i32 %reg_13
}

define float @my_fabs(float %reg_0)
{
Block0:  ;Func define at line 19
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 19
	%reg_2 = load float, ptr %reg_1
	%reg_3 = add i32 0, 0
	%reg_4 = sitofp i32 %reg_3 to float
	%reg_5 = fcmp ogt float %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 20
	%reg_6 = load float, ptr %reg_1
	ret float %reg_6
	br label %Block3
Block3:  ;If end at line 20
	%reg_7 = load float, ptr %reg_1
	%reg_8 = fsub float 0x0, %reg_7
	ret float %reg_8
}

define float @my_sqrt(float %reg_0)
{
Block0:  ;Func define at line 24
	%reg_19 = alloca i32
	%reg_2 = alloca float
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 24
	%reg_3 = load float, ptr %reg_1
	%reg_4 = add i32 8, 0
	%reg_5 = sitofp i32 %reg_4 to float
	%reg_6 = fdiv float %reg_3, %reg_5
	%reg_7 = fadd float 0x3fe0000000000000, 0x0
	%reg_8 = fadd float %reg_6, %reg_7
	%reg_9 = add i32 2, 0
	%reg_10 = sitofp i32 %reg_9 to float
	%reg_11 = load float, ptr %reg_1
	%reg_12 = fmul float %reg_10, %reg_11
	%reg_13 = add i32 4, 0
	%reg_14 = sitofp i32 %reg_13 to float
	%reg_15 = load float, ptr %reg_1
	%reg_16 = fadd float %reg_14, %reg_15
	%reg_17 = fdiv float %reg_12, %reg_16
	%reg_18 = fadd float %reg_8, %reg_17
	store float %reg_18, ptr %reg_2
	%reg_20 = add i32 10, 0
	store i32 %reg_20, ptr %reg_19
	br label %Block2
Block2:  ;While condition at line 27
	%reg_21 = load i32, ptr %reg_19
	%reg_22 = icmp ne i32 %reg_21, 0
	br i1 %reg_22, label %Block3, label %Block4
Block3:  ;While body at line 27
	%reg_23 = load float, ptr %reg_2
	%reg_24 = load float, ptr %reg_1
	%reg_25 = load float, ptr %reg_2
	%reg_26 = fdiv float %reg_24, %reg_25
	%reg_27 = fadd float %reg_23, %reg_26
	%reg_28 = add i32 2, 0
	%reg_29 = sitofp i32 %reg_28 to float
	%reg_30 = fdiv float %reg_27, %reg_29
	store float %reg_30, ptr %reg_2
	%reg_31 = load i32, ptr %reg_19
	%reg_32 = add i32 1, 0
	%reg_33 = sub i32 %reg_31, %reg_32
	store i32 %reg_33, ptr %reg_19
	br label %Block2
Block4:  ;While end at line 27
	%reg_34 = load float, ptr %reg_2
	ret float %reg_34
}

define float @p(float %reg_0)
{
Block0:  ;Func define at line 34
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 34
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
Block0:  ;Func define at line 36
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 36
	%reg_2 = load float, ptr %reg_1
	%reg_3 = call float @my_fabs(float %reg_2)
	%reg_4 = load float, ptr @EPSILON
	%reg_5 = fcmp ole float %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 37
	%reg_6 = load float, ptr %reg_1
	ret float %reg_6
	br label %Block3
Block3:  ;If end at line 37
	%reg_7 = load float, ptr %reg_1
	%reg_8 = fadd float 0x4008000000000000, 0x0
	%reg_9 = fdiv float %reg_7, %reg_8
	%reg_10 = call float @my_sin_impl(float %reg_9)
	%reg_11 = call float @p(float %reg_10)
	ret float %reg_11
}

define float @my_sin(float %reg_0)
{
Block0:  ;Func define at line 41
	%reg_9 = alloca i32
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 41
	%reg_2 = load float, ptr %reg_1
	%reg_3 = load float, ptr @TWO_PI
	%reg_4 = fcmp ogt float %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block4
Block2:  ;If then at line 42
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
Block3:  ;If end at line 42
	%reg_20 = load float, ptr %reg_1
	%reg_21 = load float, ptr @PI
	%reg_22 = fcmp ogt float %reg_20, %reg_21
	br i1 %reg_22, label %Block5, label %Block6
Block4:  ;Or opertor at line 42
	%reg_5 = load float, ptr %reg_1
	%reg_6 = load float, ptr @TWO_PI
	%reg_7 = fsub float 0x0, %reg_6
	%reg_8 = fcmp olt float %reg_5, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block5:  ;If then at line 46
	%reg_23 = load float, ptr %reg_1
	%reg_24 = load float, ptr @TWO_PI
	%reg_25 = fsub float %reg_23, %reg_24
	store float %reg_25, ptr %reg_1
	br label %Block6
Block6:  ;If end at line 46
	%reg_26 = load float, ptr %reg_1
	%reg_27 = load float, ptr @PI
	%reg_28 = fsub float 0x0, %reg_27
	%reg_29 = fcmp olt float %reg_26, %reg_28
	br i1 %reg_29, label %Block7, label %Block8
Block7:  ;If then at line 47
	%reg_30 = load float, ptr %reg_1
	%reg_31 = load float, ptr @TWO_PI
	%reg_32 = fadd float %reg_30, %reg_31
	store float %reg_32, ptr %reg_1
	br label %Block8
Block8:  ;If end at line 47
	%reg_33 = load float, ptr %reg_1
	%reg_34 = call float @my_sin_impl(float %reg_33)
	ret float %reg_34
}

define float @my_cos(float %reg_0)
{
Block0:  ;Func define at line 51
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 51
	%reg_2 = load float, ptr %reg_1
	%reg_3 = load float, ptr @PI
	%reg_4 = add i32 2, 0
	%reg_5 = sitofp i32 %reg_4 to float
	%reg_6 = fdiv float %reg_3, %reg_5
	%reg_7 = fadd float %reg_2, %reg_6
	%reg_8 = call float @my_sin(float %reg_7)
	ret float %reg_8
}

define float @circle_sdf(float %reg_0, float %reg_1, float %reg_2, float %reg_3, float %reg_4)
{
Block0:  ;Func define at line 53
	%reg_14 = alloca float
	%reg_10 = alloca float
	%reg_9 = alloca float
	%reg_8 = alloca float
	%reg_7 = alloca float
	%reg_6 = alloca float
	%reg_5 = alloca float
	store float %reg_0, ptr %reg_5
	store float %reg_1, ptr %reg_6
	store float %reg_2, ptr %reg_7
	store float %reg_3, ptr %reg_8
	store float %reg_4, ptr %reg_9
	br label %Block1
Block1:  ;Func body at line 53
	%reg_11 = load float, ptr %reg_5
	%reg_12 = load float, ptr %reg_7
	%reg_13 = fsub float %reg_11, %reg_12
	store float %reg_13, ptr %reg_10
	%reg_15 = load float, ptr %reg_6
	%reg_16 = load float, ptr %reg_8
	%reg_17 = fsub float %reg_15, %reg_16
	store float %reg_17, ptr %reg_14
	%reg_18 = load float, ptr %reg_10
	%reg_19 = load float, ptr %reg_10
	%reg_20 = fmul float %reg_18, %reg_19
	%reg_21 = load float, ptr %reg_14
	%reg_22 = load float, ptr %reg_14
	%reg_23 = fmul float %reg_21, %reg_22
	%reg_24 = fadd float %reg_20, %reg_23
	%reg_25 = call float @my_sqrt(float %reg_24)
	%reg_26 = load float, ptr %reg_9
	%reg_27 = fsub float %reg_25, %reg_26
	ret float %reg_27
}

define void @scene(float %reg_0, float %reg_1, ptr %reg_2)
{
Block0:  ;Func define at line 58
	%reg_12 = alloca float
	%reg_5 = alloca float
	%reg_4 = alloca float
	%reg_3 = alloca float
	store float %reg_0, ptr %reg_3
	store float %reg_1, ptr %reg_4
	br label %Block1
Block1:  ;Func body at line 58
	%reg_6 = load float, ptr %reg_3
	%reg_7 = load float, ptr %reg_4
	%reg_8 = fadd float 0x3fd99999a0000000, 0x0
	%reg_9 = fadd float 0x3fd99999a0000000, 0x0
	%reg_10 = fadd float 0x3fb99999a0000000, 0x0
	%reg_11 = call float @circle_sdf(float %reg_6, float %reg_7, float %reg_8, float %reg_9, float %reg_10)
	store float %reg_11, ptr %reg_5
	%reg_13 = load float, ptr %reg_3
	%reg_14 = load float, ptr %reg_4
	%reg_15 = fadd float 0x3fe3333340000000, 0x0
	%reg_16 = fadd float 0x3fe3333340000000, 0x0
	%reg_17 = fadd float 0x3fa99999a0000000, 0x0
	%reg_18 = call float @circle_sdf(float %reg_13, float %reg_14, float %reg_15, float %reg_16, float %reg_17)
	store float %reg_18, ptr %reg_12
	%reg_19 = load float, ptr %reg_5
	%reg_20 = load float, ptr %reg_12
	%reg_21 = fcmp olt float %reg_19, %reg_20
	br i1 %reg_21, label %Block2, label %Block4
Block2:  ;If then at line 61
	%reg_22 = add i32 0, 0
	%reg_23 = getelementptr float, ptr %reg_2, i32 %reg_22
	%reg_24 = load float, ptr %reg_5
	store float %reg_24, ptr %reg_23
	%reg_25 = add i32 1, 0
	%reg_26 = getelementptr float, ptr %reg_2, i32 %reg_25
	%reg_27 = fadd float 0x4008000000000000, 0x0
	store float %reg_27, ptr %reg_26
	br label %Block3
Block3:  ;If end at line 61
	ret void
Block4:  ;If else at line 61
	%reg_28 = add i32 0, 0
	%reg_29 = getelementptr float, ptr %reg_2, i32 %reg_28
	%reg_30 = load float, ptr %reg_12
	store float %reg_30, ptr %reg_29
	%reg_31 = add i32 1, 0
	%reg_32 = getelementptr float, ptr %reg_2, i32 %reg_31
	%reg_33 = fadd float 0x0, 0x0
	store float %reg_33, ptr %reg_32
	br label %Block3
}

define float @trace(float %reg_0, float %reg_1, float %reg_2, float %reg_3)
{
Block0:  ;Func define at line 70
	%reg_18 = alloca [2 x float]
	%reg_10 = alloca i32
	%reg_8 = alloca float
	%reg_7 = alloca float
	%reg_6 = alloca float
	%reg_5 = alloca float
	%reg_4 = alloca float
	store float %reg_0, ptr %reg_4
	store float %reg_1, ptr %reg_5
	store float %reg_2, ptr %reg_6
	store float %reg_3, ptr %reg_7
	br label %Block1
Block1:  ;Func body at line 70
	%reg_9 = fadd float 0x0, 0x0
	store float %reg_9, ptr %reg_8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	br label %Block2
Block2:  ;While condition at line 73
	%reg_12 = load i32, ptr %reg_10
	%reg_13 = load i32, ptr @MAX_STEP
	%reg_14 = icmp slt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block5, label %Block4
Block3:  ;While body at line 73
	call void @llvm.memset.p0.i32(ptr %reg_18, i8 0, i32 8, i1 1)
	%reg_19 = load float, ptr %reg_4
	%reg_20 = load float, ptr %reg_6
	%reg_21 = load float, ptr %reg_8
	%reg_22 = fmul float %reg_20, %reg_21
	%reg_23 = fadd float %reg_19, %reg_22
	%reg_24 = load float, ptr %reg_5
	%reg_25 = load float, ptr %reg_7
	%reg_26 = load float, ptr %reg_8
	%reg_27 = fmul float %reg_25, %reg_26
	%reg_28 = fadd float %reg_24, %reg_27
	%reg_29 = getelementptr [2 x float], ptr %reg_18, i32 0
	call void @scene(float %reg_23, float %reg_28, ptr %reg_29)
	%reg_30 = add i32 0, 0
	%reg_31 = getelementptr [2 x float], ptr %reg_18, i32 0, i32 %reg_30
	%reg_32 = load float, ptr %reg_31
	%reg_33 = load float, ptr @EPSILON
	%reg_34 = fcmp olt float %reg_32, %reg_33
	br i1 %reg_34, label %Block6, label %Block7
Block4:  ;While end at line 73
	%reg_46 = fadd float 0x0, 0x0
	ret float %reg_46
Block5:  ;And opertor at line 73
	%reg_15 = load float, ptr %reg_8
	%reg_16 = load float, ptr @MAX_DISTANCE
	%reg_17 = fcmp olt float %reg_15, %reg_16
	br i1 %reg_17, label %Block3, label %Block4
Block6:  ;If then at line 76
	%reg_35 = add i32 1, 0
	%reg_36 = getelementptr [2 x float], ptr %reg_18, i32 0, i32 %reg_35
	%reg_37 = load float, ptr %reg_36
	ret float %reg_37
	br label %Block7
Block7:  ;If end at line 76
	%reg_38 = load float, ptr %reg_8
	%reg_39 = add i32 0, 0
	%reg_40 = getelementptr [2 x float], ptr %reg_18, i32 0, i32 %reg_39
	%reg_41 = load float, ptr %reg_40
	%reg_42 = fadd float %reg_38, %reg_41
	store float %reg_42, ptr %reg_8
	%reg_43 = load i32, ptr %reg_10
	%reg_44 = add i32 1, 0
	%reg_45 = add i32 %reg_43, %reg_44
	store i32 %reg_45, ptr %reg_10
	br label %Block2
}

define float @sample(float %reg_0, float %reg_1)
{
Block0:  ;Func define at line 83
	%reg_14 = alloca float
	%reg_11 = alloca float
	%reg_6 = alloca i32
	%reg_4 = alloca float
	%reg_3 = alloca float
	%reg_2 = alloca float
	store float %reg_0, ptr %reg_2
	store float %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 83
	%reg_5 = fadd float 0x0, 0x0
	store float %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	br label %Block2
Block2:  ;While condition at line 86
	%reg_8 = load i32, ptr %reg_6
	%reg_9 = load i32, ptr @N
	%reg_10 = icmp slt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 86
	%reg_12 = call i32 @rand()
	%reg_13 = sitofp i32 %reg_12 to float
	store float %reg_13, ptr %reg_11
	%reg_15 = load float, ptr @TWO_PI
	%reg_16 = load i32, ptr %reg_6
	%reg_17 = sitofp i32 %reg_16 to float
	%reg_18 = load float, ptr %reg_11
	%reg_19 = load i32, ptr @RAND_MAX
	%reg_20 = sitofp i32 %reg_19 to float
	%reg_21 = fdiv float %reg_18, %reg_20
	%reg_22 = fadd float %reg_17, %reg_21
	%reg_23 = fmul float %reg_15, %reg_22
	%reg_24 = load i32, ptr @N
	%reg_25 = sitofp i32 %reg_24 to float
	%reg_26 = fdiv float %reg_23, %reg_25
	store float %reg_26, ptr %reg_14
	%reg_27 = load float, ptr %reg_4
	%reg_28 = load float, ptr %reg_2
	%reg_29 = load float, ptr %reg_3
	%reg_30 = load float, ptr %reg_14
	%reg_31 = call float @my_cos(float %reg_30)
	%reg_32 = load float, ptr %reg_14
	%reg_33 = call float @my_sin(float %reg_32)
	%reg_34 = call float @trace(float %reg_28, float %reg_29, float %reg_31, float %reg_33)
	%reg_35 = fadd float %reg_27, %reg_34
	store float %reg_35, ptr %reg_4
	%reg_36 = load i32, ptr %reg_6
	%reg_37 = add i32 1, 0
	%reg_38 = add i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_6
	br label %Block2
Block4:  ;While end at line 86
	%reg_39 = load float, ptr %reg_4
	%reg_40 = load i32, ptr @N
	%reg_41 = sitofp i32 %reg_40 to float
	%reg_42 = fdiv float %reg_39, %reg_41
	ret float %reg_42
}

define void @write_pgm()
{
Block0:  ;Func define at line 95
	%reg_25 = alloca i32
	%reg_22 = alloca float
	%reg_19 = alloca float
	%reg_14 = alloca i32
	%reg_9 = alloca i32
	br label %Block1
Block1:  ;Func body at line 95
	%reg_0 = add i32 80, 0
	call void @putch(i32 %reg_0)
	%reg_1 = add i32 50, 0
	call void @putch(i32 %reg_1)
	%reg_2 = add i32 10, 0
	call void @putch(i32 %reg_2)
	%reg_3 = load i32, ptr @W
	call void @putint(i32 %reg_3)
	%reg_4 = add i32 32, 0
	call void @putch(i32 %reg_4)
	%reg_5 = load i32, ptr @H
	call void @putint(i32 %reg_5)
	%reg_6 = add i32 32, 0
	call void @putch(i32 %reg_6)
	%reg_7 = add i32 255, 0
	call void @putint(i32 %reg_7)
	%reg_8 = add i32 10, 0
	call void @putch(i32 %reg_8)
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block2
Block2:  ;While condition at line 99
	%reg_11 = load i32, ptr %reg_9
	%reg_12 = load i32, ptr @H
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block3, label %Block4
Block3:  ;While body at line 99
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_14
	br label %Block5
Block4:  ;While end at line 99
	ret void
Block5:  ;While condition at line 101
	%reg_16 = load i32, ptr %reg_14
	%reg_17 = load i32, ptr @W
	%reg_18 = icmp slt i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block6, label %Block7
Block6:  ;While body at line 101
	%reg_20 = load i32, ptr %reg_14
	%reg_21 = sitofp i32 %reg_20 to float
	store float %reg_21, ptr %reg_19
	%reg_23 = load i32, ptr %reg_9
	%reg_24 = sitofp i32 %reg_23 to float
	store float %reg_24, ptr %reg_22
	%reg_26 = load float, ptr %reg_19
	%reg_27 = load i32, ptr @W
	%reg_28 = sitofp i32 %reg_27 to float
	%reg_29 = fdiv float %reg_26, %reg_28
	%reg_30 = load float, ptr %reg_22
	%reg_31 = load i32, ptr @H
	%reg_32 = sitofp i32 %reg_31 to float
	%reg_33 = fdiv float %reg_30, %reg_32
	%reg_34 = call float @sample(float %reg_29, float %reg_33)
	%reg_35 = fadd float 0x406fe00000000000, 0x0
	%reg_36 = fmul float %reg_34, %reg_35
	%reg_37 = fptosi float %reg_36 to i32
	store i32 %reg_37, ptr %reg_25
	%reg_38 = load i32, ptr %reg_25
	%reg_39 = add i32 255, 0
	%reg_40 = icmp sgt i32 %reg_38, %reg_39
	br i1 %reg_40, label %Block8, label %Block9
Block7:  ;While end at line 101
	%reg_47 = add i32 10, 0
	call void @putch(i32 %reg_47)
	%reg_48 = load i32, ptr %reg_9
	%reg_49 = add i32 1, 0
	%reg_50 = add i32 %reg_48, %reg_49
	store i32 %reg_50, ptr %reg_9
	br label %Block2
Block8:  ;If then at line 104
	%reg_41 = add i32 255, 0
	store i32 %reg_41, ptr %reg_25
	br label %Block9
Block9:  ;If end at line 104
	%reg_42 = load i32, ptr %reg_25
	call void @putint(i32 %reg_42)
	%reg_43 = add i32 32, 0
	call void @putch(i32 %reg_43)
	%reg_44 = load i32, ptr %reg_14
	%reg_45 = add i32 1, 0
	%reg_46 = add i32 %reg_44, %reg_45
	store i32 %reg_46, ptr %reg_14
	br label %Block5
}

define i32 @main()
{
Block0:  ;Func define at line 114
	br label %Block1
Block1:  ;Func body at line 114
	call void @write_pgm()
	%reg_0 = add i32 0, 0
	ret i32 %reg_0
}
