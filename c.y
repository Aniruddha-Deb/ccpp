%{
#include <cstdio>
#include <iostream>
#include "ast.hpp"
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" FILE *yyin;
 
#define YYPARSE_PARAM tu
// #define YYDEBUG 1
// #define YYERROR_VERBOSE 1
void yyerror(const char *s);

%}

%union {
    ASTTranslationUnit* ast_translation_unit;
    ASTStatement* ast_statement;
    ASTBlockStatement* ast_block_statement;
    ASTFunction* ast_function;
    ASTParameterList* ast_parameter_list;
    ASTParameter* ast_parameter;
    ASTType* ast_type;

    ASTExpression* ast_expression;
    ASTBinaryExpression* ast_binary_expression;
    ASTLiteral* ast_literal;

    char *str;
}

%token	F_CONSTANT FUNC_NAME SIZEOF
%token  <str> IDENTIFIER
%token  <str> I_CONSTANT
%token  <str> STRING_LITERAL
%token	PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME ENUMERATION_CONSTANT

%token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token	CONST RESTRICT VOLATILE
%token	BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token	COMPLEX IMAGINARY 
%token	STRUCT UNION ENUM ELLIPSIS

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL

%nterm <ast_translation_unit> translation_unit
%nterm <ast_function> function_definition
%nterm <ast_parameter_list> parameter_list
%nterm <ast_parameter>  parameter
%nterm <ast_type>  type_name
%nterm <ast_block_statement> statement_list
%nterm <ast_statement> statement
%nterm <ast_expression> expression
%nterm <ast_expression> add_sub_expression
%nterm <ast_expression> mul_div_expression
%nterm <ast_literal> unary_expression
%nterm <ast_block_statement> compound_statement

%start translation_unit
%%

translation_unit
	: function_definition { $$ = new ASTTranslationUnit($1); *((ASTTranslationUnit**)YYPARSE_PARAM) = $$; }
	| translation_unit function_definition { $1->add_function($2); $$ = $1; *((ASTTranslationUnit**)YYPARSE_PARAM) = $$; }
	;

function_definition
	: type_name IDENTIFIER '(' parameter_list ')' compound_statement { $$ = new ASTFunction($1, std::string($2), $4, $6); }
	| type_name IDENTIFIER '(' ')' compound_statement                { $$ = new ASTFunction($1, std::string($2), nullptr, $5); }
	;

type_name : INT  { $$ = new ASTType("int"); }
          | VOID { $$ = new ASTType("void"); }
          ;

parameter_list : parameter                    { $$ = new ASTParameterList($1); }
               | parameter_list ',' parameter { $1->add_parameter($3); $$ = $1; }
               ;

parameter : type_name IDENTIFIER { $$ = new ASTParameter($1, $2); }
          ;

compound_statement : '{' statement_list '}' { $$ = $2; }
                   ;

statement_list : statement                  { $$ = new ASTBlockStatement($1); }
               | statement_list statement   { $1->add_statement($2); $$ = $1; }
               ;

statement : expression ';'      { $$ = new ASTExpressionStatement($1); }
          | compound_statement  { $$ = $1; }
          ;

expression : IDENTIFIER '=' expression { $$ = new ASTBinaryExpression(new ASTLiteral($1, L_IDENTIFIER), EQUALTO, $3); }
           | add_sub_expression        { $$ = $1; }
           ;

add_sub_expression : mul_div_expression                        { $$ = $1; }
                   | add_sub_expression '+' mul_div_expression { $$ = new ASTBinaryExpression($1, ADD, $3); }
                   | add_sub_expression '-' mul_div_expression { $$ = new ASTBinaryExpression($1, SUBTRACT, $3); }
                   ;

mul_div_expression : unary_expression                          { $$ = $1; }
                   | '(' add_sub_expression ')'                { $$ = $2; }
                   | mul_div_expression '*' unary_expression   { $$ = new ASTBinaryExpression($1, MULTIPLY, $3); }
                   | mul_div_expression '/' unary_expression   { $$ = new ASTBinaryExpression($1, DIVIDE, $3); }
                   ;

unary_expression : IDENTIFIER { $$ = new ASTLiteral($1, L_IDENTIFIER); }
                 | I_CONSTANT { $$ = new ASTLiteral($1, L_INTEGER); }

%%
#include <stdio.h>

void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
}
