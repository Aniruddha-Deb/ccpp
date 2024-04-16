define i32 @empty() {
entry:
  ret i32 0
}
define i32 @simple_arith() {
entry:
  ret i32 915
}
define i32 @simple_arith_with_arg(i32 %v0) {
entry:
  %l0 = alloca i32, align 4
  store i32 %v0, i32* %l0, align 4
  %d = load i32, i32* %l0, align 4
  %d1 = load i32, i32* %l0, align 4
  %0 = sdiv i32 %d1, 2
  %1 = add i32 %d, %0
  %d2 = load i32, i32* %l0, align 4
  %2 = add i32 %d2, 100
  %3 = add i32 %1, %2
  %d3 = load i32, i32* %l0, align 4
  %4 = add i32 %d3, 99
  %5 = add i32 %3, %4
  ret i32 0
}
