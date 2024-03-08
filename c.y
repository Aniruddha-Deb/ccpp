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
    ast::Operator          ast_operator;

    vector<ast::Parameter*>* ast_parameter_list;

    char *str;
}

%token	FUNC_NAME SIZEOF
%token  <str> IDENTIFIER
%token  <str> I_CONSTANT F_CONSTANT
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
%nterm <ast_block_statement> compound_statement
%nterm <ast_declaration> declaration

%nterm <ast_expression> primary_expression
%nterm <ast_expression> constant
%nterm <ast_expression> enumeration_constant
%nterm <ast_expression> string
%nterm <ast_expression> postfix_expression
%nterm <ast_expression> argument_expression_list
%nterm <ast_expression> unary_expression
%nterm <ast_expression> cast_expression
%nterm <ast_expression> multiplicative_expression
%nterm <ast_expression> additive_expression
%nterm <ast_expression> shift_expression
%nterm <ast_expression> relational_expression
%nterm <ast_expression> equality_expression
%nterm <ast_expression> and_expression
%nterm <ast_expression> exclusive_or_expression
%nterm <ast_expression> inclusive_or_expression
%nterm <ast_expression> logical_and_expression
%nterm <ast_expression> logical_or_expression
%nterm <ast_expression> conditional_expression
%nterm <ast_expression> assignment_expression
%nterm <ast_expression> expression

%nterm <ast_operator> unary_operator
%nterm <ast_operator> assignment_operator

%start translation_unit
%%

translation_unit
	: function_definition { $$ = new ast::TranslationUnit(); $$->push_back($1); *((ast::TranslationUnit**)YYPARSE_PARAM) = $$; }
	| translation_unit function_definition { $$->push_back($2); $$ = $1; *((ast::TranslationUnit**)YYPARSE_PARAM) = $$; }
	;

function_definition
    : type_name IDENTIFIER '(' parameter_list ')' compound_statement { $$ = new ast::Function($1, std::string($2), $4, $6); }
    | type_name IDENTIFIER '(' ')' compound_statement                { $$ = new ast::Function($1, std::string($2), nullptr, $5); }
    ;

type_name : INT  { $$ = new ast::Type("int"); }
          | CHAR { $$ = new ast::Type("char"); }
          | FLOAT { $$ = new ast::Type("float"); }
          | DOUBLE { $$ = new ast::Type("double"); }
          | VOID { $$ = new ast::Type("void"); }
          ;

parameter_list : parameter                    { $$ = new vector<ast::Parameter*>(); $$->push_back($1); }
               | parameter_list ',' parameter { $1->push_back($3); $$ = $1; }
               ;

parameter : type_name IDENTIFIER { $$ = new ast::Parameter($1, $2); }

compound_statement : '{' statement_list '}' { $$ = $2; }
                   | '{' '}' { $$ = new ast::BlockStatement(); }
                   ;

statement_list : statement                  { $$ = new ast::BlockStatement(); $$->push_back($1); }
               | statement_list statement   { $1->push_back($2); $$ = $1; }
               ;

statement : declaration ';'     { $$ = new ast::DeclarationStatement($1); }
          | expression ';'      { $$ = new ast::ExpressionStatement($1); }
          | compound_statement  { $$ = $1; }
          ;

declaration : type_name IDENTIFIER { $$ = new ast::Declaration($1, $2); }

/* Expressions 
   Taken from the original C grammar */

primary_expression
    : IDENTIFIER { $$ = new ast::Identifier($1); }
    | constant { $$ = $1; }
    | string { $$ = $1; }
    | '(' expression ')' { $$ = $2; }
    ;

constant
    : I_CONSTANT { $$ = new ast::Literal($1, ast::LT_INT_LIKE); }
    | F_CONSTANT { $$ = new ast::Literal($1, ast::LT_FLOAT_LIKE); }
    ;

string
    : STRING_LITERAL { $$ = new ast::Literal($1, ast::LT_STRING); }
    ;

/* TODO make expressions for all these things */
postfix_expression
    : primary_expression { $$ = $1; }
    | postfix_expression '[' expression ']'
    | postfix_expression '(' ')'
    | postfix_expression '(' argument_expression_list ')'
    | postfix_expression '.' IDENTIFIER
    | postfix_expression PTR_OP IDENTIFIER
    | postfix_expression INC_OP { $$ = new ast::UnaryExpression(ast::OP_POST_INCR, $1); }
    | postfix_expression DEC_OP { $$ = new ast::UnaryExpression(ast::OP_POST_DECR, $1); }
    ;

argument_expression_list
    : assignment_expression { $$ = $1; }
    | argument_expression_list ',' assignment_expression
    ;

unary_expression
    : postfix_expression { $$ = $1; }
    | INC_OP unary_expression { $$ = new ast::UnaryExpression(ast::OP_PRE_INCR, $2); }
    | DEC_OP unary_expression { $$ = new ast::UnaryExpression(ast::OP_PRE_DECR, $2); }
    | unary_operator cast_expression { $$ = new ast::UnaryExpression($1, $2); }
    | SIZEOF unary_expression { $$ = new ast::UnaryExpression(ast::OP_SIZEOF, $2); }
    | SIZEOF '(' type_name ')' { $$ = new ast::TypeExpression(ast::OP_SIZEOF, $3); }
    | ALIGNOF '(' type_name ')' { $$ = new ast::TypeExpression(ast::OP_ALIGNOF, $3); }
    ;

