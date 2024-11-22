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

@maxn = global i32 1005
@n = global i32 1000
@m = global i32 zeroinitializer
@a = global [1005 x float] zeroinitializer
@ff = global [1005 x float] zeroinitializer

define float @DFS(i32 %reg_0, float %reg_1, float %reg_2, float %reg_3, ptr %reg_4)
{
Block0:  ;Func define at line 4
	%reg_46 = alloca i32
	%reg_45 = alloca [1005 x float]
	%reg_8 = alloca float
	%reg_7 = alloca float
	%reg_6 = alloca float
	%reg_5 = alloca i32
	store i32 %reg_0, ptr %reg_5
	store float %reg_1, ptr %reg_6
	store float %reg_2, ptr %reg_7
	store float %reg_3, ptr %reg_8
	br label %Block1
Block1:  ;Func body at line 4
	%reg_9 = load i32, ptr %reg_5
	%reg_10 = getelementptr [1005 x float], ptr @ff, i32 0, i32 %reg_9
	%reg_11 = load float, ptr %reg_8
	%reg_12 = load i32, ptr %reg_5
	%reg_13 = getelementptr [1005 x float], ptr @a, i32 0, i32 %reg_12
	%reg_14 = load float, ptr %reg_13
	%reg_15 = add i32 3, 0
	%reg_16 = sitofp i32 %reg_15 to float
	%reg_17 = load float, ptr %reg_6
	%reg_18 = fmul float %reg_16, %reg_17
	%reg_19 = add i32 3, 0
	%reg_20 = sitofp i32 %reg_19 to float
	%reg_21 = load float, ptr %reg_7
	%reg_22 = fmul float %reg_20, %reg_21
	%reg_23 = fadd float %reg_18, %reg_22
	%reg_24 = add i32 1, 0
	%reg_25 = sitofp i32 %reg_24 to float
	%reg_26 = fadd float %reg_23, %reg_25
	%reg_27 = fmul float %reg_14, %reg_26
	%reg_28 = fadd float %reg_11, %reg_27
	store float %reg_28, ptr %reg_10
	%reg_29 = load i32, ptr %reg_5
	%reg_30 = getelementptr float, ptr %reg_4, i32 %reg_29
	%reg_31 = load i32, ptr %reg_5
	%reg_32 = getelementptr [1005 x float], ptr @a, i32 0, i32 %reg_31
	%reg_33 = load float, ptr %reg_32
	%reg_34 = load float, ptr %reg_6
	%reg_35 = add i32 1, 0
	%reg_36 = sitofp i32 %reg_35 to float
	%reg_37 = fadd float %reg_34, %reg_36
	%reg_38 = fmul float %reg_33, %reg_37
	store float %reg_38, ptr %reg_30
	%reg_39 = load i32, ptr %reg_5
	%reg_40 = load i32, ptr @n
	%reg_41 = icmp eq i32 %reg_39, %reg_40
	br i1 %reg_41, label %Block2, label %Block3
Block2:  ;If then at line 7
	%reg_42 = load i32, ptr %reg_5
	%reg_43 = getelementptr float, ptr %reg_4
	call void @putfarray(i32 %reg_42, ptr %reg_43)
	%reg_44 = load float, ptr %reg_7
	ret float %reg_44
	br label %Block3
Block3:  ;If end at line 7
	call void @llvm.memset.p0.i32(ptr %reg_45, i8 0, i32 4020, i1 1)
	%reg_47 = add i32 0, 0
	store i32 %reg_47, ptr %reg_46
	br label %Block4
Block4:  ;While condition at line 14
	%reg_48 = load i32, ptr %reg_46
	%reg_49 = load i32, ptr @n
	%reg_50 = icmp slt i32 %reg_48, %reg_49
	br i1 %reg_50, label %Block5, label %Block6
Block5:  ;While body at line 14
	%reg_51 = load i32, ptr %reg_46
	%reg_52 = getelementptr [1005 x float], ptr %reg_45, i32 0, i32 %reg_51
	%reg_53 = load i32, ptr %reg_46
	%reg_54 = getelementptr float, ptr %reg_4, i32 %reg_53
	%reg_55 = load float, ptr %reg_54
	%reg_56 = load float, ptr %reg_6
	%reg_57 = fadd float %reg_55, %reg_56
	%reg_58 = load float, ptr %reg_7
	%reg_59 = fadd float %reg_57, %reg_58
	%reg_60 = load float, ptr %reg_8
	%reg_61 = fadd float %reg_59, %reg_60
	store float %reg_61, ptr %reg_52
	%reg_62 = load i32, ptr %reg_46
	%reg_63 = add i32 1, 0
	%reg_64 = add i32 %reg_62, %reg_63
	store i32 %reg_64, ptr %reg_46
	br label %Block4
Block6:  ;While end at line 14
	%reg_65 = load i32, ptr %reg_5
	%reg_66 = add i32 1, 0
	%reg_67 = add i32 %reg_65, %reg_66
	%reg_68 = load i32, ptr %reg_5
	%reg_69 = getelementptr float, ptr %reg_4, i32 %reg_68
	%reg_70 = load float, ptr %reg_69
	%reg_71 = load i32, ptr %reg_5
	%reg_72 = getelementptr [1005 x float], ptr @a, i32 0, i32 %reg_71
	%reg_73 = load float, ptr %reg_72
	%reg_74 = load float, ptr %reg_6
	%reg_75 = add i32 2, 0
	%reg_76 = sitofp i32 %reg_75 to float
	%reg_77 = load float, ptr %reg_7
	%reg_78 = fmul float %reg_76, %reg_77
	%reg_79 = fadd float %reg_74, %reg_78
	%reg_80 = add i32 1, 0
	%reg_81 = sitofp i32 %reg_80 to float
	%reg_82 = fadd float %reg_79, %reg_81
	%reg_83 = fmul float %reg_73, %reg_82
	%reg_84 = load i32, ptr %reg_5
	%reg_85 = getelementptr [1005 x float], ptr @ff, i32 0, i32 %reg_84
	%reg_86 = load float, ptr %reg_85
	%reg_87 = getelementptr [1005 x float], ptr %reg_45, i32 0
	%reg_88 = call float @DFS(i32 %reg_67, float %reg_70, float %reg_83, float %reg_86, ptr %reg_87)
	ret float %reg_88
}

