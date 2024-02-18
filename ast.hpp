#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

enum Operator {
    ADD, SUBTRACT, MULTIPLY, DIVIDE
};

class ASTNode {};

class ASTType {
    public:
    string name; // for now. Will be nested later to handle pointer indirection

    ASTType(string n): name{n} {}
};

class ASTExpression : ASTNode {
    Operator op;
    ASTExpression* lhs;
    ASTExpression* rhs;
};

class ASTStatement {};

class ASTBlockStatement : ASTStatement {
    vector<ASTStatement> statements;
};

class ASTExpressionStatement : ASTStatement {
    ASTExpression* expr;
};

class ASTParameter : ASTNode {
    public:
    string name;
    ASTType* type;

    ASTParameter(ASTType* type, string name) {
        cout << "Making ASTParameter from " << type->name << " " << name << endl;
        this->type = type;
        this->name = name;
    }
};

class ASTParameterList : ASTNode {
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

class ASTFunction : ASTNode {
    public:
    string name;
    ASTType* return_type;
    ASTParameterList* parameters;
    ASTStatement* statement_block;

    ASTFunction(ASTType* ret_type, string name, ASTParameterList* params) { // , ASTStatement* stmts) {
        cout << "Making ASTFunction " << ret_type->name << " " << name << endl;
        this->return_type = ret_type;
        this->name = name;
        this->parameters = params;
        this->statement_block = nullptr;
    }
};

class ASTTranslationUnit : ASTNode {
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
