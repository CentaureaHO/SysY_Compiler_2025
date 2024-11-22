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

@UP = global i32 0
@DOWN = global i32 1
@LEFT = global i32 2
@RIGHT = global i32 3
@MAP_LEN = global i32 4
@POW2 = global [20 x i32] [i32 1,i32 2,i32 4,i32 8,i32 16,i32 32,i32 64,i32 128,i32 256,i32 512,i32 1024,i32 2048,i32 4096,i32 8192,i32 16384,i32 32768,i32 65536,i32 131072,i32 262144,i32 524288]
@LEN_OF_POW2 = global [20 x i32] [i32 0,i32 1,i32 1,i32 1,i32 2,i32 2,i32 2,i32 3,i32 3,i32 3,i32 4,i32 4,i32 4,i32 4,i32 5,i32 5,i32 5,i32 6,i32 6,i32 6]
@STR_INIT = global [25 x i32] [i32 73,i32 110,i32 112,i32 117,i32 116,i32 32,i32 97,i32 32,i32 114,i32 97,i32 110,i32 100,i32 111,i32 109,i32 32,i32 110,i32 117,i32 109,i32 98,i32 101,i32 114,i32 58,i32 32,i32 10,i32 0]
@STR_HELP = global [62 x i32] [i32 119,i32 44,i32 32,i32 97,i32 44,i32 32,i32 115,i32 44,i32 32,i32 100,i32 58,i32 32,i32 109,i32 111,i32 118,i32 101,i32 10,i32 104,i32 58,i32 32,i32 112,i32 114,i32 105,i32 110,i32 116,i32 32,i32 116,i32 104,i32 105,i32 115,i32 32,i32 104,i32 101,i32 108,i32 112,i32 10,i32 113,i32 58,i32 32,i32 113,i32 117,i32 105,i32 116,i32 10,i32 112,i32 58,i32 32,i32 112,i32 114,i32 105,i32 110,i32 116,i32 32,i32 116,i32 104,i32 101,i32 32,i32 109,i32 97,i32 112,i32 10,i32 0]
@STR_SCORE = global [8 x i32] [i32 115,i32 99,i32 111,i32 114,i32 101,i32 58,i32 32,i32 0]
@STR_STEP = global [7 x i32] [i32 115,i32 116,i32 101,i32 112,i32 58,i32 32,i32 0]
@STR_GG = global [12 x i32] [i32 71,i32 97,i32 109,i32 101,i32 32,i32 111,i32 118,i32 101,i32 114,i32 46,i32 10,i32 0]
@STR_INVALID = global [26 x i32] [i32 73,i32 110,i32 118,i32 97,i32 108,i32 105,i32 100,i32 32,i32 105,i32 110,i32 112,i32 117,i32 116,i32 46,i32 32,i32 84,i32 114,i32 121,i32 32,i32 97,i32 103,i32 97,i32 105,i32 110,i32 46,i32 0]
@CHR_SPACE = global i32 32
@CHR_ENTER = global i32 10
@map = global [4 x [4 x i32]] zeroinitializer
@score = global i32 zeroinitializer
@step = global i32 zeroinitializer
@max_num_len = global i32 zeroinitializer
@alive = global i32 zeroinitializer
@seed = global i32 zeroinitializer

define i32 @or(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 36
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 36
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = icmp ne i32 %reg_4, 0
	br i1 %reg_5, label %Block2, label %Block4
Block2:  ;If then at line 38
	%reg_8 = add i32 1, 0
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 38
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
Block4:  ;Or opertor at line 38
	%reg_6 = load i32, ptr %reg_3
	%reg_7 = icmp ne i32 %reg_6, 0
	br i1 %reg_7, label %Block2, label %Block3
}

define i32 @rand()
{
Block0:  ;Func define at line 88
	br label %Block1
Block1:  ;Func body at line 88
	%reg_0 = load i32, ptr @seed
	%reg_1 = add i32 214013, 0
	%reg_2 = mul i32 %reg_0, %reg_1
	%reg_3 = add i32 2531011, 0
	%reg_4 = add i32 %reg_2, %reg_3
	%reg_5 = add i32 1073741824, 0
	%reg_6 = srem i32 %reg_4, %reg_5
	store i32 %reg_6, ptr @seed
	%reg_7 = load i32, ptr @seed
	%reg_8 = add i32 0, 0
	%reg_9 = icmp slt i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block2, label %Block3
Block2:  ;If then at line 90
	%reg_10 = load i32, ptr @seed
	%reg_11 = sub i32 0, %reg_10
	store i32 %reg_11, ptr @seed
	br label %Block3
Block3:  ;If end at line 90
	%reg_12 = load i32, ptr @seed
	%reg_13 = add i32 65536, 0
	%reg_14 = sdiv i32 %reg_12, %reg_13
	%reg_15 = add i32 32768, 0
	%reg_16 = srem i32 %reg_14, %reg_15
	ret i32 %reg_16
}

