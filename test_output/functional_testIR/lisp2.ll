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

@ERR_INVALID_DATA_TYPE = global i32 0
@ERR_BUFFER_OVERFLOW = global i32 1
@ERR_PARSE_ERROR = global i32 2
@ERR_SYMBOL_NOT_FOUND = global i32 3
@ERR_INVALID_LIST = global i32 4
@ERR_INVALID_FUNC = global i32 5
@ERR_INVALID_ARG_NUM = global i32 6
@ERR_TYPE_MISMATCH = global i32 7
@ERR_INVALID_SYMBOL = global i32 8
@DATA_TYPE = global i32 0
@DATA_VALUE = global i32 1
@DATA_NEXT = global i32 2
@DATA_REF_COUNT = global i32 3
@DATA_FIELD_COUNT = global i32 4
@MAX_DATA_LEN = global i32 2048
@data = global [2048 x [4 x i32]] zeroinitializer
@free_data_ptr = global i32 0
@DATA_TYPE_SYMBOL = global i32 0
@DATA_TYPE_NUMBER = global i32 1
@DATA_TYPE_LIST = global i32 2
@DATA_TYPE_ENV = global i32 3
@DATA_TYPE_FUNC = global i32 4
@MAX_SYM_BUF_LEN = global i32 4096
@sym_buf = global [4096 x i32] zeroinitializer
@next_sym = global i32 0
@SYM_QUOTE = global i32 -1
@SYM_ATOM = global i32 -2
@SYM_NUMBER = global i32 -3
@SYM_EQ = global i32 -4
@SYM_CAR = global i32 -5
@SYM_CDR = global i32 -6
@SYM_CONS = global i32 -7
@SYM_COND = global i32 -8
@SYM_LAMBDA = global i32 -9
@SYM_DEFINE = global i32 -10
@SYM_T = global i32 -11
@SYM_F = global i32 -12
@SYM_LIST = global i32 -13
@SYM_ADD = global i32 -14
@SYM_SUB = global i32 -15
@SYM_MUL = global i32 -16
@SYM_DIV = global i32 -17
@SYM_GT = global i32 -18
@SYM_LT = global i32 -19
@SYM_GE = global i32 -20
@SYM_LE = global i32 -21
@SYM_EQ_NUM = global i32 -22
@PRE_SYM_COUNT = global i32 22
@PREDEF_SYMS = global [22 x [8 x i32]] [[8 x i32] [i32 113,i32 117,i32 111,i32 116,i32 101,i32 0,i32 0,i32 0],[8 x i32] [i32 97,i32 116,i32 111,i32 109,i32 63,i32 0,i32 0,i32 0],[8 x i32] [i32 110,i32 117,i32 109,i32 98,i32 101,i32 114,i32 63,i32 0],[8 x i32] [i32 101,i32 113,i32 63,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 99,i32 97,i32 114,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 99,i32 100,i32 114,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 99,i32 111,i32 110,i32 115,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 99,i32 111,i32 110,i32 100,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 108,i32 97,i32 109,i32 98,i32 100,i32 97,i32 0,i32 0],[8 x i32] [i32 100,i32 101,i32 102,i32 105,i32 110,i32 101,i32 0,i32 0],[8 x i32] [i32 116,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 102,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 108,i32 105,i32 115,i32 116,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 43,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 45,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 42,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 47,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 62,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 60,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 62,i32 61,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 60,i32 61,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0],[8 x i32] [i32 61,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0,i32 0]]
@last_char = global i32 32
@TOKEN_EOF = global i32 0
@TOKEN_SYMBOL = global i32 1
@TOKEN_NUMBER = global i32 2
@TOKEN_QUOTE = global i32 3
@TOKEN_LPAREN = global i32 4
@TOKEN_RPAREN = global i32 5
@last_token = global i32 0
@last_token_type = global i32 0

define i32 @eq(i32 %reg_0, i32 %reg_1)
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
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 5
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 5
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
}

define i32 @ne(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 6
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 6
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp ne i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 6
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 6
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
}

define i32 @lt(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 7
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 7
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 7
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 7
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
}

define i32 @le(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 8
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 8
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp sle i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 8
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 8
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
}

define i32 @gt(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 9
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 9
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp sgt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 9
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 9
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
}

define i32 @ge(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 10
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 10
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp sge i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 10
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 10
	%reg_8 = add i32 0, 0
	ret i32 %reg_8
}

define i32 @panic(i32 %reg_0)
{
Block0:  ;Func define at line 27
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 27
	%reg_2 = add i32 112, 0
	call void @putch(i32 %reg_2)
	%reg_3 = add i32 97, 0
	call void @putch(i32 %reg_3)
	%reg_4 = add i32 110, 0
	call void @putch(i32 %reg_4)
	%reg_5 = add i32 105, 0
	call void @putch(i32 %reg_5)
	%reg_6 = add i32 99, 0
	call void @putch(i32 %reg_6)
	%reg_7 = add i32 32, 0
	call void @putch(i32 %reg_7)
	%reg_8 = load i32, ptr %reg_1
	call void @putint(i32 %reg_8)
	%reg_9 = add i32 10, 0
	call void @putch(i32 %reg_9)
	%reg_10 = load i32, ptr %reg_1
	ret i32 %reg_10
}

define void @init_data()
{
Block0:  ;Func define at line 75
	%reg_4 = alloca i32
	br label %Block1
Block1:  ;Func body at line 75
	%reg_0 = add i32 0, 0
	%reg_1 = load i32, ptr @DATA_NEXT
	%reg_2 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_0, i32 %reg_1
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	%reg_5 = add i32 1, 0
	store i32 %reg_5, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 78
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = load i32, ptr @MAX_DATA_LEN
	%reg_8 = icmp slt i32 %reg_6, %reg_7
	br i1 %reg_8, label %Block3, label %Block4
Block3:  ;While body at line 78
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = load i32, ptr @DATA_NEXT
	%reg_11 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_9, i32 %reg_10
	%reg_12 = load i32, ptr %reg_4
	%reg_13 = add i32 1, 0
	%reg_14 = sub i32 %reg_12, %reg_13
	store i32 %reg_14, ptr %reg_11
	%reg_15 = load i32, ptr %reg_4
	%reg_16 = add i32 1, 0
	%reg_17 = add i32 %reg_15, %reg_16
	store i32 %reg_17, ptr %reg_4
	br label %Block2
Block4:  ;While end at line 78
	%reg_18 = load i32, ptr @MAX_DATA_LEN
	%reg_19 = add i32 1, 0
	%reg_20 = sub i32 %reg_18, %reg_19
	store i32 %reg_20, ptr @free_data_ptr
	ret void
}

define i32 @alloc_data()
{
Block0:  ;Func define at line 87
	%reg_4 = alloca i32
	br label %Block1
Block1:  ;Func body at line 87
	%reg_0 = load i32, ptr @free_data_ptr
	%reg_1 = icmp eq i32 %reg_0, 0
	br i1 %reg_1, label %Block2, label %Block3
Block2:  ;If then at line 88
	%reg_2 = load i32, ptr @ERR_BUFFER_OVERFLOW
	%reg_3 = call i32 @panic(i32 %reg_2)
	br label %Block3
Block3:  ;If end at line 88
	%reg_5 = load i32, ptr @free_data_ptr
	store i32 %reg_5, ptr %reg_4
	%reg_6 = load i32, ptr %reg_4
	%reg_7 = load i32, ptr @DATA_NEXT
	%reg_8 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_6, i32 %reg_7
	%reg_9 = load i32, ptr %reg_8
	store i32 %reg_9, ptr @free_data_ptr
	%reg_10 = load i32, ptr %reg_4
	%reg_11 = load i32, ptr @DATA_NEXT
	%reg_12 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = add i32 0, 0
	store i32 %reg_13, ptr %reg_12
	%reg_14 = load i32, ptr %reg_4
	%reg_15 = load i32, ptr @DATA_REF_COUNT
	%reg_16 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_14, i32 %reg_15
	%reg_17 = add i32 1, 0
	store i32 %reg_17, ptr %reg_16
	%reg_18 = load i32, ptr %reg_4
	ret i32 %reg_18
}

define void @free_data(i32 %reg_0)
{
Block0:  ;Func define at line 98
	%reg_4 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 98
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = icmp eq i32 %reg_2, 0
	br i1 %reg_3, label %Block2, label %Block3
Block2:  ;If then at line 99
	ret void
	br label %Block3
Block3:  ;If end at line 99
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = load i32, ptr @DATA_REF_COUNT
	%reg_7 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_5, i32 %reg_6
	%reg_8 = load i32, ptr %reg_7
	%reg_9 = add i32 1, 0
	%reg_10 = sub i32 %reg_8, %reg_9
	store i32 %reg_10, ptr %reg_4
	%reg_11 = load i32, ptr %reg_1
	%reg_12 = load i32, ptr @DATA_REF_COUNT
	%reg_13 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_11, i32 %reg_12
	%reg_14 = load i32, ptr %reg_4
	store i32 %reg_14, ptr %reg_13
	%reg_15 = load i32, ptr %reg_4
	%reg_16 = add i32 0, 0
	%reg_17 = icmp sgt i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block4, label %Block5
Block4:  ;If then at line 103
	ret void
	br label %Block5
Block5:  ;If end at line 103
	%reg_18 = load i32, ptr %reg_1
	%reg_19 = load i32, ptr @DATA_TYPE
	%reg_20 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_18, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = load i32, ptr @DATA_TYPE_SYMBOL
	%reg_23 = icmp ne i32 %reg_21, %reg_22
	br i1 %reg_23, label %Block8, label %Block7
Block6:  ;If then at line 105
	%reg_30 = load i32, ptr %reg_1
	%reg_31 = load i32, ptr @DATA_VALUE
	%reg_32 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_30, i32 %reg_31
	%reg_33 = load i32, ptr %reg_32
	call void @free_data(i32 %reg_33)
	br label %Block7
Block7:  ;If end at line 105
	%reg_34 = load i32, ptr %reg_1
	%reg_35 = load i32, ptr @DATA_NEXT
	%reg_36 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_34, i32 %reg_35
	%reg_37 = load i32, ptr %reg_36
	call void @free_data(i32 %reg_37)
	%reg_38 = load i32, ptr %reg_1
	%reg_39 = load i32, ptr @DATA_NEXT
	%reg_40 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_38, i32 %reg_39
	%reg_41 = load i32, ptr @free_data_ptr
	store i32 %reg_41, ptr %reg_40
	%reg_42 = load i32, ptr %reg_1
	store i32 %reg_42, ptr @free_data_ptr
	ret void
Block8:  ;And opertor at line 105
	%reg_24 = load i32, ptr %reg_1
	%reg_25 = load i32, ptr @DATA_TYPE
	%reg_26 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_24, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	%reg_28 = load i32, ptr @DATA_TYPE_NUMBER
	%reg_29 = icmp ne i32 %reg_27, %reg_28
	br i1 %reg_29, label %Block6, label %Block7
}

define i32 @copy_ptr(i32 %reg_0)
{
Block0:  ;Func define at line 116
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 116
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = icmp eq i32 %reg_2, 0
	br i1 %reg_3, label %Block2, label %Block3
Block2:  ;If then at line 117
	%reg_4 = add i32 0, 0
	ret i32 %reg_4
	br label %Block3
Block3:  ;If end at line 117
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = load i32, ptr @DATA_REF_COUNT
	%reg_7 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_5, i32 %reg_6
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = load i32, ptr @DATA_REF_COUNT
	%reg_10 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	%reg_12 = add i32 1, 0
	%reg_13 = add i32 %reg_11, %reg_12
	store i32 %reg_13, ptr %reg_7
	%reg_14 = load i32, ptr %reg_1
	ret i32 %reg_14
}

define i32 @copy_data(i32 %reg_0)
{
Block0:  ;Func define at line 124
	%reg_14 = alloca i32
	%reg_5 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 124
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = icmp eq i32 %reg_2, 0
	br i1 %reg_3, label %Block2, label %Block3
Block2:  ;If then at line 125
	%reg_4 = add i32 0, 0
	ret i32 %reg_4
	br label %Block3
Block3:  ;If end at line 125
	%reg_6 = call i32 @alloc_data()
	store i32 %reg_6, ptr %reg_5
	%reg_7 = load i32, ptr %reg_5
	%reg_8 = load i32, ptr @DATA_TYPE
	%reg_9 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_7, i32 %reg_8
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = load i32, ptr @DATA_TYPE
	%reg_12 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	store i32 %reg_13, ptr %reg_9
	%reg_15 = load i32, ptr %reg_1
	%reg_16 = load i32, ptr @DATA_VALUE
	%reg_17 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_15, i32 %reg_16
	%reg_18 = load i32, ptr %reg_17
	store i32 %reg_18, ptr %reg_14
	%reg_19 = load i32, ptr %reg_1
	%reg_20 = load i32, ptr @DATA_TYPE
	%reg_21 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_19, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	%reg_23 = load i32, ptr @DATA_TYPE_SYMBOL
	%reg_24 = icmp ne i32 %reg_22, %reg_23
	br i1 %reg_24, label %Block6, label %Block5
Block4:  ;If then at line 129
	%reg_31 = load i32, ptr %reg_14
	%reg_32 = call i32 @copy_ptr(i32 %reg_31)
	store i32 %reg_32, ptr %reg_14
	br label %Block5
Block5:  ;If end at line 129
	%reg_33 = load i32, ptr %reg_5
	%reg_34 = load i32, ptr @DATA_VALUE
	%reg_35 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_33, i32 %reg_34
	%reg_36 = load i32, ptr %reg_14
	store i32 %reg_36, ptr %reg_35
	%reg_37 = load i32, ptr %reg_5
	%reg_38 = load i32, ptr @DATA_NEXT
	%reg_39 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_37, i32 %reg_38
	%reg_40 = add i32 0, 0
	store i32 %reg_40, ptr %reg_39
	%reg_41 = load i32, ptr %reg_5
	ret i32 %reg_41
Block6:  ;And opertor at line 129
	%reg_25 = load i32, ptr %reg_1
	%reg_26 = load i32, ptr @DATA_TYPE
	%reg_27 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_25, i32 %reg_26
	%reg_28 = load i32, ptr %reg_27
	%reg_29 = load i32, ptr @DATA_TYPE_NUMBER
	%reg_30 = icmp ne i32 %reg_28, %reg_29
	br i1 %reg_30, label %Block4, label %Block5
}

