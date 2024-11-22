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


define i32 @main()
{
Block0:  ;Func define at line 2
	%reg_26 = alloca i32
	%reg_24 = alloca i32
	%reg_22 = alloca i32
	%reg_20 = alloca i32
	%reg_18 = alloca i32
	%reg_4 = alloca [10 x i32]
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 2
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	call void @llvm.memset.p0.i32(ptr %reg_4, i8 0, i32 40, i1 1)
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_2
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 12
	%reg_7 = load i32, ptr %reg_0
	%reg_8 = add i32 10, 0
	%reg_9 = icmp slt i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block3, label %Block4
Block3:  ;While body at line 12
	%reg_10 = load i32, ptr %reg_0
	%reg_11 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_0
	%reg_13 = add i32 1, 0
	%reg_14 = add i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_11
	%reg_15 = load i32, ptr %reg_0
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 12
	%reg_19 = add i32 0, 0
	store i32 %reg_19, ptr %reg_18
	%reg_21 = add i32 0, 0
	store i32 %reg_21, ptr %reg_20
	%reg_23 = add i32 0, 0
	store i32 %reg_23, ptr %reg_22
	%reg_25 = add i32 0, 0
	store i32 %reg_25, ptr %reg_24
	%reg_27 = add i32 0, 0
	store i32 %reg_27, ptr %reg_26
	%reg_28 = add i32 10, 0
	store i32 %reg_28, ptr %reg_26
	%reg_29 = call i32 @getint()
	store i32 %reg_29, ptr %reg_18
	%reg_30 = load i32, ptr %reg_26
	%reg_31 = add i32 1, 0
	%reg_32 = sub i32 %reg_30, %reg_31
	store i32 %reg_32, ptr %reg_20
	%reg_33 = add i32 0, 0
	store i32 %reg_33, ptr %reg_22
	%reg_34 = load i32, ptr %reg_20
	%reg_35 = load i32, ptr %reg_22
	%reg_36 = add i32 %reg_34, %reg_35
	%reg_37 = add i32 2, 0
	%reg_38 = sdiv i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_24
	br label %Block5
Block5:  ;While condition at line 27
	%reg_39 = load i32, ptr %reg_24
	%reg_40 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_39
	%reg_41 = load i32, ptr %reg_40
	%reg_42 = load i32, ptr %reg_18
	%reg_43 = icmp ne i32 %reg_41, %reg_42
	br i1 %reg_43, label %Block8, label %Block7
Block6:  ;While body at line 27
	%reg_47 = load i32, ptr %reg_20
	%reg_48 = load i32, ptr %reg_22
	%reg_49 = add i32 %reg_47, %reg_48
	%reg_50 = add i32 2, 0
	%reg_51 = sdiv i32 %reg_49, %reg_50
	store i32 %reg_51, ptr %reg_24
	%reg_52 = load i32, ptr %reg_18
	%reg_53 = load i32, ptr %reg_24
	%reg_54 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_53
	%reg_55 = load i32, ptr %reg_54
	%reg_56 = icmp slt i32 %reg_52, %reg_55
	br i1 %reg_56, label %Block9, label %Block11
Block7:  ;While end at line 27
	%reg_63 = load i32, ptr %reg_18
	%reg_64 = load i32, ptr %reg_24
	%reg_65 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_64
	%reg_66 = load i32, ptr %reg_65
	%reg_67 = icmp eq i32 %reg_63, %reg_66
	br i1 %reg_67, label %Block12, label %Block14
Block8:  ;And opertor at line 27
	%reg_44 = load i32, ptr %reg_22
	%reg_45 = load i32, ptr %reg_20
	%reg_46 = icmp slt i32 %reg_44, %reg_45
	br i1 %reg_46, label %Block6, label %Block7
Block9:  ;If then at line 30
	%reg_57 = load i32, ptr %reg_24
	%reg_58 = add i32 1, 0
	%reg_59 = sub i32 %reg_57, %reg_58
	store i32 %reg_59, ptr %reg_20
	br label %Block10
Block10:  ;If end at line 30
	br label %Block5
Block11:  ;If else at line 30
	%reg_60 = load i32, ptr %reg_24
	%reg_61 = add i32 1, 0
	%reg_62 = add i32 %reg_60, %reg_61
	store i32 %reg_62, ptr %reg_22
	br label %Block10
Block12:  ;If then at line 39
	%reg_68 = load i32, ptr %reg_18
	call void @putint(i32 %reg_68)
	br label %Block13
Block13:  ;If end at line 39
	%reg_71 = add i32 10, 0
	store i32 %reg_71, ptr %reg_18
	%reg_72 = load i32, ptr %reg_18
	call void @putch(i32 %reg_72)
	%reg_73 = add i32 0, 0
	ret i32 %reg_73
Block14:  ;If else at line 39
	%reg_69 = add i32 0, 0
	store i32 %reg_69, ptr %reg_18
	%reg_70 = load i32, ptr %reg_18
	call void @putint(i32 %reg_70)
	br label %Block13
}
