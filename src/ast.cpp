#include "ast.hpp"
#include "debug.hpp"

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

string ss2str(StorageSpecifier ss) {
  switch(ss) {
    case SS_EXTERN: return "SS_EXTERN";
    case SS_STATIC: return "SS_STATIC";
    case SS_THREADLOCAL: return "SS_THREADLOCAL";
    case SS_AUTO: return "SS_AUTO";
    case SS_REGISTER: return "SS_REGISTER";
  }
}

string ts2str(TypeSpecifier ts) {
  switch(ts) {
    case TS_VOID: return "TS_VOID";
    case TS_CHAR: return "TS_CHAR";
    case TS_SHORT: return "TS_SHORT";
    case TS_INT: return "TS_INT";
    case TS_LONG: return "TS_LONG";
    case TS_FLOAT: return "TS_FLOAT";
    case TS_DOUBLE: return "TS_DOUBLE";
    case TS_SIGNED: return "TS_SIGNED";
    case TS_UNSIGNED: return "TS_UNSIGNED";
    case TS_BOOL: return "TS_BOOL";
    case TS_COMPLEX: return "TS_COMPLEX";
    case TS_IMAGINARY: return "TS_IMAGINARY";
    case TS_STRUCT: return "TS_STRUCT";
    case TS_UNION: return "TS_UNION";
    case TS_UNDEFINED: return "TS_UNDEFINED";
  }
}

string tq2str(TypeQualifier tq) {
  switch(tq) {
    case TQ_CONST: return "TQ_CONST";
    case TQ_RESTRICT: return "TQ_RESTRICT";
    case TQ_VOLATILE: return "TQ_VOLATILE";
    case TQ_ATOMIC: return "TQ_ATOMIC";
  }
}

string fs2str(FunctionSpecifier fs) {
  switch(fs) {
    case FS_INLINE: return "FS_INLINE";
    case FS_NORETURN: return "FS_NORETURN";
  }
}

Identifier::Identifier(string _name) : name(_name) {    // is this fine? Jai
    cdebug << "Identifier constructor called with name: " << _name << endl;
}

TernaryExpression::TernaryExpression(Expression *_cond,
                                     Expression *_true_branch,
                                     Expression *_false_branch)
    : cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {
    cdebug << "TernaryExpression constructor called" << endl;
}

TernaryExpression::~TernaryExpression() {
    cdebug << "TernaryExpression destructor called" << endl;
    delete cond;
    delete true_branch;
    delete false_branch;
}

FunctionInvocationExpression::FunctionInvocationExpression(Expression *_fn)
    : fn(_fn) {
    cdebug << "FunctionInvocationExpression constructor called" << endl;
}

FunctionInvocationExpression::FunctionInvocationExpression(
    Expression *_fn, vector<Expression *> *_params)
    : fn(_fn), params(_params) {
    cdebug << "FunctionInvocationExpression constructor called with params" << endl;
}

FunctionInvocationExpression::~FunctionInvocationExpression() {
    cdebug << "FunctionInvocationExpression destructor called" << endl;
    delete fn;
    for (auto expr : *params) {
        delete expr;
    }
    delete params;
}

BinaryExpression::BinaryExpression(Expression *_lhs, Operator _op,
                                   Expression *_rhs)
    : lhs(_lhs), op(_op), rhs(_rhs) {
    cdebug << "BinaryExpression constructor called" << endl;
}

BinaryExpression::~BinaryExpression() {
    cdebug << "BinaryExpression destructor called" << endl;
    delete lhs;
    delete rhs;
}

UnaryExpression::UnaryExpression(Operator _op, Expression *_expr)
    : op(_op), expr(_expr) {
    cdebug << "UnaryExpression constructor called" << endl;
}

Literal::Literal(string _value, LiteralType _ltype)
    : value(_value), ltype(_ltype) {
    cdebug << "Literal constructor called with value: " << _value << endl;
    // TODO parse literal to int/float etc
}

ExpressionStatement::ExpressionStatement(Expression *_expr) : expr(_expr) {
    cdebug << "ExpressionStatement constructor called" << endl;
}

