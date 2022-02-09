//
// Created by Alex Latz on 2/8/22.
//

#include "../includes/Lexeme.h"

Lexeme::Lexeme(TokenType type, int lineNum) {
    this->type = type;
    this->lineNum = lineNum;
}

Lexeme::Lexeme(TokenType type, int lineNum, double value) : Lexeme(type, lineNum) {
    this->doubleVal = value;
}

Lexeme::Lexeme(TokenType type, int lineNum, string& value) : Lexeme(type, lineNum) {
    this->stringVal = value;
}

Lexeme::Lexeme(TokenType type, int lineNum, char value) : Lexeme(type, lineNum) {
    this->charVal = value;
}

Lexeme::Lexeme(TokenType type, int lineNum, bool value) : Lexeme(type, lineNum) {
    this->boolVal = value;
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

double Lexeme::getDoubleValue() {
    return this->doubleVal;
}

string Lexeme::getStringValue() {
    return this->stringVal;
}

char Lexeme::getCharValue() {
    return this->charVal;
}

bool Lexeme::getBoolValue() {
    return this->boolVal;
}


