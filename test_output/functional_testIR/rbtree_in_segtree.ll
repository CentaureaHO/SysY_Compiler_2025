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

@maxn = global i32 50010
@maxm = global i32 10000000
@INT_MAX = global i32 2147483647
@son = global [2 x [10000000 x i32]] zeroinitializer
@fa = global [10000000 x i32] zeroinitializer
@size = global [10000000 x i32] zeroinitializer
@cur = global i32 zeroinitializer
@cnt = global [10000000 x i32] zeroinitializer
@col = global [10000000 x i32] zeroinitializer
@val = global [10000000 x i32] zeroinitializer
@root = global i32 zeroinitializer
@tree = global [200040 x i32] zeroinitializer
@a = global [50010 x i32] zeroinitializer
@n = global i32 zeroinitializer
@m = global i32 zeroinitializer

define i32 @min(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 5
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 5
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 7
	%reg_7 = load i32, ptr %reg_2
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 7
	%reg_8 = load i32, ptr %reg_3
	ret i32 %reg_8
}

define i32 @max(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 13
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 13
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 15
	%reg_7 = load i32, ptr %reg_3
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 15
	%reg_8 = load i32, ptr %reg_2
	ret i32 %reg_8
}

define i32 @eq(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 21
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 21
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 23
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 23
	ret i32 0
Block4:  ;If else at line 23
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
	br label %Block3
}

define i32 @lt(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 29
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 29
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 31
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 31
	ret i32 0
Block4:  ;If else at line 31
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
	br label %Block3
}

define i32 @ne(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 37
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 37
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp ne i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block4
Block2:  ;If then at line 39
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 39
	ret i32 0
Block4:  ;If else at line 39
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
	br label %Block3
}

define i32 @select(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 46
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 46
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block2, label %Block4
Block2:  ;If then at line 48
	%reg_8 = load i32, ptr %reg_4
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 48
	ret i32 0
Block4:  ;If else at line 48
	%reg_9 = load i32, ptr %reg_5
	ret i32 %reg_9
	br label %Block3
}

define void @newNode(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 55
	%reg_9 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 55
	%reg_6 = load i32, ptr @cur
	%reg_7 = add i32 1, 0
	%reg_8 = add i32 %reg_6, %reg_7
	store i32 %reg_8, ptr @cur
	%reg_10 = load i32, ptr @cur
	store i32 %reg_10, ptr %reg_9
	%reg_11 = load i32, ptr %reg_9
	%reg_12 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_5
	store i32 %reg_13, ptr %reg_12
	%reg_14 = load i32, ptr %reg_9
	%reg_15 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_14
	%reg_16 = add i32 1, 0
	store i32 %reg_16, ptr %reg_15
	%reg_17 = load i32, ptr %reg_9
	%reg_18 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_17
	%reg_19 = add i32 1, 0
	store i32 %reg_19, ptr %reg_18
	%reg_20 = load i32, ptr %reg_9
	%reg_21 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_3
	store i32 %reg_22, ptr %reg_21
	%reg_23 = load i32, ptr %reg_9
	%reg_24 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_4
	store i32 %reg_25, ptr %reg_24
	ret void
}

define void @update1(i32 %reg_0)
{
Block0:  ;Func define at line 63
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 63
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_2
	%reg_4 = add i32 0, 0
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_4, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	%reg_8 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	%reg_13 = add i32 %reg_9, %reg_12
	%reg_14 = add i32 1, 0
	%reg_15 = load i32, ptr %reg_1
	%reg_16 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_14, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	%reg_18 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = add i32 %reg_13, %reg_19
	store i32 %reg_20, ptr %reg_3
	ret void
}

define void @rotate(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 64
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 64
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp eq i32 %reg_5, 0
	%reg_7 = zext i1 %reg_6 to i32
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_7, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	store i32 %reg_10, ptr %reg_4
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = icmp eq i32 %reg_11, 0
	%reg_13 = zext i1 %reg_12 to i32
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_13, i32 %reg_14
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = load i32, ptr %reg_4
	%reg_18 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_16, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	store i32 %reg_19, ptr %reg_15
	%reg_20 = load i32, ptr %reg_3
	%reg_21 = load i32, ptr %reg_4
	%reg_22 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_20, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	%reg_24 = icmp ne i32 %reg_23, 0
	br i1 %reg_24, label %Block2, label %Block3
Block2:  ;If then at line 67
	%reg_25 = load i32, ptr %reg_3
	%reg_26 = load i32, ptr %reg_4
	%reg_27 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_25, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	%reg_29 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_2
	store i32 %reg_30, ptr %reg_29
	br label %Block3
Block3:  ;If end at line 67
	%reg_31 = load i32, ptr %reg_4
	%reg_32 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_31
	%reg_33 = load i32, ptr %reg_2
	%reg_34 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_33
	%reg_35 = load i32, ptr %reg_34
	store i32 %reg_35, ptr %reg_32
	%reg_36 = load i32, ptr %reg_2
	%reg_37 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_36
	%reg_38 = load i32, ptr %reg_37
	%reg_39 = icmp eq i32 %reg_38, 0
	br i1 %reg_39, label %Block4, label %Block6
Block4:  ;If then at line 68
	%reg_40 = load i32, ptr %reg_4
	store i32 %reg_40, ptr @root
	br label %Block5
Block5:  ;If end at line 68
	%reg_54 = load i32, ptr %reg_3
	%reg_55 = load i32, ptr %reg_4
	%reg_56 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_54, i32 %reg_55
	%reg_57 = load i32, ptr %reg_2
	store i32 %reg_57, ptr %reg_56
	%reg_58 = load i32, ptr %reg_2
	%reg_59 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_58
	%reg_60 = load i32, ptr %reg_4
	store i32 %reg_60, ptr %reg_59
	%reg_61 = load i32, ptr %reg_4
	%reg_62 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_61
	%reg_63 = load i32, ptr %reg_2
	%reg_64 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_63
	%reg_65 = load i32, ptr %reg_64
	store i32 %reg_65, ptr %reg_62
	%reg_66 = load i32, ptr %reg_2
	call void @update1(i32 %reg_66)
	ret void
Block6:  ;If else at line 68
	%reg_41 = load i32, ptr %reg_2
	%reg_42 = add i32 1, 0
	%reg_43 = load i32, ptr %reg_2
	%reg_44 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_43
	%reg_45 = load i32, ptr %reg_44
	%reg_46 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_42, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	%reg_48 = call i32 @eq(i32 %reg_41, i32 %reg_47)
	%reg_49 = load i32, ptr %reg_2
	%reg_50 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_48, i32 %reg_51
	%reg_53 = load i32, ptr %reg_4
	store i32 %reg_53, ptr %reg_52
	br label %Block5
}

define void @transplant(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 74
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 74
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_6
	%reg_8 = load i32, ptr %reg_7
	store i32 %reg_8, ptr %reg_5
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = icmp eq i32 %reg_11, 0
	br i1 %reg_12, label %Block2, label %Block4
Block2:  ;If then at line 76
	%reg_13 = load i32, ptr %reg_3
	store i32 %reg_13, ptr @root
	br label %Block3
Block3:  ;If end at line 76
	ret void
Block4:  ;If else at line 76
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = add i32 1, 0
	%reg_16 = load i32, ptr %reg_2
	%reg_17 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_15, i32 %reg_18
	%reg_20 = load i32, ptr %reg_19
	%reg_21 = call i32 @eq(i32 %reg_14, i32 %reg_20)
	%reg_22 = load i32, ptr %reg_2
	%reg_23 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_21, i32 %reg_24
	%reg_26 = load i32, ptr %reg_3
	store i32 %reg_26, ptr %reg_25
	br label %Block3
}

