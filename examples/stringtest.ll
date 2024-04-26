; ModuleID = 'Code Generator'
source_filename = "Code Generator"

@.str = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@.str.2 = private unnamed_addr constant [9 x i8] c"Multiple\00", align 1
@.str.3 = private unnamed_addr constant [7 x i8] c"String\00", align 1
@.str.4 = private unnamed_addr constant [9 x i8] c"Literals\00", align 1
@.str.5 = private unnamed_addr constant [3 x i8] c"NO\00", align 1
@.str.6 = private unnamed_addr constant [16 x i8] c"%s %s %s %s %s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %l4 = alloca i8*, align 8
  %l3 = alloca i8*, align 8
  %l2 = alloca i8*, align 8
  %l1 = alloca i8*, align 8
  %l0 = alloca i8*, align 8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str, i32 0, i32 0), i8** %l0, align 8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i32 0, i32 0), i8** %l1, align 8
  store i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.2, i32 0, i32 0), i8** %l2, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.3, i32 0, i32 0), i8** %l3, align 8
  store i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.4, i32 0, i32 0), i8** %l4, align 8
  store i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.5, i32 0, i32 0), i8** %l4, align 8
  %c = load i8*, i8** %l0, align 8
  %d = load i8*, i8** %l1, align 8
  %e = load i8*, i8** %l2, align 8
  %f = load i8*, i8** %l3, align 8
  %g = load i8*, i8** %l4, align 8
  %calltmp = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.6, i32 0, i32 0), i8* %c, i8* %d, i8* %e, i8* %f, i8* %g)
  ret i32 0
}

