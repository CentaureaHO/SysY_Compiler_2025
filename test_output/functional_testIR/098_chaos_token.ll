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

@__HELLO = global [100 x i32] [i32 87,i32 101,i32 108,i32 99,i32 111,i32 109,i32 101,i32 32,i32 116,i32 111,i32 32,i32 116,i32 104,i32 101,i32 32,i32 74,i32 97,i32 112,i32 97,i32 114,i32 105,i32 32,i32 80,i32 97,i32 114,i32 107,i32 33,i32 10,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0]
@N4__mE___ = global [6 x [50 x i32]] [[50 x i32] [i32 83,i32 97,i32 97,i32 98,i32 97,i32 114,i32 117,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[50 x i32] [i32 75,i32 97,i32 98,i32 97,i32 110,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[50 x i32] [i32 72,i32 97,i32 115,i32 104,i32 105,i32 98,i32 105,i32 114,i32 111,i32 107,i32 111,i32 117,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[50 x i32] [i32 65,i32 114,i32 97,i32 105,i32 103,i32 117,i32 109,i32 97,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[50 x i32] [i32 72,i32 117,i32 110,i32 98,i32 111,i32 114,i32 117,i32 116,i32 111,i32 32,i32 80,i32 101,i32 110,i32 103,i32 105,i32 110,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[50 x i32] [i32 84,i32 97,i32 105,i32 114,i32 105,i32 107,i32 117,i32 32,i32 79,i32 111,i32 107,i32 97,i32 109,i32 105,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0]]
@saY_HeI10_To = global [40 x i32] [i32 32,i32 115,i32 97,i32 121,i32 115,i32 32,i32 104,i32 101,i32 108,i32 108,i32 111,i32 32,i32 116,i32 111,i32 32,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0]
@RET = global [5 x i32] [i32 10,i32 0,i32 0,i32 0,i32 0]

define i32 @putstr(ptr %reg_0)
{
Block0:  ;Func define at line 103
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 103
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_1
	br label %Block2
Block2:  ;While condition at line 107
	%reg_4 = load i32, ptr %reg_1
	%reg_5 = getelementptr i32, ptr %reg_0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 107
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = getelementptr i32, ptr %reg_0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	call void @putch(i32 %reg_10)
	%reg_11 = load i32, ptr %reg_1
	%reg_12 = add i32 1, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_1
	br label %Block2
Block4:  ;While end at line 107
	%reg_14 = load i32, ptr %reg_1
	ret i32 %reg_14
}

define i32 @main()
{
Block0:  ;Func define at line 122
	%reg_10 = alloca i32
	%reg_6 = alloca i32
	%reg_2 = alloca i32
	br label %Block1
Block1:  ;Func body at line 122
	%reg_0 = getelementptr [100 x i32], ptr @__HELLO, i32 0
	%reg_1 = call i32 @putstr(ptr %reg_0)
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 130
	%reg_4 = add i32 1, 0
	%reg_5 = icmp ne i32 %reg_4, 0
	br i1 %reg_5, label %Block3, label %Block4
Block3:  ;While body at line 130
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = add i32 6, 0
	%reg_9 = sdiv i32 %reg_7, %reg_8
	store i32 %reg_9, ptr %reg_6
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = add i32 6, 0
	%reg_13 = srem i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_10
	%reg_14 = load i32, ptr %reg_6
	%reg_15 = load i32, ptr %reg_10
	%reg_16 = icmp ne i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block5, label %Block6
Block4:  ;While end at line 130
	%reg_37 = add i32 0, 0
	ret i32 %reg_37
Block5:  ;If then at line 145
	%reg_17 = load i32, ptr %reg_6
	%reg_18 = getelementptr [6 x [50 x i32]], ptr @N4__mE___, i32 0, i32 %reg_17
	%reg_19 = call i32 @putstr(ptr %reg_18)
	%reg_20 = getelementptr [40 x i32], ptr @saY_HeI10_To, i32 0
	%reg_21 = call i32 @putstr(ptr %reg_20)
	%reg_22 = load i32, ptr %reg_10
	%reg_23 = getelementptr [6 x [50 x i32]], ptr @N4__mE___, i32 0, i32 %reg_22
	%reg_24 = call i32 @putstr(ptr %reg_23)
	%reg_25 = getelementptr [5 x i32], ptr @RET, i32 0
	%reg_26 = call i32 @putstr(ptr %reg_25)
	br label %Block6
Block6:  ;If end at line 145
	%reg_27 = load i32, ptr %reg_2
	%reg_28 = add i32 17, 0
	%reg_29 = mul i32 %reg_27, %reg_28
	%reg_30 = add i32 23, 0
	%reg_31 = add i32 %reg_29, %reg_30
	%reg_32 = add i32 32, 0
	%reg_33 = srem i32 %reg_31, %reg_32
	store i32 %reg_33, ptr %reg_2
	%reg_34 = load i32, ptr %reg_2
	%reg_35 = add i32 0, 0
	%reg_36 = icmp eq i32 %reg_34, %reg_35
	br i1 %reg_36, label %Block7, label %Block8
Block7:  ;If then at line 196
	br label %Block4
Block8:  ;If end at line 196
	br label %Block2
Block9:  ;Break at line 198
	br label %Block8
}
