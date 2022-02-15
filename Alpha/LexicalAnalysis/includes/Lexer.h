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
    Lexeme* advance(char c);
    Lexeme* getNextLexeme();
    static map<string, TokenType> keywords;
    static map<string, double> digits;
    static map<string, double> getDigits();
    static map<string, TokenType> getKeywords();
    Lexeme* lexMultiChar(char c);
    Lexeme* lexQuotes();
    Lexeme* lexNumber();
public:
    Lexer(ifstream* file);
    vector<Lexeme*> lex();
};


#endif
