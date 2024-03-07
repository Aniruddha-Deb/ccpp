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

struct Node {};

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
};

struct TypecastExpression : Expression {
    Type* typ;
    Expression* expr;
};

struct TypeExpression: Expression {
    Operator op;
    Type* typ;
};

struct BinaryExpression : Expression {
    Expression* lhs;
    Operator op;
    Expression* rhs;
};

struct UnaryExpression : Expression { 
    Operator op;
    Expression* expr;
};

struct Literal : Expression {
    string value;
    LiteralType ltype;
    // TODO parse literal
};

struct Reference : Expression {
    string value;
};

struct Statement : Node {};

struct ExpressionStatement : Statement {
    Expression* expr;
};

struct Declaration : Node {
    Type* type;
    string name;
};

struct DeclarationStatement : Statement {
    Declaration* decl;
};

struct BlockStatement : Statement, vector<Statement*> {};

struct Parameter : Node {
    Type* type;
    string name;
};

struct Function : Node {
    Type* return_type;
    string name;
    vector<Parameter*>* parameters;
    BlockStatement* statement_block;
};

struct TranslationUnit : Node, vector<Function*> {};
} // namespace ast
