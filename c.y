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
    ast::TranslationUnit*  ast_translation_unit;
    ast::Statement*        ast_statement;
    ast::BlockStatement*   ast_block_statement;
    ast::Function*         ast_function;
    ast::Parameter*        ast_parameter;
    ast::Type*             ast_type;
    ast::Expression*       ast_expression;
    ast::Declaration*      ast_declaration;
    ast::BinaryExpression* ast_binary_expression;
    ast::Literal*          ast_literal;

    vector<ast::Parameter*>* ast_parameter_list;

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
%nterm <ast_declaration> declaration
%nterm <ast_expression> expression
%nterm <ast_expression> add_sub_expression
%nterm <ast_expression> mul_div_expression
%nterm <ast_expression> unary_expression
%nterm <ast_block_statement> compound_statement

%start translation_unit
%%

translation_unit
	: function_definition { $$ = new ast::TranslationUnit(); $$->push_back($1); *((ast::TranslationUnit**)YYPARSE_PARAM) = $$; }
	| translation_unit function_definition { $$->push_back($2); $$ = $1; *((ast::TranslationUnit**)YYPARSE_PARAM) = $$; }
	;

function_definition
	: type_name IDENTIFIER '(' parameter_list ')' compound_statement { $$ = new ast::Function{{}, $1, std::string($2), $4, $6}; }
	| type_name IDENTIFIER '(' ')' compound_statement                { $$ = new ast::Function{{}, $1, std::string($2), nullptr, $5}; }
	;

type_name : INT  { $$ = new ast::Type{{}, "int"}; }
          | CHAR { $$ = new ast::Type{{}, "char"}; }
          | FLOAT { $$ = new ast::Type{{}, "float"}; }
          | DOUBLE { $$ = new ast::Type{{}, "double"}; }
          | VOID { $$ = new ast::Type{{}, "void"}; }
          ;

parameter_list : parameter                    { $$ = new vector<ast::Parameter*>(); $$->push_back($1); }
               | parameter_list ',' parameter { $1->push_back($3); $$ = $1; }
               ;

parameter : type_name IDENTIFIER { $$ = new ast::Parameter{{}, $1, $2}; }
          ;

compound_statement : '{' statement_list '}' { $$ = $2; }
                   ;

statement_list : statement                  { $$ = new ast::BlockStatement(); $$->push_back($1); }
               | statement_list statement   { $1->push_back($2); $$ = $1; }
               ;

statement : declaration ';'     { $$ = new ast::DeclarationStatement{{}, $1}; }
          | expression ';'      { $$ = new ast::ExpressionStatement{{}, $1}; }
          | compound_statement  { $$ = $1; }
          ;

declaration : type_name IDENTIFIER { $$ = new ast::Declaration{{}, $1, $2}; }

expression : IDENTIFIER '=' expression { $$ = new ast::BinaryExpression{{}, new ast::Reference{{}, $1}, ast::ASSIGN, $3}; }
           | add_sub_expression        { $$ = $1; }
           ;

add_sub_expression : mul_div_expression                        { $$ = $1; }
                   | add_sub_expression '+' mul_div_expression { $$ = new ast::BinaryExpression{{}, $1, ast::ADD, $3}; }
                   | add_sub_expression '-' mul_div_expression { $$ = new ast::BinaryExpression{{}, $1, ast::SUB, $3}; }
                   ;

mul_div_expression : unary_expression                          { $$ = $1; }
                   | '(' add_sub_expression ')'                { $$ = $2; }
                   | mul_div_expression '*' unary_expression   { $$ = new ast::BinaryExpression{{}, $1, ast::MUL, $3}; }
                   | mul_div_expression '/' unary_expression   { $$ = new ast::BinaryExpression{{}, $1, ast::DIV, $3}; }
                   ;

unary_expression : IDENTIFIER { $$ = new ast::Reference{{}, $1}; }
                 | I_CONSTANT { $$ = new ast::Literal{{}, $1}; }

%%
#include <stdio.h>

void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
}
