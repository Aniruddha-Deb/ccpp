#include "error.hpp"

#include <stack>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#define C_HEADER "\033[95m"
#define C_OKBLUE "\033[94m"
#define C_OKCYAN "\033[96m"
#define C_OKGREEN "\033[92m"
#define C_WARNING "\033[93m"
#define C_FAIL "\033[91m"
#define C_ENDC "\033[0m"
#define C_BOLD "\033[1m"
#define C_UNDERLINE "\033[4m"

namespace ehdl {

const int MAX_ERR = 5;
const int MAX_WARN = 100;

static stack<string> errors;
static stack<string> warnings;
static string filename = "translation_unit";
static vector<string> filebuf;

std::string construct_code_display(ast::sympos pos) {
  stringstream s;
  s << filebuf[pos.first_line-1] << endl;
  int i;
  for (i=1; i<pos.first_column; i++) {
    s << " ";
  }
  s << C_OKGREEN << "^";
  if (pos.last_line != pos.first_line) return s.str();
  for (i+=1; i<=pos.last_column; i++) {
    s << "~";
  }
  return s.str();
}

void err(string message, ast::sympos pos) {
  if (errors.size() > MAX_ERR) {
    print_errs();
    cout << "Error limit reached, stopping emitting errors" << endl;
    exit(0);
  }
  stringstream s;
  s << C_BOLD << filename << ":" << pos.first_line << ":" << pos.first_column << ": " << C_FAIL << "error: " << C_ENDC << C_BOLD << message << C_ENDC << endl;
  s << construct_code_display(pos) << C_ENDC << endl;
  errors.push(s.str());
}

void warn(string message, ast::sympos pos) {
  if (warnings.size() > MAX_ERR) {
    print_warns();
    cout << "Warning limit reached, stopping emitting warnings" << endl;
    return;
  }
  stringstream s;
  s << C_BOLD << filename << ":" << pos.first_line << ":" << pos.first_column << ": " << C_WARNING << "warning: " << C_ENDC << C_BOLD << message << C_ENDC << endl;
  s << construct_code_display(pos) << C_ENDC << endl;
  warnings.push(s.str());
}

int n_errs() { return errors.size(); }
int n_warns() { return warnings.size(); }

void print_errs() {
  while (!errors.empty()) {
    cout << errors.top();
    errors.pop();
  }
}

void print_warns() {
  while (!warnings.empty()) {
    cout << warnings.top();
    warnings.pop();
  }
}

void set_filename(string name) {
  filename = name;
  filebuf.clear();
  std::fstream file;
  file.open(name,std::ios::in);

  std::string str;
  while(getline(file, str))
  {
    filebuf.push_back(str);
  }
}

}