define i32 @make_symbol(i32 %reg_0)
{
Block0:  ;Func define at line 139
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 139
	%reg_3 = call i32 @alloc_data()
	store i32 %reg_3, ptr %reg_2
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @DATA_TYPE
	%reg_6 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_4, i32 %reg_5
	%reg_7 = load i32, ptr @DATA_TYPE_SYMBOL
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = load i32, ptr @DATA_VALUE
	%reg_10 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = load i32, ptr %reg_1
	store i32 %reg_11, ptr %reg_10
	%reg_12 = load i32, ptr %reg_2
	ret i32 %reg_12
}

define i32 @make_number(i32 %reg_0)
{
Block0:  ;Func define at line 147
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 147
	%reg_3 = call i32 @alloc_data()
	store i32 %reg_3, ptr %reg_2
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @DATA_TYPE
	%reg_6 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_4, i32 %reg_5
	%reg_7 = load i32, ptr @DATA_TYPE_NUMBER
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = load i32, ptr @DATA_VALUE
	%reg_10 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = load i32, ptr %reg_1
	store i32 %reg_11, ptr %reg_10
	%reg_12 = load i32, ptr %reg_2
	ret i32 %reg_12
}

define i32 @make_list(i32 %reg_0)
{
Block0:  ;Func define at line 155
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 155
	%reg_3 = call i32 @alloc_data()
	store i32 %reg_3, ptr %reg_2
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @DATA_TYPE
	%reg_6 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_4, i32 %reg_5
	%reg_7 = load i32, ptr @DATA_TYPE_LIST
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = load i32, ptr @DATA_VALUE
	%reg_10 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = load i32, ptr %reg_1
	store i32 %reg_11, ptr %reg_10
	%reg_12 = load i32, ptr %reg_2
	ret i32 %reg_12
}

define i32 @is_predef_sym(i32 %reg_0)
{
Block0:  ;Func define at line 226
	%reg_9 = alloca i32
	%reg_7 = alloca i32
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 226
	%reg_3 = add i32 0, 0
	store i32 %reg_3, ptr %reg_2
	br label %Block2
Block2:  ;While condition at line 228
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @PRE_SYM_COUNT
	%reg_6 = icmp slt i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block3, label %Block4
Block3:  ;While body at line 228
	%reg_8 = add i32 0, 0
	store i32 %reg_8, ptr %reg_7
	%reg_10 = add i32 0, 0
	store i32 %reg_10, ptr %reg_9
	br label %Block5
Block4:  ;While end at line 228
	%reg_46 = load i32, ptr %reg_1
	ret i32 %reg_46
Block5:  ;While condition at line 230
	%reg_11 = load i32, ptr %reg_1
	%reg_12 = load i32, ptr %reg_7
	%reg_13 = add i32 %reg_11, %reg_12
	%reg_14 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	%reg_16 = icmp ne i32 %reg_15, 0
	br i1 %reg_16, label %Block6, label %Block7
Block6:  ;While body at line 230
	%reg_17 = load i32, ptr %reg_1
	%reg_18 = load i32, ptr %reg_7
	%reg_19 = add i32 %reg_17, %reg_18
	%reg_20 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = load i32, ptr %reg_2
	%reg_23 = load i32, ptr %reg_7
	%reg_24 = getelementptr [22 x [8 x i32]], ptr @PREDEF_SYMS, i32 0, i32 %reg_22, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = icmp ne i32 %reg_21, %reg_25
	br i1 %reg_26, label %Block8, label %Block9
Block7:  ;While end at line 230
	%reg_31 = load i32, ptr %reg_9
	%reg_32 = icmp eq i32 %reg_31, 0
	br i1 %reg_32, label %Block13, label %Block12
Block8:  ;If then at line 231
	%reg_27 = add i32 1, 0
	store i32 %reg_27, ptr %reg_9
	br label %Block7
Block9:  ;If end at line 231
	%reg_28 = load i32, ptr %reg_7
	%reg_29 = add i32 1, 0
	%reg_30 = add i32 %reg_28, %reg_29
	store i32 %reg_30, ptr %reg_7
	br label %Block5
Block10:  ;Break at line 233
	br label %Block9
Block11:  ;If then at line 237
	%reg_38 = load i32, ptr %reg_1
	store i32 %reg_38, ptr @next_sym
	%reg_39 = load i32, ptr %reg_2
	%reg_40 = sub i32 0, %reg_39
	%reg_41 = add i32 1, 0
	%reg_42 = sub i32 %reg_40, %reg_41
	ret i32 %reg_42
	br label %Block12
Block12:  ;If end at line 237
	%reg_43 = load i32, ptr %reg_2
	%reg_44 = add i32 1, 0
	%reg_45 = add i32 %reg_43, %reg_44
	store i32 %reg_45, ptr %reg_2
	br label %Block2
Block13:  ;And opertor at line 237
	%reg_33 = load i32, ptr %reg_2
	%reg_34 = load i32, ptr %reg_7
	%reg_35 = getelementptr [22 x [8 x i32]], ptr @PREDEF_SYMS, i32 0, i32 %reg_33, i32 %reg_34
	%reg_36 = load i32, ptr %reg_35
	%reg_37 = icmp eq i32 %reg_36, 0
	br i1 %reg_37, label %Block11, label %Block12
}

define i32 @is_eq_sym(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 247
	%reg_15 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 247
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 248
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 248
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = add i32 0, 0
	%reg_10 = icmp slt i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block4, label %Block6
Block4:  ;If then at line 249
	%reg_14 = add i32 0, 0
	ret i32 %reg_14
	br label %Block5
Block5:  ;If end at line 249
	%reg_16 = add i32 0, 0
	store i32 %reg_16, ptr %reg_15
	br label %Block7
Block6:  ;Or opertor at line 249
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = add i32 0, 0
	%reg_13 = icmp slt i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block4, label %Block5
Block7:  ;While condition at line 251
	%reg_17 = load i32, ptr %reg_2
	%reg_18 = load i32, ptr %reg_15
	%reg_19 = add i32 %reg_17, %reg_18
	%reg_20 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	%reg_22 = icmp ne i32 %reg_21, 0
	br i1 %reg_22, label %Block8, label %Block9
Block8:  ;While body at line 251
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = load i32, ptr %reg_15
	%reg_25 = add i32 %reg_23, %reg_24
	%reg_26 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	%reg_28 = load i32, ptr %reg_3
	%reg_29 = load i32, ptr %reg_15
	%reg_30 = add i32 %reg_28, %reg_29
	%reg_31 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	%reg_33 = icmp ne i32 %reg_27, %reg_32
	br i1 %reg_33, label %Block10, label %Block11
Block9:  ;While end at line 251
	%reg_38 = load i32, ptr %reg_2
	%reg_39 = load i32, ptr %reg_15
	%reg_40 = add i32 %reg_38, %reg_39
	%reg_41 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_40
	%reg_42 = load i32, ptr %reg_41
	%reg_43 = load i32, ptr %reg_3
	%reg_44 = load i32, ptr %reg_15
	%reg_45 = add i32 %reg_43, %reg_44
	%reg_46 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	%reg_48 = call i32 @eq(i32 %reg_42, i32 %reg_47)
	ret i32 %reg_48
Block10:  ;If then at line 252
	%reg_34 = add i32 0, 0
	ret i32 %reg_34
	br label %Block11
Block11:  ;If end at line 252
	%reg_35 = load i32, ptr %reg_15
	%reg_36 = add i32 1, 0
	%reg_37 = add i32 %reg_35, %reg_36
	store i32 %reg_37, ptr %reg_15
	br label %Block7
}

define void @print_sym(i32 %reg_0)
{
Block0:  ;Func define at line 259
	%reg_25 = alloca i32
	%reg_5 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 259
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 0, 0
	%reg_4 = icmp slt i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block4
Block2:  ;If then at line 260
	%reg_6 = add i32 0, 0
	store i32 %reg_6, ptr %reg_5
	br label %Block5
Block3:  ;If end at line 260
	ret void
Block4:  ;If else at line 260
	%reg_26 = add i32 0, 0
	store i32 %reg_26, ptr %reg_25
	br label %Block8
Block5:  ;While condition at line 262
	%reg_7 = load i32, ptr %reg_1
	%reg_8 = sub i32 0, %reg_7
	%reg_9 = add i32 1, 0
	%reg_10 = sub i32 %reg_8, %reg_9
	%reg_11 = load i32, ptr %reg_5
	%reg_12 = getelementptr [22 x [8 x i32]], ptr @PREDEF_SYMS, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	%reg_14 = icmp ne i32 %reg_13, 0
	br i1 %reg_14, label %Block6, label %Block7
Block6:  ;While body at line 262
	%reg_15 = load i32, ptr %reg_1
	%reg_16 = sub i32 0, %reg_15
	%reg_17 = add i32 1, 0
	%reg_18 = sub i32 %reg_16, %reg_17
	%reg_19 = load i32, ptr %reg_5
	%reg_20 = getelementptr [22 x [8 x i32]], ptr @PREDEF_SYMS, i32 0, i32 %reg_18, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	call void @putch(i32 %reg_21)
	%reg_22 = load i32, ptr %reg_5
	%reg_23 = add i32 1, 0
	%reg_24 = add i32 %reg_22, %reg_23
	store i32 %reg_24, ptr %reg_5
	br label %Block5
Block7:  ;While end at line 262
	br label %Block3
Block8:  ;While condition at line 268
	%reg_27 = load i32, ptr %reg_1
	%reg_28 = load i32, ptr %reg_25
	%reg_29 = add i32 %reg_27, %reg_28
	%reg_30 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_29
	%reg_31 = load i32, ptr %reg_30
	%reg_32 = icmp ne i32 %reg_31, 0
	br i1 %reg_32, label %Block9, label %Block10
Block9:  ;While body at line 268
	%reg_33 = load i32, ptr %reg_1
	%reg_34 = load i32, ptr %reg_25
	%reg_35 = add i32 %reg_33, %reg_34
	%reg_36 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_35
	%reg_37 = load i32, ptr %reg_36
	call void @putch(i32 %reg_37)
	%reg_38 = load i32, ptr %reg_25
	%reg_39 = add i32 1, 0
	%reg_40 = add i32 %reg_38, %reg_39
	store i32 %reg_40, ptr %reg_25
	br label %Block8
Block10:  ;While end at line 268
	br label %Block3
}

define i32 @make_env(i32 %reg_0)
{
Block0:  ;Func define at line 280
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 280
	%reg_3 = call i32 @alloc_data()
	store i32 %reg_3, ptr %reg_2
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @DATA_TYPE
	%reg_6 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_4, i32 %reg_5
	%reg_7 = load i32, ptr @DATA_TYPE_ENV
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = load i32, ptr @DATA_VALUE
	%reg_10 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = add i32 0, 0
	store i32 %reg_11, ptr %reg_10
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = load i32, ptr @DATA_NEXT
	%reg_14 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_12, i32 %reg_13
	%reg_15 = load i32, ptr %reg_1
	store i32 %reg_15, ptr %reg_14
	%reg_16 = load i32, ptr %reg_2
	ret i32 %reg_16
}

define void @env_add(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 289
	%reg_13 = alloca i32
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 289
	%reg_7 = load i32, ptr %reg_4
	%reg_8 = call i32 @make_symbol(i32 %reg_7)
	store i32 %reg_8, ptr %reg_6
	%reg_9 = load i32, ptr %reg_6
	%reg_10 = load i32, ptr @DATA_NEXT
	%reg_11 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_9, i32 %reg_10
	%reg_12 = load i32, ptr %reg_5
	store i32 %reg_12, ptr %reg_11
	%reg_14 = load i32, ptr %reg_6
	%reg_15 = call i32 @make_list(i32 %reg_14)
	store i32 %reg_15, ptr %reg_13
	%reg_16 = load i32, ptr %reg_13
	%reg_17 = load i32, ptr @DATA_NEXT
	%reg_18 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_16, i32 %reg_17
	%reg_19 = load i32, ptr %reg_3
	%reg_20 = load i32, ptr @DATA_VALUE
	%reg_21 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_19, i32 %reg_20
	%reg_22 = load i32, ptr %reg_21
	store i32 %reg_22, ptr %reg_18
	%reg_23 = load i32, ptr %reg_3
	%reg_24 = load i32, ptr @DATA_VALUE
	%reg_25 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_23, i32 %reg_24
	%reg_26 = load i32, ptr %reg_13
	store i32 %reg_26, ptr %reg_25
	ret void
}

define i32 @env_get(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 301
	%reg_11 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 301
	%reg_5 = load i32, ptr %reg_2
	%reg_6 = load i32, ptr @DATA_VALUE
	%reg_7 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_5, i32 %reg_6
	%reg_8 = load i32, ptr %reg_7
	store i32 %reg_8, ptr %reg_4
	br label %Block2
Block2:  ;While condition at line 303
	%reg_9 = load i32, ptr %reg_4
	%reg_10 = icmp ne i32 %reg_9, 0
	br i1 %reg_10, label %Block3, label %Block4
Block3:  ;While body at line 303
	%reg_12 = load i32, ptr %reg_4
	%reg_13 = load i32, ptr @DATA_VALUE
	%reg_14 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_12, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	store i32 %reg_15, ptr %reg_11
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = load i32, ptr %reg_11
	%reg_18 = load i32, ptr @DATA_VALUE
	%reg_19 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_17, i32 %reg_18
	%reg_20 = load i32, ptr %reg_19
	%reg_21 = call i32 @is_eq_sym(i32 %reg_16, i32 %reg_20)
	%reg_22 = icmp ne i32 %reg_21, 0
	br i1 %reg_22, label %Block5, label %Block6
Block4:  ;While end at line 303
	%reg_28 = add i32 0, 0
	ret i32 %reg_28
Block5:  ;If then at line 305
	%reg_23 = load i32, ptr %reg_4
	ret i32 %reg_23
	br label %Block6
Block6:  ;If end at line 305
	%reg_24 = load i32, ptr %reg_4
	%reg_25 = load i32, ptr @DATA_NEXT
	%reg_26 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_24, i32 %reg_25
	%reg_27 = load i32, ptr %reg_26
	store i32 %reg_27, ptr %reg_4
	br label %Block2
}

