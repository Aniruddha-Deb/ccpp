@.str = private constant [16 x i8] c"Hello, world!\\n\00"

declare void @printf(i8*, ...)

define i32 @main() {
entry:
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str, i32 0, i32 0))
  ret i32 0
}