define void @put_string(ptr %reg_0)
{
Block0:  ;Func define at line 96
	%reg_1 = alloca i32
	br label %Block1
Block1:  ;Func body at line 96
	%reg_2 = add i32 0, 0
	store i32 %reg_2, ptr %reg_1
	br label %Block2
Block2:  ;While condition at line 98
	%reg_3 = load i32, ptr %reg_1
	%reg_4 = getelementptr i32, ptr %reg_0, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = add i32 0, 0
	%reg_7 = icmp ne i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block3, label %Block4
Block3:  ;While body at line 98
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = getelementptr i32, ptr %reg_0, i32 %reg_8
	%reg_10 = load i32, ptr %reg_9
	call void @putch(i32 %reg_10)
	%reg_11 = load i32, ptr %reg_1
	%reg_12 = add i32 1, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_1
	br label %Block2
Block4:  ;While end at line 98
	ret void
}

define void @clear_map()
{
Block0:  ;Func define at line 105
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 105
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 107
	%reg_4 = load i32, ptr %reg_0
	%reg_5 = load i32, ptr @MAP_LEN
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 107
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_2
	br label %Block5
Block4:  ;While end at line 107
	ret void
Block5:  ;While condition at line 109
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = load i32, ptr @MAP_LEN
	%reg_10 = icmp slt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block6, label %Block7
Block6:  ;While body at line 109
	%reg_11 = load i32, ptr %reg_0
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_11, i32 %reg_12
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_13
	%reg_15 = load i32, ptr %reg_2
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_2
	br label %Block5
Block7:  ;While end at line 109
	%reg_18 = load i32, ptr %reg_0
	%reg_19 = add i32 1, 0
	%reg_20 = add i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_0
	br label %Block2
}

define void @init()
{
Block0:  ;Func define at line 118
	br label %Block1
Block1:  ;Func body at line 118
	call void @clear_map()
	%reg_0 = add i32 0, 0
	store i32 %reg_0, ptr @score
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr @step
	%reg_2 = add i32 1, 0
	store i32 %reg_2, ptr @max_num_len
	%reg_3 = add i32 1, 0
	store i32 %reg_3, ptr @alive
	ret void
}

define void @print_map()
{
Block0:  ;Func define at line 126
	%reg_47 = alloca i32
	%reg_24 = alloca i32
	%reg_9 = alloca i32
	%reg_7 = alloca i32
	br label %Block1
Block1:  ;Func body at line 126
	%reg_0 = load i32, ptr @CHR_ENTER
	call void @putch(i32 %reg_0)
	%reg_1 = getelementptr [7 x i32], ptr @STR_STEP, i32 0
	call void @put_string(ptr %reg_1)
	%reg_2 = load i32, ptr @step
	call void @putint(i32 %reg_2)
	%reg_3 = load i32, ptr @CHR_ENTER
	call void @putch(i32 %reg_3)
	%reg_4 = getelementptr [8 x i32], ptr @STR_SCORE, i32 0
	call void @put_string(ptr %reg_4)
	%reg_5 = load i32, ptr @score
	call void @putint(i32 %reg_5)
	%reg_6 = load i32, ptr @CHR_ENTER
	call void @putch(i32 %reg_6)
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block2
Block2:  ;While condition at line 135
	%reg_11 = load i32, ptr %reg_7
	%reg_12 = load i32, ptr @MAP_LEN
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block3, label %Block4
Block3:  ;While body at line 135
	%reg_14 = add i32 0, 0
	store i32 %reg_14, ptr %reg_9
	br label %Block5
Block4:  ;While end at line 135
	ret void
Block5:  ;While condition at line 137
	%reg_15 = load i32, ptr %reg_9
	%reg_16 = load i32, ptr @MAP_LEN
	%reg_17 = icmp slt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block6, label %Block7
Block6:  ;While body at line 137
	%reg_18 = load i32, ptr %reg_7
	%reg_19 = load i32, ptr %reg_9
	%reg_20 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_18, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = add i32 0, 0
	%reg_23 = icmp eq i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block8, label %Block10
Block7:  ;While end at line 137
	%reg_64 = load i32, ptr @CHR_ENTER
	call void @putch(i32 %reg_64)
	%reg_65 = load i32, ptr %reg_7
	%reg_66 = add i32 1, 0
	%reg_67 = add i32 %reg_65, %reg_66
	store i32 %reg_67, ptr %reg_7
	br label %Block2
Block8:  ;If then at line 138
	%reg_25 = load i32, ptr %reg_7
	%reg_26 = load i32, ptr %reg_9
	%reg_27 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_25, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	%reg_29 = getelementptr [20 x i32], ptr @LEN_OF_POW2, i32 0, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	%reg_31 = add i32 1, 0
	%reg_32 = add i32 %reg_30, %reg_31
	store i32 %reg_32, ptr %reg_24
	br label %Block11
Block9:  ;If end at line 138
	%reg_61 = load i32, ptr %reg_9
	%reg_62 = add i32 1, 0
	%reg_63 = add i32 %reg_61, %reg_62
	store i32 %reg_63, ptr %reg_9
	br label %Block5
Block10:  ;If else at line 138
	%reg_41 = load i32, ptr %reg_7
	%reg_42 = load i32, ptr %reg_9
	%reg_43 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_41, i32 %reg_42
	%reg_44 = load i32, ptr %reg_43
	%reg_45 = getelementptr [20 x i32], ptr @POW2, i32 0, i32 %reg_44
	%reg_46 = load i32, ptr %reg_45
	call void @putint(i32 %reg_46)
	%reg_48 = load i32, ptr %reg_7
	%reg_49 = load i32, ptr %reg_9
	%reg_50 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_48, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = getelementptr [20 x i32], ptr @LEN_OF_POW2, i32 0, i32 %reg_51
	%reg_53 = load i32, ptr %reg_52
	store i32 %reg_53, ptr %reg_47
	br label %Block14
Block11:  ;While condition at line 140
	%reg_33 = load i32, ptr %reg_24
	%reg_34 = load i32, ptr @max_num_len
	%reg_35 = icmp sle i32 %reg_33, %reg_34
	br i1 %reg_35, label %Block12, label %Block13
Block12:  ;While body at line 140
	%reg_36 = add i32 95, 0
	call void @putch(i32 %reg_36)
	%reg_37 = load i32, ptr %reg_24
	%reg_38 = add i32 1, 0
	%reg_39 = add i32 %reg_37, %reg_38
	store i32 %reg_39, ptr %reg_24
	br label %Block11
Block13:  ;While end at line 140
	%reg_40 = load i32, ptr @CHR_SPACE
	call void @putch(i32 %reg_40)
	br label %Block9
Block14:  ;While condition at line 148
	%reg_54 = load i32, ptr %reg_47
	%reg_55 = load i32, ptr @max_num_len
	%reg_56 = icmp sle i32 %reg_54, %reg_55
	br i1 %reg_56, label %Block15, label %Block16
Block15:  ;While body at line 148
	%reg_57 = load i32, ptr @CHR_SPACE
	call void @putch(i32 %reg_57)
	%reg_58 = load i32, ptr %reg_47
	%reg_59 = add i32 1, 0
	%reg_60 = add i32 %reg_58, %reg_59
	store i32 %reg_60, ptr %reg_47
	br label %Block14
Block16:  ;While end at line 148
	br label %Block9
}