define i32 @findMin(i32 %reg_0)
{
Block0:  ;Func define at line 80
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 80
	br label %Block2
Block2:  ;While condition at line 80
	%reg_2 = add i32 0, 0
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_2, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = icmp ne i32 %reg_5, 0
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 80
	%reg_7 = add i32 0, 0
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_7, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	store i32 %reg_10, ptr %reg_1
	br label %Block2
Block4:  ;While end at line 80
	%reg_11 = load i32, ptr %reg_1
	ret i32 %reg_11
}

define void @insertFixUp(i32 %reg_0)
{
Block0:  ;Func define at line 81
	%reg_23 = alloca i32
	%reg_16 = alloca i32
	%reg_12 = alloca i32
	%reg_8 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 81
	br label %Block2
Block2:  ;While condition at line 83
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 83
	%reg_9 = load i32, ptr %reg_1
	%reg_10 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	store i32 %reg_11, ptr %reg_8
	%reg_13 = load i32, ptr %reg_8
	%reg_14 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	store i32 %reg_15, ptr %reg_12
	%reg_17 = load i32, ptr %reg_8
	%reg_18 = add i32 0, 0
	%reg_19 = load i32, ptr %reg_12
	%reg_20 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_18, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = call i32 @eq(i32 %reg_17, i32 %reg_21)
	store i32 %reg_22, ptr %reg_16
	%reg_24 = load i32, ptr %reg_16
	%reg_25 = load i32, ptr %reg_12
	%reg_26 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_24, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	store i32 %reg_27, ptr %reg_23
	%reg_28 = load i32, ptr %reg_23
	%reg_29 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	%reg_31 = icmp ne i32 %reg_30, 0
	br i1 %reg_31, label %Block5, label %Block7
Block4:  ;While end at line 83
	%reg_67 = load i32, ptr @root
	%reg_68 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_67
	%reg_69 = add i32 0, 0
	store i32 %reg_69, ptr %reg_68
	ret void
Block5:  ;If then at line 86
	%reg_32 = load i32, ptr %reg_23
	%reg_33 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_32
	%reg_34 = add i32 0, 0
	store i32 %reg_34, ptr %reg_33
	%reg_35 = load i32, ptr %reg_8
	%reg_36 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_35
	%reg_37 = add i32 0, 0
	store i32 %reg_37, ptr %reg_36
	%reg_38 = load i32, ptr %reg_12
	store i32 %reg_38, ptr %reg_1
	%reg_39 = load i32, ptr %reg_1
	%reg_40 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_39
	%reg_41 = add i32 1, 0
	store i32 %reg_41, ptr %reg_40
	br label %Block6
Block6:  ;If end at line 86
	br label %Block2
Block7:  ;If else at line 86
	%reg_42 = load i32, ptr %reg_1
	%reg_43 = load i32, ptr %reg_16
	%reg_44 = load i32, ptr %reg_8
	%reg_45 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_43, i32 %reg_44
	%reg_46 = load i32, ptr %reg_45
	%reg_47 = icmp eq i32 %reg_42, %reg_46
	br i1 %reg_47, label %Block8, label %Block9
Block8:  ;If then at line 89
	%reg_48 = load i32, ptr %reg_8
	store i32 %reg_48, ptr %reg_1
	%reg_49 = load i32, ptr %reg_1
	%reg_50 = load i32, ptr %reg_16
	%reg_51 = icmp eq i32 %reg_50, 0
	%reg_52 = zext i1 %reg_51 to i32
	call void @rotate(i32 %reg_49, i32 %reg_52)
	br label %Block9
Block9:  ;If end at line 89
	%reg_53 = load i32, ptr %reg_1
	%reg_54 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_53
	%reg_55 = load i32, ptr %reg_54
	%reg_56 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_55
	%reg_57 = add i32 0, 0
	store i32 %reg_57, ptr %reg_56
	%reg_58 = load i32, ptr %reg_1
	%reg_59 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_58
	%reg_60 = load i32, ptr %reg_59
	%reg_61 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_60
	%reg_62 = load i32, ptr %reg_61
	%reg_63 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_62
	%reg_64 = add i32 1, 0
	store i32 %reg_64, ptr %reg_63
	%reg_65 = load i32, ptr %reg_12
	%reg_66 = load i32, ptr %reg_16
	call void @rotate(i32 %reg_65, i32 %reg_66)
	br label %Block6
}

define void @insert1(i32 %reg_0)
{
Block0:  ;Func define at line 96
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 96
	%reg_3 = load i32, ptr @root
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 99
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 99
	%reg_8 = load i32, ptr %reg_2
	store i32 %reg_8, ptr %reg_4
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = add i32 1, 0
	%reg_15 = add i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_10
	%reg_16 = load i32, ptr %reg_2
	%reg_17 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	%reg_19 = load i32, ptr %reg_1
	%reg_20 = icmp eq i32 %reg_18, %reg_19
	br i1 %reg_20, label %Block5, label %Block6
Block4:  ;While end at line 99
	%reg_36 = load i32, ptr %reg_1
	%reg_37 = add i32 1, 0
	%reg_38 = load i32, ptr %reg_4
	call void @newNode(i32 %reg_36, i32 %reg_37, i32 %reg_38)
	%reg_39 = load i32, ptr %reg_4
	%reg_40 = icmp eq i32 %reg_39, 0
	br i1 %reg_40, label %Block7, label %Block9
Block5:  ;If then at line 102
	%reg_21 = load i32, ptr %reg_2
	%reg_22 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_21
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = add i32 1, 0
	%reg_27 = add i32 %reg_25, %reg_26
	store i32 %reg_27, ptr %reg_22
	ret void
	br label %Block6
Block6:  ;If end at line 102
	%reg_28 = load i32, ptr %reg_2
	%reg_29 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	%reg_31 = load i32, ptr %reg_1
	%reg_32 = call i32 @lt(i32 %reg_30, i32 %reg_31)
	%reg_33 = load i32, ptr %reg_2
	%reg_34 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_32, i32 %reg_33
	%reg_35 = load i32, ptr %reg_34
	store i32 %reg_35, ptr %reg_2
	br label %Block2
Block7:  ;If then at line 106
	%reg_41 = load i32, ptr @cur
	store i32 %reg_41, ptr @root
	br label %Block8
Block8:  ;If end at line 106
	%reg_50 = load i32, ptr @cur
	call void @insertFixUp(i32 %reg_50)
	ret void
Block9:  ;If else at line 106
	%reg_42 = load i32, ptr %reg_4
	%reg_43 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_42
	%reg_44 = load i32, ptr %reg_43
	%reg_45 = load i32, ptr %reg_1
	%reg_46 = call i32 @lt(i32 %reg_44, i32 %reg_45)
	%reg_47 = load i32, ptr %reg_4
	%reg_48 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_46, i32 %reg_47
	%reg_49 = load i32, ptr @cur
	store i32 %reg_49, ptr %reg_48
	br label %Block8
}