define void @env_set(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 313
	%reg_12 = alloca i32
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 313
	%reg_7 = load i32, ptr %reg_3
	%reg_8 = load i32, ptr %reg_4
	%reg_9 = call i32 @env_get(i32 %reg_7, i32 %reg_8)
	store i32 %reg_9, ptr %reg_6
	%reg_10 = load i32, ptr %reg_6
	%reg_11 = icmp ne i32 %reg_10, 0
	br i1 %reg_11, label %Block2, label %Block4
Block2:  ;If then at line 315
	%reg_13 = load i32, ptr %reg_6
	%reg_14 = load i32, ptr @DATA_VALUE
	%reg_15 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_13, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	store i32 %reg_16, ptr %reg_12
	%reg_17 = load i32, ptr %reg_12
	%reg_18 = load i32, ptr @DATA_NEXT
	%reg_19 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_17, i32 %reg_18
	%reg_20 = load i32, ptr %reg_19
	call void @free_data(i32 %reg_20)
	%reg_21 = load i32, ptr %reg_12
	%reg_22 = load i32, ptr @DATA_NEXT
	%reg_23 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_21, i32 %reg_22
	%reg_24 = load i32, ptr %reg_5
	store i32 %reg_24, ptr %reg_23
	br label %Block3
Block3:  ;If end at line 315
	ret void
Block4:  ;If else at line 315
	%reg_25 = load i32, ptr %reg_3
	%reg_26 = load i32, ptr %reg_4
	%reg_27 = load i32, ptr %reg_5
	call void @env_add(i32 %reg_25, i32 %reg_26, i32 %reg_27)
	br label %Block3
}

define i32 @env_find(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 326
	%reg_7 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 326
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = icmp eq i32 %reg_4, 0
	br i1 %reg_5, label %Block2, label %Block3
Block2:  ;If then at line 327
	%reg_6 = add i32 0, 0
	ret i32 %reg_6
	br label %Block3
Block3:  ;If end at line 327
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = load i32, ptr %reg_3
	%reg_10 = call i32 @env_get(i32 %reg_8, i32 %reg_9)
	store i32 %reg_10, ptr %reg_7
	%reg_11 = load i32, ptr %reg_7
	%reg_12 = icmp ne i32 %reg_11, 0
	br i1 %reg_12, label %Block4, label %Block5
Block4:  ;If then at line 329
	%reg_13 = load i32, ptr %reg_7
	%reg_14 = load i32, ptr @DATA_VALUE
	%reg_15 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_13, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = load i32, ptr @DATA_NEXT
	%reg_18 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_16, i32 %reg_17
	%reg_19 = load i32, ptr %reg_18
	ret i32 %reg_19
	br label %Block5
Block5:  ;If end at line 329
	%reg_20 = load i32, ptr %reg_2
	%reg_21 = load i32, ptr @DATA_NEXT
	%reg_22 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_20, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	%reg_24 = load i32, ptr %reg_3
	%reg_25 = call i32 @env_find(i32 %reg_23, i32 %reg_24)
	ret i32 %reg_25
}

define i32 @make_func(i32 %reg_0, i32 %reg_1, i32 %reg_2)
{
Block0:  ;Func define at line 338
	%reg_6 = alloca i32
	%reg_5 = alloca i32
	%reg_4 = alloca i32
	%reg_3 = alloca i32
	store i32 %reg_0, ptr %reg_3
	store i32 %reg_1, ptr %reg_4
	store i32 %reg_2, ptr %reg_5
	br label %Block1
Block1:  ;Func body at line 338
	%reg_7 = call i32 @alloc_data()
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_6
	%reg_9 = load i32, ptr @DATA_TYPE
	%reg_10 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = load i32, ptr @DATA_TYPE_FUNC
	store i32 %reg_11, ptr %reg_10
	%reg_12 = load i32, ptr %reg_6
	%reg_13 = load i32, ptr @DATA_VALUE
	%reg_14 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_12, i32 %reg_13
	%reg_15 = load i32, ptr %reg_3
	store i32 %reg_15, ptr %reg_14
	%reg_16 = load i32, ptr %reg_3
	%reg_17 = load i32, ptr @DATA_NEXT
	%reg_18 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_16, i32 %reg_17
	%reg_19 = load i32, ptr %reg_4
	store i32 %reg_19, ptr %reg_18
	%reg_20 = load i32, ptr %reg_4
	%reg_21 = load i32, ptr @DATA_NEXT
	%reg_22 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_20, i32 %reg_21
	%reg_23 = load i32, ptr %reg_5
	store i32 %reg_23, ptr %reg_22
	%reg_24 = load i32, ptr %reg_6
	ret i32 %reg_24
}

define i32 @is_space(i32 %reg_0)
{
Block0:  ;Func define at line 367
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 367
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 32, 0
	%reg_4 = icmp eq i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block2, label %Block6
Block2:  ;If then at line 367
	%reg_14 = add i32 1, 0
	ret i32 %reg_14
	br label %Block3
Block3:  ;If end at line 367
	%reg_15 = add i32 0, 0
	ret i32 %reg_15
Block4:  ;Or opertor at line 367
	%reg_11 = load i32, ptr %reg_1
	%reg_12 = add i32 13, 0
	%reg_13 = icmp eq i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block2, label %Block3
Block5:  ;Or opertor at line -1
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = add i32 10, 0
	%reg_10 = icmp eq i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block2, label %Block4
Block6:  ;Or opertor at line -1
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 9, 0
	%reg_7 = icmp eq i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block5
}

define i32 @is_digit(i32 %reg_0)
{
Block0:  ;Func define at line 370
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 370
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = add i32 48, 0
	%reg_4 = icmp sge i32 %reg_2, %reg_3
	br i1 %reg_4, label %Block4, label %Block3
Block2:  ;If then at line 370
	%reg_8 = add i32 1, 0
	ret i32 %reg_8
	br label %Block3
Block3:  ;If end at line 370
	%reg_9 = add i32 0, 0
	ret i32 %reg_9
Block4:  ;And opertor at line 370
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = add i32 57, 0
	%reg_7 = icmp sle i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
}

define i32 @next_token()
{
Block0:  ;Func define at line 374
	%reg_40 = alloca i32
	%reg_25 = alloca i32
	%reg_14 = alloca i32
	br label %Block1
Block1:  ;Func body at line 374
	br label %Block2
Block2:  ;While condition at line 376
	%reg_0 = load i32, ptr @last_char
	%reg_1 = call i32 @is_space(i32 %reg_0)
	%reg_2 = icmp ne i32 %reg_1, 0
	br i1 %reg_2, label %Block3, label %Block4
Block3:  ;While body at line 376
	%reg_3 = call i32 @getch()
	store i32 %reg_3, ptr @last_char
	br label %Block2
Block4:  ;While end at line 376
	%reg_4 = load i32, ptr @last_char
	%reg_5 = add i32 64, 0
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block5, label %Block6
Block5:  ;If then at line 378
	%reg_7 = load i32, ptr @TOKEN_EOF
	ret i32 %reg_7
	br label %Block6
Block6:  ;If end at line 378
	%reg_8 = load i32, ptr @last_char
	%reg_9 = add i32 39, 0
	%reg_10 = icmp sge i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block9, label %Block8
Block7:  ;If then at line 380
	%reg_15 = load i32, ptr @last_char
	store i32 %reg_15, ptr %reg_14
	%reg_16 = call i32 @getch()
	store i32 %reg_16, ptr @last_char
	%reg_17 = load i32, ptr @TOKEN_QUOTE
	%reg_18 = load i32, ptr %reg_14
	%reg_19 = add i32 %reg_17, %reg_18
	%reg_20 = add i32 39, 0
	%reg_21 = sub i32 %reg_19, %reg_20
	ret i32 %reg_21
	br label %Block8
Block8:  ;If end at line 380
	%reg_22 = load i32, ptr @last_char
	%reg_23 = call i32 @is_digit(i32 %reg_22)
	%reg_24 = icmp ne i32 %reg_23, 0
	br i1 %reg_24, label %Block10, label %Block11
Block9:  ;And opertor at line 380
	%reg_11 = load i32, ptr @last_char
	%reg_12 = add i32 41, 0
	%reg_13 = icmp sle i32 %reg_11, %reg_12
	br i1 %reg_13, label %Block7, label %Block8
Block10:  ;If then at line 386
	%reg_26 = add i32 0, 0
	store i32 %reg_26, ptr %reg_25
	br label %Block12
Block11:  ;If end at line 386
	%reg_41 = load i32, ptr @next_sym
	store i32 %reg_41, ptr %reg_40
	br label %Block15
Block12:  ;While condition at line 388
	%reg_27 = load i32, ptr @last_char
	%reg_28 = call i32 @is_digit(i32 %reg_27)
	%reg_29 = icmp ne i32 %reg_28, 0
	br i1 %reg_29, label %Block13, label %Block14
Block13:  ;While body at line 388
	%reg_30 = load i32, ptr %reg_25
	%reg_31 = add i32 10, 0
	%reg_32 = mul i32 %reg_30, %reg_31
	%reg_33 = load i32, ptr @last_char
	%reg_34 = add i32 %reg_32, %reg_33
	%reg_35 = add i32 48, 0
	%reg_36 = sub i32 %reg_34, %reg_35
	store i32 %reg_36, ptr %reg_25
	%reg_37 = call i32 @getch()
	store i32 %reg_37, ptr @last_char
	br label %Block12
Block14:  ;While end at line 388
	%reg_38 = load i32, ptr %reg_25
	store i32 %reg_38, ptr @last_token
	%reg_39 = load i32, ptr @TOKEN_NUMBER
	ret i32 %reg_39
	br label %Block11
Block15:  ;While condition at line 397
	%reg_42 = add i32 1, 0
	%reg_43 = icmp ne i32 %reg_42, 0
	br i1 %reg_43, label %Block16, label %Block17
Block16:  ;While body at line 397
	%reg_44 = load i32, ptr @last_char
	%reg_45 = add i32 1, 0
	%reg_46 = sub i32 0, %reg_45
	%reg_47 = icmp eq i32 %reg_44, %reg_46
	br i1 %reg_47, label %Block18, label %Block19
Block17:  ;While end at line 397
	%reg_64 = load i32, ptr @next_sym
	%reg_65 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_64
	%reg_66 = add i32 0, 0
	store i32 %reg_66, ptr %reg_65
	%reg_67 = load i32, ptr @next_sym
	%reg_68 = add i32 1, 0
	%reg_69 = add i32 %reg_67, %reg_68
	store i32 %reg_69, ptr @next_sym
	%reg_70 = load i32, ptr @next_sym
	%reg_71 = load i32, ptr @MAX_SYM_BUF_LEN
	%reg_72 = icmp sge i32 %reg_70, %reg_71
	br i1 %reg_72, label %Block28, label %Block29
Block18:  ;If then at line 398
	br label %Block17
Block19:  ;If end at line 398
	%reg_48 = load i32, ptr @last_char
	%reg_49 = add i32 39, 0
	%reg_50 = icmp sge i32 %reg_48, %reg_49
	br i1 %reg_50, label %Block23, label %Block22
Block20:  ;Break at line 398
	br label %Block19
Block21:  ;If then at line 399
	br label %Block17
Block22:  ;If end at line 399
	%reg_54 = load i32, ptr @last_char
	%reg_55 = call i32 @is_space(i32 %reg_54)
	%reg_56 = icmp ne i32 %reg_55, 0
	br i1 %reg_56, label %Block25, label %Block26
Block23:  ;And opertor at line 399
	%reg_51 = load i32, ptr @last_char
	%reg_52 = add i32 41, 0
	%reg_53 = icmp sle i32 %reg_51, %reg_52
	br i1 %reg_53, label %Block21, label %Block22
Block24:  ;Break at line 399
	br label %Block22
Block25:  ;If then at line 400
	br label %Block17
Block26:  ;If end at line 400
	%reg_57 = load i32, ptr @next_sym
	%reg_58 = getelementptr [4096 x i32], ptr @sym_buf, i32 0, i32 %reg_57
	%reg_59 = load i32, ptr @last_char
	store i32 %reg_59, ptr %reg_58
	%reg_60 = load i32, ptr @next_sym
	%reg_61 = add i32 1, 0
	%reg_62 = add i32 %reg_60, %reg_61
	store i32 %reg_62, ptr @next_sym
	%reg_63 = call i32 @getch()
	store i32 %reg_63, ptr @last_char
	br label %Block15
Block27:  ;Break at line 400
	br label %Block26
Block28:  ;If then at line 407
	%reg_73 = load i32, ptr @ERR_BUFFER_OVERFLOW
	%reg_74 = call i32 @panic(i32 %reg_73)
	br label %Block29
Block29:  ;If end at line 407
	%reg_75 = load i32, ptr %reg_40
	%reg_76 = call i32 @is_predef_sym(i32 %reg_75)
	store i32 %reg_76, ptr @last_token
	%reg_77 = load i32, ptr @TOKEN_SYMBOL
	ret i32 %reg_77
}

