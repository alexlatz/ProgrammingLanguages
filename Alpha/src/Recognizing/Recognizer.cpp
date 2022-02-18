//
// Created by Alex Latz on 2/18/22.
//

#include "../../includes/Recognizing/Recognizer.h"

Recognizer::Recognizer(vector<Lexeme *>& lexemes) : lexemes(lexemes) {
    this->it = lexemes.begin();
}

void Recognizer::consume(TokenType type) {
    if (check(type) && it != lexemes.end()) it++;
    else Alpha::runtimeError((*it)->getLineNum(), "recognizing", "Expected " + tokenName[(*it)->getType()]);
}

bool Recognizer::check(TokenType type) {
    return (*it)->getType() == type;
}

TokenType Recognizer::peek() {
    return (*it)->getType();
}