define void @delFixUp(i32 %reg_0)
{
Block0:  ;Func define at line 110
	%reg_22 = alloca i32
	%reg_18 = alloca i32
	%reg_9 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 110
	br label %Block2
Block2:  ;While condition at line 112
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = load i32, ptr @root
	%reg_4 = icmp ne i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block5, label %Block4
Block3:  ;While body at line 112
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = add i32 0, 0
	%reg_12 = load i32, ptr %reg_1
	%reg_13 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_12
	%reg_14 = load i32, ptr %reg_13
	%reg_15 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_11, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = call i32 @eq(i32 %reg_10, i32 %reg_16)
	store i32 %reg_17, ptr %reg_9
	%reg_19 = load i32, ptr %reg_1
	%reg_20 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	store i32 %reg_21, ptr %reg_18
	%reg_23 = load i32, ptr %reg_9
	%reg_24 = load i32, ptr %reg_18
	%reg_25 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_23, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	store i32 %reg_26, ptr %reg_22
	%reg_27 = load i32, ptr %reg_22
	%reg_28 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_28
	%reg_30 = icmp ne i32 %reg_29, 0
	br i1 %reg_30, label %Block6, label %Block7
Block4:  ;While end at line 112
	%reg_110 = load i32, ptr %reg_1
	%reg_111 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_110
	%reg_112 = add i32 0, 0
	store i32 %reg_112, ptr %reg_111
	ret void
Block5:  ;And opertor at line 112
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	%reg_8 = icmp eq i32 %reg_7, 0
	br i1 %reg_8, label %Block3, label %Block4
Block6:  ;If then at line 115
	%reg_31 = load i32, ptr %reg_18
	%reg_32 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_31
	%reg_33 = add i32 1, 0
	store i32 %reg_33, ptr %reg_32
	%reg_34 = load i32, ptr %reg_22
	%reg_35 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_34
	%reg_36 = add i32 0, 0
	store i32 %reg_36, ptr %reg_35
	%reg_37 = load i32, ptr %reg_18
	%reg_38 = load i32, ptr %reg_9
	%reg_39 = icmp eq i32 %reg_38, 0
	%reg_40 = zext i1 %reg_39 to i32
	call void @rotate(i32 %reg_37, i32 %reg_40)
	%reg_41 = load i32, ptr %reg_9
	%reg_42 = load i32, ptr %reg_18
	%reg_43 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_41, i32 %reg_42
	%reg_44 = load i32, ptr %reg_43
	store i32 %reg_44, ptr %reg_22
	br label %Block7
Block7:  ;If end at line 115
	%reg_45 = add i32 0, 0
	%reg_46 = load i32, ptr %reg_22
	%reg_47 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_45, i32 %reg_46
	%reg_48 = load i32, ptr %reg_47
	%reg_49 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	%reg_51 = icmp eq i32 %reg_50, 0
	br i1 %reg_51, label %Block11, label %Block10
Block8:  ;If then at line 120
	%reg_59 = load i32, ptr %reg_22
	%reg_60 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_59
	%reg_61 = add i32 0, 0
	store i32 %reg_61, ptr %reg_60
	%reg_62 = load i32, ptr %reg_1
	%reg_63 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_62
	%reg_64 = load i32, ptr %reg_63
	store i32 %reg_64, ptr %reg_1
	br label %Block9
Block9:  ;If end at line 120
	br label %Block2
Block10:  ;If else at line 120
	%reg_65 = load i32, ptr %reg_9
	%reg_66 = load i32, ptr %reg_22
	%reg_67 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_65, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	%reg_69 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_68
	%reg_70 = load i32, ptr %reg_69
	%reg_71 = icmp eq i32 %reg_70, 0
	br i1 %reg_71, label %Block12, label %Block13
Block11:  ;And opertor at line 120
	%reg_52 = add i32 1, 0
	%reg_53 = load i32, ptr %reg_22
	%reg_54 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_52, i32 %reg_53
	%reg_55 = load i32, ptr %reg_54
	%reg_56 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = icmp eq i32 %reg_57, 0
	br i1 %reg_58, label %Block8, label %Block10
Block12:  ;If then at line 123
	%reg_72 = load i32, ptr %reg_22
	%reg_73 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_72
	%reg_74 = add i32 1, 0
	store i32 %reg_74, ptr %reg_73
	%reg_75 = load i32, ptr %reg_9
	%reg_76 = icmp eq i32 %reg_75, 0
	%reg_77 = zext i1 %reg_76 to i32
	%reg_78 = load i32, ptr %reg_22
	%reg_79 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_77, i32 %reg_78
	%reg_80 = load i32, ptr %reg_79
	%reg_81 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_80
	%reg_82 = add i32 0, 0
	store i32 %reg_82, ptr %reg_81
	%reg_83 = load i32, ptr %reg_22
	%reg_84 = load i32, ptr %reg_9
	call void @rotate(i32 %reg_83, i32 %reg_84)
	%reg_85 = load i32, ptr %reg_9
	%reg_86 = load i32, ptr %reg_18
	%reg_87 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_85, i32 %reg_86
	%reg_88 = load i32, ptr %reg_87
	store i32 %reg_88, ptr %reg_22
	br label %Block13
Block13:  ;If end at line 123
	%reg_89 = load i32, ptr %reg_22
	%reg_90 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_89
	%reg_91 = load i32, ptr %reg_18
	%reg_92 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_91
	%reg_93 = load i32, ptr %reg_92
	store i32 %reg_93, ptr %reg_90
	%reg_94 = load i32, ptr %reg_18
	%reg_95 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_94
	%reg_96 = add i32 0, 0
	store i32 %reg_96, ptr %reg_95
	%reg_97 = load i32, ptr %reg_9
	%reg_98 = load i32, ptr %reg_22
	%reg_99 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_97, i32 %reg_98
	%reg_100 = load i32, ptr %reg_99
	%reg_101 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_100
	%reg_102 = add i32 0, 0
	store i32 %reg_102, ptr %reg_101
	%reg_103 = load i32, ptr %reg_22
	%reg_104 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_103
	%reg_105 = load i32, ptr %reg_104
	%reg_106 = load i32, ptr %reg_9
	%reg_107 = icmp eq i32 %reg_106, 0
	%reg_108 = zext i1 %reg_107 to i32
	call void @rotate(i32 %reg_105, i32 %reg_108)
	%reg_109 = load i32, ptr @root
	store i32 %reg_109, ptr %reg_1
	br label %Block9
}

