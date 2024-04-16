define i32 @main() {
entry:
  %l6 = alloca double, align 8
  %l5 = alloca i32, align 4
  %l4 = alloca i32, align 4
  %l3 = alloca i32, align 4
  %l2 = alloca i1, align 1
  %l1 = alloca i64, align 8
  %l0 = alloca i16, align 2
  store i32 2, i32* %l3, align 4
  store i32 5, i32* %l4, align 4
  %k = load i32, i32* %l4, align 4
  %c = load i32, i32* %l3, align 4
  %temp = add i32 %k, %c
  store i32 %temp, i32* %l5, align 4
  ret i32 0
}