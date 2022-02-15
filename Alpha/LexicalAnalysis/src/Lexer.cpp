//
// Created by Alex Latz on 2/8/22.
//

#include "../includes/Lexer.h"

Lexer::Lexer(ifstream* file) {
    this->file = file;
    this->lineNum = 1;
    getDigits();
    getKeywords();
}

vector<Lexeme*> Lexer::lex() {
    char test;
    *file >> test;
    return vector<Lexeme*>();
}

map<string, TokenType> Lexer::getKeywords() {
    if (Lexer::keywords.empty()) {
        Lexer::keywords.insert({"C", TokenType::OPEN_PAREN});
        Lexer::keywords.insert({"D", TokenType::CLOSE_PAREN});
        Lexer::keywords.insert({"L", TokenType::OPEN_BLOCK});
        Lexer::keywords.insert({"J", TokenType::CLOSE_BLOCK});
        Lexer::keywords.insert({"\n", TokenType::LINE_END});
        Lexer::keywords.insert({"II", TokenType::COMMENT_LINEEND});
        Lexer::keywords.insert({"V", TokenType::OPEN_SQ_BRACKET});
        Lexer::keywords.insert({"A", TokenType::CLOSE_SQ_BRACKET});
        Lexer::keywords.insert({"dB", TokenType::STR_OPEN});
        Lexer::keywords.insert({"Db", TokenType::STR_CLOSE});
        Lexer::keywords.insert({"bD", TokenType::CHAR_OPEN});
        Lexer::keywords.insert({"Bd", TokenType::CHAR_CLOSE});
        Lexer::keywords.insert({"rtrn", TokenType::RETURN});
        Lexer::keywords.insert({"if", TokenType::IF});
        Lexer::keywords.insert({"elif", TokenType::ELIF});
        Lexer::keywords.insert({"else", TokenType::ELSE});
        Lexer::keywords.insert({"point", TokenType::POINT});
        Lexer::keywords.insert({"let", TokenType::LET});
        Lexer::keywords.insert({"fxn", TokenType::FXN});
        Lexer::keywords.insert({"for", TokenType::FOR});
        Lexer::keywords.insert({"while", TokenType::WHILE});
        Lexer::keywords.insert({"in", TokenType::IN});
        Lexer::keywords.insert({"be", TokenType::BE});
        Lexer::keywords.insert({"add", TokenType::ADD});
        Lexer::keywords.insert({"sub", TokenType::SUB});
        Lexer::keywords.insert({"X", TokenType::X});
        Lexer::keywords.insert({"div", TokenType::DIV});
        Lexer::keywords.insert({"mod", TokenType::MOD});
        Lexer::keywords.insert({"addbe", TokenType::ADDBE});
        Lexer::keywords.insert({"subbe", TokenType::SUBBE});
        Lexer::keywords.insert({"Xbe", TokenType::XBE});
        Lexer::keywords.insert({"divbe", TokenType::DIVBE});
        Lexer::keywords.insert({"modbe", TokenType::MODBE});
        Lexer::keywords.insert({"inc", TokenType::INC});
        Lexer::keywords.insert({"dec", TokenType::DEC});
        Lexer::keywords.insert({"and", TokenType::AND});
        Lexer::keywords.insert({"or", TokenType::OR});
        Lexer::keywords.insert({"not", TokenType::NOT});
        Lexer::keywords.insert({"less", TokenType::LESS});
        Lexer::keywords.insert({"more", TokenType::MORE});
        Lexer::keywords.insert({"is", TokenType::IS});
        Lexer::keywords.insert({"lessis", TokenType::LESSIS});
        Lexer::keywords.insert({"moreis", TokenType::MOREIS});
    }
    return Lexer::keywords;
}

