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

@MAX_WIDTH = global i32 1024
@MAX_HEIGHT = global i32 1024
@image = global [1048576 x i32] zeroinitializer
@width = global i32 zeroinitializer
@height = global i32 zeroinitializer
@PI = global float 0x400921fb60000000
@TWO_PI = global float 0x401921fb60000000
@EPSILON = global float 0x3eb0c6f7a0000000

define float @my_fabs(float %reg_0)
{
Block0:  ;Func define at line 6
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 6
	%reg_2 = load float, ptr %reg_1
	%reg_3 = add i32 0, 0
	%reg_4 = sitofp i32 %reg_3 to float
	%reg_5 = fcmp ogt float %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 7
	%reg_6 = load float, ptr %reg_1
	ret float %reg_6
	br label %Block3
Block3:  ;If end at line 7
	%reg_7 = load float, ptr %reg_1
	%reg_8 = fsub float 0x0, %reg_7
	ret float %reg_8
}

define float @p(float %reg_0)
{
Block0:  ;Func define at line 11
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 11
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
Block0:  ;Func define at line 13
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 13
	%reg_2 = load float, ptr %reg_1
	%reg_3 = call float @my_fabs(float %reg_2)
	%reg_4 = load float, ptr @EPSILON
	%reg_5 = fcmp ole float %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 14
	%reg_6 = load float, ptr %reg_1
	ret float %reg_6
	br label %Block3
Block3:  ;If end at line 14
	%reg_7 = load float, ptr %reg_1
	%reg_8 = fadd float 0x4008000000000000, 0x0
	%reg_9 = fdiv float %reg_7, %reg_8
	%reg_10 = call float @my_sin_impl(float %reg_9)
	%reg_11 = call float @p(float %reg_10)
	ret float %reg_11
}

define float @my_sin(float %reg_0)
{
Block0:  ;Func define at line 18
	%reg_9 = alloca i32
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 18
	%reg_2 = load float, ptr %reg_1
	%reg_3 = load float, ptr @TWO_PI
	%reg_4 = fcmp ogt float %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block4
Block2:  ;If then at line 19
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
Block3:  ;If end at line 19
	%reg_20 = load float, ptr %reg_1
	%reg_21 = load float, ptr @PI
	%reg_22 = fcmp ogt float %reg_20, %reg_21
	br i1 %reg_22, label %Block5, label %Block6
Block4:  ;Or opertor at line 19
	%reg_5 = load float, ptr %reg_1
	%reg_6 = load float, ptr @TWO_PI
	%reg_7 = fsub float 0x0, %reg_6
	%reg_8 = fcmp olt float %reg_5, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block5:  ;If then at line 23
	%reg_23 = load float, ptr %reg_1
	%reg_24 = load float, ptr @TWO_PI
	%reg_25 = fsub float %reg_23, %reg_24
	store float %reg_25, ptr %reg_1
	br label %Block6
Block6:  ;If end at line 23
	%reg_26 = load float, ptr %reg_1
	%reg_27 = load float, ptr @PI
	%reg_28 = fsub float 0x0, %reg_27
	%reg_29 = fcmp olt float %reg_26, %reg_28
	br i1 %reg_29, label %Block7, label %Block8
Block7:  ;If then at line 24
	%reg_30 = load float, ptr %reg_1
	%reg_31 = load float, ptr @TWO_PI
	%reg_32 = fadd float %reg_30, %reg_31
	store float %reg_32, ptr %reg_1
	br label %Block8
Block8:  ;If end at line 24
	%reg_33 = load float, ptr %reg_1
	%reg_34 = call float @my_sin_impl(float %reg_33)
	ret float %reg_34
}

define float @my_cos(float %reg_0)
{
Block0:  ;Func define at line 28
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 28
	%reg_2 = load float, ptr %reg_1
	%reg_3 = load float, ptr @PI
	%reg_4 = add i32 2, 0
	%reg_5 = sitofp i32 %reg_4 to float
	%reg_6 = fdiv float %reg_3, %reg_5
	%reg_7 = fadd float %reg_2, %reg_6
	%reg_8 = call float @my_sin(float %reg_7)
	ret float %reg_8
}

