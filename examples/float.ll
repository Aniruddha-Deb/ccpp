; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %l0 = alloca float, align 4
  store float 5.000000e+00, float* %l0, align 4
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), float 5.000000e+00)
  %calltmp1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), float 5.000000e+00)
  %calltmp2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), float 0x3FE5555560000000)
  ret i32 0
}

