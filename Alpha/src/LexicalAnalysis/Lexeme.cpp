//
// Created by Alex Latz on 2/8/22.
//

#include "../../includes/LexicalAnalysis/Lexeme.h"
#include "../../includes/Alpha.h"
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

int Lexeme::getChildrenLength() {
    return this->children.size();
}

void Lexeme::setChild(Lexeme* child, int index) {
    if (index >= this->children.size()) this->children.resize(index + 1);
    this->children.insert(this->children.begin() + index, child);
}

void Lexeme::setChild(Lexeme* child) {
    this->children.push_back(child);
}

Lexeme *Lexeme::getChild(int index) {
    if (index < this->children.size() && index >= 0) return this->children[index];
    else Alpha::runtimeError(0, "Accessing child", "Child not found at index " + to_string(index));
    return nullptr;
}

ostream& Lexeme::printLexemeValue(ostream& stream, Lexeme& lexeme) {
    stream << "(" << "Type: " << tokenName.at(lexeme.type) << ", " << lexeme.lineNum << ", ";
    if (lexeme.getType() == TokenType::STRING) stream << boost::get<string>(lexeme.value);
    else if (lexeme.getType() == TokenType::CHAR) stream << (boost::get<char>(lexeme.value));
    else if (lexeme.getType() == TokenType::BOOL) stream << (boost::get<bool>(lexeme.value) ? "true" : "false");
    else if (lexeme.getType() == TokenType::NUMBER) stream << ((boost::get<double>(lexeme.value) == 0) ? "0" : to_string(boost::get<double>(lexeme.value)));
    else stream << "\"\"";
    stream << ")";
    return stream;
}
ostream& Lexeme::printLexeme(ostream& stream, Lexeme& lex, int i) {
    if (!lex.children.empty()) {
        string s(i, ' ');
        stream << s;
        printLexemeValue(stream, lex) << " has children:\n" << s << "  ";
        for (Lexeme* child : lex.children) {
            printLexemeValue(stream, *child) << ", ";
        }
        stream << endl;
        for (Lexeme* child : lex.children) {
            printLexeme(stream, *child, i+2);
        }
    }
    return stream;
}