define i32 @move_base(i32 %reg_0, ptr %reg_1, ptr %reg_2, ptr %reg_3, ptr %reg_4, i32 %reg_5)
{
Block0:  ;Func define at line 162
	%reg_137 = alloca i32
	%reg_63 = alloca i32
	%reg_51 = alloca i32
	%reg_37 = alloca i32
	%reg_33 = alloca i32
	%reg_12 = alloca i32
	%reg_10 = alloca i32
	%reg_8 = alloca i32
	%reg_7 = alloca i32
	%reg_6 = alloca i32
	store i32 %reg_0, ptr %reg_6
	store i32 %reg_5, ptr %reg_7
	br label %Block1
Block1:  ;Func body at line 162
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = load i32, ptr %reg_6
	%reg_15 = add i32 1, 0
	%reg_16 = sub i32 0, %reg_15
	%reg_17 = icmp eq i32 %reg_14, %reg_16
	br i1 %reg_17, label %Block2, label %Block4
Block2:  ;If then at line 166
	%reg_18 = load i32, ptr @MAP_LEN
	%reg_19 = add i32 1, 0
	%reg_20 = sub i32 %reg_18, %reg_19
	store i32 %reg_20, ptr %reg_8
	%reg_21 = add i32 1, 0
	%reg_22 = sub i32 0, %reg_21
	store i32 %reg_22, ptr %reg_10
	br label %Block3
Block3:  ;If end at line 166
	%reg_25 = add i32 0, 0
	%reg_26 = getelementptr i32, ptr %reg_2, i32 %reg_25
	%reg_27 = load i32, ptr %reg_8
	store i32 %reg_27, ptr %reg_26
	br label %Block5
Block4:  ;If else at line 166
	%reg_23 = add i32 0, 0
	store i32 %reg_23, ptr %reg_8
	%reg_24 = load i32, ptr @MAP_LEN
	store i32 %reg_24, ptr %reg_10
	br label %Block3
Block5:  ;While condition at line 174
	%reg_28 = add i32 0, 0
	%reg_29 = getelementptr i32, ptr %reg_2, i32 %reg_28
	%reg_30 = load i32, ptr %reg_29
	%reg_31 = load i32, ptr %reg_10
	%reg_32 = icmp ne i32 %reg_30, %reg_31
	br i1 %reg_32, label %Block6, label %Block7
Block6:  ;While body at line 174
	%reg_34 = load i32, ptr %reg_8
	%reg_35 = load i32, ptr %reg_6
	%reg_36 = add i32 %reg_34, %reg_35
	store i32 %reg_36, ptr %reg_33
	%reg_38 = load i32, ptr %reg_8
	store i32 %reg_38, ptr %reg_37
	br label %Block8
Block7:  ;While end at line 174
	%reg_174 = load i32, ptr %reg_12
	ret i32 %reg_174
Block8:  ;While condition at line 176
	%reg_39 = load i32, ptr %reg_33
	%reg_40 = load i32, ptr %reg_10
	%reg_41 = icmp ne i32 %reg_39, %reg_40
	br i1 %reg_41, label %Block9, label %Block10
Block9:  ;While body at line 176
	%reg_42 = load i32, ptr %reg_33
	%reg_43 = load i32, ptr %reg_37
	%reg_44 = icmp eq i32 %reg_42, %reg_43
	br i1 %reg_44, label %Block11, label %Block12
Block10:  ;While end at line 176
	%reg_167 = add i32 0, 0
	%reg_168 = getelementptr i32, ptr %reg_2, i32 %reg_167
	%reg_169 = add i32 0, 0
	%reg_170 = getelementptr i32, ptr %reg_2, i32 %reg_169
	%reg_171 = load i32, ptr %reg_170
	%reg_172 = load i32, ptr %reg_6
	%reg_173 = add i32 %reg_171, %reg_172
	store i32 %reg_173, ptr %reg_168
	br label %Block5
Block11:  ;If then at line 177
	%reg_45 = load i32, ptr %reg_33
	%reg_46 = load i32, ptr %reg_6
	%reg_47 = add i32 %reg_45, %reg_46
	store i32 %reg_47, ptr %reg_33
	br label %Block8
Block12:  ;If end at line 177
	%reg_48 = add i32 0, 0
	%reg_49 = getelementptr i32, ptr %reg_1, i32 %reg_48
	%reg_50 = load i32, ptr %reg_33
	store i32 %reg_50, ptr %reg_49
	%reg_52 = add i32 0, 0
	%reg_53 = getelementptr i32, ptr %reg_3, i32 %reg_52
	%reg_54 = load i32, ptr %reg_53
	%reg_55 = add i32 0, 0
	%reg_56 = getelementptr i32, ptr %reg_4, i32 %reg_55
	%reg_57 = load i32, ptr %reg_56
	%reg_58 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_54, i32 %reg_57
	%reg_59 = load i32, ptr %reg_58
	store i32 %reg_59, ptr %reg_51
	%reg_60 = add i32 0, 0
	%reg_61 = getelementptr i32, ptr %reg_1, i32 %reg_60
	%reg_62 = load i32, ptr %reg_37
	store i32 %reg_62, ptr %reg_61
	%reg_64 = add i32 0, 0
	%reg_65 = getelementptr i32, ptr %reg_3, i32 %reg_64
	%reg_66 = load i32, ptr %reg_65
	%reg_67 = add i32 0, 0
	%reg_68 = getelementptr i32, ptr %reg_4, i32 %reg_67
	%reg_69 = load i32, ptr %reg_68
	%reg_70 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_66, i32 %reg_69
	%reg_71 = load i32, ptr %reg_70
	store i32 %reg_71, ptr %reg_63
	%reg_72 = load i32, ptr %reg_63
	%reg_73 = add i32 0, 0
	%reg_74 = icmp eq i32 %reg_72, %reg_73
	br i1 %reg_74, label %Block14, label %Block16
Block13:  ;Continue at line 179
	br label %Block12
Block14:  ;If then at line 185
	%reg_75 = load i32, ptr %reg_51
	%reg_76 = add i32 0, 0
	%reg_77 = icmp eq i32 %reg_75, %reg_76
	br i1 %reg_77, label %Block17, label %Block19
Block15:  ;If end at line 185
	br label %Block8
Block16:  ;If else at line 185
	%reg_107 = load i32, ptr %reg_51
	%reg_108 = load i32, ptr %reg_63
	%reg_109 = icmp eq i32 %reg_107, %reg_108
	br i1 %reg_109, label %Block22, label %Block24
Block17:  ;If then at line 186
	%reg_78 = load i32, ptr %reg_33
	%reg_79 = load i32, ptr %reg_6
	%reg_80 = add i32 %reg_78, %reg_79
	store i32 %reg_80, ptr %reg_33
	br label %Block18
Block18:  ;If end at line 186
	br label %Block15
Block19:  ;If else at line 186
	%reg_81 = load i32, ptr %reg_7
	%reg_82 = icmp ne i32 %reg_81, 0
	br i1 %reg_82, label %Block20, label %Block21
Block20:  ;If then at line 189
	%reg_83 = add i32 1, 0
	ret i32 %reg_83
	br label %Block21
Block21:  ;If end at line 189
	%reg_84 = add i32 0, 0
	%reg_85 = getelementptr i32, ptr %reg_3, i32 %reg_84
	%reg_86 = load i32, ptr %reg_85
	%reg_87 = add i32 0, 0
	%reg_88 = getelementptr i32, ptr %reg_4, i32 %reg_87
	%reg_89 = load i32, ptr %reg_88
	%reg_90 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_86, i32 %reg_89
	%reg_91 = load i32, ptr %reg_51
	store i32 %reg_91, ptr %reg_90
	%reg_92 = add i32 0, 0
	%reg_93 = getelementptr i32, ptr %reg_1, i32 %reg_92
	%reg_94 = load i32, ptr %reg_33
	store i32 %reg_94, ptr %reg_93
	%reg_95 = add i32 0, 0
	%reg_96 = getelementptr i32, ptr %reg_3, i32 %reg_95
	%reg_97 = load i32, ptr %reg_96
	%reg_98 = add i32 0, 0
	%reg_99 = getelementptr i32, ptr %reg_4, i32 %reg_98
	%reg_100 = load i32, ptr %reg_99
	%reg_101 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_97, i32 %reg_100
	%reg_102 = add i32 0, 0
	store i32 %reg_102, ptr %reg_101
	%reg_103 = add i32 1, 0
	store i32 %reg_103, ptr %reg_12
	%reg_104 = load i32, ptr %reg_33
	%reg_105 = load i32, ptr %reg_6
	%reg_106 = add i32 %reg_104, %reg_105
	store i32 %reg_106, ptr %reg_33
	br label %Block18
Block22:  ;If then at line 199
	%reg_110 = load i32, ptr %reg_7
	%reg_111 = icmp ne i32 %reg_110, 0
	br i1 %reg_111, label %Block25, label %Block26
Block23:  ;If end at line 199
	br label %Block15
Block24:  ;If else at line 199
	%reg_158 = load i32, ptr %reg_51
	%reg_159 = add i32 0, 0
	%reg_160 = icmp eq i32 %reg_158, %reg_159
	br i1 %reg_160, label %Block29, label %Block31
Block25:  ;If then at line 200
	%reg_112 = add i32 1, 0
	ret i32 %reg_112
	br label %Block26
Block26:  ;If end at line 200
	%reg_113 = add i32 0, 0
	%reg_114 = getelementptr i32, ptr %reg_1, i32 %reg_113
	%reg_115 = load i32, ptr %reg_37
	store i32 %reg_115, ptr %reg_114
	%reg_116 = add i32 0, 0
	%reg_117 = getelementptr i32, ptr %reg_3, i32 %reg_116
	%reg_118 = load i32, ptr %reg_117
	%reg_119 = add i32 0, 0
	%reg_120 = getelementptr i32, ptr %reg_4, i32 %reg_119
	%reg_121 = load i32, ptr %reg_120
	%reg_122 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_118, i32 %reg_121
	%reg_123 = load i32, ptr %reg_63
	%reg_124 = add i32 1, 0
	%reg_125 = add i32 %reg_123, %reg_124
	store i32 %reg_125, ptr %reg_122
	%reg_126 = add i32 0, 0
	%reg_127 = getelementptr i32, ptr %reg_1, i32 %reg_126
	%reg_128 = load i32, ptr %reg_33
	store i32 %reg_128, ptr %reg_127
	%reg_129 = add i32 0, 0
	%reg_130 = getelementptr i32, ptr %reg_3, i32 %reg_129
	%reg_131 = load i32, ptr %reg_130
	%reg_132 = add i32 0, 0
	%reg_133 = getelementptr i32, ptr %reg_4, i32 %reg_132
	%reg_134 = load i32, ptr %reg_133
	%reg_135 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_131, i32 %reg_134
	%reg_136 = add i32 0, 0
	store i32 %reg_136, ptr %reg_135
	%reg_138 = load i32, ptr %reg_63
	%reg_139 = add i32 1, 0
	%reg_140 = add i32 %reg_138, %reg_139
	%reg_141 = getelementptr [20 x i32], ptr @LEN_OF_POW2, i32 0, i32 %reg_140
	%reg_142 = load i32, ptr %reg_141
	store i32 %reg_142, ptr %reg_137
	%reg_143 = load i32, ptr %reg_137
	%reg_144 = load i32, ptr @max_num_len
	%reg_145 = icmp sgt i32 %reg_143, %reg_144
	br i1 %reg_145, label %Block27, label %Block28
Block27:  ;If then at line 209
	%reg_146 = load i32, ptr %reg_137
	store i32 %reg_146, ptr @max_num_len
	br label %Block28
Block28:  ;If end at line 209
	%reg_147 = load i32, ptr @score
	%reg_148 = load i32, ptr %reg_63
	%reg_149 = add i32 1, 0
	%reg_150 = add i32 %reg_148, %reg_149
	%reg_151 = getelementptr [20 x i32], ptr @POW2, i32 0, i32 %reg_150
	%reg_152 = load i32, ptr %reg_151
	%reg_153 = add i32 %reg_147, %reg_152
	store i32 %reg_153, ptr @score
	%reg_154 = add i32 1, 0
	store i32 %reg_154, ptr %reg_12
	%reg_155 = load i32, ptr %reg_37
	%reg_156 = load i32, ptr %reg_6
	%reg_157 = add i32 %reg_155, %reg_156
	store i32 %reg_157, ptr %reg_37
	br label %Block23
Block29:  ;If then at line 215
	%reg_161 = load i32, ptr %reg_33
	%reg_162 = load i32, ptr %reg_6
	%reg_163 = add i32 %reg_161, %reg_162
	store i32 %reg_163, ptr %reg_33
	br label %Block30
Block30:  ;If end at line 215
	br label %Block23
Block31:  ;If else at line 215
	%reg_164 = load i32, ptr %reg_37
	%reg_165 = load i32, ptr %reg_6
	%reg_166 = add i32 %reg_164, %reg_165
	store i32 %reg_166, ptr %reg_37
	br label %Block30
}