unary_operator
    : '&' { $$ = ast::OP_AND; }
    | '*' { $$ = ast::OP_DEREF; }
    | '+' { $$ = ast::OP_UNARY_PLUS; }
    | '-' { $$ = ast::OP_UNARY_MINUS; }
    | '~' { $$ = ast::OP_NOT; }
    | '!' { $$ = ast::OP_BOOL_NOT; }
    ;

cast_expression
    : unary_expression { $$ = $1; }
    | '(' type_name ')' cast_expression { $$ = new ast::TypecastExpression($2, $4); }
    ;

multiplicative_expression
    : cast_expression { $$ = $1; }
    | multiplicative_expression '*' cast_expression { $$ = new ast::BinaryExpression($1, ast::OP_MUL, $3); }
    | multiplicative_expression '/' cast_expression { $$ = new ast::BinaryExpression($1, ast::OP_DIV, $3); }
    | multiplicative_expression '%' cast_expression { $$ = new ast::BinaryExpression($1, ast::OP_MOD, $3); }
    ;

additive_expression
    : multiplicative_expression { $$ = $1; }
    | additive_expression '+' multiplicative_expression { $$ = new ast::BinaryExpression($1, ast::OP_ADD, $3); }
    | additive_expression '-' multiplicative_expression { $$ = new ast::BinaryExpression($1, ast::OP_SUB, $3); }
    ;

shift_expression
    : additive_expression { $$ = $1; }
    | shift_expression LEFT_OP additive_expression { $$ = new ast::BinaryExpression($1, ast::OP_LSHIFT, $3); }
    | shift_expression RIGHT_OP additive_expression { $$ = new ast::BinaryExpression($1, ast::OP_RSHIFT, $3); }
    ;

relational_expression
    : shift_expression { $$ = $1; }
    | relational_expression '<' shift_expression { $$ = new ast::BinaryExpression($1, ast::OP_LT, $3); }
    | relational_expression '>' shift_expression { $$ = new ast::BinaryExpression($1, ast::OP_GT, $3); }
    | relational_expression LE_OP shift_expression { $$ = new ast::BinaryExpression($1, ast::OP_LE, $3); }
    | relational_expression GE_OP shift_expression { $$ = new ast::BinaryExpression($1, ast::OP_GE, $3); }
    ;

equality_expression
    : relational_expression { $$ = $1; }
    | equality_expression EQ_OP relational_expression { $$ = new ast::BinaryExpression($1, ast::OP_EQ, $3); }
    | equality_expression NE_OP relational_expression { $$ = new ast::BinaryExpression($1, ast::OP_NE, $3); }
    ;

and_expression
    : equality_expression { $$ = $1; }
    | and_expression '&' equality_expression { $$ = new ast::BinaryExpression($1, ast::OP_AND, $3); }
    ;

exclusive_or_expression
    : and_expression { $$ = $1; }
    | exclusive_or_expression '^' and_expression { $$ = new ast::BinaryExpression($1, ast::OP_XOR, $3); }
    ;

inclusive_or_expression
    : exclusive_or_expression { $$ = $1; }
    | inclusive_or_expression '|' exclusive_or_expression { $$ = new ast::BinaryExpression($1, ast::OP_OR, $3); }
    ;

logical_and_expression
    : inclusive_or_expression { $$ = $1; }
    | logical_and_expression AND_OP inclusive_or_expression { $$ = new ast::BinaryExpression($1, ast::OP_BOOL_AND, $3); }
    ;

logical_or_expression
    : logical_and_expression { $$ = $1; }
    | logical_or_expression OR_OP logical_and_expression { $$ = new ast::BinaryExpression($1, ast::OP_BOOL_OR, $3); }
    ;

conditional_expression
    : logical_or_expression { $$ = $1; }
    | logical_or_expression '?' expression ':' conditional_expression { $$ = new ast::TernaryExpression($1, $3, $5); }
    ;

assignment_expression
    : conditional_expression { $$ = $1; }
    | unary_expression assignment_operator assignment_expression { $$ = new ast::BinaryExpression($1, $2, $3); }
    ;

assignment_operator
    : '=' { $$ = ast::OP_ASSIGN; }
    | MUL_ASSIGN { $$ = ast::OP_MUL_ASSIGN; }
    | DIV_ASSIGN { $$ = ast::OP_DIV_ASSIGN; }
    | MOD_ASSIGN { $$ = ast::OP_MOD_ASSIGN; }
    | ADD_ASSIGN { $$ = ast::OP_ADD_ASSIGN; }
    | SUB_ASSIGN { $$ = ast::OP_SUB_ASSIGN; }
    | LEFT_ASSIGN { $$ = ast::OP_LEFT_ASSIGN; }
    | RIGHT_ASSIGN { $$ = ast::OP_RIGHT_ASSIGN; }
    | AND_ASSIGN { $$ = ast::OP_AND_ASSIGN; }
    | XOR_ASSIGN { $$ = ast::OP_XOR_ASSIGN; }
    | OR_ASSIGN { $$ = ast::OP_OR_ASSIGN; }
    ;

expression
    : assignment_expression { $$ = $1; }
    | expression ',' assignment_expression { $$ = new ast::BinaryExpression($1, ast::OP_SEQ, $3); }
    ;

/*
expression : IDENTIFIER '=' expression { $$ = new ast::BinaryExpression{{}, new ast::Reference{{}, $1}, ast::OP_ASSIGN, $3}; }
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
*/

%%
#include <stdio.h>

void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
}
