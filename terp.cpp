#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "repl.hpp"
using namespace std;

void interpretFile(string filename) {
    Lexer lexer;
    Parser parser;
    Interpreter interpreter;
    auto tokens = lexer.lex(filename);
    for (auto m : tokens) {
        cout<<m.lineNumber<<": "<<m.stringVal<<endl;
    }
    auto ast = parser.parse(tokens);
    ASTTracer tracer;
    tracer.traverse(ast);
    interpreter.run(ast);
}

void repl() {
    REPL pler;
    pler.repl();
}

int main(int argc, char* argv[]) {
     if (argc < 2) {
        cout<<"usage: "<<endl;
        cout<<"smolz <filename>"<<endl;
        cout<<"smolz -r (repl)"<<endl;
        return 0;
    }
    if (argv[1][0] == '-' && argv[1][1] == 'r')
         repl();
    else interpretFile(argv[1]);
    return 0;
}