define i32 @parse()
{
Block0:  ;Func define at line 422
	%reg_49 = alloca i32
	%reg_44 = alloca i32
	%reg_41 = alloca i32
	%reg_33 = alloca i32
	%reg_26 = alloca i32
	%reg_24 = alloca i32
	%reg_15 = alloca i32
	%reg_7 = alloca i32
	br label %Block1
Block1:  ;Func body at line 422
	%reg_0 = load i32, ptr @last_token_type
	%reg_1 = load i32, ptr @TOKEN_EOF
	%reg_2 = icmp eq i32 %reg_0, %reg_1
	br i1 %reg_2, label %Block2, label %Block3
Block2:  ;If then at line 423
	%reg_3 = add i32 0, 0
	ret i32 %reg_3
	br label %Block3
Block3:  ;If end at line 423
	%reg_4 = load i32, ptr @last_token_type
	%reg_5 = load i32, ptr @TOKEN_SYMBOL
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block4, label %Block5
Block4:  ;If then at line 424
	%reg_8 = load i32, ptr @last_token
	%reg_9 = call i32 @make_symbol(i32 %reg_8)
	store i32 %reg_9, ptr %reg_7
	%reg_10 = call i32 @next_token()
	store i32 %reg_10, ptr @last_token_type
	%reg_11 = load i32, ptr %reg_7
	ret i32 %reg_11
	br label %Block5
Block5:  ;If end at line 424
	%reg_12 = load i32, ptr @last_token_type
	%reg_13 = load i32, ptr @TOKEN_NUMBER
	%reg_14 = icmp eq i32 %reg_12, %reg_13
	br i1 %reg_14, label %Block6, label %Block7
Block6:  ;If then at line 429
	%reg_16 = load i32, ptr @last_token
	%reg_17 = call i32 @make_number(i32 %reg_16)
	store i32 %reg_17, ptr %reg_15
	%reg_18 = call i32 @next_token()
	store i32 %reg_18, ptr @last_token_type
	%reg_19 = load i32, ptr %reg_15
	ret i32 %reg_19
	br label %Block7
Block7:  ;If end at line 429
	%reg_20 = load i32, ptr @last_token_type
	%reg_21 = load i32, ptr @TOKEN_QUOTE
	%reg_22 = icmp eq i32 %reg_20, %reg_21
	br i1 %reg_22, label %Block8, label %Block9
Block8:  ;If then at line 434
	%reg_23 = call i32 @next_token()
	store i32 %reg_23, ptr @last_token_type
	%reg_25 = call i32 @parse()
	store i32 %reg_25, ptr %reg_24
	%reg_27 = load i32, ptr @SYM_QUOTE
	%reg_28 = call i32 @make_symbol(i32 %reg_27)
	store i32 %reg_28, ptr %reg_26
	%reg_29 = load i32, ptr %reg_26
	%reg_30 = load i32, ptr @DATA_NEXT
	%reg_31 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_29, i32 %reg_30
	%reg_32 = load i32, ptr %reg_24
	store i32 %reg_32, ptr %reg_31
	%reg_34 = load i32, ptr %reg_26
	%reg_35 = call i32 @make_list(i32 %reg_34)
	store i32 %reg_35, ptr %reg_33
	%reg_36 = load i32, ptr %reg_33
	ret i32 %reg_36
	br label %Block9
Block9:  ;If end at line 434
	%reg_37 = load i32, ptr @last_token_type
	%reg_38 = load i32, ptr @TOKEN_LPAREN
	%reg_39 = icmp eq i32 %reg_37, %reg_38
	br i1 %reg_39, label %Block10, label %Block11
Block10:  ;If then at line 442
	%reg_40 = call i32 @next_token()
	store i32 %reg_40, ptr @last_token_type
	%reg_42 = add i32 0, 0
	%reg_43 = call i32 @make_list(i32 %reg_42)
	store i32 %reg_43, ptr %reg_41
	%reg_45 = add i32 0, 0
	store i32 %reg_45, ptr %reg_44
	br label %Block12
Block11:  ;If end at line 442
	%reg_64 = load i32, ptr @ERR_PARSE_ERROR
	%reg_65 = call i32 @panic(i32 %reg_64)
	ret i32 %reg_65
Block12:  ;While condition at line 445
	%reg_46 = load i32, ptr @last_token_type
	%reg_47 = load i32, ptr @TOKEN_RPAREN
	%reg_48 = icmp ne i32 %reg_46, %reg_47
	br i1 %reg_48, label %Block13, label %Block14
Block13:  ;While body at line 445
	%reg_50 = call i32 @parse()
	store i32 %reg_50, ptr %reg_49
	%reg_51 = load i32, ptr %reg_44
	%reg_52 = icmp ne i32 %reg_51, 0
	br i1 %reg_52, label %Block15, label %Block17
Block14:  ;While end at line 445
	%reg_62 = call i32 @next_token()
	store i32 %reg_62, ptr @last_token_type
	%reg_63 = load i32, ptr %reg_41
	ret i32 %reg_63
	br label %Block11
Block15:  ;If then at line 447
	%reg_53 = load i32, ptr %reg_44
	%reg_54 = load i32, ptr @DATA_NEXT
	%reg_55 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_53, i32 %reg_54
	%reg_56 = load i32, ptr %reg_49
	store i32 %reg_56, ptr %reg_55
	br label %Block16
Block16:  ;If end at line 447
	%reg_61 = load i32, ptr %reg_49
	store i32 %reg_61, ptr %reg_44
	br label %Block12
Block17:  ;If else at line 447
	%reg_57 = load i32, ptr %reg_41
	%reg_58 = load i32, ptr @DATA_VALUE
	%reg_59 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_57, i32 %reg_58
	%reg_60 = load i32, ptr %reg_49
	store i32 %reg_60, ptr %reg_59
	br label %Block16
}

define i32 @make_bool(i32 %reg_0)
{
Block0:  ;Func define at line 465
	%reg_2 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 465
	%reg_3 = call i32 @alloc_data()
	store i32 %reg_3, ptr %reg_2
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @DATA_TYPE
	%reg_6 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_4, i32 %reg_5
	%reg_7 = load i32, ptr @DATA_TYPE_SYMBOL
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = icmp ne i32 %reg_8, 0
	br i1 %reg_9, label %Block2, label %Block4
Block2:  ;If then at line 468
	%reg_10 = load i32, ptr %reg_2
	%reg_11 = load i32, ptr @DATA_VALUE
	%reg_12 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = load i32, ptr @SYM_T
	store i32 %reg_13, ptr %reg_12
	br label %Block3
Block3:  ;If end at line 468
	%reg_18 = load i32, ptr %reg_2
	ret i32 %reg_18
Block4:  ;If else at line 468
	%reg_14 = load i32, ptr %reg_2
	%reg_15 = load i32, ptr @DATA_VALUE
	%reg_16 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_14, i32 %reg_15
	%reg_17 = load i32, ptr @SYM_F
	store i32 %reg_17, ptr %reg_16
	br label %Block3
}

define i32 @is_true(i32 %reg_0)
{
Block0:  ;Func define at line 477
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 477
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = icmp eq i32 %reg_2, 0
	br i1 %reg_3, label %Block2, label %Block3
Block2:  ;If then at line 478
	%reg_4 = add i32 1, 0
	ret i32 %reg_4
	br label %Block3
Block3:  ;If end at line 478
	%reg_5 = load i32, ptr %reg_1
	%reg_6 = load i32, ptr @DATA_TYPE
	%reg_7 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_5, i32 %reg_6
	%reg_8 = load i32, ptr %reg_7
	%reg_9 = load i32, ptr @DATA_TYPE_SYMBOL
	%reg_10 = icmp ne i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block4, label %Block5
Block4:  ;If then at line 479
	%reg_11 = load i32, ptr @ERR_TYPE_MISMATCH
	%reg_12 = call i32 @panic(i32 %reg_11)
	br label %Block5
Block5:  ;If end at line 479
	%reg_13 = load i32, ptr %reg_1
	%reg_14 = load i32, ptr @DATA_VALUE
	%reg_15 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_13, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = load i32, ptr @SYM_T
	%reg_18 = icmp eq i32 %reg_16, %reg_17
	br i1 %reg_18, label %Block6, label %Block7
Block6:  ;If then at line 480
	%reg_19 = add i32 1, 0
	ret i32 %reg_19
	br label %Block7
Block7:  ;If end at line 480
	%reg_20 = load i32, ptr %reg_1
	%reg_21 = load i32, ptr @DATA_VALUE
	%reg_22 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_20, i32 %reg_21
	%reg_23 = load i32, ptr %reg_22
	%reg_24 = load i32, ptr @SYM_F
	%reg_25 = icmp eq i32 %reg_23, %reg_24
	br i1 %reg_25, label %Block8, label %Block9
Block8:  ;If then at line 481
	%reg_26 = add i32 0, 0
	ret i32 %reg_26
	br label %Block9
Block9:  ;If end at line 481
	%reg_27 = load i32, ptr @ERR_TYPE_MISMATCH
	%reg_28 = call i32 @panic(i32 %reg_27)
	ret i32 %reg_28
}

define i32 @is_equal(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 486
	%reg_64 = alloca i32
	%reg_59 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 486
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr %reg_3
	%reg_6 = icmp eq i32 %reg_4, %reg_5
	br i1 %reg_6, label %Block2, label %Block3
Block2:  ;If then at line 487
	%reg_7 = add i32 1, 0
	ret i32 %reg_7
	br label %Block3
Block3:  ;If end at line 487
	%reg_8 = load i32, ptr %reg_2
	%reg_9 = icmp eq i32 %reg_8, 0
	br i1 %reg_9, label %Block4, label %Block6
Block4:  ;If then at line 488
	%reg_12 = add i32 0, 0
	ret i32 %reg_12
	br label %Block5
Block5:  ;If end at line 488
	%reg_13 = load i32, ptr %reg_2
	%reg_14 = load i32, ptr @DATA_TYPE
	%reg_15 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_13, i32 %reg_14
	%reg_16 = load i32, ptr %reg_15
	%reg_17 = load i32, ptr %reg_3
	%reg_18 = load i32, ptr @DATA_TYPE
	%reg_19 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_17, i32 %reg_18
	%reg_20 = load i32, ptr %reg_19
	%reg_21 = icmp ne i32 %reg_16, %reg_20
	br i1 %reg_21, label %Block7, label %Block8
Block6:  ;Or opertor at line 488
	%reg_10 = load i32, ptr %reg_3
	%reg_11 = icmp eq i32 %reg_10, 0
	br i1 %reg_11, label %Block4, label %Block5
Block7:  ;If then at line 489
	%reg_22 = add i32 0, 0
	ret i32 %reg_22
	br label %Block8
Block8:  ;If end at line 489
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = load i32, ptr @DATA_TYPE
	%reg_25 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_23, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = load i32, ptr @DATA_TYPE_SYMBOL
	%reg_28 = icmp eq i32 %reg_26, %reg_27
	br i1 %reg_28, label %Block9, label %Block10
Block9:  ;If then at line 490
	%reg_29 = load i32, ptr %reg_2
	%reg_30 = load i32, ptr @DATA_VALUE
	%reg_31 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_29, i32 %reg_30
	%reg_32 = load i32, ptr %reg_31
	%reg_33 = load i32, ptr %reg_3
	%reg_34 = load i32, ptr @DATA_VALUE
	%reg_35 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_33, i32 %reg_34
	%reg_36 = load i32, ptr %reg_35
	%reg_37 = call i32 @is_eq_sym(i32 %reg_32, i32 %reg_36)
	ret i32 %reg_37
	br label %Block10
Block10:  ;If end at line 490
	%reg_38 = load i32, ptr %reg_2
	%reg_39 = load i32, ptr @DATA_TYPE
	%reg_40 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_38, i32 %reg_39
	%reg_41 = load i32, ptr %reg_40
	%reg_42 = load i32, ptr @DATA_TYPE_NUMBER
	%reg_43 = icmp eq i32 %reg_41, %reg_42
	br i1 %reg_43, label %Block11, label %Block12
Block11:  ;If then at line 493
	%reg_44 = load i32, ptr %reg_2
	%reg_45 = load i32, ptr @DATA_VALUE
	%reg_46 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_44, i32 %reg_45
	%reg_47 = load i32, ptr %reg_46
	%reg_48 = load i32, ptr %reg_3
	%reg_49 = load i32, ptr @DATA_VALUE
	%reg_50 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_48, i32 %reg_49
	%reg_51 = load i32, ptr %reg_50
	%reg_52 = call i32 @eq(i32 %reg_47, i32 %reg_51)
	ret i32 %reg_52
	br label %Block12
Block12:  ;If end at line 493
	%reg_53 = load i32, ptr %reg_2
	%reg_54 = load i32, ptr @DATA_TYPE
	%reg_55 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_53, i32 %reg_54
	%reg_56 = load i32, ptr %reg_55
	%reg_57 = load i32, ptr @DATA_TYPE_LIST
	%reg_58 = icmp eq i32 %reg_56, %reg_57
	br i1 %reg_58, label %Block13, label %Block14
Block13:  ;If then at line 496
	%reg_60 = load i32, ptr %reg_2
	%reg_61 = load i32, ptr @DATA_VALUE
	%reg_62 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_60, i32 %reg_61
	%reg_63 = load i32, ptr %reg_62
	store i32 %reg_63, ptr %reg_59
	%reg_65 = load i32, ptr %reg_3
	%reg_66 = load i32, ptr @DATA_VALUE
	%reg_67 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_65, i32 %reg_66
	%reg_68 = load i32, ptr %reg_67
	store i32 %reg_68, ptr %reg_64
	br label %Block15
Block14:  ;If end at line 496
	%reg_92 = load i32, ptr @ERR_TYPE_MISMATCH
	%reg_93 = call i32 @panic(i32 %reg_92)
	ret i32 %reg_93
Block15:  ;While condition at line 499
	%reg_69 = load i32, ptr %reg_59
	%reg_70 = icmp ne i32 %reg_69, 0
	br i1 %reg_70, label %Block18, label %Block17
Block16:  ;While body at line 499
	%reg_73 = load i32, ptr %reg_59
	%reg_74 = load i32, ptr %reg_64
	%reg_75 = call i32 @is_equal(i32 %reg_73, i32 %reg_74)
	%reg_76 = icmp eq i32 %reg_75, 0
	br i1 %reg_76, label %Block19, label %Block20
Block17:  ;While end at line 499
	%reg_86 = load i32, ptr %reg_59
	%reg_87 = icmp ne i32 %reg_86, 0
	br i1 %reg_87, label %Block21, label %Block23
Block18:  ;And opertor at line 499
	%reg_71 = load i32, ptr %reg_64
	%reg_72 = icmp ne i32 %reg_71, 0
	br i1 %reg_72, label %Block16, label %Block17
Block19:  ;If then at line 500
	%reg_77 = add i32 0, 0
	ret i32 %reg_77
	br label %Block20
Block20:  ;If end at line 500
	%reg_78 = load i32, ptr %reg_59
	%reg_79 = load i32, ptr @DATA_NEXT
	%reg_80 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_78, i32 %reg_79
	%reg_81 = load i32, ptr %reg_80
	store i32 %reg_81, ptr %reg_59
	%reg_82 = load i32, ptr %reg_64
	%reg_83 = load i32, ptr @DATA_NEXT
	%reg_84 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_82, i32 %reg_83
	%reg_85 = load i32, ptr %reg_84
	store i32 %reg_85, ptr %reg_64
	br label %Block15
Block21:  ;If then at line 504
	%reg_90 = add i32 0, 0
	ret i32 %reg_90
	br label %Block22
Block22:  ;If end at line 504
	%reg_91 = add i32 1, 0
	ret i32 %reg_91
	br label %Block14
Block23:  ;Or opertor at line 504
	%reg_88 = load i32, ptr %reg_64
	%reg_89 = icmp ne i32 %reg_88, 0
	br i1 %reg_89, label %Block21, label %Block22
}

