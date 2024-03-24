#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include <unordered_map>
#include <stack>
#include <string>

using namespace std;

class symboltable {
private:
    std::stack<std::unordered_map<std::string, int>> scopestack;

public:
    void enter_scope();
    void exit_scope();
    void add_symbol(string x, int location);
    int find_symbol(string x);
    bool check_scope(string x);
};

#endif 