define void @erase(i32 %reg_0)
{
Block0:  ;Func define at line 134
	%reg_99 = alloca i32
	%reg_47 = alloca i32
	%reg_45 = alloca i32
	%reg_43 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 134
	%reg_3 = load i32, ptr @root
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 137
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 137
	%reg_8 = load i32, ptr %reg_2
	store i32 %reg_8, ptr %reg_4
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_9
	%reg_11 = load i32, ptr %reg_4
	%reg_12 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = add i32 1, 0
	%reg_15 = sub i32 %reg_13, %reg_14
	store i32 %reg_15, ptr %reg_10
	%reg_16 = load i32, ptr %reg_1
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = icmp eq i32 %reg_16, %reg_19
	br i1 %reg_20, label %Block5, label %Block6
Block4:  ;While end at line 137
	%reg_29 = load i32, ptr %reg_2
	%reg_30 = icmp ne i32 %reg_29, 0
	br i1 %reg_30, label %Block8, label %Block10
Block5:  ;If then at line 140
	br label %Block4
Block6:  ;If end at line 140
	%reg_21 = load i32, ptr %reg_2
	%reg_22 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	%reg_24 = load i32, ptr %reg_1
	%reg_25 = call i32 @lt(i32 %reg_23, i32 %reg_24)
	%reg_26 = load i32, ptr %reg_2
	%reg_27 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_25, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	store i32 %reg_28, ptr %reg_2
	br label %Block2
Block7:  ;Break at line 140
	br label %Block6
Block8:  ;If then at line 143
	%reg_31 = load i32, ptr %reg_2
	%reg_32 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_31
	%reg_33 = load i32, ptr %reg_32
	%reg_34 = add i32 1, 0
	%reg_35 = icmp sgt i32 %reg_33, %reg_34
	br i1 %reg_35, label %Block11, label %Block12
Block9:  ;If end at line 143
	ret void
Block10:  ;If else at line 143
	br label %Block27
Block11:  ;If then at line 145
	%reg_36 = load i32, ptr %reg_2
	%reg_37 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_36
	%reg_38 = load i32, ptr %reg_2
	%reg_39 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_38
	%reg_40 = load i32, ptr %reg_39
	%reg_41 = add i32 1, 0
	%reg_42 = sub i32 %reg_40, %reg_41
	store i32 %reg_42, ptr %reg_37
	ret void
	br label %Block12
Block12:  ;If end at line 145
	%reg_44 = load i32, ptr %reg_2
	store i32 %reg_44, ptr %reg_43
	%reg_46 = add i32 0, 0
	store i32 %reg_46, ptr %reg_45
	%reg_48 = load i32, ptr %reg_43
	%reg_49 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	store i32 %reg_50, ptr %reg_47
	%reg_51 = add i32 0, 0
	%reg_52 = load i32, ptr %reg_2
	%reg_53 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_51, i32 %reg_52
	%reg_54 = load i32, ptr %reg_53
	%reg_55 = icmp eq i32 %reg_54, 0
	br i1 %reg_55, label %Block13, label %Block15
Block13:  ;If then at line 147
	%reg_56 = add i32 1, 0
	%reg_57 = load i32, ptr %reg_2
	%reg_58 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_56, i32 %reg_57
	%reg_59 = load i32, ptr %reg_58
	store i32 %reg_59, ptr %reg_45
	%reg_60 = load i32, ptr %reg_2
	%reg_61 = add i32 1, 0
	%reg_62 = load i32, ptr %reg_2
	%reg_63 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_61, i32 %reg_62
	%reg_64 = load i32, ptr %reg_63
	call void @transplant(i32 %reg_60, i32 %reg_64)
	br label %Block14
Block14:  ;If end at line 147
	%reg_155 = load i32, ptr %reg_47
	%reg_156 = icmp eq i32 %reg_155, 0
	br i1 %reg_156, label %Block25, label %Block26
Block15:  ;If else at line 147
	%reg_65 = add i32 1, 0
	%reg_66 = load i32, ptr %reg_2
	%reg_67 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_65, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	%reg_69 = icmp eq i32 %reg_68, 0
	br i1 %reg_69, label %Block16, label %Block18
Block16:  ;If then at line 148
	%reg_70 = add i32 0, 0
	%reg_71 = load i32, ptr %reg_2
	%reg_72 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_70, i32 %reg_71
	%reg_73 = load i32, ptr %reg_72
	store i32 %reg_73, ptr %reg_45
	%reg_74 = load i32, ptr %reg_2
	%reg_75 = add i32 0, 0
	%reg_76 = load i32, ptr %reg_2
	%reg_77 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_75, i32 %reg_76
	%reg_78 = load i32, ptr %reg_77
	call void @transplant(i32 %reg_74, i32 %reg_78)
	br label %Block17
Block17:  ;If end at line 148
	br label %Block14
Block18:  ;If else at line 148
	%reg_79 = add i32 1, 0
	%reg_80 = load i32, ptr %reg_2
	%reg_81 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_79, i32 %reg_80
	%reg_82 = load i32, ptr %reg_81
	%reg_83 = call i32 @findMin(i32 %reg_82)
	store i32 %reg_83, ptr %reg_43
	%reg_84 = load i32, ptr %reg_43
	%reg_85 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_84
	%reg_86 = load i32, ptr %reg_85
	store i32 %reg_86, ptr %reg_47
	%reg_87 = add i32 1, 0
	%reg_88 = load i32, ptr %reg_43
	%reg_89 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_87, i32 %reg_88
	%reg_90 = load i32, ptr %reg_89
	store i32 %reg_90, ptr %reg_45
	%reg_91 = load i32, ptr %reg_43
	%reg_92 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_91
	%reg_93 = load i32, ptr %reg_92
	%reg_94 = load i32, ptr %reg_2
	%reg_95 = icmp eq i32 %reg_93, %reg_94
	br i1 %reg_95, label %Block19, label %Block21
Block19:  ;If then at line 152
	%reg_96 = load i32, ptr %reg_45
	%reg_97 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_96
	%reg_98 = load i32, ptr %reg_43
	store i32 %reg_98, ptr %reg_97
	br label %Block20
Block20:  ;If end at line 152
	%reg_134 = load i32, ptr %reg_2
	%reg_135 = load i32, ptr %reg_43
	call void @transplant(i32 %reg_134, i32 %reg_135)
	%reg_136 = add i32 0, 0
	%reg_137 = load i32, ptr %reg_43
	%reg_138 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_136, i32 %reg_137
	%reg_139 = add i32 0, 0
	%reg_140 = load i32, ptr %reg_2
	%reg_141 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_139, i32 %reg_140
	%reg_142 = load i32, ptr %reg_141
	store i32 %reg_142, ptr %reg_138
	%reg_143 = add i32 0, 0
	%reg_144 = load i32, ptr %reg_43
	%reg_145 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_143, i32 %reg_144
	%reg_146 = load i32, ptr %reg_145
	%reg_147 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_146
	%reg_148 = load i32, ptr %reg_43
	store i32 %reg_148, ptr %reg_147
	%reg_149 = load i32, ptr %reg_43
	%reg_150 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_149
	%reg_151 = load i32, ptr %reg_2
	%reg_152 = getelementptr [10000000 x i32], ptr @col, i32 0, i32 %reg_151
	%reg_153 = load i32, ptr %reg_152
	store i32 %reg_153, ptr %reg_150
	%reg_154 = load i32, ptr %reg_43
	call void @update1(i32 %reg_154)
	br label %Block17
Block21:  ;If else at line 152
	%reg_100 = load i32, ptr %reg_43
	store i32 %reg_100, ptr %reg_99
	br label %Block22
Block22:  ;While condition at line 156
	%reg_101 = load i32, ptr %reg_99
	%reg_102 = load i32, ptr %reg_2
	%reg_103 = icmp ne i32 %reg_101, %reg_102
	br i1 %reg_103, label %Block23, label %Block24
Block23:  ;While body at line 156
	%reg_104 = load i32, ptr %reg_99
	%reg_105 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_104
	%reg_106 = load i32, ptr %reg_99
	%reg_107 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_106
	%reg_108 = load i32, ptr %reg_107
	%reg_109 = load i32, ptr %reg_43
	%reg_110 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_109
	%reg_111 = load i32, ptr %reg_110
	%reg_112 = sub i32 %reg_108, %reg_111
	store i32 %reg_112, ptr %reg_105
	%reg_113 = load i32, ptr %reg_99
	%reg_114 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_113
	%reg_115 = load i32, ptr %reg_114
	store i32 %reg_115, ptr %reg_99
	br label %Block22
Block24:  ;While end at line 156
	%reg_116 = load i32, ptr %reg_43
	%reg_117 = add i32 1, 0
	%reg_118 = load i32, ptr %reg_43
	%reg_119 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_117, i32 %reg_118
	%reg_120 = load i32, ptr %reg_119
	call void @transplant(i32 %reg_116, i32 %reg_120)
	%reg_121 = add i32 1, 0
	%reg_122 = load i32, ptr %reg_43
	%reg_123 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_121, i32 %reg_122
	%reg_124 = add i32 1, 0
	%reg_125 = load i32, ptr %reg_2
	%reg_126 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_124, i32 %reg_125
	%reg_127 = load i32, ptr %reg_126
	store i32 %reg_127, ptr %reg_123
	%reg_128 = add i32 1, 0
	%reg_129 = load i32, ptr %reg_43
	%reg_130 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_128, i32 %reg_129
	%reg_131 = load i32, ptr %reg_130
	%reg_132 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_131
	%reg_133 = load i32, ptr %reg_43
	store i32 %reg_133, ptr %reg_132
	br label %Block20
Block25:  ;If then at line 163
	%reg_157 = load i32, ptr %reg_45
	call void @delFixUp(i32 %reg_157)
	br label %Block26
Block26:  ;If end at line 163
	br label %Block9
Block27:  ;While condition at line 165
	%reg_158 = load i32, ptr %reg_4
	%reg_159 = icmp ne i32 %reg_158, 0
	br i1 %reg_159, label %Block28, label %Block29
Block28:  ;While body at line 165
	%reg_160 = load i32, ptr %reg_4
	%reg_161 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_160
	%reg_162 = load i32, ptr %reg_4
	%reg_163 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_162
	%reg_164 = load i32, ptr %reg_163
	%reg_165 = add i32 1, 0
	%reg_166 = add i32 %reg_164, %reg_165
	store i32 %reg_166, ptr %reg_161
	%reg_167 = load i32, ptr %reg_4
	%reg_168 = getelementptr [10000000 x i32], ptr @fa, i32 0, i32 %reg_167
	%reg_169 = load i32, ptr %reg_168
	store i32 %reg_169, ptr %reg_4
	br label %Block27
Block29:  ;While end at line 165
	br label %Block9
}

