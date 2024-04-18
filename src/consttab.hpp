#ifndef CONSTTABLE
#define CONSTTABLE

#include <unordered_map>
#include <stack>
#include <string>
#include "ast.hpp"

using namespace std;

struct constant_info{
    bool pointer_exists = false;
    ast::Literal* Value = nullptr;
};

class ConstTable {
private:
    std::unordered_map<int, constant_info> value_map;

public:
    // ConstTable();
    void update_value(int idx, ast::Literal* value);
    void invalidate_value(int idx);
    void pointer_taken(int idx);
    ast::Literal* get_value(int idx);
    bool is_constant(int idx);
    void clear_values();
    void clear_table();
};

#endif 