ExpressionStatement::~ExpressionStatement() {
    cdebug << "ExpressionStatement destructor called" << endl;
    delete expr;
}

IfStatement::IfStatement(Expression *_cond, Statement *_true_branch,
                         Statement *_false_branch)
    : cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {
    cdebug << "IfStatement constructor called" << endl;
}

IfStatement::~IfStatement() {
    cdebug << "IfStatement destructor called" << endl;
    delete cond;
    delete true_branch;
    delete false_branch;
}

SwitchStatement::~SwitchStatement() {
    cdebug << "SwitchStatement destructor called" << endl;
    delete expr;
    delete stmt;
}

WhileStatement::WhileStatement(Expression *_cond, Statement *_stmt)
    : cond(_cond), stmt(_stmt) {
    cdebug << "WhileStatement constructor called" << endl;
}

WhileStatement::~WhileStatement() {
    cdebug << "WhileStatement destructor called" << endl;
    delete cond;
    delete stmt;
}

DoWhileStatement::DoWhileStatement(Expression *_cond, Statement *_stmt)
    : cond(_cond), stmt(_stmt) {
    cdebug << "DoWhileStatement constructor called" << endl;
}

DoWhileStatement::~DoWhileStatement() {
    cdebug << "DoWhileStatement destructor called" << endl;
    delete cond;
    delete stmt;
}

ReturnStatement::ReturnStatement(Expression *_ret_expr) : ret_expr(_ret_expr) {
    cdebug << "ReturnStatement constructor called" << endl;
}

ReturnStatement::~ReturnStatement() {
    cdebug << "ReturnStatement destructor called" << endl;
    delete ret_expr;
}

GotoStatement::GotoStatement(char *_label) : label(_label) {
    cdebug << "GotoStatement constructor called" << endl;
}

DeclarationSpecifiers::DeclarationSpecifiers()
    : storage_specs(), type_specs{}, type_quals(), func_specs() {
    cdebug << "DeclarationSpecifiers constructor called" << endl;
}

