//
// Created by Alex Latz on 2/8/22.
//

#include "../includes/Lexer.h"

Lexer::Lexer(ifstream* file) {
    this->file = file;
    this->lineNum = 1;
}

vector<Lexeme> Lexer::lex() {
    char test;
    *file >> test;
    return vector<Lexeme>();
}

void Lexer::skipWhitespace() {

}

const map<string, TokenType> Lexer::getKeywords() {
    if (Lexer::keywords.empty()) {
        Lexer::keywords.insert(pair<string, TokenType>("C", TokenType::OPEN_PAREN));
        Lexer::keywords.insert(pair<string, TokenType>("D", TokenType::CLOSE_PAREN));
        Lexer::keywords.insert(pair<string, TokenType>("L", TokenType::OPEN_BLOCK));
        Lexer::keywords.insert(pair<string, TokenType>("J", TokenType::CLOSE_BLOCK));
        Lexer::keywords.insert(pair<string, TokenType>("\n", TokenType::LINE_END));
        Lexer::keywords.insert(pair<string, TokenType>("II", TokenType::COMMENT_LINEEND));
        Lexer::keywords.insert(pair<string, TokenType>("V", TokenType::OPEN_SQ_BRACKET));
        Lexer::keywords.insert(pair<string, TokenType>("A", TokenType::CLOSE_SQ_BRACKET));
        Lexer::keywords.insert(pair<string, TokenType>("dB ", TokenType::STR_OPEN));
        Lexer::keywords.insert(pair<string, TokenType>(" Db", TokenType::STR_CLOSE));
        Lexer::keywords.insert(pair<string, TokenType>("rtrn", TokenType::RETURN));
        Lexer::keywords.insert(pair<string, TokenType>("if", TokenType::IF));
        Lexer::keywords.insert(pair<string, TokenType>("elif", TokenType::ELIF));
        Lexer::keywords.insert(pair<string, TokenType>("else", TokenType::ELSE));
        Lexer::keywords.insert(pair<string, TokenType>("point", TokenType::POINT));
        Lexer::keywords.insert(pair<string, TokenType>("let", TokenType::LET));
        Lexer::keywords.insert(pair<string, TokenType>("fxn", TokenType::FXN));
        Lexer::keywords.insert(pair<string, TokenType>("for", TokenType::FOR));
        Lexer::keywords.insert(pair<string, TokenType>("while", TokenType::WHILE));
        Lexer::keywords.insert(pair<string, TokenType>("in", TokenType::IN));
        Lexer::keywords.insert(pair<string, TokenType>("be", TokenType::BE));
        Lexer::keywords.insert(pair<string, TokenType>("add", TokenType::ADD));
        Lexer::keywords.insert(pair<string, TokenType>("sub", TokenType::SUB));
        Lexer::keywords.insert(pair<string, TokenType>("X", TokenType::X));
        Lexer::keywords.insert(pair<string, TokenType>("div", TokenType::DIV));
        Lexer::keywords.insert(pair<string, TokenType>("mod", TokenType::MOD));
        Lexer::keywords.insert(pair<string, TokenType>("addbe", TokenType::ADDBE));
        Lexer::keywords.insert(pair<string, TokenType>("subbe", TokenType::SUBBE));
        Lexer::keywords.insert(pair<string, TokenType>("Xbe", TokenType::XBE));
        Lexer::keywords.insert(pair<string, TokenType>("divbe", TokenType::DIVBE));
        Lexer::keywords.insert(pair<string, TokenType>("modbe", TokenType::MODBE));
        Lexer::keywords.insert(pair<string, TokenType>("inc", TokenType::INC));
        Lexer::keywords.insert(pair<string, TokenType>("dec", TokenType::DEC));
        Lexer::keywords.insert(pair<string, TokenType>("and", TokenType::AND));
        Lexer::keywords.insert(pair<string, TokenType>("or", TokenType::OR));
        Lexer::keywords.insert(pair<string, TokenType>("not", TokenType::NOT));
        Lexer::keywords.insert(pair<string, TokenType>("less", TokenType::LESS));
        Lexer::keywords.insert(pair<string, TokenType>("more", TokenType::MORE));
        Lexer::keywords.insert(pair<string, TokenType>("is", TokenType::IS));
        Lexer::keywords.insert(pair<string, TokenType>("lessis", TokenType::LESSIS));
        Lexer::keywords.insert(pair<string, TokenType>("moreis", TokenType::MOREIS));
    } else return Lexer::keywords;
}

