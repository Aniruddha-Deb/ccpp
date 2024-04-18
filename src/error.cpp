#include "error.hpp"

#include <stack>
#include <iostream>
#include <string>

namespace ehdl {

const int MAX_ERR = 5;
const int MAX_WARN = 100;

static stack<string> errors;
static stack<string> warnings;

void print_all_errs() {

}

void err(string message) {
  if (errors.size() > MAX_ERR) {
    cout << "Error limit reached, stopping emitting errors" << endl;
    for (string s : errors) {

    }
    exit(0);
  }

}

void warn(string message) {

}

int n_errs() { return errors.size(); }
int n_warns() { return warnings.size(); }

}

