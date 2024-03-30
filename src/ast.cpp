#include "ast.hpp"

namespace ast {

string op2str(Operator op) {
   switch (op) {
       case OP_ADD: return "OP_ADD";
       case OP_SUB: return "OP_SUB";
       case OP_MUL: return "OP_MUL";
       case OP_DIV: return "OP_DIV";
       case OP_MOD: return "OP_MOD";
       case OP_UNARY_PLUS: return "OP_UNARY_PLUS";
       case OP_UNARY_MINUS: return "OP_UNARY_MINUS";
       case OP_PRE_INCR: return "OP_PRE_INCR";
       case OP_PRE_DECR: return "OP_PRE_DECR";
       case OP_POST_INCR: return "OP_POST_INCR";
       case OP_POST_DECR: return "OP_POST_DECR";
       case OP_DEREF: return "OP_DEREF";
       case OP_SIZEOF: return "OP_SIZEOF";
       case OP_ALIGNOF: return "OP_ALIGNOF";
       case OP_TYPECAST: return "OP_TYPECAST";
       case OP_BOOL_AND: return "OP_BOOL_AND";
       case OP_BOOL_OR: return "OP_BOOL_OR";
       case OP_BOOL_NOT: return "OP_BOOL_NOT";
       case OP_EQ: return "OP_EQ";
       case OP_GE: return "OP_GE";
       case OP_GT: return "OP_GT";
       case OP_LT: return "OP_LT";
       case OP_LE: return "OP_LE";
       case OP_NE: return "OP_NE";
       case OP_AND: return "OP_AND";
       case OP_OR: return "OP_OR";
       case OP_XOR: return "OP_XOR";
       case OP_NOT: return "OP_NOT";
       case OP_LSHIFT: return "OP_LSHIFT";
       case OP_RSHIFT: return "OP_RSHIFT";
       case OP_ASSIGN: return "OP_ASSIGN";
       case OP_MUL_ASSIGN: return "OP_MUL_ASSIGN";
       case OP_DIV_ASSIGN: return "OP_DIV_ASSIGN";
       case OP_MOD_ASSIGN: return "OP_MOD_ASSIGN";
       case OP_ADD_ASSIGN: return "OP_ADD_ASSIGN";
       case OP_SUB_ASSIGN: return "OP_SUB_ASSIGN";
       case OP_LEFT_ASSIGN: return "OP_LEFT_ASSIGN";
       case OP_RIGHT_ASSIGN: return "OP_RIGHT_ASSIGN";
       case OP_AND_ASSIGN: return "OP_AND_ASSIGN";
       case OP_XOR_ASSIGN: return "OP_XOR_ASSIGN";
       case OP_OR_ASSIGN: return "OP_OR_ASSIGN";
       case OP_SEQ: return "OP_SEQ";
       default: return "UNKNOWN_OPERATOR";
   }
}

Type::Type(string _name) : name(_name) {}

Identifier::Identifier(string _name) : name(_name), location(-1) {}

TernaryExpression::TernaryExpression(Expression *_cond,
                                     Expression *_true_branch,
                                     Expression *_false_branch)
    : cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {}

TernaryExpression::~TernaryExpression() {
  delete cond;
  delete true_branch;
  delete false_branch;
}

TypecastExpression::TypecastExpression(Type *_typ, Expression *_expr)
    : typ(_typ), expr(_expr) {}

TypecastExpression::~TypecastExpression() {
  delete typ;
  delete expr;
}

FunctionInvocationExpression::FunctionInvocationExpression(Expression *_fn)
    : fn(_fn) {}

FunctionInvocationExpression::FunctionInvocationExpression(
    Expression *_fn, vector<Expression *> *_params)
    : fn(_fn), params(_params) {}

FunctionInvocationExpression::~FunctionInvocationExpression() {
  delete fn;
  for (auto expr : *params) {
    delete expr;
  }
  delete params;
}

TypeExpression::TypeExpression(Operator _op, Type *_typ) : op(_op), typ(_typ) {}

TypeExpression::~TypeExpression() { delete typ; }

BinaryExpression::BinaryExpression(Expression *_lhs, Operator _op,
                                   Expression *_rhs)
    : lhs(_lhs), op(_op), rhs(_rhs) {}

BinaryExpression::~BinaryExpression() {
  delete lhs;
  delete rhs;
}

UnaryExpression::UnaryExpression(Operator _op, Expression *_expr)
    : op(_op), expr(_expr) {}

Literal::Literal(string _value, LiteralType _ltype)
    : value(_value), ltype(_ltype) {
  // TODO parse literal to int/float etc
}

ExpressionStatement::ExpressionStatement(Expression *_expr) : expr(_expr) {}

ExpressionStatement::~ExpressionStatement() { delete expr; }

IfStatement::IfStatement(Expression *_cond, Statement *_true_branch,
                         Statement *_false_branch)
    : cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {}

IfStatement::~IfStatement() {
  delete cond;
  delete true_branch;
  delete false_branch;
}

SwitchStatement::~SwitchStatement() {
  delete expr;
  delete stmt;
}

WhileStatement::WhileStatement(Expression *_cond, Statement *_stmt)
    : cond(_cond), stmt(_stmt) {}

WhileStatement::~WhileStatement() {
  delete cond;
  delete stmt;
}

DoWhileStatement::DoWhileStatement(Expression *_cond, Statement *_stmt)
    : cond(_cond), stmt(_stmt) {}

DoWhileStatement::~DoWhileStatement() {
  delete cond;
  delete stmt;
}

ReturnStatement::ReturnStatement(Expression *_ret_expr) : ret_expr(_ret_expr) {}

ReturnStatement::~ReturnStatement() { delete ret_expr; }

GotoStatement::GotoStatement(char *_label) : label(_label) {}

DeclarationSpecifiers::DeclarationSpecifiers()
    : storage_specs(), type{TS_UNDEFINED}, type_quals(), func_specs() {}

void DeclarationSpecifiers::set_type(TypeSpecifier ts) {
  if (type != TS_UNDEFINED) {
    cout << "ERROR: redeclaration of type not allowed" << endl;
  }
  type = ts;
}

void DeclarationSpecifiers::add_storage_specifier(StorageSpecifier ss) {
  storage_specs.insert(ss);
}

void DeclarationSpecifiers::add_type_qualifier(TypeQualifier tq) {
  type_quals.insert(tq);
}

void DeclarationSpecifiers::add_func_specifier(FunctionSpecifier fs) {
  func_specs.insert(fs);
}

Declarator::Declarator(int _ptr_depth, Identifier *_ident)
    : ptr_depth{_ptr_depth}, ident{_ident} {}

Declarator::Declarator(Identifier *_ident) : ptr_depth{0}, ident{_ident} {}

PureDeclaration::PureDeclaration(DeclarationSpecifiers *_decl_specs,
                                 int _ptr_depth, Identifier *_ident)
    : decl_specs{_decl_specs}, ptr_depth{_ptr_depth}, ident{_ident} {}

FunctionParameterList::FunctionParameterList(bool _has_varargs)
    : has_varargs{_has_varargs} {}

InitDeclarator::InitDeclarator(int _ptr_depth, Identifier *_ident,
                               Expression *_init_expr)
    : ptr_depth{_ptr_depth}, ident{_ident}, init_expr{_init_expr} {}

InitDeclarator::InitDeclarator(Identifier *_ident, Expression *_init_expr)
    : ptr_depth{0}, ident{_ident}, init_expr{_init_expr} {}

Declaration::Declaration(DeclarationSpecifiers *_decl_specs,
                         vector<InitDeclarator *> *_decl_list)
    : decl_specs(_decl_specs), decl_list(_decl_list) {}

Declaration::~Declaration() { delete typ; }

DeclarationStatement::DeclarationStatement(Declaration *_decl) : decl(_decl) {}

DeclarationStatement::~DeclarationStatement() { delete decl; }

void BlockStatement::scopify(symboltable *s, int *new_location) {
  for (auto stmt : *this) {
    stmt->scopify(s, new_location);
  }
}

BlockStatement::~BlockStatement() {
  for (auto stmt : *this) {
    delete stmt;
  }
}

LabeledStatement::LabeledStatement(char *_label, Statement *_stmt)
    : label(_label), stmt{_stmt} {}

CaseStatement::CaseStatement(Expression *_const_expr, Statement *_stmt)
    : const_expr{_const_expr}, stmt{_stmt} {}

Parameter::Parameter(Type *_typ, string _name)
    : typ(_typ), name(_name), location(-1) {}

Parameter::~Parameter() { delete typ; }

Function::Function(PureDeclaration *_func_decl, FunctionParameterList *_params,
                   BlockStatement *_stmts)
    : func_decl{_func_decl}, params{_params}, stmts{_stmts} {}

Function::~Function() {
  delete func_decl;
  if (params) {
    for (auto param : *params) {
      delete param;
    }
    delete params;
  }
  delete stmts;
}

TranslationUnit::TranslationUnit()
    : functions(new vector<Function *>),
      decls(new vector<DeclarationStatement *>), is_decl(new vector<bool>) {}

void TranslationUnit::add_function(Function *func) {
  functions->push_back(func);
  is_decl->push_back(false);
}

void TranslationUnit::add_declaration(DeclarationStatement *decl) {
  decls->push_back(decl);
  is_decl->push_back(true);
}

TranslationUnit::~TranslationUnit() {
  delete is_decl;
  for (auto decl : *decls) {
    delete decl;
  }

  delete decls;

  for (auto func : *functions) {
    delete func;
  }

  delete functions;
}

} // namespace ast
