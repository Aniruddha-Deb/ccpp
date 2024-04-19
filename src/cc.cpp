#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <fstream>
#include "ast.hpp"
#include "error.hpp"
#include "c.tab.hpp"
#include "symtab.hpp"
#include "argparse.hpp"

extern "C" FILE *yyin;

int main(int argc, char **argv) {

  argparse::ArgumentParser cc("cc", "1.0");

  cc.add_argument("source").help("Source file to compile");
  cc.add_argument("-o", "--object").help("Object file to generate");
  cc.add_argument("-t", "--print-ast").help("Print AST").flag();
  if (argc == 1) {
    std::cerr << cc;
    return 0;
  }

  try {
    cc.parse_args(argc, argv);
  }
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << cc;
    return 0;
  }

  string filename = cc.get("source");
  ehdl::set_filename(filename);
  yyin = fopen(filename.c_str(), "r");
  assert(yyin);
  ast::TranslationUnit* tu = new ast::TranslationUnit();
  int ret = yyparse(tu);

  // TODO check if ret != 0

  tu->scopify();

  if (cc["--print-ast"] == true) {
    std::cout << tu->dump_ast("") << std::endl;
  }

  if (ehdl::n_errs() > 0) {
    ehdl::print_errs();
    delete tu;
    return 0;
  }

  tu->const_prop();

  // std::cout << "Done optimizing" << std::endl;
  // std::cout << tu->dump_ast("") << std::endl;

  // printf("retv = %d\n", ret);

  string code = tu->codegen();

  if (ehdl::n_errs() > 0) {
    ehdl::print_errs();
    delete tu;
    return 0;
  }

  std::string output_fname = filename.substr(0,filename.find_last_of('.'))+".ll";
  if (auto oname = cc.present("-o")) {
    output_fname = *oname;
  }
  std::fstream f(output_fname, f.out);
  f << code << endl;
  f.flush();
  f.close();

  return 0;
}
