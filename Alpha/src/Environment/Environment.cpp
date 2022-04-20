//
// Created by Alex Latz on 3/29/22.
//

#include "../../includes/Environment/Environment.h"
#include "../../includes/Alpha.h"


Environment::Environment(Environment *parent) {
    this->parent = parent;
}

Lexeme *Environment::lookup(const string& name, int lineNum) {
    if (this->symbols.count(name) > 0) return this->symbols.at(name);
    else if (this->parent != nullptr) return this->parent->lookup(name, lineNum);
    else Alpha::runtimeError(lineNum, "Environment: missing variable", "Variable " + name + " not found.");
    return nullptr;
}

bool Environment::softLookup(const string& name, int lineNum) {
    if (this->symbols.count(name) > 0) return true;
    else if (this->parent != nullptr) return this->parent->softLookup(name, lineNum);
    else return false;
}

void Environment::addSymbol(const string& name, Lexeme *symbol) {
    if (this->symbols.count(name) == 0) this->symbols.insert(pair<string, Lexeme*>(name, symbol));
    else if (symbol != nullptr) Alpha::runtimeError(*symbol, "Environment: variable already declared");

}

void Environment::modifySymbol(const string& name, Lexeme *newSymbol) {
    if (this->symbols.count(name) > 0) {
        this->symbols.erase(name);
        this->symbols.insert(pair<string, Lexeme *>(name, newSymbol));
    } else if (this->parent != nullptr) this->parent->modifySymbol(name, newSymbol);
    else Alpha::runtimeError(*newSymbol, "Environment: variable has not been declared");
}

void Environment::printSymbols() {
    for (pair<string, Lexeme*> p : this->symbols) {
        cout << p.first << ", " << *(p.second) << endl;
    }
}