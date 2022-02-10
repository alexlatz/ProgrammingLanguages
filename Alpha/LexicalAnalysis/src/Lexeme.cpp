//
// Created by Alex Latz on 2/8/22.
//

#include "../includes/Lexeme.h"

Lexeme::Lexeme(TokenType type, int lineNum) {
    this->type = type;
    this->lineNum = lineNum;
}

Lexeme::Lexeme(TokenType type, int lineNum, double value) : Lexeme(type, lineNum) {
    this->value = value;
}

Lexeme::Lexeme(TokenType type, int lineNum, string value) : Lexeme(type, lineNum) {
    this->value = value;
}

Lexeme::Lexeme(TokenType type, int lineNum, char value) : Lexeme(type, lineNum) {
    this->value = value;
}

Lexeme::Lexeme(TokenType type, int lineNum, bool value) : Lexeme(type, lineNum) {
    this->value = value;
}

void Lexeme::setType(TokenType type) {
    this->type = type;
}

void Lexeme::setLineNum(int lineNum) {
    this->lineNum = lineNum;
}

TokenType Lexeme::getType() {
    return this->type;
}

int Lexeme::getLineNum() {
    return this->lineNum;
}

boost::variant<double, string, char, bool> Lexeme::getValue() {
    return this->value;
}