void DeclarationSpecifiers::add_type_specifier(TypeSpecifier ts) {
    // FLOAT cannot be combined with unsigned/signed
    if (ts == TS_FLOAT || ts == TS_DOUBLE) {
        if (!type_specs.empty()) {
            cout << "ERROR: cannot combine " << ts2str(ts) << " with previous decls\n";
            return;
        }
    }
    else if (type_specs.find(TS_FLOAT) != type_specs.end() || type_specs.find(TS_DOUBLE) != type_specs.end()) {
        cout << "ERROR: cannot combine a type specifier with floating point types\n";
        return;
    }

    if (ts == TS_CHAR) {
        if (type_specs.size() == 1 && !(*type_specs.begin() == TS_SIGNED || *type_specs.begin() == TS_UNSIGNED)) {
            cout << "ERROR: cannot combine char with previous decls\n";
            return;
        }
        else if (type_specs.size() > 1) {
            cout << "ERROR: cannot combine char with previous decls\n";
            return;
        }
    }
    else if (ts == TS_SIGNED || ts == TS_UNSIGNED) {
        if ((ts == TS_SIGNED && type_specs.find(TS_UNSIGNED) != type_specs.end()) ||
            (ts == TS_UNSIGNED && type_specs.find(TS_SIGNED) != type_specs.end())) {
            cout << "ERROR: cannot combine signed with unsigned\n";
            return;
        }
    }
    else if (ts == TS_SHORT) {
        if (type_specs.find(TS_CHAR) != type_specs.end() || type_specs.find(TS_LONG) != type_specs.end()) {
            cout << "ERROR: cannot combine short with char, long or llong\n";
            return;
        }
    }
    else if (ts == TS_LONG) {
        if (type_specs.find(TS_CHAR) != type_specs.end() || type_specs.find(TS_SHORT) != type_specs.end()) {
            cout << "ERROR: cannot combine long with char or short\n";
            return;
        }
    }
    else if (ts == TS_INT) {
        if (type_specs.find(TS_CHAR) != type_specs.end()) {
            cout << "ERROR: cannot combine int with char\n";
        }
    }
    type_specs.insert(ts);
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

PureDeclaration::PureDeclaration(DeclarationSpecifiers *_decl_specs,
                                 int _ptr_depth, Identifier *_ident)
    : decl_specs{_decl_specs}, ptr_depth{_ptr_depth}, ident{_ident} {
    cdebug << "PureDeclaration constructor called" << endl;
}

FunctionParameterList::FunctionParameterList(std::vector<PureDeclaration*>* _params, bool _has_varargs)
    : params{_params}, has_varargs{_has_varargs} {
    cdebug << "FunctionParameterList constructor called" << endl;
}

InitDeclarator::InitDeclarator(int _ptr_depth, Identifier *_ident,
                               Expression *_init_expr)
    : ptr_depth{_ptr_depth}, ident{_ident}, init_expr{_init_expr} {
    cdebug << "InitDeclarator constructor called" << endl;
}

InitDeclarator::InitDeclarator(Identifier *_ident, Expression *_init_expr)
    : ptr_depth{0}, ident{_ident}, init_expr{_init_expr} {
    cdebug << "InitDeclarator constructor called" << endl;
}

Declaration::Declaration(DeclarationSpecifiers *_decl_specs,
                         vector<InitDeclarator *> *_decl_list)
    : decl_specs(_decl_specs), decl_list(_decl_list) {
    cdebug << "Declaration constructor called" << endl;
}

Declaration::~Declaration() {
    cdebug << "Declaration destructor called" << endl;
    // TODO delete stuff
}

DeclarationStatement::DeclarationStatement(Declaration *_decl) : decl(_decl) {
    cdebug << "DeclarationStatement constructor called" << endl;
}

DeclarationStatement::~DeclarationStatement() {
    cdebug << "DeclarationStatement destructor called" << endl;
    delete decl;
}

BlockStatement::~BlockStatement() {
    cdebug << "BlockStatement destructor called" << endl;
    for (auto stmt : *this) {
        delete stmt;
    }
}

LabeledStatement::LabeledStatement(Identifier *_label, Statement *_stmt)
    : label{_label}, stmt{_stmt} {
    cdebug << "LabeledStatement constructor called" << endl;
}

LabeledStatement::~LabeledStatement() {
    cdebug << "LabeledStatement destructor called" << endl;
    delete label;
    delete stmt;
}

CaseStatement::CaseStatement(Expression *_const_expr, Statement *_stmt)
    : const_expr{_const_expr}, stmt{_stmt} {
    cdebug << "CaseStatement constructor called" << endl;
}

CaseStatement::~CaseStatement() {
    cdebug << "CaseStatement destructor called" << endl;
    delete const_expr;
    delete stmt;
}

PureDeclaration::~PureDeclaration() {
    cdebug << "PureDeclaration destructor called" << endl;
    delete decl_specs;
    delete ident;
}

SwitchStatement::SwitchStatement(Expression* _expr, Statement* _stmt)
    : expr{_expr}, stmt{_stmt} {
    cdebug << "SwitchStatement constructor called" << endl;
}

FunctionParameterList::~FunctionParameterList() {
    cdebug << "FunctionParameterList destructor called" << endl;
    for (auto pd : *params) {
        delete pd;
    }
}

Function::Function(PureDeclaration *_func_decl, FunctionParameterList *_params,
                   BlockStatement *_stmts)
    : func_decl{_func_decl}, params{_params}, stmts{_stmts} {
    cdebug << "Function constructor called" << endl;
}

Function::~Function() {
    cdebug << "Function destructor called" << endl;
    delete func_decl;
    delete params;
    delete stmts;
}

TranslationUnit::TranslationUnit()
    : nodes(new vector<Node *>) {
    cdebug << "TranslationUnit constructor called" << endl;
}

void TranslationUnit::add_function(Function *func) {
    nodes->push_back(func);
}

void TranslationUnit::add_declaration(DeclarationStatement *decl) {
    nodes->push_back(decl);
}

TranslationUnit::~TranslationUnit() {
    cdebug << "TranslationUnit destructor called" << endl;
    for (auto *node : *nodes) {
        delete node;
    }
    delete nodes;
}
} // namespace ast
