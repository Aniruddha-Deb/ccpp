; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [13 x i8] c"logical and\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %l5 = alloca float, align 4
  %l4 = alloca float, align 4
  %l3 = alloca float, align 4
  %l2 = alloca i32, align 4
  %l1 = alloca i32, align 4
  %l0 = alloca i1, align 1
  store i32 ()* @main, i1* %l0, align 8
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
  ret i32 0
}

