#pragma once

#include <stack>
#include <string>
#include "ast.hpp"
using namespace std;

namespace ehdl {
void err(string message, ast::sympos pos);
void warn(string message, ast::sympos pos);
void print_errs();
void print_warns();
void set_filename(string filename);
int n_errs();
int n_warns();
}
