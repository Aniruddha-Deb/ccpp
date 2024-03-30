#include "ast.hpp"

namespace ast {

void Type::scopify(symboltable *s, int *new_location) {}

void Identifier::scopify(symboltable *s, int *new_location) {
  location = s->find_symbol(name);
}

void TernaryExpression::scopify(symboltable *s, int *new_location) {
  cond->scopify(s, new_location);
  true_branch->scopify(s, new_location);
  false_branch->scopify(s, new_location);
}

void TypecastExpression::scopify(symboltable *s, int *new_location) {
  expr->scopify(s, new_location);
}

void FunctionInvocationExpression::scopify(symboltable *s, int *new_location) {
  fn->scopify(s, new_location);
  if (params) {
    for (int i = 0; i < params->size(); i++) {
      (*params)[i]->scopify(s, new_location);
    }
  }
}

void TypeExpression::scopify(symboltable *s, int *new_location) { return; }

void BinaryExpression::scopify(symboltable *s, int *new_location) {
  lhs->scopify(s, new_location);
  rhs->scopify(s, new_location);
}

void UnaryExpression::scopify(symboltable *s, int *new_location) {
  expr->scopify(s, new_location);
}

void Literal::scopify(symboltable *s, int *new_location) { return; }

void ExpressionStatement::scopify(symboltable *s, int *new_location) {
  expr->scopify(s, new_location);
}

void IfStatement::scopify(symboltable *s, int *new_location) {
  cond->scopify(s, new_location);
  s->enter_scope();
  true_branch->scopify(s, new_location);
  s->exit_scope();
  if (false_branch) {
    s->enter_scope();
    false_branch->scopify(s, new_location);
    s->exit_scope();
  }
}

void WhileStatement::scopify(symboltable *s, int *new_location) {
  cond->scopify(s, new_location);
  s->enter_scope();
  stmt->scopify(s, new_location);
  s->exit_scope();
}

void DoWhileStatement::scopify(symboltable *s, int *new_location) {
  cond->scopify(s, new_location);
  s->enter_scope();
  stmt->scopify(s, new_location);
  s->exit_scope();
}

void ReturnStatement::scopify(symboltable *s, int *new_location) {
  ret_expr->scopify(s, new_location);
}

void Declaration::scopify(symboltable *s, int *new_location) {
  if (s->check_scope(Ident->name)) {
    cout << "PANIC REDECLARATION\n";
  } else {
    (*new_location)++;
    s->add_symbol(Ident->name, (*new_location));
    Ident->scopify(s, new_location);
  }
}

void DeclarationStatement::scopify(symboltable *s, int *new_location) {
  decl->scopify(s, new_location);
}

void BlockStatement::scopify(symboltable *s, int *new_location) {
  for (auto stmt : *this) {
    stmt->scopify(s, new_location);
  }
}

void Parameter::scopify(symboltable *s, int *new_location) {
  if (s->check_scope(name)) {
    cout << "PANIC REPEATED PARAMETER\n";
  } else {
    (*new_location)++;
    s->add_symbol(name, *new_location);
    location = *new_location;
  }
}

void Function::scopify(symboltable *s, int *new_location) {
  std::string name = func_decl->ident->name;
  if (s->check_scope(name)) {
    cout << "PANIC REDECLARATION\n";
  } else {
    s->add_symbol(name, 0);
    *new_location = 0;
    s->enter_scope();
    if (params) {
      for (auto param : *params) {
        param->scopify(s, new_location);
      }
    }
    stmts->scopify(s, new_location);
    s->exit_scope();
  }
}

void TranslationUnit::scopify(symboltable *s, int *new_location) {
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
      *new_location = decl_itr - decls->size() - 1;
      (*decls)[decl_itr]->scopify(s, new_location);
      decl_itr++;
    }
  }

  delete s;
  delete new_location;
}
};
