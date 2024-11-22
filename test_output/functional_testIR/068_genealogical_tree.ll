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

@map = global [10 x [10 x i32]] zeroinitializer
@indegree = global [10 x i32] zeroinitializer
@queue = global [10 x i32] zeroinitializer

define void @topo(i32 %reg_0)
{
Block0:  ;Func define at line 4
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 4
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	%reg_10 = add i32 1, 0
	store i32 %reg_10, ptr %reg_6
	%reg_11 = add i32 1, 0
	store i32 %reg_11, ptr %reg_8
	br label %Block2
Block2:  ;While condition at line 11
	%reg_12 = load i32, ptr %reg_6
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = icmp sle i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block3, label %Block4
Block3:  ;While body at line 11
	%reg_15 = add i32 1, 0
	store i32 %reg_15, ptr %reg_8
	br label %Block5
Block4:  ;While end at line 11
	%reg_60 = add i32 0, 0
	store i32 %reg_60, ptr %reg_6
	br label %Block16
Block5:  ;While condition at line 14
	%reg_16 = load i32, ptr %reg_8
	%reg_17 = load i32, ptr %reg_1
	%reg_18 = icmp sle i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block6, label %Block7
Block6:  ;While body at line 14
	%reg_19 = load i32, ptr %reg_8
	%reg_20 = getelementptr [10 x i32], ptr @indegree, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = add i32 0, 0
	%reg_23 = icmp eq i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block8, label %Block9
Block7:  ;While end at line 14
	%reg_28 = load i32, ptr %reg_4
	%reg_29 = getelementptr [10 x i32], ptr @queue, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_2
	store i32 %reg_30, ptr %reg_29
	%reg_31 = load i32, ptr %reg_4
	%reg_32 = add i32 1, 0
	%reg_33 = add i32 %reg_31, %reg_32
	store i32 %reg_33, ptr %reg_4
	%reg_34 = load i32, ptr %reg_2
	%reg_35 = getelementptr [10 x i32], ptr @indegree, i32 0, i32 %reg_34
	%reg_36 = add i32 1, 0
	%reg_37 = sub i32 0, %reg_36
	store i32 %reg_37, ptr %reg_35
	%reg_38 = add i32 1, 0
	store i32 %reg_38, ptr %reg_8
	br label %Block11
Block8:  ;If then at line 16
	%reg_24 = load i32, ptr %reg_8
	store i32 %reg_24, ptr %reg_2
	br label %Block7
Block9:  ;If end at line 16
	%reg_25 = load i32, ptr %reg_8
	%reg_26 = add i32 1, 0
	%reg_27 = add i32 %reg_25, %reg_26
	store i32 %reg_27, ptr %reg_8
	br label %Block5
Block10:  ;Break at line 20
	br label %Block9
Block11:  ;While condition at line 28
	%reg_39 = load i32, ptr %reg_8
	%reg_40 = load i32, ptr %reg_1
	%reg_41 = icmp sle i32 %reg_39, %reg_40
	br i1 %reg_41, label %Block12, label %Block13
Block12:  ;While body at line 28
	%reg_42 = load i32, ptr %reg_2
	%reg_43 = load i32, ptr %reg_8
	%reg_44 = getelementptr [10 x [10 x i32]], ptr @map, i32 0, i32 %reg_42, i32 %reg_43
	%reg_45 = load i32, ptr %reg_44
	%reg_46 = icmp ne i32 %reg_45, 0
	br i1 %reg_46, label %Block14, label %Block15
Block13:  ;While end at line 28
	%reg_57 = load i32, ptr %reg_6
	%reg_58 = add i32 1, 0
	%reg_59 = add i32 %reg_57, %reg_58
	store i32 %reg_59, ptr %reg_6
	br label %Block2
Block14:  ;If then at line 31
	%reg_47 = load i32, ptr %reg_8
	%reg_48 = getelementptr [10 x i32], ptr @indegree, i32 0, i32 %reg_47
	%reg_49 = load i32, ptr %reg_8
	%reg_50 = getelementptr [10 x i32], ptr @indegree, i32 0, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = add i32 1, 0
	%reg_53 = sub i32 %reg_51, %reg_52
	store i32 %reg_53, ptr %reg_48
	br label %Block15
Block15:  ;If end at line 31
	%reg_54 = load i32, ptr %reg_8
	%reg_55 = add i32 1, 0
	%reg_56 = add i32 %reg_54, %reg_55
	store i32 %reg_56, ptr %reg_8
	br label %Block11
Block16:  ;While condition at line 40
	%reg_61 = load i32, ptr %reg_6
	%reg_62 = load i32, ptr %reg_1
	%reg_63 = icmp slt i32 %reg_61, %reg_62
	br i1 %reg_63, label %Block17, label %Block18
Block17:  ;While body at line 40
	%reg_64 = load i32, ptr %reg_6
	%reg_65 = getelementptr [10 x i32], ptr @queue, i32 0, i32 %reg_64
	%reg_66 = load i32, ptr %reg_65
	call void @putint(i32 %reg_66)
	%reg_67 = add i32 10, 0
	call void @putch(i32 %reg_67)
	%reg_68 = load i32, ptr %reg_6
	%reg_69 = add i32 1, 0
	%reg_70 = add i32 %reg_68, %reg_69
	store i32 %reg_70, ptr %reg_6
	br label %Block16
Block18:  ;While end at line 40
	ret void
}

define i32 @main()
{
Block0:  ;Func define at line 48
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 48
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 1, 0
	store i32 %reg_5, ptr %reg_4
	%reg_6 = add i32 5, 0
	store i32 %reg_6, ptr %reg_0
	br label %Block2
Block2:  ;While condition at line 54
	%reg_7 = load i32, ptr %reg_4
	%reg_8 = load i32, ptr %reg_0
	%reg_9 = icmp sle i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block3, label %Block4
Block3:  ;While body at line 54
	%reg_10 = call i32 @getint()
	store i32 %reg_10, ptr %reg_2
	br label %Block5
Block4:  ;While end at line 54
	%reg_29 = load i32, ptr %reg_0
	call void @topo(i32 %reg_29)
	%reg_30 = add i32 0, 0
	ret i32 %reg_30
Block5:  ;While condition at line 57
	%reg_11 = load i32, ptr %reg_2
	%reg_12 = add i32 0, 0
	%reg_13 = icmp ne i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block6, label %Block7
Block6:  ;While body at line 57
	%reg_14 = load i32, ptr %reg_4
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = getelementptr [10 x [10 x i32]], ptr @map, i32 0, i32 %reg_14, i32 %reg_15
	%reg_17 = add i32 1, 0
	store i32 %reg_17, ptr %reg_16
	%reg_18 = load i32, ptr %reg_2
	%reg_19 = getelementptr [10 x i32], ptr @indegree, i32 0, i32 %reg_18
	%reg_20 = load i32, ptr %reg_2
	%reg_21 = getelementptr [10 x i32], ptr @indegree, i32 0, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	%reg_23 = add i32 1, 0
	%reg_24 = add i32 %reg_22, %reg_23
	store i32 %reg_24, ptr %reg_19
	%reg_25 = call i32 @getint()
	store i32 %reg_25, ptr %reg_2
	br label %Block5
Block7:  ;While end at line 57
	%reg_26 = load i32, ptr %reg_4
	%reg_27 = add i32 1, 0
	%reg_28 = add i32 %reg_26, %reg_27
	store i32 %reg_28, ptr %reg_4
	br label %Block2
}
