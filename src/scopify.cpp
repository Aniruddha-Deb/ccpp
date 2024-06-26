#include "ast.hpp"
#include "debug.hpp"
#include "error.hpp"

namespace ast {

static SymbolTable *table;


SymbolType typespecs2st(std::set<TypeSpecifier> type_specs) {
  if (type_specs.find(TS_FLOAT) != type_specs.end()) return FP32;
  if (type_specs.find(TS_DOUBLE) != type_specs.end()) return FP64;
  if (type_specs.find(TS_UNSIGNED) != type_specs.end()) {
    if (type_specs.find(TS_SHORT) != type_specs.end()) return U16;
    if (type_specs.find(TS_LONG) != type_specs.end()) return U64;
    return U32;
  }
  if (type_specs.find(TS_CHAR) != type_specs.end()) {
    if (type_specs.find(TS_SIGNED) != type_specs.end()) return I8;
    return U8;
  }
  if (type_specs.find(TS_SHORT) != type_specs.end()) return I16;
  if (type_specs.find(TS_LONG) != type_specs.end()) return I64;
  if (type_specs.find(TS_BOOL) != type_specs.end()) return I1;
  if (type_specs.find(TS_VOID) != type_specs.end()) return VD;
  return I32;
}

void Identifier::scopify() {
  cdebug << "Identifier::scopify: " << endl;
  ident_info = table->find_symbol(name);
  if (ident_info.stype == UNK) {
    ehdl::err("Use of undeclared identifier " + name, pos);
  }
  // cout << name << " ";
  // cout << ident_info.ptr_depth << endl;
}

void TernaryExpression::scopify() {
  cdebug << "TernaryExpression::scopify: " << endl;
  cond->scopify();
  true_branch->scopify();
  false_branch->scopify();
}

void FunctionInvocationExpression::scopify() {
  cdebug << "FunctionInvocationExpression::scopify: " << endl;
  fn->scopify();
  if (params) {
    for (int i = 0; i < params->size(); i++) {
      (*params)[i]->scopify();
    }
  }
}

void BinaryExpression::scopify() {
  cdebug << "BinaryExpression::scopify: " << endl;
  lhs->scopify();
  rhs->scopify();
}

void UnaryExpression::scopify() {
  cdebug << "UnaryExpression::scopify: " << endl;
  expr->scopify();
}

void Literal::scopify() {
  cdebug << "Literal::scopify: " << endl; 
  return;
}

void ExpressionStatement::scopify() {
  cdebug << "ExpressionStatement::scopify: " << endl;
  if (expr) expr->scopify();
}

void IfStatement::scopify() {
  cdebug << "IfStatement::scopify: " << endl;
  cond->scopify();
  table->enter_scope();
  true_branch->scopify();
  table->exit_scope();
  if (false_branch) {
    table->enter_scope();
    false_branch->scopify();
    table->exit_scope();
  }
}

void SwitchStatement::scopify() {
  cdebug << "SwitchStatement::scopify: " << endl;
  return;
}

void WhileStatement::scopify() {
  cdebug << "WhileStatement::scopify: " << endl;
  cond->scopify();
  table->enter_scope();
  if (stmt) stmt->scopify();
  table->exit_scope();
}

void DoWhileStatement::scopify() {
  cdebug << "DoWhileStatement::scopify: " << endl;
  cond->scopify();
  table->enter_scope();
  stmt->scopify();
  table->exit_scope();
}

void ContinueStatement::scopify() {
  cdebug << "ContinueStatement::scopify: " << endl;
  return;
}
void BreakStatement::scopify() {
  cdebug << "BreakStatement::scopify: " << endl;
  return;
}
void GotoStatement::scopify() {
  cdebug << "GotoStatement::scopify: " << endl;
  return;
}
void CaseStatement::scopify() {
  cdebug << "CaseStatement::scopify: " << endl;
  return;
}
void LabeledStatement::scopify() {
  cdebug << "LabeledStatement::scopify: " << endl;
  return;
}

void ReturnStatement::scopify() {
  cdebug << "ReturnStatement::scopify: " << endl;
  ret_expr->scopify();
}

void DeclarationSpecifiers::scopify() {
  cdebug << "DeclarationSpecifiers::scopify: " << endl;
  return;
}

void InitDeclarator::scopify() {
  cdebug << "InitDeclarator::scopify: " << endl;
  cout << "Error: should not call scopify on an InitDeclarator\n";
  return;
}

void Declaration::scopify() {
  cdebug << "Declaration::scopify: " << endl;
  for (InitDeclarator *decl : *decl_list) {
    if (table->check_scope(decl->ident->name)) {
      ehdl::err("Redeclaration of variable " + decl->ident->name, decl->pos);
    }
    else {
      table->add_symbol(decl->ident->name, {-1, decl->ptr_depth, typespecs2st(decl_specs->type_specs)});
      decl->ident->scopify();
      if(decl->init_expr){
        decl->init_expr->scopify();
      }
    }
  }
}

void DeclarationStatement::scopify() {
  cdebug << "DeclarationStatement::scopify: " << endl;
  decl->scopify();
}

void BlockStatement::scopify() {
  cdebug << "BlockStatement::scopify: " << endl;
  for (auto stmt : *this) {
    stmt->scopify();
  }
}

void PureDeclaration::scopify() {
  cdebug << "PureDeclaration::scopify: " << endl;
  if (table->check_scope(ident->name)) {
    ehdl::err("Redeclaration of variable " + ident->name, pos);
  }
  else {
    table->add_symbol(ident->name, {-1, ptr_depth, typespecs2st(decl_specs->type_specs)});
    ident->scopify();
  }
}

void FunctionParameterList::scopify() {
  cdebug << "FunctionParameterList::scopify: " << endl;
  // should have entered a new scope already in function
  for (PureDeclaration *decl : *params) {
    decl->scopify();
  }
}

void Function::scopify() {
  std::string name = func_decl->ident->name;
  cdebug << "Function::scopify: " << name << endl;
  table->add_symbol(name, {-1, func_decl->ptr_depth, typespecs2st(func_decl->decl_specs->type_specs)});
  func_decl->ident->scopify();
  cdebug<<"Function decl assigned type "<<typespecs2st(func_decl->decl_specs->type_specs)<<" ptr depth "<<func_decl->ptr_depth<<endl;
  table->reset_symb_identifier();
  table->enter_scope();
  if (params) params->scopify();
  if (stmts) stmts->scopify();
  table->exit_scope();
}

void TranslationUnit::scopify() {
  cdebug << "TranslationUnit::scopify: " << endl;
  if (nodes->size() == 0) {
    return;
  }

  table = new SymbolTable();

  table->enter_scope();

  for (auto node : *nodes) {
      node->scopify();
  }

  delete table;
}
};
