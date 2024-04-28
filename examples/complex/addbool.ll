; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %l1 = alloca i1, align 1
  %l0 = alloca i1, align 1
  store i1 true, i1* %l0, align 1
  store i1 true, i1* %l1, align 1
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i1 false)
  ret i32 0
}

