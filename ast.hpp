#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

namespace ast {
enum Operator {
    // arithmetic
    OP_ADD, 
    OP_SUB, 
    OP_MUL, 
    OP_DIV, 
    OP_MOD,
    OP_UNARY_PLUS,
    OP_UNARY_MINUS,
    OP_PRE_INCR,
    OP_PRE_DECR,
    OP_POST_INCR,
    OP_POST_DECR,

    // types
    OP_DEREF,
    OP_SIZEOF,
    OP_ALIGNOF,
    OP_TYPECAST,

    // logical
    OP_BOOL_AND,
    OP_BOOL_OR,
    OP_BOOL_NOT,

    // relational
    OP_EQ,
    OP_GE,
    OP_GT,
    OP_LT,
    OP_LE,
    OP_NE,

    // bitwise
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_NOT,
    OP_LSHIFT,
    OP_RSHIFT,

    // assignment
    OP_ASSIGN,
    OP_MUL_ASSIGN,
    OP_DIV_ASSIGN,
    OP_MOD_ASSIGN,
    OP_ADD_ASSIGN,
    OP_SUB_ASSIGN,
    OP_LEFT_ASSIGN,
    OP_RIGHT_ASSIGN,
    OP_AND_ASSIGN,
    OP_XOR_ASSIGN,
    OP_OR_ASSIGN,

    // other
    OP_SEQ
};

enum LiteralType {
    LT_INT64, LT_UINT64, LT_INT32, LT_FLOAT, LT_DOUBLE,
    LT_INT_LIKE, LT_FLOAT_LIKE, 
    LT_STRING
};

string op2str(Operator op);

struct Node {
    virtual string dump_ast(int depth) = 0;
};

struct Type : Node {
    string name;

    Type(string _name) : name(_name) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- type: " + name + "\n";
    }
};

struct Expression : Node {};

struct Identifier : Expression {
    string name;
    // TODO add symbol table references here 
    
    Identifier(string _name) : name(_name) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- id: " + name + "\n";
    }
};

// there's only one kind of ternary expression (the conditional), so no need to 
// specify operators
struct TernaryExpression : Expression {
    Expression* cond;
    Expression* true_branch;
    Expression* false_branch;

    TernaryExpression(Expression* _cond, Expression* _true_branch, Expression* _false_branch) : 
        cond(_cond), true_branch(_true_branch), false_branch(_false_branch) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- op: ?: (ternary operator)\n" +
               pad_str + "`- cond:\n" + cond->dump_ast(depth+2) +
               pad_str + "`- true_branch:\n" + true_branch->dump_ast(depth+2) +
               pad_str + "`- false_branch:\n" + false_branch->dump_ast(depth+2);
    }
};

struct TypecastExpression : Expression {
    Type* typ;
    Expression* expr;

    TypecastExpression(Type* _typ, Expression* _expr) : typ(_typ), expr(_expr) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- op: (type) (type cast)\n" +
               pad_str + "`- type:\n" + typ->dump_ast(depth+2) +
               pad_str + "`- expr:\n" + expr->dump_ast(depth+2) +
               "\n";
    }
};

struct TypeExpression: Expression {
    Operator op;
    Type* typ;

    TypeExpression(Operator _op, Type* _typ) : op(_op), typ(_typ) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- op: " + op2str(op) + "\n" +
               pad_str + "`- type:\n" + typ->dump_ast(depth+2);
    }
};

struct BinaryExpression : Expression { 
    Expression* lhs;
    Operator op;
    Expression* rhs;

    BinaryExpression(Expression* _lhs, Operator _op, Expression* _rhs) : lhs(_lhs), op(_op), rhs(_rhs) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- op: " + op2str(op) + "\n" +
               pad_str + "`- lhs:\n" + lhs->dump_ast(depth+2) +
               pad_str + "`- rhs:\n" + rhs->dump_ast(depth+2);
    }
};

struct UnaryExpression : Expression { 
    Operator op;
    Expression* expr;

    UnaryExpression(Operator _op, Expression* _expr) : op(_op), expr(_expr) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- op: " + op2str(op) + "\n" +
               pad_str + "`- expr:\n" + expr->dump_ast(depth+2);
    }
};

struct Literal : Expression {
    string value;
    LiteralType ltype;

    Literal(string _value, LiteralType _ltype): value(_value), ltype(_ltype) {
        // TODO parse literal to int/float etc
    }

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- literal: " + value + " (" + to_string(ltype) + ")\n";
    }
};

struct Statement : Node {};

struct ExpressionStatement : Statement {
    Expression* expr;

    ExpressionStatement(Expression* _expr): expr(_expr) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- expr:\n" + expr->dump_ast(depth+2) + "\n";
    }
};

struct Declaration : Node {
    Type* typ;
    string name;

    Declaration(Type* _typ, string _name) : typ(_typ), name(_name) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- type:\n" + typ->dump_ast(depth+2) +
               pad_str + "`- name: " + name + "\n";
    }
};

struct DeclarationStatement : Statement {
    Declaration* decl;

    DeclarationStatement(Declaration* _decl) : decl(_decl) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- decl:\n" + decl->dump_ast(depth+2) + "\n";
    }
};

struct BlockStatement : Statement, vector<Statement*> {
    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        string result = pad_str + "`- block:\n";
        for (Statement* stmt : *this) {
            result += stmt->dump_ast(depth+2);
        }
        return result + "\n";
    }
};

struct Parameter : Node {
    Type* typ;
    string name;

    Parameter(Type* _typ, string _name) : typ(_typ), name(_name) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- type:\n" + typ->dump_ast(depth+2) +
               pad_str + "`- name: " + name + "\n";
    }
};

struct Function : Node {
    Type* return_type;
    string name;
    vector<Parameter*>* parameters;
    BlockStatement* statement_block;

    Function(Type* _return_type, string _name, vector<Parameter*>* _parameters, BlockStatement* _stmts):
        return_type(_return_type), name(_name), parameters(_parameters), statement_block(_stmts) {}

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        string result = pad_str + "`- function: " + name + "\n" +
                        pad_str + "`- return_type:\n" + return_type->dump_ast(depth+2);
        if (parameters) {
            result += pad_str + "`- parameters:\n";
            for (Parameter* param : *parameters) {
                result += param->dump_ast(depth+4);
            }
        }
        result += pad_str + "`- body:\n" + statement_block->dump_ast(depth+2);
        return result;
    }
};

struct TranslationUnit : Node, vector<Function*> {

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        string result = pad_str + "translation unit:\n";
        for (Function* func : *this) {
            result += func->dump_ast(depth);
        }
        return result + "\n";
    }
};
} // namespace ast
