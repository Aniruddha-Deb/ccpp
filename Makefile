# -lfl doesn't work on MacOS! https://stackoverflow.com/questions/21298097/library-not-found-for-lfl
LDFLAGS=-lm -ll
BISON=/opt/homebrew/opt/bison/bin/bison
OS := $(shell uname)
ifneq ($(OS),Darwin)
	LDFLAGS += -lfl
	BISON=bison
endif

DEBUG=-DDEBUG

SRC=src/cc.cpp src/c.tab.cpp src/c.lex.cpp src/ast.cpp src/symtab.cpp src/dump_ast.cpp # src/scopify.cpp
OBJ=$(patsubst src/%.cpp, bin/%.o, $(SRC))

cc: src/c.tab.cpp src/c.lex.cpp $(OBJ)
	g++ -std=c++17 $(OBJ) $(LDFLAGS) $(DEBUG) -o $@

bin/%.o: src/%.cpp
	g++ -std=c++17 -c $< $(DEBUG) -o $@

src/c.tab.cpp: src/c.y
	$(BISON) -t -o src/c.tab.cpp -d $<

src/c.lex.cpp: src/c.l src/c.tab.hpp
	flex -o src/c.lex.cpp -l src/c.l

clean:
	rm -f bin/*
	rm src/c.tab.* src/c.lex.*
