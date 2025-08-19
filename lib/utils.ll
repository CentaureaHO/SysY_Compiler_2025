target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n64-S128"
target triple = "riscv64-unknown-linux-gnu"

%struct.__va_list_tag = type { i32, i32, ptr, ptr }
%struct.shared_data = type { i32, i32, [4088 x i8] }
%struct.thread_control_block = type { i32, i32, i32, ptr, ptr, i64, ptr }

; Thread library global variables
@scheduler_initialized = internal unnamed_addr global i1 false, align 4
@shared_mem = internal unnamed_addr global ptr null, align 8
@thread_list = internal unnamed_addr global ptr null, align 8
@next_thread_id = internal unnamed_addr global i32 1, align 4

declare void @llvm.memset.p0.i32(ptr, i8, i32, i1)

define void @lsccll.lib.memset_i8(ptr %ptr, i8 %value, i32 %count) {
entry:
  call void @llvm.memset.p0.i32(ptr %ptr, i8 %value, i32 %count, i1 0)
  ret void
}

define void @lsccll.lib.memset_i32(ptr %ptr, i32 %value, i32 %count) {
entry:
  %cmp = icmp eq i32 %count, 0
  br i1 %cmp, label %exit, label %loop.preheader

loop.preheader:
  br label %loop

loop:
  %i = phi i32 [ 0, %loop.preheader ], [ %i.next, %loop ]
  %current.ptr = getelementptr i32, ptr %ptr, i32 %i
  store i32 %value, ptr %current.ptr
  %i.next = add i32 %i, 1
  %loop.cmp = icmp eq i32 %i.next, %count
  br i1 %loop.cmp, label %exit, label %loop

exit:
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @lsccll.lib.parallel.loop(ptr noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3, i32 noundef %4, ...) local_unnamed_addr #0 {
  %6 = alloca [1 x %struct.__va_list_tag], align 16
  %7 = alloca [4 x i64], align 16
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %6) #6
  call void @llvm.lifetime.start.p0(i64 32, ptr nonnull %7) #6
  %8 = shl nsw i32 %3, 2
  %9 = shl nsw i32 %4, 3
  %10 = add nsw i32 %9, %8
  %11 = add nsw i32 %10, 16
  %12 = zext i32 %11 to i64
  %13 = tail call ptr @llvm.stacksave.p0()
  %14 = alloca i8, i64 %12, align 16
  %15 = alloca i8, i64 %12, align 16
  %16 = alloca i8, i64 %12, align 16
  %17 = alloca i8, i64 %12, align 16
  store i32 0, ptr %14, align 16, !tbaa !5
  %18 = getelementptr inbounds i32, ptr %14, i64 1
  store i32 %1, ptr %18, align 4, !tbaa !5
  %19 = getelementptr inbounds i32, ptr %14, i64 2
  store i32 %2, ptr %19, align 8, !tbaa !5
  %20 = getelementptr inbounds i32, ptr %14, i64 3
  store i32 1, ptr %20, align 4, !tbaa !5
  %21 = icmp eq i32 %10, 0
  br i1 %21, label %170, label %22

22:                                               ; preds = %5
  %23 = zext i32 %3 to i64
  %24 = tail call ptr @llvm.stacksave.p0()
  %25 = alloca i32, i64 %23, align 16
  call void @llvm.va_start(ptr nonnull %6)
  %26 = icmp sgt i32 %3, 0
  br i1 %26, label %27, label %54

27:                                               ; preds = %22
  %28 = load i32, ptr %6, align 16
  %29 = getelementptr inbounds %struct.__va_list_tag, ptr %6, i64 0, i32 2
  %30 = getelementptr inbounds %struct.__va_list_tag, ptr %6, i64 0, i32 3
  %31 = load ptr, ptr %30, align 16
  %32 = load ptr, ptr %29, align 8
  %33 = and i64 %23, 1
  %34 = icmp eq i32 %3, 1
  br i1 %34, label %37, label %35

