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


define i32 @canJump(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 1
	%reg_42 = alloca i32
	%reg_16 = alloca i32
	%reg_15 = alloca [10 x i32]
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 1
	%reg_3 = load i32, ptr %reg_2
	%reg_4 = add i32 1, 0
	%reg_5 = icmp eq i32 %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 2
	%reg_6 = add i32 1, 0
	ret i32 %reg_6
	br label %Block3
Block3:  ;If end at line 2
	%reg_7 = add i32 0, 0
	%reg_8 = getelementptr i32, ptr %reg_0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = add i32 2, 0
	%reg_12 = sub i32 %reg_10, %reg_11
	%reg_13 = icmp sgt i32 %reg_9, %reg_12
	br i1 %reg_13, label %Block4, label %Block5
Block4:  ;If then at line 4
	%reg_14 = add i32 1, 0
	ret i32 %reg_14
	br label %Block5
Block5:  ;If end at line 4
	call void @llvm.memset.p0.i32(ptr %reg_15, i8 0, i32 40, i1 1)
	%reg_17 = add i32 0, 0
	store i32 %reg_17, ptr %reg_16
	%reg_18 = add i32 0, 0
	store i32 %reg_18, ptr %reg_16
	br label %Block6
Block6:  ;While condition at line 9
	%reg_19 = load i32, ptr %reg_16
	%reg_20 = load i32, ptr %reg_2
	%reg_21 = add i32 1, 0
	%reg_22 = sub i32 %reg_20, %reg_21
	%reg_23 = icmp slt i32 %reg_19, %reg_22
	br i1 %reg_23, label %Block7, label %Block8
Block7:  ;While body at line 9
	%reg_24 = load i32, ptr %reg_16
	%reg_25 = getelementptr [10 x i32], ptr %reg_15, i32 0, i32 %reg_24
	%reg_26 = add i32 0, 0
	store i32 %reg_26, ptr %reg_25
	%reg_27 = load i32, ptr %reg_16
	%reg_28 = add i32 1, 0
	%reg_29 = add i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_16
	br label %Block6
Block8:  ;While end at line 9
	%reg_30 = load i32, ptr %reg_2
	%reg_31 = add i32 1, 0
	%reg_32 = sub i32 %reg_30, %reg_31
	%reg_33 = getelementptr [10 x i32], ptr %reg_15, i32 0, i32 %reg_32
	%reg_34 = add i32 1, 0
	store i32 %reg_34, ptr %reg_33
	%reg_35 = load i32, ptr %reg_2
	%reg_36 = add i32 2, 0
	%reg_37 = sub i32 %reg_35, %reg_36
	store i32 %reg_37, ptr %reg_16
	br label %Block9
Block9:  ;While condition at line 15
	%reg_38 = load i32, ptr %reg_16
	%reg_39 = add i32 1, 0
	%reg_40 = sub i32 0, %reg_39
	%reg_41 = icmp sgt i32 %reg_38, %reg_40
	br i1 %reg_41, label %Block10, label %Block11
Block10:  ;While body at line 15
	%reg_43 = add i32 0, 0
	store i32 %reg_43, ptr %reg_42
	%reg_44 = load i32, ptr %reg_16
	%reg_45 = getelementptr i32, ptr %reg_0, i32 %reg_44
	%reg_46 = load i32, ptr %reg_45
	%reg_47 = load i32, ptr %reg_2
	%reg_48 = add i32 1, 0
	%reg_49 = sub i32 %reg_47, %reg_48
	%reg_50 = load i32, ptr %reg_16
	%reg_51 = sub i32 %reg_49, %reg_50
	%reg_52 = icmp slt i32 %reg_46, %reg_51
	br i1 %reg_52, label %Block12, label %Block14
Block11:  ;While end at line 15
	%reg_81 = add i32 0, 0
	%reg_82 = getelementptr [10 x i32], ptr %reg_15, i32 0, i32 %reg_81
	%reg_83 = load i32, ptr %reg_82
	ret i32 %reg_83
Block12:  ;If then at line 17
	%reg_53 = load i32, ptr %reg_16
	%reg_54 = getelementptr i32, ptr %reg_0, i32 %reg_53
	%reg_55 = load i32, ptr %reg_54
	store i32 %reg_55, ptr %reg_42
	br label %Block13
Block13:  ;If end at line 17
	br label %Block15
Block14:  ;If else at line 17
	%reg_56 = load i32, ptr %reg_2
	%reg_57 = add i32 1, 0
	%reg_58 = sub i32 %reg_56, %reg_57
	%reg_59 = load i32, ptr %reg_16
	%reg_60 = sub i32 %reg_58, %reg_59
	store i32 %reg_60, ptr %reg_42
	br label %Block13
Block15:  ;While condition at line 22
	%reg_61 = load i32, ptr %reg_42
	%reg_62 = add i32 1, 0
	%reg_63 = sub i32 0, %reg_62
	%reg_64 = icmp sgt i32 %reg_61, %reg_63
	br i1 %reg_64, label %Block16, label %Block17
Block16:  ;While body at line 22
	%reg_65 = load i32, ptr %reg_16
	%reg_66 = load i32, ptr %reg_42
	%reg_67 = add i32 %reg_65, %reg_66
	%reg_68 = getelementptr [10 x i32], ptr %reg_15, i32 0, i32 %reg_67
	%reg_69 = load i32, ptr %reg_68
	%reg_70 = add i32 0, 0
	%reg_71 = icmp ne i32 %reg_69, %reg_70
	br i1 %reg_71, label %Block18, label %Block19
Block17:  ;While end at line 22
	%reg_78 = load i32, ptr %reg_16
	%reg_79 = add i32 1, 0
	%reg_80 = sub i32 %reg_78, %reg_79
	store i32 %reg_80, ptr %reg_16
	br label %Block9
Block18:  ;If then at line 23
	%reg_72 = load i32, ptr %reg_16
	%reg_73 = getelementptr [10 x i32], ptr %reg_15, i32 0, i32 %reg_72
	%reg_74 = add i32 1, 0
	store i32 %reg_74, ptr %reg_73
	br label %Block19
Block19:  ;If end at line 23
	%reg_75 = load i32, ptr %reg_42
	%reg_76 = add i32 1, 0
	%reg_77 = sub i32 %reg_75, %reg_76
	store i32 %reg_77, ptr %reg_42
	br label %Block15
}

