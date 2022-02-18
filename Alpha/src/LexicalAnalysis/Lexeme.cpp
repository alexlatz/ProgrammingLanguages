//
// Created by Alex Latz on 2/8/22.
//

#include "../../includes/LexicalAnalysis/Lexeme.h"
#include "boost/lexical_cast.hpp"

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

string Lexeme::toString() {
    string value;
    if (this->value.type() == typeid(std::string)) value = boost::get<string>(this->value);
    else if (this->value.type() == typeid(bool)) value = (boost::get<bool>(this->value) ? "true" : "false");
    else if (this->value.type() == typeid(double)) value = ((boost::get<double>(this->value) == 0) ? "\"\"" : to_string(boost::get<double>(this->value)));
    else value = boost::lexical_cast<string>(this->value);
    return "(Type: " + tokenName.at(this->type) + ", " + to_string(this->lineNum) + ", " + value + ")";
}