define i32 @unwrap_list(i32 %reg_0)
{
Block0:  ;Func define at line 512
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 512
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = load i32, ptr @DATA_TYPE
	%reg_4 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_2, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = load i32, ptr @DATA_TYPE_LIST
	%reg_7 = icmp ne i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
Block2:  ;If then at line 513
	%reg_8 = load i32, ptr @ERR_TYPE_MISMATCH
	%reg_9 = call i32 @panic(i32 %reg_8)
	br label %Block3
Block3:  ;If end at line 513
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = load i32, ptr @DATA_VALUE
	%reg_12 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	ret i32 %reg_13
}

define i32 @unwrap_symbol(i32 %reg_0)
{
Block0:  ;Func define at line 519
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 519
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = load i32, ptr @DATA_TYPE
	%reg_4 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_2, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = load i32, ptr @DATA_TYPE_SYMBOL
	%reg_7 = icmp ne i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
Block2:  ;If then at line 520
	%reg_8 = load i32, ptr @ERR_TYPE_MISMATCH
	%reg_9 = call i32 @panic(i32 %reg_8)
	br label %Block3
Block3:  ;If end at line 520
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = load i32, ptr @DATA_VALUE
	%reg_12 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	ret i32 %reg_13
}

define i32 @unwrap_number(i32 %reg_0)
{
Block0:  ;Func define at line 526
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 526
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = load i32, ptr @DATA_TYPE
	%reg_4 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_2, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = load i32, ptr @DATA_TYPE_NUMBER
	%reg_7 = icmp ne i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
Block2:  ;If then at line 527
	%reg_8 = load i32, ptr @ERR_TYPE_MISMATCH
	%reg_9 = call i32 @panic(i32 %reg_8)
	br label %Block3
Block3:  ;If end at line 527
	%reg_10 = load i32, ptr %reg_1
	%reg_11 = load i32, ptr @DATA_VALUE
	%reg_12 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_10, i32 %reg_11
	%reg_13 = load i32, ptr %reg_12
	ret i32 %reg_13
}

