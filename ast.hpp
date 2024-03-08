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
    virtual string dump_ast(int depth);
};

struct Type : Node {
    string name;
};

struct Expression : Node {};

struct Identifier : Expression {
    string name;
    // TODO add symbol table references here 
};

// there's only one kind of ternary expression (the conditional), so no need to 
// specify operators
struct TernaryExpression : Expression {
    Expression* cond;
    Expression* true_branch;
    Expression* false_branch;

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- op: ?: (ternary operator)\n" +
               pad_str + "`- cond:\n" + cond->dump_ast(depth+2) +
               pad_str + "`- true_branch:\n" + true_branch->dump_ast(depth+2) +
               pad_str + "`- false_branch:\n" + false_branch->dump_ast(depth+2) +
               "\n";
    }
};

struct TypecastExpression : Expression {
    Type* typ;
    Expression* expr;

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

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- op: " + op2str(op) + "\n" +
               pad_str + "`- type:\n" + typ->dump_ast(depth+2) +
               "\n";
    }
};

struct UnaryExpression : Expression { 
    Operator op;
    Expression* expr;

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- op: " + op2str(op) + "\n" +
               pad_str + "`- expr:\n" + expr->dump_ast(depth+2) +
               "\n";
    }
};

struct Literal : Expression {
    string value;
    LiteralType ltype;
    // TODO parse literal

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- literal: " + value + " (" + to_string(ltype) + ")\n";
    }
};

struct Reference : Expression {
    string value;

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- reference: " + value + "\n";
    }
};

struct Statement : Node {};

struct ExpressionStatement : Statement {
    Expression* expr;

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- expr:\n" + expr->dump_ast(depth+2) + "\n";
    }
};

struct Declaration : Node {
    Type* type;
    string name;

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- type:\n" + type->dump_ast(depth+2) +
               pad_str + "`- name: " + name + "\n";
    }
};

struct DeclarationStatement : Statement {
    Declaration* decl;

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
    Type* type;
    string name;

    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        return pad_str + "`- type:\n" + type->dump_ast(depth+2) +
               pad_str + "`- name: " + name + "\n";
    }
};

struct Function : Node {
    Type* return_type;
    string name;
    vector<Parameter*>* parameters;
    BlockStatement* statement_block;

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
        return result + "\n";
    }
};

struct TranslationUnit : Node, vector<Function*> {
    string dump_ast(int depth) {
        string pad_str(depth, ' ');
        string result = pad_str + "`- translation unit:\n";
        for (Function* func : *this) {
            result += func->dump_ast(depth+2);
        }
        return result + "\n";
    }
};
} // namespace ast