35:                                               ; preds = %27
  %36 = and i64 %23, 2147483646
  br label %72

37:                                               ; preds = %98, %27
  %38 = phi i64 [ 0, %27 ], [ %104, %98 ]
  %39 = phi ptr [ %32, %27 ], [ %99, %98 ]
  %40 = phi i32 [ %28, %27 ], [ %100, %98 ]
  %41 = icmp eq i64 %33, 0
  br i1 %41, label %54, label %42

42:                                               ; preds = %37
  %43 = icmp ult i32 %40, 41
  br i1 %43, label %46, label %44

44:                                               ; preds = %42
  %45 = getelementptr i8, ptr %39, i64 8
  store ptr %45, ptr %29, align 8
  br label %50

46:                                               ; preds = %42
  %47 = zext nneg i32 %40 to i64
  %48 = getelementptr i8, ptr %31, i64 %47
  %49 = add nuw nsw i32 %40, 8
  store i32 %49, ptr %6, align 16
  br label %50

50:                                               ; preds = %46, %44
  %51 = phi ptr [ %48, %46 ], [ %39, %44 ]
  %52 = load i32, ptr %51, align 4
  %53 = getelementptr inbounds i32, ptr %25, i64 %38
  store i32 %52, ptr %53, align 4, !tbaa !5
  br label %54

54:                                               ; preds = %50, %37, %22
  %55 = getelementptr inbounds i8, ptr %14, i64 16
  %56 = shl nuw nsw i64 %23, 2
  call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 16 %55, ptr nonnull align 16 %25, i64 %56, i1 false)
  %57 = icmp sgt i32 %4, 0
  br i1 %57, label %58, label %128

58:                                               ; preds = %54
  %59 = load i32, ptr %6, align 16
  %60 = getelementptr inbounds %struct.__va_list_tag, ptr %6, i64 0, i32 2
  %61 = getelementptr inbounds %struct.__va_list_tag, ptr %6, i64 0, i32 3
  %62 = load ptr, ptr %61, align 16
  %63 = sext i32 %8 to i64
  %64 = getelementptr i8, ptr %14, i64 %63
  %65 = getelementptr i8, ptr %64, i64 16
  %66 = load ptr, ptr %60, align 8
  %67 = zext nneg i32 %4 to i64
  %68 = and i64 %67, 1
  %69 = icmp eq i32 %4, 1
  br i1 %69, label %110, label %70

70:                                               ; preds = %58
  %71 = and i64 %67, 2147483646
  br label %129

72:                                               ; preds = %98, %35
  %73 = phi i64 [ 0, %35 ], [ %104, %98 ]
  %74 = phi ptr [ %32, %35 ], [ %99, %98 ]
  %75 = phi i32 [ %28, %35 ], [ %100, %98 ]
  %76 = phi i64 [ 0, %35 ], [ %105, %98 ]
  %77 = icmp ult i32 %75, 41
  br i1 %77, label %78, label %82

78:                                               ; preds = %72
  %79 = zext nneg i32 %75 to i64
  %80 = getelementptr i8, ptr %31, i64 %79
  %81 = add nuw nsw i32 %75, 8
  store i32 %81, ptr %6, align 16
  br label %84

82:                                               ; preds = %72
  %83 = getelementptr i8, ptr %74, i64 8
  store ptr %83, ptr %29, align 8
  br label %84

84:                                               ; preds = %82, %78
  %85 = phi ptr [ %74, %78 ], [ %83, %82 ]
  %86 = phi i32 [ %81, %78 ], [ %75, %82 ]
  %87 = phi ptr [ %80, %78 ], [ %74, %82 ]
  %88 = load i32, ptr %87, align 4
  %89 = getelementptr inbounds i32, ptr %25, i64 %73
  store i32 %88, ptr %89, align 8, !tbaa !5
  %90 = or disjoint i64 %73, 1
  %91 = icmp ult i32 %86, 41
  br i1 %91, label %94, label %92

