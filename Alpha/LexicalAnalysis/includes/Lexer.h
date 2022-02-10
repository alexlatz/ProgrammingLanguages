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
    int currentPos;
    int startLexeme;
    int lineNum;
    void skipWhitespace();
    static map<string, TokenType> keywords;
    static const map<string, TokenType> getKeywords();
public:
    Lexer(ifstream* file);
    vector<Lexeme> lex();
};


#endif
