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

@TOKEN_NUM = global i32 0
@TOKEN_OTHER = global i32 1
@last_char = global i32 32
@num = global i32 zeroinitializer
@other = global i32 zeroinitializer
@cur_token = global i32 zeroinitializer

define i32 @next_char()
{
Block0:  ;Func define at line 5
	br label %Block1
Block1:  ;Func body at line 5
	%reg_0 = call i32 @getch()
	store i32 %reg_0, ptr @last_char
	%reg_1 = load i32, ptr @last_char
	ret i32 %reg_1
}

define i32 @is_space(i32 %reg_0)
{
Block0:  ;Func define at line 10
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 10
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 32, 0
	%reg_4 = icmp eq i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block5
Block2:  ;If then at line 11
	%reg_8 = add i32 1, 0
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 11
	ret i32 0
Block4:  ;If else at line 11
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
	br label %Block3
Block5:  ;Or opertor at line 11
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 10, 0
	%reg_7 = icmp eq i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block4
}

define i32 @is_num(i32 %reg_0)
{
Block0:  ;Func define at line 19
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 19
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 48, 0
	%reg_4 = icmp sge i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block5, label %Block4
Block2:  ;If then at line 20
	%reg_8 = add i32 1, 0
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 20
	ret i32 0
Block4:  ;If else at line 20
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
	br label %Block3
Block5:  ;And opertor at line 20
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 57, 0
	%reg_7 = icmp sle i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block4
}

define i32 @next_token()
{
Block0:  ;Func define at line 28
	br label %Block1
Block1:  ;Func body at line 28
	br label %Block2
Block2:  ;While condition at line 29
	%reg_0 = load i32, ptr @last_char
	%reg_1 = call i32 @is_space(i32 %reg_0)
	%reg_2 = icmp ne i32 %reg_1, 0
	br i1 %reg_2, label %Block3, label %Block4
Block3:  ;While body at line 29
	%reg_3 = call i32 @next_char()
	br label %Block2
Block4:  ;While end at line 29
	%reg_4 = load i32, ptr @last_char
	%reg_5 = call i32 @is_num(i32 %reg_4)
	%reg_6 = icmp ne i32 %reg_5, 0
	br i1 %reg_6, label %Block5, label %Block7
Block5:  ;If then at line 30
	%reg_7 = load i32, ptr @last_char
	%reg_8 = add i32 48, 0
	%reg_9 = sub i32 %reg_7, %reg_8
	store i32 %reg_9, ptr @num
	br label %Block8
Block6:  ;If end at line 30
	%reg_24 = load i32, ptr @cur_token
	ret i32 %reg_24
Block7:  ;If else at line 30
	%reg_21 = load i32, ptr @last_char
	store i32 %reg_21, ptr @other
	%reg_22 = call i32 @next_char()
	%reg_23 = load i32, ptr @TOKEN_OTHER
	store i32 %reg_23, ptr @cur_token
	br label %Block6
Block8:  ;While condition at line 32
	%reg_10 = call i32 @next_char()
	%reg_11 = call i32 @is_num(i32 %reg_10)
	%reg_12 = icmp ne i32 %reg_11, 0
	br i1 %reg_12, label %Block9, label %Block10
Block9:  ;While body at line 32
	%reg_13 = load i32, ptr @num
	%reg_14 = add i32 10, 0
	%reg_15 = mul i32 %reg_13, %reg_14
	%reg_16 = load i32, ptr @last_char
	%reg_17 = add i32 %reg_15, %reg_16
	%reg_18 = add i32 48, 0
	%reg_19 = sub i32 %reg_17, %reg_18
	store i32 %reg_19, ptr @num
	br label %Block8
Block10:  ;While end at line 32
	%reg_20 = load i32, ptr @TOKEN_NUM
	store i32 %reg_20, ptr @cur_token
	br label %Block6
}

define i32 @panic()
{
Block0:  ;Func define at line 45
	br label %Block1
Block1:  ;Func body at line 45
	%reg_0 = add i32 112, 0
	call void @putch(i32 %reg_0)
	%reg_1 = add i32 97, 0
	call void @putch(i32 %reg_1)
	%reg_2 = add i32 110, 0
	call void @putch(i32 %reg_2)
	%reg_3 = add i32 105, 0
	call void @putch(i32 %reg_3)
	%reg_4 = add i32 99, 0
	call void @putch(i32 %reg_4)
	%reg_5 = add i32 33, 0
	call void @putch(i32 %reg_5)
	%reg_6 = add i32 10, 0
	call void @putch(i32 %reg_6)
	%reg_7 = add i32 1, 0
	%reg_8 = sub i32 0, %reg_7
	ret i32 %reg_8
}