92:                                               ; preds = %84
  %93 = getelementptr i8, ptr %85, i64 8
  store ptr %93, ptr %29, align 8
  br label %98

94:                                               ; preds = %84
  %95 = zext nneg i32 %86 to i64
  %96 = getelementptr i8, ptr %31, i64 %95
  %97 = add nuw nsw i32 %86, 8
  store i32 %97, ptr %6, align 16
  br label %98

98:                                               ; preds = %94, %92
  %99 = phi ptr [ %85, %94 ], [ %93, %92 ]
  %100 = phi i32 [ %97, %94 ], [ %86, %92 ]
  %101 = phi ptr [ %96, %94 ], [ %85, %92 ]
  %102 = load i32, ptr %101, align 4
  %103 = getelementptr inbounds i32, ptr %25, i64 %90
  store i32 %102, ptr %103, align 4, !tbaa !5
  %104 = add nuw nsw i64 %73, 2
  %105 = add i64 %76, 2
  %106 = icmp eq i64 %105, %36
  br i1 %106, label %37, label %72, !llvm.loop !9

107:                                              ; preds = %157
  %108 = shl i64 %167, 3
  %109 = and i64 %108, 4294967280
  br label %110

110:                                              ; preds = %107, %58
  %111 = phi i64 [ 0, %58 ], [ %109, %107 ]
  %112 = phi ptr [ %66, %58 ], [ %158, %107 ]
  %113 = phi i32 [ %59, %58 ], [ %159, %107 ]
  %114 = icmp eq i64 %68, 0
  br i1 %114, label %128, label %115

115:                                              ; preds = %110
  %116 = icmp ult i32 %113, 41
  br i1 %116, label %119, label %117

117:                                              ; preds = %115
  %118 = getelementptr i8, ptr %112, i64 8
  store ptr %118, ptr %60, align 8
  br label %123

119:                                              ; preds = %115
  %120 = zext nneg i32 %113 to i64
  %121 = getelementptr i8, ptr %62, i64 %120
  %122 = add nuw nsw i32 %113, 8
  store i32 %122, ptr %6, align 16
  br label %123

123:                                              ; preds = %119, %117
  %124 = phi ptr [ %121, %119 ], [ %112, %117 ]
  %125 = load ptr, ptr %124, align 8
  %126 = ptrtoint ptr %125 to i64
  %127 = getelementptr i8, ptr %65, i64 %111
  store i64 %126, ptr %127, align 8, !tbaa !11
  br label %128

128:                                              ; preds = %123, %110, %54
  call void @llvm.va_end(ptr nonnull %6)
  call void @llvm.stackrestore.p0(ptr %24)
  br label %170

129:                                              ; preds = %157, %70
  %130 = phi i64 [ 0, %70 ], [ %167, %157 ]
  %131 = phi ptr [ %66, %70 ], [ %158, %157 ]
  %132 = phi i32 [ %59, %70 ], [ %159, %157 ]
  %133 = phi i64 [ 0, %70 ], [ %168, %157 ]
  %134 = icmp ult i32 %132, 41
  br i1 %134, label %135, label %139

135:                                              ; preds = %129
  %136 = zext nneg i32 %132 to i64
  %137 = getelementptr i8, ptr %62, i64 %136
  %138 = add nuw nsw i32 %132, 8
  store i32 %138, ptr %6, align 16
  br label %141

139:                                              ; preds = %129
  %140 = getelementptr i8, ptr %131, i64 8
  store ptr %140, ptr %60, align 8
  br label %141

141:                                              ; preds = %139, %135
  %142 = phi ptr [ %131, %135 ], [ %140, %139 ]
  %143 = phi i32 [ %138, %135 ], [ %132, %139 ]
  %144 = phi ptr [ %137, %135 ], [ %131, %139 ]
  %145 = load ptr, ptr %144, align 8
  %146 = ptrtoint ptr %145 to i64
  %147 = shl i64 %130, 3
  %148 = and i64 %147, 4294967280
  %149 = getelementptr i8, ptr %65, i64 %148
  store i64 %146, ptr %149, align 8, !tbaa !11
  %150 = icmp ult i32 %143, 41
  br i1 %150, label %153, label %151

