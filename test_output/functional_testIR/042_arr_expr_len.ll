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

@N = global i32 -1
@arr = global [6 x i32] [i32 1,i32 2,i32 33,i32 4,i32 5,i32 6]

define i32 @main()
{
Block0:  ;Func define at line 4
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 4
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 6
	%reg_4 = load i32, ptr %reg_0
	%reg_5 = add i32 6, 0
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 6
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = load i32, ptr %reg_0
	%reg_9 = getelementptr [6 x i32], ptr @arr, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = add i32 %reg_7, %reg_10
	store i32 %reg_11, ptr %reg_2
	%reg_12 = load i32, ptr %reg_0
	%reg_13 = add i32 1, 0
	%reg_14 = add i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_0
	br label %Block2
Block4:  ;While end at line 6
	%reg_15 = load i32, ptr %reg_2
	ret i32 %reg_15
}
