#include "interpreter.hpp"

string Interpreter::eval(ASTNode* node) {
    double left = stof(expression(node->left));
    double right = stof(expression(node->right));
    switch (node->data.tokenVal) {
        case PLUS:     return to_string(left+right);
        case MINUS:    return to_string(left-right);
        case DIVIDE:   return to_string(left/right);
        case MULTIPLY: return to_string(left*right);
        case EQUAL:    return to_string(left == right);
        case LESS:     return to_string(left < right);
        default:
            cout<<"Unknown Operator: "<<node->data.stringVal<<endl;
    }
    return "-1";
}

string Interpreter::expression(ASTNode* node) {
    switch (node->type.expr) {
        case OP_EXPR:
            return eval(node);
        case ID_EXPR:
            return st.lookup(node->data.stringVal);
        case CONST_EXPR:
            return node->data.stringVal;
    }
    return to_string(0);
}

void Interpreter::printStmt(ASTNode* node) {
    cout<<expression(node->left)<<endl;
}

void Interpreter::ifStmt(ASTNode* node) {
    if (atof(expression(node->left).c_str())) {
        statement(node->right);
    }
}

void Interpreter::loopStmt(ASTNode* node) {
    while (atof(expression(node->left).c_str())) {
        ASTNode* t = node->right;
        while (t != nullptr) {
            statement(t);
            t = t->next;
        }
    }
}

void Interpreter::assignStmt(ASTNode* node) {
    string name = node->left->data.stringVal;
    string value = expression(node->right);
    st.insert(name, value);
}

void Interpreter::statement(ASTNode* node) {
    switch (node->type.stmt) {
        case PRINT_STMT:
            printStmt(node);
            break;
        case ASSIGN_STMT:
            assignStmt(node);
            break;
        case IF_STMT:
            ifStmt(node);
            break;
        case LOOP_STMT:
            loopStmt(node);
            break;
        default: 
        break;
    }
}

Interpreter::Interpreter() {

}

void Interpreter::run(ASTNode* node) {
    if (node == nullptr)
        return;
    switch(node->kind) {
        case STMTNODE:
            statement(node);
            break;
        case EXPRNODE:
            expression(node);
            break;
    }
    run(node->next);
}