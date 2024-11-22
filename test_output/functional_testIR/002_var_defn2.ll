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

@a = global i32 3
@b = global i32 5

define i32 @main()
{
Block0:  ;Func define at line 5
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 5
	%reg_1 = add i32 5, 0
	store i32 %reg_1, ptr %reg_0
	%reg_2 = load i32, ptr %reg_0
	%reg_3 = load i32, ptr @b
	%reg_4 = add i32 %reg_2, %reg_3
	ret i32 %reg_4
}
