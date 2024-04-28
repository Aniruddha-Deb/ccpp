; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@.str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare void @printf(i8*, ...)

define i32 @main() {
entry:
  %l2 = alloca i32, align 4
  %l1 = alloca i32, align 4
  %l0 = alloca i32, align 4
  store i32 0, i32* %l0, align 4
  br label %loop_cond

loop_cond:                                        ; preds = %afterloop3, %entry
  %i = load i32, i32* %l0, align 4
  %temp = icmp slt i32 %i, 10
  br i1 %temp, label %loop_body, label %afterloop

loop_body:                                        ; preds = %loop_cond
  store i32 0, i32* %l1, align 4
  br label %loop_cond1

afterloop:                                        ; preds = %loop_cond
  %returnval = load i32, i32* %l2, align 4
  ret i32 %returnval

loop_cond1:                                       ; preds = %ifcont, %loop_body
  %j = load i32, i32* %l1, align 4
  %temp4 = icmp slt i32 %j, 10
  br i1 %temp4, label %loop_body2, label %afterloop3

loop_body2:                                       ; preds = %loop_cond1
  %j5 = load i32, i32* %l1, align 4
  %temp6 = srem i32 %j5, 2
  %temp7 = icmp eq i32 %temp6, 0
  br i1 %temp7, label %then, label %else

afterloop3:                                       ; preds = %loop_cond1
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0))
  %i14 = load i32, i32* %l0, align 4
  %temp15 = add i32 %i14, 1
  store i32 %temp15, i32* %l0, align 4
  br label %loop_cond

then:                                             ; preds = %loop_body2
  %j8 = load i32, i32* %l1, align 4
  %temp9 = add i32 %j8, 2
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %temp9)
  br label %ifcont

else:                                             ; preds = %loop_body2
  %i10 = load i32, i32* %l0, align 4
  %temp11 = add i32 %i10, 0
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %temp11)
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %j12 = load i32, i32* %l1, align 4
  %temp13 = add i32 %j12, 1
  store i32 %temp13, i32* %l1, align 4
  br label %loop_cond1
}