151:                                              ; preds = %141
  %152 = getelementptr i8, ptr %142, i64 8
  store ptr %152, ptr %60, align 8
  br label %157

153:                                              ; preds = %141
  %154 = zext nneg i32 %143 to i64
  %155 = getelementptr i8, ptr %62, i64 %154
  %156 = add nuw nsw i32 %143, 8
  store i32 %156, ptr %6, align 16
  br label %157

157:                                              ; preds = %153, %151
  %158 = phi ptr [ %142, %153 ], [ %152, %151 ]
  %159 = phi i32 [ %156, %153 ], [ %143, %151 ]
  %160 = phi ptr [ %155, %153 ], [ %142, %151 ]
  %161 = load ptr, ptr %160, align 8
  %162 = ptrtoint ptr %161 to i64
  %163 = shl i64 %130, 3
  %164 = and i64 %163, 4294967280
  %165 = or disjoint i64 %164, 8
  %166 = getelementptr i8, ptr %65, i64 %165
  store i64 %162, ptr %166, align 8, !tbaa !11
  %167 = add nuw nsw i64 %130, 2
  %168 = add i64 %133, 2
  %169 = icmp eq i64 %168, %71
  br i1 %169, label %107, label %129, !llvm.loop !13

170:                                              ; preds = %128, %5
  ; Initialize thread library
  %171 = call i32 @lsccll.lib.parallel.thread_lib_init()
  %172 = icmp eq i32 %171, -1
  br i1 %172, label %190, label %173

173:                                              ; preds = %170
  %174 = sext i32 %11 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 16 %15, ptr nonnull align 16 %14, i64 %174, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 16 %16, ptr nonnull align 16 %14, i64 %174, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 16 %17, ptr nonnull align 16 %14, i64 %174, i1 false)
  store i32 1, ptr %15, align 16, !tbaa !5
  store i32 2, ptr %16, align 16, !tbaa !5
  store i32 3, ptr %17, align 16, !tbaa !5
  
  ; Create threads using custom thread library
  %175 = call i32 @lsccll.lib.parallel.thread_create(ptr noundef nonnull %7, ptr noundef null, ptr noundef %0, ptr noundef nonnull %14) #6
  %176 = getelementptr inbounds [4 x i64], ptr %7, i64 0, i64 1
  %177 = call i32 @lsccll.lib.parallel.thread_create(ptr noundef nonnull %176, ptr noundef null, ptr noundef %0, ptr noundef nonnull %15) #6
  %178 = getelementptr inbounds [4 x i64], ptr %7, i64 0, i64 2
  %179 = call i32 @lsccll.lib.parallel.thread_create(ptr noundef nonnull %178, ptr noundef null, ptr noundef %0, ptr noundef nonnull %16) #6
  %180 = getelementptr inbounds [4 x i64], ptr %7, i64 0, i64 3
  %181 = call i32 @lsccll.lib.parallel.thread_create(ptr noundef nonnull %180, ptr noundef null, ptr noundef %0, ptr noundef nonnull %17) #6
  
  ; Join threads using custom thread library
  %182 = load ptr, ptr %7, align 16, !tbaa !17
  %183 = call i32 @lsccll.lib.parallel.thread_join(ptr noundef %182, ptr noundef null) #6
  %184 = load ptr, ptr %176, align 8, !tbaa !17
  %185 = call i32 @lsccll.lib.parallel.thread_join(ptr noundef %184, ptr noundef null) #6
  %186 = load ptr, ptr %178, align 16, !tbaa !17
  %187 = call i32 @lsccll.lib.parallel.thread_join(ptr noundef %186, ptr noundef null) #6
  %188 = load ptr, ptr %180, align 8, !tbaa !17
  %189 = call i32 @lsccll.lib.parallel.thread_join(ptr noundef %188, ptr noundef null) #6
  
  ; Cleanup thread library
  call void @lsccll.lib.parallel.thread_lib_cleanup()
  br label %190

