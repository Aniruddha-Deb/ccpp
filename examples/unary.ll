; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.3 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@.str.4 = private unnamed_addr constant [4 x i8] c"%u\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @getint() {
entry:
  ret i32 4
}

define double @getfloat() {
entry:
  ret double 3.000000e+00
}

define i32 @main() {
entry:
  %l0 = alloca i32, align 4
  store i32 -5, i32* %l0, align 4
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 5)
  %calltmp1 = call i32 @getint()
  %uminus = sub i32 0, %calltmp1
  %calltmp2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %uminus)
  %calltmp3 = call i32 @getint()
  %not = xor i32 %calltmp3, -1
  %calltmp4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), i32 %not)
  %calltmp5 = call double @getfloat()
  %uminus6 = fneg double %calltmp5
  %calltmp7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.3, i32 0, i32 0), double %uminus6)
  %calltmp8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.4, i32 0, i32 0), i32 -5)
  ret i32 5
}

