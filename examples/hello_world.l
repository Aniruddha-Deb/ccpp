; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private constant [15 x i8] c"hello, world\\n\00"

declare i32 @printf(i8*, ...)

define i32 @main(i32 %v0, i8** %v1) {
entry:
  %l1 = alloca i8**, align 8
  %l0 = alloca i32, align 4
  store i32 %v0, i32* %l0, align 4
  store i8** %v1, i8*** %l1, align 8
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i32 0, i32 0))
  ret i32 0
}
