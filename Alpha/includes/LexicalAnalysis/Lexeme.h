//
// Created by Alex Latz on 2/8/22.
// The Lexeme header stores all classes related to Lexemes.
//

#ifndef PROGRAMMINGLANGUAGES_LEXEME_H
#define PROGRAMMINGLANGUAGES_LEXEME_H

#include <iostream>
#include <vector>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

using namespace std;
static vector<string> tokenName = {"NUMBER", "BOOL", "STRING", "CHAR", "COLLECTION", "IDENTIFIER",
                                   "OPEN_PAREN", "CLOSE_PAREN", "OPEN_BLOCK", "CLOSE_BLOCK",
                                   "LINE_END", "COMMENT_LINEEND", "OPEN_SQ_BRACKET", "CLOSE_SQ_BRACKET",
                                   "STR_OPEN", "STR_CLOSE", "CHAR_OPEN", "CHAR_CLOSE",
                                   "RETURN", "IF", "ELIF", "ELSE", "POINT", "LET", "FXN", "FOR", "WHILE", "IN",
                                   "BE", "ADD", "SUB", "X", "DIV",
                                   "MOD", "ADDBE", "SUBBE", "XBE", "DIVBE", "MODBE",
                                   "INC", "DEC",
                                   "AND", "OR", "NOT", "LESS", "MORE", "IS", "LESSIS", "MOREIS",
                                   "ENDFILE"};
enum TokenType {
    //variable types
    NUMBER, BOOL, STRING, CHAR, COLLECTION, IDENTIFIER,
    //symbols
    OPEN_PAREN, CLOSE_PAREN, OPEN_BLOCK, CLOSE_BLOCK,
    LINE_END, COMMENT_LINEEND, OPEN_SQ_BRACKET, CLOSE_SQ_BRACKET,
    STR_OPEN, STR_CLOSE, CHAR_OPEN, CHAR_CLOSE,
    //keywords
    RETURN, IF, ELIF, ELSE, POINT, LET, FXN, FOR, WHILE, IN,
    //operators
    //binary
    BE, ADD, SUB, X, DIV,
    MOD, ADDBE, SUBBE, XBE, DIVBE, MODBE,
    //unary
    INC, DEC,
    //boolean operators
    AND, OR, NOT, LESS, MORE, IS, LESSIS, MOREIS,
    //file end (can't name it EOF bc that's a defined constant in C++
    ENDFILE,
    STATEMENTLIST, FXN_CALL, STATEMENT
};

class Lexeme {
    TokenType type;
    int lineNum;
    boost::variant<double, string, char, bool> value;
    vector<Lexeme*> children;
    friend ostream& operator<<(ostream& stream, const Lexeme& lexeme) {
        stream << "(" << "Type: " << tokenName.at(lexeme.type) << ", " << lexeme.lineNum << ", ";
        if (lexeme.value.type() == typeid(std::string)) stream << boost::get<string>(lexeme.value);
        else if (lexeme.value.type() == typeid(bool)) stream << (boost::get<bool>(lexeme.value) ? "true" : "false");
        else if (lexeme.value.type() == typeid(double)) stream << ((boost::get<double>(lexeme.value) == 0) ? "\"\"" : to_string(boost::get<double>(lexeme.value)));
        else stream << lexeme.value;
        return stream << ")\n";
    };
public:
    Lexeme(TokenType type, int lineNum, double value);
    Lexeme(TokenType type, int lineNum, string value);
    Lexeme(TokenType type, int lineNum, char value);
    Lexeme(TokenType type, int lineNum, bool value);
    Lexeme(TokenType type, int lineNum);
    string toString();
    void setType(TokenType type);
    void setLineNum(int lineNum);
    TokenType getType();
    int getLineNum();
    int getChildrenLength();
    void setChild(Lexeme* child, int index);
    void setChild(Lexeme* child);
    Lexeme* getChild(int index);
    boost::variant<double, string, char, bool> getValue();
};

//needed to redefine the operator (can't do it inside a class, but I want access to private variables)
ostream& operator<<(ostream& stream, const Lexeme& lexeme);


#endif //PROGRAMMINGLANGUAGES_LEXEME_H
