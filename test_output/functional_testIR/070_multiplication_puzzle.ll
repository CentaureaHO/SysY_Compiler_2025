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

@a = global [6 x i32] [i32 10,i32 1,i32 50,i32 50,i32 20,i32 5]
@dp = global [10 x [10 x i32]] zeroinitializer

define i32 @main()
{
Block0:  ;Func define at line 3
	%reg_11 = alloca i32
	%reg_9 = alloca i32
	%reg_7 = alloca i32
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 3
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = add i32 6, 0
	store i32 %reg_2, ptr %reg_0
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	%reg_12 = add i32 0, 0
	store i32 %reg_12, ptr %reg_11
	%reg_13 = add i32 3, 0
	store i32 %reg_13, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 10
	%reg_14 = load i32, ptr %reg_3
	%reg_15 = load i32, ptr %reg_0
	%reg_16 = icmp sle i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block3, label %Block4
Block3:  ;While body at line 10
	%reg_17 = add i32 0, 0
	store i32 %reg_17, ptr %reg_5
	br label %Block5
Block4:  ;While end at line 10
	%reg_81 = add i32 0, 0
	%reg_82 = load i32, ptr %reg_0
	%reg_83 = add i32 1, 0
	%reg_84 = sub i32 %reg_82, %reg_83
	%reg_85 = getelementptr [10 x [10 x i32]], ptr @dp, i32 0, i32 %reg_81, i32 %reg_84
	%reg_86 = load i32, ptr %reg_85
	call void @putint(i32 %reg_86)
	%reg_87 = add i32 0, 0
	ret i32 %reg_87
Block5:  ;While condition at line 13
	%reg_18 = load i32, ptr %reg_5
	%reg_19 = load i32, ptr %reg_0
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = sub i32 %reg_19, %reg_20
	%reg_22 = add i32 1, 0
	%reg_23 = add i32 %reg_21, %reg_22
	%reg_24 = icmp slt i32 %reg_18, %reg_23
	br i1 %reg_24, label %Block6, label %Block7
Block6:  ;While body at line 13
	%reg_25 = load i32, ptr %reg_5
	%reg_26 = load i32, ptr %reg_3
	%reg_27 = add i32 %reg_25, %reg_26
	%reg_28 = add i32 1, 0
	%reg_29 = sub i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_9
	%reg_30 = load i32, ptr %reg_5
	%reg_31 = add i32 1, 0
	%reg_32 = add i32 %reg_30, %reg_31
	store i32 %reg_32, ptr %reg_7
	br label %Block8
Block7:  ;While end at line 13
	%reg_78 = load i32, ptr %reg_3
	%reg_79 = add i32 1, 0
	%reg_80 = add i32 %reg_78, %reg_79
	store i32 %reg_80, ptr %reg_3
	br label %Block2
Block8:  ;While condition at line 17
	%reg_33 = load i32, ptr %reg_7
	%reg_34 = load i32, ptr %reg_9
	%reg_35 = icmp slt i32 %reg_33, %reg_34
	br i1 %reg_35, label %Block9, label %Block10
Block9:  ;While body at line 17
	%reg_36 = load i32, ptr %reg_5
	%reg_37 = load i32, ptr %reg_7
	%reg_38 = getelementptr [10 x [10 x i32]], ptr @dp, i32 0, i32 %reg_36, i32 %reg_37
	%reg_39 = load i32, ptr %reg_38
	%reg_40 = load i32, ptr %reg_7
	%reg_41 = load i32, ptr %reg_9
	%reg_42 = getelementptr [10 x [10 x i32]], ptr @dp, i32 0, i32 %reg_40, i32 %reg_41
	%reg_43 = load i32, ptr %reg_42
	%reg_44 = add i32 %reg_39, %reg_43
	%reg_45 = load i32, ptr %reg_5
	%reg_46 = getelementptr [6 x i32], ptr @a, i32 0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	%reg_48 = load i32, ptr %reg_7
	%reg_49 = getelementptr [6 x i32], ptr @a, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	%reg_51 = mul i32 %reg_47, %reg_50
	%reg_52 = load i32, ptr %reg_9
	%reg_53 = getelementptr [6 x i32], ptr @a, i32 0, i32 %reg_52
	%reg_54 = load i32, ptr %reg_53
	%reg_55 = mul i32 %reg_51, %reg_54
	%reg_56 = add i32 %reg_44, %reg_55
	store i32 %reg_56, ptr %reg_11
	%reg_57 = load i32, ptr %reg_5
	%reg_58 = load i32, ptr %reg_9
	%reg_59 = getelementptr [10 x [10 x i32]], ptr @dp, i32 0, i32 %reg_57, i32 %reg_58
	%reg_60 = load i32, ptr %reg_59
	%reg_61 = icmp eq i32 %reg_60, 0
	br i1 %reg_61, label %Block11, label %Block13
Block10:  ;While end at line 17
	%reg_75 = load i32, ptr %reg_5
	%reg_76 = add i32 1, 0
	%reg_77 = add i32 %reg_75, %reg_76
	store i32 %reg_77, ptr %reg_5
	br label %Block5
Block11:  ;If then at line 20
	%reg_68 = load i32, ptr %reg_5
	%reg_69 = load i32, ptr %reg_9
	%reg_70 = getelementptr [10 x [10 x i32]], ptr @dp, i32 0, i32 %reg_68, i32 %reg_69
	%reg_71 = load i32, ptr %reg_11
	store i32 %reg_71, ptr %reg_70
	br label %Block12
Block12:  ;If end at line 20
	%reg_72 = load i32, ptr %reg_7
	%reg_73 = add i32 1, 0
	%reg_74 = add i32 %reg_72, %reg_73
	store i32 %reg_74, ptr %reg_7
	br label %Block8
Block13:  ;Or opertor at line 20
	%reg_62 = load i32, ptr %reg_11
	%reg_63 = load i32, ptr %reg_5
	%reg_64 = load i32, ptr %reg_9
	%reg_65 = getelementptr [10 x [10 x i32]], ptr @dp, i32 0, i32 %reg_63, i32 %reg_64
	%reg_66 = load i32, ptr %reg_65
	%reg_67 = icmp slt i32 %reg_62, %reg_66
	br i1 %reg_67, label %Block11, label %Block12
}
