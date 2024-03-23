#ifndef lexer_hpp
#define lexer_hpp
#include "stringbuffer.hpp"
#include <vector>
#include <iostream>
using namespace std;

enum TOKENS {
    ERROR, WHITESPACE, EOFTOKEN,
    NUMBER, ID,
    PLUS, MINUS, MULTIPLY, DIVIDE,
    ASSIGN, LPAREN, RPAREN, LCURLY, RCURLY,
    LESS, EQUAL, IF, PRINT, LOOP, SEMI, COLON, PERIOD
};

struct Lexeme {
    TOKENS tokenVal;
    string stringVal;
    int lineNumber;
    Lexeme(TOKENS t = ID, string sv = "(empty)", int ln = 0) {
        tokenVal = t;
        stringVal = sv;
        lineNumber = ln;
    }
};

class Lexer {
    private:
        StringBuffer sb;
        vector<Lexeme> lexemes;
        Lexeme extractWord();
        Lexeme extractNumber();
        Lexeme checkSpecials();
    public:
        Lexer();
        vector<Lexeme>& lex(string filename);
};


#endif