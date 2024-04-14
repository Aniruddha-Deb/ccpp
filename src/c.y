%{
#include <cstdio>
#include <iostream>
#include <algorithm>
#include "ast.hpp"
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" FILE *yyin;
 
// #define YYDEBUG 1
// #define YYERROR_VERBOSE 1
void yyerror(ast::TranslationUnit* tu, const char *s);

%}

%parse-param {ast::TranslationUnit* tu}

%union {
    ast::TranslationUnit*  ast_translation_unit;
    ast::Statement*        ast_statement;
    ast::BlockStatement*   ast_block_statement;
    ast::Function*         ast_function;
    ast::Expression*       ast_expression;
    ast::Declaration*      ast_declaration;
    ast::BinaryExpression* ast_binary_expression;
    ast::Literal*          ast_literal;
    ast::Operator          ast_operator;
    ast::StorageSpecifier  ast_storage_specifier;
    ast::TypeSpecifier     ast_type_specifier;
    ast::TypeQualifier     ast_type_qualifier;
    ast::FunctionSpecifier ast_function_specifier;
    ast::InitDeclarator*   ast_init_declarator;
    ast::PureDeclaration*  ast_pure_declaration;         
    ast::DeclarationSpecifiers*    ast_declaration_specifiers;
    ast::FunctionParameterList*    ast_function_parameter_list;   
    ast::ExpressionStatement*      ast_expression_statement;  
    ast::DeclarationStatement*      ast_declaration_statement;  
    vector<ast::InitDeclarator*>*  ast_init_declarator_list; 
    vector<ast::PureDeclaration*>* ast_parameter_list;
    vector<ast::Expression*>* ast_expression_list;

    char* str;
    int ast_pointer_list;
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
// %nterm <ast_parameter>  parameter
%nterm <ast_block_statement> statement_list
%nterm <ast_block_statement> compound_statement
%nterm <ast_block_statement> block_item_list
%nterm <ast_statement> statement
%nterm <ast_statement> selection_statement
%nterm <ast_statement> iteration_statement
%nterm <ast_statement> jump_statement
%nterm <ast_statement> block_item
%nterm <ast_statement> labeled_statement

%nterm <ast_declaration> declaration

%nterm <ast_expression> primary_expression
%nterm <ast_expression> constant
%nterm <ast_expression> constant_expression
%nterm <ast_expression> string
%nterm <ast_expression> postfix_expression
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

%nterm <ast_expression_list> argument_expression_list

%nterm <ast_operator> unary_operator
%nterm <ast_operator> assignment_operator
%nterm <ast_init_declarator> init_declarator
%nterm <ast_pointer_list> pointer_list

%nterm <ast_type_specifier> type_specifier
%nterm <ast_storage_specifier> storage_class_specifier
%nterm <ast_type_qualifier> type_qualifier
%nterm <ast_function_specifier> function_specifier
%nterm <ast_declaration_specifiers> declaration_specifiers
%nterm <ast_init_declarator_list> init_declarator_list
%nterm <ast_function_parameter_list> function_parameter_list
%nterm <ast_pure_declaration> pure_declaration
%nterm <ast_expression_statement> expression_statement
%nterm <ast_declaration_statement> declaration_statement

%start translation_unit
%%

/* -Expressions-------------------------------------------------------------- */

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

type_name
    : 

postfix_expression
    : primary_expression { $$ = $1; }
    | postfix_expression '[' expression ']'
    | postfix_expression '(' ')' { $$ = new ast::FunctionInvocationExpression($1); }
    | postfix_expression '(' argument_expression_list ')' { $$ = new ast::FunctionInvocationExpression($1, $3); }
    | postfix_expression '.' IDENTIFIER
    | postfix_expression PTR_OP IDENTIFIER
    | postfix_expression INC_OP { $$ = new ast::UnaryExpression(ast::OP_POST_INCR, $1); }
    | postfix_expression DEC_OP { $$ = new ast::UnaryExpression(ast::OP_POST_DECR, $1); }
    ;

argument_expression_list
    : assignment_expression { $$ = new vector<ast::Expression*>(); $$->push_back($1); }
    | argument_expression_list ',' assignment_expression { $1->push_back($3); $$ = $1; }
    ;

unary_expression
    : postfix_expression { $$ = $1; }
    | INC_OP unary_expression { $$ = new ast::UnaryExpression(ast::OP_PRE_INCR, $2); }
    | DEC_OP unary_expression { $$ = new ast::UnaryExpression(ast::OP_PRE_DECR, $2); }
    | unary_operator cast_expression { $$ = new ast::UnaryExpression($1, $2); }
    ;

unary_operator
    : '&' { $$ = ast::OP_AND; }
    | '*' { $$ = ast::OP_DEREF; }
    | '+' { $$ = ast::OP_UNARY_PLUS; }
    | '-' { $$ = ast::OP_UNARY_MINUS; }
    | '~' { $$ = ast::OP_NOT; }
    | '!' { $$ = ast::OP_BOOL_NOT; }
    ;

multiplicative_expression
    : unary_expression { $$ = $1; }
    | multiplicative_expression '*' unary_expression { $$ = new ast::BinaryExpression($1, ast::OP_MUL, $3); }
    | multiplicative_expression '/' unary_expression { $$ = new ast::BinaryExpression($1, ast::OP_DIV, $3); }
    | multiplicative_expression '%' unary_expression { $$ = new ast::BinaryExpression($1, ast::OP_MOD, $3); }
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

constant_expression
	: conditional_expression { $$ = $1; } /* with constraints - should we implement those? */
	;

/* -Declarations------------------------------------------------------------- */

declaration
	: declaration_specifiers ';' { $$ = new ast::Declaration($1, nullptr); }      /* idk just put nullptr here TODO */
	| declaration_specifiers init_declarator_list ';' { $$ = new ast::Declaration($1, $2); }
	;


declaration_specifiers
	: storage_class_specifier declaration_specifiers { $2->add_storage_specifier($1); $$ = $2; }
	| storage_class_specifier { $$ = new ast::DeclarationSpecifiers(); $$->add_storage_specifier($1); }
	| type_specifier declaration_specifiers { $2->add_type_specifier($1); $$ = $2; }
	| type_specifier { $$ = new ast::DeclarationSpecifiers(); $$->add_type_specifier($1); }
	| type_qualifier declaration_specifiers { $2->add_type_qualifier($1); $$ = $2; }
	| type_qualifier { $$ = new ast::DeclarationSpecifiers(); $$->add_type_qualifier($1); }
	| function_specifier declaration_specifiers { $2->add_func_specifier($1); $$ = $2; }
	| function_specifier { $$ = new ast::DeclarationSpecifiers(); $$->add_func_specifier($1); }
	;

init_declarator_list
	: init_declarator { $$ = new std::vector<ast::InitDeclarator*>(); $$->push_back($1); }
	| init_declarator_list ',' init_declarator { $1->push_back($3); $$ = $1; }
	;

init_declarator
	: pointer_list IDENTIFIER '=' assignment_expression { $$ = new ast::InitDeclarator($1, new ast::Identifier($2), $4); }
	| IDENTIFIER '=' assignment_expression { $$ = new ast::InitDeclarator(new ast::Identifier($1), $3); }
	| pointer_list IDENTIFIER { $$ = new ast::InitDeclarator($1, new ast::Identifier($2), nullptr); }
	| IDENTIFIER { $$ = new ast::InitDeclarator(new ast::Identifier($1), nullptr); }
	;

pointer_list
  : '*' pointer_list { $$++; }
  | '*' { $$ = 1; }
  ;

storage_class_specifier
	// : TYPEDEF { $$ = ast::SS_TYPEDEF; }
	: EXTERN { $$ = ast::SS_EXTERN; }
	| STATIC { $$ = ast::SS_STATIC; }
	| THREAD_LOCAL { $$ = ast::SS_THREADLOCAL; }
	| AUTO { $$ = ast::SS_AUTO; }
	| REGISTER { $$ = ast::SS_REGISTER; }
	;

type_specifier
	: VOID { $$ = ast::TS_VOID; }
	| CHAR { $$ = ast::TS_CHAR; }
	| SHORT { $$ = ast::TS_SHORT; }
	| INT { $$ = ast::TS_INT; }
	| LONG { $$ = ast::TS_LONG; }
	| FLOAT { $$ = ast::TS_FLOAT; }
	| DOUBLE { $$ = ast::TS_DOUBLE; }
	| SIGNED { $$ = ast::TS_SIGNED; }
	| UNSIGNED { $$ = ast::TS_UNSIGNED; }
	| BOOL { $$ = ast::TS_BOOL; }
	| COMPLEX { $$ = ast::TS_COMPLEX; }
	| IMAGINARY { $$ = ast::TS_IMAGINARY; }
	;

type_qualifier
	: CONST { $$ = ast::TQ_CONST; }
	| RESTRICT { $$ = ast::TQ_RESTRICT; }
	| VOLATILE { $$ = ast::TQ_VOLATILE; }
	| ATOMIC { $$ = ast::TQ_ATOMIC; }
	;

function_specifier
	: INLINE { $$ = ast::FS_INLINE; }
	| NORETURN { $$ = ast::FS_NORETURN; }
	;

function_parameter_list
	: parameter_list ',' ELLIPSIS { $$ = new ast::FunctionParameterList($1, true); }
	| parameter_list { $$ = new ast::FunctionParameterList($1, false); }
	;

parameter_list
	: pure_declaration { $$ = new std::vector<ast::PureDeclaration*>(); $$->push_back($1); } 
	| parameter_list ',' pure_declaration { $1->push_back($3); $$ = $1; } 
	;

pure_declaration
	: declaration_specifiers pointer_list IDENTIFIER { $$ = new ast::PureDeclaration($1, $2, new ast::Identifier($3)); } 
	| declaration_specifiers IDENTIFIER { $$ = new ast::PureDeclaration($1, 0, new ast::Identifier($2)); }
        ;

/* -Statements--------------------------------------------------------------- */

statement
	: labeled_statement { $$ = $1; }
  | declaration_statement { $$ = $1; }
	| compound_statement { $$ = $1; }
	| expression_statement { $$ = $1; }
	| selection_statement { $$ = $1; }
	| iteration_statement { $$ = $1; }
	| jump_statement { $$ = $1; }
	;

declaration_statement
  : declaration { $$ = new ast::DeclarationStatement($1); }

labeled_statement
	: IDENTIFIER ':' statement { $$ = new ast::LabeledStatement(new ast::Identifier($1), $3); }
	| CASE constant_expression ':' statement { $$ = new ast::CaseStatement($2, $4); }
	| DEFAULT ':' statement { $$ = $3; }
	;

compound_statement 
  : '{' block_item_list '}' { $$ = $2; }
  | '{' '}' { $$ = new ast::BlockStatement(); }
  ;

block_item_list 
  : statement                  { $$ = new ast::BlockStatement(); $$->push_back($1); }
  | block_item_list statement  { $1->push_back($2); $$ = $1; }

expression_statement
	: ';' { $$ = new ast::ExpressionStatement(nullptr); }
	| expression ';' { $$ = new ast::ExpressionStatement($1); }
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement { $$ = new ast::IfStatement($3, $5, $7); }
	| IF '(' expression ')' statement { $$ = new ast::IfStatement($3, $5, nullptr); }
	| SWITCH '(' expression ')' statement { $$ = new ast::SwitchStatement($3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement { $$ = new ast::WhileStatement($3, $5); }
	| DO statement WHILE '(' expression ')' ';' { $$ = new ast::DoWhileStatement($5, $2); }
	;

jump_statement
	: GOTO IDENTIFIER ';' { $$ = new ast::GotoStatement($2); }
	| CONTINUE ';' { $$ = new ast::ContinueStatement(); }
	| BREAK ';' { $$ = new ast::BreakStatement(); }
	| RETURN ';' { $$ = new ast::ReturnStatement(nullptr); }
	| RETURN expression ';' { $$ = new ast::ReturnStatement($2); }
	;

/* -Translation Unit and Functions------------------------------------------- */

/* TODO function declarations without definitions*/

translation_unit
	: function_definition { $$ = new ast::TranslationUnit(); $$->add_function($1); *tu = *$$; }
	| declaration { $$ = new ast::TranslationUnit(); $$->add_declaration(new ast::DeclarationStatement($1)); *tu = *$$; }
	| translation_unit function_definition { $1->add_function($2); $$ = $1; *tu = *$$; }
	| translation_unit declaration { $1->add_declaration(new ast::DeclarationStatement($2)); $$ = $1; *tu = *$$; }
	;

function_definition
  : pure_declaration '(' function_parameter_list ')' compound_statement { $$ = new ast::Function($1, $3, $5); }
  | pure_declaration '(' ')' compound_statement { $$ = new ast::Function($1, nullptr, $4); }
  | pure_declaration '(' VOID ')' compound_statement { $$ = new ast::Function($1, nullptr, $5); }
  ;


%%
#include <stdio.h>

void yyerror(ast::TranslationUnit* tu, const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
}