map<string, double> Lexer::getDigits() {
    if (Lexer::digits.empty()) {
        Lexer::digits.insert({"zero", 0});
        Lexer::digits.insert({"one", 1});
        Lexer::digits.insert({"two", 2});
        Lexer::digits.insert({"three", 3});
        Lexer::digits.insert({"four", 4});
        Lexer::digits.insert({"five", 5});
        Lexer::digits.insert({"six", 6});
        Lexer::digits.insert({"seven", 7});
        Lexer::digits.insert({"eight", 8});
        Lexer::digits.insert({"nine", 9});
        Lexer::digits.insert({"ten", 10});
        Lexer::digits.insert({"eleven", 11});
        Lexer::digits.insert({"twelve", 12});
        Lexer::digits.insert({"thirteen", 13});
        Lexer::digits.insert({"fourteen", 14});
        Lexer::digits.insert({"fifteen", 15});
        Lexer::digits.insert({"sixteen", 16});
        Lexer::digits.insert({"seventeen", 17});
        Lexer::digits.insert({"eighteen", 18});
        Lexer::digits.insert({"nineteen", 19});
        Lexer::digits.insert({"twenty", 20});
        Lexer::digits.insert({"thirty", 30});
        Lexer::digits.insert({"forty", 40});
        Lexer::digits.insert({"fifty", 50});
        Lexer::digits.insert({"sixty", 60});
        Lexer::digits.insert({"seventy", 70});
        Lexer::digits.insert({"eighty", 80});
        Lexer::digits.insert({"ninety", 90});
        Lexer::digits.insert({"hundred", 100});
        Lexer::digits.insert({"thousand", 1000});
        Lexer::digits.insert({"million", 1000000});
        Lexer::digits.insert({"billion", 1000000000});
        Lexer::digits.insert({"trillion", 1000000000000});
        Lexer::digits.insert({"quadrillion", 1000000000000000});
        Lexer::digits.insert({"quintillion", 1000000000000000000});
        Lexer::digits.insert({"sextillion", 1000000000000000000000.0});
        Lexer::digits.insert({"septillion", 1000000000000000000000000.0});
        Lexer::digits.insert({"octillion", 1000000000000000000000000000.0});
        Lexer::digits.insert({"nonillion", 1000000000000000000000000000000.0});
        Lexer::digits.insert({"decillion", 1000000000000000000000000000000000.0});
        Lexer::digits.insert({"undecillion", 1000000000000000000000000000000000000.0});
    }
    return Lexer::digits;
}

Lexeme* Lexer::getNextLexeme() {
    char c;
    *file >> c;
    switch (c) {
        case 'O':
        case '\n':
            return advance(c);
        case '\t':
        case ' ':
            return nullptr;
        case 'C':
            return new Lexeme(TokenType::OPEN_PAREN, lineNum);
        case 'D':
            return new Lexeme(TokenType::CLOSE_PAREN, lineNum);
        case 'L':
            return new Lexeme(TokenType::OPEN_BLOCK, lineNum);
        case 'J':
            return new Lexeme(TokenType::CLOSE_BLOCK, lineNum);
        case 'V':
            return new Lexeme(TokenType::OPEN_SQ_BRACKET, lineNum);
        case 'A':
            return new Lexeme(TokenType::CLOSE_SQ_BRACKET, lineNum);
        case 'X':
            return new Lexeme(TokenType::X, lineNum);
    }
    return lexMultiChar(c);
}

Lexeme* Lexer::lexMultiChar(char c) {
    string word;
    while (isalpha(c)) {
        word.push_back(c);
        *file >> c;
    }
    if (Lexer::keywords.count(word) > 0) {
        TokenType type = Lexer::keywords.at(word);
        if ((type == TokenType::STR_OPEN || type == TokenType::CHAR_OPEN) && c == ' ') return lexQuotes();
        file->putback(c);
        return new Lexeme(type, lineNum);
    } else {
        if (Lexer::digits.count(word) > 0) return lexNumber();
        else if (word == "true" || word == "false") return new Lexeme(TokenType::BOOL, lineNum, word == "true");
        else return new Lexeme(TokenType::IDENTIFIER, lineNum, word);
    }
}

Lexeme* Lexer::advance(char c) {
    if (c == 'O') return new Lexeme(TokenType::LINE_END, lineNum);
    else if (c == '\n') {
        char peek;
        *file >> peek;
        lineNum++;
        if (peek == '\t') {
            return nullptr;
        } else {
            file->putback(peek);
            return new Lexeme(TokenType::LINE_END, lineNum);
        }
    }
    return nullptr;
}




