#include "ast.hpp"
#include "debug.hpp"

#include <sstream>

namespace ast {

////////////////////////////////////////////////////////////////////////////////
// Utilities
////////////////////////////////////////////////////////////////////////////////

string starify(int ptr_depth, string name) {
  cdebug << "starify: " << ptr_depth << ", " << name << std::endl;
  stringstream s;
  for (int i=0; i<ptr_depth; i++) {
    s << "*";
  }
  s << name;
  return s.str();
}

template<typename T>
string listify(std::vector<T*>& nodes, string prefix) {
  cdebug << "listify: " << prefix << " with " << nodes.size() << " nodes " << std::endl;
  stringstream s;
  s << "\n" << prefix;
  for (int i=0; i<int(nodes.size())-1; i++) {
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

string Identifier::dump_ast(string prefix) {
  cdebug << "Identifier::dump_ast: " << endl;
  return name + "[" + to_string(ident_info.idx) + "]";
}

string TernaryExpression::dump_ast(string prefix) {
  cdebug << "TernaryExpression::dump_ast: " << endl;
  return "ternary_op\n" + prefix +
         "`- cond: " + cond->dump_ast(prefix + "|  ") + "\n" + prefix +
         "`- true_branch: " + true_branch->dump_ast(prefix + "|  ") + "\n" + prefix +
         "`- false_branch: " + false_branch->dump_ast(prefix + "   ");
}

string FunctionInvocationExpression::dump_ast(string prefix) {
  cdebug << "FunctionInvocationExpression::dump_ast: " << endl;
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

string BinaryExpression::dump_ast(string prefix) {
  cdebug << "BinaryExpression::dump_ast: " << endl;
  return op2str(op) + "\n" + prefix +
         "`- lhs: " + lhs->dump_ast(prefix + "|  ") + "\n" + prefix +
         "`- rhs: " + rhs->dump_ast(prefix + "   ");
}

string UnaryExpression::dump_ast(string prefix) {
  cdebug << "UnaryExpression::dump_ast: " << endl;
  return op2str(op) + "\n" + prefix +
         "`- expr: " + expr->dump_ast(prefix + "   ");
}

string Literal::dump_ast(string prefix) {
  cdebug << "Literal::dump_ast: " << endl;
  if (value == "") {
    string s = to_string(data.l);
    if (ltype == LT_DOUBLE) s = to_string(data.d);
    else if (ltype == LT_FLOAT) s = to_string(data.f);
    return lt2str(ltype) + " (" + s + ")";
  }
  return "literal (" + value + ")";
}

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

string DeclarationSpecifiers::dump_ast(string prefix) {
  cdebug << "DeclarationSpecifiers::dump_ast: " << endl;
  stringstream s;
  if (!storage_specs.empty()) {
    s << "\n" << prefix << "`- ss: ";
    for (auto ss : storage_specs) {
      s << ss2str(ss) << " ";
    }
  }
  if (!type_specs.empty()) {
    s << "\n" << prefix << "`- ss: ";
    for (auto ts : type_specs) {
      s << ts2str(ts) << " ";
    }
  }
  if (!type_quals.empty()) {
    s << "\n" << prefix << "`- tq: ";
    for (auto tq : type_quals) {
      s << tq2str(tq) << " ";
    }
  }
  if (!func_specs.empty()) {
    s << "\n" << prefix << "`- fs: ";
    for (auto fs : func_specs) {
      s << fs2str(fs) << " ";
    }
  }
  return s.str();
}

string PureDeclaration::dump_ast(string prefix) {
  cdebug << "PureDeclaration::dump_ast: " << endl;
  stringstream s;
  s << starify(ptr_depth, ident->name) << "\n" << prefix
    << "`- declspec: " << decl_specs->dump_ast(prefix + "   ");
  return s.str();
}

string FunctionParameterList::dump_ast(string prefix) {
  cdebug << "FunctionParameterList::dump_ast: " << endl;
  stringstream s;
  s << listify<PureDeclaration>(*params, prefix);
  if (has_varargs) {
    s << "\n" << prefix << "`- VA_ARGS";
  }
  return s.str();
}

string InitDeclarator::dump_ast(string prefix) {
  cdebug << "InitDeclarator::dump_ast: " << endl;
  stringstream s;
  s << starify(ptr_depth, ident->name);
  if (init_expr) {
    s << "\n" << prefix << "`- init: " << init_expr->dump_ast(prefix + "   ");
  }
  return s.str();
}

string Declaration::dump_ast(string prefix) {
  cdebug << "Declaration::dump_ast: " << endl;
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
  cdebug << "DeclarationStatement::dump_ast: " << endl;
  return "decl: " + decl->dump_ast(prefix);
}

string ExpressionStatement::dump_ast(string prefix) {
  cdebug << "ExpressionStatement::dump_ast: " << endl;
  if (!expr) return "no expr";
  return "expr: " + expr->dump_ast(prefix);
}

string IfStatement::dump_ast(string prefix) {
  cdebug << "IfStatement::dump_ast: " << endl;
  std::stringstream ss;
  ss << "if\n" << prefix << "`- cond: " << cond->dump_ast(prefix + "| ") << "\n";

  if (false_branch) {
    if (true_branch) {
      ss << prefix << "`- true_branch: " << true_branch->dump_ast(prefix + "| ")
        << "\n";
    }
    else{
      ss << prefix << "`- true_branch removed " << "\n";
    }
    ss << prefix << "`- false_branch: " << false_branch->dump_ast(prefix + " ");
  } else {
    if (true_branch) {
      ss << prefix << "`- true_branch: " << true_branch->dump_ast(prefix + "| ")
        << "\n";
    }
    else{
      ss << prefix << "`- true_branch removed " << "\n";
    }
  }

  return ss.str();
}

string SwitchStatement::dump_ast(string prefix) {
  cdebug << "SwitchStatement::dump_ast: " << endl;
  return "dump_ast not implemented for switch";
}

string CaseStatement::dump_ast(string prefix) {
  cdebug << "CaseStatement::dump_ast: " << endl;
  return "dump_ast not implemented for case";
}

string WhileStatement::dump_ast(string prefix) {
  cdebug << "WhileStatement::dump_ast: " << endl;
  stringstream ss;
  if (stmt){
    ss << "while\n" << prefix << "`- cond: " << cond->dump_ast(prefix + "| ") << "\n"
      << prefix << "`- stmt: " << stmt->dump_ast(prefix + " ");
  }
  else{
    ss << "while\n" << prefix << "`- cond: " << cond->dump_ast(prefix + "| ") << "\n"
      << prefix << "`- stmt: removed";
  }
  return ss.str();
}

string DoWhileStatement::dump_ast(string prefix) {
  cdebug << "DoWhileStatement::dump_ast: " << endl;
  stringstream ss;
  ss << "do-while\n" << prefix << "`- cond: " << cond->dump_ast(prefix + "| ") << "\n"
     << prefix << "`- stmt: " << stmt->dump_ast(prefix + " ");
  return ss.str();
}

string ReturnStatement::dump_ast(string prefix) {
  cdebug << "ReturnStatement::dump_ast: " << endl;
  if (ret_expr)
    return "return\n" + prefix +
           "`- expr: " + ret_expr->dump_ast(prefix + "   ");
  return "return";
}

string LabeledStatement::dump_ast(string prefix) {
  cdebug << "LabeledStatement::dump_ast: " << endl;
  return label->name + "\n" + prefix + stmt->dump_ast(prefix + "   ");
}

string BreakStatement::dump_ast(string prefix) {
  cdebug << "BreakStatement::dump_ast: " << endl;
  return "break";
}

string ContinueStatement::dump_ast(string prefix) {
  cdebug << "ContinueStatement::dump_ast: " << endl;
  return "continue";
}

string GotoStatement::dump_ast(string prefix) {
  cdebug << "GotoStatement::dump_ast: " << endl;
  return "goto " + label;
}

string BlockStatement::dump_ast(string prefix) {
  cdebug << "BlockStatement::dump_ast: " << endl;
  return listify<Statement>(*this, prefix);
}

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

string Function::dump_ast(string prefix) {
  stringstream s;
  cdebug << "Function::dump_ast: " << endl;
  if (stmts){
    s << func_decl->dump_ast(prefix) << "def \n" << prefix;
  }
  else{
    s << func_decl->dump_ast(prefix) << "decl \n" << prefix;
  }
  
  if (params) {
    s << "`- params: " << params->dump_ast(prefix+"|  ") << "\n" << prefix;
  }

  if (stmts) {
    s << "`- block: " << stmts->dump_ast(prefix+"   ");
  }
  return s.str();
}

string TranslationUnit::dump_ast(string prefix) {
  cdebug << "TranslationUnit::dump_ast: " << endl;
  if (nodes->size() == 0) {
    return "translation_unit (empty)";
  }
  string result = "translation_unit:\n";

  return listify<Node>(*nodes, "");
}
} // namespace ast
