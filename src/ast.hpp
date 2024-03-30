#pragma once

#include "symtab.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <set>

using namespace std;

namespace ast {
enum Operator {
  // arithmetic
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_UNARY_PLUS,
  OP_UNARY_MINUS,
  OP_PRE_INCR,
  OP_PRE_DECR,
  OP_POST_INCR,
  OP_POST_DECR,

  // types
  OP_DEREF,
  OP_SIZEOF,
  OP_ALIGNOF,
  OP_TYPECAST,

  // logical
  OP_BOOL_AND,
  OP_BOOL_OR,
  OP_BOOL_NOT,

  // relational
  OP_EQ,
  OP_GE,
  OP_GT,
  OP_LT,
  OP_LE,
  OP_NE,

  // bitwise
  OP_AND,
  OP_OR,
  OP_XOR,
  OP_NOT,
  OP_LSHIFT,
  OP_RSHIFT,

  // assignment
  OP_ASSIGN,
  OP_MUL_ASSIGN,
  OP_DIV_ASSIGN,
  OP_MOD_ASSIGN,
  OP_ADD_ASSIGN,
  OP_SUB_ASSIGN,
  OP_LEFT_ASSIGN,
  OP_RIGHT_ASSIGN,
  OP_AND_ASSIGN,
  OP_XOR_ASSIGN,
  OP_OR_ASSIGN,

  // other
  OP_SEQ
};

enum LiteralType {
  LT_INT64,
  LT_UINT64,
  LT_INT32,
  LT_FLOAT,
  LT_DOUBLE,
  LT_INT_LIKE,
  LT_FLOAT_LIKE,
  LT_STRING
};

enum StorageSpecifier {
  SS_EXTERN,
  SS_STATIC,
  SS_THREADLOCAL,
  SS_AUTO,
  SS_REGISTER
};

enum TypeSpecifier {
  TS_VOID,
  TS_CHAR,
  TS_SHORT,
  TS_INT,
  TS_LONG,
  TS_FLOAT,
  TS_DOUBLE,
  TS_SIGNED,
  TS_UNSIGNED,
  TS_BOOL,
  TS_COMPLEX,
  TS_IMAGINARY,
  TS_STRUCT,
  TS_UNION,
  TS_UNDEFINED
};

enum TypeQualifier {
  TQ_CONST,
  TQ_RESTRICT,
  TQ_VOLATILE,
  TQ_ATOMIC
};

enum FunctionSpecifier {
  FS_INLINE,
  FS_NORETURN
};

string op2str(Operator op);

struct Node {
  virtual string dump_ast(string prefix) = 0;
  virtual void scopify(symboltable *s, int *new_location) = 0;
};

struct Type : Node {
  string name;

  Type(string _name);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
};

struct Expression : Node {
  virtual ~Expression() {}
};

struct Identifier : Expression {
  string name;
  int location;

  Identifier(string _name);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
};

struct TernaryExpression : Expression {
  Expression *cond;
  Expression *true_branch;
  Expression *false_branch;

  TernaryExpression(Expression *_cond, Expression *_true_branch,
                    Expression *_false_branch);

  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~TernaryExpression();
};

struct TypecastExpression : Expression {
  Type *typ;
  Expression *expr;

  TypecastExpression(Type *_typ, Expression *_expr);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~TypecastExpression();
};

struct FunctionInvocationExpression : Expression {

  Expression *fn;
  vector<Expression *> *params;

  FunctionInvocationExpression(Expression *_fn);
  FunctionInvocationExpression(Expression *_fn, vector<Expression *> *_params);

  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~FunctionInvocationExpression();
};

struct TypeExpression : Expression {
  Operator op;
  Type *typ;

  TypeExpression(Operator _op, Type *_typ);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~TypeExpression();
};

struct BinaryExpression : Expression {
  Expression *lhs;
  Operator op;
  Expression *rhs;

  BinaryExpression(Expression *_lhs, Operator _op, Expression *_rhs);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~BinaryExpression();
};

struct UnaryExpression : Expression {
  Operator op;
  Expression *expr;

  UnaryExpression(Operator _op, Expression *_expr);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
};

struct Literal : Expression {
  string value;
  LiteralType ltype;

  Literal(string _value, LiteralType _ltype);
  void scopify(symboltable *s, int *new_location);
  string dump_ast(string prefix);
};

struct Statement : Node {
  virtual ~Statement() {}
};

struct ExpressionStatement : Statement {
  Expression *expr;

