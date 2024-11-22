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

@b = global i32 5
@c = global [4 x i32] [i32 6,i32 7,i32 8,i32 9]

define i32 @main()
{
Block0:  ;Func define at line 5
	%reg_41 = alloca [7 x [1 x [5 x i32]]]
	%reg_35 = alloca i32
	%reg_26 = alloca [2 x [8 x i32]]
	%reg_14 = alloca i32
	%reg_3 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 5
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = add i32 1, 0
	store i32 %reg_2, ptr %reg_0
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = add i32 2, 0
	store i32 %reg_5, ptr %reg_3
	%reg_6 = add i32 3, 0
	store i32 %reg_6, ptr %reg_3
	%reg_7 = load i32, ptr %reg_3
	call void @putint(i32 %reg_7)
	%reg_8 = load i32, ptr %reg_3
	call void @putint(i32 %reg_8)
	%reg_9 = load i32, ptr %reg_0
	call void @putint(i32 %reg_9)
	%reg_10 = add i32 10, 0
	call void @putch(i32 %reg_10)
	br label %Block2
Block2:  ;While condition at line 20
	%reg_11 = load i32, ptr %reg_0
	%reg_12 = add i32 5, 0
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block3, label %Block4
Block3:  ;While body at line 20
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_14
	%reg_16 = load i32, ptr %reg_14
	%reg_17 = add i32 1, 0
	%reg_18 = add i32 %reg_16, %reg_17
	store i32 %reg_18, ptr %reg_14
	%reg_19 = load i32, ptr %reg_14
	%reg_20 = icmp ne i32 %reg_19, 0
	br i1 %reg_20, label %Block5, label %Block6
Block4:  ;While end at line 20
	%reg_21 = load i32, ptr %reg_0
	call void @putint(i32 %reg_21)
	%reg_22 = add i32 10, 0
	call void @putch(i32 %reg_22)
	%reg_23 = add i32 2, 0
	%reg_24 = getelementptr [4 x i32], ptr @c, i32 0, i32 %reg_23
	%reg_25 = add i32 1, 0
	store i32 %reg_25, ptr %reg_24
	call void @llvm.memset.p0.i32(ptr %reg_26, i8 0, i32 64, i1 1)
	%reg_27 = add i32 0, 0
	%reg_28 = getelementptr [2 x [8 x i32]], ptr %reg_26, i32 0, i32 0, i32 0
	store i32 %reg_27, ptr %reg_28
	%reg_29 = add i32 9, 0
	%reg_30 = getelementptr [2 x [8 x i32]], ptr %reg_26, i32 0, i32 0, i32 1
	store i32 %reg_29, ptr %reg_30
	%reg_31 = add i32 8, 0
	%reg_32 = getelementptr [2 x [8 x i32]], ptr %reg_26, i32 0, i32 1, i32 0
	store i32 %reg_31, ptr %reg_32
	%reg_33 = add i32 3, 0
	%reg_34 = getelementptr [2 x [8 x i32]], ptr %reg_26, i32 0, i32 1, i32 1
	store i32 %reg_33, ptr %reg_34
	%reg_36 = add i32 2, 0
	store i32 %reg_36, ptr %reg_35
	%reg_37 = add i32 2, 0
	%reg_38 = getelementptr [4 x i32], ptr @c, i32 0, i32 %reg_37
	%reg_39 = load i32, ptr %reg_38
	%reg_40 = icmp ne i32 %reg_39, 0
	br i1 %reg_40, label %Block8, label %Block9
Block5:  ;If then at line 23
	br label %Block4
Block6:  ;If end at line 23
	br label %Block2
Block7:  ;Break at line 24
	br label %Block6
Block8:  ;If then at line 42
	call void @llvm.memset.p0.i32(ptr %reg_41, i8 0, i32 140, i1 1)
	%reg_42 = add i32 2, 0
	%reg_43 = getelementptr [7 x [1 x [5 x i32]]], ptr %reg_41, i32 0, i32 2, i32 0, i32 0
	store i32 %reg_42, ptr %reg_43
	%reg_44 = add i32 1, 0
	%reg_45 = getelementptr [7 x [1 x [5 x i32]]], ptr %reg_41, i32 0, i32 2, i32 0, i32 1
	store i32 %reg_44, ptr %reg_45
	%reg_46 = add i32 8, 0
	%reg_47 = getelementptr [7 x [1 x [5 x i32]]], ptr %reg_41, i32 0, i32 2, i32 0, i32 2
	store i32 %reg_46, ptr %reg_47
	%reg_48 = load i32, ptr %reg_35
	%reg_49 = add i32 0, 0
	%reg_50 = add i32 0, 0
	%reg_51 = getelementptr [7 x [1 x [5 x i32]]], ptr %reg_41, i32 0, i32 %reg_48, i32 %reg_49, i32 %reg_50
	%reg_52 = load i32, ptr %reg_51
	call void @putint(i32 %reg_52)
	%reg_53 = load i32, ptr %reg_35
	%reg_54 = add i32 0, 0
	%reg_55 = add i32 1, 0
	%reg_56 = getelementptr [7 x [1 x [5 x i32]]], ptr %reg_41, i32 0, i32 %reg_53, i32 %reg_54, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	call void @putint(i32 %reg_57)
	%reg_58 = load i32, ptr %reg_35
	%reg_59 = add i32 0, 0
	%reg_60 = add i32 2, 0
	%reg_61 = getelementptr [7 x [1 x [5 x i32]]], ptr %reg_41, i32 0, i32 %reg_58, i32 %reg_59, i32 %reg_60
	%reg_62 = load i32, ptr %reg_61
	call void @putint(i32 %reg_62)
	br label %Block9
Block9:  ;If end at line 42
	%reg_63 = add i32 10, 0
	call void @putch(i32 %reg_63)
	%reg_64 = load i32, ptr @b
	call void @putint(i32 %reg_64)
	%reg_65 = add i32 10, 0
	call void @putch(i32 %reg_65)
	%reg_66 = add i32 0, 0
	%reg_67 = getelementptr [4 x i32], ptr @c, i32 0, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	call void @putint(i32 %reg_68)
	%reg_69 = add i32 1, 0
	%reg_70 = getelementptr [4 x i32], ptr @c, i32 0, i32 %reg_69
	%reg_71 = load i32, ptr %reg_70
	call void @putint(i32 %reg_71)
	%reg_72 = add i32 2, 0
	%reg_73 = getelementptr [4 x i32], ptr @c, i32 0, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	call void @putint(i32 %reg_74)
	%reg_75 = add i32 3, 0
	%reg_76 = getelementptr [4 x i32], ptr @c, i32 0, i32 %reg_75
	%reg_77 = load i32, ptr %reg_76
	call void @putint(i32 %reg_77)
	%reg_78 = add i32 10, 0
	call void @putch(i32 %reg_78)
	%reg_79 = add i32 0, 0
	ret i32 %reg_79
}