define i32 @eval(i32 %reg_0, i32 %reg_1)
{
Block0:  ;Func define at line 534
	%reg_503 = alloca i32
	%reg_500 = alloca i32
	%reg_497 = alloca i32
	%reg_493 = alloca i32
	%reg_489 = alloca i32
	%reg_473 = alloca i32
	%reg_453 = alloca i32
	%reg_449 = alloca i32
	%reg_445 = alloca i32
	%reg_442 = alloca i32
	%reg_431 = alloca i32
	%reg_428 = alloca i32
	%reg_412 = alloca i32
	%reg_400 = alloca i32
	%reg_384 = alloca i32
	%reg_369 = alloca i32
	%reg_364 = alloca i32
	%reg_357 = alloca i32
	%reg_354 = alloca i32
	%reg_350 = alloca i32
	%reg_341 = alloca i32
	%reg_332 = alloca i32
	%reg_328 = alloca i32
	%reg_324 = alloca i32
	%reg_308 = alloca i32
	%reg_296 = alloca i32
	%reg_289 = alloca i32
	%reg_285 = alloca i32
	%reg_263 = alloca i32
	%reg_259 = alloca i32
	%reg_239 = alloca i32
	%reg_235 = alloca i32
	%reg_231 = alloca i32
	%reg_215 = alloca i32
	%reg_202 = alloca i32
	%reg_198 = alloca i32
	%reg_176 = alloca i32
	%reg_172 = alloca i32
	%reg_141 = alloca i32
	%reg_134 = alloca i32
	%reg_113 = alloca i32
	%reg_108 = alloca i32
	%reg_102 = alloca i32
	%reg_97 = alloca i32
	%reg_93 = alloca i32
	%reg_88 = alloca i32
	%reg_83 = alloca i32
	%reg_78 = alloca i32
	%reg_64 = alloca i32
	%reg_46 = alloca i32
	%reg_37 = alloca i32
	%reg_10 = alloca i32
	%reg_3 = alloca i32
	%reg_2 = alloca i32
	store i32 %reg_0, ptr %reg_2
	store i32 %reg_1, ptr %reg_3
	br label %Block1
Block1:  ;Func body at line 534
	%reg_4 = load i32, ptr %reg_2
	%reg_5 = load i32, ptr @DATA_TYPE
	%reg_6 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_4, i32 %reg_5
	%reg_7 = load i32, ptr %reg_6
	%reg_8 = load i32, ptr @DATA_TYPE_SYMBOL
	%reg_9 = icmp eq i32 %reg_7, %reg_8
	br i1 %reg_9, label %Block2, label %Block3
Block2:  ;If then at line 536
	%reg_11 = load i32, ptr %reg_3
	%reg_12 = load i32, ptr %reg_2
	%reg_13 = load i32, ptr @DATA_VALUE
	%reg_14 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_12, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	%reg_16 = call i32 @env_find(i32 %reg_11, i32 %reg_15)
	store i32 %reg_16, ptr %reg_10
	%reg_17 = load i32, ptr %reg_10
	%reg_18 = icmp eq i32 %reg_17, 0
	br i1 %reg_18, label %Block4, label %Block5
Block3:  ;If end at line 536
	%reg_23 = load i32, ptr %reg_2
	%reg_24 = load i32, ptr @DATA_TYPE
	%reg_25 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_23, i32 %reg_24
	%reg_26 = load i32, ptr %reg_25
	%reg_27 = load i32, ptr @DATA_TYPE_NUMBER
	%reg_28 = icmp eq i32 %reg_26, %reg_27
	br i1 %reg_28, label %Block6, label %Block7
Block4:  ;If then at line 538
	%reg_19 = load i32, ptr @ERR_SYMBOL_NOT_FOUND
	%reg_20 = call i32 @panic(i32 %reg_19)
	br label %Block5
Block5:  ;If end at line 538
	%reg_21 = load i32, ptr %reg_10
	%reg_22 = call i32 @copy_ptr(i32 %reg_21)
	ret i32 %reg_22
	br label %Block3
Block6:  ;If then at line 542
	%reg_29 = load i32, ptr %reg_2
	%reg_30 = call i32 @copy_ptr(i32 %reg_29)
	ret i32 %reg_30
	br label %Block7
Block7:  ;If end at line 542
	%reg_31 = load i32, ptr %reg_2
	%reg_32 = load i32, ptr @DATA_TYPE
	%reg_33 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_31, i32 %reg_32
	%reg_34 = load i32, ptr %reg_33
	%reg_35 = load i32, ptr @DATA_TYPE_LIST
	%reg_36 = icmp eq i32 %reg_34, %reg_35
	br i1 %reg_36, label %Block8, label %Block9
Block8:  ;If then at line 544
	%reg_38 = load i32, ptr %reg_2
	%reg_39 = load i32, ptr @DATA_VALUE
	%reg_40 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_38, i32 %reg_39
	%reg_41 = load i32, ptr %reg_40
	store i32 %reg_41, ptr %reg_37
	%reg_42 = load i32, ptr %reg_37
	%reg_43 = icmp eq i32 %reg_42, 0
	br i1 %reg_43, label %Block10, label %Block11
Block9:  ;If end at line 544
	%reg_573 = load i32, ptr @ERR_INVALID_DATA_TYPE
	%reg_574 = call i32 @panic(i32 %reg_573)
	ret i32 %reg_574
Block10:  ;If then at line 547
	%reg_44 = load i32, ptr @ERR_INVALID_LIST
	%reg_45 = call i32 @panic(i32 %reg_44)
	br label %Block11
Block11:  ;If end at line 547
	%reg_47 = load i32, ptr %reg_37
	%reg_48 = load i32, ptr @DATA_NEXT
	%reg_49 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_47, i32 %reg_48
	%reg_50 = load i32, ptr %reg_49
	store i32 %reg_50, ptr %reg_46
	%reg_51 = load i32, ptr %reg_37
	%reg_52 = load i32, ptr @DATA_TYPE
	%reg_53 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_51, i32 %reg_52
	%reg_54 = load i32, ptr %reg_53
	%reg_55 = load i32, ptr @DATA_TYPE_SYMBOL
	%reg_56 = icmp ne i32 %reg_54, %reg_55
	br i1 %reg_56, label %Block12, label %Block14
Block12:  ;If then at line 550
	%reg_65 = load i32, ptr %reg_37
	%reg_66 = load i32, ptr %reg_3
	%reg_67 = call i32 @eval(i32 %reg_65, i32 %reg_66)
	store i32 %reg_67, ptr %reg_64
	%reg_68 = load i32, ptr %reg_64
	%reg_69 = icmp eq i32 %reg_68, 0
	br i1 %reg_69, label %Block15, label %Block17
Block13:  ;If end at line 550
	%reg_142 = load i32, ptr %reg_37
	%reg_143 = load i32, ptr @DATA_VALUE
	%reg_144 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_142, i32 %reg_143
	%reg_145 = load i32, ptr %reg_144
	store i32 %reg_145, ptr %reg_141
	%reg_146 = load i32, ptr %reg_141
	%reg_147 = load i32, ptr @SYM_QUOTE
	%reg_148 = icmp eq i32 %reg_146, %reg_147
	br i1 %reg_148, label %Block25, label %Block26
Block14:  ;Or opertor at line 550
	%reg_57 = load i32, ptr %reg_3
	%reg_58 = load i32, ptr %reg_37
	%reg_59 = load i32, ptr @DATA_VALUE
	%reg_60 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_58, i32 %reg_59
	%reg_61 = load i32, ptr %reg_60
	%reg_62 = call i32 @env_find(i32 %reg_57, i32 %reg_61)
	%reg_63 = icmp ne i32 %reg_62, 0
	br i1 %reg_63, label %Block12, label %Block13
Block15:  ;If then at line 554
	%reg_76 = load i32, ptr @ERR_INVALID_FUNC
	%reg_77 = call i32 @panic(i32 %reg_76)
	br label %Block16
Block16:  ;If end at line 554
	%reg_79 = load i32, ptr %reg_64
	%reg_80 = load i32, ptr @DATA_VALUE
	%reg_81 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_79, i32 %reg_80
	%reg_82 = load i32, ptr %reg_81
	store i32 %reg_82, ptr %reg_78
	%reg_84 = load i32, ptr %reg_78
	%reg_85 = load i32, ptr @DATA_NEXT
	%reg_86 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_84, i32 %reg_85
	%reg_87 = load i32, ptr %reg_86
	store i32 %reg_87, ptr %reg_83
	%reg_89 = load i32, ptr %reg_83
	%reg_90 = load i32, ptr @DATA_NEXT
	%reg_91 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_89, i32 %reg_90
	%reg_92 = load i32, ptr %reg_91
	store i32 %reg_92, ptr %reg_88
	%reg_94 = load i32, ptr %reg_88
	%reg_95 = call i32 @copy_ptr(i32 %reg_94)
	%reg_96 = call i32 @make_env(i32 %reg_95)
	store i32 %reg_96, ptr %reg_93
	%reg_98 = load i32, ptr %reg_78
	%reg_99 = load i32, ptr @DATA_VALUE
	%reg_100 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_98, i32 %reg_99
	%reg_101 = load i32, ptr %reg_100
	store i32 %reg_101, ptr %reg_97
	%reg_103 = load i32, ptr %reg_46
	store i32 %reg_103, ptr %reg_102
	br label %Block18
Block17:  ;Or opertor at line 554
	%reg_70 = load i32, ptr %reg_64
	%reg_71 = load i32, ptr @DATA_TYPE
	%reg_72 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_70, i32 %reg_71
	%reg_73 = load i32, ptr %reg_72
	%reg_74 = load i32, ptr @DATA_TYPE_FUNC
	%reg_75 = icmp ne i32 %reg_73, %reg_74
	br i1 %reg_75, label %Block15, label %Block16
Block18:  ;While condition at line 566
	%reg_104 = load i32, ptr %reg_97
	%reg_105 = icmp ne i32 %reg_104, 0
	br i1 %reg_105, label %Block21, label %Block20
Block19:  ;While body at line 566
	%reg_109 = load i32, ptr %reg_97
	%reg_110 = load i32, ptr @DATA_VALUE
	%reg_111 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_109, i32 %reg_110
	%reg_112 = load i32, ptr %reg_111
	store i32 %reg_112, ptr %reg_108
	%reg_114 = load i32, ptr %reg_102
	%reg_115 = load i32, ptr %reg_3
	%reg_116 = call i32 @eval(i32 %reg_114, i32 %reg_115)
	store i32 %reg_116, ptr %reg_113
	%reg_117 = load i32, ptr %reg_93
	%reg_118 = load i32, ptr %reg_108
	%reg_119 = load i32, ptr %reg_113
	call void @env_add(i32 %reg_117, i32 %reg_118, i32 %reg_119)
	%reg_120 = load i32, ptr %reg_97
	%reg_121 = load i32, ptr @DATA_NEXT
	%reg_122 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_120, i32 %reg_121
	%reg_123 = load i32, ptr %reg_122
	store i32 %reg_123, ptr %reg_97
	%reg_124 = load i32, ptr %reg_102
	%reg_125 = load i32, ptr @DATA_NEXT
	%reg_126 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_124, i32 %reg_125
	%reg_127 = load i32, ptr %reg_126
	store i32 %reg_127, ptr %reg_102
	br label %Block18
Block20:  ;While end at line 566
	%reg_128 = load i32, ptr %reg_97
	%reg_129 = icmp ne i32 %reg_128, 0
	br i1 %reg_129, label %Block22, label %Block24
Block21:  ;And opertor at line 566
	%reg_106 = load i32, ptr %reg_102
	%reg_107 = icmp ne i32 %reg_106, 0
	br i1 %reg_107, label %Block19, label %Block20
Block22:  ;If then at line 573
	%reg_132 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_133 = call i32 @panic(i32 %reg_132)
	br label %Block23
Block23:  ;If end at line 573
	%reg_135 = load i32, ptr %reg_83
	%reg_136 = load i32, ptr %reg_93
	%reg_137 = call i32 @eval(i32 %reg_135, i32 %reg_136)
	store i32 %reg_137, ptr %reg_134
	%reg_138 = load i32, ptr %reg_93
	call void @free_data(i32 %reg_138)
	%reg_139 = load i32, ptr %reg_64
	call void @free_data(i32 %reg_139)
	%reg_140 = load i32, ptr %reg_134
	ret i32 %reg_140
	br label %Block13
Block24:  ;Or opertor at line 573
	%reg_130 = load i32, ptr %reg_102
	%reg_131 = icmp ne i32 %reg_130, 0
	br i1 %reg_131, label %Block22, label %Block23
Block25:  ;If then at line 584
	%reg_149 = load i32, ptr %reg_46
	%reg_150 = icmp eq i32 %reg_149, 0
	br i1 %reg_150, label %Block27, label %Block29
Block26:  ;If end at line 584
	%reg_160 = load i32, ptr %reg_141
	%reg_161 = load i32, ptr @SYM_ATOM
	%reg_162 = icmp eq i32 %reg_160, %reg_161
	br i1 %reg_162, label %Block30, label %Block31
Block27:  ;If then at line 585
	%reg_156 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_157 = call i32 @panic(i32 %reg_156)
	br label %Block28
Block28:  ;If end at line 585
	%reg_158 = load i32, ptr %reg_46
	%reg_159 = call i32 @copy_ptr(i32 %reg_158)
	ret i32 %reg_159
	br label %Block26
Block29:  ;Or opertor at line 585
	%reg_151 = load i32, ptr %reg_46
	%reg_152 = load i32, ptr @DATA_NEXT
	%reg_153 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_151, i32 %reg_152
	%reg_154 = load i32, ptr %reg_153
	%reg_155 = icmp ne i32 %reg_154, 0
	br i1 %reg_155, label %Block27, label %Block28
Block30:  ;If then at line 589
	%reg_163 = load i32, ptr %reg_46
	%reg_164 = icmp eq i32 %reg_163, 0
	br i1 %reg_164, label %Block32, label %Block34
Block31:  ;If end at line 589
	%reg_186 = load i32, ptr %reg_141
	%reg_187 = load i32, ptr @SYM_NUMBER
	%reg_188 = icmp eq i32 %reg_186, %reg_187
	br i1 %reg_188, label %Block35, label %Block36
Block32:  ;If then at line 590
	%reg_170 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_171 = call i32 @panic(i32 %reg_170)
	br label %Block33
Block33:  ;If end at line 590
	%reg_173 = load i32, ptr %reg_46
	%reg_174 = load i32, ptr %reg_3
	%reg_175 = call i32 @eval(i32 %reg_173, i32 %reg_174)
	store i32 %reg_175, ptr %reg_172
	%reg_177 = load i32, ptr %reg_172
	%reg_178 = load i32, ptr @DATA_TYPE
	%reg_179 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_177, i32 %reg_178
	%reg_180 = load i32, ptr %reg_179
	%reg_181 = load i32, ptr @DATA_TYPE_LIST
	%reg_182 = call i32 @ne(i32 %reg_180, i32 %reg_181)
	%reg_183 = call i32 @make_bool(i32 %reg_182)
	store i32 %reg_183, ptr %reg_176
	%reg_184 = load i32, ptr %reg_172
	call void @free_data(i32 %reg_184)
	%reg_185 = load i32, ptr %reg_176
	ret i32 %reg_185
	br label %Block31
Block34:  ;Or opertor at line 590
	%reg_165 = load i32, ptr %reg_46
	%reg_166 = load i32, ptr @DATA_NEXT
	%reg_167 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_165, i32 %reg_166
	%reg_168 = load i32, ptr %reg_167
	%reg_169 = icmp ne i32 %reg_168, 0
	br i1 %reg_169, label %Block32, label %Block33
Block35:  ;If then at line 597
	%reg_189 = load i32, ptr %reg_46
	%reg_190 = icmp eq i32 %reg_189, 0
	br i1 %reg_190, label %Block37, label %Block39
Block36:  ;If end at line 597
	%reg_212 = load i32, ptr %reg_141
	%reg_213 = load i32, ptr @SYM_EQ
	%reg_214 = icmp eq i32 %reg_212, %reg_213
	br i1 %reg_214, label %Block40, label %Block41
Block37:  ;If then at line 598
	%reg_196 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_197 = call i32 @panic(i32 %reg_196)
	br label %Block38
Block38:  ;If end at line 598
	%reg_199 = load i32, ptr %reg_46
	%reg_200 = load i32, ptr %reg_3
	%reg_201 = call i32 @eval(i32 %reg_199, i32 %reg_200)
	store i32 %reg_201, ptr %reg_198
	%reg_203 = load i32, ptr %reg_198
	%reg_204 = load i32, ptr @DATA_TYPE
	%reg_205 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_203, i32 %reg_204
	%reg_206 = load i32, ptr %reg_205
	%reg_207 = load i32, ptr @DATA_TYPE_NUMBER
	%reg_208 = call i32 @eq(i32 %reg_206, i32 %reg_207)
	%reg_209 = call i32 @make_bool(i32 %reg_208)
	store i32 %reg_209, ptr %reg_202
	%reg_210 = load i32, ptr %reg_198
	call void @free_data(i32 %reg_210)
	%reg_211 = load i32, ptr %reg_202
	ret i32 %reg_211
	br label %Block36
Block39:  ;Or opertor at line 598
	%reg_191 = load i32, ptr %reg_46
	%reg_192 = load i32, ptr @DATA_NEXT
	%reg_193 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_191, i32 %reg_192
	%reg_194 = load i32, ptr %reg_193
	%reg_195 = icmp ne i32 %reg_194, 0
	br i1 %reg_195, label %Block37, label %Block38
Block40:  ;If then at line 606
	%reg_216 = load i32, ptr %reg_46
	%reg_217 = load i32, ptr @DATA_NEXT
	%reg_218 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_216, i32 %reg_217
	%reg_219 = load i32, ptr %reg_218
	store i32 %reg_219, ptr %reg_215
	%reg_220 = load i32, ptr %reg_46
	%reg_221 = icmp eq i32 %reg_220, 0
	br i1 %reg_221, label %Block42, label %Block45
Block41:  ;If end at line 606
	%reg_247 = load i32, ptr %reg_141
	%reg_248 = load i32, ptr @SYM_CAR
	%reg_249 = icmp eq i32 %reg_247, %reg_248
	br i1 %reg_249, label %Block46, label %Block47
Block42:  ;If then at line 608
	%reg_229 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_230 = call i32 @panic(i32 %reg_229)
	br label %Block43
Block43:  ;If end at line 608
	%reg_232 = load i32, ptr %reg_46
	%reg_233 = load i32, ptr %reg_3
	%reg_234 = call i32 @eval(i32 %reg_232, i32 %reg_233)
	store i32 %reg_234, ptr %reg_231
	%reg_236 = load i32, ptr %reg_215
	%reg_237 = load i32, ptr %reg_3
	%reg_238 = call i32 @eval(i32 %reg_236, i32 %reg_237)
	store i32 %reg_238, ptr %reg_235
	%reg_240 = load i32, ptr %reg_231
	%reg_241 = load i32, ptr %reg_235
	%reg_242 = call i32 @is_equal(i32 %reg_240, i32 %reg_241)
	%reg_243 = call i32 @make_bool(i32 %reg_242)
	store i32 %reg_243, ptr %reg_239
	%reg_244 = load i32, ptr %reg_231
	call void @free_data(i32 %reg_244)
	%reg_245 = load i32, ptr %reg_235
	call void @free_data(i32 %reg_245)
	%reg_246 = load i32, ptr %reg_239
	ret i32 %reg_246
	br label %Block41
Block44:  ;Or opertor at line 608
	%reg_224 = load i32, ptr %reg_215
	%reg_225 = load i32, ptr @DATA_NEXT
	%reg_226 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_224, i32 %reg_225
	%reg_227 = load i32, ptr %reg_226
	%reg_228 = icmp ne i32 %reg_227, 0
	br i1 %reg_228, label %Block42, label %Block43
Block45:  ;Or opertor at line -1
	%reg_222 = load i32, ptr %reg_215
	%reg_223 = icmp eq i32 %reg_222, 0
	br i1 %reg_223, label %Block42, label %Block44
Block46:  ;If then at line 619
	%reg_250 = load i32, ptr %reg_46
	%reg_251 = icmp eq i32 %reg_250, 0
	br i1 %reg_251, label %Block48, label %Block50
Block47:  ;If end at line 619
	%reg_273 = load i32, ptr %reg_141
	%reg_274 = load i32, ptr @SYM_CDR
	%reg_275 = icmp eq i32 %reg_273, %reg_274
	br i1 %reg_275, label %Block53, label %Block54
Block48:  ;If then at line 620
	%reg_257 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_258 = call i32 @panic(i32 %reg_257)
	br label %Block49
Block49:  ;If end at line 620
	%reg_260 = load i32, ptr %reg_46
	%reg_261 = load i32, ptr %reg_3
	%reg_262 = call i32 @eval(i32 %reg_260, i32 %reg_261)
	store i32 %reg_262, ptr %reg_259
	%reg_264 = load i32, ptr %reg_259
	%reg_265 = call i32 @unwrap_list(i32 %reg_264)
	%reg_266 = call i32 @copy_ptr(i32 %reg_265)
	store i32 %reg_266, ptr %reg_263
	%reg_267 = load i32, ptr %reg_263
	%reg_268 = icmp eq i32 %reg_267, 0
	br i1 %reg_268, label %Block51, label %Block52
Block50:  ;Or opertor at line 620
	%reg_252 = load i32, ptr %reg_46
	%reg_253 = load i32, ptr @DATA_NEXT
	%reg_254 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_252, i32 %reg_253
	%reg_255 = load i32, ptr %reg_254
	%reg_256 = icmp ne i32 %reg_255, 0
	br i1 %reg_256, label %Block48, label %Block49
Block51:  ;If then at line 623
	%reg_269 = load i32, ptr @ERR_INVALID_LIST
	%reg_270 = call i32 @panic(i32 %reg_269)
	br label %Block52
Block52:  ;If end at line 623
	%reg_271 = load i32, ptr %reg_259
	call void @free_data(i32 %reg_271)
	%reg_272 = load i32, ptr %reg_263
	ret i32 %reg_272
	br label %Block47
Block53:  ;If then at line 628
	%reg_276 = load i32, ptr %reg_46
	%reg_277 = icmp eq i32 %reg_276, 0
	br i1 %reg_277, label %Block55, label %Block57
Block54:  ;If end at line 628
	%reg_305 = load i32, ptr %reg_141
	%reg_306 = load i32, ptr @SYM_CONS
	%reg_307 = icmp eq i32 %reg_305, %reg_306
	br i1 %reg_307, label %Block60, label %Block61
Block55:  ;If then at line 629
	%reg_283 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_284 = call i32 @panic(i32 %reg_283)
	br label %Block56
Block56:  ;If end at line 629
	%reg_286 = load i32, ptr %reg_46
	%reg_287 = load i32, ptr %reg_3
	%reg_288 = call i32 @eval(i32 %reg_286, i32 %reg_287)
	store i32 %reg_288, ptr %reg_285
	%reg_290 = load i32, ptr %reg_285
	%reg_291 = call i32 @unwrap_list(i32 %reg_290)
	store i32 %reg_291, ptr %reg_289
	%reg_292 = load i32, ptr %reg_289
	%reg_293 = icmp eq i32 %reg_292, 0
	br i1 %reg_293, label %Block58, label %Block59
Block57:  ;Or opertor at line 629
	%reg_278 = load i32, ptr %reg_46
	%reg_279 = load i32, ptr @DATA_NEXT
	%reg_280 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_278, i32 %reg_279
	%reg_281 = load i32, ptr %reg_280
	%reg_282 = icmp ne i32 %reg_281, 0
	br i1 %reg_282, label %Block55, label %Block56
Block58:  ;If then at line 632
	%reg_294 = load i32, ptr @ERR_INVALID_LIST
	%reg_295 = call i32 @panic(i32 %reg_294)
	br label %Block59
Block59:  ;If end at line 632
	%reg_297 = load i32, ptr %reg_289
	%reg_298 = load i32, ptr @DATA_NEXT
	%reg_299 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_297, i32 %reg_298
	%reg_300 = load i32, ptr %reg_299
	%reg_301 = call i32 @copy_ptr(i32 %reg_300)
	%reg_302 = call i32 @make_list(i32 %reg_301)
	store i32 %reg_302, ptr %reg_296
	%reg_303 = load i32, ptr %reg_285
	call void @free_data(i32 %reg_303)
	%reg_304 = load i32, ptr %reg_296
	ret i32 %reg_304
	br label %Block54
Block60:  ;If then at line 638
	%reg_309 = load i32, ptr %reg_46
	%reg_310 = load i32, ptr @DATA_NEXT
	%reg_311 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_309, i32 %reg_310
	%reg_312 = load i32, ptr %reg_311
	store i32 %reg_312, ptr %reg_308
	%reg_313 = load i32, ptr %reg_46
	%reg_314 = icmp eq i32 %reg_313, 0
	br i1 %reg_314, label %Block62, label %Block65
Block61:  ;If end at line 638
	%reg_347 = load i32, ptr %reg_141
	%reg_348 = load i32, ptr @SYM_COND
	%reg_349 = icmp eq i32 %reg_347, %reg_348
	br i1 %reg_349, label %Block66, label %Block67
Block62:  ;If then at line 640
	%reg_322 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_323 = call i32 @panic(i32 %reg_322)
	br label %Block63
Block63:  ;If end at line 640
	%reg_325 = load i32, ptr %reg_46
	%reg_326 = load i32, ptr %reg_3
	%reg_327 = call i32 @eval(i32 %reg_325, i32 %reg_326)
	store i32 %reg_327, ptr %reg_324
	%reg_329 = load i32, ptr %reg_308
	%reg_330 = load i32, ptr %reg_3
	%reg_331 = call i32 @eval(i32 %reg_329, i32 %reg_330)
	store i32 %reg_331, ptr %reg_328
	%reg_333 = load i32, ptr %reg_324
	%reg_334 = call i32 @copy_data(i32 %reg_333)
	store i32 %reg_334, ptr %reg_332
	%reg_335 = load i32, ptr %reg_332
	%reg_336 = load i32, ptr @DATA_NEXT
	%reg_337 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_335, i32 %reg_336
	%reg_338 = load i32, ptr %reg_328
	%reg_339 = call i32 @unwrap_list(i32 %reg_338)
	%reg_340 = call i32 @copy_ptr(i32 %reg_339)
	store i32 %reg_340, ptr %reg_337
	%reg_342 = load i32, ptr %reg_332
	%reg_343 = call i32 @make_list(i32 %reg_342)
	store i32 %reg_343, ptr %reg_341
	%reg_344 = load i32, ptr %reg_324
	call void @free_data(i32 %reg_344)
	%reg_345 = load i32, ptr %reg_328
	call void @free_data(i32 %reg_345)
	%reg_346 = load i32, ptr %reg_341
	ret i32 %reg_346
	br label %Block61
Block64:  ;Or opertor at line 640
	%reg_317 = load i32, ptr %reg_308
	%reg_318 = load i32, ptr @DATA_NEXT
	%reg_319 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_317, i32 %reg_318
	%reg_320 = load i32, ptr %reg_319
	%reg_321 = icmp ne i32 %reg_320, 0
	br i1 %reg_321, label %Block62, label %Block63
Block65:  ;Or opertor at line -1
	%reg_315 = load i32, ptr %reg_308
	%reg_316 = icmp eq i32 %reg_315, 0
	br i1 %reg_316, label %Block62, label %Block64
Block66:  ;If then at line 653
	%reg_351 = load i32, ptr %reg_46
	store i32 %reg_351, ptr %reg_350
	br label %Block68
Block67:  ;If end at line 653
	%reg_381 = load i32, ptr %reg_141
	%reg_382 = load i32, ptr @SYM_LAMBDA
	%reg_383 = icmp eq i32 %reg_381, %reg_382
	br i1 %reg_383, label %Block73, label %Block74
Block68:  ;While condition at line 655
	%reg_352 = load i32, ptr %reg_350
	%reg_353 = icmp ne i32 %reg_352, 0
	br i1 %reg_353, label %Block69, label %Block70
Block69:  ;While body at line 655
	%reg_355 = load i32, ptr %reg_350
	%reg_356 = call i32 @unwrap_list(i32 %reg_355)
	store i32 %reg_356, ptr %reg_354
	%reg_358 = load i32, ptr %reg_354
	%reg_359 = load i32, ptr %reg_3
	%reg_360 = call i32 @eval(i32 %reg_358, i32 %reg_359)
	store i32 %reg_360, ptr %reg_357
	%reg_361 = load i32, ptr %reg_357
	%reg_362 = call i32 @is_true(i32 %reg_361)
	%reg_363 = icmp ne i32 %reg_362, 0
	br i1 %reg_363, label %Block71, label %Block72
Block70:  ;While end at line 655
	%reg_380 = add i32 0, 0
	ret i32 %reg_380
	br label %Block67
Block71:  ;If then at line 658
	%reg_365 = load i32, ptr %reg_354
	%reg_366 = load i32, ptr @DATA_NEXT
	%reg_367 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_365, i32 %reg_366
	%reg_368 = load i32, ptr %reg_367
	store i32 %reg_368, ptr %reg_364
	%reg_370 = load i32, ptr %reg_364
	%reg_371 = load i32, ptr %reg_3
	%reg_372 = call i32 @eval(i32 %reg_370, i32 %reg_371)
	store i32 %reg_372, ptr %reg_369
	%reg_373 = load i32, ptr %reg_357
	call void @free_data(i32 %reg_373)
	%reg_374 = load i32, ptr %reg_369
	ret i32 %reg_374
	br label %Block72
Block72:  ;If end at line 658
	%reg_375 = load i32, ptr %reg_357
	call void @free_data(i32 %reg_375)
	%reg_376 = load i32, ptr %reg_350
	%reg_377 = load i32, ptr @DATA_NEXT
	%reg_378 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_376, i32 %reg_377
	%reg_379 = load i32, ptr %reg_378
	store i32 %reg_379, ptr %reg_350
	br label %Block68
Block73:  ;If then at line 670
	%reg_385 = load i32, ptr %reg_46
	%reg_386 = load i32, ptr @DATA_NEXT
	%reg_387 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_385, i32 %reg_386
	%reg_388 = load i32, ptr %reg_387
	store i32 %reg_388, ptr %reg_384
	%reg_389 = load i32, ptr %reg_46
	%reg_390 = icmp eq i32 %reg_389, 0
	br i1 %reg_390, label %Block75, label %Block78
Block74:  ;If end at line 670
	%reg_409 = load i32, ptr %reg_141
	%reg_410 = load i32, ptr @SYM_DEFINE
	%reg_411 = icmp eq i32 %reg_409, %reg_410
	br i1 %reg_411, label %Block79, label %Block80
Block75:  ;If then at line 672
	%reg_398 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_399 = call i32 @panic(i32 %reg_398)
	br label %Block76
Block76:  ;If end at line 672
	%reg_401 = load i32, ptr %reg_46
	%reg_402 = call i32 @copy_data(i32 %reg_401)
	%reg_403 = load i32, ptr %reg_384
	%reg_404 = call i32 @copy_data(i32 %reg_403)
	%reg_405 = load i32, ptr %reg_3
	%reg_406 = call i32 @copy_ptr(i32 %reg_405)
	%reg_407 = call i32 @make_func(i32 %reg_402, i32 %reg_404, i32 %reg_406)
	store i32 %reg_407, ptr %reg_400
	%reg_408 = load i32, ptr %reg_400
	ret i32 %reg_408
	br label %Block74
Block77:  ;Or opertor at line 672
	%reg_393 = load i32, ptr %reg_384
	%reg_394 = load i32, ptr @DATA_NEXT
	%reg_395 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_393, i32 %reg_394
	%reg_396 = load i32, ptr %reg_395
	%reg_397 = icmp ne i32 %reg_396, 0
	br i1 %reg_397, label %Block75, label %Block76
Block78:  ;Or opertor at line -1
	%reg_391 = load i32, ptr %reg_384
	%reg_392 = icmp eq i32 %reg_391, 0
	br i1 %reg_392, label %Block75, label %Block77
Block79:  ;If then at line 680
	%reg_413 = load i32, ptr %reg_46
	%reg_414 = load i32, ptr @DATA_NEXT
	%reg_415 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_413, i32 %reg_414
	%reg_416 = load i32, ptr %reg_415
	store i32 %reg_416, ptr %reg_412
	%reg_417 = load i32, ptr %reg_46
	%reg_418 = icmp eq i32 %reg_417, 0
	br i1 %reg_418, label %Block81, label %Block84
Block80:  ;If end at line 680
	%reg_439 = load i32, ptr %reg_141
	%reg_440 = load i32, ptr @SYM_LIST
	%reg_441 = icmp eq i32 %reg_439, %reg_440
	br i1 %reg_441, label %Block85, label %Block86
Block81:  ;If then at line 682
	%reg_426 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_427 = call i32 @panic(i32 %reg_426)
	br label %Block82
Block82:  ;If end at line 682
	%reg_429 = load i32, ptr %reg_46
	%reg_430 = call i32 @unwrap_symbol(i32 %reg_429)
	store i32 %reg_430, ptr %reg_428
	%reg_432 = load i32, ptr %reg_412
	%reg_433 = load i32, ptr %reg_3
	%reg_434 = call i32 @eval(i32 %reg_432, i32 %reg_433)
	store i32 %reg_434, ptr %reg_431
	%reg_435 = load i32, ptr %reg_3
	%reg_436 = load i32, ptr %reg_428
	%reg_437 = load i32, ptr %reg_431
	call void @env_set(i32 %reg_435, i32 %reg_436, i32 %reg_437)
	%reg_438 = add i32 0, 0
	ret i32 %reg_438
	br label %Block80
Block83:  ;Or opertor at line 682
	%reg_421 = load i32, ptr %reg_412
	%reg_422 = load i32, ptr @DATA_NEXT
	%reg_423 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_421, i32 %reg_422
	%reg_424 = load i32, ptr %reg_423
	%reg_425 = icmp ne i32 %reg_424, 0
	br i1 %reg_425, label %Block81, label %Block82
Block84:  ;Or opertor at line -1
	%reg_419 = load i32, ptr %reg_412
	%reg_420 = icmp eq i32 %reg_419, 0
	br i1 %reg_420, label %Block81, label %Block83
Block85:  ;If then at line 691
	%reg_443 = add i32 0, 0
	%reg_444 = call i32 @make_list(i32 %reg_443)
	store i32 %reg_444, ptr %reg_442
	%reg_446 = add i32 0, 0
	store i32 %reg_446, ptr %reg_445
	br label %Block87
Block86:  ;If end at line 691
	%reg_474 = load i32, ptr %reg_46
	%reg_475 = load i32, ptr @DATA_NEXT
	%reg_476 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_474, i32 %reg_475
	%reg_477 = load i32, ptr %reg_476
	store i32 %reg_477, ptr %reg_473
	%reg_478 = load i32, ptr %reg_46
	%reg_479 = icmp eq i32 %reg_478, 0
	br i1 %reg_479, label %Block93, label %Block96
Block87:  ;While condition at line 693
	%reg_447 = load i32, ptr %reg_46
	%reg_448 = icmp ne i32 %reg_447, 0
	br i1 %reg_448, label %Block88, label %Block89
Block88:  ;While body at line 693
	%reg_450 = load i32, ptr %reg_46
	%reg_451 = load i32, ptr %reg_3
	%reg_452 = call i32 @eval(i32 %reg_450, i32 %reg_451)
	store i32 %reg_452, ptr %reg_449
	%reg_454 = load i32, ptr %reg_449
	%reg_455 = call i32 @copy_data(i32 %reg_454)
	store i32 %reg_455, ptr %reg_453
	%reg_456 = load i32, ptr %reg_449
	call void @free_data(i32 %reg_456)
	%reg_457 = load i32, ptr %reg_445
	%reg_458 = icmp ne i32 %reg_457, 0
	br i1 %reg_458, label %Block90, label %Block92
Block89:  ;While end at line 693
	%reg_472 = load i32, ptr %reg_442
	ret i32 %reg_472
	br label %Block86
Block90:  ;If then at line 697
	%reg_459 = load i32, ptr %reg_445
	%reg_460 = load i32, ptr @DATA_NEXT
	%reg_461 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_459, i32 %reg_460
	%reg_462 = load i32, ptr %reg_453
	store i32 %reg_462, ptr %reg_461
	br label %Block91
Block91:  ;If end at line 697
	%reg_467 = load i32, ptr %reg_453
	store i32 %reg_467, ptr %reg_445
	%reg_468 = load i32, ptr %reg_46
	%reg_469 = load i32, ptr @DATA_NEXT
	%reg_470 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_468, i32 %reg_469
	%reg_471 = load i32, ptr %reg_470
	store i32 %reg_471, ptr %reg_46
	br label %Block87
Block92:  ;If else at line 697
	%reg_463 = load i32, ptr %reg_442
	%reg_464 = load i32, ptr @DATA_VALUE
	%reg_465 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_463, i32 %reg_464
	%reg_466 = load i32, ptr %reg_453
	store i32 %reg_466, ptr %reg_465
	br label %Block91
Block93:  ;If then at line 709
	%reg_487 = load i32, ptr @ERR_INVALID_ARG_NUM
	%reg_488 = call i32 @panic(i32 %reg_487)
	br label %Block94
Block94:  ;If end at line 709
	%reg_490 = load i32, ptr %reg_46
	%reg_491 = load i32, ptr %reg_3
	%reg_492 = call i32 @eval(i32 %reg_490, i32 %reg_491)
	store i32 %reg_492, ptr %reg_489
	%reg_494 = load i32, ptr %reg_473
	%reg_495 = load i32, ptr %reg_3
	%reg_496 = call i32 @eval(i32 %reg_494, i32 %reg_495)
	store i32 %reg_496, ptr %reg_493
	%reg_498 = load i32, ptr %reg_489
	%reg_499 = call i32 @unwrap_number(i32 %reg_498)
	store i32 %reg_499, ptr %reg_497
	%reg_501 = load i32, ptr %reg_493
	%reg_502 = call i32 @unwrap_number(i32 %reg_501)
	store i32 %reg_502, ptr %reg_500
	%reg_504 = add i32 0, 0
	store i32 %reg_504, ptr %reg_503
	%reg_505 = load i32, ptr %reg_141
	%reg_506 = load i32, ptr @SYM_ADD
	%reg_507 = icmp eq i32 %reg_505, %reg_506
	br i1 %reg_507, label %Block97, label %Block99
Block95:  ;Or opertor at line 709
	%reg_482 = load i32, ptr %reg_473
	%reg_483 = load i32, ptr @DATA_NEXT
	%reg_484 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_482, i32 %reg_483
	%reg_485 = load i32, ptr %reg_484
	%reg_486 = icmp ne i32 %reg_485, 0
	br i1 %reg_486, label %Block93, label %Block94
Block96:  ;Or opertor at line -1
	%reg_480 = load i32, ptr %reg_473
	%reg_481 = icmp eq i32 %reg_480, 0
	br i1 %reg_481, label %Block93, label %Block95
Block97:  ;If then at line 716
	%reg_508 = load i32, ptr %reg_497
	%reg_509 = load i32, ptr %reg_500
	%reg_510 = add i32 %reg_508, %reg_509
	%reg_511 = call i32 @make_number(i32 %reg_510)
	store i32 %reg_511, ptr %reg_503
	br label %Block98
Block98:  ;If end at line 716
	%reg_570 = load i32, ptr %reg_489
	call void @free_data(i32 %reg_570)
	%reg_571 = load i32, ptr %reg_493
	call void @free_data(i32 %reg_571)
	%reg_572 = load i32, ptr %reg_503
	ret i32 %reg_572
	br label %Block9
Block99:  ;If else at line 716
	%reg_512 = load i32, ptr %reg_141
	%reg_513 = load i32, ptr @SYM_SUB
	%reg_514 = icmp eq i32 %reg_512, %reg_513
	br i1 %reg_514, label %Block100, label %Block102
Block100:  ;If then at line 718
	%reg_515 = load i32, ptr %reg_497
	%reg_516 = load i32, ptr %reg_500
	%reg_517 = sub i32 %reg_515, %reg_516
	%reg_518 = call i32 @make_number(i32 %reg_517)
	store i32 %reg_518, ptr %reg_503
	br label %Block101
Block101:  ;If end at line 718
	br label %Block98
Block102:  ;If else at line 718
	%reg_519 = load i32, ptr %reg_141
	%reg_520 = load i32, ptr @SYM_MUL
	%reg_521 = icmp eq i32 %reg_519, %reg_520
	br i1 %reg_521, label %Block103, label %Block105
Block103:  ;If then at line 720
	%reg_522 = load i32, ptr %reg_497
	%reg_523 = load i32, ptr %reg_500
	%reg_524 = mul i32 %reg_522, %reg_523
	%reg_525 = call i32 @make_number(i32 %reg_524)
	store i32 %reg_525, ptr %reg_503
	br label %Block104
Block104:  ;If end at line 720
	br label %Block101
Block105:  ;If else at line 720
	%reg_526 = load i32, ptr %reg_141
	%reg_527 = load i32, ptr @SYM_DIV
	%reg_528 = icmp eq i32 %reg_526, %reg_527
	br i1 %reg_528, label %Block106, label %Block108
Block106:  ;If then at line 722
	%reg_529 = load i32, ptr %reg_497
	%reg_530 = load i32, ptr %reg_500
	%reg_531 = sdiv i32 %reg_529, %reg_530
	%reg_532 = call i32 @make_number(i32 %reg_531)
	store i32 %reg_532, ptr %reg_503
	br label %Block107
Block107:  ;If end at line 722
	br label %Block104
Block108:  ;If else at line 722
	%reg_533 = load i32, ptr %reg_141
	%reg_534 = load i32, ptr @SYM_LT
	%reg_535 = icmp eq i32 %reg_533, %reg_534
	br i1 %reg_535, label %Block109, label %Block111
Block109:  ;If then at line 724
	%reg_536 = load i32, ptr %reg_497
	%reg_537 = load i32, ptr %reg_500
	%reg_538 = call i32 @lt(i32 %reg_536, i32 %reg_537)
	%reg_539 = call i32 @make_bool(i32 %reg_538)
	store i32 %reg_539, ptr %reg_503
	br label %Block110
Block110:  ;If end at line 724
	br label %Block107
Block111:  ;If else at line 724
	%reg_540 = load i32, ptr %reg_141
	%reg_541 = load i32, ptr @SYM_LE
	%reg_542 = icmp eq i32 %reg_540, %reg_541
	br i1 %reg_542, label %Block112, label %Block114
Block112:  ;If then at line 726
	%reg_543 = load i32, ptr %reg_497
	%reg_544 = load i32, ptr %reg_500
	%reg_545 = call i32 @le(i32 %reg_543, i32 %reg_544)
	%reg_546 = call i32 @make_bool(i32 %reg_545)
	store i32 %reg_546, ptr %reg_503
	br label %Block113
Block113:  ;If end at line 726
	br label %Block110
Block114:  ;If else at line 726
	%reg_547 = load i32, ptr %reg_141
	%reg_548 = load i32, ptr @SYM_GT
	%reg_549 = icmp eq i32 %reg_547, %reg_548
	br i1 %reg_549, label %Block115, label %Block117
Block115:  ;If then at line 728
	%reg_550 = load i32, ptr %reg_497
	%reg_551 = load i32, ptr %reg_500
	%reg_552 = call i32 @gt(i32 %reg_550, i32 %reg_551)
	%reg_553 = call i32 @make_bool(i32 %reg_552)
	store i32 %reg_553, ptr %reg_503
	br label %Block116
Block116:  ;If end at line 728
	br label %Block113
Block117:  ;If else at line 728
	%reg_554 = load i32, ptr %reg_141
	%reg_555 = load i32, ptr @SYM_GE
	%reg_556 = icmp eq i32 %reg_554, %reg_555
	br i1 %reg_556, label %Block118, label %Block120
Block118:  ;If then at line 730
	%reg_557 = load i32, ptr %reg_497
	%reg_558 = load i32, ptr %reg_500
	%reg_559 = call i32 @ge(i32 %reg_557, i32 %reg_558)
	%reg_560 = call i32 @make_bool(i32 %reg_559)
	store i32 %reg_560, ptr %reg_503
	br label %Block119
Block119:  ;If end at line 730
	br label %Block116
Block120:  ;If else at line 730
	%reg_561 = load i32, ptr %reg_141
	%reg_562 = load i32, ptr @SYM_EQ_NUM
	%reg_563 = icmp eq i32 %reg_561, %reg_562
	br i1 %reg_563, label %Block121, label %Block123
Block121:  ;If then at line 732
	%reg_564 = load i32, ptr %reg_497
	%reg_565 = load i32, ptr %reg_500
	%reg_566 = call i32 @eq(i32 %reg_564, i32 %reg_565)
	%reg_567 = call i32 @make_bool(i32 %reg_566)
	store i32 %reg_567, ptr %reg_503
	br label %Block122
Block122:  ;If end at line 732
	br label %Block119
Block123:  ;If else at line 732
	%reg_568 = load i32, ptr @ERR_INVALID_SYMBOL
	%reg_569 = call i32 @panic(i32 %reg_568)
	br label %Block122
}

