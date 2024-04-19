; ModuleID = 'Code Generator'
source_filename = "Code Generator"

define i32 @main() {
entry:
  %l6 = alloca double, align 8
  %l5 = alloca i16, align 2
  %l4 = alloca i16, align 2
  %l3 = alloca i16, align 2
  %l2 = alloca i1, align 1
  %l1 = alloca i64, align 8
  %l0 = alloca i16, align 2
  store i32 2, i16* %l3, align 4
  store i32 5, i16* %l4, align 4
  store i32 2, i16* %l5, align 4
  ret i32 0
}

