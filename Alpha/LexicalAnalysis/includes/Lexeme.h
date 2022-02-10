//
// Created by Alex Latz on 2/8/22.
// The Lexeme header stores all classes related to Lexemes.
//

#ifndef PROGRAMMINGLANGUAGES_LEXEME_H
#define PROGRAMMINGLANGUAGES_LEXEME_H

#include <iostream>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <boost/describe.hpp>

using namespace std;
enum TokenType {
    //variable types
    NUMBER, BOOL, STRING, CHAR, COLLECTION, IDENTIFIER,
    //symbols
    OPEN_PAREN, CLOSE_PAREN, OPEN_BLOCK, CLOSE_BLOCK,
    LINE_END, COMMENT_LINEEND, OPEN_SQ_BRACKET, CLOSE_SQ_BRACKET,
    STR_OPEN, STR_CLOSE,
    //keywords
    RETURN, IF, ELIF, ELSE, POINT, LET, FXN, FOR, WHILE, IN,
    //operators
    //binary
    BE, ADD, SUB, X, DIV,
    MOD, ADDBE, SUBBE, XBE, DIVBE, MODBE,
    //unary
    INC, DEC,
    //boolean operators
    AND, OR, NOT, LESS, MORE, IS, LESSIS, MOREIS
};

class Lexeme {
    TokenType type;
    int lineNum;
    boost::variant<double, string, char, bool> value;
    //C++ doesn't really have a toString alternative, so I overloaded the << operator to add something similar
    //Also there's no real way to do enum toString, so I'm leaving it like this for now
    friend ostream& operator<<(ostream& stream, const Lexeme& lexeme) {
        stream << "(" << "Type #:" << lexeme.type << ", " << lexeme.lineNum << ", ";
        if (lexeme.value.type() == typeid(std::string)) stream << boost::get<string>(lexeme.value);
        else if (lexeme.value.type() == typeid(bool)) stream << (boost::get<bool>(lexeme.value) ? "true" : "false");
        else stream << lexeme.value;
        return stream << ")\n";
    };
public:
    Lexeme(TokenType type, int lineNum, double value);
    Lexeme(TokenType type, int lineNum, string value);
    Lexeme(TokenType type, int lineNum, char value);
    Lexeme(TokenType type, int lineNum, bool value);
    Lexeme(TokenType type, int lineNum);
    void setType(TokenType type);
    void setLineNum(int lineNum);
    TokenType getType();
    int getLineNum();
    boost::variant<double, string, char, bool> getValue();
};

//needed to redefine the operator (can't do it inside a class, but I want access to private variables)
ostream& operator<<(ostream& stream, const Lexeme& lexeme);


#endif //PROGRAMMINGLANGUAGES_LEXEME_H
