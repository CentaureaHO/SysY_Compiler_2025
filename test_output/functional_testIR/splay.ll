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

@N = global i32 100005
@inf = global i32 1000000000
@data = global [100005 x i32] zeroinitializer
@ch = global [100005 x [2 x i32]] zeroinitializer
@fa = global [100005 x i32] zeroinitializer
@siz = global [100005 x i32] zeroinitializer
@root = global i32 zeroinitializer
@tag = global [100005 x i32] zeroinitializer
@tot = global i32 zeroinitializer
@a = global [100005 x i32] zeroinitializer
@cnt = global [100005 x i32] zeroinitializer

define i32 @eq(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 4
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 4
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 6
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 6
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
}

define i32 @son(i32 %reg_0)
{
Block0:  ;Func define at line 12
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 12
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = add i32 1, 0
	%reg_7 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_5, i32 %reg_6
	%reg_8 = load i32, ptr %reg_7
	%reg_9 = call i32 @eq(i32 %reg_2, i32 %reg_8)
	ret i32 %reg_9
}

define void @push_up(i32 %reg_0)
{
Block0:  ;Func define at line 13
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 13
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = getelementptr [100005 x i32], ptr @siz, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_1
	%reg_5 = add i32 0, 0
	%reg_6 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_4, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	%reg_8 = getelementptr [100005 x i32], ptr @siz, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = add i32 1, 0
	%reg_12 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = getelementptr [100005 x i32], ptr @siz, i32 0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	%reg_16 = add i32 %reg_9, %reg_15
	%reg_17 = load i32, ptr %reg_1
	%reg_18 = getelementptr [100005 x i32], ptr @cnt, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	%reg_20 = add i32 %reg_16, %reg_19
	store i32 %reg_20, ptr %reg_3
	ret void
}

define i32 @xor_01(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 15
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 15
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = add i32 1, 0
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block5, label %Block4
Block2:  ;If then at line 17
	%reg_10 = add i32 0, 0
	ret i32 %reg_10
	br label %Block3
Block3:  ;If end at line 17
	ret i32 0
Block4:  ;If else at line 17
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = add i32 0, 0
	%reg_13 = icmp eq i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block9, label %Block8
Block5:  ;And opertor at line 17
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = add i32 1, 0
	%reg_9 = icmp eq i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block2, label %Block4
Block6:  ;If then at line 19
	%reg_17 = add i32 0, 0
	ret i32 %reg_17
	br label %Block7
Block7:  ;If end at line 19
	br label %Block3
Block8:  ;If else at line 19
	%reg_18 = add i32 1, 0
	ret i32 %reg_18
	br label %Block7
Block9:  ;And opertor at line 19
	%reg_14 = load i32, ptr %reg_3
	%reg_15 = add i32 0, 0
	%reg_16 = icmp eq i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block6, label %Block8
}