define i32 @get_op_prec(i32 %reg_0)
{
Block0:  ;Func define at line 56
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 56
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 43, 0
	%reg_4 = icmp eq i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block4
Block2:  ;If then at line 58
	%reg_8 = add i32 10, 0
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 58
	%reg_9 = load i32, ptr %reg_1
	%reg_10 = add i32 42, 0
	%reg_11 = icmp eq i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block5, label %Block8
Block4:  ;Or opertor at line 58
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 45, 0
	%reg_7 = icmp eq i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
Block5:  ;If then at line 60
	%reg_18 = add i32 20, 0
	ret i32 %reg_18
	br label %Block6
Block6:  ;If end at line 60
	%reg_19 = add i32 0, 0
	ret i32 %reg_19
Block7:  ;Or opertor at line 60
	%reg_15 = load i32, ptr %reg_1
	%reg_16 = add i32 37, 0
	%reg_17 = icmp eq i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block5, label %Block6
Block8:  ;Or opertor at line -1
	%reg_12 = load i32, ptr %reg_1
	%reg_13 = add i32 47, 0
	%reg_14 = icmp eq i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block5, label %Block7
}

define void @stack_push(ptr %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 65
	%reg_2 = alloca i32
	store i32 %reg_1, ptr %reg_2
	br label %Block1
Block1:  ;Func body at line 65
	%reg_3 = add i32 0, 0
	%reg_4 = getelementptr i32, ptr %reg_0, i32 %reg_3
	%reg_5 = add i32 0, 0
	%reg_6 = getelementptr i32, ptr %reg_0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	%reg_8 = add i32 1, 0
	%reg_9 = add i32 %reg_7, %reg_8
	store i32 %reg_9, ptr %reg_4
	%reg_10 = add i32 0, 0
	%reg_11 = getelementptr i32, ptr %reg_0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	%reg_13 = getelementptr i32, ptr %reg_0, i32 %reg_12
	%reg_14 = load i32, ptr %reg_2
	store i32 %reg_14, ptr %reg_13
	ret void
}

define i32 @stack_pop(ptr %reg_0)
{
Block0:  ;Func define at line 70
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 70
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr i32, ptr %reg_0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = getelementptr i32, ptr %reg_0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_5
	store i32 %reg_6, ptr %reg_1
	%reg_7 = add i32 0, 0
	%reg_8 = getelementptr i32, ptr %reg_0, i32 %reg_7
	%reg_9 = add i32 0, 0
	%reg_10 = getelementptr i32, ptr %reg_0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = add i32 1, 0
	%reg_13 = sub i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_8
	%reg_14 = load i32, ptr %reg_1
	ret i32 %reg_14
}

define i32 @stack_peek(ptr %reg_0)
{
Block0:  ;Func define at line 76
	br label %Block1
Block1:  ;Func body at line 76
	%reg_1 = add i32 0, 0
	%reg_2 = getelementptr i32, ptr %reg_0, i32 %reg_1
	%reg_3 = load i32, ptr %reg_2
	%reg_4 = getelementptr i32, ptr %reg_0, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	ret i32 %reg_5
}

define i32 @stack_size(ptr %reg_0)
{
Block0:  ;Func define at line 80
	br label %Block1
Block1:  ;Func body at line 80
	%reg_1 = add i32 0, 0
	%reg_2 = getelementptr i32, ptr %reg_0, i32 %reg_1
	%reg_3 = load i32, ptr %reg_2
	ret i32 %reg_3
}

