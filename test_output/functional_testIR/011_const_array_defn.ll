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

@a = global [5 x i32] [i32 0,i32 1,i32 2,i32 3,i32 4]

define i32 @main()
{
Block0:  ;Func define at line 3
	br label %Block1
Block1:  ;Func body at line 3
	%reg_0 = add i32 4, 0
	%reg_1 = getelementptr [5 x i32], ptr @a, i32 0, i32 %reg_0
	%reg_2 = load i32, ptr %reg_1
	ret i32 %reg_2
}
