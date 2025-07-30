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

@MAX_WIDTH = global i32 1024
@MAX_HEIGHT = global i32 1024
@image = global [1048576 x i32] zeroinitializer
@width = global i32 zeroinitializer
@height = global i32 zeroinitializer
@PI = global float 0x400921fb60000000
@TWO_PI = global float 0x401921fb60000000
@EPSILON = global float 0x3eb0c6f7a0000000

define float @my_fabs(float %reg_0)
{
Block0:
	br label %Block1
Block1:
	%reg_5 = fcmp ogt float %reg_0, 0x0
	br i1 %reg_5, label %Block2, label %Block3
Block2:
	br label %Block4
Block3:
	%reg_8 = fsub float 0x0, %reg_0
	br label %Block4
Block4:
	%reg_9 = phi float [ %reg_0, %Block2 ], [ %reg_8, %Block3 ]
	ret float %reg_9
}

define float @p(float %reg_0)
{
Block0:
	br label %Block1
Block1:
	%reg_5 = fmul float 0x4008000000000000, %reg_0
	%reg_9 = fmul float 0x4010000000000000, %reg_0
	%reg_11 = fmul float %reg_9, %reg_0
	%reg_13 = fmul float %reg_11, %reg_0
	%reg_14 = fsub float %reg_5, %reg_13
	ret float %reg_14
}

define float @my_sin_impl(float %reg_0)
{
Block0:
	br label %Block1
Block1:
	br label %Block6
Block2:
	br label %Block4
Block3:
	%reg_9 = fdiv float %reg_0, 0x4008000000000000
	%reg_10 = call float @my_sin_impl(float %reg_9)
	br label %Block15
Block4:
	%reg_12 = phi float [ %reg_0, %Block2 ], [ %reg_11, %Block14 ]
	ret float %reg_12
Block5:
	%reg_4 = load float, ptr @EPSILON
	%reg_5 = fcmp ole float %reg_3, %reg_4
	br i1 %reg_5, label %Block2, label %Block3
Block6:
	%reg_17 = fcmp ogt float %reg_0, 0x0
	br i1 %reg_17, label %Block7, label %Block8
Block7:
	br label %Block9
Block8:
	%reg_20 = fsub float 0x0, %reg_0
	br label %Block9
Block9:
	%reg_21 = phi float [ %reg_0, %Block7 ], [ %reg_20, %Block8 ]
	%reg_3 = fadd float %reg_21, 0x0
	br label %Block5
Block14:
	br label %Block4
Block15:
	%reg_26 = fmul float 0x4008000000000000, %reg_10
	%reg_30 = fmul float 0x4010000000000000, %reg_10
	%reg_32 = fmul float %reg_30, %reg_10
	%reg_34 = fmul float %reg_32, %reg_10
	%reg_35 = fsub float %reg_26, %reg_34
	%reg_11 = fadd float %reg_35, 0x0
	br label %Block14
}

define float @my_sin(float %reg_0)
{
Block0:
	br label %Block1
Block1:
	%reg_3 = load float, ptr @TWO_PI
	%reg_4 = fcmp ogt float %reg_0, %reg_3
	br i1 %reg_4, label %Block2, label %Block4
Block2:
	%reg_11 = load float, ptr @TWO_PI
	%reg_12 = fdiv float %reg_0, %reg_11
	%reg_13 = fptosi float %reg_12 to i32
	%reg_16 = sitofp i32 %reg_13 to float
	%reg_18 = fmul float %reg_16, %reg_11
	%reg_19 = fsub float %reg_0, %reg_18
	br label %Block3
Block3:
	%reg_35 = phi float [ %reg_19, %Block2 ], [ %reg_0, %Block4 ]
	%reg_21 = load float, ptr @PI
	%reg_22 = fcmp ogt float %reg_35, %reg_21
	br i1 %reg_22, label %Block5, label %Block6
Block4:
	%reg_6 = load float, ptr @TWO_PI
	%reg_7 = fsub float 0x0, %reg_6
	%reg_8 = fcmp olt float %reg_0, %reg_7
	br i1 %reg_8, label %Block2, label %Block3
Block5:
	%reg_24 = load float, ptr @TWO_PI
	%reg_25 = fsub float %reg_35, %reg_24
	br label %Block6
Block6:
	%reg_36 = phi float [ %reg_35, %Block3 ], [ %reg_25, %Block5 ]
	%reg_27 = load float, ptr @PI
	%reg_28 = fsub float 0x0, %reg_27
	%reg_29 = fcmp olt float %reg_36, %reg_28
	br i1 %reg_29, label %Block7, label %Block8
Block7:
	%reg_31 = load float, ptr @TWO_PI
	%reg_32 = fadd float %reg_36, %reg_31
	br label %Block8
Block8:
	%reg_37 = phi float [ %reg_36, %Block6 ], [ %reg_32, %Block7 ]
	%reg_34 = call float @my_sin_impl(float %reg_37)
	ret float %reg_34
}