define i32 @eval_op(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 84
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 84
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = add i32 43, 0
	%reg_8 = icmp eq i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block2:  ;If then at line 86
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = load i32, ptr %reg_5
	%reg_11 = add i32 %reg_9, %reg_10
	ret i32 %reg_11
	br label %Block3
Block3:  ;If end at line 86
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = add i32 45, 0
	%reg_14 = icmp eq i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block4, label %Block5
Block4:  ;If then at line 88
	%reg_15 = load i32, ptr %reg_4
	%reg_16 = load i32, ptr %reg_5
	%reg_17 = sub i32 %reg_15, %reg_16
	ret i32 %reg_17
	br label %Block5
Block5:  ;If end at line 88
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = add i32 42, 0
	%reg_20 = icmp eq i32 %reg_18, %reg_19
	br i1 %reg_20, label %Block6, label %Block7
Block6:  ;If then at line 90
	%reg_21 = load i32, ptr %reg_4
	%reg_22 = load i32, ptr %reg_5
	%reg_23 = mul i32 %reg_21, %reg_22
	ret i32 %reg_23
	br label %Block7
Block7:  ;If end at line 90
	%reg_24 = load i32, ptr %reg_3
	%reg_25 = add i32 47, 0
	%reg_26 = icmp eq i32 %reg_24, %reg_25
	br i1 %reg_26, label %Block8, label %Block9
Block8:  ;If then at line 92
	%reg_27 = load i32, ptr %reg_4
	%reg_28 = load i32, ptr %reg_5
	%reg_29 = sdiv i32 %reg_27, %reg_28
	ret i32 %reg_29
	br label %Block9
Block9:  ;If end at line 92
	%reg_30 = load i32, ptr %reg_3
	%reg_31 = add i32 37, 0
	%reg_32 = icmp eq i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block10, label %Block11
Block10:  ;If then at line 94
	%reg_33 = load i32, ptr %reg_4
	%reg_34 = load i32, ptr %reg_5
	%reg_35 = srem i32 %reg_33, %reg_34
	ret i32 %reg_35
	br label %Block11
Block11:  ;If end at line 94
	%reg_36 = add i32 0, 0
	ret i32 %reg_36
}

define i32 @eval()
{
Block0:  ;Func define at line 99
	%reg_60 = alloca i32
	%reg_57 = alloca i32
	%reg_54 = alloca i32
	%reg_33 = alloca i32
	%reg_30 = alloca i32
	%reg_27 = alloca i32
	%reg_12 = alloca i32
	%reg_1 = alloca [256 x i32]
	%reg_0 = alloca [256 x i32]
	br label %Block1
Block1:  ;Func body at line 99
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 1024, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 1024, i1 1)
	%reg_2 = load i32, ptr @cur_token
	%reg_3 = load i32, ptr @TOKEN_NUM
	%reg_4 = icmp ne i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block3
Block2:  ;If then at line 102
	%reg_5 = call i32 @panic()
	ret i32 %reg_5
	br label %Block3
Block3:  ;If end at line 102
	%reg_6 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_7 = load i32, ptr @num
	call void @stack_push(ptr %reg_6, i32 %reg_7)
	%reg_8 = call i32 @next_token()
	br label %Block4
Block4:  ;While condition at line 106
	%reg_9 = load i32, ptr @cur_token
	%reg_10 = load i32, ptr @TOKEN_OTHER
	%reg_11 = icmp eq i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block5, label %Block6
Block5:  ;While body at line 106
	%reg_13 = load i32, ptr @other
	store i32 %reg_13, ptr %reg_12
	%reg_14 = load i32, ptr %reg_12
	%reg_15 = call i32 @get_op_prec(i32 %reg_14)
	%reg_16 = icmp eq i32 %reg_15, 0
	br i1 %reg_16, label %Block7, label %Block8
Block6:  ;While end at line 106
	%reg_50 = call i32 @next_token()
	br label %Block16
Block7:  ;If then at line 109
	br label %Block6
Block8:  ;If end at line 109
	%reg_17 = call i32 @next_token()
	br label %Block10
Block9:  ;Break at line 109
	br label %Block8
Block10:  ;While condition at line 112
	%reg_18 = getelementptr [256 x i32], ptr %reg_1, i32 0
	%reg_19 = call i32 @stack_size(ptr %reg_18)
	%reg_20 = icmp ne i32 %reg_19, 0
	br i1 %reg_20, label %Block13, label %Block12
