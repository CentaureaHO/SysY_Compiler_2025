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

@k = global i32 zeroinitializer

define i32 @main()
{
Block0:  ;Func define at line 3
	%reg_26 = alloca i32
	%reg_20 = alloca i32
	%reg_18 = alloca i32
	%reg_7 = alloca i32
	br label %Block1
Block1:  ;Func body at line 3
	%reg_0 = add i32 3389, 0
	store i32 %reg_0, ptr @k
	%reg_1 = load i32, ptr @k
	%reg_2 = add i32 10000, 0
	%reg_3 = icmp slt i32 %reg_1, %reg_2
	br i1 %reg_3, label %Block2, label %Block3
Block2:  ;If then at line 5
	%reg_4 = load i32, ptr @k
	%reg_5 = add i32 1, 0
	%reg_6 = add i32 %reg_4, %reg_5
	store i32 %reg_6, ptr @k
	%reg_8 = add i32 112, 0
	store i32 %reg_8, ptr %reg_7
	br label %Block4
Block3:  ;If end at line 5
	%reg_34 = load i32, ptr @k
	ret i32 %reg_34
Block4:  ;While condition at line 8
	%reg_9 = load i32, ptr %reg_7
	%reg_10 = add i32 10, 0
	%reg_11 = icmp sgt i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block5, label %Block6
Block5:  ;While body at line 8
	%reg_12 = load i32, ptr %reg_7
	%reg_13 = add i32 88, 0
	%reg_14 = sub i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_7
	%reg_15 = load i32, ptr %reg_7
	%reg_16 = add i32 1000, 0
	%reg_17 = icmp slt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block7, label %Block8
Block6:  ;While end at line 8
	%reg_33 = load i32, ptr %reg_7
	call void @putint(i32 %reg_33)
	br label %Block3
Block7:  ;If then at line 10
	%reg_19 = add i32 9, 0
	store i32 %reg_19, ptr %reg_18
	%reg_21 = add i32 11, 0
	store i32 %reg_21, ptr %reg_20
	%reg_22 = add i32 10, 0
	store i32 %reg_22, ptr %reg_18
	%reg_23 = load i32, ptr %reg_7
	%reg_24 = load i32, ptr %reg_18
	%reg_25 = sub i32 %reg_23, %reg_24
	store i32 %reg_25, ptr %reg_7
	%reg_27 = add i32 11, 0
	store i32 %reg_27, ptr %reg_26
	%reg_28 = load i32, ptr %reg_7
	%reg_29 = load i32, ptr %reg_26
	%reg_30 = add i32 %reg_28, %reg_29
	%reg_31 = load i32, ptr %reg_20
	%reg_32 = add i32 %reg_30, %reg_31
	store i32 %reg_32, ptr %reg_7
	br label %Block8
Block8:  ;If end at line 10
	br label %Block4
}
