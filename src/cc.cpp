#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "ast.hpp"
#include "c.tab.hpp"
#include "symtab.hpp"

// extern "C" int yylex();
// int yyparse(void* tu);
extern "C" FILE *yyin;

static void usage()
{
  printf("Usage: cc <prog.c>\n");
}

int
main(int argc, char **argv)
{
  if (argc != 2) {
    usage();
    exit(1);
  }
  char const *filename = argv[1];
  yyin = fopen(filename, "r");
  assert(yyin);
  ast::TranslationUnit* tu = new ast::TranslationUnit();
  int ret = yyparse(tu);

  std::cout << std::endl;

  tu->scopify();
  std::cout << "Done scopifying" << std::endl;
  std::cout << tu->dump_ast("") << std::endl;

  // printf("retv = %d\n", ret);
  tu->codegen();
  exit(0);
}
