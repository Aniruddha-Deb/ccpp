define i32 @factorial(i32 %v0) {
entry:
  %l0 = alloca i32, align 4
  store i32 %v0, i32* %l0, align 4
  %n = load i32, i32* %l0, align 4
  %0 = icmp sle i32 %n, 0
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry
  ret i32 1
  br label %ifcont

else:                                             ; preds = %entry
  %n1 = load i32, i32* %l0, align 4
  %n2 = load i32, i32* %l0, align 4
  %1 = sub i32 %n2, 1
  %calltmp = call i32 @factorial(i32 %1)
  %2 = mul i32 %n1, %calltmp
  ret i32 %2
  br label %ifcont

ifcont:                                           ; preds = %else, %then
}
empty else
define i32 @fibonacci(i32 %v0, i32 %v1, i32 %v2) {
entry:
  %l3 = alloca i32, align 4
  %l2 = alloca i32, align 4
  %l1 = alloca i32, align 4
  %l0 = alloca i32, align 4
  store i32 %v0, i32* %l0, align 4
  store i32 %v1, i32* %l1, align 4
  store i32 %v2, i32* %l2, align 4
  %n = load i32, i32* %l2, align 4
  %0 = icmp sle i32 %n, 0
  br i1 %0, label %then, label %else

then:                                             ; preds = %entry
  %a = load i32, i32* %l0, align 4
  ret i32 %a
  br label %ifcont

else:                                             ; preds = %entry
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  br label %loop_cond

loop_cond:                                        ; preds = %loop_body, %ifcont
  %n1 = load i32, i32* %l2, align 4
  %1 = icmp sgt i32 %n1, 0
  br i1 %1, label %loop_body, label %afterloop

loop_body:                                        ; preds = %loop_cond
  %b = load i32, i32* %l1, align 4
  store volatile i32 %b, i32* %l3, align 4
  %a2 = load i32, i32* %l0, align 4
  %b3 = load i32, i32* %l1, align 4
  %2 = add i32 %a2, %b3
  store volatile i32 %2, i32* %l1, align 4
  %t = load i32, i32* %l3, align 4
  store volatile i32 %t, i32* %l0, align 4
  %n4 = load i32, i32* %l2, align 4
  %3 = sub i32 %n4, 1
  store volatile i32 %3, i32* %l2, align 4
  br label %loop_cond

afterloop:                                        ; preds = %loop_cond
  %b5 = load i32, i32* %l1, align 4
  ret i32 %b5
}
define i32 @main() {
entry:
  %calltmp = call i32 @factorial(i32 1)
  %calltmp1 = call i32 @factorial(i32 2)
  %calltmp2 = call i32 @factorial(i32 5)
  %calltmp3 = call i32 @fibonacci(i32 %calltmp, i32 %calltmp1, i32 %calltmp2)
}
