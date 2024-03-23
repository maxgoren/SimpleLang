#ifndef interpreter_hpp
#define interpreter_hpp
#include <iostream>
#include "parser.hpp"
#include "symboltable.hpp"
using namespace std;

class Interpreter {
    private:
        SymbolTable st;
        string eval(ASTNode* node);
        string expression(ASTNode* node);
        void printStmt(ASTNode* node);
        void ifStmt(ASTNode* node);
        void loopStmt(ASTNode* node);
        void assignStmt(ASTNode* node);
        void statement(ASTNode* node);
    public:
        Interpreter();
        void run(ASTNode* node);
};

#endif