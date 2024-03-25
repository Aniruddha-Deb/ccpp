#pragma once

#include "symtab.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

string op2str(Operator op);

struct Node {
  virtual string dump_ast(string prefix) = 0;
  virtual void scopify(symboltable *s, int *new_location) = 0;
};

struct Type : Node {
  string name;

  Type(string _name) : name(_name) {}

  string dump_ast(string prefix) { return name; }

  void scopify(symboltable *s, int *new_location) { return; }

  ~Type() { return; }
};

struct Expression : Node {
  virtual ~Expression() {}
};

struct Identifier : Expression {
  string name;
  // TODO add symbol table references here
  int location;

  Identifier(string _name) : name(_name), location(-1) {}

  string dump_ast(string prefix) { return name + "[" + to_string(location) + "]" ; }

  void scopify(symboltable *s, int *new_location) {
    location = s->find_symbol(name);
  }
};

// there's only one kind of ternary expression (the conditional), so no need to
// specify operators
struct TernaryExpression : Expression {
  Expression *cond;
  Expression *true_branch;
  Expression *false_branch;

  TernaryExpression(Expression *_cond, Expression *_true_branch,
                    Expression *_false_branch)
      : cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {}

  string dump_ast(string prefix) {
    return "ternary_op\n" + prefix +
           "`- cond: " + cond->dump_ast(prefix + "|  ") + "\n" + prefix +
           "`- true_branch: " + true_branch->dump_ast(prefix + "|  ") + "\n" +
           prefix +
           "`- false_branch: " + false_branch->dump_ast(prefix + "   ");
  }

  void scopify(symboltable *s, int *new_location) {
    cond->scopify(s, new_location);
    true_branch->scopify(s, new_location);
    false_branch->scopify(s, new_location);
  }
  ~TernaryExpression() {
    delete cond;
    delete true_branch;
    delete false_branch;
  }
};

struct TypecastExpression : Expression {
  Type *typ;
  Expression *expr;

  TypecastExpression(Type *_typ, Expression *_expr) : typ(_typ), expr(_expr) {}

  string dump_ast(string prefix) {
    return "typecast\n" + prefix + "`- type: " + typ->dump_ast(prefix + "|  ") +
           "\n" + prefix + "`- expr: " + expr->dump_ast(prefix + "   ");
  }

  void scopify(symboltable *s, int *new_location) {
    // typ.scopify();
    expr->scopify(s, new_location);
  }

  ~TypecastExpression() {
    delete typ;
    delete expr;
  }
};

struct FunctionInvocationExpression : Expression {

  Expression *fn; // since a function invocation need not be a string
                  // eg arr_fn_ptr[0](foo);
  vector<Expression *> *params;

  FunctionInvocationExpression(Expression *_fn) : fn(_fn) {}
  FunctionInvocationExpression(Expression *_fn, vector<Expression *> *_params)
      : fn(_fn), params(_params) {}

  string dump_ast(string prefix) {
    string result =
        "fn_call\n" + prefix + "`- fn: " + fn->dump_ast(prefix + "|  ");
    if (params) {
      string param_str = "params:\n";
      int i = 0;
      auto param = (*params)[i];
      while (i < params->size() - 1) {
        param_str +=
            prefix + "   " + "`- " + param->dump_ast(prefix + "   |  ") + "\n";
        i++;
        param = (*params)[i];
      }
      param_str += prefix + "   " + "`- " + param->dump_ast(prefix + "      ");
      result += "\n" + prefix + "`- " + param_str;
    }
    return result;
  }

  void scopify(symboltable *s, int *new_location) {
    fn->scopify(s, new_location);
    if (params) {
      for (int i = 0; i < params->size(); i++) {
        (*params)[i]->scopify(s, new_location);
      }
    }
  }

  ~FunctionInvocationExpression() {
    delete fn;
    for (auto expr : *params) {
      delete expr;
    }
    delete params;
  }
};

struct TypeExpression : Expression {
  Operator op;
  Type *typ;

  TypeExpression(Operator _op, Type *_typ) : op(_op), typ(_typ) {}

