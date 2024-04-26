; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [17 x i8] c"%d %ld %u %u %u\0A\00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"%lf\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %l5 = alloca double, align 8
  %l4 = alloca i16, align 2
  %l3 = alloca i16, align 2
  %l2 = alloca i16, align 2
  %l1 = alloca i64, align 8
  %l0 = alloca i16, align 2
  store i16 5, i16* %l0, align 2
  store i64 10, i64* %l1, align 4
  store i16 2, i16* %l2, align 2
  store i16 15, i16* %l3, align 2
  store i16 7, i16* %l4, align 2
  store double 0x4020666666666666, double* %l5, align 8
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str, i32 0, i32 0), i32 5, i64 10, i32 2, i32 15, i32 7)
  %calltmp1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.1, i32 0, i32 0), double 0x4020666666666666)
  ret i32 0
}