190:                                              ; preds = %173, %170
  call void @llvm.stackrestore.p0(ptr %13)
  call void @llvm.lifetime.end.p0(i64 32, ptr nonnull %7) #6
  call void @llvm.lifetime.end.p0(i64 24, ptr nonnull %6) #6
  ret void
}

; Thread library functions

; Function Attrs: nounwind uwtable
define dso_local noundef signext i32 @lsccll.lib.parallel.thread_lib_init() local_unnamed_addr #0 {
  %1 = load i1, ptr @scheduler_initialized, align 4
  br i1 %1, label %7, label %2

2:                                                ; preds = %0
  %3 = tail call ptr @mmap(ptr noundef null, i64 noundef 4096, i32 noundef 3, i32 noundef 33, i32 noundef -1, i64 noundef 0) #6
  store ptr %3, ptr @shared_mem, align 8, !tbaa !17
  %4 = icmp eq ptr %3, inttoptr (i64 -1 to ptr)
  br i1 %4, label %7, label %5

5:                                                ; preds = %2
  store volatile i32 0, ptr %3, align 4, !tbaa !18
  %6 = getelementptr inbounds %struct.shared_data, ptr %3, i64 0, i32 1
  store volatile i32 0, ptr %6, align 4, !tbaa !20
  store ptr null, ptr @thread_list, align 8, !tbaa !17
  store i32 1, ptr @next_thread_id, align 4, !tbaa !5
  store i1 true, ptr @scheduler_initialized, align 4
  br label %7

7:                                                ; preds = %2, %0, %5
  %8 = phi i32 [ 0, %5 ], [ 0, %0 ], [ -1, %2 ]
  ret i32 %8
}

; Function Attrs: nounwind uwtable
define dso_local void @lsccll.lib.parallel.thread_lib_cleanup() local_unnamed_addr #0 {
  %1 = alloca i32, align 4
  %2 = load ptr, ptr @thread_list, align 8, !tbaa !17
  %3 = icmp eq ptr %2, null
  br i1 %3, label %14, label %4

4:                                                ; preds = %0, %12
  %5 = phi ptr [ %7, %12 ], [ %2, %0 ]
  %6 = getelementptr inbounds %struct.thread_control_block, ptr %5, i64 0, i32 6
  %7 = load ptr, ptr %6, align 8, !tbaa !28
  %8 = load i32, ptr %5, align 8, !tbaa !21
  %9 = icmp sgt i32 %8, 0
  br i1 %9, label %10, label %12

10:                                               ; preds = %4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %1) #6
  %11 = call i32 @waitpid(i32 %8, ptr nonnull %1, i32 0) #6
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %1) #6
  br label %12

12:                                               ; preds = %10, %4
  call void @free(ptr nonnull %5) #6
  %13 = icmp eq ptr %7, null
  br i1 %13, label %14, label %4, !llvm.loop !29

14:                                               ; preds = %12, %0
  %15 = load ptr, ptr @shared_mem, align 8, !tbaa !17
  %16 = icmp eq ptr %15, null
  br i1 %16, label %19, label %17

17:                                               ; preds = %14
  %18 = call i32 @munmap(ptr nonnull %15, i64 4096) #6
  store ptr null, ptr @shared_mem, align 8, !tbaa !17
  br label %19

19:                                               ; preds = %17, %14
  store ptr null, ptr @thread_list, align 8, !tbaa !17
  store i1 false, ptr @scheduler_initialized, align 4
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local noundef signext i32 @lsccll.lib.parallel.thread_create(ptr nocapture noundef writeonly %0, ptr nocapture noundef readnone %1, ptr noundef %2, ptr noundef %3) local_unnamed_addr #0 {
  %5 = load i1, ptr @scheduler_initialized, align 4
  br i1 %5, label %6, label %8

