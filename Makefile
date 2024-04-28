# -lfl doesn't work on MacOS! https://stackoverflow.com/questions/21298097/library-not-found-for-lfl
CC:=clang
CPPC:=/opt/homebrew/opt/llvm@14/bin/clang++
LLI:=/opt/homebrew/opt/llvm@14/bin/lli
LDFLAGS:=-lm -ll -L/opt/homebrew/opt/llvm@14/lib/ -lLLVM-14
INCLUDE:=-Iinclude -I/opt/homebrew/opt/llvm@14/include/
BISON:=/opt/homebrew/opt/bison/bin/bison
OS := $(shell uname)
ifneq ($(OS),Darwin)
	CPPC:=g++
	CC:=gcc
	LLI:=lli
	LDFLAGS := -lm -ll -lfl -L/usr/lib/llvm-14/lib -lLLVM-14
	INCLUDE:=-Iinclude -I/usr/lib/llvm-14/include
	BISON:=bison
endif

DEBUG=#-DDEBUG

SRC:=src/cc.cpp src/c.tab.cpp src/c.lex.cpp src/ast.cpp src/symtab.cpp src/dump_ast.cpp src/codegen.cpp src/scopify.cpp src/error.cpp src/consttab.cpp src/optim.cpp
OBJ:=$(patsubst src/%.cpp, bin/%.o, $(SRC))
TEST:=$(shell find examples -name '*.c' -maxdepth 1)
TESTOBJ:=$(patsubst examples/%.c, test/clang/%, $(TEST))
TESTLL:=$(patsubst examples/%.c, test/cc/%.ll, $(TEST))
RED:=\033[0;31m
BOLD:=\033[1m
END:=\033[0m
GREEN:=\033[0;32m
LINE:="-----------------------------------------------------------------------"

cc: mkbindir src/c.tab.cpp src/c.lex.cpp $(OBJ)
	$(CPPC) -std=c++17 $(OBJ) $(INCLUDE) $(LDFLAGS) $(DEBUG) -o $@

test: cleantest cc $(TESTOBJ) $(TESTLL)

submit:
	mkdir -p final
	rm -rf final/*
	cp -r src final/
	cp -r examples final/
	cp -r include final/
	cp Makefile final/
	cp README.md final/
	zip -r 2020CS10869_2021CS10074 final

test/clang/%: examples/%.c
	$(CC) -std=c11 $< -o $@

test/cc/%.ll: examples/%.c test/clang/%
	@echo "Testing $@"
	@./cc $< -o $@
	@if [ "`$(LLI) $@`" != "`./$(word 2,$^)`" ]; then \
		echo "$(RED)$(BOLD)[X] $@$(END)"; \
		echo $(LINE); \
	else \
		echo "$(GREEN)$(BOLD)[.] $@$(END)"; \
		echo $(LINE); \
	fi

test_literal: bin/test_literal.o bin/ast.o bin/dump_ast.o bin/codegen.o bin/scopify.o bin/symtab.o
	$(CPPC) -std=c++17 $^ $(INCLUDE) $(LDFLAGS) $(DEBUG) -o $@

bin/%.o: src/%.cpp
	$(CPPC) -std=c++17 -c $< $(INCLUDE) $(DEBUG) -o $@

src/c.tab.cpp: src/c.y
	$(BISON) -t -o src/c.tab.cpp -d $<

src/c.lex.cpp: src/c.l src/c.tab.hpp
	flex -o src/c.lex.cpp -l src/c.l

mkbindir:
	mkdir -p bin

mktestdir:
	mkdir -p test test/cc test/clang

cleantest: mktestdir
	rm -f test/cc/* test/clang/*

clean: cleantest
	rm -f bin/*
	rm -f src/c.tab.* src/c.lex.*

.PHONY: test clean
