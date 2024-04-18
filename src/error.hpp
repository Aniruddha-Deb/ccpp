#pragma once

#include <stack>
#include <string>
using namespace std;

namespace ehdl {
void err(string message);
void warn(string message);
int n_errs();
int n_warns();
}
