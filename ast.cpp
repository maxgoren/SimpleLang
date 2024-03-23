#include "ast.hpp"

ASTNode* makeExprNode(ExprK type, TOKENS token, string str) {
    ASTNode* node = new ASTNode;
    node->kind = EXPRNODE;
    node->type.expr = type;
    node->data.stringVal = str;
    node->data.tokenVal = token;
    return node;
}

ASTNode* makeStmtNode(StmtK type, TOKENS token, string str) {
    ASTNode* node = new ASTNode;
    node->kind = STMTNODE;
    node->type.stmt = type;
    node->data.stringVal = str;
    node->data.tokenVal = token;
    return node;
}