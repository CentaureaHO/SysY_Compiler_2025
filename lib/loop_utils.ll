; ModuleID = 'lib/loop_utils.c'
source_filename = "lib/loop_utils.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.__va_list_tag = type { i32, i32, ptr, ptr }

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
  %171 = sext i32 %11 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 16 %15, ptr nonnull align 16 %14, i64 %171, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 16 %16, ptr nonnull align 16 %14, i64 %171, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr nonnull align 16 %17, ptr nonnull align 16 %14, i64 %171, i1 false)
  store i32 1, ptr %15, align 16, !tbaa !5
  store i32 2, ptr %16, align 16, !tbaa !5
  store i32 3, ptr %17, align 16, !tbaa !5
  %172 = call i32 @pthread_create(ptr noundef nonnull %7, ptr noundef null, ptr noundef %0, ptr noundef nonnull %14) #6
  %173 = getelementptr inbounds [4 x i64], ptr %7, i64 0, i64 1
  %174 = call i32 @pthread_create(ptr noundef nonnull %173, ptr noundef null, ptr noundef %0, ptr noundef nonnull %15) #6
  %175 = getelementptr inbounds [4 x i64], ptr %7, i64 0, i64 2
  %176 = call i32 @pthread_create(ptr noundef nonnull %175, ptr noundef null, ptr noundef %0, ptr noundef nonnull %16) #6
  %177 = getelementptr inbounds [4 x i64], ptr %7, i64 0, i64 3
  %178 = call i32 @pthread_create(ptr noundef nonnull %177, ptr noundef null, ptr noundef %0, ptr noundef nonnull %17) #6
  %179 = load i64, ptr %7, align 16, !tbaa !14
  %180 = call i32 @pthread_join(i64 noundef %179, ptr noundef null) #6
  %181 = load i64, ptr %173, align 8, !tbaa !14
  %182 = call i32 @pthread_join(i64 noundef %181, ptr noundef null) #6
  %183 = load i64, ptr %175, align 16, !tbaa !14
  %184 = call i32 @pthread_join(i64 noundef %183, ptr noundef null) #6
  %185 = load i64, ptr %177, align 8, !tbaa !14
  %186 = call i32 @pthread_join(i64 noundef %185, ptr noundef null) #6
  call void @llvm.stackrestore.p0(ptr %13)
  call void @llvm.lifetime.end.p0(i64 32, ptr nonnull %7) #6
  call void @llvm.lifetime.end.p0(i64 24, ptr nonnull %6) #6
  ret void
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare ptr @llvm.stacksave.p0() #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start(ptr) #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #3

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end(ptr) #2

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn
declare void @llvm.stackrestore.p0(ptr) #2

; Function Attrs: nounwind
declare i32 @pthread_create(ptr noundef, ptr noundef, ptr noundef, ptr noundef) local_unnamed_addr #4

declare i32 @pthread_join(i64 noundef, ptr noundef) local_unnamed_addr #5

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nocallback nofree nosync nounwind willreturn }
attributes #3 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nounwind }

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