define void @generate()
{
Block0:  ;Func define at line 227
	%reg_39 = alloca i32
	%reg_8 = alloca i32
	%reg_6 = alloca i32
	%reg_4 = alloca i32
	%reg_2 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 227
	%reg_1 = add i32 0, 0
	store i32 %reg_1, ptr %reg_0
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 0, 0
	store i32 %reg_5, ptr %reg_4
	%reg_7 = add i32 0, 0
	store i32 %reg_7, ptr %reg_6
	%reg_9 = add i32 0, 0
	store i32 %reg_9, ptr %reg_8
	br label %Block2
Block2:  ;While condition at line 229
	%reg_10 = load i32, ptr %reg_0
	%reg_11 = load i32, ptr @MAP_LEN
	%reg_12 = icmp slt i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block3, label %Block4
Block3:  ;While body at line 229
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_2
	br label %Block5
Block4:  ;While end at line 229
	%reg_40 = add i32 0, 0
	store i32 %reg_40, ptr %reg_39
	%reg_41 = call i32 @rand()
	%reg_42 = add i32 2, 0
	%reg_43 = srem i32 %reg_41, %reg_42
	%reg_44 = add i32 1, 0
	%reg_45 = icmp slt i32 %reg_43, %reg_44
	br i1 %reg_45, label %Block12, label %Block14
Block5:  ;While condition at line 231
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = load i32, ptr @MAP_LEN
	%reg_16 = icmp slt i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block6, label %Block7
Block6:  ;While body at line 231
	%reg_17 = load i32, ptr %reg_0
	%reg_18 = load i32, ptr %reg_2
	%reg_19 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_17, i32 %reg_18
	%reg_20 = load i32, ptr %reg_19
	%reg_21 = add i32 0, 0
	%reg_22 = icmp eq i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block8, label %Block9
Block7:  ;While end at line 231
	%reg_36 = load i32, ptr %reg_0
	%reg_37 = add i32 1, 0
	%reg_38 = add i32 %reg_36, %reg_37
	store i32 %reg_38, ptr %reg_0
	br label %Block2
Block8:  ;If then at line 232
	%reg_23 = load i32, ptr %reg_8
	%reg_24 = add i32 1, 0
	%reg_25 = add i32 %reg_23, %reg_24
	store i32 %reg_25, ptr %reg_8
	%reg_26 = call i32 @rand()
	%reg_27 = load i32, ptr %reg_8
	%reg_28 = srem i32 %reg_26, %reg_27
	%reg_29 = add i32 0, 0
	%reg_30 = icmp eq i32 %reg_28, %reg_29
	br i1 %reg_30, label %Block10, label %Block11
Block9:  ;If end at line 232
	%reg_33 = load i32, ptr %reg_2
	%reg_34 = add i32 1, 0
	%reg_35 = add i32 %reg_33, %reg_34
	store i32 %reg_35, ptr %reg_2
	br label %Block5
Block10:  ;If then at line 234
	%reg_31 = load i32, ptr %reg_0
	store i32 %reg_31, ptr %reg_4
	%reg_32 = load i32, ptr %reg_2
	store i32 %reg_32, ptr %reg_6
	br label %Block11
Block11:  ;If end at line 234
	br label %Block9
Block12:  ;If then at line 244
	%reg_46 = add i32 1, 0
	store i32 %reg_46, ptr %reg_39
	br label %Block13
Block13:  ;If end at line 244
	%reg_48 = load i32, ptr %reg_4
	%reg_49 = load i32, ptr %reg_6
	%reg_50 = getelementptr [4 x [4 x i32]], ptr @map, i32 0, i32 %reg_48, i32 %reg_49
	%reg_51 = load i32, ptr %reg_39
	store i32 %reg_51, ptr %reg_50
	ret void
Block14:  ;If else at line 244
	%reg_47 = add i32 2, 0
	store i32 %reg_47, ptr %reg_39
	br label %Block13
}

