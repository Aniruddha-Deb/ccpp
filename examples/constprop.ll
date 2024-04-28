; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [15 x i8] c"%d %d %d %d %d\00", align 1

declare i32 @foo(i32, i32*)

declare void @printf(i8*, ...)

define i32 @main() {
entry:
  %l22 = alloca i32, align 4
  %l7 = alloca i32, align 4
  %l6 = alloca i32, align 4
  %l5 = alloca i32, align 4
  %l4 = alloca i32, align 4
  %l3 = alloca i32, align 4
  %l2 = alloca i32, align 4
  %l1 = alloca i32, align 4
  %l0 = alloca i32, align 4
  store i32 3, i32* %l0, align 4
  store i32 3, i32* %l1, align 4
  store i32 6, i32* %l2, align 4
  store i32 9, i32* %l4, align 4
  store i32 81, i32* %l5, align 4
  store i32 4, i32* %l0, align 4
  store i32 6, i32* %l0, align 4
  store i32 8, i32* %l0, align 4
  store i32 11, i32* %l0, align 4
  store i32 29, i32* %l4, align 4
  store i32 5, i32* %l0, align 4
  store i32 6, i32* %l0, align 4
  store i32 6, i32* %l1, align 4
  store i32 11, i32* %l1, align 4
  %b = load i32, i32* %l1, align 4
  %temp = add i32 6, %b
  store i32 %temp, i32* %l2, align 4
  %b1 = load i32, i32* %l1, align 4
  %c = load i32, i32* %l2, align 4
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i32 0, i32 0), i32 6, i32 %b1, i32 %c, i32 29, i32 81)
  %returnval = load i32, i32* %l22, align 4
  ret i32 %returnval
}

