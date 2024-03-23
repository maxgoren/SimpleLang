#include "parser.hpp"

TOKENS Parser::lookahead() {
    return current.tokenVal;
}

void Parser::nexttoken() {
    if (lexPos+1 == lexemes.size()) {
        current = Lexeme(EOFTOKEN, "<fin>", lexPos);
    } else {
        current = lexemes[++lexPos];
    }
}

bool Parser::match(TOKENS token) {
    if (token == current.tokenVal) {
        nexttoken();
        return true;
    }
    if (current.tokenVal != EOFTOKEN) {
        cout<<"mismatched token on line "<<current.lineNumber<<": "<<current.stringVal<<endl;
    }
    return false;
}

Parser::Parser() {

}

ASTNode* Parser::parse(vector<Lexeme>& tokens) {
    lexemes = tokens;
    current = tokens[0];
    lexPos = 0;
    return program();
}

ASTNode* Parser::program() {
    ASTNode* node = statement();
    ASTNode* m = node;
    while (lookahead() != EOFTOKEN) {
        ASTNode* t = statement();
        if (lookahead() == RCURLY)
            match(RCURLY);
        if (m == nullptr) {
            node = m = t;
            node->next = m;
        } else {
            m->next = t;
            m = t;
        }
    }
    m->next = nullptr;
    return node;
}

ASTNode* Parser::statement() {
    ASTNode* node;
    if (lookahead() == PRINT) {
        match(PRINT);
        node = makeStmtNode(PRINT_STMT, lookahead(), current.stringVal);
        node->left = simpleExpr();
        match(SEMI);
        return node;
    }
    if  (lookahead() == IF) {
        node = makeStmtNode(IF_STMT, lookahead(), current.stringVal);
        match(IF);
        match(LPAREN);
        node->left = simpleExpr();
        match(RPAREN);
        match(LCURLY);
        node->right = statement();
        match(RCURLY);
        return node;
    }
    if (lookahead() == LOOP) {
        node = makeStmtNode(LOOP_STMT, lookahead(), current.stringVal);
        match (LOOP);
        match(LPAREN);
        node->left = simpleExpr();
        match(RPAREN);
        match(LCURLY);
        node->right = program();
        match(RCURLY);
        return node;
    }
    if (lookahead() == ID) {
        node = makeExprNode(ID_EXPR, lookahead(), current.stringVal);
        match(ID);
        if (lookahead() == ASSIGN) {
            ASTNode* t = makeStmtNode(ASSIGN_STMT, lookahead(), current.stringVal);
            t->left = node;
            node = t;
            match(ASSIGN);
            node->right = simpleExpr();
            match(SEMI);
        }
        return node;
    } else if (lookahead() == LPAREN) {
        node = simpleExpr();
        match(SEMI);
    } else {
        cout<<"Unknown Token: "<<current.stringVal<<endl;
    }
    return node;
}

ASTNode* Parser::simpleExpr() {
    ASTNode* node = expression();
    if (lookahead() == EQUAL || lookahead() == LESS) {
        ASTNode* t = makeExprNode(OP_EXPR, lookahead(), current.stringVal);
        t->left = node;
        match(lookahead());
        node = t;
        node->right = expression();
    }
    return node;
}

ASTNode* Parser::expression() {
    ASTNode* node = term();
    while (lookahead() == PLUS || lookahead() == MINUS) {
        ASTNode* expNode = makeExprNode(OP_EXPR, lookahead(), current.stringVal);
        expNode->left = node;
        node = expNode;
        match(lookahead());
        node->right = term();
    }
    return node;
}

ASTNode* Parser::term() {
    ASTNode* node = var();
    while (lookahead() == MULTIPLY || lookahead() == DIVIDE) {
        ASTNode* expNode = makeExprNode(OP_EXPR, lookahead(), current.stringVal);
        expNode->left = node;
        node = expNode;
        match(lookahead());
        node->right = var();
    }
    return node;
}

ASTNode* Parser::var() {
    ASTNode* node;
    if (lookahead() == NUMBER) {
        node = makeExprNode(CONST_EXPR, lookahead(), current.stringVal);
        match(NUMBER);
        return node;
    }
    if (lookahead() == ID) {
        node = makeExprNode(ID_EXPR, lookahead(), current.stringVal);
        match(ID);
        return node;
    }
    if (lookahead() == LPAREN) {
        match(LPAREN);
        node = simpleExpr();
        match(RPAREN);
    }
    return node;
}
