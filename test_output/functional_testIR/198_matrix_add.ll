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

@M = global i32 zeroinitializer
@L = global i32 zeroinitializer
@N = global i32 zeroinitializer

define i32 @add(ptr %reg_0, ptr %reg_1, ptr %reg_2, ptr %reg_3, ptr %reg_4, ptr %reg_5, ptr %reg_6, ptr %reg_7, ptr %reg_8)
{
Block0:  ;Func define at line 6
	%reg_9 = alloca i32
	br label %Block1
Block1:  ;Func body at line 6
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_9
	br label %Block2
Block2:  ;While condition at line 10
	%reg_12 = load i32, ptr %reg_9
	%reg_13 = load i32, ptr @M
	%reg_14 = icmp slt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 10
	%reg_15 = load i32, ptr %reg_9
	%reg_16 = getelementptr float, ptr %reg_6, i32 %reg_15
	%reg_17 = load i32, ptr %reg_9
	%reg_18 = getelementptr float, ptr %reg_0, i32 %reg_17
	%reg_19 = load float, ptr %reg_18
	%reg_20 = load i32, ptr %reg_9
	%reg_21 = getelementptr float, ptr %reg_3, i32 %reg_20
	%reg_22 = load float, ptr %reg_21
	%reg_23 = fadd float %reg_19, %reg_22
	store float %reg_23, ptr %reg_16
	%reg_24 = load i32, ptr %reg_9
	%reg_25 = getelementptr float, ptr %reg_7, i32 %reg_24
	%reg_26 = load i32, ptr %reg_9
	%reg_27 = getelementptr float, ptr %reg_1, i32 %reg_26
	%reg_28 = load float, ptr %reg_27
	%reg_29 = load i32, ptr %reg_9
	%reg_30 = getelementptr float, ptr %reg_4, i32 %reg_29
	%reg_31 = load float, ptr %reg_30
	%reg_32 = fadd float %reg_28, %reg_31
	store float %reg_32, ptr %reg_25
	%reg_33 = load i32, ptr %reg_9
	%reg_34 = getelementptr float, ptr %reg_8, i32 %reg_33
	%reg_35 = load i32, ptr %reg_9
	%reg_36 = getelementptr float, ptr %reg_2, i32 %reg_35
	%reg_37 = load float, ptr %reg_36
	%reg_38 = load i32, ptr %reg_9
	%reg_39 = getelementptr float, ptr %reg_5, i32 %reg_38
	%reg_40 = load float, ptr %reg_39
	%reg_41 = fadd float %reg_37, %reg_40
	store float %reg_41, ptr %reg_34
	%reg_42 = load i32, ptr %reg_9
	%reg_43 = add i32 1, 0
	%reg_44 = add i32 %reg_42, %reg_43
	store i32 %reg_44, ptr %reg_9
	br label %Block2
Block4:  ;While end at line 10
	%reg_45 = add i32 0, 0
	ret i32 %reg_45
}