define i32 @cmp(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 168
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 168
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = call i32 @lt(i32 %reg_6, i32 %reg_7)
	%reg_9 = add i32 0, 0
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	%reg_13 = load i32, ptr %reg_3
	%reg_14 = call i32 @ne(i32 %reg_12, i32 %reg_13)
	%reg_15 = add i32 1, 0
	%reg_16 = add i32 1, 0
	%reg_17 = sub i32 0, %reg_16
	%reg_18 = call i32 @select(i32 %reg_14, i32 %reg_15, i32 %reg_17)
	%reg_19 = call i32 @select(i32 %reg_8, i32 %reg_9, i32 %reg_18)
	ret i32 %reg_19
}

define i32 @suc2(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 169
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 169
	%reg_5 = load i32, ptr @root
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = load i32, ptr @INT_MAX
	%reg_10 = sub i32 0, %reg_9
	store i32 %reg_10, ptr %reg_8
	br label %Block2
Block2:  ;While condition at line 172
	%reg_11 = load i32, ptr %reg_4
	%reg_12 = icmp ne i32 %reg_11, 0
	br i1 %reg_12, label %Block3, label %Block4
Block3:  ;While body at line 172
	%reg_13 = load i32, ptr %reg_4
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = call i32 @cmp(i32 %reg_13, i32 %reg_14)
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = icmp eq i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block5, label %Block7
Block4:  ;While end at line 172
	%reg_32 = load i32, ptr %reg_8
	%reg_33 = load i32, ptr @INT_MAX
	%reg_34 = sub i32 0, %reg_33
	%reg_35 = call i32 @ne(i32 %reg_32, i32 %reg_34)
	%reg_36 = load i32, ptr %reg_8
	%reg_37 = load i32, ptr %reg_3
	%reg_38 = load i32, ptr @INT_MAX
	%reg_39 = load i32, ptr @INT_MAX
	%reg_40 = sub i32 0, %reg_39
	%reg_41 = call i32 @select(i32 %reg_37, i32 %reg_38, i32 %reg_40)
	%reg_42 = call i32 @select(i32 %reg_35, i32 %reg_36, i32 %reg_41)
	ret i32 %reg_42
Block5:  ;If then at line 173
	%reg_18 = load i32, ptr %reg_4
	store i32 %reg_18, ptr %reg_6
	%reg_19 = load i32, ptr %reg_6
	%reg_20 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	store i32 %reg_21, ptr %reg_8
	%reg_22 = load i32, ptr %reg_3
	%reg_23 = icmp eq i32 %reg_22, 0
	%reg_24 = zext i1 %reg_23 to i32
	%reg_25 = load i32, ptr %reg_4
	%reg_26 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_24, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	store i32 %reg_27, ptr %reg_4
	br label %Block6
Block6:  ;If end at line 173
	br label %Block2
Block7:  ;If else at line 173
	%reg_28 = load i32, ptr %reg_3
	%reg_29 = load i32, ptr %reg_4
	%reg_30 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_28, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	store i32 %reg_31, ptr %reg_4
	br label %Block6
}

define i32 @rank(i32 %reg_0)
{
Block0:  ;Func define at line 178
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 178
	%reg_3 = load i32, ptr @root
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 181
	%reg_6 = load i32, ptr %reg_2
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 181
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = getelementptr [10000000 x i32], ptr @val, i32 0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	%reg_11 = load i32, ptr %reg_1
	%reg_12 = icmp slt i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block5, label %Block7
Block4:  ;While end at line 181
	%reg_33 = load i32, ptr %reg_4
	ret i32 %reg_33
Block5:  ;If then at line 183
	%reg_13 = load i32, ptr %reg_4
	%reg_14 = add i32 0, 0
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_14, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	%reg_18 = getelementptr [10000000 x i32], ptr @size, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = add i32 %reg_13, %reg_19
	%reg_21 = load i32, ptr %reg_2
	%reg_22 = getelementptr [10000000 x i32], ptr @cnt, i32 0, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	%reg_24 = add i32 %reg_20, %reg_23
	store i32 %reg_24, ptr %reg_4
	%reg_25 = add i32 1, 0
	%reg_26 = load i32, ptr %reg_2
	%reg_27 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_25, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	store i32 %reg_28, ptr %reg_2
	br label %Block6
Block6:  ;If end at line 183
	br label %Block2
Block7:  ;If else at line 183
	%reg_29 = add i32 0, 0
	%reg_30 = load i32, ptr %reg_2
	%reg_31 = getelementptr [2 x [10000000 x i32]], ptr @son, i32 0, i32 %reg_29, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	store i32 %reg_32, ptr %reg_2
	br label %Block6
}

define void @insert(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 190
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 190
	%reg_5 = add i32 1, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 1, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = load i32, ptr @n
	store i32 %reg_9, ptr %reg_8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	br label %Block2
Block2:  ;While condition at line 193
	%reg_12 = load i32, ptr %reg_6
	%reg_13 = load i32, ptr %reg_8
	%reg_14 = icmp ne i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 193
	%reg_15 = load i32, ptr %reg_4
	%reg_16 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	store i32 %reg_17, ptr @root
	%reg_18 = load i32, ptr %reg_3
	call void @insert1(i32 %reg_18)
	%reg_19 = load i32, ptr %reg_4
	%reg_20 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr @root
	store i32 %reg_21, ptr %reg_20
	%reg_22 = load i32, ptr %reg_6
	%reg_23 = load i32, ptr %reg_8
	%reg_24 = add i32 %reg_22, %reg_23
	%reg_25 = add i32 2, 0
	%reg_26 = sdiv i32 %reg_24, %reg_25
	store i32 %reg_26, ptr %reg_10
	%reg_27 = load i32, ptr %reg_4
	%reg_28 = add i32 2, 0
	%reg_29 = mul i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_4
	%reg_30 = load i32, ptr %reg_2
	%reg_31 = load i32, ptr %reg_10
	%reg_32 = icmp sle i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block5, label %Block7
Block4:  ;While end at line 193
	%reg_40 = load i32, ptr %reg_4
	%reg_41 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	store i32 %reg_42, ptr @root
	%reg_43 = load i32, ptr %reg_3
	call void @insert1(i32 %reg_43)
	%reg_44 = load i32, ptr %reg_4
	%reg_45 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_44
	%reg_46 = load i32, ptr @root
	store i32 %reg_46, ptr %reg_45
	ret void
Block5:  ;If then at line 197
	%reg_33 = load i32, ptr %reg_10
	store i32 %reg_33, ptr %reg_8
	br label %Block6
Block6:  ;If end at line 197
	br label %Block2
Block7:  ;If else at line 197
	%reg_34 = load i32, ptr %reg_10
	%reg_35 = add i32 1, 0
	%reg_36 = add i32 %reg_34, %reg_35
	store i32 %reg_36, ptr %reg_6
	%reg_37 = load i32, ptr %reg_4
	%reg_38 = add i32 1, 0
	%reg_39 = add i32 %reg_37, %reg_38
	store i32 %reg_39, ptr %reg_4
	br label %Block6
}

