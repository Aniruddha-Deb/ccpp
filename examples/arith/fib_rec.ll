; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @getint() {
entry:
  ret i32 0
}

define i64 @fib(i32 %v0) {
entry:
  %l0 = alloca i32, align 4
  store i32 %v0, i32* %l0, align 4
  %n = load i32, i32* %l0, align 4
  %temp = icmp eq i32 %n, 0
  br i1 %temp, label %then, label %else

then:                                             ; preds = %entry
  %calltmp = call i32 @getint()
  %widen = sitofp i32 %calltmp to double
  %temp1 = fadd double %widen, 0.000000e+00
  %widen2 = fptosi double %temp1 to i64
  ret i64 %widen2
  br label %ifcont

else:                                             ; preds = %entry
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %n3 = load i32, i32* %l0, align 4
  %temp4 = icmp eq i32 %n3, 1
  br i1 %temp4, label %then5, label %else6

then5:                                            ; preds = %ifcont
  ret i64 1
  br label %ifcont7

else6:                                            ; preds = %ifcont
  br label %ifcont7

ifcont7:                                          ; preds = %else6, %then5
  %n8 = load i32, i32* %l0, align 4
  %temp9 = sub i32 %n8, 1
  %calltmp10 = call i64 @fib(i32 %temp9)
  %n11 = load i32, i32* %l0, align 4
  %temp12 = sub i32 %n11, 2
  %calltmp13 = call i64 @fib(i32 %temp12)
  %temp14 = add i64 %calltmp10, %calltmp13
  ret i64 %temp14
}

define i32 @main() {
entry:
  %calltmp = call i64 @fib(i32 20)
  %calltmp1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0), i64 %calltmp)
  ret i32 0
}

