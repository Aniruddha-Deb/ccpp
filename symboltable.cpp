#include <iostream>
#include "symboltable.hpp"

using namespace std;

void symboltable::enter_scope(){
  // cout<<"ENTERSCOPE---"<<endl;
  unordered_map<string, int> newscope;
  scopestack.push(newscope);
}

void symboltable::exit_scope(){
  // cout<<"EXITSCOPE\n";
  if(scopestack.empty()){
    cout<<"PANIC EXIT_SCOPE\n"<<endl;
  }
  else{
    scopestack.pop();
  }
}

bool symboltable::check_scope(string x){
  // cout<<"CHECK:"<<x<<"->"<<endl;
  if(scopestack.empty()){
    cout<<"PANIC CHECK_SCOPE\n";
    return false; // can remove ig
  }
  else{
    auto topscope = scopestack.top();
    return (topscope.find(x) != topscope.end());
  }
}

int symboltable::find_symbol(string x){
  // cout<<"FIND:"<<x<<"->"<<endl;
  auto copystack = scopestack;
  while(!copystack.empty()){
    auto topscope = copystack.top();
    if(topscope.find(x) != topscope.end()){
      return topscope[x];
    }
    copystack.pop();
  }
  return -1;
}

void symboltable::add_symbol(string x, int location){
  // cout<<"ADD:"<<x<<"->"<<endl;
  if(check_scope(x)){
    cout<<"PANIC ADD_SYMBOL\n";
  }
  else{
    scopestack.top()[x] = location;
  }
}


// int main(){
//   symboltable* s = new symboltable;
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