define void @update(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 203
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 203
	%reg_5 = add i32 1, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 1, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = load i32, ptr @n
	store i32 %reg_9, ptr %reg_8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	br label %Block2
Block2:  ;While condition at line 206
	%reg_12 = load i32, ptr %reg_6
	%reg_13 = load i32, ptr %reg_8
	%reg_14 = icmp ne i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 206
	%reg_15 = load i32, ptr %reg_4
	%reg_16 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	store i32 %reg_17, ptr @root
	%reg_18 = load i32, ptr %reg_3
	call void @insert1(i32 %reg_18)
	%reg_19 = load i32, ptr %reg_2
	%reg_20 = getelementptr [50010 x i32], ptr @a, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	call void @erase(i32 %reg_21)
	%reg_22 = load i32, ptr %reg_4
	%reg_23 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_22
	%reg_24 = load i32, ptr @root
	store i32 %reg_24, ptr %reg_23
	%reg_25 = load i32, ptr %reg_6
	%reg_26 = load i32, ptr %reg_8
	%reg_27 = add i32 %reg_25, %reg_26
	%reg_28 = add i32 2, 0
	%reg_29 = sdiv i32 %reg_27, %reg_28
	store i32 %reg_29, ptr %reg_10
	%reg_30 = load i32, ptr %reg_4
	%reg_31 = add i32 2, 0
	%reg_32 = mul i32 %reg_30, %reg_31
	store i32 %reg_32, ptr %reg_4
	%reg_33 = load i32, ptr %reg_2
	%reg_34 = load i32, ptr %reg_10
	%reg_35 = icmp sle i32 %reg_33, %reg_34
	br i1 %reg_35, label %Block5, label %Block7
Block4:  ;While end at line 206
	%reg_43 = load i32, ptr %reg_4
	%reg_44 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_43
	%reg_45 = load i32, ptr %reg_44
	store i32 %reg_45, ptr @root
	%reg_46 = load i32, ptr %reg_3
	call void @insert1(i32 %reg_46)
	%reg_47 = load i32, ptr %reg_2
	%reg_48 = getelementptr [50010 x i32], ptr @a, i32 0, i32 %reg_47
	%reg_49 = load i32, ptr %reg_48
	call void @erase(i32 %reg_49)
	%reg_50 = load i32, ptr %reg_4
	%reg_51 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_50
	%reg_52 = load i32, ptr @root
	store i32 %reg_52, ptr %reg_51
	%reg_53 = load i32, ptr %reg_2
	%reg_54 = getelementptr [50010 x i32], ptr @a, i32 0, i32 %reg_53
	%reg_55 = load i32, ptr %reg_3
	store i32 %reg_55, ptr %reg_54
	ret void
Block5:  ;If then at line 210
	%reg_36 = load i32, ptr %reg_10
	store i32 %reg_36, ptr %reg_8
	br label %Block6
Block6:  ;If end at line 210
	br label %Block2
Block7:  ;If else at line 210
	%reg_37 = load i32, ptr %reg_10
	%reg_38 = add i32 1, 0
	%reg_39 = add i32 %reg_37, %reg_38
	store i32 %reg_39, ptr %reg_6
	%reg_40 = load i32, ptr %reg_4
	%reg_41 = add i32 1, 0
	%reg_42 = add i32 %reg_40, %reg_41
	store i32 %reg_42, ptr %reg_4
	br label %Block6
}

define i32 @lower(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5)
{
Block0:  ;Func define at line 217
	%reg_40 = alloca i32
	%reg_36 = alloca i32
	%reg_30 = alloca i32
	%reg_11 = alloca i32
	%reg_10 = alloca i32
	%reg_9 = alloca i32
	%reg_8 = alloca i32
	%reg_7 = alloca i32
	%reg_6 = alloca i32
	store i32 %reg_0, ptr %reg_6
	store i32 %reg_1, ptr %reg_7
	store i32 %reg_2, ptr %reg_8
	store i32 %reg_3, ptr %reg_9
	store i32 %reg_4, ptr %reg_10
	store i32 %reg_5, ptr %reg_11
	br label %Block1
Block1:  ;Func body at line 217
	%reg_12 = load i32, ptr %reg_7
	%reg_13 = load i32, ptr %reg_10
	%reg_14 = icmp slt i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block2, label %Block4
Block2:  ;If then at line 219
	%reg_18 = add i32 0, 0
	ret i32 %reg_18
	br label %Block3
Block3:  ;If end at line 219
	%reg_19 = load i32, ptr %reg_9
	%reg_20 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	store i32 %reg_21, ptr @root
	%reg_22 = load i32, ptr %reg_6
	%reg_23 = load i32, ptr %reg_10
	%reg_24 = icmp sle i32 %reg_22, %reg_23
	br i1 %reg_24, label %Block7, label %Block6
Block4:  ;Or opertor at line 219
	%reg_15 = load i32, ptr %reg_11
	%reg_16 = load i32, ptr %reg_6
	%reg_17 = icmp slt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block2, label %Block3
Block5:  ;If then at line 221
	%reg_28 = load i32, ptr %reg_8
	%reg_29 = call i32 @rank(i32 %reg_28)
	ret i32 %reg_29
	br label %Block6
Block6:  ;If end at line 221
	%reg_31 = load i32, ptr %reg_10
	%reg_32 = load i32, ptr %reg_11
	%reg_33 = add i32 %reg_31, %reg_32
	%reg_34 = add i32 2, 0
	%reg_35 = sdiv i32 %reg_33, %reg_34
	store i32 %reg_35, ptr %reg_30
	%reg_37 = load i32, ptr %reg_9
	%reg_38 = add i32 2, 0
	%reg_39 = mul i32 %reg_37, %reg_38
	store i32 %reg_39, ptr %reg_36
	%reg_41 = load i32, ptr %reg_36
	%reg_42 = add i32 1, 0
	%reg_43 = add i32 %reg_41, %reg_42
	store i32 %reg_43, ptr %reg_40
	%reg_44 = load i32, ptr %reg_6
	%reg_45 = load i32, ptr %reg_7
	%reg_46 = load i32, ptr %reg_8
	%reg_47 = load i32, ptr %reg_36
	%reg_48 = load i32, ptr %reg_10
	%reg_49 = load i32, ptr %reg_30
	%reg_50 = call i32 @lower(i32 %reg_44, i32 %reg_45, i32 %reg_46, i32 %reg_47, i32 %reg_48, i32 %reg_49)
	%reg_51 = load i32, ptr %reg_6
	%reg_52 = load i32, ptr %reg_7
	%reg_53 = load i32, ptr %reg_8
	%reg_54 = load i32, ptr %reg_40
	%reg_55 = load i32, ptr %reg_30
	%reg_56 = add i32 1, 0
	%reg_57 = add i32 %reg_55, %reg_56
	%reg_58 = load i32, ptr %reg_11
	%reg_59 = call i32 @lower(i32 %reg_51, i32 %reg_52, i32 %reg_53, i32 %reg_54, i32 %reg_57, i32 %reg_58)
	%reg_60 = add i32 %reg_50, %reg_59
	ret i32 %reg_60
Block7:  ;And opertor at line 221
	%reg_25 = load i32, ptr %reg_11
	%reg_26 = load i32, ptr %reg_7
	%reg_27 = icmp sle i32 %reg_25, %reg_26
	br i1 %reg_27, label %Block5, label %Block6
}

