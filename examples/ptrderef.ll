; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [12 x i8] c"%d %d %d %d\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %l3 = alloca i32***, align 8
  %l2 = alloca i32**, align 8
  %l1 = alloca i32*, align 8
  %l0 = alloca i32, align 4
  store i32 5, i32* %l0, align 4
  store i32* %l0, i32** %l1, align 8
  store i32** %l1, i32*** %l2, align 8
  store i32*** %l2, i32**** %l3, align 8
  %d = load i32***, i32**** %l3, align 8
  %dereftemp = load i32**, i32*** %d, align 8
  %dereftemp1 = load i32*, i32** %dereftemp, align 8
  store volatile i32 8, i32* %dereftemp1, align 4
  %a = load i32, i32* %l0, align 4
  %b = load i32*, i32** %l1, align 8
  %dereftemp2 = load i32, i32* %b, align 4
  %c = load i32**, i32*** %l2, align 8
  %dereftemp3 = load i32*, i32** %c, align 8
  %dereftemp4 = load i32, i32* %dereftemp3, align 4
  %d5 = load i32***, i32**** %l3, align 8
  %dereftemp6 = load i32**, i32*** %d5, align 8
  %dereftemp7 = load i32*, i32** %dereftemp6, align 8
  %dereftemp8 = load i32, i32* %dereftemp7, align 4
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0), i32 %a, i32 %dereftemp2, i32 %dereftemp4, i32 %dereftemp8)
  ret i32 0
}