define void @push_down(i32 %reg_0)
{
Block0:  ;Func define at line 26
	%reg_26 = alloca i32
	%reg_12 = alloca i32
	%reg_7 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 26
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_2
	%reg_4 = load i32, ptr %reg_3
	%reg_5 = add i32 0, 0
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 27
	ret void
	br label %Block3
Block3:  ;If end at line 27
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = add i32 0, 0
	%reg_10 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	store i32 %reg_11, ptr %reg_7
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = add i32 1, 0
	%reg_15 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_13, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	store i32 %reg_16, ptr %reg_12
	%reg_17 = load i32, ptr %reg_7
	%reg_18 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_17
	%reg_19 = load i32, ptr %reg_7
	%reg_20 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = load i32, ptr %reg_1
	%reg_23 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = call i32 @xor_01(i32 %reg_21, i32 %reg_24)
	store i32 %reg_25, ptr %reg_18
	%reg_27 = load i32, ptr %reg_7
	%reg_28 = add i32 0, 0
	%reg_29 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_27, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	store i32 %reg_30, ptr %reg_26
	%reg_31 = load i32, ptr %reg_7
	%reg_32 = add i32 0, 0
	%reg_33 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_31, i32 %reg_32
	%reg_34 = load i32, ptr %reg_7
	%reg_35 = add i32 1, 0
	%reg_36 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_34, i32 %reg_35
	%reg_37 = load i32, ptr %reg_36
	store i32 %reg_37, ptr %reg_33
	%reg_38 = load i32, ptr %reg_7
	%reg_39 = add i32 1, 0
	%reg_40 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_38, i32 %reg_39
	%reg_41 = load i32, ptr %reg_26
	store i32 %reg_41, ptr %reg_40
	%reg_42 = load i32, ptr %reg_12
	%reg_43 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_42
	%reg_44 = load i32, ptr %reg_12
	%reg_45 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_44
	%reg_46 = load i32, ptr %reg_45
	%reg_47 = load i32, ptr %reg_1
	%reg_48 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_47
	%reg_49 = load i32, ptr %reg_48
	%reg_50 = call i32 @xor_01(i32 %reg_46, i32 %reg_49)
	store i32 %reg_50, ptr %reg_43
	%reg_51 = load i32, ptr %reg_12
	%reg_52 = add i32 0, 0
	%reg_53 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_51, i32 %reg_52
	%reg_54 = load i32, ptr %reg_53
	store i32 %reg_54, ptr %reg_26
	%reg_55 = load i32, ptr %reg_12
	%reg_56 = add i32 0, 0
	%reg_57 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_55, i32 %reg_56
	%reg_58 = load i32, ptr %reg_12
	%reg_59 = add i32 1, 0
	%reg_60 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_58, i32 %reg_59
	%reg_61 = load i32, ptr %reg_60
	store i32 %reg_61, ptr %reg_57
	%reg_62 = load i32, ptr %reg_12
	%reg_63 = add i32 1, 0
	%reg_64 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_62, i32 %reg_63
	%reg_65 = load i32, ptr %reg_26
	store i32 %reg_65, ptr %reg_64
	%reg_66 = load i32, ptr %reg_1
	%reg_67 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_66
	%reg_68 = add i32 0, 0
	store i32 %reg_68, ptr %reg_67
	ret void
}

define void @rotate(i32 %reg_0)
{
Block0:  ;Func define at line 46
	%reg_16 = alloca i32
	%reg_13 = alloca i32
	%reg_10 = alloca i32
	%reg_6 = alloca i32
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 46
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	store i32 %reg_5, ptr %reg_2
	%reg_7 = load i32, ptr %reg_2
	%reg_8 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	store i32 %reg_9, ptr %reg_6
	%reg_11 = load i32, ptr %reg_1
	%reg_12 = call i32 @son(i32 %reg_11)
	store i32 %reg_12, ptr %reg_10
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = call i32 @son(i32 %reg_14)
	store i32 %reg_15, ptr %reg_13
	%reg_17 = load i32, ptr %reg_1
	%reg_18 = load i32, ptr %reg_10
	%reg_19 = icmp eq i32 %reg_18, 0
	%reg_20 = zext i1 %reg_19 to i32
	%reg_21 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_17, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	store i32 %reg_22, ptr %reg_16
	%reg_23 = load i32, ptr %reg_2
	call void @push_down(i32 %reg_23)
	%reg_24 = load i32, ptr %reg_1
	call void @push_down(i32 %reg_24)
	%reg_25 = load i32, ptr %reg_6
	%reg_26 = icmp ne i32 %reg_25, 0
	br i1 %reg_26, label %Block2, label %Block4
Block2:  ;If then at line 49
	%reg_27 = load i32, ptr %reg_6
	%reg_28 = load i32, ptr %reg_13
	%reg_29 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_27, i32 %reg_28
	%reg_30 = load i32, ptr %reg_1
	store i32 %reg_30, ptr %reg_29
	br label %Block3
Block3:  ;If end at line 49
	%reg_32 = load i32, ptr %reg_1
	%reg_33 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_32
	%reg_34 = load i32, ptr %reg_6
	store i32 %reg_34, ptr %reg_33
	%reg_35 = load i32, ptr %reg_1
	%reg_36 = load i32, ptr %reg_10
	%reg_37 = icmp eq i32 %reg_36, 0
	%reg_38 = zext i1 %reg_37 to i32
	%reg_39 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_35, i32 %reg_38
	%reg_40 = load i32, ptr %reg_2
	store i32 %reg_40, ptr %reg_39
	%reg_41 = load i32, ptr %reg_2
	%reg_42 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_41
	%reg_43 = load i32, ptr %reg_1
	store i32 %reg_43, ptr %reg_42
	%reg_44 = load i32, ptr %reg_2
	%reg_45 = load i32, ptr %reg_10
	%reg_46 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_44, i32 %reg_45
	%reg_47 = load i32, ptr %reg_16
	store i32 %reg_47, ptr %reg_46
	%reg_48 = load i32, ptr %reg_16
	%reg_49 = icmp ne i32 %reg_48, 0
	br i1 %reg_49, label %Block5, label %Block6
Block4:  ;If else at line 49
	%reg_31 = load i32, ptr %reg_1
	store i32 %reg_31, ptr @root
	br label %Block3
Block5:  ;If then at line 51
	%reg_50 = load i32, ptr %reg_16
	%reg_51 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_50
	%reg_52 = load i32, ptr %reg_2
	store i32 %reg_52, ptr %reg_51
	br label %Block6
Block6:  ;If end at line 51
	%reg_53 = load i32, ptr %reg_2
	call void @push_up(i32 %reg_53)
	%reg_54 = load i32, ptr %reg_1
	call void @push_up(i32 %reg_54)
	ret void
}

