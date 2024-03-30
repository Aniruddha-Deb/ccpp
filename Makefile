# -lfl doesn't work on MacOS! https://stackoverflow.com/questions/21298097/library-not-found-for-lfl
CCFLAGS=-lm -ll
BISON=/opt/homebrew/opt/bison/bin/bison
OS := $(shell uname)
ifneq ($(OS),Darwin)
	CCFLAGS += -lfl
	BISON=bison
endif

SRC=src/cc.cpp src/c.tab.cpp src/c.lex.cpp src/ast.cpp src/symtab.cpp src/dump_ast.cpp src/scopify.cpp

cc: src/c.tab.cpp src/c.lex.cpp
	g++ -std=c++17 $(SRC) $(CCFLAGS) -o $@

src/c.tab.cpp: src/c.y
	$(BISON) -t -o src/c.tab.cpp -d $<

src/c.lex.cpp: src/c.l src/c.tab.hpp
	flex -o src/c.lex.cpp -l src/c.l

clean:
	rm -f bin/*
	rm src/c.tab.* src/c.lex.*
