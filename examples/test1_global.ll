; ModuleID = 'test1.c'
source_filename = "test1.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

@global_var = global i32 11, align 4

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define void @empty() #0 {
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define void @simple_arith(i32 noundef %0) #0 {
  %2 = add %2, 5, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define void @simple_arith_with_arg(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  %5 = sdiv i32 %4, 2
  %6 = icmp sgt i32 %3, %5
  br i1 %6, label %15, label %7

7:                                                ; preds = %1
  %8 = load i32, ptr %2, align 4
  %9 = icmp sge i32 %8, 100
  br i1 %9, label %10, label %13

10:                                               ; preds = %7
  %11 = load i32, ptr %2, align 4
  %12 = icmp slt i32 %11, 99
  br label %13

13:                                               ; preds = %10, %7
  %14 = phi i1 [ false, %7 ], [ %12, %10 ]
  br label %15

15:                                               ; preds = %13, %1
  %16 = phi i1 [ true, %1 ], [ %14, %13 ]
  %17 = zext i1 %16 to i32
  ret void
}

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 14, i32 0]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Apple clang version 15.0.0 (clang-1500.0.40.1)"}