define float @my_cos(float %reg_0)
{
Block0:
	br label %Block1
Block1:
	%reg_3 = load float, ptr @PI
	%reg_6 = fdiv float %reg_3, 0x4000000000000000
	%reg_7 = fadd float %reg_0, %reg_6
	%reg_8 = call float @my_sin(float %reg_7)
	ret float %reg_8
}

define i32 @read_image()
{
Block0:
	br label %Block1
Block1:
	%reg_0 = call i32 @getch()
	%reg_2 = icmp ne i32 %reg_0, 80
	br i1 %reg_2, label %Block2, label %Block4
Block2:
	br label %Block15
Block3:
	%reg_8 = call i32 @getint()
	store i32 %reg_8, ptr @width
	%reg_9 = call i32 @getint()
	store i32 %reg_9, ptr @height
	%reg_10 = load i32, ptr @width
	%reg_11 = load i32, ptr @MAX_WIDTH
	%reg_12 = icmp sgt i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block5, label %Block8
Block4:
	%reg_3 = call i32 @getch()
	%reg_5 = icmp ne i32 %reg_3, 50
	br i1 %reg_5, label %Block2, label %Block3
Block5:
	br label %Block15
Block6:
	br label %Block17
Block7:
	%reg_16 = call i32 @getint()
	%reg_18 = icmp ne i32 %reg_16, 255
	br i1 %reg_18, label %Block5, label %Block6
Block8:
	%reg_13 = load i32, ptr @height
	%reg_14 = load i32, ptr @MAX_HEIGHT
	%reg_15 = icmp sgt i32 %reg_13, %reg_14
	br i1 %reg_15, label %Block5, label %Block7
Block9: ; Loop 1 header
	%reg_46 = phi i32 [ %reg_43, %Block14 ], [ 0, %Block20 ]
	br label %Block10
Block10: ; Loop 1 body
	br label %Block16
Block11: ; Loop 1 exit target
	br label %Block15
Block12: ; Loop 0 header (nested depth: 1)
	%reg_66 = phi ptr [ %reg_65, %Block18 ], [ %reg_67, %Block13 ]
	%reg_48 = phi i32 [ %reg_40, %Block13 ], [ 0, %Block18 ]
	br label %Block13
Block13: ; Loop 0 latch (nested depth: 1)
	%reg_37 = call i32 @getint()
	store i32 %reg_37, ptr %reg_66
	%reg_40 = add i32 %reg_48, 1
	%reg_52 = load i32, ptr @width
	%reg_53 = icmp slt i32 %reg_40, %reg_52
	%reg_67 = getelementptr i32, ptr %reg_66, i32 1
	br i1 %reg_53, label %Block12, label %Block19
Block14: ; Loop 1 latch
	%reg_43 = add i32 %reg_46, 1
	%reg_57 = load i32, ptr @height
	%reg_58 = icmp slt i32 %reg_43, %reg_57
	br i1 %reg_58, label %Block9, label %Block21
Block15:
	%reg_45 = phi i32 [ -1, %Block2 ], [ -1, %Block5 ], [ 0, %Block11 ]
	ret i32 %reg_45
Block16: ; Loop 1 body
	%reg_50 = load i32, ptr @width
	%reg_51 = icmp slt i32 0, %reg_50
	br i1 %reg_51, label %Block18, label %Block14
Block17:
	%reg_55 = load i32, ptr @height
	%reg_56 = icmp slt i32 0, %reg_55
	br i1 %reg_56, label %Block20, label %Block11
Block18: ; Loop 0 preheader (nested depth: 1)
	%reg_65 = getelementptr [1048576 x i32], ptr @image, i32 0, i32 1
	br label %Block12
Block19: ; Loop 0 dedicated exit (nested depth: 1)
	br label %Block14
Block20: ; Loop 1 preheader
	br label %Block9
Block21: ; Loop 1 dedicated exit
	br label %Block11
}

