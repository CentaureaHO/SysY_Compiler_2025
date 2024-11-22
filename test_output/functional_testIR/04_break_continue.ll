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
Block0:  ;Func define at line 1
	%reg_39 = alloca i32
	%reg_17 = alloca i32
	%reg_12 = alloca i32
	%reg_7 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 1
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 4
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = add i32 20, 0
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 4
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	br label %Block5
Block4:  ;While end at line 4
	%reg_72 = load i32, ptr %reg_0
	ret i32 %reg_72
Block5:  ;While condition at line 6
	%reg_9 = load i32, ptr %reg_7
	%reg_10 = add i32 10, 0
	%reg_11 = icmp slt i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block6, label %Block7
Block6:  ;While body at line 6
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	br label %Block8
Block7:  ;While end at line 6
	%reg_69 = load i32, ptr %reg_2
	%reg_70 = add i32 1, 0
	%reg_71 = add i32 %reg_69, %reg_70
	store i32 %reg_71, ptr %reg_2
	br label %Block2
Block8:  ;While condition at line 8
	%reg_14 = load i32, ptr %reg_12
	%reg_15 = add i32 5, 0
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block9, label %Block10
Block9:  ;While body at line 8
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_17
	br label %Block11
Block10:  ;While end at line 8
	%reg_63 = load i32, ptr %reg_7
	%reg_64 = add i32 1, 0
	%reg_65 = add i32 %reg_63, %reg_64
	store i32 %reg_65, ptr %reg_7
	br label %Block5
Block11:  ;While condition at line 10
	%reg_19 = load i32, ptr %reg_17
	%reg_20 = add i32 3, 0
	%reg_21 = icmp slt i32 %reg_19, %reg_20
	br i1 %reg_21, label %Block12, label %Block13
Block12:  ;While body at line 10
	%reg_22 = load i32, ptr %reg_17
	%reg_23 = add i32 1, 0
	%reg_24 = add i32 %reg_22, %reg_23
	%reg_25 = add i32 3, 0
	%reg_26 = icmp sge i32 %reg_24, %reg_25
	br i1 %reg_26, label %Block14, label %Block15
Block13:  ;While end at line 10
	br label %Block30
Block14:  ;If then at line 11
	%reg_27 = load i32, ptr %reg_17
	%reg_28 = icmp ne i32 %reg_27, 0
	br i1 %reg_28, label %Block16, label %Block17
Block15:  ;If end at line 11
	%reg_40 = add i32 0, 0
	store i32 %reg_40, ptr %reg_39
	br label %Block25
Block16:  ;If then at line 12
	%reg_29 = load i32, ptr %reg_17
	%reg_30 = icmp ne i32 %reg_29, 0
	br i1 %reg_30, label %Block18, label %Block20
Block17:  ;If end at line 12
	br label %Block15
Block18:  ;If then at line 13
	%reg_33 = load i32, ptr %reg_17
	%reg_34 = add i32 1, 0
	%reg_35 = sub i32 0, %reg_34
	%reg_36 = sub i32 %reg_33, %reg_35
	%reg_37 = add i32 3, 0
	%reg_38 = icmp sge i32 %reg_36, %reg_37
	br i1 %reg_38, label %Block21, label %Block22
Block19:  ;If end at line 13
	br label %Block17
Block20:  ;Or opertor at line 13
	%reg_31 = load i32, ptr %reg_17
	%reg_32 = icmp eq i32 %reg_31, 0
	br i1 %reg_32, label %Block18, label %Block19
Block21:  ;If then at line 14
	br label %Block13
Block22:  ;If end at line 14
	br label %Block19
Block23:  ;Break at line 15
	br label %Block11
Block24:  ;Continue at line 15
	br label %Block22
Block25:  ;While condition at line 17
	%reg_41 = load i32, ptr %reg_39
	%reg_42 = add i32 2, 0
	%reg_43 = icmp slt i32 %reg_41, %reg_42
	br i1 %reg_43, label %Block26, label %Block27
Block26:  ;While body at line 17
	%reg_44 = load i32, ptr %reg_39
	%reg_45 = add i32 1, 0
	%reg_46 = add i32 %reg_44, %reg_45
	store i32 %reg_46, ptr %reg_39
	br label %Block25
Block27:  ;While end at line 17
	%reg_50 = load i32, ptr %reg_17
	%reg_51 = add i32 1, 0
	%reg_52 = add i32 %reg_50, %reg_51
	store i32 %reg_52, ptr %reg_17
	%reg_53 = load i32, ptr %reg_0
	%reg_54 = add i32 1, 0
	%reg_55 = add i32 %reg_53, %reg_54
	store i32 %reg_55, ptr %reg_0
	br label %Block11
Block28:  ;Continue at line 19
	br label %Block27
Block29:  ;Break at line 20
	%reg_47 = load i32, ptr %reg_0
	%reg_48 = add i32 1, 0
	%reg_49 = add i32 %reg_47, %reg_48
	store i32 %reg_49, ptr %reg_0
	br label %Block25
Block30:  ;While condition at line 26
	%reg_56 = add i32 1, 0
	%reg_57 = icmp ne i32 %reg_56, 0
	br i1 %reg_57, label %Block31, label %Block32
Block31:  ;While body at line 26
	br label %Block33
Block32:  ;While end at line 26
	%reg_60 = load i32, ptr %reg_12
	%reg_61 = add i32 1, 0
	%reg_62 = add i32 %reg_60, %reg_61
	store i32 %reg_62, ptr %reg_12
	br label %Block8
Block33:  ;While condition at line 27
	%reg_58 = add i32 1, 0
	%reg_59 = icmp ne i32 %reg_58, 0
	br i1 %reg_59, label %Block34, label %Block35
Block34:  ;While body at line 27
	br label %Block35
Block35:  ;While end at line 27
	br label %Block32
Block36:  ;Break at line 27
	br label %Block33
Block37:  ;Break at line 28
	br label %Block30
Block38:  ;Continue at line 33
	%reg_66 = load i32, ptr %reg_7
	%reg_67 = add i32 1, 0
	%reg_68 = add i32 %reg_66, %reg_67
	store i32 %reg_68, ptr %reg_7
	br label %Block5
}