define void @splay(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 55
	%reg_13 = alloca i32
	%reg_9 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 55
	br label %Block2
Block2:  ;While condition at line 56
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = icmp ne i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 56
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr %reg_11
	store i32 %reg_12, ptr %reg_9
	%reg_14 = load i32, ptr %reg_9
	%reg_15 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	store i32 %reg_16, ptr %reg_13
	%reg_17 = load i32, ptr %reg_13
	%reg_18 = load i32, ptr %reg_3
	%reg_19 = icmp eq i32 %reg_17, %reg_18
	br i1 %reg_19, label %Block5, label %Block7
Block4:  ;While end at line 56
	ret void
Block5:  ;If then at line 58
	%reg_20 = load i32, ptr %reg_2
	call void @rotate(i32 %reg_20)
	br label %Block6
Block6:  ;If end at line 58
	br label %Block2
Block7:  ;If else at line 58
	%reg_21 = load i32, ptr %reg_2
	%reg_22 = call i32 @son(i32 %reg_21)
	%reg_23 = load i32, ptr %reg_9
	%reg_24 = call i32 @son(i32 %reg_23)
	%reg_25 = call i32 @xor_01(i32 %reg_22, i32 %reg_24)
	%reg_26 = icmp ne i32 %reg_25, 0
	br i1 %reg_26, label %Block8, label %Block10
Block8:  ;If then at line 59
	%reg_27 = load i32, ptr %reg_2
	call void @rotate(i32 %reg_27)
	%reg_28 = load i32, ptr %reg_2
	call void @rotate(i32 %reg_28)
	br label %Block9
Block9:  ;If end at line 59
	br label %Block6
Block10:  ;If else at line 59
	%reg_29 = load i32, ptr %reg_9
	call void @rotate(i32 %reg_29)
	%reg_30 = load i32, ptr %reg_2
	call void @rotate(i32 %reg_30)
	br label %Block9
}