6:                                                ; preds = %4
  %7 = load ptr, ptr @shared_mem, align 8, !tbaa !17
  br label %13

8:                                                ; preds = %4
  %9 = tail call ptr @mmap(ptr noundef null, i64 noundef 4096, i32 noundef 3, i32 noundef 33, i32 noundef -1, i64 noundef 0) #6
  store ptr %9, ptr @shared_mem, align 8, !tbaa !17
  %10 = icmp eq ptr %9, inttoptr (i64 -1 to ptr)
  br i1 %10, label %13, label %11

11:                                               ; preds = %8
  store volatile i32 0, ptr %9, align 4, !tbaa !18
  %12 = getelementptr inbounds %struct.shared_data, ptr %9, i64 0, i32 1
  store volatile i32 0, ptr %12, align 4, !tbaa !20
  store ptr null, ptr @thread_list, align 8, !tbaa !17
  store i32 1, ptr @next_thread_id, align 4, !tbaa !5
  store i1 true, ptr @scheduler_initialized, align 4
  br label %13

13:                                               ; preds = %6, %11, %8
  %14 = phi ptr [ %7, %6 ], [ %9, %11 ], [ inttoptr (i64 -1 to ptr), %8 ]
  %15 = load volatile i32, ptr %14, align 4, !tbaa !18
  %16 = icmp sgt i32 %15, 127
  br i1 %16, label %36, label %17

17:                                               ; preds = %13
  %18 = tail call noalias ptr @malloc(i64 48) #6
  %19 = icmp eq ptr %18, null
  br i1 %19, label %36, label %20

20:                                               ; preds = %17
  %21 = load i32, ptr @next_thread_id, align 4, !tbaa !5
  %22 = add nsw i32 %21, 1
  store i32 %22, ptr @next_thread_id, align 4, !tbaa !5
  %23 = getelementptr inbounds %struct.thread_control_block, ptr %18, i64 0, i32 2
  store i32 %21, ptr %23, align 8, !tbaa !23
  %24 = getelementptr inbounds %struct.thread_control_block, ptr %18, i64 0, i32 1
  store i32 0, ptr %24, align 4, !tbaa !24
  %25 = getelementptr inbounds %struct.thread_control_block, ptr %18, i64 0, i32 3
  store ptr %2, ptr %25, align 8, !tbaa !25
  %26 = getelementptr inbounds %struct.thread_control_block, ptr %18, i64 0, i32 4
  store ptr %3, ptr %26, align 8, !tbaa !26
  %27 = getelementptr inbounds %struct.thread_control_block, ptr %18, i64 0, i32 5
  store i64 0, ptr %27, align 8, !tbaa !27
  %28 = load ptr, ptr @thread_list, align 8, !tbaa !17
  %29 = getelementptr inbounds %struct.thread_control_block, ptr %18, i64 0, i32 6
  store ptr %28, ptr %29, align 8, !tbaa !28
  store ptr %18, ptr @thread_list, align 8, !tbaa !17
  %30 = tail call i32 @fork() #6
  switch i32 %30, label %33 [
    i32 -1, label %31
    i32 0, label %32
  ]

31:                                               ; preds = %20
  tail call void @free(ptr nonnull %18) #6
  br label %36

32:                                               ; preds = %20
  tail call fastcc void @lsccll.lib.parallel.thread_process_main(ptr noundef %2, ptr noundef %3)
  unreachable

33:                                               ; preds = %20
  store i32 %30, ptr %18, align 8, !tbaa !21
  store i32 1, ptr %24, align 4, !tbaa !24
  %34 = load ptr, ptr @shared_mem, align 8, !tbaa !17
  %35 = atomicrmw add ptr %34, i32 1 seq_cst, align 4
  store ptr %18, ptr %0, align 8, !tbaa !17
  br label %36

36:                                               ; preds = %17, %33, %31, %13
  %37 = phi i32 [ -1, %13 ], [ -1, %17 ], [ -1, %31 ], [ 0, %33 ]
  ret i32 %37
}