define void @move(i32 %reg_0)
{
Block0:  ;Func define at line 252
	%reg_12 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca [1 x i32]
	%reg_2 = alloca [1 x i32]
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 252
	call void @llvm.memset.p0.i32(ptr %reg_2, i8 0, i32 4, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_3, i8 0, i32 4, i1 1)
	%reg_5 = add i32 1, 0
	%reg_6 = load i32, ptr %reg_1
	%reg_7 = add i32 2, 0
	%reg_8 = srem i32 %reg_6, %reg_7
	%reg_9 = add i32 2, 0
	%reg_10 = mul i32 %reg_8, %reg_9
	%reg_11 = sub i32 %reg_5, %reg_10
	store i32 %reg_11, ptr %reg_4
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = load i32, ptr %reg_1
	%reg_15 = add i32 2, 0
	%reg_16 = sdiv i32 %reg_14, %reg_15
	%reg_17 = add i32 0, 0
	%reg_18 = icmp eq i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block2, label %Block4
Block2:  ;If then at line 255
	%reg_19 = load i32, ptr %reg_4
	%reg_20 = getelementptr [1 x i32], ptr %reg_2, i32 0
	%reg_21 = getelementptr [1 x i32], ptr %reg_3, i32 0
	%reg_22 = getelementptr [1 x i32], ptr %reg_2, i32 0
	%reg_23 = getelementptr [1 x i32], ptr %reg_3, i32 0
	%reg_24 = add i32 0, 0
	%reg_25 = call i32 @move_base(i32 %reg_19, ptr %reg_20, ptr %reg_21, ptr %reg_22, ptr %reg_23, i32 %reg_24)
	store i32 %reg_25, ptr %reg_12
	br label %Block3
Block3:  ;If end at line 255
	%reg_33 = load i32, ptr %reg_12
	%reg_34 = icmp eq i32 %reg_33, 0
	br i1 %reg_34, label %Block5, label %Block6
Block4:  ;If else at line 255
	%reg_26 = load i32, ptr %reg_4
	%reg_27 = getelementptr [1 x i32], ptr %reg_3, i32 0
	%reg_28 = getelementptr [1 x i32], ptr %reg_2, i32 0
	%reg_29 = getelementptr [1 x i32], ptr %reg_2, i32 0
	%reg_30 = getelementptr [1 x i32], ptr %reg_3, i32 0
	%reg_31 = add i32 0, 0
	%reg_32 = call i32 @move_base(i32 %reg_26, ptr %reg_27, ptr %reg_28, ptr %reg_29, ptr %reg_30, i32 %reg_31)
	store i32 %reg_32, ptr %reg_12
	br label %Block3
Block5:  ;If then at line 260
	%reg_35 = getelementptr [26 x i32], ptr @STR_INVALID, i32 0
	call void @put_string(ptr %reg_35)
	%reg_36 = load i32, ptr @CHR_ENTER
	call void @putch(i32 %reg_36)
	ret void
	br label %Block6
Block6:  ;If end at line 260
	%reg_37 = load i32, ptr @step
	%reg_38 = add i32 1, 0
	%reg_39 = add i32 %reg_37, %reg_38
	store i32 %reg_39, ptr @step
	call void @generate()
	call void @print_map()
	ret void
}

