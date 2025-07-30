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
declare void @_sysy_starttime(i32)
declare void @_sysy_stoptime(i32)
declare void @llvm.memset.p0.i32(ptr, i8, i32, i1)


define i32 @uniquePaths(i32 %reg_0, i32 %reg_1)
{
Block0:
	%reg_11 = alloca [9 x i32]
	br label %Block1
Block1:
	%reg_6 = icmp eq i32 %reg_0, 1
	br i1 %reg_6, label %Block2, label %Block4
Block2:
	br label %Block17
Block3:
	call void @llvm.memset.p0.i32(ptr %reg_11, i8 0, i32 36, i1 1)
	br label %Block21
Block4:
	%reg_9 = icmp eq i32 %reg_1, 1
	br i1 %reg_9, label %Block2, label %Block3
Block5: ; Loop 0 header
	%reg_97 = phi i32 [ %reg_31, %Block6 ], [ 0, %Block22 ]
	br label %Block6
Block6: ; Loop 0 latch
	%reg_22 = mul i32 %reg_97, 3
	%reg_24 = add i32 %reg_22, %reg_1
	%reg_26 = sub i32 %reg_24, 1
	%reg_27 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_26
	store i32 1, ptr %reg_27
	%reg_31 = add i32 %reg_97, 1
	%reg_123 = icmp slt i32 %reg_31, %reg_0
	br i1 %reg_123, label %Block5, label %Block23
Block7:
	br label %Block20
Block8: ; Loop 1 header
	%reg_98 = phi i32 [ %reg_47, %Block9 ], [ 0, %Block28 ]
	br label %Block9
Block9: ; Loop 1 latch
	%reg_38 = sub i32 %reg_0, 1
	%reg_40 = mul i32 %reg_38, 3
	%reg_42 = add i32 %reg_40, %reg_98
	%reg_43 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_42
	store i32 1, ptr %reg_43
	%reg_47 = add i32 %reg_98, 1
	%reg_120 = icmp slt i32 %reg_47, %reg_1
	br i1 %reg_120, label %Block8, label %Block29
Block10:
	%reg_50 = sub i32 %reg_0, 2
	br label %Block18
Block11: ; Loop 3 header
	%reg_99 = phi i32 [ %reg_92, %Block16 ], [ %reg_50, %Block26 ]
	br label %Block12
Block12: ; Loop 3 body
	%reg_57 = sub i32 %reg_1, 2
	br label %Block19
Block13:
	%reg_94 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 0
	%reg_95 = load i32, ptr %reg_94
	br label %Block17
Block14: ; Loop 2 header (nested depth: 1)
	%reg_101 = phi i32 [ %reg_89, %Block15 ], [ %reg_57, %Block24 ]
	br label %Block15
Block15: ; Loop 2 latch (nested depth: 1)
	%reg_64 = mul i32 %reg_99, 3
	%reg_66 = add i32 %reg_64, %reg_101
	%reg_67 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_66
	%reg_70 = add i32 %reg_99, 1
	%reg_72 = mul i32 %reg_70, 3
	%reg_74 = add i32 %reg_72, %reg_101
	%reg_75 = getelementptr [9 x i32], ptr %reg_11, i32 0, i32 %reg_74
	%reg_76 = load i32, ptr %reg_75
	%reg_84 = getelementptr i32, ptr %reg_67, i32 1
	%reg_85 = load i32, ptr %reg_84
	%reg_86 = add i32 %reg_76, %reg_85
	store i32 %reg_86, ptr %reg_67
	%reg_89 = sub i32 %reg_101, 1
	%reg_117 = icmp sgt i32 %reg_89, -1
	br i1 %reg_117, label %Block14, label %Block25
Block16: ; Loop 3 latch
	%reg_92 = sub i32 %reg_99, 1
	%reg_110 = icmp sgt i32 %reg_92, -1
	br i1 %reg_110, label %Block11, label %Block27
Block17:
	%reg_96 = phi i32 [ 1, %Block2 ], [ %reg_95, %Block13 ]
	ret i32 %reg_96
Block18:
	%reg_107 = icmp sgt i32 %reg_50, -1
	br i1 %reg_107, label %Block26, label %Block13
Block19: ; Loop 3 body
	%reg_114 = icmp sgt i32 %reg_57, -1
	br i1 %reg_114, label %Block24, label %Block16
Block20:
	%reg_119 = icmp slt i32 0, %reg_1
	br i1 %reg_119, label %Block28, label %Block10
Block21:
	%reg_122 = icmp slt i32 0, %reg_0
	br i1 %reg_122, label %Block22, label %Block7
Block22: ; Loop 0 preheader
	br label %Block5
Block23: ; Loop 0 exit target
	br label %Block7
Block24: ; Loop 2 preheader (nested depth: 1)
	br label %Block14
Block25: ; Loop 3 body
	br label %Block16
Block26: ; Loop 3 preheader
	br label %Block11
Block27: ; Loop 3 exit target
	br label %Block13
Block28: ; Loop 1 preheader
	br label %Block8
Block29: ; Loop 1 exit target
	br label %Block10
}

define i32 @main()
{
Block0:
	br label %Block1
Block1:
	%reg_7 = call i32 @uniquePaths(i32 3, i32 3)
	ret i32 %reg_7
}