define i32 @get_kth(i32 %reg_0)
{
Block0:  ;Func define at line 64
	%reg_5 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 64
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 1, 0
	%reg_4 = sub i32 %reg_2, %reg_3
	store i32 %reg_4, ptr %reg_1
	%reg_6 = load i32, ptr @root
	store i32 %reg_6, ptr %reg_5
	br label %Block2
Block2:  ;While condition at line 67
	%reg_7 = load i32, ptr %reg_5
	%reg_8 = icmp ne i32 %reg_7, 0
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 67
	%reg_9 = load i32, ptr %reg_5
	call void @putint(i32 %reg_9)
	%reg_10 = add i32 32, 0
	call void @putch(i32 %reg_10)
	%reg_11 = load i32, ptr %reg_5
	%reg_12 = add i32 0, 0
	%reg_13 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_11, i32 %reg_12
	%reg_14 = load i32, ptr %reg_13
	call void @putint(i32 %reg_14)
	%reg_15 = add i32 32, 0
	call void @putch(i32 %reg_15)
	%reg_16 = load i32, ptr %reg_5
	%reg_17 = add i32 1, 0
	%reg_18 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_16, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	call void @putint(i32 %reg_19)
	%reg_20 = add i32 10, 0
	call void @putch(i32 %reg_20)
	%reg_21 = load i32, ptr %reg_5
	%reg_22 = add i32 0, 0
	%reg_23 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_21, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	%reg_25 = getelementptr [100005 x i32], ptr @siz, i32 0, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = load i32, ptr %reg_1
	%reg_28 = icmp sgt i32 %reg_26, %reg_27
	br i1 %reg_28, label %Block5, label %Block7
Block4:  ;While end at line 67
	%reg_64 = add i32 0, 0
	ret i32 %reg_64
Block5:  ;If then at line 69
	%reg_29 = load i32, ptr %reg_5
	call void @push_down(i32 %reg_29)
	%reg_30 = load i32, ptr %reg_5
	%reg_31 = add i32 0, 0
	%reg_32 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_30, i32 %reg_31
	%reg_33 = load i32, ptr %reg_32
	store i32 %reg_33, ptr %reg_5
	br label %Block6
Block6:  ;If end at line 69
	br label %Block2
Block7:  ;If else at line 69
	%reg_34 = load i32, ptr %reg_5
	%reg_35 = add i32 0, 0
	%reg_36 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_34, i32 %reg_35
	%reg_37 = load i32, ptr %reg_36
	%reg_38 = getelementptr [100005 x i32], ptr @siz, i32 0, i32 %reg_37
	%reg_39 = load i32, ptr %reg_38
	%reg_40 = load i32, ptr %reg_5
	%reg_41 = getelementptr [100005 x i32], ptr @cnt, i32 0, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	%reg_43 = add i32 %reg_39, %reg_42
	%reg_44 = load i32, ptr %reg_1
	%reg_45 = icmp sgt i32 %reg_43, %reg_44
	br i1 %reg_45, label %Block8, label %Block10
Block8:  ;If then at line 70
	%reg_46 = load i32, ptr %reg_5
	ret i32 %reg_46
	br label %Block9
Block9:  ;If end at line 70
	br label %Block6
Block10:  ;If else at line 70
	%reg_47 = load i32, ptr %reg_1
	%reg_48 = load i32, ptr %reg_5
	%reg_49 = add i32 0, 0
	%reg_50 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_48, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = getelementptr [100005 x i32], ptr @siz, i32 0, i32 %reg_51
	%reg_53 = load i32, ptr %reg_52
	%reg_54 = load i32, ptr %reg_5
	%reg_55 = getelementptr [100005 x i32], ptr @cnt, i32 0, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	%reg_57 = add i32 %reg_53, %reg_56
	%reg_58 = sub i32 %reg_47, %reg_57
	store i32 %reg_58, ptr %reg_1
	%reg_59 = load i32, ptr %reg_5
	call void @push_down(i32 %reg_59)
	%reg_60 = load i32, ptr %reg_5
	%reg_61 = add i32 1, 0
	%reg_62 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_60, i32 %reg_61
	%reg_63 = load i32, ptr %reg_62
	store i32 %reg_63, ptr %reg_5
	br label %Block9
}

define void @reverse(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 76
	%reg_32 = alloca i32
	%reg_20 = alloca i32
	%reg_7 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 76
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = call i32 @get_kth(i32 %reg_5)
	store i32 %reg_6, ptr %reg_4
	%reg_8 = load i32, ptr %reg_3
	%reg_9 = add i32 2, 0
	%reg_10 = add i32 %reg_8, %reg_9
	%reg_11 = call i32 @get_kth(i32 %reg_10)
	store i32 %reg_11, ptr %reg_7
	%reg_12 = load i32, ptr %reg_4
	call void @putint(i32 %reg_12)
	%reg_13 = add i32 32, 0
	call void @putch(i32 %reg_13)
	%reg_14 = load i32, ptr %reg_7
	call void @putint(i32 %reg_14)
	%reg_15 = add i32 10, 0
	call void @putch(i32 %reg_15)
	%reg_16 = load i32, ptr %reg_4
	%reg_17 = add i32 0, 0
	call void @splay(i32 %reg_16, i32 %reg_17)
	%reg_18 = load i32, ptr %reg_7
	%reg_19 = load i32, ptr @root
	call void @splay(i32 %reg_18, i32 %reg_19)
	%reg_21 = load i32, ptr %reg_7
	%reg_22 = add i32 0, 0
	%reg_23 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_21, i32 %reg_22
	%reg_24 = load i32, ptr %reg_23
	store i32 %reg_24, ptr %reg_20
	%reg_25 = load i32, ptr %reg_20
	%reg_26 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_25
	%reg_27 = load i32, ptr %reg_20
	%reg_28 = getelementptr [100005 x i32], ptr @tag, i32 0, i32 %reg_27
	%reg_29 = load i32, ptr %reg_28
	%reg_30 = add i32 1, 0
	%reg_31 = call i32 @xor_01(i32 %reg_29, i32 %reg_30)
	store i32 %reg_31, ptr %reg_26
	%reg_33 = load i32, ptr %reg_20
	%reg_34 = add i32 0, 0
	%reg_35 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_33, i32 %reg_34
	%reg_36 = load i32, ptr %reg_35
	store i32 %reg_36, ptr %reg_32
	%reg_37 = load i32, ptr %reg_20
	%reg_38 = add i32 0, 0
	%reg_39 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_37, i32 %reg_38
	%reg_40 = load i32, ptr %reg_20
	%reg_41 = add i32 1, 0
	%reg_42 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_40, i32 %reg_41
	%reg_43 = load i32, ptr %reg_42
	store i32 %reg_43, ptr %reg_39
	%reg_44 = load i32, ptr %reg_20
	%reg_45 = add i32 1, 0
	%reg_46 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_44, i32 %reg_45
	%reg_47 = load i32, ptr %reg_32
	store i32 %reg_47, ptr %reg_46
	ret void
}