Block11:  ;While body at line 112
	%reg_28 = getelementptr [256 x i32], ptr %reg_1, i32 0
	%reg_29 = call i32 @stack_pop(ptr %reg_28)
	store i32 %reg_29, ptr %reg_27
	%reg_31 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_32 = call i32 @stack_pop(ptr %reg_31)
	store i32 %reg_32, ptr %reg_30
	%reg_34 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_35 = call i32 @stack_pop(ptr %reg_34)
	store i32 %reg_35, ptr %reg_33
	%reg_36 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_37 = load i32, ptr %reg_27
	%reg_38 = load i32, ptr %reg_33
	%reg_39 = load i32, ptr %reg_30
	%reg_40 = call i32 @eval_op(i32 %reg_37, i32 %reg_38, i32 %reg_39)
	call void @stack_push(ptr %reg_36, i32 %reg_40)
	br label %Block10
Block12:  ;While end at line 112
	%reg_41 = getelementptr [256 x i32], ptr %reg_1, i32 0
	%reg_42 = load i32, ptr %reg_12
	call void @stack_push(ptr %reg_41, i32 %reg_42)
	%reg_43 = load i32, ptr @cur_token
	%reg_44 = load i32, ptr @TOKEN_NUM
	%reg_45 = icmp ne i32 %reg_43, %reg_44
	br i1 %reg_45, label %Block14, label %Block15
Block13:  ;And opertor at line 112
	%reg_21 = getelementptr [256 x i32], ptr %reg_1, i32 0
	%reg_22 = call i32 @stack_peek(ptr %reg_21)
	%reg_23 = call i32 @get_op_prec(i32 %reg_22)
	%reg_24 = load i32, ptr %reg_12
	%reg_25 = call i32 @get_op_prec(i32 %reg_24)
	%reg_26 = icmp sge i32 %reg_23, %reg_25
	br i1 %reg_26, label %Block11, label %Block12
Block14:  ;If then at line 120
	%reg_46 = call i32 @panic()
	ret i32 %reg_46
	br label %Block15
Block15:  ;If end at line 120
	%reg_47 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_48 = load i32, ptr @num
	call void @stack_push(ptr %reg_47, i32 %reg_48)
	%reg_49 = call i32 @next_token()
	br label %Block4
Block16:  ;While condition at line 127
	%reg_51 = getelementptr [256 x i32], ptr %reg_1, i32 0
	%reg_52 = call i32 @stack_size(ptr %reg_51)
	%reg_53 = icmp ne i32 %reg_52, 0
	br i1 %reg_53, label %Block17, label %Block18
Block17:  ;While body at line 127
	%reg_55 = getelementptr [256 x i32], ptr %reg_1, i32 0
	%reg_56 = call i32 @stack_pop(ptr %reg_55)
	store i32 %reg_56, ptr %reg_54
	%reg_58 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_59 = call i32 @stack_pop(ptr %reg_58)
	store i32 %reg_59, ptr %reg_57
	%reg_61 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_62 = call i32 @stack_pop(ptr %reg_61)
	store i32 %reg_62, ptr %reg_60
	%reg_63 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_64 = load i32, ptr %reg_54
	%reg_65 = load i32, ptr %reg_60
	%reg_66 = load i32, ptr %reg_57
	%reg_67 = call i32 @eval_op(i32 %reg_64, i32 %reg_65, i32 %reg_66)
	call void @stack_push(ptr %reg_63, i32 %reg_67)
	br label %Block16
Block18:  ;While end at line 127
	%reg_68 = getelementptr [256 x i32], ptr %reg_0, i32 0
	%reg_69 = call i32 @stack_peek(ptr %reg_68)
	ret i32 %reg_69
}

define i32 @main()
{
Block0:  ;Func define at line 135
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 135
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	%reg_2 = call i32 @getch()
	%reg_3 = call i32 @next_token()
	br label %Block2
Block2:  ;While condition at line 139
	%reg_4 = load i32, ptr %reg_0
	%reg_5 = icmp ne i32 %reg_4, 0
	br i1 %reg_5, label %Block3, label %Block4
Block3:  ;While body at line 139
	%reg_6 = call i32 @eval()
	call void @putint(i32 %reg_6)
	%reg_7 = add i32 10, 0
	call void @putch(i32 %reg_7)
	%reg_8 = load i32, ptr %reg_0
	%reg_9 = add i32 1, 0
	%reg_10 = sub i32 %reg_8, %reg_9
	store i32 %reg_10, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 139
	%reg_11 = add i32 0, 0
	ret i32 %reg_11
}