define i32 @read_image()
{
Block0:  ;Func define at line 30
	%reg_26 = alloca i32
	%reg_21 = alloca i32
	br label %Block1
Block1:  ;Func body at line 30
	%reg_0 = call i32 @getch()
	%reg_1 = add i32 80, 0
	%reg_2 = icmp ne i32 %reg_0, %reg_1
	br i1 %reg_2, label %Block2, label %Block4
Block2:  ;If then at line 31
	%reg_6 = add i32 1, 0
	%reg_7 = sub i32 0, %reg_6
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 31
	%reg_8 = call i32 @getint()
	store i32 %reg_8, ptr @width
	%reg_9 = call i32 @getint()
	store i32 %reg_9, ptr @height
	%reg_10 = load i32, ptr @width
	%reg_11 = load i32, ptr @MAX_WIDTH
	%reg_12 = icmp sgt i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block5, label %Block8
Block4:  ;Or opertor at line 31
	%reg_3 = call i32 @getch()
	%reg_4 = add i32 50, 0
	%reg_5 = icmp ne i32 %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block5:  ;If then at line 34
	%reg_19 = add i32 1, 0
	%reg_20 = sub i32 0, %reg_19
	ret i32 %reg_20
	br label %Block6
Block6:  ;If end at line 34
	%reg_22 = add i32 0, 0
	store i32 %reg_22, ptr %reg_21
	br label %Block9
Block7:  ;Or opertor at line 34
	%reg_16 = call i32 @getint()
	%reg_17 = add i32 255, 0
	%reg_18 = icmp ne i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block5, label %Block6
Block8:  ;Or opertor at line -1
	%reg_13 = load i32, ptr @height
	%reg_14 = load i32, ptr @MAX_HEIGHT
	%reg_15 = icmp sgt i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block5, label %Block7
Block9:  ;While condition at line 36
	%reg_23 = load i32, ptr %reg_21
	%reg_24 = load i32, ptr @height
	%reg_25 = icmp slt i32 %reg_23, %reg_24
	br i1 %reg_25, label %Block10, label %Block11
Block10:  ;While body at line 36
	%reg_27 = add i32 0, 0
	store i32 %reg_27, ptr %reg_26
	br label %Block12
Block11:  ;While end at line 36
	%reg_44 = add i32 0, 0
	ret i32 %reg_44
Block12:  ;While condition at line 38
	%reg_28 = load i32, ptr %reg_26
	%reg_29 = load i32, ptr @width
	%reg_30 = icmp slt i32 %reg_28, %reg_29
	br i1 %reg_30, label %Block13, label %Block14
Block13:  ;While body at line 38
	%reg_31 = load i32, ptr %reg_21
	%reg_32 = load i32, ptr @width
	%reg_33 = mul i32 %reg_31, %reg_32
	%reg_34 = load i32, ptr %reg_26
	%reg_35 = add i32 %reg_33, %reg_34
	%reg_36 = getelementptr [1048576 x i32], ptr @image, i32 0, i32 %reg_35
	%reg_37 = call i32 @getint()
	store i32 %reg_37, ptr %reg_36
	%reg_38 = load i32, ptr %reg_26
	%reg_39 = add i32 1, 0
	%reg_40 = add i32 %reg_38, %reg_39
	store i32 %reg_40, ptr %reg_26
	br label %Block12
Block14:  ;While end at line 38
	%reg_41 = load i32, ptr %reg_21
	%reg_42 = add i32 1, 0
	%reg_43 = add i32 %reg_41, %reg_42
	store i32 %reg_43, ptr %reg_21
	br label %Block9
}