define i32 @main()
{
Block0:  ;Func define at line 22
	%reg_55 = alloca i32
	%reg_12 = alloca i32
	%reg_11 = alloca [3 x float]
	%reg_10 = alloca [3 x float]
	%reg_9 = alloca [6 x float]
	%reg_8 = alloca [3 x float]
	%reg_7 = alloca [3 x float]
	%reg_6 = alloca [3 x float]
	%reg_5 = alloca [3 x float]
	%reg_4 = alloca [3 x float]
	%reg_3 = alloca [3 x float]
	br label %Block1
Block1:  ;Func body at line 22
	%reg_0 = add i32 3, 0
	store i32 %reg_0, ptr @N
	%reg_1 = add i32 3, 0
	store i32 %reg_1, ptr @M
	%reg_2 = add i32 3, 0
	store i32 %reg_2, ptr @L
	call void @llvm.memset.p0.i32(ptr %reg_3, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_4, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_5, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_6, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_7, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_8, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_9, i8 0, i32 24, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_10, i8 0, i32 12, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_11, i8 0, i32 12, i1 1)
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_12
	br label %Block2
Block2:  ;While condition at line 30
	%reg_15 = load i32, ptr %reg_12
	%reg_16 = load i32, ptr @M
	%reg_17 = icmp slt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block3, label %Block4
Block3:  ;While body at line 30
	%reg_18 = load i32, ptr %reg_12
	%reg_19 = getelementptr [3 x float], ptr %reg_3, i32 0, i32 %reg_18
	%reg_20 = load i32, ptr %reg_12
	%reg_21 = sitofp i32 %reg_20 to float
	store float %reg_21, ptr %reg_19
	%reg_22 = load i32, ptr %reg_12
	%reg_23 = getelementptr [3 x float], ptr %reg_4, i32 0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_12
	%reg_25 = sitofp i32 %reg_24 to float
	store float %reg_25, ptr %reg_23
	%reg_26 = load i32, ptr %reg_12
	%reg_27 = getelementptr [3 x float], ptr %reg_5, i32 0, i32 %reg_26
	%reg_28 = load i32, ptr %reg_12
	%reg_29 = sitofp i32 %reg_28 to float
	store float %reg_29, ptr %reg_27
	%reg_30 = load i32, ptr %reg_12
	%reg_31 = getelementptr [3 x float], ptr %reg_6, i32 0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_12
	%reg_33 = sitofp i32 %reg_32 to float
	store float %reg_33, ptr %reg_31
	%reg_34 = load i32, ptr %reg_12
	%reg_35 = getelementptr [3 x float], ptr %reg_7, i32 0, i32 %reg_34
	%reg_36 = load i32, ptr %reg_12
	%reg_37 = sitofp i32 %reg_36 to float
	store float %reg_37, ptr %reg_35
	%reg_38 = load i32, ptr %reg_12
	%reg_39 = getelementptr [3 x float], ptr %reg_8, i32 0, i32 %reg_38
	%reg_40 = load i32, ptr %reg_12
	%reg_41 = sitofp i32 %reg_40 to float
	store float %reg_41, ptr %reg_39
	%reg_42 = load i32, ptr %reg_12
	%reg_43 = add i32 1, 0
	%reg_44 = add i32 %reg_42, %reg_43
	store i32 %reg_44, ptr %reg_12
	br label %Block2
Block4:  ;While end at line 30
	%reg_45 = getelementptr [3 x float], ptr %reg_3, i32 0
	%reg_46 = getelementptr [3 x float], ptr %reg_4, i32 0
	%reg_47 = getelementptr [3 x float], ptr %reg_5, i32 0
	%reg_48 = getelementptr [3 x float], ptr %reg_6, i32 0
	%reg_49 = getelementptr [3 x float], ptr %reg_7, i32 0
	%reg_50 = getelementptr [3 x float], ptr %reg_8, i32 0
	%reg_51 = getelementptr [6 x float], ptr %reg_9, i32 0
	%reg_52 = getelementptr [3 x float], ptr %reg_10, i32 0
	%reg_53 = getelementptr [3 x float], ptr %reg_11, i32 0
	%reg_54 = call i32 @add(ptr %reg_45, ptr %reg_46, ptr %reg_47, ptr %reg_48, ptr %reg_49, ptr %reg_50, ptr %reg_51, ptr %reg_52, ptr %reg_53)
	store i32 %reg_54, ptr %reg_12
	%reg_56 = add i32 0, 0
	store i32 %reg_56, ptr %reg_55
	br label %Block5
Block5:  ;While condition at line 42
	%reg_57 = load i32, ptr %reg_12
	%reg_58 = load i32, ptr @N
	%reg_59 = icmp slt i32 %reg_57, %reg_58
	br i1 %reg_59, label %Block6, label %Block7
Block6:  ;While body at line 42
	%reg_60 = load i32, ptr %reg_12
	%reg_61 = getelementptr [6 x float], ptr %reg_9, i32 0, i32 %reg_60
	%reg_62 = load float, ptr %reg_61
	%reg_63 = fptosi float %reg_62 to i32
	store i32 %reg_63, ptr %reg_55
	%reg_64 = load i32, ptr %reg_55
	call void @putint(i32 %reg_64)
	%reg_65 = load i32, ptr %reg_12
	%reg_66 = add i32 1, 0
	%reg_67 = add i32 %reg_65, %reg_66
	store i32 %reg_67, ptr %reg_12
	br label %Block5
Block7:  ;While end at line 42
	%reg_68 = add i32 10, 0
	store i32 %reg_68, ptr %reg_55
	%reg_69 = load i32, ptr %reg_55
	call void @putch(i32 %reg_69)
	%reg_70 = add i32 0, 0
	store i32 %reg_70, ptr %reg_12
	br label %Block8
Block8:  ;While condition at line 51
	%reg_71 = load i32, ptr %reg_12
	%reg_72 = load i32, ptr @N
	%reg_73 = icmp slt i32 %reg_71, %reg_72
	br i1 %reg_73, label %Block9, label %Block10
Block9:  ;While body at line 51
	%reg_74 = load i32, ptr %reg_12
	%reg_75 = getelementptr [3 x float], ptr %reg_10, i32 0, i32 %reg_74
	%reg_76 = load float, ptr %reg_75
	%reg_77 = fptosi float %reg_76 to i32
	store i32 %reg_77, ptr %reg_55
	%reg_78 = load i32, ptr %reg_55
	call void @putint(i32 %reg_78)
	%reg_79 = load i32, ptr %reg_12
	%reg_80 = add i32 1, 0
	%reg_81 = add i32 %reg_79, %reg_80
	store i32 %reg_81, ptr %reg_12
	br label %Block8
Block10:  ;While end at line 51
	%reg_82 = add i32 10, 0
	store i32 %reg_82, ptr %reg_55
	%reg_83 = load i32, ptr %reg_55
	call void @putch(i32 %reg_83)
	%reg_84 = add i32 0, 0
	store i32 %reg_84, ptr %reg_12
	br label %Block11
Block11:  ;While condition at line 60
	%reg_85 = load i32, ptr %reg_12
	%reg_86 = load i32, ptr @N
	%reg_87 = icmp slt i32 %reg_85, %reg_86
	br i1 %reg_87, label %Block12, label %Block13
Block12:  ;While body at line 60
	%reg_88 = load i32, ptr %reg_12
	%reg_89 = getelementptr [3 x float], ptr %reg_11, i32 0, i32 %reg_88
	%reg_90 = load float, ptr %reg_89
	%reg_91 = fptosi float %reg_90 to i32
	store i32 %reg_91, ptr %reg_55
	%reg_92 = load i32, ptr %reg_55
	call void @putint(i32 %reg_92)
	%reg_93 = load i32, ptr %reg_12
	%reg_94 = add i32 1, 0
	%reg_95 = add i32 %reg_93, %reg_94
	store i32 %reg_95, ptr %reg_12
	br label %Block11
Block13:  ;While end at line 60
	%reg_96 = add i32 10, 0
	store i32 %reg_96, ptr %reg_55
	%reg_97 = load i32, ptr %reg_55
	call void @putch(i32 %reg_97)
	%reg_98 = add i32 0, 0
	ret i32 %reg_98
}
