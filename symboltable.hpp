#ifndef symboltable_hpp
#define symboltable_hpp
#include <iostream>
using std::string;

class SymbolTable {
    private:
        struct node {
            string key;
            string value;
            node* next;
            node(string k, string v, node* n) {
                key = k; value = v; next = n;
            }
        };
        typedef node* link;
        link head;
        int n;
        link addToTable(link h, link m);
    public:
        SymbolTable();
        SymbolTable(const SymbolTable& st);
        ~SymbolTable();
        void insert(string key, string value);
        string lookup(string key);
        SymbolTable& operator=(const SymbolTable& st);
};

#endif