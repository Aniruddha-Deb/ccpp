#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "ast.hpp"
#include "c.tab.hpp"

extern "C" int yylex();
int yyparse(void* tu);
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
  ast::TranslationUnit** tu_ptr;
  int ret = yyparse(tu_ptr);
  ast::TranslationUnit* tu = *tu_ptr;

  std::cout << std::endl;

  /*
  for (auto f : tu->m_functions) {
      std::cout << "function: " << f->return_type->name << " " << f->name << std::endl;
      std::cout << "params:" << std::endl;
      for (auto p : f->parameters->params) {
          std::cout << p->type->name << " " << p->name << std::endl;
      }
      std::cout << "statements:" << std::endl;
      std::cout << f->statement_block->statements.size() << std::endl;
  }
  */
  printf("retv = %d\n", ret);
  exit(0);
}
