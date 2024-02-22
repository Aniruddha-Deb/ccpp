#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

enum Operator {
    ADD, SUBTRACT, MULTIPLY, DIVIDE, EQUALTO
};

enum LiteralType {
    L_INTEGER, L_FLOAT, L_IDENTIFIER 
};

class ASTNode {};

class ASTType : public ASTNode {
    public:
    string name; // for now. Will be nested later to handle pointer indirection

    ASTType(string n): name{n} {}
};

class ASTExpression : public ASTNode {};

class ASTBinaryExpression : public ASTExpression {
    public:
    Operator op;
    ASTExpression* lhs;
    ASTExpression* rhs;

    ASTBinaryExpression(ASTExpression* lhs, Operator op, ASTExpression* rhs) {
        this->lhs = lhs;
        this->op = op;
        this->rhs = rhs;
    }
};

class ASTLiteral : public ASTExpression {
    public:
    string value;
    LiteralType literal_type;

    ASTLiteral(string value, LiteralType lt) {
        this->value = value;
        this->literal_type = lt;
    }
};

class ASTStatement : public ASTNode {};

class ASTExpressionStatement : public ASTStatement {
    public:
    ASTExpression* expr;

    ASTExpressionStatement(ASTExpression* expr) {
        this->expr = expr;
    }
};

class ASTBlockStatement : public ASTStatement {
    public:
    vector<ASTStatement*> statements;

    ASTBlockStatement(ASTStatement* first) {
        statements.push_back(first);
    }

    void add_statement(ASTStatement* param) {
        cout << "Adding statement " << ((ASTExpressionStatement*)param)->expr << std::endl;
        statements.push_back(param);
    }
};

class ASTParameter : public ASTNode {
    public:
    string name;
    ASTType* type;

    ASTParameter(ASTType* type, string name) {
        cout << "Making ASTParameter from " << type->name << " " << name << endl;
        this->type = type;
        this->name = name;
    }
};

class ASTParameterList : public ASTNode {
    public:
    vector<ASTParameter*> params;

    ASTParameterList(ASTParameter* first) {
        cout << "Making ASTParameterList from first " << first->type->name << " " << first->name << endl;
        params.push_back(first);
    }

    void add_parameter(ASTParameter* param) {
        params.push_back(param);
    }
};

class ASTFunction : public ASTNode {
    public:
    string name;
    ASTType* return_type;
    ASTParameterList* parameters;
    ASTBlockStatement* statement_block;

    ASTFunction(ASTType* ret_type, string name, ASTParameterList* params, ASTBlockStatement* stmts) {
        cout << "Making ASTFunction " << ret_type->name << " " << name << endl;
        this->return_type = ret_type;
        this->name = name;
        this->parameters = params;
        this->statement_block = stmts;
    }
};

class ASTTranslationUnit : public ASTNode {
    public:
    vector<ASTFunction*> m_functions;

    ASTTranslationUnit(ASTFunction* fn) {
        cout << "Making ASTTranslationUnit from function " << fn->name << endl;
        m_functions.push_back(fn);
    }

    void add_function(ASTFunction* fn) {
        cout << "Adding function " << fn->name << std::endl;
        m_functions.push_back(fn);
    }
};