define i32 @main()
{
Block0:  ;Func define at line 33
	%reg_2 = alloca [10 x i32]
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 33
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 40, i1 1)
	%reg_3 = add i32 0, 0
	%reg_4 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_3
	%reg_5 = add i32 3, 0
	store i32 %reg_5, ptr %reg_4
	%reg_6 = add i32 1, 0
	%reg_7 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_6
	%reg_8 = add i32 3, 0
	store i32 %reg_8, ptr %reg_7
	%reg_9 = add i32 2, 0
	%reg_10 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_9
	%reg_11 = add i32 9, 0
	store i32 %reg_11, ptr %reg_10
	%reg_12 = add i32 3, 0
	%reg_13 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_12
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_13
	%reg_15 = add i32 4, 0
	%reg_16 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_15
	%reg_17 = add i32 0, 0
	store i32 %reg_17, ptr %reg_16
	%reg_18 = add i32 5, 0
	%reg_19 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_18
	%reg_20 = add i32 1, 0
	store i32 %reg_20, ptr %reg_19
	%reg_21 = add i32 6, 0
	%reg_22 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_21
	%reg_23 = add i32 1, 0
	store i32 %reg_23, ptr %reg_22
	%reg_24 = add i32 7, 0
	%reg_25 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_24
	%reg_26 = add i32 5, 0
	store i32 %reg_26, ptr %reg_25
	%reg_27 = add i32 8, 0
	%reg_28 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_27
	%reg_29 = add i32 7, 0
	store i32 %reg_29, ptr %reg_28
	%reg_30 = add i32 9, 0
	%reg_31 = getelementptr [10 x i32], ptr %reg_2, i32 0, i32 %reg_30
	%reg_32 = add i32 8, 0
	store i32 %reg_32, ptr %reg_31
	%reg_33 = add i32 10, 0
	store i32 %reg_33, ptr %reg_0
	%reg_34 = getelementptr [10 x i32], ptr %reg_2, i32 0
	%reg_35 = load i32, ptr %reg_0
	%reg_36 = call i32 @canJump(ptr %reg_34, i32 %reg_35)
	store i32 %reg_36, ptr %reg_0
	%reg_37 = load i32, ptr %reg_0
	ret i32 %reg_37
}
