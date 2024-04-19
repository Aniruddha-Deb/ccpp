; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [17 x i8] c"\22hello, world\\n\22\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"\22\22\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main(i32 %v0, i8** %v1) {
entry:
  %l2 = alloca i8, align 1
  %l1 = alloca i8**, align 8
  %l0 = alloca i32, align 4
  store i32 %v0, i32* %l0, align 4
  store i8** %v1, i8*** %l1, align 8
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str, i32 0, i32 0))
  %dereftemp = load float, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), align 4
  store float %dereftemp, i8* %l2, align 4
  ret i32 0
}

