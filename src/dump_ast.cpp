#include "ast.hpp"

#include <sstream>

namespace ast {

////////////////////////////////////////////////////////////////////////////////
// Utilities
////////////////////////////////////////////////////////////////////////////////

string starify(int ptr_depth, string name) {
  stringstream s;
  for (int i=0; i<ptr_depth; i++) {
    s << "*";
  }
  s << name;
  return s.str();
}

template<typename T>
string listify(std::vector<T*>& nodes, string prefix) {
  stringstream s;
  s << "\n" << prefix;
  for (int i=0; i<nodes.size()-1; i++) {
    s << "`- " << nodes[i]->dump_ast(prefix+"|  ") << "\n" << prefix;
  }
  if (nodes.size() > 0) {
    s << "`- " << nodes[nodes.size()-1]->dump_ast(prefix+"   ");
  }
  return s.str();
}

////////////////////////////////////////////////////////////////////////////////
// Expressions
////////////////////////////////////////////////////////////////////////////////

string Type::dump_ast(string prefix) { return name; }

string Identifier::dump_ast(string prefix) {
  return name + "[" + to_string(location) + "]";
}

string TernaryExpression::dump_ast(string prefix) {
  return "ternary_op\n" + prefix +
         "`- cond: " + cond->dump_ast(prefix + "|  ") + "\n" + prefix +
         "`- true_branch: " + true_branch->dump_ast(prefix + "|  ") + "\n" + prefix +
         "`- false_branch: " + false_branch->dump_ast(prefix + "   ");
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

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

string DeclarationSpecifiers::dump_ast(string prefix) {
  stringstream s;
  s << ts2str(type) << "\n" << prefix;
  if (!storage_specs.empty()) {
    s << "`- ss: ";
    for (auto ss : storage_specs) {
      s << ss2str(ss) << " ";
    }
    s << "\n" << prefix;
  }
  if (!type_quals.empty()) {
    s << "`- tq: ";
    for (auto tq : type_quals) {
      s << tq2str(tq) << " ";
    }
    s << "\n" << prefix;
  }
  if (!func_specs.empty()) {
    s << "`- fs: ";
    for (auto fs : func_specs) {
      s << fs2str(fs) << " ";
    }
    s << "\n" << prefix;
  }
  return s.str();
}

string PureDeclaration::dump_ast(string prefix) {
  stringstream s;
  s << starify(ptr_depth, ident->name) << "\n" << prefix
    << "`- declspec: " << decl_specs->dump_ast(prefix + "   ");
  return s.str();
}

string FunctionParameterList::dump_ast(string prefix) {
  stringstream s;
  s << listify<PureDeclaration>(*this, prefix);
  if (has_varargs) {
    s << "\n" << prefix << "`- VA_ARGS";
  }
  return s.str();
}

string InitDeclarator::dump_ast(string prefix) {
  stringstream s;
  s << starify(ptr_depth, ident->name);
  if (init_expr) {
    s << "\n" << prefix << "`- init: " << init_expr->dump_ast(prefix + "   ");
  }
  return s.str();
}

string Declaration::dump_ast(string prefix) {
  stringstream s;
  s << "\n" << prefix
    << "`- declspec: " << decl_specs->dump_ast(prefix + "|  ") << "\n" << prefix 
    << "`- decls: " << listify(*decl_list, prefix+"   ");
  return s.str();
}

////////////////////////////////////////////////////////////////////////////////
// Statements
////////////////////////////////////////////////////////////////////////////////

string DeclarationStatement::dump_ast(string prefix) {
  return "decl: " + decl->dump_ast(prefix);
}

string ExpressionStatement::dump_ast(string prefix) {
  return "expr: " + expr->dump_ast(prefix);
}

string IfStatement::dump_ast(string prefix) {
  std::stringstream ss;
  ss << "if\n" << prefix << "`- cond: " << cond->dump_ast(prefix + "| ") << "\n";

  if (false_branch) {
    ss << prefix << "`- true_branch: " << true_branch->dump_ast(prefix + "| ")
       << "\n";
    ss << prefix << "`- false_branch: " << false_branch->dump_ast(prefix + " ");
  } else {
    ss << prefix << "`- true_branch: " << true_branch->dump_ast(prefix + " ");
  }

  return ss.str();
}

string SwitchStatement::dump_ast(string prefix) {
  return "dump_ast not implemented for switch";
}

string CaseStatement::dump_ast(string prefix) {
  return "dump_ast not implemented for case";
}

string WhileStatement::dump_ast(string prefix) {
    stringstream ss;
    ss << "while\n" << prefix << "`- cond: " << cond->dump_ast(prefix + "| ") << "\n"
       << prefix << "`- stmt: " << stmt->dump_ast(prefix + " ");
    return ss.str();
}

string DoWhileStatement::dump_ast(string prefix) {
    stringstream ss;
    ss << "do-while\n" << prefix << "`- cond: " << cond->dump_ast(prefix + "| ") << "\n"
       << prefix << "`- stmt: " << stmt->dump_ast(prefix + " ");
    return ss.str();
}

string ReturnStatement::dump_ast(string prefix) {
  if (ret_expr)
    return "return\n" + prefix +
           "`- expr: " + ret_expr->dump_ast(prefix + "   ");
  return "return";
}

string BreakStatement::dump_ast(string prefix) {
  return "break";
}

string ContinueStatement::dump_ast(string prefix) {
  return "continue";
}

string GotoStatement::dump_ast(string prefix) {
  return "goto " + label;
}

string BlockStatement::dump_ast(string prefix) {
  return listify<Statement>(*this, prefix);
}

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

string Function::dump_ast(string prefix) {
  stringstream s;
  s << func_decl->dump_ast(prefix) << "\n" << prefix 
    << "`- params: " << params->dump_ast(prefix+"|  ") << "\n" << prefix 
    << "`- block: " << stmts->dump_ast(prefix+"   ");
  return s.str();
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