define i32 @rotate(i32 %reg_0, i32 %reg_1, float %reg_2)
{
Block0:  ;Func define at line 47
	%reg_42 = alloca i32
	%reg_28 = alloca i32
	%reg_24 = alloca i32
	%reg_20 = alloca i32
	%reg_16 = alloca i32
	%reg_12 = alloca i32
	%reg_9 = alloca float
	%reg_6 = alloca float
	%reg_5 = alloca float
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store float %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 47
	%reg_7 = load float, ptr %reg_5
	%reg_8 = call float @my_sin(float %reg_7)
	store float %reg_8, ptr %reg_6
	%reg_10 = load float, ptr %reg_5
	%reg_11 = call float @my_cos(float %reg_10)
	store float %reg_11, ptr %reg_9
	%reg_13 = load i32, ptr @width
	%reg_14 = add i32 2, 0
	%reg_15 = sdiv i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_12
	%reg_17 = load i32, ptr @height
	%reg_18 = add i32 2, 0
	%reg_19 = sdiv i32 %reg_17, %reg_18
	store i32 %reg_19, ptr %reg_16
	%reg_21 = load i32, ptr %reg_3
	%reg_22 = load i32, ptr %reg_12
	%reg_23 = sub i32 %reg_21, %reg_22
	store i32 %reg_23, ptr %reg_20
	%reg_25 = load i32, ptr %reg_4
	%reg_26 = load i32, ptr %reg_16
	%reg_27 = sub i32 %reg_25, %reg_26
	store i32 %reg_27, ptr %reg_24
	%reg_29 = load i32, ptr %reg_20
	%reg_30 = sitofp i32 %reg_29 to float
	%reg_31 = load float, ptr %reg_9
	%reg_32 = fmul float %reg_30, %reg_31
	%reg_33 = load i32, ptr %reg_24
	%reg_34 = sitofp i32 %reg_33 to float
	%reg_35 = load float, ptr %reg_6
	%reg_36 = fmul float %reg_34, %reg_35
	%reg_37 = fsub float %reg_32, %reg_36
	%reg_38 = load i32, ptr %reg_12
	%reg_39 = sitofp i32 %reg_38 to float
	%reg_40 = fadd float %reg_37, %reg_39
	%reg_41 = fptosi float %reg_40 to i32
	store i32 %reg_41, ptr %reg_28
	%reg_43 = load i32, ptr %reg_20
	%reg_44 = sitofp i32 %reg_43 to float
	%reg_45 = load float, ptr %reg_6
	%reg_46 = fmul float %reg_44, %reg_45
	%reg_47 = load i32, ptr %reg_24
	%reg_48 = sitofp i32 %reg_47 to float
	%reg_49 = load float, ptr %reg_9
	%reg_50 = fmul float %reg_48, %reg_49
	%reg_51 = fadd float %reg_46, %reg_50
	%reg_52 = load i32, ptr %reg_16
	%reg_53 = sitofp i32 %reg_52 to float
	%reg_54 = fadd float %reg_51, %reg_53
	%reg_55 = fptosi float %reg_54 to i32
	store i32 %reg_55, ptr %reg_42
	%reg_56 = load i32, ptr %reg_28
	%reg_57 = add i32 0, 0
	%reg_58 = icmp slt i32 %reg_56, %reg_57
	br i1 %reg_58, label %Block2, label %Block6
Block2:  ;If then at line 53
	%reg_68 = add i32 0, 0
	ret i32 %reg_68
	br label %Block3
Block3:  ;If end at line 53
	%reg_69 = load i32, ptr %reg_42
	%reg_70 = load i32, ptr @width
	%reg_71 = mul i32 %reg_69, %reg_70
	%reg_72 = load i32, ptr %reg_28
	%reg_73 = add i32 %reg_71, %reg_72
	%reg_74 = getelementptr [1048576 x i32], ptr @image, i32 0, i32 %reg_73
	%reg_75 = load i32, ptr %reg_74
	ret i32 %reg_75
Block4:  ;Or opertor at line 53
	%reg_65 = load i32, ptr %reg_42
	%reg_66 = load i32, ptr @height
	%reg_67 = icmp sge i32 %reg_65, %reg_66
	br i1 %reg_67, label %Block2, label %Block3
Block5:  ;Or opertor at line -1
	%reg_62 = load i32, ptr %reg_42
	%reg_63 = add i32 0, 0
	%reg_64 = icmp slt i32 %reg_62, %reg_63
	br i1 %reg_64, label %Block2, label %Block4
Block6:  ;Or opertor at line -1
	%reg_59 = load i32, ptr %reg_28
	%reg_60 = load i32, ptr @width
	%reg_61 = icmp sge i32 %reg_59, %reg_60
	br i1 %reg_61, label %Block2, label %Block5
}

