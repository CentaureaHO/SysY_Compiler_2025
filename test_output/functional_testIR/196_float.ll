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

@RADIUS = global float 0x4016000000000000
@PI = global float 0x400921fb60000000
@EPS = global float 0x3eb0c6f7a0000000
@PI_HEX = global float 0x400921fb60000000
@HEX2 = global float 0x3fb4000000000000
@FACT = global float 0xc0e01d0000000000
@EVAL1 = global float 0x4057c21fc0000000
@EVAL2 = global float 0x4041475ce0000000
@EVAL3 = global float 0x4041475ce0000000
@CONV1 = global float 0x406d200000000000
@CONV2 = global float 0x40affe0000000000
@MAX = global i32 1000000000
@TWO = global i32 2
@THREE = global i32 3
@FIVE = global i32 5

define float @float_abs(float %reg_0)
{
Block0:  ;Func define at line 15
	%reg_1 = alloca float
	store float %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 15
	%reg_2 = load float, ptr %reg_1
	%reg_3 = add i32 0, 0
	%reg_4 = sitofp i32 %reg_3 to float
	%reg_5 = fcmp olt float %reg_2, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 16
	%reg_6 = load float, ptr %reg_1
	%reg_7 = fsub float 0x0, %reg_6
	ret float %reg_7
	br label %Block3
Block3:  ;If end at line 16
	%reg_8 = load float, ptr %reg_1
	ret float %reg_8
}

define float @circle_area(i32 %reg_0)
{
Block0:  ;Func define at line 21
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 21
	%reg_2 = load float, ptr @PI
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = sitofp i32 %reg_3 to float
	%reg_5 = fmul float %reg_2, %reg_4
	%reg_6 = load i32, ptr %reg_1
	%reg_7 = sitofp i32 %reg_6 to float
	%reg_8 = fmul float %reg_5, %reg_7
	%reg_9 = load i32, ptr %reg_1
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = mul i32 %reg_9, %reg_10
	%reg_12 = sitofp i32 %reg_11 to float
	%reg_13 = load float, ptr @PI
	%reg_14 = fmul float %reg_12, %reg_13
	%reg_15 = fadd float %reg_8, %reg_14
	%reg_16 = add i32 2, 0
	%reg_17 = sitofp i32 %reg_16 to float
	%reg_18 = fdiv float %reg_15, %reg_17
	ret float %reg_18
}

define i32 @float_eq(float %reg_0, float %reg_1)
{
Block0:  ;Func define at line 26
	%reg_3 = alloca float
	%reg_2 = alloca float
	store float %reg_0, ptr %reg_2
	store float %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 26
	%reg_4 = load float, ptr %reg_2
	%reg_5 = load float, ptr %reg_3
	%reg_6 = fsub float %reg_4, %reg_5
	%reg_7 = call float @float_abs(float %reg_6)
	%reg_8 = load float, ptr @EPS
	%reg_9 = fcmp olt float %reg_7, %reg_8
	br i1 %reg_9, label %Block2, label %Block4
Block2:  ;If then at line 27
	%reg_10 = add i32 1, 0
	%reg_11 = sitofp i32 %reg_10 to float
	%reg_12 = fadd float 0x4000000000000000, 0x0
	%reg_13 = fmul float %reg_11, %reg_12
	%reg_14 = add i32 2, 0
	%reg_15 = sitofp i32 %reg_14 to float
	%reg_16 = fdiv float %reg_13, %reg_15
	%reg_17 = fptosi float %reg_16 to i32
	ret i32 %reg_17
	br label %Block3
Block3:  ;If end at line 27
	ret i32 0
Block4:  ;If else at line 27
	%reg_18 = add i32 0, 0
	ret i32 %reg_18
	br label %Block3
}

define void @error()
{
Block0:  ;Func define at line 34
	br label %Block1
Block1:  ;Func body at line 34
	%reg_0 = add i32 101, 0
	call void @putch(i32 %reg_0)
	%reg_1 = add i32 114, 0
	call void @putch(i32 %reg_1)
	%reg_2 = add i32 114, 0
	call void @putch(i32 %reg_2)
	%reg_3 = add i32 111, 0
	call void @putch(i32 %reg_3)
	%reg_4 = add i32 114, 0
	call void @putch(i32 %reg_4)
	%reg_5 = add i32 10, 0
	call void @putch(i32 %reg_5)
	ret void
}

