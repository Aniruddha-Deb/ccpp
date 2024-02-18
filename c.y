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
    ASTTranslationUnit* ast_tu;
    ASTFunction* ast_fd;
    ASTParameterList* ast_pl;
    ASTParameter* ast_p;
    ASTType* ast_t;
    int n;
    char *s;
    char *id;
}

%token	F_CONSTANT FUNC_NAME SIZEOF
%token  <id> IDENTIFIER
%token  <n> I_CONSTANT
%token  <s> STRING_LITERAL
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

%nterm <ast_tu> translation_unit
%nterm <ast_fd> function_definition // these decls allow us to do this stuff
%nterm <ast_pl> parameter_list
%nterm <ast_p>  parameter
%nterm <ast_t>  type_name

%start translation_unit
%%

translation_unit
	: function_definition { $$ = new ASTTranslationUnit($1); *((ASTTranslationUnit**)YYPARSE_PARAM) = $$; }
	| translation_unit function_definition { $1->add_function($2); $$ = $1; *((ASTTranslationUnit**)YYPARSE_PARAM) = $$; }
	;

function_definition
	: type_name IDENTIFIER '(' parameter_list ')' { $$ = new ASTFunction($1, std::string($2), $4); }
	| type_name IDENTIFIER '(' ')' { $$ = new ASTFunction($1, std::string($2), nullptr); }
	;

type_name : INT  { $$ = new ASTType("int"); }
          | VOID { $$ = new ASTType("void"); }
          ;

parameter_list : parameter { $$ = new ASTParameterList($1); }
               | parameter_list ',' parameter { $1->add_parameter($3); $$ = $1; }
               ;

parameter : type_name IDENTIFIER { $$ = new ASTParameter($1, $2); }
          ;

compound_statement : '{' statement_list '}'
                   ;

statement_list : statement 
               | statement_list statement
               ;

statement : expression ';' 
          | compound_statement
          ;

expression : IDENTIFIER '=' expression
           | add_sub_expression
           ;

add_sub_expression : mul_div_expression
                   | add_sub_expression '+' mul_div_expression
                   | add_sub_expression '-' mul_div_expression 
                   ;

mul_div_expression : unary_expression 
                   | '(' add_sub_expression ')'
                   | mul_div_expression '*' unary_expression 
                   | mul_div_expression '/' unary_expression
                   ;

unary_expression : IDENTIFIER | I_CONSTANT;

%%
#include <stdio.h>

void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
}
