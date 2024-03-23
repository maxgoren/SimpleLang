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

ASTTracer::ASTTracer() {
    depth = 0;
}

void ASTTracer::traverse(ASTNode* node) {
    depth++;
    if (node != nullptr) {
        printNode(node);
        traverse(node->left);
        traverse(node->right);
        if (node->next != nullptr)
            traverse(node->next);
    }
    depth--;
}

void ASTTracer::printNode(ASTNode* node) {
    if (node == nullptr) return;
    for (int i = 0; i < depth; i++)
        cout<<"  ";
    if (node->kind == EXPRNODE) {
        switch (node->type.expr) {
            case ID_EXPR:    cout<<"[ID_EXPR]"<<endl; break;
            case CONST_EXPR: cout<<"[CONST_EXPR]"<<endl; break;
            case OP_EXPR:    cout<<"[OP_EXPR]"<<endl; break;
            default:
                break;
        }
    } else if (node->kind == STMTNODE) {
        switch (node->type.stmt) {
            case IF_STMT:     cout<<"[IF_STMT]"<<endl; break;
            case LOOP_STMT:   cout<<"[LOOP_STMT]"<<endl; break;
            case PRINT_STMT:  cout<<"[PRINT_STMT]"<<endl; break;
            case ASSIGN_STMT: cout<<"[ASSIGN_STMT]"<<endl; break;
            default:
                break;
        }
    }
}
