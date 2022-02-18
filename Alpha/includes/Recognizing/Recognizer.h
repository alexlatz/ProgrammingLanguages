//
// Created by Alex Latz on 2/18/22.
//

#ifndef PROGRAMMINGLANGUAGES_RECOGNIZER_H
#define PROGRAMMINGLANGUAGES_RECOGNIZER_H

#include "../../includes/Alpha.h"

class Recognizer {
    const vector<Lexeme*> lexemes;
    vector<Lexeme*>::iterator it;
    void consume(TokenType type);
    bool check(TokenType type);
    TokenType peek();
    public:
        Recognizer(vector<Lexeme*>& lexemes);

};


#endif //PROGRAMMINGLANGUAGES_RECOGNIZER_H