define void @ok()
{
Block0:  ;Func define at line 43
	br label %Block1
Block1:  ;Func body at line 43
	%reg_0 = add i32 111, 0
	call void @putch(i32 %reg_0)
	%reg_1 = add i32 107, 0
	call void @putch(i32 %reg_1)
	%reg_2 = add i32 10, 0
	call void @putch(i32 %reg_2)
	ret void
}

define void @assert(i32 %reg_0)
{
Block0:  ;Func define at line 49
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 49
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = icmp eq i32 %reg_2, 0
	br i1 %reg_3, label %Block2, label %Block4
Block2:  ;If then at line 50
	call void @error()
	br label %Block3
Block3:  ;If end at line 50
	ret void
Block4:  ;If else at line 50
	call void @ok()
	br label %Block3
}

define i32 @main()
{
Block0:  ;Func define at line 57
	%reg_62 = alloca float
	%reg_56 = alloca float
	%reg_54 = alloca float
	%reg_48 = alloca i32
	%reg_42 = alloca [10 x float]
	%reg_40 = alloca i32
	%reg_38 = alloca i32
	br label %Block1
Block1:  ;Func body at line 57
	%reg_0 = load float, ptr @HEX2
	%reg_1 = load float, ptr @FACT
	%reg_2 = call i32 @float_eq(float %reg_0, float %reg_1)
	%reg_3 = icmp eq i32 %reg_2, 0
	%reg_4 = zext i1 %reg_3 to i32
	call void @assert(i32 %reg_4)
	%reg_5 = load float, ptr @EVAL1
	%reg_6 = load float, ptr @EVAL2
	%reg_7 = call i32 @float_eq(float %reg_5, float %reg_6)
	%reg_8 = icmp eq i32 %reg_7, 0
	%reg_9 = zext i1 %reg_8 to i32
	call void @assert(i32 %reg_9)
	%reg_10 = load float, ptr @EVAL2
	%reg_11 = load float, ptr @EVAL3
	%reg_12 = call i32 @float_eq(float %reg_10, float %reg_11)
	call void @assert(i32 %reg_12)
	%reg_13 = load float, ptr @RADIUS
	%reg_14 = fptosi float %reg_13 to i32
	%reg_15 = call float @circle_area(i32 %reg_14)
	%reg_16 = load i32, ptr @FIVE
	%reg_17 = call float @circle_area(i32 %reg_16)
	%reg_18 = call i32 @float_eq(float %reg_15, float %reg_17)
	call void @assert(i32 %reg_18)
	%reg_19 = load float, ptr @CONV1
	%reg_20 = load float, ptr @CONV2
	%reg_21 = call i32 @float_eq(float %reg_19, float %reg_20)
	%reg_22 = icmp eq i32 %reg_21, 0
	%reg_23 = zext i1 %reg_22 to i32
	call void @assert(i32 %reg_23)
	%reg_24 = fadd float 0x3ff8000000000000, 0x0
	%reg_25 = fcmp one float %reg_24, 0x0
	br i1 %reg_25, label %Block2, label %Block3
Block2:  ;If then at line 66
	call void @ok()
	br label %Block3
Block3:  ;If end at line 66
	%reg_26 = fadd float 0x400a666660000000, 0x0
	%reg_27 = fcmp oeq float %reg_26, 0x0
	%reg_28 = zext i1 %reg_27 to i32
	%reg_29 = icmp eq i32 %reg_28, 0
	br i1 %reg_29, label %Block4, label %Block5
Block4:  ;If then at line 67
	call void @ok()
	br label %Block5
Block5:  ;If end at line 67
	%reg_30 = fadd float 0x0, 0x0
	%reg_31 = fcmp one float %reg_30, 0x0
	br i1 %reg_31, label %Block8, label %Block7
Block6:  ;If then at line 68
	call void @error()
	br label %Block7
Block7:  ;If end at line 68
	%reg_34 = add i32 0, 0
	%reg_35 = icmp ne i32 %reg_34, 0
	br i1 %reg_35, label %Block9, label %Block11
Block8:  ;And opertor at line 68
	%reg_32 = add i32 3, 0
	%reg_33 = icmp ne i32 %reg_32, 0
	br i1 %reg_33, label %Block6, label %Block7
Block9:  ;If then at line 69
	call void @ok()
	br label %Block10
Block10:  ;If end at line 69
	%reg_39 = add i32 1, 0
	store i32 %reg_39, ptr %reg_38
	%reg_41 = add i32 0, 0
	store i32 %reg_41, ptr %reg_40
	call void @llvm.memset.p0.i32(ptr %reg_42, i8 0, i32 40, i1 1)
	%reg_43 = fadd float 0x3ff0000000000000, 0x0
	%reg_44 = getelementptr [10 x float], ptr %reg_42, i32 0, i32 0
	store float %reg_43, ptr %reg_44
	%reg_45 = add i32 2, 0
	%reg_46 = sitofp i32 %reg_45 to float
	%reg_47 = getelementptr [10 x float], ptr %reg_42, i32 0, i32 1
	store float %reg_46, ptr %reg_47
	%reg_49 = getelementptr [10 x float], ptr %reg_42, i32 0
	%reg_50 = call i32 @getfarray(ptr %reg_49)
	store i32 %reg_50, ptr %reg_48
	br label %Block12
Block11:  ;Or opertor at line 69
	%reg_36 = fadd float 0x3fd3333340000000, 0x0
	%reg_37 = fcmp one float %reg_36, 0x0
	br i1 %reg_37, label %Block9, label %Block10
Block12:  ;While condition at line 75
	%reg_51 = load i32, ptr %reg_38
	%reg_52 = load i32, ptr @MAX
	%reg_53 = icmp slt i32 %reg_51, %reg_52
	br i1 %reg_53, label %Block13, label %Block14
Block13:  ;While body at line 75
	%reg_55 = call float @getfloat()
	store float %reg_55, ptr %reg_54
	%reg_57 = load float, ptr @PI
	%reg_58 = load float, ptr %reg_54
	%reg_59 = fmul float %reg_57, %reg_58
	%reg_60 = load float, ptr %reg_54
	%reg_61 = fmul float %reg_59, %reg_60
	store float %reg_61, ptr %reg_56
	%reg_63 = load float, ptr %reg_54
	%reg_64 = fptosi float %reg_63 to i32
	%reg_65 = call float @circle_area(i32 %reg_64)
	store float %reg_65, ptr %reg_62
	%reg_66 = load i32, ptr %reg_40
	%reg_67 = getelementptr [10 x float], ptr %reg_42, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_40
	%reg_69 = getelementptr [10 x float], ptr %reg_42, i32 0, i32 %reg_68
	%reg_70 = load float, ptr %reg_69
	%reg_71 = load float, ptr %reg_54
	%reg_72 = fadd float %reg_70, %reg_71
	store float %reg_72, ptr %reg_67
	%reg_73 = load float, ptr %reg_56
	call void @putfloat(float %reg_73)
	%reg_74 = add i32 32, 0
	call void @putch(i32 %reg_74)
	%reg_75 = load float, ptr %reg_62
	%reg_76 = fptosi float %reg_75 to i32
	call void @putint(i32 %reg_76)
	%reg_77 = add i32 10, 0
	call void @putch(i32 %reg_77)
	%reg_78 = load i32, ptr %reg_38
	%reg_79 = sitofp i32 %reg_78 to float
	%reg_80 = fadd float 0x4024000000000000, 0x0
	%reg_81 = fsub float 0x0, %reg_80
	%reg_82 = fsub float 0x0, %reg_81
	%reg_83 = fmul float %reg_79, %reg_82
	%reg_84 = fptosi float %reg_83 to i32
	store i32 %reg_84, ptr %reg_38
	%reg_85 = load i32, ptr %reg_40
	%reg_86 = add i32 1, 0
	%reg_87 = add i32 %reg_85, %reg_86
	store i32 %reg_87, ptr %reg_40
	br label %Block12
Block14:  ;While end at line 75
	%reg_88 = load i32, ptr %reg_48
	%reg_89 = getelementptr [10 x float], ptr %reg_42, i32 0
	call void @putfarray(i32 %reg_88, ptr %reg_89)
	%reg_90 = add i32 0, 0
	ret i32 %reg_90
}
