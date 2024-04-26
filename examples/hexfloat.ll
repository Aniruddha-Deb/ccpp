; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(i8*, ...)

define float @getfloat() {
entry:
  %l0 = alloca float, align 4
  store float 5.000000e+02, float* %l0, align 4
  ret float 4.265000e+03
}

define i32 @main() {
entry:
  %l0 = alloca float, align 4
  %calltmp = call float @getfloat()
  store float %calltmp, float* %l0, align 4
  %f = load float, float* %l0, align 4
  %calltmp1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), float %f)
  ret i32 0
}

