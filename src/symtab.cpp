#include <iostream>
#include "symtab.hpp"
#include "error.hpp"

using namespace std;

SymbolTable::SymbolTable(): scopestack(), curr_symb_identifier{0}, depth{0} {}

void SymbolTable::enter_scope(){
  // cout<<"ENTERSCOPE---"<<endl;
  depth++;
  unordered_map<string, SymbolInfo> newscope;
  scopestack.push(newscope);
}

void SymbolTable::exit_scope(){
  // cout<<"EXITSCOPE\n";
  depth--;
  if(scopestack.empty()){
    ehdl::err("Scope stack is empty while exiting scope", {});
  }
  else{
    scopestack.pop();
  }
}

bool SymbolTable::check_scope(string x){
  // cout<<"CHECK:"<<x<<"->"<<endl;
  if(scopestack.empty()){
    ehdl::err("Scope stack is empty while checking scope", {});
    return false; // can remove ig
  }
  else{
    auto topscope = scopestack.top();
    return (topscope.find(x) != topscope.end());
  }
}

SymbolInfo SymbolTable::find_symbol(string x){
  // cout<<"FIND:"<<x<<"->"<<endl;
  auto copystack = scopestack;
  while(!copystack.empty()){
    auto topscope = copystack.top();
    if(topscope.find(x) != topscope.end()){
      return topscope[x];
    }
    copystack.pop();
  }
  return {-1, 0, UNK};
}

void SymbolTable::reset_symb_identifier() {
    curr_symb_identifier = 0;
}

void SymbolTable::add_symbol(string x, SymbolInfo info){
  if(check_scope(x)){
    ehdl::err("Symbol " + x + " already exists in scope", {});
  }
  else{
    if(depth == 1){
       info.idx = -1-curr_symb_identifier;
    }
    else{
      info.idx = curr_symb_identifier;
    }
    scopestack.top()[x] = info;
    curr_symb_identifier++;
  }
}

// int main(){
//   SymbolTable* s = new SymbolTable;
//   s->enter_scope();
//   s->add_symbol("HI", 0);
//   cout<<s->check_scope("HI");
//   cout<<s->check_scope("BYE");
//   s->enter_scope();
//   s->add_symbol("BYE", 5);
//   s->add_symbol("HI", 10);
//   cout<<s->find_symbol("HI");
//   cout<<s->check_scope("BYE");
//   s->exit_scope();
//   cout<<s->find_symbol("HI");
// }




