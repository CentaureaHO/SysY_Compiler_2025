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

@n = global i32 zeroinitializer

define i32 @gcd(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 3
	%reg_12 = alloca i32
	%reg_10 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 3
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_2
	store i32 %reg_8, ptr %reg_4
	%reg_9 = load i32, ptr %reg_3
	store i32 %reg_9, ptr %reg_6
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = load i32, ptr %reg_3
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block2, label %Block3
Block2:  ;If then at line 13
	%reg_17 = load i32, ptr %reg_2
	store i32 %reg_17, ptr %reg_10
	%reg_18 = load i32, ptr %reg_3
	store i32 %reg_18, ptr %reg_2
	%reg_19 = load i32, ptr %reg_10
	store i32 %reg_19, ptr %reg_3
	br label %Block3
Block3:  ;If end at line 13
	%reg_20 = load i32, ptr %reg_2
	%reg_21 = load i32, ptr %reg_3
	%reg_22 = srem i32 %reg_20, %reg_21
	store i32 %reg_22, ptr %reg_12
	br label %Block4
Block4:  ;While condition at line 17
	%reg_23 = load i32, ptr %reg_12
	%reg_24 = add i32 0, 0
	%reg_25 = icmp ne i32 %reg_23, %reg_24
	br i1 %reg_25, label %Block5, label %Block6
Block5:  ;While body at line 17
	%reg_26 = load i32, ptr %reg_3
	store i32 %reg_26, ptr %reg_2
	%reg_27 = load i32, ptr %reg_12
	store i32 %reg_27, ptr %reg_3
	%reg_28 = load i32, ptr %reg_2
	%reg_29 = load i32, ptr %reg_3
	%reg_30 = srem i32 %reg_28, %reg_29
	store i32 %reg_30, ptr %reg_12
	br label %Block4
Block6:  ;While end at line 17
	%reg_31 = load i32, ptr %reg_4
	%reg_32 = load i32, ptr %reg_6
	%reg_33 = mul i32 %reg_31, %reg_32
	%reg_34 = load i32, ptr %reg_3
	%reg_35 = sdiv i32 %reg_33, %reg_34
	ret i32 %reg_35
}

define i32 @main()
{
Block0:  ;Func define at line 24
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 24
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_4 = call i32 @getint()
	store i32 %reg_4, ptr %reg_0
	%reg_5 = call i32 @getint()
	store i32 %reg_5, ptr %reg_2
	%reg_6 = load i32, ptr %reg_0
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = call i32 @gcd(i32 %reg_6, i32 %reg_7)
	ret i32 %reg_8
}
