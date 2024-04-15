#pragma once

#include "symtab.hpp"
#include "llvm/IR/IRBuilder.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;
// using namespace llvm;

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
string ss2str(StorageSpecifier ss);
string ts2str(TypeSpecifier ts);
string tq2str(TypeQualifier tq);
string fs2str(FunctionSpecifier fs);

struct Node {
  virtual string dump_ast(string prefix) = 0;
  virtual void scopify() = 0;
};

////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////

struct ExprTypeInfo{
  SymbolInfo st;
  bool is_ref;
};

struct Expression : Node {
  virtual ~Expression() {}
  ExprTypeInfo type_info;
  virtual llvm::Value* codegen();                      // codegen when rvalue
  // virtual llvm::Value* assign(Expression* R);         // codegen when lvalue
  virtual llvm::Value* get_address();                                // use this to replace assign               
};

struct Identifier : Expression {
  string name;
  SymbolInfo ident_info;

  Identifier(string _name);
  string dump_ast(string prefix);
  llvm::Value* codegen() override;
  // llvm::Value* assign(Expression* R) override;
  llvm::Value* get_address() override;
  void scopify();
};

struct TernaryExpression : Expression {
  Expression *cond;
  Expression *true_branch;
  Expression *false_branch;

  TernaryExpression(Expression *_cond, Expression *_true_branch,
                    Expression *_false_branch);

  string dump_ast(string prefix);
  void scopify();
  ~TernaryExpression();
};

struct FunctionInvocationExpression : Expression {

  Expression *fn;
  vector<Expression *> *params;

  FunctionInvocationExpression(Expression *_fn);
  FunctionInvocationExpression(Expression *_fn, vector<Expression *> *_params);

  string dump_ast(string prefix);
  llvm::Value* codegen() override;
  void scopify();
  ~FunctionInvocationExpression();
};

struct BinaryExpression : Expression {
  Expression *lhs;
  Operator op;
  Expression *rhs;

  BinaryExpression(Expression *_lhs, Operator _op, Expression *_rhs);
  string dump_ast(string prefix);
  void scopify();
  llvm::Value* codegen() override;
  ~BinaryExpression();
};

struct UnaryExpression : Expression {
  Operator op;
  Expression *expr;

  UnaryExpression(Operator _op, Expression *_expr);
  llvm::Value* codegen() override;
  llvm::Value* get_address() override;
  string dump_ast(string prefix);                      // Add assign method
  void scopify();
};

struct Literal : Expression {
  string value;
  LiteralType ltype;

  Literal(string _value, LiteralType _ltype);
  void scopify();
  llvm::Value* codegen() override;
  string dump_ast(string prefix);
};

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

struct DeclarationSpecifiers : Node {
  std::set<StorageSpecifier> storage_specs;
  std::set<TypeSpecifier> type_specs;
  std::set<TypeQualifier> type_quals;
  std::set<FunctionSpecifier> func_specs;

  DeclarationSpecifiers();

  void add_type_specifier(TypeSpecifier ts);
  void add_storage_specifier(StorageSpecifier ss);
  void add_type_qualifier(TypeQualifier tq);
  void add_func_specifier(FunctionSpecifier fs);

  string dump_ast(string prefix);
  void scopify();
};

struct PureDeclaration : Node {
  DeclarationSpecifiers* decl_specs;
  int ptr_depth;
  Identifier *ident;

  PureDeclaration(DeclarationSpecifiers* _decl_specs, int _ptr_depth, Identifier *_ident);

  string dump_ast(string prefix);
  void scopify();
  ~PureDeclaration();
};

struct FunctionParameterList : Node {

  std::vector<PureDeclaration*>* params;
  bool has_varargs;

  FunctionParameterList(std::vector<PureDeclaration*>* _params, bool _has_varargs);
  string dump_ast(string prefix);
  void scopify();
  ~FunctionParameterList();
};

struct InitDeclarator : Node {
  int ptr_depth;
  Identifier *ident;
  Expression *init_expr;

  InitDeclarator(int _ptr_depth, Identifier *_ident, Expression *_init_expr);
  InitDeclarator(Identifier *_ident, Expression *_init_expr);

  string dump_ast(string prefix);
  void scopify();
  // Value* codegen();
  ~InitDeclarator();
};

