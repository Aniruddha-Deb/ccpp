#include "ast.hpp"

namespace ast {

string Type::dump_ast(string prefix) { return name; }

string Identifier::dump_ast(string prefix) {
  return name + "[" + to_string(location) + "]";
}

string TernaryExpression::dump_ast(string prefix) {
  return "ternary_op\n" + prefix +
         "`- cond: " + cond->dump_ast(prefix + "|  ") + "\n" + prefix +
         "`- true_branch: " + true_branch->dump_ast(prefix + "|  ") + "\n" +
         prefix + "`- false_branch: " + false_branch->dump_ast(prefix + "   ");
}

string TypecastExpression::dump_ast(string prefix) {
  return "typecast\n" + prefix + "`- type: " + typ->dump_ast(prefix + "|  ") +
         "\n" + prefix + "`- expr: " + expr->dump_ast(prefix + "   ");
}

string FunctionInvocationExpression::dump_ast(string prefix) {
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

string TypeExpression::dump_ast(string prefix) {
  return op2str(op) + "\n" + prefix +
         "`- type: " + typ->dump_ast(prefix + "   ");
}

string BinaryExpression::dump_ast(string prefix) {
  return op2str(op) + "\n" + prefix +
         "`- lhs: " + lhs->dump_ast(prefix + "|  ") + "\n" + prefix +
         "`- rhs: " + rhs->dump_ast(prefix + "   ");
}

string UnaryExpression::dump_ast(string prefix) {
  return op2str(op) + "\n" + prefix +
         "`- expr: " + expr->dump_ast(prefix + "   ");
}

string Literal::dump_ast(string prefix) { return "literal (" + value + ")"; }

string ExpressionStatement::dump_ast(string prefix) {
  return "expr: " + expr->dump_ast(prefix);
}

string IfStatement::dump_ast(string prefix) {
  string retval =
      "if\n" + prefix + "`- cond: " + cond->dump_ast(prefix + "|  ") + "\n";
  if (false_branch) {
    retval += prefix +
              "`- true_branch: " + true_branch->dump_ast(prefix + "|  ") + "\n";
    retval +=
        prefix + "`- false_branch: " + false_branch->dump_ast(prefix + "   ");
  } else {
    retval +=
        prefix + "`- true_branch: " + true_branch->dump_ast(prefix + "   ");
  }
  return retval;
}

string WhileStatement::dump_ast(string prefix) {
  return "while\n" + prefix + "`- cond: " + cond->dump_ast(prefix + "|  ") +
         "\n" + prefix + "`- stmt: " + stmt->dump_ast(prefix + "   ");
}

string DoWhileStatement::dump_ast(string prefix) {
  return "while\n" + prefix + "`- cond: " + cond->dump_ast(prefix + "|  ") +
         "\n" + prefix + "`- stmt: " + stmt->dump_ast(prefix + "   ");
}

string ReturnStatement::dump_ast(string prefix) {
  if (ret_expr)
    return "return\n" + prefix +
           "`- expr: " + ret_expr->dump_ast(prefix + "   ");
  return "return";
}

string Declaration::dump_ast(string prefix) {
  return Ident->dump_ast(prefix) + "\n" + prefix +
         "`- type: " + typ->dump_ast(prefix + "   ");
}

string DeclarationStatement::dump_ast(string prefix) {
  return "decl: " + decl->dump_ast(prefix);
}

string BlockStatement::dump_ast(string prefix) {
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

string Parameter::dump_ast(string prefix) {
  return name + "[" + to_string(location) + "]" + "\n" + prefix +
         "`- type: " + typ->dump_ast(prefix + "   ");
}

string TranslationUnit::dump_ast(string prefix) {
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
};