define i32 @build(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 90
	%reg_59 = alloca i32
	%reg_46 = alloca i32
	%reg_14 = alloca i32
	%reg_8 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 90
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp sgt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 91
	%reg_7 = add i32 0, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 91
	%reg_9 = load i32, ptr %reg_2
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = add i32 %reg_9, %reg_10
	%reg_12 = add i32 2, 0
	%reg_13 = sdiv i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_8
	%reg_15 = load i32, ptr %reg_8
	%reg_16 = getelementptr [100005 x i32], ptr @a, i32 0, i32 %reg_15
	%reg_17 = load i32, ptr %reg_16
	store i32 %reg_17, ptr %reg_14
	%reg_18 = load i32, ptr %reg_14
	%reg_19 = load i32, ptr @inf
	%reg_20 = sub i32 0, %reg_19
	%reg_21 = icmp eq i32 %reg_18, %reg_20
	br i1 %reg_21, label %Block4, label %Block6
Block4:  ;If then at line 93
	%reg_25 = load i32, ptr @tot
	%reg_26 = add i32 1, 0
	%reg_27 = add i32 %reg_25, %reg_26
	store i32 %reg_27, ptr %reg_14
	%reg_28 = load i32, ptr @tot
	%reg_29 = add i32 1, 0
	%reg_30 = add i32 %reg_28, %reg_29
	store i32 %reg_30, ptr @tot
	br label %Block5
Block5:  ;If end at line 93
	%reg_31 = load i32, ptr %reg_14
	%reg_32 = getelementptr [100005 x i32], ptr @data, i32 0, i32 %reg_31
	%reg_33 = load i32, ptr %reg_8
	%reg_34 = getelementptr [100005 x i32], ptr @a, i32 0, i32 %reg_33
	%reg_35 = load i32, ptr %reg_34
	store i32 %reg_35, ptr %reg_32
	%reg_36 = load i32, ptr %reg_14
	%reg_37 = getelementptr [100005 x i32], ptr @cnt, i32 0, i32 %reg_36
	%reg_38 = add i32 1, 0
	store i32 %reg_38, ptr %reg_37
	%reg_39 = load i32, ptr %reg_14
	%reg_40 = getelementptr [100005 x i32], ptr @siz, i32 0, i32 %reg_39
	%reg_41 = add i32 1, 0
	store i32 %reg_41, ptr %reg_40
	%reg_42 = load i32, ptr %reg_2
	%reg_43 = load i32, ptr %reg_3
	%reg_44 = icmp eq i32 %reg_42, %reg_43
	br i1 %reg_44, label %Block7, label %Block8
Block6:  ;Or opertor at line 93
	%reg_22 = load i32, ptr %reg_14
	%reg_23 = load i32, ptr @inf
	%reg_24 = icmp eq i32 %reg_22, %reg_23
	br i1 %reg_24, label %Block4, label %Block5
Block7:  ;If then at line 96
	%reg_45 = load i32, ptr %reg_14
	ret i32 %reg_45
	br label %Block8
Block8:  ;If end at line 96
	%reg_47 = load i32, ptr %reg_2
	%reg_48 = load i32, ptr %reg_8
	%reg_49 = add i32 1, 0
	%reg_50 = sub i32 %reg_48, %reg_49
	%reg_51 = call i32 @build(i32 %reg_47, i32 %reg_50)
	store i32 %reg_51, ptr %reg_46
	%reg_52 = load i32, ptr %reg_14
	%reg_53 = add i32 0, 0
	%reg_54 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_52, i32 %reg_53
	%reg_55 = load i32, ptr %reg_46
	store i32 %reg_55, ptr %reg_54
	%reg_56 = load i32, ptr %reg_46
	%reg_57 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_56
	%reg_58 = load i32, ptr %reg_14
	store i32 %reg_58, ptr %reg_57
	%reg_60 = load i32, ptr %reg_8
	%reg_61 = add i32 1, 0
	%reg_62 = add i32 %reg_60, %reg_61
	%reg_63 = load i32, ptr %reg_3
	%reg_64 = call i32 @build(i32 %reg_62, i32 %reg_63)
	store i32 %reg_64, ptr %reg_59
	%reg_65 = load i32, ptr %reg_14
	%reg_66 = add i32 1, 0
	%reg_67 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_65, i32 %reg_66
	%reg_68 = load i32, ptr %reg_59
	store i32 %reg_68, ptr %reg_67
	%reg_69 = load i32, ptr %reg_59
	%reg_70 = getelementptr [100005 x i32], ptr @fa, i32 0, i32 %reg_69
	%reg_71 = load i32, ptr %reg_14
	store i32 %reg_71, ptr %reg_70
	%reg_72 = load i32, ptr %reg_14
	call void @push_up(i32 %reg_72)
	%reg_73 = load i32, ptr %reg_2
	call void @putint(i32 %reg_73)
	%reg_74 = add i32 32, 0
	call void @putch(i32 %reg_74)
	%reg_75 = load i32, ptr %reg_3
	call void @putint(i32 %reg_75)
	%reg_76 = add i32 32, 0
	call void @putch(i32 %reg_76)
	%reg_77 = load i32, ptr %reg_14
	call void @putint(i32 %reg_77)
	%reg_78 = add i32 32, 0
	call void @putch(i32 %reg_78)
	%reg_79 = load i32, ptr %reg_14
	%reg_80 = add i32 0, 0
	%reg_81 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_79, i32 %reg_80
	%reg_82 = load i32, ptr %reg_81
	call void @putint(i32 %reg_82)
	%reg_83 = add i32 32, 0
	call void @putch(i32 %reg_83)
	%reg_84 = load i32, ptr %reg_14
	%reg_85 = add i32 1, 0
	%reg_86 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_84, i32 %reg_85
	%reg_87 = load i32, ptr %reg_86
	call void @putint(i32 %reg_87)
	%reg_88 = add i32 10, 0
	call void @putch(i32 %reg_88)
	%reg_89 = load i32, ptr %reg_14
	ret i32 %reg_89
}

