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

@x = global [3 x i32] [i32 2344,i32 1232,i32 3435]
@y = global [3 x i32] [i32 234,i32 566,i32 127378]
@d = global i32 zeroinitializer

define void @modx()
{
Block0:  ;Func define at line 4
	br label %Block1
Block1:  ;Func body at line 4
	%reg_0 = add i32 0, 0
	%reg_1 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_0
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = add i32 2, 0
	%reg_6 = add i32 %reg_4, %reg_5
	store i32 %reg_6, ptr %reg_1
	%reg_7 = add i32 1, 0
	%reg_8 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_7
	%reg_9 = add i32 1, 0
	%reg_10 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = add i32 2, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_8
	%reg_14 = add i32 2, 0
	%reg_15 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_14
	%reg_16 = add i32 2, 0
	%reg_17 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = add i32 2, 0
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_15
	ret void
}

define void @mody()
{
Block0:  ;Func define at line 10
	br label %Block1
Block1:  ;Func body at line 10
	%reg_0 = add i32 0, 0
	%reg_1 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_0
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = add i32 2, 0
	%reg_6 = add i32 %reg_4, %reg_5
	store i32 %reg_6, ptr %reg_1
	%reg_7 = add i32 1, 0
	%reg_8 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_7
	%reg_9 = add i32 1, 0
	%reg_10 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = add i32 2, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_8
	%reg_14 = add i32 2, 0
	%reg_15 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_14
	%reg_16 = add i32 2, 0
	%reg_17 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = add i32 2, 0
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_15
	ret void
}

define void @refx()
{
Block0:  ;Func define at line 17
	br label %Block1
Block1:  ;Func body at line 17
	%reg_0 = add i32 0, 0
	%reg_1 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_0
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 1, 0
	%reg_4 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = add i32 %reg_2, %reg_5
	%reg_7 = add i32 2, 0
	%reg_8 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = add i32 %reg_6, %reg_9
	store i32 %reg_10, ptr @d
	ret void
}

define void @refy()
{
Block0:  ;Func define at line 22
	br label %Block1
Block1:  ;Func body at line 22
	%reg_0 = add i32 0, 0
	%reg_1 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_0
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 1, 0
	%reg_4 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = add i32 %reg_2, %reg_5
	%reg_7 = add i32 2, 0
	%reg_8 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = add i32 %reg_6, %reg_9
	store i32 %reg_10, ptr @d
	ret void
}

define void @modxrefy()
{
Block0:  ;Func define at line 27
	br label %Block1
Block1:  ;Func body at line 27
	%reg_0 = add i32 0, 0
	%reg_1 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_0
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = add i32 3, 0
	%reg_6 = add i32 %reg_4, %reg_5
	store i32 %reg_6, ptr %reg_1
	%reg_7 = add i32 1, 0
	%reg_8 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_7
	%reg_9 = add i32 1, 0
	%reg_10 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = add i32 4, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_8
	%reg_14 = add i32 2, 0
	%reg_15 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_14
	%reg_16 = add i32 2, 0
	%reg_17 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = add i32 5, 0
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_15
	ret void
}

define void @modyrefx()
{
Block0:  ;Func define at line 34
	br label %Block1
Block1:  ;Func body at line 34
	%reg_0 = add i32 0, 0
	%reg_1 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_0
	%reg_2 = add i32 0, 0
	%reg_3 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = add i32 5, 0
	%reg_6 = add i32 %reg_4, %reg_5
	store i32 %reg_6, ptr %reg_1
	%reg_7 = add i32 1, 0
	%reg_8 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_7
	%reg_9 = add i32 1, 0
	%reg_10 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = add i32 6, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_8
	%reg_14 = add i32 2, 0
	%reg_15 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_14
	%reg_16 = add i32 2, 0
	%reg_17 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = add i32 7, 0
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_15
	ret void
}