  string dump_ast(string prefix) {
    return op2str(op) + "\n" + prefix +
           "`- type: " + typ->dump_ast(prefix + "   ");
  }

  void scopify(symboltable *s, int *new_location) { return; }

  ~TypeExpression() { delete typ; }
};

struct BinaryExpression : Expression {
  Expression *lhs;
  Operator op;
  Expression *rhs;

  BinaryExpression(Expression *_lhs, Operator _op, Expression *_rhs)
      : lhs(_lhs), op(_op), rhs(_rhs) {}

  string dump_ast(string prefix) {
    return op2str(op) + "\n" + prefix +
           "`- lhs: " + lhs->dump_ast(prefix + "|  ") + "\n" + prefix +
           "`- rhs: " + rhs->dump_ast(prefix + "   ");
  }

  void scopify(symboltable *s, int *new_location) {
    lhs->scopify(s, new_location);
    rhs->scopify(s, new_location);
  }

  ~BinaryExpression() {
    delete lhs;
    delete rhs;
  }
};

struct UnaryExpression : Expression {
  Operator op;
  Expression *expr;

  UnaryExpression(Operator _op, Expression *_expr) : op(_op), expr(_expr) {}

  string dump_ast(string prefix) {
    return op2str(op) + "\n" + prefix +
           "`- expr: " + expr->dump_ast(prefix + "   ");
  }

  void scopify(symboltable *s, int *new_location) {
    expr->scopify(s, new_location);
  }
};

struct Literal : Expression {
  string value;
  LiteralType ltype;

  Literal(string _value, LiteralType _ltype) : value(_value), ltype(_ltype) {
    // TODO parse literal to int/float etc
  }

  void scopify(symboltable *s, int *new_location) { return; }

  string dump_ast(string prefix) { return "literal (" + value + ")"; }
};

struct Statement : Node {
  virtual ~Statement() {}
};

struct ExpressionStatement : Statement {
  Expression *expr;

  ExpressionStatement(Expression *_expr) : expr(_expr) {}

  string dump_ast(string prefix) { return "expr: " + expr->dump_ast(prefix); }

  void scopify(symboltable *s, int *new_location) {
    expr->scopify(s, new_location);
  }

  ~ExpressionStatement() { delete expr; }
};

struct IfStatement : Statement {

  Expression *cond;
  Statement *true_branch;
  Statement *false_branch;

  IfStatement(Expression *_cond, Statement *_true_branch,
              Statement *_false_branch)
      : cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {}

  string dump_ast(string prefix) {
    string retval =
        "if\n" + prefix + "`- cond: " + cond->dump_ast(prefix + "|  ") + "\n";
    if (false_branch) {
      retval += prefix +
                "`- true_branch: " + true_branch->dump_ast(prefix + "|  ") +
                "\n";
      retval +=
          prefix + "`- false_branch: " + false_branch->dump_ast(prefix + "   ");
    } else {
      retval +=
          prefix + "`- true_branch: " + true_branch->dump_ast(prefix + "   ");
    }
    return retval;
  }

  void scopify(symboltable *s, int *new_location) {
    cond->scopify(s, new_location);
    s->enter_scope();
    true_branch->scopify(s, new_location);
    s->exit_scope();
    if (false_branch) {
      s->enter_scope();
      false_branch->scopify(s, new_location);
      s->exit_scope();
    }
    // TODO ARE TRUE AND FALSE BRANCHES IN SEPERATE SCOPES??
  }

  ~IfStatement() {
    delete cond;
    delete true_branch;
    delete false_branch;
  }
};

struct WhileStatement : Statement {

  Expression *cond;
  Statement *stmt;

  WhileStatement(Expression *_cond, Statement *_stmt)
      : cond(_cond), stmt(_stmt) {}

  string dump_ast(string prefix) {
    return "while\n" + prefix + "`- cond: " + cond->dump_ast(prefix + "|  ") +
           "\n" + prefix + "`- stmt: " + stmt->dump_ast(prefix + "   ");
  }

