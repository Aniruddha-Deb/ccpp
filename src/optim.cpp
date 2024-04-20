#include "symtab.hpp"
#include <map>
#include "ast.hpp"
#include "debug.hpp"
#include "consttab.hpp"

namespace ast {

ConstTable constant_table;

Literal* LiteralCopy(Literal* src){
    Literal* dest = new Literal("", LT_STRING);
    dest->data = src->data;
    dest->ltype = src->ltype;
    dest->value = src->value;
    return dest;
}

Expression* Expression::const_prop(){
    // cout << "virtual" <<endl;
    return nullptr;
}

Statement* Statement::const_prop(){
    // cout << "virtual const prop statement" <<endl;
    return nullptr;
}

Statement* BlockStatement::const_prop(){
    for(int i = 0; i  < this->size(); i++) {
        (*this)[i] = ((*this)[i])->const_prop();
    }
    return this;
}

Statement* IfStatement::const_prop(){
    Expression* exp = cond->const_prop();
    // cond = exp;    // delete old exp?
    cond = FoldConstants(exp);
    // constant_table.clear_values();
    true_branch = true_branch->const_prop();
    
    if (false_branch) {
        constant_table.clear_values();
        false_branch = false_branch->const_prop();
    }
    
    constant_table.clear_values();
    return this;
}

Statement* WhileStatement::const_prop(){
    constant_table.clear_values();
    Expression* exp = cond->const_prop();
    // cond = exp;    // delete old exp?
    cond = FoldConstants(exp);
    // constant_table.clear_values();
    stmt = stmt->const_prop();
    constant_table.clear_values();
    return this;
}

Statement* ExpressionStatement::const_prop(){
    // BlockStatement* b = new BlockStatement();
    // if (BinaryExpression* binexp = dynamic_cast<BinaryExpression*> (expr)){
    //     if (binexp->op == OP_ASSIGN) {
    //         expr = expr->flatten_tree(b);
    //     }
    //     else{
    //         expr = expr->flatten_tree(b);
    //         b->push_back(this);
    //     }
    // }
    // else {
    //     expr = expr->flatten_tree(b);
    //     b->push_back(this);
    // }

    // // cout << b->dump_ast("") << endl;

    // for(int i = 0; i < (b->size())/2; i++){
    //     Statement* temp = (*b)[i];
    //     (*b)[i] = (*b)[b->size() - i - 1]; 
    //     (*b)[b->size() - i - 1] = temp;
    // }

    // for(int i = 0; i < b->size(); i++){
    //     ExpressionStatement* es = dynamic_cast<ExpressionStatement*>((*b)[i]);
    //     es->expr = es->expr->const_prop();
    //     es->expr = FoldConstants(es->expr);
    //     cout << "set" << endl;
    // }

    
    expr = expr->const_prop();  // delete old exp?
    
    expr = FoldConstants(expr);

    return this;
}



Statement* DeclarationStatement::const_prop(){
    for(int i = 0; i < (decl->decl_list)->size(); i++){
        if((*(decl->decl_list))[i]->init_expr){
             // cout << "Const ";
            Expression* exp = (*(decl->decl_list))[i]->init_expr->const_prop();
             // cout << "Const ";
            // delete old exp? 
            // dec->init_expr = exp;
            
            (*(decl->decl_list))[i]->init_expr = FoldConstants(exp);

            // cout<<"WHAT"<<endl;
            if (dynamic_cast<Literal*> ((*(decl->decl_list))[i]->init_expr)){
                // cout << "Const ";
                cout << (*decl->decl_list)[i]->ident->ident_info.idx << endl;
            }
            constant_table.update_value((*decl->decl_list)[i]->ident->ident_info.idx, dynamic_cast<Literal*> ((*decl->decl_list)[i]->init_expr) );
        }
    }
    return this;
}

Statement* ReturnStatement::const_prop(){
    if (ret_expr) {
        Expression* exp = ret_expr->const_prop(); // delete old exp?
        ret_expr = exp;
        ret_expr = FoldConstants(ret_expr);
        if (ret_expr) {
            // cout << "return pointer exists" << endl;
        }
    }
    return this;
}

Expression* Identifier::const_prop(){
    if (constant_table.is_constant(ident_info.idx)){
        // cout<<"Found constant"<<endl;
        Literal* l = dynamic_cast<Literal*>((constant_table.get_value(ident_info.idx))->copy_exp());
        if(!l){
            // cout << "something is wrong" << endl;
        }
        return l;
    }
    return this;   // copy current exp and return so old version can be deleted
}

Expression* BinaryExpression::const_prop(){
    if (op == OP_ASSIGN) {
        Expression* newrhs = rhs->const_prop();
        rhs = FoldConstants(newrhs);
        // if (rhs->const_value) {
        //     const_value = rhs->const_value;
        // }
        // rhs = newrhs;
        Identifier* ident = dynamic_cast<Identifier*> (lhs);
        if (ident) {
            constant_table.update_value(ident->ident_info.idx, dynamic_cast<Literal*> (rhs) );
        }
        else{
            lhs = lhs->const_prop();       // not sure
        }
        return this;

    }                                // check for all assignment types
    Expression* newlhs = lhs->const_prop();
    Expression* newrhs = rhs->const_prop();   // delete old expressions
    if (newrhs && newlhs) {
        // cout << "Both exist\n";
    } 
    lhs = newlhs;
    rhs = newrhs;
    return  this;  // copy current exp and return so old version can be deleted
}

Expression* UnaryExpression::const_prop(){
    if(op == OP_AND){                           // check for all assignment types
        // cout<<"POINTER" <<endl;
        Identifier* ident = dynamic_cast<Identifier*> (expr);
        if (ident) {
            constant_table.pointer_taken(ident->ident_info.idx);
        }
        else{
            expr = expr->const_prop();
        }
        return this;
    }
    else{
        Expression* exp = expr->const_prop();
        expr = exp;
        return this;   // copy current exp and return so old version can be deleted
    }
}

Expression* FunctionInvocationExpression::const_prop(){
    if (params) {
        for(int i = 0; i < params->size(); i++){
            Expression* exp = (*params)[i]->const_prop();
            (*params)[i] = exp;
            (*params)[i] = FoldConstants(exp);
        }
    }
    return this;
}

void TranslationUnit::const_prop() {
  
  Function* func_ptr;
  for (auto node_ptr: *nodes){
    if ((func_ptr = dynamic_cast<Function*>(node_ptr))) {
      constant_table.clear_table();
      func_ptr->const_prop();
    }
  }

}

void Function::const_prop(){
    if (params) {
        for(int i = 0; i < (params->params)->size(); i++){
            constant_table.update_value((*(params->params))[i]->ident->ident_info.idx, nullptr);
        }
    }
    
    if (this->stmts) {
        stmts->const_prop();
    }
}

Expression* Literal::const_prop(){
    return this;
}



// flatten_tree

// Expression* Expression::flatten_tree(Statement* b){
//     cout << "virtual" << endl;
//     return nullptr;
// }

Expression* Identifier::flatten_tree(Statement* b){
    return this;
}

Expression* Literal::flatten_tree(Statement* b){
    return this;
}

Expression* UnaryExpression::flatten_tree(Statement* b){
    expr = expr->flatten_tree(b);
    return this;
}

Expression* FunctionInvocationExpression::flatten_tree(Statement* b){
    fn = fn->flatten_tree(b);

    if(params){
        for(int i = params->size() - 1; i >= 0; i --){
            (*params)[i] = (*params)[i]->flatten_tree(b);
        }
    }

    return this;
}

Expression* BinaryExpression::flatten_tree(Statement* b){
    // if (left_assoc_op)
    if (op == OP_ASSIGN) {
        BlockStatement* bs = dynamic_cast<BlockStatement*>(b);
        lhs = lhs->flatten_tree(b);
        rhs = rhs->flatten_tree(b);
        bs->push_back(new ExpressionStatement(this));
        // cout << dump_ast("") << endl;
        return rhs->copy_exp();
    }
    else{
        rhs = rhs->flatten_tree(b);
        lhs = lhs->flatten_tree(b);
        return this;
    }
}

Expression* Expression::flatten_tree(Statement* b){
    // cout << "virtual f tree" << endl;
    return nullptr;
}


}
