#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
using namespace std;

void lexFile(string filename) {
    Lexer lexer;
    Parser parser;
    Interpreter interpreter;
    auto tokens = lexer.lex(filename);
    auto ast = parser.parse(tokens);
    interpreter.run(ast);
}

int main(int argc, char* argv[]) {
     if (argc < 2) {
        cout<<"usage: list <filename>"<<endl;
        return 0;
    }
    lexFile(argv[1]);
    return 0;
}