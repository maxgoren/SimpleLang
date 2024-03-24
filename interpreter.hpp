#ifndef interpreter_hpp
#define interpreter_hpp
#include <iostream>
#include <map>
#include "parser.hpp"
#include "symboltable.hpp"
using namespace std;


struct Procedure {
    string name;
    ASTNode* paramList;
    ASTNode* functionBody;
};

struct ActivationRecord {
    Procedure* function;
    SymbolTable env;
    string returnValue;
};


class CallStack {
    private:
        ActivationRecord* stack[255];
        int p;
    public:
        CallStack() {
            p = 0;
        }
        bool empty() {
            return p == 0;
        }
        void push(ActivationRecord* ar) {
            stack[p++] = ar;
        }
        void pop() {
            ActivationRecord* t = top();
            p--;
            delete t;
        }
        ActivationRecord* top() {
            return stack[p-1];
        }
};

class Interpreter {
    private:
        bool stopProcedure;
        int recDepth;
        void enter(string s);
        void leave(string s);
        void leave();
        void say(string s);
        SymbolTable st;
        CallStack callStack;
        map<string, Procedure*> procedures;
        ActivationRecord* prepareActivationRecord(ASTNode* node);
        string procedureCall(ASTNode* node);
        string eval(ASTNode* node);
        string expression(ASTNode* node);
        void returnStmt(ASTNode* node);
        void printStmt(ASTNode* node);
        void ifStmt(ASTNode* node);
        void loopStmt(ASTNode* node);
        void assignStmt(ASTNode* node);
        void statement(ASTNode* node);
        void defineFunction(ASTNode* node);
    public:
        Interpreter();
        void run(ASTNode* node);
};

#endif