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
	%reg_43 = alloca i32
	%reg_39 = alloca i32
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
	%reg_40 = add i32 0, 0
	store i32 %reg_40, ptr %reg_39
	%reg_41 = add i32 0, 0
	store i32 %reg_41, ptr %reg_39
	%reg_42 = add i32 0, 0
	store i32 %reg_42, ptr %reg_0
	%reg_44 = add i32 0, 0
	store i32 %reg_44, ptr %reg_43
	%reg_45 = add i32 0, 0
	store i32 %reg_45, ptr %reg_43
	br label %Block5
Block5:  ;While condition at line 33
	%reg_46 = load i32, ptr %reg_0
	%reg_47 = add i32 10, 0
	%reg_48 = icmp slt i32 %reg_46, %reg_47
	br i1 %reg_48, label %Block8, label %Block7
Block6:  ;While body at line 33
	%reg_52 = load i32, ptr %reg_0
	%reg_53 = getelementptr [10 x i32], ptr %reg_4, i32 0, i32 %reg_52
	%reg_54 = load i32, ptr %reg_53
	%reg_55 = load i32, ptr %reg_18
	%reg_56 = icmp eq i32 %reg_54, %reg_55
	br i1 %reg_56, label %Block9, label %Block10
Block7:  ;While end at line 33
	%reg_62 = load i32, ptr %reg_39
	%reg_63 = add i32 1, 0
	%reg_64 = icmp eq i32 %reg_62, %reg_63
	br i1 %reg_64, label %Block11, label %Block13
Block8:  ;And opertor at line 33
	%reg_49 = load i32, ptr %reg_39
	%reg_50 = add i32 0, 0
	%reg_51 = icmp eq i32 %reg_49, %reg_50
	br i1 %reg_51, label %Block6, label %Block7
Block9:  ;If then at line 35
	%reg_57 = add i32 1, 0
	store i32 %reg_57, ptr %reg_39
	%reg_58 = load i32, ptr %reg_0
	store i32 %reg_58, ptr %reg_43
	br label %Block10
Block10:  ;If end at line 35
	%reg_59 = load i32, ptr %reg_0
	%reg_60 = add i32 1, 0
	%reg_61 = add i32 %reg_59, %reg_60
	store i32 %reg_61, ptr %reg_0
	br label %Block5
Block11:  ;If then at line 45
	%reg_65 = load i32, ptr %reg_43
	call void @putint(i32 %reg_65)
	br label %Block12
Block12:  ;If end at line 45
	%reg_68 = add i32 10, 0
	store i32 %reg_68, ptr %reg_18
	%reg_69 = load i32, ptr %reg_18
	call void @putch(i32 %reg_69)
	%reg_70 = add i32 0, 0
	ret i32 %reg_70
Block13:  ;If else at line 45
	%reg_66 = add i32 0, 0
	store i32 %reg_66, ptr %reg_18
	%reg_67 = load i32, ptr %reg_18
	call void @putint(i32 %reg_67)
	br label %Block12
}
