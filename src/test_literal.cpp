#include "ast.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

void print_literal(ast::Literal& l) {
  double d = *((double*)&l.data);
  cout << l.value << " " << std::setprecision(15) << d << " " << lt2str(l.ltype) << endl;

}

int main(int argc, char** argv) {

  ast::Literal l(std::string(argv[1]), ast::LT_FLOAT_LIKE);
  print_literal(l);
  cout << 0x8Af.38p2f << endl;

  return 0;
}