struct Declaration : Node {
  DeclarationSpecifiers* decl_specs;
  std::vector<InitDeclarator*>* decl_list;

  Declaration(DeclarationSpecifiers* _decl_specs , vector<InitDeclarator*> *_decl_list);
  string dump_ast(string prefix);
  void scopify();
  llvm::Value* codegen();
  llvm::Value* globalgen();
  ~Declaration();
};

////////////////////////////////////////////////////////////////////////////////
// Statements
////////////////////////////////////////////////////////////////////////////////

struct Statement : Node {
  virtual llvm::Value* codegen();
  virtual ~Statement() {}
};

struct DeclarationStatement : Statement {
  Declaration *decl;

  DeclarationStatement(Declaration *_decl);
  string dump_ast(string prefix);
  void scopify();
  llvm::Value* codegen() override;
  llvm::Value* globalgen();
  ~DeclarationStatement();
};

struct ExpressionStatement : Statement {
  Expression *expr;

  ExpressionStatement(Expression *_expr);
  string dump_ast(string prefix);
  void scopify();
  llvm::Value* codegen() override;
  ~ExpressionStatement();
};

struct IfStatement : Statement {

  Expression *cond;
  Statement *true_branch;
  Statement *false_branch;

  IfStatement(Expression *_cond, Statement *_true_branch,
              Statement *_false_branch);
  string dump_ast(string prefix);
  void scopify();
  llvm::Value* codegen() override;
  ~IfStatement();
};

struct SwitchStatement : Statement {

  Expression *expr;
  Statement *stmt;

  SwitchStatement(Expression* _expr, Statement* _stmt);
  string dump_ast(string prefix);
  void scopify();
  // llvm::Value* codegen();
  ~SwitchStatement();
};

struct WhileStatement : Statement {

  Expression *cond;
  Statement *stmt;

  WhileStatement(Expression *_cond, Statement *_stmt);
  string dump_ast(string prefix);
  void scopify();
  llvm::Value* codegen() override;
  ~WhileStatement();
};

struct DoWhileStatement : Statement {

  Expression *cond;
  Statement *stmt;

  DoWhileStatement(Expression *_cond, Statement *_stmt);
  string dump_ast(string prefix);
  void scopify();
  // llvm::Value* codegen();
  ~DoWhileStatement();
};

struct ReturnStatement : Statement {

  Expression *ret_expr;

  ReturnStatement(Expression *_ret_expr);
  string dump_ast(string prefix);
  void scopify();
  llvm::Value* codegen() override;
  ~ReturnStatement();
};

struct GotoStatement : Statement {
  string label;
  GotoStatement(char* _label);
  string dump_ast(string prefix);
  // llvm::Value* codegen();
  void scopify();
};

struct ContinueStatement : Statement {
  string dump_ast(string prefix);
  void scopify();
  // llvm::Value* codegen();
};

struct BreakStatement : Statement {

  string dump_ast(string prefix);
  void scopify();
  // llvm::Value* codegen();
};

struct BlockStatement : Statement, vector<Statement *> {

  string dump_ast(string prefix);
  void scopify();

  llvm::Value* codegen() override;
  ~BlockStatement();
};

struct LabeledStatement : Statement {

  Identifier* label;
  Statement* stmt;

  LabeledStatement(Identifier* _label, Statement* _stmt);
  string dump_ast(string prefix);
  // llvm::Value* codegen();
  void scopify();
  ~LabeledStatement();
};

struct CaseStatement : Statement {

  Expression* const_expr;
  Statement* stmt;

  CaseStatement(Expression* _const_expr, Statement* _stmt);
  string dump_ast(string prefix);
  // llvm::Value* codegen();
  void scopify();
  ~CaseStatement();
};

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

struct Function : Node {

  PureDeclaration* func_decl;
  FunctionParameterList* params;
  BlockStatement* stmts;

  Function(PureDeclaration* _func_decl, FunctionParameterList* _params, BlockStatement* _stmts);
  string dump_ast(string prefix);
  void scopify();
  llvm::Function* codegen(); 
  ~Function();
};

struct TranslationUnit : Node {

  vector<Node*> *nodes;

  TranslationUnit();

  void add_function(Function *func);
  void add_declaration(DeclarationStatement *decl);

  string dump_ast(string prefix);
  void scopify();
  void codegen();
  ~TranslationUnit();
};
} // namespace ast