define i32 @try_move()
{
Block0:  ;Func define at line 270
	%reg_1 = alloca [1 x i32]
	%reg_0 = alloca [1 x i32]
	br label %Block1
Block1:  ;Func body at line 270
	call void @llvm.memset.p0.i32(ptr %reg_0, i8 0, i32 4, i1 1)
	call void @llvm.memset.p0.i32(ptr %reg_1, i8 0, i32 4, i1 1)
	%reg_2 = add i32 1, 0
	%reg_3 = getelementptr [1 x i32], ptr %reg_0, i32 0
	%reg_4 = getelementptr [1 x i32], ptr %reg_1, i32 0
	%reg_5 = getelementptr [1 x i32], ptr %reg_0, i32 0
	%reg_6 = getelementptr [1 x i32], ptr %reg_1, i32 0
	%reg_7 = add i32 1, 0
	%reg_8 = call i32 @move_base(i32 %reg_2, ptr %reg_3, ptr %reg_4, ptr %reg_5, ptr %reg_6, i32 %reg_7)
	%reg_9 = add i32 1, 0
	%reg_10 = getelementptr [1 x i32], ptr %reg_1, i32 0
	%reg_11 = getelementptr [1 x i32], ptr %reg_0, i32 0
	%reg_12 = getelementptr [1 x i32], ptr %reg_0, i32 0
	%reg_13 = getelementptr [1 x i32], ptr %reg_1, i32 0
	%reg_14 = add i32 1, 0
	%reg_15 = call i32 @move_base(i32 %reg_9, ptr %reg_10, ptr %reg_11, ptr %reg_12, ptr %reg_13, i32 %reg_14)
	%reg_16 = call i32 @or(i32 %reg_8, i32 %reg_15)
	%reg_17 = add i32 1, 0
	%reg_18 = sub i32 0, %reg_17
	%reg_19 = getelementptr [1 x i32], ptr %reg_0, i32 0
	%reg_20 = getelementptr [1 x i32], ptr %reg_1, i32 0
	%reg_21 = getelementptr [1 x i32], ptr %reg_0, i32 0
	%reg_22 = getelementptr [1 x i32], ptr %reg_1, i32 0
	%reg_23 = add i32 1, 0
	%reg_24 = call i32 @move_base(i32 %reg_18, ptr %reg_19, ptr %reg_20, ptr %reg_21, ptr %reg_22, i32 %reg_23)
	%reg_25 = add i32 1, 0
	%reg_26 = sub i32 0, %reg_25
	%reg_27 = getelementptr [1 x i32], ptr %reg_1, i32 0
	%reg_28 = getelementptr [1 x i32], ptr %reg_0, i32 0
	%reg_29 = getelementptr [1 x i32], ptr %reg_0, i32 0
	%reg_30 = getelementptr [1 x i32], ptr %reg_1, i32 0
	%reg_31 = add i32 1, 0
	%reg_32 = call i32 @move_base(i32 %reg_26, ptr %reg_27, ptr %reg_28, ptr %reg_29, ptr %reg_30, i32 %reg_31)
	%reg_33 = call i32 @or(i32 %reg_24, i32 %reg_32)
	%reg_34 = call i32 @or(i32 %reg_16, i32 %reg_33)
	ret i32 %reg_34
}

