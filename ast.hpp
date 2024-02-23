#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

namespace ast {
enum Operator {
    ADD, SUB, MUL, DIV, ASSIGN
};

enum LiteralType {
    L_INTEGER, L_FLOAT, L_IDENTIFIER 
};

struct Node {};

struct Type : Node {
    string name;
};

struct Expression : Node {};

struct BinaryExpression : Expression {
    Expression* lhs;
    Operator op;
    Expression* rhs;
};

struct Literal : Expression {
    string value;
};

struct Reference : Expression {
    string value;
};

struct Statement : Node {};

struct ExpressionStatement : Statement {
    Expression* expr;
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
