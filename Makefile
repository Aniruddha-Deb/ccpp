# -lfl doesn't work on MacOS! https://stackoverflow.com/questions/21298097/library-not-found-for-lfl
CCFLAGS=-lm -ll
BISON=/opt/homebrew/opt/bison/bin/bison
OS := $(shell uname)
ifneq ($(OS),Darwin)
	CCFLAGS += -lfl
	BISON=bison
endif

cc: cc.cpp c.tab.cpp c.lex.cpp ast.hpp ast.cpp symboltable.cpp symboltable.hpp
	g++ -std=c++17 c.tab.cpp c.lex.cpp ast.cpp cc.cpp symboltable.cpp $(CCFLAGS) -o $@

c.tab.cpp c.tab.hpp: c.y
	$(BISON) -t -o c.tab.cpp -d c.y

c.lex.cpp: c.l c.tab.hpp
	flex -o c.lex.cpp -l c.l

clean::
	rm -f c.tab.cpp c.tab.hpp c.lex.cpp cc c.output
