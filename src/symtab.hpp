#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include <unordered_map>
#include <stack>
#include <string>

using namespace std;

enum SymbolType {
    FP32,
    FP64,
    I1,
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
    PTR,
    FUNC,
    UNK
};

struct SymbolInfo {
    int idx;
    int ptr_depth;
    SymbolType stype;
};

class SymbolTable {
private:
    std::stack<std::unordered_map<std::string, SymbolInfo>> scopestack;
    int curr_symb_identifier;
     int depth;

public:
    SymbolTable();
    void enter_scope();
    void exit_scope();
    void add_symbol(string x, SymbolInfo info);
    void reset_symb_identifier();
    SymbolInfo find_symbol(string x);
    bool check_scope(string x);
};

#endif 

