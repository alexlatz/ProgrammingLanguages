//
// Created by Alex Latz on 3/29/22.
//

#include "../../includes/Environment/Environment.h"
#include "../../includes/Alpha.h"


Environment::Environment(Environment *parent) {
    this->parent = parent;
}

Lexeme *Environment::lookup(string name, int lineNum) {
    if (this->symbols.count(name) > 0) return this->symbols.at(name);
    else if (this->parent != nullptr) return this->parent->lookup(name, lineNum);
    else Alpha::runtimeError(lineNum, "Environment: missing variable", "Variable " + name + " not found.");
}

bool Environment::softLookup(string name, int lineNum) {
    if (this->symbols.count(name) > 0) return true;
    else if (this->parent != nullptr) return this->parent->softLookup(name, lineNum);
    else return false;
}

void Environment::addSymbol(string name, Lexeme *symbol) {
    if (this->symbols.count(name) == 0) this->symbols.insert(pair<string, Lexeme*>(name, symbol));
    else Alpha::runtimeError(*symbol, "Environment: variable already declared");
}

void Environment::modifySymbol(string name, Lexeme *newSymbol) {
    if (this->symbols.count(name) > 0) {
        this->symbols.erase(name);
        this->symbols.insert(pair<string, Lexeme*>(name, newSymbol));
    } else Alpha::runtimeError(*newSymbol, "Environment: variable has not been declared");
}

int main(int argc, char const *argv[]) {
    Environment global(nullptr);
    Environment local(&global);
    global.addSymbol("test", new Lexeme(TokenType::NUMBER, 0, 1.0));
    global.addSymbol("localtest", new Lexeme(TokenType::NUMBER, 0, 2.0));
    cout << local.softLookup("localtest", 0) << endl;
    cout << local.softLookup("test", 0) << endl;
    cout << global << endl;
    cout << local << endl;
    cout << local.softLookup("fake", 0) << endl;
}

