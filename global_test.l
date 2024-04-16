; ModuleID = 'examples/global_test.c'
source_filename = "examples/global_test.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

@var3 = common global i32 0, align 4
@var2 = common global i32 0, align 4
@ptr = common global ptr null, align 8
@var1 = common global i32 0, align 4
@var4 = common global float 0.000000e+00, align 4

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @fun(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store i32 2, ptr %5, align 4
  store i32 18, ptr %5, align 4
  %6 = load i32, ptr @var3, align 4
  store i32 %6, ptr @var2, align 4
  store ptr @var3, ptr @ptr, align 8
  store i32 1, ptr @var2, align 4
  %7 = load i32, ptr @var2, align 4
  %8 = add nsw i32 %7, 1
  %9 = load ptr, ptr @ptr, align 8
  store i32 %8, ptr %9, align 4
  %10 = load i32, ptr %2, align 4
  ret i32 %10
}

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define i32 @main() #0 {
  ret i32 0
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
