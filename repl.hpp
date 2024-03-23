#ifndef repl_hpp
#define repl_hpp
#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
using std::string;
using std::cout;
using std::endl;


class ASTBuilder {
    private:
    
    public:
        ASTBuilder() {
            
        }
        ASTNode* build(string text) {
            Lexer lexer;
            Parser parser;
            return parser.parse(lexer.lexString(text));
        }
};


class REPL {
    private:
        ASTBuilder astBuilder;
        Interpreter interpreter;
    public:
        REPL() {

        }
        void repl() {
            string input;
            while (input != ".exit") {
                cout<<"Smolz> ";
                getline(cin, input);
                if (input != ".exit") {
                    auto ast = astBuilder.build(input);
                    ASTTracer trace;
                    trace.traverse(ast);
                    interpreter.run(ast);
                }
            }
        }
};

#endif