define void @write_pgm(float %reg_0)
{
Block0:  ;Func define at line 57
	%reg_16 = alloca i32
	%reg_11 = alloca i32
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 57
	%reg_2 = add i32 80, 0
	call void @putch(i32 %reg_2)
	%reg_3 = add i32 50, 0
	call void @putch(i32 %reg_3)
	%reg_4 = add i32 10, 0
	call void @putch(i32 %reg_4)
	%reg_5 = load i32, ptr @width
	call void @putint(i32 %reg_5)
	%reg_6 = add i32 32, 0
	call void @putch(i32 %reg_6)
	%reg_7 = load i32, ptr @height
	call void @putint(i32 %reg_7)
	%reg_8 = add i32 32, 0
	call void @putch(i32 %reg_8)
	%reg_9 = add i32 255, 0
	call void @putint(i32 %reg_9)
	%reg_10 = add i32 10, 0
	call void @putch(i32 %reg_10)
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_11
	br label %Block2
Block2:  ;While condition at line 62
	%reg_13 = load i32, ptr %reg_11
	%reg_14 = load i32, ptr @height
	%reg_15 = icmp slt i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block3, label %Block4
Block3:  ;While body at line 62
	%reg_17 = add i32 0, 0
	store i32 %reg_17, ptr %reg_16
	br label %Block5
Block4:  ;While end at line 62
	ret void
Block5:  ;While condition at line 64
	%reg_18 = load i32, ptr %reg_16
	%reg_19 = load i32, ptr @width
	%reg_20 = icmp slt i32 %reg_18, %reg_19
	br i1 %reg_20, label %Block6, label %Block7
Block6:  ;While body at line 64
	%reg_21 = load i32, ptr %reg_16
	%reg_22 = load i32, ptr %reg_11
	%reg_23 = load float, ptr %reg_1
	%reg_24 = call i32 @rotate(i32 %reg_21, i32 %reg_22, float %reg_23)
	call void @putint(i32 %reg_24)
	%reg_25 = add i32 32, 0
	call void @putch(i32 %reg_25)
	%reg_26 = load i32, ptr %reg_16
	%reg_27 = add i32 1, 0
	%reg_28 = add i32 %reg_26, %reg_27
	store i32 %reg_28, ptr %reg_16
	br label %Block5
Block7:  ;While end at line 64
	%reg_29 = add i32 10, 0
	call void @putch(i32 %reg_29)
	%reg_30 = load i32, ptr %reg_11
	%reg_31 = add i32 1, 0
	%reg_32 = add i32 %reg_30, %reg_31
	store i32 %reg_32, ptr %reg_11
	br label %Block2
}

define i32 @main()
{
Block0:  ;Func define at line 74
	%reg_0 = alloca float
	br label %Block1
Block1:  ;Func body at line 74
	%reg_1 = call float @getfloat()
	store float %reg_1, ptr %reg_0
	%reg_2 = call i32 @getch()
	%reg_3 = call i32 @read_image()
	%reg_4 = add i32 0, 0
	%reg_5 = icmp slt i32 %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 77
	%reg_6 = add i32 1, 0
	%reg_7 = sub i32 0, %reg_6
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 77
	%reg_8 = load float, ptr %reg_0
	call void @write_pgm(float %reg_8)
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
}
