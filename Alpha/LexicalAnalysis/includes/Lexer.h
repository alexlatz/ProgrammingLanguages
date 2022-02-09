//
// Created by Alex Latz on 2/8/22.
//

#ifndef PROGRAMMINGLANGUAGES_LEXER_H
#define PROGRAMMINGLANGUAGES_LEXER_H

#include "Lexeme.h"
#include <vector>
#include <fstream>

class Lexer {
    ifstream* file;
public:
    Lexer(ifstream* file);
    vector<Lexeme> lex();
};


#endif
