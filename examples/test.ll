TranslationUnit constructor called
DeclarationSpecifiers constructor called
Identifier constructor called with name: empty
PureDeclaration constructor called
Function constructor called
TranslationUnit constructor called
DeclarationSpecifiers constructor called
Identifier constructor called with name: simple_arith
PureDeclaration constructor called
Literal constructor called with value: 10
Literal constructor called with value: 10
Literal constructor called with value: 3
BinaryExpression constructor called
BinaryExpression constructor called
Literal constructor called with value: 3
BinaryExpression constructor called
Literal constructor called with value: 23
Literal constructor called with value: 8
Literal constructor called with value: 12
BinaryExpression constructor called
BinaryExpression constructor called
BinaryExpression constructor called
Literal constructor called with value: 1024
BinaryExpression constructor called
ReturnStatement constructor called
Function constructor called
DeclarationSpecifiers constructor called
Identifier constructor called with name: simple_arith_with_arg
PureDeclaration constructor called
DeclarationSpecifiers constructor called
Identifier constructor called with name: d
PureDeclaration constructor called
FunctionParameterList constructor called
Identifier constructor called with name: d
Identifier constructor called with name: d
Literal constructor called with value: 2
BinaryExpression constructor called
BinaryExpression constructor called
Identifier constructor called with name: d
Literal constructor called with value: 100
BinaryExpression constructor called
BinaryExpression constructor called
Identifier constructor called with name: d
Literal constructor called with value: 99
BinaryExpression constructor called
BinaryExpression constructor called
ExpressionStatement constructor called
Function constructor called
TranslationUnit::dump_ast: 
listify:  with 3 nodes 
Function::dump_ast: 
PureDeclaration::dump_ast: 
starify: 0, empty
DeclarationSpecifiers::dump_ast: 
BlockStatement::dump_ast: 
listify: |      with 0 nodes 
Function::dump_ast: 
PureDeclaration::dump_ast: 
starify: 0, simple_arith
DeclarationSpecifiers::dump_ast: 
BlockStatement::dump_ast: 
listify: |      with 1 nodes 
ReturnStatement::dump_ast: 
BinaryExpression::dump_ast: 
Literal::dump_ast: 
BinaryExpression::dump_ast: 
BinaryExpression::dump_ast: 
BinaryExpression::dump_ast: 
Literal::dump_ast: 
Literal::dump_ast: 
Literal::dump_ast: 
BinaryExpression::dump_ast: 
Literal::dump_ast: 
BinaryExpression::dump_ast: 
BinaryExpression::dump_ast: 
Literal::dump_ast: 
Literal::dump_ast: 
Literal::dump_ast: 
Function::dump_ast: 
PureDeclaration::dump_ast: 
starify: 0, simple_arith_with_arg
DeclarationSpecifiers::dump_ast: 
FunctionParameterList::dump_ast: 
listify:    |   with 1 nodes 
PureDeclaration::dump_ast: 
starify: 0, d
DeclarationSpecifiers::dump_ast: 
BlockStatement::dump_ast: 
listify:        with 1 nodes 
ExpressionStatement::dump_ast: 
BinaryExpression::dump_ast: 
BinaryExpression::dump_ast: 
Literal::dump_ast: 
Identifier::dump_ast: 
BinaryExpression::dump_ast: 
BinaryExpression::dump_ast: 
Literal::dump_ast: 
Identifier::dump_ast: 
BinaryExpression::dump_ast: 
BinaryExpression::dump_ast: 
Literal::dump_ast: 
Identifier::dump_ast: 
Identifier::dump_ast: 
define i32 @empty() {
entry:
}
define i32 @simple_arith() {
entry:
  ret i32 915
}
define i32 @simple_arith_with_arg(i32 %d) {
entry:
  %divtmp = sdiv i32 %d, 2
  %addtmp = add i32 %d, %divtmp
  %addtmp1 = add i32 %d, 100
  %addtmp2 = add i32 %addtmp, %addtmp1
  %addtmp3 = add i32 %d, 99
  %addtmp4 = add i32 %addtmp2, %addtmp3
}
