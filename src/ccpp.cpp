#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "ast.hpp"
#include "error.hpp"
#include "c.tab.hpp"
#include "symtab.hpp"
#include "argparse.hpp"

extern "C" FILE *yyin;

int main(int argc, char **argv) {

  argparse::ArgumentParser ccpp("ccpp", "1.0");

  ccpp.add_argument("source").help("Source file to compile");
  if (argc == 1) {
    std::cerr << ccpp;
    return 0;
  }

  try {
    ccpp.parse_args(argc, argv);
  }
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << ccpp;
    return 0;
  }

  string filename = ccpp.get("source");
  ehdl::set_filename(filename);
  yyin = fopen(filename.c_str(), "r");
  assert(yyin);
  ast::TranslationUnit* tu = new ast::TranslationUnit();
  int ret = yyparse(tu);

  std::cout << std::endl;

  tu->scopify();
  std::cout << "Done scopifying" << std::endl;
  std::cout << tu->dump_ast("") << std::endl;

  if (ehdl::n_errs() > 0) {
    ehdl::print_errs();
    exit(0);
  }

  tu->const_prop();

  std::cout << "Done optimizing" << std::endl;
  std::cout << tu->dump_ast("") << std::endl;

  // printf("retv = %d\n", ret);


  tu->codegen();
  exit(0);
}