define void @get_ans(i32 %reg_0)
{
Block0:  ;Func define at line 106
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 106
	%reg_2 = load i32, ptr %reg_1
	call void @push_down(i32 %reg_2)
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = add i32 0, 0
	%reg_5 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_3, i32 %reg_4
	%reg_6 = load i32, ptr %reg_5
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block2, label %Block3
Block2:  ;If then at line 108
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = add i32 0, 0
	%reg_10 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	call void @get_ans(i32 %reg_11)
	br label %Block3
Block3:  ;If end at line 108
	%reg_12 = load i32, ptr %reg_1
	%reg_13 = getelementptr [100005 x i32], ptr @data, i32 0, i32 %reg_12
	%reg_14 = load i32, ptr %reg_13
	%reg_15 = load i32, ptr @inf
	%reg_16 = sub i32 0, %reg_15
	%reg_17 = icmp ne i32 %reg_14, %reg_16
	br i1 %reg_17, label %Block6, label %Block5
Block4:  ;If then at line 109
	%reg_23 = load i32, ptr %reg_1
	%reg_24 = getelementptr [100005 x i32], ptr @data, i32 0, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	call void @putint(i32 %reg_25)
	%reg_26 = add i32 32, 0
	call void @putch(i32 %reg_26)
	br label %Block5
Block5:  ;If end at line 109
	%reg_27 = load i32, ptr %reg_1
	%reg_28 = add i32 1, 0
	%reg_29 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_27, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	%reg_31 = icmp ne i32 %reg_30, 0
	br i1 %reg_31, label %Block7, label %Block8
Block6:  ;And opertor at line 109
	%reg_18 = load i32, ptr %reg_1
	%reg_19 = getelementptr [100005 x i32], ptr @data, i32 0, i32 %reg_18
	%reg_20 = load i32, ptr %reg_19
	%reg_21 = load i32, ptr @inf
	%reg_22 = icmp ne i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block4, label %Block5
Block7:  ;If then at line 110
	%reg_32 = load i32, ptr %reg_1
	%reg_33 = add i32 1, 0
	%reg_34 = getelementptr [100005 x [2 x i32]], ptr @ch, i32 0, i32 %reg_32, i32 %reg_33
	%reg_35 = load i32, ptr %reg_34
	call void @get_ans(i32 %reg_35)
	br label %Block8
Block8:  ;If end at line 110
	ret void
}

