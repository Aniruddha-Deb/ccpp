; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [13 x i8] c"logical and\0A\00", align 1
@.str.1 = private unnamed_addr constant [16 x i8] c"Positive Zero!\0A\00", align 1
@.str.2 = private unnamed_addr constant [16 x i8] c"Negative Zero!\0A\00", align 1
@.str.3 = private unnamed_addr constant [10 x i8] c"Not zero\0A\00", align 1
@.str.4 = private unnamed_addr constant [13 x i8] c"bitwise and\0A\00", align 1
@.str.5 = private unnamed_addr constant [15 x i8] c"helloworld %d\0A\00", align 1

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
  %calltmp = call i32 @getint(i32 5)
  %calltmp1 = call i32 @getint(i32 2)
  %0 = icmp ne i32 %calltmp1, 0
  %1 = icmp ne i32 %calltmp, 0
  %temp = and i1 %1, %0
  br i1 %temp, label %then, label %else

then:                                             ; preds = %entry
  %calltmp2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0))
  br label %ifcont

else:                                             ; preds = %entry
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %pz = load float, float* %l3, align 4
  %2 = fcmp one float %pz, 0.000000e+00
  br i1 %2, label %then3, label %else5

then3:                                            ; preds = %ifcont
  %calltmp4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1, i32 0, i32 0))
  br label %ifcont6

else5:                                            ; preds = %ifcont
  br label %ifcont6

ifcont6:                                          ; preds = %else5, %then3
  %nz = load float, float* %l4, align 4
  %calltmp7 = call float @getfloat(float %nz)
  %widen = fpext float %calltmp7 to double
  %temp8 = fadd double %widen, 1.000000e+00
  %notz = load float, float* %l5, align 4
  %calltmp9 = call float @getfloat(float %notz)
  %notz10 = load float, float* %l5, align 4
  %calltmp11 = call float @getfloat(float %notz10)
  %temp12 = fcmp oeq float %calltmp9, %calltmp11
  %widen13 = uitofp i1 %temp12 to double
  %temp14 = fadd double %temp8, %widen13
  %3 = fcmp one double %temp14, 0.000000e+00
  br i1 %3, label %then15, label %else17

then15:                                           ; preds = %ifcont6
  %calltmp16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0))
  br label %ifcont18

else17:                                           ; preds = %ifcont6
  br label %ifcont18

ifcont18:                                         ; preds = %else17, %then15
  %notz19 = load float, float* %l5, align 4
  %calltmp20 = call float @getfloat(float %notz19)
  %widen21 = fpext float %calltmp20 to double
  %temp22 = fadd double %widen21, 1.000000e+00
  %4 = fcmp one double %temp22, 0.000000e+00
  br i1 %4, label %then23, label %else25

then23:                                           ; preds = %ifcont18
  %calltmp24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.3, i32 0, i32 0))
  br label %ifcont26

else25:                                           ; preds = %ifcont18
  br label %ifcont26

ifcont26:                                         ; preds = %else25, %then23
  %a = load i32, i32* %l1, align 4
  %b = load i32, i32* %l2, align 4
  %temp27 = and i32 %a, %b
  %5 = icmp ne i32 %temp27, 0
  br i1 %5, label %then28, label %else30

then28:                                           ; preds = %ifcont26
  %calltmp29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.4, i32 0, i32 0))
  br label %ifcont31

else30:                                           ; preds = %ifcont26
  br label %ifcont31

ifcont31:                                         ; preds = %else30, %then28
  %calltmp32 = call i32 @getint(i32 2)
  %calltmp33 = call i32 @getint(i32 1)
  %temp34 = icmp sgt i32 %calltmp32, %calltmp33
  %calltmp35 = call i32 @getint(i32 2)
  %widen36 = zext i1 %temp34 to i32
  %temp37 = add i32 %widen36, %calltmp35
  store i32 %temp37, i32* %l6, align 4
  %x = load i32, i32* %l6, align 4
  %calltmp38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.5, i32 0, i32 0), i32 %x)
  ret i32 0
}

