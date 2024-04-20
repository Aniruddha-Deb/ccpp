#include <iostream>
#include "consttab.hpp"

using namespace std;

ast::Literal* ConstTable::get_value(int idx){
    if(value_map.find(idx) != value_map.end()){
        if (value_map[idx].pointer_exists){
            return nullptr;
        }
        return value_map[idx].Value;
    }
    return nullptr;
}

void ConstTable::clear_values(){
    for(auto i: value_map){
        i.second.Value = nullptr;
    }
}


void ConstTable::clear_table(){
    value_map.clear();
}

void ConstTable::invalidate_value(int idx){
    if(value_map.find(idx) != value_map.end()){
        value_map[idx].Value = nullptr;
    }

}


void ConstTable::pointer_taken(int idx){
    // cout << "VALUE REMOVED" << endl;
    if(value_map.find(idx) != value_map.end()){
        value_map[idx].Value = nullptr;
        value_map[idx].pointer_exists = 1;
    }
}

bool ConstTable::is_constant(int idx){
    if(value_map.find(idx) != value_map.end()){
        // cout<<"FOUND\n";
        if (value_map[idx].pointer_exists){
            return false;
        }
        if (value_map[idx].Value){
            // cout<<"FOUND\n";
            return true;
        }
    }
    return false;
}

void ConstTable::update_value(int idx, ast::Literal* value){
    if(value_map.find(idx) != value_map.end()){
        if (!value_map[idx].pointer_exists){
            value_map[idx].Value = value;
            return;
        }
        else{
            value_map[idx].Value = nullptr;
            return;
        }
    }
    value_map[idx].Value = value;
    return;
}