define i32 @suc(i32 %reg_0, i32 %reg_1, i32 %reg_2, i32 %reg_3, i32 %reg_4, i32 %reg_5, i32 %reg_6)
{
Block0:  ;Func define at line 227
	%reg_60 = alloca i32
	%reg_51 = alloca i32
	%reg_47 = alloca i32
	%reg_43 = alloca i32
	%reg_37 = alloca i32
	%reg_13 = alloca i32
	%reg_12 = alloca i32
	%reg_11 = alloca i32
	%reg_10 = alloca i32
	%reg_9 = alloca i32
	%reg_8 = alloca i32
	%reg_7 = alloca i32
	store i32 %reg_0, ptr %reg_7
	store i32 %reg_1, ptr %reg_8
	store i32 %reg_2, ptr %reg_9
	store i32 %reg_3, ptr %reg_10
	store i32 %reg_4, ptr %reg_11
	store i32 %reg_5, ptr %reg_12
	store i32 %reg_6, ptr %reg_13
	br label %Block1
Block1:  ;Func body at line 227
	%reg_14 = load i32, ptr %reg_8
	%reg_15 = load i32, ptr %reg_12
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block2, label %Block4
Block2:  ;If then at line 229
	%reg_20 = load i32, ptr %reg_10
	%reg_21 = load i32, ptr @INT_MAX
	%reg_22 = load i32, ptr @INT_MAX
	%reg_23 = sub i32 0, %reg_22
	%reg_24 = call i32 @select(i32 %reg_20, i32 %reg_21, i32 %reg_23)
	ret i32 %reg_24
	br label %Block3
Block3:  ;If end at line 229
	%reg_25 = load i32, ptr %reg_11
	%reg_26 = getelementptr [200040 x i32], ptr @tree, i32 0, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	store i32 %reg_27, ptr @root
	%reg_28 = load i32, ptr %reg_7
	%reg_29 = load i32, ptr %reg_12
	%reg_30 = icmp sle i32 %reg_28, %reg_29
	br i1 %reg_30, label %Block7, label %Block6
Block4:  ;Or opertor at line 229
	%reg_17 = load i32, ptr %reg_13
	%reg_18 = load i32, ptr %reg_7
	%reg_19 = icmp slt i32 %reg_17, %reg_18
	br i1 %reg_19, label %Block2, label %Block3
Block5:  ;If then at line 231
	%reg_34 = load i32, ptr %reg_9
	%reg_35 = load i32, ptr %reg_10
	%reg_36 = call i32 @suc2(i32 %reg_34, i32 %reg_35)
	ret i32 %reg_36
	br label %Block6
Block6:  ;If end at line 231
	%reg_38 = load i32, ptr %reg_12
	%reg_39 = load i32, ptr %reg_13
	%reg_40 = add i32 %reg_38, %reg_39
	%reg_41 = add i32 2, 0
	%reg_42 = sdiv i32 %reg_40, %reg_41
	store i32 %reg_42, ptr %reg_37
	%reg_44 = load i32, ptr %reg_11
	%reg_45 = add i32 2, 0
	%reg_46 = mul i32 %reg_44, %reg_45
	store i32 %reg_46, ptr %reg_43
	%reg_48 = load i32, ptr %reg_43
	%reg_49 = add i32 1, 0
	%reg_50 = add i32 %reg_48, %reg_49
	store i32 %reg_50, ptr %reg_47
	%reg_52 = load i32, ptr %reg_7
	%reg_53 = load i32, ptr %reg_8
	%reg_54 = load i32, ptr %reg_9
	%reg_55 = load i32, ptr %reg_10
	%reg_56 = load i32, ptr %reg_43
	%reg_57 = load i32, ptr %reg_12
	%reg_58 = load i32, ptr %reg_37
	%reg_59 = call i32 @suc(i32 %reg_52, i32 %reg_53, i32 %reg_54, i32 %reg_55, i32 %reg_56, i32 %reg_57, i32 %reg_58)
	store i32 %reg_59, ptr %reg_51
	%reg_61 = load i32, ptr %reg_7
	%reg_62 = load i32, ptr %reg_8
	%reg_63 = load i32, ptr %reg_9
	%reg_64 = load i32, ptr %reg_10
	%reg_65 = load i32, ptr %reg_47
	%reg_66 = load i32, ptr %reg_37
	%reg_67 = add i32 1, 0
	%reg_68 = add i32 %reg_66, %reg_67
	%reg_69 = load i32, ptr %reg_13
	%reg_70 = call i32 @suc(i32 %reg_61, i32 %reg_62, i32 %reg_63, i32 %reg_64, i32 %reg_65, i32 %reg_68, i32 %reg_69)
	store i32 %reg_70, ptr %reg_60
	%reg_71 = load i32, ptr %reg_10
	%reg_72 = icmp ne i32 %reg_71, 0
	br i1 %reg_72, label %Block8, label %Block10
Block7:  ;And opertor at line 231
	%reg_31 = load i32, ptr %reg_13
	%reg_32 = load i32, ptr %reg_8
	%reg_33 = icmp sle i32 %reg_31, %reg_32
	br i1 %reg_33, label %Block5, label %Block6
Block8:  ;If then at line 235
	%reg_73 = load i32, ptr %reg_51
	%reg_74 = load i32, ptr %reg_60
	%reg_75 = call i32 @min(i32 %reg_73, i32 %reg_74)
	ret i32 %reg_75
	br label %Block9
Block9:  ;If end at line 235
	ret i32 0
Block10:  ;If else at line 235
	%reg_76 = load i32, ptr %reg_51
	%reg_77 = load i32, ptr %reg_60
	%reg_78 = call i32 @max(i32 %reg_76, i32 %reg_77)
	ret i32 %reg_78
	br label %Block9
}