define i32 @main()
{
Block0:  ;Func define at line 276
	%reg_4 = alloca i32
	br label %Block1
Block1:  ;Func body at line 276
	%reg_0 = getelementptr [25 x i32], ptr @STR_INIT, i32 0
	call void @put_string(ptr %reg_0)
	%reg_1 = call i32 @getint()
	store i32 %reg_1, ptr @seed
	call void @init()
	call void @generate()
	call void @print_map()
	br label %Block2
Block2:  ;While condition at line 282
	%reg_2 = load i32, ptr @alive
	%reg_3 = icmp ne i32 %reg_2, 0
	br i1 %reg_3, label %Block3, label %Block4
Block3:  ;While body at line 282
	%reg_5 = call i32 @getch()
	store i32 %reg_5, ptr %reg_4
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = add i32 119, 0
	%reg_8 = icmp eq i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block5, label %Block7
Block4:  ;While end at line 282
	%reg_58 = add i32 0, 0
	ret i32 %reg_58
Block5:  ;If then at line 284
	%reg_9 = load i32, ptr @UP
	call void @move(i32 %reg_9)
	br label %Block6
Block6:  ;If end at line 284
	%reg_55 = call i32 @try_move()
	%reg_56 = icmp eq i32 %reg_55, 0
	br i1 %reg_56, label %Block33, label %Block34
Block7:  ;If else at line 284
	%reg_10 = load i32, ptr %reg_4
	%reg_11 = add i32 97, 0
	%reg_12 = icmp eq i32 %reg_10, %reg_11
	br i1 %reg_12, label %Block8, label %Block10
Block8:  ;If then at line 286
	%reg_13 = load i32, ptr @LEFT
	call void @move(i32 %reg_13)
	br label %Block9
Block9:  ;If end at line 286
	br label %Block6
Block10:  ;If else at line 286
	%reg_14 = load i32, ptr %reg_4
	%reg_15 = add i32 115, 0
	%reg_16 = icmp eq i32 %reg_14, %reg_15
	br i1 %reg_16, label %Block11, label %Block13
Block11:  ;If then at line 288
	%reg_17 = load i32, ptr @DOWN
	call void @move(i32 %reg_17)
	br label %Block12
Block12:  ;If end at line 288
	br label %Block9
Block13:  ;If else at line 288
	%reg_18 = load i32, ptr %reg_4
	%reg_19 = add i32 100, 0
	%reg_20 = icmp eq i32 %reg_18, %reg_19
	br i1 %reg_20, label %Block14, label %Block16
Block14:  ;If then at line 290
	%reg_21 = load i32, ptr @RIGHT
	call void @move(i32 %reg_21)
	br label %Block15
Block15:  ;If end at line 290
	br label %Block12
Block16:  ;If else at line 290
	%reg_22 = load i32, ptr %reg_4
	%reg_23 = add i32 104, 0
	%reg_24 = icmp eq i32 %reg_22, %reg_23
	br i1 %reg_24, label %Block17, label %Block19
Block17:  ;If then at line 292
	%reg_25 = getelementptr [62 x i32], ptr @STR_HELP, i32 0
	call void @put_string(ptr %reg_25)
	br label %Block18
Block18:  ;If end at line 292
	br label %Block15
Block19:  ;If else at line 292
	%reg_26 = load i32, ptr %reg_4
	%reg_27 = add i32 113, 0
	%reg_28 = icmp eq i32 %reg_26, %reg_27
	br i1 %reg_28, label %Block20, label %Block23
Block20:  ;If then at line 295
	%reg_33 = getelementptr [12 x i32], ptr @STR_GG, i32 0
	call void @put_string(ptr %reg_33)
	%reg_34 = add i32 0, 0
	ret i32 %reg_34
	br label %Block21
Block21:  ;If end at line 295
	br label %Block18
Block22:  ;If else at line 295
	%reg_35 = load i32, ptr %reg_4
	%reg_36 = add i32 112, 0
	%reg_37 = icmp eq i32 %reg_35, %reg_36
	br i1 %reg_37, label %Block24, label %Block26
Block23:  ;Or opertor at line 295
	%reg_29 = load i32, ptr %reg_4
	%reg_30 = add i32 1, 0
	%reg_31 = sub i32 0, %reg_30
	%reg_32 = icmp eq i32 %reg_29, %reg_31
	br i1 %reg_32, label %Block20, label %Block22
Block24:  ;If then at line 299
	%reg_38 = load i32, ptr @CHR_ENTER
	call void @putch(i32 %reg_38)
	call void @print_map()
	br label %Block25
Block25:  ;If end at line 299
	br label %Block21
Block26:  ;If else at line 299
	%reg_39 = load i32, ptr %reg_4
	%reg_40 = add i32 32, 0
	%reg_41 = icmp eq i32 %reg_39, %reg_40
	br i1 %reg_41, label %Block27, label %Block31
Block27:  ;If then at line 303
	br label %Block2
Block28:  ;If end at line 303
	br label %Block25
Block29:  ;If else at line 303
	%reg_48 = getelementptr [26 x i32], ptr @STR_INVALID, i32 0
	call void @put_string(ptr %reg_48)
	%reg_49 = load i32, ptr @CHR_ENTER
	call void @putch(i32 %reg_49)
	%reg_50 = load i32, ptr @seed
	%reg_51 = load i32, ptr %reg_4
	%reg_52 = add i32 %reg_50, %reg_51
	%reg_53 = add i32 1073741824, 0
	%reg_54 = srem i32 %reg_52, %reg_53
	store i32 %reg_54, ptr @seed
	br label %Block28
Block30:  ;Or opertor at line 303
	%reg_45 = load i32, ptr %reg_4
	%reg_46 = add i32 13, 0
	%reg_47 = icmp eq i32 %reg_45, %reg_46
	br i1 %reg_47, label %Block27, label %Block29
Block31:  ;Or opertor at line -1
	%reg_42 = load i32, ptr %reg_4
	%reg_43 = add i32 10, 0
	%reg_44 = icmp eq i32 %reg_42, %reg_43
	br i1 %reg_44, label %Block27, label %Block30
Block32:  ;Continue at line 305
	br label %Block28
Block33:  ;If then at line 312
	%reg_57 = getelementptr [12 x i32], ptr @STR_GG, i32 0
	call void @put_string(ptr %reg_57)
	br label %Block4
Block34:  ;If end at line 312
	br label %Block2
Block35:  ;Break at line 314
	br label %Block34
}