define void @print(i32 %reg_0)
{
Block0:  ;Func define at line 749
	%reg_29 = alloca i32
	%reg_1 = alloca i32
	store i32 %reg_0, ptr %reg_1
	br label %Block1
Block1:  ;Func body at line 749
	%reg_2 = load i32, ptr %reg_1
	%reg_3 = load i32, ptr @DATA_TYPE
	%reg_4 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_2, i32 %reg_3
	%reg_5 = load i32, ptr %reg_4
	%reg_6 = load i32, ptr @DATA_TYPE_SYMBOL
	%reg_7 = icmp eq i32 %reg_5, %reg_6
	br i1 %reg_7, label %Block2, label %Block3
Block2:  ;If then at line 750
	%reg_8 = load i32, ptr %reg_1
	%reg_9 = load i32, ptr @DATA_VALUE
	%reg_10 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_8, i32 %reg_9
	%reg_11 = load i32, ptr %reg_10
	call void @print_sym(i32 %reg_11)
	ret void
	br label %Block3
Block3:  ;If end at line 750
	%reg_12 = load i32, ptr %reg_1
	%reg_13 = load i32, ptr @DATA_TYPE
	%reg_14 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_12, i32 %reg_13
	%reg_15 = load i32, ptr %reg_14
	%reg_16 = load i32, ptr @DATA_TYPE_NUMBER
	%reg_17 = icmp eq i32 %reg_15, %reg_16
	br i1 %reg_17, label %Block4, label %Block5
Block4:  ;If then at line 754
	%reg_18 = load i32, ptr %reg_1
	%reg_19 = load i32, ptr @DATA_VALUE
	%reg_20 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_18, i32 %reg_19
	%reg_21 = load i32, ptr %reg_20
	call void @putint(i32 %reg_21)
	ret void
	br label %Block5
Block5:  ;If end at line 754
	%reg_22 = load i32, ptr %reg_1
	%reg_23 = load i32, ptr @DATA_TYPE
	%reg_24 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_22, i32 %reg_23
	%reg_25 = load i32, ptr %reg_24
	%reg_26 = load i32, ptr @DATA_TYPE_LIST
	%reg_27 = icmp eq i32 %reg_25, %reg_26
	br i1 %reg_27, label %Block6, label %Block7
Block6:  ;If then at line 758
	%reg_28 = add i32 40, 0
	call void @putch(i32 %reg_28)
	%reg_30 = load i32, ptr %reg_1
	%reg_31 = load i32, ptr @DATA_VALUE
	%reg_32 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_30, i32 %reg_31
	%reg_33 = load i32, ptr %reg_32
	store i32 %reg_33, ptr %reg_29
	br label %Block8
Block7:  ;If end at line 758
	%reg_45 = load i32, ptr %reg_1
	%reg_46 = load i32, ptr @DATA_TYPE
	%reg_47 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_45, i32 %reg_46
	%reg_48 = load i32, ptr %reg_47
	%reg_49 = load i32, ptr @DATA_TYPE_FUNC
	%reg_50 = icmp eq i32 %reg_48, %reg_49
	br i1 %reg_50, label %Block13, label %Block14
Block8:  ;While condition at line 761
	%reg_34 = load i32, ptr %reg_29
	%reg_35 = icmp ne i32 %reg_34, 0
	br i1 %reg_35, label %Block9, label %Block10
Block9:  ;While body at line 761
	%reg_36 = load i32, ptr %reg_29
	call void @print(i32 %reg_36)
	%reg_37 = load i32, ptr %reg_29
	%reg_38 = load i32, ptr @DATA_NEXT
	%reg_39 = getelementptr [2048 x [4 x i32]], ptr @data, i32 0, i32 %reg_37, i32 %reg_38
	%reg_40 = load i32, ptr %reg_39
	store i32 %reg_40, ptr %reg_29
	%reg_41 = load i32, ptr %reg_29
	%reg_42 = icmp ne i32 %reg_41, 0
	br i1 %reg_42, label %Block11, label %Block12
Block10:  ;While end at line 761
	%reg_44 = add i32 41, 0
	call void @putch(i32 %reg_44)
	ret void
	br label %Block7
Block11:  ;If then at line 764
	%reg_43 = add i32 32, 0
	call void @putch(i32 %reg_43)
	br label %Block12
Block12:  ;If end at line 764
	br label %Block8
Block13:  ;If then at line 769
	%reg_51 = add i32 35, 0
	call void @putch(i32 %reg_51)
	%reg_52 = add i32 60, 0
	call void @putch(i32 %reg_52)
	%reg_53 = add i32 102, 0
	call void @putch(i32 %reg_53)
	%reg_54 = add i32 117, 0
	call void @putch(i32 %reg_54)
	%reg_55 = add i32 110, 0
	call void @putch(i32 %reg_55)
	%reg_56 = add i32 99, 0
	call void @putch(i32 %reg_56)
	%reg_57 = add i32 32, 0
	call void @putch(i32 %reg_57)
	%reg_58 = load i32, ptr %reg_1
	call void @putint(i32 %reg_58)
	%reg_59 = add i32 62, 0
	call void @putch(i32 %reg_59)
	ret void
	br label %Block14
Block14:  ;If end at line 769
	%reg_60 = load i32, ptr @ERR_INVALID_DATA_TYPE
	%reg_61 = call i32 @panic(i32 %reg_60)
	ret void
}

