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

@TAPE_LEN = global i32 65536
@BUFFER_LEN = global i32 32768
@tape = global [65536 x i32] zeroinitializer
@program = global [32768 x i32] zeroinitializer
@ptr = global i32 0

define void @read_program()
{
Block0:  ;Func define at line 11
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 11
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = call i32 @getint()
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 13
	%reg_4 = load i32, ptr %reg_0
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 13
	%reg_7 = load i32, ptr %reg_0
	%reg_8 = getelementptr [32768 x i32], ptr @program, i32 0, i32 %reg_7
	%reg_9 = call i32 @getch()
	store i32 %reg_9, ptr %reg_8
	%reg_10 = load i32, ptr %reg_0
	%reg_11 = add i32 1, 0
	%reg_12 = add i32 %reg_10, %reg_11
	store i32 %reg_12, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 13
	%reg_13 = load i32, ptr %reg_0
	%reg_14 = getelementptr [32768 x i32], ptr @program, i32 0, i32 %reg_13
	%reg_15 = add i32 0, 0
	store i32 %reg_15, ptr %reg_14
	ret void
}

define void @interpret(ptr %reg_0)
{
Block0:  ;Func define at line 21
	%reg_5 = alloca i32
	%reg_3 = alloca i32
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 21
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	br label %Block2
Block2:  ;While condition at line 23
	%reg_7 = load i32, ptr %reg_5
	%reg_8 = getelementptr i32, ptr %reg_0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = icmp ne i32 %reg_9, 0
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 23
	%reg_11 = load i32, ptr %reg_5
	%reg_12 = getelementptr i32, ptr %reg_0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	store i32 %reg_13, ptr %reg_1
	%reg_14 = load i32, ptr %reg_1
	%reg_15 = add i32 62, 0
	%reg_16 = icmp eq i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block5, label %Block7
Block4:  ;While end at line 23
	ret void
Block5:  ;If then at line 25
	%reg_17 = load i32, ptr @ptr
	%reg_18 = add i32 1, 0
	%reg_19 = add i32 %reg_17, %reg_18
	store i32 %reg_19, ptr @ptr
	br label %Block6
Block6:  ;If end at line 25
	%reg_87 = load i32, ptr %reg_5
	%reg_88 = add i32 1, 0
	%reg_89 = add i32 %reg_87, %reg_88
	store i32 %reg_89, ptr %reg_5
	br label %Block2
Block7:  ;If else at line 25
	%reg_20 = load i32, ptr %reg_1
	%reg_21 = add i32 60, 0
	%reg_22 = icmp eq i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block8, label %Block10
Block8:  ;If then at line 29
	%reg_23 = load i32, ptr @ptr
	%reg_24 = add i32 1, 0
	%reg_25 = sub i32 %reg_23, %reg_24
	store i32 %reg_25, ptr @ptr
	br label %Block9
Block9:  ;If end at line 29
	br label %Block6
Block10:  ;If else at line 29
	%reg_26 = load i32, ptr %reg_1
	%reg_27 = add i32 43, 0
	%reg_28 = icmp eq i32 %reg_26, %reg_27
	br i1 %reg_28, label %Block11, label %Block13
Block11:  ;If then at line 33
	%reg_29 = load i32, ptr @ptr
	%reg_30 = getelementptr [65536 x i32], ptr @tape, i32 0, i32 %reg_29
	%reg_31 = load i32, ptr @ptr
	%reg_32 = getelementptr [65536 x i32], ptr @tape, i32 0, i32 %reg_31
	%reg_33 = load i32, ptr %reg_32
	%reg_34 = add i32 1, 0
	%reg_35 = add i32 %reg_33, %reg_34
	store i32 %reg_35, ptr %reg_30
	br label %Block12
Block12:  ;If end at line 33
	br label %Block9
Block13:  ;If else at line 33
	%reg_36 = load i32, ptr %reg_1
	%reg_37 = add i32 45, 0
	%reg_38 = icmp eq i32 %reg_36, %reg_37
	br i1 %reg_38, label %Block14, label %Block16
Block14:  ;If then at line 37
	%reg_39 = load i32, ptr @ptr
	%reg_40 = getelementptr [65536 x i32], ptr @tape, i32 0, i32 %reg_39
	%reg_41 = load i32, ptr @ptr
	%reg_42 = getelementptr [65536 x i32], ptr @tape, i32 0, i32 %reg_41
	%reg_43 = load i32, ptr %reg_42
	%reg_44 = add i32 1, 0
	%reg_45 = sub i32 %reg_43, %reg_44
	store i32 %reg_45, ptr %reg_40
	br label %Block15
Block15:  ;If end at line 37
	br label %Block12
Block16:  ;If else at line 37
	%reg_46 = load i32, ptr %reg_1
	%reg_47 = add i32 46, 0
	%reg_48 = icmp eq i32 %reg_46, %reg_47
	br i1 %reg_48, label %Block17, label %Block19
Block17:  ;If then at line 41
	%reg_49 = load i32, ptr @ptr
	%reg_50 = getelementptr [65536 x i32], ptr @tape, i32 0, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	call void @putch(i32 %reg_51)
	br label %Block18
Block18:  ;If end at line 41
	br label %Block15
Block19:  ;If else at line 41
	%reg_52 = load i32, ptr %reg_1
	%reg_53 = add i32 44, 0
	%reg_54 = icmp eq i32 %reg_52, %reg_53
	br i1 %reg_54, label %Block20, label %Block22
Block20:  ;If then at line 45
	%reg_55 = load i32, ptr @ptr
	%reg_56 = getelementptr [65536 x i32], ptr @tape, i32 0, i32 %reg_55
	%reg_57 = call i32 @getch()
	store i32 %reg_57, ptr %reg_56
	br label %Block21
Block21:  ;If end at line 45
	br label %Block18
Block22:  ;If else at line 45
	%reg_58 = load i32, ptr %reg_1
	%reg_59 = add i32 93, 0
	%reg_60 = icmp eq i32 %reg_58, %reg_59
	br i1 %reg_60, label %Block25, label %Block24
Block23:  ;If then at line 49
	%reg_65 = add i32 1, 0
	store i32 %reg_65, ptr %reg_3
	br label %Block26
Block24:  ;If end at line 49
	br label %Block21
Block25:  ;And opertor at line 49
	%reg_61 = load i32, ptr @ptr
	%reg_62 = getelementptr [65536 x i32], ptr @tape, i32 0, i32 %reg_61
	%reg_63 = load i32, ptr %reg_62
	%reg_64 = icmp ne i32 %reg_63, 0
	br i1 %reg_64, label %Block23, label %Block24
Block26:  ;While condition at line 52
	%reg_66 = load i32, ptr %reg_3
	%reg_67 = add i32 0, 0
	%reg_68 = icmp sgt i32 %reg_66, %reg_67
	br i1 %reg_68, label %Block27, label %Block28
Block27:  ;While body at line 52
	%reg_69 = load i32, ptr %reg_5
	%reg_70 = add i32 1, 0
	%reg_71 = sub i32 %reg_69, %reg_70
	store i32 %reg_71, ptr %reg_5
	%reg_72 = load i32, ptr %reg_5
	%reg_73 = getelementptr i32, ptr %reg_0, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	store i32 %reg_74, ptr %reg_1
	%reg_75 = load i32, ptr %reg_1
	%reg_76 = add i32 91, 0
	%reg_77 = icmp eq i32 %reg_75, %reg_76
	br i1 %reg_77, label %Block29, label %Block31
Block28:  ;While end at line 52
	br label %Block24
Block29:  ;If then at line 55
	%reg_78 = load i32, ptr %reg_3
	%reg_79 = add i32 1, 0
	%reg_80 = sub i32 %reg_78, %reg_79
	store i32 %reg_80, ptr %reg_3
	br label %Block30
Block30:  ;If end at line 55
	br label %Block26
Block31:  ;If else at line 55
	%reg_81 = load i32, ptr %reg_1
	%reg_82 = add i32 93, 0
	%reg_83 = icmp eq i32 %reg_81, %reg_82
	br i1 %reg_83, label %Block32, label %Block33
Block32:  ;If then at line 59
	%reg_84 = load i32, ptr %reg_3
	%reg_85 = add i32 1, 0
	%reg_86 = add i32 %reg_84, %reg_85
	store i32 %reg_86, ptr %reg_3
	br label %Block33
Block33:  ;If end at line 59
	br label %Block30
}

define i32 @main()
{
Block0:  ;Func define at line 69
	br label %Block1
Block1:  ;Func body at line 69
	call void @read_program()
	%reg_0 = getelementptr [32768 x i32], ptr @program, i32 0
	call void @interpret(ptr %reg_0)
	%reg_1 = add i32 0, 0
	ret i32 %reg_1
}