define i32 @k_th(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 242
	%reg_12 = alloca i32
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 242
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = add i32 100000001, 0
	store i32 %reg_9, ptr %reg_8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	br label %Block2
Block2:  ;While condition at line 245
	%reg_14 = load i32, ptr %reg_6
	%reg_15 = load i32, ptr %reg_8
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block3, label %Block4
Block3:  ;While body at line 245
	%reg_17 = load i32, ptr %reg_6
	%reg_18 = load i32, ptr %reg_8
	%reg_19 = add i32 %reg_17, %reg_18
	%reg_20 = add i32 2, 0
	%reg_21 = sdiv i32 %reg_19, %reg_20
	store i32 %reg_21, ptr %reg_10
	%reg_22 = load i32, ptr %reg_3
	%reg_23 = load i32, ptr %reg_4
	%reg_24 = load i32, ptr %reg_10
	%reg_25 = add i32 1, 0
	%reg_26 = add i32 1, 0
	%reg_27 = load i32, ptr @n
	%reg_28 = call i32 @lower(i32 %reg_22, i32 %reg_23, i32 %reg_24, i32 %reg_25, i32 %reg_26, i32 %reg_27)
	store i32 %reg_28, ptr %reg_12
	%reg_29 = load i32, ptr %reg_12
	%reg_30 = load i32, ptr %reg_5
	%reg_31 = icmp slt i32 %reg_29, %reg_30
	br i1 %reg_31, label %Block5, label %Block7
Block4:  ;While end at line 245
	%reg_36 = load i32, ptr %reg_6
	%reg_37 = add i32 1, 0
	%reg_38 = sub i32 %reg_36, %reg_37
	ret i32 %reg_38
Block5:  ;If then at line 248
	%reg_32 = load i32, ptr %reg_10
	%reg_33 = add i32 1, 0
	%reg_34 = add i32 %reg_32, %reg_33
	store i32 %reg_34, ptr %reg_6
	br label %Block6
Block6:  ;If end at line 248
	br label %Block2
Block7:  ;If else at line 248
	%reg_35 = load i32, ptr %reg_10
	store i32 %reg_35, ptr %reg_8
	br label %Block6
}

define i32 @main()
{
Block0:  ;Func define at line 253
	%reg_35 = alloca i32
	%reg_33 = alloca i32
	%reg_31 = alloca i32
	%reg_29 = alloca i32
	%reg_2 = alloca i32
	br label %Block1
Block1:  ;Func body at line 253
	%reg_0 = call i32 @getint()
	store i32 %reg_0, ptr @n
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr @m
	%reg_3 = add i32 1, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 258
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @n
	%reg_6 = icmp sle i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 258
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = getelementptr [50010 x i32], ptr @a, i32 0, i32 %reg_7
	%reg_9 = call i32 @getint()
	store i32 %reg_9, ptr %reg_8
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = add i32 1, 0
	%reg_12 = add i32 %reg_10, %reg_11
	store i32 %reg_12, ptr %reg_2
	br label %Block2
Block4:  ;While end at line 258
	%reg_13 = add i32 1, 0
	store i32 %reg_13, ptr %reg_2
	br label %Block5
Block5:  ;While condition at line 264
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = load i32, ptr @n
	%reg_16 = icmp sle i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block6, label %Block7
Block6:  ;While body at line 264
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = load i32, ptr %reg_2
	%reg_19 = getelementptr [50010 x i32], ptr @a, i32 0, i32 %reg_18
	%reg_20 = load i32, ptr %reg_19
	call void @insert(i32 %reg_17, i32 %reg_20)
	%reg_21 = load i32, ptr %reg_2
	%reg_22 = add i32 1, 0
	%reg_23 = add i32 %reg_21, %reg_22
	store i32 %reg_23, ptr %reg_2
	br label %Block5
Block7:  ;While end at line 264
	br label %Block8
Block8:  ;While condition at line 269
	%reg_24 = load i32, ptr @m
	%reg_25 = icmp ne i32 %reg_24, 0
	br i1 %reg_25, label %Block9, label %Block10
Block9:  ;While body at line 269
	%reg_26 = load i32, ptr @m
	%reg_27 = add i32 1, 0
	%reg_28 = sub i32 %reg_26, %reg_27
	store i32 %reg_28, ptr @m
	%reg_30 = call i32 @getint()
	store i32 %reg_30, ptr %reg_29
	%reg_32 = call i32 @getint()
	store i32 %reg_32, ptr %reg_31
	%reg_34 = call i32 @getint()
	store i32 %reg_34, ptr %reg_33
	%reg_36 = add i32 0, 0
	store i32 %reg_36, ptr %reg_35
	%reg_37 = load i32, ptr %reg_29
	%reg_38 = add i32 1, 0
	%reg_39 = icmp eq i32 %reg_37, %reg_38
	br i1 %reg_39, label %Block11, label %Block12
Block10:  ;While end at line 269
	%reg_83 = add i32 0, 0
	ret i32 %reg_83
Block11:  ;If then at line 273
	%reg_40 = call i32 @getint()
	store i32 %reg_40, ptr %reg_35
	%reg_41 = load i32, ptr %reg_31
	%reg_42 = load i32, ptr %reg_33
	%reg_43 = load i32, ptr %reg_35
	%reg_44 = add i32 1, 0
	%reg_45 = add i32 1, 0
	%reg_46 = load i32, ptr @n
	%reg_47 = call i32 @lower(i32 %reg_41, i32 %reg_42, i32 %reg_43, i32 %reg_44, i32 %reg_45, i32 %reg_46)
	%reg_48 = add i32 1, 0
	%reg_49 = add i32 %reg_47, %reg_48
	call void @putint(i32 %reg_49)
	%reg_50 = add i32 10, 0
	call void @putch(i32 %reg_50)
	br label %Block12
Block12:  ;If end at line 273
	%reg_51 = load i32, ptr %reg_29
	%reg_52 = add i32 2, 0
	%reg_53 = icmp eq i32 %reg_51, %reg_52
	br i1 %reg_53, label %Block13, label %Block14
Block13:  ;If then at line 276
	%reg_54 = call i32 @getint()
	store i32 %reg_54, ptr %reg_35
	%reg_55 = load i32, ptr %reg_31
	%reg_56 = load i32, ptr %reg_33
	%reg_57 = load i32, ptr %reg_35
	%reg_58 = call i32 @k_th(i32 %reg_55, i32 %reg_56, i32 %reg_57)
	call void @putint(i32 %reg_58)
	%reg_59 = add i32 10, 0
	call void @putch(i32 %reg_59)
	br label %Block14
Block14:  ;If end at line 276
	%reg_60 = load i32, ptr %reg_29
	%reg_61 = add i32 3, 0
	%reg_62 = icmp eq i32 %reg_60, %reg_61
	br i1 %reg_62, label %Block15, label %Block16
Block15:  ;If then at line 279
	%reg_63 = load i32, ptr %reg_31
	%reg_64 = load i32, ptr %reg_33
	call void @update(i32 %reg_63, i32 %reg_64)
	br label %Block16
Block16:  ;If end at line 279
	%reg_65 = load i32, ptr %reg_29
	%reg_66 = add i32 4, 0
	%reg_67 = icmp eq i32 %reg_65, %reg_66
	br i1 %reg_67, label %Block17, label %Block19
Block17:  ;If then at line 282
	%reg_71 = call i32 @getint()
	store i32 %reg_71, ptr %reg_35
	%reg_72 = load i32, ptr %reg_31
	%reg_73 = load i32, ptr %reg_33
	%reg_74 = load i32, ptr %reg_35
	%reg_75 = load i32, ptr %reg_29
	%reg_76 = add i32 4, 0
	%reg_77 = sub i32 %reg_75, %reg_76
	%reg_78 = add i32 1, 0
	%reg_79 = add i32 1, 0
	%reg_80 = load i32, ptr @n
	%reg_81 = call i32 @suc(i32 %reg_72, i32 %reg_73, i32 %reg_74, i32 %reg_77, i32 %reg_78, i32 %reg_79, i32 %reg_80)
	call void @putint(i32 %reg_81)
	%reg_82 = add i32 10, 0
	call void @putch(i32 %reg_82)
	br label %Block18
Block18:  ;If end at line 282
	br label %Block8
Block19:  ;Or opertor at line 282
	%reg_68 = load i32, ptr %reg_29
	%reg_69 = add i32 5, 0
	%reg_70 = icmp eq i32 %reg_68, %reg_69
	br i1 %reg_70, label %Block17, label %Block18
}
