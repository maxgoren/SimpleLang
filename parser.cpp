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
        cout<<"mismatched token on line "<<current.lineNumber<<": "<<tokenString[current.tokenVal]<<",expected: "<<tokenString[token]<<endl;
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
    while (lookahead() != RCURLY && lookahead() != EOFTOKEN) {
        ASTNode* t = statement();
        if (m == nullptr) {
            node = m = t;
            node->next = m;
        } else {
            m->next = t;
            m = t;
        }
    }
    return node;
}

ASTNode* Parser::paramList() {
    ASTNode* node;
    if (lookahead() == ID) {
        node = makeExprNode(ID_EXPR, ID, current.stringVal);
        match(ID);
    }
    return node;
}

ASTNode* Parser::statement() {
    ASTNode* node;
    if (lookahead() == PRINT) {
        match(PRINT);
        node = makeStmtNode(PRINT_STMT, lookahead(), current.stringVal);
        node->left = expression();
        if (lookahead() == SEMI)
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
        node->right = program();
        if (lookahead() == RCURLY)
            match(RCURLY);
        else cout<<"Hey, who ate my closing brace?"<<endl;
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
    if (lookahead() == DEF) {
        node = makeStmtNode(DEF_STMT, lookahead(), current.stringVal);
        match(DEF);
        node->data.stringVal = current.stringVal;
        match(ID);
        match(LPAREN);
        node->left = paramList();
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
            if (lookahead() == SEMI)
                match(SEMI);
            return node;
        }
        if (lookahead() == LPAREN) {
            node->type.expr = FUNC_EXPR;
            node->left = simpleExpr();
            if (lookahead() == SEMI)
                match(SEMI);
            return node;
        }
    }
    if (lookahead() == LPAREN) {
        node = simpleExpr();
        if (lookahead() == SEMI)
            match(SEMI);
        return node;
    }
    if (lookahead() == RETURN) {
        node = makeStmtNode(RETURN_STMT, lookahead(), current.stringVal);
        match(RETURN);
        node->left = simpleExpr();
        if (lookahead() == SEMI)
            match(SEMI);
        return node;
    }
    cout<<"Unknown Token: "<<current.stringVal<<endl;
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
        if (lookahead() == LPAREN) {
            match(LPAREN);
            node->type.expr = FUNC_EXPR;
            if (lookahead() != RPAREN)
                node->left = simpleExpr();
            match(RPAREN);
        }
        return node;
    }
    if (lookahead() == LPAREN) {
        match(LPAREN);
        node = simpleExpr();
        match(RPAREN);
    }
    return node;
}