define i32 @main()
{
Block0:  ;Func define at line 788
	%reg_11 = alloca i32
	%reg_6 = alloca i32
	%reg_0 = alloca i32
	br label %Block1
Block1:  ;Func body at line 788
	call void @init_data()
	%reg_1 = add i32 0, 0
	%reg_2 = call i32 @make_env(i32 %reg_1)
	store i32 %reg_2, ptr %reg_0
	%reg_3 = call i32 @next_token()
	store i32 %reg_3, ptr @last_token_type
	br label %Block2
Block2:  ;While condition at line 796
	%reg_4 = add i32 1, 0
	%reg_5 = icmp ne i32 %reg_4, 0
	br i1 %reg_5, label %Block3, label %Block4
Block3:  ;While body at line 796
	%reg_7 = call i32 @parse()
	store i32 %reg_7, ptr %reg_6
	%reg_8 = load i32, ptr %reg_6
	%reg_9 = add i32 0, 0
	%reg_10 = icmp eq i32 %reg_8, %reg_9
	br i1 %reg_10, label %Block5, label %Block6
Block4:  ;While end at line 796
	%reg_21 = add i32 0, 0
	ret i32 %reg_21
Block5:  ;If then at line 799
	br label %Block4
Block6:  ;If end at line 799
	%reg_12 = load i32, ptr %reg_6
	%reg_13 = load i32, ptr %reg_0
	%reg_14 = call i32 @eval(i32 %reg_12, i32 %reg_13)
	store i32 %reg_14, ptr %reg_11
	%reg_15 = load i32, ptr %reg_11
	%reg_16 = icmp ne i32 %reg_15, 0
	br i1 %reg_16, label %Block8, label %Block9
Block7:  ;Break at line 799
	br label %Block6
Block8:  ;If then at line 803
	%reg_17 = load i32, ptr %reg_11
	call void @print(i32 %reg_17)
	%reg_18 = add i32 10, 0
	call void @putch(i32 %reg_18)
	br label %Block9
Block9:  ;If end at line 803
	%reg_19 = load i32, ptr %reg_6
	call void @free_data(i32 %reg_19)
	%reg_20 = load i32, ptr %reg_11
	call void @free_data(i32 %reg_20)
	br label %Block2
}