; Function Attrs: noreturn nounwind uwtable
define internal fastcc void @lsccll.lib.parallel.thread_process_main(ptr nocapture noundef readonly %0, ptr noundef %1) unnamed_addr #7 {
  %3 = tail call ptr %0(ptr noundef %1) #6
  %4 = load ptr, ptr @shared_mem, align 8, !tbaa !17
  %5 = icmp eq ptr %4, null
  br i1 %5, label %9, label %6

6:                                                ; preds = %2
  %7 = getelementptr inbounds %struct.shared_data, ptr %4, i64 0, i32 1
  %8 = atomicrmw add ptr %7, i32 1 seq_cst, align 4
  br label %9

9:                                                ; preds = %6, %2
  %10 = ptrtoint ptr %3 to i64
  %11 = trunc i64 %10 to i32
  %12 = and i32 %11, 255
  tail call void @exit(i32 %12) #8
  unreachable
}

; Function Attrs: nounwind uwtable
define dso_local noundef signext i32 @lsccll.lib.parallel.thread_join(ptr noundef %0, ptr noundef writeonly %1) local_unnamed_addr #0 {
  %3 = alloca i32, align 4
  %4 = icmp eq ptr %0, null
  br i1 %4, label %43, label %5

5:                                                ; preds = %2
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %3) #6
  %6 = load i32, ptr %0, align 8, !tbaa !21
  %7 = call i32 @waitpid(i32 %6, ptr nonnull %3, i32 0) #6
  %8 = icmp eq i32 %7, -1
  br i1 %8, label %41, label %9

9:                                                ; preds = %5
  %10 = icmp eq ptr %1, null
  br i1 %10, label %20, label %11

11:                                               ; preds = %9
  %12 = load i32, ptr %3, align 4, !tbaa !5
  %13 = and i32 %12, 127
  %14 = icmp eq i32 %13, 0
  %15 = lshr i32 %12, 8
  %16 = and i32 %15, 255
  %17 = zext nneg i32 %16 to i64
  %18 = inttoptr i64 %17 to ptr
  %19 = select i1 %14, ptr %18, ptr null
  store ptr %19, ptr %1, align 8, !tbaa !17
  br label %20

20:                                               ; preds = %11, %9
  %21 = load ptr, ptr @thread_list, align 8, !tbaa !17
  %22 = icmp eq ptr %21, null
  br i1 %22, label %38, label %23

23:                                               ; preds = %20
  %24 = icmp eq ptr %21, %0
  br i1 %24, label %34, label %25

25:                                               ; preds = %23, %30
  %26 = phi ptr [ %28, %30 ], [ %21, %23 ]
  %27 = getelementptr inbounds %struct.thread_control_block, ptr %26, i64 0, i32 6
  %28 = load ptr, ptr %27, align 8, !tbaa !17
  %29 = icmp eq ptr %28, null
  br i1 %29, label %38, label %30, !llvm.loop !29

30:                                               ; preds = %25
  %31 = icmp eq ptr %28, %0
  br i1 %31, label %32, label %25, !llvm.loop !29

32:                                               ; preds = %30
  %33 = getelementptr inbounds %struct.thread_control_block, ptr %26, i64 0, i32 6
  br label %34

34:                                               ; preds = %32, %23
  %35 = phi ptr [ @thread_list, %23 ], [ %33, %32 ]
  %36 = getelementptr inbounds %struct.thread_control_block, ptr %0, i64 0, i32 6
  %37 = load ptr, ptr %36, align 8, !tbaa !28
  store ptr %37, ptr %35, align 8, !tbaa !17
  br label %38

38:                                               ; preds = %25, %20, %34
  call void @free(ptr %0) #6
  %39 = load ptr, ptr @shared_mem, align 8, !tbaa !17
  %40 = atomicrmw sub ptr %39, i32 1 seq_cst, align 4
  br label %41

