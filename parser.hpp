#ifndef parser_hpp
#define parser_hpp
#include <iostream>
#include "lexer.hpp"
#include "symboltable.hpp"
#include "ast.hpp"
using std::string;



class Parser {
        vector<Lexeme> lexemes;
        int lexPos;
        Lexeme current;
        TOKENS lookahead();
        void nexttoken();
        bool match(TOKENS token);
    public:
        Parser();
        ASTNode* parse(vector<Lexeme>& tokens);
        ASTNode* program();
        ASTNode* statement();
        ASTNode* simpleExpr();
        ASTNode* expression();
        ASTNode* term();
        ASTNode* var();
};

#endif