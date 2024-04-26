; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(i8*, ...)

define i8 @getchar() {
entry:
  ret i8 99
}

define i32 @getint() {
entry:
  ret i32 5
}

define float @getfloat() {
entry:
  ret float 5.000000e+00
}

define i32 @main() {
entry:
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), float 1.060000e+02)
  %calltmp1 = call i8 @getchar()
  %calltmp2 = call i32 @getint()
  %widen = zext i8 %calltmp1 to i32
  %temp = add i32 %widen, %calltmp2
  %calltmp3 = call float @getfloat()
  %widen4 = sitofp i32 %temp to float
  %temp5 = fadd float %widen4, %calltmp3
  %widen6 = fpext float %temp5 to double
  %temp7 = fadd double %widen6, 1.000000e+00
  %calltmp8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), double %temp7)
  ret i32 0
}

