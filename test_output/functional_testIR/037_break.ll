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
	%reg_3 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 2
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_0
	%reg_4 = add i32 0, 0
	store i32 %reg_4, ptr %reg_3
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_3
	br label %Block2
Block2:  ;While condition at line 7
	%reg_6 = load i32, ptr %reg_0
	%reg_7 = add i32 100, 0
	%reg_8 = icmp slt i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 7
	%reg_9 = load i32, ptr %reg_0
	%reg_10 = add i32 50, 0
	%reg_11 = icmp eq i32 %reg_9, %reg_10
	br i1 %reg_11, label %Block5, label %Block6
Block4:  ;While end at line 7
	%reg_18 = load i32, ptr %reg_3
	ret i32 %reg_18
Block5:  ;If then at line 8
	br label %Block4
Block6:  ;If end at line 8
	%reg_12 = load i32, ptr %reg_3
	%reg_13 = load i32, ptr %reg_0
	%reg_14 = add i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_3
	%reg_15 = load i32, ptr %reg_0
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_0
	br label %Block2
Block7:  ;Break at line 9
	br label %Block6
}