define i32 @main()
{
Block0:  ;Func define at line 113
	%reg_40 = alloca i32
	%reg_38 = alloca i32
	%reg_13 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 113
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr %reg_0
	%reg_3 = call i32 @getint()
	store i32 %reg_3, ptr %reg_2
	%reg_4 = add i32 1, 0
	%reg_5 = getelementptr [100005 x i32], ptr @a, i32 0, i32 %reg_4
	%reg_6 = load i32, ptr @inf
	%reg_7 = sub i32 0, %reg_6
	store i32 %reg_7, ptr %reg_5
	%reg_8 = load i32, ptr %reg_0
	%reg_9 = add i32 2, 0
	%reg_10 = add i32 %reg_8, %reg_9
	%reg_11 = getelementptr [100005 x i32], ptr @a, i32 0, i32 %reg_10
	%reg_12 = load i32, ptr @inf
	store i32 %reg_12, ptr %reg_11
	%reg_14 = add i32 2, 0
	store i32 %reg_14, ptr %reg_13
	br label %Block2
Block2:  ;While condition at line 117
	%reg_15 = load i32, ptr %reg_13
	%reg_16 = load i32, ptr %reg_0
	%reg_17 = add i32 1, 0
	%reg_18 = add i32 %reg_16, %reg_17
	%reg_19 = icmp sle i32 %reg_15, %reg_18
	br i1 %reg_19, label %Block3, label %Block4
Block3:  ;While body at line 117
	%reg_20 = load i32, ptr %reg_13
	%reg_21 = getelementptr [100005 x i32], ptr @a, i32 0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_13
	%reg_23 = add i32 1, 0
	%reg_24 = sub i32 %reg_22, %reg_23
	store i32 %reg_24, ptr %reg_21
	%reg_25 = load i32, ptr %reg_13
	%reg_26 = add i32 1, 0
	%reg_27 = add i32 %reg_25, %reg_26
	store i32 %reg_27, ptr %reg_13
	br label %Block2
Block4:  ;While end at line 117
	%reg_28 = load i32, ptr %reg_0
	store i32 %reg_28, ptr @tot
	%reg_29 = add i32 1, 0
	%reg_30 = load i32, ptr %reg_0
	%reg_31 = add i32 2, 0
	%reg_32 = add i32 %reg_30, %reg_31
	%reg_33 = call i32 @build(i32 %reg_29, i32 %reg_32)
	store i32 %reg_33, ptr @root
	%reg_34 = add i32 1, 0
	store i32 %reg_34, ptr %reg_13
	br label %Block5
Block5:  ;While condition at line 125
	%reg_35 = load i32, ptr %reg_13
	%reg_36 = load i32, ptr %reg_2
	%reg_37 = icmp sle i32 %reg_35, %reg_36
	br i1 %reg_37, label %Block6, label %Block7
Block6:  ;While body at line 125
	%reg_39 = call i32 @getint()
	store i32 %reg_39, ptr %reg_38
	%reg_41 = call i32 @getint()
	store i32 %reg_41, ptr %reg_40
	%reg_42 = load i32, ptr %reg_38
	%reg_43 = load i32, ptr %reg_40
	call void @reverse(i32 %reg_42, i32 %reg_43)
	%reg_44 = load i32, ptr %reg_13
	%reg_45 = add i32 1, 0
	%reg_46 = add i32 %reg_44, %reg_45
	store i32 %reg_46, ptr %reg_13
	br label %Block5
Block7:  ;While end at line 125
	%reg_47 = load i32, ptr @root
	call void @get_ans(i32 %reg_47)
	%reg_48 = add i32 10, 0
	call void @putch(i32 %reg_48)
	%reg_49 = add i32 0, 0
	ret i32 %reg_49
}