  ExpressionStatement(Expression *_expr);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~ExpressionStatement();
};

struct IfStatement : Statement {

  Expression *cond;
  Statement *true_branch;
  Statement *false_branch;

  IfStatement(Expression *_cond, Statement *_true_branch,
              Statement *_false_branch);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~IfStatement();
};

struct SwitchStatement : Statement {

  Expression *expr;
  Statement *stmt;

  SwitchStatement(Expression* _expr, Statement* _stmt);
  ~SwitchStatement();
};

struct WhileStatement : Statement {

  Expression *cond;
  Statement *stmt;

  WhileStatement(Expression *_cond, Statement *_stmt);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~WhileStatement();
};

struct DoWhileStatement : Statement {

  Expression *cond;
  Statement *stmt;

  DoWhileStatement(Expression *_cond, Statement *_stmt);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~DoWhileStatement();
};

struct ReturnStatement : Statement {

  Expression *ret_expr;

  ReturnStatement(Expression *_ret_expr);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~ReturnStatement();
};

struct GotoStatement : Statement {
  string label;
  GotoStatement(char* _label);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
};

struct ContinueStatement : Statement {
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
};

struct BreakStatement : Statement {

  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
};

struct DeclarationSpecifiers : Node {
  std::set<StorageSpecifier> storage_specs;
  TypeSpecifier type;
  std::set<TypeQualifier> type_quals;
  std::set<FunctionSpecifier> func_specs;

  DeclarationSpecifiers();

  void set_type(TypeSpecifier ts);
  void add_storage_specifier(StorageSpecifier ss);
  void add_type_qualifier(TypeQualifier tq);
  void add_func_specifier(FunctionSpecifier fs);

  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
};

struct Declarator : Node {
  int ptr_depth;
  Identifier *ident;

  Declarator(int _ptr_depth, Identifier *_ident);
  Declarator(Identifier *_ident);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~Declarator();
};

struct PureDeclaration : Node {
  DeclarationSpecifiers* decl_specs;
  int ptr_depth;
  Identifier *ident;

  PureDeclaration(DeclarationSpecifiers* _decl_specs, int _ptr_depth, Identifier *_ident);

  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~PureDeclaration();
};

struct FunctionParameterList : Node, std::vector<PureDeclaration*> {

  bool has_varargs;

  FunctionParameterList(bool _has_varargs);
};

struct InitDeclarator : Node {
  int ptr_depth;
  Identifier *ident;
  Expression *init_expr;

  InitDeclarator(int _ptr_depth, Identifier *_ident, Expression *_init_expr);
  InitDeclarator(Identifier *_ident, Expression *_init_expr);

  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~InitDeclarator();
};

struct Declaration : Node {
  Type *typ;
  Identifier *Ident;

  DeclarationSpecifiers* decl_specs;
  std::vector<InitDeclarator*>* decl_list;

  Declaration(DeclarationSpecifiers* _decl_specs , vector<InitDeclarator*> *_decl_list);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~Declaration();
};

struct DeclarationStatement : Statement {
  Declaration *decl;

  DeclarationStatement(Declaration *_decl);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~DeclarationStatement();
};

struct BlockStatement : Statement, vector<Statement *> {

  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~BlockStatement();
};

struct LabeledStatement : Statement {

  string label;
  Statement* stmt;

  LabeledStatement(char* _label, Statement* _stmt);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~LabeledStatement();
};

struct CaseStatement : Statement {

  Expression* const_expr;
  Statement* stmt;

  CaseStatement(Expression* _const_expr, Statement* _stmt);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~CaseStatement();
};

struct Parameter : Node {
  Type *typ;
  string name;
  int location;

  Parameter(Type *_typ, string _name);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~Parameter();
};

struct Function : Node {

  PureDeclaration* func_decl;
  FunctionParameterList* params;
  BlockStatement* stmts;

  Function(PureDeclaration* _func_decl, FunctionParameterList* _params, BlockStatement* _stmts);
  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~Function();
};

struct TranslationUnit : Node {

  vector<Function *> *functions;
  vector<DeclarationStatement *> *decls;
  vector<bool> *is_decl;

  TranslationUnit();

  void add_function(Function *func);
  void add_declaration(DeclarationStatement *decl);

  string dump_ast(string prefix);
  void scopify(symboltable *s, int *new_location);
  ~TranslationUnit();
};
} // namespace ast