  void scopify(symboltable *s, int *new_location) {
    cond->scopify(s, new_location);
    s->enter_scope();
    stmt->scopify(s, new_location);
    s->exit_scope();
  }

  ~WhileStatement() {
    delete cond;
    delete stmt;
  }
};

struct ReturnStatement : Statement {

  Expression *ret_expr;

  ReturnStatement(Expression *_ret_expr) : ret_expr(_ret_expr) {}

  string dump_ast(string prefix) {
    if (ret_expr)
      return "return\n" + prefix +
             "`- expr: " + ret_expr->dump_ast(prefix + "   ");
    return "return";
  }

  void scopify(symboltable *s, int *new_location) {
    ret_expr->scopify(s, new_location);
  }

  ~ReturnStatement() { delete ret_expr; }
};

struct Declaration : Node {
  Type *typ;
  Identifier *Ident;

  Declaration(Type *_typ, Identifier *_name) : typ(_typ), Ident(_name) {}

  string dump_ast(string prefix) {
    return Ident->dump_ast(prefix) + "\n" + prefix + "`- type: " + typ->dump_ast(prefix + "   ");
  }

  void scopify(symboltable *s, int *new_location) {
    if (s->check_scope(Ident->name)) {
      cout << "PANIC REDECLARATION\n"; // TODO error
    } else {
      (*new_location)++;
      s->add_symbol(Ident->name, (*new_location));
      Ident->scopify(s, new_location);
    }
  }

  ~Declaration() { delete typ; }
};

struct DeclarationStatement : Statement {
  Declaration *decl;

  DeclarationStatement(Declaration *_decl) : decl(_decl) {}

  string dump_ast(string prefix) { return "decl: " + decl->dump_ast(prefix); }

  void scopify(symboltable *s, int *new_location) {
    decl->scopify(s, new_location);
  }

  ~DeclarationStatement() { delete decl; }
};

struct BlockStatement : Statement, vector<Statement *> {
  string dump_ast(string prefix) {
    if (this->size() == 0) {
      return "block (empty)";
    }
    string result = "block:\n";
    int i = 0;
    auto stmt = (*this)[i];
    while (i < this->size() - 1) {
      result += prefix + "`- " + stmt->dump_ast(prefix + "|  ") + "\n";
      i++;
      stmt = (*this)[i];
    }
    result += prefix + "`- " + stmt->dump_ast(prefix + "   ");
    return result;
  }

  void scopify(symboltable *s, int *new_location) {
    for (auto stmt : *this) {
      stmt->scopify(s, new_location);
    }
  }

  ~BlockStatement() {
    for (auto stmt : *this) {
      delete stmt;
    }
  }
};

struct Parameter : Node {
  Type *typ;
  string name;
  int location;

  Parameter(Type *_typ, string _name) : typ(_typ), name(_name), location(-1) {}

  string dump_ast(string prefix) {
    return name + "[" + to_string(location) +"]" +"\n" + prefix + "`- type: " + typ->dump_ast(prefix + "   ");
  }

  void scopify(symboltable *s, int *new_location) {
    if (s->check_scope(name)) {
      cout << "PANIC REPEATED PARAMETER\n"; // TODO error
    } else {
      (*new_location)++;
      s->add_symbol(name, *new_location);
      location = *new_location;
    }
  }

  ~Parameter() { delete typ; }
};

struct Function : Node {
  Type *return_type;
  string name;
  vector<Parameter *> *parameters;
  BlockStatement *statement_block;

  Function(Type *_return_type, string _name, vector<Parameter *> *_parameters,
           BlockStatement *_stmts)
      : return_type(_return_type), name(_name), parameters(_parameters),
        statement_block(_stmts) {}

