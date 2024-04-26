; ModuleID = 'Code Generator'
source_filename = "Code Generator"

define i32 @main() {
entry:
  %l0 = alloca i32, align 4
  store i32 1, i32* %l0, align 4
  ret i32 0
}

