#include "interpreter.hpp"

string Interpreter::eval(ASTNode* node) {
    enter("eval");
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
    leave();
    return "-1";
}

ActivationRecord* Interpreter::prepareActivationRecord(ASTNode* node) {
    ActivationRecord* ar = new ActivationRecord();
    Procedure* func = procedures[node->data.stringVal];
    ar->function = func;
    for (auto it = func->paramList; it != nullptr; it = it->left) {
        ar->env.insert(it->data.stringVal, expression(node->left));
        say(it->data.stringVal + " added to AR.");
    }
    ar->returnValue = "0";
    return ar;
}

string Interpreter::procedureCall(ASTNode* node) {
    string retVal = "0";
    enter("[procedureCall]");
    if (procedures.find(node->data.stringVal) != procedures.end()) {
        callStack.push(prepareActivationRecord(node));
        say("Calling: " + node->data.stringVal);
        auto body = callStack.top()->function->functionBody;
        stopProcedure = false;
        run(body);
        retVal = callStack.top()->returnValue;
        say("Returned: " + retVal + " from " + node->data.stringVal);
        callStack.pop();
    } else {
        say("No such function: " + node->data.stringVal);
    }
    return retVal;
}

string Interpreter::expression(ASTNode* node) {
    string result;
    switch (node->type.expr) {
        case OP_EXPR:
            enter("[op expression]" + node->data.stringVal); leave();
            return eval(node);
        case ID_EXPR:
            enter("[id expression]");
            if (!callStack.empty() && callStack.top()->env.lookup(node->data.stringVal) != "-1") {
                result = callStack.top()->env.lookup(node->data.stringVal);
                leave("local variable: " + result);
                return result;
            }
            result = st.lookup(node->data.stringVal);
            leave("global variable: " + result);
            return result;
        case CONST_EXPR:
            enter("const expression: " + node->data.stringVal); leave();
            return node->data.stringVal;
        case FUNC_EXPR:
            enter("procedure call: " + node->data.stringVal); leave();
            return procedureCall(node);
        default:
            leave("invalid expression.");
    }
    return to_string(0);
}

void Interpreter::printStmt(ASTNode* node) {
    enter("[print]");
    cout<<expression(node->left)<<endl;
    leave();
}

void Interpreter::ifStmt(ASTNode* node) {
    enter("[if statement]");
    if (atof(expression(node->left).c_str())) {
        ASTNode* t = node->right;
        while (t != nullptr) {
            statement(t);
            t = t->next;
        }
    }
    leave();
}

void Interpreter::loopStmt(ASTNode* node) {
    enter("[loop]");
    while (atof(expression(node->left).c_str())) {
        ASTNode* t = node->right;
        while (t != nullptr) {
            statement(t);
            t = t->next;
        }
    }
}

void Interpreter::assignStmt(ASTNode* node) {
    enter("[assign]");
    string name = node->left->data.stringVal;
    string value = expression(node->right);
    st.insert(name, value);
    leave();
}

void Interpreter::defineFunction(ASTNode* node) {
    Procedure* np = new Procedure;
    np->name = node->data.stringVal;
    np->paramList = node->left;
    np->functionBody = node->right;
    procedures[np->name] = np;
    cout<<np->name<<" defined."<<endl;
}

void Interpreter::returnStmt(ASTNode* node) {
    enter("[return]");
    if (!callStack.empty()) {
        callStack.top()->returnValue = expression(node->left);
        say("Returning: " + callStack.top()->returnValue);
        stopProcedure = true;
    }
    leave();
}

void Interpreter::statement(ASTNode* node) {
    enter("[statement]");
    switch (node->type.stmt) {
        case PRINT_STMT:
            printStmt(node);
            break;
        case ASSIGN_STMT:
            assignStmt(node);
            break;
        case DEF_STMT:
            defineFunction(node);
            break;
        case IF_STMT:
            ifStmt(node);
            break;
        case LOOP_STMT:
            loopStmt(node);
            break;
        case RETURN_STMT:
            returnStmt(node);
            break;
        default: 
            cout<<"Invalid Statement: "<<node->data.stringVal<<endl;
        break;
    }
    leave();
}

Interpreter::Interpreter() {
    recDepth = 0;
    stopProcedure = false;
}

void Interpreter::run(ASTNode* node) {
    enter("[run]");
    if (node == nullptr)
        return;
    switch(node->kind) {
        case STMTNODE:
            say("-statement");
            statement(node);
            break;
        case EXPRNODE:
            say("-expression");
            expression(node);
            break;
    }
    if (stopProcedure) {
        stopProcedure = false;
        return;
    }
    say("sibling");
    run(node->next);
}

void Interpreter::enter(string s) {
    recDepth++;
    say(s);
}

void Interpreter::say(string s) {
    //for (int i = 0; i < recDepth; i++)
    //    cout<<"  ";
   // cout<<"("<<recDepth<<") "<<s<<endl;
}

void Interpreter::leave(string s) {
    say(s);
    recDepth--;
}

void Interpreter::leave() {
    --recDepth;
}