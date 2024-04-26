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
  br i1 false, label %then1, label %else3

then1:                                            ; preds = %ifcont
  %calltmp2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1, i32 0, i32 0))
  br label %ifcont4

else3:                                            ; preds = %ifcont
  br label %ifcont4

ifcont4:                                          ; preds = %else3, %then1
  br i1 true, label %then5, label %else7

then5:                                            ; preds = %ifcont4
  %calltmp6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0))
  br label %ifcont8

else7:                                            ; preds = %ifcont4
  br label %ifcont8

ifcont8:                                          ; preds = %else7, %then5
  %calltmp9 = call i32 @getint(i32 2)
  %calltmp10 = call i32 @getint(i32 1)
  %temp = icmp slt i32 %calltmp9, %calltmp10
  %widen = zext i1 %temp to i32
  %temp11 = or i32 %widen, 12
  store i32 %temp11, i32* %l6, align 4
  %x = load i32, i32* %l6, align 4
  %calltmp12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.3, i32 0, i32 0), i32 %x)
  ret i32 0
}

