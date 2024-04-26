; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [13 x i8] c"logical and\0A\00", align 1
@.str.1 = private unnamed_addr constant [16 x i8] c"Negative Zero!\0A\00", align 1
@.str.2 = private unnamed_addr constant [10 x i8] c"Not zero\0A\00", align 1
@.str.3 = private unnamed_addr constant [15 x i8] c"helloworld %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @getint(i32 %v0) {
entry:
  %l0 = alloca i32, align 4
  store i32 %v0, i32* %l0, align 4
  %a = load i32, i32* %l0, align 4
  ret i32 %a
}

define float @getfloat(float %v0) {
entry:
  %l0 = alloca float, align 4
  store float %v0, float* %l0, align 4
  %f = load float, float* %l0, align 4
  ret float %f
}

define i32 @main() {
entry:
  %l6 = alloca i32, align 4
  %l5 = alloca float, align 4
  %l4 = alloca float, align 4
  %l3 = alloca float, align 4
  %l2 = alloca i32, align 4
  %l1 = alloca i32, align 4
  %l0 = alloca i1, align 1
  store i1 true, i1* %l0, align 1
  store i32 2, i32* %l1, align 4
  store i32 5, i32* %l2, align 4
  store float 0.000000e+00, float* %l3, align 4
  store float -0.000000e+00, float* %l4, align 4
  store float 1.000000e+00, float* %l5, align 4
  br i1 true, label %then, label %else

then:                                             ; preds = %entry
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0))
  br label %ifcont

else:                                             ; preds = %entry
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %calltmp1 = call float @getfloat(float -0.000000e+00)
  %widen = fpext float %calltmp1 to double
  %temp = fadd double %widen, 1.000000e+00
  %calltmp2 = call float @getfloat(float 1.000000e+00)
  %calltmp3 = call float @getfloat(float 1.000000e+00)
  %temp4 = fcmp oeq float %calltmp2, %calltmp3
  %widen5 = uitofp i1 %temp4 to double
  %temp6 = fadd double %temp, %widen5
  %0 = fcmp one double %temp6, 0.000000e+00
  br i1 %0, label %then7, label %else9

then7:                                            ; preds = %ifcont
  %calltmp8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1, i32 0, i32 0))
  br label %ifcont10

else9:                                            ; preds = %ifcont
  br label %ifcont10

ifcont10:                                         ; preds = %else9, %then7
  %calltmp11 = call float @getfloat(float 1.000000e+00)
  %widen12 = fpext float %calltmp11 to double
  %temp13 = fadd double %widen12, 1.000000e+00
  %1 = fcmp one double %temp13, 0.000000e+00
  br i1 %1, label %then14, label %else16

then14:                                           ; preds = %ifcont10
  %calltmp15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0))
  br label %ifcont17

else16:                                           ; preds = %ifcont10
  br label %ifcont17

ifcont17:                                         ; preds = %else16, %then14
  %calltmp18 = call i32 @getint(i32 2)
  %calltmp19 = call i32 @getint(i32 1)
  %temp20 = icmp sgt i32 %calltmp18, %calltmp19
  %calltmp21 = call i32 @getint(i32 2)
  %widen22 = zext i1 %temp20 to i32
  %temp23 = add i32 %widen22, %calltmp21
  store i32 %temp23, i32* %l6, align 4
  %x = load i32, i32* %l6, align 4
  %calltmp24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.3, i32 0, i32 0), i32 %x)
  ret i32 0
}

