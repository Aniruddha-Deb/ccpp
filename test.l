
@.str = private unnamed_addr constant [16 x i8] c"\22hello world\\n\22\00", align 1

declare i8* @malloc(i32)

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %l2 = alloca i8*, align 8
  %l1 = alloca i8*, align 8
  %l0 = alloca float, align 4
  store float 4.687500e-01, float* %l0, align 4
  %calltmp = call i8* @malloc(i32 4)
  store i8* %calltmp, i8** %l1, align 8
  store i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str, i32 0, i32 0), i8** %l2, align 8
  %c = load i8*, i8** %l1, align 8
  %d = load i8*, i8** %l2, align 8
  %dereftemp = load i8, i8* %d, align 1
  store volatile i8 %dereftemp, i8* %c, align 1
  %c1 = load i8*, i8** %l1, align 8
  %calltmp2 = call i32 (i8*, ...) @printf(i8* %c1)
  ret i32 0
}