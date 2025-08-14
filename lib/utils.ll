; 用作中端测试

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