  string dump_ast(string prefix) {
    string result = "function: " + name + "\n" + prefix +
                    "`- return_type: " + return_type->dump_ast(prefix + "|  ");
    if (parameters) {
      string params = "parameters:\n";
      int i = 0;
      auto param = (*parameters)[i];
      while (i < parameters->size() - 1) {
        params +=
            prefix + "|  " + "`- " + param->dump_ast(prefix + "|  |  ") + "\n";
        i++;
        param = (*parameters)[i];
      }
      params += prefix + "|  " + "`- " + param->dump_ast(prefix + "|     ");
      result += "\n" + prefix + "`- " + params;
    }
    result += "\n" + prefix + "`- " + statement_block->dump_ast(prefix + "   ");
    return result;
  }

  void scopify(symboltable *s, int *new_location) {
    if (s->check_scope(name)) {
      cout << "PANIC REDECLARATION\n"; // TODO no error if this is the
                                       // definition
    } else {
      s->add_symbol(name, 0);
      *new_location = 0;
      s->enter_scope();
      if (parameters) {
        for (auto param : *parameters) {
          param->scopify(s, new_location);
        }
      }
      statement_block->scopify(s, new_location);
      s->exit_scope();
    }
  }

  ~Function() {
    delete return_type;
    for (auto param : *parameters) {
      delete param;
    }
    delete parameters;
    delete statement_block;
  }
};

struct TranslationUnit : Node {

  vector<Function *> *functions;
  vector<DeclarationStatement *> *decls;
  vector<bool> *is_decl;

  TranslationUnit(vector<Function *> *_functions,
                  vector<DeclarationStatement *> *_decls,
                  vector<bool> *_is_decl)
      : functions(_functions), decls(_decls), is_decl(_is_decl) {}

  void add_function(Function *func) {
    functions->push_back(func);
    is_decl->push_back(false);
  }

  void add_declaration(DeclarationStatement *decl) {
    decls->push_back(decl);
    is_decl->push_back(true);
  }

  string dump_ast(string prefix) {
    // if (this->size() == 0) {
    //   return "translation_unit (empty)";
    // }
    // string result = "translation_unit:\n";
    // int i = 0;
    // auto fn = (*this)[i];
    // while (i < this->size() - 1) {
    //   result += prefix + "`- " + fn->dump_ast(prefix + "|  ") + "\n";
    //   i++;
    //   fn = (*this)[i];
    // }
    // result += prefix + "`- " + fn->dump_ast(prefix + "   ");
    // return result;
    if (is_decl->size() == 0) {
      return "translation_unit (empty)";
    }
    string result = "translation_unit:\n";

    int func_itr = 0;
    int decl_itr = 0;

    for (int i = 0; i < is_decl->size(); i++) {
      if (!(*is_decl)[i]) {
        if (func_itr < functions->size() - 1) {
          result += prefix + "`- " +
                    ((*functions)[func_itr])->dump_ast(prefix + "|  ") + "\n";
        } else {
          result += prefix + "`- " +
                    ((*functions)[func_itr])->dump_ast(prefix + "   ") + "\n";
        }
        func_itr++;
      } else {
        result += prefix + "`- " +
                  ((*decls)[decl_itr])->dump_ast(prefix + "   ") + "\n";
        decl_itr++;
      }
    }
    return result;
  }

  void scopify(symboltable *s, int *new_location) {
    // if(this->size() == 0){
    //   return;
    // }
    // s = new symboltable;
    // new_location = new int;
    // new_location = 0;
    // for(int i = 0; i < this->size(); i++){
    //   (*this)[i]->scopify(s, new_location);
    // }
    // delete s;
    // delete new_location;

    if (is_decl->size() == 0) {
      return;
    }
    s = new symboltable;
    new_location = new int;
    *new_location = 0;

    s->enter_scope();

    int func_itr = 0;
    int decl_itr = 0;

    for (int i = 0; i < is_decl->size(); i++) {
      if (!(*is_decl)[i]) {
        (*functions)[func_itr]->scopify(s, new_location);
        func_itr++;
      } else {
        // ASSIGNING NEGATIVE VALUES TO GLOBAL SCOPE
        *new_location = decl_itr - decls->size() - 1;
        (*decls)[decl_itr]->scopify(s, new_location);
        decl_itr++;
      }
    }

    delete s;
    delete new_location;
  }

  ~TranslationUnit() {
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
};
} // namespace ast
