; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.3 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @getint(i32 %v0) {
entry:
  %l0 = alloca i32, align 4
  store i32 %v0, i32* %l0, align 4
  %a = load i32, i32* %l0, align 4
  ret i32 %a
}

define i32 @main() {
entry:
  %l1 = alloca double, align 8
  %l0 = alloca float, align 4
  store float 5.000000e+00, float* %l0, align 4
  store double 5.000000e+00, double* %l1, align 8
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), double 5.000000e+00)
  %calltmp1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), double 5.000000e+00)
  %calltmp2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), double 0x3FFAAAAAAAAAAAAB)
  %calltmp3 = call i32 @getint(i32 2)
  %calltmp4 = call i32 @getint(i32 1)
  %temp = icmp slt i32 %calltmp3, %calltmp4
  %widen = uitofp i1 %temp to double
  %temp5 = fadd double 1.000000e+00, %widen
  %calltmp6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i32 0, i32 0), double %temp5)
  ret i32 0
}

