//
// Created by Alex Latz on 2/8/22.
//

#include "../includes/Lexer.h"

Lexer::Lexer(ifstream* file) {
    this->file = file;
}

vector<Lexeme> Lexer::lex() {
    char test;
    *file >> test;
    return vector<Lexeme>();
}

