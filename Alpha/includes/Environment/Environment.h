//
// Created by Alex Latz on 3/29/22.
//

#ifndef PROGRAMMINGLANGUAGES_ENVIRONMENT_H
#define PROGRAMMINGLANGUAGES_ENVIRONMENT_H

#include "../LexicalAnalysis/Lexeme.h"
#include <map>

class Environment {
    Environment* parent;
    map<string, Lexeme*> symbols;
    friend ostream& operator<<(ostream& stream, const Environment& environment) {
        stream << "Environment ID: " << addressof(environment) << "\n";
        if (environment.parent != nullptr) stream << "Parent ID: " << addressof(environment.parent) << "\n";
        stream << "Name/Value Pairs: \n";
        for (pair<string, Lexeme*> p : environment.symbols) {
            stream << "(" << p.first << ", " << p.second << ")\n";
        }
        return stream;
    }
    public:
        Environment(Environment* parent);
        Lexeme* lookup(const string& name, int lineNum);
        bool softLookup(const string& name, int lineNum);
        void addSymbol(const string& name, Lexeme* symbol);
        void modifySymbol(const string& name, Lexeme* newSymbol);
};

ostream& operator<<(ostream& stream, const Lexeme& lexeme);

#endif //PROGRAMMINGLANGUAGES_ENVIRONMENT_H