41:                                               ; preds = %5, %38
  %42 = phi i32 [ 0, %38 ], [ -1, %5 ]
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %3) #6
  br label %43

43:                                               ; preds = %2, %41
  %44 = phi i32 [ %42, %41 ], [ -1, %2 ]
  ret i32 %44
}

; Function Attrs: noreturn nounwind uwtable
define dso_local void @lsccll.lib.parallel.thread_exit(ptr noundef %0) local_unnamed_addr #7 {
  %2 = ptrtoint ptr %0 to i64
  %3 = trunc i64 %2 to i32
  %4 = and i32 %3, 255
  tail call void @exit(i32 %4) #8
  unreachable
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noalias noundef ptr @lsccll.lib.parallel.thread_self() local_unnamed_addr #9 {
  ret ptr null
}

; Function Attrs: nounwind uwtable
define dso_local signext i32 @lsccll.lib.parallel.thread_yield() local_unnamed_addr #0 {
  %1 = tail call i32 @sched_yield() #6
  ret i32 %1
}

; Function Attrs: mustprogress nofree norecurse nounwind willreturn uwtable
define dso_local signext i32 @lsccll.lib.parallel.get_thread_count() local_unnamed_addr #10 {
  %1 = load ptr, ptr @shared_mem, align 8, !tbaa !17
  %2 = icmp eq ptr %1, null
  br i1 %2, label %5, label %3

3:                                                ; preds = %0
  %4 = load volatile i32, ptr %1, align 4, !tbaa !18
  br label %5

5:                                                ; preds = %0, %3
  %6 = phi i32 [ %4, %3 ], [ 0, %0 ]
  ret i32 %6
}

; Function declarations
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1
declare ptr @llvm.stacksave.p0() #2
declare void @llvm.va_start(ptr) #2
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #3
declare void @llvm.va_end(ptr) #2
declare void @llvm.stackrestore.p0(ptr) #2
declare ptr @mmap(ptr noundef, i64 noundef, i32 noundef, i32 noundef, i32 noundef, i64 noundef) local_unnamed_addr #4
declare i32 @waitpid(i32 noundef, ptr noundef, i32 noundef) local_unnamed_addr #5
declare void @free(ptr allocptr nocapture noundef) local_unnamed_addr #4
declare i32 @munmap(ptr noundef, i64 noundef) local_unnamed_addr #4
declare noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #5
declare i32 @fork() local_unnamed_addr #6
declare void @exit(i32 noundef) local_unnamed_addr #8
declare i32 @sched_yield() local_unnamed_addr #4

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn }
attributes #3 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #5 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #6 = { nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #7 = { noreturn nounwind uwtable "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #8 = { noreturn nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #9 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #10 = { mustprogress nofree norecurse nounwind willreturn uwtable "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 18.1.8 (https://github.com/llvm/llvm-project.git 3b5b5c1ec4a3095ab096dd780e84d7ab81f3d7ff)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!12, !12, i64 0}
!12 = !{!"long long", !7, i64 0}
!13 = distinct !{!13, !10}
!14 = !{!15, !15, i64 0}
!15 = !{!"long", !7, i64 0}
!16 = !{!17, !17, i64 0}
!17 = !{!"any pointer", !7, i64 0}
!18 = !{!19, !6, i64 0}
!19 = !{!"shared_data", !6, i64 0, !6, i64 4, !7, i64 8}
!20 = !{!19, !6, i64 4}
!21 = !{!22, !6, i64 0}
!22 = !{!"thread_control_block", !6, i64 0, !6, i64 4, !6, i64 8, !17, i64 16, !17, i64 24, !15, i64 32, !17, i64 40}
!23 = !{!22, !6, i64 8}
!24 = !{!22, !6, i64 4}
!25 = !{!22, !17, i64 16}
!26 = !{!22, !17, i64 24}
!27 = !{!22, !15, i64 32}
!28 = !{!22, !17, i64 40}
!29 = distinct !{!29, !10}
