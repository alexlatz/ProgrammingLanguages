//
// Created by Alex Latz on 2/8/22.
//

#ifndef PROGRAMMINGLANGUAGES_LEXER_H
#define PROGRAMMINGLANGUAGES_LEXER_H

#include "Lexeme.h"
#include "../Alpha.h"
#include <fstream>
#include <map>

class Lexer {
    ifstream* file;
    int lineNum;
    Lexeme* advance(char c);
    Lexeme* getNextLexeme();
    static map<string, TokenType> keywords;
    static map<string, double> digits;
    Lexeme* lexMultiChar(char c);
    Lexeme* lexQuotes(bool str);
    Lexeme* lexNumber(string& firstWord);
    double parseHundredsGroup(vector<string>::reverse_iterator begin, vector<string>::reverse_iterator end) const;
public:
    Lexer(ifstream* file);
    vector<Lexeme*> lex();
};


#endif