define i32 @main()
{
Block0:  ;Func define at line 41
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 41
	%reg_1 = add i32 2, 0
	%reg_2 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_1
	%reg_3 = load i32, ptr %reg_2
	store i32 %reg_3, ptr %reg_0
	%reg_4 = add i32 2, 0
	%reg_5 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_4
	%reg_6 = add i32 3, 0
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr %reg_0
	%reg_8 = add i32 2, 0
	%reg_9 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = add i32 %reg_7, %reg_10
	store i32 %reg_11, ptr %reg_0
	%reg_12 = add i32 2, 0
	%reg_13 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_12
	%reg_14 = add i32 3, 0
	store i32 %reg_14, ptr %reg_13
	%reg_15 = load i32, ptr %reg_0
	%reg_16 = add i32 2, 0
	%reg_17 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = add i32 %reg_15, %reg_18
	%reg_20 = load i32, ptr @d
	%reg_21 = add i32 %reg_19, %reg_20
	store i32 %reg_21, ptr %reg_0
	%reg_22 = add i32 5, 0
	store i32 %reg_22, ptr @d
	%reg_23 = load i32, ptr %reg_0
	%reg_24 = add i32 2, 0
	%reg_25 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = add i32 %reg_23, %reg_26
	%reg_28 = load i32, ptr @d
	%reg_29 = add i32 %reg_27, %reg_28
	%reg_30 = load i32, ptr @d
	%reg_31 = add i32 %reg_29, %reg_30
	store i32 %reg_31, ptr %reg_0
	call void @refx()
	%reg_32 = load i32, ptr %reg_0
	%reg_33 = add i32 2, 0
	%reg_34 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_33
	%reg_35 = load i32, ptr %reg_34
	%reg_36 = add i32 %reg_32, %reg_35
	%reg_37 = load i32, ptr @d
	%reg_38 = add i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_0
	call void @modx()
	%reg_39 = load i32, ptr %reg_0
	%reg_40 = add i32 2, 0
	%reg_41 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	%reg_43 = add i32 %reg_39, %reg_42
	%reg_44 = load i32, ptr @d
	%reg_45 = add i32 %reg_43, %reg_44
	store i32 %reg_45, ptr %reg_0
	call void @mody()
	%reg_46 = load i32, ptr %reg_0
	%reg_47 = add i32 2, 0
	%reg_48 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_47
	%reg_49 = load i32, ptr %reg_48
	%reg_50 = add i32 %reg_46, %reg_49
	%reg_51 = load i32, ptr @d
	%reg_52 = add i32 %reg_50, %reg_51
	store i32 %reg_52, ptr %reg_0
	call void @modyrefx()
	%reg_53 = load i32, ptr %reg_0
	%reg_54 = add i32 2, 0
	%reg_55 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	%reg_57 = add i32 %reg_53, %reg_56
	%reg_58 = add i32 2, 0
	%reg_59 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_58
	%reg_60 = load i32, ptr %reg_59
	%reg_61 = add i32 %reg_57, %reg_60
	%reg_62 = load i32, ptr @d
	%reg_63 = add i32 %reg_61, %reg_62
	store i32 %reg_63, ptr %reg_0
	call void @modxrefy()
	%reg_64 = load i32, ptr %reg_0
	%reg_65 = add i32 2, 0
	%reg_66 = getelementptr [3 x i32], ptr @x, i32 0, i32 %reg_65
	%reg_67 = load i32, ptr %reg_66
	%reg_68 = add i32 %reg_64, %reg_67
	%reg_69 = load i32, ptr @d
	%reg_70 = add i32 %reg_68, %reg_69
	store i32 %reg_70, ptr %reg_0
	%reg_71 = load i32, ptr %reg_0
	%reg_72 = add i32 1, 0
	%reg_73 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_72
	%reg_74 = load i32, ptr %reg_73
	%reg_75 = add i32 %reg_71, %reg_74
	%reg_76 = add i32 2, 0
	%reg_77 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_76
	%reg_78 = load i32, ptr %reg_77
	%reg_79 = add i32 %reg_75, %reg_78
	%reg_80 = load i32, ptr @d
	%reg_81 = add i32 %reg_79, %reg_80
	store i32 %reg_81, ptr %reg_0
	call void @modyrefx()
	%reg_82 = load i32, ptr %reg_0
	%reg_83 = add i32 1, 0
	%reg_84 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_83
	%reg_85 = load i32, ptr %reg_84
	%reg_86 = add i32 %reg_82, %reg_85
	%reg_87 = add i32 2, 0
	%reg_88 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	%reg_90 = add i32 %reg_86, %reg_89
	%reg_91 = load i32, ptr @d
	%reg_92 = add i32 %reg_90, %reg_91
	store i32 %reg_92, ptr %reg_0
	call void @refy()
	%reg_93 = load i32, ptr %reg_0
	%reg_94 = add i32 1, 0
	%reg_95 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_94
	%reg_96 = load i32, ptr %reg_95
	%reg_97 = add i32 %reg_93, %reg_96
	%reg_98 = add i32 2, 0
	%reg_99 = getelementptr [3 x i32], ptr @y, i32 0, i32 %reg_98
	%reg_100 = load i32, ptr %reg_99
	%reg_101 = add i32 %reg_97, %reg_100
	%reg_102 = load i32, ptr @d
	%reg_103 = add i32 %reg_101, %reg_102
	store i32 %reg_103, ptr %reg_0
	%reg_104 = load i32, ptr %reg_0
	call void @putint(i32 %reg_104)
	%reg_105 = add i32 10, 0
	call void @putch(i32 %reg_105)
	%reg_106 = add i32 3, 0
	%reg_107 = getelementptr [3 x i32], ptr @x, i32 0
	call void @putarray(i32 %reg_106, ptr %reg_107)
	%reg_108 = add i32 3, 0
	%reg_109 = getelementptr [3 x i32], ptr @y, i32 0
	call void @putarray(i32 %reg_108, ptr %reg_109)
	%reg_110 = load i32, ptr @d
	call void @putint(i32 %reg_110)
	%reg_111 = add i32 10, 0
	call void @putch(i32 %reg_111)
	%reg_112 = add i32 0, 0
	ret i32 %reg_112
}