define i32 @rotate(i32 %reg_0, i32 %reg_1, float %reg_2)
{
Block0:
	br label %Block1
Block1:
	%reg_8 = call float @my_sin(float %reg_2)
	br label %Block9
Block2:
	br label %Block7
Block3:
	%reg_70 = load i32, ptr @width
	%reg_71 = mul i32 %reg_55, %reg_70
	%reg_73 = add i32 %reg_71, %reg_41
	%reg_74 = getelementptr [1048576 x i32], ptr @image, i32 0, i32 %reg_73
	%reg_75 = load i32, ptr %reg_74
	br label %Block7
Block4:
	%reg_66 = load i32, ptr @height
	%reg_67 = icmp sge i32 %reg_55, %reg_66
	br i1 %reg_67, label %Block2, label %Block3
Block5:
	%reg_64 = icmp slt i32 %reg_55, 0
	br i1 %reg_64, label %Block2, label %Block4
Block6:
	%reg_60 = load i32, ptr @width
	%reg_61 = icmp sge i32 %reg_41, %reg_60
	br i1 %reg_61, label %Block2, label %Block5
Block7:
	%reg_76 = phi i32 [ 0, %Block2 ], [ %reg_75, %Block3 ]
	ret i32 %reg_76
Block8:
	%reg_13 = load i32, ptr @width
	%reg_85 = ashr i32 %reg_13, 31
	%reg_86 = and i32 %reg_85, 1
	%reg_87 = add i32 %reg_13, %reg_86
	%reg_15 = ashr i32 %reg_87, 1
	%reg_17 = load i32, ptr @height
	%reg_88 = ashr i32 %reg_17, 31
	%reg_89 = and i32 %reg_88, 1
	%reg_90 = add i32 %reg_17, %reg_89
	%reg_19 = ashr i32 %reg_90, 1
	%reg_23 = sub i32 %reg_0, %reg_15
	%reg_27 = sub i32 %reg_1, %reg_19
	%reg_30 = sitofp i32 %reg_23 to float
	%reg_32 = fmul float %reg_30, %reg_11
	%reg_34 = sitofp i32 %reg_27 to float
	%reg_36 = fmul float %reg_34, %reg_8
	%reg_37 = fsub float %reg_32, %reg_36
	%reg_39 = sitofp i32 %reg_15 to float
	%reg_40 = fadd float %reg_37, %reg_39
	%reg_41 = fptosi float %reg_40 to i32
	%reg_46 = fmul float %reg_30, %reg_8
	%reg_50 = fmul float %reg_34, %reg_11
	%reg_51 = fadd float %reg_46, %reg_50
	%reg_53 = sitofp i32 %reg_19 to float
	%reg_54 = fadd float %reg_51, %reg_53
	%reg_55 = fptosi float %reg_54 to i32
	%reg_58 = icmp slt i32 %reg_41, 0
	br i1 %reg_58, label %Block2, label %Block6
Block9:
	%reg_79 = load float, ptr @PI
	%reg_82 = fdiv float %reg_79, 0x4000000000000000
	%reg_83 = fadd float %reg_2, %reg_82
	%reg_84 = call float @my_sin(float %reg_83)
	%reg_11 = fadd float %reg_84, 0x0
	br label %Block8
}

define void @write_pgm(float %reg_0)
{
Block0:
	br label %Block1
Block1:
	call void @putch(i32 80)
	call void @putch(i32 50)
	call void @putch(i32 10)
	%reg_5 = load i32, ptr @width
	call void @putint(i32 %reg_5)
	call void @putch(i32 32)
	%reg_7 = load i32, ptr @height
	call void @putint(i32 %reg_7)
	call void @putch(i32 32)
	call void @putint(i32 255)
	call void @putch(i32 10)
	br label %Block9
Block2: ; Loop 1 header
	%reg_33 = phi i32 [ %reg_32, %Block7 ], [ 0, %Block12 ]
	br label %Block3
Block3: ; Loop 1 body
	br label %Block8
Block4: ; Loop 1 exit target
	ret void
Block5: ; Loop 0 header (nested depth: 1)
	%reg_35 = phi i32 [ %reg_28, %Block6 ], [ 0, %Block10 ]
	br label %Block6
Block6: ; Loop 0 latch (nested depth: 1)
	%reg_24 = call i32 @rotate(i32 %reg_35, i32 %reg_33, float %reg_0)
	call void @putint(i32 %reg_24)
	call void @putch(i32 32)
	%reg_28 = add i32 %reg_35, 1
	%reg_39 = load i32, ptr @width
	%reg_40 = icmp slt i32 %reg_28, %reg_39
	br i1 %reg_40, label %Block5, label %Block11
Block7: ; Loop 1 latch
	call void @putch(i32 10)
	%reg_32 = add i32 %reg_33, 1
	%reg_44 = load i32, ptr @height
	%reg_45 = icmp slt i32 %reg_32, %reg_44
	br i1 %reg_45, label %Block2, label %Block13
Block8: ; Loop 1 body
	%reg_37 = load i32, ptr @width
	%reg_38 = icmp slt i32 0, %reg_37
	br i1 %reg_38, label %Block10, label %Block7
Block9:
	%reg_42 = load i32, ptr @height
	%reg_43 = icmp slt i32 0, %reg_42
	br i1 %reg_43, label %Block12, label %Block4
Block10: ; Loop 0 preheader (nested depth: 1)
	br label %Block5
Block11: ; Loop 0 dedicated exit (nested depth: 1)
	br label %Block7
Block12: ; Loop 1 preheader
	br label %Block2
Block13: ; Loop 1 dedicated exit
	br label %Block4
}

define i32 @main()
{
Block0:
	br label %Block1
Block1:
	%reg_1 = call float @getfloat()
	%reg_2 = call i32 @getch()
	%reg_3 = call i32 @read_image()
	%reg_5 = icmp slt i32 %reg_3, 0
	br i1 %reg_5, label %Block2, label %Block3
Block2:
	br label %Block4
Block3:
	call void @write_pgm(float %reg_1)
	br label %Block4
Block4:
	%reg_10 = phi i32 [ -1, %Block2 ], [ 0, %Block3 ]
	ret i32 %reg_10
}