define i32 @main()
{
Block0:  ;Func define at line 21
	%reg_13 = alloca float
	%reg_2 = alloca [1005 x float]
	br label %Block1
Block1:  ;Func body at line 21
	%reg_0 = getelementptr [1005 x float], ptr @a, i32 0
	%reg_1 = call i32 @getfarray(ptr %reg_0)
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 4020, i1 1)
	%reg_3 = add i32 0, 0
	%reg_4 = getelementptr [1005 x float], ptr %reg_2, i32 0, i32 %reg_3
	%reg_5 = add i32 0, 0
	%reg_6 = getelementptr [1005 x float], ptr @a, i32 0, i32 %reg_5
	%reg_7 = load float, ptr %reg_6
	store float %reg_7, ptr %reg_4
	%reg_8 = add i32 0, 0
	%reg_9 = getelementptr [1005 x float], ptr @ff, i32 0, i32 %reg_8
	%reg_10 = add i32 0, 0
	%reg_11 = getelementptr [1005 x float], ptr @a, i32 0, i32 %reg_10
	%reg_12 = load float, ptr %reg_11
	store float %reg_12, ptr %reg_9
	%reg_14 = fadd float 0x0, 0x0
	store float %reg_14, ptr %reg_13
	%reg_15 = add i32 1, 0
	%reg_16 = add i32 0, 0
	%reg_17 = getelementptr [1005 x float], ptr @a, i32 0, i32 %reg_16
	%reg_18 = load float, ptr %reg_17
	%reg_19 = add i32 0, 0
	%reg_20 = getelementptr [1005 x float], ptr @a, i32 0, i32 %reg_19
	%reg_21 = load float, ptr %reg_20
	%reg_22 = add i32 0, 0
	%reg_23 = getelementptr [1005 x float], ptr @a, i32 0, i32 %reg_22
	%reg_24 = load float, ptr %reg_23
	%reg_25 = getelementptr [1005 x float], ptr %reg_2, i32 0
	%reg_26 = call float @DFS(i32 %reg_15, float %reg_18, float %reg_21, float %reg_24, ptr %reg_25)
	store float %reg_26, ptr %reg_13
	%reg_27 = load float, ptr %reg_13
	call void @putfloat(float %reg_27)
	%reg_28 = add i32 32, 0
	call void @putch(i32 %reg_28)
	%reg_29 = load i32, ptr @n
	%reg_30 = getelementptr [1005 x float], ptr @ff, i32 0
	call void @putfarray(i32 %reg_29, ptr %reg_30)
	%reg_31 = add i32 0, 0
	ret i32 %reg_31
}
